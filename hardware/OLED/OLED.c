
///***************************************************************************************
//  * ±¾³ÌĞòÓÉ½­Ğ­¿Æ¼¼´´½¨²¢Ãâ·Ñ¿ªÔ´¹²Ïí
//  * Äã¿ÉÒÔÈÎÒâ²é¿´¡¢Ê¹ÓÃºÍĞŞ¸Ä£¬²¢Ó¦ÓÃµ½×Ô¼ºµÄÏîÄ¿Ö®ÖĞ
//  * ³ÌĞò°æÈ¨¹é½­Ğ­¿Æ¼¼ËùÓĞ£¬ÈÎºÎÈË»ò×éÖ¯²»µÃ½«Æä¾İÎª¼ºÓĞ
//  * 
//  * ³ÌĞòÃû³Æ£º				0.96´çOLEDÏÔÊ¾ÆÁÇı¶¯³ÌĞò£¨4Õë½ÅI2C½Ó¿Ú£©
//  * ³ÌĞò´´½¨Ê±¼ä£º			2023.10.24
//  * µ±Ç°³ÌĞò°æ±¾£º			V1.1
//  * µ±Ç°°æ±¾·¢²¼Ê±¼ä£º		2023.12.8
//  * 
//  * ½­Ğ­¿Æ¼¼¹Ù·½ÍøÕ¾£º		jiangxiekeji.com
//  * ½­Ğ­¿Æ¼¼¹Ù·½ÌÔ±¦µê£º	jiangxiekeji.taobao.com
//  * ³ÌĞò½éÉÜ¼°¸üĞÂ¶¯Ì¬£º	jiangxiekeji.com/tutorial/oled.html
//  * 
//  * Èç¹ûÄã·¢ÏÖ³ÌĞòÖĞµÄÂ©¶´»òÕß±ÊÎó£¬¿ÉÍ¨¹ıÓÊ¼şÏòÎÒÃÇ·´À¡£ºfeedback@jiangxiekeji.com
//  * ·¢ËÍÓÊ¼şÖ®Ç°£¬Äã¿ÉÒÔÏÈµ½¸üĞÂ¶¯Ì¬Ò³Ãæ²é¿´×îĞÂ³ÌĞò£¬Èç¹û´ËÎÊÌâÒÑ¾­ĞŞ¸Ä£¬ÔòÎŞĞèÔÙ·¢ÓÊ¼ş
//  ***************************************************************************************
//  */

#include "OLED.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>


#if (OLED_mode==Hardware)

/**
  * Êı¾İ´æ´¢¸ñÊ½£º
  * ×İÏò8µã£¬¸ßÎ»ÔÚÏÂ£¬ÏÈ´Ó×óµ½ÓÒ£¬ÔÙ´ÓÉÏµ½ÏÂ
  * Ã¿Ò»¸öBit¶ÔÓ¦Ò»¸öÏñËØµã
  * 
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |   
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  * 
  * ×ø±êÖá¶¨Òå£º
  * ×óÉÏ½ÇÎª(0, 0)µã
  * ºáÏòÏòÓÒÎªXÖá£¬È¡Öµ·¶Î§£º0~127
  * ×İÏòÏòÏÂÎªYÖá£¬È¡Öµ·¶Î§£º0~63
  * 
  *       0             XÖá           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  YÖá |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */


/*È«¾Ö±äÁ¿*********************/

/**
  * OLEDÏÔ´æÊı×é
  * ËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶Ô´ËÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  * Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  * ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  */
uint8_t OLED_DisplayBuf[8][128] __attribute__((aligned(4)));

/*********************È«¾Ö±äÁ¿*/


/*Òı½ÅÅäÖÃ*********************/

/**
  * º¯    Êı£ºOLEDĞ´SCL¸ßµÍµçÆ½
  * ²Î    Êı£ºÒªĞ´ÈëSCLµÄµçÆ½Öµ£¬·¶Î§£º0/1
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ±ÉÏ²ãº¯ÊıĞèÒªĞ´SCLÊ±£¬´Ëº¯Êı»á±»µ÷ÓÃ
  *           ÓÃ»§ĞèÒª¸ù¾İ²ÎÊı´«ÈëµÄÖµ£¬½«SCLÖÃÎª¸ßµçÆ½»òÕßµÍµçÆ½
  *           µ±²ÎÊı´«Èë0Ê±£¬ÖÃSCLÎªµÍµçÆ½£¬µ±²ÎÊı´«Èë1Ê±£¬ÖÃSCLÎª¸ßµçÆ½
  */
void OLED_W_SCL(uint8_t BitValue)
{
	/*¸ù¾İBitValueµÄÖµ£¬½«SCLÖÃ¸ßµçÆ½»òÕßµÍµçÆ½*/
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	
	/*Èç¹ûµ¥Æ¬»úËÙ¶È¹ı¿ì£¬¿ÉÔÚ´ËÌí¼ÓÊÊÁ¿ÑÓÊ±£¬ÒÔ±ÜÃâ³¬³öI2CÍ¨ĞÅµÄ×î´óËÙ¶È*/
	//...
}

/**
  * º¯    Êı£ºOLEDĞ´SDA¸ßµÍµçÆ½
  * ²Î    Êı£ºÒªĞ´ÈëSDAµÄµçÆ½Öµ£¬·¶Î§£º0/1
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ±ÉÏ²ãº¯ÊıĞèÒªĞ´SDAÊ±£¬´Ëº¯Êı»á±»µ÷ÓÃ
  *           ÓÃ»§ĞèÒª¸ù¾İ²ÎÊı´«ÈëµÄÖµ£¬½«SDAÖÃÎª¸ßµçÆ½»òÕßµÍµçÆ½
  *           µ±²ÎÊı´«Èë0Ê±£¬ÖÃSDAÎªµÍµçÆ½£¬µ±²ÎÊı´«Èë1Ê±£¬ÖÃSDAÎª¸ßµçÆ½
  */
void OLED_W_SDA(uint8_t BitValue)
{
	/*¸ù¾İBitValueµÄÖµ£¬½«SDAÖÃ¸ßµçÆ½»òÕßµÍµçÆ½*/
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	
	/*Èç¹ûµ¥Æ¬»úËÙ¶È¹ı¿ì£¬¿ÉÔÚ´ËÌí¼ÓÊÊÁ¿ÑÓÊ±£¬ÒÔ±ÜÃâ³¬³öI2CÍ¨ĞÅµÄ×î´óËÙ¶È*/
	//...
}
// È«¾Ö±äÁ¿£ºDMA·¢ËÍÍê³É±êÖ¾£¨ÖĞ¶ÏÖĞÖÃ1£¬Ö÷Ñ­»·ÖĞÇå0£©
volatile uint8_t g_I2C2_DMA_TxComplete = 0;
/**
  * º¯    Êı£ºOLEDÒı½Å³õÊ¼»¯
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ±ÉÏ²ãº¯ÊıĞèÒª³õÊ¼»¯Ê±£¬´Ëº¯Êı»á±»µ÷ÓÃ
  *           ÓÃ»§ĞèÒª½«SCLºÍSDAÒı½Å³õÊ¼»¯Îª¿ªÂ©Ä£Ê½£¬²¢ÊÍ·ÅÒı½Å
  */
void OLED_GPIO_Init(void)//PB10-->SCL || PB11-->SDA
{
	uint32_t i, j;
	
	/*ÔÚ³õÊ¼»¯Ç°£¬¼ÓÈëÊÊÁ¿ÑÓÊ±£¬´ıOLED¹©µçÎÈ¶¨*/
	for (i = 0; i < 1000; i ++)
	{
		for (j = 0; j < 1000; j ++);
	}

	/* 1. Ê¹ÄÜÊ±ÖÓ£ºGPIOB + I2C2 + DMA1 + ÖĞ¶Ï¿ØÖÆÆ÷ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // ÅäÖÃÖĞ¶Ï·Ö×é£¨°´Ğèµ÷Õû£©
 
	GPIO_InitTypeDef IIC1_IO;
	IIC1_IO.GPIO_Mode = GPIO_Mode_AF;
	IIC1_IO.GPIO_PuPd = GPIO_PuPd_UP; 
	IIC1_IO.GPIO_OType = GPIO_OType_OD;
	IIC1_IO.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	IIC1_IO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &IIC1_IO);
 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);	//ÕâÀï¾ÍÊÇF1ºÍF4µÄ²»Í¬£¡£¡£¡
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
	
	I2C_InitTypeDef IIC1;
	IIC1.I2C_Ack = I2C_Ack_Disable;	//»Ø¸´Ó¦´ğ
	IIC1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//×÷Îª´Ó»úÊ±£¬×Ô¼ºµÄµØÖ·Î»Êı
	IIC1.I2C_ClockSpeed = 400*1000;	//IICËÙÂÊ£º0-400Khz£¨ÏŞÖÆIICËÙÂÊµÄÔ­ÒòÊÇÈõÉÏÀ­£¬ÒòÎª¸ßµçÆ½ÊÇ¶ÁÈ¡Êı¾İ£¬µ±ÈõÉÏÀ­»Øµ÷²»¼°Ê±Ôò»áÓ°ÏìÊı¾İµÄ¶ÁÈ¡£©
	IIC1.I2C_DutyCycle = I2C_DutyCycle_16_9;	//Èç¹ûÉÏÃæ²ÎÊıËÙÂÊÔÚ100khz-400khz£¬ÔòÕâÀïÒª±ØĞëÒªÑ¡
	IIC1.I2C_Mode = I2C_Mode_I2C;  //Ñ¡ÔñIICÄ£Ê½
	IIC1.I2C_OwnAddress1 = 0x00;	//×÷Îª´Ó»úµÄµØÖ·
	I2C_Init(I2C2, &IIC1);						//½«½á¹¹Ìå±äÁ¿½»¸øI2C_Init£¬ÅäÖÃI2C2
	
	/* 4. DMAÅäÖÃ£¨·¢ËÍ+ÖĞ¶Ï£©ºËĞÄĞŞ¸Ä */
	DMA_InitTypeDef DMA_InitStruct;
	// I2C2_TX¶ÔÓ¦DMA1_Stream7_Channel7£¨F4ÏµÁĞ£©
	DMA_DeInit(DMA1_Stream7);
	DMA_InitStruct.DMA_Channel = DMA_Channel_7;          
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&I2C2->DR; // I2CÊı¾İ¼Ä´æÆ÷
	DMA_InitStruct.DMA_Memory0BaseAddr = 0;               // ¶¯Ì¬ÉèÖÃÄÚ´æµØÖ·
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;  // ÄÚ´æ¡úÍâÉè£¨·¢ËÍ£©
	DMA_InitStruct.DMA_BufferSize = 0;                    // ¶¯Ì¬ÉèÖÃ³¤¶È
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;            // ÆÕÍ¨Ä£Ê½£¨µ¥´Î´«Êä£©
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;    
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;   
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &DMA_InitStruct);

	/* 5. ÆôÓÃDMAÖĞ¶Ï£¨¹Ø¼ü£¡£© */
	// ÆôÓÃ´«ÊäÍê³ÉÖĞ¶Ï£¨TC£©+ ´«Êä´íÎóÖĞ¶Ï£¨TE£©
	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC | DMA_IT_TE, ENABLE);

	/* 6. ÅäÖÃDMAÖĞ¶ÏÓÅÏÈ¼¶£¨NVIC£© */
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream7_IRQn;  // DMA1_Stream7ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4; // ÇÀÕ¼ÓÅÏÈ¼¶£¨°´Ğèµ÷£©
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;        // ×ÓÓÅÏÈ¼¶
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           // ÆôÓÃÖĞ¶Ï
//	NVIC_Init(&NVIC_InitStruct);

	/* 7. ¹ØÁªDMAºÍI2C2 */
	DMA_Cmd(DMA1_Stream7, DISABLE); // ÏÈ¹Ø±Õ£¬·¢ËÍÊ±¿ªÆô
	I2C_DMACmd(I2C2, ENABLE);       // ÆôÓÃI2C2µÄDMAÇëÇó
	I2C_Cmd(I2C2, ENABLE);          // ÆôÓÃI2C2
}

/*********************Òı½ÅÅäÖÃ*/

/* ********************* ºËĞÄ£ºDMA·¢ËÍÍê³ÉÖĞ¶Ï·şÎñº¯Êı ********************* */
void DMA1_Stream7_IRQHandler(void)
{
	// 1. ¼ì²é´«ÊäÍê³ÉÖĞ¶Ï±êÖ¾
	if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TCIF7); // Çå³ıÍê³É±êÖ¾
		g_I2C2_DMA_TxComplete = 1;                         // ÖÃÎ»Íê³É±êÖ¾
		I2C_GenerateSTOP(I2C2, ENABLE);                    // ·¢ËÍÍ£Ö¹ĞÅºÅ
	}
	
	// 2. ¼ì²é´«Êä´íÎóÖĞ¶Ï±êÖ¾£¨Òì³£´¦Àí£©
	if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TEIF7) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TEIF7); // Çå³ı´íÎó±êÖ¾
		// ¿ÉÑ¡£ºÌí¼Ó´íÎó´¦ÀíÂß¼­£¨±ÈÈçÖØÆôI2C¡¢´òÓ¡ÈÕÖ¾£©
		g_I2C2_DMA_TxComplete = 2; // ÓÃ²»Í¬Öµ±ê¼Ç´íÎó
	}
	
	// 3. ¹Ø±ÕDMAÁ÷£¨×¼±¸ÏÂÒ»´Î´«Êä£©
	DMA_Cmd(DMA1_Stream7, DISABLE);
}

/*Í¨ĞÅĞ­Òé*********************/

