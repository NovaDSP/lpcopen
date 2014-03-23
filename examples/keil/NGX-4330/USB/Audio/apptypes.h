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

#include "config.h"
#include "appenum.h"

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

