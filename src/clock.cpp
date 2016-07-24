#ifdef QTSIMU
#include "integration.hpp"
#endif

#include "stopwatchutils.hpp"
#include "clock.hpp"
#include "7seg-display.hpp"
#include "pins.hpp"

Clock clk = {
    .currentClockMode = ClockModeTime,
    .alarmOn = false,
    .alarming = false,
    .currentDigit = DigitHour0,
    .clockTime = {
        .hour = 0,
        .minute = 0,
        .seconds = 0,
        .mseconds = 0
    },
    .alarmTime = {
        .hour = 0,
        .minute = 0,
        .seconds = 0,
        .mseconds = 0
    }
};

static bool handleBtnReset(BtnState);
static bool handleBtnStartStop(BtnState);
static bool handleBtnMode(BtnState);
static bool handleBtnSetup(BtnState);
static void setMode(eClockMode);
static void switchOffAlarm();
static void switchOnAlarm();
static void updateCurrentTime(uint8_t ms);
static void checkAlarm();
static void playAlarm();
static void stopPlayingAlarm();
static void increaseDigit(ClockTime& time, eDigit digit);

void onTickClock(uint8_t ms)
{
    if (clk.currentClockMode!=ClockModeSetTime)
        updateCurrentTime(ms);

    if ((clk.currentClockMode!=ClockModeSetTime) && (clk.currentClockMode!=ClockModeSetAlarm))
        checkAlarm();
}

