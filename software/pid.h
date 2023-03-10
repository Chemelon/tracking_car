#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"

typedef struct pid_type_uint
{
    uint32_t kp;
    uint32_t ki;
    uint32_t kd;
} pid_type_uint;


#endif