#include "stopwatch.hpp"
#include "7seg-display.hpp"

StopWatch stp = {
    .time = {
        .minutes = 0,
        .seconds = 0,
        .mseconds = 0
    },
    .running = false
};

static void updateCurrentTime(uint16_t ms);
static bool handleBtnReset(BtnState);
static bool handleBtnStartStop(BtnState);
static bool handleBtnMode(BtnState);
static bool handleBtnSetup(BtnState);

void onTickStopwatch(uint16_t ms)
{
    if (!stp.running)
        return;

    updateCurrentTime(ms);
}

void updateDisplayStopwatch()
{
    setDisplayDigit(0,getDigit(stp.time.minutes,1));
    setDisplayDigit(1,getDigit(stp.time.minutes,0));
    setDisplayDigit(2,getDigit(stp.time.seconds,1));
    setDisplayDigit(3,getDigit(stp.time.seconds,0));
}

void updateCurrentTime(uint16_t ms)
{
    stp.time.mseconds += ms;
    if (stp.time.mseconds >= 1000)
    {
        stp.time.seconds += stp.time.mseconds/1000;
        stp.time.mseconds = stp.time.mseconds%1000;
        if (stp.time.seconds>=60)
        {
            stp.time.minutes += stp.time.seconds/60;
            stp.time.seconds = stp.time.seconds%60;
            if (stp.time.minutes>=60)
            {
                stp.time.minutes = stp.time.minutes%60;
            }
        }
    }
}

bool onButtonStopwatch(ButtonId btnId, BtnState buttonState)
{
    bool handled;
    if (buttonState != RELEASED)
        return true;

    switch (btnId)
    {
    case BTN_MODE:
        handled = handleBtnMode(buttonState);
        break;
    case BTN_RESET:
        handled = handleBtnReset(buttonState);
        break;
    case BTN_SETUP:
        handled = handleBtnSetup(buttonState);
        break;
    case BTN_STARTSTOP:
        handled = handleBtnStartStop(buttonState);
        break;
    default:
        handled=false;
    }

    return handled;
}

bool handleBtnReset(BtnState)
{
    if (!stp.running)
    {
        stp.time.mseconds = 0;
        stp.time.seconds = 0;
        stp.time.minutes = 0;
    }
    return true;
}

bool handleBtnStartStop(BtnState)
{
    stp.running = !stp.running;
    return true;
}

bool handleBtnMode(BtnState)
{
    if (stp.running) //if stopwatch running - don't allow to change mode
        return true;
    else
        return false;
}

bool handleBtnSetup(BtnState)
{
    return true;
}
