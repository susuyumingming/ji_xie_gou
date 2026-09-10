#include "sys.h"
#include "controls.h"
#include "command_queue.h"
#include "FreeRTOS.h"
#include "task.h"
#include "test_16_motors.h"

// 测试16个电机的控制
void Test_16_Motors(void)
{
    uint8_t addrs[16] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1};
    uint8_t dirs[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float velocities[16] = {100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f};
    float positions[16] = {36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f, 36000.0f};
    float pos_results[16] = {0.0f};
    
    uint32_t start_time, end_time, total_time;
    
    // 测试批量位置控制
    printf("测试16个电机批量控制...\r\n");
    start_time = xTaskGetTickCount();
    
    // 使用批量控制函数
    bool success = ZDT_Traj_Position_Control_Batch(
        addrs, dirs, 6000, 6000, velocities, positions, 1, 1, 16
    );
    bool success_2 = ZDT_Synchronous_motion(0);
	
    end_time = xTaskGetTickCount();
    total_time = end_time - start_time;
    
    printf("批量控制%s, 时间: %lu ms\r\n", success ? "成功" : "失败", total_time);
    
    // 不需要等待运动完成，直接开始读取位置
    
    // 循环读取位置
    printf("开始循环读取电机位置...\r\n");
    for (uint8_t i = 0; i < 5; i++) // 读取5次位置
    {
        start_time = xTaskGetTickCount();
        
        success = ZDT_ReadPos_Batch(addrs, pos_results, 16);
        
        end_time = xTaskGetTickCount();
        total_time = end_time - start_time;
        
        printf("第%d次读取%s, 时间: %lu ms\r\n", i+1, success ? "成功" : "失败", total_time);
        
        // 打印读取结果
        printf("电机位置:\r\n");
        for (uint8_t j = 0; j < 16; j++)
        {
            printf("电机 %d: %.2f 度\r\n", addrs[j], pos_results[j]);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500)); // 每500ms读取一次
    }
    
    // 测试命令队列
    printf("测试命令队列...\r\n");
    start_time = xTaskGetTickCount();
    
    MotorCmd_t cmds[16];
    for (uint8_t i = 0; i < 16; i++)
    {
        cmds[i].type = CMD_TYPE_POSITION_TRAJ;
        cmds[i].addr = addrs[i];
        cmds[i].params.position_traj.dir = dirs[i];
        cmds[i].params.position_traj.acc = 6000;
        cmds[i].params.position_traj.dec = 6000;
        cmds[i].params.position_traj.velocity = velocities[i];
        cmds[i].params.position_traj.position = positions[i];
        cmds[i].params.position_traj.raf = 1;
        cmds[i].params.position_traj.snF = 1;
    }
    
    // 发送批量命令到队列
    success = CommandQueue_SendBatch(cmds, 16);
    
    // 处理命令队列
    CommandQueue_ProcessBatch(16);
    
    end_time = xTaskGetTickCount();
    total_time = end_time - start_time;
    
    printf("命令队列%s, 时间: %lu ms\r\n", success ? "成功" : "失败", total_time);
    
    printf("16个电机测试完成!\r\n");
}

// 测试函数任务
void Test_16_Motors_Task(void *pvParameters)
{
    uint32_t cycle_count = 0;
    while (1)
    {
        cycle_count++;
        printf("\r\n=== 第 %lu 轮测试 ===\r\n", cycle_count);
        Test_16_Motors();
        vTaskDelay(pdMS_TO_TICKS(2000)); // 每2秒运行一次
    }
}

