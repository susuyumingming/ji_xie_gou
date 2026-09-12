#include "modbus_slave.h"
#include "string.h"


// ========== 寄存器全局变量 ==========
// 【输入寄存器 04】传感器数据，MCU采集更新，主机只读
uint16_t input_reg[INPUT_REG_NUM] = {0};
// 【保持寄存器03/06】控制参数，主机可读可写
uint16_t hold_reg[HOLD_REG_NUM] = {0};

//全局接收缓冲区
uint8_t modbus_rx_buf[MODBUS_RX_BUF_LEN];

__IO bool modbus_frame_ready = false;

// CRC16 查表，Modbus标准多项式 0xA001
static const uint16_t crc_table[256] = {
0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040
};

uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len)
{
    uint8_t  i;
    uint16_t crc = 0xFFFF;
    while(len--)
    {
        i = crc ^ (*buf++);
        crc = (crc >> 8) ^ crc_table[i];
    }
    return crc;
}

// 发送应答帧，这里调用你的HAL串口发送函数，改成你的串口句柄
static void Modbus_SendResp(uint8_t *txbuf, uint16_t txlen)
{
    // 示例 USART1，替换成你实际的UART句柄
    Zigbee_Serial_SendArray(txbuf, txlen);
}

// 生成异常应答
static void Modbus_SendException(uint8_t slave_addr, uint8_t func_code, uint8_t err_code)
{
    uint8_t txbuf[8];
    uint16_t crc;
    txbuf[0] = slave_addr;
    txbuf[1] = func_code | 0x80; // 异常功能码 +0x80
    txbuf[2] = err_code;
    crc = Modbus_CRC16(txbuf,3);
    txbuf[3] = crc & 0xFF;
    txbuf[4] = (crc >> 8) & 0xFF;
    Modbus_SendResp(txbuf,5);
}

void Modbus_ProcessFrame(uint8_t *pFrame, uint16_t len)
{
    uint8_t slave_addr = pFrame[0];
    uint8_t func_code = pFrame[1];
    uint16_t crc_recv, crc_calc;
    uint16_t reg_addr, reg_cnt;
    uint16_t i;
    uint8_t txbuf[MODBUS_TX_BUF_LEN];
    uint16_t tx_idx = 0;
    uint16_t crc;

    // 最小报文长度校验：地址(1)+功能码(1)+CRC(2) =4字节
    if(len < 4) return;

    // CRC校验
    crc_recv = (pFrame[len-1] <<8) | pFrame[len-2];
    crc_calc = Modbus_CRC16(pFrame, len-2);
    if(crc_recv != crc_calc) return;

    // 从站地址匹配判断
    if(slave_addr != MODBUS_SLAVE_ADDR) return;

    switch(func_code)
    {
        // ========== 03 读保持寄存器 ==========
        case 0x03:
        {
            if(len != 8)
            {
                Modbus_SendException(slave_addr,func_code,0x02); //非法地址
                return;
            }
            reg_addr = (pFrame[2] <<8) | pFrame[3];
            reg_cnt  = (pFrame[4] <<8) | pFrame[5];
            // 地址越界判断
            if(reg_addr >= HOLD_REG_NUM || (reg_addr + reg_cnt) > HOLD_REG_NUM || reg_cnt ==0 || reg_cnt>125)
            {
                Modbus_SendException(slave_addr,func_code,0x02);
                return;
            }
            txbuf[tx_idx++] = slave_addr;
            txbuf[tx_idx++] = func_code;
            txbuf[tx_idx++] = reg_cnt *2;
            for(i=0; i<reg_cnt; i++)
            {
                uint16_t val = hold_reg[reg_addr + i];
                txbuf[tx_idx++] = (val >>8) & 0xFF;
                txbuf[tx_idx++] = val & 0xFF;
            }
            crc = Modbus_CRC16(txbuf, tx_idx);
            txbuf[tx_idx++] = crc &0xFF;
            txbuf[tx_idx++] = (crc>>8)&0xFF;
            Modbus_SendResp(txbuf, tx_idx);
        }
        break;

        // ========== 04 读输入寄存器【传感器只读】 ==========
        case 0x04:
        {
            if(len != 8)
            {
                Modbus_SendException(slave_addr,func_code,0x02);
                return;
            }
            reg_addr = (pFrame[2] <<8) | pFrame[3];
            reg_cnt  = (pFrame[4] <<8) | pFrame[5];
            if(reg_addr >= INPUT_REG_NUM || (reg_addr + reg_cnt) > INPUT_REG_NUM || reg_cnt ==0 || reg_cnt>125)
            {
                Modbus_SendException(slave_addr,func_code,0x02);
                return;
            }
            txbuf[tx_idx++] = slave_addr;
            txbuf[tx_idx++] = func_code;
            txbuf[tx_idx++] = reg_cnt *2;
            for(i=0; i<reg_cnt; i++)
            {
                uint16_t val = input_reg[reg_addr + i];
                txbuf[tx_idx++] = (val >>8) & 0xFF;
                txbuf[tx_idx++] = val & 0xFF;
            }
            crc = Modbus_CRC16(txbuf, tx_idx);
            txbuf[tx_idx++] = crc &0xFF;
            txbuf[tx_idx++] = (crc>>8)&0xFF;
            Modbus_SendResp(txbuf, tx_idx);
        }
        break;

        // ========== 06 写单个保持寄存器 ==========
        case 0x06:
        {
            if(len != 8)
            {
                Modbus_SendException(slave_addr,func_code,0x02);
                return;
            }
            reg_addr = (pFrame[2] <<8) | pFrame[3];
            uint16_t write_val = (pFrame[4] <<8) | pFrame[5];
            if(reg_addr >= HOLD_REG_NUM)
            {
                Modbus_SendException(slave_addr,func_code,0x02);
                return;
            }
            hold_reg[reg_addr] = write_val;
            //正常应答，原样返回请求帧
            for(i=0;i<len;i++) txbuf[i]=pFrame[i];
            Modbus_SendResp(txbuf, len);
        }
        break;

        // 不支持的功能码，返回异常01
        default:
            Modbus_SendException(slave_addr,func_code,0x01);
        break;
    }
}


