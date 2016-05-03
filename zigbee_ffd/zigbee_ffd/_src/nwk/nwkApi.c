/*
 * nwkApi.c
 *
 * Created: 2016-05-03 15:42:22
 *  Author: Maciek
 */ 

#include <main.h>

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fNwkApiRequestConfirm (void * request, void * confirm, eNwkRequestType requestType, eNwkConfirmType confirmType)
{
	
}

/**	
* @brief: 	
  @param: 
  *			
  *			
  */

void fMacApiIndicationResponse (void * indication, void * response, eMacIndicationType indicationType, eMacResponseType responseType)
{
	if (indicationType == macIndicationTypeAssociate)
	{
		tMacIndicationAssociate *indicationPointer = indication;
		tMacResponseAssociate *reponsePointer = response;

		// join indication

	} else if (indicationType == macIndicationTypeBeaconNotify)
	{
		tMacIndicationBeaconNotify *indicationPointer = indication;

		uint8_t i;
		bool thereIsOne;
		
		for (i = 0; i < 10; i++)			// Check if that device is already in neighbour tables
		{
			if (System.nwk.tableNeighbour[i].networkAddress == indicationPointer->AddrList &&
				System.nwk.tableNeighbour[i].panId == indicationPointer->PANDescriptor.CoordPANId)
			{
				thereIsOne = true;			// Indicate that device is in database
			}
		}

		if (thereIsOne == false)			// Save new device if it is not present in database
		{
			for (i = 0; i < 10; i++)		// Scan for a free slot
			{
				if (System.nwk.tableNeighbour[i].networkAddress == 0)		// Select free slot
				{
					System.nwk.tableNeighbour[i].networkAddress = indicationPointer->AddrList;
					System.nwk.tableNeighbour[i].panId = indicationPointer->PANDescriptor.CoordPANId;
					System.nwk.tableNeighbour[i].lqi = indicationPointer->PANDescriptor.LinkQuality;
					break;
				}
			}
		}
	} else if (indicationType == macIndicationTypeOrphan)
	{

	}
}