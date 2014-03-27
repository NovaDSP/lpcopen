/*


*/

#ifndef LOGGER_H
#define LOGGER_H

//-----------------------------------------------------------------------------
#ifndef APPTYPES_H
#include "apptypes.h"
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
	eREQ_GetStatus,
	eREQ_ClearFeature,
	eREQ_SetFeature,
	eREQ_SetAddress,
	eREQ_GetDescriptor,
	eREQ_GetConfiguration,
	eREQ_SetConfiguration,
	eREQ_Unknown,
	eGet_DeviceQualifier_Descriptor,
	eGet_Unknown_Descriptor,
	MaxStates
} state_index;

//-----------------------------------------------------------------------------
// logging strings
extern const char* states[MaxStates];

//-----------------------------------------------------------------------------
// yet more macros

// Device Request Direction (bmRequestType bit7)
#define DRD_MASK				0x80		// Mask for device request direction
#define DRD_OUT					0x00		// OUT: host to device
#define DRD_IN					0x80		// IN:	device to host

// Device Request Type (bmRequestType bit6-5)
#define DRT_MASK				0x60		// Mask for device request type
#define DRT_STD					0x00		// Standard device request
#define DRT_CLASS				0x20		// Class specific request
#define DRT_VENDOR				0x40		// Vendor specific request

// Device Request Recipient (bmRequestType bit4-0)
#define DRR_MASK				0x1F		// Mask for device request recipient
#define DRR_DEVICE				0x00		// Device
#define DRR_INTERFACE			0x01		// Interface
#define DRR_ENDPOINT			0x02		// Endpoint

// Define bmRequestType bitmaps
#define OUT_DEVICE			(DRD_OUT | DRT_STD | DRR_DEVICE)		// Request made to device,
#define IN_DEVICE			(DRD_IN  | DRT_STD | DRR_DEVICE)		// Request made to device,
#define OUT_INTERFACE		(DRD_OUT | DRT_STD | DRR_INTERFACE)		// Request made to interface,
#define IN_INTERFACE		(DRD_IN  | DRT_STD | DRR_INTERFACE)		// Request made to interface,
#define OUT_ENDPOINT		(DRD_OUT | DRT_STD | DRR_ENDPOINT)		// Request made to endpoint,
#define IN_ENDPOINT			(DRD_IN  | DRT_STD | DRR_ENDPOINT)		// Request made to endpoint,

#define OUT_CL_INTERFACE	(DRD_OUT | DRT_CLASS | DRR_INTERFACE)	// Request made to class interface,
#define IN_CL_INTERFACE		(DRD_IN  | DRT_CLASS | DRR_INTERFACE)	// Request made to class interface,

//-----------------------------------------------------------------------------
extern void Log(int index,int v1,int v2, int v3);
extern void LogReq(const char* psz,int v1,int v2, int v3,int v4,int v5);
extern void Log5(const char* psz,int v1,int v2, int v3,int v4,int v5);
extern void Log3(const char* psz,int v1,int v2, int v3);

#endif
