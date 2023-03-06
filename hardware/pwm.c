#include "pwm.h"

#define PWM_OUT1_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT1_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT1_PORT GPIOA
#define PWM_OUT1_PIN GPIO_Pin_0

#define PWM_OUT2_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT2_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT2_PORT GPIOA
#define PWM_OUT2_PIN GPIO_Pin_1

#define PWM_OUT3_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT3_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT3_PORT GPIOA
#define PWM_OUT3_PIN GPIO_Pin_2

#define PWM_OUT4_PERICMD RCC_APB2PeriphClockCmd
#define PWM_OUT4_PERIPH RCC_APB2Periph_GPIOA
#define PWM_OUT4_PORT GPIOA
#define PWM_OUT4_PIN GPIO_Pin_3

#define PWM_SERVO_PERICMD RCC_APB2PeriphClockCmd
#define PWM_SERVO_PERIPH RCC_APB2Periph_GPIOA
#define PWM_SERVO_PORT GPIOA
#define PWM_SERVO_PIN GPIO_Pin_11

/**
 * @brief 初始化GPIO为复用推挽输出
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

    gpio_init_struct.GPIO_Pin = PWM_OUT3_PIN;
    GPIO_Init(PWM_OUT3_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = PWM_OUT4_PIN;
    GPIO_Init(PWM_OUT4_PORT, &gpio_init_struct);

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
#define CKCNT_FREQ 2000000           // 2 Mhz
    /* 开启外设时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /*10000 KHz*/
    TIM2->PSC = (FCK_FREQ / CKCNT_FREQ - 1);
    /* 1 kHz (0~2000)*/
    TIM2->ARR = CKCNT_FREQ / TARGET_FREQ - 1;
    /* 将四个通道分别配置成PWM输出模式 OCM = 0X110*/
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
    TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    /* 占空比 0% 初始化为停车状态*/
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    TIM2->CCR4 = 0;
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
    /*1000 KHz*/
#define TIM1_CLKCNT_FREQ 1000000
    /* 开启外设时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    /*1000 KHz*/
    TIM1->PSC = (FCK_FREQ / TIM1_CLKCNT_FREQ - 1);
    /* 50Hz (20000)*/
    TIM1->ARR = TIM1_CLKCNT_FREQ / 50 - 1;
    /* 通道4 PWM输出 */
    TIM1->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    /* 占空比 0% */
    TIM1->CCR4 = 0;
    /* 使能比较功能 */
    TIM1->CCER |= TIM_CCER_CC4E;
    /* Main output enable */
    TIM1->BDTR |= TIM_BDTR_MOE;
    /* 开启TIM1时钟 */
    TIM1->CR1 |= TIM_CR1_CEN;
}
