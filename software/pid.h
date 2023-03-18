#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"

extern int32_t pid_integral;

typedef struct pid_type_int
{
    int32_t kp;
    int32_t ki;
    int32_t kd;
    int32_t max_p;
    int32_t max_n;
    int32_t i;
} pid_type_int;

int32_t positional_pid_int(pid_type_int *pid_this, int32_t target, int32_t real);
int32_t positional_pid_int_independent(pid_type_int *pid_this, int32_t target, int32_t real);

#endif

