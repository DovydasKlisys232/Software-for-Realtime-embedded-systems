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
#include<string.h> //for strlen, memcmp
#include<stdlib.h> //for atoi, abs

/*-----------------------------------------------------------*/
static void printTask(void *pvParameters); 
static void LEDTask(void *pvParameters);
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
// Globals
static QueueHandle_t delay_queue;
static QueueHandle_t msg_queue;

static const int msg_queue_len = 5;
static const int delay_queue_len = 5; 
static const uint8_t buf_len = 255; //length of buffer to store user input
static const char command[] = "delay "; //store delay command string from user for parsing later

typedef struct Message {
  char body[20];
  int count;
} Message;

void ledblink(Message msg,int count, int delay);
/*-----------------------------------------------------------*/


int main(void)
{
	//setup usart 
	usartInit(9600);
	
	//create queues
	delay_queue = xQueueCreate(delay_queue_len, sizeof(int)); //queue to hold delay times
	msg_queue = xQueueCreate(msg_queue_len, sizeof(Message)); //queue to hold messages

	//same priority so no higher priority task doesn't block the other
   	xTaskCreate(printTask, NULL, 256, NULL, 1, NULL);
	xTaskCreate(LEDTask, NULL, 256, NULL, 1, NULL);
		
	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

static void printTask(void *pvParameters) // print to the terminal
{
	Message rcv_msg; //stores received message from queue
	int t; //delay time entered by user
	char ch; //read in character
	char userinput[buf_len]; //buffer to store user input
	uint8_t idx = 0; //index for user input buffer
	uint8_t cmd_len = strlen(command); //size of command string ("delay ")
	char str[20]; //buffer to store value that is sent to msg_queue

	// Clear whole buffer
  	memset(userinput, 0, buf_len);	
	
	while(1)
	{
		
		//read messages from msg_queue (outputs: blinked! and count) or (outputs: message received)
		if (xQueueReceive(msg_queue, &rcv_msg, 0) == pdTRUE) {
			usartSendString(rcv_msg.body);
			usartSendString("\n");
			sprintf(str, "%d\r\n", rcv_msg.count);
			usartSendString(str);
		}

		if(usartCharReceived())
		{
			ch = usartReadChar(); //read char from usart

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
				if (memcmp(userinput, command, cmd_len) == 0) {
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
				memset(userinput, 0, buf_len);
				idx = 0;

			} 
			// Otherwise, echo character back to serial terminal
			else {
				usartSendChar(ch);
			}
		} 
		vTaskDelay(50 / portTICK_PERIOD_MS ); //short delay to prevent task hogging CPU
	}
}

static void LEDTask(void *pvParameters) // print to the terminal
{
	//setup
	DDRB |= (1<<2);	
	int t = 500; //initial delay time
	Message msg;
	int counter = 0;
	
	while(1)
	{
		// See if there's a message in the queue (do not block)
		if (xQueueReceive(delay_queue, &t, 0) == pdTRUE) {
			sprintf(msg.body, "message received\r\n");
			msg.count = 1;
			xQueueSend(msg_queue, &msg, 10);
		}
		//blink led with delay time t
		//ledblink(msg,counter,t);
		PORTB |=  (1<<2); //LED on
		vTaskDelay( t / portTICK_PERIOD_MS );
		PORTB &= ~(1<<2); //LED off
		vTaskDelay( t / portTICK_PERIOD_MS );
		counter++;
		//every 100 blinks send message to delay_queue
		if(counter == 100)
		{
			msg.count = counter;
			sprintf(msg.body, "blinked: ");
			xQueueSend(msg_queue, &msg, portMAX_DELAY);
			counter = 0;
		}
	}
}

/*-----------------------------------------------------------*/
// function that tracks the number of blinks and prints to usart every 100 blinks
void ledblink(Message msg,int count, int delay)
{
	PORTB |=  (1<<2); //LED on
	vTaskDelay( delay / portTICK_PERIOD_MS );
	PORTB &= ~(1<<2); //LED off
	vTaskDelay( delay / portTICK_PERIOD_MS );
	count++;
	//every 100 blinks send message to delay_queue
	if(count == 100)
	{
		msg.count = count;
		sprintf(msg.body, "blinked: \r\n");
		xQueueSend(msg_queue, &msg, portMAX_DELAY);
		count = 0;
	}
}