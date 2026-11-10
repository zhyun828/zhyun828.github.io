/*
 * mode.c - 控制模式管理模块实现
 * 实现系统状态机、PID控制器和用户界面
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#include "mode.h"
#include "gpio.h"
#include "pwm.h"
#include "eqep.h"
#include "lis331hh.h"

/* ===== 控制参数定义 ===== */
#define DT_SEC              0.02f      /* 控制时间间隔:20ms */
#define PWM_PERIOD_NS       500000     /* PWM周期500us,频率2kHz */
#define PWM_DUTY_MIN_NS     0
#define PWM_DUTY_MAX_NS     500000

/* PID控制器参数 */
#define KP                  8000.0f    /* 比例系数 */
#define KI                  500.0f     /* 积分系数 */
#define KD                  1000.0f    /* 微分系数 */
#define INTEGRAL_MAX        50.0f      /* 积分项上限(抗饱和) */
#define INTEGRAL_MIN       -50.0f      /* 积分项下限(抗饱和) */

/* 位置单位转换 */
#define COUNT_TO_METER      0.00005f   /* 编码器计数转米:每计数0.00005m */
#define POS_EPSILON         0.001f     /* 位置误差死区:±1mm */

/* 归零操作参数 */
#define HOMING_PWM          0.18f      /* 归零时的PWM占空比(18%) */
#define HOMING_TIMEOUT_SEC  8.0f       /* 归零超时时间(秒) */

/* IMU(加速度计)全局上下文 */
typedef struct {
    lis331hh_t dev;      /* 加速度计设备 */
    int ready;           /* 初始化标志 */
} imu_context_t;

static imu_context_t g_imu = {.ready = 0};

/* ===== 工具函数 ===== */

/*
 * clampf - 浮点数范围限制函数
 * @x: 输入值
 * @min_v: 最小值
 * @max_v: 最大值
 * 返回值: 限制后的值
 */
static float clampf(float x, float min_v, float max_v)
{
    if (x < min_v) return min_v;
    if (x > max_v) return max_v;
    return x;
}

/* ===== 电机控制函数 ===== */

/*
 * motor_stop - 停止电机
 * 停止PWM输出，电机切断电源
 */
static void motor_stop(void)
{
    set_pwm(EHRPWM1A, PWM_DUTY, 0);
    set_pwm(EHRPWM1A, PWM_RUN, 0);
}

/*
 * motor_apply - 向电机施加控制力
 * @u: 控制输入(-1.0到+1.0), 负值表示向左，正值表示向右
 *
 * 计算占空比并设置GPIO方向和PWM信号
 */
static void motor_apply(float u)
{
    int dir = (u >= 0.0f) ? 1 : 0;                                /* 确定方向 */
    float abs_u = clampf(fabsf(u), 0.0f, 1.0f);                    /* 限制幅值 */
    unsigned int duty_ns = (unsigned int)(abs_u * PWM_PERIOD_NS);  /* 转换为纳秒 */

    /* 检查占空比范围 */
    if (duty_ns > PWM_DUTY_MAX_NS) duty_ns = PWM_DUTY_MAX_NS;
    if (duty_ns < PWM_DUTY_MIN_NS) duty_ns = PWM_DUTY_MIN_NS;

    /* 设置方向和PWM信号 */
    set_gpio(GPIO0_31, VALUE, dir);
    set_pwm(EHRPWM1A, PWM_DUTY, duty_ns);
    set_pwm(EHRPWM1A, PWM_RUN, 1);
}

/* ===== 传感器读取函数 ===== */

/*
 * limits_read_left - 读取左极限开关状态
 */
static int limits_read_left(void)
{
    return get_gpio(GPIO1_28, VALUE);
}

/*
 * limits_read_right - 读取右极限开关状态
 */
static int limits_read_right(void)
{
    return get_gpio(GPIO1_18, VALUE);
}

/*
 * read_position_meter - 读取当前位置(米)
 * 从编码器读取计数值并转换为米
 */
static float read_position_meter(void)
{
    int pos_count = get_eqep(EQEP1, POSITION);
    return (float)pos_count * COUNT_TO_METER;
}

