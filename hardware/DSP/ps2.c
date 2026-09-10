/*
  ******************************************************************************
  * @说  明
  *   PS2数据定义
  *   BYTE   DATA   解释
  *   01     idle
  *   02     0x73   手柄工作模式
  *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
  *   04     data   SLCT  JOYR  JOYL  STRT   UP   RGIHT  DOWN   L
  *   05     data   L2     R2     L1    R1   /\     O     X    口
  *   06     data   右边摇杆  0x00 = 左    0xff = 右
  *   07     data   右边摇杆  0x00 = 上    0xff = 下
  *   08     data   左边摇杆  0x00 = 左    0xff = 右
  *   09     data   左边摇杆  0x00 = 上    0xff = 下
  * 
  ******************************************************************************
  */

#include "ps2.h"

//PS2手柄的输入输出口
#define DI()     LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_15)      //数据输入引脚

#define CMD_H()   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3)   //命令位高
#define CMD_L()   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3)   //命令位低

#define CS_H()   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4)   //CS拉高(别名ATT)
#define CS_L()   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4)   //CS拉低(别名ATT)

#define CLK_H()  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5)   //时钟拉高
#define CLK_L()  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5)   //时钟拉低


const  uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   //请求获取数据命令                         
static uint8_t PS2_data[9] = {0};  //接收的数据

void PS2_SetInit(void);

/**
  * @简  述  PS2初始化
  * @参  数  无
  * @返回值  无
  */
void AX_PS2_Init(void)
{
	PS2_SetInit();
}


/**
  * @简  述  PS2数据读写函数
  * @参  数  data:要写入的数据
  * @返回值  读出数据
  */
static uint8_t PS2_ReadWriteData(uint8_t data)
{
	uint8_t ref=0,res=0;
	for(ref = 0x01; ref > 0x00; ref <<= 1)
	{
		CLK_L();
		if(ref&data)
			CMD_H();
		else
			CMD_L();	
		
		delay_us(16);
		CLK_H();
//		delay_us(1);
		if(DI())
			res |= ref; 
		
		delay_us(16);		
	}
	
	CMD_H();

	//返回读出数据
    return res;	
}
//向手柄发送命令
uint8_t Data[2] = {0};
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			CMD_H();                   //输出一位控制位
		}
		else CMD_H();

		CLK_H();                        //时钟拉高
		delay_us(5);
		CLK_L();
		delay_us(5);
		CLK_H();
		if(DI())
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}
/**
  * @简  述  PS2获取按键及摇杆数值。
  * @参  数  *JoystickStruct 手柄键值结构体
  * @返回值  无
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
	uint8_t i;
	//使能手柄
	CS_L();
	delay_us(16);
//	CMD_H();
	//读取PS2数据
	for(i=0; i<9; i++)
	{
		PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
		delay_us(16);
	}
	
	//关闭使能
	CS_H();

	//数值传递
	JoystickStruct->mode = PS2_data[1];
	JoystickStruct->btn1 = ~PS2_data[3];
	JoystickStruct->btn2 = ~PS2_data[4];
	JoystickStruct->RJoy_LR = PS2_data[5];
	JoystickStruct->RJoy_UD = PS2_data[6];
	JoystickStruct->LJoy_LR = PS2_data[7];
	JoystickStruct->LJoy_UD = PS2_data[8];
}


/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L();
	delay_us(16);
  PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H();
	delay_us(16);  
}

//short poll
void PS2_ShortPoll(void)
{
	CS_L();
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H();
	delay_us(16);	
}
//进入配置
void PS2_EnterConfing(void)
{
  CS_L();
	delay_us(16);
	PS2_ReadWriteData(0x01);  
	PS2_ReadWriteData(0x43);  
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0x01);
	PS2_ReadWriteData(0x00);
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	CS_H();
	delay_us(16);
}
//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	CS_L();
	PS2_ReadWriteData(0x01);  
	PS2_ReadWriteData(0x44);  
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_ReadWriteData(0xEE); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				         //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0X00);
	CS_H();
	delay_us(16);
}
//振动设置
void PS2_VibrationMode(void)
{
	CS_L();
	delay_us(16);
	PS2_ReadWriteData(0x01);  
	PS2_ReadWriteData(0x4D);  
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0x00);
	PS2_ReadWriteData(0X01);
	CS_H();
	delay_us(16);	
}
//完成并保存配置
void PS2_ExitConfing(void)
{
  CS_L();
	delay_us(16);
	PS2_ReadWriteData(0x01);  
	PS2_ReadWriteData(0x43);  
	PS2_ReadWriteData(0X00);
	PS2_ReadWriteData(0x00);
	PS2_ReadWriteData(0x5A);
	PS2_ReadWriteData(0x5A);
	PS2_ReadWriteData(0x5A);
	PS2_ReadWriteData(0x5A);
	PS2_ReadWriteData(0x5A);
	CS_H();
	delay_us(16);
}
//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置
}
/******************* (C) 版权 2022 XTARK **************************************/
