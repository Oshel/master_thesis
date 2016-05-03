/*
 * trxStateTransitions.c
 *
 * Created: 2016-04-11 12:12:45
 *  Author: Maciek
 */ 

#include <main.h>

eTrxState fPhyTrxStateCheck (void);
eTrxStatus fPhyTrxStatusCheck (void);
bool fPhyTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceTrxOff, bool forcePllOn);

void fPhyTrxStateTransitionFromResetToTrxOff(void);

void fPhyTrxStateTransitionFromTrxOffToSleep (void);
void fPhyTrxStateTransitionFromTrxOffToRxOn (void);
void fPhyTrxStateTransitionFromTrxOffToRxAackOn (void);
void fPhyTrxStateTransitionFromTrxOffToPllOn (bool force);
void fPhyTrxStateTransitionFromTrxOffToTxAaretOn (void);

void fPhyTrxStateTransitionFromSleepToTrxOff (void);

void fPhyTrxStateTransitionFromPllOnToTrxOff (bool force);
void fPhyTrxStateTransitionFromPllOnToRxOn (void);
void fPhyTrxStateTransitionFromPllOnToBusyTx (void);
void fPhyTrxStateTransitionFromPllOnToTxAretOn (void);
void fPhyTrxStateTransitionFromPllOnToRxAackOn (void);

void fPhyTrxStateTransitionFromRxOnToTrxOff (bool force);
void fPhyTrxStateTransitionFromRxOnToPllOn (bool force);

void fPhyTrxStateTransitionFromRxAackOnToTrxOff (bool force);
void fPhyTrxStateTransitionFromRxAackOnToPllOn (bool force);

void fPhyTrxStateTransitionFromTxAaretOnToPllOn (bool force);
void fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret (void);
void fPhyTrxStateTransitionFromTxAaretOnToTrxOff (bool force);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

