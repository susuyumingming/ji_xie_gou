#include "deng.h"

void DENG_Init(void)
{
	/*开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		//开启GPIOD的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
		/*开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//开启GPIOD的时钟
	
	/*GPIO初始化*/

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}

void Red_DENG_ON(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_0, (BitAction)1);
}

//void Buzzer_ON(void)
//{
//	GPIO_WriteBit(GPIOC, GPIO_Pin_8, (BitAction)1);
//}

//void Buzzer_OFF(void)
//{
//	GPIO_WriteBit(GPIOC, GPIO_Pin_8, (BitAction)0);
//}

void Red_DENG_OFF(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_0, (BitAction)0);
}

void Green_DENG_ON(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_1, (BitAction)1);
}

void Green_DENG_OFF(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_1, (BitAction)0);
}




void HongWai_Init(void)
{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;        
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;        
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int16_t Read_HongWai(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
	{
		return 0;
	}
	else return 1;
}


