#pragma once
#define BAUD 2000000
#include <Arduino.h>
#include <util/setbaud.h>
#include <string.h>

void usart_put(byte data);
void usart_put(byte data, int8_t base);
void usart_put(char *s);
byte usart_get();
void usart_init();