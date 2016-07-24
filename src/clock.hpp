#ifndef CLOCK_HPP
#define CLOCK_HPP

#ifndef QTSIMU
#include <Arduino.h>
#endif
#include "stopwatchutils.hpp"

enum eClockMode {ClockModeTime, ClockModeSetTime, ClockModeAlarm, ClockModeSetAlarm};
enum eDigit {DigitHour0=0, DigitHour1=1, DigitMinute0=2, DigitMinute1=3};

struct ClockTime
{
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
  uint16_t mseconds;
};

struct Clock
{
  eClockMode currentClockMode;
  bool alarmOn;
  bool alarming;
  eDigit currentDigit;
  ClockTime clockTime;
  ClockTime alarmTime;
};

void onTickClock(uint8_t ms);
bool onButtonClock(ButtonId btnId, BtnState buttonState);
void updateDisplayClock();


#endif // CLOCK_HPP
