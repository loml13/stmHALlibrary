#ifndef JY901S_HPP
#define JY901S_HPP

#include "stm32f4xx_hal.h"
#include <cstdint>

namespace JY901S
{
/* ---- 寄存器/指令常量 ---- */
static const uint8_t CMD_UNLOCK[5]    = {0xFF,0xAA,0x69,0x88,0xB5}; // 解锁寄存器
static const uint8_t CMD_SAVE[5]      = {0xFF,0xAA,0x00,0x00,0x00}; // 保存寄存器
static const uint8_t CMD_ACC_CAL[5]   = {0xFF,0xAA,0x01,0x01,0x00}; // 加速度计校准
static const uint8_t CMD_MAG_CAL[5]   = {0xFF,0xAA,0x01,0x07,0x00}; // 磁力计校准

/* ---- 原始数据帧类型 ---- */
enum FrameID
{
    FID_ACC    = 0x51, // 加速度计
    FID_GYRO   = 0x52, // 陀螺仪
    FID_ANGLE  = 0x53, // 姿态角
    FID_MAG    = 0x54, // 磁力计
    FID_STATUS = 0x55, // 状态
    FID_PRESS  = 0x56, // 气压
    FID_LONLAT = 0x57, // 经纬度
    FID_GPS    = 0x58, // GPS
    FID_QUAT   = 0x59  // 四元数
};

/* ---- 物理量结构 ---- */
struct Vector3f { float x, y, z; }; // g, °/s, uT
struct Attitude  { float roll, pitch, yaw; }; // °
struct Quaternion{ float q0, q1, q2, q3; }; // 单位化

/* ---- 解析结果 ---- */
struct DataPacket
{
    Vector3f acc;      // g
    Vector3f gyro;     // °/s
    Attitude angle;    // °
    Vector3f mag;      // uT
    Quaternion quat;   // 单位化
    uint32_t  tick;    // 接收时间戳 (ms)
};

/* ---- C++ 驱动类 ---- */
class Driver
{
public:
    explicit Driver(UART_HandleTypeDef* huart);

    void start();                      // 开启 DMA 接收
    bool update(DataPacket& out);      // 若解析到完整姿态帧则返回 true
    void sendCmd(const uint8_t cmd[5]); // 发送 5 字节指令

    /* 供中断回调调用 */
    void isr();
    void processData(uint16_t size);  // 处理接收到的数据

private:
    static const uint16_t RX_LEN = 128;   // DMA 缓冲区长度
    UART_HandleTypeDef*   _huart;         // UART 句柄
    uint8_t               _rxBuf[RX_LEN]; // 接收缓冲区
    volatile uint16_t     _fill;          // 当前填充长度
    volatile bool         _newFrameReady; // 新帧就绪标志

    /* 帧解析辅助 */
    bool parseOneByte(uint8_t ch); // 解析单字节，返回是否解析到完整帧
    uint8_t _frame[11];            // 当前解析的帧数据
    uint8_t _idx;   
};

} // namespace JY901S
#endif
