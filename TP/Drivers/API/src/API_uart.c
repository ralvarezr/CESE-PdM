/*
 * API_uart.c
 *
 *  Created on: 22 nov. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"	/* Para las funciones de la HAL */
#include <stdio.h>			/* Para sprintf */
#include "stm32f4xx_hal_cortex.h" /* Para inicializar las interrupciones */

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

/*Se declara el Handler de la UART */
static UART_HandleTypeDef UartHandle;


/* Buffer de Recepción por Interrupción */
static uint8_t byte = 0;

//****************** FUNCIONES USADAS POR EL SISTEMA **********************

/* UART3 Interrupt Service Routine */
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}

/* Callback llamado por HAL_UART_IRQHandler cuando el número de bytes es recibido */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {

    /* Recibo un byte en modo interrupción */
    HAL_UART_Receive_IT(&UartHandle, &byte, 1);

  }
}

//****************************************************************************

bool_t uartInit(void)
{

	/* Se configuran los parámetros de la UART. */
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


	/* Inicializo la interrupcion */
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_UART_Receive_IT(&UartHandle, &byte, 1);

	return true;
}

bool_t uartSendString(uint8_t *pstring)
{
	/* En caso de que sea nulo el puntero devuelvo false */
	if((pstring == NULL))
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
	ret_val = HAL_UART_Transmit(&UartHandle, pstring, i, 100);
	if(ret_val != HAL_OK)
		return false;

	return true;
}

uint8_t uartGetBuffer(void)
{
	/* Devuelvo el valor del buffer y lo limpio */
	uint8_t retval = byte;

	byte = 0;

	return retval;
}