/*
 * get_time_sec - 获取当前系统时间(秒)
 * 返回值: 浮点秒数，精度到微秒
 */
static float get_time_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (float)tv.tv_sec + (float)tv.tv_usec * 1e-6f;
}

/* ===== 线程安全的状态更新函数 ===== */

/*
 * update_limits - 更新极限开关状态到共享结构体
 * 读取GPIO并在线程安全的保护下更新状态
 */
static void update_limits(struct status *slide_status)
{
    pthread_mutex_lock(&slide_status->access);
    slide_status->lim_left = limits_read_left();
    slide_status->lim_right = limits_read_right();
    pthread_mutex_unlock(&slide_status->access);
}

/*
 * limit_hit_left - 安全读取左极限开关状态
 */
static int limit_hit_left(struct status *slide_status)
{
    int v;
    pthread_mutex_lock(&slide_status->access);
    v = slide_status->lim_left;
    pthread_mutex_unlock(&slide_status->access);
    return v;
}

/*
 * limit_hit_right - 安全读取右极限开关状态
 */
static int limit_hit_right(struct status *slide_status)
{
    int v;
    pthread_mutex_lock(&slide_status->access);
    v = slide_status->lim_right;
    pthread_mutex_unlock(&slide_status->access);
    return v;
}

/* ===== IMU(加速度计)管理函数 ===== */

/*
 * imu_init_once - 初始化加速度计(仅一次)
 * 返回值: 成功返回0，失败返回-1
 */
static int imu_init_once(void)
{
    if (g_imu.ready) return 0;
    if (lis331hh_open(&g_imu.dev, LIS331HH_I2C_DEV, LIS331HH_I2C_ADDR) != 0) return -1;
    if (lis331hh_init(&g_imu.dev, LIS331HH_RANGE_6G) != 0) {
        lis331hh_close(&g_imu.dev);
        return -1;
    }
    g_imu.ready = 1;
    return 0;
}

/*
 * imu_update_status - 读取加速度计数据并更新共享状态
 * 线程安全地更新加速度值
 */
static void imu_update_status(struct status *slide_status)
{
    float ax, ay, az;
    if (!g_imu.ready) {
        if (imu_init_once() != 0) return;
    }
    if (lis331hh_read_xyz_g(&g_imu.dev, &ax, &ay, &az) == 0) {
        pthread_mutex_lock(&slide_status->access);
        slide_status->acc_x = ax;
        slide_status->acc_y = ay;
        slide_status->acc_z = az;
        pthread_mutex_unlock(&slide_status->access);
    }
}

/* ===== 系统初始化序列 ===== */

/*
 * homing_sequence - 归零操作序列
 * @slide_status: 系统状态指针
 * 返回值: 成功返回0，超时返回-1
 *
 * 向左驱动直到触发左端极限开关，将该位置设为原点(0)
 */
static int homing_sequence(struct status *slide_status)
{
    float t0 = get_time_sec();
    printf("[HOME] moving toward left limit...\n");

    while ((get_time_sec() - t0) < HOMING_TIMEOUT_SEC) {
        update_limits(slide_status);
        if (limit_hit_left(slide_status) == 1) {
            motor_stop();
            set_eqep(EQEP1, POSITION, 0);  /* 重置编码器为0 */
            pthread_mutex_lock(&slide_status->access);
            slide_status->act_pos = 0.0f;
            slide_status->act_vel = 0.0f;
            slide_status->homed = 1;
            pthread_mutex_unlock(&slide_status->access);
            printf("[HOME] left limit reached, origin set.\n");
            return 0;
        }

        motor_apply(-HOMING_PWM);
        imu_update_status(slide_status);
        usleep((useconds_t)(DT_SEC * 1e6f));
    }

    motor_stop();
    fprintf(stderr, "[HOME] timeout.\n");
    return -1;
}

/* ===== 模式处理函数 ===== */

/*
 * mode_stop - 停止模式处理
 * 关闭电机和所有外设，释放资源
 */
