/*
 * appLayer.h
 *
 * Created: 2016-04-10 17:19:37
 *  Author: Maciek
 */ 

 #include "_inc/types.h"

#ifndef APPTRX_H_
#define APPTRX_H_

#include "_inc/app/trxStateTransitions.h"

typedef struct {
	eTrxState						trxStateCurrent;
	eTrxState						trxStatePrevious;
} tTrxState;

typedef struct {
	uint8_t							psyPhr;
	tMacMessage						macMessage;
	uint8_t							macMsdu[122];
	uint8_t							phyLqi;
	int8_t							phyRssi;
	int8_t							phyEd;
} tTrxMessageReceived;

typedef struct {
	tTrxState						state;
	tTrxMessageReceived				messageReceived;
	int8_t							rssiAfterReceive;
} tTrx;

extern void fAppTrxInit(void);
extern void fAppTrxMessageReceive (tTrxMessageReceived* message,
								   volatile uint8_t* addressLength,
								   volatile uint8_t* addressSource);
extern void fAppTrxMessageSend(tMacMessage* macMessage,
						 	   uint8_t* macMsdu,
							   uint8_t macMsduLength);
extern int8_t fAppTrxReadRssi (void);


#endif /* APPTRX_H_ */