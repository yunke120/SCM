#ifndef __DAC0832_H__
#define __DAC0832_H__

#include "main.h"

typedef enum{
	Sin,	/* 正弦波 */
	Rect,	/* 矩形波&方波 */
}Wave;

typedef struct{
	Wave type;			/* 类型 */
	float ampl;			/* 幅值 */
	uint16_t freq;		/* 频率 */
}wave_info_t;

void generate_wave2(uint8_t i, wave_info_t *info);

#endif