void mode_stop(struct status *slide_status)
{
    motor_stop();
    set_eqep(EQEP1, ENABLED, 0);

    pthread_mutex_lock(&slide_status->access);
    slide_status->des_vel = 0.0f;
    pthread_mutex_unlock(&slide_status->access);

    if (g_imu.ready) {
        lis331hh_close(&g_imu.dev);
        g_imu.ready = 0;
    }

    printf("[MODE] STOP\n");
}

/*
 * mode_start - 启动模式处理
 * 初始化所有外设，执行归零操作，转移到RUN模式
 */
void mode_start(struct status *slide_status)
{
    printf("[MODE] START\n");

    /* 配置GPIO方向 */
    set_gpio(GPIO0_31, DIRECTION, 0);  /* 电机方向脚输出 */
    set_gpio(GPIO0_31, VALUE, 0);
    set_gpio(GPIO1_28, DIRECTION, 1);  /* 左极限开关脚输入 */
    set_gpio(GPIO1_18, DIRECTION, 1);  /* 右极限开关脚输入 */

    /* 配置PWM */
    set_pwm(EHRPWM1A, PWM_PERIOD, PWM_PERIOD_NS);
    set_pwm(EHRPWM1A, PWM_DUTY, 0);
    set_pwm(EHRPWM1A, PWM_POLARITY, 0);
    set_pwm(EHRPWM1A, PWM_RUN, 1);

    /* 配置EQEP编码器 */
    set_eqep(EQEP1, MODE, 0);
    set_eqep(EQEP1, POSITION, 0);
    set_eqep(EQEP1, ENABLED, 1);

    /* 初始化加速度计 */
    imu_init_once();

    /* 执行归零操作 */
    if (homing_sequence(slide_status) != 0) {
        pthread_mutex_lock(&slide_status->access);
        slide_status->mode = IDLE;
        pthread_mutex_unlock(&slide_status->access);
        return;
    }

    /* 转移到运行模式 */
    pthread_mutex_lock(&slide_status->access);
    slide_status->mode = RUN;
    pthread_mutex_unlock(&slide_status->access);

    printf("[MODE] START done -> RUN\n");
}

/*
 * mode_idle - 待机模式处理
 * 停止电机但保持设备初始化状态，可快速进入RUN模式
 */
void mode_idle(struct status *slide_status)
{
    motor_stop();
    set_eqep(EQEP1, ENABLED, 0);

    pthread_mutex_lock(&slide_status->access);
    slide_status->des_vel = 0.0f;
    pthread_mutex_unlock(&slide_status->access);

    printf("[MODE] IDLE\n");
    usleep(200000);
}

/*
 * mode_run - 运行模式处理
 * 主控制循环：执行PID控制、安全检查和状态更新
 *
 * 控制算法：
 * - 读取编码器位置
 * - 计算速度(数值微分)
 * - PID控制器计算控制力
 * - 检查极限开关
 * - 向电机施加力
 */
