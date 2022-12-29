#include "rs485.h"
#include "usart.h"
#include "adc.h"
#include <stdio.h>
#include <string.h>




uint8_t RS485_RX_BUF[3] = {0};
uint8_t RS485_TX_BUF[5] = {0xa5,0x81,0,0,0x0d};
uint8_t RS485_RX_TEMP = 0;
uint8_t RS485_RX_CNT = 0;
uint8_t RS485_RX_FINISH = 0;
void RS485_Tx(uint8_t *pdata, uint16_t sz)
{
	uint16_t cnt = 100;
	RE_DE_TX();
//	while(cnt--);
//	cnt = 100;
	HAL_UART_Transmit(&huart2, pdata, sz, 100); 
	while(cnt--);
	RE_DE_RX(); /* 发送完成设置为接收模式 */ 
}


void RS485_Rx(uint8_t *pdata, uint16_t sz)
{
	HAL_UART_Receive_IT(&huart2, pdata, sz); 
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t flag = 0;
	
	HAL_UART_Receive_IT(&huart2, &RS485_RX_TEMP, 1); 
	if(RS485_RX_FINISH) return;
	
	if(RS485_RX_TEMP == 90)
	{
		flag = 1; 
		RS485_RX_CNT = 0;
	}
	
	if(flag)
	{
		RS485_RX_BUF[RS485_RX_CNT++] = RS485_RX_TEMP;
		if(RS485_RX_CNT == 3) 
		{
			flag = 0;
			RS485_RX_FINISH = 1;
		}
	}
}

uint16_t get_volatge(void)
{
	uint16_t val;
	HAL_ADC_Start(&hadc1);         /* 开启ADC转换 */
	if(HAL_OK == HAL_ADC_PollForConversion(&hadc1, 50)) /* 等待转换完成 */
	{
		val = HAL_ADC_GetValue(&hadc1); /* 获取ADC值 */
	}
	HAL_ADC_Stop(&hadc1); /* 关闭ADC转换 */
	return val;
}



//	RS485_Rx(RS485_RX_BUF, 3); 
//	printf("enter\r\n");
//	printf("%d %d %d\r\n", RS485_RX_BUF[0], RS485_RX_BUF[1], RS485_RX_BUF[2]);
//	if(RS485_RX_BUF[1] != Slave_Addr) //  || RS485_RX_BUF[0] != 0x5a
//	{	
//		memset(RS485_RX_BUF, 0, 3);
//		return;
//	}
//	
//	if(huart->Instance == USART2) 
//	{
//		memset(RS485_RX_BUF, 0, 3);
//		val = get_volatge();
//		printf("current adc value: %d\r\n", val);
//		RS485_TX_BUF[0] = 0xa5;
//		RS485_TX_BUF[1] = 0x81;
//		RS485_TX_BUF[2] = val >> 8;
//		RS485_TX_BUF[3] = val;
//		RS485_TX_BUF[4] = 0x0d;
//		RS485_Tx(RS485_TX_BUF, 5);	
//		
//	}

