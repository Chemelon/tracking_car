#include "stm32f10x.h"
#include "tracking.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"
#include "motor.h"

extern void servo_turnleft(uint16_t angle);

int main(void)
{
    /* 嵌套向量中断控制器组选择 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_Init();
    USART_Config();
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
    GPIO_tracker_init();
    NVIC_tracker_init();
    gostright(500);

    servo_turnleft(0);
    Delay_ms(100);
    servo_turnleft(45);
    Delay_ms(100);
    servo_turnleft(90);
    Delay_ms(100);
    servo_turnleft(135);
    for (;;)
    {
        Usart_SendString(USART1, "helloworld\n");
    }
}
