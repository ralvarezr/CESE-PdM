/*
 * API_leds_mef.c
 *
 *  Created on: 5 dic. 2021
 *      Author: Ricardo Alvarez
 */


#include "API_leds_mef.h"
#include "API_led.h"		/* Para usar los leds. */

/* Estados de la MEF */
typedef enum
{
	TODOS_APAGADOS,			/* Estado donde están todos los leds apagados */
	VERDE_ENCENDIDO,		/* Estado donde está el led verde encendido */
	AZUL_ENCENDIDO,			/* Estado donde está el led azul encendido */
	ROJO_ENCENDIDO,			/* Estado donde está el led rojo encendido */
	TODOS_ENCENDIDOS		/* Estado donde están todos los leds encendidos */
}leds_mef;


/* Declaración de los tres leds*/
static led_t verde;
static led_t rojo;
static led_t azul;

/* Declaración de la MEF */
static leds_mef estado_actual;



void ledsInit(void)
{
	ledInit(&verde, VERDE);
	ledInit(&rojo, ROJO);
	ledInit(&azul, AZUL);

	ledStopBlinking(&verde);
	ledStopBlinking(&azul);
	ledStopBlinking(&rojo);


	estado_actual = TODOS_APAGADOS;
}


void ledsUpdate(void)
{
	switch(estado_actual)
	{

		case TODOS_APAGADOS:
			break;

		case TODOS_ENCENDIDOS:
			ledUpdate(&verde);
			ledUpdate(&rojo);
			ledUpdate(&azul);
			break;


		case VERDE_ENCENDIDO:
			ledUpdate(&verde);
			break;


		case AZUL_ENCENDIDO:
			ledUpdate(&azul);
			break;

		case ROJO_ENCENDIDO:
			ledUpdate(&rojo);
			break;

		default:
			break;
	}
}



void startBlinkGreenLed(void)
{
	ledStopBlinking(&azul);
	ledStopBlinking(&rojo);
	ledStartBlinking(&verde);
	estado_actual = VERDE_ENCENDIDO;
}



void startBlinkRedLed(void)
{
	ledStopBlinking(&verde);
	ledStopBlinking(&azul);
	ledStartBlinking(&rojo);
	estado_actual = ROJO_ENCENDIDO;
}



void startBlinkBlueLed(void)
{
	ledStopBlinking(&verde);
	ledStopBlinking(&rojo);
	ledStartBlinking(&azul);
	estado_actual = AZUL_ENCENDIDO;
}


void startBlinkAllLeds(void)
{
	estado_actual = TODOS_ENCENDIDOS;
}

void stopBlinkAllLeds(void)
{

	ledStopBlinking(&verde);
	ledStopBlinking(&azul);
	ledStopBlinking(&rojo);
	estado_actual = TODOS_APAGADOS;
}

void setGreenLedBlinkTime(tick_t time)
{
	ledSetBlinkDuration(&verde, time);
}


void setRedLedBlinkTime(tick_t time)
{
	ledSetBlinkDuration(&rojo, time);
}


void setBlueLedBlinkTime(tick_t time)
{
	ledSetBlinkDuration(&azul, time);
}
