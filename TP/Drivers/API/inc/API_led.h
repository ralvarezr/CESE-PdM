/*
 * API_led.h
 *
 *  Created on: 5 dic. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef API_LED_H_
#define API_LED_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_nucleo_144.h"	/* Para los enum de los Leds */
#include "API_delay.h"				/* Para el temporizador del led */

/* Redefinición de bool */
typedef bool bool_t;

/* Color de los leds */
typedef enum
{
	VERDE = LED_GREEN,
	AZUL = LED_BLUE,
	RED = LED_RED
}led_color_t;

/* Definición de la estructura led_t */
//typedef struct led_s led_t;
typedef struct led_s
{
	Led_TypeDef color;
	delay_t delay;
}led_t;



/*
 * Inicia el led asignando el color y el tiempo de parpadeo de 500ms por defecto.
 * Recibe el led a inicializar y el color.
 * Devuelve true en caso de éxito y false en caso de error.
 * */
void ledInit(led_t* led, Led_TypeDef color);

/*
 * Establece el tiempo de parpadeo del led.
 * Recibe el led y el tiempo a establecer.
 * */
void ledSetBlinkDuration(led_t* led, uint32_t time);

/*
 * Inicia el parpadeo del led.
 * Recibe el led.
 * */
void ledStartBlinking(led_t* led);

/*
 * Actualiza el estado led, apagándolo y encendiéndolo cuando corresponde.
 * Recibe el led.
 * */
void ledUpdate(led_t* led);

/*
 * Apaga el led.
 * */
void ledStopBlinking(led_t* led);


#endif /* API_LED_H_ */
