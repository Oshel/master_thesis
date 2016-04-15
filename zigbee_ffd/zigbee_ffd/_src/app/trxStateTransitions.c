/*
 * trxStateTransitions.c
 *
 * Created: 2016-04-11 12:12:45
 *  Author: Maciek
 */ 

#include <main.h>

eTrxState fAppTrxStateCheck (void);
bool fAppTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceTrxOff, bool forcePllOn);

void fAppTrxStateTransitionFromResetToTrxOff(void);

void fAppTrxStateTransitionFromTrxOffToSleep (void);
void fAppTrxStateTransitionFromTrxOffToRxOn (void);
void fAppTrxStateTransitionFromTrxOffToRxAackOn (void);
void fAppTrxStateTransitionFromTrxOffToPllOn (bool force);
void fAppTrxStateTransitionFromTrxOffToTxAaretOn (void);

void fAppTrxStateTransitionFromSleepToTrxOff (void);

void fAppTrxStateTransitionFromPllOnToTrxOff (bool force);
void fAppTrxStateTransitionFromPllOnToRxOn (void);
void fAppTrxStateTransitionFromPllOnToBusyTx (void);
void fAppTrxStateTransitionFromPllOnToTxAretOn (void);
void fAppTrxStateTransitionFromPllOnToRxAackOn (void);

void fAppTrxStateTransitionFromRxOnToTrxOff (bool force);
void fAppTrxStateTransitionFromRxOnToPllOn (bool force);

void fAppTrxStateTransitionFromRxAackOnToTrxOff (bool force);
void fAppTrxStateTransitionFromRxAackOnToPllOn (bool force);

void fAppTrxStateTransitionFromTxAaretOnToPllOn (bool force);
void fAppTrxStateTransitionFromTxAaretOnToBusyTxAret (void);
void fAppTrxStateTransitionFromTxAaretOnToTrxOff (bool force);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

eTrxState fAppTrxStateCheck (void)
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

bool fAppTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceTrxOff, bool forcePllOn)
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
				case trxStatePllOn:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToRxOn();
					break;
				case trxStateRxAackOn:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToRxAackOn();
					break;
				case trxStateTxAretOn:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToTxAaretOn();
					break;
				case trxStateBusyTx:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromSleepToTrxOff();
					fAppTrxStateTransitionFromTrxOffToTxAaretOn();
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
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
					fAppTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
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
				case trxStateBusyTx:
					fAppTrxStateTransitionFromTrxOffToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromTrxOffToTxAaretOn();
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStateRxOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromRxOnToTrxOff(forceTrxOff);
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					break;
				case trxStateTxAretOn:
					fAppTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToTxAretOn();
					break;
				case trxStateRxAackOn:
					fAppTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToRxAackOn();
					break;
				case trxStateSleep:
					fAppTrxStateTransitionFromRxOnToTrxOff(forceTrxOff);
					fAppTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fAppTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromRxOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToTxAretOn();
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStatePllOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromPllOnToTrxOff(forceTrxOff);
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
				case trxStateSleep:
					fAppTrxStateTransitionFromPllOnToTrxOff(forceTrxOff);
					fAppTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromPllOnToTxAretOn();
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				default:
					break;
			}
			break;
		case trxStateTxAretOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromTxAaretOnToTrxOff(forceTrxOff);
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fAppTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateSleep:
					fAppTrxStateTransitionFromTxAaretOnToTrxOff(forceTrxOff);
					fAppTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fAppTrxStateTransitionFromTxAaretOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				default:
					break;
			}
				break;
		case trxStateRxAackOn:
			switch ((uint8_t) toState)
			{
				case trxStateTrxOff:
					fAppTrxStateTransitionFromRxAackOnToTrxOff(forceTrxOff);
					break;
				case trxStatePllOn:
					fAppTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					break;
				case trxStateRxOn:
					fAppTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToRxOn();
					break;
				case trxStateSleep:
					fAppTrxStateTransitionFromRxAackOnToTrxOff(forceTrxOff);
					fAppTrxStateTransitionFromTrxOffToSleep();
					break;
				case trxStateBusyTx:
					fAppTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToBusyTx();
					break;
				case trxStateBusyTxAret:
					fAppTrxStateTransitionFromRxAackOnToPllOn(forcePllOn);
					fAppTrxStateTransitionFromPllOnToTxAretOn();
					fAppTrxStateTransitionFromTxAaretOnToBusyTxAret();
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

void fAppTrxStateTransitionFromResetToTrxOff(void)
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

void fAppTrxStateTransitionFromTrxOffToSleep (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	TRXPR |= (1 << SLPTR);

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

void fAppTrxStateTransitionFromTrxOffToRxOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_ON;

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

void fAppTrxStateTransitionFromTrxOffToRxAackOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_AACK_ON;

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

void fAppTrxStateTransitionFromTrxOffToPllOn (bool force)
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

void fAppTrxStateTransitionFromTrxOffToTxAaretOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_ARET_ON;

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

void fAppTrxStateTransitionFromSleepToTrxOff (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);

	TRXPR &= ~(1 << SLPTR);

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

void fAppTrxStateTransitionFromPllOnToTrxOff (bool force)
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

void fAppTrxStateTransitionFromPllOnToRxOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_ON;

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

void fAppTrxStateTransitionFromPllOnToBusyTx (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_START;

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

void fAppTrxStateTransitionFromPllOnToTxAretOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_ARET_ON;

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

void fAppTrxStateTransitionFromPllOnToRxAackOn (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_RX_AACK_ON;

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

void fAppTrxStateTransitionFromRxOnToTrxOff (bool force)
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

void fAppTrxStateTransitionFromRxOnToPllOn (bool force)
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

void fAppTrxStateTransitionFromRxAackOnToTrxOff (bool force)
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

void fAppTrxStateTransitionFromRxAackOnToPllOn (bool force)
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

void fAppTrxStateTransitionFromTxAaretOnToPllOn (bool force)
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

void fAppTrxStateTransitionFromTxAaretOnToBusyTxAret (void)
{
	while ((TRX_STATUS & 0x1F) == STATE_TRANSITION_IN_PROGRESS);
	TRX_STATE &= ~0x1F;

	TRX_STATE |= CMD_TX_START;

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

void fAppTrxStateTransitionFromTxAaretOnToTrxOff (bool force)
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

	// If TRX_STATUS = 0x1F (STATE_TRANSITION_IN_PROGRESS) the radio transceiver
	// is  on  a  state  transition.  Do  not  try  to  initiate  a  further  state  change  while  the  radio
	// transceiver is in STATE_TRANSITION_IN_PROGRESS.

	System.trx.state.trxStatePrevious = System.trx.state.trxStateCurrent;
	System.trx.state.trxStateCurrent = trxStateTrxOff;
}