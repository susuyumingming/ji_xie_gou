/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持
#include "SPI.h"
#include "spi.h"
#include "touch.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task__ONE */
osThreadId_t Task__ONEHandle;
const osThreadAttr_t Task__ONE_attributes = {
  .name = "Task__ONE",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Task__TWO */
osThreadId_t Task__TWOHandle;
const osThreadAttr_t Task__TWO_attributes = {
  .name = "Task__TWO",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};
/* Definitions for Task__THREE */
osThreadId_t Task__THREEHandle;
const osThreadAttr_t Task__THREE_attributes = {
  .name = "Task__THREE",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for Task__FOUR */
osThreadId_t Task__FOURHandle;
const osThreadAttr_t Task__FOUR_attributes = {
  .name = "Task__FOUR",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for Task__FIVE */
osThreadId_t Task__FIVEHandle;
const osThreadAttr_t Task__FIVE_attributes = {
  .name = "Task__FIVE",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow5,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
TaskHandle_t  motor_task_handle = NULL;
/* USER CODE END FunctionPrototypes */

void Task__ONE_FUNCTION(void *argument);
void Task__TWO_FUNCTION(void *argument);
void Task__THREE_FUNCTION(void *argument);
void Task__FOUR_FUNCTION(void *argument);
void Task__FIVE_FUNCTION(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task__ONE */
  Task__ONEHandle = osThreadNew(Task__ONE_FUNCTION, NULL, &Task__ONE_attributes);

  /* creation of Task__TWO */
  Task__TWOHandle = osThreadNew(Task__TWO_FUNCTION, NULL, &Task__TWO_attributes);

  /* creation of Task__THREE */
  Task__THREEHandle = osThreadNew(Task__THREE_FUNCTION, NULL, &Task__THREE_attributes);

  /* creation of Task__FOUR */
  Task__FOURHandle = osThreadNew(Task__FOUR_FUNCTION, NULL, &Task__FOUR_attributes);

  /* creation of Task__FIVE */
  Task__FIVEHandle = osThreadNew(Task__FIVE_FUNCTION, NULL, &Task__FIVE_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Task__ONE_FUNCTION */
/**
  * @brief  Function implementing the Task__ONE thread.
  * @param  argument: Not used
  * @retval None
  */
int i=0;
int16_t filteredAngle[12];
int filteredAngle_amend[12];
float degrees[12];
uint16_t agcValue[12];
static uint16_t last_x = 0;
static uint16_t last_y = 0;
JOYSTICK_TypeDef my_joystick;
int key=0;
int Key_num=0;
/* USER CODE END Header_Task__ONE_FUNCTION */
void Task__ONE_FUNCTION(void *argument)
{
  /* USER CODE BEGIN Task__ONE_FUNCTION */
  /* Infinite loop */
  for(;;)
  {
	
    OLED_Clear();
		OLED_ShowNum_My(1,1,my_joystick.mode,3,OLED_6X8);
		OLED_ShowNum_My(1,5,my_joystick.btn1,3,OLED_6X8);
		OLED_ShowNum_My(1,9,my_joystick.btn2,3,OLED_6X8);
		OLED_ShowNum_My(1,13,my_joystick.RJoy_LR,3,OLED_6X8);

		OLED_ShowNum_My(2,1,my_joystick.RJoy_UD,3,OLED_6X8);
		OLED_ShowNum_My(2,5,my_joystick.LJoy_LR,3,OLED_6X8);
		OLED_ShowNum_My(2,9,my_joystick.LJoy_UD,3,OLED_6X8);

		OLED_ShowNum_My(7,1,i,6,OLED_6X8);
    OLED_ShowFloatNum_My(3,1,Yaw,3,2,OLED_6X8);
    OLED_ShowFloatNum_My(4,1,JY61P_Roll,3,2,OLED_6X8);
    OLED_ShowFloatNum_My(5,1,JY61P_Pitch,3,2,OLED_6X8);
    OLED_ShowFloatNum_My(6,1,JY61P_Yaw,3,2,OLED_6X8);
	OLED_ShowNum_My(8,1,Key_num,2,OLED_6X8);
	OLED_ShowNum_My(8,5,Zigbee_coordinate[0],2,OLED_6X8);
	OLED_ShowNum_My(8,9,Zigbee_coordinate[1],2,OLED_6X8);
	OLED_ShowNum_My(8,13,Zigbee_coordinate[2],2,OLED_6X8);
    
    // TP_Read_XY2(&last_x, &last_y);
	// OLED_ShowNum_My(2,7,last_x*0.17,5,OLED_6X8);
	// OLED_ShowNum_My(3,7,last_y*0.24,5,OLED_6X8);
    
    // AS5600_GetAGC(agcValue);
	// if(touch_is_pressed())
	// {  
	// 	OLED_ShowNum_My(1,1,1,3,OLED_6X8);
	// }
	// else
	// {
	// 	OLED_ShowNum_My(1,1,0,3,OLED_6X8);
	// }
	OLED_Update();
	// printf("Task__ONE_FUNCTION %d times\r\n",i);
    // printf("Zigbee_coordinate[0]:%d\r\n",Zigbee_coordinate[0]);
    // printf("JY61P_Roll:%f\r\n",JY61P_Roll);
    // printf("JY61P_Pitch:%f\r\n",JY61P_Pitch);
    // printf("JY61P_Yaw:%f\r\n",JY61P_Yaw);
    // printf("Yaw:%f\r\n",Yaw);
    /*
    printf("filteredAngle[0]:%d\r\n",filteredAngle[0]);
    printf("degrees[0]:%f\r\n",degrees[0]);
    printf("filteredAngle[1]:%d\r\n",filteredAngle[1]);
    printf("degrees[1]:%f\r\n",degrees[1]);

    printf("filteredAngle[2]:%d\r\n",filteredAngle[2]);
    printf("degrees[2]:%f\r\n",degrees[2]);
    printf("filteredAngle[3]:%d\r\n",filteredAngle[3]);
    printf("degrees[3]:%f\r\n",degrees[3]);

    printf("filteredAngle[4]:%d\r\n",filteredAngle[4]);
    printf("degrees[4]:%f\r\n",degrees[4]);
    printf("filteredAngle[5]:%d\r\n",filteredAngle[5]);
    printf("degrees[5]:%f\r\n",degrees[5]);

    printf("filteredAngle[6]:%d\r\n",filteredAngle[6]);
    printf("degrees[6]:%f\r\n",degrees[6]);
    printf("filteredAngle[7]:%d\r\n",filteredAngle[7]);
    printf("degrees[7]:%f\r\n",degrees[7]);

    printf("filteredAngle[8]:%d\r\n",filteredAngle[8]);
    printf("degrees[8]:%f\r\n",degrees[8]);
    printf("filteredAngle[9]:%d\r\n",filteredAngle[9]);
    printf("degrees[9]:%f\r\n",degrees[9]);

    printf("filteredAngle[10]:%d\r\n",filteredAngle[10]);
    printf("degrees[10]:%f\r\n",degrees[10]);
    printf("filteredAngle[11]:%d\r\n",filteredAngle[11]);
    printf("degrees[11]:%f\r\n",degrees[11]);
    */
    // printf("filteredAngle[0]:%d,filteredAngle[1]:%d,filteredAngle[2]:%d,filteredAngle[3]:%d,filteredAngle[4]:%d,filteredAngle[5]:%d,filteredAngle[6]:%d,filteredAngle[7]:%d,filteredAngle[8]:%d,filteredAngle[9]:%d,filteredAngle[10]:%d,filteredAngle[11]:%d \r\n",filteredAngle[0],filteredAngle[1],filteredAngle[2],filteredAngle[3],filteredAngle[4],filteredAngle[5],filteredAngle[6],filteredAngle[7],filteredAngle[8],filteredAngle[9],filteredAngle[10],filteredAngle[11]);
    // printf("degrees[0]:%d,degrees[1]:%d,degrees[2]:%d,degrees[3]:%d,degrees[4]:%d,degrees[5]:%d,degrees[6]:%d,degrees[7]:%d,degrees[8]:%d,degrees[9]:%d,degrees[10]:%d,degrees[11]:%d \r\n",degrees[0],degrees[1],degrees[2],degrees[3],degrees[4],degrees[5],degrees[6],degrees[7],degrees[8],degrees[9],degrees[10],degrees[11]);
    // printf("agcValue[0]:%d,agcValue[1]:%d,agcValue[2]:%d,agcValue[3]:%d,agcValue[4]:%d,agcValue[5]:%d,agcValue[6]:%d,agcValue[7]:%d,agcValue[8]:%d,agcValue[9]:%d,agcValue[10]:%d,agcValue[11]:%d \r\n",agcValue[0],agcValue[1],agcValue[2],agcValue[3],agcValue[4],agcValue[5],agcValue[6],agcValue[7],agcValue[8],agcValue[9],agcValue[10],agcValue[11]);
    // printf("Yaw_int:%d\r\n",Yaw_int);
    // printf("HWT101_rxCmd:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",HWT101_rxCmd[0],HWT101_rxCmd[1],HWT101_rxCmd[2],HWT101_rxCmd[3],HWT101_rxCmd[4]
    // ,HWT101_rxCmd[5],HWT101_rxCmd[6],HWT101_rxCmd[7],HWT101_rxCmd[8]);
    // printf("JY61P_rxCmd:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",JY61P_rxCmd[22],JY61P_rxCmd[23],JY61P_rxCmd[24],JY61P_rxCmd[25],JY61P_rxCmd[26]
    // ,JY61P_rxCmd[27],JY61P_rxCmd[28],JY61P_rxCmd[29],JY61P_rxCmd[30]);
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  /* USER CODE END Task__ONE_FUNCTION */
}

/* USER CODE BEGIN Header_Task__TWO_FUNCTION */
/**
* @brief Function implementing the Task__TWO thread.
* @param argument: Not used
* @retval None
*/
static const int multipliers[] = {1, 10, 100, 1000, 10000};
void display_signed_float(lv_obj_t* label, const char* name, float value, int decimals)
{
    if(decimals < 0) decimals = 0;
    if(decimals > 4) decimals = 4;  // 限制范围
    
    int is_negative = value < 0;
    float abs_value = is_negative ? -value : value;
    
    int multiplier = multipliers[decimals];
    int scaled_value = (int)(abs_value * multiplier);
    
    int integer = scaled_value / multiplier;
    int fractional = scaled_value % multiplier;
    
    if(is_negative) {
        lv_label_set_text_fmt(label, "-%d.%0*d", integer, decimals, fractional);
    } else {
        lv_label_set_text_fmt(label, "%d.%0*d", integer, decimals, fractional);
    }
}
// 按键点击回调（可选，串口打印信息，验证触摸触发）
static void test_btn_click_cb(lv_event_t *e)
{
		lv_event_code_t code = lv_event_get_code(e);
		if(code == LV_EVENT_PRESSED)
		{
			Buzzer_ON();
			delay_ms(25);
			Buzzer_OFF();
			// ZDT_Traj_Position_Control(1,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(2,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(3,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(4,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(5,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(6,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(7,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(8,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(9,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Traj_Position_Control(10,1,60,60,40,1000000,1,1,Task__TWO_Handle);
			// ZDT_Synchronous_motion(0,Task__TWO_Handle);
				// 触摸点击时，串口打印（需确保你的工程已初始化串口，如USART1）
//				printf("触摸测试按键被点击！\r\n");
				// 可选：按键文字闪烁反馈
//				lv_label_set_text(btn_label, "点击成功");
//				lv_timer_create([](lv_timer_t *t){
//						lv_label_set_text((lv_obj_t*)t->user_data, "触摸测试");
//						lv_timer_del(t);
//				}, 500, btn_label);
		}
}
/* USER CODE END Header_Task__TWO_FUNCTION */
void Task__TWO_FUNCTION(void *argument)
{
  /* USER CODE BEGIN Task__TWO_FUNCTION */
//  3. 获取屏幕对象（v8.3写法）
	lv_obj_t *scr = lv_scr_act();
	// 设置屏幕背景为白色（v8.3样式设置，建议补全不透明，白色更纯净）
	lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN); // 新增：背景不透明（必加，白色完全显示）
	lv_obj_invalidate(scr);

	// 6. 创建运行时间标签（左下角）
	lv_obj_t *time_label = lv_label_create(scr);
	lv_label_set_text(time_label, "run timer: 0s");
	lv_obj_align(time_label, LV_ALIGN_BOTTOM_LEFT, 10, -10);
	lv_obj_set_style_text_color(time_label, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_set_style_text_font(time_label, lv_font_default(), LV_PART_MAIN);

	// ========== 触摸测试按键 - 直接复制粘贴 ==========
	// 创建测试按键（右下角，不遮挡原有UI）
	lv_obj_t *test_btn = lv_btn_create(scr);
	lv_obj_set_size(test_btn, 60, 60);                // 按键尺寸：120*60
	lv_obj_align(test_btn, LV_ALIGN_BOTTOM_RIGHT, 0, -50); // 右下角偏移20px
	lv_obj_set_style_radius(test_btn, 8, LV_PART_MAIN); // 轻微圆角，不耗性能
	lv_obj_set_style_border_width(test_btn, 0, LV_PART_MAIN); // 无边框

	// 按键默认样式（浅蓝，适配白色背景）
	lv_obj_set_style_bg_color(test_btn, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(test_btn,	lv_color_hex(0x00FF00), LV_STATE_PRESSED);

	lv_obj_add_event_cb(test_btn, test_btn_click_cb, LV_EVENT_PRESSED, NULL);
	// ========== 触摸测试按键代码结束 ==========

	/*JY61P_Roll*/
	lv_obj_t *label_JY61P_Roll= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Roll,"JY61P_Roll:");
	lv_obj_set_pos(label_JY61P_Roll,0,0);
	lv_obj_set_style_text_font(label_JY61P_Roll, &lv_font_montserrat_20, LV_PART_MAIN);

	lv_obj_t *label_JY61P_Roll_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Roll_num,"%d",0);
	lv_obj_set_style_text_font(label_JY61P_Roll_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_JY61P_Roll_num, label_JY61P_Roll, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*HWT101Yaw角度*/
	lv_obj_t *label_Yaw= lv_label_create(scr);
	lv_label_set_text_fmt(label_Yaw,"Yaw:");
	lv_obj_set_style_text_font(label_Yaw, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Yaw, label_JY61P_Roll_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Yaw_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Yaw_num,"%d",0);
	lv_obj_set_style_text_font(label_Yaw_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Yaw_num, label_Yaw, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*JY61P_Pitch*/
	lv_obj_t *label_JY61P_Pitch= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Pitch,"JY61P_Pitch:");
	lv_obj_set_style_text_font(label_JY61P_Pitch, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_JY61P_Pitch, label_JY61P_Roll, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_JY61P_Pitch_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Pitch_num,"%d",0);
	lv_obj_set_style_text_font(label_JY61P_Pitch_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_JY61P_Pitch_num, label_JY61P_Pitch, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*JY61P_Yaw*/
	lv_obj_t *label_JY61P_Yaw= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Yaw,"JY61P_Yaw:");
	lv_obj_set_style_text_font(label_JY61P_Yaw, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_JY61P_Yaw, label_JY61P_Pitch, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_JY61P_Yaw_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_JY61P_Yaw_num,"%d",0);
	lv_obj_set_style_text_font(label_JY61P_Yaw_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_JY61P_Yaw_num, label_JY61P_Yaw, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*VL53_6*/
	lv_obj_t *label_VL53_6= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_6,"VL53_6:");
	lv_obj_set_style_text_font(label_VL53_6, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_6, label_JY61P_Yaw, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_VL53_6_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_6_num,"%d",0);
	lv_obj_set_style_text_font(label_VL53_6_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_6_num, label_VL53_6, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	// lv_obj_t *label_VL53_6_unit= lv_label_create(scr);
	// lv_label_set_text_fmt(label_VL53_6_unit,"mm");
	// lv_obj_set_style_text_font(label_VL53_6_unit, &lv_font_montserrat_20, LV_PART_MAIN);
	// lv_obj_align_to(label_VL53_6_unit, label_VL53_6_num, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*VL53_7*/
	lv_obj_t *label_VL53_7= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_7,"VL53_7:");
	lv_obj_set_style_text_font(label_VL53_7, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_7, label_VL53_6, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_VL53_7_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_7_num,"%d",0);
	lv_obj_set_style_text_font(label_VL53_7_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_7_num, label_VL53_7, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	// lv_obj_t *label_VL53_7_unit= lv_label_create(scr);
	// lv_label_set_text_fmt(label_VL53_7_unit,"mm");
	// lv_obj_set_style_text_font(label_VL53_7_unit, &lv_font_montserrat_20, LV_PART_MAIN);
	// lv_obj_align_to(label_VL53_7_unit, label_VL53_7_num, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*VL53_8*/
	lv_obj_t *label_VL53_8= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_8,"VL53_8:");
	lv_obj_set_style_text_font(label_VL53_8, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_8, label_VL53_7, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_VL53_8_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_VL53_8_num,"%d",0);
	lv_obj_set_style_text_font(label_VL53_8_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_VL53_8_num, label_VL53_8, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	// lv_obj_t *label_VL53_8_unit= lv_label_create(scr);
	// lv_label_set_text_fmt(label_VL53_8_unit,"mm");
	// lv_obj_set_style_text_font(label_VL53_8_unit, &lv_font_montserrat_20, LV_PART_MAIN);
	// lv_obj_align_to(label_VL53_8_unit, label_VL53_8_num, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	
	/*Motor_Cur_Pos_robot_1*/
	lv_obj_t *label_Motor_Cur_Pos_robot_1= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_1,"left_anterior_hip:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_1, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_1, label_VL53_8, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_1_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_1_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_1_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_1_num, label_Motor_Cur_Pos_robot_1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_1_total= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_1_total,"Total_1:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_1_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_1_total, label_Motor_Cur_Pos_robot_1_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_1_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_1_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_1_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_1_total_num, label_Motor_Cur_Pos_robot_1_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_2*/
	lv_obj_t *label_Motor_Cur_Pos_robot_2= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_2,"right_anterior_hip:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_2, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_2, label_Motor_Cur_Pos_robot_1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_2_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_2_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_2_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_2_num, label_Motor_Cur_Pos_robot_2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_2_total= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_2_total,"Total_2:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_2_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_2_total, label_Motor_Cur_Pos_robot_2_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_2_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_2_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_2_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_2_total_num, label_Motor_Cur_Pos_robot_2_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_3*/
	lv_obj_t *label_Motor_Cur_Pos_robot_3= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_3,"left_posterior_hip:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_3, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_3, label_Motor_Cur_Pos_robot_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_3_num= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_3_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_3_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_3_num, label_Motor_Cur_Pos_robot_3, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_3_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_3_total,"Total_3:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_3_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_3_total, label_Motor_Cur_Pos_robot_3_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_3_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_3_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_3_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_3_total_num, label_Motor_Cur_Pos_robot_3_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_4*/
	lv_obj_t *label_Motor_Cur_Pos_robot_4= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_4,"right_posterior_hip:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_4, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_4, label_Motor_Cur_Pos_robot_3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_4_num= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_4_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_4_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_4_num, label_Motor_Cur_Pos_robot_4, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_4_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_4_total,"Total_4:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_4_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_4_total, label_Motor_Cur_Pos_robot_4_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_4_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_4_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_4_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_4_total_num, label_Motor_Cur_Pos_robot_4_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_5*/
	lv_obj_t *label_Motor_Cur_Pos_robot_5= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_5,"left_anterior_thigh:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_5, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_5, label_Motor_Cur_Pos_robot_4, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_5_num= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_5_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_5_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_5_num, label_Motor_Cur_Pos_robot_5, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*	
	lv_obj_t *label_Motor_Cur_Pos_robot_5_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_5_total,"Total_5:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_5_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_5_total, label_Motor_Cur_Pos_robot_5_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_5_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_5_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_5_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_5_total_num, label_Motor_Cur_Pos_robot_5_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_6*/
	lv_obj_t *label_Motor_Cur_Pos_robot_6= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_6,"right_anterior_thigh:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_6, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_6, label_Motor_Cur_Pos_robot_5, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_6_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_6_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_6_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_6_num, label_Motor_Cur_Pos_robot_6, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_6_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_6_total,"Total_6:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_6_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_6_total, label_Motor_Cur_Pos_robot_6_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_6_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_6_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_6_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_6_total_num, label_Motor_Cur_Pos_robot_6_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_7*/
	lv_obj_t *label_Motor_Cur_Pos_robot_7= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_7,"left_posterior_thigh:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_7, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_7, label_Motor_Cur_Pos_robot_6, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_7_num= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_7_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_7_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_7_num, label_Motor_Cur_Pos_robot_7, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_7_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_7_total,"Total_7:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_7_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_7_total, label_Motor_Cur_Pos_robot_7_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_7_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_7_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_7_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_7_total_num, label_Motor_Cur_Pos_robot_7_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_8*/
	lv_obj_t *label_Motor_Cur_Pos_robot_8= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_8,"right_posterior_thigh:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_8, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_8, label_Motor_Cur_Pos_robot_7, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_8_num= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_8_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_8_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_8_num, label_Motor_Cur_Pos_robot_8, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_8_total= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_8_total,"Total_8:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_8_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_8_total, label_Motor_Cur_Pos_robot_8_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_8_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_8_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_8_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_8_total_num, label_Motor_Cur_Pos_robot_8_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_9*/	
	lv_obj_t *label_Motor_Cur_Pos_robot_9= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9,"left_anterior_lower_leg:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9, label_Motor_Cur_Pos_robot_8, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_9_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9_num, label_Motor_Cur_Pos_robot_9, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_9_total= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_total,"Total_9:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9_total, label_Motor_Cur_Pos_robot_9_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_9_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9_total_num, label_Motor_Cur_Pos_robot_9_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_10*/	
	lv_obj_t *label_Motor_Cur_Pos_robot_10= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_10,"right_anterior_lower_leg:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_10, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_10, label_Motor_Cur_Pos_robot_9, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_10_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_10_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_10_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_10_num, label_Motor_Cur_Pos_robot_10, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
/*
	lv_obj_t *label_Motor_Cur_Pos_robot_9_total= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_total,"Total_9:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9_total, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9_total, label_Motor_Cur_Pos_robot_9_num, LV_ALIGN_OUT_RIGHT_MID, 80, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_9_total_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_total_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_9_total_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_9_total_num, label_Motor_Cur_Pos_robot_9_total, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
*/
	/*Motor_Cur_Pos_robot_11*/	
	lv_obj_t *label_Motor_Cur_Pos_robot_11= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_11,"left_posterior_lower_leg:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_11, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_11, label_Motor_Cur_Pos_robot_10, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_11_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_11_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_11_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_11_num, label_Motor_Cur_Pos_robot_11, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

	/*Motor_Cur_Pos_robot_12*/	
	lv_obj_t *label_Motor_Cur_Pos_robot_12= lv_label_create(scr);
   lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_12,"right_posterior_lower_leg:");
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_12, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_12, label_Motor_Cur_Pos_robot_11, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_Motor_Cur_Pos_robot_12_num= lv_label_create(scr);
	lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_12_num,"%d",0);
	lv_obj_set_style_text_font(label_Motor_Cur_Pos_robot_12_num, &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_align_to(label_Motor_Cur_Pos_robot_12_num, label_Motor_Cur_Pos_robot_12, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
   // 8. 主循环（纯显示，无按键
  /* Infinite loop */
  for(;;)
  {
	
     // 处理LVGL刷新（v8.3核心函数）
	display_signed_float(label_Yaw_num, "", Yaw, 2);  // 显示：Yaw:-0.50
	display_signed_float(label_JY61P_Roll_num, "", JY61P_Roll, 2); // 显示：Yaw:1.26
	display_signed_float(label_JY61P_Pitch_num, "", JY61P_Pitch, 2);  // 显示：Yaw:-0.50
	display_signed_float(label_JY61P_Yaw_num, "", JY61P_Yaw, 2); // 显示：Yaw:1.26

	display_signed_float(label_Motor_Cur_Pos_robot_1_num, "", filteredAngle[degrees_left_anterior_hip], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_2_num, "", filteredAngle[degrees_right_anterior_hip], 2); // 显示：Yaw:1.26
	display_signed_float(label_Motor_Cur_Pos_robot_3_num, "", filteredAngle[degrees_left_posterior_hip], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_4_num, "", filteredAngle[degrees_right_posterior_hip], 2); // 显示：Yaw:1.26
	display_signed_float(label_Motor_Cur_Pos_robot_5_num, "", filteredAngle[degrees_left_anterior_thigh], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_6_num, "", filteredAngle[degrees_right_anterior_thigh], 2); // 显示：Yaw:1.26
	display_signed_float(label_Motor_Cur_Pos_robot_7_num, "", filteredAngle[degrees_left_posterior_thigh], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_8_num, "", filteredAngle[degrees_right_posterior_thigh], 2); // 显示：Yaw:1.26
	display_signed_float(label_Motor_Cur_Pos_robot_9_num, "", filteredAngle[degrees_left_anterior_lower_leg], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_10_num, "", filteredAngle[degrees_right_anterior_lower_leg], 2); // 显示：Yaw:1.26
	display_signed_float(label_Motor_Cur_Pos_robot_11_num, "", filteredAngle[degrees_left_posterior_lower_leg], 2);  // 显示：Yaw:-0.50
	display_signed_float(label_Motor_Cur_Pos_robot_12_num, "", filteredAngle[degrees_right_posterior_lower_leg], 2); // 显示：Yaw:1.26
	lv_label_set_text_fmt(label_VL53_6_num, "%d", (int32_t)VL53_Distance_6);
	lv_label_set_text_fmt(label_VL53_7_num, "%d", (int32_t)VL53_Distance_7);
	lv_label_set_text_fmt(label_VL53_8_num, "%d", (int32_t)VL53_Distance_8);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_4_num, "%d", (int32_t)Motor_Control[3].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_5_num, "%d", (int32_t)Motor_Control[4].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_6_num, "%d", (int32_t)Motor_Control[5].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_7_num, "%d", (int32_t)Motor_Control[6].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_8_num, "%d", (int32_t)Motor_Control[7].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_9_num, "%d", (int32_t)Motor_Control[8].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_10_num, "%d", (int32_t)Motor_Control[9].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_11_num, "%d", (int32_t)Motor_Control[10].Motor_Cur_Pos_robot);
		// lv_label_set_text_fmt(label_Motor_Cur_Pos_robot_12_num, "%d", (int32_t)Motor_Control[11].Motor_Cur_Pos_robot);
    // taskENTER_CRITICAL(); //  进入临界区

	
	lv_timer_handler();
	// taskEXIT_CRITICAL(); // 退出临界区
	// TickType_t tick = xTaskGetTickCount();
    
    // if(tick % 500 == 0)
    // {
    //     i++;
    // }
	i++;
	
    vTaskDelay(pdMS_TO_TICKS(1)); // 延时10ms，不占满CPU
  }
  /* USER CODE END Task__TWO_FUNCTION */
}

/* USER CODE BEGIN Header_Task__THREE_FUNCTION */
/**
* @brief Function implementing the Task__THREE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task__THREE_FUNCTION */
void Task__THREE_FUNCTION(void *argument)
{
  /* USER CODE BEGIN Task__THREE_FUNCTION */
  /* Infinite loop */
  for(;;)
  {
    vTaskDelay(pdMS_TO_TICKS(5000)); // 延时10ms，不占满CPU
  }
  /* USER CODE END Task__THREE_FUNCTION */
}

/* USER CODE BEGIN Header_Task__FOUR_FUNCTION */
/**
* @brief Function implementing the Task__FOUR thread.
* @param argument: Not used
* @retval None
*/
char buf[512];
UBaseType_t uxTaskGetStackHighWaterMark(TaskHandle_t xTask);
// 定义缓冲区存储统计结果
char pcRunTimeStatsBuffer[1024];
uint8_t buf_tx[51]={0x55,0x55,0};
/* USER CODE END Header_Task__FOUR_FUNCTION */
void Task__FOUR_FUNCTION(void *argument)
{
  /* USER CODE BEGIN Task__FOUR_FUNCTION */
  buf_tx[0]=0x55;
  buf_tx[1]=0x52;
  /* Infinite loop */
  for(;;)
  {
	motor_task_handle = Task__FOURHandle;
	// // 【发送前立刻打印】看看是不是对的
    // printf("发送前：%02X %02X %02X %02X\r\n", buf_tx[0],buf_tx[1],buf_tx[2],buf_tx[3]);
	// uint32_t start = xTaskGetTickCount();
    taskENTER_CRITICAL(); //  进入临界区
	MPUData_processing(&buf_tx[3],&buf_tx[2]);
	JY61_Data_processing(&buf_tx[5],&buf_tx[4],&buf_tx[7],&buf_tx[6],&buf_tx[9],&buf_tx[8]);
	VL53_data_processing_6(&buf_tx[11],&buf_tx[10]);
	VL53_data_processing_7(&buf_tx[13],&buf_tx[12]);
	VL53_data_processing_8(&buf_tx[15],&buf_tx[14]);
	AS5600_GetAngle(filteredAngle,buf_tx);
	Zigbee_processing();
	// for(int i=0;i<12;i++)
	// {
	// 	degrees[i] = (filteredAngle[i] * 360.0f) / 4096.0f;
	// }
	
	AX_PS2_ScanKey(&my_joystick);
	buf_tx[40]=my_joystick.mode;
	buf_tx[41]=my_joystick.btn1;
	buf_tx[42]=my_joystick.btn2;
	buf_tx[43]=my_joystick.RJoy_LR;
	buf_tx[44]=my_joystick.RJoy_UD;
	buf_tx[45]=my_joystick.LJoy_LR;
	buf_tx[46]=my_joystick.LJoy_UD;
	
	key=Key_GetNum();
	if(key)
	{
		Key_num=key;
	}
	buf_tx[47]=Key_num;
	buf_tx[48]=0xBB;
	buf_tx[49]=0xCC;
	if(Zigbee_coordinate[0]==1)
	{
		Zigbee_Serial_SendArray(buf_tx,50);
		Zigbee_coordinate[0]=0;
		Key_num=0;
	}
	
	// printf("%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n"
	// 	,Yaw,JY61P_Roll,JY61P_Pitch,JY61P_Yaw,
	// 	VL53_Distance_6,VL53_Distance_7,VL53_Distance_8,
	// 	degrees[0],degrees[1],degrees[2],
	// 	degrees[3],degrees[4],degrees[5],
	// 	degrees[6],degrees[7],degrees[8],
	// 	degrees[9],degrees[10],degrees[11]);
	// for(int i=0; i<48; i++)
	// {
	// 	printf("%02X ", buf_tx[i]);  // 每个字节以两位十六进制输出
	// }
	// printf("\r\n");  // 换行
	// printf("%02x", buf_tx[0]);
	// printf("%s\r\n", buf_tx);
	taskEXIT_CRITICAL(); // 退出临界区
	
	// for(int i=0; i<48; i++)
	// {
	// 	printf("%02X ", buf_tx[i]);  // 每个字节以两位十六进制输出
	// }
	// printf("\r\n");  // 换行
	// vTaskList(buf);// 打印任务列表（需开启configUSE_TRACE_FACILITY=1）
	// printf("===== 任务栈剩余 =====\r\n%s\r\n", buf);
	// memset(pcRunTimeStatsBuffer, 0, sizeof(pcRunTimeStatsBuffer));
	// // 获取任务运行时间统计
	// vTaskGetRunTimeStats(pcRunTimeStatsBuffer);
	// // 串口打印结果（替换成你的串口打印函数）
	// printf("任务运行时间统计：\r\n%s\r\n", pcRunTimeStatsBuffer);
	// uint32_t response_time = xTaskGetTickCount() - start;
	// printf("Response time : %lu ms\r\n",response_time);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  /* USER CODE END Task__FOUR_FUNCTION */
}

/* USER CODE BEGIN Header_Task__FIVE_FUNCTION */
/**
* @brief Function implementing the Task__FIVE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task__FIVE_FUNCTION */
void Task__FIVE_FUNCTION(void *argument)
{
  /* USER CODE BEGIN Task__FIVE_FUNCTION */
  /* Infinite loop */
  for(;;)
  {
	
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  /* USER CODE END Task__FIVE_FUNCTION */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

