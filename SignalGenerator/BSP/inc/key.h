#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

#define KEY0_VALUE	 	1
#define KEY1_VALUE 		2
#define KEY2_VALUE 		3

#define KEY0 	HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)
#define KEY1 	HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define KEY2 	HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)


uint8_t key_scan(void);
	


#endif



