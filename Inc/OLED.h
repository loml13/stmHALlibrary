#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "main.h"

// 外部字体数组声明
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t CC_F16x16[][16];

// OLED函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFNum(uint8_t Line, uint8_t Column, float Number, uint8_t Length, uint8_t Flength);
void OLED_ShowCC_F16x16(uint8_t Line, uint8_t Column, uint8_t num);

// 新增函数声明
void OLED_Fill(uint8_t fill_data);
HAL_StatusTypeDef OLED_Test_Connection(void);
uint8_t OLED_Safe_ShowString(uint8_t Line, uint8_t Column, char *String);
uint8_t OLED_Safe_Clear(void);

#endif
