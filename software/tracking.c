#include "tracking.h"
#include "usart.h"
#include "FunctionList.h"
#include "pid.h"
#include "motor.h"
#include "SysTick.h"
#include "roundabout.h"

#define TRACKER1_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER1_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER1_PORT GPIOB
#define TRACKER1_PIN GPIO_Pin_3

#define TRACKER2_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER2_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER2_PORT GPIOB
#define TRACKER2_PIN GPIO_Pin_4

#define TRACKER3_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER3_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER3_PORT GPIOB
#define TRACKER3_PIN GPIO_Pin_5

#define TRACKER4_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER4_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER4_PORT GPIOB
#define TRACKER4_PIN GPIO_Pin_8

#define TRACKER5_PERICMD RCC_APB2PeriphClockCmd
#define TRACKER5_PERIPH RCC_APB2Periph_GPIOB
#define TRACKER5_PORT GPIOB
#define TRACKER5_PIN GPIO_Pin_9

/* 位带操作 */
#define GPIOB_IDR_BIT3 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 3 * 4))
#define GPIOB_IDR_BIT4 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 4 * 4))
#define GPIOB_IDR_BIT5 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 5 * 4))
#define GPIOB_IDR_BIT8 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 8 * 4))
#define GPIOB_IDR_BIT9 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 9 * 4))

#define TRACKER_PIN1 GPIOB_IDR_BIT9
#define TRACKER_PIN2 GPIOB_IDR_BIT8
#define TRACKER_PIN3 GPIOB_IDR_BIT5
#define TRACKER_PIN4 GPIOB_IDR_BIT4
#define TRACKER_PIN5 GPIOB_IDR_BIT3

volatile static tracker_type tracker_status = {status_resloved, 0, 0, 0, 0, 0, 0, 0};
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
    tracker_status.update = status_updated;
    tracker_status.tarcker1 = TRACKER_PIN1;
    tracker_status.tarcker2 = TRACKER_PIN2;
    tracker_status.tarcker3 = TRACKER_PIN3;
    tracker_status.tarcker4 = TRACKER_PIN4;
    tracker_status.tarcker5 = TRACKER_PIN5;
    // Usart_SendString(DEBUG_USARTx, "ext4\r\n");
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
    tracker_status.update = status_updated;
    tracker_status.tarcker1 = GPIOB_IDR_BIT5;
    tracker_status.tarcker2 = GPIOB_IDR_BIT7;
    tracker_status.tarcker3 = GPIOB_IDR_BIT6;
    tracker_status.tarcker4 = GPIOB_IDR_BIT4;
    tracker_status.tarcker5 = GPIOB_IDR_BIT8;
    // Usart_SendString(DEBUG_USARTx, "ext5-9\r\n");
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
typedef struct trackerfilter
{
    uint32_t filterbuf1[5];
    uint32_t filterbuf2[5];
    uint32_t filterbuf3[5];
    uint32_t filterbuf4[5];
    uint32_t filterbuf5[5];

} trackerfilter_type;

trackerfilter_type trackerfilter = {0};

/**
 * @brief 中断频率为500hz 累计10次的采样总值 即实际控制处理频率约为50hz
 *
 */
void TIM3_IRQHandler(void)
{
    volatile uint32_t *pfiltered = &tracker_status.tarcker1;
    volatile uint32_t *pfilersrc = (volatile uint32_t *)&trackerfilter;
    uint8_t temp;
    TIM3->SR = ~TIM_SR_UIF;
    if (tracker_status.tracker_cnt_it == POLLING_CNT)
    {
        /* 关闭计数器 */
        TIM3->CR1 &= ~TIM_CR1_CEN;

        tracker_status.tracker_cnt_it = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (*pfilersrc)
                {
                    temp++;
                }
                pfilersrc++;
            }
            if (temp > 4)
            {
                *pfiltered = 1;
            }
            else
            {
                *pfiltered = 0;
            }
            pfiltered++;
        }
        tracker_status.tracker_sum = 1800 + (300 * tracker_status.tarcker2) - (300 * tracker_status.tarcker4);

        /* 通知更新 */
        tracker_status.update = status_updated;
        return;
    }
    {
        /* 将光电管的状态保存至内存 */
        // tracker_status.update = status_updated;
        trackerfilter.filterbuf1[tracker_status.tracker_cnt_it] = TRACKER_PIN1;
        trackerfilter.filterbuf2[tracker_status.tracker_cnt_it] = TRACKER_PIN2;
        trackerfilter.filterbuf3[tracker_status.tracker_cnt_it] = TRACKER_PIN3;
        trackerfilter.filterbuf4[tracker_status.tracker_cnt_it] = TRACKER_PIN4;
        trackerfilter.filterbuf5[tracker_status.tracker_cnt_it] = TRACKER_PIN5;
        /* 将光电管的状态保存至内存 */
        tracker_status.tracker_cnt_it++;
    }
}

