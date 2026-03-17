#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lis331hh.h"

static int lis331hh_write_reg(lis331hh_t *dev, uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    if (write(dev->fd, buf, 2) != 2) {
        perror("lis331hh_write_reg");
        return -1;
    }
    return 0;
}

static int lis331hh_read_reg(lis331hh_t *dev, uint8_t reg, uint8_t *value)
{
    if (write(dev->fd, &reg, 1) != 1) {
        perror("lis331hh_read_reg addr");
        return -1;
    }
    if (read(dev->fd, value, 1) != 1) {
        perror("lis331hh_read_reg data");
        return -1;
    }
    return 0;
}

static int lis331hh_read_multi(lis331hh_t *dev, uint8_t start_reg, uint8_t *buf, int len)
{
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

int lis331hh_open(lis331hh_t *dev, const char *i2c_dev, int addr)
{
    dev->fd = open(i2c_dev, O_RDWR);
    if (dev->fd < 0) {
        perror("open i2c");
        return -1;
    }

    if (ioctl(dev->fd, I2C_SLAVE, addr) < 0) {
        perror("ioctl I2C_SLAVE");
        close(dev->fd);
        dev->fd = -1;
        return -1;
    }

    return 0;
}

int lis331hh_init(lis331hh_t *dev, lis331hh_range_t range)
{
    uint8_t who = 0;
    uint8_t ctrl4 = 0;

    if (lis331hh_read_reg(dev, LIS331HH_WHO_AM_I, &who) != 0) {
        return -1;
    }

    if (who != LIS331HH_WHOAMI_VAL) {
        fprintf(stderr, "Unexpected WHO_AM_I: 0x%02X\n", who);
        return -1;
    }

    dev->range = range;
    switch (range) {
        case LIS331HH_RANGE_6G:
            dev->sensitivity_mg_lsb = 3.0f;
            ctrl4 = 0x00;
            break;
        case LIS331HH_RANGE_12G:
            dev->sensitivity_mg_lsb = 6.0f;
            ctrl4 = 0x10;
            break;
        case LIS331HH_RANGE_24G:
        default:
            dev->sensitivity_mg_lsb = 12.0f;
            ctrl4 = 0x30;
            break;
    }

    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG1, 0x27) != 0) return -1; /* 50Hz, normal, XYZ enabled */
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG2, 0x00) != 0) return -1;
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG3, 0x00) != 0) return -1;
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG4, ctrl4) != 0) return -1;
    if (lis331hh_write_reg(dev, LIS331HH_CTRL_REG5, 0x00) != 0) return -1;

    usleep(10000);
    return 0;
}

int lis331hh_read_xyz_g(lis331hh_t *dev, float *x_g, float *y_g, float *z_g)
{
    uint8_t raw[6];
    int16_t x, y, z;

    if (lis331hh_read_multi(dev, LIS331HH_OUT_X_L, raw, 6) != 0) {
        return -1;
    }

    x = (int16_t)((raw[1] << 8) | raw[0]);
    y = (int16_t)((raw[3] << 8) | raw[2]);
    z = (int16_t)((raw[5] << 8) | raw[4]);

    *x_g = (x >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    *y_g = (y >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    *z_g = (z >> 4) * dev->sensitivity_mg_lsb / 1000.0f;
    return 0;
}

void lis331hh_close(lis331hh_t *dev)
{
    if (dev->fd >= 0) {
        close(dev->fd);
        dev->fd = -1;
    }
}
