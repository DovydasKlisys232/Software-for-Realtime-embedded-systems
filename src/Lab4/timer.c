#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "usart.h"

//variables to store the time
uint16_t count = 0;

//internal interrupt
//occurs when the timer capture occurs 
ISR()
{

}

int main(void)
{
	DDRB |= (1<<5);
	

	while(1)
	{
		
	}
}
