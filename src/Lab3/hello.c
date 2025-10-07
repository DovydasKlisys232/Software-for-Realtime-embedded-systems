#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

int main(void)
{
    DDRB |= (1 << 5);  //set pin pb5 as output
    
    usartInit();	//initialize usart
    
    // Blink LED to confirm program is running
    PORTB |= (1 << 5);
    _delay_ms(1000);
    PORTB &= ~(1 << 5);
    
    while(1)
    {
        PORTB |= (1 << 5);  // LED on during transmit
        usartSendString("Hello World\r\n");
        PORTB &= ~(1 << 5);  // LED off after transmit
        _delay_ms(1000);
    }
    return 0;
}
