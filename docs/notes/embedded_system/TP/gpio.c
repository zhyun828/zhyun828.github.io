/*
 * gpio.c - GPIO模块实现
 * 通过Linux系统文件接口与GPIO硬件交互
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"

/*
 * set_gpio - 向GPIO引脚写入值
 * @dev: GPIO设备节点路径
 * @reg: 寄存器名称（DIRECTION 或 VALUE）
 * @val: 要写入的值
 *
 * 如果写入DIRECTION: val=0表示输出(out), val=1表示输入(in)
 * 如果写入VALUE: val=0表示低电平, val=1表示高电平
 */
void set_gpio(const char *dev, const char *reg, int val)
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

    /* 根据寄存器类型写入不同的值 */
    if (strcmp(reg, DIRECTION) == 0) {
        /* 方向设置: 0=输出, 1=输入 */
        fprintf(fp, "%s", val == 0 ? "out" : "in");
    } else {
        /* 值设置: 直接写入数值 */
        fprintf(fp, "%d", val);
    }

    fclose(fp);
}

/*
 * get_gpio - 从GPIO引脚读取值
 * @dev: GPIO设备节点路径
 * @reg: 寄存器名称
 * 返回值: 读取的整数值，错误时返回-1
 *
 * 读取GPIO的当前电平(0 或 1)
 */
int get_gpio(const char *dev, const char *reg)
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
