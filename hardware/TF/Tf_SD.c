#include "Tf_SD.h"

//发送1byte数据的同时接收1byte
uint8_t SPI0_Sendread(uint8_t data)
{
  uint8_t rx_data;
  HAL_SPI_TransmitReceive(&hspi3, &data, &rx_data, 1, 100);  
  return rx_data;
}


//命令数据格式   uint8_t *buff传入装一帧数据的数组地址   
// uint8_t CMD_value 指定命令的序号 比如CMD0就填0  不支持的命令将打印错误
// 返回填充好命令的数据地址
uint8_t * TF_CMD(uint8_t *buff,uint8_t CMD_value)
{
    switch(CMD_value)
    {
    case 0:
        *buff = 0x00 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x95;
        break; // The CMD res 0X01 (复位成功)
    case 1:
        *buff = 1 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0xff;
        break; // The CMD res 0X01 (复位成功)
    case 8:
        *buff = 8 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x01;
        *(buff + 4) = 0xAA;
        *(buff + 5) = 0x87;
        break;//  The CMD res 0X01 0x00 0x00 0x01 0xaa
    case 12:
        *buff = 12 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  停止命令 用来停止多块读
    case 17:
        *buff = 17 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  单块读  中间4字节参数为地址，这里只是初始化  返回0x00
    case 18:
        *buff = 18 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  多块读  中间4字节参数为地址，这里只是初始化  返回0x00
    case 24:
        *buff = 24 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  单块写  中间4字节参数为地址，这里只是初始化  返回0x00
    case 25:
        *buff = 25 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  多块写  中间4字节参数为地址，这里只是初始化  返回0x00
    case 55:
        *buff = 55 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  send ACMD41 之前必须发送该命令
    case 58:
        *buff = 58 | 0x40;
        *(buff + 1) = 0x00;
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  The CMD res 0X00 0xc0 0xff 0x080 0x00   电压范围 2.7-3.6V
    case 41:      
        *buff = 41 | 0x40;
        *(buff + 1) = 0x40;//高容量40  标准容量 00
        *(buff + 2) = 0x00;
        *(buff + 3) = 0x00;
        *(buff + 4) = 0x00;
        *(buff + 5) = 0x01;
        break;//  The CMD res 0x00 初始化完成  ACMD41发送之前需要发送 CMD55
    default:printf("CMD ERROE!\n");break;
    }
    return buff;
}
//TF卡发送时钟脉冲 同步上电 时钟脉冲个数  时钟脉冲期间CS和MOSI必须为高电平 1num = 8个时钟脉冲
void TF_Send_clocks(uint8_t num)
{
    uint8_t txbuff = 0xff;
    uint8_t rxbuff = 0;
    for(uint8_t i = 0;i < num;i++)
    {
        rxbuff=SPI0_Sendread(txbuff);
    }
}
//TF卡发送命令 并接收相对应的数据  如果匹配则表示命令成功 
// uint8_t CMDNum为命令序号  返回接收到的数据 第一个数据在低位
uint32_t TF_Send_CMD(uint8_t CMDNum)
{
    uint8_t Txbuff[6] = {0};
    uint32_t data = 0;
    
    TF_CMD(Txbuff,CMDNum);//命令填充
    SD_CS_CLR;//片选拉低开始建立通信
    for(uint8_t i = 0;i<6;i++)
    {
        SPI0_Sendread(Txbuff[i]);
    }
    SPI0_Sendread(0xFF);//等待一个8位数据后接收数据 MOSI必须高！！
    data |= SPI0_Sendread(0xFF) & 0xff;
    data |= (SPI0_Sendread(0xFF) & 0xff) << 8;
    data |= (SPI0_Sendread(0xFF) & 0xff) << 16;
    data |= (SPI0_Sendread(0xFF) & 0xff) << 24;
    SD_CS_SET;//CS拉高后给时钟脉冲
    TF_Send_clocks(2);
    return data;
}
//TF卡一帧数据为6byte   上电初始化复位TF卡  SPI的输入输出脚已经被外接上拉电阻 还有CS也被上拉
void TF_RES_Init(void)
{
    uint32_t data = 0;
    uint8_t num = 250;
    uint8_t res = 0;
    uint8_t count = 0;
L:
    delay_ms(500);  // 由300 → 500ms，给TF卡充足上电复位时间

    SD_CS_SET;   // CS必须拉高！！
    TF_Send_clocks(40); // 增加同步时钟，40*8=320个时钟

    data = TF_Send_CMD(0);//发送命令0
		printf("CMD0 raw data: 0x%08X\r\n", data);
    if((data & 0xff) == 0x01)
    {
        res = 1;
    }
    //复位成功 进入IDLE状态
    if(res)
    {
        data = TF_Send_CMD(8);//CMD 8 查询是否2.0SD卡还是MMC卡
        if((data & 0x0f) == 0x01)//SD V2.0
        {
            while(num--)
            {
                data = TF_Send_CMD(55);
                if((data & 0xff) == 0x01)//表示使用ACMD命令
                {
                    data = TF_Send_CMD(41);
                    if((data & 0xff) == 0x00)
                    {
                          printf("SD-SPI V2.0 Init OK  2.7~3.3V!\n");
                          TF_Send_clocks(50);//时钟脉冲
                        return;
                    }
                }
            }
            printf("SD V2.0 ERROR!\n");
        }
        else//SD V1.X/MMC V3
        {
             TF_Send_CMD(55);
             data = TF_Send_CMD(41);
             if((data & 0xff) <= 0x01)// 1 || 0 表示进入 SD V1.0
             {
                while(num--)
                {
                    data = TF_Send_CMD(55);
                    if((data & 0xff) == 0x01)//表示使用ACMD命令
                    {
                        data = TF_Send_CMD(41);
                        if((data & 0xff) == 0x00)
                        {
                            printf("SD-SPI V1.0 Init OK!\n");
                            return;
                        }
                    }
                }
                printf("SD V1.0 ERROR!\n");
            }
            else//MMC卡不支持 CMD55 + CMD41
            {
                 data = TF_Send_CMD(1);//CMD1
                 while(num--)
                 {
                    if(data == 0x00)
                    {
                        printf("MMC-SPI Init OK!\n");
                        return;
                    }
                    data = TF_Send_CMD(1);
                 }
                 printf("MMC ERROR!\n");
            }
         }
    }
    else
    {
        count++;
        if(count < 3)
        {
            goto L;
        }
        else
        {
            SD_CS_SET;
            TF_Send_clocks(2);
            printf("RES_ERROR\n");
        }
    }
}
 
