#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#include <stdint.h>

#define LOW 0
#define HIGH 1
//typedef char uint8_t;
//typedef unsigned short uint16_t;

void digitalWrite(uint8_t, uint8_t);
void init(void);
int digitalRead(uint8_t);
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t _pin);
void mapPins();

enum PinFunctions {
    PINF_UNSUPPORTED,
    PINF_SEGA,
    PINF_SEGB,
    PINF_SEGC,
    PINF_SEGD,
    PINF_SEGE,
    PINF_SEGF,
    PINF_SEGG,
    PINF_LEDS,
    PINF_DISP0,
    PINF_DISP1,
    PINF_DISP2,
    PINF_DISP3,
    PINF_BUTTONRESET,
    PINF_BUTTONMODE,
    PINF_BUTTONSET,
    PINF_BUTTONSTARTSTOP
};

enum AtmegaPinNumbers {

};

class IntegrationInterface
{
public:
    virtual void mapPin(unsigned int pin, PinFunctions)=0;
    virtual void digitalWriteHandler(uint8_t,uint8_t)=0;
    virtual void initHandler(void)=0;
    virtual int digitalReadHandler(uint8_t)=0;
};

struct integration_t
{
    IntegrationInterface* interface;
    void (*tickHandler)(uint16_t);
};

#endif // INTEGRATION_HPP
