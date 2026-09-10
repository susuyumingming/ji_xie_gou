#include "Buzzer.h"
void Buzzer_ON(void)
{
    LL_TIM_OC_SetCompareCH2(TIM1, 10000);
    
    // LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);
//    TIM_SetCompare3(TIM8, 5000);		//设置CCR2的值
}

void Buzzer_OFF(void)
{
    LL_TIM_OC_SetCompareCH2(TIM1, 0);
    // LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_9);
//    TIM_SetCompare3(TIM8, 0);		//设置CCR2的值
}
// 蜂鸣器初始化函数
void Buzzer_Init(void)
{
    LL_TIM_EnableAllOutputs(TIM1);  // 高级定时器主输出使能
    LL_TIM_EnableCounter(TIM1);     // ? 开启定时器计数！
    Buzzer_OFF();
    Buzzer_ON();
    delay_ms(100);
    Buzzer_OFF();
    delay_ms(500);
    Buzzer_ON();
    delay_ms(100);
    Buzzer_OFF();
    // Buzzer_ON();
}





