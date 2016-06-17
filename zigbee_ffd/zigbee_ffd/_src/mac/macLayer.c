/*
 * macLayer.c
 *
 * Created: 2016-04-10 17:06:40
 *  Author: Maciek
 */ 

#include <main.h>


// Decode

void fMacMessageFcfDecode (tMacFcf* macMessageFcfPointer,
	 volatile uint8_t* addressSource);

void fMacMessageSequenceNumberDecode (uint8_t* macMessageSequenceNumberPointer,
	volatile uint8_t* addressSource);

uint8_t fMacMessageAddressingFieldsDecode (eMacDestAddMode macMessageDestAddField,
	eMacSrcAddMode macMessageSrcAddField,
	uint16_t* macMessageAddressingFieldsDestPanPointer,
	uint16_t* macMessageAddressingFieldsDestAddPointer,
	uint16_t* macMessageAddressingFieldsSrcPanPointer,
	uint16_t* macMessageAddressingFieldsSrcAddPointer,
	volatile uint8_t* addressSource);

void fMacMessageMsduDecode (uint8_t* macMessageMsduPointer,
	uint8_t frameOffset,
	uint8_t macMessageMpduLength,
	volatile uint8_t* addressSource);

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						GET SECTION										//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

eMacFrameTypeCommandIdentifier fMacGetMsduCommandIdentifier (
	uint8_t * buffer,
	uint8_t mhrOffset);

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						PREPARE SECTION									//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

void fMacMessageFcfPrepare (uint8_t* macMessageFcfPointer,
	eMacFrameType macFrameType,
	eMacSecurity macSecurity,
	eMacFramePending macFramePending,
	eMacAckRequest macAckRequest,
	eMacIntraPan macIntraPan,
	eMacDestAddMode macDestAddMode,
	eMacFrameVersion macFrameVersion,
	eMacSrcAddMode macSrcAddMode);

void fMacMessageSequenceNumberPrepare (uint8_t* macMessageSequenceNumberPointer,
	uint8_t sequenceNumber);

void fMacMessageAddressingFieldsPrepare (uint8_t * macMessageAddressingFieldsPointer,
	eMacDestAddMode macDestinationMode,
	eMacSrcAddMode macSourceMode,
	tMacAdd * macAddresses,
	uint8_t * length);

void fMacPrepareMhr (tMacMhr * sourceMacMessage,
	uint8_t * destinationAddress,
	uint8_t * lengthAddress);

void fMacPreparePayload (eMacFrameType frameType,
	void * configurationAddress,
	eMacFrameTypeCommandIdentifier commandType,
	uint8_t * destinationAddress,
	uint8_t * length);

