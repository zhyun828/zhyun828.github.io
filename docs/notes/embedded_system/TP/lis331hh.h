#ifndef LIS331HH_H
#define LIS331HH_H

#include <stdint.h>

#define LIS331HH_I2C_DEV      "/dev/i2c-1"
#define LIS331HH_I2C_ADDR     0x18

#define LIS331HH_WHO_AM_I     0x0F
#define LIS331HH_CTRL_REG1    0x20
#define LIS331HH_CTRL_REG2    0x21
#define LIS331HH_CTRL_REG3    0x22
#define LIS331HH_CTRL_REG4    0x23
#define LIS331HH_CTRL_REG5    0x24
#define LIS331HH_STATUS_REG   0x27
#define LIS331HH_OUT_X_L      0x28
#define LIS331HH_OUT_X_H      0x29
#define LIS331HH_OUT_Y_L      0x2A
#define LIS331HH_OUT_Y_H      0x2B
#define LIS331HH_OUT_Z_L      0x2C
#define LIS331HH_OUT_Z_H      0x2D

#define LIS331HH_AUTOINC      0x80
#define LIS331HH_WHOAMI_VAL   0x32

typedef enum {
    LIS331HH_RANGE_6G  = 0,
    LIS331HH_RANGE_12G = 1,
    LIS331HH_RANGE_24G = 3
} lis331hh_range_t;

typedef struct {
    int fd;
    lis331hh_range_t range;
    float sensitivity_mg_lsb;
} lis331hh_t;

int lis331hh_open(lis331hh_t *dev, const char *i2c_dev, int addr);
int lis331hh_init(lis331hh_t *dev, lis331hh_range_t range);
int lis331hh_read_xyz_g(lis331hh_t *dev, float *x_g, float *y_g, float *z_g);
void lis331hh_close(lis331hh_t *dev);

#endif
