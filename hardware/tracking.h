#ifndef __TRACKING_H
#define __TRACKING_H
#include "stm32f10x.h"

#define TRACKER1_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER1_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER1_PORT GPIOB
#define TRACKER1_PIN GPIO_Pin_4

#define TRACKER2_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER2_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER2_PORT GPIOB
#define TRACKER2_PIN GPIO_Pin_5

#define TRACKER3_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER3_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER3_PORT GPIOB
#define TRACKER3_PIN GPIO_Pin_6

#define TRACKER4_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER4_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER4_PORT GPIOB
#define TRACKER4_PIN GPIO_Pin_7

#define TRACKER5_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER5_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER5_PORT GPIOB
#define TRACKER5_PIN GPIO_Pin_8

#endif