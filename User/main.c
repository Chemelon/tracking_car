#include "stm32f10x.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"
#include "motor.h"


int main(void)
{
    USART_Config();
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
    motor_gostright(500);
    for (;;)
    {
        Usart_SendString(USART1, "helloworld\n");
    }
}
