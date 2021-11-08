#include "API_delay.h"



void delayInit( delay_t * delay, tick_t duration )
{
    delay->duration = duration;
    delay->running = false;
}

bool_t delayRead( delay_t * delay )
{
    bool_t retVal = false;

    if(!(delay->running))
    {
        delay->startTime = HAL_GetTick();
        delay->running = true;

    }else if((HAL_GetTick() - delay->startTime) >= delay->duration)
    {
        delay->running = false;
        retVal = true;
    }

    return retVal;
}

void delayWrite( delay_t * delay, tick_t duration )
{
    delay->duration = duration;
}