/*
 * systemTimerInterrupts.c
 *
 * Created: 2016-04-11 15:25:19
 *  Author: Maciek
 */ 

#include "main.h"

 /**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TIMER0_COMPA_vect)
{
	uint8_t i;
	uint16_t *pointer;

	pointer = (uint16_t*) &System.TimeOutMs;

	for (i = sizeof(System.TimeOutMs); i > 0; i = i - 2, pointer++) 
	{
		if (*pointer > 0)
			*pointer = *pointer - 1;
	}
}