#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f7xx_hal.h>


uint Timer::TimeMS()
{
    return HAL_GetTick();
}


uint TimeMeter::ElapsedTime()
{
    return TIME_MS - time_reset;
}


void TimeMeter::Reset()
{
    time_reset = TIME_MS;
}


void Timer::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}