void mode_run(struct status *slide_status)
{
    float integral = 0.0f;
    float last_pos = 0.0f;
    float t_prev = get_time_sec();

    printf("[MODE] RUN\n");
    last_pos = read_position_meter();

    while (1) {
        float des_pos, des_vel, act_pos, act_vel;
        float error_pos, error_vel;
        float p_term, i_term, d_term;
        float u_unsat, u_sat;
        unsigned char mode_snapshot;
        float t_now;

        /* 20ms控制周期 */
        usleep((useconds_t)(DT_SEC * 1e6f));
        t_now = get_time_sec();

        /* ===== 状态获取 ===== */
        act_pos = read_position_meter();
        act_vel = (act_pos - last_pos) / (t_now - t_prev + 1e-6f);  /* 数值微分 */
        update_limits(slide_status);
        imu_update_status(slide_status);

        /* 线程安全地读取共享状态 */
        pthread_mutex_lock(&slide_status->access);
        slide_status->act_pos = act_pos;
        slide_status->act_vel = act_vel;
        des_pos = slide_status->des_pos;
        des_vel = slide_status->des_vel;
        mode_snapshot = slide_status->mode;
        pthread_mutex_unlock(&slide_status->access);

        /* 检查模式切换 */
        if (mode_snapshot != RUN) {
            motor_stop();
            break;
        }

        /* ===== 安全检查：极限开关 ===== */
        if ((limit_hit_left(slide_status) == 1 && des_pos < act_pos) ||
            (limit_hit_right(slide_status) == 1 && des_pos > act_pos)) {
            fprintf(stderr, "[SAFE] limit switch active, stop motion.\n");
            motor_stop();
            pthread_mutex_lock(&slide_status->access);
            slide_status->des_pos = act_pos;  /* 停在当前位置 */
            slide_status->des_vel = 0.0f;
            pthread_mutex_unlock(&slide_status->access);
            continue;
        }

        /* ===== PID控制器 ===== */
        error_pos = des_pos - act_pos;
        error_vel = des_vel - act_vel;

        /* 比例项 */
        p_term = KP * error_pos;
        
        /* 微分项 */
        d_term = KD * error_vel;

        /* 积分项(带抗饱和) */
        integral += error_pos * DT_SEC;
        integral = clampf(integral, INTEGRAL_MIN, INTEGRAL_MAX);
        i_term = KI * integral;

        /* 合成控制力 */
        u_unsat = p_term + i_term + d_term;
        u_sat = clampf(u_unsat, -1.0f, 1.0f);

        /* 反向积分抗饱和：若饱和，则减少积分累积 */
        if (fabsf(u_unsat - u_sat) > 1e-6f) {
            integral -= error_pos * DT_SEC;
            integral = clampf(integral, INTEGRAL_MIN, INTEGRAL_MAX);
        }

        /* 死区检测：位置误差小且速度目标小则停止 */
        if (fabsf(error_pos) < POS_EPSILON && fabsf(des_vel) < 1e-4f) {
            u_sat = 0.0f;
            integral = 0.0f;
        }

        /* ===== 电机控制 ===== */
        motor_apply(u_sat);

        /* 更新上一周期的数据 */
        last_pos = act_pos;
        t_prev = t_now;
    }
}

/* ===== 状态机 ===== */

/*
 * state_machine - 系统状态机主循环
 * @slide_status: 系统状态指针
 *
 * 状态转移：
 * IDLE -> START -> RUN -> IDLE
 * IDLE -> STOP (退出)
 */
void state_machine(struct status *slide_status)
{
    int exit_flag = 1;

    while (exit_flag) {
        unsigned char current_mode;

        pthread_mutex_lock(&slide_status->access);
        current_mode = slide_status->mode;
        pthread_mutex_unlock(&slide_status->access);

        switch (current_mode) {
            case START:
                mode_start(slide_status);
                break;
            case RUN:
                mode_run(slide_status);
                break;
            case STOP:
                mode_stop(slide_status);
                exit_flag = 0;
                break;
            case IDLE:
                mode_idle(slide_status);
                break;
            default:
                break;
        }

        usleep(50000);
    }
}

/* ===== 用户界面线程 ===== */

/*
 * ihm - 人机交互界面线程(Interface Homme-Machine)
 * @arg: 指向slide_status的指针
 *
 * 提供交互菜单以控制系统状态和参数
 * 菜单选项：
 * 0 - START: 启动系统(初始化并归零)
 * 1 - IDLE: 进入待机模式
 * 2 - STOP: 停止系统并退出
 * 3 - GETX: 显示当前位置、速度和传感器状态
 * 4 - SETX: 设置目标位置和速度，执行轨迹跟踪
 * 5 - GETACC: 显示加速度值
 */
