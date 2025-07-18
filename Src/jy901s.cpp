#include "jy901s.hpp"
#include <cstring>
#include <cmath>
#include <cstdio>
#include <stdio.h>
#include <string.h>

using namespace JY901S;

/* ---------- 公有接口 ---------- */
Driver::Driver(UART_HandleTypeDef* huart) : _huart(huart), _fill(0), _newFrameReady(false), _idx(0) 
{
    // 清零帧缓冲区
    for (int i = 0; i < 11; i++) _frame[i] = 0;
    for (int i = 0; i < RX_LEN; i++) _rxBuf[i] = 0;
}

void Driver::start()
{
    _fill = 0;
    _newFrameReady = false;
    _idx = 0;
    // 清零所有缓冲区
    for (int i = 0; i < 11; i++) _frame[i] = 0;
    for (int i = 0; i < RX_LEN; i++) _rxBuf[i] = 0;
    HAL_UARTEx_ReceiveToIdle_DMA(_huart, _rxBuf, RX_LEN);
}

void Driver::sendCmd(const uint8_t cmd[5])
{
    HAL_UART_Transmit(_huart, const_cast<uint8_t*>(cmd), 5, 100); // 发送 5 字节指令
}

bool Driver::update(DataPacket& out)
{
    /* 高性能版本 - 使用HAL_UARTEx_ReceiveToIdle_DMA模式，无调试输出 */
    /* 优化关键路径，帧有效性检查已在parseOneByte中完成 */
    if (_newFrameReady)
    {
        // 快速计算角度值 - 不再检查帧头(已在parseOneByte中确认)
        float roll  = static_cast<int16_t>((_frame[3]<<8)|_frame[2]) / 32768.f * 180.f;
        float pitch = static_cast<int16_t>((_frame[5]<<8)|_frame[4]) / 32768.f * 180.f;
        float yaw   = static_cast<int16_t>((_frame[7]<<8)|_frame[6]) / 32768.f * 180.f;
        
        // 更新输出值
        out.angle.roll = roll;
        out.angle.pitch = pitch;
        out.angle.yaw = yaw;
        out.tick = HAL_GetTick();
        
        // 清除标志
        _newFrameReady = false;
        return true;
    }
    return false;
}

/* ---------- 中断服务 ---------- */
void Driver::isr()
{
    /* 高性能版本 - 最小化延迟 */
    HAL_UART_DMAStop(_huart);
    
    /* 使用最高优先级启动DMA接收 */
    HAL_UARTEx_ReceiveToIdle_DMA(_huart, _rxBuf, RX_LEN);
    __HAL_DMA_DISABLE_IT(_huart->hdmarx, DMA_IT_HT); // 禁用半传输中断，减少中断开销
    
    _fill = 0;  // 清零填充指针
}

void Driver::processData(uint16_t size)
{
    /* 超高性能数据处理 - 极速版 */
    if (size > 0 && size <= RX_LEN)
    {
        // 快速批量处理数据
        uint16_t i = 0;
        
        #ifdef USE_UNROLLED_LOOP
        // 使用循环展开加速处理 (每4个一组)
        for (; i <= size - 4; i += 4)
        {
            if (parseOneByte(_rxBuf[i]))   _newFrameReady = true;
            if (parseOneByte(_rxBuf[i+1])) _newFrameReady = true;
            if (parseOneByte(_rxBuf[i+2])) _newFrameReady = true;
            if (parseOneByte(_rxBuf[i+3])) _newFrameReady = true;
        }
        #endif
        
        // 处理剩余字节
        for (; i < size; i++)
        {
            if (parseOneByte(_rxBuf[i]))
            {
                _newFrameReady = true;
            }
        }
    }
}

/* ---------- 内部解析状态机 ---------- */
bool Driver::parseOneByte(uint8_t ch)
{
    // 优化的帧解析状态机
    switch(_idx)
    {
    case 0:
        // 帧头必须是0x55
        if(ch == 0x55) { 
            _frame[_idx++] = ch;
        }
        else {
            // 静默处理非法帧头，不再输出调试信息
            _idx = 0;
        }
        break;
    case 1:
        // 只接受角度帧类型(0x53)
        if(ch == FID_ANGLE) {  
            _frame[_idx++] = ch;
        }
        else {
            _idx = 0; // 重置状态机
            // 如果是0x55，说明可能是新帧开始
            if(ch == 0x55) {
                _frame[0] = ch;
                _idx = 1;
            }
        }
        break;
    default:
        _frame[_idx++] = ch;       // 存储数据帧内容
        if(_idx == 11)  // 帧完整
        {
            /* 快速校验和验证 */
            uint8_t sum = 0;
            for(int i=0;i<10;++i) sum += _frame[i];
            
            _idx = 0;  // 重置状态机，准备接收下一帧
            
            // 帧类型和校验都正确才处理
            if (sum == _frame[10] && _frame[1] == FID_ANGLE) {
                // 校验通过，直接返回有效
                return true;
            }
            // 静默处理校验错误，不再输出调试信息
        }
        break;
    }
    return false;
}
