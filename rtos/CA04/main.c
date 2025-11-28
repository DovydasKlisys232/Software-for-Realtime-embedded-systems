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
#include "usart.h"

#include <util/delay.h>
#include<avr/interrupt.h>
#include<stdio.h> //for sprintf

/*-----------------------------------------------------------*/
static void TaskReadTemp(void *pvParameters); 
static void TaskAlarm(void *pvParameters);
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
uint8_t iTemperatures[2] = {0}; 
SemaphoreHandle_t tempSem; // accessed by both tasks
/*-----------------------------------------------------------*/
// this isr gives a semaphore to a task
// if the waiting task is higher priority than the running task the isr will switch tasks
ISR(INT0_vect)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(tempSem);
	portYIELD_FROM_ISR();
}

int main(void)
{
	//setup usart and enable interrupts
	usartInit();
	interrupt_init();
	sei();

	//create semaphore
	tempSem = xSemaphoreCreateBinary();
	xSemaphoreGive(tempSem);
	
   	xTaskCreate(TaskReadTemp, NULL, 256, NULL, 3, NULL);
	xTaskCreate(TaskAlarm, NULL, 256, NULL, 2, NULL);
		
	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

/*-----------------------------------------------------------*/

static void TaskReadTemp(void *pvParameters) // LED Flash
{

	DDRB |= (1<<0);	
	char str1[20];
	char str2[20];
	
	while(1)
	{
		xSemaphoreTakeFromISR( tempSem, portMAX_DELAY); //have semaphore available forever
		
		iTemperatures[0]++;
		sprintf(str1, "Temp1: %u\r\n", iTemperatures[0]);
		usartSendString(str1);
		
		iTemperatures[1]++;
		sprintf(str2, "Freq: %u\r\n", iTemperatures[1]);
        usartSendString(str2);
        
		xSemaphoreGive(tempSem);
		
		PORTB ^= (1<<0);
		
		vTaskDelay( 123 / portTICK_PERIOD_MS );
	}
}

static void TaskAlarm(void *pvParameters) // LED Flash
{
	uint8_t temp1, temp2;

	DDRB |= (1<<2);
	char str[20];

 	while(1)
 	{
		xSemaphoreTakeFromISR( tempSem, portMAX_DELAY); //have semaphore available forever
 	
		temp1 = iTemperatures[0];
		vTaskDelay( 550 / portTICK_PERIOD_MS );
		temp2 = iTemperatures[1];

		xSemaphoreGive(tempSem);


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
