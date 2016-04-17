/*
 * macLayer.c
 *
 * Created: 2016-04-10 17:06:40
 *  Author: Maciek
 */ 

#include <main.h>

// Local definitions
// #define DEFINITION



// Local variables declarations
// uint8_t NameVariable;



// Functions declarations
// void nameFunction(uint8_t variableName);

void fMacMessageFcfDecode (tMacFcf* macMessageFcfPointer,
						   volatile uint8_t* addressSource);

void fMacMessageSequenceNumberDecode (uint8_t* macMessageSequenceNumberPointer,
									  volatile uint8_t* addressSource);

uint8_t fMacMessageAddressingFieldsDecode (tMacFcf* macMessageFcfPointer,
									 	   uint16_t* macMessageAddressingFieldsDestPanPointer,
										   uint16_t* macMessageAddressingFieldsDestAddPointer,
										   uint16_t* macMessageAddressingFieldsSrcPanPointer,
										   uint16_t* macMessageAddressingFieldsSrcAddPointer,
										   volatile uint8_t* addressSource);

void fMacMessageAuxiliarySecurityHeaderDecode (uint8_t* macMessageAuxiliarySecurityHeaderPointer,
											   uint8_t frameOffset,
											   volatile uint8_t* addressSource);

void fMacMessageMsduDecode (uint8_t* macMessageMsduPointer,
							uint8_t frameOffset,
							uint8_t macMessageMpduLength,
							volatile uint8_t* addressSource);
							
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

void fMacMessageAddressingFieldsPrepare (uint8_t* macMessageAddressingFieldsPointer,
										 uint16_t macDestinationPanId,
										 uint16_t macDestinationAddress,
										 uint16_t macSourcePanId,
										 uint16_t macSourceAddress);

void fMacMessageAuxiliarySecurityHeaderPrepare (uint8_t* macMessageAuxiliarySecurityHeaderPointer,
												uint8_t* macAuxiliarySecurityHeader);

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

