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

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/
static void TaskBlinkLED(void *pvParameters);
/*-----------------------------------------------------------*/

int main(void)
{
    	xTaskCreate(
		TaskBlinkLED
		,  (const char *)"GreenLED"
		,  256
		,  NULL
		,  3
		,  NULL );


	vTaskStartScheduler();    //This never returns... control handed to the RTOS
}

/*-----------------------------------------------------------*/

static void TaskBlinkLED(void *pvParameters) // LED Flash
{
    	(void) pvParameters;  //Just keeping the compiler happy...

	DDRB |= (1<<5);

    	while(1)
    	{
    		PORTB |=  (1<<5); //LED on

		vTaskDelay( 500 / portTICK_PERIOD_MS );

		PORTB &= ~_BV(PORTB5); //LED off.
		vTaskDelay( 500 / portTICK_PERIOD_MS );
    	}
}
/*---------------------------------------------------------------------------*/
