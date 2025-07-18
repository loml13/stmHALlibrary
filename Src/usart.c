/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <string.h>
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Stream1;
    hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart3_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_usart3_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_usart3_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**********************************************************************************************************************
 * @brief 串口发送数据结构体初始化函数
 * @param[in] data   指向要初始化的DataTransmit结构体的指针
 * @param[in] head1  协议帧头1
 * @param[in] head2  协议帧头2
 * @param[in] length 有效数据的长度
 * @note  此函数用于设置串口发送数据帧的固定部分，并清空数据缓冲区。
 **********************************************************************************************************************/
void Data_Transmit_Init(DataTransmit *data, uint8_t head1, uint8_t head2, uint8_t length)
{
	data -> head1  = head1;
	data -> head2  = head2;
	data -> length = length;
	data -> cnt    = length + 4; // 总长度 = 帧头1 + 帧头2 + 长度位 + 有效数据 + 校验和

	// 清空有效数据缓冲区
	for(uint8_t i = 0; i < length; i++)
	{
		data -> data[i] = 0;
	}

	// 清空完整发送帧缓冲区
	for(uint8_t j = 0; j < data -> cnt; j++)
	{
		data -> transmit_data[j] = 0;
	}
}

/**********************************************************************************************************************
 * @brief 串口发送数据打包函数
 * @param[in,out] data 指向需要打包的DataTransmit结构体的指针
 * @note  此函数将有效数据和协议控制信息（帧头、长度、校验和）组装成一个完整的待发送数据帧。
 **********************************************************************************************************************/
void Data_Pack(DataTransmit *data)
{
	// 填充帧头和长度
	data -> transmit_data[0] = data -> head1;
	data -> transmit_data[1] = data -> head2;
	data -> transmit_data[2] = data -> length;

	// 填充有效数据
	for(uint8_t i = 0; i < data -> length; i++)
	{
		data -> transmit_data[3+i] = data -> data[i];
	}

	uint8_t sum = 0;

    // 计算校验和（从帧头到有效数据的所有字节累加）
    for(uint8_t j = 0; j < data -> length + 3; j++)
	{
		sum = sum + data -> transmit_data[j];
	}

    // 填充校验和
    data -> transmit_data[data -> length + 3] = sum;
}

/**********************************************************************************************************************
 * @brief 串口接收数据结构体初始化函数
 * @param[in] data   指向要初始化的DataReceive结构体的指针
 * @param[in] head1  要识别的协议帧头1
 * @param[in] head2  要识别的协议帧头2
 * @note  此函数用于重置串口接收状态机和缓冲区，为接收新的数据帧做准备。
 **********************************************************************************************************************/
void Data_Receive_Init(DataReceive *data, uint8_t head1, uint8_t head2)
{
	data -> head1  = head1;
	data -> head2  = head2;
	data -> length = 0;     // 有效数据长度
	data -> cnt    = 0;     // 接收数据计数
	data -> state  = 0;     // 接收状态机状态
	data -> i      = 0;     // 辅助计数器
	data -> data   = 0;     // (此行似乎未在后续使用，可考虑移除)
	data -> complete = 0; // 初始化接收完成标志位

	// 清空接收缓冲区
	for(uint8_t j = 0; j < 50; j++)
	{
		data -> receive_data[j] = 0;
	}
}

/**********************************************************************************************************************
 * @brief 串口接收数据处理函数（状态机）
 * @param[in,out] data 指向用于处理接收的DataReceive结构体的指针
 * @param[in]     buf  从串口接收到的单个字节数据
 * @note  此函数是协议解析的核心，通过一个状态机来逐字节地识别和解析数据帧。
 *        应在串口接收中断中被调用。
 **********************************************************************************************************************/
void Data_Receive(DataReceive *data, uint8_t buf)
{
	if(data -> state == 0 && buf == data -> head1)
	{
		data -> state = 1;
		data -> receive_data[0] = buf;
	}
	else if(data -> state == 1 && buf == data -> head2)
	{
		data -> state = 2;
		data -> receive_data[1] = buf;
	}
	else if(data -> state == 2 && buf < 40)
	{
		data -> state = 3;
		data -> length = buf;
		data -> cnt = buf + 5;
		data -> receive_data[2] = buf;
	}
	else if(data -> state == 3 && data -> length > 0)
	{
		data -> length--;
		data -> receive_data[3 + data -> i] = buf;
		data -> i++;

		if(data -> length == 0)
		{
			data -> state = 4;
		}
	}
	else if(data -> state == 4)
	{
		data -> receive_data[3 + data -> i] = buf;
		data -> i++;
		data -> state = 5;
	}
	else if(data -> state == 5 && buf == 0x55)
	{
		data -> receive_data[3 + data -> i] = buf;
		data -> complete = 1;
		data -> state = 0;
		data -> i = 0;
	}
	else
	{
		data -> state = 0;
		data -> i = 0;
	}
}

/**********************************************************************************************************************
 * @brief 目标属性数据结构体初始化函数
 * @param[in] target 指向要初始化的TargetProperty结构体的指针
 * @note  用于清空存储解析结果的结构体。
 **********************************************************************************************************************/
void Target_Init(TargetProperty *target)
{
	target -> x     = 0;
	target -> y     = 0;
	target -> flag  = 0;
}

