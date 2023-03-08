#include "encoder.h"

void encoder_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_TimeBaseInitTypeDef tim_timbaseinit_struct;
    TIM_ICInitTypeDef tim_icinit_struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
    
    /* TODO:这样写不能使能TIM4?????? */
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);

    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &gpio_init_struct);

    TIM_TimeBaseStructInit(&tim_timbaseinit_struct);
    tim_timbaseinit_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_timbaseinit_struct.TIM_CounterMode = TIM_CounterMode_Up;
    tim_timbaseinit_struct.TIM_Prescaler = 0x00;
    tim_timbaseinit_struct.TIM_Period = 0xffff;

    TIM_TimeBaseInit(TIM3, &tim_timbaseinit_struct);
    TIM_TimeBaseInit(TIM4, &tim_timbaseinit_struct);

    TIM_ICStructInit(&tim_icinit_struct);
    tim_icinit_struct.TIM_ICFilter = 10;

    TIM_ICInit(TIM3, &tim_icinit_struct);
    TIM_ICInit(TIM4, &tim_icinit_struct);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM3->CNT = 0;
    TIM4->CNT = 0;

    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

