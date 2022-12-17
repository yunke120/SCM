#ifndef __OLED_H
#define __OLED_H	 

#include "main.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64


#define OLED_SCLK_Set() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)

#define OLED_SDIN_Set() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
 

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_GRAM(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);               //��ָ��λ����ʾһ���ַ�,���������ַ�
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);        //��ʾ2������
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	           //��ʾһ���ַ��Ŵ�
void OLED_Set_Pos(uint8_t x, uint8_t y);                                 //��������
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);                //���� 
void OLED_Refresh_Gram(void);                                                        //�����Դ浽LCD	
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2,uint16_t y2);//����
void draw_circle(uint8_t x0,uint8_t y0,uint8_t r);                                   //Բ��(x0,y0),�뾶r
void draw_fill_circle(uint8_t x0,uint8_t y0,uint8_t r,uint8_t dot);                  //д��ʵ��Բ��(x0,y0),�뾶r
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);                               //��ʾ����
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);//��ʾBMPͼƬ
void fill_picture(uint8_t fill_Data);

void OLED_Shift(uint8_t data);

void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(uint8_t IIC_Command);
void Write_IIC_Data(uint8_t IIC_Data);
void Write_IIC_Byte(uint8_t IIC_Byte);
 
void IIC_Wait_Ack(void);

#endif

