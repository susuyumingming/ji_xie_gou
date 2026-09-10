#ifndef __SERVO_H
#define __SERVO_H

#include "sys.h"

void Servo_Init(u16 arr,u16 psc);

void Servo_SetAngle_1(float Angle);
void Servo_SetAngle_2(float Angle);
void Scara_SetAngle_right(float Angle);
void Scara_SetAngle_left(float Angle);
void Servo_SetAngle_zuazi(float Angle);
void Scara_SetAngle_right_left(float Angle_right,float Angle_left,int steps,int time);

#endif
