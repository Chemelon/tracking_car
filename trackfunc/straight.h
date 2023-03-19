#ifndef __STRAIGHT_H
#define __STRAIGHT_H
#include "usart.h"
#include "pid.h"

/* 直线循迹log开关 */
#define DEBUG_STRAIGHT 0

#if DEBUG_STRAIGHT
#define STRAIGHT_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define STRAIGHT_LOG(msg)
#endif
/* 直线时舵机角度 */
#define S_STRAIGHTWARD 90
/* 向右修正时舵机角度 */
#define S_RIGHTWARD 95
/* 向左修正时舵机角度 */
#define S_LEFTWARD 85
/* 向右修正时差速PWM值 即左比右快的值 CCMR 最大值36000*/
#define RIGHTWARD_ADD 2000
/* 向左修正时差速PWM值 即右比左快的值 */
#define LEFTWARD_ADD 2000

extern pid_type_int independent_pid;

void tracking_cross(void);
void tracking_cross_pid(void);
void tracking_straight_pid_s(uint16_t s);
void tracking_straightfast_pid_sm(uint16_t s, uint16_t m, int32_t basespeed);
void tracking_straight(void);
void tracking_straight_pid(void);
void tracking_final_pid(void);

#endif