/**
  * º¯    Êı£ºI2CÆğÊ¼
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);		//ÊÍ·ÅSDA£¬È·±£SDAÎª¸ßµçÆ½
	OLED_W_SCL(1);		//ÊÍ·ÅSCL£¬È·±£SCLÎª¸ßµçÆ½
	OLED_W_SDA(0);		//ÔÚSCL¸ßµçÆ½ÆÚ¼ä£¬À­µÍSDA£¬²úÉúÆğÊ¼ĞÅºÅ
	OLED_W_SCL(0);		//ÆğÊ¼ºó°ÑSCLÒ²À­µÍ£¬¼´ÎªÁËÕ¼ÓÃ×ÜÏß£¬Ò²ÎªÁË·½±ã×ÜÏßÊ±ĞòµÄÆ´½Ó
}

/**
  * º¯    Êı£ºI2CÖÕÖ¹
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);		//À­µÍSDA£¬È·±£SDAÎªµÍµçÆ½
	OLED_W_SCL(1);		//ÊÍ·ÅSCL£¬Ê¹SCL³ÊÏÖ¸ßµçÆ½
	OLED_W_SDA(1);		//ÔÚSCL¸ßµçÆ½ÆÚ¼ä£¬ÊÍ·ÅSDA£¬²úÉúÖÕÖ¹ĞÅºÅ
}

/**
  * º¯    Êı£ºI2C·¢ËÍÒ»¸ö×Ö½Ú
  * ²Î    Êı£ºByte Òª·¢ËÍµÄÒ»¸ö×Ö½ÚÊı¾İ£¬·¶Î§£º0x00~0xFF
  * ·µ »Ø Öµ£ºÎŞ
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	
	uint8_t i;
	
	/*Ñ­»·8´Î£¬Ö÷»úÒÀ´Î·¢ËÍÊı¾İµÄÃ¿Ò»Î»*/
	for (i = 0; i < 8; i++)
	{
		/*Ê¹ÓÃÑÚÂëµÄ·½Ê½È¡³öByteµÄÖ¸¶¨Ò»Î»Êı¾İ²¢Ğ´Èëµ½SDAÏß*/
		/*Á½¸ö!µÄ×÷ÓÃÊÇ£¬ÈÃËùÓĞ·ÇÁãµÄÖµ±äÎª1*/
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		
		OLED_W_SCL(1);	//ÊÍ·ÅSCL£¬´Ó»úÔÚSCL¸ßµçÆ½ÆÚ¼ä¶ÁÈ¡SDA
		OLED_W_SCL(0);	//À­µÍSCL£¬Ö÷»ú¿ªÊ¼·¢ËÍÏÂÒ»Î»Êı¾İ
	}
	
	OLED_W_SCL(1);		//¶îÍâµÄÒ»¸öÊ±ÖÓ£¬²»´¦ÀíÓ¦´ğĞÅºÅ
	OLED_W_SCL(0);
	
}

/**
  * º¯    Êı£ºOLEDĞ´ÃüÁî
  * ²Î    Êı£ºCommand ÒªĞ´ÈëµÄÃüÁîÖµ£¬·¶Î§£º0x00~0xFF
  * ·µ »Ø Öµ£ºÎŞ
  */
#define OLED_TIMEOUT      0x2000  // 400kHzÏÂ³¬Ê±ãĞÖµËõ¶Ì£¨ÊÊÅä¸ßËÙ£©
void OLED_WriteCommand(uint8_t Command)
{
//	OLED_I2C_Start();				//I2CÆğÊ¼
//	OLED_I2C_SendByte(0x78);		//·¢ËÍOLEDµÄI2C´Ó»úµØÖ·
//	OLED_I2C_SendByte(0x00);		//¿ØÖÆ×Ö½Ú£¬¸ø0x00£¬±íÊ¾¼´½«Ğ´ÃüÁî
//	OLED_I2C_SendByte(Command);		//Ğ´ÈëÖ¸¶¨µÄÃüÁî
//	OLED_I2C_Stop();				//I2CÖÕÖ¹

	while(SET == I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	//ÊÇ·ñÃ¦Âµ
	
	I2C_GenerateSTART(I2C2, ENABLE);	//ÆğÊ¼Î»
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	
	I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter); //·¢ËÍµØÖ·
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	
	I2C_SendData(I2C2, 0x00);	//·¢ËÍ¿ØÖÆÎ»
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	
 
	I2C_SendData(I2C2, Command);	//·¢ËÍÊı¾İ
	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	
	
	I2C_GenerateSTOP(I2C2 , ENABLE);


}

/**
  * º¯    Êı£ºOLEDĞ´Êı¾İ
  * ²Î    Êı£ºData ÒªĞ´ÈëÊı¾İµÄÆğÊ¼µØÖ·
  * ²Î    Êı£ºCount ÒªĞ´ÈëÊı¾İµÄÊıÁ¿
  * ·µ »Ø Öµ£ºÎŞ
  */
volatile uint8_t g_I2C2_DMA_Error = 0; // ´íÎó±ê¼Ç
void OLED_WriteData(uint8_t *Data, uint16_t Count)
{	
uint32_t timeout;
	
	// 1. Ç¿ÖÆ¸´Î»I2C
	I2C_Cmd(I2C2, DISABLE);
	for(volatile uint16_t i=0; i<50; i++);
	I2C_Cmd(I2C2, ENABLE);
	I2C_ClearFlag(I2C2, I2C_FLAG_AF | I2C_FLAG_BERR | I2C_FLAG_OVR);
	
	// 2. µÈ´ı×ÜÏß¿ÕÏĞ
	timeout = OLED_TIMEOUT;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) && timeout--);
	if(timeout == 0) { 
		printf("´íÎó: ×ÜÏßÃ¦Âµ! SR1=0x%04X\r\n", I2C2->SR1);
		g_I2C2_DMA_Error = 1; 
		return; 
	}
	
	// 3. ÅäÖÃDMA
	DMA_Cmd(DMA1_Stream7, DISABLE);
	DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_TEIF7);
	DMA1_Stream7->M0AR = (uint32_t)Data;
	DMA1_Stream7->NDTR = Count;
	
	// 4. ·¢ËÍSTART
	I2C_GenerateSTART(I2C2, ENABLE);
	timeout = OLED_TIMEOUT;
	while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_SB) && timeout--);
	if(timeout == 0) {
		printf("´íÎó: START³¬Ê±! SR1=0x%04X\r\n", I2C2->SR1);
		I2C_GenerateSTOP(I2C2, ENABLE);
		g_I2C2_DMA_Error = 1;
		return;
	}
	
	// Çå³ıSB±êÖ¾
	uint32_t temp = I2C2->SR1;
	
	// 5. ·¢ËÍµØÖ·
	I2C_Send7bitAddress(I2C2, OLED_I2C_ADDR, I2C_Direction_Transmitter);
	
	// ¹Ø¼üĞŞ¸Ä1£ºÇ¿ÖÆµÈ´ıADDR±êÖ¾
	timeout = OLED_TIMEOUT;
	while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_ADDR) && timeout--) {
		// ¼ì²é´íÎó
		if(I2C_GetFlagStatus(I2C2, I2C_FLAG_AF)) {
			printf("¼ì²âµ½AF´íÎó£¬Çå³ı±êÖ¾¼ÌĞøÖ´ĞĞ...\r\n");
			I2C_ClearFlag(I2C2, I2C_FLAG_AF);
			break;
		}
	}
	
	// ¹Ø¼üĞŞ¸Ä2£ºÇå³ıADDR±êÖ¾
	if(timeout > 0) {
		temp = I2C2->SR1;  // ¶ÁÈ¡SR1Çå³ıADDR
		temp = I2C2->SR2;  // ¶ÁÈ¡SR2Çå³ıADDR
//		printf("ADDR±êÖ¾Çå³ı³É¹¦! SR1=0x%04X, SR2=0x%04X\r\n", I2C2->SR1, I2C2->SR2);
	}
	
	// 6. ·¢ËÍ¿ØÖÆ×Ö½Ú0x40
	I2C_SendData(I2C2, 0x40);
	
	// ¹Ø¼üĞŞ¸Ä3£ºµÈ´ıTXE±êÖ¾
//	timeout = OLED_TIMEOUT;
//	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE) == RESET && timeout--) {
//		if(timeout % 1000 == 0) {
//			printf("µÈ´ıTXE: SR1=0x%04X\r\n", I2C2->SR1);
//		}
//	}
//	
//	if(timeout == 0) {
//		printf("¾¯¸æ: TXE³¬Ê±£¬Ç¿ÖÆ¼ÌĞø! SR1=0x%04X\r\n", I2C2->SR1);
//	}
	
	// 7. Æô¶¯DMA
	DMA_Cmd(DMA1_Stream7, ENABLE);
	
	// 8. µÈ´ıDMAÍê³É
	timeout = OLED_TIMEOUT * 3;
	while(!DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) && timeout--) {
		if(timeout % 10000 == 0) {
//			printf("µÈ´ıDMA: timeout=%lu, NDTR=%d, HISR=0x%08X\r\n", 
//				   timeout, DMA1_Stream7->NDTR, DMA1->HISR);
		}
	}
	
	// 9. ºóĞø´¦Àí
	if(timeout > 0) {
		DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
		DMA_Cmd(DMA1_Stream7, DISABLE);
		delay_us(20);
		I2C_GenerateSTOP(I2C2, ENABLE);
		g_I2C2_DMA_Error = 0;
//		printf("´«Êä³É¹¦! NDTR=%d\r\n", DMA1_Stream7->NDTR);
	} else {
		printf("´íÎó: DMA´«Êä³¬Ê±! NDTR=%d\r\n", DMA1_Stream7->NDTR);
		DMA_Cmd(DMA1_Stream7, DISABLE);
		I2C_GenerateSTOP(I2C2, ENABLE);
		g_I2C2_DMA_Error = 1;
	}
	
//	printf("=== OLED_WriteData½áÊø ===\r\n\r\n");
//	uint16_t i;

//	while(SET == I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	//ÊÇ·ñÃ¦Âµ
//	I2C_GenerateSTART(I2C2, ENABLE);	//ÆğÊ¼Î»
//	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
//	
//	
//	I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter); //·¢ËÍµØÖ·
//	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
//	
//	
//	I2C_SendData(I2C2, 0x40);	//·¢ËÍ¿ØÖÆÎ»
//	while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
//	/*Ñ­»·Count´Î£¬½øĞĞÁ¬ĞøµÄÊı¾İĞ´Èë*/
////	for (j = 0; j < 8; j ++)
////	{
//	for (i = 0; i < Count; i ++)
//	{
//		I2C_SendData(I2C2, Data[i]);	//·¢ËÍÊı¾İ
//		while(I2C_CheckEvent(I2C2 , I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
//	}
//	I2C_GenerateSTOP(I2C2 , ENABLE);

}

/*********************Í¨ĞÅĞ­Òé*/


/*Ó²¼şÅäÖÃ*********************/

/**
  * º¯    Êı£ºOLED³õÊ¼»¯
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºÊ¹ÓÃÇ°£¬ĞèÒªµ÷ÓÃ´Ë³õÊ¼»¯º¯Êı
  */
void OLED_Init(void)
{
	OLED_GPIO_Init();			//ÏÈµ÷ÓÃµ×²ãµÄ¶Ë¿Ú³õÊ¼»¯

	/*Ğ´ÈëÒ»ÏµÁĞµÄÃüÁî£¬¶ÔOLED½øĞĞ³õÊ¼»¯ÅäÖÃ*/
	OLED_WriteCommand(0xAE);	//ÉèÖÃÏÔÊ¾¿ªÆô/¹Ø±Õ£¬0xAE¹Ø±Õ£¬0xAF¿ªÆô
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x10);
	
	OLED_WriteCommand(0x40);	//ÉèÖÃÏÔÊ¾¿ªÊ¼ĞĞ£¬0x40~0x7F
	
	OLED_WriteCommand(0xB0);
	
	OLED_WriteCommand(0x81);	//ÉèÖÃ¶Ô±È¶È
	OLED_WriteCommand(0xFF);	//0x00~0xFF
	OLED_WriteCommand(0xA1);	//ÉèÖÃ×óÓÒ·½Ïò£¬0xA1Õı³££¬0xA0×óÓÒ·´ÖÃ
	OLED_WriteCommand(0xA6);	//ÉèÖÃÕı³£/·´É«ÏÔÊ¾£¬0xA6Õı³££¬0xA7·´É«
	OLED_WriteCommand(0xA8);	//ÉèÖÃ¶àÂ·¸´ÓÃÂÊ
	OLED_WriteCommand(0x3F);	//0x0E~0x3F
	OLED_WriteCommand(0xC8);	//ÉèÖÃÉÏÏÂ·½Ïò£¬0xC8Õı³££¬0xC0ÉÏÏÂ·´ÖÃ

	OLED_WriteCommand(0xD3);	//ÉèÖÃÏÔÊ¾Æ«ÒÆ
	OLED_WriteCommand(0x00);	//0x00~0x7F
	
	OLED_WriteCommand(0xD5);	//ÉèÖÃÏÔÊ¾Ê±ÖÓ·ÖÆµ±È/Õñµ´Æ÷ÆµÂÊ
	OLED_WriteCommand(0x80);	//0x00~0xFF
	
	OLED_WriteCommand(0xD8);
	OLED_WriteCommand(0x05);
	OLED_WriteCommand(0xD9);	//ÉèÖÃÔ¤³äµçÖÜÆÚ
	OLED_WriteCommand(0xF1);
	OLED_WriteCommand(0xDA);	//ÉèÖÃCOMÒı½ÅÓ²¼şÅäÖÃ
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0xDB);	//ÉèÖÃVCOMHÈ¡ÏûÑ¡Ôñ¼¶±ğ
	OLED_WriteCommand(0x30);
	
	OLED_WriteCommand(0x8D);	//ÉèÖÃ³äµç±Ã
	OLED_WriteCommand(0x14);
	
	OLED_WriteCommand(0xAF);	//¿ªÆôÏÔÊ¾
	
	OLED_WriteCommand(0x20);	//¿ªÆôÏÔÊ¾
	OLED_WriteCommand(0x00);	//¿ªÆôÏÔÊ¾
	
	OLED_Clear();				//Çå¿ÕÏÔ´æÊı×é
	OLED_Update();				//¸üĞÂÏÔÊ¾£¬ÇåÆÁ£¬·ÀÖ¹³õÊ¼»¯ºóÎ´ÏÔÊ¾ÄÚÈİÊ±»¨ÆÁ
}

/**
  * º¯    Êı£ºOLEDÉèÖÃÏÔÊ¾¹â±êÎ»ÖÃ
  * ²Î    Êı£ºPage Ö¸¶¨¹â±êËùÔÚµÄÒ³£¬·¶Î§£º0~7
  * ²Î    Êı£ºX Ö¸¶¨¹â±êËùÔÚµÄXÖá×ø±ê£¬·¶Î§£º0~127
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºOLEDÄ¬ÈÏµÄYÖá£¬Ö»ÄÜ8¸öBitÎªÒ»×éĞ´Èë£¬¼´1Ò³µÈÓÚ8¸öYÖá×ø±ê
  */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*Èç¹ûÊ¹ÓÃ´Ë³ÌĞòÇı¶¯1.3´çµÄOLEDÏÔÊ¾ÆÁ£¬ÔòĞèÒª½â³ı´Ë×¢ÊÍ*/
	/*ÒòÎª1.3´çµÄOLEDÇı¶¯Ğ¾Æ¬£¨SH1106£©ÓĞ132ÁĞ*/
	/*ÆÁÄ»µÄÆğÊ¼ÁĞ½ÓÔÚÁËµÚ2ÁĞ£¬¶ø²»ÊÇµÚ0ÁĞ*/
	/*ËùÒÔĞèÒª½«X¼Ó2£¬²ÅÄÜÕı³£ÏÔÊ¾*/
	X += 2;
	
	/*Í¨¹ıÖ¸ÁîÉèÖÃÒ³µØÖ·ºÍÁĞµØÖ·*/
	OLED_WriteCommand(0xB0 | Page);					//ÉèÖÃÒ³Î»ÖÃ
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//ÉèÖÃXÎ»ÖÃ¸ß4Î»
	OLED_WriteCommand(0x00 | (X & 0x0F));			//ÉèÖÃXÎ»ÖÃµÍ4Î»
}

