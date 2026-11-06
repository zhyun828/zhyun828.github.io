/*
 * eqep.h - 编码器位置检测模块(EQEP - Enhanced Quadrature Encoder Pulse)
 * 用于读取电机编码器的位置信息
 */

#ifndef EQEP_H
#define EQEP_H

/* EQEP1 设备节点路径 - BeagleBone Black系统中的编码器接口 */
#define EQEP1   "/sys/devices/ocp.2/48302000.epwmss/48302180.eqep"

/* EQEP 相关属性名称 */
#define POSITION "position"  /* 编码器当前位置计数值 */
#define MODE     "mode"      /* 编码器工作模式 */
#define PERIOD   "period"    /* 编码周期 */
#define ENABLED  "enabled"   /* 编码器是否启用 */

/* 向EQEP设备写入寄存器值 */
void set_eqep(const char *dev, const char *reg, int val);

/* 从EQEP设备读取寄存器值 */
int  get_eqep(const char *dev, const char *reg);

#endif
