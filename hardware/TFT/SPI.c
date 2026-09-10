//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PB5          //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB4          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PD13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB3          //液晶屏SPI总线时钟信号
//      DC/RS        接          PD14         //液晶屏数据/命令控制信号
//       RST         接          PD12         //液晶屏复位控制信号
//       CS          接          PD15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "SPI.h"
// #include "lvgl.h"
/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
  uint8_t rx_data;
  HAL_SPI_TransmitReceive(&hspi1, &Byte, &rx_data, 1, 100);  
  // LL_SPI_SetTransferSize(SPI1, 1);
  // LL_SPI_StartMasterTransfer(SPI1);
  // while(LL_SPI_IsActiveFlag_TXP(SPI1) == 0);
  // // {printf("SPI_TXP\r\n");}
  // LL_SPI_TransmitData8(SPI1, Byte);
  // while(LL_SPI_IsActiveFlag_RXP(SPI1) == 0);
  // // {printf("SPI_RXP\r\n");}
  // rx_data = LL_SPI_ReceiveData8(SPI1);
  
  // while(LL_SPI_IsActiveFlag_EOT(SPI1) == 0);
  // // {printf("SPI_EOT\r\n");}
  // LL_SPI_ClearFlag_EOT(SPI1);
  // LL_SPI_ClearFlag_TXTF(SPI1);
  // printf("spi 1 byte:0x%02X,recv:0x%02X\r\n", Byte, rx_data);
  return rx_data;
} 

