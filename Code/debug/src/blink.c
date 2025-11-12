#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>

int main(void){
	DDRB |= (1 << 2);

	while(1)
	{
		PORTB |= (1 << 2);
#ifndef DEBUG
		_delay_ms(500);
#endif
		PORTB &= ~(1 << 2);
#ifndef DEBUG
		_delay_ms(500);
#endif
	}
	return 0;
}
