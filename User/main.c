#include "stm32f10x.h"
#include "pwm.h"


int main(void)
{
    GPIO_PWM_init();
    TIM2_PWM_init();
    TIM1_PWM_init();
    for (;;)
    {
    }
}
