#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>
#include "setup.h"
#include "usart.h"

//variables to store the time
uint16_t count = 0;
uint16_t freq = 0;
uint8_t new_data_ready = 0;

ISR(INT0_vect)
{
	count++;
}

ISR(TIMER1_COMPA_vect)
{
	freq = count;
    count = 0;
    new_data_ready = 1;
}

int main(void)
{
	
	interrupt_init();
	clock_init();
	usartInit();
	
	sei();
	char str[20];
		
	while(1)
	{
		if(new_data_ready) //check if the timer interrupt occured
		{
            sprintf(str, "Freq: %u\r\n", freq);
            usartSendString(str);
            new_data_ready = 0;
		}
		_delay_ms(250);
	}
}
