/*
 * menu.h
 *
 *  Created on: 6 dic. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef API_MENU_H_
#define API_MENU_H_

#include <stdbool.h>

typedef bool bool_t;

/*
 * Inicializa el menú de opciones.
 * Devuelve true en caso de éxito o false en caso de fallo.
 * */
bool_t menuInit(void);

/*
 * Actualiza el estado del menú de opciones.
 * */
void menuUpdate(void);




#endif /* API_MENU_H_ */
