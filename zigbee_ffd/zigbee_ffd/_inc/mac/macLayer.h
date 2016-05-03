/*
 * macLayer.h
 *
 * Created: 2016-04-10 17:06:53
 *  Author: Maciek
 */ 


#ifndef MACLAYER_H_
#define MACLAYER_H_

 /**	
* @brief: MAC sublayer constans (p. 145)
  @param: 
  *			
  *			
  */

#define MAC_CONST_aBaseSlotDuration				60
#define MAC_CONST_aBaseSuperframeDuration		(MAC_CONST_aBaseSlotDuration * MAC_CONST_aNumSuperframeSlots)
#define MAC_CONST_aGTSDescPersistenceTime		4
#define MAC_CONST_aMaxBeaconOverhead			75
#define MAC_CONST_aMaxBeaconPayloadLength		(PHY_CONST_aMaxPHYPacketSize - MAC_CONST_aMaxBeaconOverhead)
#define MAC_CONST_aMaxLostBeacons				4
#define MAC_CONST_aMaxMACSafePayloadSize		(PHY_CONST_aMaxPHYPacketSize - MAC_CONST_aMaxMPDUUnsecuredOverhead)
#define MAC_CONST_aMaxMACPayloadSize			(PHY_CONST_aMaxPHYPacketSize - MAC_CONST_aMinMPDUOverhead)
#define MAC_CONST_aMaxMPDUUnsecuredOverhead		25
#define MAC_CONST_aMaxSIFSFrameSize				18
#define MAC_CONST_aMinCAPLength					440
#define MAC_CONST_aMinMPDUOverhead				9
#define MAC_CONST_aNumSuperframeSlots			16
#define MAC_CONST_aUnitBackoffPeriod			20

 /**	
* @brief: MAC PIB attributes (p. 146)
  @param: 
  *			
  *			
  */

typedef struct {
	//uint64_t		macExtendedAddress;
	//uint8_t			macAckWaitDuration;
	//bool			macAssociatedPANCoord;
	//bool			macAssociationPermit;
	//bool			macAutoRequest;
	//bool			macBattLifeExt;
	//uint8_t			macBattLifeExtPeriods;
	////uint8_t			macBeaconPayload[];
	//uint8_t			macBeaconPayloadLength;
	//uint8_t			macBeaconOrder;
	//uint32_t		macBeaconTxTime;
	//uint8_t			macBSN;
	uint64_t		macCoordExtendedAddress;
	uint16_t		macCoordShortAddress;
	//uint8_t			macDSN;
	//bool			macGTSPermit;
	//uint8_t			macMaxBE;
	//uint8_t			macMaxCSMABackoffs;
	//uint8_t			macMaxFrameTotalWaitTime;
	//uint8_t			macMaxFrameRetries;
	//uint8_t			macMinBE;
	//uint8_t			macLIFSPeriod;
	//uint8_t			macSIFSPeriod;
	uint16_t		macPANId;
	//bool			macPromiscuousMode;
	//bool			macRangingSupported;
	//uint8_t			macResponseWaitTime;
	//bool			macRxOnWhenIdle;
	//bool			macSecurityEnabled;
	uint16_t		macShortAddress;
	//uint8_t			macSuperframeOrder;
	//uint8_t			macSyncSymbolOffset;
	//bool			macTimestampSupported;
	//uint16_t		macTransactionPersistenceTime;
	//uint32_t		macTxControlActiveDuration;
	//uint16_t		macTxControlPauseDuration;
	//uint32_t		macTxTotalDuration;
} tMacPib;

/**	
* @brief:	MAC - FCF- Frame Control Field typedefs
  *			
  *			
  */

#define MAC_POSITION_FCF						0

#define MAC_POSITION_FCF_FRAME_TYPE				0
#define MAC_POSITION_FCF_FRAME_TYPE_L			3

#define MAC_POSITION_FCF_SECURITY				3
#define MAC_POSITION_FCF_SECURITY_L				1

#define MAC_POSITION_FCF_FRAME_PENDING			4
#define MAC_POSITION_FCF_FRAME_PENDING_L		1

#define MAC_POSITION_FCF_ACK_REQUEST			5
#define MAC_POSITION_FCF_ACK_REQUEST_L			1

#define MAC_POSITION_FCF_INTRA_PAN				6
#define MAC_POSITION_FCF_INTRA_PAN_L			1

