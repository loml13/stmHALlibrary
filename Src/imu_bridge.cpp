/*  imu_bridge.cpp  ———— C++ 实现层，隐藏所有 JY901S 细节 */
#include "imu_bridge.h"          // 放在 extern "C" 外
#include "jy901s.hpp"            // C++ 头文件
#include <cstring>               // memset
#include "oled_imu.h"            // IMU数据OLED显示
#include "usart.h"               // UART DMA发送

// C库标准函数前向声明
extern "C" {
    int sprintf(char *str, const char *format, ...);
    size_t strlen(const char *s);
    // 不再需要OLED_IMU相关函数的前向声明，已通过包含oled_imu.h获取
    HAL_StatusTypeDef UART_SendData_DMA(const char* data, uint16_t len);
    void UART_CheckTransmissionComplete(void);
}

/* --------- 私有静态对象 --------- */
static JY901S::Driver    *pImu  = NULL;
static JY901S::DataPacket imuPkt = {0};  // 显式初始化为0
static volatile uint8_t  imu_data_ready = 0;   // 数据就绪标志
static volatile uint16_t imu_data_size = 0;    // 数据长度

// IMU数据与时间戳
static float roll = 0, pitch = 0, yaw = 0;
static uint32_t last_uart_tick = 0;
static char uart_msg[50]; // 用于串口输出的缓冲区

/* --------- C 接口实现 --------- */
extern "C" {

// 确保这些函数是以C风格导出的，在链接时不会被修饰
#define EXTERN_C_FUNC extern "C" __attribute__((visibility("default")))

EXTERN_C_FUNC
void IMU_Init(UART_HandleTypeDef *huart)
{
    static JY901S::Driver imuLocal(huart);  // 静态，生命周期 = 程序全程
    pImu = &imuLocal;
    pImu->start();          // 开启 DMA+IDLE
    
    // 等待IMU稳定
    HAL_Delay(200);
    
    // 配置JY901S只输出角度帧，避免多帧混淆
    const uint8_t ONLY_ANGLE[5] = {0xFF,0xAA,0x02,0x04,0x00}; // RSWL=0x04 -> 只开角度输出
    pImu->sendCmd(ONLY_ANGLE);
    HAL_Delay(50);
    
    // 保存设置
    pImu->sendCmd(JY901S::CMD_SAVE);
    HAL_Delay(50);
    
    // 重新启动DMA接收
    pImu->start();
}

EXTERN_C_FUNC
uint8_t IMU_Poll(void)
{
    // 只有在有新数据时才返回true
    if (pImu && pImu->update(imuPkt))
    {
        return 1;  // 有新数据
    }
    return 0;  // 没有新数据
}

void IMU_GetAngle(float *roll, float *pitch, float *yaw)
{
    if (roll)  *roll  = imuPkt.angle.roll;
    if (pitch) *pitch = imuPkt.angle.pitch;
    if (yaw)   *yaw   = imuPkt.angle.yaw;
}

void IMU_UartIdleIsr(void)
{
    if (pImu) pImu->isr();
}

void IMU_ProcessData(uint16_t size)
{
    if (pImu) pImu->processData(size);
}

uint8_t IMU_CheckDataReady(void)
{
    return imu_data_ready;
}

EXTERN_C_FUNC
void IMU_HandleRxEvent(uint16_t size)
{
    // 在中断中只设置标志，不做数据处理
    imu_data_size = size;
    imu_data_ready = 1;
}

EXTERN_C_FUNC
void IMU_ProcessPendingData(void)
{
    if (imu_data_ready)
    {
        // 高速处理接收到的IMU数据
        if (imu_data_size > 0)
        {
            // 直接处理数据
            IMU_ProcessData(imu_data_size);
        }
        // 立即重新启动DMA接收，减少延迟
        IMU_UartIdleIsr();
        // 清除标志
        imu_data_ready = 0;
        
        // 消除任何额外延迟，确保最快的更新速度
    }
}

// 实现新的IMU显示和数据处理函数
EXTERN_C_FUNC
void IMU_UpdateDisplay(void)
{
    // 获取当前系统时间
    uint32_t current_tick = HAL_GetTick();
    
    if(IMU_Poll()) // 如果有新IMU数据
    {
        // 获取最新角度数据
        IMU_GetAngle(&roll, &pitch, &yaw);
        
        // 更新OLED显示
        OLED_IMU_UpdateData(roll, pitch, yaw);
        
        // 每100ms (10Hz)发送一次串口数据，避免串口成为瓶颈
        if (current_tick - last_uart_tick >= 100) {
            // 格式化串口消息
            sprintf(uart_msg, "%.1f,%.1f,%.1f\r\n", roll, pitch, yaw);
            
            // 使用DMA发送，不阻塞主循环
            if (UART_SendData_DMA(uart_msg, strlen(uart_msg)) == HAL_OK) {
                last_uart_tick = current_tick;
            }
        }
        
        return; // 有新数据
    }
}

EXTERN_C_FUNC
void IMU_ShowWaitingMessage(void)
{
    // 使用OLED显示等待信息
    OLED_IMU_ShowWaiting();
    
    // 非阻塞发送等待消息
    static const char wait_msg[] = "Waiting for IMU data...\r\n";
    UART_SendData_DMA(wait_msg, sizeof(wait_msg)-1);
}

EXTERN_C_FUNC
void IMU_InitDisplay(void)
{
    // 初始化OLED显示模式
    OLED_IMU_Init();
}

} /* extern "C" */