void *ihm(void *arg)
{
    struct status *slide_status = (struct status *)arg;
    int cmd;
    int exit_flag = 1;

    while (exit_flag) {
        printf("\n0: START\n1: IDLE\n2: STOP\n3: GETX\n4: SETX\n5: GETACC\nChoix: ");
        fflush(stdout);

        if (scanf("%d", &cmd) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (cmd) {
            /* 启动系统 */
            case 0:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = START;
                pthread_mutex_unlock(&slide_status->access);
                break;

            /* 进入待机 */
            case 1:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = IDLE;
                pthread_mutex_unlock(&slide_status->access);
                break;

            /* 停止系统 */
            case 2:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = STOP;
                pthread_mutex_unlock(&slide_status->access);
                exit_flag = 0;
                break;

            /* 获取位置和速度 */
            case 3: {
                float pos, vel;
                int ll, lr, homed;
                pthread_mutex_lock(&slide_status->access);
                pos = slide_status->act_pos;
                vel = slide_status->act_vel;
                ll = slide_status->lim_left;
                lr = slide_status->lim_right;
                homed = slide_status->homed;
                pthread_mutex_unlock(&slide_status->access);
                printf("act_pos = %.6f m, act_vel = %.6f m/s, limL=%d limR=%d homed=%d\n",
                       pos, vel, ll, lr, homed);
                break;
            }

            /* 设置目标位置和速度 */
            case 4: {
                float Xf, V;
                float Xi, direction;
                struct timeval t0, tc;
                float elapsed, duration;

                printf("Xf (m) = ");
                scanf("%f", &Xf);
                printf("V (m/s) = ");
                scanf("%f", &V);

                /* 获取当前位置 */
                pthread_mutex_lock(&slide_status->access);
                Xi = slide_status->act_pos;
                pthread_mutex_unlock(&slide_status->access);

                /* 计算运动方向和持续时间 */
                direction = (Xf >= Xi) ? 1.0f : -1.0f;
                V = fabsf(V) * direction;
                duration = fabsf(Xf - Xi) / (fabsf(V) + 1e-6f);
                gettimeofday(&t0, NULL);
                elapsed = 0.0f;

                /* 线性轨迹跟踪：从Xi以速度V到Xf */
                while (elapsed < duration) {
                    float des_pos;
                    gettimeofday(&tc, NULL);
                    elapsed = (float)(tc.tv_sec - t0.tv_sec)
                            + (float)(tc.tv_usec - t0.tv_usec) * 1e-6f;
                    des_pos = Xi + V * elapsed;

                    /* 确保不超过目标位置 */
                    if ((direction > 0.0f && des_pos > Xf) ||
                        (direction < 0.0f && des_pos < Xf)) {
                        des_pos = Xf;
                    }

                    /* 更新目标值 */
                    pthread_mutex_lock(&slide_status->access);
                    slide_status->des_pos = des_pos;
                    slide_status->des_vel = V;
                    pthread_mutex_unlock(&slide_status->access);

                    usleep((useconds_t)(DT_SEC * 1e6f));
                }

                /* 到达目标，停止运动 */
                pthread_mutex_lock(&slide_status->access);
                slide_status->des_pos = Xf;
                slide_status->des_vel = 0.0f;
                pthread_mutex_unlock(&slide_status->access);
                break;
            }

            /* 获取加速度 */
            case 5: {
                float ax, ay, az;
                imu_update_status(slide_status);
                pthread_mutex_lock(&slide_status->access);
                ax = slide_status->acc_x;
                ay = slide_status->acc_y;
                az = slide_status->acc_z;
                pthread_mutex_unlock(&slide_status->access);
                printf("acc = [%.3f, %.3f, %.3f] g\n", ax, ay, az);
                break;
            }

            default:
                break;
        }
    }

    pthread_exit(NULL);
}

static float clampf(float x, float min_v, float max_v)
{
    if (x < min_v) return min_v;
    if (x > max_v) return max_v;
    return x;
}

static void motor_stop(void)
{
    set_pwm(EHRPWM1A, PWM_DUTY, 0);
    set_pwm(EHRPWM1A, PWM_RUN, 0);
}

static void motor_apply(float u)
{
    int dir = (u >= 0.0f) ? 1 : 0;
    float abs_u = clampf(fabsf(u), 0.0f, 1.0f);
    unsigned int duty_ns = (unsigned int)(abs_u * PWM_PERIOD_NS);

    if (duty_ns > PWM_DUTY_MAX_NS) duty_ns = PWM_DUTY_MAX_NS;
    if (duty_ns < PWM_DUTY_MIN_NS) duty_ns = PWM_DUTY_MIN_NS;

    set_gpio(GPIO0_31, VALUE, dir);
    set_pwm(EHRPWM1A, PWM_DUTY, duty_ns);
    set_pwm(EHRPWM1A, PWM_RUN, 1);
}

static int limits_read_left(void)
{
    return get_gpio(GPIO1_28, VALUE);
}

static int limits_read_right(void)
{
    return get_gpio(GPIO1_18, VALUE);
}

static float read_position_meter(void)
{
    int pos_count = get_eqep(EQEP1, POSITION);
    return (float)pos_count * COUNT_TO_METER;
}

static float get_time_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (float)tv.tv_sec + (float)tv.tv_usec * 1e-6f;
}