#define MAC_POSITION_FCF_DEST_ADD_MODE			(10 - 8)
#define MAC_POSITION_FCF_DEST_ADD_MODE_L		2

#define MAC_POSITION_FCF_FRAME_VERSION			(12 - 8)
#define MAC_POSITION_FCF_FRAME_VERSION_L		2

#define MAC_POSITION_FCF_SRC_ADD_MODE			(14 - 8)
#define MAC_POSITION_FCF_SRC_ADD_MODE_L			2

// 0-2 bits in FCF
typedef enum {
	macFrameTypeBeacon							= 0,
	macFrameTypeData							= 1,
	macFrameTypeAck								= 2,
	macFrameTypeMacCommand						= 3
} eMacFrameType;

// 3 bit in FCF
typedef enum {
	macSecurityDisabled							= 0,
	macSecurityEnabled							= 1
} eMacSecurity;

// 4 bit in FCF	
typedef enum {
	macFramePendingNo							= 0,
	macFramePendingYes							= 1
} eMacFramePending;

// 5 bit in FCF
typedef enum {
	macAckRequestNo								= 0,
	macAckRequestYes							= 1
} eMacAckRequest;

// 6 bit in FCF
typedef enum {
	macIntraPanNo								= 0,
	macIntraPanYes								= 1
} eMacIntraPan;

// 10-11 bits in FCF
typedef enum {
	macDestAddModePanAndAddFieldsNotPresent		= 0,
	macDestAddModeReserved						= 1,
	macDestAddModeAdd16Bit						= 2,
	macDestAddModeAdd64Bit						= 3
} eMacDestAddMode;

// 12-13 bits in FCF
typedef enum {
	macFrameVersionIeee2003						= 0,
	macFrameVersionIeee2006						= 1
} eMacFrameVersion;

// 14-15 bits in FCF
typedef enum {
	macSrcAddModePanAndAddFieldsNotPresent		= 0,
	macSrcAddModeReserved						= 1,
	macSrcAddModeAdd16Bit						= 2,
	macSrcAddModeAdd64Bit						= 3
} eMacSrcAddMode;

typedef struct {
	eMacFrameType		macFrameType;
	eMacSecurity		macSecurity;
	eMacFramePending	macFramePending;
	eMacAckRequest		macAckRequest;
	eMacIntraPan		macIntraPan;
	eMacDestAddMode		macDestAddMode;
	eMacFrameVersion	macFrameVersion;
	eMacSrcAddMode		macSrcAddMode;
} tMacFcf;

#define MAC_POSITION_AF							3

typedef struct {
	uint16_t			macDestPan;
	uint16_t			macDestAdd;
	uint16_t			macSrcPan;
	uint16_t			macSrcAdd;
} tMacAdd;

typedef struct {
	tMacFcf				macFcf;
	uint8_t				macSequenceNumber;
	tMacAdd				macAdd;
} tMacMhr;

 /**	
* @brief: MAC Beacon
  @param: Field dimensions in bits, sections in bytes
  *			
  *			
  */

 // SS Fields - Superframe Specification

#define MAC_POSITION_MSDU_BEACON_SS						0

#define MAC_POSITION_MSDU_BEACON_SS_BEACONORDER			0
#define MAC_POSITION_MSDU_BEACON_SS_BEACONORDER_L		4

#define MAC_POSITION_MSDU_BEACON_SS_SSORDER				4
#define MAC_POSITION_MSDU_BEACON_SS_SSORDER_L			4

#define MAC_POSITION_MSDU_BEACON_SS_FINALCAPSLOT		(8 - 8)
#define MAC_POSITION_MSDU_BEACON_SS_FINALCAPSLOT_L		4

#define MAC_POSITION_MSDU_BEACON_SS_BLE					(12 - 8)
#define MAC_POSITION_MSDU_BEACON_SS_BLE_L				1

#define MAC_POSITION_MSDU_BEACON_SS_PANCOORD			(14 - 8)
#define MAC_POSITION_MSDU_BEACON_SS_PANCOORD_L			1

#define MAC_POSITION_MSDU_BEACON_SS_ASSPERMIT			(15 - 8)
#define MAC_POSITION_MSDU_BEACON_SS_ASSPERMIT_L			1

typedef struct {
	uint8_t							beaconOrder;
	uint8_t							ssOrder;
	uint8_t							finalCapOrder;
	bool							ble;
	bool							panCoordinator;
	bool							associationPermit;
} tMacFrameTypeBeaconSs;

