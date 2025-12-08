/*
Student: Dovudas Klisys
ID: B00165094
Date: 07/12/25
Purpose: this is the main code for CA05. The code implements the two queues and tasks that were 
specified in the end of the video.
*/

#define F_CPU 1000000UL

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "usart.h" //usart library for serial comms

#include<avr/interrupt.h> //for ISR
#include<stdio.h> //for sprintf

/*-----------------------------------------------------------*/
static void printTask(void *pvParameters); 
static void LEDTask(void *pvParameters);
/*-----------------------------------------------------------*/
void ledblink(int blink_count, char* str1);
/*-----------------------------------------------------------*/
// Globals
static QueueHandle_t delay_queue;
static QueueHandle_t msg_queue;

static const msg_queue_len = 5;
static const int delay_queue_len = 5; 
static const uint8_t buf_len = 255; //length of buffer to store user input
static const char command[] = "delay ";; //store delay command string from user for parsing later

typedef struct Message {
  char body[20];
  int count;
} Message;
/*-----------------------------------------------------------*/


int main(void)
{
	//setup usart 
	usartInit();
	
	//create queues
	delay_queue = xQueueCreate(delay_queue_len, sizeof(int)); //queue to hold delay times
	msg_queue = xQueueCreate(msg_queue_len, sizeof(int)); //queue to hold messages

   	xTaskCreate(printTask, NULL, 256, NULL, 1, NULL);
	xTaskCreate(LEDTask, NULL, 256, NULL, 1, NULL);
		
	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

static void printTask(void *pvParameters) // print to the terminal
{
	//setup
	Message rcv_msg;
	int t;
	char ch;
	char userinput[buf_len];
	uint8_t idx = 0;
	uint8_t cmd_len = strlen(command);

	// Clear whole buffer
  	memset(buf, 0, buf_len);	
	
	while(1)
	{
		//read messages from msg_queue
		if (xQueueReceive(msg_queue, &rcv_msg, portMAX_DELAY) == pdTRUE) {
			usartSendString(rcv_msg.body);
			usartSendString("\n");
			usartSendString(rcv_msg.count);
			usartSendString("\n");
		}

		if(usartCharReceived())
		{
			ch = usartReadChar(); //read char from usart
			
			if(ch >= '1' && ch <= '10000') //max range of delay times inputted in terminal
			{
				t = (ch - '0') * 100; //convert char to int and multiply by 100ms
				xQueueSend(delay_queue, &t, portMAX_DELAY);
			}

			usartSendChar(ch); //echo back to terminal

			// Store received character to buffer if not over buffer limit
			if (idx < buf_len - 1) {
				userinput[idx] = ch;
				idx++ ;
			}

			// Print newline and check input on 'enter'
			if ((ch == '\n') || (ch == '\r')) {

				// Print newline to terminal
				usartSendString("\r\n");

				// Check if the first 6 characters are "delay "
				if (memcmp(buf, command, cmd_len) == 0) {
					// Convert last part to positive integer (negative int crashes)
					char* tail = userinput + cmd_len;
					t = atoi(tail);
					t = abs(t);

					// Send integer to other task via queue
					if (xQueueSend(delay_queue, &t, 10) != pdTRUE) {
						usartSendString("ERROR: Could not put item on delay queue.");
					}
				}

				// Reset receive buffer and index counter
				memset(buf, 0, buf_len);
				idx = 0;

			} 
			// Otherwise, echo character back to serial terminal
			else {
				Serial.print(c);
			}
		} 
	}
		
	vTaskDelay( 50 / portTICK_PERIOD_MS );
	
}

static void LEDTask(void *pvParameters) // print to the terminal
{
	//setup
	DDRB |= (1<<2);	
	int t = 500; //initial delay time
	message msg;
	int counter = 0;
	
	while(1)
	{
		// See if there's a message in the queue (do not block)
		if (xQueueReceive(delay_queue, &t, 0) == pdTRUE) {
			// Best practice: use only one task to manage serial comms
			sprintf(msg.body, "message received\r\n");
			msg.count = 1;
			xQueueSend(msg_queue, &msg, 10);
		}
		//blink led with delay time t
		ledblink(msg, t);
	}
}

/*-----------------------------------------------------------*/
// function that tracks the number of blinks and prints to usart every 100 blinks
void ledblink(message msg,int counter int delay)
{
	PORTB |=  (1<<2); //LED on
	vTaskDelay( delay / portTICK_PERIOD_MS );
	PORTB &= ~(1<<2); //LED off
	vTaskDelay( delay / portTICK_PERIOD_MS );
	counter++;
	//every 100 blinks send message to delay_queue
	if(counter == 100)
	{
		msg.count = counter;
		sprintf(msg.body, "blinked: \r\n");
		xQueueSend(msg_queue, &msg, portMAX_DELAY);
		counter = 0;
	}
}