#include "Servo.h"

/**
  * 函    数：舵机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Servo_Init(u16 arr,u16 psc)
{
	PWM_Init_TIM8(arr,psc);	
}

/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
  */
void Servo_SetAngle_1(float Angle)
{
	PWM_SetCompare1(Angle / 180 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

void Servo_SetAngle_2(float Angle)
{
	PWM_SetCompare2(Angle / 270 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

void Servo_SetAngle_zuazi(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}



void Scara_left(float Angle)
{
	if(Angle<=0)
	{
		Angle=0;
	}
	PWM_SetCompare1( 2000 - Angle + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

void Scara_right(float Angle)
{
	if(Angle<=0)
	{
		Angle=0;
	}
	PWM_SetCompare2(Angle + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

void Scara_SetAngle_left(float Angle)
{

	Scara_left(Angle);
}

void Scara_SetAngle_right(float Angle)
{

	Scara_right(Angle);
}

int delta_right=0;
int delta_left=0;

int PWM_Pulse_right_late=0;
int PWM_Pulse_left_late=0;

void Scara_SetAngle_right_left(float Angle_right,float Angle_left,int steps,int time)
{
	float delta_right = Angle_right - PWM_Pulse_right_late;
	float delta_left = Angle_left - PWM_Pulse_left_late;
	for (int i = 0; i <= steps; i++) 
	{
			float t = (float)i / steps; // 计算插值比例，0.0到1.0
			float angle_right = PWM_Pulse_right_late + delta_right * t;
			Scara_SetAngle_right(angle_right);
			float angle_left = PWM_Pulse_left_late + delta_left * t;
			Scara_SetAngle_left(angle_left);
			delay_ms(time); // 转换为微秒
	}
	PWM_Pulse_right_late=Angle_right;
	PWM_Pulse_left_late=Angle_left;
}
