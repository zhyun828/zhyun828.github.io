/*
 * lis331hh.h - LIS331HH 加速度传感器模块头文件
 * I2C接口的三轴加速度计驱动
 */

#ifndef LIS331HH_H
#define LIS331HH_H

#include <stdint.h>

/* I2C设备配置 */
#define LIS331HH_I2C_DEV      "/dev/i2c-1"  /* I2C总线1 */
#define LIS331HH_I2C_ADDR     0x18          /* LIS331HH从地址 */

/* 寄存器地址定义 */
#define LIS331HH_WHO_AM_I     0x0F  /* 器件ID寄存器 */
#define LIS331HH_CTRL_REG1    0x20  /* 控制寄存器1 - 配置数据率、工作模式等 */
#define LIS331HH_CTRL_REG2    0x21  /* 控制寄存器2 */
#define LIS331HH_CTRL_REG3    0x22  /* 控制寄存器3 */
#define LIS331HH_CTRL_REG4    0x23  /* 控制寄存器4 - 配置满量程范围 */
#define LIS331HH_CTRL_REG5    0x24  /* 控制寄存器5 */
#define LIS331HH_STATUS_REG   0x27  /* 状态寄存器 */
#define LIS331HH_OUT_X_L      0x28  /* X轴加速度低字节 */
#define LIS331HH_OUT_X_H      0x29  /* X轴加速度高字节 */
#define LIS331HH_OUT_Y_L      0x2A  /* Y轴加速度低字节 */
#define LIS331HH_OUT_Y_H      0x2B  /* Y轴加速度高字节 */
#define LIS331HH_OUT_Z_L      0x2C  /* Z轴加速度低字节 */
#define LIS331HH_OUT_Z_H      0x2D  /* Z轴加速度高字节 */

/* 寄存器控制位 */
#define LIS331HH_AUTOINC      0x80  /* 自动地址递增位(用于多字节读取) */
#define LIS331HH_WHOAMI_VAL   0x32  /* WHO_AM_I预期值 */

/* 加速度计满量程范围枚举 */
typedef enum {
    LIS331HH_RANGE_6G  = 0,   /* ±6g范围 */
    LIS331HH_RANGE_12G = 1,   /* ±12g范围 */
    LIS331HH_RANGE_24G = 3    /* ±24g范围 */
} lis331hh_range_t;

/* LIS331HH设备结构体 */
typedef struct {
    int fd;                        /* I2C设备文件描述符 */
    lis331hh_range_t range;       /* 当前满量程范围设置 */
    float sensitivity_mg_lsb;     /* 灵敏度:毫G/LSB */
} lis331hh_t;

/* 公共接口函数 */

/* 打开I2C设备并初始化设备结构体 */
int lis331hh_open(lis331hh_t *dev, const char *i2c_dev, int addr);

/* 配置加速度计的测量范围和工作模式 */
int lis331hh_init(lis331hh_t *dev, lis331hh_range_t range);

/* 读取三轴加速度值(单位:G) */
int lis331hh_read_xyz_g(lis331hh_t *dev, float *x_g, float *y_g, float *z_g);

/* 关闭I2C设备 */
void lis331hh_close(lis331hh_t *dev);

#endif
