
#ifndef SETUP_H_
#define SETUP_H_

#include <avr/io.h>

//initialize the pin pd2 for input capture interrupt
void interrupt_init();

//initialize the clock for ctc
void clock_init();
 
#endif /* SETUP_H_ */
