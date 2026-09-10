#include "MyI2C.h"

/*引脚配置层*/
void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint8_t BitAction)
{
	if(BitAction)
	{
		LL_GPIO_SetOutputPin(GPIOx, PinMask);
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOx, PinMask);
	}
}
// /**
//   * 函    数：I2C写SCL引脚电平
//   * 参    数：BitValue 协议层传入的当前需要写入SCL的电平，范围0~1
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCL为低电平，当BitValue为1时，需要置SCL为高电平
//   */
// void MyI2C_W_SCL(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOF, GPIO_Pin_11, (uint8_t)BitValue);		//根据BitValue，设置SCL引脚的电平
// 	delay_us(2);												//延时10us，防止时序频率超过要求
// }

// /**
//   * 函    数：I2C写SDA引脚电平
//   * 参    数：BitValue 协议层传入的当前需要写入SDA的电平，范围0~0xFF
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SDA为低电平，当BitValue非0时，需要置SDA为高电平
//   */
// void MyI2C_W_SDA(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOF, GPIO_Pin_12, (uint8_t)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
// 	delay_us(2);												//延时10us，防止时序频率超过要求
// }

// /**
//   * 函    数：I2C读SDA引脚电平
//   * 参    数：无
//   * 返 回 值：协议层需要得到的当前SDA的电平，范围0~1
//   * 注意事项：此函数需要用户实现内容，当前SDA为低电平时，返回0，当前SDA为高电平时，返回1
//   */
// uint8_t MyI2C_R_SDA(void)
// {
// 	uint8_t BitValue;
// 	BitValue = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_12);		//读取SDA电平
// 	delay_us(2);												//延时10us，防止时序频率超过要求
// 	return BitValue;											//返回SDA电平
// }

// /**
//   * 函    数：I2C初始化
//   * 参    数：无
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，实现SCL和SDA引脚的初始化
//   */
// void MyI2C_Init(void)
// {
// 	// 开启GPIOB时钟
//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12;
//     GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
//     GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  // I2C必须开漏
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//     GPIO_Init(GPIOF, &GPIO_InitStruct);

//     // 默认拉高释放总线
//     GPIO_SetBits(GPIOF, GPIO_Pin_11 | GPIO_Pin_12);
// }

// /*协议层*/

// /**
//   * 函    数：I2C起始
//   * 参    数：无
//   * 返 回 值：无
//   */
// void MyI2C_Start(void)
// {
// 	MyI2C_W_SDA(1);							//释放SDA，确保SDA为高电平
// 	MyI2C_W_SCL(1);							//释放SCL，确保SCL为高电平
// 	MyI2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
// 	MyI2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
// }

// /**
//   * 函    数：I2C终止
//   * 参    数：无
//   * 返 回 值：无
//   */
// void MyI2C_Stop(void)
// {
// 	MyI2C_W_SCL(0);	
// 	MyI2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
// 	MyI2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
// 	MyI2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
// }

// /**
//   * 函    数：I2C发送一个字节
//   * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
//   * 返 回 值：无
//   */
// void MyI2C_SendByte(uint8_t Byte)
// {
// 	uint8_t i;
// 	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
// 	{
// 		MyI2C_W_SDA(Byte & (0x80 >> i));	//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
// 		MyI2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
// 		MyI2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
// 	}
// }

// /**
//   * 函    数：I2C接收一个字节
//   * 参    数：无
//   * 返 回 值：接收到的一个字节数据，范围：0x00~0xFF
//   */
// uint8_t MyI2C_ReceiveByte(void)
// {
// 	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
// 	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
// 	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
// 	{
// 		MyI2C_W_SCL(1);						//释放SCL，主机机在SCL高电平期间读取SDA
// 		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
// 														//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
// 		MyI2C_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
// 	}
// 	return Byte;							//返回接收到的一个字节数据
// }

// /**
//   * 函    数：I2C发送应答位
//   * 参    数：Byte 要发送的应答位，范围：0~1，0表示应答，1表示非应答
//   * 返 回 值：无
//   */
// void MyI2C_SendAck(uint8_t AckBit)
// {
// 	MyI2C_W_SDA(AckBit);					//主机把应答位数据放到SDA线
// 	MyI2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
// 	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
// }

