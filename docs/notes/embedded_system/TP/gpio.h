#ifndef GPIO_H
#define GPIO_H

#define GPIO0_31   "/sys/class/gpio/gpio31"
#define GPIO1_28   "/sys/class/gpio/gpio60"
#define GPIO1_18   "/sys/class/gpio/gpio50"

#define DIRECTION  "direction"
#define VALUE      "value"

void set_gpio(const char *dev, const char *reg, int val);
int  get_gpio(const char *dev, const char *reg);

#endif
