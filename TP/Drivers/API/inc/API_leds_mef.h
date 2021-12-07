/*
 * API_leds_mef.h
 *
 *  Created on: 5 dic. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef API_LEDS_MEF_H_
#define API_LEDS_MEF_H_

#include <stdint.h>

typedef uint32_t tick_t;


/*
 * Inicializa los leds con un tiempo predeterminado de 500ms.
 * */
void ledsInit(void);

/*
 * Actualiza el estado de los leds.
 * */
void ledsUpdate(void);

/*
 * Establece el tiempo de parpadeo del Led Verde.
 * */
void setGreenLedBlinkTime(tick_t time);

/*
 * Establece el tiempo de parpadeo del Led Rojo.
 * */
void setRedLedBlinkTime(tick_t time);

/*
 * Estable el tiempo de parpadeo del Led Azul.
 * */
void setBlueLedBlinkTime(tick_t time);

/*
 * Inicia el parpadeo del Led Verde.
 * */
void startBlinkGreenLed(void);


/*
 * Inicia el parpadeo del Led Rojo.
 * */
void startBlinkRedLed(void);


/*
 * Inicia el parpadeo del Led Azul.
 * */
void startBlinkBlueLed(void);


/*
 * Inicia el parpadeo de todos los Leds.
 * */
void startBlinkAllLeds(void);

/*
 * Apaga todos los leds
 * */
void stopBlinkAllLeds(void);


#endif /* API_LEDS_MEF_H_ */
