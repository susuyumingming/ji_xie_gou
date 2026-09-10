#ifndef __VISION_H
#define __VISION_H
#include "stdio.h"	
#include "sys.h"

void Vision_nvic_init(void);
void Vision_clock_init(void);
void Vision_usart_init(void);
void Vision_board_init(void);
int Vision_processing(void);
extern int16_t coordinate[40]; 
void Vision_Serial_SendByte(uint16_t Byte);
void Vision_Serial_SendArray(uint16_t *Array, uint16_t Length);
void Vision_Serial_SendString(char *String);

extern uint8_t Vision_RxFlag;			//定义接收边框坐标数据包标志位
extern __IO uint8_t HWT101_rxCmd[FIFO_SIZE];

extern char Vision_RxPacket[40];				//定义接收数据包数组，数据包格式
#endif


