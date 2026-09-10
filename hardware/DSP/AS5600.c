#include "AS5600.h"
#define AS5600_ADDRESS          0x36 << 1  // AS5600的I2C从机地址（左移1位）
#define AS5600_REG_ZPOS_H 		0x01      // 起始位置11-8
#define AS5600_REG_ZPOS_L 		0x02      // 起始位置7-0
#define AS5600_REG_MPOS_H     	0x03      // 终止位置11-8
#define AS5600_REG_MPOS_L     	0x04      // 终止位置7-0
#define AS5600_REG_MANG_H     	0x05      // 最大角度11-8
#define AS5600_REG_MANG_L     	0x06      // 最大角度7-0
#define AS5600_REG_CFG_1     	0x07      // CONF高位寄存器
#define AS5600_REG_CFG_2     	0x08      // CONF低位寄存器
#define AS5600_REG_RAW_ANGLE_H 	0x0C      // 原始角度11-8(读)
#define AS5600_REG_RAW_ANGLE_L 	0x0D      // 原始角度7-0(读)
#define AS5600_REG_ANGLE_H     	0x0E      // 角度11-8(读)
#define AS5600_REG_ANGLE_L     	0x0F      // 角度7-0(读)
#define AS5600_REG_STATUS      	0x0B      // 状态寄存器(读)
#define AS5600_REG_AGC         	0x1A      // AGC寄存器(读)
#define AS5600_REG_MAGNITUDE_1  0x1B      // MAGNITUDE寄存器(读)
#define AS5600_REG_MAGNITUDE_2  0x1C      // MAGNITUDE寄存器(读)

int angular_error[12]={0};

void AS5600_data_Init(void)
{
  angular_error[degrees_left_anterior_hip]=degrees_left_anterior_hip_error;
  angular_error[degrees_right_anterior_hip]=degrees_right_anterior_hip_error;
  angular_error[degrees_left_posterior_hip]=degrees_left_posterior_hip_error;
  angular_error[degrees_right_posterior_hip]=degrees_right_posterior_hip_error;

  angular_error[degrees_left_anterior_thigh]=degrees_left_anterior_thigh_error;
  angular_error[degrees_right_anterior_thigh]=degrees_right_anterior_thigh_error;
  angular_error[degrees_left_posterior_thigh]=degrees_left_posterior_thigh_error;
  angular_error[degrees_right_posterior_thigh]=degrees_right_posterior_thigh_error;

  angular_error[degrees_left_anterior_lower_leg]=degrees_left_anterior_lower_leg_error;
  angular_error[degrees_right_anterior_lower_leg]=degrees_right_anterior_lower_leg_error;
  angular_error[degrees_left_posterior_lower_leg]=degrees_left_posterior_lower_leg_error;
  angular_error[degrees_right_posterior_lower_leg]=degrees_right_posterior_lower_leg_error;
}

void AS5600_Init(void)
{
  MyI2C_Init();
  AS5600_data_Init();
}

// 读取ADC值函数
u16 ADC_GetValue(void)
{
	uint32_t sum=0;
	for(u8 i=0; i<10; i++)
	{
//			ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_480Cycles);
//			ADC_SoftwareStartConv(ADC1);
//			while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//			sum += ADC_GetConversionValue(ADC1);
	}
	return sum / 10;
}
 
/*
  * 函    数：AS5600写寄存器
  * 参    数：RegAddress 寄存器地址
  * 参    数：Data 要写入寄存器的数据
*/
void AS5600_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();                       // I2C起始
    MyI2C_SendByte(AS5600_ADDRESS);      // 发送从机地址（写模式）
    MyI2C_ReceiveAck();                  // 接收应答
    MyI2C_SendByte(RegAddress);           // 发送寄存器地址
    MyI2C_ReceiveAck();                  // 接收应答
    MyI2C_SendByte(Data);                // 发送要写入寄存器的数据
    MyI2C_ReceiveAck();                  // 接收应答
    MyI2C_Stop();                        // I2C终止
}
 