// /**
//   * 函    数：I2C接收应答位
//   * 参    数：无
//   * 返 回 值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
//   */
// uint8_t MyI2C_ReceiveAck(void)
// {
// 	uint8_t ucErrTime=0;
// 	uint8_t AckBit;							//定义应答位变量
// 	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
// 	MyI2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
// 	while(MyI2C_R_SDA())
// 	{
// 		ucErrTime++;
// 		if(ucErrTime>250)
// 		{
// 			MyI2C_Stop();
// 			return 1;
// 		}
// 		AckBit = 0;					//将应答位存储到变量里
// 	}
// 	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
// 	return AckBit;							//返回定义应答位变量
// }

/**
  * 函    数：I2C写SCL引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCL的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCL为低电平，当BitValue为1时，需要置SCL为高电平
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(AS5600_1_GPIO_PORT, AS5600_1_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_2_GPIO_PORT, AS5600_2_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_3_GPIO_PORT, AS5600_3_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_4_GPIO_PORT, AS5600_4_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_5_GPIO_PORT, AS5600_5_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_6_GPIO_PORT, AS5600_6_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_7_GPIO_PORT, AS5600_7_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_8_GPIO_PORT, AS5600_8_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_9_GPIO_PORT, AS5600_9_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_10_GPIO_PORT, AS5600_10_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_11_GPIO_PORT, AS5600_11_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(AS5600_12_GPIO_PORT, AS5600_12_SCL_PIN, BitValue);		//根据BitValue，设置SCL引脚的电平
	delay_us(2);												//延时10us，防止时序频率超过要求
}
void MyI2C_W_SCL_alone(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint8_t BitAction)
{
	GPIO_WriteBit(GPIOx, PinMask, BitAction);		//根据BitValue，设置SCL引脚的电平
	delay_us(2);												//延时10us，防止时序频率超过要求
}
/**
  * 函    数：I2C写SDA引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SDA的电平，范围0~0xFF
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SDA为低电平，当BitValue非0时，需要置SDA为高电平
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(AS5600_1_GPIO_PORT, AS5600_1_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_2_GPIO_PORT, AS5600_2_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_3_GPIO_PORT, AS5600_3_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_4_GPIO_PORT, AS5600_4_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_5_GPIO_PORT, AS5600_5_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_6_GPIO_PORT, AS5600_6_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_7_GPIO_PORT, AS5600_7_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_8_GPIO_PORT, AS5600_8_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_9_GPIO_PORT, AS5600_9_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_10_GPIO_PORT, AS5600_10_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_11_GPIO_PORT, AS5600_11_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	GPIO_WriteBit(AS5600_12_GPIO_PORT, AS5600_12_SDA_PIN, BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	delay_us(2);												//延时10us，防止时序频率超过要求
}
void MyI2C_W_SDA_alone(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint8_t BitAction)
{
	GPIO_WriteBit(GPIOx, PinMask, BitAction);		//根据BitValue，设置SDA引脚的电平
	delay_us(2);												//延时10us，防止时序频率超过要求
}
/**
  * 函    数：I2C读SDA引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前SDA的电平，范围0~1
  * 注意事项：此函数需要用户实现内容，当前SDA为低电平时，返回0，当前SDA为高电平时，返回1
  */
uint8_t MyI2C_R_SDA(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	uint8_t BitValue;
	BitValue = LL_GPIO_IsInputPinSet(GPIOx, PinMask);		//读取SDA电平
	delay_us(2);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}

/**
  * 函    数：I2C初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SCL和SDA引脚的初始化
  */
void MyI2C_Init(void)
{
	MyI2C_W_SCL(1);							//初始化SCL引脚为高电平
	MyI2C_W_SDA(1);							//初始化SDA引脚为高电平
}

/*协议层*/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);							//释放SDA，确保SDA为高电平
	MyI2C_W_SCL(1);							//释放SCL，确保SCL为高电平
	MyI2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
	MyI2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void MyI2C_Stop(void)
{
	MyI2C_W_SCL(0);	
	MyI2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
	MyI2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
	MyI2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
}
void MyI2C_Stop_alone(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	MyI2C_W_SCL_alone(GPIOx,PinMask,0);	
	MyI2C_W_SDA_alone(GPIOx,PinMask,0);							//拉低SDA，确保SDA为低电平
	MyI2C_W_SCL_alone(GPIOx,PinMask,1);							//释放SCL，使SCL呈现高电平
	MyI2C_W_SDA_alone(GPIOx,PinMask,1);							//在SCL高电平期间，释放SDA，产生终止信号
}
/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));	//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		MyI2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		MyI2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}

