#ifndef __RS485_H__
#define __RS485_H__

#include "main.h"

// �����Ÿߵ�ƽ�Ƿ�����Ч������Ч���͵�ƽʱ������Ч������Ч
#define RE_DE_TX() HAL_GPIO_WritePin(RS485_CTRL_GPIO_Port, RS485_CTRL_Pin, GPIO_PIN_SET)
#define RE_DE_RX() HAL_GPIO_WritePin(RS485_CTRL_GPIO_Port, RS485_CTRL_Pin, GPIO_PIN_RESET)



#endif