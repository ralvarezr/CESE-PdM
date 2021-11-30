/*
 * API_uart.h
 *
 *  Created on: 22 nov. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef __API_UART_H_
#define __API_UART_H_

#include <stdbool.h>
#include <stdint.h>		/* Para uint8_t y uint16_t */

typedef bool bool_t;


/*
 * Inicializa la UART con los siguiente parámetros:
 * - Baud Rate: 9600
 * - Word Length: 8bits
 * - Stop Bits: 1bit
 * - Parity: Odd
 * - HW Flow Control: None
 *
 * Imprime por la UART el baud rate luego de ser inicializada.
 *
 * Devuelve true si la inicialización fue exitosa, y false en caso contrario.
 * */
bool_t uartInit();

/*
 * Envía un el mensaje por la UART.
 * pstring es la cadena que se enviará.
 *
 * Devuelve true si se envío correctamente, o false en caso de error.
 * */
bool_t uartSendString(uint8_t *pstring);

/*
 * Envía un el mensaje por la UART.
 * pstring es la cadena que se enviará.
 * size es el tamaño de la cadena.
 *
 * Devuelve true si se envío correctamente, o false en caso de error.
 * */
bool_t uartSendStringSize(uint8_t *pstring, uint16_t size);

#endif /* __API_UART_H_ */
