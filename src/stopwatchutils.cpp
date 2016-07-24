#include "stopwatchutils.hpp"

//returns digit
uint8_t getDigit(unsigned int value, uint8_t digit)
{
    unsigned int bcd = toBCD(value);
    return (bcd >> (digit*4)) & 0xF;
}

void setDigit(uint8_t value, uint8_t digit, unsigned int &destVal)
{
    unsigned int v = value;
    unsigned int mask = 0xF;
    unsigned int shift = digit << 2;
    mask <<= shift;
    mask = ~mask;
    destVal &= mask;
    destVal |= (v << shift);
}

int getNextDigit(uint8_t digit)
{
    digit = (++digit) & 0x00000011b;
    return digit;
}

unsigned int toBCD(unsigned int val)
{
    unsigned int bcd=0;
    unsigned int p=10;
    while (val/p)
    {
        p*=10;
    }
    p=p/10;

    while (p)
    {
        unsigned int m = val/p;
        bcd <<=4;
        val=val-(m*p);
        p=p/10;
        bcd+=m;
    }
    return bcd;
}
