#ifndef __CONTROLS_H
#define __CONTROLS_H

#include "sys.h"

#define left_anterior_hip 7     //左前髋关节
#define right_anterior_hip 6    //右前髋关节  
#define left_posterior_hip 8     //左后髋关节
#define right_posterior_hip 5    //右后髋关节

#define left_anterior_thigh 9       //左前大腿
#define right_anterior_thigh 11     //右前大腿
#define left_posterior_thigh 10     //左后大腿
#define right_posterior_thigh 12     //右后大腿

#define left_anterior_lower_leg 4        //左前小腿
#define right_anterior_lower_leg 1       //右前小腿
#define left_posterior_lower_leg 3       //左后小腿
#define right_posterior_lower_leg 2      //右后小腿

/**********************************************************
***	
***	
***	
***	
***	
***	
**********************************************************/

extern float pos_robot_1,pos_robot_2,pos_robot_3,pos_robot_4,pos_robot_5,pos_robot_6,pos_robot_7,pos_robot_8; 
extern float Motor_Cur_Pos_robot_1,//右后
             Motor_Cur_Pos_robot_2,//右前
             Motor_Cur_Pos_robot_3,//左前
             Motor_Cur_Pos_robot_4,//左后
             Motor_Cur_Pos_robot_5,//上
             Motor_Cur_Pos_robot_6,//下
             Motor_Cur_Pos_robot_7,//左
             Motor_Cur_Pos_robot_8;//右
extern float pos_robot;
extern float Motor_Cur_Pos_robot[17];

typedef struct 
{
	float pos_robot;
	float Motor_Cur_Pos_robot;
    uint16_t phase_current;
    uint16_t total_current;
}Motor;

extern Motor Motor_Control[16];

void Motor_Control_Init(void);

void Read_clk(void);
void Readclk(uint8_t addr,TaskHandle_t xTaskToNotify);
void Read_phase_current(uint8_t addr,TaskHandle_t xTaskToNotify);//相电流
void Read_total_current(uint8_t addr,TaskHandle_t xTaskToNotify);//总电流
bool ZDT_Torque_Control(uint8_t addr, uint8_t sign, uint16_t t_ramp, uint16_t torque, uint8_t snF,TaskHandle_t xTaskToNotify);
bool ZDT_Synchronous_motion(uint8_t addr,TaskHandle_t xTaskToNotify);
bool ZDT_Velocity_Control(uint8_t addr, uint8_t dir, uint16_t v_ramp, float velocity, uint8_t snF,TaskHandle_t xTaskToNotify);
bool ZDT_Bypass_Position_LV_Control(uint8_t addr, uint8_t dir, float velocity, float position, uint8_t raf, uint8_t snF,TaskHandle_t xTaskToNotify);
bool ZDT_Traj_Position_Control(uint8_t addr, uint8_t dir, uint16_t acc, uint16_t dec, float velocity, float position, uint8_t raf, uint8_t snF,TaskHandle_t xTaskToNotify);

extern int mode_num;
extern int mode_num_2;
extern int hengxian_num;
extern int hengxian_flag;
extern int weizhi;

#endif
