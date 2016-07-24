#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#ifdef QTSIMU
#include <stdint.h>
#endif

#include "stopwatchutils.hpp"

struct StopwatchTime
{
    uint8_t minutes;
    uint8_t seconds;
    uint16_t mseconds;
};

struct StopWatch
{
    StopwatchTime time;
    bool running;
};

void onTickStopwatch(uint16_t ms);
bool onButtonStopwatch(ButtonId btnId, BtnState buttonState);
void updateDisplayStopwatch();

#endif // STOPWATCH_HPP