/*
  * 函    数：AS5600读寄存器
  * 参    数：RegAddress 寄存器地址
  * 返 回 值：读取寄存器的数据
*/
uint8_t AS5600_ReadReg(uint8_t RegAddress, uint16_t *out_array)
{
    uint8_t Data;
    
    MyI2C_Start();                       // I2C起始
    MyI2C_SendByte(AS5600_ADDRESS);      // 发送从机地址（写模式）
    MyI2C_ReceiveAck();                  // 接收应答
    MyI2C_SendByte(RegAddress);           // 发送寄存器地址
    MyI2C_ReceiveAck();                  // 接收应答
    
    MyI2C_Start();                       // I2C重复起始
    MyI2C_SendByte(AS5600_ADDRESS | 0x01); // 发送从机地址（读模式）
    MyI2C_ReceiveAck();                  // 接收应答
    Data = MyI2C_ReceiveByte(out_array);          // 接收寄存器数据
    MyI2C_SendAck(1);                    // 发送非应答，停止读取
    MyI2C_Stop();                        // I2C终止
    
    return Data;
}
 
 
/*
  * 函    数：AS5600获取原始角度
  * 参    数：无
  * 返 回 值：12位原始角度值（0-4095）
*/
uint16_t AS5600_GetRawAngle(uint16_t *out_array)
{
    uint16_t AngleH[12], AngleL[12];
    uint16_t i=0;
    uint16_t RawAngle;
    // 读取角度高位寄存器（0x0E）
    AS5600_ReadReg(AS5600_REG_ANGLE_H,AngleH);
    // 读取角度低位寄存器（0x0F）
    AS5600_ReadReg(AS5600_REG_ANGLE_L,AngleL);
    for(i=0; i<12; i++)
    {
      // 组合12位角度值（高4位 + 低8位）
      out_array[i] = ((uint16_t)(AngleH[i] & 0x0F) << 8) | AngleL[i];
    }
    
    return RawAngle;

}
 
/**
  * 函    数：AS5600获取状态
  * 参    数：无
  * 返 回 值：状态寄存器值
  * 说    明：bit5(MD)=1表示磁体在有效范围
  */
uint8_t AS5600_GetStatus(uint16_t *out_array)
{
    return AS5600_ReadReg(AS5600_REG_STATUS, out_array);
}
 
/**
  * 函    数：AS5600获取AGC值
  * 参    数：无
  * 返 回 值：AGC值（0-255）
  * 说    明：反映磁场强度，理想值100-150
  */
uint8_t AS5600_GetAGC(uint16_t *out_array)
{
    return AS5600_ReadReg(AS5600_REG_AGC, out_array);
}
 
/**
  * 函    数：AS5600获取处理后的角度
  * 参    数：无
  * 返 回 值：12位角度值（0-4095）
  * 说    明：经过滤波和零位校准后的角度值
  */
