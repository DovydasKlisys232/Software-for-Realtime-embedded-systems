#define F_CPU 1000000UL

#include <avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

//int array that stores the temperature values (both set to 0)
static int iTemperatures[2] = {0,0};

//function
void Pin_init();

//interrupt that occurs when falling edge detected on PD2 pin
//increments both temperature values in array when falling edge detected on PD2 pin
ISR(INT0_vect)
{
	iTemperatures[0] += 1;
	iTemperatures[1] += 1;
}

//registers setup and interrupts are enabled
//in the loop the first temperature is saved into iTemp0
//there is a 1 second delay and second temperature is saved
/*delay causes the if statement to be true for some time since the temperatures are saved
at different times and interrupts can occur at any time to increment the values (race conditions). 
This causes the led to turn on. 
If they are equal then led is off.
*/
int main(void)
{
	Pin_init();
	
	sei();

	int iTemp0, iTemp1;
		
	while(1)
	{
		iTemp0 = iTemperatures[0];
		_delay_ms(1000);
		iTemp1 = iTemperatures[1];
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

//function that sets up the registers
//PB2 set as output for led
//PD2 set up as input (INT0)
//correct registers set for falling edge capture interrupts
void Pin_init()
{
	DDRB |= (1 << PB2);		// set PB2 to set as output
	
	DDRD &= ~(1 << PD2);	// Clear PD2 to set as input
	PORTD |= (1 << 2);		// enable internal pull up resistor on pd2
	EICRA |= (1 << ISC01); 	// Set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0
		
}
