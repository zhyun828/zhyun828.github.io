#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"

void set_gpio(const char *dev, const char *reg, int val)
{
    char path[256];
    FILE *fp;

    snprintf(path, sizeof(path), "%s/%s", dev, reg);
    fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return;
    }

    if (strcmp(reg, DIRECTION) == 0) {
        fprintf(fp, "%s", val == 0 ? "out" : "in");
    } else {
        fprintf(fp, "%d", val);
    }

    fclose(fp);
}

int get_gpio(const char *dev, const char *reg)
{
    char path[256];
    FILE *fp;
    int val = 0;

    snprintf(path, sizeof(path), "%s/%s", dev, reg);
    fp = fopen(path, "r");
    if (!fp) {
        perror(path);
        return -1;
    }

    fscanf(fp, "%d", &val);
    fclose(fp);
    return val;
}
