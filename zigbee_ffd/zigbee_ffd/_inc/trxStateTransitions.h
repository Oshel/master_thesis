/*
 * trxTransitions.h
 *
 * Created: 2016-04-11 12:13:03
 *  Author: Maciek
 */ 


#ifndef TRXTRANSITIONS_H_
#define TRXTRANSITIONS_H_

typedef enum {
	trxStateTrxOff					= 0,
	trxStateReset					= 1,
	trxStateSleep					= 2,
	trxStateRxOn					= 3,
	trxStatePllOn					= 4,
	trxStateBusyRx					= 5,
	trxStateBusyTx					= 6,
	trxStateTxAretOn				= 7,
	trxStateBusyTxAret				= 8,
	trxStateRxAackOn				= 9,
	trxStateBusyRxAack				= 10
} eTrxState;

#endif /* TRXTRANSITIONS_H_ */