/*****************************************************************************
 * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
 * @date       :2018-08-09 
 * @function   :Set hardware SPI Speed
 * @parameters :SPIx: SPI type,x for 1,2,3
                SpeedSet:0-high speed
												 1-low speed
 * @retvalue   :None
******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	// SPIx->CR1&=0XFFC7;
	// if(SpeedSet==1)//高速
	// {
	// 	SPIx->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
	// }
	// else//低速
	// {
	// 	SPIx->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	// }
	// SPIx->CR1|=1<<6; //SPI设备使能
} 

/*****************************************************************************
 * @name       :void SPI1_Init(void)	
 * @date       :2018-08-09 
 * @function   :Initialize the STM32 hardware SPI1
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SPI1_Init(void)	
{
// 	GPIO_InitTypeDef  GPIO_InitStructure;
//   SPI_InitTypeDef  SPI_InitStructure;
	
//    // 使能GPIOA（PA5）和GPIOB（PB4、PB5）时钟，以及SPI1时钟
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
//   // --------------------------
//     // 配置PA5（SCK）引脚
//     // --------------------------
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                // PA5引脚
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // 复用功能
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // 推挽输出
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       // 100MHz速率
//     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             // 上拉
//     GPIO_Init(GPIOA, &GPIO_InitStructure);                   // 初始化GPIOA
	
// 	// --------------------------
//     // 配置PB4（MISO）和PB5（MOSI）引脚
//     // --------------------------
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;   // PB4和PB5引脚
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // 复用功能
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // 推挽输出
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       // 100MHz速率
//     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             // 上拉
//     GPIO_Init(GPIOB, &GPIO_InitStructure);                   // 初始化GPIOB
	
// // 引脚复用映射到SPI1
//     GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);  // PA5复用为SPI1_SCK
//     GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);  // PB4复用为SPI1_MISO
//     GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);  // PB5复用为SPI1_MOSI
		
// 	//这里只针对SPI口初始化
// 	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
// 	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
// 	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
// 	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	
}
void LCD_SPI_DMA_Init(void)
{
//     DMA_InitTypeDef DMA_InitStruct;
//     NVIC_InitTypeDef NVIC_InitStructure;
	
//     // 1. 使能DMA2时钟（SPI1_DMA对应DMA2）
//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
//     // 2. 复位DMA2_Stream3，清除原有配置
//     DMA_DeInit(DMA2_Stream3);
//     while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE); // 等待复位完成
    
//     // 3. 配置DMA参数
//     DMA_InitStruct.DMA_Channel = DMA_Channel_3;          // SPI1专属通道
//     DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&SPI1->DR; // 外设基地址：SPI1数据寄存器
//     DMA_InitStruct.DMA_Memory0BaseAddr = 0;                 // 内存地址动态指定
//     DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;    // 内存→外设（SPI发送）
//     DMA_InitStruct.DMA_BufferSize = 0;                      // 传输长度动态指定
//     DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增
//     DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;    // 内存地址递增（像素数据连续）
//     DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设8位
//     DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;       // 内存8位
//     DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;              // 普通模式（一次传输）
//     DMA_InitStruct.DMA_Priority = DMA_Priority_High;        // 高优先级
//     DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;     // 关闭FIFO
//     DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//     DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//     DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//     DMA_Init(DMA2_Stream3, &DMA_InitStruct);
    
// 		DMA_ITConfig(DMA2_Stream3,DMA_IT_TC,ENABLE);
		
// 		// 2. 接收DMA配置（Stream0）
//     DMA_DeInit(DMA2_Stream0);
//     DMA_InitStruct.DMA_Channel = DMA_Channel_3;
//     DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
//     DMA_InitStruct.DMA_Memory0BaseAddr = 0;  // 虚拟接收缓冲区
//     DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
//     DMA_InitStruct.DMA_BufferSize = 0;
//     DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//     DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
//     DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//     DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//     DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
//     DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
//     DMA_Init(DMA2_Stream0, &DMA_InitStruct);
		
// 		// 启用接收完成中断
//     DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
		
// 		// 5. 配置NVIC中断（DMA2_Stream3_IRQn）
//     NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn; // DMA2流3中断通道
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级1（高于LVGL的2/3）
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      // 子优先级0
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         // 使能中断通道
//     NVIC_Init(&NVIC_InitStructure);
		
// 		// 接收DMA中断（低优先级）
//     NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
		
//     // 4. 启用SPI DMA
//     SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
//     SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
// }
//void LCD_Color565_to_666(u16 src_565, u8 dst_666[3])
//{
//    // 严格复用你原有拆分逻辑，保证颜色一致
//    dst_666[0] = (src_565 >> 8) & 0xF8;  // RED 6位（你原有逻辑）
//    dst_666[1] = (src_565 >> 3) & 0xFC;  // GREEN 6位（你原有逻辑）
//    dst_666[2] = src_565 << 3;           // BLUE 6位（你原有逻辑）
//}
//static u8 rgb666_buf[480*16*3]; 
//// lcd.c 中SPI1_DMA数据发送函数（固定代码，无需修改）
//void LCD_SPI_DMA_Send(u16 *pBuf_565, u32 pixel_num)
//{
//		 // 1. 计算18位RGB666总字节数（1像素=3字节）
//    u32 total_byte = pixel_num * 3;
//    // 2. 定义临时缓冲区（存储转换后的3字节/像素数据，需确保内存足够，这里用静态数组适配常规屏幕）
//    // 若屏幕分辨率>320*240，可改为动态内存或全局数组，示例适配320*240（320*240*3=230400字节）
//    
//    u32 i;
//    
//    // 3. 批量转换：16位RGB565 → 18位RGB666（逐像素转换，复用你的拆分规则）
//    for(i=0; i<pixel_num; i++)
//    {
//        LCD_Color565_to_666(pBuf_565[i], &rgb666_buf[i*3]);
//    }
//    // 1. 关闭DMA流，重新配置
//    DMA_Cmd(DMA2_Stream3, DISABLE);
//    // 2. 设置传输长度和内存首地址
//    DMA_SetCurrDataCounter(DMA2_Stream3, total_byte);
//    // 替换 DMA_Memory0BaseAddrConfig，直接操作寄存器
//    DMA2_Stream3->M0AR = (u32)rgb666_buf;
//    // 3. 清除传输完成标志（避免旧标志干扰）
//    DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
//    // 4. 启用DMA，开始传输
//    DMA_Cmd(DMA2_Stream3, ENABLE);
//    // 5. 阻塞等待传输完成（DMA硬件传输，耗时极短）
//    while(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);
//    // 6. 传输完成，关闭DMA并清除标志
//    DMA_Cmd(DMA2_Stream3, DISABLE);
//    DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
}
uint8_t dma_transfer_busy = 0;
void LCD_SPI_DMA_Send(u8 *pBuf, u32 len)
{
//		if(dma_transfer_busy || len == 0 || pBuf == NULL) return; // 忙则直接返回，避免重入
//		dma_transfer_busy = 1; // 置位：DMA开始传输，标记忙

  // 1. 关闭DMA流，重新配置
  LL_DMA_DisableStream(DMA2,LL_DMA_STREAM_0);
  while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_0) != 0);

  // 2. 动态设置：传输长度 + 内存地址
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, len);
  LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_0, (uint32_t)pBuf);

  // 3. 清除DMA传输完成标志
  LL_DMA_ClearFlag_TC3(DMA2);

  // 4. 启动DMA传输
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);

//	// 5. 阻塞等待传输完成（DMA硬件传输，耗时极短）
//    while(DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);
//    // 6. 传输完成，关闭DMA并清除标志
//    DMA_Cmd(DMA2_Stream3, DISABLE);
//    DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
}


