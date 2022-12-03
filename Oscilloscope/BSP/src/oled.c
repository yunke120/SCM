#include "oled.h"
#include "oledfont.h" 
#include "math.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
uint8_t OLED_GRAM[128][8];
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
		OLED_SCLK_Set();
		OLED_SDIN_Set();
		OLED_SDIN_Clr();
		OLED_SCLK_Clr();
}
 
/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
    OLED_SCLK_Set();
		OLED_SDIN_Clr();
		OLED_SDIN_Set();
	
}
 
void IIC_Wait_Ack(void)
{
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/
 
void Write_IIC_Byte(uint8_t IIC_Byte)
{
		uint8_t i;
		uint8_t m,da;
		da=IIC_Byte;
		OLED_SCLK_Clr();
		for(i=0;i<8;i++)		
		{
			m=da;
			m=m&0x80;
			if(m==0x80)
					OLED_SDIN_Set();
			else 
					OLED_SDIN_Clr();
			da=da<<1;
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
		}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(uint8_t IIC_Command)
{
		 IIC_Start();
		 Write_IIC_Byte(0x78);            //Slave address,SA0=0
		 IIC_Wait_Ack();	
		 Write_IIC_Byte(0x00);			//write command
		 IIC_Wait_Ack();	
		 Write_IIC_Byte(IIC_Command); 
		 IIC_Wait_Ack();	
		 IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(uint8_t IIC_Data)
{
		 IIC_Start();
		 Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		 IIC_Wait_Ack();	
		 Write_IIC_Byte(0x40);			//write data
		 IIC_Wait_Ack();	
		 Write_IIC_Byte(IIC_Data);
		 IIC_Wait_Ack();	
		 IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
		if(cmd)
		{
				Write_IIC_Data(dat);  
		}
		else 
		{
				Write_IIC_Command(dat);		
		}
}
 
 
/********************************************
// fill_Picture
********************************************/
void fill_picture(uint8_t fill_Data)
{
		uint8_t m,n;
		for(m=0;m<8;m++)
		{
			OLED_WR_Byte(0xb0+m,0);		//page0-page1
			OLED_WR_Byte(0x02,0);		//low column start address
			OLED_WR_Byte(0x10,0);		//high column start address
			for(n=0;n<128;n++)
			{
					OLED_WR_Byte(fill_Data,1);
			}
		}
}
 
 
 
//�������� 
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  
 
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)     //�ô˺�������Ҫ������µ�LCD������OLED_Refresh_Gram();
{
		uint8_t pos,bx,temp=0;
		if(x>127||y>63)
			return;//������Χ��.
		pos = 7-y/8;            //�ڼ�ҳ  ��ʵ�Ƿ�������Ȼ��       Ȼ��ᷭת
		bx = y%8;
		temp = 1<<(7-bx);       //����ʹ��� 1 ������y��ƥ��
		if(t)
			OLED_GRAM[x][pos] |= temp;
		else 
			OLED_GRAM[x][pos] &= ~temp;	
}
 
//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
		uint8_t i,n;		    
		for(i=0;i<8;i++)  
		{  
				OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
				OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
				OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
				for(n=0;n<128;n++)
					OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); //������д��O_LED��GRAM��
		}   
}

void OLED_Clear_GRAM(void)
{
	memset(OLED_GRAM, 0, 128*8*sizeof(uint8_t));
}
 