/*********************Ó²¼şÅäÖÃ*/


/*¹¤¾ßº¯Êı*********************/

/*¹¤¾ßº¯Êı½ö¹©ÄÚ²¿²¿·Öº¯ÊıÊ¹ÓÃ*/

/**
  * º¯    Êı£º´Î·½º¯Êı
  * ²Î    Êı£ºX µ×Êı
  * ²Î    Êı£ºY Ö¸Êı
  * ·µ »Ø Öµ£ºµÈÓÚXµÄY´Î·½
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//½á¹ûÄ¬ÈÏÎª1
	while (Y --)			//ÀÛ³ËY´Î
	{
		Result *= X;		//Ã¿´Î°ÑXÀÛ³Ëµ½½á¹ûÉÏ
	}
	return Result;
}

/**
  * º¯    Êı£ºÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨¶à±ßĞÎÄÚ²¿
  * ²Î    Êı£ºnvert ¶à±ßĞÎµÄ¶¥µãÊı
  * ²Î    Êı£ºvertx verty °üº¬¶à±ßĞÎ¶¥µãµÄxºÍy×ø±êµÄÊı×é
  * ²Î    Êı£ºtestx testy ²âÊÔµãµÄXºÍy×ø±ê
  * ·µ »Ø Öµ£ºÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨¶à±ßĞÎÄÚ²¿£¬1£ºÔÚÄÚ²¿£¬0£º²»ÔÚÄÚ²¿
  */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i, j, c = 0;
	
	/*´ËËã·¨ÓÉW. Randolph FranklinÌá³ö*/
	/*²Î¿¼Á´½Ó£ºhttps://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
  * º¯    Êı£ºÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ²¿
  * ²Î    Êı£ºX Y Ö¸¶¨µãµÄ×ø±ê
  * ²Î    Êı£ºStartAngle EndAngle ÆğÊ¼½Ç¶ÈºÍÖÕÖ¹½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ·µ »Ø Öµ£ºÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ²¿£¬1£ºÔÚÄÚ²¿£¬0£º²»ÔÚÄÚ²¿
  */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//¼ÆËãÖ¸¶¨µãµÄ»¡¶È£¬²¢×ª»»Îª½Ç¶È±íÊ¾
	if (StartAngle < EndAngle)	//ÆğÊ¼½Ç¶ÈĞ¡ÓÚÖÕÖ¹½Ç¶ÈµÄÇé¿ö
	{
		/*Èç¹ûÖ¸¶¨½Ç¶ÈÔÚÆğÊ¼ÖÕÖ¹½Ç¶ÈÖ®¼ä£¬ÔòÅĞ¶¨Ö¸¶¨µãÔÚÖ¸¶¨½Ç¶È*/
		if (PointAngle >= StartAngle && PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	else			//ÆğÊ¼½Ç¶È´óÓÚÓÚÖÕÖ¹½Ç¶ÈµÄÇé¿ö
	{
		/*Èç¹ûÖ¸¶¨½Ç¶È´óÓÚÆğÊ¼½Ç¶È»òÕßĞ¡ÓÚÖÕÖ¹½Ç¶È£¬ÔòÅĞ¶¨Ö¸¶¨µãÔÚÖ¸¶¨½Ç¶È*/
		if (PointAngle >= StartAngle || PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	return 0;		//²»Âú×ãÒÔÉÏÌõ¼ş£¬ÔòÅĞ¶ÏÅĞ¶¨Ö¸¶¨µã²»ÔÚÖ¸¶¨½Ç¶È
}

/*********************¹¤¾ßº¯Êı*/


/*¹¦ÄÜº¯Êı*********************/

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é¸üĞÂµ½OLEDÆÁÄ»
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶ÔOLEDÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  *           Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  *           ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  *           ¹Êµ÷ÓÃÏÔÊ¾º¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Update(void)
{
	uint8_t j;
	/*ÉèÖÃ¹â±êÎ»ÖÃÎªÃ¿Ò»Ò³µÄµÚÒ»ÁĞ*/
//	OLED_SetCursor(0, 0);
	/*Á¬ĞøĞ´Èë128¸öÊı¾İ£¬½«ÏÔ´æÊı×éµÄÊı¾İĞ´Èëµ½OLEDÓ²¼ş*/
//	OLED_WriteData(OLED_DisplayBuf[0], 1024);
//	/*±éÀúÃ¿Ò»Ò³*/
	for (j = 0; j < 8; j ++)
	{
		/*ÉèÖÃ¹â±êÎ»ÖÃÎªÃ¿Ò»Ò³µÄµÚÒ»ÁĞ*/
		OLED_SetCursor(j, 0);
		/*Á¬ĞøĞ´Èë128¸öÊı¾İ£¬½«ÏÔ´æÊı×éµÄÊı¾İĞ´Èëµ½OLEDÓ²¼ş*/
		OLED_WriteData(OLED_DisplayBuf[j], 128);
	}
//	while(g_I2C2_DMA_TxComplete == 0)
//	{
//		vTaskDelay(pdMS_TO_TICKS(1)); // ÑÓÊ±10ms£¬²»Õ¼ÂúCPU
//	}
//	if(g_I2C2_DMA_TxComplete == 1)
//	{
//		// ·¢ËÍÍê³É£ºÇå±êÖ¾£¬ÍË³öÑ­»·£¬¼ÌĞøÏÂÒ»´ÎË¢ÆÁ
//		g_I2C2_DMA_TxComplete = 0;
//	}
//	else if(g_I2C2_DMA_TxComplete == 2)
//	{
//		// ·¢ËÍ´íÎó£º´¦ÀíÒì³££¨±ÈÈçÖØ·¢£©
//		g_I2C2_DMA_TxComplete = 0;
//		printf("OLED error\n");
////		I2C2_DMA_Send_NoBlock(OLED_I2C_ADDR, OLED_ScreenData, sizeof(OLED_ScreenData));
//	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·Ö¸üĞÂµ½OLEDÆÁÄ»
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£º´Ëº¯Êı»áÖÁÉÙ¸üĞÂ²ÎÊıÖ¸¶¨µÄÇøÓò
  *           Èç¹û¸üĞÂÇøÓòYÖáÖ»°üº¬²¿·ÖÒ³£¬ÔòÍ¬Ò»Ò³µÄÊ£Óà²¿·Ö»á¸úËæÒ»Æğ¸üĞÂ
  * Ëµ    Ã÷£ºËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶ÔOLEDÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  *           Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  *           ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  *           ¹Êµ÷ÓÃÏÔÊ¾º¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	/*±éÀúÖ¸¶¨ÇøÓòÉæ¼°µÄÏà¹ØÒ³*/
	/*(Y + Height - 1) / 8 + 1µÄÄ¿µÄÊÇ(Y + Height) / 8²¢ÏòÉÏÈ¡Õû*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*ÉèÖÃ¹â±êÎ»ÖÃÎªÏà¹ØÒ³µÄÖ¸¶¨ÁĞ*/
		OLED_SetCursor(j, X);
		/*Á¬ĞøĞ´ÈëWidth¸öÊı¾İ£¬½«ÏÔ´æÊı×éµÄÊı¾İĞ´Èëµ½OLEDÓ²¼ş*/
		OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×éÈ«²¿ÇåÁã
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Clear(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//±éÀú8Ò³
	{
		for (i = 0; i < 128; i ++)			//±éÀú128ÁĞ
		{
			OLED_DisplayBuf[j][i] = 0x00;	//½«ÏÔ´æÊı×éÊı¾İÈ«²¿ÇåÁã
		}
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·ÖÇåÁã
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//±éÀúÖ¸¶¨Ò³
	{
		for (i = X; i < X + Width; i ++)	//±éÀúÖ¸¶¨ÁĞ
		{
			OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));	//½«ÏÔ´æÊı×éÖ¸¶¨Êı¾İÇåÁã
		}
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×éÈ«²¿È¡·´
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Reverse(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//±éÀú8Ò³
	{
		for (i = 0; i < 128; i ++)			//±éÀú128ÁĞ
		{
			OLED_DisplayBuf[j][i] ^= 0xFF;	//½«ÏÔ´æÊı×éÊı¾İÈ«²¿È¡·´
		}
	}
}
	
/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·ÖÈ¡·´
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//±éÀúÖ¸¶¨Ò³
	{
		for (i = X; i < X + Width; i ++)	//±éÀúÖ¸¶¨ÁĞ
		{
			OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//½«ÏÔ´æÊı×éÖ¸¶¨Êı¾İÈ¡·´
		}
	}
}

void OLED_ReverseArea_My(uint8_t Y, uint8_t X, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ReverseArea((X-1)*6,(Y-1)*8,Length*6,8);
	}
	else if(FontSize==OLED_8X16)
	{
		OLED_ReverseArea((X-1)*8,(Y-1)*16,Length*8,16);
	}
}


/**
  * º¯    Êı£ºOLEDÏÔÊ¾Ò»¸ö×Ö·û
  * ²Î    Êı£ºX Ö¸¶¨×Ö·û×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨×Ö·û×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºChar Ö¸¶¨ÒªÏÔÊ¾µÄ×Ö·û£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
	if (FontSize == OLED_8X16)		//×ÖÌåÎª¿í8ÏñËØ£¬¸ß16ÏñËØ
	{
		/*½«ASCII×ÖÄ£¿âOLED_F8x16µÄÖ¸¶¨Êı¾İÒÔ8*16µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
		OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
	}
	else if(FontSize == OLED_6X8)	//×ÖÌåÎª¿í6ÏñËØ£¬¸ß8ÏñËØ
	{
		/*½«ASCII×ÖÄ£¿âOLED_F6x8µÄÖ¸¶¨Êı¾İÒÔ6*8µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
		OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
	}
}

void OLED_ShowChar_My(uint8_t Y, uint8_t X, char Char, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowChar((X-1)*6,(Y-1)*8,Char,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowChar((X-1)*8,(Y-1)*16,Char,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾×Ö·û´®
  * ²Î    Êı£ºX Ö¸¶¨×Ö·û´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨×Ö·û´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºString Ö¸¶¨ÒªÏÔÊ¾µÄ×Ö·û´®£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û×é³ÉµÄ×Ö·û´®
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)		//±éÀú×Ö·û´®µÄÃ¿¸ö×Ö·û
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸ö×Ö·û*/
		OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
	}
}

void OLED_ShowString_My(uint8_t Y, uint8_t X, char *String, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowString((X-1)*6,(Y-1)*8,String,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowString((X-1)*8,(Y-1)*16,String,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Êı×Ö£¨Ê®½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0~4294967295
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»							
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number / OLED_Pow(10, Length - i - 1) % 10 ¿ÉÒÔÊ®½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

void OLED_ShowNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾ÓĞ·ûºÅÊı×Ö£¨Ê®½øÖÆ£¬ÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º-2147483648~2147483647
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	uint32_t Number1;
	
	if (Number >= 0)						//Êı×Ö´óÓÚµÈÓÚ0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//ÏÔÊ¾+ºÅ
		Number1 = Number;					//Number1Ö±½ÓµÈÓÚNumber
	}
	else									//Êı×ÖĞ¡ÓÚ0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//ÏÔÊ¾-ºÅ
		Number1 = -Number;					//Number1µÈÓÚNumberÈ¡¸º
	}
	
	for (i = 0; i < Length; i++)			//±éÀúÊı×ÖµÄÃ¿Ò»Î»								
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number1 / OLED_Pow(10, Length - i - 1) % 10 ¿ÉÒÔÊ®½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

void OLED_ShowSignedNum_My(uint8_t Y, uint8_t X, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowSignedNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowSignedNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Ê®Áù½øÖÆÊı×Ö£¨Ê®Áù½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0x00000000~0xFFFFFFFF
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~8
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»
	{
		/*ÒÔÊ®Áù½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		
		if (SingleNumber < 10)			//µ¥¸öÊı×ÖĞ¡ÓÚ10
		{
			/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÏÔÊ¾´ËÊı×Ö*/
			/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
		}
		else							//µ¥¸öÊı×Ö´óÓÚ10
		{
			/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÏÔÊ¾´ËÊı×Ö*/
			/*+ 'A' ¿É½«Êı×Ö×ª»»Îª´ÓA¿ªÊ¼µÄÊ®Áù½øÖÆ×Ö·û*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

void OLED_ShowHexNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowHexNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowHexNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾¶ş½øÖÆÊı×Ö£¨¶ş½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0x00000000~0xFFFFFFFF
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~16
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»	
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number / OLED_Pow(2, Length - i - 1) % 2 ¿ÉÒÔ¶ş½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
	}
}

void OLED_ShowBinNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowBinNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowBinNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾¸¡µãÊı×Ö£¨Ê®½øÖÆ£¬Ğ¡Êı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º-4294967295.0~4294967295.0
  * ²Î    Êı£ºIntLength Ö¸¶¨Êı×ÖµÄÕûÊıÎ»³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFraLength Ö¸¶¨Êı×ÖµÄĞ¡ÊıÎ»³¤¶È£¬·¶Î§£º0~9£¬Ğ¡Êı½øĞĞËÄÉáÎåÈëÏÔÊ¾
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	uint32_t PowNum, IntNum, FraNum;
	
	if (Number >= 0)						//Êı×Ö´óÓÚµÈÓÚ0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//ÏÔÊ¾+ºÅ
	}
	else									//Êı×ÖĞ¡ÓÚ0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//ÏÔÊ¾-ºÅ
		Number = -Number;					//NumberÈ¡¸º
	}
	
	/*ÌáÈ¡ÕûÊı²¿·ÖºÍĞ¡Êı²¿·Ö*/
	IntNum = Number;						//Ö±½Ó¸³Öµ¸øÕûĞÍ±äÁ¿£¬ÌáÈ¡ÕûÊı
	Number -= IntNum;						//½«NumberµÄÕûÊı¼õµô£¬·ÀÖ¹Ö®ºó½«Ğ¡Êı³Ëµ½ÕûÊıÊ±ÒòÊı¹ı´óÔì³É´íÎó
	PowNum = OLED_Pow(10, FraLength);		//¸ù¾İÖ¸¶¨Ğ¡ÊıµÄÎ»Êı£¬È·¶¨³ËÊı
	FraNum = round(Number * PowNum);		//½«Ğ¡Êı³Ëµ½ÕûÊı£¬Í¬Ê±ËÄÉáÎåÈë£¬±ÜÃâÏÔÊ¾Îó²î
	IntNum += FraNum / PowNum;				//ÈôËÄÉáÎåÈëÔì³ÉÁË½øÎ»£¬ÔòĞèÒªÔÙ¼Ó¸øÕûÊı
	
	/*ÏÔÊ¾ÕûÊı²¿·Ö*/
	OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);
	
	/*ÏÔÊ¾Ğ¡Êıµã*/
	OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
	
	/*ÏÔÊ¾Ğ¡Êı²¿·Ö*/
	OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);
}

void OLED_ShowFloatNum_My(uint8_t Y, uint8_t X, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowFloatNum((X-1)*6,(Y-1)*8,Number,IntLength,FraLength,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowFloatNum((X-1)*8,(Y-1)*16,Number,IntLength,FraLength,FontSize);
	}
}


/**
  * º¯    Êı£ºOLEDÏÔÊ¾ºº×Ö´®
  * ²Î    Êı£ºX Ö¸¶¨ºº×Ö´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ºº×Ö´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºChinese Ö¸¶¨ÒªÏÔÊ¾µÄºº×Ö´®£¬·¶Î§£º±ØĞëÈ«²¿Îªºº×Ö»òÕßÈ«½Ç×Ö·û£¬²»Òª¼ÓÈëÈÎºÎ°ë½Ç×Ö·û
  *           ÏÔÊ¾µÄºº×ÖĞèÒªÔÚOLED_Data.cÀïµÄOLED_CF16x16Êı×é¶¨Òå
  *           Î´ÕÒµ½Ö¸¶¨ºº×ÖÊ±£¬»áÏÔÊ¾Ä¬ÈÏÍ¼ĞÎ£¨Ò»¸ö·½¿ò£¬ÄÚ²¿Ò»¸öÎÊºÅ£©
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
	uint8_t pChinese = 0;
	uint8_t pIndex;
	uint8_t i;
	char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
	
	for (i = 0; Chinese[i] != '\0'; i ++)		//±éÀúºº×Ö´®
	{
		SingleChinese[pChinese] = Chinese[i];	//ÌáÈ¡ºº×Ö´®Êı¾İµ½µ¥¸öºº×ÖÊı×é
		pChinese ++;							//¼Æ´Î×ÔÔö
		
		/*µ±ÌáÈ¡´ÎÊıµ½´ïOLED_CHN_CHAR_WIDTHÊ±£¬¼´´ú±íÌáÈ¡µ½ÁËÒ»¸öÍêÕûµÄºº×Ö*/
		if (pChinese >= OLED_CHN_CHAR_WIDTH)
		{
			pChinese = 0;		//¼Æ´Î¹éÁã
			
			/*±éÀúÕû¸öºº×Ö×ÖÄ£¿â£¬Ñ°ÕÒÆ¥ÅäµÄºº×Ö*/
			/*Èç¹ûÕÒµ½×îºóÒ»¸öºº×Ö£¨¶¨ÒåÎª¿Õ×Ö·û´®£©£¬Ôò±íÊ¾ºº×ÖÎ´ÔÚ×ÖÄ£¿â¶¨Òå£¬Í£Ö¹Ñ°ÕÒ*/
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				/*ÕÒµ½Æ¥ÅäµÄºº×Ö*/
				if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
				{
					break;		//Ìø³öÑ­»·£¬´ËÊ±pIndexµÄÖµÎªÖ¸¶¨ºº×ÖµÄË÷Òı
				}
			}
			
			/*½«ºº×Ö×ÖÄ£¿âOLED_CF16x16µÄÖ¸¶¨Êı¾İÒÔ16*16µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
			OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
		}
	}
}

void OLED_ShowChinese_My(uint8_t Y, uint8_t X, char *Chinese)
{
	OLED_ShowChinese((X-1)*8,(Y-1)*16,Chinese);
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Í¼Ïñ
  * ²Î    Êı£ºX Ö¸¶¨Í¼Ïñ×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Í¼Ïñ×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨Í¼ÏñµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨Í¼ÏñµÄ¸ß¶È£¬·¶Î§£º0~64
  * ²Î    Êı£ºImage Ö¸¶¨ÒªÏÔÊ¾µÄÍ¼Ïñ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Í¼Ïñ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*½«Í¼ÏñËùÔÚÇøÓòÇå¿Õ*/
	OLED_ClearArea(X, Y, Width, Height);
	
	/*±éÀúÖ¸¶¨Í¼ÏñÉæ¼°µÄÏà¹ØÒ³*/
	/*(Height - 1) / 8 + 1µÄÄ¿µÄÊÇHeight / 8²¢ÏòÉÏÈ¡Õû*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*±éÀúÖ¸¶¨Í¼ÏñÉæ¼°µÄÏà¹ØÁĞ*/
		for (i = 0; i < Width; i ++)
		{
			/*³¬³ö±ß½ç£¬ÔòÌø¹ıÏÔÊ¾*/
			if (X + i > 127) {break;}
			if (Y / 8 + j > 7) {return;}
			
			/*ÏÔÊ¾Í¼ÏñÔÚµ±Ç°Ò³µÄÄÚÈİ*/
			OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
			
			/*³¬³ö±ß½ç£¬ÔòÌø¹ıÏÔÊ¾*/
			/*Ê¹ÓÃcontinueµÄÄ¿µÄÊÇ£¬ÏÂÒ»Ò³³¬³ö±ß½çÊ±£¬ÉÏÒ»Ò³µÄºóĞøÄÚÈİ»¹ĞèÒª¼ÌĞøÏÔÊ¾*/
			if (Y / 8 + j + 1 > 7) {continue;}
			
			/*ÏÔÊ¾Í¼ÏñÔÚÏÂÒ»Ò³µÄÄÚÈİ*/
			OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
		}
	}
}

