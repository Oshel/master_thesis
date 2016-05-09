/*
 * phyTrx.c
 *
 * Created: 2016-04-10 17:19:26
 *  Author: Maciek
 */ 
 
#include "main.h"

// Local definitions
// #define DEFINITION

#define TRX_AES_KEY	"MheBXkVCjOxRm35r"

// Local variables declarations
// uint8_t NameVariable;



// Functions declarations
// void nameFunction(uint8_t variableName);

void fPhyTrxInit(void);

eTrxStatus fPhyTrxMessageSend (
	uint8_t * macMpdu,
	eMacAckRequest ackRequest,
	uint8_t length);

//void fPhyTrxMessageReceive (tTrxMessageReceived* macMessage,
							//volatile tPhyFifoMessage* addressSource);


// Radio configuartion functions

void fPhyTrxSetPower(uint8_t powerLevel);
void fPhyTrxSetChannel (eTrxChannel channel);
void fPhyTrxSetCcaMode (eTrxCcaMode ccaMode);
void fPhyTrxSetCcaEdThresh (int8_t thresholdInDbm);
void fPhyTrxSetReceiversSensivity (eTrxReceiverSensivity receiverSensivity);

// Received frame protection

void fPhyTrxSetRxBufferLatchSetClear (bool setClearLatch);
void fPhyTrxSetSuspendReceiving (bool setClearSuspend);

// Extended options

void fPhyTrxSetCsmaSeed (uint16_t seed);
void fPhyTrxSetAackVersion (eTrxAackVersion aackVersion);
void fPhyTrxSetBackOffExponentMax (eTrxBackOffExponent backOffExponent);
void fPhyTrxSetBackOffExponentMin (eTrxBackOffExponent backOffExponent);
void fPhyTrxSetRetriesFrameNumber (eTrxRetriesFrameNumber retriesFrameNumber);
void fPhyTrxSetRetriesCsmaNumber (eTrxRetriesCsmaNumber retriesCsmaNumber);

// Address setting

void fPhyTrxSetAddIeee (uint64_t addIeee);
void fPhyTrxSetAddPan (uint16_t addPan);
void fPhyTrxSetAddShort (uint16_t addShort);

// Calibration

void fPhyTrxDoCalibrationFilterNetwork (void);
void fPhyTrxDoCalibrationPllCenterFreq (void);
void fPhyTrxDoCalibrationPllDelayCell (void);

// Signal strength

int8_t fPhyTrxReadEd (void);
int8_t fPhyTrxReadRssi (void);

// AES encryption

