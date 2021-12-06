/*
 * API_led.c
 *
 *  Created on: 5 dic. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_led.h"




/*
 * Estructura interna led_s
 * Contiene el color de led y su delay correspondiente.
 * */
/*struct led_s
{
	Led_TypeDef color;
	delay_t delay;
};*/



void ledInit(led_t* self, Led_TypeDef color)
{
	/* Establezco el color del led*/
	self->color = color;

	/* Inicializo el led */
	BSP_LED_Init(self->color);

	/* Inicializo el delay de led en 500ms */
	delayInit(&(self->delay), (tick_t)500);

	/* Enciendo el led */
	BSP_LED_On(self->color);

}


void ledSetBlinkDuration(led_t* self, tick_t time)
{
	/* Sobreescribo el tiempo del delay. */
	delayWrite(&(self->delay), time);
}


void ledStartBlinking(led_t* self)
{
	/* Hago una primera lectura del tiempo para activar el conteo. */
	delayRead(&(self->delay));
}


void ledUpdate(led_t* self)
{
	/* Leo si ya el temporizador llegó al tiempo */
	if(delayRead(&(self->delay)))
			BSP_LED_Toggle(self->color);
}


void ledStopBlinking(led_t* self)
{
	/* Apago el led */
	BSP_LED_Off(self->color);
	/* Detengo el delay del led*/
	delayStop(&(self->delay));

}