//读取块数据  一块读 512byte  返回0读取成功
//uint32_t chunkaddr 要读取的块地址 1表示块1的地址   
//uint8_t *Rbuff 读取数据的缓冲区
uint8_t TF_Readdata(uint32_t chunkaddr,uint8_t *Rxbuff)
{
    uint8_t Tbuff[6] = {0};
    uint8_t data = 0xff;
    uint8_t num = 20;
    uint16_t delay = 500;//超时判断值
    
    chunkaddr *= 512;
    
    TF_CMD(Tbuff,17);//CMD17填充
    Tbuff[1] = chunkaddr >> 24;//地址参数填充
    Tbuff[2] = chunkaddr >> 16;
    Tbuff[3] = chunkaddr >> 8;
    Tbuff[4] = chunkaddr & 0xff;
    // 读取命令填充完毕
    SD_CS_CLR;//CS拉低开始通信
    for(uint8_t i = 0;i < 6;i++)
    {
        SPI0_Sendread(Tbuff[i]);
    }
    SPI0_Sendread(0xFF);//等待一个8位数据后接收数据（提速后需要思考这里）
    while(num--)
    {
        data = SPI0_Sendread(0xFF);
        if(data == 0)//CMD生效
        {
            while(delay)//开始读
            {
                 data = SPI0_Sendread(0x00);
                 if(data == 0xfe)//数据起始字节
                 {
                     for(uint16_t i = 0;i < 512;i++)
                     {
                         Rxbuff[i] = SPI0_Sendread(0x00);
                     }
                     SPI0_Sendread(0x00);//两个CRC直接忽略
                     SPI0_Sendread(0x00);//两个CRC直接忽略
                     break;
                 }
                 delay--;
            }
            SD_CS_SET;//CS拉高结束通信
            TF_Send_clocks(2);//时钟脉冲
            if(delay == 0) return 1;
            else return 0;
        }
    }
    SD_CS_SET;//CS拉高结束通信
    TF_Send_clocks(2);//时钟脉冲
    return 1;
}
//写入数据到块  一次写一块 512byte  返回0写入成功
//uint32_t chunkaddr 要写入的块地址 1表示块1的地址   
//uint8_t *Rbuff 写入数据的缓冲区 最少512
uint8_t TF_Writedata(uint32_t chunkaddr,uint8_t *Txbuff)
{
    uint8_t Tbuff[6] = {0};
    uint8_t data = 0xff;
    uint8_t num = 20;
    uint16_t delay = 500;//超时判断值
    chunkaddr *= 512;
    
    TF_CMD(Tbuff,24);//CMD24填充
    Tbuff[1] = chunkaddr >> 24;//地址参数填充
    Tbuff[2] = chunkaddr >> 16;
    Tbuff[3] = chunkaddr >> 8;
    Tbuff[4] = chunkaddr & 0xff;
    // 读取命令填充完毕
    SD_CS_CLR;//CS拉低开始通信
    for(uint8_t i = 0;i < 6;i++)
    {
        SPI0_Sendread(Tbuff[i]);
    }
    SPI0_Sendread(0xFF);//等待一个8位数据后接收数据（提速后需要思考这里）
    while(num--)
    {
        data = SPI0_Sendread(0xFF);
        if(data == 0)//CMD生效
        {
            TF_Send_clocks(2);//发送一点时钟脉冲
            SPI0_Sendread(0xFE);//开始字节
            for(uint16_t i = 0;i<512;i++)
            {
                SPI0_Sendread(Txbuff[i]);
            }
            SPI0_Sendread(0xff);//两个CRC
            SPI0_Sendread(0xff);//两个CRC
            while(delay)
            {
                data = SPI0_Sendread(0xff);//连续读
                if((data & 0x1f) == 0x05)//表示写操作成功
                {
                    while(delay)
                    {
                        data = SPI0_Sendread(0xff);//连续读
                        if(data == 0xff)//直到不忙碌
                        {
                            break;
                        }
                        delay--;
                    }
                    break;
                }
                delay--;
            }
            SD_CS_SET;//CS拉高结束通信
            TF_Send_clocks(2);//时钟脉冲
            if(delay == 0) return 1;
            else return 0;
        }
    }
    printf("CMD ERROR\n");
    SD_CS_SET;//CS拉高结束通信
    TF_Send_clocks(2);//时钟脉冲
    return 1;
}
//读取块数据  一块读 512byte  返回0读取成功
//uint32_t chunkaddr 要读取的块地址 1表示块1的地址   
//uint8_t *Rbuff 读取数据的缓冲区
//uint32_t chunklen 为要读取的块数量
uint8_t TF_ReadSector(uint32_t chunkaddr,uint8_t *Rxbuff,uint32_t chunklen)
{
    uint8_t res = 1;
    if(chunklen == 0) return 1;
    while(chunklen)
    {
       res = TF_Readdata(chunkaddr,Rxbuff);
       chunkaddr += 1;
       Rxbuff += 512;
       chunklen--;
    }
    
    return res;
}
//写入数据到块  一次写一块 512byte  返回0写入成功
//uint32_t chunkaddr 要写入的块地址 1表示块1的地址   
//uint8_t *Rbuff 写入数据的缓冲区 最少512
//chunklen 要写入块的数量
uint8_t TF_WriteSector(uint32_t chunkaddr,uint8_t *Txbuff,uint32_t chunklen)
{
    uint8_t res = 1;
    if(chunklen == 0) return 1;
    while(chunklen)
    {
       res = TF_Writedata(chunkaddr,Txbuff);
       chunkaddr += 1;
       Txbuff += 512;
       chunklen--;
    }
    return res;
}

