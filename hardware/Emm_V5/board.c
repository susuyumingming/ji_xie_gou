#include "board.h"
#include "usart.h"

/**********************************************************
***	Emm_V5.0???????????????
***	??§Õ?????ZHANGDATOU
***	??????????????????
***	????????https://zhangdatou.taobao.com
***	CSDN?????http s://blog.csdn.net/zhangdatou666
***	qq???????262438510
**********************************************************/

__IO bool Emm_V5_rxFrameFlag = false;
__IO uint8_t Emm_V5_rxCmd[Emm_V5_FIFO_SIZE] = {0};// DMA?????????
__IO uint8_t Emm_V5_rxCount = 0;
__IO uint8_t usart_tx_done = 1;           // ???????????1-???§µ?0-??????
/**
	* @brief   ????NVIC??????
	* @param   ??
	* @retval  ??
	*/
void nvic_init(void)
{	
	// 4bit????????¦Ë
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// USART1?§Ø???????????§Ø??
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	// DMA2_Stream7?§Ø??????????????
//	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; // ???????????????
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
}

/**
	*	@brief		???????????
	*	@param		??
	*	@retval		??
	*/
void clock_init(void)
{
	// ???GPIOA??AFIO???????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//???GPIOA???
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 

	// ???USART1???????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// ???DMA2???
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	//????1?????????????
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9?????USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10?????USART1
	

}


/**
	* @brief   ?????USART
	* @param   ??
	* @retval  ??
	*/
void usart_init(void)
{
	clock_init();
	
/**********************************************************
***	?????USART1????
**********************************************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	// PA9 - USART1_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // ?????????TX????????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// PA10 - USART1_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					/* ???????? */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/**********************************************************
***	?????USART1
**********************************************************/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);

/**********************************************************
	***	?????DMA (USART1_RX???DMA2_Stream5_Channel4)
	**********************************************************/
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA2_Stream5);
	
	// ???DMA?????
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  // USART1_RX???DMA???4
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  // ??????(????????)
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Emm_V5_rxCmd;  // ?????(?????????)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  // ???Úb??œ]??
	DMA_InitStructure.DMA_BufferSize = Emm_V5_FIFO_SIZE;  // ????????§³
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // ????????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  // ?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  // ??????
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  // ??????
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  // ???????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // ???????
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);
	
	/**********************************************************
	***	?????DMA?????USART1_TX - DMA2_Stream7_Channel4??
	**********************************************************/
	DMA_DeInit(DMA2_Stream7);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); // ????????

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = 0; // ???????
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 0; // ???????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	
/**********************************************************
***	?????§Ø?
**********************************************************/	
	nvic_init();
	
/**********************************************************
***	???USART1?§Ø?
**********************************************************/
	USART1->SR; 
	USART1->DR;
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

/**********************************************************
	***	???DMA?????UART?§Ø?
	**********************************************************/
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // ???DMA????
	DMA_Cmd(DMA2_Stream5, ENABLE);  // ???DMA??
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // ???????§Ø?
	
// ???DMA?????????????§Ø?
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE); // ??????????§Ø?
	
/**********************************************************
***	???USART1
**********************************************************/
	USART_Cmd(USART1, ENABLE);
}

/**
	* 	@brief		????????
	* 	@param		??
	* 	@retval		??
	*/
void board_init(void)
{
	usart_init();

}