uint16_t AS5600_GetAngle(int16_t *out_array,uint8_t *buf)
{
    uint16_t AngleH[12], AngleL[12];
    uint16_t Angle;
    // 读取角度高位寄存器（0x0E）
    AS5600_ReadReg(AS5600_REG_ANGLE_H,AngleH);
    // 读取角度低位寄存器（0x0F）
    AS5600_ReadReg(AS5600_REG_ANGLE_L,AngleL);
    // for(i=0; i<12; i++)
    // {
    //   // 组合12位角度值（高4位 + 低8位）
    //   out_array[i] = ((uint16_t)(AngleH[i] & 0x0F) << 8) | AngleL[i];
    // }
    out_array[degrees_left_anterior_hip] =-((((uint16_t)(AngleH[degrees_left_anterior_hip] & 0x0F) << 8) | AngleL[degrees_left_anterior_hip])- angular_error[degrees_left_anterior_hip]);
    out_array[degrees_right_anterior_hip] = (((uint16_t)(AngleH[degrees_right_anterior_hip] & 0x0F) << 8) | AngleL[degrees_right_anterior_hip])- angular_error[degrees_right_anterior_hip];
    out_array[degrees_left_posterior_hip] = (((uint16_t)(AngleH[degrees_left_posterior_hip] & 0x0F) << 8) | AngleL[degrees_left_posterior_hip])- angular_error[degrees_left_posterior_hip];
    out_array[degrees_right_posterior_hip] = -((((uint16_t)(AngleH[degrees_right_posterior_hip] & 0x0F) << 8) | AngleL[degrees_right_posterior_hip])- angular_error[degrees_right_posterior_hip]);
    
    out_array[degrees_left_anterior_thigh] = 4096-((((uint16_t)(AngleH[degrees_left_anterior_thigh] & 0x0F) << 8) | AngleL[degrees_left_anterior_thigh]));//- angular_error[degrees_left_anterior_thigh]
    if(out_array[degrees_left_anterior_thigh] >=4080)
    {
      out_array[degrees_left_anterior_thigh]=out_array[degrees_left_anterior_thigh]-4080;
    }
    else
    {
      out_array[degrees_left_anterior_thigh]=out_array[degrees_left_anterior_thigh]+4096-4080;
    }
    out_array[degrees_right_anterior_thigh] = (((uint16_t)(AngleH[degrees_right_anterior_thigh] & 0x0F) << 8) | AngleL[degrees_right_anterior_thigh])- angular_error[degrees_right_anterior_thigh];
    out_array[degrees_left_posterior_thigh] = (((uint16_t)(AngleH[degrees_left_posterior_thigh] & 0x0F) << 8) | AngleL[degrees_left_posterior_thigh])- angular_error[degrees_left_posterior_thigh];
    out_array[degrees_right_posterior_thigh] = 4096-((((uint16_t)(AngleH[degrees_right_posterior_thigh] & 0x0F) << 8) | AngleL[degrees_right_posterior_thigh]));//- angular_error[degrees_right_posterior_thigh]
    if(out_array[degrees_right_posterior_thigh] >=3980)
    {
      out_array[degrees_right_posterior_thigh]=out_array[degrees_right_posterior_thigh]-3980;
    }
    else
    {
      out_array[degrees_right_posterior_thigh]=out_array[degrees_right_posterior_thigh]+4096-3980;
    }
    out_array[degrees_left_anterior_lower_leg] =-((((uint16_t)(AngleH[degrees_left_anterior_lower_leg] & 0x0F) << 8) | AngleL[degrees_left_anterior_lower_leg])- angular_error[degrees_left_anterior_lower_leg]);
    out_array[degrees_right_anterior_lower_leg] = (((uint16_t)(AngleH[degrees_right_anterior_lower_leg] & 0x0F) << 8) | AngleL[degrees_right_anterior_lower_leg])- angular_error[degrees_right_anterior_lower_leg];
    out_array[degrees_left_posterior_lower_leg] = (((uint16_t)(AngleH[degrees_left_posterior_lower_leg] & 0x0F) << 8) | AngleL[degrees_left_posterior_lower_leg])- angular_error[degrees_left_posterior_lower_leg];
    out_array[degrees_right_posterior_lower_leg] = -((((uint16_t)(AngleH[degrees_right_posterior_lower_leg] & 0x0F) << 8) | AngleL[degrees_right_posterior_lower_leg])- angular_error[degrees_right_posterior_lower_leg]);
    buf[17] = (out_array[degrees_left_anterior_hip]>>8) & 0xFF;  buf[16] = out_array[degrees_left_anterior_hip]& 0xFF;
    buf[19] = (out_array[degrees_right_anterior_hip]>>8) & 0xFF;  buf[18] = out_array[degrees_right_anterior_hip]& 0xFF;
    buf[21] = (out_array[degrees_left_posterior_hip]>>8) & 0xFF;  buf[20] = out_array[degrees_left_posterior_hip]& 0xFF;
    buf[23] = (out_array[degrees_right_posterior_hip]>>8) & 0xFF;  buf[22] = out_array[degrees_right_posterior_hip]& 0xFF;
    buf[25] = (out_array[degrees_left_anterior_thigh]>>8) & 0xFF;  buf[24] = out_array[degrees_left_anterior_thigh]& 0xFF;
    buf[27] = (out_array[degrees_right_anterior_thigh]>>8) & 0xFF;  buf[26] = out_array[degrees_right_anterior_thigh]& 0xFF;
    buf[29] = (out_array[degrees_left_posterior_thigh]>>8) & 0xFF;  buf[28] = out_array[degrees_left_posterior_thigh]& 0xFF;
    buf[31] = (out_array[degrees_right_posterior_thigh]>>8) & 0xFF;  buf[30] = out_array[degrees_right_posterior_thigh]& 0xFF;
    buf[33] = (out_array[degrees_left_anterior_lower_leg]>>8) & 0xFF;  buf[32] = out_array[degrees_left_anterior_lower_leg]& 0xFF;
    buf[35] = (out_array[degrees_right_anterior_lower_leg]>>8) & 0xFF;  buf[34] = out_array[degrees_right_anterior_lower_leg]& 0xFF;
    buf[37] = (out_array[degrees_left_posterior_lower_leg]>>8) & 0xFF;  buf[36] = out_array[degrees_left_posterior_lower_leg]& 0xFF;
    buf[39] = (out_array[degrees_right_posterior_lower_leg]>>8) & 0xFF;  buf[38] = out_array[degrees_right_posterior_lower_leg]& 0xFF;
    return Angle;
}
 
