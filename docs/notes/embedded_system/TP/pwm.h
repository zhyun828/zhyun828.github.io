#ifndef PWM_H
#define PWM_H

#define EHRPWM1A "/sys/devices/ocp.2/pwm_test_P9_14.10"
/* Alternative path often seen on other BeagleBone images:
 * #define EHRPWM1A "/sys/devices/ocp.2/pwm_test_P9_14.9"
 */

#define PWM_DUTY     "duty"
#define PWM_PERIOD   "period"
#define PWM_POLARITY "polarity"
#define PWM_RUN      "run"

void set_pwm(const char *dev, const char *reg, unsigned int val);
int  get_pwm(const char *dev, const char *reg);

#endif
