#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "usart.h"

static volatile int iTemperatures[2] = {0,0};

ISR(INT0_vect)
{
	iTemperatures[0] += 1;
	iTemperatures[1] += 1;
}

int main(void)
{
	usartInit();

	DDRB |= (1 << PB2);		// set PB2 to set as output

	DDRD &= ~(1 << PD2);	// Clear PD2 to set as input
	PORTD |= (1 << 2);		// enable internal pull up resistor on pd2
	EICRA |= (1 << ISC01); 	// Set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0
	
	sei();

	int iTemp0, iTemp1;
		
	while(1)
	{
	//	cli();
		iTemp0 = iTemperatures[0];
	//	_delay_ms(1000);
		iTemp1 = iTemperatures[1];
	//	sei();
		if(iTemp0 != iTemp1)
		{
			PORTB |= (1 << PB2);   // Set PB2 HIGH
		}
		else
		{
			PORTB &= ~(1 << PB2);  // Set PB2 low
		}
	}
}
