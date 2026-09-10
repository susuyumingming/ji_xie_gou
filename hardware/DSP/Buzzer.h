#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "sys.h"


//#define BUZZER_PIN GPIO_Pin_13
//#define BUZZER_GPIO_PORT GPIOC
//#define BUZZER_RCC_CLK RCC_AHB1Periph_GPIOC

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#endif
    
		
