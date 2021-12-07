/*
 * API_debounce.c
 *
 *  Created on: 9 nov. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_debounce.h"
#include "API_delay.h"				/* Para usar el delay */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */



/* MEF del antirrebote */
typedef enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_RISING,
	BUTTON_FALLING
} debounce_t;

#define DURACION_BOTON 	40 		/* Se usa un tiempo de 40ms para el antirrebote */


/* Variables globales */
static debounce_t estado_actual;	/* Estado de la MEF */
static delay_t delay_boton;			/* Delay para el antirrebote del botón */
static bool_t presionado = false;	/* Variable que indica si el botón está siendo presionado */


void debounceInit(void)
{


	estado_actual = BUTTON_UP;								/* Asigno el estado Inicial de la MEF a BUTTON_UP */

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);				/* Inicializo el BUTTON_USER*/
	delayInit(&delay_boton, (tick_t) DURACION_BOTON);		/* Inicializo el delay de 40ms para el botón */

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

				delayRead(&delay_boton); 			/* Inicio el delay */
				estado_actual = BUTTON_FALLING;		/* Cambio de estado */

			}
			break;

		case BUTTON_FALLING:
			if(delayRead(&delay_boton))				/* Si el delay finalizó */
			{
				if(BSP_PB_GetState(BUTTON_USER))	/* Si el botón sigue presionado */
				{
					presionado = true;				/* Indico que el botón está siendo presionado */
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
					presionado = false;					/* Indico que el botón ya no está siendo presionado */
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

bool_t buttonPressed(void)
{
	/* Se devuelve el valor de la variable presionado y se limpia la bandera para que sea
	 * leído su valor una única vez en caso de que el botón se mantega presionado.
	 * */

	bool_t ret_val = presionado;

	presionado = false;

	return ret_val;
}
