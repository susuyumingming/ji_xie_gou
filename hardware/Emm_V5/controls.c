#include "controls.h"



Motor Motor_Control[16];

void Motor_Control_Init(void)
{
	Motor_Control[0].pos_robot = 0.0f;
	Motor_Control[0].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[0].phase_current = 0;
	Motor_Control[0].total_current = 0;
	
	Motor_Control[1].pos_robot = 0.0f;
	Motor_Control[1].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[1].phase_current = 0;
	Motor_Control[1].total_current = 0;
	
	Motor_Control[2].pos_robot = 0.0f;
	Motor_Control[2].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[2].phase_current = 0;
	Motor_Control[2].total_current = 0;
	
	Motor_Control[3].pos_robot = 0.0f;
	Motor_Control[3].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[3].phase_current = 0;
	Motor_Control[3].total_current = 0;
	
	Motor_Control[4].pos_robot = 0.0f;
	Motor_Control[4].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[4].phase_current = 0;
	Motor_Control[4].total_current = 0;
		
	Motor_Control[5].pos_robot = 0.0f;
	Motor_Control[5].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[5].phase_current = 0;
	Motor_Control[5].total_current = 0;
		
	Motor_Control[6].pos_robot = 0.0f;
	Motor_Control[6].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[6].phase_current = 0;
	Motor_Control[6].total_current = 0;
		
	Motor_Control[7].pos_robot = 0.0f;
	Motor_Control[7].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[7].phase_current = 0;
	Motor_Control[7].total_current = 0;
		
	Motor_Control[8].pos_robot = 0.0f;
	Motor_Control[8].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[8].phase_current = 0;
	Motor_Control[8].total_current = 0;
		
	Motor_Control[9].pos_robot = 0.0f;
	Motor_Control[9].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[9].phase_current = 0;
	Motor_Control[9].total_current = 0;
		
	Motor_Control[10].pos_robot = 0.0f;
	Motor_Control[10].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[10].phase_current = 0;
	Motor_Control[10].total_current = 0;
		
	Motor_Control[11].pos_robot = 0.0f;
	Motor_Control[11].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[11].phase_current = 0;
	Motor_Control[11].total_current = 0;
		
	Motor_Control[12].pos_robot = 0.0f;
	Motor_Control[12].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[12].phase_current = 0;
	Motor_Control[12].total_current = 0;
		
	Motor_Control[13].pos_robot = 0.0f;
	Motor_Control[13].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[13].phase_current = 0;
	Motor_Control[13].total_current = 0;
		
	Motor_Control[14].pos_robot = 0.0f;
	Motor_Control[14].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[14].phase_current = 0;
	Motor_Control[14].total_current = 0;
		
	Motor_Control[15].pos_robot = 0.0f;
	Motor_Control[15].Motor_Cur_Pos_robot = 0.0f;
	Motor_Control[15].phase_current = 0;
	Motor_Control[15].total_current = 0;
}

void Read_clk(void)
{

}

void Read_phase_current(uint8_t addr,TaskHandle_t xTaskToNotify)//相电流
{
	uint8_t retry_count = 0;
 	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Read_Sys_Params(addr, S_CPHA);
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
		{
			if(Emm_V5_rxCmd[0] == addr && Emm_V5_rxCmd[1] == 0x27 && Emm_V5_rxCount == 5)
			{
				// 拼接成uint16_t类型
				Motor_Control[addr-1].phase_current = (uint16_t)(
													((uint16_t)Emm_V5_rxCmd[2] << 8)     |
													((uint16_t)Emm_V5_rxCmd[3] << 0)
												);		
				return;
			}
			else 
			{
				retry_count++;
				vTaskDelay(pdMS_TO_TICKS(1));
			}
		}
		else
		{
			 retry_count++;
		}
	}
}

void Read_total_current(uint8_t addr,TaskHandle_t xTaskToNotify)//总电流
{
	uint8_t retry_count = 0;
 	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Read_Sys_Params(addr, S_CBUS);
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
		{
			if(Emm_V5_rxCmd[0] == addr && Emm_V5_rxCmd[1] == 0x26 && Emm_V5_rxCount == 5)
			{
				// 拼接成uint16_t类型
				Motor_Control[addr-1].total_current = (uint16_t)(
													((uint16_t)Emm_V5_rxCmd[2] << 8)     |
													((uint16_t)Emm_V5_rxCmd[3] << 0)
												);		
				return;
			}
			else 
			{
				retry_count++;
				vTaskDelay(pdMS_TO_TICKS(1));
			}
		}
		else
		{
			 retry_count++;
		}
	}
}