/**
  * 函    数：AS5600检查磁体状态
  * 参    数：无
  * 返 回 值：1-磁体正常，0-磁体丢失或太弱
  */
uint8_t AS5600_CheckMagnet(uint16_t *out_array)
{
    uint8_t status = AS5600_GetStatus(out_array);
    // 检查状态寄存器的bit5(MD)
    return (status & 0x20) ? 1 : 0;
}
 // 解锁 AS5600 寄存器写入（必须加！）
void AS5600_Unlock_Write(void)
{
    // MyI2C_Start();
    // MyI2C_SendByte(0x36 << 1);
    // MyI2C_SendByte(0x07);       // CONF 寄存器
    // MyI2C_SendByte(0x00);       // 清空 CONF，允许写入 ZPOS
    // MyI2C_Stop();
    // delay_ms(10);
    AS5600_WriteReg(AS5600_REG_ZPOS_H, 0x00);
    AS5600_WriteReg(AS5600_REG_ZPOS_L, 0x00);

}
 // 写零点 ZPOS (0x01 0x02)
void AS5600_WriteZero(u16 raw)
{
    // 先解锁！！！
    AS5600_Unlock_Write();

    // MyI2C_Start();
    // MyI2C_SendByte(0x36 << 1);
    // MyI2C_SendByte(0x01);          // ZPOS 高8位
    // MyI2C_SendByte( (raw>>8) & 0x0F );
    // MyI2C_Stop();
    AS5600_WriteReg(AS5600_REG_ZPOS_H, (raw>>8) & 0x0F);
    AS5600_WriteReg(AS5600_REG_ZPOS_L, raw & 0xFF);
    
    // MyI2C_Start();
    // MyI2C_SendByte(0x36 << 1);
    // MyI2C_SendByte(0x02);          // ZPOS 低8位
    // MyI2C_SendByte( raw & 0xFF );
    // MyI2C_Stop();
}

// 把当前角度设为 0°
void AS5600_SetCurrentAsZero(void)
{
    // u16 now_raw = AS5600_GetRawAngle();
    // AS5600_WriteZero(now_raw);
}




