/*
 * gpio.h - GPIO(通用输入输出)模块头文件
 * 用于控制GPIO引脚的方向和电平
 */

#ifndef GPIO_H
#define GPIO_H

/* GPIO端口定义 - BeagleBone Black引脚映射 */
#define GPIO0_31   "/sys/class/gpio/gpio31"  /* GPIO0_31 - 电机方向控制 */
#define GPIO1_28   "/sys/class/gpio/gpio60"  /* GPIO1_28 - 左极限开关 */
#define GPIO1_18   "/sys/class/gpio/gpio50"  /* GPIO1_18 - 右极限开关 */

/* GPIO属性名称 */
#define DIRECTION  "direction"  /* GPIO输入输出方向: "in" 或 "out" */
#define VALUE      "value"      /* GPIO电平值: 0 或 1 */

/* 向GPIO引脚写入值 */
void set_gpio(const char *dev, const char *reg, int val);

/* 从GPIO引脚读取值 */
int  get_gpio(const char *dev, const char *reg);

#endif
