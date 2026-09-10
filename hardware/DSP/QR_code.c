#include "QR_code.h"

/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void QR_code_nvic_init(void)
{	
	// 4bit抢占优先级位
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void QR_code_clock_init(void)
{
	// 使能GPIOA、AFIO外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟

	// 使能USART1外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA10复用为USART1

}


/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void QR_code_usart_init(void)
{
/**********************************************************
***	初始化USART1引脚
**********************************************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	// PD8 - USART3_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// PC11 - USART1_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					/* 浮空输入 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

/**********************************************************
***	初始化USART3
**********************************************************/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);

/**********************************************************
***	清除USART1中断
**********************************************************/
	USART3->SR; USART3->DR;
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);

/**********************************************************
***	使能USART1中断
**********************************************************/	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

/**********************************************************
***	使能USART1
**********************************************************/
	USART_Cmd(USART3, ENABLE);
}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void QR_code_board_init(void)
{
	QR_code_nvic_init();
	QR_code_clock_init();
	QR_code_usart_init();
}

//接收状态

int16_t QR_code;
uint8_t Serial_RxFlag=0;			//定义接收边框坐标数据包标志位

uint8_t pRxPacket_QR_code = 0;		//边框//定义表示当前接收数据位置的静态变量
char Serial_RxPacket_QR_code[40];				//定义接收数据包数组，数据包格式
int16_t QR_code_data[3];
uint8_t QR_code_Data_processing=0;
uint8_t QR_code_Data_processing_2=0;
int QR_code_OK=0;
void QR_code_processing(void)
{
	if(QR_code_Data_processing==1)
	{
		for(int i=0;i<40;i++)
		{
			if(Serial_RxPacket_QR_code[i]=='+')
			{
				char extracted_str[4];
				strncpy(extracted_str, &Serial_RxPacket_QR_code[i-3], 3);
				extracted_str[3] = '\0';
				QR_code_data[0] = atoi(extracted_str);
				
				strncpy(extracted_str, &Serial_RxPacket_QR_code[i+1], 3);
				extracted_str[3] = '\0';
				QR_code_data[1] = atoi(extracted_str);
			}
		}	

		
		QR_code_Data_processing=0;
	}
//	if (Vision_RxFlag == 1)		//如果接收到数据包
//	{
//		uint8_t n=0,le=0;
//		Vision_RxFlag = 0;
//		for (int i = 0; i < 50; i++) 
//		{
//			if (Vision_RxPacket[i] == ',') 
//			{
//				int length = i - le;
//				char Vision_extracted_str[length + 1];
//				strncpy(Vision_extracted_str, &Vision_RxPacket[le], length);
//				Vision_extracted_str[length] = '\0';
//				coordinate[n] = atoi(Vision_extracted_str);
//				n = n + 1;
//				le = i + 1;
//			}
//			else if (n == 32) 
//			{
//				break;
//			}
//		}
//		Vision_RxFlag=0;
//	}	
}

void QR_code_show(void)
{
	if(QR_code_Data_processing_2==1)
	{
		char extracted_str_2[8];
		strncpy(extracted_str_2, &Serial_RxPacket_QR_code[0], 7);
		extracted_str_2[7] = '\0';

		LCD_ShowStr_256(0,26,extracted_str_2);
		Vision_Serial_SendString(extracted_str_2);
		QR_code_Data_processing_2=0;
		QR_code_OK=1;
		USART_Cmd(USART3,DISABLE);
		
	}
}


//void USART3_IRQHandler(void)                	//串口1中断服务程序
//{
//	  if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
//   {  
//     USART_ReceiveData(USART3);  
//     USART_ClearFlag(USART3, USART_FLAG_PE);  
//   }  
//      
//   if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
//   {  
//     USART_ReceiveData(USART3);  
//     USART_ClearFlag(USART3, USART_FLAG_ORE);  
//   }  
//   
//   if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
//   {  
//     USART_ReceiveData(USART3);  
//     USART_ClearFlag(USART3, USART_FLAG_FE);  
//   } 
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		QR_code=USART_ReceiveData(USART3);
//		if(QR_code=='Q')
//		{
//			Serial_RxFlag=1;
//		}
//		else if(Serial_RxFlag==1)
//		{
//			Serial_RxPacket_QR_code[pRxPacket_QR_code]=QR_code;
//			pRxPacket_QR_code++;
//		}
//		if(pRxPacket_QR_code==7)
//		{
//			pRxPacket_QR_code=0;
//			Serial_RxFlag=0;
//			QR_code_Data_processing=1;
//			QR_code_Data_processing_2=1;
//		}

//    USART_ClearFlag(USART3, USART_FLAG_RXNE);  		
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		//清除标志位
//  } 
//} 