/**
  * º¯    Êı£ºOLEDÊ¹ÓÃprintfº¯Êı´òÓ¡¸ñÊ½»¯×Ö·û´®
  * ²Î    Êı£ºX Ö¸¶¨¸ñÊ½»¯×Ö·û´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨¸ñÊ½»¯×Ö·û´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ²Î    Êı£ºformat Ö¸¶¨ÒªÏÔÊ¾µÄ¸ñÊ½»¯×Ö·û´®£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û×é³ÉµÄ×Ö·û´®
  * ²Î    Êı£º... ¸ñÊ½»¯×Ö·û´®²ÎÊıÁĞ±í
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
{
	char String[30];						//¶¨Òå×Ö·ûÊı×é
	va_list arg;							//¶¨Òå¿É±ä²ÎÊıÁĞ±íÊı¾İÀàĞÍµÄ±äÁ¿arg
	va_start(arg, format);					//´Óformat¿ªÊ¼£¬½ÓÊÕ²ÎÊıÁĞ±íµ½arg±äÁ¿
	vsprintf(String, format, arg);			//Ê¹ÓÃvsprintf´òÓ¡¸ñÊ½»¯×Ö·û´®ºÍ²ÎÊıÁĞ±íµ½×Ö·ûÊı×éÖĞ
	va_end(arg);							//½áÊø±äÁ¿arg
	OLED_ShowString(X, Y, String, FontSize);//OLEDÏÔÊ¾×Ö·ûÊı×é£¨×Ö·û´®£©
}

/**
  * º¯    Êı£ºOLEDÔÚÖ¸¶¨Î»ÖÃ»­Ò»¸öµã
  * ²Î    Êı£ºX Ö¸¶¨µãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨µãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Î»ÖÃ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*½«ÏÔ´æÊı×éÖ¸¶¨Î»ÖÃµÄÒ»¸öBitÊı¾İÖÃ1*/
	OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

/**
  * º¯    Êı£ºOLED»ñÈ¡Ö¸¶¨Î»ÖÃµãµÄÖµ
  * ²Î    Êı£ºX Ö¸¶¨µãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨µãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÖ¸¶¨Î»ÖÃµãÊÇ·ñ´¦ÓÚµãÁÁ×´Ì¬£¬1£ºµãÁÁ£¬0£ºÏ¨Ãğ
  */
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y)
{
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Î»ÖÃ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return 0;}
	if (Y > 63) {return 0;}
	
	/*ÅĞ¶ÏÖ¸¶¨Î»ÖÃµÄÊı¾İ*/
	if (OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8))
	{
		return 1;	//Îª1£¬·µ»Ø1
	}
	
	return 0;		//·ñÔò£¬·µ»Ø0
}

/**
  * º¯    Êı£ºOLED»­Ïß
  * ²Î    Êı£ºX0 Ö¸¶¨Ò»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY0 Ö¸¶¨Ò»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX1 Ö¸¶¨ÁíÒ»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY1 Ö¸¶¨ÁíÒ»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//ºáÏßµ¥¶À´¦Àí
	{
		/*0ºÅµãX×ø±ê´óÓÚ1ºÅµãX×ø±ê£¬Ôò½»»»Á½µãX×ø±ê*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*±éÀúX×ø±ê*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//ÒÀ´Î»­µã
		}
	}
	else if (x0 == x1)	//ÊúÏßµ¥¶À´¦Àí
	{
		/*0ºÅµãY×ø±ê´óÓÚ1ºÅµãY×ø±ê£¬Ôò½»»»Á½µãY×ø±ê*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*±éÀúY×ø±ê*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//ÒÀ´Î»­µã
		}
	}
	else				//Ğ±Ïß
	{
		/*Ê¹ÓÃBresenhamËã·¨»­Ö±Ïß£¬¿ÉÒÔ±ÜÃâºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
		/*²Î¿¼ÎÄµµ£ºhttps://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*²Î¿¼½Ì³Ì£ºhttps://www.bilibili.com/video/BV1364y1d7Lo*/
		
		if (x0 > x1)	//0ºÅµãX×ø±ê´óÓÚ1ºÅµãX×ø±ê
		{
			/*½»»»Á½µã×ø±ê*/
			/*½»»»ºó²»Ó°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»¡¢¶ş¡¢Èı¡¢ËÄÏóÏŞ±äÎªµÚÒ»¡¢ËÄÏóÏŞ*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0ºÅµãY×ø±ê´óÓÚ1ºÅµãY×ø±ê
		{
			/*½«Y×ø±êÈ¡¸º*/
			/*È¡¸ººóÓ°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»¡¢ËÄÏóÏŞ±äÎªµÚÒ»ÏóÏŞ*/
			y0 = -y0;
			y1 = -y1;
			
			/*ÖÃ±êÖ¾Î»yflag£¬¼Ç×¡µ±Ç°±ä»»£¬ÔÚºóĞøÊµ¼Ê»­ÏßÊ±£¬ÔÙ½«×ø±ê»»»ØÀ´*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//»­ÏßĞ±ÂÊ´óÓÚ1
		{
			/*½«X×ø±êÓëY×ø±ê»¥»»*/
			/*»¥»»ºóÓ°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»ÏóÏŞ0~90¶È·¶Î§±äÎªµÚÒ»ÏóÏŞ0~45¶È·¶Î§*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*ÖÃ±êÖ¾Î»xyflag£¬¼Ç×¡µ±Ç°±ä»»£¬ÔÚºóĞøÊµ¼Ê»­ÏßÊ±£¬ÔÙ½«×ø±ê»»»ØÀ´*/
			xyflag = 1;
		}
		
		/*ÒÔÏÂÎªBresenhamËã·¨»­Ö±Ïß*/
		/*Ëã·¨ÒªÇó£¬»­Ïß·½Ïò±ØĞëÎªµÚÒ»ÏóÏŞ0~45¶È·¶Î§*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*»­ÆğÊ¼µã£¬Í¬Ê±ÅĞ¶Ï±êÖ¾Î»£¬½«×ø±ê»»»ØÀ´*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//±éÀúXÖáµÄÃ¿¸öµã
		{
			x ++;
			if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
			{
				d += incrE;
			}
			else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«±±·½
			{
				y ++;
				d += incrNE;
			}
			
			/*»­Ã¿Ò»¸öµã£¬Í¬Ê±ÅĞ¶Ï±êÖ¾Î»£¬½«×ø±ê»»»ØÀ´*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}

/**
  * º¯    Êı£ºOLED¾ØĞÎ
  * ²Î    Êı£ºX Ö¸¶¨¾ØĞÎ×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨¾ØĞÎ×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨¾ØĞÎµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨¾ØĞÎµÄ¸ß¶È£¬·¶Î§£º0~64
  * ²Î    Êı£ºIsFilled Ö¸¶¨¾ØĞÎÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	uint8_t i, j;
	if (!IsFilled)		//Ö¸¶¨¾ØĞÎ²»Ìî³ä
	{
		/*±éÀúÉÏÏÂX×ø±ê£¬»­¾ØĞÎÉÏÏÂÁ½ÌõÏß*/
		for (i = X; i < X + Width; i ++)
		{
			OLED_DrawPoint(i, Y);
			OLED_DrawPoint(i, Y + Height - 1);
		}
		/*±éÀú×óÓÒY×ø±ê£¬»­¾ØĞÎ×óÓÒÁ½ÌõÏß*/
		for (i = Y; i < Y + Height; i ++)
		{
			OLED_DrawPoint(X, i);
			OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//Ö¸¶¨¾ØĞÎÌî³ä
	{
		/*±éÀúX×ø±ê*/
		for (i = X; i < X + Width; i ++)
		{
			/*±éÀúY×ø±ê*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³äÂú¾ØĞÎ*/
				OLED_DrawPoint(i, j);
			}
		}
	}
}

