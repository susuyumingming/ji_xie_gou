/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "fatfs.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//a
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  SystemClock_Config();
  PeriphCommonClock_Config();

  
  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_TIM8_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  MX_TIM1_Init();//初始化TIM1
  delay_init();//初始化延时函数
  MX_GPIO_Init();//初始化GPIO
  MX_SPI1_Init();//初始化SPI1
  MX_SPI2_Init();//初始化SPI2
	MX_SPI3_Init();//初始化SPI3
	MX_USART1_UART_Init();//初始化USART1
  MX_USART2_UART_Init();//初始化USART2
	MX_USART3_UART_Init();//初始化USART3
  MX_USART6_UART_Init();//初始化USART6
  MX_UART7_Init();//初始化UART7
  MX_UART8_Init();//初始化UART8
  // TX DMA
  // LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0); // 开启传输完成中断
  // // RX DMA  
  // LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2); // 开启传输完成中断
  OLED_Init();

  LCD_Init();
  // LCD_ShowChar(0,0,BLUE,YELLOW,'a',32,1);
	// LCD_ShowChar(0,32,BLUE,YELLOW,'A',32,1);
	// LCD_ShowChar(0,64,BLUE,YELLOW,'1',32,1);
  // while(1);
	TP_Init();
  // while(1)
  // {

  // }
  lv_init();                           	  // LVGL 初始化
	lv_port_disp_init();                  	// 注册LVGL的显示任务
  lv_port_indev_init();                  // 注册LVGL的触屏检测任务
  debug_uart_board_init();
  HWT101_board_init();
	printf("Zigbee_board_init success\r\n");
  JY61P_board_init();
	printf("Zigbee_board_init success\r\n");
	Zigbee_board_init();
	printf("Zigbee_board_init success\r\n");
	VL53_board_init_6();
	printf("Zigbee_board_init success\r\n");
  VL53_board_init_7();
	printf("Zigbee_board_init success\r\n");
  VL53_board_init_8();
	printf("Zigbee_board_init success\r\n");
  AS5600_Init();
  AX_PS2_Init();
  Buzzer_Init();
  printf("Zigbee_board_init success\r\n");
  TF_RES_Init();
  TF_SD_Test();
  // TF_SD_MultiSector_Test();
  // while(1)
  // {
  //   LL_TIM_OC_SetCompareCH2(TIM1, 5000);
  //   i++;
  //   if(i>20000)
  //   {
  //     i=0;
  //   }
  //   delay_ms(10);
  // }
  // static uint8_t dummy_rx_buffer[20]={0};  // 虚拟接收缓冲区
  // static uint8_t line_buffer[20]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,
  //                                     0xBB,0xCC,0xDD,0xEE,0xFF,0x12,0x34,0x56,0x78,0x9A};  // 足够存储一行
  // u8 adta=0;
  // extern SPI_HandleTypeDef hspi1;
  // while(1)
  // {
  //     // adta=SPI_WriteByte(SPI1, 0x55);
  //     // printf("adta=0x%02X\r\n", adta);
  //   memset(dummy_rx_buffer, 0, 20);

  //   // ? HAL 库 SPI + DMA 全双工收发（只需要这一行！）
  //   if(HAL_SPI_TransmitReceive_DMA(&hspi1, line_buffer, dummy_rx_buffer, 20) == HAL_OK)
  //   {
  //       // 等待传输完成
  //       while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
  //   }
  //   printf("RX DATA: ");
  //   for(int i=0; i<20; i++)
  //   {
  //       printf("0x%02X ", dummy_rx_buffer[i]);
  //   }
  //   printf("\r\n");
  //       // LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
  //       // printf("dma stream 0 disable\r\n");
  //       // LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
  //       // printf("dma stream 2 disable\r\n");

  //       // // 清 DMA 标志
  //       // LL_DMA_ClearFlag_TC0(DMA2);
  //       // printf("dma stream 0 tc flag clear\r\n");
  //       // LL_DMA_ClearFlag_TC2(DMA2);
  //       // printf("dma stream 2 tc flag clear\r\n");
  //       // LL_DMA_ClearFlag_TE0(DMA2);
  //       // printf("dma stream 0 te flag clear\r\n");
  //       // LL_DMA_ClearFlag_TE2(DMA2);
  //       // printf("dma stream 2 te flag clear\r\n");
        
  //       // // 【H7必加】清空SPI RX FIFO残留
  //       // while(LL_SPI_IsActiveFlag_RXP(SPI1)) {
  //       //     (void)LL_SPI_ReceiveData8(SPI1);
  //       // }
  //       // // (void)LL_SPI_ReceiveData8(SPI1); // 强制触发一次接收，确保接收完成中断被清除
  //       // // 清空SPI状态
  //       // LL_SPI_ClearFlag_EOT(SPI1);
  //       // printf("spi 1 eot flag clear\r\n");
  //       // LL_SPI_ClearFlag_OVR(SPI1);
  //       // printf("spi 1 ovr flag clear\r\n");
  //       // LL_SPI_ClearFlag_TXTF(SPI1);
  //       // printf("spi 1 txtf flag clear\r\n");

  //       // // ===================== 4. 配置 DMA =====================
  //       // LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, 20);
  //       // printf("dma stream 0 data length set to 20\r\n");
  //       // LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_0, (uint32_t)line_buffer);
  //       // printf("dma stream 0 memory address set to 0x%08X\r\n", (uint32_t)line_buffer);

  //       // LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, 20);
  //       // printf("dma stream 2 data length set to 20\r\n");
  //       // LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_2, (uint32_t)dummy_rx_buffer);
  //       // printf("dma stream 2 memory address set to 0x%08X\r\n", (uint32_t)dummy_rx_buffer);

  //       // // ===================== 5. 启动 SPI + DMA 双工 =====================
  //       // LL_SPI_EnableDMAReq_RX(SPI1);
  //       // printf("spi 1 dma req enable rx\r\n");
  //       // LL_SPI_EnableDMAReq_TX(SPI1);
  //       // printf("spi 1 dma req enable tx\r\n");


  //       // LL_SPI_SetTransferSize(SPI1, 20);
  //       // printf("spi 1 transfer size set to 20\r\n");
  //       // LL_SPI_Enable(SPI1);
  //       // printf("spi 1 enable\r\n");

  //       // // 先开 RX
  //       // LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
  //       // printf("dma stream 2 enable\r\n");
  //       // // 再开 TX
  //       // LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
  //       // printf("dma stream 0 enable\r\n");
        
  //       // // 启动传输
  //       // LL_SPI_StartMasterTransfer(SPI1);
  //       // printf("spi 1 start master transfer\r\n");

  //       // // ===================== 6. 阻塞等完成（核心！） =====================
  //       // while(LL_DMA_IsActiveFlag_TC0(DMA2) == 0);
  //       // printf("dma stream 0 tc flag set\r\n");
  //       // while(LL_DMA_IsActiveFlag_TC2(DMA2) == 0);
  //       // printf("dma stream 2 tc flag set\r\n");

  //       // // 等待 SPI 真正结束（H7 必须用 EOT！）
  //       // while(LL_SPI_IsActiveFlag_EOT(SPI1) == 0);
  //       // printf("spi 1 eot flag set\r\n");

  //       // // 最后清空RX FIFO
  //       // while(LL_SPI_IsActiveFlag_RXP(SPI1)) {
  //       //     (void)LL_SPI_ReceiveData8(SPI1);
  //       // }
        
  //       // // ===================== 7. 关闭，恢复干净状态 =====================
  //       // // LL_SPI_DisableDMAReq_TX(SPI1);
  //       // // printf("spi 1 dma req disable tx\r\n");
  //       // // LL_SPI_DisableDMAReq_RX(SPI1);
  //       // // printf("spi 1 dma req disable rx\r\n");
  //       // LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
  //       // printf("dma stream 0 disable\r\n");
  //       // LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
  //       // printf("dma stream 2 disable\r\n");
  //       // // LL_SPI_Disable(SPI1);
  //       // // printf("spi 1 disable\r\n");
  //       // printf("data:");
  //       // for(int i=0;i<20;i++)
  //       // {
  //       //   printf("0x%02X ", dummy_rx_buffer[i]);
  //       // }
  //       // printf("\r\n");
        
  // }
  // while(1)
  // {
  //   uint8_t rx_data;
	//   uint8_t Byte=0xA5;
  //   LL_SPI_SetTransferSize(SPI1, 1);
  //   LL_SPI_StartMasterTransfer(SPI1);
  //   while(LL_SPI_IsActiveFlag_TXP(SPI1) == 0);
	//   LL_SPI_TransmitData8(SPI1, Byte);
	//   while(LL_SPI_IsActiveFlag_RXP(SPI1) == 0);
	//   rx_data = LL_SPI_ReceiveData8(SPI1);
  //   while(LL_SPI_IsActiveFlag_EOT(SPI1) == 0);
  //   LL_SPI_ClearFlag_EOT(SPI1);
  //   LL_SPI_ClearFlag_TXTF(SPI1);
  //   printf("spi 1 recv:0x%02X\r\n", rx_data);
  //   Byte=0xA5;
  //   // LL_SPI_SetTransferSize(SPI2, 1);
  //   // LL_SPI_StartMasterTransfer(SPI2);
  //   // while(LL_SPI_IsActiveFlag_TXP(SPI2) == 0);
	//   // LL_SPI_TransmitData8(SPI2, Byte);
	//   // while(LL_SPI_IsActiveFlag_RXP(SPI2) == 0);
	//   // rx_data = LL_SPI_ReceiveData8(SPI2);
  //   // while(LL_SPI_IsActiveFlag_EOT(SPI2) == 0);
  //   // LL_SPI_ClearFlag_EOT(SPI2);
  //   // LL_SPI_ClearFlag_TXTF(SPI2);
  //   printf("spi 2 recv:0x%02X\r\n", rx_data);
  //   delay_ms(100);
  // }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(5);
  LL_RCC_PLL1_SetN(160);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(4);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {

  }
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);
  LL_SetSystemCoreClock(400000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  LL_RCC_PLL2Q_Enable();
  LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL2_SetM(5);
  LL_RCC_PLL2_SetN(100);
  LL_RCC_PLL2_SetP(5);
  LL_RCC_PLL2_SetQ(5);
  LL_RCC_PLL2_SetR(2);
  LL_RCC_PLL2_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL2_IsReady() != 1)
  {
  }

  LL_RCC_PLL3P_Enable();
  LL_RCC_PLL3R_Enable();
  LL_RCC_PLL3_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL3_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL3_SetM(5);
  LL_RCC_PLL3_SetN(100);
  LL_RCC_PLL3_SetP(5);
  LL_RCC_PLL3_SetQ(4);
  LL_RCC_PLL3_SetR(10);
  LL_RCC_PLL3_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL3_IsReady() != 1)
  {
  }

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
