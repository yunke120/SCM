#include "key.h"

/*
	°´¼üÉ¨Ãèº¯Êý
*/
uint8_t key_scan(void)
{
	static uint8_t key = 1;
	if((key == 1) && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0))
	{
		key = 0;
	
		if(KEY0 == 0)
			return KEY0_VALUE;
		else if(KEY1 == 0)
			return KEY1_VALUE;
		else if(KEY2 == 0)
			return KEY2_VALUE;
	}
	else if(KEY0 == 1 && KEY2 == 1 && KEY1 == 1) 
		key = 1;
	
	return 0; 
}



