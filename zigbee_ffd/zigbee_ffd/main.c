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
	fAppSystemTimerInit();
	
	// 3.5dBm Tx Power
	// 11 Channel, 2.405 GHz
	// Short add = 0x0001
	// PAN ID = 0x0001
	fPhyTrxInit();

	fPhyTrxStateCheck();
	fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStateRxAackOn, false, false);

	sei();

	//DEBUG
	DDRD &= ~(1 << 6);
	PORTD |= (1 << 6);
	//GUBED

	{
		tNwkRequestNetworkDiscovery networkDiscoveryRequest;
		tNwkConfirmNetworkDiscovery networkDiscoveryConfirm;
		networkDiscoveryRequest.ScanChannels = (0xFFFFFFFF) & (1 << 11);
		networkDiscoveryRequest.ScanDuration = 100;

		fNwkApiRequestConfirm(&networkDiscoveryRequest, &networkDiscoveryConfirm, nwkRequestNetworkDiscovery, nwkConfirmNetworkDiscovery);

		if (networkDiscoveryConfirm.Status == nwkStartStatusSuccess)
		{
			// Join

			tNwkRequestJoin joinRequest;
			tNwkConfirmJoin joinConfirm;
			
			joinRequest.CapabilityInformation.allocateAddress = System.nwk.nib.nwkCapabilityInformation.allocateAddress;
			joinRequest.CapabilityInformation.deviceType = System.nwk.nib.nwkCapabilityInformation.deviceType;
			joinRequest.CapabilityInformation.powerSource = System.nwk.nib.nwkCapabilityInformation.powerSource;
			joinRequest.CapabilityInformation.receiverOnWhenIdle = System.nwk.nib.nwkCapabilityInformation.receiverOnWhenIdle;
			joinRequest.CapabilityInformation.securityCapability = System.nwk.nib.nwkCapabilityInformation.securityCapability;

			joinRequest.ScanChannels = (0xFFFFFFFF) & (1 << 11);

			joinRequest.ScanDuration = 10;

			// Search for highest LQI PAN
			uint8_t i, highestLqi;
			for (i = 0; i < networkDiscoveryConfirm.NetworkCount; i++)
			{	
				if (i == 0)
				{
					highestLqi = 0;
				} else {
					if (networkDiscoveryConfirm.networkDescriptor[i].Lqi > networkDiscoveryConfirm.networkDescriptor[highestLqi].Lqi)
					{
						highestLqi = i;
					}
				}
			}

			// Join to the PAN with highest LQI
			joinRequest.PANId = networkDiscoveryConfirm.networkDescriptor[highestLqi].PANId;
			
			fNwkApiRequestConfirm(&joinRequest, &joinConfirm, nwkRequestJoin, nwkConfirmJoin);

			// TODO ASSOCIATION PROCEDURE

			tNwkRequestStartRouter startRouterRequest;
			tNwkConfirmStartRouter startRouterConfirm;

			fNwkApiRequestConfirm(&startRouterRequest, &startRouterConfirm, nwkRequestStartRouter, nwkConfirmStartRouter);

			DDRG &= (1 << 2);

		} else if (networkDiscoveryConfirm.Status == nwkScanStatusNoBeacon)
		{
			// Create network

			tNwkRequestNetworkFormation networkFormationRequest;
			tNwkConfirmNetworkFormation networkFormationConfirm;

			networkFormationRequest.ScanChannels = (0xFFFFFFFF) & (1 << 11);
			networkFormationRequest.ScanDuration = 10;

			fNwkApiRequestConfirm(&networkFormationRequest, &networkFormationConfirm, nwkRequestNetworkFormation, nwkConfirmNetworkFormation);

			DDRG |= (1 << 2);
		}
	}

    while (1) 
    {
		if (fPhyFifoTakeCheck())
		{
			tPhyFifoMessage* pointer = fPhyFifoTake();
			macReceiveMessage(pointer, pointer->phyFifoMessageLength, 0);
		}
		
		//if ((PIND & (1 << 6)) == 0 && System.TimeOutMs.test == 0)
		//{
			//System.TimeOutMs.test = 500;
			//DDRG ^= (1 << 2);
//
			//tNwkRequestNetworkDiscovery networkDiscoveryRequest;
			//tNwkConfirmNetworkDiscovery networkDiscoveryConfirm;
			//networkDiscoveryRequest.ScanChannels = (0xFFFFFFFF) & (1 << 11);
			//networkDiscoveryRequest.ScanDuration = 100;
			//
			//fNwkApiRequestConfirm(&networkDiscoveryRequest, &networkDiscoveryConfirm, nwkRequestNetworkDiscovery, nwkConfirmNetworkDiscovery);
		//}
		
		//if (System.TimeOutMs.test == 0)
		//{
			//System.TimeOutMs.test = 1000;
//
			//tMacCapabilityInformation capInfo;
//
			//capInfo.allocateAddress = true;
			//capInfo.deviceType = true;
			//capInfo.powerSource = false;
			//capInfo.receiverOnWhenIdle = true;
			//capInfo.securityCapability = true;
//
			//fMacPrepareAssociationRequest(0x0001, 0x0001, &capInfo);
			//fPhyTrxMessageSend(System.phy.txBuf, macAckRequestYes, System.phy.txBufLength);
//
			//DDRG ^= (1 << 2);
		//}
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
	return System.randomNumber;
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