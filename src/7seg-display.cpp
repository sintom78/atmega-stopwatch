#ifdef QTSIMU
#include <QDebug>
#include "integration.hpp"
#endif

#include "7seg-display.hpp"
#include "pins.hpp"

#ifdef QTSIMU
const uint8_t digit_to_pin[DISPLAYS] =
#else
const uint8_t PROGMEM digit_to_pin[DISPLAYS] =
#endif
{
  PIN_DIGIT0,
  PIN_DIGIT1,
  PIN_DIGIT2,
  PIN_DIGIT3
};

static uint8_t segmentCodes[] = {
//bits: .gfedcba
  0x3F, //0 - .-fe dcba
  0x06, //1 - .--- -cb-
  0x5B, //2 - .g-e d-ba
  0x4F, //3 - .g-- dcba
  0x66, //4 - .gf- -cb-
  0x6D, //5 - .gf- dc-a
  0x7D, //6 - .gfe dc-a
  0x07, //7 - .--- -cba
  0x7F, //8 - .gfe dcba
  0x6F  //9 - .gf- dcba
};

//private data

struct DisplayConfig {
    uint8_t digit;
    bool enabled;
    DisplayState userState;
    uint8_t blinkCycles;
};

static DisplayConfig sDisplayConfig[DISPLAYS] = {
    {
        .digit = 0,
        .enabled=true,
        .userState=DISPLAY_STATE_ENABLED,
        .blinkCycles=0
    },
    {
        .digit = 0,
        .enabled=true,
        .userState=DISPLAY_STATE_ENABLED,
        .blinkCycles=0
    },
    {
        .digit = 0,
        .enabled=true,
        .userState=DISPLAY_STATE_ENABLED,
        .blinkCycles=0
    },
    {
        .digit = 0,
        .enabled=true,
        .userState=DISPLAY_STATE_ENABLED,
        .blinkCycles=0
    }
};

static const uint16_t ticksToSwitchDisplay = DEFAULT_DISPLAY_TIME;
static uint16_t ticks = 0;
static uint8_t currentDisplay=0;

//private functions
static void nextDisplay();
static void updateSegPins();
static void updateDisplayState();

//implementation
void setDisplayDigit(uint8_t digitNo, uint8_t value)
{
  //qDebug() << "SetDisplayDigit: " << digitNo << " val: " << value;
  sDisplayConfig[digitNo].digit=value;
}

void onTickDisplay()
{
  ticks++;
  if (ticks >= ticksToSwitchDisplay)
  {
    ticks=0;
    nextDisplay();
  }
}

void setDisplayState(uint8_t displayNo, DisplayState state)
{
    if (displayNo > DISPLAYS-1)
        return;

    sDisplayConfig[displayNo].userState = state;
}

void nextDisplay()
{
  uint8_t display;
  display=digit_to_pin[currentDisplay];
  digitalWrite(display, LOW);
  currentDisplay++;
  if (currentDisplay>=DISPLAYS)
  {
    currentDisplay=0;
  }
  display=digit_to_pin[currentDisplay];
  digitalWrite(display, HIGH);
  updateDisplayState();
  updateSegPins();
}

void updateDisplayState()
{
    switch(sDisplayConfig[currentDisplay].userState) {
    case DISPLAY_STATE_DISABLED:
        sDisplayConfig[currentDisplay].enabled=false;
        break;
    case DISPLAY_STATE_ENABLED:
        sDisplayConfig[currentDisplay].enabled=true;
        break;
    case DISPLAY_STATE_BLINKING:
        if (!sDisplayConfig[currentDisplay].blinkCycles)
        {
            sDisplayConfig[currentDisplay].enabled = !sDisplayConfig[currentDisplay].enabled;
            sDisplayConfig[currentDisplay].blinkCycles = DISPLAY_BLINK_CYCLES;
        } else {
            sDisplayConfig[currentDisplay].blinkCycles--;
        }
    }
}

void updateSegPins()
{
  uint8_t segCode = 0;

  if (sDisplayConfig[currentDisplay].enabled)
    segCode = segmentCodes[ sDisplayConfig[currentDisplay].digit ];

  digitalWrite(PIN_SEGA, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGB, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGC, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGD, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGE, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGF, segCode & 1);
  segCode >>= 1;
  digitalWrite(PIN_SEGG, segCode & 1);
}
