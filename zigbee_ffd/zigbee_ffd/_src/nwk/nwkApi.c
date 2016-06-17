/*
 * nwkApi.c
 *
 * Created: 2016-05-03 15:42:22
 *  Author: Maciek
 */ 

#include <main.h>

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fNwkApiRequestConfirm (
	void * request, 
	void * confirm, 
	eNwkRequestType requestType,
	eNwkConfirmType confirmType)
{
	if (requestType == nwkRequestNetworkDiscovery)
	{
		tNwkRequestNetworkDiscovery * requestPointer = request;
		tNwkConfirmNetworkDiscovery * confirmPointer = confirm;

		tMacRequestScan scanRequest;
		tMacConfirmScan scanConfirm;

		scanRequest.ScanChannels = requestPointer->ScanChannels;
		scanRequest.ScanDuration = requestPointer->ScanDuration;
		scanRequest.ScanType = macScanTypeActive;

		fMacApiRequestConfirm(&scanRequest, &scanConfirm, macRequestTypeScan, macConfirmTypeScan);

		confirmPointer->NetworkCount = scanConfirm.ResultListSize;

		if (confirmPointer->NetworkCount > 0)
		{
			uint8_t i;

			for (i = 0; i < confirmPointer->NetworkCount; i++)
			{
				confirmPointer->networkDescriptor[i].PANId = scanConfirm.PANDescriptorList[i];
				confirmPointer->networkDescriptor[i].ShortAdd = scanConfirm.ShortAddressesDescriptorList[i];
				confirmPointer->networkDescriptor[i].Lqi = scanConfirm.EnergyDetectList[i];
			}
		}

		if (confirmPointer->NetworkCount == 0)
		{
			confirmPointer->Status = nwkScanStatusNoBeacon;
		}

		confirmPointer->Status = scanConfirm.status;

	} else if (requestType == nwkRequestNetworkFormation)
	{
		tNwkRequestNetworkFormation * requestPointer = request;
		tNwkConfirmNetworkFormation * confirmPointer = confirm;

		uint8_t selectedChannel;

		{
			tMacRequestScan scanRequest;
			tMacConfirmScan scanConfirm;

			scanRequest.ScanChannels = requestPointer->ScanChannels;
			scanRequest.ScanDuration = requestPointer->ScanDuration;
			scanRequest.ScanType = macScanTypeEd;

			fMacApiRequestConfirm(&scanRequest, &scanConfirm, macRequestTypeScan, macConfirmTypeScan);

			// Look for channel with lowest ED
			uint8_t actual, lowest;
		
			if (scanConfirm.ResultListSize > 0)
			{
				for (actual = 0; actual < scanConfirm.ResultListSize; actual++)
				{
					if (actual == 0)
					{
						lowest = 0;
					} else {
						if (scanConfirm.EnergyDetectList[actual] < scanConfirm.EnergyDetectList[lowest])
						{
							lowest = actual;
						}
					}
				}
			}

			selectedChannel = scanConfirm.ShortAddressesDescriptorList[lowest];			// Take channel number with lowest ED
		}

		{
			tMacRequestScan scanRequest;
			tMacConfirmScan scanConfirm;

			scanRequest.ScanChannels = (0xFFFFFFFF) & (1 << selectedChannel);
			scanRequest.ScanDuration = requestPointer->ScanDuration;
			scanRequest.ScanType = macScanTypeActive;

			fMacApiRequestConfirm(&scanRequest, &scanConfirm, macRequestTypeScan, macConfirmTypeScan);

			uint16_t panIdCandidate;
			uint8_t i;
			bool panIdFree = true;

			// Find Free PAN ID number

			do
			{
				panIdFree = true;
				panIdCandidate = System.randomNumber;

				for (i = 0; i < scanConfirm.ResultListSize; i++)
				{
					if (scanConfirm.PANDescriptorList[i] == panIdCandidate)
					{
						panIdFree = false;
						break;
					}
				}

				if (panIdCandidate == 0)
				{
					panIdFree = false;			// PAN ID = 0 is forbidden
				}

			} while (panIdFree == false);
			
			// Set MAC PAN ID

			{
				tMacRequestSet setRequest;
				tMacConfirmSet setConfirm;

				setRequest.PIBAttributeNamePointer = "macPANId";
				setRequest.PIBAttributeValue = &panIdCandidate;

				fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);
			}

			// Set MAC Short Address

			{
				tMacRequestSet setRequest;
				tMacConfirmSet setConfirm;

				setRequest.PIBAttributeNamePointer = "macShortAddress";
				setRequest.PIBAttributeValue = &panIdCandidate;

				fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);
			}

			// Set channel

			{
				tMacRequestSet setRequest;
				tMacConfirmSet setConfirm;

				setRequest.PIBAttributeNamePointer = "phyCurrentChannel";
				setRequest.PIBAttributeValue = &selectedChannel;

				fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);
			}

			// Start the network

			{
				tMacRequestStart startRequest;
				tMacConfirmStart startConfirm;

				startRequest.ChannelNumber = System.phy.pib.phyCurrentChannel;			// Should be GET request to retrive information from MAC layer
				startRequest.PANCoordinator = true;
				startRequest.PANId = System.mac.pib.macPANId;							// Should be GET request to retrive information from MAC layer
				startRequest.shortAdd = System.mac.pib.macShortAddress;					// Should be GET request to retrive information from MAC layer

				fMacApiRequestConfirm(&startRequest, &startConfirm, macRequestTypeStart, macConfirmTypeStart);
			}
		}
		
		confirmPointer->Status = nwkStartStatusSuccess;

	} else if (requestType == nwkRequestJoin)
	{
		tNwkRequestJoin * requestPointer = request;
		tNwkConfirmJoin * confirmPointer = confirm;
		
		{
			tMacRequestAssociate associateRequest;
			tMacConfirmAssociate associateConfirm;

			// Select Channel to associate given in join request

			uint32_t i;
			uint8_t j;
			for (i = 0x800, j = 0; i <= 0x4000000; i = i << 1, j++)
			{
				if (i & requestPointer->ScanChannels)
				{
					associateRequest.ChannelNumber = (j + 11);
					break;
				}
			}

			// Select short address with given PAN ID with the highest LQI

			uint8_t highestLqi = 0xFF;
			bool selectionDone = false;
			for (i = 0; i < 10; i++)
			{
				if (System.nwk.nib.nwkNeighborTable[i].panId != 0 &&
					System.nwk.nib.nwkNeighborTable[i].networkAddress != 0)
				{
					if (System.nwk.nib.nwkNeighborTable[i].panId == requestPointer->PANId)
					{
						if (highestLqi == 0xFF)
						{
							highestLqi = i;
						} else {
							if (System.nwk.nib.nwkNeighborTable[highestLqi].lqi < System.nwk.nib.nwkNeighborTable[i].lqi)
							{
								highestLqi = i;
							}
						}
						selectionDone = true;
					}
				}
			}

			if (selectionDone == true)
			{
				// Select short address
				associateRequest.CoordAddress = System.nwk.nib.nwkNeighborTable[highestLqi].networkAddress;

				// Select PAN ID
				associateRequest.CoordPANId = requestPointer->PANId;

				// Select Capability Information
				associateRequest.CapabilityInformation.deviceType = System.nwk.nib.nwkCapabilityInformation.deviceType;
				associateRequest.CapabilityInformation.powerSource = System.nwk.nib.nwkCapabilityInformation.powerSource;
				associateRequest.CapabilityInformation.receiverOnWhenIdle = System.nwk.nib.nwkCapabilityInformation.receiverOnWhenIdle;
				associateRequest.CapabilityInformation.securityCapability = System.nwk.nib.nwkCapabilityInformation.securityCapability;
				associateRequest.CapabilityInformation.allocateAddress = System.nwk.nib.nwkCapabilityInformation.allocateAddress;
				
				fMacApiRequestConfirm(&associateRequest, &associateConfirm, macRequestTypeAssociate, macConfirmTypeAssociate);

				if (associateConfirm.status == macAssociationStatusSuccess)
				{
					tMacRequestSet setRequest;
					tMacConfirmSet setConfirm;

					setRequest.PIBAttributeNamePointer = "macPANId";
					setRequest.PIBAttributeValue = &associateRequest.CoordPANId;

					fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);

					setRequest.PIBAttributeNamePointer = "macShortAddress";
					setRequest.PIBAttributeValue = &associateConfirm.AssocShortAddress;

					fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);

					setRequest.PIBAttributeNamePointer = "phyCurrentChannel";
					setRequest.PIBAttributeValue = &highestLqi;

					fMacApiRequestConfirm(&setRequest, &setConfirm, macRequestTypeSet, macConfirmTypeSet);

					confirmPointer->ActiveChannel = highestLqi;
					confirmPointer->NetworkAddress = associateConfirm.AssocShortAddress;
					confirmPointer->PANID = requestPointer->PANId;
					confirmPointer->Status = nwkJoinStatusSuccess;
				} else if (associateConfirm.status == macAssociationStatusNoData)
				{
					confirmPointer->Status = nwkJoinStatusNoNetworks;
				}

			} else {
				confirmPointer->Status = nwkJoinStatusNoNetworks;
			}
		}
	} else if (requestType == nwkRequestStartRouter)
	{
		tNwkRequestStartRouter * requestPointer = request;
		tNwkConfirmStartRouter * confirmPointer = confirm;

		{
			tMacRequestStart startRequest;
			tMacConfirmStart startConfirm;

			startRequest.ChannelNumber = System.phy.pib.phyCurrentChannel;
			startRequest.PANId = System.mac.pib.macPANId;
			startRequest.shortAdd = System.mac.pib.macShortAddress;
			startRequest.PANCoordinator = false;

			fMacApiRequestConfirm(&startRequest, &startConfirm, macRequestTypeStart, macConfirmTypeStart);

			if (startConfirm.status == macStartStatusSuccess)
			{
				confirmPointer->Status = nwkStartStatusSuccess;
			}
		}
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacApiIndicationResponse (
	void * indication,
	void * response,
	eMacIndicationType indicationType,
	eMacResponseType responseType,
	uint8_t * nwkPayload,
	uint8_t nwkOffset)
{
	if (indicationType == macIndicationTypeAssociate)
	{
		tMacIndicationAssociate *indicationPointer = indication;
		tMacResponseAssociate *responsePointer = response;

		responsePointer->AssocShortAddress = System.randomNumber;

		// TODO associate adresses in a better maneer

		responsePointer->status = macAssociationStatusSuccess;

	} else if (indicationType == macIndicationTypeBeaconNotify)
	{
		tMacIndicationBeaconNotify *indicationPointer = indication;

		uint8_t i;
		bool thereIsOne = false;
		
		for (i = 0; i < 10; i++)			// Check if that device is already in neighbour tables
		{
			if (System.nwk.nib.nwkNeighborTable[i].networkAddress == indicationPointer->AddrList &&
				System.nwk.nib.nwkNeighborTable[i].panId == indicationPointer->PANDescriptor.CoordPANId)
			{
				thereIsOne = true;			// Indicate that device is in database
			}
		}

		if (thereIsOne == false)			// Save new device if it is not present in database
		{
			for (i = 0; i < 10; i++)		// Scan for a free slot
			{
				if (System.nwk.nib.nwkNeighborTable[i].networkAddress == 0)		// Select free slot
				{
					System.nwk.nib.nwkNeighborTable[i].networkAddress = indicationPointer->AddrList;
					System.nwk.nib.nwkNeighborTable[i].panId = indicationPointer->PANDescriptor.CoordPANId;
					System.nwk.nib.nwkNeighborTable[i].lqi = indicationPointer->PANDescriptor.LinkQuality;
					break;
				}
			}
		}
	} else if (indicationType == macIndicationTypeOrphan)
	{

	}
}