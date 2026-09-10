#include "JY61P.h"
#include "stm32h7xx_ll_dma.h"
__IO bool JY61P_rxFrameFlag = false;
__IO uint8_t JY61P_rxCmd[JY61P_FIFO_SIZE] = {0};// DMA接收缓冲区


/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void JY61P_nvic_init(void)
{	

}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void JY61P_clock_init(void)
{

}


/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void JY61P_usart_init(void)
{	

}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void JY61P_board_init(void)
{
	 // 先关闭串口（必须！！！）
	LL_USART_Disable(USART2);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(USART2);  
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_7, (uint32_t)&USART2->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_7, (uint32_t)JY61P_rxCmd);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, JY61P_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(USART2);
	while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);
	LL_USART_EnableIT_IDLE(USART2);
	
	// JY61P_usart_init();
	JY61P_rxFrameFlag = false;
}


float JY61P_Roll=0;
float JY61P_Pitch=0;
float JY61P_Yaw=0;

void JY61_Data_processing(	int8_t *JY61_Roll_high_byte,int8_t *JY61_Roll_low_byte,
							int8_t *JY61_Pitch_high_byte,int8_t *JY61_Pitch_low_byte,
							int8_t *JY61_Yaw_high_byte,int8_t *JY61_Yaw_low_byte)
{
	if(JY61P_rxFrameFlag == true)
	{
		JY61P_Roll=(short)(((short)JY61P_rxCmd[25]<<8)|JY61P_rxCmd[24])/32768.0f*180.0f;
		JY61P_Pitch=(short)(((short)JY61P_rxCmd[27]<<8)|JY61P_rxCmd[26])/32768.0f*180.0f;
		JY61P_Yaw=(short)(((short)JY61P_rxCmd[29]<<8)|JY61P_rxCmd[28])/32768.0f*180.0f;
		*JY61_Roll_high_byte=JY61P_rxCmd[25];
		*JY61_Roll_low_byte=JY61P_rxCmd[24];
		*JY61_Pitch_high_byte=JY61P_rxCmd[27];
		*JY61_Pitch_low_byte=JY61P_rxCmd[26];
		*JY61_Yaw_high_byte=JY61P_rxCmd[29];
		*JY61_Yaw_low_byte=JY61P_rxCmd[28];
		JY61P_rxFrameFlag = false;
	}
	
}
//			OLED_ShowHexNum_My(4,1,JY61P_rxCmd[0],2,OLED_6X8);
//			OLED_ShowHexNum_My(4,4,JY61P_rxCmd[1],2,OLED_6X8);
//			OLED_ShowHexNum_My(4,7,JY61P_rxCmd[2],2,OLED_6X8);
//			OLED_ShowHexNum_My(4,10,JY61P_rxCmd[3],2,OLED_6X8);
//			OLED_ShowHexNum_My(4,13,JY61P_rxCmd[4],2,OLED_6X8);
//			OLED_ShowHexNum_My(4,16,JY61P_rxCmd[5],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,1,JY61P_rxCmd[6],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,4,JY61P_rxCmd[7],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,7,JY61P_rxCmd[8],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,10,JY61P_rxCmd[9],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,13,JY61P_rxCmd[10],2,OLED_6X8);
//			OLED_ShowHexNum_My(5,16,JY61P_rxCmd[11],2,OLED_6X8);
void JY61P(void)
{
//	USART_Cmd(UART4,ENABLE);
//	delay_ms(20);
//	MPUData_processing();
//	USART_Cmd(UART4,DISABLE);
}


/**********************************************************
***	USART2中断服务程序
**********************************************************/
void USART2_IRQHandler_FONCTION(void) {
  // 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(USART2))
	{
		LL_USART_ReceiveData8(USART2);
		LL_USART_ClearFlag_PE(USART2);
	}

	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(USART2))
	{
		LL_USART_ReceiveData8(USART2);
		LL_USART_ClearFlag_ORE(USART2);
	}

	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(USART2))
	{
		LL_USART_ReceiveData8(USART2);
		LL_USART_ClearFlag_FE(USART2);
	}
   
   // 空闲中断处理（核心逻辑不变，DMA改为DMA2_Stream1）
   if (LL_USART_IsActiveFlag_IDLE(USART2)) {
		JY61P_rxFrameFlag = true;
		LL_USART_ClearFlag_IDLE(USART2);
       
       // 关闭DMA2_Stream7，防止继续接收
       LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);
       // 等待DMA完全关闭（确保可靠性）
       while (LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_7));
       
       // 清除DMA2_Stream7所有标志（原DMA1_Stream2改为DMA2_Stream1）
		LL_DMA_ClearFlag_TC7(DMA2);
		LL_DMA_ClearFlag_HT7(DMA2);
		LL_DMA_ClearFlag_TE7(DMA2);
		LL_DMA_ClearFlag_DME7(DMA2);
		LL_DMA_ClearFlag_FE7(DMA2);
          
		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_7, (uint32_t)JY61P_rxCmd);
    	// 重新设置数据长度
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, JY61P_FIFO_SIZE);	
		// 重新开启 DMA
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);	

   }
}

