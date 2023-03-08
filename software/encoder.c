#include "encoder.h"

void encoder_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_Cmd(TIM3,ENABLE);
    TIM_Cmd(TIM4,ENABLE);  
}