//����
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2,uint16_t y2)//�ô˺�������Ҫ������µ�LCD������OLED_Refresh_Gram();
{
		uint16_t t; 
		int xerr=0,yerr=0,delta_x,delta_y,distance; 
		int incx,incy,uRow,uCol; 
		delta_x=x2-x1; //������������ 
		delta_y=y2-y1; 
		uRow=x1; 
		uCol=y1; 
		if(delta_x>0)
			incx=1; //���õ������� 
		else if(delta_x==0)
			incx=0;//��ֱ�� 
		else 
		{
			incx=-1;
			delta_x=-delta_x;
		} 
		if(delta_y>0)
			incy=1; 
		else if(delta_y==0)
			incy=0;//ˮƽ�� 
		else
		{
				incy=-1;
				delta_y=-delta_y;
		} 
		if(delta_x>delta_y)
				distance=delta_x; //ѡȡ�������������� 
		else 
				distance=delta_y; 
		for(t=0;t<=distance+1;t++ )//������� 
		{  
				OLED_DrawPoint(uRow,uCol,1);//���� 
				xerr+=delta_x ; 
				yerr+=delta_y ; 
				if(xerr>distance) 
				{ 
						xerr-=distance; 
						uRow+=incx; 
				} 
				if(yerr>distance) 
				{ 
						yerr-=distance; 
						uCol+=incy; 
				} 
		}  
} 
 
//��Բ
void draw_circle(uint8_t x0,uint8_t y0,uint8_t r) //Բ��(x0,y0),�뾶r
{
	uint8_t x;
	uint8_t y_old;
	for(x=x0-r;x<=x0+r;x++)
	{
			
			y_old=sqrt(pow(r,2)-pow(x-x0,2));  
			OLED_DrawPoint(x,y_old+y0,1);    //��Բ
			OLED_DrawPoint(x,y0-y_old,1);  //��Բ
	}
}
 
//д��ʵ��Բ��(x0,y0),�뾶r
void draw_fill_circle(uint8_t x0,uint8_t y0,uint8_t r,uint8_t dot)
{	
		uint8_t x = 0,y = 0,R = 0;
		for(x = x0-r;x <= x0+r;x++)
	  {
				for(y = y0-r; y <= y0+r ;y++ )
				{
						R = sqrt(pow(r,2)-pow(x-x0,2))+y0; //Բ����  x,y����		
						if( (y >= y0 && y <= R) || (y < y0 && y >= 2*y0-R )|| dot == 0 ) //�������� Բ������	
						{  
								OLED_DrawPoint(y,x,dot);
						}	
				}
		}
}
 
 
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
				OLED_WR_Byte(0,OLED_DATA);     //������ʾ
	} 
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
				OLED_WR_Byte(1,OLED_DATA);     //������ʾ
	} 
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
		unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1)
		{
				x=0;
				y=y+2;
		}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
					OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
					OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
					OLED_WR_Byte(F6x8[c][i],OLED_DATA);
			
		}
}
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
		uint32_t result=1;	 
		while(n--)result*=m;    
		return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else 
				enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120)
		{
			x=0;
			y+=2;
		}
		j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
		uint8_t t,adder=0;
		OLED_Set_Pos(x,y);	
		for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
		 }	
		OLED_Set_Pos(x,y+1);	
		for(t=0;t<16;t++)
		{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
		}					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
	 uint16_t j=0;
	 uint8_t x,y;
  
  if(y1%8==0) 
		y=y1/8;      
  else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	  {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	  }
	}
} 
 
//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
		GPIO_InitTypeDef GPIO_InitStruct = {0};
	 
		__HAL_RCC_GPIOB_CLK_ENABLE();
	 
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);//����SCL��SDA
		/*Configure GPIO pins : PB10 PB11 */
		GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//	HAL_Delay(100);
		OLED_WR_Byte(0xAE,OLED_CMD);//--display off
		OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
		OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
		OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
		OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
		OLED_WR_Byte(0x81,OLED_CMD); // contract control
		OLED_WR_Byte(0xFF,OLED_CMD);//--128   
		OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
		OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
		OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
		OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
		OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
		OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
		OLED_WR_Byte(0x00,OLED_CMD);//
		
		OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
		OLED_WR_Byte(0x80,OLED_CMD);//
		
		OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
		OLED_WR_Byte(0x05,OLED_CMD);//
		
		OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
		OLED_WR_Byte(0xF1,OLED_CMD);//
		
		OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
		OLED_WR_Byte(0x12,OLED_CMD);//
		
		OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
		OLED_WR_Byte(0x30,OLED_CMD);//
		
		OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
		OLED_WR_Byte(0x14,OLED_CMD);//
		
		OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}   

