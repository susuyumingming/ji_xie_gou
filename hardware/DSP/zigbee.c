#include "zigbee.h"
#include <stdarg.h> 
#include <stdio.h>
#include "usart.h"
__IO bool zigbee_rxFrameFlag = false;
__IO uint8_t zigbee_rxCmd[ZIGBEE_FIFO_SIZE] = {0};// DMA接收缓冲区
__IO uint8_t zigbee_rxCount;
uint8_t RxData;
/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void Zigbee_nvic_init(void)
{	

}
/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void Zigbee_clock_init(void)
{

}
/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void Zigbee_usart_init(void)
{

}
/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/

void Zigbee_board_init(void)
{
	 // 先关闭串口（必须！！！）
	LL_USART_Disable(USART3);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(USART3);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_3, (uint32_t)&USART3->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_3, (uint32_t)zigbee_rxCmd);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_3, ZIGBEE_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(USART3);
	while((!(LL_USART_IsActiveFlag_TEACK(USART3))) || (!(LL_USART_IsActiveFlag_REACK(USART3))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
	LL_USART_EnableIT_IDLE(USART3);
	
	// Zigbee_usart_init();
	zigbee_rxFrameFlag = false;
}

//接收状态

uint8_t Zigbee_RxFlag=0;			//定义接收边框坐标数据包标志位

char Zigbee_RxPacket[40];				//定义接收数据包数组，数据包格式

int32_t Zigbee_coordinate[40]; 

void Zigbee_processing(void)
{
	if (zigbee_rxFrameFlag == true)		//如果接收到数据包
	{
		// printf("zigbee_rxCount = %c, %c\r\n", zigbee_rxCmd[0], zigbee_rxCmd[1]);
		int8_t n=0,le=0;
		zigbee_rxFrameFlag = false;
		for (int i = 0; i < 50; i++) 
		{
			if (zigbee_rxCmd[i] == ',') 
			{
				int length = i - le;
				char Zigbee_extracted_str[length + 1];
				if(length!=0)
				{
					strncpy(Zigbee_extracted_str, &zigbee_rxCmd[le], length);
					Zigbee_extracted_str[length] = '\0';
					Zigbee_coordinate[n] = atoi(Zigbee_extracted_str);
					n = n + 1;
				}
				le = i + 1;
			}
			else if (n == 32) 
			{
				break;
			}
		}
	}	
}

void Zigbee_Serial_SendByte(uint8_t Byte)
{
	LL_USART_TransmitData8(USART3, Byte);//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while(LL_USART_IsActiveFlag_TXE(USART3) == 0);//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
//	delay_ms(1);
}

void Zigbee_Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	for (uint16_t i = 0; i < Length; i++)
	{
		Zigbee_Serial_SendByte(Array[i]);
	}
}

void Zigbee_Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Zigbee_Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Zigbee_Serial_SendString(String);		//串口发送字符数组（字符串）
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	Zigbee_Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

void USART3_IRQHandler_FONCTION()                	//串口3中断服务程序
{
// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(USART3))
	{
		LL_USART_ReceiveData8(USART3);
		LL_USART_ClearFlag_PE(USART3);
	}

	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(USART3))
	{
		LL_USART_ReceiveData8(USART3);
		LL_USART_ClearFlag_ORE(USART3);
	}

	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(USART3))
	{
		LL_USART_ReceiveData8(USART3);
		LL_USART_ClearFlag_FE(USART3);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(USART3))
   {
		zigbee_rxFrameFlag=true;
		LL_USART_ClearFlag_IDLE(USART3);
		// 调试：打印DMA当前传输计数，看有没有收到数据
		// uint32_t cnt = LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_3);
		// printf("DMA cnt: %d\r\n", cnt); // 如果收到数据，cnt会小于初始值ZIGBEE_FIFO_SIZE
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_3);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_3));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC3(DMA1);
		LL_DMA_ClearFlag_HT3(DMA1);
		LL_DMA_ClearFlag_TE3(DMA1);
		LL_DMA_ClearFlag_DME3(DMA1);
		LL_DMA_ClearFlag_FE3(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_3, (uint32_t)zigbee_rxCmd);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_3, ZIGBEE_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
   }
} 

