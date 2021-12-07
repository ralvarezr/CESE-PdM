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
 * Inicializa la Interrupción por RX.
 *
 * Devuelve true si la inicialización fue exitosa, y false en caso contrario.
 * */
bool_t uartInit(void);

/*
 * Envía un el mensaje por la UART.
 * pstring es la cadena que se enviará.
 *
 * Devuelve true si se envío correctamente, o false en caso de error.
 * */
bool_t uartSendString(uint8_t *pstring);

/*
 * Devuelve el byte recbido por la UART.
 *  */
uint8_t uartGetBuffer(void);


#endif /* __API_UART_H_ */
