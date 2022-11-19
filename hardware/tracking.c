#include "tracking.h"

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

void GPIO_tracker_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    EXTI_InitTypeDef exti_init_struct;
    TRACKER1_PERICMD(TRACKER1_PERIPH, ENABLE);
    TRACKER2_PERICMD(TRACKER2_PERIPH, ENABLE);
    TRACKER3_PERICMD(TRACKER3_PERIPH, ENABLE);
    TRACKER4_PERICMD(TRACKER4_PERIPH, ENABLE);
    TRACKER5_PERICMD(TRACKER5_PERIPH, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Pin = TRACKER1_PIN;
    /* 初始化上拉输入 */
    GPIO_Init(TRACKER1_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = TRACKER2_PIN;
    GPIO_Init(TRACKER2_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = TRACKER3_PIN;
    GPIO_Init(TRACKER3_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = TRACKER4_PIN;
    GPIO_Init(TRACKER4_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = TRACKER5_PIN;
    GPIO_Init(TRACKER5_PORT, &gpio_init_struct);
    /*配置AFIO选择中断线*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
    /* 初始化外部中断 */
    exti_init_struct.EXTI_Line = EXTI_Line4;
    exti_init_struct.EXTI_LineCmd = ENABLE;
    exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&exti_init_struct);
    exti_init_struct.EXTI_Line = EXTI_Line5;
    EXTI_Init(&exti_init_struct);
    exti_init_struct.EXTI_Line = EXTI_Line6;
    EXTI_Init(&exti_init_struct);
    exti_init_struct.EXTI_Line = EXTI_Line7;
    EXTI_Init(&exti_init_struct);
    exti_init_struct.EXTI_Line = EXTI_Line8;
    EXTI_Init(&exti_init_struct);
}
