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

void fMacMessageFcfDecode (tMacFcf* macMessageFcfPointer);

void fMacMessageSequenceNumberDecode (uint8_t* macMessageSequenceNumberPointer);

void fMacMessageAddressingFieldsDecode (uint16_t* macMessageAddressingFieldsDestPanPointer,
										uint16_t* macMessageAddressingFieldsDestAddPointer,
										uint16_t* macMessageAddressingFieldsSrcPanPointer,
										uint16_t* macMessageAddressingFieldsSrcAddPointer);

void fMacMessageAuxiliarySecurityHeaderDecode (uint8_t* macMessageAuxiliarySecurityHeaderPointer);

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

void fMacMessageFcfDecode (tMacFcf* macMessageFcfPointer)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageSequenceNumberDecode (uint8_t* macMessageSequenceNumberPointer)
{

}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageAddressingFieldsDecode (uint16_t* macMessageAddressingFieldsDestPanPointer,
										uint16_t* macMessageAddressingFieldsDestAddPointer,
										uint16_t* macMessageAddressingFieldsSrcPanPointer,
										uint16_t* macMessageAddressingFieldsSrcAddPointer)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageAuxiliarySecurityHeaderDecode (uint8_t* macMessageAuxiliarySecurityHeaderPointer)
{
	
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
	macMessageAddressingFieldsPointer[0] = (uint8_t)(macDestinationPanId >> 8);		// MSB
	macMessageAddressingFieldsPointer[1] = (uint8_t) macDestinationPanId;			// LSB

	macMessageAddressingFieldsPointer[2] = (uint8_t)(macDestinationAddress >> 8);	// MSB
	macMessageAddressingFieldsPointer[3] = (uint8_t) macDestinationAddress;			// LSB

	macMessageAddressingFieldsPointer[4] = (uint8_t)(macSourcePanId >> 8);			// MSB
	macMessageAddressingFieldsPointer[5] = (uint8_t) macSourcePanId;				// LSB

	macMessageAddressingFieldsPointer[6] = (uint8_t)(macSourceAddress >> 8);		// MSB
	macMessageAddressingFieldsPointer[7] = (uint8_t) macSourceAddress;				// LSB
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacMessageAuxiliarySecurityHeader (uint8_t* macMessageAuxiliarySecurityHeaderPointer,
										 uint8_t* macAuxiliarySecurityHeader)
{
	macMessageAuxiliarySecurityHeaderPointer[0] = macAuxiliarySecurityHeader[0];
	macMessageAuxiliarySecurityHeaderPointer[1] = macAuxiliarySecurityHeader[1];
	macMessageAuxiliarySecurityHeaderPointer[2] = macAuxiliarySecurityHeader[2];
	macMessageAuxiliarySecurityHeaderPointer[3] = macAuxiliarySecurityHeader[3];
	macMessageAuxiliarySecurityHeaderPointer[4] = macAuxiliarySecurityHeader[4];
}