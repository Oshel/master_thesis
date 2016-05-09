/*
 * nwkLayer.h
 *
 * Created: 2016-04-10 17:07:06
 *  Author: Maciek
 */ 


#ifndef NWKLAYER_H_
#define NWKLAYER_H_

#include "nwkApi.h"

 /**	
* @brief: NWK sublayer constans
  @param: 
  *			
  *			
  */
  
#define NWK_CONST_CoordinatorCapable			1
#define NWK_CONST_DefaultSecurityLevel			0
#define NWK_CONST_MinHeaderOverhead				0x08
#define NWK_CONST_ProtocolVersion				0x02
#define NWK_CONST_WaitBeforeValidation			0x9C40
#define NWK_CONST_RouteDiscoveryTime			0xC4B4
#define NWK_CONST_MaxBroadcastJitter			0x7D0
#define NWK_CONST_InitialRREQRetries			0x03
#define NWK_CONST_RREQRetries					0x02
#define NWK_CONST_RREQRetryInterval				0x1F02
#define NWK_CONST_MinRREQJitter					0x3F
#define NWK_CONST_MaxRREQJitter					0xFA0
#define NWK_CONST_MACFrameOverhead				0x0B

 /**	
* @brief: NWK NIB attributes
  @param: 
  *			
  *			
  */

typedef struct {
	//nwkSequenceNumber
	uint8_t							nwkPassiveAckTimeout;
	uint8_t							nwkMaxBroadcastRetries;
	uint8_t							nwkMaxChildren;
	uint8_t							nwkMaxDepth;
	uint8_t							nwkMaxRouters;
	tNwkTableNeighbour				nwkNeighborTable[10];
	//nwkNetworkBroadcastDeliveryTime
	//nwkReportConstantCost
	//nwkRouteTable					// TODO
	//nwkSymLink
	tNwkCapabilityInformation		nwkCapabilityInformation;
	//nwkAddrAlloc
	//nwkUseTreeRouting
	//nwkManagerAddr
	uint8_t							nwkMaxSourceRoute;
	//nwkUpdateId
	//nwkTransactionPersistenceTime
	uint16_t						nwkNetworkAddress;
	//nwkStackProfile
	//nwkBroadcastTransactionTable	// TODO
	//nwkGroupIDTable
	//nwkExtendedPANID
	//nwkUseMulticast
	//nwkRouteRecordTable			// TODO
	//nwkIsConcentrator
	//nwkConcentratorRadius
	//nwkConcentratorDiscoveryTime
	//nwkSecurityLevel
	//nwkSecurityMaterialSet
	//nwkActiveKeySeqNumber
	//nwkAllFresh
	//nwkSecureAllFrames
	//nwkLinkStatusPeriod
	uint8_t							nwkRouterAgeLimit;
	//nwkUniqueAddr
	//nwkAddressMap
	//nwkTimeStamp
	uint16_t						nwkPANId;
	uint16_t						nwkTxTotal;
	//nwkLeaveRequestAllowed
} tNwkNib;

/**	
* @brief:	NWK - FCF- Frame Control Field typedefs
  *			
  *			
  */

#define NWK_POSITION_FCF						0

#define NWK_POSITION_FCF_FRAME_TYPE				0
#define NWK_POSITION_FCF_FRAME_TYPE_L			2

#define NWK_POSITION_FCF_PROTOCOL_VER			2
#define NWK_POSITION_FCF_PROTOCOL_VER_L			4

#define NWK_POSITION_FCF_DISCOVERY_ROUTE		6
#define NWK_POSITION_FCF_DISCOVERY_ROUTE_L		2

#define NWK_POSITION_FCF_MULTI_FLAG				(8 - 8)
#define NWK_POSITION_FCF_MULTI_FLAG_L			1

#define NWK_POSITION_FCF_SECURITY				(9 - 8)
#define NWK_POSITION_FCF_SECURITY_L				1

#define NWK_POSITION_FCF_SOURCE_ROUTE			(10 - 8)
#define NWK_POSITION_FCF_SOURCE_ROUTE_L			1

#define NWK_POSITION_FCF_DEST_IEEE				(11 - 8)
#define NWK_POSITION_FCF_DEST_IEEE_L			1

#define NWK_POSITION_FCF_SOURCE_IEEE			(12 - 8)
#define NWK_POSITION_FCF_SOURCE_IEEE_L			1

typedef enum {
	nwkFrameTypeData = 0,
	nwkFrameTypeNwkCommand,
	nwkFrameTypeReserved,
	nwkFrameTypeInterPan
} eNwkFcfFrameType;

typedef enum {
	nwkProtocolVersion = 0
} eNwkFcfProtocolVersion;

typedef enum {
	nwkDiscoveryRouteSupressRouteDiscovery = 0,
	nwkDiscoveryRouteEnableRouteDiscovery
} eNwkFcfDiscoveryRoute;

typedef enum {
	nwkMultiFlagUnicastOrBroadcast = 0,
	nwkMultiFlagMulticast
} eNwkFcfMultiFlag;

typedef enum {
	nwkSecurityNo = 0,
	nwkSecurityYes
} eNwkFcfSecurity;

typedef enum {
	nwkSourceRouteNotPresent = 0,
	nwkSourceRoutePresent
} eNwkFcfSourceRoute;

