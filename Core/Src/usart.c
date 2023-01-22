/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>

static char Tx_Buffer[TX_BUFF_LEN];
static volatile uint8_t c[4]; // received char

volatile uint8_t Rx_flag = 0;

/******************************************************************************/

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/******************************************************************************
@brief	 	 Receives a char by UART serial port
******************************************************************************/

// set the interrupt for UART3 Rx
void Rx_UART_init(void)
{
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&c, 1);
}

//implementation of UART ISR
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	// if ((huart->Instance == USART3) && (huart->ErrorCode == HAL_UART_ERROR_NONE) && (huart->RxState == HAL_UART_STATE_READY))
	// if ((huart->Instance == USART3) && (huart->RxXferSize == 1))
	if (huart->Instance == USART3)
	{
		Rx_flag = 1;
	}
}

//implementation of UART ISR
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
//{
//	if (huart->Instance == USART3) //current UART?
//		Tx_flag = 1;
//}

/******************************************************************************
@brief
@param  	 Char received
******************************************************************************/
char UART_getchar(void)
{
	return c[0];
}

/******************************************************************************
@brief	 	 Sends a char by UART - Polling (Waits for UART_Tx to transmitt
							queued data
@param  	 Char to be transmitted
******************************************************************************/
void UART_putchar(char ch)
{
	// Waits for UART_Tx to transmit queued data
	while(huart3.gState == HAL_UART_STATE_BUSY_TX)
		;

	// 'ch' cannot be used to transmit since its local to this function
	c[0] = ch;
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)&c, 1);
}

/******************************************************************************
@brief	 	 Sends a string by UART - Polling (Waits for UART_Tx to transmitt
							queued data
@param  	 String to be transmitted
******************************************************************************/

void UART_puts(const char *s)
{
	// check for empty string
	if((s == NULL) || (s[0] == 0))
		return;

	int len = strlen(s);
	// string size bigger than the max size of Tx_Buffer?
	if(len > TX_BUFF_LEN)
		return;

	// Waits for UART_Tx to transmit queued data
	while(huart3.gState == HAL_UART_STATE_BUSY_TX)
		;

	strcpy(Tx_Buffer, s);
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)Tx_Buffer, len);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
