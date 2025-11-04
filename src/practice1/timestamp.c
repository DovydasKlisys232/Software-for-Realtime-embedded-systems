/* This code is for testing some of the learned interrupt functionality by
coding a timestamp tracker*/

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "usart.h"

void interrupt_init();
void clock_init();

// Timer1 Compare Match A ISR - generates timestamps
ISR(TIMER1_COMPA_vect)
{
    timestamp_counter++; // Increment timestamp every second
}

// External Interrupt 0 ISR - handles button presses
ISR(INT0_vect)
{
    // Simple debounce - wait a bit and check if button is still high
    _delay_ms(50);
    if(PIND & (1 << PIND2)) // Check if PD2 is still high (rising edge)
    {
        event_timestamp = timestamp_counter; // Capture current timestamp
        event_occurred = 1; // Set flag to indicate new event
    }
}

int main(void)
{
	/*setup an interrupt and timer1*/
	interrupt_init();
	clock_init();
	
	DDRD &= ~(1<<2); //set pd2 as an input (int0)
	PORTD |= (1 << 2); //enable internal pull up resistor on pd2
	EICRA |= ((1 << ISC00) | (1 << ISC01)); //interrupt on rising edge
    EIMSK |= (1 << INT0); //interrupt pin enable

    usartInit();

    sei(); //enalbe global interrupts

	while(1)
	{
		
	}	
}

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
