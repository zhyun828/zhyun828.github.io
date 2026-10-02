/*
 * pwm.h - PWM(脉冲宽度调制)模块头文件
 * 用于控制电机的速度和方向
 */

#ifndef PWM_H
#define PWM_H

/* PWM输出通道 - BeagleBone Black的EHRPWM1A输出引脚 */
#define EHRPWM1A "/sys/devices/ocp.2/pwm_test_P9_14.10"
/* 备选路径，某些BeagleBone系统上可能使用:
 * #define EHRPWM1A "/sys/devices/ocp.2/pwm_test_P9_14.9"
 */

/* PWM参数名称 */
#define PWM_DUTY     "duty"     /* PWM占空比(纳秒) */
#define PWM_PERIOD   "period"   /* PWM周期(纳秒) */
#define PWM_POLARITY "polarity" /* PWM极性 */
#define PWM_RUN      "run"      /* PWM使能标志: 0=停止, 1=运行 */

/* 向PWM通道写入值 */
void set_pwm(const char *dev, const char *reg, unsigned int val);

/* 从PWM通道读取值 */
int  get_pwm(const char *dev, const char *reg);

#endif
