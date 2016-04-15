/*
 * trxStateTransitions.h
 *
 * Created: 2016-04-11 12:13:03
 *  Author: Maciek
 */ 


#ifndef TRXSTATETRANSITIONS_H_
#define TRXSTATETRANSITIONS_H_

#include "_inc/types.h"

typedef enum {
	trxStateReset					= 0x00,
	trxStateBusyRx					= 0x01,
	trxStateBusyTx					= 0x02,
	trxStateRxOn					= 0x06,
	trxStateTrxOff					= 0x08,
	trxStatePllOn					= 0x09,
	trxStateSleep					= 0x0F,
	trxStateBusyRxAack				= 0x11,
	trxStateBusyTxAret				= 0x12,
	trxStateRxAackOn				= 0x16,
	trxStateTxAretOn				= 0x19,
	trxStateTransitionInProgress	= 0x1F
} eTrxState;

extern eTrxState fAppTrxStateCheck (void);
extern bool fAppTrxStateTransition (eTrxState fromState, eTrxState toState, bool forceTrxOff, bool forcePllOn);

#endif /* TRXSTATETRANSITIONS_H_ */