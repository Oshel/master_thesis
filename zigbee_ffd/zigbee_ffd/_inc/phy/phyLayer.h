/*
 * phyLayer.h
 *
 * Created: 2016-05-08 15:25:28
 *  Author: Maciek
 */ 


#ifndef PHYLAYER_H_
#define PHYLAYER_H_

typedef struct
{
	uint8_t			phyCurrentChannel;
	//phyChannelsSupported;
	//phyTXPowerTolerance;
	//int8_t			phyTXPower;
	//uint8_t			phyCCAMode;
	//phyCurrentPage;
	//phyMaxFrameDuration;
	//phySHRDuration;
	//phySymbolsPerOctet;
	//phyPreambleSymbolLength;
	//phyUWBDataRatesSupported;
	//phyCSSLowDataRateSupported;
	//phyUWBCoUSupported;
	//phyUWBCSSupported;
	//phyUWBLCPSupported;
	//phyUWBCurrentPulseShape;
	//phyUWBCoUpulse;
	//phyUWBCSpulse;
	//phyUWBLCPWeight1;
	//phyUWBLCPWeight2;
	//phyUWBLCPWeight3;
	//phyUWBLCPWeight4;
	//phyUWBLCPDelay2;
	//phyUWBLCPDelay3;
	//phyUWBLCPDelay4;
	//phyRanging;
	//phyRangingCrystalOffset;
	//phyRangingDPS;
	//phyCurrentCode;
	//phyNativePRF;
	//phyUWBScanBinsPerChannel;
	//phyUWBInsertedPreambleInterval;
	//phyTXRMARKEROffset;
	//phyRXRMARKEROffset;
	//phyRFRAMEProcessingTime;
	//phyCCADuration;					//This attribute shall only be implemented with PHYs operating in the 950 MHz band.
} tPhyPib;



#endif /* PHYLAYER_H_ */