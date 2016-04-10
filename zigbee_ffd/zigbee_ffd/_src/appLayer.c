/*
 * appLayer.c
 *
 * Created: 2016-04-10 17:19:26
 *  Author: Maciek
 */ 
 
#include <main.h>

// Local definitions
// #define DEFINITION

#define TRX_AES_KEY	"MheBXkVCjOxRm35r"

// Local variables declarations
// uint8_t NameVariable;



// Functions declarations
// void nameFunction(uint8_t variableName);

void fAppTrxInit(void);

bool fAppTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceState);

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

void fAppTrxStateTransitionFromResetToTrxOff(void);

void fAppTrxStateTransitionFromTrxOffToSleep (void);
void fAppTrxStateTransitionFromTrxOffToRxOn (void);
void fAppTrxStateTransitionFromTrxOffToRxAackOn (void);
void fAppTrxStateTransitionFromTrxOffToPllOn (void);
void fAppTrxStateTransitionFromTrxOffToTxAaretOn (void);

void fAppTrxStateTransitionFromSleepToTrxOff (void);

void fAppTrxStateTransitionFromPllOnToTrxOff (uint8_t force);
void fAppTrxStateTransitionFromPllOnToRxOn (void);
void fAppTrxStateTransitionFromPllOnToBusyTx (void);
void fAppTrxStateTransitionFromPllOnToTxAretOn (void);
void fAppTrxStateTransitionFromPllOnToRxAackOn (void);

void fAppTrxStateTransitionFromRxOnToTrxOff (uint8_t force);
void fAppTrxStateTransitionFromRxOnToPllOn (void);

void fAppTrxStateTransitionFromRxAackOnToTrxOff (uint8_t force);
void fAppTrxStateTransitionFromRxAackOnToPllOn (void);

void fAppTrxStateTransitionFromTxAaretOnToPllOn (void);
void fAppTrxStateTransitionFromTxAaretOnToBusyTxAret (void);
void fAppTrxStateTransitionFromTxAaretOnToTrxOff (uint8_t force);

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
	fAppTrxSetCcaEdThresh(-60);									// Set ED for CCA on -60 dBm
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

	IRQ_MASK |= PLL_LOCK_EN |					// Interrupt enable
				PLL_UNLOCK_EN |
				RX_START_EN |
				RX_END_EN |
				CCA_ED_DONE_EN |
				AMI_EN |
				TX_END_EN |
				AWAKE_EN;
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
	AES_CTRL &= ~(AES_DIR);
	AES_CTRL |= AES_REQUEST;
	while ((AES_STATUS & AES_DONE) == false);		// 24 us
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
	AES_CTRL |= AES_DIR;
	AES_CTRL |= AES_REQUEST;
	while ((AES_STATUS & AES_DONE) == false);		// 24 us
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
	PLL_DCU |= PLL_DCU_START;
	while (PLL_DCU & PLL_DCU_START);	// At most 6 us
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxDoCalibrationPllCenterFreq (void)
{
	PLL_CF |= PLL_CF_START;
	while (PLL_CF & PLL_CF_START);		// Typical 35 us
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxDoCalibrationFilterNetwork (void)
{
	FTN_CTRL |= FTN_START;
	while (FTN_CTRL & FTN_START);		// At most 25 us
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
		RX_SYN |= RX_PDT_DIS;
	} else if (setClearSuspend == false)
	{
		RX_SYN &= ~RX_PDT_DIS;
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
		TRX_CTRL_2 |= RX_SAFE_MODE;
	} else if (setClearLatch == false)
	{
		TRX_CTRL_2 &= ~RX_SAFE_MODE;
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


/**	
* @brief: 	
  @param: 
  *			
  *			
  */

bool fAppTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceState)
{
	switch ((uint8_t) fromState)
	{
		case trxStateReset:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromResetToTrxOff();
					break;
				default:
					break;
			}
			break;
		case trxStateSleep:
			switch((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromSleepToTrxOff();
					break;
				default:
					break;
			}
			break;
		case trxStateTrxOff:
			switch((uint8_t) toState)
			{
				case trxStateSleep:
					fAppTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromTrxOffToPllOn();
					break;
				case trxStateRxOn:
					fAppTrxStateTransitionFromTrxOffToRxOn();
					break;
				case trxStateRxAackOn:
					fAppTrxStateTransitionFromTrxOffToRxAackOn();
					break;
				case trxStateTxAretOn:
					fAppTrxStateTransitionFromTrxOffToTxAaretOn();
					break;
				default:
					break;
			}
			break;
		case trxStateRxOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromRxOnToTrxOff(forceState);
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromRxOnToPllOn();
					break;
				default:
					break;
			}
			break;
		case trxStatePllOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromPllOnToTrxOff(forceState);
					break;
				case trxStateBusyTx:
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateRxAackOn:
					fAppTrxStateTransitionFromPllOnToRxAackOn();
					break;
				case trxStateRxOn:
					fAppTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateTxAretOn:
					fAppTrxStateTransitionFromPllOnToTxAretOn();
					break;
				default:
					break;
			}
			break;
		case trxStateTxAretOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromTxAaretOnToTrxOff(forceState);
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromTxAaretOnToPllOn();
					break;
				default:
					break;
			}
				break;
		case trxStateRxAackOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromRxAackOnToTrxOff(forceState);
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromRxAackOnToPllOn();
					break;
				default:
					break;
			}
			break;
	}

	return 0;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromResetToTrxOff(void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTrxOffToSleep (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTrxOffToRxOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTrxOffToRxAackOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTrxOffToPllOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTrxOffToTxAaretOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromSleepToTrxOff (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromPllOnToTrxOff (uint8_t force)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromPllOnToRxOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromPllOnToBusyTx (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromPllOnToTxAretOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromPllOnToRxAackOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromRxOnToTrxOff (uint8_t force)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromRxOnToPllOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromRxAackOnToTrxOff (uint8_t force)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromRxAackOnToPllOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTxAaretOnToPllOn (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTxAaretOnToBusyTxAret (void)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fAppTrxStateTransitionFromTxAaretOnToTrxOff (uint8_t force)
{
	
}