uint8_t fMacMessageAddressingFieldsDecode (tMacFcf* macMessageFcfPointer,
										   uint16_t* macMessageAddressingFieldsDestPanPointer,
										   uint16_t* macMessageAddressingFieldsDestAddPointer,
										   uint16_t* macMessageAddressingFieldsSrcPanPointer,
										   uint16_t* macMessageAddressingFieldsSrcAddPointer,
										   volatile uint8_t* addressSource)
{
	if (macMessageFcfPointer->macDestAddMode == macDestAddModePanAndAddFielsNotPresent)
	{
		if (macMessageFcfPointer->macSrcAddMode == macSrcAddModePanAndAddFielsNotPresent)
		{
			// No offset, no pan/add fields
			// No women, no cry
			return 0;
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd16Bit)
		{
			// 2 bytes PAN Src
			// 2 bytes Add Src
			return 4;
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd64Bit)
		{
			// 2 bytes PAN Src
			// 8 bytes Add Src
			return 10;
		}
	} else if (macMessageFcfPointer->macDestAddMode == macDestAddModeAdd16Bit)
	{
		if (macMessageFcfPointer->macSrcAddMode == macSrcAddModePanAndAddFielsNotPresent)
		{
			// 2 bytes PAN Dest
			// 2 bytes Add Dest
			return 4;
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd16Bit)
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
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd64Bit)
		{
			// 2 bytes PAN Dest
			// 2 bytes Add Dest
			// 2 bytes PAN Src
			// 8 bytes Add Src
			return 14;
		}
	} else if (macMessageFcfPointer->macDestAddMode == macDestAddModeAdd64Bit)
	{
		if (macMessageFcfPointer->macSrcAddMode == macSrcAddModePanAndAddFielsNotPresent)
		{
			// 2 bytes PAN Dest
			// 8 bytes Add Dest
			return 10;
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd16Bit)
		{
			// 2 bytes PAN Dest
			// 8 bytes Add Dest
			// 2 bytes PAN Src
			// 2 bytes Add Src
			return 14;
		} else if (macMessageFcfPointer->macSrcAddMode == macSrcAddModeAdd64Bit)
		{
			// 2 bytes PAN Dest
			// 8 bytes Add Dest
			// 2 bytes PAN Src
			// 8 bytes Add Src
			return 20;
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

void fMacMessageAuxiliarySecurityHeaderDecode (uint8_t* macMessageAuxiliarySecurityHeaderPointer,
											   uint8_t frameOffset,
											   volatile uint8_t* addressSource)
{
	*macMessageAuxiliarySecurityHeaderPointer = addressSource[3 + frameOffset];
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageMsduDecode (uint8_t* macMessageMsduPointer,
							uint8_t frameOffset,
							uint8_t macMessageMpduLength,
							volatile uint8_t * addressSource)
{
	memcpy((void *) macMessageMsduPointer, (void *)(addressSource + 3 + frameOffset), macMessageMpduLength - frameOffset - 2 - 3);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageFcfPrepare (uint8_t* macMessageFcfPointer,
							eMacFrameType macFrameType,
							eMacSecurity macSecurity,
							eMacFramePending macFramePending,
							eMacAckRequest macAckRequest,
							eMacIntraPan macIntraPan,
							eMacDestAddMode macDestAddMode,
							eMacFrameVersion macFrameVersion,
							eMacSrcAddMode macSrcAddMode)
{
	macMessageFcfPointer[0] |= ((uint8_t) macFrameType) << MAC_POSITION_FCF_FRAME_TYPE;
	macMessageFcfPointer[0] |= ((uint8_t) macSecurity) << MAC_POSITION_FCF_SECURITY;
	macMessageFcfPointer[0] |= ((uint8_t) macFramePending) << MAC_POSITION_FCF_FRAME_PENDING;
	macMessageFcfPointer[0] |= ((uint8_t) macAckRequest) << MAC_POSITION_FCF_ACK_REQUEST;
	macMessageFcfPointer[0] |= ((uint8_t) macIntraPan) << MAC_POSITION_FCF_INTRA_PAN;
	
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

void fMacMessageAddressingFieldsPrepare (uint8_t* macMessageAddressingFieldsPointer,
										 uint16_t macDestinationPanId,
										 uint16_t macDestinationAddress,
										 uint16_t macSourcePanId,
										 uint16_t macSourceAddress)
{
	macMessageAddressingFieldsPointer[1] = (uint8_t)(macDestinationPanId >> 8);		// MSB
	macMessageAddressingFieldsPointer[0] = (uint8_t) macDestinationPanId;			// LSB

	macMessageAddressingFieldsPointer[3] = (uint8_t)(macDestinationAddress >> 8);	// MSB
	macMessageAddressingFieldsPointer[2] = (uint8_t) macDestinationAddress;			// LSB

	macMessageAddressingFieldsPointer[5] = (uint8_t)(macSourcePanId >> 8);			// MSB
	macMessageAddressingFieldsPointer[4] = (uint8_t) macSourcePanId;				// LSB

	macMessageAddressingFieldsPointer[7] = (uint8_t)(macSourceAddress >> 8);		// MSB
	macMessageAddressingFieldsPointer[6] = (uint8_t) macSourceAddress;				// LSB
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageAuxiliarySecurityHeaderPrepare (uint8_t* macMessageAuxiliarySecurityHeaderPointer,
												uint8_t* macAuxiliarySecurityHeader)
{
	macMessageAuxiliarySecurityHeaderPointer[0] = macAuxiliarySecurityHeader[0];
	macMessageAuxiliarySecurityHeaderPointer[1] = macAuxiliarySecurityHeader[1];
	macMessageAuxiliarySecurityHeaderPointer[2] = macAuxiliarySecurityHeader[2];
	macMessageAuxiliarySecurityHeaderPointer[3] = macAuxiliarySecurityHeader[3];
	macMessageAuxiliarySecurityHeaderPointer[4] = macAuxiliarySecurityHeader[4];
}