bool onButtonClock(ButtonId btnId, BtnState buttonState)
{
    bool handled = false;

    if (buttonState != RELEASED)
            return true;

    if (clk.alarmOn)
    {
        switchOffAlarm();
        return true;
    }

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

void updateDisplayClock()
{
    if ((clk.currentClockMode==ClockModeTime) ||
            (clk.currentClockMode==ClockModeSetTime))
    {
        setDisplayDigit(0,getDigit(clk.clockTime.hour,1));
        setDisplayDigit(1,getDigit(clk.clockTime.hour,0));
        setDisplayDigit(2,getDigit(clk.clockTime.minute,1));
        setDisplayDigit(3,getDigit(clk.clockTime.minute,0));
    } else {
        setDisplayDigit(0,getDigit(clk.alarmTime.hour,1));
        setDisplayDigit(1,getDigit(clk.alarmTime.hour,0));
        setDisplayDigit(2,getDigit(clk.alarmTime.minute,1));
        setDisplayDigit(3,getDigit(clk.alarmTime.minute,0));
    }
}


bool handleBtnReset(BtnState buttonState)
{
    bool handled = true;
    switch(clk.currentClockMode)
    {
    case ClockModeTime:
        setMode(ClockModeAlarm);
        break;

    case ClockModeAlarm:
        setMode(ClockModeTime);
        break;

    case ClockModeSetAlarm:
        switchOffAlarm();
        break;
    case ClockModeSetTime:
        setMode(ClockModeTime);
        break;
    default:
        handled = false;
        break;
    }
    return handled;
}

bool handleBtnStartStop(BtnState buttonState)
{
    bool handled = true;
    switch(clk.currentClockMode)
    {
    case ClockModeSetAlarm:
        increaseDigit(clk.alarmTime, clk.currentDigit);
        break;

    case ClockModeSetTime:
        increaseDigit(clk.clockTime, clk.currentDigit);
        break;
    default:
        handled = false;
        break;
    }

    return handled;
}

bool handleBtnMode(BtnState buttonState)
{
    bool handled;
    switch(clk.currentClockMode)
    {
    case ClockModeSetAlarm:
    case ClockModeSetTime:
        setDisplayState(clk.currentDigit, DISPLAY_STATE_ENABLED);
#ifdef QTSIMU
        clk.currentDigit = static_cast<eDigit>(getNextDigit(clk.currentDigit));
#else
        clk.currentDigit = static_cast<eDigit>(getNextDigit(clk.currentDigit));
#endif
        setDisplayState(clk.currentDigit, DISPLAY_STATE_BLINKING);
        handled = true;
        break;
    default:
        handled = false;
        break;
    }
    return handled;
}

bool handleBtnSetup(BtnState buttonState)
{
    bool handled = true;
    switch(clk.currentClockMode)
    {
    case ClockModeAlarm:
        setMode(ClockModeSetAlarm);
        clk.currentDigit = DigitHour0;
        setDisplayState(clk.currentDigit, DISPLAY_STATE_BLINKING);
        break;

    case ClockModeSetAlarm:
        setMode(ClockModeAlarm);
        setDisplayState(clk.currentDigit, DISPLAY_STATE_ENABLED);
        break;

    case ClockModeTime:
        setMode(ClockModeSetTime);
        clk.currentDigit = DigitHour0;
        setDisplayState(clk.currentDigit, DISPLAY_STATE_BLINKING);
        break;

    case ClockModeSetTime:
        setMode(ClockModeTime);
        setDisplayState(clk.currentDigit, DISPLAY_STATE_ENABLED);
        break;
    default:
        handled = false;
        break;
    }
    return handled;
}

void setMode(eClockMode mode)
{
    clk.currentClockMode = mode;
    //TBD: is any verification needed if we can switch clock mode?
    //     if not remove the function and update handleBtnX functions
}

void switchOffAlarm()
{
    clk.alarmOn = false;
    if (clk.alarming) {
        stopPlayingAlarm();
    }
}

void switchOnAlarm()
{
    clk.alarmOn = true;
}

void updateCurrentTime(uint8_t ms)
{
    clk.clockTime.mseconds += ms;
    if (clk.clockTime.mseconds >= 1000)
    {
        clk.clockTime.seconds += clk.clockTime.mseconds/1000;
        clk.clockTime.mseconds = clk.clockTime.mseconds%1000;
        if (clk.clockTime.seconds>=60)
        {
            clk.clockTime.minute += clk.clockTime.seconds/60;
            clk.clockTime.seconds = clk.clockTime.seconds%60;
            if (clk.clockTime.minute>=60)
            {
                clk.clockTime.hour += clk.clockTime.minute/60;
                clk.clockTime.minute = clk.clockTime.minute%60;
                if (clk.clockTime.hour >=24)
                {
                    clk.clockTime.hour = 0;
                }
            }
        }
    }
}

void checkAlarm()
{
    if (!clk.alarmOn)
        return;

    if (!clk.alarming &&
            (clk.alarmTime.hour == clk.clockTime.hour) &&
            (clk.alarmTime.minute == clk.clockTime.hour) )
    {
        playAlarm();
    } else if (clk.alarming) {
        stopPlayingAlarm();
    }
}

void playAlarm()
{
    //TODO: play melody?
    unsigned int freq = 1000; //1khz?
    unsigned int duration = 1000; //1s?
//    tone(PIN_ALARM_SOUND,freq,duration);
}

void stopPlayingAlarm()
{
//    noTone(PIN_ALARM_SOUND);
}

void increaseDigit(ClockTime& time, eDigit digit)
{
    switch (digit)
    {
    case DigitHour1:
        time.hour++;
        if ((time.hour > 20) && (time.hour > 23))
        {
            time.hour = 20;
        } else {
            if (time.hour == 20)
            {
                time.hour = 10;
            } else if (time.hour == 10) {
                time.hour = 0;
            }
        }

        break;

    case DigitHour0:
        time.hour +=10;
        if ((time.hour>23) && (time.hour<30))
        {
            time.hour = 23;
        } else if (time.hour >= 30){
            time.hour -= 30;
        }
        break;

    case DigitMinute1:
        time.minute++;
        break;

    case DigitMinute0:
        time.minute +=10;
        time.minute %= 60;
        break;
    }
}
