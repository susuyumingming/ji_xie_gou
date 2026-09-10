#ifndef __TF_SD_H
#define __TF_SD_H

#include "sys.h"

#define	SD_CS_SET	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13)//片选端口		    
#define	SD_CS_CLR  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13)//片选端口  	

#include "Tf_SD.h"

uint8_t SPI0_Sendread(uint8_t data);
uint8_t * TF_CMD(uint8_t *buff,uint8_t CMD_value);
void TF_Send_clocks(uint8_t num);
uint32_t TF_Send_CMD(uint8_t CMDNum);
void TF_RES_Init(void);
uint8_t TF_Readdata(uint32_t chunkaddr,uint8_t *Rxbuff);
uint8_t TF_Writedata(uint32_t chunkaddr,uint8_t *Txbuff);
uint8_t TF_ReadSector(uint32_t chunkaddr,uint8_t *Rxbuff,uint32_t chunklen);
uint8_t TF_WriteSector(uint32_t chunkaddr,uint8_t *Txbuff,uint32_t chunklen);
uint8_t TF_SD_Test(void);
uint8_t TF_SD_MultiSector_Test(void);

#endif
