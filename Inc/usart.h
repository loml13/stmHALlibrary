/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

// UART协议相关定义
// 串口发送相关结构体
typedef struct
{
	uint8_t	head1;							// 帧头1
	uint8_t	head2;							// 帧头2
	uint8_t length;							// 有效数据长度
	uint8_t cnt;							// 总数据长度
	uint8_t data[40];						// 有效数据数组
	uint8_t transmit_data[50];	// 实际发送的数组 附带上帧头1 帧头2 有效数据长度位 校验位
}DataTransmit;

// 串口接收相关结构体
typedef struct
{
	uint8_t	head1;							// 帧头1
	uint8_t	head2;							// 帧头2
	uint8_t length;							// 有效数据长度
	uint8_t cnt;							// 总数据长度
	uint8_t state;							// 接收状态
	uint8_t i;                              // 有效数据下标
	uint8_t receive_data[50];				// 实际接收的数组
	uint8_t data;                           
	uint8_t complete;                       // 接收完成标志位
}DataReceive;

// 接收数据解析相关结构体
typedef struct
{
	uint16_t	x;							// 目标x轴坐标
	uint16_t	y;							// 目标y轴坐标
	uint8_t   flag;						// 目标标志位
}TargetProperty;

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

// 串口发送相关函数
void Data_Transmit_Init(DataTransmit *data, uint8_t head1, uint8_t head2, uint8_t length);
void Data_Pack(DataTransmit *data);

// 串口接收相关函数
void Data_Receive_Init(DataReceive *data, uint8_t head1, uint8_t head2);
void Data_Receive(DataReceive *data, uint8_t buf);

// 接收数据解析相关函数
void Target_Init(TargetProperty *target);
void Target_Parse(DataReceive *data, TargetProperty *target);

// UART DMA发送相关函数
HAL_StatusTypeDef UART_SendData_DMA(const char* data, uint16_t len);
void UART_CheckTransmissionComplete(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