/**
  * º¯    Êı£ºOLEDÈı½ÇĞÎ
  * ²Î    Êı£ºX0 Ö¸¶¨µÚÒ»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY0 Ö¸¶¨µÚÒ»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX1 Ö¸¶¨µÚ¶ş¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY1 Ö¸¶¨µÚ¶ş¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX2 Ö¸¶¨µÚÈı¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY2 Ö¸¶¨µÚÈı¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºIsFilled Ö¸¶¨Èı½ÇĞÎÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled)
{
	uint8_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	uint8_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};
	
	if (!IsFilled)			//Ö¸¶¨Èı½ÇĞÎ²»Ìî³ä
	{
		/*µ÷ÓÃ»­Ïßº¯Êı£¬½«Èı¸öµãÓÃÖ±ÏßÁ¬½Ó*/
		OLED_DrawLine(X0, Y0, X1, Y1);
		OLED_DrawLine(X0, Y0, X2, Y2);
		OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//Ö¸¶¨Èı½ÇĞÎÌî³ä
	{
		/*ÕÒµ½Èı¸öµã×îĞ¡µÄX¡¢Y×ø±ê*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}
		
		/*ÕÒµ½Èı¸öµã×î´óµÄX¡¢Y×ø±ê*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}
		
		/*×îĞ¡×î´ó×ø±êÖ®¼äµÄ¾ØĞÎÎª¿ÉÄÜĞèÒªÌî³äµÄÇøÓò*/
		/*±éÀú´ËÇøÓòÖĞËùÓĞµÄµã*/
		/*±éÀúX×ø±ê*/		
		for (i = minx; i <= maxx; i ++)
		{
			/*±éÀúY×ø±ê*/	
			for (j = miny; j <= maxy; j ++)
			{
				/*µ÷ÓÃOLED_pnpoly£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨Èı½ÇĞÎÖ®ÖĞ*/
				/*Èç¹ûÔÚ£¬Ôò»­µã£¬Èç¹û²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
  * º¯    Êı£ºOLED»­Ô²
  * ²Î    Êı£ºX Ö¸¶¨Ô²µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Ô²µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºRadius Ö¸¶¨Ô²µÄ°ë¾¶£¬·¶Î§£º0~255
  * ²Î    Êı£ºIsFilled Ö¸¶¨Ô²ÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*Ê¹ÓÃBresenhamËã·¨»­Ô²£¬¿ÉÒÔ±ÜÃâºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
	/*²Î¿¼ÎÄµµ£ºhttps://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
	/*²Î¿¼½Ì³Ì£ºhttps://www.bilibili.com/video/BV1VM4y1u7wJ*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*»­Ã¿¸ö°Ë·ÖÖ®Ò»Ô²»¡µÄÆğÊ¼µã*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X + y, Y + x);
	OLED_DrawPoint(X - y, Y - x);
	
	if (IsFilled)		//Ö¸¶¨Ô²Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	while (x < y)		//±éÀúXÖáµÄÃ¿¸öµã
	{
		x ++;
		if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d += 2 * x + 1;
		}
		else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*»­Ã¿¸ö°Ë·ÖÖ®Ò»Ô²»¡µÄµã*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X + y, Y + x);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - y, Y - x);
		OLED_DrawPoint(X + x, Y - y);
		OLED_DrawPoint(X + y, Y - x);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X - y, Y + x);
		
		if (IsFilled)	//Ö¸¶¨Ô²Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
			
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -x; j < x; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
				OLED_DrawPoint(X - y, Y + j);
				OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}

/**
  * º¯    Êı£ºOLED»­ÍÖÔ²
  * ²Î    Êı£ºX Ö¸¶¨ÍÖÔ²µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÍÖÔ²µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºA Ö¸¶¨ÍÖÔ²µÄºáÏò°ëÖá³¤¶È£¬·¶Î§£º0~255
  * ²Î    Êı£ºB Ö¸¶¨ÍÖÔ²µÄ×İÏò°ëÖá³¤¶È£¬·¶Î§£º0~255
  * ²Î    Êı£ºIsFilled Ö¸¶¨ÍÖÔ²ÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;
	
	/*Ê¹ÓÃBresenhamËã·¨»­ÍÖÔ²£¬¿ÉÒÔ±ÜÃâ²¿·ÖºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
	/*²Î¿¼Á´½Ó£ºhttps://blog.csdn.net/myf_666/article/details/128167392*/
	
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);
	
	if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
			OLED_DrawPoint(X, Y + j);
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	/*»­ÍÖÔ²»¡µÄÆğÊ¼µã*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X - x, Y + y);
	OLED_DrawPoint(X + x, Y - y);
	
	/*»­ÍÖÔ²ÖĞ¼ä²¿·Ö*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;
		
		if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*»­ÍÖÔ²ÖĞ¼ä²¿·ÖÔ²»¡*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
	
	/*»­ÍÖÔ²Á½²à²¿·Ö*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	
	while (y > 0)
	{
		if (d2 <= 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;
			
		}
		else				//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;
		
		if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
		{
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*»­ÍÖÔ²Á½²à²¿·ÖÔ²»¡*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
}

/**
  * º¯    Êı£ºOLED»­Ô²»¡
  * ²Î    Êı£ºX Ö¸¶¨Ô²»¡µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Ô²»¡µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºRadius Ö¸¶¨Ô²»¡µÄ°ë¾¶£¬·¶Î§£º0~255
  * ²Î    Êı£ºStartAngle Ö¸¶¨Ô²»¡µÄÆğÊ¼½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ²Î    Êı£ºEndAngle Ö¸¶¨Ô²»¡µÄÖÕÖ¹½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ²Î    Êı£ºIsFilled Ö¸¶¨Ô²»¡ÊÇ·ñÌî³ä£¬Ìî³äºóÎªÉÈĞÎ
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*´Ëº¯Êı½èÓÃBresenhamËã·¨»­Ô²µÄ·½·¨*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*ÔÚ»­Ô²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
	if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
	if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
	if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
	if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
	
	if (IsFilled)	//Ö¸¶¨Ô²»¡Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
			if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
		}
	}
	
	while (x < y)		//±éÀúXÖáµÄÃ¿¸öµã
	{
		x ++;
		if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d += 2 * x + 1;
		}
		else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*ÔÚ»­Ô²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
		if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
		if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
		if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
		if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
		if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
		if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
		if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
		if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}
		
		if (IsFilled)	//Ö¸¶¨Ô²»¡Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
				if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
			}
			
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -x; j < x; j ++)
			{
				/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
				if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}

/*********************¹¦ÄÜº¯Êı*/


/*****************½­Ğ­¿Æ¼¼|°æÈ¨ËùÓĞ****************/
/*****************jiangxiekeji.com*****************/

/***************************************************************************************
  * ±¾³ÌĞòÓÉ½­Ğ­¿Æ¼¼´´½¨²¢Ãâ·Ñ¿ªÔ´¹²Ïí
  * Äã¿ÉÒÔÈÎÒâ²é¿´¡¢Ê¹ÓÃºÍĞŞ¸Ä£¬²¢Ó¦ÓÃµ½×Ô¼ºµÄÏîÄ¿Ö®ÖĞ
  * ³ÌĞò°æÈ¨¹é½­Ğ­¿Æ¼¼ËùÓĞ£¬ÈÎºÎÈË»ò×éÖ¯²»µÃ½«Æä¾İÎª¼ºÓĞ
  * 
  * ³ÌĞòÃû³Æ£º				0.96´çOLEDÏÔÊ¾ÆÁÇı¶¯³ÌĞò£¨4Õë½ÅI2C½Ó¿Ú£©
  * ³ÌĞò´´½¨Ê±¼ä£º			2023.10.24
  * µ±Ç°³ÌĞò°æ±¾£º			V1.1
  * µ±Ç°°æ±¾·¢²¼Ê±¼ä£º		2023.12.8
  * 
  * ½­Ğ­¿Æ¼¼¹Ù·½ÍøÕ¾£º		jiangxiekeji.com
  * ½­Ğ­¿Æ¼¼¹Ù·½ÌÔ±¦µê£º	jiangxiekeji.taobao.com
  * ³ÌĞò½éÉÜ¼°¸üĞÂ¶¯Ì¬£º	jiangxiekeji.com/tutorial/oled.html
  * 
  * Èç¹ûÄã·¢ÏÖ³ÌĞòÖĞµÄÂ©¶´»òÕß±ÊÎó£¬¿ÉÍ¨¹ıÓÊ¼şÏòÎÒÃÇ·´À¡£ºfeedback@jiangxiekeji.com
  * ·¢ËÍÓÊ¼şÖ®Ç°£¬Äã¿ÉÒÔÏÈµ½¸üĞÂ¶¯Ì¬Ò³Ãæ²é¿´×îĞÂ³ÌĞò£¬Èç¹û´ËÎÊÌâÒÑ¾­ĞŞ¸Ä£¬ÔòÎŞĞèÔÙ·¢ÓÊ¼ş
  ***************************************************************************************
  */


#elif (OLED_mode==Software)

/**
  * Êı¾İ´æ´¢¸ñÊ½£º
  * ×İÏò8µã£¬¸ßÎ»ÔÚÏÂ£¬ÏÈ´Ó×óµ½ÓÒ£¬ÔÙ´ÓÉÏµ½ÏÂ
  * Ã¿Ò»¸öBit¶ÔÓ¦Ò»¸öÏñËØµã
  * 
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |   
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  * 
  * ×ø±êÖá¶¨Òå£º
  * ×óÉÏ½ÇÎª(0, 0)µã
  * ºáÏòÏòÓÒÎªXÖá£¬È¡Öµ·¶Î§£º0~127
  * ×İÏòÏòÏÂÎªYÖá£¬È¡Öµ·¶Î§£º0~63
  * 
  *       0             XÖá           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  YÖá |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */


/*È«¾Ö±äÁ¿*********************/

/**
  * OLEDÏÔ´æÊı×é
  * ËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶Ô´ËÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  * Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  * ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  */
uint8_t OLED_DisplayBuf[8][128];

/*********************È«¾Ö±äÁ¿*/


/*Òı½ÅÅäÖÃ*********************/


/* I2Cµ×²ãÇı¶¯ */
#define I2C_DELAY()     delay_us(1)    // I2CÊ±ĞòÑÓÊ±
#define OLED_SCL_HIGH() LL_GPIO_SetOutputPin(OLED_GPIO_PORT, OLED_SCL_PIN)
#define OLED_SCL_LOW()  LL_GPIO_ResetOutputPin(OLED_GPIO_PORT, OLED_SCL_PIN)
#define OLED_SDA_HIGH() LL_GPIO_SetOutputPin(OLED_GPIO_PORT, OLED_SDA_PIN)
#define OLED_SDA_LOW()  LL_GPIO_ResetOutputPin(OLED_GPIO_PORT, OLED_SDA_PIN)
#define OLED_SDA_READ() LL_GPIO_IsInputPinSet(OLED_GPIO_PORT, OLED_SDA_PIN)
/* ¼Ä´æÆ÷Ö±½Ó²Ù×÷¶¨Òå */
//#define OLED_GPIOB_BASE 0x40020400
//#define GPIOB_BSRR    (*(volatile uint32_t *)(OLED_GPIOB_BASE + 0x18))		// ÖÃÎ»¼Ä´æÆ÷
//#define GPIOB_BRR     (*(volatile uint32_t *)(OLED_GPIOB_BASE + 0x28))			// ¸´Î»¼Ä´æÆ÷
//#define GPIOB_IDR     (*(volatile uint32_t *)(OLED_GPIOB_BASE + 0x10)) 	 // ÊäÈëÊı¾İ¼Ä´æÆ÷
//#define SDA_PIN       11  // PB11
//#define SCL_PIN       10  // PB10

///* ¼Ä´æÆ÷²Ù×÷ºê¶¨Òå */
//#define OLED_SCL_HIGH()  GPIOB_BSRR = (1 << SCL_PIN)        // SCLÖÃ¸ß
//#define OLED_SCL_LOW()   GPIOB_BRR  = (1 << SCL_PIN)       // SCLÖÃµÍ
//#define OLED_SDA_HIGH()  GPIOB_BSRR = (1 << SDA_PIN)       // SDAÖÃ¸ß
//#define OLED_SDA_LOW()   GPIOB_BRR  = (1 << SDA_PIN)       // SDAÖÃµÍ
//#define OLED_SDA_READ()  (GPIOB_IDR & (1 << SDA_PIN))      // ¶ÁÈ¡SDA×´Ì¬

/* ¾«È·ÑÓÊ±£¨Ê¹ÓÃNOPÖ¸Áî£© */
//#define I2C_DELAY() do { __NOP();__NOP(); __NOP(); __NOP(); __NOP(); } while(0)


/**
  * º¯    Êı£ºI2C³õÊ¼»¯
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  */
static void OLED_I2C_Init(void)
{
    // GPIO_InitTypeDef GPIO_InitStruct = {
    //     .GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN,
    //     .GPIO_Mode = GPIO_Mode_OUT,       // Êä³öÄ£Ê½
    //     .GPIO_OType = GPIO_OType_OD,      // ¿ªÂ©Êä³ö
    //     .GPIO_Speed = GPIO_Speed_100MHz,   // ¸ßËÙÄ£Ê½
    //     .GPIO_PuPd = GPIO_PuPd_UP         // ÉÏÀ­µç×è
    // };
    // RCC_AHB1PeriphClockCmd(OLED_RCC_CLOCK, ENABLE);
    // GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);
    OLED_SCL_HIGH();  // ³õÊ¼ÖÃ¸ß
    OLED_SDA_HIGH();
}

/*********************Òı½ÅÅäÖÃ*/


/*Í¨ĞÅĞ­Òé*********************/

/**
  * º¯    Êı£ºI2CÆğÊ¼ĞÅºÅ
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  */
static void OLED_I2C_Start(void)
{
    OLED_SDA_HIGH();  // È·±£SDA¸ßµçÆ½
    OLED_SCL_HIGH();  // È·±£SCL¸ßµçÆ½
    I2C_DELAY();      // ±£³ÖÊ±¼ä>4.7us
    OLED_SDA_LOW();   // SDAÏÂ½µÑØ
    I2C_DELAY();
    OLED_SCL_LOW();   // Ç¯×¡×ÜÏß¿ªÊ¼´«Êä
}


/**
  * º¯    Êı£ºI2CÍ£Ö¹ĞÅºÅ
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  */
static void OLED_I2C_Stop(void)
{
    OLED_SDA_LOW();   // È·±£SDAµÍµçÆ½
    I2C_DELAY();
    OLED_SCL_HIGH();  // SCL¸ßµçÆ½
    I2C_DELAY();
    OLED_SDA_HIGH();  // SDAÉÏÉıÑØ
}
/**
  * º¯    Êı£ºI2C·¢ËÍÒ»¸ö×Ö½Ú
  * ²Î    Êı£ºbyte Òª·¢ËÍµÄÊı¾İ
  * ·µ »Ø Öµ£ºÎŞ
  */
static void OLED_I2C_SendByte(uint8_t byte)
{
    /* Ñ­»··¢ËÍ8Î»Êı¾İ */
    for(uint8_t i = 0; i < 8; i++) {
        /* ´Ó¸ßÎ»¿ªÊ¼ÒÀ´Î·¢ËÍ */
        if(byte & 0x80) {
						OLED_SDA_HIGH();
				} else {
						OLED_SDA_LOW();
				}
        byte <<= 1;  // ×óÒÆ×¼±¸ÏÂÒ»Î»
        
        I2C_DELAY();
        OLED_SCL_HIGH();  // ²úÉúÉÏÉıÑØ
        I2C_DELAY();
        OLED_SCL_LOW();   // À­µÍ×¼±¸ÏÂÒ»Î»
    }
    OLED_SDA_HIGH();  // ÊÍ·ÅSDAÏß
}

/**
  * º¯    Êı£ºI2C½ÓÊÕÓ¦´ğ
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£º0-Ó¦´ğ³É¹¦£¬1-ÎŞÓ¦´ğ
  */
static uint8_t OLED_I2C_ReceiveAck(void)
{
    uint8_t ack = 1;
    OLED_SDA_HIGH();  // Ö÷»úÊÍ·ÅSDA
    I2C_DELAY();
    OLED_SCL_HIGH();  // µÚ9¸öÊ±ÖÓÂö³å
    I2C_DELAY();
    ack = OLED_SDA_READ(); // ¶ÁÈ¡´Ó»úÓ¦´ğ
    OLED_SCL_LOW();   // ½áÊøÊ±ÖÓÂö³å
    return ack;
}
/**
  * º¯    Êı£ºOLEDĞ´ÃüÁî
  * ²Î    Êı£ºcmd ÒªĞ´ÈëµÄÃüÁî
  * ·µ »Ø Öµ£ºÎŞ
  */
void OLED_WriteCommand(uint8_t cmd)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_I2C_ADDR); // ·¢ËÍÆ÷¼şµØÖ·
    OLED_I2C_ReceiveAck();
    OLED_I2C_SendByte(0x00);          // ¿ØÖÆ×Ö½Ú£¨Ğ´ÃüÁî£©
    OLED_I2C_ReceiveAck();
    OLED_I2C_SendByte(cmd);           // ·¢ËÍ¾ßÌåÃüÁî
    OLED_I2C_ReceiveAck();
    OLED_I2C_Stop();
}


