#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include "usart.h"

//variables to store the time
uint16_t count = 0;
uint16_t finish = 0;
uint16_t freq = 0;

ISR(INT0_vect)
{
	count++;
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

	//set the correct prescale (64) and set to ctc mode
	TCCR1B |= ((1 << CS10) | (1 << CS11)); 
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
	OCR1A = 15624; // Set CTC compare value to 1 Hz at 1 MHz AVR clock , with a prescaler of 64

	sei();
	char str[10];
		
	while(1)
	{
		if(TIFR1 & (1 << OCF1A)) //check if the timer interrupt occured
		{
			freq = count;
			count = 0;
			usartSendString("Freq: ");
			sprintf(str, "%u\n", freq);
			usartSendString(str);
			TIFR1 = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
		}	

	}
}
