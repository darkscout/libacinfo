#ifndef BASIC_CONVERT_H
#define BASIC_CONVERT_H 1

#include <sys/types.h>

//#define AQUASTREAMXT_EDGES_FAN 8
//#define AQUASTREAMXT_EDGES_FLOW 8
#define AQUASTREAMXT_IMP_FLOW 256 // 0x100

double convert2temp(u_int16_t value);
double convert2flow(u_int32_t value, int edges, int calibrationImpulses);
double convert2fanrpm(u_int32_t value, int edges);

double convert2pump_frequency(u_int32_t value);
double convert2frequency_limiter(u_int32_t value);

double convert2VDD(u_int32_t value);
double convert2FanVoltage(u_int32_t value);

double convert2scalepercent(u_int16_t value);

#endif
