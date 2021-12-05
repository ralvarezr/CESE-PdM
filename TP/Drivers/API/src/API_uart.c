/*
 * API_uart.c
 *
 *  Created on: 22 nov. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"	/* Para las funciones de la HAL */
#include <stdio.h>			/* Para sprintf */

/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

//Se declara el Handler de la UART.
static UART_HandleTypeDef UartHandle;

bool_t uartInit()
{

	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_ODD;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	/* Inicializo la UART y verifico que se haya inicializado correctamente. */
	if (HAL_UART_Init(&UartHandle) != HAL_OK)
		return false;

	/* Defino la cadena para imprimir el baud rate. */
	char buffer[30];
	sprintf(buffer, "BAUD RATE: %ld\n\r", UartHandle.Init.BaudRate);

	/* Imprimo la cadena. */
	uartSendString((uint8_t *)buffer);

	return true;
}

bool_t uartSendString(uint8_t *pstring)
{
	/* En caso de que sea nulo el puntero o el handle no haya sido inicializado, devuelvo false*/
	if((pstring == NULL) || (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY))
		return false;

	/* Busco la cantidad de bytes de la cadena, tiene un límite de hasta 100 bytes */
	uint16_t i = 0;
	for(; pstring[i]; i++)
	{
		if(i >= 100)
			return false;
	}

	HAL_StatusTypeDef ret_val;

	/* Trasmito el mensaje y verifico que se haya enviado correctamente. */
	ret_val = HAL_UART_Transmit(&UartHandle, pstring, i, 0xFFFF);
	if(ret_val != HAL_OK)
		return false;

	return true;
}

bool_t uartSendStringSize(uint8_t *pstring, uint16_t size)
{
	/* En caso de que sea nulo el puntero, el handle no haya sido inicializado, o el tamaño es menor o igual a 0, devuelvo false*/
	if((pstring == NULL) || (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY) || size <= 0)
		return false;

	HAL_StatusTypeDef ret_val;

	/* Trasmito el mensaje y verifico que se haya enviado correctamente. */
	ret_val = HAL_UART_Transmit(&UartHandle, pstring, size, 0xFFFF);
	if(ret_val != HAL_OK)
		return false;

	return true;
}