typedef enum {
	nwkDestIeeeNo = 0,
	nwkDestIeeeYes
} eNwkFcfDestIeee;

typedef enum {
	nwkSourceIeeeNo = 0,
	nwkSourceIeeeYes
} eNwkFcfSourceIeee;

typedef struct {
	eNwkFcfFrameType			nwkFrameType;
	eNwkFcfProtocolVersion		nwkProtocolVersion;
	eNwkFcfDiscoveryRoute		nwkDiscoveryRoute;
	eNwkFcfMultiFlag			nwkMultiFlag;
	eNwkFcfSecurity				nwkSecurity;
	eNwkFcfSourceRoute			nwkSourceRoute;
	eNwkFcfDestIeee				nwkDestIeee;
	eNwkFcfSourceIeee			nwkSourceIeee;
} tNwkFcf;

#define NWK_POSITION_AF_DEST_ADD	2
#define NWK_POSITION_AF_SOURCE_ADD	4

typedef struct {
	uint16_t					nwkDestAdd;
	uint16_t					nwkSourceAdd;
} tNwkAdd;

#define NWK_POSITION_RADIUS			6
#define NWK_POSITION_SEQUENCE_NUM	7

typedef struct {
	tNwkFcf						nwkFcf;
	tNwkAdd						nwkAdd;
	uint8_t						nwkRadius;
	uint8_t						nwkSequenceNumber;
} tNwkMhr;

 /**	
* @brief: NWK Command
  @param: Field dimensions in bits, sections in bytes
  *			
  *			
  */

#define NWK_POSITION_NSDU_COMMAND_IDENTIFIER		0

#define NWK_POSITION_NSDU_COMMAND_IDENTIFIER_ID		0
#define NWK_POSITION_NSDU_COMMAND_IDENTIFIER_ID_L	8

typedef enum {
	nwkCommandIdentifierRouteRequest = 1,
	nwkCommandIdentifierRouteReply,
	nwkCommandIdentifierNetworkStatus,
	nwkCommandIdentifierLeave,
	nwkCommandIdentifierRouteRecord,
	nwkCommandIdentifierRejoinRequest,
	nwkCommandIdentifierRejoinResponse,
	nwkCommandIdentifierLinkStatus,
	nwkCommandIdentifierNetworkReport,
	nwkCommandIdentifierNetworkUpdate,
} eNwkFrameTypeCommandIdentifier;

// Command Route Request

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT				1

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_MANYTOONE	3
#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_MANYTOONE_L	2

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_DESTIEEE		5
#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_DESTIEEE_L	1

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_MULTICAST	6
#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_OPT_MULTICAST_L	1

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_ID				2

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_DESTADD			3

#define NWK_POSITION_NSDU_COMMAND_ROUTEREQ_PATHCOST			5

typedef struct {
	uint8_t											manyToOne;
	bool											destIeee;
	bool											multicast;
} tNwkFrameTypeCommandRouteRequestCommandOptions;

typedef struct {
	tNwkFrameTypeCommandRouteRequestCommandOptions	commandOptions;
	uint8_t											routeRequestId;
	uint16_t										destAdd;
	uint8_t											pathCost;
} tNwkFrameTypeCommandRouteRequest;

// Command Route Reply

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT				1

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_ORIGIEEE		4
#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_ORIGIEEE_L	1

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_RESPIEEE		5
#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_RESPIEEE_L	1

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_MULTICAST	6
#define NWK_POSITION_NSDU_COMMAND_ROUTERES_OPT_MULTICAST_L	1

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_ID				2

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_ORIGADD			3

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_RESPADD			5

#define NWK_POSITION_NSDU_COMMAND_ROUTERES_PATHCOST			7

typedef struct {
	bool											originatorIeee;
	bool											responderIeee;
	bool											multicast;
} tNwkFrameTypeCommandRouteResponseCommandOptions;

typedef struct {
	tNwkFrameTypeCommandRouteResponseCommandOptions	commandOptions;
	uint8_t											routeRequestId;
	uint16_t										originatorAdd;
	uint16_t										reponderAdd;
	uint8_t											pathCost;
} tNwkFrameTypeCommandRouteResponse;

// Command Network Status

typedef struct {

} tNwkFrameTypeCommandNetworkStatus;

// Command Leave

typedef struct {

} tNwkFrameTypeCommandLeave;

// Command Route Record

#define NWK_POSITION_NSDU_COMMAND_ROUTEREC_RELAYCOUNT		1

#define NWK_POSITION_NSDU_COMMAND_ROUTEREC_RELAYLIST		2

typedef struct {
	uint8_t											relayCount;
	uint16_t										relayList[10];
} tNwkFrameTypeCommandRouteRecord;

// Command Rejoin Request

typedef struct {

} tNwkFrameTypeCommandRejoinRequest;

// Command Rejoin Response

typedef struct {

} tNwkFrameTypeCommandRejoinResponse;

// Command Link Status

typedef struct {

} tNwkFrameTypeCommandLinkStatus;

// Command Network Report

typedef struct {

} tNwkFrameTypeCommandNetworkReport;

// Command Network Update

typedef struct {

} tNwkFrameTypeCommandNetworkUpdate;


#endif /* NWKLAYER_H_ */