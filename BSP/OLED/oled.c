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
/*********************************************************
*   GPIO���Ŷ��壺                                         
*   OLED_CS|OLED_RES|OLED_DC ---> GPIO OUTPUT Output push pull
*********************************************************
*   SPI���壺                                          
*   Mode:Transmit only Master                          
*   Hardware Nss Signal:Disable                        
*   Data Size : 8Bits                                  
*   First Bit : MSB First                              
*   CPOL : High                                         
*   CPHA : 2 Edge
*   Baud Rate Prescaler : 4(�����ƣ�ֻҪ����ʾ����)	 
*********************************************************
*   ���ߣ�
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
*   OLED��ʾ�ֱ��ʣ�160*128
*   OLED��ʾ��ɫ��16λ��ɫ
*********************************************************
*   2024/09/23�������ļ������о�԰�Ĺ�ʽ��׼����ֲ��HAL��
********************************************************/


//-----------------����----------------

#include "stdio.h"
#include <string.h>
#include <stdarg.h>
#include "main.h"
#include "oled.h"
#include "oledfont.h"
#include "pic.h"
 
//-----------------����---------------
uint8_t WHICH_CS = 0;

/******************************************************************************
      ����˵������Ļ��ת
      ������ݣ�i  0-������ʾ  1-���ҷ�ת  2-���·�ת
      ����ֵ��  ��
******************************************************************************/
// void OLED_DisplayTurn(uint8_t i)
// {
// 	if(i==0)
// 		{
// 			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
// 			OLED_WR_Byte(0xA1,OLED_CMD);
// 		}
// 	if(i==1)
// 		{
// 			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
// 			OLED_WR_Byte(0xA0,OLED_CMD);
// 		}
// 	if(i==2)
// 		{
// 			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
// 			OLED_WR_Byte(0xA1,OLED_CMD);
// 		}
// }
 
 
/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
 void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint16_t color)
 {
 	uint8_t i,k,k1,k2;
 	if((x2>128)||(y2>64)||(x1>x2)||(y1>y2))return;
 	if(x1==x2)    //������
 	{
 			for(i=0;i<(y2-y1);i++)
 			{
 				OLED_DrawPoint(x1,y1+i, color);
 			}
   }
 	else if(y1==y2)   //������
 	{
 			for(i=0;i<(x2-x1);i++)
 			{
 				OLED_DrawPoint(x1+i,y1, color);
 			}
   }
 	else      //��б��
 	{
 		k1=y2-y1;
 		k2=x2-x1;
 		k=k1*10/k2;
 		for(i=0;i<(x2-x1);i++)
 			{
 			  OLED_DrawPoint(x1+i,y1+i*k/10, color);
 			}
 	}
 	
 }


/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
 void OLED_DrawSquare(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint16_t color)
 {
 	OLED_DrawLine(x1,y1,x2,y1, color);
 	OLED_DrawLine(x1,y2,x2,y2, color);
 	OLED_DrawLine(x1,y1,x1,y2, color);
 	OLED_DrawLine(x2,y1,x2,y2+1, color);
 }


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
 void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r, uint16_t color)
 {
 	int a, b,num;
     a = 0;
     b = r;
     while(2 * b * b >= (r-1) * (r))      
     {
         OLED_DrawPoint(x + a, y - b, color);
         OLED_DrawPoint(x - a, y - b, color);
         OLED_DrawPoint(x - a, y + b, color);
         OLED_DrawPoint(x + a, y + b, color);
 
         OLED_DrawPoint(x + b, y + a, color);
         OLED_DrawPoint(x + b, y - a, color);
         OLED_DrawPoint(x - b, y - a, color);
         OLED_DrawPoint(x - b, y + a, color);
        
         a++;
         num = (a * a + b * b) - r*r- 4;//���㻭�ĵ���Բ�ĵľ���
         if(num > 0)
         {
             b--;
            
         }
     }
 }


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t;
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=sizex/8*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	OLED_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)temp=ascii_1608[num][i];		       //����8x16����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t))
					OLED_WR_16Byte(fc,OLED_DATA);
				else 
					OLED_WR_16Byte(bc,OLED_DATA);
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t))OLED_DrawPoint(x,y,fc);//��һ����
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		OLED_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}



/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowChinese16_16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			OLED_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))OLED_WR_16Byte(fc,OLED_DATA);
						else OLED_WR_16Byte(bc,OLED_DATA);
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))OLED_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}


