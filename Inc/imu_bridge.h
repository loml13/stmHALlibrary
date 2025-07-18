#ifndef IMU_BRIDGE_H
#define IMU_BRIDGE_H

#include "stm32f4xx_hal.h"   /* C 头文件只能放在 extern "C" 之外 */
#include "oled_imu.h"    /* 添加IMU显示相关头文件 */

#ifdef __cplusplus
extern "C" {
#endif

/* ── API ───────────────────────────────────────────────────────── */
void  IMU_Init(UART_HandleTypeDef *huart);
uint8_t IMU_Poll(void);                       /* 解析到新角度帧返回 1 */
void  IMU_GetAngle(float *roll, float *pitch, float *yaw);
void  IMU_UartIdleIsr(void);                  /* 串口 Idle ISR 转发 */
void  IMU_ProcessData(uint16_t size);         /* 处理接收到的数据 */
uint8_t IMU_CheckDataReady(void);             /* 检查是否有新数据需要处理 */
void  IMU_HandleRxEvent(uint16_t size);       /* 处理接收事件 */
void  IMU_ProcessPendingData(void);           /* 处理待处理的数据 */

// 新增封装函数 - 将整个IMU的处理、显示和发送逻辑封装
void  IMU_UpdateDisplay(void);               /* 更新显示和串口输出，返回是否有新数据 */
void  IMU_ShowWaitingMessage(void);          /* 显示等待IMU数据的消息 */
void  IMU_InitDisplay(void);                 /* 初始化IMU显示 */

#ifdef __cplusplus
}
#endif
#endif /* IMU_BRIDGE_H */