/*
 * mode.h - 控制模式管理模块头文件
 * 定义系统的运行状态机和共享状态结构体
 */

#ifndef MODE_H
#define MODE_H

#include <pthread.h>

/* 系统运行模式定义 */
#define STOP  0  /* 停止模式 - 系统关闭 */
#define START 1  /* 启动模式 - 初始化阶段 */
#define RUN   2  /* 运行模式 - 正常控制阶段 */
#define IDLE  3  /* 待机模式 - 可随时启动 */

/* 系统状态共享结构体 - 在线程间共享 */
struct status
{
    /* 期望(目标)值 */
    float des_pos;    /* 期望位置(米) */
    float des_vel;    /* 期望速度(米/秒) */
    
    /* 实际值 */
    float act_pos;    /* 实际位置(米) */
    float act_vel;    /* 实际速度(米/秒) */
    
    /* 加速度传感器读取值 */
    float acc_x;      /* X轴加速度(G) */
    float acc_y;      /* Y轴加速度(G) */
    float acc_z;      /* Z轴加速度(G) */
    
    /* 系统状态 */
    unsigned char mode;  /* 当前运行模式 */
    int lim_left;       /* 左端极限开关状态: 0=未触发, 1=触发 */
    int lim_right;      /* 右端极限开关状态: 0=未触发, 1=触发 */
    int homed;          /* 归零标志: 0=未归零, 1=已归零 */
    
    /* 线程同步 */
    pthread_mutex_t access;  /* 互斥锁，用于保护结构体中的数据 */
};

/* 模式处理函数 */
void mode_stop(struct status *slide_status);   /* 停止模式处理 */
void mode_start(struct status *slide_status);  /* 启动模式处理 */
void mode_run(struct status *slide_status);    /* 运行模式处理 */
void mode_idle(struct status *slide_status);   /* 待机模式处理 */

/* 状态机主函数 */
void state_machine(struct status *slide_status);

/* 用户界面/交互线程 */
void *ihm(void *arg);  /* IHM = Interface Homme-Machine (人机交互界面) */

#endif