/**
  * 函    数：I2C接收一个字节
  * 参    数：无
  * 返 回 值：接收到的一个字节数据，范围：0x00~0xFF
  */
uint8_t MyI2C_ReceiveByte(uint16_t *out_array)
{
	uint8_t i;
	uint8_t Byte;
	//初始化接收数据数组
	for (i = 0; i < 12; i ++)
	{
		out_array[i] = 0x00;//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	}		
	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		MyI2C_W_SCL(1);						//释放SCL，主机机在SCL高电平期间读取SDA
		if (MyI2C_R_SDA(AS5600_1_GPIO_PORT, AS5600_1_SDA_PIN) == 1){out_array[0] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_2_GPIO_PORT, AS5600_2_SDA_PIN) == 1){out_array[1] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_3_GPIO_PORT, AS5600_3_SDA_PIN) == 1){out_array[2] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_4_GPIO_PORT, AS5600_4_SDA_PIN) == 1){out_array[3] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_5_GPIO_PORT, AS5600_5_SDA_PIN) == 1){out_array[4] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_6_GPIO_PORT, AS5600_6_SDA_PIN) == 1){out_array[5] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_7_GPIO_PORT, AS5600_7_SDA_PIN) == 1){out_array[6] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_8_GPIO_PORT, AS5600_8_SDA_PIN) == 1){out_array[7] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_9_GPIO_PORT, AS5600_9_SDA_PIN) == 1){out_array[8] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_10_GPIO_PORT, AS5600_10_SDA_PIN) == 1){out_array[9] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_11_GPIO_PORT, AS5600_11_SDA_PIN) == 1){out_array[10] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
		if (MyI2C_R_SDA(AS5600_12_GPIO_PORT, AS5600_12_SDA_PIN) == 1){out_array[11] |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量	
														//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		MyI2C_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

/**
  * 函    数：I2C发送应答位
  * 参    数：Byte 要发送的应答位，范围：0~1，0表示应答，1表示非应答
  * 返 回 值：无
  */
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);					//主机把应答位数据放到SDA线
	MyI2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

/**
  * 函    数：I2C接收应答位
  * 参    数：无
  * 返 回 值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
  */
uint8_t MyI2C_ReceiveAck(void)
{
	uint16_t ucErrTime=250;
	uint8_t ack_result[12];
	uint8_t all_ack = 1;
	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	MyI2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	while(ucErrTime--)
	{
		ack_result[0] = MyI2C_R_SDA(AS5600_1_GPIO_PORT, AS5600_1_SDA_PIN);
		ack_result[1] = MyI2C_R_SDA(AS5600_2_GPIO_PORT, AS5600_2_SDA_PIN);
		ack_result[2] = MyI2C_R_SDA(AS5600_3_GPIO_PORT, AS5600_3_SDA_PIN);
		ack_result[3] = MyI2C_R_SDA(AS5600_4_GPIO_PORT, AS5600_4_SDA_PIN);
		ack_result[4] = MyI2C_R_SDA(AS5600_5_GPIO_PORT, AS5600_5_SDA_PIN);
		ack_result[5] = MyI2C_R_SDA(AS5600_6_GPIO_PORT, AS5600_6_SDA_PIN);
		ack_result[6] = MyI2C_R_SDA(AS5600_7_GPIO_PORT, AS5600_7_SDA_PIN);
		ack_result[7] = MyI2C_R_SDA(AS5600_8_GPIO_PORT, AS5600_8_SDA_PIN);
		ack_result[8] = MyI2C_R_SDA(AS5600_9_GPIO_PORT, AS5600_9_SDA_PIN);
		ack_result[9] = MyI2C_R_SDA(AS5600_10_GPIO_PORT, AS5600_10_SDA_PIN);
		ack_result[10] = MyI2C_R_SDA(AS5600_11_GPIO_PORT, AS5600_11_SDA_PIN);
		ack_result[11] = MyI2C_R_SDA(AS5600_12_GPIO_PORT, AS5600_12_SDA_PIN);
		// 检查：只要所有路都拉低了，就退出
        all_ack = 1;
        for(int i=0;i<12;i++) if(ack_result[i]) all_ack = 0;
        if(all_ack) break;
	}
	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return all_ack;							//返回定义应答位变量
}


