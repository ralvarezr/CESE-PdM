/*
 * API_debounce.c
 *
 *  Created on: 9 nov. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_debounce.h"
#include "API_delay.h"				/* Para usar el delay */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "stm32f4xx_hal.h"  		/* <- HAL include */

/* Nuevo tipo de datos enumerado llamado estadoMEF_t */
typedef enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_RISING,
	BUTTON_FALLING
} estadoMEF_t;

#define DURACION 40 	/* Se usa un tiempo de 40ms para el antirrebote */

/* Variables globales */
static estadoMEF_t estado_actual;
static delay_t tiempo;

/*
 * Togglea el LED1
 */
static void buttonPressed(void)
{
	BSP_LED_Toggle(LED1);
}

/*
 * Togglea el LED2
 */
static void buttonReleased(void)
{

	BSP_LED_Toggle(LED2);
}

/*
 * Inicializa el estado de la MEF,
 * el botón, los leds, y el delay para el antirrebote.
 * Devuelve true si todo sale bien, o false en caso de error.
 */
bool_t debounceInit(void)
{


	estado_actual = BUTTON_UP;						/* Asigno el estado Inicial de la MEF a BUTTON_UP */
	BSP_LED_Init(LED1);								/* Inicializo los leds LED1 y LED2*/
	BSP_LED_Init(LED2);
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);		/* Inicializo el BUTTON_USER*/
	delayInit(&tiempo, (tick_t) DURACION);			/* Inicializo el delay de 40ms */

	 return (true);
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
	switch (estado_actual)
	{
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER))       /* Si se detecta que el botón es presionado */
			{

				delayRead(&tiempo); 				/* Inicio el delay */
				estado_actual = BUTTON_FALLING;		/* Cambio de estado */

			}
			break;

		case BUTTON_FALLING:
			if(delayRead(&tiempo))					/* Si el delay finalizó */
			{
				if(BSP_PB_GetState(BUTTON_USER))	/* Verifico que el botón siga presionado */
				{
					buttonPressed();				/* Toggleo LED1 */
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

				delayRead(&tiempo); 					/* Inicio el delay */
				estado_actual = BUTTON_RISING;			/* Cambio de estado */

			}
			break;

		case BUTTON_RISING:
			if(delayRead(&tiempo))					/* Si el delay finalizó */
			{
				if(BSP_PB_GetState(BUTTON_USER) == 0)	/* Verifico que el botón siga suelto */
				{
					buttonReleased();					/* Toggleo LED2 */
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