static void update_limits(struct status *slide_status)
{
    pthread_mutex_lock(&slide_status->access);
    slide_status->lim_left = limits_read_left();
    slide_status->lim_right = limits_read_right();
    pthread_mutex_unlock(&slide_status->access);
}

static int limit_hit_left(struct status *slide_status)
{
    int v;
    pthread_mutex_lock(&slide_status->access);
    v = slide_status->lim_left;
    pthread_mutex_unlock(&slide_status->access);
    return v;
}

static int limit_hit_right(struct status *slide_status)
{
    int v;
    pthread_mutex_lock(&slide_status->access);
    v = slide_status->lim_right;
    pthread_mutex_unlock(&slide_status->access);
    return v;
}

static int imu_init_once(void)
{
    if (g_imu.ready) return 0;
    if (lis331hh_open(&g_imu.dev, LIS331HH_I2C_DEV, LIS331HH_I2C_ADDR) != 0) return -1;
    if (lis331hh_init(&g_imu.dev, LIS331HH_RANGE_6G) != 0) {
        lis331hh_close(&g_imu.dev);
        return -1;
    }
    g_imu.ready = 1;
    return 0;
}

static void imu_update_status(struct status *slide_status)
{
    float ax, ay, az;
    if (!g_imu.ready) {
        if (imu_init_once() != 0) return;
    }
    if (lis331hh_read_xyz_g(&g_imu.dev, &ax, &ay, &az) == 0) {
        pthread_mutex_lock(&slide_status->access);
        slide_status->acc_x = ax;
        slide_status->acc_y = ay;
        slide_status->acc_z = az;
        pthread_mutex_unlock(&slide_status->access);
    }
}

static int homing_sequence(struct status *slide_status)
{
    float t0 = get_time_sec();
    printf("[HOME] moving toward left limit...\n");

    while ((get_time_sec() - t0) < HOMING_TIMEOUT_SEC) {
        update_limits(slide_status);
        if (limit_hit_left(slide_status) == 1) {
            motor_stop();
            set_eqep(EQEP1, POSITION, 0);
            pthread_mutex_lock(&slide_status->access);
            slide_status->act_pos = 0.0f;
            slide_status->act_vel = 0.0f;
            slide_status->homed = 1;
            pthread_mutex_unlock(&slide_status->access);
            printf("[HOME] left limit reached, origin set.\n");
            return 0;
        }

        motor_apply(-HOMING_PWM);
        imu_update_status(slide_status);
        usleep((useconds_t)(DT_SEC * 1e6f));
    }

    motor_stop();
    fprintf(stderr, "[HOME] timeout.\n");
    return -1;
}

void mode_stop(struct status *slide_status)
{
    motor_stop();
    set_eqep(EQEP1, ENABLED, 0);

    pthread_mutex_lock(&slide_status->access);
    slide_status->des_vel = 0.0f;
    pthread_mutex_unlock(&slide_status->access);

    if (g_imu.ready) {
        lis331hh_close(&g_imu.dev);
        g_imu.ready = 0;
    }

    printf("[MODE] STOP\n");
}

