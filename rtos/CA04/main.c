/*
Student: Dovudas Klisys
ID: B00165094
Date: 28/11/25
Purpose: this is the main code for CA04. it reimplements the nuclear reactor code but this
time the rtos libraries are used to split the functions into tasks and include an ISR
which gives the semaphore to a specific task when interrupt occurs.
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
static void TaskReadTemp(void *pvParameters); 
static void TaskAlarm(void *pvParameters);
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
// i have an array to store the two temperature values. this is a global variable which can cause race conditions
// using a semaphore to protect access to this variable
// i have two semaphores, one for the increment task and one for the alarm task
uint8_t iTemperatures[2] = {0}; 
SemaphoreHandle_t incrementSem; // accessed by both tasks
SemaphoreHandle_t alarmSem; // accessed by both tasks
SemaphoreHandle_t usartmutex; // to protect usart access 
/*-----------------------------------------------------------*/
void interrupt_init();

// this isr gives a semaphore to a task when an interrupt occurs on pd2
ISR(INT0_vect)
{
	xSemaphoreGiveFromISR(incrementSem, NULL);
}

int main(void)
{
	//setup usart and enable interrupts
	usartInit();
	interrupt_init();
	sei();

	//create semaphores
	incrementSem = xSemaphoreCreateBinary();
	alarmSem = xSemaphoreCreateBinary();
	usartmutex = xSemaphoreCreateMutex();

	//make semaphore available at start. this ensures the low priority task can run first
	xSemaphoreGive(alarmSem); 
	xSemaphoreGive(usartmutex);
	
   	xTaskCreate(TaskReadTemp, NULL, 256, NULL, 3, NULL);
	xTaskCreate(TaskAlarm, NULL, 256, NULL, 2, NULL);
		
	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

/*-----------------------------------------------------------*/

static void TaskReadTemp(void *pvParameters) // LED Flash
{
	//setup
	DDRB |= (1<<0);	
	char str1[20];
	char str2[20];
	
	while(1)
	{
		xSemaphoreTake( incrementSem, portMAX_DELAY); //have semaphore available forever
		//if first take fails it is blocked, even if there is another take after
		xSemaphoreTake( alarmSem, portMAX_DELAY); //blocks low priority task if high priority task has the semaphore
		
		iTemperatures[0]++;
		iTemperatures[1]++;

		//printing out temperature values
		//using mutex to protect usart access
		xSemaphoreTake(usartmutex, portMAX_DELAY);
		sprintf(str1, "Temp1: %u\r\n", iTemperatures[0]);
		usartSendString(str1);
		sprintf(str2, "Temp2: %u\r\n", iTemperatures[1]);
        usartSendString(str2);
		xSemaphoreGive(usartmutex);
		
		PORTB ^= (1<<0);

		xSemaphoreGive(alarmSem);
		
		vTaskDelay( 50 / portTICK_PERIOD_MS );
	}
}

static void TaskAlarm(void *pvParameters) // LED Flash
{
	//setup
	uint8_t temp1, temp2;

	DDRB |= (1<<2);
	
	char str[20];

 	while(1)
 	{
		xSemaphoreTake( alarmSem, portMAX_DELAY); //have semaphore available forever
		temp1 = iTemperatures[0];
		temp2 = iTemperatures[1];
		xSemaphoreGive(alarmSem);

		//using mutex to protect usart access
		xSemaphoreTake(usartmutex, portMAX_DELAY);
		if(temp1 != temp2)
		{
			PORTB |=  (1<<2); //LED on
			sprintf(str, "Alarm on\r\n");
	        usartSendString(str);
		}
		else
		{
			PORTB &= ~(1<<2); //LED off.
			sprintf(str, "Alarm off\r\n");
	        usartSendString(str);
		}
		xSemaphoreGive(usartmutex);
	
 		vTaskDelay( 200 / portTICK_PERIOD_MS );
 	}
}
/*---------------------------------------------------------------------------*/

//initialize the pin pd2 for input capture interrupt
void interrupt_init()
{
	DDRD &= ~(1<<2); //set pd2 as an input
	PORTD |= (1 << 2); //enable internal pull up resistor on pd2
	EICRA |= ((1 << ISC00) | (1 << ISC01)); //interrupt on rising edge
    EIMSK |= (1 << INT0); //interrupt pin enable
}
