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
#include "semphr.h"

#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include "usart.h"

// Some string to print
const char msg[] = "Barkadeer brig Arr booty rum.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//*****************************************************************************
// Tasks

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter) {

  // Count number of characters in string
  int msg_len = strlen(msg);

  // Print string to Terminal
  while (1) {
    usartSendChar('\n');
    for (int i = 0; i < msg_len; i++) {
      usartSendChar(msg[i]);
    }
    usartSendChar('\n');
    _delay_ms(1000);
  }
}

// Task: print to Serial Terminal with higher priority
void startTask2(void *parameter) {
  int delayTime = 0;
  while (1) {
    usartSendChar('*');
    delayTime = rand()%1000;
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

int main(void)
{
    usartInit(300);

    // Wait a moment to start (so we don't miss Serial output)
    _delay_ms(1000);

    usartSendString("\n---FreeRTOS Task Demo---\n");


        xTaskCreate(startTask1,  NULL,  256,  NULL,  1,  NULL );

        xTaskCreate(startTask2, NULL,  256,  NULL,  2,  NULL );
    

    vTaskStartScheduler();    //This never returns... control handed to the RTOS
}