void Readclk(uint8_t addr,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
 	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Read_Sys_Params(addr, S_CPOS);
//		 if(xSemaphoreTake(motor_task_handle, pdMS_TO_TICKS(5)) == pdTRUE)
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
		{
			if(Emm_V5_rxCmd[0] == addr && Emm_V5_rxCmd[1] == 0x36 && Emm_V5_rxCount == 8)
			{
				// 拼接成uint32_t类型
				Motor_Control[addr-1].pos_robot = (uint32_t)(
													((uint32_t)Emm_V5_rxCmd[3] << 24)    |
													((uint32_t)Emm_V5_rxCmd[4] << 16)    |
													((uint32_t)Emm_V5_rxCmd[5] << 8)     |
													((uint32_t)Emm_V5_rxCmd[6] << 0)
												);
				// 转换成角度
				Motor_Control[addr-1].Motor_Cur_Pos_robot = (float)Motor_Control[addr-1].pos_robot * 0.1f ;
				// 符号
				if(Emm_V5_rxCmd[2]==0) { Motor_Control[addr-1].Motor_Cur_Pos_robot = -Motor_Control[addr-1].Motor_Cur_Pos_robot; }			
				return;
			}
			else 
			{
				retry_count++;
				vTaskDelay(pdMS_TO_TICKS(1));
			}
		}
		else
		{
			 retry_count++;
		}
	}
}

bool ZDT_Torque_Control(uint8_t addr, uint8_t sign, uint16_t t_ramp, uint16_t torque, uint8_t snF,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
  	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Torque_Control(addr,sign,t_ramp,torque,snF);		// 等待通知（非阻塞等待）
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5)) == pdTRUE )
		{
			return true;
		}
		else
		{
			retry_count++;
			vTaskDelay(pdMS_TO_TICKS(1));  // 重试前延迟
		}
	}
	return false;
}
	
bool ZDT_Synchronous_motion(uint8_t addr,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
 	 const uint8_t max_retries = 3;
 	 motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Synchronous_motion(addr);
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5)) == pdTRUE )
        {
				return true;
		}
		else
		{
			retry_count++;
			vTaskDelay(pdMS_TO_TICKS(1));  // 重试前延迟
		}
	}
	return false;
}

bool ZDT_Velocity_Control(uint8_t addr, uint8_t dir, uint16_t v_ramp, float velocity, uint8_t snF,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Velocity_Control(addr,dir,v_ramp,velocity,snF);		// 等待通知（非阻塞等待）
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
		{
			return true;
		}
		else
		{
			retry_count++;
			vTaskDelay(pdMS_TO_TICKS(1));  // 重试前延迟
		}
	}
	return false;
}

bool ZDT_Bypass_Position_LV_Control(uint8_t addr, uint8_t dir, float velocity, float position, uint8_t raf, uint8_t snF,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
  	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Bypass_Position_LV_Control(addr,dir,velocity,position,raf,snF); // 直通限速位置模式控制
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
		{
			return true;
		}
		else
		{
			retry_count++;
			vTaskDelay(pdMS_TO_TICKS(1));  // 重试前延迟
		}
	}
	return false;
}

bool ZDT_Traj_Position_Control(uint8_t addr, uint8_t dir, uint16_t acc, uint16_t dec, float velocity, float position, uint8_t raf, uint8_t snF,TaskHandle_t xTaskToNotify)
{
	uint8_t retry_count = 0;
	const uint8_t max_retries = 3;
	motor_task_handle = xTaskToNotify;
	while(retry_count < max_retries) 
	{
		ZDT_X42_V2_Traj_Position_Control(addr,dir,acc,dec,velocity,position,raf,snF);
		if( xTaskNotifyWait(0, 0x01, NULL, pdMS_TO_TICKS(5))==pdTRUE)
  		  {
			return true;
		}
		else
		{
			retry_count++;
			vTaskDelay(pdMS_TO_TICKS(1));  // 重试前延迟
		}
	}
	return false;
}
