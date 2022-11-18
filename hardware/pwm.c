#include "pwm.h"

/**
 * @brief 初始化GPIO为复用推挽输出 用于输出驱动电机和舵机的
 * 
 */
void GPIO_PWM_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    /* 开启外设时钟 */
    PWM_OUT1_PERICMD(PWM_OUT1_PERIPH, ENABLE);
    PWM_OUT2_PERICMD(PWM_OUT2_PERIPH, ENABLE);
    PWM_SERVO_PERICMD(PWM_SERVO_PERIPH, ENABLE);
    /* 初始化TIM2的四个channel 复用推挽输出 */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Pin = PWM_OUT1_PIN;
    GPIO_Init(PWM_OUT1_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = PWM_OUT2_PIN;
    GPIO_Init(PWM_OUT2_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = PWM_SERVO_PIN;
    GPIO_Init(PWM_SERVO_PORT, &gpio_init_struct);
}

/**
 * @brief 初始化TIM2为PWM输出模式 开启所有输出通道 频率=1khz 
 * 
 */
void TIM2_PWM_init(void)
{
#define TARGET_FREQ 1000 // 目标频率

#define SYS_CLOCK_FREQ 72000000       // APB2 定时器频率
#define FCK_FREQ (SYS_CLOCK_FREQ / 1) // 定时器有PLL补偿所以和APB1频率一样
#define CKCNT_FREQ 100000             // 100khz
    /* 开启外设时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /*100 KHz*/
    TIM2->PSC = (FCK_FREQ / CKCNT_FREQ - 1);
    /* 1 kHz (100)*/
    TIM2->ARR = CKCNT_FREQ / TARGET_FREQ;
    /* 将四个通道分别配置成PWM输出模式 OCM = 0X110*/
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
    TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    /* 占空比 50% */
    TIM2->CCR1 = 50;
    TIM2->CCR2 = 50;
    TIM2->CCR3 = 50;
    TIM2->CCR4 = 50;
    /* 使能每个通道的比较功能 */
    TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    /* 开启TIM2时钟 */
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 初始化TIM1为PWM输出模式 开启所输出通道4 频率=50hz
 * 
 */
void TIM1_PWM_init(void)
{
    /* 开启外设时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    /*10 KHz*/
    TIM1->PSC = (FCK_FREQ / 10000 - 1);
    /* 50Hz (200)*/
    TIM1->ARR = 10000 / 50;
    /* 通道4 PWM输出 */
    TIM1->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    /* 占空比 50% */
    TIM1->CCR4 = 100;
    /* 使能比较功能 */
    TIM1->CCER |= TIM_CCER_CC4E;
    /* Main output enable */
    TIM1->BDTR |= TIM_BDTR_MOE;
    /* 开启TIM1时钟 */
    TIM1->CR1 |= TIM_CR1_CEN;
}

