/*
 * zigbee_ffd.c
 *
 * Created: 2016-04-10 16:04:32
 * Author : Maciek
 */ 

#include "main.h"

tSystem System;

int main(void)
{
	//tMacMessage msg;
	//uint8_t buf[3] = "ALA";

	//msg.macFcf.macFrameType = macFrameTypeData;
	//msg.macFcf.macSecurity = macSecurityDisabled;
	//msg.macFcf.macFramePending = macFramePendingNo;
	//msg.macFcf.macAckRequest = macAckRequestNo;
	//msg.macFcf.macIntraPan = macIntraPanNo;
	//msg.macFcf.macDestAddMode = macDestAddModeAdd16Bit;
	//msg.macFcf.macFrameVersion = macFrameVersionIeee2006;
	//msg.macFcf.macSrcAddMode = macSrcAddModeAdd16Bit;
	//msg.macDestPan = 0x0002;
	//msg.macDestAdd = 0x0002;
	//msg.macSrcPan = 0x0001;
	//msg.macSrcAdd = 0x0001;

	DDRG |= (1 << 2);

	fAppSystemTimerInit();
	fPhyTrxInit();

	fPhyTrxStateCheck();
	fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStateRxAackOn, false, false);

	sei();

    while (1) 
    {
		if (fPhyFifoTakeCheck())
		{
			tPhyFifoMessage* pointer = fPhyFifoTake();
			DDRG ^= (1 << 2);
		}
		
    }
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

uint16_t fMainGetRandomShort (void)
{
	return System.TimeOutMs.random;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMainMoveAndAttach (uint8_t * destination,
	uint8_t * source,
	uint8_t lengthDestination,
	uint8_t lengthSource)
{
	// Shift destination by lengthSource 
	memmove((void *)(destination + lengthSource), (void *) destination, lengthDestination);

	// Copy source to destnation
	memcpy((void *) destination, (void *) source, lengthSource);
}

ISR(BADISR_vect)
{
	// Handling unexpected interrupt (interrupt is enabled and no handler is installed)
}