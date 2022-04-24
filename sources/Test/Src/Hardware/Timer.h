#pragma once


#define TIME_MS Timer::TimeMS()


namespace Timer
{
    uint TimeMS();
}


class TimeMeter
{
    uint time_reset;
public:
    TimeMeter() { Reset(); }

    uint ElapsedTime();

    void Reset();
};
