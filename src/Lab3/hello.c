#include <avr/io.h>

#define F_CPU 1000000UL
#include <util/delay.h>

void usartInit(void);
void usartSendString(char str[]);

int main()
{
	usartInit();
	while(1)
	{
		usartSendString("Hello\n");
		_delay_ms(1000);
	}	
}

void usartInit(void)
{
    UCSR0B |= ((1<<TXEN0)|(1<<RXEN0)); //transmit and recieve bits set to 1
    UCSR0B &= ~(1<<TXB80); //sneaky bit to zero

    UCSR0C &= ~((1<<UMSEL01)|(1<<UMSEL00)); //set to asynchronous UART mode
    UCSR0C &= ~((1<<UPM01)|(1<<UPM00)); //parity mode disabled
    UCSR0C &= ~(1<<USBS0); //1 stop bit
    UCSR0C |= ((1<<UCSZ01)|(1<<UCSZ00)); //8 bit data set

    UBRR0 = 103; //UART baud rate set
}

//function to read the string array out to bluetooth
void usartSendString(char str[]) {
    for (int i = 0; str[i]; i++) {
        while (!(UCSR0A & (1 << UDRE0))); //wait until buffer is empty
        UDR0 = str[i]; //send the character out to the usart
    }
}
