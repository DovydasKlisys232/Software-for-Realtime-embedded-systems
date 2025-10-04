#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

#define F_CPU 1000000UL  // 1MHz for your RAW AVR board

int main(void)
{
    // Initialize USART for serial communication
    usartInit();
    
    // Main loop
    while(1)
    {
        // Send "Hello World" string
        usartSendString("Hello World\r\n");
        
        // Delay for 1 second (1000ms)
        _delay_ms(1000);
    }
    
    return 0;
}
