#include "VL53.h"

__IO bool VL53_rxFrameFlag_6 = false;
__IO bool VL53_rxFrameFlag_7 = false;
__IO bool VL53_rxFrameFlag_8 = false;
char VL53_rxCmd_6[VL53_FIFO_SIZE] = {0};// DMA接收缓冲区
char VL53_rxCmd_7[VL53_FIFO_SIZE] = {0};// DMA接收缓冲区
char VL53_rxCmd_8[VL53_FIFO_SIZE] = {0};// DMA接收缓冲区

/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void VL53_nvic_init(void)
{	

}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void VL53_clock_init(void)
{

}


/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void VL53_usart_init(void)
{	
	VL53_clock_init();
	VL53_nvic_init();  // 已改为UART5中断

}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void VL53_board_init_6(void)
{
	// 先关闭串口（必须！！！）
	LL_USART_Disable(USART6);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(USART6);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)&USART6->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)VL53_rxCmd_6);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, VL53_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(USART6);
	while((!(LL_USART_IsActiveFlag_TEACK(USART6))) || (!(LL_USART_IsActiveFlag_REACK(USART6))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
	LL_USART_EnableIT_IDLE(USART6);
	
	VL53_rxFrameFlag_6 = false;
}
void VL53_board_init_7(void)
{
	// 先关闭串口（必须！！！）
	LL_USART_Disable(UART7);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(UART7);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)&UART7->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)VL53_rxCmd_7);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, VL53_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(UART7);
	while((!(LL_USART_IsActiveFlag_TEACK(UART7))) || (!(LL_USART_IsActiveFlag_REACK(UART7))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
	LL_USART_EnableIT_IDLE(UART7);
	
	VL53_rxFrameFlag_7 = false;
}
void VL53_board_init_8(void)
{
	// 先关闭串口（必须！！！）
	LL_USART_Disable(UART8);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(UART8);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)&UART8->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)VL53_rxCmd_8);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, VL53_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(UART8);
	while((!(LL_USART_IsActiveFlag_TEACK(UART8))) || (!(LL_USART_IsActiveFlag_REACK(UART8))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
	LL_USART_EnableIT_IDLE(UART8);
	
	VL53_rxFrameFlag_8 = false;
}


int16_t VL53_Distance_6 = 0;
char VL53_extracted_str_6[10];//length + 1
int16_t VL53_Distance_7 = 0;
char VL53_extracted_str_7[10];//length + 1
int16_t VL53_Distance_8 = 0;
char VL53_extracted_str_8[10];//length + 1
void VL53_data_processing_6(int8_t *VL53_high_byte,int8_t *VL53_low_byte)
{
	memcpy(VL53_extracted_str_6, &VL53_rxCmd_6[9], 4);//length
	VL53_Distance_6 = atoi((char*)VL53_extracted_str_6);
	*VL53_low_byte = VL53_Distance_6 & 0xFF;
	*VL53_high_byte = (VL53_Distance_6>>8) & 0xFF;
	// printf("VL53_Distance_6 = %s\r\n",VL53_rxCmd_6);
}
void VL53_data_processing_7(int8_t *VL53_high_byte,int8_t *VL53_low_byte)
{
	memcpy(VL53_extracted_str_7, &VL53_rxCmd_7[9], 4);//length
	VL53_Distance_7 = atoi((char*)VL53_extracted_str_7);
	*VL53_low_byte = VL53_Distance_7 & 0xFF;
	*VL53_high_byte = (VL53_Distance_7>>8) & 0xFF;
	// printf("VL53_Distance_7 = %s\r\n",VL53_rxCmd_7);
}
void VL53_data_processing_8(int8_t *VL53_high_byte,int8_t *VL53_low_byte)
{
	memcpy(VL53_extracted_str_8, &VL53_rxCmd_8[9], 4);//length
	VL53_Distance_8 = atoi((char*)VL53_extracted_str_8);
	*VL53_low_byte = VL53_Distance_8 & 0xFF;
	*VL53_high_byte = (VL53_Distance_8>>8) & 0xFF;
	// printf("VL53_Distance_8 = %s\r\n",VL53_rxCmd_8);
}
void VL53(void)
{
//	USART_Cmd(UART5,ENABLE);
//	delay_ms(20);
//	VL53_processing();
//	USART_Cmd(UART5,DISABLE);
}

void USART6_IRQHandler_FONCTION(void) 
{
	// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(USART6))
	{
		LL_USART_ReceiveData8(USART6);
		LL_USART_ClearFlag_PE(USART6);
	}
	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(USART6))
	{
		LL_USART_ReceiveData8(USART6);
		LL_USART_ClearFlag_ORE(USART6);
	}
	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(USART6))
	{
		LL_USART_ReceiveData8(USART6);
		LL_USART_ClearFlag_FE(USART6);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(USART6))
   {
		VL53_rxFrameFlag_6 = true;
		LL_USART_ClearFlag_IDLE(USART6);
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_0));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC0(DMA1);
		LL_DMA_ClearFlag_HT0(DMA1);
		LL_DMA_ClearFlag_TE0(DMA1);
		LL_DMA_ClearFlag_DME0(DMA1);
		LL_DMA_ClearFlag_FE0(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)VL53_rxCmd_6);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, VL53_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
   }
}
void UART7_IRQHandler_FONCTION(void) 
{
	// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(UART7))
	{
		LL_USART_ReceiveData8(UART7);
		LL_USART_ClearFlag_PE(UART7);
	}
	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(UART7))
	{
		LL_USART_ReceiveData8(UART7);
		LL_USART_ClearFlag_ORE(UART7);
	}
	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(UART7))
	{
		LL_USART_ReceiveData8(UART7);
		LL_USART_ClearFlag_FE(UART7);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(UART7))
   {
		VL53_rxFrameFlag_7 = true;
		LL_USART_ClearFlag_IDLE(UART7);
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_1));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC1(DMA1);
		LL_DMA_ClearFlag_HT1(DMA1);
		LL_DMA_ClearFlag_TE1(DMA1);
		LL_DMA_ClearFlag_DME1(DMA1);
		LL_DMA_ClearFlag_FE1(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)VL53_rxCmd_7);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, VL53_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
   }
}
void UART8_IRQHandler_FONCTION(void) 
{
	// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(UART8))
	{
		LL_USART_ReceiveData8(UART8);
		LL_USART_ClearFlag_PE(UART8);
	}
	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(UART8))
	{
		LL_USART_ReceiveData8(UART8);
		LL_USART_ClearFlag_ORE(UART8);
	}
	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(UART8))
	{
		LL_USART_ReceiveData8(UART8);
		LL_USART_ClearFlag_FE(UART8);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(UART8))
   {
		VL53_rxFrameFlag_8 = true;
		LL_USART_ClearFlag_IDLE(UART8);
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_2));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC2(DMA1);
		LL_DMA_ClearFlag_HT2(DMA1);
		LL_DMA_ClearFlag_TE2(DMA1);
		LL_DMA_ClearFlag_DME2(DMA1);
		LL_DMA_ClearFlag_FE2(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)VL53_rxCmd_8);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, VL53_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
   }
}


