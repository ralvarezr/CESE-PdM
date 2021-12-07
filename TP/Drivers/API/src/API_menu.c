/*
 * API_menu.c
 *
 *  Created on: 6 dic. 2021
 *      Author: Ricardo Alvarez
 */

#include "API_menu.h"
#include "API_uart.h" 		/* Para usar la UART */
#include "API_debounce.h"	/* Para usar el botón */
#include "API_leds_mef.h" 	/* Para usar los leds de la placa */

/* Estados del menú */
typedef enum
{
	MSJ_INICIO,				/* Estado de transición donde se muestra el mensaje al usuario */
	INICIO,					/* Estado de Inicio. Todos los leds están apagados */
	MSJ_VERDE,				/* Estado de transición donde se muestra el mensaje al usuario */
	VERDE,					/* Estado donde solo está encendido el led verde */
	MSJ_AZUL,				/* Estado de transición donde se muestra el mensaje al usuario */
	AZUL,					/* Estado donde solo está encendido el led azul */
	MSJ_ROJO,				/* Estado de transición donde se muestra el mensaje al usuario */
	ROJO,					/* Estado donde solo está encendido el led rojo */
	MSJ_TODOS,				/* Estado de transición donde se muestra el mensaje al usuario */
	TODOS					/* Estado donde están todos los leds encendidos */
}menu_t;


#define TIEMPO_100	100
#define TIEMPO_500	500
#define TIEMPO_1000 1000

/* Variables Globales */
static menu_t estado_actual;	/* Contiene el estado actual del menú */
static uint8_t byte;			/* Contiene el valor leido del buffer de la UART */


bool_t menuInit(void)
{

	ledsInit();					/* Inicializo los leds. */
	debounceInit();				/* Inicializo el botón. */
	if(uartInit() == false) 	/* Inicializo la UART y verifico si hubo algún fallo. */
	{
		return false;
	}

	/* Envío un mensaje por la UART. */
	uartSendString((uint8_t *)"Bienvenido!!!\n\r");

	/* Inicializo el estado del menú. */
	estado_actual = MSJ_INICIO;


	return true;

}