void mode_start(struct status *slide_status)
{
    printf("[MODE] START\n");

    set_gpio(GPIO0_31, DIRECTION, 0);
    set_gpio(GPIO0_31, VALUE, 0);
    set_gpio(GPIO1_28, DIRECTION, 1);
    set_gpio(GPIO1_18, DIRECTION, 1);

    set_pwm(EHRPWM1A, PWM_PERIOD, PWM_PERIOD_NS);
    set_pwm(EHRPWM1A, PWM_DUTY, 0);
    set_pwm(EHRPWM1A, PWM_POLARITY, 0);
    set_pwm(EHRPWM1A, PWM_RUN, 1);

    set_eqep(EQEP1, MODE, 0);
    set_eqep(EQEP1, POSITION, 0);
    set_eqep(EQEP1, ENABLED, 1);

    imu_init_once();

    if (homing_sequence(slide_status) != 0) {
        pthread_mutex_lock(&slide_status->access);
        slide_status->mode = IDLE;
        pthread_mutex_unlock(&slide_status->access);
        return;
    }

    pthread_mutex_lock(&slide_status->access);
    slide_status->mode = RUN;
    pthread_mutex_unlock(&slide_status->access);

    printf("[MODE] START done -> RUN\n");
}

void mode_idle(struct status *slide_status)
{
    motor_stop();
    set_eqep(EQEP1, ENABLED, 0);

    pthread_mutex_lock(&slide_status->access);
    slide_status->des_vel = 0.0f;
    pthread_mutex_unlock(&slide_status->access);

    printf("[MODE] IDLE\n");
    usleep(200000);
}

void mode_run(struct status *slide_status)
{
    float integral = 0.0f;
    float last_pos = 0.0f;
    float t_prev = get_time_sec();

    printf("[MODE] RUN\n");
    last_pos = read_position_meter();

    while (1) {
        float des_pos, des_vel, act_pos, act_vel;
        float error_pos, error_vel;
        float p_term, i_term, d_term;
        float u_unsat, u_sat;
        unsigned char mode_snapshot;
        float t_now;

        usleep((useconds_t)(DT_SEC * 1e6f));
        t_now = get_time_sec();

        act_pos = read_position_meter();
        act_vel = (act_pos - last_pos) / (t_now - t_prev + 1e-6f);
        update_limits(slide_status);
        imu_update_status(slide_status);

        pthread_mutex_lock(&slide_status->access);
        slide_status->act_pos = act_pos;
        slide_status->act_vel = act_vel;
        des_pos = slide_status->des_pos;
        des_vel = slide_status->des_vel;
        mode_snapshot = slide_status->mode;
        pthread_mutex_unlock(&slide_status->access);

        if (mode_snapshot != RUN) {
            motor_stop();
            break;
        }

        if ((limit_hit_left(slide_status) == 1 && des_pos < act_pos) ||
            (limit_hit_right(slide_status) == 1 && des_pos > act_pos)) {
            fprintf(stderr, "[SAFE] limit switch active, stop motion.\n");
            motor_stop();
            pthread_mutex_lock(&slide_status->access);
            slide_status->des_pos = act_pos;
            slide_status->des_vel = 0.0f;
            pthread_mutex_unlock(&slide_status->access);
            continue;
        }

        error_pos = des_pos - act_pos;
        error_vel = des_vel - act_vel;

        p_term = KP * error_pos;
        d_term = KD * error_vel;

        integral += error_pos * DT_SEC;
        integral = clampf(integral, INTEGRAL_MIN, INTEGRAL_MAX);
        i_term = KI * integral;

        u_unsat = p_term + i_term + d_term;
        u_sat = clampf(u_unsat, -1.0f, 1.0f);

        /* stronger anti-windup: back-calculation style */
        if (fabsf(u_unsat - u_sat) > 1e-6f) {
            integral -= error_pos * DT_SEC;
            integral = clampf(integral, INTEGRAL_MIN, INTEGRAL_MAX);
        }

        if (fabsf(error_pos) < POS_EPSILON && fabsf(des_vel) < 1e-4f) {
            u_sat = 0.0f;
            integral = 0.0f;
        }

        motor_apply(u_sat);

        last_pos = act_pos;
        t_prev = t_now;
    }
}

