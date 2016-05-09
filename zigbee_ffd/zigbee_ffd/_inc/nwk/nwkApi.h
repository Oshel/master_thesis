/*
 * nwkApi.h
 *
 * Created: 2016-05-03 15:41:45
 *  Author: Maciek
 */ 


#ifndef NWKAPI_H_
#define NWKAPI_H_

 /**	
* @brief: NWK Api Enums
  @param: 
  *			
  *			
  */

typedef enum {
	nwkRequestNone = 0,
	nwkRequestNetworkDiscovery,
	nwkRequestNetworkFormation,
	nwkRequestPermitJoining,
	nwkRequestStartRouter,
	nwkRequestEdScan,
	nwkRequestJoin,
	nwkRequestDirectJoin,
	nwkRequestLeave,
	nwkRequestReset,
	nwkRequestSync,
	nwkRequestGet,
	nwkRequestSet,
	nwkRequestRouteDiscovery
} eNwkRequestType;

typedef enum {
	nwkIndicationNone = 0,
	nwkIndicationJoin,
	nwkIndicationLeave,
	nwkIndicationSyncLoss,
	nwkIndicationNwkStatus
} eNwkIndicationType;

typedef enum {
	nwkResponseNone = 0,
} eNwkResponseType;

typedef enum {
	nwkConfirmNone = 0,
	nwkConfirmNetworkDiscovery,
	nwkConfirmNetworkFormation,
	nwkConfirmPermitJoining,
	nwkConfirmStartRouter,
	nwkConfirmEdScan,
	nwkConfirmJoin,
	nwkConfirmDirectJoin,
	nwkConfirmLeave,
	nwkConfirmReset,
	nwkConfirmSync,
	nwkConfirmGet,
	nwkConfirmSet,
	nwkConfirmRouteDiscovery
} eNwkConfirmType;

typedef enum {
	nwkScanStatusSuccess = 0,
	nwkScanStatusLimitReached,
	nwkScanStatusNoBeacon,
	nwkScanStatusScanInProgress,
	nwkScanStatusCounterError,
	nwkScanStatusFrameTooLong,
	nwkScanStatusUnavailableKey,
	nwkScanStatusUnsupportedSecurity,
	nwkScanStatusInvalidParameter
} eNwkScanStatus;

typedef enum {
	nwkStartStatusSuccess,
	nwkStartStatusNoShortAddress,
	nwkStartStatusSuperframeOverlap,
	nwkStartStatusTrackingOff,
	nwkStartStatusInvalidParameter,
	nwkStartStatusCounterError,
	nwkStartStatusFrameTooLong,
	nwkStartStatusUnavailableKey,
	nwkStartStatusUnsupportedSecurity,
	nwkStartStatusChannelAccessFailure,
	nwkStartStatusInvalidRequest,
	nwkStartStatusStartupFailure
} eNwkStartStatus;

typedef enum {
	nwkDstAddrModeNoAddress = 0,
	nwkDstAddrMode16BitMulticast,
	nwkDstAddrMode16BitIndividual
} eNwkDstAddrMode;

typedef enum {
	nwkStatusCodeNoRouteAvailable = 0,
	nwkStatusCodeTreeLinkFailure,
	nwkStatusCodeNonTreeLinkFailure,
	nwkStatusCodeLowBatteryLevel,
	nwkStatusCodeNoRoutingCapacity,
	nwkStatusCodeNoIndirectCapacity,
	nwkStatusCodeIndirectTransactionExpiry,
	nwkStatusCodeTargetDeviceUnavailable,
	nwkStatusCodeTargetAdressUnallocated,
	nwkStatusCodeParentLinkFailure,
	nwkStatusCodeValidateRoute,
	nwkStatusCodeSourceRouteFailure,
	nwkStatusCodeManyToOneRouteFailure,
	nwkStatusCodeAddressConflict,
	nwkStatusCodeVerifyAddresses,
	nwkStatusCodePanIdentifierUpdate,
	nwkStatusCodeNetworkAdressUpdate,
	nwkStatusCodeBadFrameCounter,
	nwkStatusCodeBadKeySequenceNumber
} eNwkStatusCode;

typedef enum {
	nwkStatusRouteDiscoverySuccess,
	nwkStatusRouteDiscoveryTransactionOverflow,
	nwkStatusRouteDiscoveryTransactionExpired,
	nwkStatusRouteDiscoveryChannelAccessFailure,
	nwkStatusRouteDiscoveryInvalidAddress,
	nwkStatusRouteDiscoveryInvalidGts,
	nwkStatusRouteDiscoveryNoAck,
	nwkStatusRouteDiscoveryCounterError,
	nwkStatusRouteDiscoveryFrameTooLong,
	nwkStatusRouteDiscoveryUnavailableKey,
	nwkStatusRouteDiscoveryUnsupportedSecurity,
	nwkStatusRouteDiscoveryInvalidParameter,
	nwkStatusRouteDiscoveryInvalidRequest,
	nwkStatusRouteDiscoveryRouteError
} eNwkStatusRouteDiscovery;

// Not ZigBee Specification compatible!
typedef enum {
	nwkJoinStatusSuccess,
	nwkJoinStatusInvalidRequest,
	nwkJoinStatusNotPermitted,
	nwkJoinStatusNoNetworks
} eNwkJoinStatus;

 /**	
* @brief: NWK overall sub-frame formats
  @param: 
  *			
  *			
  */