void fPhyTrxAesSetKey(char* aesKey);
void fPhyTrxAesWriteBuffer(uint8_t* aesBuffer);
void fPhyTrxAesReadBuffer(uint8_t* aesBuffer);
void fPhyTrxAesEncrypt(uint8_t* aesBuffer);
void fPhyTrxAesDecrypt(uint8_t* aesBuffer);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxInit(void)
{
	uint8_t buffer;

	fPhyTrxSetPower(trxPowerLevelP35dBm);						// -Set power 3.5 dBm (maximum one)
	fPhyTrxSetChannel(trxChannel2405MHz);						// -Set channel to 2440 MHz
	fPhyTrxSetCcaMode(trxCcaModeCsAndEd);						// -Set CCA to carrier sense and energy detection
	fPhyTrxSetCcaEdThresh(-90);									// -Set ED for CCA on -90 dBm
	fPhyTrxSetReceiversSensivity(trxReceiverSensivityNone);		// -All frames with a valid SHR and PHR are received

	fPhyTrxSetExtendedTypeDevice(phyTypeDeviceRouter,
								 0x0001,
								 0x0001,
								 0x00000000);

	// Reset values
	// AES_CTRL - AES_MODE - 0x00 - AES Mode is ECB (Electronic Code Book)
	// TRX_CTRL_1 - - 0x01 - Enable Automatic CRC Calculation 
	// RX_CTRL - Receiver Sensitivity Control - 0x07 - Reset value, to be used if Antenna Diversity algorithm is disabled
	// SFD_VALUE - Start of Frame Delimiter Value - 0xA7 - Reset value, IEEE 802.15.4 compliant value of the SFD
	// TRX_CTRL_2 - Data Rate Selection - 0x00 - 250 kb/s (IEEE 802.15.4 compliant)

	buffer = IRQ_STATUS;						// -If an interrupt is enabled it is recommended to read the interrupt status register IRQ_STATUS first to clear the history.

	IRQ_MASK |= (1 << PLL_LOCK_EN) |			// -Interrupt enable
				(1 << PLL_UNLOCK_EN) |
				(1 << RX_START_EN) |
				(1 << RX_END_EN) |
				(1 << CCA_ED_DONE_EN) |
				(1 << AMI_EN) |
				(1 << TX_END_EN) |
				(1 << AWAKE_EN);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

//void fPhyTrxMessageReceive (tTrxMessageReceived* message,
							//volatile tPhyFifoMessage* addressSource)
//{
	//// frameOffset is a variable which is responsible for indicating what is the offset based on destination/source addressing mode
	//// and on security enabling (5 bytes of security)
	//uint8_t frameOffset;
//
	//message->phyRssi = addressSource->phyFifoMessageRssi;
	//message->phyLqi = addressSource->phyFifoMessageLqi;
	//message->phyEd = addressSource->phyFifoMessageEd;
	//message->psyPhr = addressSource->phyFifoMessageLength;
//
	//fMacMessageFcfDecode(&message->macMessage.macFcf,
						 //addressSource->phyFifoMessage);
//
	//fMacMessageSequenceNumberDecode(&message->macMessage.macSequenceNumber,
									//addressSource->phyFifoMessage);
									//
	//frameOffset = fMacMessageAddressingFieldsDecode(message->macMessage.macFcf.macDestAddMode,
													//message->macMessage.macFcf.macSrcAddMode,
												    //&message->macMessage.macDestPan,
												    //&message->macMessage.macDestAdd,
												    //&message->macMessage.macSrcPan,
												    //&message->macMessage.macSrcAdd,
												    //addressSource->phyFifoMessage);
//
	////if (message->macMessage.macFcf.macSecurity == macSecurityEnabled)
	////{
		////fMacMessageAuxiliarySecurityHeaderDecode(message->macMessage.macAuxSecurityHdr,
												 ////frameOffset,
												 ////addressSource->phyFifoMessage);
		////
		////// 5 additional bytes
		////frameOffset += 5;
	////}
//
	//fMacMessageMsduDecode(message->macMsdu,
						  //frameOffset,
						  //message->psyPhr,
						  //addressSource->phyFifoMessage);
//}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

eTrxStatus fPhyTrxMessageSend (
	uint8_t * macMpdu,
	eMacAckRequest ackRequest,
	uint8_t length)
{
	// Wait for RX/TX busy states finishes
	do 
	{
		// Check current state
		fPhyTrxStateCheck();
	} while (System.trx.state.trxStateCurrent == trxStateBusyRx ||
		System.trx.state.trxStateCurrent == trxStateBusyRxAack ||
		System.trx.state.trxStateCurrent == trxStateBusyTx ||
		System.trx.state.trxStateCurrent == trxStateBusyTxAret);

	// Clear the TX buffer
	System.phy.txBufLength = 0;

	if (ackRequest == macAckRequestYes)
	{
		// Change state to TxAretOn to prevent from receiving new frames
		fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStateTxAretOn, false, false);

		// Load frame length (plus CRC)
		TRXFBST = length + 2;
		// Load the frame
		memcpy((void *)(&TRXFBST + 1), macMpdu, length);

		// Change state to BusyTx to send the frame
		fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStateBusyTxAret, false, false);

		// Wait until transmission is over- it is confirmed by one of the states below
		while ((System.phy.flags.txAretStatusFlag != trxStatusSuccess) &&
			(System.phy.flags.txAretStatusFlag != trxStatusChannelAccessFailure) &&
			(System.phy.flags.txAretStatusFlag != trxStatusNoAck));

		return System.phy.flags.txAretStatusFlag;
	} else if (ackRequest == macAckRequestNo)
	{
		// Change state to TxAretOn to prevent from receiving new frames
		fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStatePllOn, false, false);

		// Load frame length (plus CRC)
		TRXFBST = length + 2;
		// Load the frame
		memcpy((void *)(&TRXFBST + 1), macMpdu, length);

		// Change state to BusyTx to send the frame
		fPhyTrxStateTransition(System.trx.state.trxStateCurrent, trxStateBusyTx, false, false);

		return trxStatusSuccess;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxAesWriteBuffer(uint8_t* aesBuffer)
{
	uint8_t i;

	for (i = 0; i <= 15; i++)
	{
		AES_STATE = aesBuffer[i];
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxAesReadBuffer(uint8_t* aesBuffer)
{
	uint8_t i;

	for (i = 0; i <= 15; i++)
	{
		aesBuffer[i] = AES_STATE;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxAesEncrypt(uint8_t* aesBuffer)
{
	fPhyTrxAesSetKey(TRX_AES_KEY);
	fPhyTrxAesWriteBuffer(aesBuffer);
	AES_CTRL &= ~(1 <<AES_DIR);
	AES_CTRL |= (1 << AES_REQUEST);
	while ((AES_STATUS & (1 << AES_DONE)) == false);		// 24 us
	fPhyTrxAesReadBuffer(aesBuffer);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxAesDecrypt(uint8_t* aesBuffer)
{
	fPhyTrxAesSetKey(TRX_AES_KEY);
	fPhyTrxAesWriteBuffer(aesBuffer);
	AES_CTRL |= (1 << AES_DIR);
	AES_CTRL |= (1 << AES_REQUEST);
	while ((AES_STATUS & (1 << AES_DONE)) == false);		// 24 us
	fPhyTrxAesReadBuffer(aesBuffer);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxAesSetKey(char* aesKey)
{
	uint8_t i;

	for (i = 0; i <= 15; i++)
	{
		AES_KEY = aesKey[i];
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetBackOffExponentMax (eTrxBackOffExponent backOffExponent)
{
	CSMA_BE &= ~(0x0F << 4);
	CSMA_BE |= backOffExponent << 4;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetBackOffExponentMin (eTrxBackOffExponent backOffExponent)
{
	CSMA_BE &= ~(0x0F << 0);
	CSMA_BE |= backOffExponent << 0;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetAackVersion (eTrxAackVersion aackVersion)
{
	CSMA_SEED_1 &= ~(0x03 << 6);
	CSMA_SEED_1 |= (aackVersion << 6);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetCsmaSeed(uint16_t seed)
{
	CSMA_SEED_0 = (uint8_t) seed;
	CSMA_SEED_1 &= ~(0x07 << 0);
	CSMA_SEED_1 |= (uint8_t)((seed >> 8) & 0x07);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetRetriesFrameNumber (eTrxRetriesFrameNumber retriesFrameNumber)
{
	XAH_CTRL_0 &= ~(0x0F << 4);
	XAH_CTRL_0 |= retriesFrameNumber << 4;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetRetriesCsmaNumber (eTrxRetriesCsmaNumber retriesCsmaNumber)
{
	XAH_CTRL_0 &= ~(0x07 << 1);
	XAH_CTRL_0 |= retriesCsmaNumber << 1;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetAddIeee (uint64_t addIeee)
{
	// This register contains the bits [64:0] of the MAC IEEE address for Frame Filter address
	// recognition.

	IEEE_ADDR_0 = (uint8_t) addIeee;
	IEEE_ADDR_1 = (uint8_t) (addIeee >> 8);
	IEEE_ADDR_2 = (uint8_t) (addIeee >> 16);
	IEEE_ADDR_3 = (uint8_t) (addIeee >> 24);
	IEEE_ADDR_4 = (uint8_t) (addIeee >> 32);
	IEEE_ADDR_5 = (uint8_t) (addIeee >> 40);
	IEEE_ADDR_6 = (uint8_t) (addIeee >> 48);
	IEEE_ADDR_7 = (uint8_t) (addIeee >> 56);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetAddPan (uint16_t addPan)
{
	// This  register  contains  the  lower/upper  8  bits  of  the  MAC  PAN  ID  for  Frame  Filter  address
	// recognition.

	System.mac.pib.macPANId = addPan;

	PAN_ID_0 = (uint8_t) addPan;
	PAN_ID_1 = (uint8_t) (addPan >> 8);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetAddShort (uint16_t addShort)
{
	// This  register  contains  the  lower/upper  8  bits  of  the  MAC  short  address  for  Frame  Filter
	// address recognition.

	System.mac.pib.macShortAddress = addShort;

	SHORT_ADDR_0 = (uint8_t) addShort;
	SHORT_ADDR_1 = (uint8_t) (addShort >> 8);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxDoCalibrationPllDelayCell (void)
{
	PLL_DCU |= (1 << PLL_DCU_START);
	while (PLL_DCU & (1 << PLL_DCU_START));	// At most 6 us
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxDoCalibrationPllCenterFreq (void)
{
	PLL_CF |= (1 << PLL_CF_START);
	while (PLL_CF & (1 << PLL_CF_START));		// Typical 35 us
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxDoCalibrationFilterNetwork (void)
{
	FTN_CTRL |= (1 << FTN_START);
	while (FTN_CTRL & (1 << FTN_START));		// At most 25 us
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetReceiversSensivity (eTrxReceiverSensivity receiverSensivity)
{
	// These register bits reduce the receiver sensitivity such that frames with  a RSSI level
	// below the RX_PDT_LEVEL threshold level are not received (RX_PDT_LEVEL>0). If  register  bits
	// RX_PDT_LEVEL>0  the  current  consumption  of  the  receiver  in  states  RX_ON  and
	// RX_AACK_ON is reduced by 500 µA. If register bits RX_PDT_LEVEL=0 (reset value)
	// all  frames  with  a  valid  SHR  and  PHR  are  received,  independently  of  their  signal
	// strength.

	RX_SYN &= ~(0x0F << 0);
	RX_SYN |= (receiverSensivity << 0);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetSuspendReceiving (bool setClearSuspend)
{
	// RX_PDT_DIS = 1 prevents the reception of a frame even if the radio transceiver is in
	// receive  modes.  An  ongoing  frame  reception  is  not  affected.  This  operation  mode  is
	// independent of the setting of register bits RX_PDT_LEVEL

	if (setClearSuspend == true)
	{
		RX_SYN |= (1 << RX_PDT_DIS);
	} else if (setClearSuspend == false)
	{
		RX_SYN &= ~(1 << RX_PDT_DIS);
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetRxBufferLatchSetClear (bool setClearLatch)
{
	// If  this  bit  is  set,  the  next  received  frame  will  be  protected  and  not  overwritten  by
	// following  frames.  Set  this  bit  to  0  to  release  the  buffer  (and  set  it  again  for  further protection).

	if (setClearLatch == true)
	{
		TRX_CTRL_2 |= (1 << RX_SAFE_MODE);
	} else if (setClearLatch == false)
	{
		TRX_CTRL_2 &= ~(1 << RX_SAFE_MODE);
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetCcaEdThresh (int8_t thresholdInDbm)
{
	CCA_THRES &= ~(0x0F << 0);		// Clear CCA/ED thresh value

	if (thresholdInDbm <= -90)
	{
		CCA_THRES |= (0x00 << 0);
	} else if (thresholdInDbm > -90 && thresholdInDbm  <= 0)
	{
		CCA_THRES |= ((90 + thresholdInDbm) / 2) << 0;
	} else if (thresholdInDbm > 0)
	{
		CCA_THRES |= (90 << 0);
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetCcaMode (eTrxCcaMode ccaMode)
{
	PHY_CC_CCA &= ~(0x03 << 5);		// Clear CCA mode bits
	PHY_CC_CCA |= (ccaMode << 5);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetChannel (eTrxChannel channel)
{
	PHY_CC_CCA &= ~(0x1F);			// Clear channel bits
	PHY_CC_CCA |= ((uint8_t) channel) << 0;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

int8_t fPhyTrxReadEd (void)
{
	if (PHY_ED_LEVEL == 0)
	{
		return -90;
	} else if ((PHY_ED_LEVEL > 0) && (PHY_ED_LEVEL < 0x53))
	{
		return (-90 + (PHY_ED_LEVEL));
	} else if (PHY_ED_LEVEL >= 0x53)
	{
		return 0x07;
	}

	return 0xFF;		// 0xFF- reset value
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

int8_t fPhyTrxReadRssi (void)
{
	if ((PHY_RSSI & 0x1F) == 0)
	{
		return -90;
	} else if ((PHY_RSSI & 0x1F) > 0 && (PHY_RSSI & 0x1F) < 28)
	{
		return (-90 + 3 * ((PHY_RSSI & 0x1F) - 1));
	} else if ((PHY_RSSI & 0x1F) == 28)
	{
		return -10;
	}

	return 0xFF;		// Wrong value
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetPower (eTrxPowerLevel powerLevel)
{
	PHY_TX_PWR &= ~(0x0F);		// Clear power registers
	PHY_TX_PWR |= ((uint8_t) powerLevel) << 0;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxSetExtendedTypeDevice (ePhyTypeDevice typeDevice,
								   uint16_t addShort,
								   uint16_t addPan,
								   uint64_t addIeee)
{	
	// Extended RX_AACK configuration (page 47-48)
	XAH_CTRL_0 &= ~(1 <<  SLOTTED_OPERATION);					// -The radio transceiver operates in unslotted mode. An acknowledgment frame is automatically sent if requested.
	XAH_CTRL_1 &= ~(1 << AACK_UPLD_RES_FT |						// -Do not proceed reserved frames- do not generate RX_END interrupt
					1 << AACK_FLTR_RES_FT |						// -Do not check 3rd level (PAN/ADD) for reserved frames- do not generate _AMI interrupt
					1 << AACK_ACK_TIME);						// -12 symbols (12 * 16 us = 192 us) acknowledgment time
					
	fPhyTrxSetAackVersion(trxAackVersion0or1);

	CSMA_SEED_1 &= ~(1 << AACK_SET_PD);							// -The  content  of  AACK_SET_PD  bit  is  copied  into  the  frame  pending  subfield  of  the
																// acknowledgment  frame  if  the  acknowledgment  is  the  answer  to  a  data  request  MAC
																// command  frame.  If  in  addition  the  bits  AACK_FVN_MODE  of  this  register  are
																// configured  to  accept  frames  with  a  frame  version  other  than  0  or  1,  the  content  of
																// register  bit  AACK_SET_PD  is  also  copied  into  the  frame  pending  subfield  of  the
																// acknowledgment frame for any MAC command frame with a frame version of 2 or 3 that
																// have the security enabled subfield set to 1. This is done in the assumption that a future
																// version  of  the  IEEE  802.15.4  standard  might  change  the  length  or  structure  of  the
																// auxiliary  security  header,  so  that  it  is  not  possible  to  safely  detect  whether  the  MAC
																// command frame is actually a data request command or not.
					



	// Extended TX_ARET configuration (page 47-48)
	TRX_CTRL_1 |= (1 << TX_AUTO_CRC_ON);						//

	fPhyTrxSetRetriesFrameNumber(trxRetriesFrameNumber5);		// -ACK sensitive
	fPhyTrxSetRetriesCsmaNumber(trxRetriesCsmaNumber5);			// -CSMA (energy on channel) sensitive
	fPhyTrxSetCsmaSeed(0x1234);									//
	fPhyTrxSetBackOffExponentMax(trxBackOffExponent5);			// -Reset value
	fPhyTrxSetBackOffExponentMin(trxBackOffExponent3);			// -Reset value

	switch ((uint8_t) typeDevice)
	{
		case phyTypeDeviceRouter:
			CSMA_SEED_1 &= ~(1 <<  AACK_DIS_ACK);				// -Do not disable automatic ACK response
			CSMA_SEED_1 &= ~(1 << AACK_I_AM_COORD);				// -Don't be a PAN coordinator
			XAH_CTRL_1 &= ~(1 << AACK_PROM_MODE);				// -If this bit is set, every incoming frame with a valid
																// PHR finishes with a RX_END interrupt even if the third level filter rules do not match or
																// the FCS is not valid. The bit RX_CRC_VALID of register PHY_RSSI is set accordingly.
																// If this bit is set and a frame passes the third level filter rules, an acknowledgment frame
																// is  generated  and  transmitted  unless  disabled  by  bit  AACK_DIS_ACK  of  register CSMA_SEED_1.
			fPhyTrxSetAddShort(addShort);
			fPhyTrxSetAddPan(addPan);
			fPhyTrxSetAddIeee(addIeee);
			break;
		case phyTypeDeviceEndDevice:
			CSMA_SEED_1 &= ~(1 <<  AACK_DIS_ACK);				// -Do not disable automatic ACK response
			CSMA_SEED_1 &= ~(1 << AACK_I_AM_COORD);				// -Don't be a PAN coordinator
			XAH_CTRL_1 &= ~(1 << AACK_PROM_MODE);				// -If this bit is set, every incoming frame with a valid
																// PHR finishes with a RX_END interrupt even if the third level filter rules do not match or
																// the FCS is not valid. The bit RX_CRC_VALID of register PHY_RSSI is set accordingly.
																// If this bit is set and a frame passes the third level filter rules, an acknowledgment frame
																// is  generated  and  transmitted  unless  disabled  by  bit  AACK_DIS_ACK  of  register CSMA_SEED_1.
			fPhyTrxSetAddShort(addShort);
			fPhyTrxSetAddPan(addPan);
			fPhyTrxSetAddIeee(addIeee);
			break;
		case phyTypeDeviceCoordinator:
			CSMA_SEED_1 &= ~(1 <<  AACK_DIS_ACK);				// -Do not disable automatic ACK response
			CSMA_SEED_1 |= (1 << AACK_I_AM_COORD);				// -Be a PAN coordinator
			XAH_CTRL_1 &= ~(1 << AACK_PROM_MODE);				// -If this bit is set, every incoming frame with a valid
																// PHR finishes with a RX_END interrupt even if the third level filter rules do not match or
																// the FCS is not valid. The bit RX_CRC_VALID of register PHY_RSSI is set accordingly.
																// If this bit is set and a frame passes the third level filter rules, an acknowledgment frame
																// is  generated  and  transmitted  unless  disabled  by  bit  AACK_DIS_ACK  of  register CSMA_SEED_1.
			fPhyTrxSetAddShort(addShort);
			fPhyTrxSetAddPan(addPan);
			fPhyTrxSetAddIeee(addIeee);
			break;
		case phyTypeDeviceProm:
			CSMA_SEED_1 |= (1 <<  AACK_DIS_ACK);				// -Disable automatic ACK response
			CSMA_SEED_1 &= ~(1 << AACK_I_AM_COORD);				// -Don't be a PAN coordinator
			XAH_CTRL_1 |= (1 << AACK_PROM_MODE);				// -If this bit is set, every incoming frame with a valid
																// PHR finishes with a RX_END interrupt even if the third level filter rules do not match or
																// the FCS is not valid. The bit RX_CRC_VALID of register PHY_RSSI is set accordingly.
																// If this bit is set and a frame passes the third level filter rules, an acknowledgment frame
																// is  generated  and  transmitted  unless  disabled  by  bit  AACK_DIS_ACK  of  register CSMA_SEED_1.
			fPhyTrxSetAddShort(0);
			fPhyTrxSetAddPan(0);
			fPhyTrxSetAddIeee(0);
			break;
	}

	System.trx.phyTypeDevice = typeDevice;
}