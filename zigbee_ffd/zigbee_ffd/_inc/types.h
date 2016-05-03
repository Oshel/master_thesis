/*
 * types.h
 *
 * Created: 2016-04-15 18:31:11
 *  Author: Maciek
 */ 


#ifndef TYPES_H_
#define TYPES_H_

#define bool		uint8_t
#define true		1
#define false		0

typedef enum {
	primitiveNone					= 0,
	primitiveRequest				= 1,
	primitiveIndication				= 2,
	primitiveResponse				= 3,
	primitiveConfirm				= 4
} isoPrimitive;

#endif /* TYPES_H_ */