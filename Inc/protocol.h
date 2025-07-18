#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 协议数据结构定义
typedef struct {
    uint8_t head1;          // 帧头1
    uint8_t head2;          // 帧头2
    uint8_t data[32];       // 数据缓冲区
    uint8_t index;          // 当前数据索引
    uint8_t complete;       // 接收完成标志
    uint8_t data_len;       // 数据长度
} DataReceive;

// 目标属性结构定义
typedef struct {
    int x;                  // X坐标
    int y;                  // Y坐标
    int flag;               // 标志位
} TargetProperty;

// 函数声明
void Data_Receive_Init(DataReceive* rx_struct, uint8_t head1, uint8_t head2);
void Target_Init(TargetProperty* target);
void Data_Receive(DataReceive* rx_struct, uint8_t data);
void Target_Parse(DataReceive* rx_struct, TargetProperty* target);

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_H
