/*
 * appSystemTimer.c
 *
 * Created: 2016-04-11 14:49:46
 *  Author: Maciek
 */ 
 
 #include "main.h"

 // Local definitions
 // #define DEFINITION



 // Local variables declarations
 // uint8_t NameVariable;



// Functions declarations
// void nameFunction(uint8_t variableName);

void fAppSystemTimerInit (void);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppSystemTimerInit (void)
{
	TCCR0B |= (0x03 << 0);		// /64 clock
	TCCR0A |= (0x02 << 0);
	OCR0A = 125;
	TIMSK0 |= (0x01 << OCIE0A);	// overflow interrupt
}