/**********************************************************************************************************************
 * @brief 串口接收数据解析函数
 * @param[in] data   指向包含待解析数据的DataReceive结构体的指针
 * @param[out] target 指向用于存储解析结果的TargetProperty结构体的指针
 * @note  此函数在接收到一帧完整的数据后被调用，用于校验数据完整性并将数据解析到目标结构体中。
 **********************************************************************************************************************/
void Target_Parse(DataReceive *data, TargetProperty *target)
{
    // 显示接收到的原始数据用于调试
    char raw_msg[150];
    sprintf(raw_msg, "原始接收数据(%d字节): ", data->cnt);
    for(uint8_t i = 0; i < data->cnt; i++) {
        char byte_str[6];
        sprintf(byte_str, "%02X ", data->receive_data[i]);
        strcat(raw_msg, byte_str);
    }
    strcat(raw_msg, "\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)raw_msg, strlen(raw_msg), HAL_MAX_DELAY);
    
    // 优先检查标准协议格式：AA BB LEN PAYLOAD CHK 55
    if(data->cnt == 10 && data->receive_data[data->cnt-1] == 0x55) {
        uint8_t received_checksum = data->receive_data[data->cnt - 2];
        
        // 计算校验和（除校验和和帧尾）
        uint8_t sum_all = 0;
        for(uint8_t i = 0; i < data->cnt - 2; i++) {
            sum_all += data->receive_data[i];
        }
        
        if(sum_all == received_checksum) {
            // 校验成功，解析数据（大端序）
            target->x = (data->receive_data[3] << 8) | data->receive_data[4];
            target->y = (data->receive_data[5] << 8) | data->receive_data[6];
            target->flag = data->receive_data[7];
            
            char result_msg[80];
            sprintf(result_msg, "Standard format parsed: x=%d, y=%d, flag=%d\r\n",  
                    target->x, target->y, target->flag);   //标准格式解析成功
            HAL_UART_Transmit(&huart2, (uint8_t*)result_msg, strlen(result_msg), HAL_MAX_DELAY);
        }
        else {
            char error_msg[100];
            sprintf(error_msg, "Standard format checksum error: calc=0x%02X, recv=0x%02X\r\n", 
                    sum_all, received_checksum); // 标准格式校验和错误: 计算=0x%02X, 接收=0x%02X
            HAL_UART_Transmit(&huart2, (uint8_t*)error_msg, strlen(error_msg), HAL_MAX_DELAY);
        }
        return;
    }
    
  // 检查无帧尾格式
    if(data->cnt == 9 && data->receive_data[data->cnt-1] != 0x55) {
        uint8_t received_checksum = data->receive_data[data->cnt - 1];
        
        // 计算所有字节校验和（除最后的校验和）
        uint8_t sum_all = 0;
        for(uint8_t i = 0; i < data->cnt - 1; i++) {
            sum_all += data->receive_data[i];
        }
        
        if(sum_all == received_checksum) {
            // 校验成功，解析数据（大端序）
            target->x = (data->receive_data[3] << 8) | data->receive_data[4];
            target->y = (data->receive_data[5] << 8) | data->receive_data[6];
            target->flag = data->receive_data[7];
            
            char result_msg[80];
            sprintf(result_msg, "No trailer format parsed: x=%d, y=%d, flag=%d\r\n", 
                    target->x, target->y, target->flag);
            HAL_UART_Transmit(&huart2, (uint8_t*)result_msg, strlen(result_msg), HAL_MAX_DELAY);
        }
        else {
            char error_msg[100];
            sprintf(error_msg, "无帧尾格式校验和错误: 计算=0x%02X, 接收=0x%02X\r\n", 
                    sum_all, received_checksum);
            HAL_UART_Transmit(&huart2, (uint8_t*)error_msg, strlen(error_msg), HAL_MAX_DELAY);
        }
        return;
    }
    
    // 未知格式
    char debug_msg[100];
    sprintf(debug_msg, "未知格式: %d 字节, 最后一字节=0x%02X\r\n", 
            data->cnt, data->receive_data[data->cnt-1]);
    HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
}

// 串口忙标志，静态变量，防止外部直接访问
static uint8_t uart_busy = 0;

/**
  * @brief  使用DMA方式发送数据到USART1
  * @param  data: 要发送的数据指针
  * @param  len: 数据长度
  * @retval HAL状态
  */
HAL_StatusTypeDef UART_SendData_DMA(const char* data, uint16_t len)
{
    // 检查串口是否忙
    if (uart_busy) {
        return HAL_BUSY;
    }
    
    // 使用DMA发送数据
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&huart1, (uint8_t*)data, len);
    
    // 如果发送成功，标记串口为忙状态
    if (status == HAL_OK) {
        uart_busy = 1;
    }
    
    return status;
}

/**
  * @brief  检查并更新串口传输完成状态
  * @param  None
  * @retval None
  */
void UART_CheckTransmissionComplete(void)
{
    // 检查传输完成标志并重置忙状态
    if (uart_busy && (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != RESET)) {
        uart_busy = 0;
    }
}

/**
  * @brief  串口传输完成回调
  * @param  huart: 串口句柄
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // 清除串口忙标志，允许新的传输
    if (huart->Instance == USART1)
    {
        uart_busy = 0;
    }
}

/* USER CODE END 1 */
