#include "tracking.h"
#include "usart.h"
#include "FunctionList.h"
#include "motor.h"
#include "SysTick.h"

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

#define TRACKER_PIN1 GPIOB_IDR_BIT3
#define TRACKER_PIN2 GPIOB_IDR_BIT4
#define TRACKER_PIN3 GPIOB_IDR_BIT5
#define TRACKER_PIN4 GPIOB_IDR_BIT8
#define TRACKER_PIN5 GPIOB_IDR_BIT9


enum tracker_color
{
    t_color_white,
    t_color_black

};

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
/**
 * @brief 中断频率为500hz 累计10次的采样总值 即实际控制处理频率约为50hz
 *
 */
void TIM3_IRQHandler(void)
{
    TIM3->SR = ~TIM_SR_UIF;
    if (tracker_status.tracker_cnt_it > POLLING_CNT)
    {
        /* 关闭计数器 */
        TIM3->CR1 &= ~TIM_CR1_CEN;
        /* 通知更新 */
        tracker_status.update = status_updated;
        return;
    }
    {
        /* 将光电管的状态保存至内存 */
        // tracker_status.update = status_updated;
        tracker_status.tarcker1 = TRACKER_PIN1;
        tracker_status.tarcker2 = TRACKER_PIN2; // 1
        tracker_status.tarcker3 = TRACKER_PIN3; // 1
        tracker_status.tarcker4 = TRACKER_PIN4; // 1
        tracker_status.tarcker5 = TRACKER_PIN5;
        /* 将光电管的状态保存至内存 */
        //tracker_status.tracker_sum_signed += GPIOB_IDR_BIT7 - GPIOB_IDR_BIT4;
        tracker_status.tracker_cnt_it++;
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
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    
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

void NVIC_tracker_init_polling(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置TIM3为中断源 */
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

void tracking_resume(void)
{
#if TRACKER_POLLING
    /* 清除累计值 */
    ptracker_status->tracker_sum_signed = 0;
    /* 复位计数值 */
    ptracker_status->tracker_cnt_it = 0;
    /* 更新状态 */
    ptracker_status->update = status_resloved;
    /* 复位CNT */
    TIM3->CNT = 0;
    /* 使能定时器 */
    TIM3->CR1 |= TIM_CR1_CEN;
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
        // printf("total: %d \r\n", ptracker_status->tracker_sum_signed);
        /* 更新状态 */
        // tracking_resume();
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

/* 直线循迹 */
void tracking_straight(void)
{
    /* TODO:找到舵机角度和pwm占空比关系 */
    // servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            // STRAIGHT_LOG("STRAIGHTING\r\n");
            continue;
        }
        // tracker_sendinfo();
        /* 退出循环所需满足的条件 TODO:有时候场地脏污使得错误识别提前退出,需要比较苛刻的约束,以后具体调试 */
        if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
        {
            // tracker_sendinfo();
            /* 根据实际数据修改的条件 OK */
            if ((TRACKER2_STATUS == t_color_white) && (TRACKER3_STATUS == t_color_white) && (TRACKER4_STATUS == t_color_white))
            {
                STRAIGHT_LOG("STRAITHTEXIT\r\n");
                tracking_resume();
                break;
            }
        }
        // STRAIGHT_LOG("UPDATED\r\n");
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            // servo_setangle(S_RIGHTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT + RIGHTWARD_ADD);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
            tracker_sendinfo();
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            // servo_setangle(S_LEFTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + LEFTWARD_ADD);
            tracker_sendinfo();
            STRAIGHT_LOG("LEFTWARD\r\n");
        }
        gostraight(0);
        // servo_set_dutyclcle(2400);
        tracking_resume();
    }
}

/* 巡线90度左转 */
void tracking_left(void)
{
    TRACKLEFT90_LOG("LEFT90IN");
    /* 向左大转弯 右轮加速左轮减速 */
    // servo_setangle(S_LEFT90);
    motor_setforward_right(RIGHTTURNBASE_RIGHT + LEFTTURN_ADD);
    motor_setbrake_left();
    motor_setbackward_left(LEFTTURNBASE_LEFT + LEFTTURN_SUB);
    // Delay_ms(50);
    motor_setbrake_left();
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            // TRACKLEFT90_LOG("LEFT90TURNING\r\n");
            continue;
        }
        tracker_sendinfo();
        /* 中心和靠外圈的光电均为黑色退出转弯模式 */
        // if (TRACKER3_STATUS == t_color_black && TRACKER2_STATUS == t_color_black)
        if (TRACKER2_STATUS == t_color_black)
        {
            tracker_sendinfo();
            tracking_resume();
            TRACKLEFT90_LOG("LEFT90EXIT");
            break;
        }
        tracking_resume();
    }
}

/* 巡线90度右转 */
void tracking_right(void)
{
    TRACKRIGHT90_LOG("RIGHT90IN");
    /* 向右大转弯 左轮加速右轮减速 */
    // servo_setangle(S_RIGHT90);
    motor_setforward_left(RIGHTTURNBASE_LEFT + RIGHTTURN_ADD);
    motor_setbrake_right();
    motor_setbackward_right(RIGHTTURNBASE_RIGHT + RIGHTTURN_SUB);
    // Delay_ms(50);
    motor_setbrake_right();
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            // TRACKRIGHT90_LOG("RIGHT90TURNING\r\n");
            continue;
        }
        /* 中心和靠外圈的光电均为黑色退出转弯模式 */
        if (TRACKER4_STATUS == t_color_black)
        {
            tracker_sendinfo();
            tracking_resume();
            TRACKRIGHT90_LOG("RIGHT90EXIT");
            break;
        }
        tracking_resume();
    }
}


/* 进环 */
void circle_in(void)
{

}

/* 出环 */
void circle_out(void)
{
    
}
