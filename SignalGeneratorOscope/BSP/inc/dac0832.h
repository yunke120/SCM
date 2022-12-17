#ifndef __DAC0832_H__
#define __DAC0832_H__

#include "main.h"

typedef enum{
	Sin,	/* ���Ҳ� */
	Rect,	/* ���β�&���� */
}Wave;

typedef struct{
	Wave type;			/* ���� */
	float ampl;			/* ��ֵ */
	uint16_t freq;		/* Ƶ�� */
}wave_info_t;

void generate_wave2(uint8_t i, wave_info_t *info);

#endif

