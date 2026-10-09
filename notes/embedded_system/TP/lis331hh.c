/*
 * lis331hh.c - LIS331HH加速度传感器驱动实现
 * 通过I2C接口与传感器通信
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lis331hh.h"

/*
 * lis331hh_write_reg - 向传感器寄存器写入单字节
 * @dev: 设备指针
 * @reg: 寄存器地址
 * @value: 要写入的值
 * 返回值: 成功返回0，失败返回-1
 */
static int lis331hh_write_reg(lis331hh_t *dev, uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    if (write(dev->fd, buf, 2) != 2) {
        perror("lis331hh_write_reg");
        return -1;
    }
    return 0;
}

/*
 * lis331hh_read_reg - 从传感器寄存器读取单字节
 * @dev: 设备指针
 * @reg: 寄存器地址
 * @value: 存储读取值的指针
 * 返回值: 成功返回0，失败返回-1
 */
static int lis331hh_read_reg(lis331hh_t *dev, uint8_t reg, uint8_t *value)
{
    /* 先发送寄存器地址 */
    if (write(dev->fd, &reg, 1) != 1) {
        perror("lis331hh_read_reg addr");
        return -1;
    }
    /* 再读取寄存器数据 */
    if (read(dev->fd, value, 1) != 1) {
        perror("lis331hh_read_reg data");
        return -1;
    }
    return 0;
}

/*
 * lis331hh_read_multi - 从传感器连续读取多个寄存器
 * @dev: 设备指针
 * @start_reg: 起始寄存器地址
 * @buf: 存储读取数据的缓冲区
 * @len: 要读取的字节数
 * 返回值: 成功返回0，失败返回-1
 *
 * 使用自动地址递增功能提高效率
 */
static int lis331hh_read_multi(lis331hh_t *dev, uint8_t start_reg, uint8_t *buf, int len)
{
    /* 设置自动地址递增位以支持多字节读取 */
    uint8_t reg = start_reg | LIS331HH_AUTOINC;
    if (write(dev->fd, &reg, 1) != 1) {
        perror("lis331hh_read_multi addr");
        return -1;
    }
    if (read(dev->fd, buf, len) != len) {
        perror("lis331hh_read_multi data");
        return -1;
    }
    return 0;
}

/*
 * lis331hh_open - 打开I2C设备并初始化传感器结构体
 * @dev: 传感器结构体指针
 * @i2c_dev: I2C设备文件路径(如"/dev/i2c-1")
 * @addr: I2C从地址
 * 返回值: 成功返回0，失败返回-1
 */
int lis331hh_open(lis331hh_t *dev, const char *i2c_dev, int addr)
{
    /* 打开I2C设备文件 */
    dev->fd = open(i2c_dev, O_RDWR);
    if (dev->fd < 0) {
        perror("open i2c");
        return -1;
    }

    /* 设置I2C从地址 */
    if (ioctl(dev->fd, I2C_SLAVE, addr) < 0) {
        perror("ioctl I2C_SLAVE");
        close(dev->fd);
        dev->fd = -1;
        return -1;
    }

    return 0;
}

/*
 * lis331hh_init - 初始化加速度传感器
 * @dev: 传感器结构体指针
 * @range: 满量程范围(6G/12G/24G)
 * 返回值: 成功返回0，失败返回-1
 *
 * 配置工作模式、数据率、满量程范围等参数
 */
int lis331hh_init(lis331hh_t *dev, lis331hh_range_t range)
{
    uint8_t who = 0;
    uint8_t ctrl4 = 0;

    /* 读取WHO_AM_I寄存器验证设备 */
    if (lis331hh_read_reg(dev, LIS331HH_WHO_AM_I, &who) != 0) {
        return -1;
    }

    if (who != LIS331HH_WHOAMI_VAL) {
        fprintf(stderr, "Unexpected WHO_AM_I: 0x%02X\n", who);
        return -1;
    }

    /* 根据满量程范围设置灵敏度并确定控制寄存器值 */
    dev->range = range;
    switch (range) {
        case LIS331HH_RANGE_6G:
            dev->sensitivity_mg_lsb = 3.0f;    /* 3mg/LSB */
            ctrl4 = 0x00;
            break;
        case LIS331HH_RANGE_12G:
            dev->sensitivity_mg_lsb = 6.0f;    /* 6mg/LSB */
            ctrl4 = 0x10;
            break;
        case LIS331HH_RANGE_24G:
        default:
            dev->sensitivity_mg_lsb = 12.0f;   /* 12mg/LSB */
            ctrl4 = 0x30;
            break;
    }

    /* 配置各控制寄存器 */
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG1, 0x27) != 0) return -1; /* 数据率50Hz, 工作模式, XYZ使能 */
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG2, 0x00) != 0) return -1;
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG3, 0x00) != 0) return -1;
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG4, ctrl4) != 0) return -1;  /* 设置满量程范围 */
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG5, 0x00) != 0) return -1;

    /* 等待传感器稳定 */
    usleep(10000);
    return 0;
}

/*
 * lis331hh_read_xyz_g - 读取三轴加速度值
 * @dev: 传感器结构体指针
 * @x_g: 存储X轴加速度值的指针(单位:G)
 * @y_g: 存储Y轴加速度值的指针(单位:G)
 * @z_g: 存储Z轴加速度值的指针(单位:G)
 * 返回值: 成功返回0，失败返回-1
 *
 * 从传感器读取原始数据并转换为G(重力加速度)单位
 */
int lis331hh_read_xyz_g(lis331hh_t *dev, float *x_g, float *y_g, float *z_g)
{
    uint8_t raw[6];
    int16_t x, y, z;

    /* 一次性读取6个字节(3个轴各2字节) */
    if (lis331hh_read_multi(dev, LIS331HH_OUT_X_L, raw, 6) != 0) {
        return -1;
    }

    /* 将字节串组合成16位有符号整数(小端格式) */
    x = (int16_t)((raw[1] << 8) | raw[0]);
    y = (int16_t)((raw[3] << 8) | raw[2]);
    z = (int16_t)((raw[5] << 8) | raw[4]);

    /* 转换为G单位 
     * 右移4位(因为LIS331HH的数据格式是12位)
     * 乘以灵敏度系数(mg/LSB)
     * 除以1000转换为G(重力加速度)
     */
    *x_g = (x >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    *y_g = (y >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    *z_g = (z >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    return 0;
}

/*
 * lis331hh_close - 关闭I2C设备
 * @dev: 传感器结构体指针
 *
 * 释放I2C设备资源
 */
void lis331hh_close(lis331hh_t *dev)
{
    if (dev->fd >= 0) {
        close(dev->fd);
        dev->fd = -1;
    }
}
