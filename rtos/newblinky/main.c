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
#include <util/delay.h>

/*-----------------------------------------------------------*/
static void TaskBlinkGreenLED(void *pvParameters); 
static void TaskBlinkRedLED(void *pvParameters);
/*-----------------------------------------------------------*/

int main(void)
{
   	xTaskCreate(TaskBlinkGreenLED, NULL, 256, NULL, 3, NULL);
	xTaskCreate(TaskBlinkRedLED, NULL, 256, NULL, 3, NULL);
		

	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

/*-----------------------------------------------------------*/

static void TaskBlinkGreenLED(void *pvParameters) // LED Flash
{
    	(void) pvParameters;  //Just keeping the compiler happy...

	DDRB |= (1<<0);

    	while(1)
    	{
    		PORTB |=  (1<<0); //LED on

		vTaskDelay( 333 / portTICK_PERIOD_MS );
		_delay_ms(333);

		PORTB &= ~(1<<0); //LED off.
		vTaskDelay( 333 / portTICK_PERIOD_MS );
		_delay_ms(333);
    	}
}

static void TaskBlinkRedLED(void *pvParameters) // LED Flash
{
    	(void) pvParameters;  //Just keeping the compiler happy...

	DDRB |= (1<<2);

    	while(1)
    	{
    		PORTB |=  (1<<2); //LED on

		vTaskDelay( 333 / portTICK_PERIOD_MS );
		_delay_ms(333);

		PORTB &= ~(1<<2); //LED off.
		vTaskDelay( 333 / portTICK_PERIOD_MS );
		_delay_ms(333);
    	}
}
/*---------------------------------------------------------------------------*/
