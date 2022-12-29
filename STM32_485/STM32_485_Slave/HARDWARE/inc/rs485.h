#ifndef __RS485_H__
#define __RS485_H__

#include "main.h"

#define Slave_Addr		0x01

// 此引脚高电平是发送有效接收无效；低电平时接收有效发送无效
#define RE_DE_TX() HAL_GPIO_WritePin(RS485_CTRL_GPIO_Port, RS485_CTRL_Pin, GPIO_PIN_SET)
#define RE_DE_RX() HAL_GPIO_WritePin(RS485_CTRL_GPIO_Port, RS485_CTRL_Pin, GPIO_PIN_RESET)

uint16_t get_volatge(void);
void RS485_Tx(uint8_t *pdata, uint16_t sz);
#endif