// GTS Fields - Guaranteed Time Slots

#define MAC_POSITION_MSDU_BEACON_GTSS					2

#define MAC_POSITION_MSDU_BEACON_GTSS_DESCCNT			0
#define MAC_POSITION_MSDU_BEACON_GTSS_DESCCNT_L			3

#define MAC_POSITION_MSDU_BEACON_GTSS_GTSPERM			7
#define MAC_POSITION_MSDU_BEACON_GTSS_GTSPERM_L			1		// If the value of this field is zero, the GTS Directions field and GTS List field of the beacon frame are not present.

typedef struct {
	uint8_t							gtsDescCount;
	bool							gtsPermit;
} tMacFrameTypeBeaconGtsGtsS;

typedef struct {
	tMacFrameTypeBeaconGtsGtsS		gtsS;
} tMacFrameTypeBeaconGts;

// PA Fields - Pending Address

#define MAC_POSITION_MSDU_BEACON_PA						3

#define MAC_POSITION_MSDU_BEACON_PANUMSADDPENDING		0
#define MAC_POSITION_MSDU_BEACON_PANUMSADDPENDING_L		3

#define MAC_POSITION_MSDU_BEACON_PANUMEADDPENDING		4
#define MAC_POSITION_MSDU_BEACON_PANUMEADDPENDING_L		3

typedef struct {
	uint8_t							numOfShortPending;
	uint8_t							numOfExtPending;
} tMacFrameTypeBeaconPaS;

typedef struct {
	tMacFrameTypeBeaconPaS			paS;
} tMacFrameTypeBeaconPa;

typedef struct {
	tMacFrameTypeBeaconSs			ss;
	tMacFrameTypeBeaconGts			gts;
	tMacFrameTypeBeaconPa			pa;
} tMacFrameTypeBeacon;

 /**	
* @brief: MAC Command
  @param: Field dimensions in bits, sections in bytes
  *			
  *			
  */

// Command Frame Identifier

#define MAC_POSITION_MSDU_COMMAND_IDENTIFIER			0

#define MAC_POSITION_MSDU_COMMAND_IDENTIFIER_ID			0
#define MAC_POSITION_MSDU_COMMAND_IDENTIFIER_ID_L		8

typedef enum {
	commandIdentifierAssociationRequest = 1,
	commandIdentifierAssociationResponse,
	commandIdentifierDisassociationRequest,
	commandIdentifierDataRequest,
	commandIdentifierPanIdConflictNoticication,
	commandIdentifierOrphanNotification,
	commandIdentifierBeaconRequest,
	commandIdentifierCoordinatorRealignment,
	commandIdentifierGtsRequest
} eMacFrameTypeCommandIdentifier;

typedef struct {
	eMacFrameTypeCommandIdentifier	identifier;
} tMacFrameTypeCommand;

// Command association request

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP			1

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_DTYPE		1
#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_DTYPE_L	1

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_PSRC		2
#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_PSRC_L		1

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_RXONIDLE	3
#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_RXONIDLE_L	1

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_SCAP		6
#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_SCAP_L		1

#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_ALLADD		7
#define MAC_POSITION_MSDU_COMMAND_ASSREQ_CAP_ALLADD_L	1

typedef struct {
	bool							deviceType;				// 1- FFD, 0- RFD
	bool							powerSource;			// 1- not from battery, 0- from battery
	bool							rxOnIdle;				// 1- receiving all the time,
	bool							securityCapability;		// 1- is capable receiving cryptographically protected MAC frames
	bool							allocateAddress;		// 1- shall be set to one if the device wishes the coordinator to allocate a short address as a result of the association procedure
} tMacFrameTypeCommandAssociationRequestCap;

typedef struct {
	tMacFrameTypeCommandAssociationRequestCap		cap;
} tMacFrameTypeCommandAssociationRequest;

// Command association response

#define MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD		1

#define MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_LSB	0
#define MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_LSB_L	8

#define MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_MSB	0
#define MAC_POSITION_MSDU_COMMAND_ASSRES_SHORTADD_MSB_L	8

#define MAC_POSITION_MSDU_COMMAND_ASSRES_ASSSTATUS		3

#define MAC_POSITION_MSDU_COMMAND_ASSRES_ASSSTATUS_STATUS	0
#define MAC_POSITION_MSDU_COMMAND_ASSRES_ASSSTATUS_STATUS_L	8