eTrxState fPhyTrxStateCheck (void)
{
	eTrxState actualState;

	actualState = (eTrxState)(TRX_STATUS & 0x1F);

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = actualState;

	return actualState;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

eTrxStatus fPhyTrxStatusCheck (void)
{
	eTrxStatus actualStatus;

	actualStatus = (eTrxStatus)((TRX_STATE >> 5) & 0x07);

	System.trx.status.trxStatusCurrent = actualStatus;

	return actualStatus;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

bool fPhyTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceTrxOff, bool forcePllOn)
{
	switch ((uint8_t) fromState)
	{
		case trxStateReset:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromResetToTrxOff();
					break;
				default:
					break;
			}
			break;
		case trxStateSleep:
			switch((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					break;
				case trxStatePllOn:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToRxOn();
					break;
				case trxStateRxAackOn:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToRxAackOn();
					break;
				case trxStateTxAretOn:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToTxAaretOn();
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromSleepToTrxOff();
					fPhyTrxStateTransitionFromTrxOffToTxAaretOn();
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStateTrxOff:
			switch((uint8_t) toState)
			{
				case trxStateSleep:
					fPhyTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStatePllOn:
					fPhyTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fPhyTrxStateTransitionFromTrxOffToRxOn();
					break;
				case trxStateRxAackOn:
					fPhyTrxStateTransitionFromTrxOffToRxAackOn();
					break;
				case trxStateTxAretOn:
					fPhyTrxStateTransitionFromTrxOffToTxAaretOn();
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromTrxOffToTxAaretOn();
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStateRxOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromRxOnToTrxOff(forceTrxOff);
					break;
				case trxStatePllOn:
					fPhyTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					break;
				case trxStateTxAretOn:
					fPhyTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToTxAretOn();
					break;
				case trxStateRxAackOn:
					fPhyTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToRxAackOn();
					break;
				case trxStateSleep:
					fPhyTrxStateTransitionFromRxOnToTrxOff(forceTrxOff);
					fPhyTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToTxAretOn();
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStatePllOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromPllOnToTrxOff(forceTrxOff);
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateRxAackOn:
					fPhyTrxStateTransitionFromPllOnToRxAackOn();
					break;
				case trxStateRxOn:
					fPhyTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateTxAretOn:
					fPhyTrxStateTransitionFromPllOnToTxAretOn();
					break;
				case trxStateSleep:
					fPhyTrxStateTransitionFromPllOnToTrxOff(forceTrxOff);
					fPhyTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromPllOnToTxAretOn();
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStateTxAretOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromTxAaretOnToTrxOff(forceTrxOff);
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				case trxStatePllOn:
					fPhyTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fPhyTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateSleep:
					fPhyTrxStateTransitionFromTxAaretOnToTrxOff(forceTrxOff);
					fPhyTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				default:
					break;
			}
				break;
		case trxStateRxAackOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fPhyTrxStateTransitionFromRxAackOnToTrxOff(forceTrxOff);
					break;
				case trxStatePllOn:
					fPhyTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fPhyTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateSleep:
					fPhyTrxStateTransitionFromRxAackOnToTrxOff(forceTrxOff);
					fPhyTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fPhyTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fPhyTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fPhyTrxStateTransitionFromPllOnToTxAretOn();
					fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
	}

	return false;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromResetToTrxOff(void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	TRXPR &= ~(1 << TRXRST);

	// Entering  the  TRX_OFF  state  from  radio  transceiver  SLEEP,  or  RESET  state  is
	// indicated by the TRX24_AWAKE interrupt.  

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS. 
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTrxOffToSleep (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	TRXPR |= (1 << SLPTR);

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateSleep;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTrxOffToRxOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_ON;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// During  RX_ON  state  the  receiver  listens  for  incoming  frames.  After  detecting  a  valid
	// synchronization  header (SHR), the receiver automatically enters the  BUSY_RX state. 

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateRxOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTrxOffToRxAackOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	// Note that a state change request from TRX_OFF to RX_AACK_ON or TX_ARET_ON
	// internally passes the state PLL_ON to initiate the radio transceiver. Thus the readiness
	// to receive or transmit data is delayed accordingly. It is recommended to use interrupt
	// TRX24_PLL_LOCK as an indicator

	//System.trx.pllLockForExtended = false;

	TRX_STATE |= CMD_RX_AACK_ON;

	//while (System.trx.pllLockForExtended == false);

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// During  RX_ON  state  the  receiver  listens  for  incoming  frames.  After  detecting  a  valid
	// synchronization  header (SHR), the receiver automatically enters the  BUSY_RX state.

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateRxAackOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTrxOffToPllOn (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_PLL_ON;
	} else if (force == false)
	{
		TRX_STATE |= CMD_PLL_ON;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStatePllOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTrxOffToTxAaretOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	// Note that a state change request from TRX_OFF to RX_AACK_ON or TX_ARET_ON
	// internally passes the state PLL_ON to initiate the radio transceiver. Thus the readiness
	// to receive or transmit data is delayed accordingly. It is recommended to use interrupt
	// TRX24_PLL_LOCK as an indicator

	//System.trx.pllLockForExtended = false;

	TRX_STATE |= CMD_TX_ARET_ON;

	//while (System.trx.pllLockForExtended == false);

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTxAretOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromSleepToTrxOff (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	TRXPR &= ~(1 << SLPTR);

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// Entering  the  TRX_OFF  state  from  radio  transceiver  SLEEP,  or  RESET  state  is
	// indicated by the TRX24_AWAKE interrupt.

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.
	
	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromPllOnToTrxOff (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_TRX_OFF;
	} else if (force == false)
	{
		TRX_STATE |= CMD_TRX_OFF;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromPllOnToRxOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_ON;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// During  RX_ON  state  the  receiver  listens  for  incoming  frames.  After  detecting  a  valid
	// synchronization  header (SHR), the receiver automatically enters the  BUSY_RX state.

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateRxOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromPllOnToBusyTx (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_START;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateBusyTx;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromPllOnToTxAretOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_ARET_ON;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTxAretOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromPllOnToRxAackOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_AACK_ON;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// During  RX_ON  state  the  receiver  listens  for  incoming  frames.  After  detecting  a  valid
	// synchronization  header (SHR), the receiver automatically enters the  BUSY_RX state.

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateRxAackOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromRxOnToTrxOff (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_TRX_OFF;
	} else if (force == false)
	{
		TRX_STATE |= CMD_TRX_OFF;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromRxOnToPllOn (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_PLL_ON;
	} else if (force == false)
	{
		TRX_STATE |= CMD_PLL_ON;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStatePllOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromRxAackOnToTrxOff (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_TRX_OFF;
	} else if (force == false)
	{
		TRX_STATE |= CMD_TRX_OFF;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromRxAackOnToPllOn (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_PLL_ON;
	} else if (force == false)
	{
		TRX_STATE |= CMD_PLL_ON;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStatePllOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTxAaretOnToPllOn (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_PLL_ON;
	} else if (force == false)
	{
		TRX_STATE |= CMD_PLL_ON;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStatePllOn;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTxAaretOnToBusyTxAret (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_START;

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateBusyTxAret;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyTrxStateTransitionFromTxAaretOnToTrxOff (bool force)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	if (force == true)
	{
		TRX_STATE |= CMD_FORCE_TRX_OFF;
	} else if (force == false)
	{
		TRX_STATE |= CMD_TRX_OFF;
	}

	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}