#include "Serial.h"

void usart_put(byte data)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
    loop_until_bit_is_set(UCSR0A, TXC0);
}

void usart_put(byte data, int8_t base)
{
    char s[10] = {NULL};
    char tmp[10] = {NULL};
    (base == HEX) && strcat(s, "0x");
    (base == BIN) && strcat(s, "B");
    itoa(data, tmp, base);
    strcat(s, tmp);
    usart_put(s);
}

void usart_put(char *s)
{
    while (*s)
    {
        usart_put(*s);
        s++;
    }
}

byte usart_get()
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void usart_init()
{
    UBRR0 = UBRR_VALUE;

    UCSR0A = 0;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00) | _BV(USBS0);
    delay(10);
}