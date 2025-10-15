
#include "setup.h"

//initialize the pin pd2 for input capture interrupt
void interrupt_init()
{
	DDRD &= ~(1<<2); //set pd2 as an input
	PORTD |= (1 << 2); //enable internal pull up resistor on pd2
	EICRA |= ((1 << ISC00) | (1 << ISC01)); //interrupt on rising edge
    EIMSK |= (1 << INT0); //interrupt pin enable
}

//initialize the clock for ctc
void clock_init()
{
	//set the correct prescale (64) and set to ctc mode
	TCCR1B |= ((1 << CS10) | (1 << CS11)); 
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
	OCR1A = 15624; // Set CTC compare value to 1 Hz at 1 MHz AVR clock , with a prescaler of 64	
}
