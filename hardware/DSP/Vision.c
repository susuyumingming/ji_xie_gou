#include "Vision.h"
#include "delay.h"
/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void Vision_nvic_init(void)
{	
	// 4bit抢占优先级位
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void Vision_clock_init(void)
{
	// 使能GPIOA、AFIO外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟

	// 使能USART1外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
	
}

/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void Vision_usart_init(void)
{	
	Vision_clock_init();

/**********************************************************
***	初始化USART1引脚
**********************************************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	// PC10 - USART1_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PC11 - USART1_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					/* 浮空输入 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/**********************************************************
***	初始化UART4
**********************************************************/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);

	Vision_nvic_init();

/**********************************************************
***	清除USART1中断
**********************************************************/
	USART2->SR; USART2->DR;
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);

/**********************************************************
***	使能USART1中断
**********************************************************/	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

/**********************************************************
***	使能USART1
**********************************************************/
	USART_Cmd(USART2, ENABLE);
}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void Vision_board_init(void)
{
	Vision_usart_init();
}

//接收状态

uint8_t Vision_RxFlag=0;			//定义接收边框坐标数据包标志位

char Vision_RxPacket[40];				//定义接收数据包数组，数据包格式

int16_t coordinate[40]; 

int Vision_processing(void)
{
	if (Vision_RxFlag == 1)		//如果接收到数据包
	{
		uint8_t n=0,le=0;
		Vision_RxFlag = 0;
		for (int i = 0; i < 50; i++) 
		{
			if (Vision_RxPacket[i] == ',') 
			{
				int length = i - le;
				char Vision_extracted_str[10];//length + 1
				strncpy(Vision_extracted_str, &Vision_RxPacket[le], length);//length
				Vision_extracted_str[length] = '\0';//length
				coordinate[n] = atoi(Vision_extracted_str);
				n = n + 1;
				le = i + 1;
			}
			else if (n == 32) 
			{
				break;
			}
		}
		Vision_RxFlag=0;
		return 0;
	}	
	else return 1;
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Vision_Serial_SendByte(uint16_t Byte)
{
	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
//	delay_ms(1);
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Vision_Serial_SendArray(uint16_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Vision_Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Vision_Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Vision_Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	static uint8_t RxState = 0;		//边框//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;		//边框//定义表示当前接收数据位置的静态变量
	
//	if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)  
//	{  
//		USART_ReceiveData(USART2);  
//		USART_ClearFlag(USART2, USART_FLAG_PE);  
//	}  

//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
//	{  
//		USART_ReceiveData(USART2);  
//		USART_ClearFlag(USART2, USART_FLAG_ORE);  
//	}  

//	if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)  
//	{  
//		USART_ReceiveData(USART2);  
//		USART_ClearFlag(USART2, USART_FLAG_FE);  
//	}
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	//判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(USART2);			//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (RxData == '#' && Vision_RxFlag == 0)		//如果数据确实是包头，并且上一个数据包已处理完毕
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据，同时判断是否接收到了第一个包尾*/
		else if (RxState == 1)
		{
			if (RxData == 'b')	//如果收到第一个包尾
			{
				RxState = 0;			//置下一个状态
				Vision_RxPacket[pRxPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
				Vision_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
				Vision_processing();
			}
			else						//接收到了正常的数据
			{
				Vision_RxPacket[pRxPacket] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket ++;			//数据包的位置自增
			}
		}
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE); 
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//清除标志位
	}
} 




