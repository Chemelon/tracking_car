#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"

/* 电机PWM输出基准值,此状态为小车以一个合适的速度前进 */
#define MOTOR_PWMBASE_LEFT  3400
#define MOTOR_PWMBASH_RIGHT 3500

/* 舵机直线方向时的占空比 */
/* 0度 高电平0.5ms = 500 us CCR 每加一高电平时间增加1us */
#define K_SERVO_STRBASE 1290

/* 舵机转向参数 用于将角度换算成PWM占空比来控制舵机 */
#define K_SERVO_TURNLEFT (9)
#define K_SERVO_TURNRIGHT (9)

/* 决定是否使用减速电机差速辅助转弯 1 开启 0 关闭 */
#define DIFFAL_SUP 0
/* 此值越大 转弯幅度越大 */
#define K_MOTOR_TURNLEFT 1000
#define K_MOTOR_TURNRIGHT 1000
/* 转弯速度 此值越大 转弯速度越快 */
#define K_DIFFAL_SUP_SPEED 1500
#define k_DIFFAL_SUP_SPEED 1500


void gostraight(uint16_t speed);
void goback(uint16_t speed);
void servo_turnleft(uint16_t angle);
void servo_turnright(uint16_t angle);
void turnleft(uint16_t angle);
void turnright(uint16_t angle);
void stop(void);

#endif

