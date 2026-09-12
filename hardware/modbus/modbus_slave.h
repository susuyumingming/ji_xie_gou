#ifndef __MODBUS_SLAVE_H
#define __MODBUS_SLAVE_H

#include "sys.h"

#define MODBUS_SLAVE_ADDR     1        // 从站地址
#define MODBUS_RX_BUF_LEN     256
#define MODBUS_TX_BUF_LEN     256

extern __IO bool modbus_frame_ready;

// 输入寄存器 04功能码：传感器数据(只读)
#define INPUT_REG_NUM         8
extern uint16_t input_reg[INPUT_REG_NUM];

// 保持寄存器 03/06：控制参数(读写)
#define HOLD_REG_NUM          8
extern uint16_t hold_reg[HOLD_REG_NUM];

//全局接收缓冲区
extern uint8_t modbus_rx_buf[MODBUS_RX_BUF_LEN];

void Modbus_ProcessFrame(uint8_t *pFrame, uint16_t len);
uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len);

#endif


