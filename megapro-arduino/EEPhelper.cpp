#include "EEPhelper.h"

void unlock()
{
    // OE -> HIGH
    PORTB |= 1 << OEP;
    writeEEPROM(0x5555, 0x00aa);
    writeEEPROM(0x2aaa, 0x0055);
    writeEEPROM(0x5555, 0x0080);
    writeEEPROM(0x5555, 0x00aa);
    writeEEPROM(0x2aaa, 0x0055);
    writeEEPROM(0x5555, 0x0020);
}

void writeEEPROM(uint16_t address, byte *data)
{
    DD = OUT;
    // OE CE WE -> HIGH
    PORTB |= 1 << OEP | 1 << WEP | 1 << CEP;
    // Address Line A15 - A6
    AHP = ADDHI(address);
    for (int8_t offset = 0; offset <= 63; offset++)
    {
        // t_SA
        ALP = ADDLO(address + offset);
        delayMicroseconds(10);

        // pulse H -> L
        // WE CE -> LOW
        PORTB &= ~(1 << WEP) & ~(1 << CEP);
        delayMicroseconds(10);

        // t_SD
        DP = data[offset];
        delayMicroseconds(10);

        // pulse L -> H
        // WE CE -> HIGH
        PORTB |= 1 << WEP | 1 << CEP;
        delayMicroseconds(10);
    }
    delay(10);
}

void writeEEPROM(uint16_t address, uint8_t data)
{
    // D0 - D7 OUTPUT
    DD = OUT;
    SETADD(address);
    DP = data;
    // WE CE -> LOW
    PORTB &= ~(1 << WEP) & ~(1 << CEP);
    delayMicroseconds(10);
    // WE CE -> HIGH
    PORTB |= 1 << WEP | 1 << CEP;
    delay(10);
}

byte readEEPROM(uint16_t address)
{
    DD = IN;
    SETADD(address);
    byte data = DATA;
    return data;
}

void readEEPROM(uint16_t address, byte *pageContent)
{
    DD = IN;
    // CE OE -> LOW | WE -> HIGH
    PORTB = PORTB & ~(1 << CEP) & ~(1 << OEP) | 1 << WEP;
    // A0 - A7
    AHP = ADDHI(address);
    for (int8_t offset = 0; offset <= 63; offset++)
    {
        ALP = ADDLO(address + offset);
        delayMicroseconds(10);
        pageContent[offset] = DATA;
    }
}