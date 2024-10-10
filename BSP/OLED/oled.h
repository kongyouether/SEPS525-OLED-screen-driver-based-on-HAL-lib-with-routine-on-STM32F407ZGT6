/* <HAL library based SEPS525 screen driver>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * Copyright (C) 2024 KongyouxEther
 * All rights reserved.
 * 
 * Author: KongyouxEther
 * Email: kongyouether@qq.com
 */
#ifndef __OLED_H
#define __OLED_H 
 
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "stdio.h"

//-----------------OLED端口定义---------------- 
#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)
 
#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_SET)


//-----------------重要参数定义----------------
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
 
#define OLED_W 160
#define OLED_H 128

#define WHICH_SPI &hspi1//根据使用的SPI更改

#define USE_HORIZONTAL 1  //设置显示方向  1正向显示  0旋转180度显示

#define uint8_t unsigned char
#define uint32_t unsigned int


//-----------------函数声明----------------
// void OLED_DisplayTurn(uint8_t i);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color);
void OLED_DrawSquare(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint16_t color);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r, uint16_t color);
void OLED_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);
void OLED_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);
void OLED_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);
void OLED_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);
void OLED_ScreenProtect(void);
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_WR_16Byte(uint16_t dat,uint8_t cmd);
void OLED_Init(void);
void OLED_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void OLED_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void OLED_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

//-----------------画笔颜色----------------
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE           	 	0x001F  
#define BRED             	0XF81F
#define GRED 			 				0XFFE0
#define GBLUE		     			0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 			0XBC40 //棕色
#define BRRED 			 			0XFC07 //棕红色
#define GRAY  			 			0X8430 //灰色
#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 //灰蓝色
#define LIGHTGREEN     	 	0X841F //浅绿色
#define LGRAY 			 			0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           	0X2B12 //浅棕蓝色(选择条目的反色)

#endif
