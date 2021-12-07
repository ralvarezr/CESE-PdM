#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include <assert.h>

void delayInit(delay_t * delay, tick_t duration)
{
	assert(delay != NULL);
	assert(duration >= 0);

    delay->duration = duration;
    delay->running = false;
}

bool_t delayRead(delay_t * delay)
{
    assert(delay != NULL);

	bool_t ret_val = false;

    if(!(delay->running))
    {
        delay->startTime = HAL_GetTick();
        delay->running = true;

    }else if((HAL_GetTick() - delay->startTime) >= delay->duration)
    {
        delay->running = false;
        ret_val = true;
    }

    return ret_val;
}

void delayWrite(delay_t * delay, tick_t duration)
{

	assert(delay != NULL);
	assert(duration >= 0);

	delay->duration = duration;
}

void delayStop(delay_t* delay)
{
	assert(delay != NULL);

	delay->running = false;
}
