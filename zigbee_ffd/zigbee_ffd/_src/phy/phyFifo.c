/*
 * phyFifo.c
 *
 * Created: 2016-04-20 19:07:46
 *  Author: Maciek
 */ 

#include <main.h>

#define FIFO_LENGTH		5

void fPhyFifoTailAdd (void);
void fPhyFifoHeadAdd (void);

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

uint8_t fPhyFifoAdd(void)
{

	if (System.phy.fifo.phyFifoTail == (FIFO_LENGTH - 1))
	{
		if (System.phy.fifo.phyFifoHead == 0)
		{
			return 0;
		}
	} else {
		if (System.phy.fifo.phyFifoHead == (System.phy.fifo.phyFifoTail + 1))
		{
			return 0;
		}
	}

	System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoTail].phyFifoMessageLength = (TST_RX_LENGTH - 1);
	memcpy((&System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoTail].phyFifoMessage[0]), (void *)(&TRXFBST), 127);
	System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoTail].phyFifoMessageEd = fPhyTrxReadEd();
	System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoTail].phyFifoMessageLqi = *(&TRXFBST + TST_RX_LENGTH);
	System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoTail].phyFifoMessageRssi = fPhyTrxReadRssi();

	fPhyFifoTailAdd();

	return 1;
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

tPhyFifoMessage* fPhyFifoTake(void)
{
	tPhyFifoMessage* pointer;

	pointer = (tPhyFifoMessage *) &System.phy.fifo.phyFifoMessage[System.phy.fifo.phyFifoHead];

	if (System.phy.fifo.phyFifoHead != System.phy.fifo.phyFifoTail)
	{
		fPhyFifoHeadAdd();
		return pointer;
	} else {
		return 0;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyFifoTailAdd (void)
{
	if ((System.phy.fifo.phyFifoTail + 1) == FIFO_LENGTH)
	{
		System.phy.fifo.phyFifoTail = 0;
	} else {
		System.phy.fifo.phyFifoTail++;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyFifoHeadAdd (void)
{
	if ((System.phy.fifo.phyFifoHead + 1) == FIFO_LENGTH)
	{
		System.phy.fifo.phyFifoHead = 0;
		} else {
		System.phy.fifo.phyFifoHead ++;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

bool fPhyFifoTakeCheck (void)
{
	fPhyTrxStateTransition(fPhyTrxStateCheck(), trxStateRxAackOn , false, false);

	if (System.phy.fifo.phyFifoHead != System.phy.fifo.phyFifoTail)
	{
		return true;
	} else {
		return false;
	}
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fPhyFifoFlush (void)
{
	System.phy.fifo.phyFifoHead = System.phy.fifo.phyFifoTail;
}