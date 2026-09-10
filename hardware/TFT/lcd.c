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
//       LED         接          PB13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB3          //液晶屏SPI总线时钟信号
//      DC/RS        接          PB14         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
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
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 
#include "spi.h"
// #include "lv_draw_sw.h"

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
  LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPI_WriteByte(SPI1,data);
  LCD_CS_SET;	
}
void LCD_WR_REG_lvgl(u8 data)
{ 
//   LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPI_WriteByte(SPI1,data);
//   LCD_CS_SET;	
}
/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
  LCD_CS_CLR;
	 LCD_RS_SET;
   SPI_WriteByte(SPI1,data);
  LCD_CS_SET;
}

void LCD_WR_DATA_lvgl(u8 data)
{
   
   SPI_WriteByte(SPI1,data);
   
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 
void LCD_WriteRAM_Prepare_lvgl(void)
{
	LCD_WR_REG_lvgl(lcddev.wramcmd);
}	
/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
  //18Bit	
//		LCD_WR_DATA(Data >> 8);  // 发送高8位（原18位拆分逻辑删除）
//		LCD_WR_DATA(Data&0xFF);
	LCD_WR_DATA((Data>>8)&0xF8);//RED
	LCD_WR_DATA((Data>>3)&0xFC);//GREEN
	LCD_WR_DATA(Data<<3);//BLUE
	
//	LCD_WR_DATA(Data>>8);
//	LCD_WR_DATA(Data);
}

void Lcd_WriteData_16Bit_lvgl(u16 Data)
{	
  //18Bit	
	LCD_CS_CLR;
	LCD_RS_SET;
//	LCD_WR_DATA_lvgl(Data >> 8);  // 发送高8位（原18位拆分逻辑删除）
//  LCD_WR_DATA_lvgl(Data&0xFF); // 发送低8位（直接取原始低8位，无位操作）
	LCD_WR_DATA_lvgl((Data>>8)&0xF8);//RED
	LCD_WR_DATA_lvgl((Data>>3)&0xFC);//GREEN
	LCD_WR_DATA_lvgl(Data<<3);//BLUE
	LCD_CS_SET;
//	LCD_WR_DATA(Data>>8);
//	LCD_WR_DATA(Data);
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

void LCD_DrawPoint_2(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(BACK_COLOR); 
}

void LCD_DrawPoint_lvgl(u16 x,u16 y,u16 Data)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(Data); 
}
/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			Lcd_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	// GPIO_InitTypeDef  GPIO_InitStructure;
	      
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);
	
	// GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12| GPIO_Pin_13|GPIO_Pin_14| GPIO_Pin_15;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	// GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	// for(int i=0;i<100;i++)
	// {
	// 	delay_us(1000);
	// }
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
	// for(int i=0;i<50;i++)
	// {
	// 	delay_us(1000);
	// }
}

//// 已修复的 DMA2D 填充函数（无调色板参数，适配旧版库）
//void lv_gpu_dma2d_fill(lv_color_t *dest_buf, lv_coord_t dest_w, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t color)
//{
//    if(w == 0 || h == 0) return;

//    // 1. 使能时钟
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
//    DMA2D_DeInit();

//    // 2. 初始化 DMA2D
//    DMA2D_InitTypeDef DMA2D_InitStruct;
//    DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M;
//    DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
//    DMA2D_Init(&DMA2D_InitStruct);

//    // 3. 配置目标缓冲区
//    uint32_t dest_addr = (uint32_t)(dest_buf + y * dest_w + x);
//    DMA2D->OMAR = dest_addr;
//    DMA2D->OOR = dest_w - w;
//    DMA2D->OCOLR = color.full;
//    DMA2D->NLR = (h << 16) | w;

//    // 4. 启动传输（替换 DMA2D_Cmd(ENABLE)）
//    DMA2D_StartTransfer();
//    while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);
//    // 停止传输（替换 DMA2D_Cmd(DISABLE)）
//    DMA2D_AbortTransfer();
//    DMA2D_ClearFlag(DMA2D_FLAG_TC);
//}

