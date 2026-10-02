/*
 * pwm.c - PWM模块实现
 * 通过Linux系统文件接口与PWM硬件交互
 */

#include <stdio.h>
#include <stdlib.h>
#include "pwm.h"

/*
 * set_pwm - 向PWM通道写入值
 * @dev: PWM设备节点路径
 * @reg: 寄存器名称
 * @val: 要写入的值(纳秒)
 *
 * 用于设置PWM的周期、占空比、极性等参数
 */
void set_pwm(const char *dev, const char *reg, unsigned int val)
{
    char path[256];
    FILE *fp;

    /* 构建完整的文件路径 */
    snprintf(path, sizeof(path), "%s/%s", dev, reg);
    
    /* 打开文件以写入模式 */
    fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return;
    }

    /* 向文件写入无符号整数值(以纳秒为单位) */
    fprintf(fp, "%u", val);
    fclose(fp);
}

/*
 * get_pwm - 从PWM通道读取值
 * @dev: PWM设备节点路径
 * @reg: 寄存器名称
 * 返回值: 读取的整数值，错误时返回-1
 *
 * 用于查询PWM的当前参数设置
 */
int get_pwm(const char *dev, const char *reg)
{
    char path[256];
    FILE *fp;
    int val = 0;

    /* 构建完整的文件路径 */
    snprintf(path, sizeof(path), "%s/%s", dev, reg);
    
    /* 打开文件以读取模式 */
    fp = fopen(path, "r");
    if (!fp) {
        perror(path);
        return -1;
    }

    /* 从文件读取整数值 */
    fscanf(fp, "%d", &val);
    fclose(fp);
    return val;
}
