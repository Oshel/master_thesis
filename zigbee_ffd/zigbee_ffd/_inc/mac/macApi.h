/*
 * macApi.h
 *
 * Created: 2016-04-21 22:24:37
 *  Author: Maciek
 */ 


#ifndef MACAPI_H_
#define MACAPI_H_

 /**	
* @brief: MAC Api Enums
  @param: 
  *			
  *			
  */

typedef enum {
	macRequestTypeNone = 0,
	macRequestTypeAssociate,
	macRequestTypeDisassociate,
	macRequestTypeGet,
	macRequestTypeGts,
	macRequestTypeReset,
	macRequestTypeRxEnable,
	macRequestTypeScan,
	macRequestTypeSet,
	macRequestTypeStart,
	macRequestTypeSync,
	macRequestTypePoll,
	macRequestTypeDps,
	macRequestTypeSounding,
	macRequestTypeCalibrate,
	macRequestTypeData,
	macRequestTypePurge
} eMacRequestType;

typedef enum {
	macIndicationTypeNone = 0,
	macIndicationTypeAssociate,
	macIndicationTypeDisassociate,
	macIndicationTypeBeaconNotify,
	macIndicationTypeCommStatus,
	macIndicationTypeGts,
	macIndicationTypeOrphan,
	macIndicationTypeSyncLoss,
	macIndicationTypeDps,
	macIndicationTypeData
} eMacIndicationType;

typedef enum {
	macResponseTypeNone = 0,
	macResponseTypeAssociate,
	macResponseTypeOrphan
} eMacResponseType;

typedef enum {
	macConfirmTypeNone = 0,
	macConfirmTypeAssociate,
	macConfirmTypeDisassociate,
	macConfirmTypeGet,
	macConfirmTypeGts,
	macConfirmTypeReset,
	macConfirmTypeRxEnable,
	macConfirmTypeScan,
	macConfirmTypeSet,
	macConfirmTypeStart,
	macConfirmTypeSync,
	macConfirmTypePoll,
	macConfirmTypeDps,
	macConfirmTypeSounding,
	macConfirmTypeCalibrate,
	macConfirmTypeData,
	macConfirmTypePurge
} eMacConfirmType;

typedef enum {
	macScanTypeEd = 0,
	macScanTypeActive,
	macScanTypePassive,
	macScanTypeOrphan
} eMacScanType;

typedef enum {
	macScanStatusSuccess = 0,
	macScanStatusLimitReached,
	macScanStatusNoBeacon,
	macScanStatusScanInProgress,
	macScanStatusCounterError,
	macScanStatusFrameTooLong,
	macScanStatusUnavailableKey,
	macScanStatusUnsupportedSecurity,
	macScanStatusInvalidParameter
} eMacScanStatus;

typedef enum {
	macAssociationStatusSuccess = 0,
	macAssociationStatusPanAtCapacity,
	macAssociationStatusPanAccessDenied,
	macAssociationStatusChannelAccessFailure = 0x80,
	macAssociationStatusNoAck,
	macAssociationStatusNoData,
	macAssociationStatusCounterError,
	macAssociationStatusFrameTooLong,
	macAssociationStatusImproperKeyType,
	macAssociationStatusImproperSecurityLevel,
	macAssociationStatusSecurityError,
	macAssociationStatusUnavailableKey,
	macAssociationStatusUnsupportedLegacy,
	macAssociationStatusUnsupportedSecurity,
	macAssociationStatusInvalidParameter,
} eMacAssociationStatus;

typedef enum {
	macStartStatusSuccess = 0,
	macStartStatusNoShortAddress,
	macStartStatusSuperframeOverlap,
	macStartStatusTrackingOff,
	macStartStatusInvalidParameter,
	macStartStatusCounterError,
	macStartStatusFrameTooLong,
	macStartStatusUnavailableKey,
	macStartStatusUnsupportedSecurity,
	macStartStatusChannelAccessFailure
} eMacStartStatus;

typedef enum {
	macSetStatusSuccess = 0,
	macSetStatusReadOnly,
	macSetStatusUnsupportedAttribute,
	macSetStatusInvalidIndex,
	macSetStatusInvalidParameter
} eMacSetStatus;

 /**	
* @brief: MAC overall sub-frame formats
  @param: 
  *			
  *			
  */

typedef struct {
	//CoordAddrMode
	uint16_t			CoordPANId;
	uint16_t			CoordAddress;
	//ChannelNumber;
	//ChannelPage;
	//SuperframeSpec;
	//GTSPermit
	uint8_t				LinkQuality;
	//TimeStamp;
	//SecurityStatus;
	//SecurityLevel;
	//KeyIdMode
	//KeySource
	//KeyIndex
	//CodeList
} tMacPanDescriptor;

typedef struct {
	bool				deviceType;
	bool				powerSource;
	bool				receiverOnWhenIdle;
	bool				securityCapability;
	bool				allocateAddress;
} tMacCapabilityInformation;

 /**	
* @brief: MAC Requests
  @param: 
  *			
  *			
  */

