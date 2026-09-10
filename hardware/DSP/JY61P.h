#ifndef __JY61P_H
#define __JY61P_H

#include "sys.h"
#include <stdbool.h>


#define 	JY61P_FIFO_SIZE   128

extern __IO bool JY61P_rxFrameFlag;
extern __IO uint8_t JY61P_rxCmd[JY61P_FIFO_SIZE];
extern __IO uint8_t JY61P_rxCount;

void JY61P_nvic_init(void);
void JY61P_clock_init(void);
void JY61P_usart_init(void);
void JY61P_board_init(void);//初始化函数
void JY61_Data_processing(int8_t *JY61_Roll_high_byte,int8_t *JY61_Roll_low_byte,
							int8_t *JY61_Pitch_high_byte,int8_t *JY61_Pitch_low_byte,
							int8_t *JY61_Yaw_high_byte,int8_t *JY61_Yaw_low_byte);//数据处理函数

void JY61P(void);
void USART2_IRQHandler_FONCTION(void);
extern float JY61P_Roll;
extern float JY61P_Pitch;
extern float JY61P_Yaw;

#endif


