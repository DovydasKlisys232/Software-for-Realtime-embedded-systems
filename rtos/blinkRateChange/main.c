#include "FreeRTOS.h"
#include "task.h"

#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include "usart.h"

#define LED_PIN 5

// Globals
static int led_delay = 500;   // ms
static const uint8_t buf_len = 20;  //Static here just means "global to this file only"

//task prototypes
void toggleLED(void *parameter);
void readSerial(void *parameters);

int main(void)
{
	_delay_ms(1000);
	usartInit(9600);

  	usartSendString("Multi-task LED Demo");
  	usartSendString("Enter a number in milliseconds to change the LED delay.");

  	// Start blink task
  	xTaskCreate(  
			toggleLED,      // Function to be called
            "Toggle LED",   // Name of task - not used here
            256,           // Stack size
            NULL,           // Parameter to pass
            1,              // Task priority
            NULL);           // Task handle

            
  	// Start serial read task
  	xTaskCreate(  
            readSerial,     // Function to be called
            "Read Serial",  // Name of task
            256,            // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority (must be same to prevent lockup)
            NULL);           // Task handle

	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}


// Task: Blink LED at rate set by global variable
void toggleLED(void *parameter) {

	DDRB |= (1<<LED_PIN);
	
	while (1) {
		PORTB |= (1<<LED_PIN);
		vTaskDelay(led_delay / portTICK_PERIOD_MS);
		PORTB &= ~(1<<LED_PIN);
		vTaskDelay(led_delay / portTICK_PERIOD_MS);
  	}
}


// Task: Read from serial terminal using atoi()
void readSerial(void *parameters) 
{
	char c;
  	char buf[buf_len];  //local so full of random rubbish
  	uint8_t idx = 0;

  	// Clear whole buffer
  	memset(buf, 0, buf_len); //Standard way of doing this

  	// Loop forever
  	while (1) 
	{

		// Read characters from serial
		if (usartCharReceived())
		{  
			c = usartReadChar();

		  	// Update delay variable and reset buffer if we get a carrage return character
		  	if (c == '\r') 
			{	
				//convert the buffer into a string
				//currently has '\r' at the end instead of '\0'
				//atoi() and usartSendString() rely on a '\0' at the end (a string)
				buf[buf_len-1] = '\0';

				led_delay = atoi(buf);
				usartSendString("\nUpdated LED delay to: ");

				usartSendString(buf);

				memset(buf, 0, buf_len);
				idx = 0;
		  	} 
			else 
			{        
		    	// Only append if index is not over message limit
		    	if (idx < buf_len - 1) 
				{
		      		buf[idx] = c;
		      		idx++;
		    	}
		  	}
		}
  	}
}