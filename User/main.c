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
    for (;;)
    {
    }
}
