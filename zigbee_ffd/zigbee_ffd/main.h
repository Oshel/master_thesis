/*
 * main.h
 *
 * Created: 2016-04-10 16:13:39
 *  Author: Maciek
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <macLayer.h>

typedef enum {
	false				= 0,
	true				= 1
} bool;

typedef enum {
	trxStateReset		= 0,
	trxStateTrxOff		= 1,
	trxStateSleep		= 2,
	trxStateRxOn		= 3,
	trxStatePllOn		= 4,
	trxStateBusyRx		= 5,
	trxStateBusyTx		= 6,
	trxStateTxAretOn	= 7,
	trxStateBusyTxAret	= 8,
	trxStateRxAackOn	= 9,
	trxStateBusyRxAack	= 10
} eTrxState;

typedef enum {
	trxPowerLevelP35dBm		= 0,
	trxPowerLevelP33dBm		= 1,
	trxPowerLevelP28dBm		= 2,
	trxPowerLevelP23dBm		= 3,
	trxPowerLevelP18dBm		= 4,
	trxPowerLevelP12dBm		= 5,
	trxPowerLevelP05dBm		= 6,
	trxPowerLevelM05dBm		= 7,
	trxPowerLevelM15dBm		= 8,
	trxPowerLevelM25dBm		= 9,
	trxPowerLevelM35dBm		= 10,
	trxPowerLevelM45dBm		= 11,
	trxPowerLevelM65dBm		= 12,
	trxPowerLevelM85dBm		= 13,
	trxPowerLevelM115dBm	= 14,
	trxPowerLevelM165dBm	= 15,

} eTrxPowerLevel;

typedef enum {
	trxChannel2405MHz		= 11,
	trxChannel2410MHz		= 12,
	trxChannel2415MHz		= 13,
	trxChannel2420MHz		= 14,
	trxChannel2425MHz		= 15,
	trxChannel2430MHz		= 16,
	trxChannel2435MHz		= 17,
	trxChannel2440MHz		= 18,
	trxChannel2445MHz		= 19,
	trxChannel2450MHz		= 20,
	trxChannel2455MHz		= 21,
	trxChannel2460MHz		= 22,
	trxChannel2465MHz		= 23,
	trxChannel2470MHz		= 24,
	trxChannel2475MHz		= 25,
	trxChannel2480MHz		= 26
} eTrxChannel;

typedef enum {
	trxReceiverSensivityNone		= 0,
	trxReceiverSensivityM90dBm		= 1,
	trxReceiverSensivityM87dBm		= 2,
	trxReceiverSensivityM84dBm		= 3,
	trxReceiverSensivityM81dBm		= 4,
	trxReceiverSensivityM78dBm		= 5,
	trxReceiverSensivityM75dBm		= 6,
	trxReceiverSensivityM72dBm		= 7,
	trxReceiverSensivityM69dBm		= 8,
	trxReceiverSensivityM66dBm		= 9,
	trxReceiverSensivityM63dBm		= 10,
	trxReceiverSensivityM60dBm		= 11,
	trxReceiverSensivityM57dBm		= 12,
	trxReceiverSensivityM54dBm		= 13,
	trxReceiverSensivityM51dBm		= 14,
	trxReceiverSensivityM48dBm		= 15
} eTrxReceiverSensivity;

typedef enum {
	trxCcaModeCsOrEd		= 0,
	trxCcaModeEd			= 1,
	trxCcaModeCs			= 2,
	trxCcaModeCsAndEd		= 3
} eTrxCcaMode;

typedef enum {
	trxRetriesFrameNumber0			= 0,
	trxRetriesFrameNumber1			= 1,
	trxRetriesFrameNumber2			= 2,
	trxRetriesFrameNumber3			= 3,
	trxRetriesFrameNumber4			= 4,
	trxRetriesFrameNumber5			= 5,
	trxRetriesFrameNumber6			= 6,
	trxRetriesFrameNumber7			= 7,
	trxRetriesFrameNumber8			= 8,
	trxRetriesFrameNumber9			= 9,
	trxRetriesFrameNumber10			= 10,
	trxRetriesFrameNumber11			= 11,
	trxRetriesFrameNumber12			= 12,
	trxRetriesFrameNumber13			= 13,
	trxRetriesFrameNumber14			= 14,
	trxRetriesFrameNumber15			= 15
} eTrxRetriesFrameNumber;

typedef enum {
	trxRetriesCsmaNumber0			= 0,
	trxRetriesCsmaNumber1			= 1,
	trxRetriesCsmaNumber2			= 2,
	trxRetriesCsmaNumber3			= 3,
	trxRetriesCsmaNumber4			= 4,
	trxRetriesCsmaNumber5			= 5,
	trxRetriesCsmaNumberImmediate	= 7
} eTrxRetriesCsmaNumber;

typedef enum {
	trxAackVersion0					= 0,
	trxAackVersion0or1				= 1,
	trxAackVersion0or1or2			= 2,
	trxAackVersionIndependent		= 3
} eTrxAackVersion;

typedef enum {
	trxBackOffExponentCsmaPeriod0	= 0,
	trxBackOffExponent1				= 1,
	trxBackOffExponent2				= 2,
	trxBackOffExponent3				= 3,
	trxBackOffExponent4				= 4,
	trxBackOffExponent5				= 5,
	trxBackOffExponent6				= 6,
	trxBackOffExponent7				= 7,
	trxBackOffExponent8				= 8, 
} eTrxBackOffExponent;

typedef struct {
	eMacFrameType		macFrameType;
	eMacSecurity		macSecurity;
	eMacFramePending	macFramePending;
	eMacAckRequest		macAckRequest;
	eMacIntraPan		macIntraPan;
	eMacDestAddMode		macDestAddMode;
	eMacFrameVersion	macFrameVersion;
	eMacSrcAddMode		macSrcAddMode;
} tMacFcf;

typedef struct {
	tMacFcf				macFcf;
	uint16_t			macSequenceNumber;
	uint16_t			macDestPan;
	uint16_t			macDestAdd;
	uint16_t			macSrcPan;
	uint16_t			macSrcAdd;
	uint64_t			macAuxSecurityHdr;
	uint16_t			macCrc;
} tMacMessage;

#endif /* MAIN_H_ */