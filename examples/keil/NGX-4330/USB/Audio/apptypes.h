/*



*/


#ifndef APPTYPES_H
#define APPTYPES_H

#include <stdint.h>

#ifndef INC_FREERTOS_H
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#endif

//-----------------------------------------------------------------------------
typedef enum _state_index
{
	eTarget,
	eEnabled,
	eDisabled,
	eGetSampleRate,
	eGetSampleRateNoData,
	eSetSampleRate,
	eSetSampleRateNoData,
	eGetSetInterfaceProperty,
	eUnknownEndpointProperty,
	eOtherEndpointProperty,
	eConfigureEndpoints,
	eGetDescriptor,
	MaxStates
} state_index;

typedef void* xQueueHandle;

extern const char* states[MaxStates];
extern xQueueHandle xqh;

//-----------------------------------------------------------------------------
typedef struct _DbgMessage
{
	// pointer to const string
	const char* psz;
	uint32_t value;
	uint32_t flags;
	uint32_t v2;
} DbgMessage;

typedef DbgMessage dbg_message;

//-----------------------------------------------------------------------------
//
extern int GetConfigStructSize(void);
extern void* GetConfigStruct(void);

#endif

