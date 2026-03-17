#include <stdio.h>
#include <stdlib.h>
#include "eqep.h"

void set_eqep(const char *dev, const char *reg, int val)
{
    char path[256];
    FILE *fp;

    snprintf(path, sizeof(path), "%s/%s", dev, reg);
    fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return;
    }

    fprintf(fp, "%d", val);
    fclose(fp);
}

int get_eqep(const char *dev, const char *reg)
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
