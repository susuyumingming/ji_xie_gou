#include "debug_uart.h"

__IO bool debug_uart_rxFrameFlag = false;
char debug_uart_rxCmd[debug_uart_FIFO_SIZE] = {0};// DMA接收缓冲区
int32_t debug_uart_coordinate[40];
/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void debug_uart_nvic_init(void)
{	

}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void debug_uart_clock_init(void)
{

}


/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void debug_uart_usart_init(void)
{	
	debug_uart_clock_init();
	debug_uart_nvic_init();  // 已改为UART5中断

}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void debug_uart_board_init(void)
{
	// 先关闭串口（必须！！！）
	LL_USART_Disable(UART5);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(UART5);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_7, (uint32_t)&UART5->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_7, (uint32_t)debug_uart_rxCmd);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, debug_uart_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(UART5);
	while((!(LL_USART_IsActiveFlag_TEACK(UART5))) || (!(LL_USART_IsActiveFlag_REACK(UART5))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
	LL_USART_EnableIT_IDLE(UART5);
	
	debug_uart_rxFrameFlag = false;
}

void debug_uartprocessing(void)
{
	if (debug_uart_rxFrameFlag == true)		//如果接收到数据包
	{
		// printf("debug_uartrxCount = %c, %c\r\n", debug_uartrxCmd[0], debug_uartrxCmd[1]);
		int8_t n=0,le=0;
		debug_uart_rxFrameFlag = false;
		for (int i = 0; i < 50; i++) 
		{
			if (debug_uart_rxCmd[i] == ',') 
			{
				int length = i - le;
				char debug_uartextracted_str[length + 1];
				if(length!=0)
				{
					strncpy(debug_uartextracted_str, &debug_uart_rxCmd[le], length);
					debug_uartextracted_str[length] = '\0';
					debug_uart_coordinate[n] = atoi(debug_uartextracted_str);
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

void debug_uart(void)
{
//	USART_Cmd(UART5,ENABLE);
//	delay_ms(20);
//	debug_uart_processing();
//	USART_Cmd(UART5,DISABLE);
}

void debug_uart_SendByte(uint8_t Byte)
{
	LL_USART_TransmitData8(UART5, Byte);//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while(LL_USART_IsActiveFlag_TXE(UART5) == 0);//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
//	delay_ms(1);
}

void debug_uart_SendArray(uint8_t *Array, uint16_t Length)
{
	for (uint16_t i = 0; i < Length; i++)
	{
		debug_uart_SendByte(Array[i]);
	}
}

void debug_uart_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		debug_uart_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
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
	debug_uart_SendString(String);		//串口发送字符数组（字符串）
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	debug_uart_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

void UART5_IRQHandler_FONCTION(void) 
{
	// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(UART5))
	{
		LL_USART_ReceiveData8(UART5);
		LL_USART_ClearFlag_PE(UART5);
	}
	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(UART5))
	{
		LL_USART_ReceiveData8(UART5);
		LL_USART_ClearFlag_ORE(UART5);
	}
	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(UART5))
	{
		LL_USART_ReceiveData8(UART5);
		LL_USART_ClearFlag_FE(UART5);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(UART5))
   {
		debug_uart_rxFrameFlag = true;
		LL_USART_ClearFlag_IDLE(UART5);
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_7));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC0(DMA1);
		LL_DMA_ClearFlag_HT0(DMA1);
		LL_DMA_ClearFlag_TE0(DMA1);
		LL_DMA_ClearFlag_DME0(DMA1);
		LL_DMA_ClearFlag_FE0(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_7, (uint32_t)debug_uart_rxCmd);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, debug_uart_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
   }
}




