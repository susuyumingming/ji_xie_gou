#ifndef __SYS_H
#define __SYS_H	 

#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "main.h"
#include "cmsis_os.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;



/* FreeRTOS头文件 */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "event_groups.h"
//#include "queue.h"
//#include "semphr.h"

/*tft屏*/
#include "lcd.h"
#include "touch.h"
#include "gui.h"
//#include "test.h"//测试文件，基本不用管
/*延时函数*/
#include "delay.h"
/*oled屏*/
#include "OLED.h"
/*步进*/
//#include "board.h"
//#include "Emm_V5.h"
//#include "fifo.h"
//#include "usart.h"
/*控制*/
//#include "controls.h"
/*陀螺仪*/
#include "HWT101.h"
/*JY61陀螺仪*/
#include "JY61p.h"
/*视觉*/
//#include "Vision.h"
/*激光测距*/
#include "VL53.h"
//无线通讯与二维码共用usart3
/*串口中断函数只能有一个，用哪个就需要去哪边解除注释*/
/*二维码*/
//#include "QR_code.h"
/*无线通讯*/

#include "zigbee.h"

#include "debug_uart.h"

/*舵机底层，里面有延时函数底层*/
//#include "PWM.h"
/*舵机应用层*/
//#include "Servo.h"
/*按键*/
#include "Key.h"
/*led灯,一些简单io口控制*/
//#include "deng.h"
/*直流电机*/
//#include "Motor.h"
/*编码器*/
//#include "Encoder.h"
/*嗡鸣器*/
#include "Buzzer.h"
/*灰度*/
//#include "Gray.h"
//#include "Gray_data.h"
/*ps2遥控*/
#include "ps2.h"

#include "AS5600.h"
#include "MyI2C.h"

#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持

#include "Tf_SD.h"

//#include "PID.h"

#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/*******/

extern uint8_t buf_tx[51];
extern TaskHandle_t motor_task_handle;
//extern int16_t QR_code_data[3];//二维码
//extern int QR_code_OK;//发到tft屏后的标志位
//extern int mode;
//extern int zigbee_mode;

//extern int num;

//extern int32_t Speed_sum;

//// 声明为extern（在其他文件中使用）
//extern TaskHandle_t motor_task_handle;



/******/

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
//STM32H743 适配版
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  	 *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
//=========================
// STM32H743 GPIO 基地址
//=========================
#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) //0x58020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) //0x58020414
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) //0x58020814
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14) //0x58020C14
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14) //0x58021014
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14) //0x58021414
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14) //0x58021814
#define GPIOH_ODR_Addr    (GPIOH_BASE+0x14) //0x58021C14
#define GPIOI_ODR_Addr    (GPIOI_BASE+0x14) //0x58022014

#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) //0x58020010
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) //0x58020410
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) //0x58020810
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10) //0x58020C10
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10) //0x58021010
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10) //0x58021410
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10) //0x58021810
#define GPIOH_IDR_Addr    (GPIOH_BASE+0x10) //0x58021C10
#define GPIOI_IDR_Addr    (GPIOI_BASE+0x10) //0x58022010
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 

#endif











