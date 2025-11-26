/**
 * @file main.c
 * 
 * @author Tiago Lobao
 * 
 * @brief Blink example based on the example for atmega328p
 * https://github.com/feilipu/avrfreertos/blob/master/UnoBlink/main.c
 *
 * Modified BT 05/12/24
 */

#define F_CPU 1000000UL

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <util/delay.h>

/*-----------------------------------------------------------*/
static void TaskReadTemp(void *pvParameters); 
static void TaskAlarm(void *pvParameters);
/*-----------------------------------------------------------*/

uint8_t iTemperatures[2] = {0}; 

SemaphoreHandle_t tempSem; // accessed by both tasks

int main(void)
{
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
	while(1)
	{
		xSemaphoreTake( tempSem, portMAX_DELAY); //have semaphore available forever
		iTemperatures[0]++;
		iTemperatures[1]++;
		xSemaphoreGive(tempSem);
		
		PORTB ^= (1<<0);
		
		vTaskDelay( 123 / portTICK_PERIOD_MS );
	}
}

static void TaskAlarm(void *pvParameters) // LED Flash
{
	uint8_t temp1, temp2;

	DDRB |= (1<<2);

 	while(1)
 	{
		xSemaphoreTake( tempSem, portMAX_DELAY); //have semaphore available forever
 	
		temp1 = iTemperatures[0];
		vTaskDelay( 550 / portTICK_PERIOD_MS );
		temp2 = iTemperatures[1];

		xSemaphoreGive(tempSem);


		if(temp1 != temp2)
		{
			PORTB |=  (1<<2); //LED on
		}
		else
		{
			PORTB &= ~(1<<2); //LED off.
		}
		
 		vTaskDelay( 200 / portTICK_PERIOD_MS );
 	}
}
/*---------------------------------------------------------------------------*/