void fMacPrepareBeacon (void);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageFcfDecode (tMacFcf* macMessageFcfPointer,
	volatile uint8_t* addressSource)
{
	macMessageFcfPointer->macFrameType = ((*addressSource >> MAC_POSITION_FCF_FRAME_TYPE) & 0x07);
	macMessageFcfPointer->macSecurity = ((*addressSource >> MAC_POSITION_FCF_SECURITY) & 0x01);
	macMessageFcfPointer->macFramePending = ((*addressSource >> MAC_POSITION_FCF_FRAME_PENDING) & 0x01);
	macMessageFcfPointer->macAckRequest = ((*addressSource >> MAC_POSITION_FCF_ACK_REQUEST) & 0x01);
	macMessageFcfPointer->macIntraPan = ((*addressSource >> MAC_POSITION_FCF_INTRA_PAN) & 0x01);

	macMessageFcfPointer->macDestAddMode = ((*(addressSource + 1) >> MAC_POSITION_FCF_DEST_ADD_MODE) & 0x03);
	macMessageFcfPointer->macFrameVersion = ((*(addressSource + 1) >> MAC_POSITION_FCF_FRAME_VERSION) & 0x03);
	macMessageFcfPointer->macSrcAddMode = ((*(addressSource + 1) >> MAC_POSITION_FCF_SRC_ADD_MODE) & 0x03);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageSequenceNumberDecode (uint8_t* macMessageSequenceNumberPointer,
	volatile uint8_t* addressSource)
{
	*macMessageSequenceNumberPointer = *(addressSource + 2);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

uint8_t fMacMessageAddressingFieldsDecode (eMacDestAddMode macMessageDestAddField,
	eMacSrcAddMode macMessageSrcAddField,
	uint16_t* macMessageAddressingFieldsDestPanPointer,
	uint16_t* macMessageAddressingFieldsDestAddPointer,
	uint16_t* macMessageAddressingFieldsSrcPanPointer,
	uint16_t* macMessageAddressingFieldsSrcAddPointer,
	volatile uint8_t* addressSource)
{
	if ((uint8_t)macMessageDestAddField == macDestAddModePanAndAddFieldsNotPresent)
	{
		if ((uint8_t)macMessageSrcAddField == macSrcAddModePanAndAddFieldsNotPresent)
		{
			// No offset, no pan/add fields
			// No women, no cry
			return 0;
		} else if ((uint8_t)macMessageSrcAddField == macSrcAddModeAdd16Bit)
		{
			*macMessageAddressingFieldsSrcPanPointer = (*(addressSource + 4) << 8) |
														(*(addressSource + 3));

			*macMessageAddressingFieldsSrcAddPointer = (*(addressSource + 6) << 8) |
														(*(addressSource + 5));

			// 2 bytes PAN Src
			// 2 bytes Add Src
			return 4;
		}
	} else if ((uint8_t)macMessageDestAddField == macDestAddModeAdd16Bit)
	{
		if ((uint8_t)macMessageSrcAddField == macSrcAddModePanAndAddFieldsNotPresent)
		{
			*macMessageAddressingFieldsDestPanPointer = (*(addressSource + 4) << 8) |
														(*(addressSource + 3));

			*macMessageAddressingFieldsDestAddPointer = (*(addressSource + 6) << 8) |
														(*(addressSource + 5));

			// 2 bytes PAN Dest
			// 2 bytes Add Dest
			return 4;
		} else if ((uint8_t)macMessageSrcAddField == macSrcAddModeAdd16Bit)
		{
			*macMessageAddressingFieldsDestPanPointer = (*(addressSource + 4) << 8) |
													    (*(addressSource + 3));

			*macMessageAddressingFieldsDestAddPointer = (*(addressSource + 6) << 8) |
													    (*(addressSource + 5));
			
			*macMessageAddressingFieldsSrcPanPointer = (*(addressSource + 8) << 8) |
													   (*(addressSource + 7));
			
			*macMessageAddressingFieldsSrcAddPointer = (*(addressSource + 10) << 8) |
													   (*(addressSource + 9));

			// 2 bytes PAN Dest
			// 2 bytes Add Dest
			// 2 bytes PAN Src
			// 2 bytes Add Src
			return 8;
		}
	}

	// Reserved Addressing Modes
	return 0;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageMsduDecode (
	uint8_t* macMessageMsduPointer,
	uint8_t frameOffset,
	uint8_t macMessageMpduLength,
	volatile uint8_t * addressSource)
{
	memcpy((void *) macMessageMsduPointer, (void *)(addressSource + 3 + frameOffset), macMessageMpduLength + 1 - frameOffset - 2 - 3);
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						RECEIVE SECTION									//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

/**	
* @brief:
  @param: 
  *			
  *			
  */

void macReceiveMessage (
	tPhyFifoMessage * buffer,
	uint8_t bufferLength,
	uint8_t prevLayerOffset)
{
	uint8_t mhrOffset, msduOffset;

	// FCF- 2 bytes
	// Sequence number- 1 byte
	mhrOffset = 2 + 1; 
	uint16_t srcPan, srcAdd, dstPan, dstAdd;

	if (fMacGetFcfDestAddMode(buffer->phyFifoMessage + prevLayerOffset) == macDestAddModeAdd16Bit)
	{
		mhrOffset += 4;
		dstPan = fMacGetAfAddressPanDestination(buffer->phyFifoMessage + prevLayerOffset, macDestAddModeAdd16Bit, fMacGetFcfSrcAddMode(buffer->phyFifoMessage + prevLayerOffset));
		dstAdd = fMacGetAfAddressShortDestination(buffer->phyFifoMessage + prevLayerOffset, macDestAddModeAdd16Bit, fMacGetFcfSrcAddMode(buffer->phyFifoMessage + prevLayerOffset));
	}

	if (fMacGetFcfSrcAddMode(buffer->phyFifoMessage + prevLayerOffset) == macSrcAddModeAdd16Bit)
	{
		mhrOffset += 4;
		srcPan = fMacGetAfAddressPanSource(buffer->phyFifoMessage + prevLayerOffset, fMacGetFcfDestAddMode(buffer->phyFifoMessage + prevLayerOffset), macSrcAddModeAdd16Bit);
		srcAdd = fMacGetAfAddressShortSource(buffer->phyFifoMessage + prevLayerOffset, fMacGetFcfDestAddMode(buffer->phyFifoMessage + prevLayerOffset), macSrcAddModeAdd16Bit);
	}

	msduOffset = mhrOffset;

	uint8_t frameType = (uint8_t) fMacGetFcfFrameType(buffer->phyFifoMessage + prevLayerOffset);

	if (frameType == (uint8_t)macFrameTypeMacCommand)
	{
		msduOffset += 1;

		uint8_t commandIdentifier = (uint8_t) fMacGetMsduCommandIdentifier(buffer->phyFifoMessage + prevLayerOffset, mhrOffset);

		if (commandIdentifier == (uint8_t)commandIdentifierAssociationRequest)
		{
			tMacIndicationAssociate indication;
			tMacResponseAssociate response;

			indication.DeviceAddress = srcAdd;
			indication.CapabilityInformation.allocateAddress = (*(buffer->phyFifoMessage + msduOffset) >> MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_ALLADD) & 0x01;
			indication.CapabilityInformation.deviceType = (*(buffer->phyFifoMessage + msduOffset) >> MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_DTYPE) & 0x01;
			indication.CapabilityInformation.powerSource = (*(buffer->phyFifoMessage + msduOffset) >> MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_PSRC) & 0x01;
			indication.CapabilityInformation.receiverOnWhenIdle = (*(buffer->phyFifoMessage + msduOffset) >> MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_RXONIDLE) & 0x01;
			indication.CapabilityInformation.securityCapability = (*(buffer->phyFifoMessage + msduOffset) >> MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_SCAP) & 0x01;

			fMacApiIndicationResponse(&indication, &response, macIndicationTypeAssociate, macResponseTypeAssociate);

			fMacPrepareAssociationResponse(srcAdd, response.AssocShortAddress, response.status);
			fPhyTrxMessageSend(System.phy.txBuf, macAckRequestYes, System.phy.txBufLength);

		} else if (commandIdentifier == (uint8_t)commandIdentifierAssociationResponse)
		{
			
		} else if (commandIdentifier == (uint8_t)commandIdentifierDisassociationRequest)
		{

		} else if (commandIdentifier == (uint8_t)commandIdentifierDataRequest)
		{

		} else if (commandIdentifier == (uint8_t)commandIdentifierPanIdConflictNoticication)
		{

		} else if (commandIdentifier == (uint8_t)commandIdentifierOrphanNotification)
		{

		} else if (commandIdentifier == (uint8_t)commandIdentifierBeaconRequest)
		{

			fMacPrepareBeacon();
			fPhyTrxMessageSend(System.phy.txBuf, macAckRequestNo, System.phy.txBufLength);

		} else if (commandIdentifier == (uint8_t)commandIdentifierCoordinatorRealignment)
		{

		} else if (commandIdentifier == (uint8_t)commandIdentifierGtsRequest)
		{

		}
	} else if (frameType == (uint8_t)macFrameTypeBeacon)
	{

		tMacIndicationBeaconNotify indication;

		indication.AddrList = srcAdd;
		//indication.PANDescriptor.CoordAddress =
		indication.PANDescriptor.CoordPANId = srcPan;
		indication.PANDescriptor.LinkQuality = buffer->phyFifoMessageLqi;
			
		fMacApiIndicationResponse(&indication, 0, macIndicationTypeBeaconNotify, macResponseTypeNone);

	} else if (frameType == (uint8_t)macFrameTypeData)
	{

	} else if (frameType == (uint8_t)macFrameTypeAck)
	{

	}
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						GET SECTION										//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

/**	
* @brief: We take raw 127 byte buffer and check it's frame type
  @param: 
  *			
  *			
  */

eMacFrameType fMacGetFcfFrameType (
	uint8_t * buffer)
{
	return ((*(buffer + MAC_POSITION_FCF) >> MAC_POSITION_FCF_FRAME_TYPE) & 0x07);
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

eMacAckRequest fMacGetFcfAckRequest (
	uint8_t * buffer)
{
	return ((*(buffer + MAC_POSITION_FCF) >> MAC_POSITION_FCF_ACK_REQUEST) & 0x01);
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

eMacDestAddMode fMacGetFcfDestAddMode (
	uint8_t * buffer)
{
	return ((*(buffer + MAC_POSITION_FCF + 1) >> MAC_POSITION_FCF_DEST_ADD_MODE) & 0x03);
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

eMacSrcAddMode fMacGetFcfSrcAddMode (
	uint8_t * buffer)
{
	return ((*(buffer + MAC_POSITION_FCF + 1) >> MAC_POSITION_FCF_SRC_ADD_MODE) & 0x03);
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

uint16_t fMacGetAfAddressPanDestination (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource)
{
	if (modeDestination == macDestAddModePanAndAddFieldsNotPresent)
	{
		return 0x0000;
	} else if (modeDestination == macDestAddModeAdd16Bit)
	{
		return (*(buffer + MAC_POSITION_AF) | (*(buffer + MAC_POSITION_AF + 1) << 8));
	}

	return 0x0000;
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

uint16_t fMacGetAfAddressShortDestination (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource)
{
	if (modeDestination == macDestAddModePanAndAddFieldsNotPresent)
	{
		return 0x0000;
	} else if (modeDestination == macDestAddModeAdd16Bit)
	{
		return (*(buffer + MAC_POSITION_AF + 2) | (*(buffer + MAC_POSITION_AF + 3) << 8));
	}

	return 0x0000;
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

uint16_t fMacGetAfAddressPanSource (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource)
{
	if (modeDestination == macDestAddModePanAndAddFieldsNotPresent)
	{
		if (modeSource == macSrcAddModePanAndAddFieldsNotPresent)
		{
			return 0x0000;
		} else if (modeSource == macSrcAddModeAdd16Bit)
		{
			return (*(buffer + MAC_POSITION_AF) | (*(buffer + MAC_POSITION_AF + 1) << 8));
		}
	} else if (modeDestination == macDestAddModeAdd16Bit)
	{
		if (modeSource == macSrcAddModePanAndAddFieldsNotPresent)
		{
			return 0x0000;
		} else if (modeSource == macSrcAddModeAdd16Bit)
		{
			return (*(buffer + MAC_POSITION_AF + 4) | (*(buffer + MAC_POSITION_AF + 5) << 8));
		}
	}

	return 0x0000;
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

uint16_t fMacGetAfAddressShortSource (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource)
{
	if (modeDestination == macDestAddModePanAndAddFieldsNotPresent)
	{
		if (modeSource == macSrcAddModePanAndAddFieldsNotPresent)
		{
			return 0x0000;
		} else if (modeSource == macSrcAddModeAdd16Bit)
		{
			return (*(buffer + MAC_POSITION_AF + 2) | (*(buffer + MAC_POSITION_AF + 3) << 8));
		}
	} else if (modeDestination == macDestAddModeAdd16Bit)
	{
		if (modeSource == macSrcAddModePanAndAddFieldsNotPresent)
		{
			return 0x0000;
		} else if (modeSource == macSrcAddModeAdd16Bit)
		{
			return (*(buffer + MAC_POSITION_AF + 6) | (*(buffer + MAC_POSITION_AF + 7) << 8));
		}
	}

	return 0x0000;
}

/**	
* @brief:
  @param: 
  *			
  *			
  */

eMacFrameTypeCommandIdentifier fMacGetMsduCommandIdentifier (
	uint8_t * buffer,
	uint8_t mhrOffset)
{
	return (eMacFrameTypeCommandIdentifier) (*(buffer + mhrOffset) & 0xFF);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						PREPARE SECTION									//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////


/**	
* @brief: 	
  @param: givenAddress is a short address that we are giving to the node
	destinationAddress is a short address of the node
  *			
  *			
  */

void fMacPrepareAssociationResponse (
	uint16_t destinationAddress,
	uint16_t givenAddress,
	eMacFrameTypeCommandAssociationResponseStatus status)
{
	tMacMhr mhr;
	tMacFrameTypeCommandAssociationResponse command;
	uint8_t length = 0;
	uint8_t workingBuffer[127];

	// FCF

	mhr.macFcf.macFrameType = macFrameTypeMacCommand;
	mhr.macFcf.macSecurity = macSecurityDisabled;
	mhr.macFcf.macFramePending = macFramePendingNo;
	mhr.macFcf.macAckRequest = macAckRequestNo;
	mhr.macFcf.macIntraPan = macIntraPanNo;
	mhr.macFcf.macDestAddMode = macDestAddModeAdd16Bit;						// Should be extended 64-bit but we don't use 64 bit (802 standard p. 69)
	mhr.macFcf.macFrameVersion = macFrameVersionIeee2006;
	mhr.macFcf.macSrcAddMode = macSrcAddModePanAndAddFieldsNotPresent;		// Should be ommited (802 standard p. 69)

	// Sequence number

	mhr.macSequenceNumber = 0;

	// Addresses
	
	mhr.macAdd.macDestPan = System.mac.pib.macPANId;		// 
	mhr.macAdd.macDestAdd = destinationAddress;				// 
	//mhr.macAdd.macSrcPan = System.mac.pib.macPANId;			// ??
	//mhr.macAdd.macSrcAdd = System.mac.pib.macShortAddress;	// ??
	
	fMacPrepareMhr(&mhr, workingBuffer, &length);

	if (status == responseStatusSuccess)
	{
		command.status = responseStatusSuccess;
		command.shortAddress = givenAddress;
	} else if (status == responseStatusPanAtCapacity)
	{
		command.status = responseStatusPanAtCapacity;
		command.shortAddress = 0xFFFF;					// If the coordinator was not able to associate the device to its PAN, the Short Address field shall be set to 0xffff (802 standard p. 70)
	} else if (status == responseStatusPanAccessDenied)
	{
		command.status = responseStatusPanAccessDenied;
		command.shortAddress = 0xFFFF;					// If the coordinator was not able to associate the device to its PAN, the Short Address field shall be set to 0xffff (802 standard p. 70)
	}

	fMacPreparePayload(macFrameTypeMacCommand, &command, commandIdentifierAssociationResponse, workingBuffer, &length);

	fMainMoveAndAttach(System.phy.txBuf, workingBuffer, System.phy.txBufLength, length);

	System.phy.txBufLength += length;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacPrepareAssociationRequest (
	uint16_t destinationPan,
	uint16_t destinationAddress,
	tMacCapabilityInformation * capabilityInformation)
{
	tMacMhr mhr;
	tMacFrameTypeCommandAssociationRequest command;
	uint8_t length = 0;
	uint8_t workingBuffer[127];

	// FCF

	mhr.macFcf.macFrameType = macFrameTypeMacCommand;
	mhr.macFcf.macSecurity = macSecurityDisabled;
	mhr.macFcf.macFramePending = macFramePendingNo;
	mhr.macFcf.macAckRequest = macAckRequestNo;
	mhr.macFcf.macIntraPan = macIntraPanNo;
	mhr.macFcf.macDestAddMode = macDestAddModeAdd16Bit;
	mhr.macFcf.macFrameVersion = macFrameVersionIeee2006;	// Should be same as in beacon but we use only 16 bit (802 standard p. 68)
	mhr.macFcf.macSrcAddMode = macSrcAddModeAdd16Bit;		// Should be extanded 64 bit but we dont use it (802 standard p. 68)

	// Sequence number

	mhr.macSequenceNumber = 0;

	// Addresses
	
	mhr.macAdd.macDestPan = destinationPan;								// PAN which we want to associate to (802 standard p. 68)
	mhr.macAdd.macDestAdd = destinationAddress;							// Add of beacon (802 standard p. 68)
	mhr.macAdd.macSrcPan = 0xFFFF;										// Set as broadcast (802 standard p. 68)
	mhr.macAdd.macSrcAdd = System.mac.pib.macShortAddress;				// Should be set as IEEE add but we don't use it so we use random one (802 standard p. 68)
	
	fMacPrepareMhr(&mhr, workingBuffer, &length);

	command.cap.deviceType = capabilityInformation->deviceType;						// FFD
	command.cap.powerSource = capabilityInformation->powerSource;					// not from battery
	command.cap.rxOnIdle = capabilityInformation->receiverOnWhenIdle;				// receive all the time
	command.cap.securityCapability = capabilityInformation->securityCapability;		// not encrypted
	command.cap.allocateAddress = capabilityInformation->allocateAddress;			// wants to get an address

	fMacPreparePayload(macFrameTypeMacCommand, &command, commandIdentifierAssociationRequest, workingBuffer, &length);

	fMainMoveAndAttach(System.phy.txBuf, workingBuffer, System.phy.txBufLength, length);

	System.phy.txBufLength += length;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacPrepareBeaconRequest (void)
{
	tMacMhr mhr;
	tMacFrameTypeCommandBeaconRequest command;
	uint8_t length = 0;
	uint8_t workingBuffer[127];

	// FCF

	mhr.macFcf.macFrameType = macFrameTypeMacCommand;
	mhr.macFcf.macSecurity = macSecurityDisabled;
	mhr.macFcf.macFramePending = macFramePendingNo;
	mhr.macFcf.macAckRequest = macAckRequestNo;
	mhr.macFcf.macIntraPan = macIntraPanNo;
	mhr.macFcf.macDestAddMode = macDestAddModeAdd16Bit;
	mhr.macFcf.macFrameVersion = macFrameVersionIeee2006;
	mhr.macFcf.macSrcAddMode = macSrcAddModePanAndAddFieldsNotPresent;	// (802 documentation p. 73)

	// Sequence number

	mhr.macSequenceNumber = 0;

	// Addresses
	
	mhr.macAdd.macDestPan = 0xFFFF;	// Set as broadcast (802 documentation p. 73)
	mhr.macAdd.macDestAdd = 0xFFFF;	// Set as broadcast (802 documentation p. 73)
	
	fMacPrepareMhr(&mhr, workingBuffer, &length);

	// Nothing to do

	fMacPreparePayload(macFrameTypeMacCommand, &command, commandIdentifierBeaconRequest, workingBuffer, &length);

	fMainMoveAndAttach(System.phy.txBuf, workingBuffer, System.phy.txBufLength, length);

	System.phy.txBufLength += length;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacPrepareBeacon (void)
{
	tMacMhr mhr;
	tMacFrameTypeBeacon beacon;
	uint8_t length = 0;
	uint8_t workingBuffer[127];

	// FCF

	mhr.macFcf.macFrameType = macFrameTypeBeacon;
	mhr.macFcf.macSecurity = macSecurityDisabled;
	mhr.macFcf.macFramePending = macFramePendingNo;
	mhr.macFcf.macAckRequest = macAckRequestNo;
	mhr.macFcf.macIntraPan = macIntraPanNo;
	mhr.macFcf.macDestAddMode = macDestAddModePanAndAddFieldsNotPresent;
	mhr.macFcf.macFrameVersion = macFrameVersionIeee2006; 
	mhr.macFcf.macSrcAddMode = macSrcAddModeAdd16Bit;

	// Sequence number

	mhr.macSequenceNumber = 0;

	// Addresses
	
	mhr.macAdd.macSrcPan = System.mac.pib.macPANId;
	mhr.macAdd.macSrcAdd = System.mac.pib.macShortAddress;
	
	fMacPrepareMhr(&mhr, workingBuffer, &length);

	// Superframe specification

	beacon.ss.beaconOrder = 0;
	beacon.ss.ssOrder = 0;
	beacon.ss.finalCapOrder = 0;
	beacon.ss.ble = false;
	beacon.ss.panCoordinator = false;
	beacon.ss.associationPermit = false;

	// Guaranteed time slot

	beacon.gts.gtsS.gtsDescCount = 0;
	beacon.gts.gtsS.gtsPermit = false;

	// Pending address

	beacon.pa.paS.numOfShortPending = 0;
	beacon.pa.paS.numOfExtPending = 0;

	fMacPreparePayload(macFrameTypeBeacon, &beacon, 0, workingBuffer, &length);

	fMainMoveAndAttach(System.phy.txBuf, workingBuffer, System.phy.txBufLength, length);

	System.phy.txBufLength += length;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacPrepareMhr (tMacMhr * sourceMacMessage,
	uint8_t * destinationAddress,
	uint8_t * lengthAddress)
{

	fMacMessageFcfPrepare(destinationAddress,
		sourceMacMessage->macFcf.macFrameType,
		sourceMacMessage->macFcf.macSecurity,
		sourceMacMessage->macFcf.macFramePending,
		sourceMacMessage->macFcf.macAckRequest,
		sourceMacMessage->macFcf.macIntraPan,
		sourceMacMessage->macFcf.macDestAddMode,
		sourceMacMessage->macFcf.macFrameVersion,
		sourceMacMessage->macFcf.macSrcAddMode);

	fMacMessageSequenceNumberPrepare(destinationAddress + 2,
		sourceMacMessage->macSequenceNumber);

	*lengthAddress = 3;	// After FCF and sequence number

	fMacMessageAddressingFieldsPrepare(destinationAddress + 3,
		sourceMacMessage->macFcf.macDestAddMode,
		sourceMacMessage->macFcf.macSrcAddMode,
		&sourceMacMessage->macAdd,
		lengthAddress);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacPreparePayload (eMacFrameType frameType,
	void * configurationAddress,
	eMacFrameTypeCommandIdentifier commandType,
	uint8_t * destinationAddress,
	uint8_t * length)
{
	uint8_t counter = 0;

	// Shift it by a length
	destinationAddress += *length;

	if (frameType == macFrameTypeBeacon)
	{
		tMacFrameTypeBeacon * pointer = configurationAddress;

		// Superframe specification

		destinationAddress[MAC_POSITION_MSDU_BEACON_SS] = 0x00;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS] |= pointer->ss.beaconOrder << MAC_POSITION_MSDU_BEACON_SS_BEACONORDER;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS] |= pointer->ss.ssOrder << MAC_POSITION_MSDU_BEACON_SS_SSORDER;

		destinationAddress[MAC_POSITION_MSDU_BEACON_SS + 1] = 0x00;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS + 1] |= pointer->ss.finalCapOrder << MAC_POSITION_MSDU_BEACON_SS_FINALCAPSLOT;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS + 1] |= pointer->ss.ble << MAC_POSITION_MSDU_BEACON_SS_BLE;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS + 1] |= pointer->ss.panCoordinator << MAC_POSITION_MSDU_BEACON_SS_PANCOORD;
		destinationAddress[MAC_POSITION_MSDU_BEACON_SS + 1] |= pointer->ss.associationPermit << MAC_POSITION_MSDU_BEACON_SS_ASSPERMIT;

		counter += 2;

		// GTS fields

		destinationAddress[MAC_POSITION_MSDU_BEACON_GTSS] = 0x00;
		destinationAddress[MAC_POSITION_MSDU_BEACON_GTSS] |= pointer->gts.gtsS.gtsDescCount << MAC_POSITION_MSDU_BEACON_GTSS_DESCCNT;
		destinationAddress[MAC_POSITION_MSDU_BEACON_GTSS] |= pointer->gts.gtsS.gtsPermit << MAC_POSITION_MSDU_BEACON_GTSS_GTSPERM;

		counter++;

		// Pending Address Fields

		destinationAddress[MAC_POSITION_MSDU_BEACON_PA] = 0x00;
		destinationAddress[MAC_POSITION_MSDU_BEACON_PA] |= pointer->pa.paS.numOfShortPending << MAC_POSITION_MSDU_BEACON_PANUMSADDPENDING;
		destinationAddress[MAC_POSITION_MSDU_BEACON_PA] |= pointer->pa.paS.numOfExtPending << MAC_POSITION_MSDU_BEACON_PANUMEADDPENDING;

		counter++;

	} else if (frameType == macFrameTypeData)
	{

	} else if (frameType == macFrameTypeMacCommand)
	{
		// Command type
		destinationAddress[MAC_POSITION_MSDU_COMMAND_IDENTIFIER] = (uint8_t)commandType;
		counter++;

		if (commandType == commandIdentifierAssociationRequest)
		{
			tMacFrameTypeCommandAssociationRequest * pointer = configurationAddress;

			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] = 0x00;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] |= pointer->cap.deviceType << MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_DTYPE;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] |= pointer->cap.powerSource << MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_PSRC;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] |= pointer->cap.rxOnIdle << MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_RXONIDLE;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] |= pointer->cap.securityCapability << MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_SCAP;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP] |= pointer->cap.allocateAddress << MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_ALLADD;

			counter++;

		} else if (commandType == commandIdentifierAssociationResponse)
		{
			tMacFrameTypeCommandAssociationResponse * pointer = configurationAddress;
			
			// Short address

			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD] = ((uint8_t)(pointer->shortAddress)) << MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_LSB;
			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD + 1] = ((uint8_t)(pointer->shortAddress >> 8)) << MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_MSB;

			counter += 2;

			// Association status

			destinationAddress[MAC_POSITION_MSDU_COMMAND_ASSRES_ASSSTATUS] = pointer->status << MAC_POSITION_MSDU_COMMAND_ASSRES_ASSSTATUS_STATUS;

			counter++;

		} else if (commandType == commandIdentifierDisassociationRequest)
		{
			
		} else if (commandType == commandIdentifierDataRequest)
		{
			// Nothing to do
		} else if (commandType == commandIdentifierPanIdConflictNoticication)
		{
			// Nothing to do
		} else if (commandType == commandIdentifierOrphanNotification)
		{
			// Nothing to do
		} else if (commandType == commandIdentifierBeaconRequest)
		{
			// Nothing to do
		} else if (commandType == commandIdentifierCoordinatorRealignment)
		{

		} else if (commandType == commandIdentifierGtsRequest)
		{

		}
	}

	*length += counter;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageFcfPrepare (
	uint8_t* macMessageFcfPointer,
	eMacFrameType macFrameType,
	eMacSecurity macSecurity,
	eMacFramePending macFramePending,
	eMacAckRequest macAckRequest,
	eMacIntraPan macIntraPan,
	eMacDestAddMode macDestAddMode,
	eMacFrameVersion macFrameVersion,
	eMacSrcAddMode macSrcAddMode)
{
	macMessageFcfPointer[0] = 0x00;
	macMessageFcfPointer[0] |= ((uint8_t) macFrameType) << MAC_POSITION_FCF_FRAME_TYPE;
	macMessageFcfPointer[0] |= ((uint8_t) macSecurity) << MAC_POSITION_FCF_SECURITY;
	macMessageFcfPointer[0] |= ((uint8_t) macFramePending) << MAC_POSITION_FCF_FRAME_PENDING;
	macMessageFcfPointer[0] |= ((uint8_t) macAckRequest) << MAC_POSITION_FCF_ACK_REQUEST;
	macMessageFcfPointer[0] |= ((uint8_t) macIntraPan) << MAC_POSITION_FCF_INTRA_PAN;
	
	macMessageFcfPointer[1] = 0x00;
	macMessageFcfPointer[1] |= ((uint8_t) macDestAddMode) << MAC_POSITION_FCF_DEST_ADD_MODE;
	macMessageFcfPointer[1] |= ((uint8_t) macFrameVersion) << MAC_POSITION_FCF_FRAME_VERSION;
	macMessageFcfPointer[1] |= ((uint8_t) macSrcAddMode) << MAC_POSITION_FCF_SRC_ADD_MODE;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageSequenceNumberPrepare (uint8_t* macMessageSequenceNumberPointer,
									   uint8_t sequenceNumber)
{
	*macMessageSequenceNumberPointer = sequenceNumber;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageAddressingFieldsPrepare (uint8_t * macMessageAddressingFieldsPointer,
	eMacDestAddMode macDestinationMode,
	eMacSrcAddMode macSourceMode,
	tMacAdd * macAddresses,
	uint8_t * length)
{
	uint8_t counter = 0;

	if (macDestinationMode == macDestAddModeAdd16Bit)
	{
		macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macDestPan;			// LSB
		macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macDestPan >> 8);	// MSB
		
		macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macDestAdd;			// LSB
		macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macDestAdd >> 8);	// MSB

		if (macSourceMode == macSrcAddModeAdd16Bit)
		{
			macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macSrcPan;			// LSB
			macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macSrcPan >> 8);		// MSB
			
			macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macSrcAdd;			// LSB
			macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macSrcAdd >> 8);		// MSB
		} else if (macSourceMode == macSrcAddModePanAndAddFieldsNotPresent)
		{

		}

	} else if (macDestinationMode == macDestAddModePanAndAddFieldsNotPresent)
	{
		if (macSourceMode == macSrcAddModeAdd16Bit)
		{
			macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macSrcPan;			// LSB
			macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macSrcPan >> 8);		// MSB
			
			macMessageAddressingFieldsPointer[counter++] = (uint8_t) macAddresses->macSrcAdd;			// LSB
			macMessageAddressingFieldsPointer[counter++] = (uint8_t)(macAddresses->macSrcAdd >> 8);		// MSB
		} else if (macSourceMode == macSrcAddModePanAndAddFieldsNotPresent)
		{

		}
	}
	
	*length += counter;
}