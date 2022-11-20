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

/* 位带操作 */
#define GPIOB_IDR_BIT4 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000)* 32 + 4 * 4))
#define GPIOB_IDR_BIT5 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000)* 32 + 5 * 4))
#define GPIOB_IDR_BIT6 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000)* 32 + 6 * 4))
#define GPIOB_IDR_BIT7 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000)* 32 + 7 * 4))
#define GPIOB_IDR_BIT8 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000)* 32 + 8 * 4))

typedef struct tracker_status_struct{
    volatile tracker_update_status_type update;
    volatile uint32_t tarcker1_status;
    volatile uint32_t tarcker2_status;
    volatile uint32_t tarcker3_status;
    volatile uint32_t tarcker4_status;
    volatile uint32_t tarcker5_status;
}tracker_type;

volatile static tracker_type tracker_status;

/**
 * @brief 光电1中断服务函数
 * 
 */
void EXTI4_IRQHandler(void)
{
    EXTI->PR = (EXTI->PR & EXTI_Line4)?EXTI_Line4:0;
    /* 将光电管的状态保存至内存 */
    tracker_status.update = tracker_updated;
    tracker_status.tarcker1_status = GPIOB_IDR_BIT4;
    tracker_status.tarcker2_status = GPIOB_IDR_BIT5;
    tracker_status.tarcker3_status = GPIOB_IDR_BIT6;
    tracker_status.tarcker4_status = GPIOB_IDR_BIT7;
    tracker_status.tarcker5_status = GPIOB_IDR_BIT8;
}

/**
 * @brief 光电2-5中断服务函数
 * 
 */
void EXTI9_5_IRQHandler(void)
{
    /* rc_w1 */
    EXTI->PR = (EXTI->PR & EXTI_Line5)?EXTI_Line5:0;
    EXTI->PR = (EXTI->PR & EXTI_Line6)?EXTI_Line6:0;
    EXTI->PR = (EXTI->PR & EXTI_Line7)?EXTI_Line7:0;
    EXTI->PR = (EXTI->PR & EXTI_Line8)?EXTI_Line8:0;
    /* 将光电管的状态保存至内存 */
    tracker_status.update = tracker_updated;
    tracker_status.tarcker1_status = GPIOB_IDR_BIT4;
    tracker_status.tarcker2_status = GPIOB_IDR_BIT5;
    tracker_status.tarcker3_status = GPIOB_IDR_BIT6;
    tracker_status.tarcker4_status = GPIOB_IDR_BIT7;
    tracker_status.tarcker5_status = GPIOB_IDR_BIT8;
    
}

/**
 * @brief 初始化光电的端口为外部中断
 *
 */
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

/**
 * @brief 配置外部中断优先级
 *
 */
void NVIC_tracker_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 实现循迹功能
 * 
 */
void tracking(void)
{

}