#include "rs485.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

#define Master_Addr		0x81
#define Slave1_Addr		0x01
#define Slave2_Addr		0x02

uint8_t RS485_TX_BUF[3] = {0x5a, 0, 0x0d};
uint8_t RS485_RX_BUF[5] = {0xa5, 0x81, 0, 0, 0x0d};
uint8_t RS485_RX_TEMP;
/*
* 函数名：void RS485_Tx(uint8_t *pdata, uint16_t sz)
* 输入参数：pdata->指向发送数据所存储的首地址
sz->发送数据个数
* 输出参数：无
* 返回值：无
* 函数作用：USART2 的发送函数
*/
void RS485_Tx(uint8_t *pdata, uint16_t sz)
{
	RE_DE_TX();
	HAL_UART_Transmit(&huart2, pdata, sz, 100); 
//	
}

/*
* 函数名：void RS485_Rx(uint8_t *pdata, uint16_t sz)
* 输入参数：pdata->指向接收数据所存储的首地址
			sz->接收数据个数
* 输出参数：无
* 返回值：无
* 函数作用：USART2 的接收函数
*/
void RS485_Rx(uint8_t *pdata, uint16_t sz)
{
	HAL_UART_Receive_IT(&huart2, pdata, sz); 
}

void printf_float(uint16_t val)
{
	uint8_t tmp1, tmp2, tmp3;
	tmp1 = val / 1000;
	tmp2 = val % 1000 / 100;
	tmp3 = val % 100 / 10;
	printf("%d.%d%dV\r\n", tmp1, tmp2, tmp3);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint16_t val = 0, voltage;
	HAL_UART_Receive_IT(&huart2, RS485_RX_BUF, 5); 
	printf("%d %d %d %d %d\r\n", RS485_RX_BUF[0], RS485_RX_BUF[1], RS485_RX_BUF[2], RS485_RX_BUF[3], RS485_RX_BUF[4]);
	if(RS485_RX_BUF[1] != Master_Addr) //  || RS485_RX_BUF[0] != 0xa5
	{	
		memset(RS485_RX_BUF, 0, 5);
		return;
	}
	
	if(huart->Instance == USART2) 
	{
		printf("Receive From Slave %d: ", RS485_TX_BUF[1]);
		val = (uint16_t)((RS485_RX_BUF[2]<<8)|RS485_RX_BUF[3]);
		voltage = (uint16_t)((val*3.3/4096)*1000);
		printf_float(voltage);
		memset(RS485_RX_BUF, 0, 5);
		RE_DE_TX();
		
	} 
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint16_t cnt = 100;
	if(GPIO_Pin == KEY1_Pin) /* 查询从机1数据 */
	{
		RS485_TX_BUF[0] = 0x5a;
		RS485_TX_BUF[1] = Slave1_Addr; 
		RS485_TX_BUF[2] = 0x0d;
		
		RS485_Tx(RS485_TX_BUF, 3);
	}
	else if(GPIO_Pin == KEY2_Pin) /* 查询从机2数据 */
	{
		RS485_TX_BUF[0] = 0x5a;
		RS485_TX_BUF[1] = Slave2_Addr; 
		RS485_TX_BUF[2] = 0x0d;
		
		RS485_Tx(RS485_TX_BUF, 3);

	}
	while(cnt--);
	RE_DE_RX(); /* 发送完成设置为接收模式 */ 
}