#ifndef MODE_H
#define MODE_H

#include <pthread.h>

#define STOP  0
#define START 1
#define RUN   2
#define IDLE  3

struct status
{
    float des_pos;
    float des_vel;
    float act_pos;
    float act_vel;
    float acc_x;
    float acc_y;
    float acc_z;
    unsigned char mode;
    int lim_left;
    int lim_right;
    int homed;
    pthread_mutex_t access;
};

void mode_stop(struct status *slide_status);
void mode_start(struct status *slide_status);
void mode_run(struct status *slide_status);
void mode_idle(struct status *slide_status);
void state_machine(struct status *slide_status);
void *ihm(void *arg);

#endif
