#ifndef __OLED_IMU_H
#define __OLED_IMU_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IMU数据OLED显示模块
 * @note 本模块提供IMU数据在OLED上的显示功能
 */

/**
  * @brief  初始化OLED用于IMU数据显示
  * @param  无
  * @retval 无
  */
void OLED_IMU_Init(void);

/**
  * @brief  显示IMU等待信息
  * @param  无
  * @retval 无
  */
void OLED_IMU_ShowWaiting(void);

/**
  * @brief  更新IMU数据到OLED显示
  * @param  roll: 横滚角
  * @param  pitch: 俯仰角
  * @param  yaw: 偏航角
  * @retval 无
  */
void OLED_IMU_UpdateData(float roll, float pitch, float yaw);

/**
  * @brief  清除OLED指定行
  * @param  Line: 行号 (1-4)
  * @retval 无
  */
void OLED_ClearLine(uint8_t Line);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_IMU_H */
