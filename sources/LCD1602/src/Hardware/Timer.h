#pragma once


#define TIME_MS Timer::TimeMS()


namespace Timer
{
    uint TimeMS();
    
    void Delay(uint timeMS);
}


class TimeMeter
{
    uint time_reset;
public:
    TimeMeter() { Reset(); }

    uint ElapsedTime();

    void Reset();
};
