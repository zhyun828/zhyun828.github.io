#ifndef EQEP_H
#define EQEP_H

#define EQEP1   "/sys/devices/ocp.2/48302000.epwmss/48302180.eqep"

#define POSITION "position"
#define MODE     "mode"
#define PERIOD   "period"
#define ENABLED  "enabled"

void set_eqep(const char *dev, const char *reg, int val);
int  get_eqep(const char *dev, const char *reg);

#endif
