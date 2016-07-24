#ifndef QTSIMU
#include <Arduino.h>
#else
#include "integration.hpp"
#endif

#include "stopwatchutils.hpp"
#include "pins.hpp"
#include "clock.hpp"
#include "stopwatch.hpp"
#include "countdown.hpp"
#include "7seg-display.hpp"


//Modes
enum Mode {ModeClock, ModeStopWatch, ModeCountDown};

//CHECK FOR HARDWARE PWM PINS - PLAY TONE?
//ANSWER: ARDUINO DOES NOT USES OC PIN. Tone is played by toggling state of given pin on interrupt.
//Interrupt used for that is OICE2

static Mode sCurrentMode = ModeClock;

extern TickHandler tickHandler;

//bit - so we can have up to 8 buttons
static uint8_t sButtonsStates;

void changeMode(Mode mode);
void handleTick(uint16_t ms);
void updateDisplay();

static BtnState calculateButtonState(uint8_t btnStates, uint8_t mask);
void handleButtons();
void handleButtonState(ButtonId id, BtnState state);
bool handleButtonMode(BtnState state);
bool handleButtonStartStop(BtnState state);
bool handleButtonReset(BtnState state);
bool handleButtonSetup(BtnState state);

#ifdef QTSIMU
int _main(void)
#else
int main(void)
#endif
{
  tickHandler = onTickClock;
  init();
}

void changeMode(Mode mode)
{
    sCurrentMode = mode;
    updateDisplay();
}

void handleTick(uint16_t ms)
{
  onTickClock(ms); //clock always is udated regardless its mode
  switch(sCurrentMode)
  {
  case ModeStopWatch:
      onTickStopwatch(ms);
      break;
  case ModeCountDown:
      onTickCountdown(ms);
      break;
  }
  handleButtons();
  updateDisplay();
}

void updateDisplay()
{
    switch(sCurrentMode)
    {
    case ModeStopWatch:
        updateDisplayStopwatch();
        break;
    case ModeCountDown:
        updateDisplayCountdown();
        break;
    case ModeClock:
        updateDisplayClock();
        break;
    }
    onTickDisplay();
}

BtnState calculateButtonState(uint8_t btnStates, uint8_t mask)
{
    bool pressed = btnStates & mask;
    if (pressed && (sButtonsStates & mask))
    {
        return LONGPRESS;//TBD for longpress use timer
    } else if (pressed && !(sButtonsStates & mask))
    {
        return PRESSED;
    } else if (!pressed && (sButtonsStates & mask)) {
        return RELEASED;
    } else {
        return UNDEFINED;
    }
}

void handleButtons()
{
    uint8_t btnStates = 0;
    uint8_t btnMask = 1;
    BtnState btnState;
    btnStates |= digitalRead(PIN_BUTTON_MODE);
    btnStates <<= 1;
    btnStates |= digitalRead(PIN_BUTTON_STARTSTOP);
    btnStates <<= 1;
    btnStates |= digitalRead(PIN_BUTTON_RESET);
    btnStates <<= 1;
    btnStates |= digitalRead(PIN_BUTTON_SETUP);

    if (btnStates == sButtonsStates) //no button state changed
        return;

    btnState = calculateButtonState(btnStates, btnMask);
    handleButtonState(BTN_SETUP, btnState);

    btnMask <<= 1;
    btnState = calculateButtonState(btnStates, btnMask);
    handleButtonState(BTN_RESET, btnState);

    btnMask <<= 1;
    btnState = calculateButtonState(btnStates, btnMask);
    handleButtonState(BTN_STARTSTOP, btnState);

    btnMask <<= 1;
    btnState = calculateButtonState(btnStates, btnMask);
    handleButtonState(BTN_MODE, btnState);

    sButtonsStates = btnStates;
}

ButtonStateHandler getButtonHandler()
{
    ButtonStateHandler handler;
    switch (sCurrentMode)
    {
    case ModeClock:
        handler = onButtonClock;
        break;
    case ModeStopWatch:
        handler = onButtonStopwatch;
        break;
    case ModeCountDown:
        handler = onButtonCountdown;
        break;
    default:
        handler = NULL;
    }

    return handler;
}

void handleButtonState(ButtonId id, BtnState state)
{
    if (state==UNDEFINED)
        return;

    ButtonStateHandler handler = getButtonHandler();

    bool handled = false;
    if (handler)
        handled = handler(id,state);

    if (!handled)
    {
        switch(id)
        {
        case BTN_RESET:
            handleButtonReset(state);
            break;
        case BTN_SETUP:
            handleButtonSetup(state);
            break;
        case BTN_MODE:
            handleButtonMode(state);
            break;
        case BTN_STARTSTOP:
            handleButtonStartStop(state);
            break;
        }
    }
}

bool handleButtonMode(BtnState state)
{
    if (state!=RELEASED)
        return true;

    switch(sCurrentMode)
    {
    case ModeStopWatch:
        changeMode(ModeCountDown);
        break;
    case ModeCountDown:
        changeMode(ModeClock);
        break;
    case ModeClock:
        changeMode(ModeStopWatch);
        break;
    }

    return true;
}

bool handleButtonStartStop(BtnState state)
{
    return true;
}

bool handleButtonReset(BtnState state)
{
    return true;
}

bool handleButtonSetup(BtnState state)
{
    return true;
}
