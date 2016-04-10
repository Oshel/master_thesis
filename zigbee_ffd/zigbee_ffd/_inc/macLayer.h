/*
 * macLayer.h
 *
 * Created: 2016-04-10 17:06:53
 *  Author: Maciek
 */ 


#ifndef MACLAYER_H_
#define MACLAYER_H_

/**	
* @brief:	MAC - FCF- Frame Control Field typedefs
  *			
  *			
  */

#define MAC_POSITION_FCF_FRAME_TYPE				0
#define MAC_POSITION_FCF_SECURITY				3
#define MAC_POSITION_FCF_FRAME_PENDING			4
#define MAC_POSITION_FCF_ACK_REQUEST			5
#define MAC_POSITION_FCF_INTRA_PAN				6
#define MAC_POSITION_FCF_DEST_ADD_MODE			10
#define MAC_POSITION_FCF_FRAME_VERSION			12
#define MAC_POSITION_FCF_SRC_ADD_MODE			14

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
	macDestAddModePanAndAddFielsNotPresent		= 0,
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
	macSrcAddModePanAndAddFielsNotPresent		= 0,
	macSrcAddModeReserved						= 1,
	macSrcAddModeAdd16Bit						= 2,
	macSrcAddModeAdd64Bit						= 3
} eMacSrcAddMode;

#endif /* MACLAYER_H_ */