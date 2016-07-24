#ifndef SEG_DISPLAY_HPP
#define SEG_DISPLAY_HPP

#ifndef QTSIMU
#include <Arduino.h>
#endif

#define DEFAULT_DISPLAY_TIME 15
#define DISPLAY_BLINK_CYCLES 10;
#define DISPLAYS 4

enum DisplayState {DISPLAY_STATE_DISABLED, DISPLAY_STATE_ENABLED, DISPLAY_STATE_BLINKING};

//interface
void setDisplayDigit(uint8_t digitNo, uint8_t value);
void onTickDisplay();
void setDisplayState(uint8_t displayNo, DisplayState state);

#endif // 7SEG_DISPLAY_HPP
