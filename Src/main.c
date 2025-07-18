/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "OLED.h"
#include "oled_imu.h"  // 添加IMU显示相关头文件
#include "imu_bridge.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 协议接收相关变量
DataReceive g_rx_data_struct;   // 接收数据结构体
TargetProperty g_target_property; // 解析目标结构体
uint8_t g_uart_rx_byte = 0;    // 单字节接收缓冲区

// 平滑显示模式 - 使用插值平滑过渡
static uint32_t valid_count = 0;
static uint8_t wait_init = 1;
static uint32_t last_uart_tick = 0;
static char uart_msg[50]; // 静态缓冲区避免重复分配
static float roll = 0, pitch = 0, yaw = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM10_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  
  // 初始化协议接收结构体
  Data_Receive_Init(&g_rx_data_struct, 0xAA, 0xBB);
  Target_Init(&g_target_property);
  
  // 初始化OLED显示
  OLED_Init();
  
  // 使用安全的OLED显示函数
  if (OLED_Safe_Clear()) {
      OLED_Safe_ShowString(1, 1, " System Start");
  }

  // 启动消息（在IMU初始化之前发送）- 初始化消息仍使用阻塞方式确保立即发送
  char start_msg[] = "System Start - USART1 Ready for data\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t*)start_msg, strlen(start_msg), HAL_MAX_DELAY);

  //初始化陀螺仪
  IMU_Init(&huart3);
  
  // 初始化IMU显示
  IMU_InitDisplay();

  // 启动USART3中断接收
  // HAL_UART_Receive_IT(&huart3, &g_uart_rx_byte, 1);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
    // 优先处理IMU数据（在主循环中，不在中断中）
    IMU_ProcessPendingData();
    
    /*------------------maixcam串口接收-----------------------
    // 检查协议是否接收完成
    if (g_rx_data_struct.complete == 1)
    {
        // 解析接收到的数据
        Target_Parse(&g_rx_data_struct, &g_target_property);
        
        // 发送解析结果到串口助手
        char result_msg[100];
        sprintf(result_msg, "Target Info: x=%d, y=%d, flag=%d\r\n", 
                g_target_property.x, g_target_property.y, g_target_property.flag); 
        HAL_UART_Transmit(&huart6, (uint8_t*)result_msg, strlen(result_msg), HAL_MAX_DELAY);
        
        // 在OLED上显示坐标
        OLED_ShowNum(1, 1, g_target_property.x, 4);
        OLED_ShowNum(2, 1, g_target_property.y, 4);
        
        // 清除接收完成标志
        g_rx_data_struct.complete = 0;
    }
  -------------------------------------------------------*/
    
    // 使用封装的IMU处理函数更新显示和串口输出
    IMU_UpdateDisplay();
    
    // 如果需要显示等待信息
    if (wait_init)
    {
        // 使用封装的API显示等待信息
        IMU_ShowWaitingMessage();
        
        // 确保完成等待信息后置位标志
        wait_init = 0;
        valid_count++;    
    }
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  串口空闲中断回调函数
  * @param  huart: 串口句柄
  * @param  Size: 接收到的数据长度
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART3)
    {
        // 高优先级处理IMU数据
        IMU_HandleRxEvent(Size);
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();    // 禁用所有中断，防止进一步的系统操作
  while (1)    // 进入无限循环，系统停止运行
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */    // 用户可以在此添加自己的实现来报告文件名和行号
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
