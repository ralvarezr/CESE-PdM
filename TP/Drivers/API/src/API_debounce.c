/*
 * API_debounce.c
 *
 *  Created on: 9 nov. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_debounce.h"
#include "API_delay.h"				/* Para usar el delay */
#include "API_uart.h"				/* Para usar la UART */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */



/* Nuevo tipo de datos enumerado llamado estadoMEF_t */
typedef enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_RISING,
	BUTTON_FALLING
} estadoMEF_t;

#define DURACION_BOTON 	40 		/* Se usa un tiempo de 40ms para el antirrebote */
#define DURACION_100 	100 	/* Tiempo de 100ms */
#define DURACION_500 	500 	/* Tiempo de 500ms */
#define DURACION_1000 	1000 	/* Tiempo de 1000ms */

/* Variables globales */
static estadoMEF_t estado_actual;	/* Estado de la MEF */
static delay_t delay_boton;			/* Delay para el antirrebote del botón */
static delay_t delay_led1;			/* Delay para el encendido y apagado del LED1 */
static delay_t delay_led2;			/* Delay para el encendido y apagado del LED2 */
static delay_t delay_led3;			/* Delay para el encendido y apagado del LED3 */
static bool_t sequence;				/* Indica la secuencia en la que se encienden los leds. */


/*
 * Togglea los leds si se alcanzó el tiempo del delay correspondiente.
 */
static void toggleLeds(void)
{
	if(delayRead(&delay_led1))
		BSP_LED_Toggle(LED1);

	if(delayRead(&delay_led2))
		BSP_LED_Toggle(LED2);

	if(delayRead(&delay_led3))
		BSP_LED_Toggle(LED3);

}

/*
 * Cuando el botón es presionado, cambia el tiempo
 * en el que los leds se encienden y apagan.
 * La secuencia inicial es:
 * LED1 = 100ms, LED2 = 500ms, LED3 = 1000ms.
 * La secuencia alternativa es:
 * LED1 = 500ms, LED2 = 500ms, LED3 = 500ms.
 */
static void buttonPressed(void)
{
	sequence = !sequence;

	if(sequence)
	{

		delayWrite(&delay_led1, DURACION_100);
		delayWrite(&delay_led2, DURACION_500);
		delayWrite(&delay_led3, DURACION_1000);

		/* Escribo por la UART los tiempos de los Leds */
		uartSendString((uint8_t *) "Led 1: 100ms\n\r");
		uartSendString((uint8_t *) "Led 2: 500ms\n\r");
		uartSendString((uint8_t *) "Led 3: 1000ms\n\r");



	}
	else
	{
		delayWrite(&delay_led1, DURACION_500);
		delayWrite(&delay_led2, DURACION_500);
		delayWrite(&delay_led3, DURACION_500);

		/* Escribo por la UART los tiempos de los Leds */
		uartSendString((uint8_t *) "Led 1: 500ms\n\r");
		uartSendString((uint8_t *) "Led 2: 500ms\n\r");
		uartSendString((uint8_t *) "Led 3: 500ms\n\r");

	}

	/* Imprimo una línea en blanco */
	uartSendString((uint8_t *) "\n\r");
}



/*
 * Inicializa el estado de la MEF,
 * el botón, los leds, el delay para el antirrebote,
 * y los delays de los leds.
 * Devuelve true si todo sale bien, o false en caso de error.
 */
bool_t debounceInit(void)
{

	bool_t ret_val = true;									/* Declaro el valor de retorno */

	estado_actual = BUTTON_UP;								/* Asigno el estado Inicial de la MEF a BUTTON_UP */
	BSP_LED_Init(LED1);										/* Inicializo los leds LED1, LED2 y LED3 */
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);				/* Inicializo el BUTTON_USER*/
	delayInit(&delay_boton, (tick_t) DURACION_BOTON);		/* Inicializo el delay de 40ms para el botón */
	delayInit(&delay_led1, (tick_t) DURACION_100);			/* Inicializo el delay de 100ms para el LED1 */
	delayInit(&delay_led2, (tick_t) DURACION_500);			/* Inicializo el delay de 500ms para el LED2 */
	delayInit(&delay_led3, (tick_t) DURACION_1000);			/* Inicializo el delay de 1000ms para el LED3 */

	delayRead(&delay_led1);									/* Inicio el delay de cada led */
	delayRead(&delay_led2);
	delayRead(&delay_led3);

	sequence = true;										/* Inicializo el estado de la secuencia de encendido de los leds */


	ret_val = uartInit();									/* Inicializo la UART */
	if(ret_val)
	{
		/* Escribo por la UART los tiempos de los Leds */
		uartSendString((uint8_t *) "Led 1: 100ms\n\r");
		uartSendString((uint8_t *) "Led 2: 500ms\n\r");
		uartSendString((uint8_t *) "Led 3: 1000ms\n\r");

		/* Imprimo una línea en blanco */
		uartSendString((uint8_t *) "\n\r");
	}



	 return (ret_val);
}


/*
 * Actualiza el estado de la MEF.
 *
 * En caso de estar en BUTTON_UP o BUTTON_DOWN
 * verifico si el botón fue presionado o suelto respectivamente,
 * inicio un delay de 40ms de duración, y cambio de estado.
 *
 * En caso de estar en BUTTON_FALLING o BUTTON_RISIING,
 * se verifica que el delay haya terminado, y se verifica
 * el estado de botón, si sigue presionado o suelto
 * se cambia de estado a BUTTON_DOWN o BUTTON_UP respectivamente,
 * y se togglea el LED correspondiente.
 *
 */
void debounceUpdate(void)
{
	toggleLeds();								   /* Toggleo los leds */

	switch (estado_actual)
	{
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER))       /* Si se detecta que el botón es presionado */
			{

				delayRead(&delay_boton); 			/* Inicio el delay */
				estado_actual = BUTTON_FALLING;		/* Cambio de estado */

			}
			break;

		case BUTTON_FALLING:
			if(delayRead(&delay_boton))				/* Si el delay finalizó */
			{
				if(BSP_PB_GetState(BUTTON_USER))	/* Si el botón sigue presionado */
				{
					buttonPressed();				/* Cambio el delay de los leds */
					estado_actual = BUTTON_DOWN;	/* Actualizo el estado */
				}
				else								/* Si no sigue presionado el botón */
				{
					estado_actual = BUTTON_UP;		/* Vuelvo al estado anterior */
				}
			}

			break;

		case BUTTON_DOWN:
			if(BSP_PB_GetState(BUTTON_USER) == 0)		/* Si se detecta que el botón es soltado */
			{

				delayRead(&delay_boton); 				/* Inicio el delay */
				estado_actual = BUTTON_RISING;			/* Cambio de estado */

			}
			break;

		case BUTTON_RISING:
			if(delayRead(&delay_boton))					/* Si el delay finalizó */
			{
				if(BSP_PB_GetState(BUTTON_USER) == 0)	/* Si el botón sigue suelto */
				{
					estado_actual = BUTTON_UP;			/* Actualizo el estado */
				}
				else									/* Si no sigue suelto el botón */
				{
					estado_actual = BUTTON_DOWN;		/* Vuelvo al estado anterior */
				}
			}

			break;

		default:
			break;

	}
}
