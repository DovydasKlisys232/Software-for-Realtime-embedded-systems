/*
 * usart.h
 *
 *  Created on: 15 Mar 2019
 *      Author: btoland
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>  //for uint_8

/*****************************************************
 * This function should set up the USART for 9600 8N1
 ****************************************************/
void usartInit(void);

/*****************************************************
 * This function should simply set the bit that
 * enables the USART to generate an interrupt on rx
 * of a byte
 ****************************************************/
void usartEnableRxInt(void);

/*****************************************************
 * This function should send a single char and wait
 * until the transmission is complete.
 ****************************************************/
void usartSendChar(char c);

/*****************************************************
 * This function should send a string of chars and 
 * wait until the transmission is complete.
 ****************************************************/
void usartSendString(char []);

/*****************************************************
 * This function should return a 1 if a char has been
 * received and a 0 if not. 
 ****************************************************/
uint8_t usartCharReceived(void);

/*****************************************************
 * This function should simply return the received 
 * char by copying it from the UDR register.
 *
 * N.B. Thus function should only be called after 
 * a call to usartCharReceived() returns a 1.
 ****************************************************/
char usartReadChar(void);

#endif /* USART_H_ */
