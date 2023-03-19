#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

/* TIM2 */
#define TARGET_FREQ 1000 // 目标频率

#define SYS_CLOCK_FREQ 72000000       // APB2 定时器频率
#define FCK_FREQ (SYS_CLOCK_FREQ / 1) // 定时器有PLL补偿所以和APB1频率一样
#define CKCNT_FREQ 36000000           // 36 Mhz
#define TIM2_ARR (CKCNT_FREQ / TARGET_FREQ - 1)

void GPIO_PWM_init(void);
void TIM2_PWM_init(void);
void TIM1_PWM_init(void);
#endif

