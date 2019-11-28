#define PAGE_SIZE 64
#include "EEPhelper.h"
#include "Serial.h"

// the setup function runs once when you press reset or power the board
void setup()
{
    // OE CE WE pinMode OUTPUT
    DDRB = DDRB | 1 << OED | 1 << WED | 1 << CED;
    // USART Initialize
    // BAUD rate defined in Serial.h
    usart_init();
    // A0 - A15 pinMode OUTPUT
    ALD = OUT;
    AHD = OUT;
    // unlock();
    // Inital Handshake
    usart_put(0xcc);
}

// CRC-8 - based on the CRC8 formulas by Dallas/Maxim
// code released under the therms of the GNU GPL 3.0 license
void CRC8(const byte *data1, const byte *data2, byte *crc1, byte *crc2, byte len)
{
    *crc1 = 0x00;
    *crc2 = 0x00;
    while (len--)
    {
        byte extract1 = *data1++;
        byte extract2 = *data2++;
        for (byte tempI = 8; tempI; tempI--)
        {
            byte sum1 = (*crc1 ^ extract1) & 0x01;
            byte sum2 = (*crc2 ^ extract2) & 0x01;
            *crc1 >>= 1;
            *crc2 >>= 1;
            sum1 && (*crc1 ^= 0x8C);
            sum2 && (*crc2 ^= 0x8C);
            extract1 >>= 1;
            extract2 >>= 1;
        }
    }
}

byte pagedata[64];
byte pageContent[64];
uint16_t page = 0;
uint16_t offset = 0;
void loop()
{
    // read 64 bytes in one time
    for (offset = 0; offset <= PAGE_SIZE - 1; offset++)
        pagedata[offset] = usart_get();
    writeEEPROM(uint16_t((page << 6) & 0xFFC0), pagedata);
    readEEPROM(uint16_t((page << 6) & 0xFFC0), pageContent);
    // calculate CRC and check
    byte dataCRC;
    byte romCRC;
    //combine two CRC calculate
    CRC8(pagedata, pageContent, &dataCRC, &romCRC, PAGE_SIZE);
    usart_put(dataCRC);
    if (dataCRC == romCRC)
        usart_put(byte(1));
    else
        usart_put(byte(0));
    page += 1;
    offset = 0;
}