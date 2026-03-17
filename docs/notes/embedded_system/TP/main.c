#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mode.h"

int main(void)
{
    struct status slide_status;
    pthread_t ihm_thread;

    memset(&slide_status, 0, sizeof(slide_status));

    slide_status.des_pos = 0.0f;
    slide_status.des_vel = 0.0f;
    slide_status.act_pos = 0.0f;
    slide_status.act_vel = 0.0f;
    slide_status.acc_x = 0.0f;
    slide_status.acc_y = 0.0f;
    slide_status.acc_z = 0.0f;
    slide_status.mode = IDLE;
    slide_status.lim_left = 0;
    slide_status.lim_right = 0;
    slide_status.homed = 0;

    pthread_mutex_init(&slide_status.access, NULL);

    if (pthread_create(&ihm_thread, NULL, ihm, (void *)&slide_status) != 0) {
        perror("pthread_create");
        pthread_mutex_destroy(&slide_status.access);
        return 1;
    }

    usleep(300000);
    state_machine(&slide_status);

    pthread_join(ihm_thread, NULL);
    pthread_mutex_destroy(&slide_status.access);
    return 0;
}
