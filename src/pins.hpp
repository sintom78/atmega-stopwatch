#ifndef PINS_HPP
#define PINS_HPP

//buttons pins
#define PIN_BUTTON_RESET 14       //reset / set alarm?
#define PIN_BUTTON_STARTSTOP 15   //start/stop up
#define PIN_BUTTON_MODE 16        //changes between clock/stopwatch/countdown
#define PIN_BUTTON_SETUP 17       //is this needed?

//clock
#define PIN_ALARM_SOUND 18

//Pin configurations for display
#define PIN_SEGA 13 //SCK
#define PIN_SEGB 12 //MISO
#define PIN_SEGC 11 //MOSI
#define PIN_SEGD 10 //SS
#define PIN_SEGE 9
#define PIN_SEGF 8
#define PIN_SEGG 7

#define PIN_DIGIT0 6
#define PIN_DIGIT1 5
#define PIN_DIGIT2 4
#define PIN_DIGIT3 3

#define PIN_BLINKING_DIODES 2

#endif // PINS_HPP
