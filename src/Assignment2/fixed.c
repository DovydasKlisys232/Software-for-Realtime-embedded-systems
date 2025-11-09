#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf
#include <util/delay.h>

//int array that holds two different temperature values(starts at 0 for both temperatures)
static volatile int iTemperatures[2] = {0,0};

//function that sets up the interrupt pin and led pin
void Pin_init();

//interrupt occurs whenever a falling edge is detected on PD2
//increments both temperatures
ISR(INT0_vect)
{
	iTemperatures[0] += 1;
	iTemperatures[1] += 1;
}

int main(void)
{
	
	Pin_init();
	
	sei();

	int iTemp0, iTemp1;

	//interrupts are disabled to prevent changes to temperatures and cause timing mismatches
	//first value of array is read into iTemp0 then after a 1 second delay the second value is read into iTemp1
	//first condition checks if temperatures are different and if so then led lights up
	//second condition keeps led off if temperatures are the same
	while(1)
	{
		cli();
		iTemp0 = iTemperatures[0];
		_delay_ms(1000);
		iTemp1 = iTemperatures[1];
		sei();
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

void Pin_init()
{
	DDRB |= (1 << PB2);		// set PB2 to set as output
	
	DDRD &= ~(1 << PD2);	// Clear PD2 to set as input
	PORTD |= (1 << 2);		// enable internal pull up resistor on pd2
	EICRA |= (1 << ISC01); 	// Set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0
}