void menuUpdate(void)
{
	ledsUpdate();
	debounceUpdate();
	byte = uartGetBuffer();

	switch(estado_actual)
	{
		case MSJ_INICIO:

			/* Mando los mensajes al usuario */
			uartSendString((uint8_t *)"\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'v' para encender el led Verde\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'a' para encender el led Azul\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'r' para encender el led Rojo\n\r");
			uartSendString((uint8_t *)"- Presione el boton de la placa para encender todos los leds\n\r");

			/* Cambio al estado Inicio para esperar la entrada del usuario */
			estado_actual = INICIO;

			break;

		case INICIO:

			/* Verifico la entrada del usuario */
			if('v' == byte)
			{
				/* Si se presionó la tecla v, enciendo el led verde, envío el mensaje y cambio de estado */
				startBlinkGreenLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'v'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Verde!\n\r");

				estado_actual = MSJ_VERDE;
			}

			if('r' == byte)
			{

				/* Si se presionó la tecla r, enciendo el led rojo, envío el mensaje y cambio de estado */
				startBlinkRedLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'r'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Rojo!\n\r");

				estado_actual = MSJ_ROJO;
			}

			if('a' == byte)
			{

				/* Si se presionó la tecla a, enciendo el led azul, envío el mensaje y cambio de estado */
				startBlinkBlueLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'a'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Azul!\n\r");

				estado_actual = MSJ_AZUL;
			}

			if(buttonPressed())
			{

				/* Si se presionó el botón, enciendo todos los leds, envío el mensaje y cambio de estado */
				uartSendString((uint8_t *)"\n\rSe presiono el boton!\n\r");
				uartSendString((uint8_t *)"Se encenderan todos los leds!\n\r");

				estado_actual = MSJ_TODOS;

				startBlinkAllLeds();
			}
			break;

		case MSJ_VERDE:

			/* Mando los mensajes al usuario */
			uartSendString((uint8_t *)"\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'a' para encender el led Azul\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'r' para encender el led Rojo\n\r");
			uartSendString((uint8_t *)"- Presione el boton de la placa para encender todos los leds\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '1' para cambiar el tiempo de parpadeo del led Verde a 100ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '2' para cambiar el tiempo de parpadeo del led Verde a 500ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '3' para cambiar el tiempo de parpadeo del led Verde a 1000ms\n\r");

			/* Cambio al estado Inicio para esperar la entrada del usuario */
			estado_actual = VERDE;

			break;

		case VERDE:

			/* Verifico la entrada del usuario */

			if('r' == byte)
			{

				/* Si se presionó la tecla r, enciendo el led rojo, envío el mensaje y cambio de estado */
				startBlinkRedLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'r'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Rojo!\n\r");

				estado_actual = MSJ_ROJO;
			}

			if('a' == byte)
			{

				/* Si se presionó la tecla a, enciendo el led azul, envío el mensaje y cambio de estado */
				startBlinkBlueLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'a'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Azul!\n\r");

				estado_actual = MSJ_AZUL;
			}

			if('1' == byte)
			{
				/* Si se presionó la tecla 1 se cambia el tiempo de parpadeo del led a 100ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '1'!\n\r");
				uartSendString((uint8_t *)"El led Verde ahora parpadea a 100ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_100);

				estado_actual = MSJ_VERDE;
			}

			if('2' == byte)
			{
				/* Si se presionó la tecla 2 se cambia el tiempo de parpadeo del led a 500ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '2'!\n\r");
				uartSendString((uint8_t *)"El led Verde ahora parpadea a 500ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_500);

				estado_actual = MSJ_VERDE;
			}

			if('3' == byte)
			{
				/* Si se presionó la tecla 3 se cambia el tiempo de parpadeo del led a 1000ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '3'!\n\r");
				uartSendString((uint8_t *)"El led Verde ahora parpadea a 1000ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_1000);

				estado_actual = MSJ_VERDE;
			}

			if(buttonPressed())
			{

				/* Si se presionó el botón, enciendo todos los leds, envío el mensaje y cambio de estado */
				uartSendString((uint8_t *)"\n\rSe presiono el boton!\n\r");
				uartSendString((uint8_t *)"Se encenderan todos los leds!\n\r");

				estado_actual = MSJ_TODOS;

				startBlinkAllLeds();
			}

			break;

		case MSJ_ROJO:

			/* Mando los mensajes al usuario */
			uartSendString((uint8_t *)"\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'a' para encender el led Azul\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'v' para encender el led Verde\n\r");
			uartSendString((uint8_t *)"- Presione el boton de la placa para encender todos los leds\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '1' para cambiar el tiempo de parpadeo del led Rojo a 100ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '2' para cambiar el tiempo de parpadeo del led Rojo a 500ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '3' para cambiar el tiempo de parpadeo del led Rojo a 1000ms\n\r");

			/* Cambio al estado Inicio para esperar la entrada del usuario */
			estado_actual = ROJO;

			break;

		case ROJO:
			/* Verifico la entrada del usuario */

			if('v' == byte)
			{

				/* Si se presionó la tecla v, enciendo el led verde, envío el mensaje y cambio de estado */
				startBlinkGreenLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'v'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Verde!\n\r");

				estado_actual = MSJ_VERDE;
			}

			if('a' == byte)
			{

				/* Si se presionó la tecla a, enciendo el led azul, envío el mensaje y cambio de estado */
				startBlinkBlueLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'a'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Azul!\n\r");

				estado_actual = MSJ_AZUL;
			}

			if('1' == byte)
			{
				/* Si se presionó la tecla 1 se cambia el tiempo de parpadeo del led a 100ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '1'!\n\r");
				uartSendString((uint8_t *)"El led Rojo ahora parpadea a 100ms!\n\r");
				setRedLedBlinkTime(TIEMPO_100);

				estado_actual = MSJ_ROJO;
			}

			if('2' == byte)
			{
				/* Si se presionó la tecla 2 se cambia el tiempo de parpadeo del led a 500ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '2'!\n\r");
				uartSendString((uint8_t *)"El led Rojo ahora parpadea a 500ms!\n\r");

				setRedLedBlinkTime(TIEMPO_500);

				estado_actual = MSJ_ROJO;
			}

			if('3' == byte)
			{
				/* Si se presionó la tecla 3 se cambia el tiempo de parpadeo del led a 1000ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '3'!\n\r");
				uartSendString((uint8_t *)"El led Rojo ahora parpadea a 1000ms!\n\r");

				setRedLedBlinkTime(TIEMPO_1000);

				estado_actual = MSJ_ROJO;
			}

			if(buttonPressed())
			{

				/* Si se presionó el botón, enciendo todos los leds, envío el mensaje y cambio de estado */
				uartSendString((uint8_t *)"\n\rSe presiono el boton!\n\r");
				uartSendString((uint8_t *)"Se encenderan todos los leds!\n\r");

				estado_actual = MSJ_TODOS;

				startBlinkAllLeds();
			}

			break;

		case MSJ_AZUL:

			/* Mando los mensajes al usuario */
			uartSendString((uint8_t *)"\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'v' para encender el led Verde\n\r");
			uartSendString((uint8_t *)"- Envie la tecla 'r' para encender el led Rojo\n\r");
			uartSendString((uint8_t *)"- Presione el boton de la placa para encender todos los leds\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '1' para cambiar el tiempo de parpadeo del led Azul a 100ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '2' para cambiar el tiempo de parpadeo del led Azul a 500ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '3' para cambiar el tiempo de parpadeo del led Azul a 1000ms\n\r");

			/* Cambio al estado Inicio para esperar la entrada del usuario */
			estado_actual = AZUL;

			break;

		case AZUL:
			/* Verifico la entrada del usuario */

			if('r' == byte)
			{

				/* Si se presionó la tecla r, enciendo el led rojo, envío el mensaje y cambio de estado */
				startBlinkRedLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'r'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Rojo!\n\r");

				estado_actual = MSJ_ROJO;
			}

			if('v' == byte)
			{

				/* Si se presionó la tecla v, enciendo el led verde, envío el mensaje y cambio de estado */
				startBlinkGreenLed();

				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla 'v'!\n\r");
				uartSendString((uint8_t *)"Se encendera el led Verde!\n\r");

				estado_actual = MSJ_VERDE;
			}

			if('1' == byte)
			{
				/* Si se presionó la tecla 1 se cambia el tiempo de parpadeo del led a 100ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '1'!\n\r");
				uartSendString((uint8_t *)"El led Azul ahora parpadea a 100ms!\n\r");

				setBlueLedBlinkTime(TIEMPO_100);

				estado_actual = MSJ_AZUL;
			}

			if('2' == byte)
			{
				/* Si se presionó la tecla 2 se cambia el tiempo de parpadeo del led a 500ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '2'!\n\r");
				uartSendString((uint8_t *)"El led Azul ahora parpadea a 500ms!\n\r");

				setBlueLedBlinkTime(TIEMPO_500);

				estado_actual = MSJ_AZUL;
			}

			if('3' == byte)
			{
				/* Si se presionó la tecla 3 se cambia el tiempo de parpadeo del led a 1000ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '3'!\n\r");
				uartSendString((uint8_t *)"El led Azul ahora parpadea a 1000ms!\n\r");

				setBlueLedBlinkTime(TIEMPO_1000);

				estado_actual = MSJ_AZUL;
			}

			if(buttonPressed())
			{

				/* Si se presionó el botón, enciendo todos los leds, envío el mensaje y cambio de estado */
				uartSendString((uint8_t *)"\n\rSe presiono el boton!\n\r");
				uartSendString((uint8_t *)"Se encenderan todos los leds!\n\r");

				estado_actual = MSJ_TODOS;

				startBlinkAllLeds();
			}

			break;

		case MSJ_TODOS:

			/* Mando los mensajes al usuario */
			uartSendString((uint8_t *)"\n\r");
			uartSendString((uint8_t *)"- Presione el boton de la placa para apagar todos los leds\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '1' para cambiar el tiempo de parpadeo de todos los leds a 100ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '2' para cambiar el tiempo de parpadeo de todos los leds a 500ms\n\r");
			uartSendString((uint8_t *)"- Envie la tecla '3' para cambiar el tiempo de parpadeo de todos los leds a 1000ms\n\r");

			/* Cambio al estado Inicio para esperar la entrada del usuario */
			estado_actual = TODOS;

			break;

		case TODOS:

			/* Verifico la entrada del usuario */

			if('1' == byte)
			{
				/* Si se presionó la tecla 1 se cambia el tiempo de parpadeo de los leds a 100ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '1'!\n\r");
				uartSendString((uint8_t *)"Todos los leds ahora parpadean a 100ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_100);
				setRedLedBlinkTime(TIEMPO_100);
				setBlueLedBlinkTime(TIEMPO_100);

				estado_actual = MSJ_TODOS;
			}

			if('2' == byte)
			{
				/* Si se presionó la tecla 2 se cambia el tiempo de parpadeo de los leds a 500ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '2'!\n\r");
				uartSendString((uint8_t *)"Todos los leds ahora parpadean a 500ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_500);
				setRedLedBlinkTime(TIEMPO_500);
				setBlueLedBlinkTime(TIEMPO_500);

				estado_actual = MSJ_TODOS;
			}

			if('3' == byte)
			{
				/* Si se presionó la tecla 3 se cambia el tiempo de parpadeo de los leds a 1000ms */
				uartSendString((uint8_t *)"\n\rSe ha recibido la tecla '3'!\n\r");
				uartSendString((uint8_t *)"Todos los leds ahora parpadean a 1000ms!\n\r");

				setGreenLedBlinkTime(TIEMPO_1000);
				setRedLedBlinkTime(TIEMPO_1000);
				setBlueLedBlinkTime(TIEMPO_1000);

				estado_actual = MSJ_TODOS;
			}

			if(buttonPressed())
			{

				/* Si se presionó el botón, apago todos los leds, envío el mensaje y cambio de estado */
				uartSendString((uint8_t *)"\n\rSe presiono el boton!\n\r");
				uartSendString((uint8_t *)"Todos los leds se apagaran!\n\r");

				estado_actual = MSJ_INICIO;

				stopBlinkAllLeds();
			}

			break;

		default:
			break;

	}



}
