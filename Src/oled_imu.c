#include "oled_imu.h"  // 添加对应的头文件
#include "OLED.h"      // OLED基础函数
#include <string.h>
#include <stdio.h>
#include <math.h>

/**
  * @brief  初始化OLED用于IMU数据显示
  * @param  无
  * @retval 无
  */
void OLED_IMU_Init(void)
{
    OLED_Clear();
    OLED_ShowString(1, 1, "IMU Data");
}

/**
  * @brief  显示IMU等待信息
  * @param  无
  * @retval 无
  */
void OLED_IMU_ShowWaiting(void)
{
    static uint8_t displayed_wait_msg = 0;
    
    if (!displayed_wait_msg) {
        OLED_Clear();
        OLED_ShowString(1, 1, "Wait IMU...");
        displayed_wait_msg = 1;
    }
}

/**
  * @brief  更新IMU数据到OLED显示 - 平滑显示版本
  * @param  roll: 横滚角
  * @param  pitch: 俯仰角
  * @param  yaw: 偏航角
  * @retval 无
  */
void OLED_IMU_UpdateData(float roll, float pitch, float yaw)
{
    // 静态变量用于保存上一次显示的数据和计时
    static float last_roll = 0.0f, last_pitch = 0.0f, last_yaw = 0.0f;
    static float display_roll = 0.0f, display_pitch = 0.0f, display_yaw = 0.0f;
    static uint8_t is_first_update = 1;
    static uint32_t last_update_tick = 0;
    static char str_roll[16], str_pitch[16], str_yaw[16]; // 预分配静态缓冲区
    
    // 获取当前tick，用于控制更新频率
    uint32_t current_tick = HAL_GetTick();
    
    // 平滑因子 - 越小，跟随越平滑但反应越慢
    const float smooth_factor = 1.0f;  // 设置为1.0，完全不平滑，立即响应
    
    // 初始化时清屏并显示标题
    if (is_first_update) {
        OLED_Clear();
        OLED_ShowString(1, 1, "IMU Data");
        is_first_update = 0;
        
        // 初始化显示值和上一次的值
        display_roll = roll;
        display_pitch = pitch;
        display_yaw = yaw;
        last_roll = roll;
        last_pitch = pitch;
        last_yaw = yaw;
        
        // 预先格式化并显示初始值
        sprintf(str_roll, "R:%.1f   ", display_roll);
        sprintf(str_pitch, "P:%.1f   ", display_pitch);
        sprintf(str_yaw, "Y:%.1f   ", display_yaw);
        
        OLED_ShowString(2, 1, str_roll);
        OLED_ShowString(3, 1, str_pitch);
        OLED_ShowString(4, 1, str_yaw);
        
        last_update_tick = current_tick;
        return;
    }
    
    // 更新IMU数据到显示
    // 以10Hz的频率更新显示
    if (current_tick - last_update_tick >= 100) { // 刷新率10Hz
        // 计算当前值和目标值的差值
        float roll_diff = roll - display_roll;
        float pitch_diff = pitch - display_pitch;
        float yaw_diff = yaw - display_yaw;
        
        // 平滑过渡 - 使用线性插值
        display_roll += roll_diff * smooth_factor;
        display_pitch += pitch_diff * smooth_factor;
        display_yaw += yaw_diff * smooth_factor;
        
        // 仅在变化足够大时更新显示，避免微小抖动
        const float display_threshold = 0.03f; // 显示阈值，调小以更灵敏地响应小变化
        
        // 计算与上次显示值的差异
        float roll_display_diff = fabsf(display_roll - last_roll);
        float pitch_display_diff = fabsf(display_pitch - last_pitch);
        float yaw_display_diff = fabsf(display_yaw - last_yaw);
        
        // 只有差异超过阈值或时间间隔较长才更新显示
        if (roll_display_diff > display_threshold || current_tick - last_update_tick >= 100) {
            sprintf(str_roll, "R:%.1f   ", display_roll);
            OLED_ShowString(2, 1, str_roll);
            last_roll = display_roll;
        }
        
        if (pitch_display_diff > display_threshold || current_tick - last_update_tick >= 100) {
            sprintf(str_pitch, "P:%.1f   ", display_pitch);
            OLED_ShowString(3, 1, str_pitch);
            last_pitch = display_pitch;
        }
        
        if (yaw_display_diff > display_threshold || current_tick - last_update_tick >= 100) {
            sprintf(str_yaw, "Y:%.1f   ", display_yaw);
            OLED_ShowString(4, 1, str_yaw);
            last_yaw = display_yaw;
        }
        
        last_update_tick = current_tick;
    }
}

/**
  * @brief  清除OLED指定行
  * @param  Line: 行号 (1-4)
  * @retval 无
  */
void OLED_ClearLine(uint8_t Line)
{
    // 检查行号范围
    if (Line < 1 || Line > 4)
        return;
    
    // OLED每行8个像素高度，每个字符8个像素宽度
    // 直接使用空格填充整行，实现清除效果
    OLED_ShowString(Line, 1, "                ");  // 16个空格，覆盖128像素宽度
}
