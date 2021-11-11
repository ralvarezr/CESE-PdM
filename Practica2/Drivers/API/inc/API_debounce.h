/*
 * API_debounce.h
 *
 *  Created on: 9 nov. 2021
 *      Author: Ricardo Alvarez
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"	/* Para bool_t */


// Prototipos de funciones
bool_t debounceInit(void);
void debounceUpdate(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
