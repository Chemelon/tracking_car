#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"

#define M_PWM_LEFT_P (TIM2->CCR1)
#define M_PWM_LEFT_N (TIM2->CCR2)
#define M_PWM_RIGHT_P (TIM2->CCR3)
#define M_PWM_RIGHT_N (TIM2->CCR4)

/* 设置TIM1 CCR4 改变占空比从而控制舵机 */
#define S_PWM_CCR (TIM1->CCR4)

/* CCR 单位为us */
#define S_PWM_CCR_0 1           // 0 度时PWM占空比
#define S_PWM_CCR_180 2         //180 度时PWM占空比

/* 电机PWM输出基准值,此状态为小车以一个合适的速度前进 */
#define PWMBASE_LEFT 3400
#define PWMBASH_RIGHT 3500

void servo_set_dutyclcle(uint16_t CCR_value);
void servo_setangle(uint16_t angle);
void motor_setforward_left(uint16_t pwm_ccr_val);
void motor_setforward_right(uint16_t pwm_ccr_val);
void motor_setbackward_left(uint16_t pwm_ccr_val);
void motor_setbackward_right(uint16_t pwm_ccr_val);
void motor_setbrake_left(void);
void motor_setbrake_right(void);
void gostraight(uint16_t speed);
void goback(uint16_t speed);
void stop(void);

#endif
