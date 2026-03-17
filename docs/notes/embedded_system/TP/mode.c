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

#define DT_SEC              0.02f
#define PWM_PERIOD_NS       500000
#define PWM_DUTY_MIN_NS     0
#define PWM_DUTY_MAX_NS     500000

#define KP                  8000.0f
#define KI                  500.0f
#define KD                  1000.0f
#define INTEGRAL_MAX        50.0f
#define INTEGRAL_MIN       -50.0f
#define COUNT_TO_METER      0.00005f
#define POS_EPSILON         0.001f

#define HOMING_PWM          0.18f
#define HOMING_TIMEOUT_SEC  8.0f

typedef struct {
    lis331hh_t dev;
    int ready;
} imu_context_t;

static imu_context_t g_imu = {.ready = 0};

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
