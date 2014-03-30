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
//
typedef struct _LongByte
{
	union
	{
		uint8_t bval[4];
		uint32_t lval;
	};
} ATTR_PACKED LongByte;

//-----------------------------------------------------------------------------
typedef enum
{
	eUndefined,
	eMilliseconds,
	ePCMSamples,
} LockDelayUnits;

//-----------------------------------------------------------------------------
// This requires some abtraction too
typedef void* xQueueHandle;
extern xQueueHandle logQueueHandle;
extern xQueueHandle usbQueueHandle;
//-----------------------------------------------------------------------------
typedef struct _DbgMessage
{
	const char* file;
	int line;
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
typedef struct _USBMessage
{
	const char* file;
	int line;
	USB_Request_Header_t request;
	
} USBMessage;

//-----------------------------------------------------------------------------
//
extern int GetConfigStructSize(void);
extern void* GetConfigStruct(void);

#endif

