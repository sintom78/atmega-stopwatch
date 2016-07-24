#ifndef STOPWATCHUTILS_HPP
#define STOPWATCHUTILS_HPP

#ifdef QTSIMU
#include <stdint.h>
#else
#include <Arduino.h>
#endif

/*
#define BTN_RESET_MASK  0b00000001
#define BTN_STARTSTOP_MASK  0b00000010
#define BTN_MODE_MASK 0b00000100
*/
enum ButtonId { BTN_RESET=1, BTN_STARTSTOP=2, BTN_MODE=4, BTN_SETUP=8 };
enum BtnState {PRESSED, RELEASED, LONGPRESS, UNDEFINED};

uint8_t getDigit(unsigned int value, uint8_t digit);
void setDigit(unsigned int& val, uint8_t digit);
int getNextDigit(uint8_t digit);
unsigned int toBCD(unsigned int);

typedef bool (*ButtonStateHandler)(ButtonId,BtnState);

#endif // STOPWATCHUTILS_HPP
