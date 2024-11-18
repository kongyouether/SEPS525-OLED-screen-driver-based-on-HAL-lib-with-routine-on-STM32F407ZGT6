 * <HAL library based SEPS525 screen driver>
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

 * Copyright (C) 2024 KongyouxEther
 * All rights reserved.
 * 
 * Author: KongyouxEther
 * Email: kongyouether@qq.com
   
*   GPIO引脚定义：                                         
*   OLED_CS|OLED_RES|OLED_DC ---> GPIO OUTPUT Output push pull
*********************************************************
*   SPI定义：                                          
*   Mode:Transmit only Master                          
*   Hardware Nss Signal:Disable                        
*   Data Size : 8Bits                                  
*   First Bit : MSB First                              
*   CPOL : High                                         
*   CPHA : 2 Edge
*   Baud Rate Prescaler : 4(无限制，只要能显示就行)	 
*********************************************************
*   接线：
*   OLED 	---> 	STM32F407ZGT6                                             
*
*   GND 	---> 	GND                                       
*   VCC 	---> 	3.3V                                      
*   SCL  	---> 	SPI_SCK                                   
*   SDI  	---> 	SPI_MOSI                                  
*   RES 	---> 	OLED_RES                                  
*   RS  	---> 	OLED_DC                                   
*   CSB  	---> 	OLED_CS                                   
*
*********************************************************
*   OLED显示分辨率：160*128
*   OLED显示颜色：16位彩色
*********************************************************
*   2024/09/23：创建文件，从中景园的构式标准库移植成HAL库
