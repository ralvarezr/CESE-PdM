/*
 * API_debounce.h
 *
 *  Created on: 9 nov. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef API_DEBOUNCE_H_
#define API_DEBOUNCE_H_

#include <stdbool.h>

typedef bool bool_t;


/*
 * Inicializa el antirebote.
 * */
void debounceInit(void);

/*
 * Actualiza el estado del antirrebote.
 * */
void debounceUpdate(void);

/*
 * Devuelve true si el botón está siendo presionado o false si está suelto.
 * */
bool_t buttonPressed(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