/**
 * @brief 中断频率为1000hz 累计10次的采样总值 即实际控制处理频率约为100hz
 *
 */
#if 0
void TIM1_UP_IRQHandler(void)
{
    volatile uint32_t * pfiltered = &tracker_status.tarcker1;
    volatile uint32_t * pfilersrc = (volatile uint32_t *)&trackerfilter;
    uint8_t temp = 0;
    TIM1->SR = ~TIM_SR_UIF;
    if (tracker_status.tracker_cnt_it == POLLING_CNT)
    {
        /* 关闭计数器 */
        TIM1->CR1 &= ~TIM_CR1_CEN;

        tracker_status.tracker_cnt_it = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if(*pfilersrc++)
                {
                    temp++;
                }
            }
            if(temp > 4)
            {
                *pfiltered = 1;
            }
            else
            {
                *pfiltered = 0;
            }
            temp = 0;
            pfiltered++;
        }
        tracker_status.tracker_sum = 1800 + (300 * tracker_status.tarcker2) - (300 * tracker_status.tarcker4);

        /* 通知更新 */
        tracker_status.update = status_updated;
        return;
    }
    {
        /* 将光电管的状态保存至内存 */
        // tracker_status.update = status_updated;
        trackerfilter.filterbuf1[tracker_status.tracker_cnt_it] = TRACKER_PIN1;
        trackerfilter.filterbuf2[tracker_status.tracker_cnt_it] = TRACKER_PIN2;
        trackerfilter.filterbuf3[tracker_status.tracker_cnt_it] = TRACKER_PIN3;
        trackerfilter.filterbuf4[tracker_status.tracker_cnt_it] = TRACKER_PIN4;
        trackerfilter.filterbuf5[tracker_status.tracker_cnt_it] = TRACKER_PIN5;
        /* 将光电管的状态保存至内存 */
        tracker_status.tracker_cnt_it++;
    }
}
#else
void TIM1_UP_IRQHandler(void)
{

    TIM1->SR = ~TIM_SR_UIF;
    if (tracker_status.update == status_resloved)
    {
        {
            tracker_status.tarcker1 = TRACKER_PIN1;
            tracker_status.tarcker2 = TRACKER_PIN2; // 1
            tracker_status.tarcker3 = TRACKER_PIN3; // 1
            tracker_status.tarcker4 = TRACKER_PIN4; // 1
            tracker_status.tarcker5 = TRACKER_PIN5;
        }
        tracker_status.tracker_cnt_it++;
        if ((tracker_status.tracker_cnt_it > POLLING_CNT))
        {
            if (TRACKER_PIN3)
            {
                tracker_status.tracker_sum = (TRACKER_PIN4 * 100) - (TRACKER_PIN2 * 100);
            }
            else
            {
                tracker_status.tracker_sum = (TRACKER_PIN4 * 200) - (TRACKER_PIN2 * 200);
                tracker_status.tracker_sum += TRACKER_PIN5 * 300 - TRACKER_PIN1 * 300; 
                
            }
            /* 关闭计数器 */
            // TIM1->CR1 &= ~TIM_CR1_CEN;
            /* 复位计数值 */
            ptracker_status->tracker_cnt_it = 0;
            /* 通知更新 */
            tracker_status.update = status_updated;
            return;
        }
    }
}
#endif

