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
	tMacMessage msg;
	uint8_t buf[3] = "ALA";

	msg.macFcf.macFrameType = macFrameTypeData;
	msg.macFcf.macSecurity = macSecurityDisabled;
	msg.macFcf.macFramePending = macFramePendingNo;
	msg.macFcf.macAckRequest = macAckRequestNo;
	msg.macFcf.macIntraPan = macIntraPanNo;
	msg.macFcf.macDestAddMode = macDestAddModeAdd16Bit;
	msg.macFcf.macFrameVersion = macFrameVersionIeee2006;
	msg.macFcf.macSrcAddMode = macSrcAddModeAdd16Bit;
	msg.macDestPan = 0x0002;
	msg.macDestAdd = 0x0002;
	msg.macSrcPan = 0x0001;
	msg.macSrcAdd = 0x0001;

	fAppTrxMessageSend(&msg, buf, sizeof(buf));

	DDRG |= (1 << 2);

	fAppSystemTimerInit();
	fAppTrxInit();

	sei();

    /* Replace with your application code */
    while (1) 
    {
		if (System.TimeOutMs.test == 0)
		{
			System.TimeOutMs.test = 1000;
			DDRG ^= (1 << 2);
			fAppTrxMessageSend(&msg, buf, sizeof(buf));
		}
    }
}

ISR(BADISR_vect)
{
	// Handling unexpected interrupt (interrupt is enabled and no handler is installed)
}