/*
 * appTrx.c
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

void fAppTrxInit(void);

void fAppTrxSetPower(uint8_t powerLevel);
void fAppTrxSetChannel (eTrxChannel channel);
void fAppTrxSetCcaMode (eTrxCcaMode ccaMode);
void fAppTrxSetCcaEdThresh (int8_t thresholdInDbm);
void fAppTrxSetRxBufferLatchSetClear (bool setClearLatch);
void fAppTrxSetSuspendReceiving (bool setClearSuspend);
void fAppTrxSetReceiversSensivity (eTrxReceiverSensivity receiverSensivity);
void fAppTrxSetCsmaSeed (uint16_t seed);
void fAppTrxSetAackVersion (eTrxAackVersion aackVersion);

void fAppTrxSetBackOffExponentMax (eTrxBackOffExponent backOffExponent);
void fAppTrxSetBackOffExponentMin (eTrxBackOffExponent backOffExponent);

void fAppTrxSetRetriesFrameNumber (eTrxRetriesFrameNumber retriesFrameNumber);
void fAppTrxSetRetriesCsmaNumber (eTrxRetriesCsmaNumber retriesCsmaNumber);

void fAppTrxSetAddIeee (uint64_t addIeee);
void fAppTrxSetAddPan (uint16_t addPan);
void fAppTrxSetAddShort (uint16_t addShort);

void fAppTrxDoCalibrationFilterNetwork (void);
void fAppTrxDoCalibrationPllCenterFreq (void);
void fAppTrxDoCalibrationPllDelayCell (void);

int8_t fAppTrxReadEd (void);
int8_t fAppTrxReadRssi (void);

void fAppTrxAesSetKey(char* aesKey);
void fAppTrxAesWriteBuffer(uint8_t* aesBuffer);
void fAppTrxAesReadBuffer(uint8_t* aesBuffer);
void fAppTrxAesEncrypt(uint8_t* aesBuffer);
void fAppTrxAesDecrypt(uint8_t* aesBuffer);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxInit(void)
{
	uint8_t buffer;

	fAppTrxSetPower(trxPowerLevelP35dBm);						// Set power 3.5 dBm (maximum one)
	fAppTrxSetChannel(trxChannel2440MHz);						// Set channel to 2440 MHz
	fAppTrxSetCcaMode(trxCcaModeCsAndEd);						// Set CCA to carrier sense and energy detection
	fAppTrxSetCcaEdThresh(-90);									// Set ED for CCA on -60 dBm
	fAppTrxSetReceiversSensivity(trxReceiverSensivityNone);		// All frames with a valid SHR and PHR are received
	fAppTrxSetBackOffExponentMax(trxBackOffExponent5);			// Reset value
	fAppTrxSetBackOffExponentMin(trxBackOffExponent3);			// Reset value

	// Reset values
	// AES_CTRL - AES_MODE - 0x00 - AES Mode is ECB (Electronic Code Book)
	// TRX_CTRL_1 - - 0x01 - Enable Automatic CRC Calculation 
	// RX_CTRL - Receiver Sensitivity Control - 0x07 - Reset value, to be used if Antenna Diversity algorithm is disabled
	// SFD_VALUE - Start of Frame Delimiter Value - 0xA7 - Reset value, IEEE 802.15.4 compliant value of the SFD
	// TRX_CTRL_2 - Data Rate Selection - 0x00 - 250 kb/s (IEEE 802.15.4 compliant)
	// XAH_CTRL_1 - Filter Reserved Frames - 0x00 - If  AACK_FLTR_RES_FT = 0 a received, reserved frame is only checked for a valid FCS.
	// XAH_CTRL_1 - Process Reserved Frames - 0x00 - If  AACK_UPLD_RES_FT = 1 received frames indicated as reserved are further processed.
	// XAH_CTRL_1 - Reduce Acknowledgment Time - 0x00 - 12 symbols (12 * 16 us = 192 us) acknowledgment time
	// XAH_CTRL_1 - Enable Promiscuous Mode - 0x00 - If this bit is set, every incoming frame with a valid PHR finishes with a RX_END interrupt even if the third level filter rules do not match or the FCS is not valid. The bit RX_CRC_VALID of register PHY_RSSI is set accordingly.
	// XAH_CTRL_0 - Set Slotted Acknowledgment - 0x00 - The radio transceiver operates in unslotted mode. An acknowledgment frame is automatically sent if requested.
	// CSMA_SEED_1 - Set Frame Pending Sub-field - 0x00 - 
	// CSMA_SEED_1 - Disable Acknowledgment Frame Transmission - 0x00 - If  this  bit  is  set  no  acknowledgment  frames  are  transmitted  in  RX_AACK  Extended Operating Mode even if requested
	// CSMA_SEED_1 - Set Personal Area Network Coordinator - 0x00 - This register bit has to be set if the node is a PAN coordinator. It is used for address filtering in RX_AACK.

	buffer = IRQ_STATUS;						// If an interrupt is enabled it is recommended to read the interrupt status register IRQ_STATUS first to clear the history.

	IRQ_MASK |= (1 << PLL_LOCK_EN) |			// Interrupt enable
				(1 << PLL_UNLOCK_EN) |
				(1 << RX_START_EN) |
				(1 << RX_END_EN) |
				(1 << CCA_ED_DONE_EN) |
				(1 << AMI_EN) |
				(1 << TX_END_EN) |
				(1 << AWAKE_EN);
}

void fAppTrxMessageReceive (void)
{

}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxMessageSend (tMacMessage* macMessage,
						uint8_t* macMsdu,
						uint8_t macMsduLength)
{
	uint8_t sendBuffer [127];
	uint8_t byteCounter = 0;
	
	fMacMessageFcfPrepare(&sendBuffer[byteCounter],
						  macMessage->macFcf.macFrameType,
						  macMessage->macFcf.macSecurity,
						  macMessage->macFcf.macFramePending,
						  macMessage->macFcf.macAckRequest,
						  macMessage->macFcf.macIntraPan,
						  macMessage->macFcf.macDestAddMode,
						  macMessage->macFcf.macFrameVersion,
						  macMessage->macFcf.macSrcAddMode);

	byteCounter += 2;

	fMacMessageSequenceNumberPrepare(&sendBuffer[byteCounter],
									 macMessage->macSequenceNumber);
	
	byteCounter++;

	fMacMessageAddressingFieldsPrepare(&sendBuffer[byteCounter],
									   macMessage->macDestPan,
									   macMessage->macDestAdd,
									   macMessage->macSrcPan,
									   macMessage->macSrcAdd);
	
	byteCounter += 8;

	if (macMessage->macFcf.macSecurity == macSecurityEnabled)
	{
		fMacMessageAuxiliarySecurityHeaderPrepare(&sendBuffer[byteCounter],
												  macMessage->macAuxSecurityHdr);

		byteCounter += 5;
	}

	memcpy(&sendBuffer[byteCounter], macMsdu, macMsduLength);

	byteCounter += macMsduLength;

	// Wait for RX/TX states finishes
	do 
	{
		// Check current state
		fAppTrxStateCheck();
	} while (System.trx.state.trxStateCurrent == trxStateBusyRx ||
			 System.trx.state.trxStateCurrent == trxStateBusyRxAack ||
			 System.trx.state.trxStateCurrent == trxStateBusyTx ||
			 System.trx.state.trxStateCurrent == trxStateBusyTxAret);

	// Change state to PllOn to prevent from receiving new frames
	fAppTrxStateTransition(System.trx.state.trxStateCurrent, trxStatePllOn, false, false);

	// Load frame length
	TRXFBST = byteCounter;
	// Load the frame
	memcpy((void *)(&TRXFBST+1), sendBuffer, byteCounter);

	// Change state to BusyTx to send the frame
	fAppTrxStateTransition(System.trx.state.trxStateCurrent, trxStateBusyTx, false, false);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxAesWriteBuffer(uint8_t* aesBuffer)
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

void fAppTrxAesReadBuffer(uint8_t* aesBuffer)
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

void fAppTrxAesEncrypt(uint8_t* aesBuffer)
{
	fAppTrxAesSetKey(TRX_AES_KEY);
	fAppTrxAesWriteBuffer(aesBuffer);
	AES_CTRL &= ~(1 <<AES_DIR);
	AES_CTRL |= (1 << AES_REQUEST);
	while ((AES_STATUS & (1 << AES_DONE)) == false);		// 24 us
	fAppTrxAesReadBuffer(aesBuffer);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxAesDecrypt(uint8_t* aesBuffer)
{
	fAppTrxAesSetKey(TRX_AES_KEY);
	fAppTrxAesWriteBuffer(aesBuffer);
	AES_CTRL |= (1 << AES_DIR);
	AES_CTRL |= (1 << AES_REQUEST);
	while ((AES_STATUS & (1 << AES_DONE)) == false);		// 24 us
	fAppTrxAesReadBuffer(aesBuffer);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxAesSetKey(char* aesKey)
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

void fAppTrxSetBackOffExponentMax (eTrxBackOffExponent backOffExponent)
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

void fAppTrxSetBackOffExponentMin (eTrxBackOffExponent backOffExponent)
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

void fAppTrxSetAackVersion (eTrxAackVersion aackVersion)
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

void fAppTrxSetCsmaSeed(uint16_t seed)
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

void fAppTrxSetRetriesFrameNumber (eTrxRetriesFrameNumber retriesFrameNumber)
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

void fAppTrxSetRetriesCsmaNumber (eTrxRetriesCsmaNumber retriesCsmaNumber)
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

void fAppTrxSetAddIeee (uint64_t addIeee)
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

void fAppTrxSetAddPan (uint16_t addPan)
{
	// This  register  contains  the  lower/upper  8  bits  of  the  MAC  PAN  ID  for  Frame  Filter  address
	// recognition.

	PAN_ID_0 = (uint8_t) addPan;
	PAN_ID_1 = (uint8_t) (addPan >> 8);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxSetAddShort (uint16_t addShort)
{
	// This  register  contains  the  lower/upper  8  bits  of  the  MAC  short  address  for  Frame  Filter
	// address recognition.

	SHORT_ADDR_0 = (uint8_t) addShort;
	SHORT_ADDR_1 = (uint8_t) (addShort >> 8);
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxDoCalibrationPllDelayCell (void)
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

void fAppTrxDoCalibrationPllCenterFreq (void)
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

void fAppTrxDoCalibrationFilterNetwork (void)
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

void fAppTrxSetReceiversSensivity (eTrxReceiverSensivity receiverSensivity)
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

void fAppTrxSetSuspendReceiving (bool setClearSuspend)
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

void fAppTrxSetRxBufferLatchSetClear (bool setClearLatch)
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

void fAppTrxSetCcaEdThresh (int8_t thresholdInDbm)
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

void fAppTrxSetCcaMode (eTrxCcaMode ccaMode)
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

void fAppTrxSetChannel (eTrxChannel channel)
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

int8_t fAppTrxReadEd (void)
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

int8_t fAppTrxReadRssi (void)
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

void fAppTrxSetPower (eTrxPowerLevel powerLevel)
{
	PHY_TX_PWR &= ~(0x0F);		// Clear power registers
	PHY_TX_PWR |= ((uint8_t) powerLevel) << 0;
}