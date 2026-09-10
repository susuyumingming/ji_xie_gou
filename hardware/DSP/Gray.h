#ifndef __GRAY_H
#define __GRAY_H

#include "sys.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} PIO;


extern const PIO Pins[];
extern uint16_t HuiDu_flag;

void HuiDu_Init(void);
uint16_t Get_gray(void);
int HuiDu_compare(uint16_t num_1,uint16_t num_2);

#define HuiDu_Read(index) \
    ((uint16_t)( ( (index) >= 0 && (index) <= 10 ) ? GPIO_ReadInputDataBit(Pins[(index)].port, Pins[(index)].pin) : 0 ))



#endif