/**
  * º¯    Êı£ºOLEDĞ´Êı¾İ
  * ²Î    Êı£ºdata Êı¾İÖ¸Õë
  * ²Î    Êı£ºlen  Êı¾İ³¤¶È
  * ·µ »Ø Öµ£ºÎŞ
  */
void OLED_WriteData(uint8_t* data, uint16_t len)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_I2C_ADDR);
    OLED_I2C_ReceiveAck();
    OLED_I2C_SendByte(0x40);          // ¿ØÖÆ×Ö½Ú£¨Ğ´Êı¾İ£©
    OLED_I2C_ReceiveAck();
    
    /* Á¬ĞøĞ´ÈëÊı¾İ */
    for(uint16_t i = 0; i < len; i++) {
        OLED_I2C_SendByte(data[i]);
        if(OLED_I2C_ReceiveAck()) {   // ¼ì²âÓ¦´ğ
            OLED_I2C_Stop();          // ´íÎóÊ±ÌáÇ°ÖÕÖ¹
            return;
        }
    }
    OLED_I2C_Stop();
}


/*********************Í¨ĞÅĞ­Òé*/


/*Ó²¼şÅäÖÃ*********************/

/**
  * º¯    Êı£ºOLED³õÊ¼»¯
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºÊ¹ÓÃÇ°£¬ĞèÒªµ÷ÓÃ´Ë³õÊ¼»¯º¯Êı
  */
void OLED_Init(void)
{
    OLED_I2C_Init();  // ÏÈ³õÊ¼»¯I2C×ÜÏß
    
    /* OLED³õÊ¼»¯ÃüÁîĞòÁĞ */
    uint8_t init_cmds[] = {
        0xAE,       // ¹Ø±ÕÏÔÊ¾
        0xD5, 0x80, // ÉèÖÃÏÔÊ¾Ê±ÖÓ·ÖÆµ±È
        0xA8, 0x3F, // ÉèÖÃ¶àÂ·¸´ÓÃÂÊ
        0xD3, 0x00, // ÉèÖÃÏÔÊ¾Æ«ÒÆ
        0x40,       // ÉèÖÃÏÔÊ¾ÆğÊ¼ĞĞ
        0xA1,       // ÉèÖÃ¶ÎÖØ¶¨Ïò
        0xC8,       // ÉèÖÃCOMÉ¨Ãè·½Ïò
        0xDA, 0x12, // ÉèÖÃCOMÓ²¼şÅäÖÃ
        0x81, 0xCF, // ÉèÖÃ¶Ô±È¶È
        0xD9, 0xF1, // ÉèÖÃÔ¤³äµçÖÜÆÚ
        0xDB, 0x30, // ÉèÖÃVCOMHµçÆ½
        0xA4,       // ¹Ø±ÕÈ«¾ÖÏÔÊ¾
        0xA6,       // ÉèÖÃÕı³£ÏÔÊ¾Ä£Ê½
        0xAF        // ¿ªÆôÏÔÊ¾
    };
    
    for(uint8_t i = 0; i < sizeof(init_cmds); i++) {
        OLED_WriteCommand(init_cmds[i]);
    }
    OLED_Clear();    // Çå¿ÕÏÔ´æ
    OLED_Update();   // È«ÆÁË¢ĞÂ
}

/**
  * º¯    Êı£ºOLEDÉèÖÃÏÔÊ¾¹â±êÎ»ÖÃ
  * ²Î    Êı£ºPage Ö¸¶¨¹â±êËùÔÚµÄÒ³£¬·¶Î§£º0~7
  * ²Î    Êı£ºX Ö¸¶¨¹â±êËùÔÚµÄXÖá×ø±ê£¬·¶Î§£º0~127
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºOLEDÄ¬ÈÏµÄYÖá£¬Ö»ÄÜ8¸öBitÎªÒ»×éĞ´Èë£¬¼´1Ò³µÈÓÚ8¸öYÖá×ø±ê
  */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*Èç¹ûÊ¹ÓÃ´Ë³ÌĞòÇı¶¯1.3´çµÄOLEDÏÔÊ¾ÆÁ£¬ÔòĞèÒª½â³ı´Ë×¢ÊÍ*/
	/*ÒòÎª1.3´çµÄOLEDÇı¶¯Ğ¾Æ¬£¨SH1106£©ÓĞ132ÁĞ*/
	/*ÆÁÄ»µÄÆğÊ¼ÁĞ½ÓÔÚÁËµÚ2ÁĞ£¬¶ø²»ÊÇµÚ0ÁĞ*/
	/*ËùÒÔĞèÒª½«X¼Ó2£¬²ÅÄÜÕı³£ÏÔÊ¾*/
	X += 2;
	
	/*Í¨¹ıÖ¸ÁîÉèÖÃÒ³µØÖ·ºÍÁĞµØÖ·*/
	OLED_WriteCommand(0xB0 | Page);					//ÉèÖÃÒ³Î»ÖÃ
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//ÉèÖÃXÎ»ÖÃ¸ß4Î»
	OLED_WriteCommand(0x00 | (X & 0x0F));			//ÉèÖÃXÎ»ÖÃµÍ4Î»
}

/*********************Ó²¼şÅäÖÃ*/


/*¹¤¾ßº¯Êı*********************/

/*¹¤¾ßº¯Êı½ö¹©ÄÚ²¿²¿·Öº¯ÊıÊ¹ÓÃ*/

