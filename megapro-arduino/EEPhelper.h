#define WED DDB0
#define OED DDB2
#define CED DDB3
#define WEP PORTB0
#define OEP PORTB2
#define CEP PORTB3
#define ALD DDRA
#define AHD DDRC
#define ALP PORTA
#define AHP PORTC
#define DD DDRL
#define DP PORTL
#define DATA PINL
#define OUT 0xFF
#define IN 0x00
#define PW 100
#define ADDLO(add) (add) & 0xFF
#define ADDHI(add) ((add) >> 8) & 0xFF
#define SETADD(add)   \
    ALP = ADDLO(add); \
    AHP = ADDHI(add);
#include <Arduino.h>

void unlock();
void writeEEPROM(uint16_t address, byte *data);   // page write
void writeEEPROM(uint16_t address, uint8_t data); // byte write
byte readEEPROM(uint16_t address);
void readEEPROM(uint16_t address, byte *pageContent);