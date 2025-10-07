#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "usart.h"

//variables to store the time
uint16_t count = 0;

//External interrupt
//occurs when the correct edge is detected on INT0 (pin pd2) 
ISR(INT0_vect)
{
    count++;
    PORTB ^= (1<<5);
}

int main(void)
{
	usartInit();

	usartSendString("Hello\n");
    DDRB |= (1<<5); //set pb5 as an output
    DDRD &= ~(1<<2); //set pd2 as an input
    PORTD |= (1 << 2); //enable internal pull up resistor on pd2

    EICRA |= ((1 << ISC00) | (1 << ISC01)); //interrupt on rising edge
    EIMSK |= (1 << INT0); //interrupt pin enable

	uint8_t loopCnt = 0;

	sei(); //enables global interrupts 
    char str[10]; //stores the count as a string

	while(1)
	    {
	        loopCnt ++;
	
	        uint16_t oldCount;
	        uint16_t freq;
	        
	        if(loopCnt == 4)
	        {
	            loopCnt = 0;
	
	            freq = count - oldCount;
	
	            usartSendString("Freq: ");
	            sprintf(str, "%u\n", freq);
	            usartSendString(str);
	
	            oldCount = count;
	        }
	        
	        usartSendString("Count: ");
	        sprintf(str, "%u\n", count);
	        usartSendString(str);
	        _delay_ms(250);
	    }
}
