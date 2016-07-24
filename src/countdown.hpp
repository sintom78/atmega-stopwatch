#ifndef COUNTDOWN_HPP

#ifdef QTSIMU
#include <stdint.h>
#include <QDebug>
#include "integration.hpp"
#define QDEBUG(a) qDebug() << (a)
#else
#include <Arduino.h>
#endif

#include "stopwatchutils.hpp"

enum eCDownDigit {CDownDigitMinute0=0, CDownDigitMinute1=1, CDownDigitSecond0=2, CDownDigitSecond1=3};

void onTickCountdown(uint16_t ms);
bool onButtonCountdown(ButtonId btnId, BtnState buttonState);
void updateDisplayCountdown();

struct CountdownTime
{
    uint8_t minutes;
    uint8_t seconds;
    uint16_t mseconds;
};

enum CountdownMode {CountdownModeIdle, CountdownModeSetup, CountdownModeRunning};

struct Countdown
{
    CountdownTime time;
    CountdownMode currentMode;
    eCDownDigit currentDigit;
};


#endif
