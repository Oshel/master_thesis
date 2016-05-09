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
#include <string.h>

#include "_inc/phy/phyTrx.h"
#include "_inc/phy/phyTrxStateTransitions.h"
#include "_inc/phy/phyFifo.h"
#include "_inc/phy/phyLayer.h"

#include "_inc/mac/macLayer.h"
#include "_inc/mac/macApi.h"

#include "_inc/nwk/nwkLayer.h"
#include "_inc/nwk/nwkApi.h"

#include "_inc/app/appSystemTimer.h"

typedef struct {
	uint16_t						test;
	uint16_t						scanDuration;
} tTimeOutMs;

typedef struct {
	eTrxStatus						txAretStatusFlag;
	bool							txCcaEdDone;
} tPhyFlags;

typedef struct {
	tPhyFifo						fifo;
	uint8_t							txBuf[127];
	uint8_t							txBufLength;
	tPhyFlags						flags;
	tPhyPib							pib;
} tPhy;

typedef struct {
	tMacPib							pib;
} tMac;

typedef struct {
	tNwkNib							nib;
} tNwk;

typedef struct {
	tTrx							trx;
	uint8_t							msg[127];
	
	tPhy							phy;
	tMac							mac;
	tNwk							nwk;
	//tAps							aps;

	tTimeOutMs						TimeOutMs;

	uint16_t						randomNumber;
} tSystem;

extern tSystem System;

void fMainMoveAndAttach (uint8_t * destination,
	uint8_t * source,
	uint8_t lengthDestination,
	uint8_t lengthSource);

#endif /* MAIN_H_ */