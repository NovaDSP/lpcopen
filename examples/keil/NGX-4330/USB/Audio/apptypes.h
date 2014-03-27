/*



*/


#ifndef APPTYPES_H
#define APPTYPES_H

#include <stdint.h>
#include "config.h"
#include "appenum.h"

//-----------------------------------------------------------------------------
// lets do byte twiddling correctly ...
typedef struct _ByteByte
{
	uint8_t lobyte;
	uint8_t hibyte;
} ATTR_PACKED ByteByte;

//-----------------------------------------------------------------------------
//
typedef struct _WordByte
{
	union
	{
		ByteByte bval;
		uint16_t wval;
	};
} ATTR_PACKED WordByte;

//-----------------------------------------------------------------------------
typedef enum
{
	eUndefined,
	eMilliseconds,
	ePCMSamples,
} LockDelayUnits;

typedef void* xQueueHandle;
extern xQueueHandle xqh;

//-----------------------------------------------------------------------------
typedef struct _DbgMessage
{
	// pointer to const string
	const char* psz;
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;
	uint32_t v4;
	uint32_t v5;
} DbgMessage;

typedef DbgMessage dbg_message;

//-----------------------------------------------------------------------------
//
extern int GetConfigStructSize(void);
extern void* GetConfigStruct(void);

#endif

