#ifndef __AS5600_H
#define __AS5600_H

#include "sys.h"

#define degrees_left_anterior_hip 11-1     //左前髋关节
#define degrees_left_anterior_hip_error 1060     //左前髋关节误差
#define degrees_right_anterior_hip 12-1    //右前髋关节  
#define degrees_right_anterior_hip_error 2377    //右前髋关节误差
#define degrees_left_posterior_hip 7-1     //左后髋关节
#define degrees_left_posterior_hip_error 486     //左后髋关节误差
#define degrees_right_posterior_hip 2-1    //右后髋关节
#define degrees_right_posterior_hip_error 2401    //右后髋关节误差

#define degrees_left_anterior_thigh 8-1       //左前大腿
#define degrees_left_anterior_thigh_error 8       //左前大腿误差
#define degrees_right_anterior_thigh 1-1     //右前大腿
#define degrees_right_anterior_thigh_error 2068     //右前大腿误差
#define degrees_left_posterior_thigh 6-1     //左后大腿
#define degrees_left_posterior_thigh_error 1747     //左后大腿误差
#define degrees_right_posterior_thigh 4-1    //右后大腿
#define degrees_right_posterior_thigh_error 103     //右后大腿误差

#define degrees_left_anterior_lower_leg 10-1        //左前小腿
#define degrees_left_anterior_lower_leg_error 1224        //左前小腿误差
#define degrees_right_anterior_lower_leg 9-1       //右前小腿
#define degrees_right_anterior_lower_leg_error 449       //右前小腿误差
#define degrees_left_posterior_lower_leg 5-1       //左后小腿
#define degrees_left_posterior_lower_leg_error 2939       //左后小腿误差
#define degrees_right_posterior_lower_leg 3-1      //右后小腿
#define degrees_right_posterior_lower_leg_error 1647      //右后小腿误差

extern int angular_error[12];

void AS5600_Init(void);
u16 ADC_GetValue(void);
void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t AS5600_ReadReg(uint8_t RegAddress, uint16_t *out_array);
uint16_t AS5600_GetRawAngle(uint16_t *out_array);
uint8_t AS5600_GetStatus(uint16_t *out_array);
uint8_t AS5600_GetAGC(uint16_t *out_array);
uint16_t AS5600_GetAngle(int16_t *out_array,uint8_t *buf);
uint8_t AS5600_CheckMagnet(uint16_t *out_array);
void AS5600_SetCurrentAsZero(void);
#endif
