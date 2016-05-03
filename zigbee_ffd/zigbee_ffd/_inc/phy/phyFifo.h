/*
 * phyFifo.h
 *
 * Created: 2016-04-20 19:08:13
 *  Author: Maciek
 */ 


#ifndef PHYFIFO_H_
#define PHYFIFO_H_

typedef struct {
	uint8_t							phyFifoMessage[127];			// FIFO? 127 bytes of message, 1 byte of length
	uint8_t							phyFifoMessageLength;
	int8_t							phyFifoMessageEd;
	int8_t							phyFifoMessageRssi;
	uint8_t							phyFifoMessageLqi;
} tPhyFifoMessage;

typedef struct {
	tPhyFifoMessage					phyFifoMessage[5];
	uint8_t							phyFifoTail;
	uint8_t							phyFifoHead;
} tPhyFifo;

extern uint8_t fPhyFifoAdd (void);
extern uint8_t fPhyFifoTakeCheck (void);
extern tPhyFifoMessage* fPhyFifoTake(void);
extern void fPhyFifoFlush (void);

#endif /* PHYFIFO_H_ */