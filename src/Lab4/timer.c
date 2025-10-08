#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "usart.h"

//variables to store the time
uint16_t count = 0;

ISR(INT0_vect)
{
	count++;
	PORTB ^= (1 << 5);
}


int main(void)
{
	usartInit();
	usartSendString("Hello\n");
	
	DDRB |= (1<<5); //set led pin
	DDRD &= ~(1<<2); //set pd2 as an input
	PORTD |= (1 << 2); //enable internal pull up resistor on pd2

	EICRA |= ((1 << ISC00) | (1 << ISC01)); //interrupt on rising edge
    EIMSK |= (1 << INT0); //interrupt pin enable

	//set the correct prescale (64)
	TCCR1B |= ((1 << CS10) | (1 << CS11)); 
	TCCR1B &= ~(1 << CS12);

	sei();
	char str[10];
		
	while(1)
	{

		uint16_t oldCount;
		uint16_t freq;
	
		if(TCNT1 == 15624) //if 1 second has passed
		{
			TCNT1 = 0;

			freq = count - oldCount;

			usartSendString("Freq: ");
			sprintf(str, "%u\n", freq);
			usartSendString(str);

			oldCount = count;
		}	

	}
}
