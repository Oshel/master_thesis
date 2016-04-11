/*
 * trxInterrupts.c
 *
 * Created: 2016-04-10 18:52:11
 *  Author: Maciek
 */ 

#include <main.h>

 /**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_AWAKE_vect)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_CCA_ED_DONE_vect)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_PLL_LOCK_vect)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_PLL_UNLOCK_vect)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_RX_END_vect)
{
	// The completion  of  the  frame  reception  is  indicated  by  an
	// TRX24_RX_END interrupt and the radio transceiver reenters the state RX_ON.
	// At the same time the bits RX_CRC_VALID of register PHY_RSSI are updated with the result
	// of the FCS check.
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_RX_START_vect)
{
	// During  RX_ON  state  the  receiver  listens  for  incoming  frames.  After  detecting  a  valid
	// synchronization  header (SHR), the receiver automatically enters the  BUSY_RX state. 
	// The reception of a valid PHY header (PHR) generates an TRX24_RX_START interrupt
	// and receives and demodulates the PSDU data. 
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_TX_END_vect)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

ISR(TRX24_XAH_AMI_vect)
{
	//If  the  content  of  the  MAC  addressing  fields  of  a  frame    (refer  to
	//IEEE 802.15.4  section  7.2.1)  matches  to  the  expected  addresses,  which  is  further
	//dependent on the addressing mode, an address match interrupt (TRX24_XAH_AMI) is
	//issued, refer to "Interrupt  Logic" on page 34. The expected address values are to be
	//stored  in  the  registers  Short-Address,  PAN-ID  and  IEEE-address.  Frame  filtering  is
	//available in Basic and Extended Operating Mode, refer to section "Frame Filtering" on page 56.
}

