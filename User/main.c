#include "stm32f10x.h"
#include "FunctionList.h"
#include "tracking.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"

#define GPIOB_IDR_BIT3 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 3 * 4))
#define GPIOB_IDR_BIT4 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 4 * 4))
#define GPIOB_IDR_BIT5 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 5 * 4))
#define GPIOB_IDR_BIT8 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 8 * 4))
#define GPIOB_IDR_BIT9 (*(uint32_t *)(0x42000000 + (GPIOB_BASE + 0x08 - 0x40000000) * 32 + 9 * 4))

int main(void)
{
    /* 嵌套向量中断控制器组选择 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_Init();
    USART_Config();
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
#if TRACKER_POLLING
    GPIO_tracker_init_polling();
    //TIM3_tracker_init_polling();
    //TIM1_tracker_init_polling();
    NVIC_tracker_init_polling();
#else
    GPIO_tracker_init();
    NVIC_tracker_init();
#endif
    encoder_init();

    Usart_SendString(DEBUG_USARTx, "system inited\r\n");
    stop();
#if 1

    for(;;)
    {
        //printf("%d %d \r\n",TIM3->CNT,TIM4->CNT);
        //servo_set_dutyclcle(1500);
        servo_setangle(90);
        tracker_sendinfo();
        tracking_resume();
        Delay_ms(200);
    }
#endif

    //stateswitcher();
    /* TODO: 舵机的角度对应关系还没有调好 */
       for (int i = 500;;i+=100)
       {
    
           Delay_ms(500);
           servo_set_dutyclcle(i);
           printf("%d\r\n",i);
           if (i > 2500)
           {
               i = 500;
           }
           Usart_SendString(DEBUG_USARTx, "running\r\n");
       }
}

void DEBUG_USART_IRQHandler(void)
{
    if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
    {
        FunList_Call(USART_ReceiveData(DEBUG_USARTx));
    }
}
