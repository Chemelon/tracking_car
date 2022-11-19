#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"
#define MOTOR_PWMP_LEFT     (TIM2->CCR1)
#define MOTOR_PWMN_LEFT     (TIM2->CCR2)
#define MOTOR_PWMP_RIGHT     (TIM2->CCR3)
#define MOTOR_PWMN_RIGHT     (TIM2->CCR4)


/* 电机PWM输出基准值,此状态为小车以一个合适的速度前进 */
#define MOTOR_PWMBASE_LEFT  20
#define MOTOR_PWMBASH_RIGHT 20
/*  */

/**
 * @brief 前进
 * 
 * @param pwm_val 前进速度
 */
void motor_gostright(uint16_t speed);

#endif

