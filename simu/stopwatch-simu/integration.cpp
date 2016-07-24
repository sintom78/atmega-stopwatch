#include "integration.hpp"
#include "../../src/pins.hpp"

integration_t integration;

void setIntegrationInterface(IntegrationInterface* interface)
{
    integration.interface = interface;
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    integration.interface->digitalWriteHandler(pin,value);
}

void init(void)
{
    integration.interface->initHandler();
}

int digitalRead(uint8_t pin)
{
    return integration.interface->digitalReadHandler(pin);
}

void tone(uint8_t /*_pin*/, unsigned int /*frequency*/, unsigned long /*duration*/)
{
    //TBD - play tone using Qt??
}

void noTone(uint8_t /*_pin*/)
{

}

void mapPins()
{
    integration.interface->mapPin(PIN_BUTTON_MODE, PINF_BUTTONMODE);
    integration.interface->mapPin(PIN_BUTTON_STARTSTOP, PINF_BUTTONSTARTSTOP);
    integration.interface->mapPin(PIN_BUTTON_RESET, PINF_BUTTONRESET);
    integration.interface->mapPin(PIN_BUTTON_SETUP, PINF_BUTTONSET);
    integration.interface->mapPin(PIN_DIGIT0, PINF_DISP0);
    integration.interface->mapPin(PIN_DIGIT1, PINF_DISP1);
    integration.interface->mapPin(PIN_DIGIT2, PINF_DISP2);
    integration.interface->mapPin(PIN_DIGIT3, PINF_DISP3);
    integration.interface->mapPin(PIN_SEGA, PINF_SEGA);
    integration.interface->mapPin(PIN_SEGB, PINF_SEGB);
    integration.interface->mapPin(PIN_SEGC, PINF_SEGC);
    integration.interface->mapPin(PIN_SEGD, PINF_SEGD);
    integration.interface->mapPin(PIN_SEGE, PINF_SEGE);
    integration.interface->mapPin(PIN_SEGF, PINF_SEGF);
    integration.interface->mapPin(PIN_SEGG, PINF_SEGG);
}
