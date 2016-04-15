/*
 * appLayer.h
 *
 * Created: 2016-04-10 17:19:37
 *  Author: Maciek
 */ 


#ifndef APPTRX_H_
#define APPTRX_H_

extern void fAppTrxInit(void);
extern void fAppTrxMessageSend(tMacMessage* macMessage,
						uint8_t* macMsdu,
						uint8_t macMsduLength);


#endif /* APPTRX_H_ */