#ifndef __COMMAND_QUEUE_H
#define __COMMAND_QUEUE_H

#include "sys.h"
#include "FreeRTOS.h"
#include "queue.h"

// 命令类型定义
typedef enum {
    CMD_TYPE_TORQUE,           // 力矩控制
    CMD_TYPE_VELOCITY,         // 速度控制
    CMD_TYPE_POSITION_LV,      // 直通限速位置控制
    CMD_TYPE_POSITION_TRAJ,    // 梯形曲线位置控制
    CMD_TYPE_READ_POS,         // 读取位置
    CMD_TYPE_SYNCHRONOUS       // 同步运动
} CmdType_t;

// 命令结构体
typedef struct {
    CmdType_t type;           // 命令类型
    uint8_t addr;             // 电机地址
    union {
        // 力矩控制参数
        struct {
            uint8_t sign;
            uint16_t t_ramp;
            uint16_t torque;
            uint8_t snF;
        } torque;
        
        // 速度控制参数
        struct {
            uint8_t dir;
            uint16_t v_ramp;
            float velocity;
            uint8_t snF;
        } velocity;
        
        // 直通限速位置控制参数
        struct {
            uint8_t dir;
            float velocity;
            float position;
            uint8_t raf;
            uint8_t snF;
        } position_lv;
        
        // 梯形曲线位置控制参数
        struct {
            uint8_t dir;
            uint16_t acc;
            uint16_t dec;
            float velocity;
            float position;
            uint8_t raf;
            uint8_t snF;
        } position_traj;
    } params;
} MotorCmd_t;

// 命令队列句柄
extern QueueHandle_t xMotorCmdQueue;

// 初始化命令队列
void CommandQueue_Init(void);

// 发送命令到队列
bool CommandQueue_Send(MotorCmd_t *cmd);

// 批量发送命令到队列
bool CommandQueue_SendBatch(MotorCmd_t *cmds, uint8_t count);

// 处理命令队列
void CommandQueue_Process(void);

// 批量处理命令
void CommandQueue_ProcessBatch(uint8_t max_count);

#endif

