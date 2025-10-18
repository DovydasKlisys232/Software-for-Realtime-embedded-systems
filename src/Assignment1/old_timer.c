#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include "setup.h"
#include "usart.h"

//variables to store the time
uint16_t count = 0;
uint16_t freq = 0;

ISR(INT0_vect)
{
	count++;
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
		if(TIFR1 & (1 << OCF1A )) //check if the timer interrupt occured
		{
			freq = count;
		    count = 0;
		    usartSendString("Freq: ");
            sprintf(str,"%u\n", freq);
            usartSendString(str);
            TIFR1 = (1 << OCF1A); 
		}
	}
}
