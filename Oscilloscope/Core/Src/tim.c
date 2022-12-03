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
#include <stdio.h>
#include <string.h>
#include "oled.h"
#include "usart.h"
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM2)
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
}

/* USER CODE BEGIN 1 */

#define DATALEN	16 /* 修改这个值可以改变屏幕显示的曲线区域 */
volatile uint16_t raw = 0; /* ADC原始数据 */
uint8_t yBuf[DATALEN]; /* 存储y坐标值 */

/* 数据移位 */
static void shift(uint8_t *buf, uint8_t len, uint8_t data)
{
//	char yybuf[32]={0};
	
	int i;
	for(i = 0; i < len-1; i++)
	{
		buf[i] = buf[i+1];
	}
	buf[i] = data;

//	snprintf(yybuf, sizeof(yybuf), "%d %d %d %d %d %d %d %d \r\n", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
//	HAL_UART_Transmit(&huart1, (uint8_t *)yybuf, sizeof(yybuf), 100);
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
//	LCD_DrawLine(63,0, 63,63);
//	LCD_DrawLine(0,32, 127,32);
//	LCD_DrawLine(124,35, 127,32);
//	LCD_DrawLine(124,29, 127,32);
//	LCD_DrawLine(60,60, 63,63);
//	LCD_DrawLine(66,60, 63,63);
	
	/* 画曲线 */
	for(i = 0; i < len-1; i++)
	{
		LCD_DrawLine(xStart + i * span, buf[i], xStart + (i+1) * span, buf[i+1]);
	}
	OLED_Refresh_Gram();
}


//重写TIM2中断调用函数 10HZ
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int val;
	char buf[32] = {0};
	uint8_t y = 0;
    if((htim->Instance) == (htim2.Instance))
    {          
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		raw = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		
		y = raw >> 6; // 相当于 raw / 4095 * 63 ,使用位操作计算速度更快

		shift(yBuf, DATALEN, y);
		show(yBuf, DATALEN);
			
		/* 打印电压值，会影响采样 */
//		val = (int)((3.3 * 1000 * raw) / 4095.0); // Proteus不能打印浮点数,转换成毫伏进行显示
//		snprintf(buf, sizeof(buf), "voltage: %d mv \r\n", val);
//		HAL_UART_Transmit(&huart1, (uint8_t *)buf, sizeof(buf), 100);
    }

}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