/**
  * º¯    Êı£º´Î·½º¯Êı
  * ²Î    Êı£ºX µ×Êı
  * ²Î    Êı£ºY Ö¸Êı
  * ·µ »Ø Öµ£ºµÈÓÚXµÄY´Î·½
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//½á¹ûÄ¬ÈÏÎª1
	while (Y --)			//ÀÛ³ËY´Î
	{
		Result *= X;		//Ã¿´Î°ÑXÀÛ³Ëµ½½á¹ûÉÏ
	}
	return Result;
}

/**
  * º¯    Êı£ºÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨¶à±ßĞÎÄÚ²¿
  * ²Î    Êı£ºnvert ¶à±ßĞÎµÄ¶¥µãÊı
  * ²Î    Êı£ºvertx verty °üº¬¶à±ßĞÎ¶¥µãµÄxºÍy×ø±êµÄÊı×é
  * ²Î    Êı£ºtestx testy ²âÊÔµãµÄXºÍy×ø±ê
  * ·µ »Ø Öµ£ºÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨¶à±ßĞÎÄÚ²¿£¬1£ºÔÚÄÚ²¿£¬0£º²»ÔÚÄÚ²¿
  */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i, j, c = 0;
	
	/*´ËËã·¨ÓÉW. Randolph FranklinÌá³ö*/
	/*²Î¿¼Á´½Ó£ºhttps://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
  * º¯    Êı£ºÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ²¿
  * ²Î    Êı£ºX Y Ö¸¶¨µãµÄ×ø±ê
  * ²Î    Êı£ºStartAngle EndAngle ÆğÊ¼½Ç¶ÈºÍÖÕÖ¹½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ·µ »Ø Öµ£ºÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ²¿£¬1£ºÔÚÄÚ²¿£¬0£º²»ÔÚÄÚ²¿
  */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//¼ÆËãÖ¸¶¨µãµÄ»¡¶È£¬²¢×ª»»Îª½Ç¶È±íÊ¾
	if (StartAngle < EndAngle)	//ÆğÊ¼½Ç¶ÈĞ¡ÓÚÖÕÖ¹½Ç¶ÈµÄÇé¿ö
	{
		/*Èç¹ûÖ¸¶¨½Ç¶ÈÔÚÆğÊ¼ÖÕÖ¹½Ç¶ÈÖ®¼ä£¬ÔòÅĞ¶¨Ö¸¶¨µãÔÚÖ¸¶¨½Ç¶È*/
		if (PointAngle >= StartAngle && PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	else			//ÆğÊ¼½Ç¶È´óÓÚÓÚÖÕÖ¹½Ç¶ÈµÄÇé¿ö
	{
		/*Èç¹ûÖ¸¶¨½Ç¶È´óÓÚÆğÊ¼½Ç¶È»òÕßĞ¡ÓÚÖÕÖ¹½Ç¶È£¬ÔòÅĞ¶¨Ö¸¶¨µãÔÚÖ¸¶¨½Ç¶È*/
		if (PointAngle >= StartAngle || PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	return 0;		//²»Âú×ãÒÔÉÏÌõ¼ş£¬ÔòÅĞ¶ÏÅĞ¶¨Ö¸¶¨µã²»ÔÚÖ¸¶¨½Ç¶È
}

/*********************¹¤¾ßº¯Êı*/


/*¹¦ÄÜº¯Êı*********************/

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é¸üĞÂµ½OLEDÆÁÄ»
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶ÔOLEDÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  *           Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  *           ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  *           ¹Êµ÷ÓÃÏÔÊ¾º¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Update(void)
{
	uint8_t j;
	/*±éÀúÃ¿Ò»Ò³*/
	for (j = 0; j < 8; j ++)
	{
		/*ÉèÖÃ¹â±êÎ»ÖÃÎªÃ¿Ò»Ò³µÄµÚÒ»ÁĞ*/
		OLED_SetCursor(j, 0);
		/*Á¬ĞøĞ´Èë128¸öÊı¾İ£¬½«ÏÔ´æÊı×éµÄÊı¾İĞ´Èëµ½OLEDÓ²¼ş*/
		OLED_WriteData(OLED_DisplayBuf[j], 128);
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·Ö¸üĞÂµ½OLEDÆÁÄ»
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£º´Ëº¯Êı»áÖÁÉÙ¸üĞÂ²ÎÊıÖ¸¶¨µÄÇøÓò
  *           Èç¹û¸üĞÂÇøÓòYÖáÖ»°üº¬²¿·ÖÒ³£¬ÔòÍ¬Ò»Ò³µÄÊ£Óà²¿·Ö»á¸úËæÒ»Æğ¸üĞÂ
  * Ëµ    Ã÷£ºËùÓĞµÄÏÔÊ¾º¯Êı£¬¶¼Ö»ÊÇ¶ÔOLEDÏÔ´æÊı×é½øĞĞ¶ÁĞ´
  *           Ëæºóµ÷ÓÃOLED_Updateº¯Êı»òOLED_UpdateAreaº¯Êı
  *           ²Å»á½«ÏÔ´æÊı×éµÄÊı¾İ·¢ËÍµ½OLEDÓ²¼ş£¬½øĞĞÏÔÊ¾
  *           ¹Êµ÷ÓÃÏÔÊ¾º¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	/*±éÀúÖ¸¶¨ÇøÓòÉæ¼°µÄÏà¹ØÒ³*/
	/*(Y + Height - 1) / 8 + 1µÄÄ¿µÄÊÇ(Y + Height) / 8²¢ÏòÉÏÈ¡Õû*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*ÉèÖÃ¹â±êÎ»ÖÃÎªÏà¹ØÒ³µÄÖ¸¶¨ÁĞ*/
		OLED_SetCursor(j, X);
		/*Á¬ĞøĞ´ÈëWidth¸öÊı¾İ£¬½«ÏÔ´æÊı×éµÄÊı¾İĞ´Èëµ½OLEDÓ²¼ş*/
		OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×éÈ«²¿ÇåÁã
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Clear(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//±éÀú8Ò³
	{
		for (i = 0; i < 128; i ++)			//±éÀú128ÁĞ
		{
			OLED_DisplayBuf[j][i] = 0x00;	//½«ÏÔ´æÊı×éÊı¾İÈ«²¿ÇåÁã
		}
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·ÖÇåÁã
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//±éÀúÖ¸¶¨Ò³
	{
		for (i = X; i < X + Width; i ++)	//±éÀúÖ¸¶¨ÁĞ
		{
			OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));	//½«ÏÔ´æÊı×éÖ¸¶¨Êı¾İÇåÁã
		}
	}
}

/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×éÈ«²¿È¡·´
  * ²Î    Êı£ºÎŞ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Reverse(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//±éÀú8Ò³
	{
		for (i = 0; i < 128; i ++)			//±éÀú128ÁĞ
		{
			OLED_DisplayBuf[j][i] ^= 0xFF;	//½«ÏÔ´æÊı×éÊı¾İÈ«²¿È¡·´
		}
	}
}
	
/**
  * º¯    Êı£º½«OLEDÏÔ´æÊı×é²¿·ÖÈ¡·´
  * ²Î    Êı£ºX Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÇøÓò×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨ÇøÓòµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨ÇøÓòµÄ¸ß¶È£¬·¶Î§£º0~64
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨ÇøÓò²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//±éÀúÖ¸¶¨Ò³
	{
		for (i = X; i < X + Width; i ++)	//±éÀúÖ¸¶¨ÁĞ
		{
			OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//½«ÏÔ´æÊı×éÖ¸¶¨Êı¾İÈ¡·´
		}
	}
}

void OLED_ReverseArea_My(uint8_t Y, uint8_t X, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ReverseArea((X-1)*6,(Y-1)*8,Length*6,8);
	}
	else if(FontSize==OLED_8X16)
	{
		OLED_ReverseArea((X-1)*8,(Y-1)*16,Length*8,16);
	}
}


/**
  * º¯    Êı£ºOLEDÏÔÊ¾Ò»¸ö×Ö·û
  * ²Î    Êı£ºX Ö¸¶¨×Ö·û×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨×Ö·û×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºChar Ö¸¶¨ÒªÏÔÊ¾µÄ×Ö·û£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
	if (FontSize == OLED_8X16)		//×ÖÌåÎª¿í8ÏñËØ£¬¸ß16ÏñËØ
	{
		/*½«ASCII×ÖÄ£¿âOLED_F8x16µÄÖ¸¶¨Êı¾İÒÔ8*16µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
		OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
	}
	else if(FontSize == OLED_6X8)	//×ÖÌåÎª¿í6ÏñËØ£¬¸ß8ÏñËØ
	{
		/*½«ASCII×ÖÄ£¿âOLED_F6x8µÄÖ¸¶¨Êı¾İÒÔ6*8µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
		OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
	}
}

void OLED_ShowChar_My(uint8_t Y, uint8_t X, char Char, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowChar((X-1)*6,(Y-1)*8,Char,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowChar((X-1)*8,(Y-1)*16,Char,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾×Ö·û´®
  * ²Î    Êı£ºX Ö¸¶¨×Ö·û´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨×Ö·û´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºString Ö¸¶¨ÒªÏÔÊ¾µÄ×Ö·û´®£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û×é³ÉµÄ×Ö·û´®
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)		//±éÀú×Ö·û´®µÄÃ¿¸ö×Ö·û
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸ö×Ö·û*/
		OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
	}
}

void OLED_ShowString_My(uint8_t Y, uint8_t X, char *String, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowString((X-1)*6,(Y-1)*8,String,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowString((X-1)*8,(Y-1)*16,String,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Êı×Ö£¨Ê®½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0~4294967295
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»							
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number / OLED_Pow(10, Length - i - 1) % 10 ¿ÉÒÔÊ®½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

void OLED_ShowNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾ÓĞ·ûºÅÊı×Ö£¨Ê®½øÖÆ£¬ÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º-2147483648~2147483647
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	uint32_t Number1;
	
	if (Number >= 0)						//Êı×Ö´óÓÚµÈÓÚ0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//ÏÔÊ¾+ºÅ
		Number1 = Number;					//Number1Ö±½ÓµÈÓÚNumber
	}
	else									//Êı×ÖĞ¡ÓÚ0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//ÏÔÊ¾-ºÅ
		Number1 = -Number;					//Number1µÈÓÚNumberÈ¡¸º
	}
	
	for (i = 0; i < Length; i++)			//±éÀúÊı×ÖµÄÃ¿Ò»Î»								
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number1 / OLED_Pow(10, Length - i - 1) % 10 ¿ÉÒÔÊ®½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

void OLED_ShowSignedNum_My(uint8_t Y, uint8_t X, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowSignedNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowSignedNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Ê®Áù½øÖÆÊı×Ö£¨Ê®Áù½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0x00000000~0xFFFFFFFF
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~8
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»
	{
		/*ÒÔÊ®Áù½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		
		if (SingleNumber < 10)			//µ¥¸öÊı×ÖĞ¡ÓÚ10
		{
			/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÏÔÊ¾´ËÊı×Ö*/
			/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
		}
		else							//µ¥¸öÊı×Ö´óÓÚ10
		{
			/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÏÔÊ¾´ËÊı×Ö*/
			/*+ 'A' ¿É½«Êı×Ö×ª»»Îª´ÓA¿ªÊ¼µÄÊ®Áù½øÖÆ×Ö·û*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

void OLED_ShowHexNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowHexNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowHexNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}	
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾¶ş½øÖÆÊı×Ö£¨¶ş½øÖÆ£¬ÕıÕûÊı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º0x00000000~0xFFFFFFFF
  * ²Î    Êı£ºLength Ö¸¶¨Êı×ÖµÄ³¤¶È£¬·¶Î§£º0~16
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//±éÀúÊı×ÖµÄÃ¿Ò»Î»	
	{
		/*µ÷ÓÃOLED_ShowCharº¯Êı£¬ÒÀ´ÎÏÔÊ¾Ã¿¸öÊı×Ö*/
		/*Number / OLED_Pow(2, Length - i - 1) % 2 ¿ÉÒÔ¶ş½øÖÆÌáÈ¡Êı×ÖµÄÃ¿Ò»Î»*/
		/*+ '0' ¿É½«Êı×Ö×ª»»Îª×Ö·û¸ñÊ½*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
	}
}

void OLED_ShowBinNum_My(uint8_t Y, uint8_t X, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowBinNum((X-1)*6,(Y-1)*8,Number,Length,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowBinNum((X-1)*8,(Y-1)*16,Number,Length,FontSize);
	}
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾¸¡µãÊı×Ö£¨Ê®½øÖÆ£¬Ğ¡Êı£©
  * ²Î    Êı£ºX Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Êı×Ö×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºNumber Ö¸¶¨ÒªÏÔÊ¾µÄÊı×Ö£¬·¶Î§£º-4294967295.0~4294967295.0
  * ²Î    Êı£ºIntLength Ö¸¶¨Êı×ÖµÄÕûÊıÎ»³¤¶È£¬·¶Î§£º0~10
  * ²Î    Êı£ºFraLength Ö¸¶¨Êı×ÖµÄĞ¡ÊıÎ»³¤¶È£¬·¶Î§£º0~9£¬Ğ¡Êı½øĞĞËÄÉáÎåÈëÏÔÊ¾
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	uint32_t PowNum, IntNum, FraNum;
	
	if (Number >= 0)						//Êı×Ö´óÓÚµÈÓÚ0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//ÏÔÊ¾+ºÅ
	}
	else									//Êı×ÖĞ¡ÓÚ0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//ÏÔÊ¾-ºÅ
		Number = -Number;					//NumberÈ¡¸º
	}
	
	/*ÌáÈ¡ÕûÊı²¿·ÖºÍĞ¡Êı²¿·Ö*/
	IntNum = Number;						//Ö±½Ó¸³Öµ¸øÕûĞÍ±äÁ¿£¬ÌáÈ¡ÕûÊı
	Number -= IntNum;						//½«NumberµÄÕûÊı¼õµô£¬·ÀÖ¹Ö®ºó½«Ğ¡Êı³Ëµ½ÕûÊıÊ±ÒòÊı¹ı´óÔì³É´íÎó
	PowNum = OLED_Pow(10, FraLength);		//¸ù¾İÖ¸¶¨Ğ¡ÊıµÄÎ»Êı£¬È·¶¨³ËÊı
	FraNum = round(Number * PowNum);		//½«Ğ¡Êı³Ëµ½ÕûÊı£¬Í¬Ê±ËÄÉáÎåÈë£¬±ÜÃâÏÔÊ¾Îó²î
	IntNum += FraNum / PowNum;				//ÈôËÄÉáÎåÈëÔì³ÉÁË½øÎ»£¬ÔòĞèÒªÔÙ¼Ó¸øÕûÊı
	
	/*ÏÔÊ¾ÕûÊı²¿·Ö*/
	OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);
	
	/*ÏÔÊ¾Ğ¡Êıµã*/
	OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
	
	/*ÏÔÊ¾Ğ¡Êı²¿·Ö*/
	OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);
}

void OLED_ShowFloatNum_My(uint8_t Y, uint8_t X, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	if(FontSize==OLED_6X8)
	{
		OLED_ShowFloatNum((X-1)*6,(Y-1)*8,Number,IntLength,FraLength,FontSize);
	}else if(FontSize==OLED_8X16)
	{
		OLED_ShowFloatNum((X-1)*8,(Y-1)*16,Number,IntLength,FraLength,FontSize);
	}
}


/**
  * º¯    Êı£ºOLEDÏÔÊ¾ºº×Ö´®
  * ²Î    Êı£ºX Ö¸¶¨ºº×Ö´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ºº×Ö´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºChinese Ö¸¶¨ÒªÏÔÊ¾µÄºº×Ö´®£¬·¶Î§£º±ØĞëÈ«²¿Îªºº×Ö»òÕßÈ«½Ç×Ö·û£¬²»Òª¼ÓÈëÈÎºÎ°ë½Ç×Ö·û
  *           ÏÔÊ¾µÄºº×ÖĞèÒªÔÚOLED_Data.cÀïµÄOLED_CF16x16Êı×é¶¨Òå
  *           Î´ÕÒµ½Ö¸¶¨ºº×ÖÊ±£¬»áÏÔÊ¾Ä¬ÈÏÍ¼ĞÎ£¨Ò»¸ö·½¿ò£¬ÄÚ²¿Ò»¸öÎÊºÅ£©
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
	uint8_t pChinese = 0;
	uint8_t pIndex;
	uint8_t i;
	char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
	
	for (i = 0; Chinese[i] != '\0'; i ++)		//±éÀúºº×Ö´®
	{
		SingleChinese[pChinese] = Chinese[i];	//ÌáÈ¡ºº×Ö´®Êı¾İµ½µ¥¸öºº×ÖÊı×é
		pChinese ++;							//¼Æ´Î×ÔÔö
		
		/*µ±ÌáÈ¡´ÎÊıµ½´ïOLED_CHN_CHAR_WIDTHÊ±£¬¼´´ú±íÌáÈ¡µ½ÁËÒ»¸öÍêÕûµÄºº×Ö*/
		if (pChinese >= OLED_CHN_CHAR_WIDTH)
		{
			pChinese = 0;		//¼Æ´Î¹éÁã
			
			/*±éÀúÕû¸öºº×Ö×ÖÄ£¿â£¬Ñ°ÕÒÆ¥ÅäµÄºº×Ö*/
			/*Èç¹ûÕÒµ½×îºóÒ»¸öºº×Ö£¨¶¨ÒåÎª¿Õ×Ö·û´®£©£¬Ôò±íÊ¾ºº×ÖÎ´ÔÚ×ÖÄ£¿â¶¨Òå£¬Í£Ö¹Ñ°ÕÒ*/
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				/*ÕÒµ½Æ¥ÅäµÄºº×Ö*/
				if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
				{
					break;		//Ìø³öÑ­»·£¬´ËÊ±pIndexµÄÖµÎªÖ¸¶¨ºº×ÖµÄË÷Òı
				}
			}
			
			/*½«ºº×Ö×ÖÄ£¿âOLED_CF16x16µÄÖ¸¶¨Êı¾İÒÔ16*16µÄÍ¼Ïñ¸ñÊ½ÏÔÊ¾*/
			OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
		}
	}
}

void OLED_ShowChinese_My(uint8_t Y, uint8_t X, char *Chinese)
{
	OLED_ShowChinese((X-1)*8,(Y-1)*16,Chinese);
}

/**
  * º¯    Êı£ºOLEDÏÔÊ¾Í¼Ïñ
  * ²Î    Êı£ºX Ö¸¶¨Í¼Ïñ×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Í¼Ïñ×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨Í¼ÏñµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨Í¼ÏñµÄ¸ß¶È£¬·¶Î§£º0~64
  * ²Î    Êı£ºImage Ö¸¶¨ÒªÏÔÊ¾µÄÍ¼Ïñ
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Í¼Ïñ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*½«Í¼ÏñËùÔÚÇøÓòÇå¿Õ*/
	OLED_ClearArea(X, Y, Width, Height);
	
	/*±éÀúÖ¸¶¨Í¼ÏñÉæ¼°µÄÏà¹ØÒ³*/
	/*(Height - 1) / 8 + 1µÄÄ¿µÄÊÇHeight / 8²¢ÏòÉÏÈ¡Õû*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*±éÀúÖ¸¶¨Í¼ÏñÉæ¼°µÄÏà¹ØÁĞ*/
		for (i = 0; i < Width; i ++)
		{
			/*³¬³ö±ß½ç£¬ÔòÌø¹ıÏÔÊ¾*/
			if (X + i > 127) {break;}
			if (Y / 8 + j > 7) {return;}
			
			/*ÏÔÊ¾Í¼ÏñÔÚµ±Ç°Ò³µÄÄÚÈİ*/
			OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
			
			/*³¬³ö±ß½ç£¬ÔòÌø¹ıÏÔÊ¾*/
			/*Ê¹ÓÃcontinueµÄÄ¿µÄÊÇ£¬ÏÂÒ»Ò³³¬³ö±ß½çÊ±£¬ÉÏÒ»Ò³µÄºóĞøÄÚÈİ»¹ĞèÒª¼ÌĞøÏÔÊ¾*/
			if (Y / 8 + j + 1 > 7) {continue;}
			
			/*ÏÔÊ¾Í¼ÏñÔÚÏÂÒ»Ò³µÄÄÚÈİ*/
			OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
		}
	}
}

/**
  * º¯    Êı£ºOLEDÊ¹ÓÃprintfº¯Êı´òÓ¡¸ñÊ½»¯×Ö·û´®
  * ²Î    Êı£ºX Ö¸¶¨¸ñÊ½»¯×Ö·û´®×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨¸ñÊ½»¯×Ö·û´®×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºFontSize Ö¸¶¨×ÖÌå´óĞ¡
  *           ·¶Î§£ºOLED_8X16		¿í8ÏñËØ£¬¸ß16ÏñËØ
  *                 OLED_6X8		¿í6ÏñËØ£¬¸ß8ÏñËØ
  * ²Î    Êı£ºformat Ö¸¶¨ÒªÏÔÊ¾µÄ¸ñÊ½»¯×Ö·û´®£¬·¶Î§£ºASCIIÂë¿É¼û×Ö·û×é³ÉµÄ×Ö·û´®
  * ²Î    Êı£º... ¸ñÊ½»¯×Ö·û´®²ÎÊıÁĞ±í
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
{
	char String[30];						//¶¨Òå×Ö·ûÊı×é
	va_list arg;							//¶¨Òå¿É±ä²ÎÊıÁĞ±íÊı¾İÀàĞÍµÄ±äÁ¿arg
	va_start(arg, format);					//´Óformat¿ªÊ¼£¬½ÓÊÕ²ÎÊıÁĞ±íµ½arg±äÁ¿
	vsprintf(String, format, arg);			//Ê¹ÓÃvsprintf´òÓ¡¸ñÊ½»¯×Ö·û´®ºÍ²ÎÊıÁĞ±íµ½×Ö·ûÊı×éÖĞ
	va_end(arg);							//½áÊø±äÁ¿arg
	OLED_ShowString(X, Y, String, FontSize);//OLEDÏÔÊ¾×Ö·ûÊı×é£¨×Ö·û´®£©
}

/**
  * º¯    Êı£ºOLEDÔÚÖ¸¶¨Î»ÖÃ»­Ò»¸öµã
  * ²Î    Êı£ºX Ö¸¶¨µãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨µãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Î»ÖÃ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*½«ÏÔ´æÊı×éÖ¸¶¨Î»ÖÃµÄÒ»¸öBitÊı¾İÖÃ1*/
	OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

/**
  * º¯    Êı£ºOLED»ñÈ¡Ö¸¶¨Î»ÖÃµãµÄÖµ
  * ²Î    Êı£ºX Ö¸¶¨µãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨µãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÖ¸¶¨Î»ÖÃµãÊÇ·ñ´¦ÓÚµãÁÁ×´Ì¬£¬1£ºµãÁÁ£¬0£ºÏ¨Ãğ
  */
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y)
{
	/*²ÎÊı¼ì²é£¬±£Ö¤Ö¸¶¨Î»ÖÃ²»»á³¬³öÆÁÄ»·¶Î§*/
	if (X > 127) {return 0;}
	if (Y > 63) {return 0;}
	
	/*ÅĞ¶ÏÖ¸¶¨Î»ÖÃµÄÊı¾İ*/
	if (OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8))
	{
		return 1;	//Îª1£¬·µ»Ø1
	}
	
	return 0;		//·ñÔò£¬·µ»Ø0
}

