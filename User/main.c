#include "stm32f10x.h"
#include "tracking.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"
#include "motor.h"

int main(void)
{
    /* 嵌套向量中断控制器组选择 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    // SysTick_Init();
    USART_Config();
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
#if TRACKER_POLLING
    GPIO_tracker_init_polling();
    TIM3_tracker_init_polling();
    NVIC_tracker_init_polling();
#else
    GPIO_tracker_init();
    NVIC_tracker_init();
#endif
    Usart_SendString(USART1, "system inited\r\n");
    while (1)
    {
        if (ptracker_status->update == tracker_updated)
        {
            printf("%d ", ptracker_status->tarcker1_status);
            printf("%d ", ptracker_status->tarcker2_status);
            printf("%d ", ptracker_status->tarcker3_status);
            printf("%d ", ptracker_status->tarcker4_status);
            printf("%d ", ptracker_status->tarcker5_status);

            /* 取得累计值 */
            printf("%d \r\n", ptracker_status->tracker_sum_signed);
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
    }
    // gostright(1500);
    for (;;)
    {
#if 1
        if (ptracker_status->update == tracker_updated)
        {
            if (ptracker_status->tarcker2_status && ptracker_status->tarcker3_status)
            {
                turnleft(45);
            }
            if (ptracker_status->tarcker4_status && ptracker_status->tarcker3_status)
            {
                turnright(45);
            }
            if (!(ptracker_status->tarcker4_status) && ptracker_status->tarcker3_status && !(ptracker_status->tarcker2_status))
            {
                gostright(1500);
            }
        }
#endif
        // Usart_SendString(USART1, "helloworld\n");
    }
}