typedef enum {
	nwkDeviceTypeCoordinator = 0,
	nwkDeviceTypeRouter,
	nwkDeviceTypeEndDevice
} eNwkDeviceType;

typedef enum {
	nwkRelationshipParent = 0,
	nwkRelationshipChild,
	nwkRelationshipSibling,
	nwkRelationshipNoneOfAbove,
	nwkRelationshipPreviousChild,
	nwkRelationshipUnauthenticatedChild
} eNwkRelationship;

typedef struct {
	//extendedAddress
	uint16_t			networkAddress;
	eNwkDeviceType		deviceType;
	//RxOnWhenIdle
	eNwkRelationship	relationship;
	uint8_t				transmitFailure;
	uint8_t				lqi;
	uint8_t				outgoingCost;
	//age
	//incomingBeaconTimestamp
	//beaconTransmissionTimeOffset
	//extendedPanId
	//logicalChannel
	uint8_t				depth;
	//beaconOrder
	//permitJoining
	//potentialParent

	// Added

	uint16_t			panId;
} tNwkTableNeighbour;

typedef struct {
	//ExtendedPANId
	//LogicalChannel
	//StackProfile
	//ZigBeeVersion
	//BeaconOrder
	//SuperframeOrder
	//PermitJoining
	//RouterCapacity
	//EndDeviceCapacity

	// Added

	uint16_t			PANId;
	uint16_t			ShortAdd;
	uint8_t				Lqi;
} tNwkNetworkDescriptor;

typedef struct {
	bool				deviceType;
	bool				powerSource;
	bool				receiverOnWhenIdle;
	bool				securityCapability;
	bool				allocateAddress;
} tNwkCapabilityInformation;

 /**	
* @brief: NWK Requests
  @param: 
  *			
  *			
  */
  
typedef struct {
	uint32_t			ScanChannels;
	uint8_t				ScanDuration;
} tNwkRequestNetworkDiscovery;

typedef struct {
	uint32_t			ScanChannels;
	uint8_t				ScanDuration;
	//BeaconOrder
	//SuperframeOrder
	//BatteryLifeExtension
} tNwkRequestNetworkFormation;

typedef struct {

} tNwkRequestPermitJoining;

typedef struct {
	//BeaconOrder,
	//SuperframeOrder,
	//BatteryLifeExtension
} tNwkRequestStartRouter;

typedef struct {

} tNwkRequestEdScan;

typedef struct {
	uint16_t					PANId;		// changed from 64 bit extended
	//RejoinNetwork,
	uint32_t					ScanChannels;
	uint8_t						ScanDuration;
	tNwkCapabilityInformation	CapabilityInformation;
	//SecurityEnable
} tNwkRequestJoin;

typedef struct {

} tNwkRequestDirectJoin;

typedef struct {

} tNwkRequestLeave;

typedef struct {

} tNwkRequestReset;

typedef struct {

} tNwkRequestSync;

typedef struct {

} tNwkRequestGet;

typedef struct {

} tNwkRequestSet;

typedef struct {
	eNwkDstAddrMode	DstAddrMode;
	uint16_t		DstAddr;
	uint8_t			Radius;
	bool			NoRouteCache;
} tNwkRequestRouteDiscovery;

 /**	
* @brief: NWK Indication
  @param: 
  *			
  *			
  */

typedef struct {
	uint16_t					NetworkAddress;
	//ExtendedAddress,
	tNwkCapabilityInformation	CapabilityInformation;
	//RejoinNetwork
	//SecureRejoin
} tNwkIndicationJoin;

typedef struct {

} tNwkIndicationLeave;

typedef struct {

} tNwkIndicationSyncLoss;

typedef struct {

} tNwkIndicationNwkStatus;

 /**	
* @brief: NWK Response
  @param: 
  *			
  *			
  */



 /**	
* @brief: NWK Confirm
  @param: 
  *			
  *			
  */

typedef struct {
	eNwkScanStatus			Status;
	uint8_t					NetworkCount;
	tNwkNetworkDescriptor	networkDescriptor[10];
} tNwkConfirmNetworkDiscovery;

typedef struct {
	eNwkStartStatus			Status;
} tNwkConfirmNetworkFormation;

typedef struct {

} tNwkConfirmPermitJoining;

typedef struct {
	eNwkStartStatus			Status;
} tNwkConfirmStartRouter;

typedef struct {

} tNwkConfirmEdScan;

typedef struct {
	eNwkJoinStatus			Status;
	uint16_t				NetworkAddress;
	uint16_t				PANID;
	uint8_t					ActiveChannel;
} tNwkConfirmJoin;

typedef struct {

} tNwkConfirmDirectJoin;

typedef struct {

} tNwkConfirmLeave;

typedef struct {

} tNwkConfirmReset;

typedef struct {

} tNwkConfirmSync;

typedef struct {

} tNwkConfirmGet;

typedef struct {

} tNwkConfirmSet;

typedef struct {
	eNwkStatusRouteDiscovery	Status;
	eNwkStatusCode				NetworkStatusCode;
} tNwkConfirmRouteDiscovery;

#endif /* NWKAPI_H_ */