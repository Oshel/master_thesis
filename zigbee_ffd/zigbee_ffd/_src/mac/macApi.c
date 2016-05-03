/*
 * macApi.c
 *
 * Created: 2016-04-21 22:24:24
 *  Author: Maciek
 */ 

#include <main.h>

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacApiRequestConfirm (void * request, void * confirm, eMacRequestType requestType, eMacConfirmType confirmType)
{
	if (requestType == macRequestTypeScan)
	{
		tMacRequestScan *requestPointer = request;
		tMacConfirmScan *confirmPointer = confirm;
		
		uint32_t i, j;

		confirmPointer->status = macScanStatusScanInProgress;
		confirmPointer->ResultListSize = 0;

		if (requestPointer->ScanType == macScanTypeEd)
		{
			fPhyTrxSetSuspendReceiving(true);
			for (i = 0x800, j = 0; i <= 0x4000000; i = i << 1, j++)
			{
				if (i & requestPointer->ScanChannels)
				{
					fPhyTrxSetChannel((eTrxChannel) j);											// Change channel
					fPhyTrxStateCheck();														// Update the state
					fPhyTrxStateTransition(fPhyTrxStateCheck(), trxStateRxOn, true, false);		// Force TrxOff state and go to RxOn
					System.phy.flags.txCcaEdDone = false;										// Clear the flag
					PHY_ED_LEVEL = 0xFF;														// A manual ED measurement can be initiated by a write access to this register
					while (System.phy.flags.txCcaEdDone == false);								// Wait for scan to end
					confirmPointer->EnergyDetectList[j] = fPhyTrxReadEd();						// Take the result

					confirmPointer->ResultListSize++;
				}
			}
			fPhyTrxSetSuspendReceiving(false);

			confirmPointer->status = macScanStatusSuccess;

		} else if (requestPointer->ScanType == macScanTypeActive)
		{			
			fPhyTrxSetSuspendReceiving(true);			// Suspend the receiving to not get any frame while flushing
			fPhyFifoFlush();							// Clear the FIFO
			uint16_t savePan = System.mac.pib.macPANId;	// Save current PAN
			fPhyTrxSetAddPan(0xFFFF);					//  MAC sublayer shall store the value of macPANId and
														// then set it to 0xffff for the duration of the scan. This enables the receive filter to accept all beacons rather
														// than just the beacons from its current PAN, as described in 5.1.6.2
			fPhyTrxSetSuspendReceiving(false);			// Start receiving

			fMacPrepareBeaconRequest();					// Prepare Beacon Request message (MHR and Payload, add to tx buffer)
			fPhyTrxMessageSend(System.phy.txBuf, macAckRequestNo, System.phy.txBufLength);		// Send the message

			// This is from specs:
			//System.TimeOutMs.test = MAC_CONST_aBaseSuperframeDuration * ((2 ^ (requestPointer->ScanDuration)) + 1); in symbols
			// But we are using data in ms from argument:
			System.TimeOutMs.scanDuration = requestPointer->ScanDuration;

			// Pointer for retrive info from FIFO
			tPhyFifoMessage* pointer;

			// Perform scan for scanDuration ms and until buffer is full (we colected 10 beacons)
			while (System.TimeOutMs.scanDuration && (confirmPointer->ResultListSize < 10))
			{
				if (fPhyFifoTakeCheck() == true)								// We have a new message!
				{
					pointer = fPhyFifoTake();									// Get new message from FIFO
					if (fMacGetFcfFrameType((uint8_t *) pointer) == macFrameTypeBeacon)		// Check if it is a beacon frame, otherwise discard
					{
						// Get energy
						//confirmPointer->EnergyDetectList[confirmPointer->ResultListSize] = pointer->phyFifoMessageEd;
						// Get LQI
						confirmPointer->EnergyDetectList[confirmPointer->ResultListSize] = pointer->phyFifoMessageLqi;

						// Get PAN
						confirmPointer->PANDescriptorList[confirmPointer->ResultListSize] = fMacGetAfAddressPanSource((uint8_t *) pointer, macDestAddModePanAndAddFieldsNotPresent, macSrcAddModeAdd16Bit);	

						// Get Short Address
						confirmPointer->ShortAddressesDescriptorList[confirmPointer->ResultListSize] = fMacGetAfAddressShortSource((uint8_t *) pointer, macDestAddModePanAndAddFieldsNotPresent, macSrcAddModeAdd16Bit);

						confirmPointer->ResultListSize++;

						tMacIndicationBeaconNotify indication;

						indication.AddrList = confirmPointer->ShortAddressesDescriptorList[confirmPointer->ResultListSize];
						//indication.PANDescriptor.CoordAddress = 
						indication.PANDescriptor.CoordPANId = confirmPointer->PANDescriptorList[confirmPointer->ResultListSize];
						indication.PANDescriptor.LinkQuality = confirmPointer->EnergyDetectList[confirmPointer->ResultListSize];

						fMacApiIndicationResponse(&indication, 0, macIndicationTypeBeaconNotify, macResponseTypeNone);
					}
				}
			}

			fPhyTrxSetSuspendReceiving(true);			// Suspend the receiving to not get any frame while flushing 
			fPhyFifoFlush();							// Clear the FIFO
			fPhyTrxSetAddPan(savePan);					// Get back to previous PAN
			fPhyTrxSetSuspendReceiving(false);			// Start receiving again

			if (confirmPointer->ResultListSize == 0)
			{
				confirmPointer->status = macScanStatusNoBeacon;
			} else {
				confirmPointer->status = macScanStatusSuccess;
			}
		}
	} else if (requestType == macRequestTypeAssociate)
	{
		tMacRequestAssociate *requestPointer = request;
		tMacConfirmAssociate *confirmPointer = confirm;
	}
}