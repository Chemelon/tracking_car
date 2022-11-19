#include "tracking.h"

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
    gpio_init_struct.GPIO_Pin = TRACKER1_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
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
