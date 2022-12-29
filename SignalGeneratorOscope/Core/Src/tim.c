/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "dac0832.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
wave_info_t wave_info = {
	.type = Sin,
	.ampl = 3.0,
	.freq = 100
};

#define CalcCounter		0.341553 // ((1405-6)>>12)
#define CalcFreq		4.859585 // ((20000-100)>>12)

#define DATALEN	64 /* 修改这个值可以改变屏幕显示的曲线区域 */
uint8_t yBuf[DATALEN]; /* 存储y坐标值 */

uint16_t ADCBuf[RecvLen][ChannelNum] = {0}; /* 三路ADC值存储区 */
uint8_t RecvFlag = 0; 				/* DMA接收完成标志 */
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 719;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 3599;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* 数据移位 */
static void shift(uint8_t *buf, uint8_t len, uint8_t data)
{
	int i;
	for(i = 0; i < len-1; i++)
	{
		buf[i] = buf[i+1];
	}
	buf[i] = data;
}

/* 显示到OLED */
static void show(uint8_t *buf, uint8_t len)
{
	uint8_t i;
	uint8_t span, xStart;
	span = X_WIDTH / len;
	xStart = (X_WIDTH % len) >> 1;
	
	OLED_Clear_GRAM(); /* OLED_Clear()函数不能清除使用了Gram的函数 */

	/* 画坐标系 */
//	LCD_DrawLine(0,0, 0,63);
//	LCD_DrawLine(0,0, 127,0);
//	LCD_DrawLine(0,63, 2,60);
//	LCD_DrawLine(127,0, 124,2);
	
	/* 画曲线 */
	for(i = 0; i < len-1; i++)
	{
//		OLED_DrawPoint(xStart + i * span, buf[i], 1);
		LCD_DrawLine(xStart + i * span, buf[i], xStart + (i+1) * span, buf[i+1]);
	}
	OLED_Refresh_Gram();
}
/**
 * @brief 定时器中断函数
 *        TIM2用来产生波形
 *        
 * 
 * @param htim 
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	char buf[32] = {0};
	uint16_t i, cnt = 0;
	uint16_t counter;
	uint32_t adc_freq_sum, adc_ampl_sum, adc_signal_sum;
	float temp;
	uint8_t y = 0;
	static uint8_t index; /* 256个数据的索引 */
	if(htim->Instance == TIM2)
	{
		generate_wave2(index, &wave_info);
		index++; // index 为uint8_t 类型，最大值为255，当其自增到255后，会默认从0开始计数，这里无需进行处理
	}
	else if(htim->Instance == TIM3) // 100ms中断一次
	{
	  if(RecvFlag)
	  {
		  RecvFlag = 0;
		  adc_freq_sum = 0;
		  adc_ampl_sum = 0;
		 	  
		  for(i = 0; i < RecvLen; i++)
		  {
			  adc_freq_sum += ADCBuf[i][0];
			  adc_ampl_sum += ADCBuf[i][1];
			  adc_signal_sum += ADCBuf[i][2];
		  }
		  memset(ADCBuf, 0, sizeof(ADCBuf));
		  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCBuf, ChannelNum*RecvLen);
			  y = (adc_signal_sum/RecvLen) >> 6; // 相当于 raw / 4095 * 63 ,使用位操作计算速度更快
			  shift(yBuf, DATALEN, y);
			  show(yBuf, DATALEN);
		  
//		  printf("%d %d %d\r\n", ADCBuf[0][2], ADCBuf[1][2], ADCBuf[2][2]); /* 第一个接收的数据是错的 */
//		  printf("ampl: %d\r\n", adc_ampl_sum / RecvLen);
//		  printf("freq: %d\r\n", adc_freq_sum / RecvLen);
		  temp = CalcCounter * (adc_freq_sum / RecvLen);
		  counter = (uint16_t)temp;
		  __HAL_TIM_SET_AUTORELOAD(&htim2, counter); /* 重新设置计数值 */
		  
		  temp = CalcFreq * (adc_freq_sum / RecvLen);
		  wave_info.freq = 20000-200-(uint16_t)temp;
		  
		  wave_info.ampl = 3.3*(adc_ampl_sum / RecvLen)/4095.0;
		  
		  
	  }	
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
