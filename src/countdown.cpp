#include "countdown.hpp"
#include "7seg-display.hpp"

Countdown cdown = {
    .time = {
        .minutes = 1,
        .seconds = 0,
        .mseconds = 0
    },
    .currentMode = CountdownModeIdle,
    .currentDigit = CDownDigitMinute0
};

static void updateCurrentTime(uint16_t ms);
static uint8_t decMseconds(uint16_t);
static uint8_t decSeconds(uint8_t sec);
static uint8_t decMinutes(uint8_t m);
static bool handleBtnReset(BtnState);
static bool handleBtnStartStop(BtnState);
static bool handleBtnMode(BtnState);
static bool handleBtnSetup(BtnState);
void increaseDigit(CountdownTime& time, eCDownDigit digit);

void onTickCountdown(uint16_t ms)
{
    if (cdown.currentMode != CountdownModeRunning)
        return;

    updateCurrentTime(ms);
    updateDisplayCountdown();
}

bool onButtonCountdown(ButtonId btnId, BtnState buttonState)
{
    bool handled = false;

    if (buttonState != RELEASED)
            return true;

    switch (btnId)
    {
    case BTN_RESET:
        handled = handleBtnReset(buttonState);
        break;

    case BTN_STARTSTOP:
        handled = handleBtnStartStop(buttonState);
        break;

    case BTN_MODE:
        handled = handleBtnMode(buttonState);
        break;

    case BTN_SETUP:
        handled = handleBtnSetup(buttonState);
        break;
    default:
        handled = false;
    }

    return handled;
}

void updateDisplayCountdown()
{
    setDisplayDigit(0,getDigit(cdown.time.minutes,1));
    setDisplayDigit(1,getDigit(cdown.time.minutes,0));
    setDisplayDigit(2,getDigit(cdown.time.seconds,1));
    setDisplayDigit(3,getDigit(cdown.time.seconds,0));
}

void updateCurrentTime(uint16_t ms)
{
    uint8_t x;
    x = decMseconds(ms);
    if ((cdown.time.seconds>0) || (cdown.time.minutes>0))
    {
        x = decSeconds(x);
    }
    if (cdown.time.minutes>0)
    {
        x = decMinutes(x);
    }

    if ((cdown.time.mseconds==0) &&
            (cdown.time.seconds==0) &&
            (cdown.time.minutes==0)
       )
    {
        cdown.currentMode = CountdownModeIdle;
        //TBD - turno on alarm?
    }
}

uint8_t decMseconds(uint16_t ms)
{
    uint8_t s = 0;
    if (cdown.time.mseconds<ms)
    {
        s = ms / 1000;
        ms = ms % 1000;
        if (cdown.time.mseconds<ms)
        {
            s++;
            cdown.time.mseconds = 1000 - (ms - cdown.time.mseconds);
        } else {
            cdown.time.mseconds -= ms;
        }
    } else {
        cdown.time.mseconds -= ms;
    }
    return s;
}

uint8_t decSeconds(uint8_t sec)
{
    uint8_t m = 0;
    if (cdown.time.seconds<sec)
    {
        m = sec / 60;
        sec = sec % 60;
        if (cdown.time.seconds<sec)
        {
            m++;
            cdown.time.seconds = 60 - (sec - cdown.time.seconds);
        } else {
            cdown.time.seconds -= sec;
        }
    } else {
        cdown.time.seconds -= sec;
    }
    return m;
}

uint8_t decMinutes(uint8_t min)
{
    //TBD - currently I dont care if wrapping for minutes occured
    if(cdown.time.minutes<min)
    {
        cdown.time.minutes = 0;
    } else {
        cdown.time.minutes -= min;
    }
    return 0;
}

bool handleBtnReset(BtnState)
{
    return true;
}

bool handleBtnStartStop(BtnState)
{
    switch(cdown.currentMode)
    {
    case CountdownModeRunning:
        cdown.currentMode = CountdownModeIdle;
        break;
    case CountdownModeIdle:
        cdown.currentMode = CountdownModeRunning;
        break;
    case CountdownModeSetup:
        increaseDigit(cdown.time, cdown.currentDigit);
        break;
    }
    return true;
}

bool handleBtnMode(BtnState)
{
    bool handled=false;
    if (cdown.currentMode==CountdownModeSetup)
    {
        setDisplayState(cdown.currentDigit, DISPLAY_STATE_ENABLED);
#ifdef QTSIMU
        cdown.currentDigit = static_cast<eCDownDigit>(getNextDigit(cdown.currentDigit));
#else
        cdown.currentDigit = static_cast<eCDownDigit>(getNextDigit(cdown.currentDigit));
#endif
        setDisplayState(cdown.currentDigit, DISPLAY_STATE_BLINKING);
        handled=true;
    } else if (cdown.currentMode ==CountdownModeRunning)
    {
        handled = true;
    }

    return handled;
}

bool handleBtnSetup(BtnState)
{
    switch(cdown.currentMode)
    {
    case CountdownModeIdle:
        cdown.currentMode = CountdownModeSetup;
        cdown.currentDigit = CDownDigitMinute0;
        setDisplayState(cdown.currentDigit, DISPLAY_STATE_BLINKING);
        break;
    case CountdownModeSetup:
        cdown.currentMode = CountdownModeIdle;
        setDisplayState(cdown.currentDigit, DISPLAY_STATE_ENABLED);
        break;
    }

    return true;
}

void increaseDigit(CountdownTime& time, eCDownDigit digit)
{
    switch (digit)
    {
    case CDownDigitMinute1:
        cdown.time.minutes++;
        break;

    case CDownDigitMinute0:
        cdown.time.minutes +=10;
        cdown.time.minutes %= 60;
        break;

    case CDownDigitSecond1:
        cdown.time.seconds++;
        break;

    case CDownDigitSecond0:
        cdown.time.seconds +=10;
        cdown.time.seconds %= 60;
        break;
    }
}
