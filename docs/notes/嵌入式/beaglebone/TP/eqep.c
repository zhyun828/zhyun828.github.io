/*
 * eqep.c - 编码器位置检测模块实现
 * 通过Linux系统文件接口与EQEP硬件交互
 */

#include <stdio.h>
#include <stdlib.h>
#include "eqep.h"

/*
 * set_eqep - 向EQEP寄存器写入整数值
 * @dev: 设备节点路径
 * @reg: 寄存器名称
 * @val: 要写入的值
 * 
 * 通过系统文件接口将值写入编码器寄存器
 */
void set_eqep(const char *dev, const char *reg, int val)
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

    /* 向文件写入整数值 */
    fprintf(fp, "%d", val);
    fclose(fp);
}

/*
 * get_eqep - 从EQEP寄存器读取整数值
 * @dev: 设备节点路径
 * @reg: 寄存器名称
 * 返回值: 读取的整数值，错误时返回-1
 * 
 * 通过系统文件接口从编码器寄存器读取值
 */
int get_eqep(const char *dev, const char *reg)
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
