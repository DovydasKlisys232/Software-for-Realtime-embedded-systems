/*
 * usart.c
 *
 *  Created on: 12 Nov 2017
 *      Author: btoland
 */

#include <avr/interrupt.h>
#include <util/atomic.h>

#include "usart.h"

void usartInit(void)
{
	UCSR0B |= ((1<<RXEN0) | (1<<TXEN0));

	UBRR0 = 12;  //1Mhz - RAW AVR board
	UCSR0A |= (1<<U2X0);
}

void usartEnableRxInt(void)
{
	UCSR0B |= (1<<RXCIE0);
}

void usartSendChar(char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
	UDR0 = c;
}

void usartSendString(char string[])
{
	uint8_t i = 0;

	while(string[i] != '\0')
	{
		usartSendChar(string[i++]);
	}
}

uint8_t usartCharReceived(void)
{
	uint8_t retVal = 0;

	if(UCSR0A & (1 << RXC0))
	{
		retVal = 1;
	}

	return retVal;
}

char usartReadChar(void)
{
	return UDR0;
}