typedef enum {
	responseStatusSuccess = 0,
	responseStatusPanAtCapacity,
	responseStatusPanAccessDenied
} eMacFrameTypeCommandAssociationResponseStatus;

typedef struct {
	uint16_t										shortAddress;
	eMacFrameTypeCommandAssociationResponseStatus	status;
} tMacFrameTypeCommandAssociationResponse;

// Command disassociation request

typedef struct {
	
} tMacFrameTypeCommandDisassociationRequest;

// Command data request

typedef struct {
} tMacFrameTypeCommandDataRequest;

// Command PAN ID conflict noticication

typedef struct {
} tMacFrameTypeCommandPanIdConflictNoticication;

// Command orphan notification

typedef struct {
} tMacFrameTypeCommandOrphanNotification;

// Command beacon request

// The Destination Addressing Mode field shall be set to indicate short addressing, and the Source Addressing
// Mode field shall be set to indicate that the source addressing information is not present.
// The Frame Pending field shall be set to zero and ignored upon reception. The AR field and Security Enabled
// field shall also be set to zero.
// The Destination PAN Identifier field shall contain the broadcast PAN identifier. The Destination Address
// field shall contain the broadcast short address.

typedef struct {
} tMacFrameTypeCommandBeaconRequest;

// Command coordinator realignment

typedef struct {
} tMacFrameTypeCommandCoordinatorRealignment;

// Command GTS request

typedef struct {
} tMacFrameTypeCommandGtsRequest;

extern void fMacMessageFcfDecode (
	tMacFcf* macMessageFcfPointer,
	volatile uint8_t* addressSource);

extern void fMacMessageSequenceNumberDecode (
	uint8_t* macMessageSequenceNumberPointer,
	volatile uint8_t* addressSource);

extern uint8_t fMacMessageAddressingFieldsDecode (
	eMacDestAddMode macMessageDestAddField,
	eMacSrcAddMode macMessageSrcAddField,
	uint16_t* macMessageAddressingFieldsDestPanPointer,
	uint16_t* macMessageAddressingFieldsDestAddPointer,
	uint16_t* macMessageAddressingFieldsSrcPanPointer,
	uint16_t* macMessageAddressingFieldsSrcAddPointer,
	volatile uint8_t* addressSource);

extern void fMacMessageMsduDecode (uint8_t* macMessageMsduPointer,
	uint8_t frameOffset,
	uint8_t macMessageMpduLength,
	volatile uint8_t* addressSource);

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						GET SECTION										//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

extern eMacFrameType fMacGetFcfFrameType (
	uint8_t * buffer);

extern eMacDestAddMode fMacGetFcfDestAddMode (
	uint8_t * buffer);

extern eMacSrcAddMode fMacGetFcfSrcAddMode (
	uint8_t * buffer);

extern uint16_t fMacGetAfAddressPanDestination (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource);

extern uint16_t fMacGetAfAddressShortDestination (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource);

extern uint16_t fMacGetAfAddressPanSource (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource);

extern uint16_t fMacGetAfAddressShortSource (
	uint8_t * buffer,
	eMacDestAddMode modeDestination,
	eMacSrcAddMode modeSource);

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//						PREPARE SECTION									//
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

extern void fMacPrepareAssociationResponse (
	uint16_t destinationAddress,
	uint16_t givenAddress,
	eMacFrameTypeCommandAssociationResponseStatus status);

extern void fMacPrepareAssociationRequest (
	uint16_t destinationPan,
	uint16_t destinationAddress);

extern void fMacPrepareBeaconRequest (void);

extern void fMacMessageFcfPrepare (
	uint8_t* macMessageFcfPointer,
	eMacFrameType macFrameType,
	eMacSecurity macSecurity,
	eMacFramePending macFramePending,
	eMacAckRequest macAckRequest,
	eMacIntraPan macIntraPan,
	eMacDestAddMode macDestAddMode,
	eMacFrameVersion macFrameVersion,
	eMacSrcAddMode macSrcAddMode);

extern void fMacMessageSequenceNumberPrepare (
	uint8_t* macMessageSequenceNumberPointer,
	uint8_t sequenceNumber);

extern void fMacMessageAddressingFieldsPrepare (
	uint8_t * macMessageAddressingFieldsPointer,
	eMacDestAddMode macDestinationMode,
	eMacSrcAddMode macSourceMode,
	tMacAdd * macAddresses,
	uint8_t * length);

#endif /* MACLAYER_H_ */