/**
  * º¯    Êı£ºOLED»­Ïß
  * ²Î    Êı£ºX0 Ö¸¶¨Ò»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY0 Ö¸¶¨Ò»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX1 Ö¸¶¨ÁíÒ»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY1 Ö¸¶¨ÁíÒ»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//ºáÏßµ¥¶À´¦Àí
	{
		/*0ºÅµãX×ø±ê´óÓÚ1ºÅµãX×ø±ê£¬Ôò½»»»Á½µãX×ø±ê*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*±éÀúX×ø±ê*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//ÒÀ´Î»­µã
		}
	}
	else if (x0 == x1)	//ÊúÏßµ¥¶À´¦Àí
	{
		/*0ºÅµãY×ø±ê´óÓÚ1ºÅµãY×ø±ê£¬Ôò½»»»Á½µãY×ø±ê*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*±éÀúY×ø±ê*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//ÒÀ´Î»­µã
		}
	}
	else				//Ğ±Ïß
	{
		/*Ê¹ÓÃBresenhamËã·¨»­Ö±Ïß£¬¿ÉÒÔ±ÜÃâºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
		/*²Î¿¼ÎÄµµ£ºhttps://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*²Î¿¼½Ì³Ì£ºhttps://www.bilibili.com/video/BV1364y1d7Lo*/
		
		if (x0 > x1)	//0ºÅµãX×ø±ê´óÓÚ1ºÅµãX×ø±ê
		{
			/*½»»»Á½µã×ø±ê*/
			/*½»»»ºó²»Ó°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»¡¢¶ş¡¢Èı¡¢ËÄÏóÏŞ±äÎªµÚÒ»¡¢ËÄÏóÏŞ*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0ºÅµãY×ø±ê´óÓÚ1ºÅµãY×ø±ê
		{
			/*½«Y×ø±êÈ¡¸º*/
			/*È¡¸ººóÓ°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»¡¢ËÄÏóÏŞ±äÎªµÚÒ»ÏóÏŞ*/
			y0 = -y0;
			y1 = -y1;
			
			/*ÖÃ±êÖ¾Î»yflag£¬¼Ç×¡µ±Ç°±ä»»£¬ÔÚºóĞøÊµ¼Ê»­ÏßÊ±£¬ÔÙ½«×ø±ê»»»ØÀ´*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//»­ÏßĞ±ÂÊ´óÓÚ1
		{
			/*½«X×ø±êÓëY×ø±ê»¥»»*/
			/*»¥»»ºóÓ°Ïì»­Ïß£¬µ«ÊÇ»­Ïß·½ÏòÓÉµÚÒ»ÏóÏŞ0~90¶È·¶Î§±äÎªµÚÒ»ÏóÏŞ0~45¶È·¶Î§*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*ÖÃ±êÖ¾Î»xyflag£¬¼Ç×¡µ±Ç°±ä»»£¬ÔÚºóĞøÊµ¼Ê»­ÏßÊ±£¬ÔÙ½«×ø±ê»»»ØÀ´*/
			xyflag = 1;
		}
		
		/*ÒÔÏÂÎªBresenhamËã·¨»­Ö±Ïß*/
		/*Ëã·¨ÒªÇó£¬»­Ïß·½Ïò±ØĞëÎªµÚÒ»ÏóÏŞ0~45¶È·¶Î§*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*»­ÆğÊ¼µã£¬Í¬Ê±ÅĞ¶Ï±êÖ¾Î»£¬½«×ø±ê»»»ØÀ´*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//±éÀúXÖáµÄÃ¿¸öµã
		{
			x ++;
			if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
			{
				d += incrE;
			}
			else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«±±·½
			{
				y ++;
				d += incrNE;
			}
			
			/*»­Ã¿Ò»¸öµã£¬Í¬Ê±ÅĞ¶Ï±êÖ¾Î»£¬½«×ø±ê»»»ØÀ´*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}

/**
  * º¯    Êı£ºOLED¾ØĞÎ
  * ²Î    Êı£ºX Ö¸¶¨¾ØĞÎ×óÉÏ½ÇµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨¾ØĞÎ×óÉÏ½ÇµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºWidth Ö¸¶¨¾ØĞÎµÄ¿í¶È£¬·¶Î§£º0~128
  * ²Î    Êı£ºHeight Ö¸¶¨¾ØĞÎµÄ¸ß¶È£¬·¶Î§£º0~64
  * ²Î    Êı£ºIsFilled Ö¸¶¨¾ØĞÎÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	uint8_t i, j;
	if (!IsFilled)		//Ö¸¶¨¾ØĞÎ²»Ìî³ä
	{
		/*±éÀúÉÏÏÂX×ø±ê£¬»­¾ØĞÎÉÏÏÂÁ½ÌõÏß*/
		for (i = X; i < X + Width; i ++)
		{
			OLED_DrawPoint(i, Y);
			OLED_DrawPoint(i, Y + Height - 1);
		}
		/*±éÀú×óÓÒY×ø±ê£¬»­¾ØĞÎ×óÓÒÁ½ÌõÏß*/
		for (i = Y; i < Y + Height; i ++)
		{
			OLED_DrawPoint(X, i);
			OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//Ö¸¶¨¾ØĞÎÌî³ä
	{
		/*±éÀúX×ø±ê*/
		for (i = X; i < X + Width; i ++)
		{
			/*±éÀúY×ø±ê*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³äÂú¾ØĞÎ*/
				OLED_DrawPoint(i, j);
			}
		}
	}
}

/**
  * º¯    Êı£ºOLEDÈı½ÇĞÎ
  * ²Î    Êı£ºX0 Ö¸¶¨µÚÒ»¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY0 Ö¸¶¨µÚÒ»¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX1 Ö¸¶¨µÚ¶ş¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY1 Ö¸¶¨µÚ¶ş¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºX2 Ö¸¶¨µÚÈı¸ö¶ËµãµÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY2 Ö¸¶¨µÚÈı¸ö¶ËµãµÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºIsFilled Ö¸¶¨Èı½ÇĞÎÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled)
{
	uint8_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	uint8_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};
	
	if (!IsFilled)			//Ö¸¶¨Èı½ÇĞÎ²»Ìî³ä
	{
		/*µ÷ÓÃ»­Ïßº¯Êı£¬½«Èı¸öµãÓÃÖ±ÏßÁ¬½Ó*/
		OLED_DrawLine(X0, Y0, X1, Y1);
		OLED_DrawLine(X0, Y0, X2, Y2);
		OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//Ö¸¶¨Èı½ÇĞÎÌî³ä
	{
		/*ÕÒµ½Èı¸öµã×îĞ¡µÄX¡¢Y×ø±ê*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}
		
		/*ÕÒµ½Èı¸öµã×î´óµÄX¡¢Y×ø±ê*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}
		
		/*×îĞ¡×î´ó×ø±êÖ®¼äµÄ¾ØĞÎÎª¿ÉÄÜĞèÒªÌî³äµÄÇøÓò*/
		/*±éÀú´ËÇøÓòÖĞËùÓĞµÄµã*/
		/*±éÀúX×ø±ê*/		
		for (i = minx; i <= maxx; i ++)
		{
			/*±éÀúY×ø±ê*/	
			for (j = miny; j <= maxy; j ++)
			{
				/*µ÷ÓÃOLED_pnpoly£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨Èı½ÇĞÎÖ®ÖĞ*/
				/*Èç¹ûÔÚ£¬Ôò»­µã£¬Èç¹û²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
  * º¯    Êı£ºOLED»­Ô²
  * ²Î    Êı£ºX Ö¸¶¨Ô²µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Ô²µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºRadius Ö¸¶¨Ô²µÄ°ë¾¶£¬·¶Î§£º0~255
  * ²Î    Êı£ºIsFilled Ö¸¶¨Ô²ÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Ö  µ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*Ê¹ÓÃBresenhamËã·¨»­Ô²£¬¿ÉÒÔ±ÜÃâºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
	/*²Î¿¼ÎÄµµ£ºhttps://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
	/*²Î¿¼½Ì³Ì£ºhttps://www.bilibili.com/video/BV1VM4y1u7wJ*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*»­Ã¿¸ö°Ë·ÖÖ®Ò»Ô²»¡µÄÆğÊ¼µã*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X + y, Y + x);
	OLED_DrawPoint(X - y, Y - x);
	
	if (IsFilled)		//Ö¸¶¨Ô²Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	while (x < y)		//±éÀúXÖáµÄÃ¿¸öµã
	{
		x ++;
		if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d += 2 * x + 1;
		}
		else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*»­Ã¿¸ö°Ë·ÖÖ®Ò»Ô²»¡µÄµã*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X + y, Y + x);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - y, Y - x);
		OLED_DrawPoint(X + x, Y - y);
		OLED_DrawPoint(X + y, Y - x);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X - y, Y + x);
		
		if (IsFilled)	//Ö¸¶¨Ô²Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
			
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -x; j < x; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÔ²*/
				OLED_DrawPoint(X - y, Y + j);
				OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}

/**
  * º¯    Êı£ºOLED»­ÍÖÔ²
  * ²Î    Êı£ºX Ö¸¶¨ÍÖÔ²µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨ÍÖÔ²µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºA Ö¸¶¨ÍÖÔ²µÄºáÏò°ëÖá³¤¶È£¬·¶Î§£º0~255
  * ²Î    Êı£ºB Ö¸¶¨ÍÖÔ²µÄ×İÏò°ëÖá³¤¶È£¬·¶Î§£º0~255
  * ²Î    Êı£ºIsFilled Ö¸¶¨ÍÖÔ²ÊÇ·ñÌî³ä
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;
	
	/*Ê¹ÓÃBresenhamËã·¨»­ÍÖÔ²£¬¿ÉÒÔ±ÜÃâ²¿·ÖºÄÊ±µÄ¸¡µãÔËËã£¬Ğ§ÂÊ¸ü¸ß*/
	/*²Î¿¼Á´½Ó£ºhttps://blog.csdn.net/myf_666/article/details/128167392*/
	
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);
	
	if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
			OLED_DrawPoint(X, Y + j);
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	/*»­ÍÖÔ²»¡µÄÆğÊ¼µã*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X - x, Y + y);
	OLED_DrawPoint(X + x, Y - y);
	
	/*»­ÍÖÔ²ÖĞ¼ä²¿·Ö*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;
		
		if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*»­ÍÖÔ²ÖĞ¼ä²¿·ÖÔ²»¡*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
	
	/*»­ÍÖÔ²Á½²à²¿·Ö*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	
	while (y > 0)
	{
		if (d2 <= 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;
			
		}
		else				//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;
		
		if (IsFilled)	//Ö¸¶¨ÍÖÔ²Ìî³ä
		{
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÖ¸¶¨ÇøÓò»­µã£¬Ìî³ä²¿·ÖÍÖÔ²*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*»­ÍÖÔ²Á½²à²¿·ÖÔ²»¡*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
}

/**
  * º¯    Êı£ºOLED»­Ô²»¡
  * ²Î    Êı£ºX Ö¸¶¨Ô²»¡µÄÔ²ĞÄºá×ø±ê£¬·¶Î§£º0~127
  * ²Î    Êı£ºY Ö¸¶¨Ô²»¡µÄÔ²ĞÄ×İ×ø±ê£¬·¶Î§£º0~63
  * ²Î    Êı£ºRadius Ö¸¶¨Ô²»¡µÄ°ë¾¶£¬·¶Î§£º0~255
  * ²Î    Êı£ºStartAngle Ö¸¶¨Ô²»¡µÄÆğÊ¼½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ²Î    Êı£ºEndAngle Ö¸¶¨Ô²»¡µÄÖÕÖ¹½Ç¶È£¬·¶Î§£º-180~180
  *           Ë®Æ½ÏòÓÒÎª0¶È£¬Ë®Æ½Ïò×óÎª180¶È»ò-180¶È£¬ÏÂ·½ÎªÕıÊı£¬ÉÏ·½Îª¸ºÊı£¬Ë³Ê±ÕëĞı×ª
  * ²Î    Êı£ºIsFilled Ö¸¶¨Ô²»¡ÊÇ·ñÌî³ä£¬Ìî³äºóÎªÉÈĞÎ
  *           ·¶Î§£ºOLED_UNFILLED		²»Ìî³ä
  *                 OLED_FILLED			Ìî³ä
  * ·µ »Ø Öµ£ºÎŞ
  * Ëµ    Ã÷£ºµ÷ÓÃ´Ëº¯Êıºó£¬ÒªÏëÕæÕıµØ³ÊÏÖÔÚÆÁÄ»ÉÏ£¬»¹Ğèµ÷ÓÃ¸üĞÂº¯Êı
  */
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*´Ëº¯Êı½èÓÃBresenhamËã·¨»­Ô²µÄ·½·¨*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*ÔÚ»­Ô²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
	if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
	if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
	if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
	if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
	
	if (IsFilled)	//Ö¸¶¨Ô²»¡Ìî³ä
	{
		/*±éÀúÆğÊ¼µãY×ø±ê*/
		for (j = -y; j < y; j ++)
		{
			/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
			if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
		}
	}
	
	while (x < y)		//±éÀúXÖáµÄÃ¿¸öµã
	{
		x ++;
		if (d < 0)		//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«·½
		{
			d += 2 * x + 1;
		}
		else			//ÏÂÒ»¸öµãÔÚµ±Ç°µã¶«ÄÏ·½
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*ÔÚ»­Ô²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
		if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
		if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
		if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
		if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
		if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
		if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
		if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
		if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}
		
		if (IsFilled)	//Ö¸¶¨Ô²»¡Ìî³ä
		{
			/*±éÀúÖĞ¼ä²¿·Ö*/
			for (j = -y; j < y; j ++)
			{
				/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
				if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
			}
			
			/*±éÀúÁ½²à²¿·Ö*/
			for (j = -x; j < x; j ++)
			{
				/*ÔÚÌî³äÔ²µÄÃ¿¸öµãÊ±£¬ÅĞ¶ÏÖ¸¶¨µãÊÇ·ñÔÚÖ¸¶¨½Ç¶ÈÄÚ£¬ÔÚ£¬Ôò»­µã£¬²»ÔÚ£¬Ôò²»×ö´¦Àí*/
				if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
				if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}

/*********************¹¦ÄÜº¯Êı*/


/*****************½­Ğ­¿Æ¼¼|°æÈ¨ËùÓĞ****************/
/*****************jiangxiekeji.com*****************/

#endif 