/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowChinese24_24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			OLED_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))OLED_WR_16Byte(fc,OLED_DATA);
						else OLED_WR_16Byte(bc,OLED_DATA);
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))OLED_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowChinese32_32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			OLED_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))OLED_WR_16Byte(fc,OLED_DATA);
						else OLED_WR_16Byte(bc,OLED_DATA);
					}
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))OLED_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==16) OLED_ShowChinese16_16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) OLED_ShowChinese24_24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) OLED_ShowChinese32_32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}


 /******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
******************************************************************************/
void OLED_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j,k=0;
	OLED_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			OLED_WR_Byte(pic[k*2],OLED_DATA);
			OLED_WR_Byte(pic[k*2+1],OLED_DATA);
			k++;
		}
	}			
}


/******************************************************************************
      ����˵��������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void OLED_ScreenProtect(void)
{
	int DVD_H,DVD_W;
	int dh,dw;
	uint16_t i,color = 0;
	DVD_H = 0;
	DVD_W = 0;
	dh = 1;
	dw = 2;
	OLED_Fill(0, 0, 160, 128, 0x0000);
	color = WHITE;
	while(1)
	{	
		if (DVD_W-2>=0){
			OLED_Fill(DVD_W-2, DVD_H, DVD_W, DVD_H+16, 0x0000);	
		}
		else if(DVD_W-1>=0){
			OLED_Fill(DVD_W-1, DVD_H, DVD_W, DVD_H+16, 0x0000);	
		}
		else{
			OLED_Fill(0, DVD_H, DVD_W, DVD_H+16, 0x0000);	
		}
		if(DVD_H-1>0){
			OLED_Fill(DVD_W, DVD_H-1, DVD_W+24, DVD_H, 0x0000);
		}
		if(DVD_W+26<160){
			OLED_Fill(DVD_W+24, DVD_H, DVD_W+26, DVD_H+16, 0x0000);
		}
		else if(DVD_W+25<160){
			OLED_Fill(DVD_W+24, DVD_H, DVD_W+25, DVD_H+16, 0x0000);
		}
		if(DVD_H+17<128){
			OLED_Fill(DVD_W, DVD_H+16, DVD_W+24, DVD_H+17, 0x0000);
		}
		
		OLED_ShowString(DVD_W, DVD_H,"DVD",color,0x0000,16,0);
		
		DVD_W += dw;
		DVD_H += dh;
		
		if(DVD_W > 160-24)
		{
			i++;
			DVD_W = 160-24;
			dw = -2;
		}
		if(DVD_W <= 0)
		{
			i++;
			DVD_W = 0;
			dw = 2;
		}
		if(DVD_H > 128-16)
		{
			i++;
			DVD_H = 128-16;
			dh = -1;
		}
		if(DVD_H <= 0)
		{
			i++;
			DVD_H = 0;
			dh = 1;
		}
		switch(i%10)
		{
			case 0:
				color = WHITE;
				break;
			case 1:
				color = BLUE;
				break;
			case 2:
				color = BRED;
				break;
			case 3:	
				color = GRED;
				break;
			case 4:
				color = GBLUE;
				break;
			case 5:
				color = RED;
				break;
			case 6:
				color = MAGENTA;
				break;
			case 7:
				color = GREEN;
				break;
			case 8:
				color = CYAN;
				break;
			case 9:
				color = YELLOW;
				break;
		}
			
		
		HAL_Delay(50);
	}
}
	


/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
                color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void OLED_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{          
    uint16_t i, j; 
    OLED_Address_Set(xsta, ysta, xend-1, yend-1);  // ������ʾ��Χ
    for(i = ysta; i < yend; i++)
    {                                                     
        for(j = xsta; j < xend; j++)
        {
            OLED_WR_16Byte(color, OLED_DATA);
        }
    }                       
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void OLED_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    OLED_Address_Set(x, y, x, y);  // ���ù��λ�� 
    OLED_WR_16Byte(color, OLED_DATA);
		
}


/******************************************************************************
      ����˵��:	OLEDƬѡ ��0
      �������:	uint8_t WHICH_CS ѡ��Ƭѡ
      ����ֵ��  ��
	  ע��WHICH_CSΪ0ʱ��4��ͬʱ��0
******************************************************************************/
void OLED_CS_Clr()
{
	switch (WHICH_CS)
	{
	case 0:
		HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(OLED_CS2_GPIO_Port, OLED_CS2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(OLED_CS3_GPIO_Port, OLED_CS3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(OLED_CS4_GPIO_Port, OLED_CS4_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_RESET);
		break;
	
	case 2:
		HAL_GPIO_WritePin(OLED_CS2_GPIO_Port, OLED_CS2_Pin, GPIO_PIN_RESET);
		break;

	case 3:
		HAL_GPIO_WritePin(OLED_CS3_GPIO_Port, OLED_CS3_Pin, GPIO_PIN_RESET);
		break;

	case 4:
		HAL_GPIO_WritePin(OLED_CS4_GPIO_Port, OLED_CS4_Pin, GPIO_PIN_RESET);
		break;
		
	default:
		break;
	}
}


/******************************************************************************
      ����˵��:	OLEDƬѡ ��1
      �������:	uint8_t WHICH_CS ѡ��Ƭѡ
      ����ֵ��  ��
	  ע��WHICH_CSΪ0ʱ��4��ͬʱ��1
******************************************************************************/
void OLED_CS_Set()
{
	switch (WHICH_CS)
	{
	case 0:
		HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(OLED_CS2_GPIO_Port, OLED_CS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(OLED_CS3_GPIO_Port, OLED_CS3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(OLED_CS4_GPIO_Port, OLED_CS4_Pin, GPIO_PIN_SET);
	case 1:
		HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_SET);
		break;
	
	case 2:
		HAL_GPIO_WritePin(OLED_CS2_GPIO_Port, OLED_CS2_Pin, GPIO_PIN_SET);
		break;

	case 3:
		HAL_GPIO_WritePin(OLED_CS3_GPIO_Port, OLED_CS3_Pin, GPIO_PIN_SET);
		break;

	case 4:
		HAL_GPIO_WritePin(OLED_CS4_GPIO_Port, OLED_CS4_Pin, GPIO_PIN_SET);
		break;

	default:
		break;
	}
}


/******************************************************************************
      ����˵��:OLEDд������
      �������:	dat д�������
				cmd д�����������:OLED_CMD(����)��OLED_DATA(����)
      ����ֵ��  ��
******************************************************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t * data = &dat;		  
	if(cmd){ //д����

		OLED_DC_Set();
		
		OLED_CS_Clr();
		HAL_SPI_Transmit(WHICH_SPI,(uint8_t *)data,1,1000); //�����ʵ������޸� 
		OLED_CS_Set();
	}
	  
	else{
	OLED_DC_Clr(); //д����

	OLED_CS_Clr();
	HAL_SPI_Transmit(WHICH_SPI,(uint8_t *)data,1,1000); //�����ʵ������޸�
	OLED_CS_Set(); 
	
	OLED_DC_Set(); 
	}
}


/******************************************************************************
      ����˵��:OLEDд��16λ����
      �������:	dat д�������
				cmd д�����������:OLED_DATA(����)
      ����ֵ��  ��
******************************************************************************/
void OLED_WR_16Byte(uint16_t dat,uint8_t cmd)
{		
	uint8_t data0 = (dat >> 8) & 0xFF;  // ��8λ
	uint8_t data1 = dat & 0xFF;         // ��8λ
	uint8_t * data0_spi = &data0;
	uint8_t * data1_spi = &data1;
	OLED_CS_Clr();
	HAL_SPI_Transmit(WHICH_SPI,(uint8_t *)data0_spi,1,1000); //�����ʵ������޸�
	HAL_SPI_Transmit(WHICH_SPI,(uint8_t *)data1_spi,1,1000); //�����ʵ������޸�
	OLED_CS_Set();
}




/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void OLED_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    OLED_WR_Byte(0x20, OLED_CMD);  // Specify the Horizontal Start Position of a Window for Written in Memory (MEMORY_ACCESSPOINTER X)
    OLED_WR_Byte(x1, OLED_DATA);
    OLED_WR_Byte(0x21, OLED_CMD);  // Specify the Vertical Start Position of a Window for Written in Memory (MEMORY_ACCESSPOINTER Y)
    OLED_WR_Byte(y1, OLED_DATA);
    OLED_WR_Byte(0x17, OLED_CMD);  // ����ʼ��ַ����
    OLED_WR_Byte(x1, OLED_DATA);
    OLED_WR_Byte(0x18, OLED_CMD);  // �н�����ַ����
    OLED_WR_Byte(x2, OLED_DATA);
    OLED_WR_Byte(0x19, OLED_CMD);  // ����ʼ��ַ����
    OLED_WR_Byte(y1, OLED_DATA);
    OLED_WR_Byte(0x1A, OLED_CMD);  // �н�����ַ����
    OLED_WR_Byte(y2, OLED_DATA);
    OLED_WR_Byte(0x22, OLED_CMD);  // ��ʼд��RAM
}


/******************************************************************************
      ����˵����OLED��ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void OLED_Init(void)
{
	

	OLED_RES_Set();
	HAL_Delay(20);	
	OLED_RES_Clr(); 
	HAL_Delay(20);
	OLED_RES_Set();
	HAL_Delay(120);
	
	OLED_WR_Byte(0x04, OLED_CMD);  // Set Power Save Mode (REDUCE_CURRENT)
	OLED_WR_Byte(0x01, OLED_DATA);
	HAL_Delay(10);
	OLED_WR_Byte(0x04, OLED_CMD);  // Set Power Save Mode (REDUCE_CURRENT)
	OLED_WR_Byte(0x00, OLED_DATA);

	OLED_WR_Byte(0x05, OLED_CMD);  // Software Reset (SOFT_RST)
	OLED_WR_Byte(0x00, OLED_DATA);

	OLED_WR_Byte(0x06, OLED_CMD);  // Set Display Off
	OLED_WR_Byte(0x00, OLED_DATA);

	OLED_WR_Byte(0x02, OLED_CMD);  // Set Oscillator Control (OSC_CTL)
	OLED_WR_Byte(0x01, OLED_DATA);

	OLED_WR_Byte(0x03, OLED_CMD);  // Set Display Frequency Divide Ratio / Oscillator Frequency (CLOCK_DIV)
	OLED_WR_Byte(0x30, OLED_DATA);

	OLED_WR_Byte(0x28, OLED_CMD);  // Display Duty Ratio (DUTY)
	OLED_WR_Byte(0x7F, OLED_DATA);

	OLED_WR_Byte(0x20, OLED_CMD);  // Specify the Horizontal Start Position of a Window for Written in Memory (MEMORY_ACCESSPOINTER X)
	OLED_WR_Byte(0x00, OLED_DATA);  // Default => 0x00
	OLED_WR_Byte(0x21, OLED_CMD);  // Specify the Vertical Start Position of a Window for Written in Memory (MEMORY_ACCESSPOINTER Y)
	OLED_WR_Byte(0x00, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x29, OLED_CMD);  // Set Display Start Line (DSL)
	OLED_WR_Byte(0x00, OLED_DATA);

	OLED_WR_Byte(0x14, OLED_CMD);  // Set Interface Mode (RGB_IF)
	OLED_WR_Byte(0x31, OLED_DATA);

	OLED_WR_Byte(0x15, OLED_CMD);  // Set RGB Interface Polarity (RGB_POL)
	OLED_WR_Byte(0x00, OLED_DATA);

	OLED_WR_Byte(0x13, OLED_CMD);  // Set Column Data Display Control / Re-Map Format (DISPLAY_MODE_SET)
	if (USE_HORIZONTAL) {
		OLED_WR_Byte(0x00, OLED_DATA);
	} 
	else {
		OLED_WR_Byte(0x30, OLED_DATA);
	}

	OLED_WR_Byte(0x16, OLED_CMD);  // Set Memory Access Control / Interface Pixel Format (MEMORY_WRITE_MODE)
	OLED_WR_Byte(0x66, OLED_DATA);


	//driving current r g b (uA)
	OLED_WR_Byte(0x10, OLED_CMD);  // Set Driving Current of Red (DRIVING_CURRENT_R)
	OLED_WR_Byte(0x2F, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x11, OLED_CMD);  // Set Driving Current of Green (DRIVING_CURRENT_G)
	OLED_WR_Byte(0x31, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x12, OLED_CMD);  // Set Driving Current of Blue (DRIVING_CURRENT_B)
	OLED_WR_Byte(0x1E, OLED_DATA);  // Default => 0x00


	// precharge time r g b
	OLED_WR_Byte(0x08, OLED_CMD);  // Set Pre-Charge Time of Red (PRECHARGE_TIME_R)
	OLED_WR_Byte(0x03, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x09, OLED_CMD);  // Set Pre-Charge Time of Green (PRECHARGE_TIME_G)
	OLED_WR_Byte(0x04, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x0A, OLED_CMD);  // Set Pre-Charge Time of Blue (PRECHARGE_TIME_B)
	OLED_WR_Byte(0x01, OLED_DATA);  // Default => 0x00


	// precharge current r g b
	OLED_WR_Byte(0x0B, OLED_CMD);  // Set Pre-Charge Current of Red (PRECHARGE_CURRENT_R)
	OLED_WR_Byte(0x1A, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x0C, OLED_CMD);  // Set Pre-Charge Current of Green (PRECHARGE_CURRENT_G)
	OLED_WR_Byte(0x19, OLED_DATA);  // Default => 0x00

	OLED_WR_Byte(0x0D, OLED_CMD);  // Set Pre-Charge Current of Blue (PRECHARGE_CURRENT_B)
	OLED_WR_Byte(0x0A, OLED_DATA);  // Default => 0x00


	OLED_WR_Byte(0x80, OLED_CMD);  // Control Reference Voltage Generation (IREF)
	OLED_WR_Byte(0x00, OLED_DATA);


	OLED_WR_Byte(0x06, OLED_CMD);  // Set Display On
	OLED_WR_Byte(0x01, OLED_DATA);
}
