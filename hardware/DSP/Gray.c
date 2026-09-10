#include "Gray.h"

#define HuiDu_num 10
uint16_t HuiDu_flag=0;

//从右往左数
const PIO Pins[] = {
    {GPIOE, GPIO_Pin_0},  // HuiDu_0
    {GPIOE, GPIO_Pin_1},  // HuiDu_1
    {GPIOE, GPIO_Pin_2},  // HuiDu_2
    {GPIOE, GPIO_Pin_3},  // HuiDu_3
    {GPIOE, GPIO_Pin_4},  // HuiDu_4
    {GPIOE, GPIO_Pin_5},  // HuiDu_5
    {GPIOE, GPIO_Pin_6}, // HuiDu_6
    {GPIOD, GPIO_Pin_12}, // HuiDu_7
    {GPIOD, GPIO_Pin_13}, // HuiDu_8
    {GPIOD, GPIO_Pin_14},  // HuiDu_9
		{GPIOD, GPIO_Pin_15},  // HuiDu_10
		{GPIOD, GPIO_Pin_0}  // HuiDu_11
};

/****************
  灰度传感器 GPIO 初始化
**************************  */
void HuiDu_Init(void) {
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;          
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;        
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 高速

	
    GPIO_InitStruct.GPIO_Pin = Pins[0].pin;// HuiDu_0
    GPIO_Init(Pins[0].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[1].pin;
    GPIO_Init(Pins[1].port, &GPIO_InitStruct);// HuiDu_1

    GPIO_InitStruct.GPIO_Pin = Pins[2].pin;// HuiDu_2
    GPIO_Init(Pins[2].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[3].pin;// HuiDu_3
    GPIO_Init(Pins[3].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[4].pin;// HuiDu_4
    GPIO_Init(Pins[4].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[5].pin;// HuiDu_5
    GPIO_Init(Pins[5].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[6].pin;// HuiDu_6
    GPIO_Init(Pins[6].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[7].pin;// HuiDu_7
    GPIO_Init(Pins[7].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[8].pin;// HuiDu_8
    GPIO_Init(Pins[8].port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = Pins[9].pin;// HuiDu_9
    GPIO_Init(Pins[9].port, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin = Pins[10].pin;// HuiDu_10
    GPIO_Init(Pins[10].port, &GPIO_InitStruct);
}

/*灰度序号从左往右数，依次增加
标志位写入时，从右往左写，因此需颠倒*/
uint16_t Get_gray(void)
{
    uint16_t new_flag = 0;
    for (int i = HuiDu_num; i >= 0; i--) 
    {
			new_flag |= (HuiDu_Read(i) ?   (1 << i):0);
    }
    HuiDu_flag = new_flag; 
    return HuiDu_flag;
}

/*二进制比较，相同输出1，不同输出0
二进制比较需要前缀0b
if(HuiDu_compare(Get_gray(),0b010010))
{
	
}
*/
int HuiDu_compare(uint16_t num_1,uint16_t num_2)
{
	return (num_1^num_2)==0;
}



