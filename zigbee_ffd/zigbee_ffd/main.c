/*
 * zigbee_ffd.c
 *
 * Created: 2016-04-10 16:04:32
 * Author : Maciek
 */ 

#include <main.h>

ISR(BADISR_vect)
{
	// Handling unexpected interrupt (interrupt is enabled and no handler is installed)
}

int main(void)
{
	DDRG |= (1 << 2);

    /* Replace with your application code */
    while (1) 
    {
    }
}

