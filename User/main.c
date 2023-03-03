#include "stm32f10x.h"
#include "FunctionList.h"
#include "tracking.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"
#include "motor.h"

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
    TIM3_tracker_init_polling();
    NVIC_tracker_init_polling();
#else
    GPIO_tracker_init();
    NVIC_tracker_init();
#endif
    Usart_SendString(DEBUG_USARTx, "system inited\r\n");



    //servo_set_dutyclcle(2400);
//    gostraight(0);
//    while(1);
    stop();
    
    
    //tracking_straight();
    //stop();

#if 0
    for(;;)
    {
        tracker_sendinfo();
        tracking_resume();
        Delay_ms(50);
    }
#endif

    stateswitcher();  
    
    
    for(;;)
    {
        servo_setangle(45);
        Delay_ms(1000);
        servo_setangle(135);
        Delay_ms(1000);
    
    }

    /* TODO: 舵机的角度对应关系还没有调好 */
//    for (int i = 0;;i+=1)
//    {
//        
//        Delay_ms(500);
//        servo_setangle(i);
//        printf("%d\r\n",i);
//        if (i > 180)
//        {
//            i = 0;
//        }
//        // Usart_SendString(DEBUG_USARTx, "running\r\n");
//    }
}

void DEBUG_USART_IRQHandler(void)
{
    if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
    {
        FunList_Call(USART_ReceiveData(DEBUG_USARTx));
    }
}
