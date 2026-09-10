#include "HWT101.h"
#include "stm32h7xx_ll_dma.h"

__IO bool HWT101_rxFrameFlag = false;
__IO uint8_t HWT101_rxCmd[HWT101_FIFO_SIZE] = {0};// DMA接收缓冲区


/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void HWT101_nvic_init(void)
{	

}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void HWT101_clock_init(void)
{

}


/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void HWT101_usart_init(void)
{	

}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void HWT101_board_init(void)
{
	 // 先关闭串口（必须！！！）
	LL_USART_Disable(USART1);

	// ??? 在这里配置 DMA 请求（串口关闭时写！！！）
	LL_USART_EnableDMAReq_RX(USART1);  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_5, (uint32_t)&USART1->RDR); 
	// 配置 DMA
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_5, (uint32_t)HWT101_rxCmd);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, HWT101_FIFO_SIZE);

	// 重新使能串口
	LL_USART_Enable(USART1);
	while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))));

	// 最后开 DMA + 空闲中断
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
	LL_USART_EnableIT_IDLE(USART1);
	
	// HWT101_usart_init();
	HWT101_rxFrameFlag = false;
}

//接收状态

int16_t mpu;

uint8_t Serial_RxFla_Data_processing=0;			//定义接收边框坐标数据包标志位
uint8_t Serial_RxFlag_mpu=0;			//定义接收边框坐标数据包标志位
uint8_t Serial_RxFlag_mpu_1=0;			//定义接收边框坐标数据包标志位
uint8_t Serial_RxFlag_mpu_2=0;			//定义接收边框坐标数据包标志位

uint8_t pRxPacket_mpu = 0;		//边框//定义表示当前接收数据位置的静态变量
int16_t Serial_RxPacket_Mpu[40];				//定义接收数据包数组，数据包格式

float Yaw=0;

//__IO bool HWT101_rxFrameFlag = false;
//__IO uint8_t HWT101_rxCmd[HWT101_FIFO_SIZE] = {0};
__IO uint8_t HWT101_rxCount = 0;

void MPUData_processing(int8_t *HWT101_high_byte,int8_t *HWT101_low_byte)
{
	if(HWT101_rxFrameFlag == true)
	{
		Yaw=(short)(((short)HWT101_rxCmd[7]<<8)|HWT101_rxCmd[6])/32768.0f*180.0f;
		*HWT101_high_byte = HWT101_rxCmd[7];
		*HWT101_low_byte = HWT101_rxCmd[6];
		HWT101_rxFrameFlag = false;
	}
}

void HWT101(void)
{
//	USART_Cmd(UART4,ENABLE);
//	delay_ms(20);
//	MPUData_processing();
//	USART_Cmd(UART4,DISABLE);
}


/**********************************************************
***	UART4中断服务程序
**********************************************************/
int Yaw_int;//陀螺仪整数部分
void USART1_IRQHandler_FONCTION(void) 
{
	// 校验位错误 PE
	if(LL_USART_IsActiveFlag_PE(USART1))
	{
		LL_USART_ReceiveData8(USART1);
		LL_USART_ClearFlag_PE(USART1);
	}

	// 溢出错误 ORE
	if(LL_USART_IsActiveFlag_ORE(USART1))
	{
		LL_USART_ReceiveData8(USART1);
		LL_USART_ClearFlag_ORE(USART1);
	}

	// 帧错误 FE
	if(LL_USART_IsActiveFlag_FE(USART1))
	{
		LL_USART_ReceiveData8(USART1);
		LL_USART_ClearFlag_FE(USART1);
	}
   // 空闲中断处理
   if (LL_USART_IsActiveFlag_IDLE(USART1))
   {
		HWT101_rxFrameFlag=true;
		LL_USART_ClearFlag_IDLE(USART1);
		// 调试：打印DMA当前传输计数，看有没有收到数据
		// uint32_t cnt = LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_5);
		// printf("DMA cnt: %d\r\n", cnt); // 如果收到数据，cnt会小于初始值HWT101_FIFO_SIZE
		// 关闭DMA，防止继续接收
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
		// 等待DMA完全关闭（可选，但更可靠）
		while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_5));

		// 清除 DMA 所有中断标志
		LL_DMA_ClearFlag_TC5(DMA1);
		LL_DMA_ClearFlag_HT5(DMA1);
		LL_DMA_ClearFlag_TE5(DMA1);
		LL_DMA_ClearFlag_DME5(DMA1);
		LL_DMA_ClearFlag_FE5(DMA1);

		// 重新设置内存地址
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_5, (uint32_t)HWT101_rxCmd);

		// 重新设置数据长度
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, HWT101_FIFO_SIZE);

		// 重新开启 DMA
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
   }
}

