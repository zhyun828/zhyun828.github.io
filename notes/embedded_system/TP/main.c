/*
 * main.c - 线性滑轨控制系统主程序
 * 创建两个线程：
 * 1. 状态机线程 - 执行系统控制逻辑
 * 2. 用户界面线程 - 处理用户输入命令
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mode.h"

/*
 * main - 程序入口
 * 返回值: 0 表示正常退出
 *
 * 执行流程：
 * 1. 初始化系统状态结构体
 * 2. 创建用户界面(IHM)线程
 * 3. 启动状态机控制循环
 * 4. 等待IHM线程终止
 * 5. 释放资源并退出
 */
int main(void)
{
    struct status slide_status;
    pthread_t ihm_thread;

    /* 清空系统状态结构体 */
    memset(&slide_status, 0, sizeof(slide_status));

    /* 初始化各状态字段为默认值 */
    slide_status.des_pos = 0.0f;       /* 期望位置 = 0 */
    slide_status.des_vel = 0.0f;       /* 期望速度 = 0 */
    slide_status.act_pos = 0.0f;       /* 实际位置 = 0 */
    slide_status.act_vel = 0.0f;       /* 实际速度 = 0 */
    slide_status.acc_x = 0.0f;         /* X加速度 = 0 */
    slide_status.acc_y = 0.0f;         /* Y加速度 = 0 */
    slide_status.acc_z = 0.0f;         /* Z加速度 = 0 */
    slide_status.mode = IDLE;          /* 初始模式为待机 */
    slide_status.lim_left = 0;         /* 左极限未触发 */
    slide_status.lim_right = 0;        /* 右极限未触发 */
    slide_status.homed = 0;            /* 未归零 */

    /* 初始化互斥锁(用于线程间同步) */
    pthread_mutex_init(&slide_status.access, NULL);

    /* 创建用户界面线程 */
    if (pthread_create(&ihm_thread, NULL, ihm, (void *)&slide_status) != 0) {
        perror("pthread_create");
        pthread_mutex_destroy(&slide_status.access);
        return 1;
    }

    /* 等待IHM线程初始化(300ms延迟) */
    usleep(300000);
    
    /* 启动状态机控制循环(主线程运行) */
    state_machine(&slide_status);

    /* 等待IHM线程终止 */
    pthread_join(ihm_thread, NULL);
    
    /* 清理资源：销毁互斥锁 */
    pthread_mutex_destroy(&slide_status.access);
    
    return 0;
}
