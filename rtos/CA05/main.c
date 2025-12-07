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

/*-----------------------------------------------------------*/
// Globals
static QueueHandle_t delay_queue;
static QueueHandle_t msg_queue;
/*-----------------------------------------------------------*/


int main(void)
{
	//setup usart and enable interrupts
	usartInit();
	
   	xTaskCreate(printTask, NULL, 256, NULL, 1, NULL);
	xTaskCreate(LEDTask, NULL, 256, NULL, 1, NULL);
		
	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

/*-----------------------------------------------------------*/