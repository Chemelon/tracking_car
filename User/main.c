#include "stm32f10x.h"
#include "pwm.h"
#include "SysTick.h"
#include "usart.h"


int main(void)
{
    HSI_sysclock_config(RCC_PLLMul_9);
    SystemCoreClockUpdate();
    // SysTick_Init();
    USART_Config();
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
    for (;;)
    {
        // Delay_ms(100);
        Usart_SendString(USART1, "helloworld\n");
    }
}