void state_machine(struct status *slide_status)
{
    int exit_flag = 1;

    while (exit_flag) {
        unsigned char current_mode;

        pthread_mutex_lock(&slide_status->access);
        current_mode = slide_status->mode;
        pthread_mutex_unlock(&slide_status->access);

        switch (current_mode) {
            case START:
                mode_start(slide_status);
                break;
            case RUN:
                mode_run(slide_status);
                break;
            case STOP:
                mode_stop(slide_status);
                exit_flag = 0;
                break;
            case IDLE:
                mode_idle(slide_status);
                break;
            default:
                break;
        }

        usleep(50000);
    }
}

void *ihm(void *arg)
{
    struct status *slide_status = (struct status *)arg;
    int cmd;
    int exit_flag = 1;

    while (exit_flag) {
        printf("\n0: START\n1: IDLE\n2: STOP\n3: GETX\n4: SETX\n5: GETACC\nChoix: ");
        fflush(stdout);

        if (scanf("%d", &cmd) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (cmd) {
            case 0:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = START;
                pthread_mutex_unlock(&slide_status->access);
                break;

            case 1:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = IDLE;
                pthread_mutex_unlock(&slide_status->access);
                break;

            case 2:
                pthread_mutex_lock(&slide_status->access);
                slide_status->mode = STOP;
                pthread_mutex_unlock(&slide_status->access);
                exit_flag = 0;
                break;

            case 3: {
                float pos, vel;
                int ll, lr, homed;
                pthread_mutex_lock(&slide_status->access);
                pos = slide_status->act_pos;
                vel = slide_status->act_vel;
                ll = slide_status->lim_left;
                lr = slide_status->lim_right;
                homed = slide_status->homed;
                pthread_mutex_unlock(&slide_status->access);
                printf("act_pos = %.6f m, act_vel = %.6f m/s, limL=%d limR=%d homed=%d\n",
                       pos, vel, ll, lr, homed);
                break;
            }

            case 4: {
                float Xf, V;
                float Xi, direction;
                struct timeval t0, tc;
                float elapsed, duration;

                printf("Xf (m) = ");
                scanf("%f", &Xf);
                printf("V (m/s) = ");
                scanf("%f", &V);

                pthread_mutex_lock(&slide_status->access);
                Xi = slide_status->act_pos;
                pthread_mutex_unlock(&slide_status->access);

                direction = (Xf >= Xi) ? 1.0f : -1.0f;
                V = fabsf(V) * direction;
                duration = fabsf(Xf - Xi) / (fabsf(V) + 1e-6f);
                gettimeofday(&t0, NULL);
                elapsed = 0.0f;

                while (elapsed < duration) {
                    float des_pos;
                    gettimeofday(&tc, NULL);
                    elapsed = (float)(tc.tv_sec - t0.tv_sec)
                            + (float)(tc.tv_usec - t0.tv_usec) * 1e-6f;
                    des_pos = Xi + V * elapsed;

                    if ((direction > 0.0f && des_pos > Xf) ||
                        (direction < 0.0f && des_pos < Xf)) {
                        des_pos = Xf;
                    }

                    pthread_mutex_lock(&slide_status->access);
                    slide_status->des_pos = des_pos;
                    slide_status->des_vel = V;
                    pthread_mutex_unlock(&slide_status->access);

                    usleep((useconds_t)(DT_SEC * 1e6f));
                }

                pthread_mutex_lock(&slide_status->access);
                slide_status->des_pos = Xf;
                slide_status->des_vel = 0.0f;
                pthread_mutex_unlock(&slide_status->access);
                break;
            }

            case 5: {
                float ax, ay, az;
                imu_update_status(slide_status);
                pthread_mutex_lock(&slide_status->access);
                ax = slide_status->acc_x;
                ay = slide_status->acc_y;
                az = slide_status->acc_z;
                pthread_mutex_unlock(&slide_status->access);
                printf("acc = [%.3f, %.3f, %.3f] g\n", ax, ay, az);
                break;
            }

            default:
                break;
        }
    }

    pthread_exit(NULL);
}