//// 替换原有的 lv_gpu_dma2d_copy 函数，直接调用 LVGL 的软件拷贝实现
//void lv_gpu_dma2d_copy(lv_color_t *dest_buf, lv_coord_t dest_w, lv_color_t *src_buf, lv_coord_t src_w, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h)
//{
//    if(w == 0 || h == 0) return;

//    // 1. 定义源和目标区域
//    lv_area_t src_area = {0, 0, w-1, h-1};
//    lv_area_t dest_area = {x, y, x+w-1, y+h-1};

//    // 2. 调用正确的软件拷贝函数（替换 lv_draw_sw_copy）
//    lv_draw_sw_buffer_copy(
//        NULL,                     // draw_ctx 参数在你的版本中不需要，传 NULL
//        dest_buf, dest_w, &dest_area,
//        src_buf, src_w, &src_area
//    );
//}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_GPIOInit();//LCD GPIO初始化	
	SPI1_Init();
	LCD_SPI_DMA_Init();
	
	
 	LCD_RESET(); //LCD 复位
	LCD_CS_CLR;
//************* ILI9488初始化**********//	
	LCD_WR_REG(0XF7);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x82);
	
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x09);
	
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);
	
	LCD_WR_REG(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x80);
	
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0xB0);
	LCD_WR_DATA(0x11);
	
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02);
	
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x42);
	
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xc6);
	
	LCD_WR_REG(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	
	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0x36);
	LCD_WR_DATA((1<<3)|(0<<7)|(1<<6)|(1<<5));
	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x66);
	
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x41);
	LCD_WR_DATA(0x89);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x0F);
	
	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x0F);
	
	LCD_WR_REG(0x11);
	delay_ms(120);
	LCD_WR_REG(0x29);
	
  	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	// LCD_LED=1;//点亮背光	 
	// LCD_Clear(WHITE);//清全屏白色
	LCD_CS_CLR;
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   
void LCD_SetWindows_lvgl(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_CS_CLR;     
	LCD_RS_CLR;	  
	LCD_WR_REG_lvgl(lcddev.setxcmd);
//   LCD_CS_SET;
	
	// LCD_CS_CLR;   // 拉低片选，准备写数据
	LCD_RS_SET;   // 设为数据模式（RS=1）
	LCD_WR_DATA_lvgl(xStar>>8);
	LCD_WR_DATA_lvgl(0x00FF&xStar);		
	LCD_WR_DATA_lvgl(xEnd>>8);
	LCD_WR_DATA_lvgl(0x00FF&xEnd);
	// LCD_CS_SET;
	
	// LCD_CS_CLR;     
	LCD_RS_CLR;	  
	LCD_WR_REG_lvgl(lcddev.setycmd);  
	// LCD_CS_SET;
		
	// LCD_CS_CLR;   // 拉低片选，准备写数据
	LCD_RS_SET;   // 设为数据模式（RS=1）
	LCD_WR_DATA_lvgl(yStar>>8);
	LCD_WR_DATA_lvgl(0x00FF&yStar);		
	LCD_WR_DATA_lvgl(yEnd>>8);
	LCD_WR_DATA_lvgl(0x00FF&yEnd);
	// LCD_CS_SET;
	
	// LCD_CS_CLR;     
	LCD_RS_CLR;	  
	LCD_WriteRAM_Prepare_lvgl();	//开始写入GRAM			
  	LCD_CS_SET;
}  
/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
	
}	 

void LCD_Color565_to_666(u16 src_565, u8 dst_666[3])
{
    dst_666[0] = (src_565 >> 8) & 0xF8;  // RED（你的原有逻辑）
    dst_666[1] = (src_565 >> 3) & 0xFC;  // GREEN（你的原有逻辑）
    dst_666[2] = src_565 << 3;           // BLUE（你的原有逻辑）
}
