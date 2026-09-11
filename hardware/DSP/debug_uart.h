#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H
#include "stdio.h"	
#include "sys.h"

#define 	debug_uart_FIFO_SIZE   128

extern __IO bool debug_uart_rxFrameFlag;
extern  char debug_uart_rxCmd[debug_uart_FIFO_SIZE];
extern int32_t debug_uart_coordinate[40]; 
void debug_uart_nvic_init(void);
void debug_uart_clock_init(void);
void debug_uart_usart_init(void);
void debug_uart_board_init(void);//³õÊ¼»¯º¯Êý
void debug_uartprocessing(void);
void UART5_IRQHandler_FONCTION(void);

#endif


