#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"
typedef struct pid
{
    float kp;
    float ki;
    float kd;
}pid_type;

/* 整数 */
typedef struct pid_uint
{
    uint32_t kp;
    uint32_t ki;
    uint32_t kd;
}pid_type_uint;

float positional_pid(pid_type *pid_this, float target, float real);
float incremental_pid(pid_type *pid_this, float target, float real);
uint32_t incremental_pid_uint(pid_type_uint *pid_this, uint32_t target, uint32_t real);
uint32_t positional_pid_uint(pid_type_uint *pid_this, uint32_t target, uint32_t real);

#endif