void GPIO_tracker_init_polling(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    TRACKER1_PERICMD(TRACKER1_PERIPH, ENABLE);
    TRACKER2_PERICMD(TRACKER2_PERIPH, ENABLE);
    TRACKER3_PERICMD(TRACKER3_PERIPH, ENABLE);
    TRACKER4_PERICMD(TRACKER4_PERIPH, ENABLE);
    TRACKER5_PERICMD(TRACKER5_PERIPH, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

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
#define TARGET_FREQ POLLING_FREQ // 目标频率

#define SYS_CLOCK_FREQ 72000000       // APB1 定时器频率
#define FCK_FREQ (SYS_CLOCK_FREQ / 1) // 定时器有PLL补偿所以和APB1频率一样
#define CKCNT_FREQ 100000             // 100 000 hz
    /* 开启外设时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /*10000 KHz*/
    TIM3->PSC = (FCK_FREQ / CKCNT_FREQ - 1);
    /* 清空计数器 */
    TIM3->CNT = 0;
    /* 500 Hz */
    TIM3->ARR = CKCNT_FREQ / TARGET_FREQ - 1;
    /* 开启更新中断 */
    TIM3->DIER |= TIM_DIER_UIE;
    /* 开启TIM3时钟 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 初始化定时器1 周期性扫描光电状态
 *
 */
void TIM1_tracker_init_polling(void)
{
#define TIM1_TARGET_FREQ POLLING_FREQ // 目标频率
#define FCK_FREQ (SYS_CLOCK_FREQ / 1) // 定时器有PLL补偿所以和APB1频率一样
#define TIM1_CKCNT_FREQ 100000        // 100 000 hz
#define TIM1_ARR (TIM1_CKCNT_FREQ / TIM1_TARGET_FREQ - 1)
    /* 开启外设时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    /*100000 Hz*/
    TIM1->PSC = (FCK_FREQ / TIM1_CKCNT_FREQ - 1);
    /* 清空计数器 */
    TIM1->CNT = 0;
    /* 0~99 */
    TIM1->ARR = TIM1_ARR;
    /* 开启更新中断 */
    TIM1->DIER |= TIM_DIER_UIE;
    /* 开启TIM1时钟 */
    TIM1->CR1 |= TIM_CR1_CEN;
}

void NVIC_tracker_init_polling(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置TIM3为中断源 */
    // NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
#endif

void tracking_resume(void)
{
#if TRACKER_POLLING
    /* 清除累计值 */
    ptracker_status->tracker_sum = 1000;
    /* 更新状态 */
    ptracker_status->update = status_resloved;
    /* 复位CNT */
    // TIM1->CNT = 0;
    /* 使能定时器 */
    TIM1->CR1 |= TIM_CR1_CEN;
#else
    ptracker_status->update = status_resloved;
#endif
}

/**
 * @brief 通过串口发送光电管状态 DEBUG 用
 *
 */
void tracker_sendinfo(void)
{
    if (ptracker_status->update == status_updated)
    {
        Usart_SendString(DEBUG_USARTx, (TRACKER1_STATUS ? "1 " : "0 "));
        Usart_SendString(DEBUG_USARTx, (TRACKER2_STATUS ? "1 " : "0 "));
        Usart_SendString(DEBUG_USARTx, (TRACKER3_STATUS ? "1 " : "0 "));
        Usart_SendString(DEBUG_USARTx, (TRACKER4_STATUS ? "1 " : "0 "));
        Usart_SendString(DEBUG_USARTx, (TRACKER5_STATUS ? "1 " : "0 "));
        /* 取得累计值 */
        Usart_SendHalfWord(DEBUG_USARTx, 0x0d0a);
        //printf("sum: %d \r\n", ptracker_status->tracker_sum);
        /* 更新状态 */
        // tracking_resume();
    }
}

/**
 * @brief 状态切换器 按照顺序切换状态
 *
 */
void stateswitcher(void)
{
    static uint8_t status_list[STATE_NUM] = STATUS_MEMBER;
    for (int i = 0; i < STATE_NUM; i++)
    {
        FunList_Call(status_list[i]);
    }
    stop();
    while (1)
        ;
}

void func_caller(void)
{    
    tracking_straight_pid_s(1000);
    tracking_left();

    pid_integral = -3500; 
    tracking_cross_pid();
    //Delay_ms(40);
    tracking_crossright();
    //DEBUG_ACTIONSTOP;

    pid_integral = 4700;
    tracking_straight_pid_s(150);
    tracking_right();
    pid_integral = 3500; 
    tracking_straight_pid();
    tracking_left();
    pid_integral = -3500; 
    tracking_straight_pid();
    tracking_left();
    
    beforeround();
    circle_in(posit_left);
    Delay_ms(1000);
    pid_integral = -100;
    tracking_straight_pid();
    
    tracking_left();
    pid_integral = -3500; 
    tracking_straight_pid();
    tracking_left();
    
    pid_integral = -3500; 
    tracking_cross_pid();
    tracking_crossright();
    //tracking_right();
    
    pid_integral = 3700;
    tracking_straight_pid_s(5000);
    
    tracking_right();
    pid_integral = 3500; 
    tracking_straight_pid();
    tracking_left();
    pid_integral = -3500; 
    tracking_straight_pid();
    tracking_left();

    before_curve();
    curve_out();
    
    tracking_left();
    pid_integral = -3500;
    tracking_straight_pid();
    tracking_left();
    pid_integral = -3500; 
    tracking_final_pid();
    
    stop();
    while (1)
    {
    };
}