/**
 * @brief SD卡综合测试函数，初始化完成后调用，一次性执行，不阻塞死循环
 * 功能：读取0扇区、写入测试扇区、回读校验、统计读写耗时
 * 注意：测试会占用【第10扇区】做读写测试，不要选0扇区(MBR)
 * 返回 0=全部测试通过，非0代表对应环节出错
 */
uint8_t TF_SD_Test(void)
{
    uint8_t w_buf[512], r_buf[512];
    uint32_t tick_start, tick_end;
    uint32_t i;
    uint8_t res;
    const uint32_t test_sector = 10;   // 测试使用扇区10，避开MBR和FAT表

    printf("\r\n===== SD Card Speed & ReadWrite Test Start =====\r\n");

    // ---------- 1. 读取测试扇区原始数据（备份） ----------
    tick_start = HAL_GetTick();
    res = TF_Readdata(test_sector, r_buf);
    tick_end = HAL_GetTick();
    if(res != 0)
    {
        printf("Read sector %d FAILED!\r\n", test_sector);
        return 1;
    }
    printf("Read sector %d OK, time: %d ms\r\n", test_sector, tick_end - tick_start);

    // ---------- 2. 填充测试数据 0~255循环 ----------
    for(i = 0; i < 512; i++)
    {
        w_buf[i] = i & 0xFF;
    }

    // ---------- 3. 写测试扇区，计时 ----------
    tick_start = HAL_GetTick();
    res = TF_Writedata(test_sector, w_buf);
    tick_end = HAL_GetTick();
    if(res != 0)
    {
        printf("Write sector %d FAILED!\r\n", test_sector);
        return 2;
    }
    printf("Write sector %d OK, time: %d ms\r\n", test_sector, tick_end - tick_start);

    // ---------- 4. 回读校验 ----------
    memset(r_buf, 0xFF, 512); //清空读缓存
    tick_start = HAL_GetTick();
    res = TF_Readdata(test_sector, r_buf);
    tick_end = HAL_GetTick();
    if(res != 0)
    {
        printf("Re-read sector %d FAILED!\r\n", test_sector);
        return 3;
    }
    printf("Re-read sector %d OK, time: %d ms\r\n", test_sector, tick_end - tick_start);

    // ---------- 5. 数据比对校验 ----------
    uint8_t data_ok = 1;
    for(i = 0; i < 512; i++)
    {
        if(r_buf[i] != (i & 0xFF))
        {
            printf("Data mismatch at idx:%d, w:0x%02X r:0x%02X\r\n",i, (uint8_t)(i&0xFF), r_buf[i]);
            data_ok = 0;
            break;
        }
    }
    if(!data_ok)
    {
        printf("Data Verify FAILED!\r\n");
        return 4;
    }
    printf("Data Verify PASS!\r\n");

    // ---------- 6. 恢复原来扇区数据（可选，防止破坏卡内容） ----------
    //TF_Writedata(test_sector, r_buf_back); //你可以把第一步读到的原始数据写回去

    printf("===== All SD Test SUCCESS! =====\r\n\r\n");
    return 0;
}
uint8_t TF_SD_MultiSector_Test(void)
{
    #define TEST_BLOCK_CNT 16
    uint8_t w_buf[TEST_BLOCK_CNT*512], r_buf[TEST_BLOCK_CNT*512];
    uint32_t tick_start,tick_end,i;
    uint8_t res;
    const uint32_t test_sector = 20;
    printf("\r\n===== Multi sector test start, total %d sectors =====\r\n",TEST_BLOCK_CNT);

    for(i=0;i<TEST_BLOCK_CNT*512;i++) w_buf[i]=i&0xff;

    tick_start=HAL_GetTick();
    res=TF_WriteSector(test_sector,w_buf,TEST_BLOCK_CNT);
    tick_end=HAL_GetTick();
    if(res) {printf("Multi write fail\r\n");return 1;}
    printf("Multi write %d sectors time:%d ms\r\n",TEST_BLOCK_CNT,tick_end-tick_start);

    memset(r_buf,0xff,sizeof(r_buf));
    tick_start=HAL_GetTick();
    res=TF_ReadSector(test_sector,r_buf,TEST_BLOCK_CNT);
    tick_end=HAL_GetTick();
    if(res) {printf("Multi read fail\r\n");return 2;}
    printf("Multi read %d sectors time:%d ms\r\n",TEST_BLOCK_CNT,tick_end-tick_start);

    uint8_t ok=1;
    for(i=0;i<TEST_BLOCK_CNT*512;i++)
    {
        if(r_buf[i]!=(i&0xff))
        {
            printf("Verify error idx:%d\r\n",i);
            ok=0;break;
        }
    }
    if(ok) printf("Multi sector verify PASS!\r\n");
    else return 3;
    printf("===== Multi sector test end =====\r\n");
    return 0;
}

