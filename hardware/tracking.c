#include "tracking.h"
#include "usart.h"

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
#define GPIOB_IDR_BIT4 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 4 * 4))
#define GPIOB_IDR_BIT5 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 5 * 4))
#define GPIOB_IDR_BIT6 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 6 * 4))
#define GPIOB_IDR_BIT7 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 7 * 4))
#define GPIOB_IDR_BIT8 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 8 * 4))

volatile static tracker_type tracker_status = {tracker_resloved, 0, 0, 0, 0, 0, 0, 0};
volatile tracker_type *ptracker_status = &tracker_status;

/**
 * @brief 中断方式相关函数
 *
 */
#if !TRACKER_POLLING
/**
 * @brief 光电1中断服务函数
 *
 */
void EXTI4_IRQHandler(void)
{
    EXTI->PR = (EXTI->PR & EXTI_Line4) ? EXTI_Line4 : 0;
    /* 将光电管的状态保存至内存 */
    tracker_status.update = tracker_updated;
    tracker_status.tarcker1_status = GPIOB_IDR_BIT4;
    tracker_status.tarcker2_status = GPIOB_IDR_BIT5;
    tracker_status.tarcker3_status = GPIOB_IDR_BIT6;
    tracker_status.tarcker4_status = GPIOB_IDR_BIT7;
    tracker_status.tarcker5_status = GPIOB_IDR_BIT8;
    // Usart_SendString(USART1, "ext4\r\n");
}

/**
 * @brief 光电2-5中断服务函数
 *
 */
void EXTI9_5_IRQHandler(void)
{
    /* rc_w1 */
    EXTI->PR = (EXTI->PR & EXTI_Line5) ? EXTI_Line5 : 0;
    EXTI->PR = (EXTI->PR & EXTI_Line6) ? EXTI_Line6 : 0;
    EXTI->PR = (EXTI->PR & EXTI_Line7) ? EXTI_Line7 : 0;
    EXTI->PR = (EXTI->PR & EXTI_Line8) ? EXTI_Line8 : 0;
    /* 将光电管的状态保存至内存 */
    tracker_status.update = tracker_updated;
    tracker_status.tarcker1_status = GPIOB_IDR_BIT4;
    tracker_status.tarcker2_status = GPIOB_IDR_BIT5;
    tracker_status.tarcker3_status = GPIOB_IDR_BIT6;
    tracker_status.tarcker4_status = GPIOB_IDR_BIT7;
    tracker_status.tarcker5_status = GPIOB_IDR_BIT8;
    // Usart_SendString(USART1, "ext5-9\r\n");
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
    exti_init_struct.EXTI_LineCmd = ENABLE;
    exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti_init_struct.EXTI_Line = EXTI_Line4;
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
#endif

/**
 * @brief 轮询方式相关函数
 *
 */
#if TRACKER_POLLING
/**
 * @brief 中断频率为500hz 累计10次的采样总值 即实际控制处理频率约为50hz
 *
 */
void TIM3_IRQHandler(void)
{
    TIM3->SR = ~TIM_SR_UIF;
    tracker_status.tracker_cnt_it++;

    if (tracker_status.tracker_cnt_it < 11)
    // if (1)
    {
        /* 将光电管的状态保存至内存 */
        tracker_status.update = tracker_updated;
        tracker_status.tarcker1_status = GPIOB_IDR_BIT4;
        tracker_status.tarcker2_status = GPIOB_IDR_BIT5; // 1
        tracker_status.tarcker3_status = GPIOB_IDR_BIT6; // 1
        tracker_status.tarcker4_status = GPIOB_IDR_BIT7; // 1
        tracker_status.tarcker5_status = GPIOB_IDR_BIT8;
        /* 将光电管的状态保存至内存 */
        tracker_status.tracker_sum_signed += GPIOB_IDR_BIT5 - GPIOB_IDR_BIT7;
    }
    else
    {
        /* 关闭计数器 */
        TIM3->CR1 &= ~TIM_CR1_CEN;
        /* 通知更新 */
        tracker_status.update = tracker_updated;
    }
}

void GPIO_tracker_init_polling(void)
{
    GPIO_InitTypeDef gpio_init_struct;

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
}

/**
 * @brief 初始化定时器3 周期性扫描光电状态
 *
 */
void TIM3_tracker_init_polling(void)
{
#define TARGET_FREQ 500 // 目标频率

#define SYS_CLOCK_FREQ 72000000       // APB1 定时器频率
#define FCK_FREQ (SYS_CLOCK_FREQ / 1) // 定时器有PLL补偿所以和APB1频率一样
#define CKCNT_FREQ 100000             // 100 000 hz
    /* 开启外设时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /*10000 KHz*/
    TIM3->PSC = (FCK_FREQ / CKCNT_FREQ - 1);
    /* 清空计数器 */
    TIM3->CNT = 0;
    /* 500 Hz (200)*/
    TIM3->ARR = CKCNT_FREQ / TARGET_FREQ - 1;
    /* 开启更新中断 */
    TIM3->DIER |= TIM_DIER_UIE;
    /* 开启TIM3时钟 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void NVIC_tracker_init_polling(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
#endif

void tracker_resume(void)
{
    /* 清除累计值 */
    ptracker_status->tracker_sum_signed = 0;
    /* 复位计数值 */
    ptracker_status->tracker_cnt_it = 0;
    /* 更新状态 */
    ptracker_status->update = tracker_resloved;
    /* 复位CNT */
    TIM3->CNT = 0;
    /* 使能定时器 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void USART_sendinfo(void)
{
    if (ptracker_status->update == tracker_updated)
    {
        Usart_SendString(USART1, ((ptracker_status->tarcker1_status) ? "1 " : "0 "));
        Usart_SendString(USART1, ((ptracker_status->tarcker2_status) ? "1 " : "0 "));
        Usart_SendString(USART1, ((ptracker_status->tarcker3_status) ? "1 " : "0 "));
        Usart_SendString(USART1, ((ptracker_status->tarcker4_status) ? "1 " : "0 "));
        Usart_SendString(USART1, ((ptracker_status->tarcker5_status) ? "1 " : "0 "));
        /* 取得累计值 */
        printf("total: %d \r\n", ptracker_status->tracker_sum_signed);
    }
}

int32_t caclu_pid(void)
{
    /* 上个误差值以及上上个误差值 */
    static int err_priv1 = 0, err_priv2 = 0;
    int err_curr;
    int pid_delta;
    /* tracker_sum_signed 是一个-10~10 的数 */
    err_curr = tracker_status.tracker_sum_signed;

    pid_delta = (KP * (err_curr - err_priv1)) + (KI * (err_curr)) + (KD * (err_curr - 2 * err_priv1 + err_priv2));

    err_priv2 = err_priv1;
    err_priv1 = err_curr;

    return pid_delta;
}

/**
 * @brief 实现循迹
 *
 */
void tracking(void)
{
}
