#include "usart.h"

/**********************************************************
***	Emm_V5.0步进闭环控制例程
***	编写作者：ZHANGDATOU
***	技术支持：张大头闭环伺服
***	淘宝店铺：https://zhangdatou.taobao.com
***	CSDN博客：http s://blog.csdn.net/zhangdatou666
***	qq交流群：262438510
**********************************************************/

/**
	* @brief   USART1中断函数
	* @param   无
	* @retval  无
	*/
int RxState_1;
int Serial_RxFlag_1;
int pRxPacket_1;

unsigned int Serial_RxPacket_1[10];
int32_t clk_corret=0;

// 任务句柄
extern TaskHandle_t motor_task_handle;
uint32_t g_idle_interrupt_count=0;
void USART1_IRQHandler(void)
{
	// 错误处理
	if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET) {
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_PE);
	}
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET) {
			USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_FE);
	}

/**********************************************************
***	串口空闲中断
**********************************************************/
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// 先读SR再读DR，清除IDLE中断
		volatile uint16_t temp;
		temp = USART1->SR; 
		temp = USART1->DR;
		(void)temp;  // 避免编译器警告
		
		// 关闭DMA，防止继续接收
		DMA_Cmd(DMA2_Stream5, DISABLE);
		// 等待DMA完全关闭（可选，但更可靠）
    	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);
		
		// 计算接收到的数据长度
		Emm_V5_rxCount = Emm_V5_FIFO_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);
		Emm_V5_rxFrameFlag = true;  // 设置帧接收完成标志
		
		// 清除DMA所有标志
		DMA_ClearFlag(DMA2_Stream5, 
				 DMA_FLAG_TCIF5  // 传输完成标志
				 | DMA_FLAG_HTIF5  // 半传输标志
				 | DMA_FLAG_TEIF5  // 传输错误标志
				 | DMA_FLAG_DMEIF5 // 直接模式错误标志
				 | DMA_FLAG_FEIF5); // FIFO错误标志
		// 重新设置内存地址和传输计数器
		DMA2_Stream5->M0AR = (uint32_t)Emm_V5_rxCmd;
		DMA_SetCurrDataCounter(DMA2_Stream5, Emm_V5_FIFO_SIZE);
		
		// ====================== 重点修改：任务通知（中断版） ======================
		if(motor_task_handle != NULL)  // 你说任务句柄是 motor_task_handle
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			// 任务通知发送（替代 xSemaphoreGiveFromISR）
			xTaskNotifyFromISR(
				motor_task_handle,
				1,                  // 给一个标记
				eSetBits,           // 模式和 xTaskNotifyWait 匹配
				&xHigherPriorityTaskWoken
   			 );
//			vTaskNotifyGiveFromISR(motor_task_handle, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		
//		// 通知任务 - 使用任务通知
//		if(motor_task_handle != NULL) {
//				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//        // 给信号量“发令牌”，唤醒等待的任务
//        xSemaphoreGiveFromISR(motor_task_handle, &xHigherPriorityTaskWoken);
//        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
////			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
////			xTaskNotifyFromISR(motor_task_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
////			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		}
//		g_idle_interrupt_count++;
		// 重新使能DMA
		DMA_Cmd(DMA2_Stream5, ENABLE);
	}
}

/**
  * @brief  DMA2_Stream7中断服务函数
  * @note   处理USART1_TX DMA传输完成中断
  * @retval None
  */
void USARTx_DMA_TC_IRQHandler(void)
{
    // 检查DMA传输完成标志
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
    {
        // 清除传输完成中断标志
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
        
        // 通知任务 - 使用任务通知
        if(motor_task_handle != NULL) {
//            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//            xTaskNotifyFromISR(motor_task_handle, 2, eNoAction, &xHigherPriorityTaskWoken);
//            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
					BaseType_t xHigherPriorityTaskWoken = pdFALSE;
					// 给信号量“发令牌”，唤醒等待的任务
					xSemaphoreGiveFromISR(motor_task_handle, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

void Serial_SendByte_1(uint16_t Byte);

/**
	* @brief   USART发送多个字节（非阻塞）
	* @param   cmd：命令数据
	* @param   len：数据长度
	* @retval  true：发送成功，false：发送失败
	*/
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
//	// 1. 关闭DMA2_Stream7，必须等待流真正关闭！！！
//	DMA_Cmd(DMA2_Stream7, DISABLE);
//	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);// 关键：等待关闭完成
//	
	// 2. 清除所有DMA2_Stream7标志（避免残留状态）
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
	
	// 3. 配置DMA发送参数
	DMA_SetCurrDataCounter(DMA2_Stream7, len);                  // 设置长度
	DMA2_Stream7->M0AR = (uint32_t)cmd; // 内存地址
	
	// 4. 开启DMA流
	DMA_Cmd(DMA2_Stream7, ENABLE);
	
	// 关键修改：死等DMA发送完成（核心改动，仅新增这几行）
	// 等待DMA传输完成标志置位
	while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)	{}
	// 清除传输完成标志（可选，但建议加，避免残留）
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	// 发送完成后关闭DMA（新增，避免下次调用冗余操作）
 	DMA_Cmd(DMA2_Stream7, DISABLE);
	// 5. 非阻塞，直接返回
//	return true;
}

/**
	* @brief   USART发送多个字节（阻塞）
	* @param   cmd：命令数据
	* @param   len：数据长度
	* @retval  true：发送成功，false：发送失败
	*/
//bool usart_SendCmd_Blocking(__IO uint8_t *cmd, uint8_t len)
//{
//	if(!usart_SendCmd(cmd, len))
//	{
//		return false;
//	}
//	
//	// 等待发送完成 - 使用查询方式
//	uint32_t start = xTaskGetTickCount();
//	while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
//	{
//		if(xTaskGetTickCount() - start > pdMS_TO_TICKS(100))
//		{
//			return false;
//		}
//	}
//	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
//	
//	return true;
//}

/**
	* @brief   批量发送命令
	* @param   cmds：命令数组
	* @param   lens：每个命令的长度数组
	* @param   count：命令数量
	* @retval  true：发送成功，false：发送失败
	*/
//bool usart_SendBatchCmds(uint8_t **cmds, uint8_t *lens, uint8_t count)
//{
//	for(uint8_t i = 0; i < count; i++)
//	{
//		if(!usart_SendCmd_Blocking(cmds[i], lens[i]))
//		{
//			return false;
//		}
//	}
//	return true;
//}

/**
	* @brief   USART发送一个字节
	* @param   无
	* @retval  无
	*/
void usart_SendByte(uint16_t data)
{
	__IO uint16_t t0 = 0;
	
	USART1->DR = (data & (uint16_t)0x01FF);

	while(!(USART1->SR & USART_FLAG_TXE))
	{
		++t0; if(t0 > 8000) { return; }
	}
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte_1(uint16_t Byte)
{
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}



/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t *Array, uint8_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte_1(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：等待接收完成
  * 参    数：timeout_ms：超时时间（毫秒）
  * 返 回 值：true：接收成功，false：超时
  */
bool usart_WaitForReceive(uint32_t timeout_ms)
{
	// 使用任务通知
	if(motor_task_handle != NULL)
	{
		return ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(timeout_ms)) > 0;
	}
	return false;
}
