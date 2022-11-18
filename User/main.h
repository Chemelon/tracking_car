#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"
/* 引脚不够用初始化成上拉下拉作为VCC GND用 */
#define GND5_PORT       GPIOB
#define GND5_PIN        GPIO_Pin_10

#define GND6_PORT       GPIOB
#define GND6_PIN        GPIO_Pin_11

#define GND12_PORT      GPIOA
#define GND12_PIN       GPIO_Pin_9

#define GND18_PORT      GPIOA
#define GND18_PIN       GPIO_Pin_10

#define VDD17_PORT      GPIOA
#define VDD17_PIN       GPIO_Pin_11

/* 信号线引脚 */
#define LCD_DC_PORT             GPIOB
#define LCD_DC_PIN              GPIO_Pin_0

#define LCD_CS_PORT             GPIOA
#define LCD_CS_PIN              GPIO_Pin_4

#define LCD_SCL_PORT            GPIOA
#define LCD_SCL_PIN             GPIO_Pin_5

#define LCD_SDA_PORT            GPIOA
#define LCD_SDA_PIN             GPIO_Pin_7

#define LCD_RST_PORT            GPIOA
#define LCD_RST_PIN             GPIO_Pin_8

#define TP_SCL_PORT             GPIOB
#define TP_SCL_PIN              GPIO_Pin_13

#define TP_SDA_PORT             GPIOB
#define TP_SDA_PIN              GPIO_Pin_15

#define TP_RST_PORT             GPIOB
#define TP_RST_PIN              GPIO_Pin_12

#define TP_INT_PORT             GPIOA
#define TP_INT_PIN              GPIO_Pin_3

void delay_ms(uint32_t ms);

#endif
