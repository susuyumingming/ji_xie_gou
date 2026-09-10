#ifndef __ZIGBEE_H_
#define __ZIGBEE_H_

#include "sys.h"
#define 	ZIGBEE_FIFO_SIZE   128

extern __IO bool zigbee_rxFrameFlag;
extern __IO uint8_t zigbee_rxCmd[ZIGBEE_FIFO_SIZE];
extern __IO uint8_t zigbee_rxCount;

void Zigbee_nvic_init(void);

void Zigbee_clock_init(void);

void Zigbee_usart_init(void);

void Zigbee_board_init(void);

void Zigbee_processing(void);

void Zigbee_Serial_SendByte(uint8_t Byte);

void Zigbee_Serial_SendArray(uint8_t *Array, uint16_t Length);

void Zigbee_Serial_SendString(char *String);

extern int32_t Zigbee_coordinate[40]; 
void USART3_IRQHandler_FONCTION(void);                	//串口3中断服务程序
#endif


