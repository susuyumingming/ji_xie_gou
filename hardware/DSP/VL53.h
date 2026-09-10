#ifndef __VL53_H
#define __VL53_H
#include "stdio.h"	
#include "sys.h"

#define 	VL53_FIFO_SIZE   128

extern __IO bool VL53_rxFrameFlag_6;
extern  char VL53_rxCmd_6[VL53_FIFO_SIZE];
extern __IO bool VL53_rxFrameFlag_7;
extern  char VL53_rxCmd_7[VL53_FIFO_SIZE];
extern __IO bool VL53_rxFrameFlag_8;
extern  char VL53_rxCmd_8[VL53_FIFO_SIZE];

void VL53_nvic_init(void);
void VL53_clock_init(void);
void VL53_usart_init(void);
void VL53_board_init_6(void);//初始化函数
void VL53_board_init_7(void);//初始化函数
void VL53_board_init_8(void);//初始化函数
void VL53_data_processing_6(int8_t *VL53_high_byte,int8_t *VL53_low_byte);//数据处理函数
void VL53_data_processing_7(int8_t *VL53_high_byte,int8_t *VL53_low_byte);//数据处理函数
void VL53_data_processing_8(int8_t *VL53_high_byte,int8_t *VL53_low_byte);//数据处理函数
void USART6_IRQHandler_FONCTION(void);
void UART7_IRQHandler_FONCTION(void);
void UART8_IRQHandler_FONCTION(void);
extern int16_t VL53_Distance_6;			//定义接收边框坐标数据包标志位
extern char VL53_extracted_str_6[10];//length + 1
extern int16_t VL53_Distance_7;			//定义接收边框坐标数据包标志位
extern char VL53_extracted_str_7[10];//length + 1
extern int16_t VL53_Distance_8;			//定义接收边框坐标数据包标志位
extern char VL53_extracted_str_8[10];//length + 1

#endif


