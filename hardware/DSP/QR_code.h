#ifndef __QR_CODE_H
#define __QR_CODE_H
#include "stdio.h"	
#include "sys.h"

void QR_code_nvic_init(void);
void QR_code_clock_init(void);
void QR_code_usart_init(void);
void QR_code_board_init(void);
void QR_code_processing(void);
void QR_code_show(void);


#endif


