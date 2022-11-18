#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

#define PWM_OUT1_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT1_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT1_PORT GPIOA
#define PWM_OUT1_PIN GPIO_Pin_1

#define PWM_OUT2_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT2_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT2_PORT GPIOA
#define PWM_OUT2_PIN GPIO_Pin_2

#define PWM_SERVO_PERICMD RCC_APB2PeriphClockCmd
#define PWM_SERVO_PERIPH RCC_APB2Periph_GPIOA
#define PWM_SERVO_PORT GPIOA
#define PWM_SERVO_PIN GPIO_Pin_11

void GPIO_PWM_init(void);
void TIM2_PWM_init(void);
void TIM1_PWM_init(void);
#endif