typedef struct {
	uint8_t						ChannelNumber;
	//ChannelPage;
	//CoordAddrMode;	// Always 16-bit
	uint16_t					CoordPANId;
	uint16_t					CoordAddress;
	tMacCapabilityInformation	CapabilityInformation;
	//SecurityLevel;
	//KeyIdMode;
	//KeySource;
	//KeyIndex;
} tMacRequestAssociate;

typedef struct {

} tMacRequestDisassociate;

typedef struct {

} tMacRequestGet;

typedef struct {

} tMacRequestGts;

typedef struct {

} tMacRequestReset;

typedef struct {

} tMacRequestRxEnable;

typedef struct {
	eMacScanType		ScanType;
	uint32_t			ScanChannels;
	uint8_t				ScanDuration;
	//uint8_t				ScanPage;
	//uint8_t				SecurityLevel;
	//uint8_t				KeyIdMode;
	//uint8_t				KeySource[X];
	//uint8_t				KeyIndex;
} tMacRequestScan;

typedef struct {
	uint8_t *			PIBAttributeNamePointer;
	void *				PIBAttributeValue;
} tMacRequestSet;

typedef struct {
	uint16_t			PANId;
	uint16_t			ChannelNumber;
	//ChannelPage,
	//StartTime,
	//BeaconOrder,
	//SuperframeOrder,
	bool				PANCoordinator;
	//BatteryLifeExtension,
	//CoordRealignment,
	//CoordRealignSecurityLevel,
	//CoordRealignKeyIdMode,
	//CoordRealignKeySource,
	//CoordRealignKeyIndex,
	//BeaconSecurityLevel,
	//BeaconKeyIdMode,
	//BeaconKeySource,
	//BeaconKeyIndex

	// Added

	uint16_t			shortAdd;
} tMacRequestStart;

typedef struct {

} tMacRequestSync;

typedef struct {

} tMacRequestPoll;

typedef struct {

} tMacRequestDps;

typedef struct {

} tMacRequestSounding;

typedef struct {

} tMacRequestCalibrate;

typedef struct {

} tMacRequestData;

typedef struct {

} tMacRequestPurge;

 /**	
* @brief: MAC Indication
  @param: 
  *			
  *			
  */

typedef struct {
	uint16_t					DeviceAddress;			// from 64 bit to 16
	tMacCapabilityInformation	CapabilityInformation;
	//SecurityLevel;
	//KeyIdMode
	//KeySource
	//KeyIndex
} tMacIndicationAssociate;

typedef struct {

} tMacIndicationDisassociate;

typedef struct {
	//BSN;
	tMacPanDescriptor	PANDescriptor;
	//PendAddrSpec
	uint16_t			AddrList;
	//sduLength
	//sdu
} tMacIndicationBeaconNotify;

typedef struct {

} tMacIndicationCommStatus;

typedef struct {

} tMacIndicationGts;

typedef struct {

} tMacIndicationOrphan;

typedef struct {

} tMacIndicationSyncLoss;

typedef struct {

} tMacIndicationDps;

typedef struct {

} tMacIndicationData;

 /**	
* @brief: MAC Response
  @param: 
  *			
  *			
  */

typedef struct {
	//DeviceAddress
	uint16_t				AssocShortAddress;
	eMacAssociationStatus	status;
	//SecurityLevel
	//KeyIdMode
	//KeySource
	//KeyIndex
} tMacResponseAssociate;

typedef struct {

} tMacResponseOrphan;

 /**	
* @brief: MAC Confirm
  @param: 
  *			
  *			
  */

typedef struct {
	uint16_t				AssocShortAddress;
	eMacAssociationStatus	status;
	//SecurityLevel
	//KeyIdMode
	//KeySource
	//KeyIndex
} tMacConfirmAssociate;

typedef struct {
	
} tMacConfirmDisassociate;

typedef struct {
	
} tMacConfirmGet;

typedef struct {
	
} tMacConfirmGts;

typedef struct {

} tMacConfirmReset;

typedef struct {

} tMacConfirmRxEnable;

typedef struct {
	eMacScanStatus		status;
	eMacScanType		ScanType;
	//uint8_t				ChannelPage;
	//uint8_t				UnscannedChannels[X];
	uint8_t				ResultListSize;
	int8_t				EnergyDetectList[10];
	uint8_t				PANDescriptorList[10];
	//uint8_t				DetectedCategory;
	//uint8_t				UWBEnergyDetectList[X];

	// Added

	uint8_t				ShortAddressesDescriptorList[10];
} tMacConfirmScan;

typedef struct {
	eMacSetStatus		status;
	//uint8_t *			PIBAttributePointer;
	//uint8_t				PIBAttributeNameLength;
} tMacConfirmSet;

typedef struct {
	eMacStartStatus		status;
} tMacConfirmStart;

typedef struct {

} tMacConfirmPoll;

typedef struct {

} tMacConfirmDps;

typedef struct {

} tMacConfirmSounding;

typedef struct {

} tMacConfirmCalibrate;

typedef struct {

} tMacConfirmData;

typedef struct {

} tMacConfirmPurge;

void fMacApiRequestConfirm (
	void * request,
	void * confirm,
	eMacRequestType requestType,
	eMacConfirmType confirmType);

#endif /* MACAPI_H_ */