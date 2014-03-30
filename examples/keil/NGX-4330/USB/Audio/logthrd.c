/*



*/

#include "AudioInput.h"
#include "apptypes.h"

#ifndef INC_FREERTOS_H
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#endif

const int bufferSize = 256;
static char szBuffer[bufferSize];
static char iBuffer[36];
//-----------------------------------------------------------------------------
const char* szRequests[] =
{
	"GetStatus 0",
	"ClearFeature (1)",
	"Reserved (2)",
	"SetFeature (3)",
	"Reserved (4)",
	"SetAddress (5)",
	"GetDescriptor (6)",
	"Reserved (7)",
	"GetConfiguration (8)",
	"SetConfiguration (9)",
};

// mask is DRD_MASK
const char* szDirection[] =
{
	"Unknown direction type",
	"Device to Host (1 << 7)",
	"Host to Device (0 << 7)"
};

// mask is DRT_MASK
const char* szReqType[] =
{
	"Unknown request type",
	"Standard (0x00)",
	"Class (0x20)",
	"Vendor (0x40)"
};

// mask is DRR_MASK
const char* szRecipient[] =
{
	"Unknown recipient",
	"Device (0x00)",
	"Interface (0x01)",
	"Endpoint (0x02)"
};

//-----------------------------------------------------------------------------
char* itoa(int value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	}
	while ( value );
	// Apply negative sign
	if (tmp_value < 0)
	{
		*ptr++ = '-';
	}
	*ptr-- = '\0';
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
//-----------------------------------------------------------------------------
const char* Decode_Setup(USB_Request_Header_t* USB_ControlRequest, char* buffer)
{
	WordByte wb;
	int index = 0;
	wb.wval = USB_ControlRequest->wIndex;
	const char* pszRecipient = 0;
	const char* pszDirection = 0;
	const char* pszType = 0;
	const char* pszRequest = szRequests[USB_ControlRequest->bRequest];

	index = (USB_ControlRequest->bmRequestType & DRD_MASK);
	pszDirection = szDirection[0];
	if (index == DRD_IN)
	{
		pszDirection = szDirection[1];
	}
	else if (index == DRD_OUT)
	{
		pszDirection = szDirection[2];
	}
	index = (USB_ControlRequest->bmRequestType & DRT_MASK);
	pszType = szReqType[0];
	if (index == DRT_STD)
	{
		pszType = szReqType[1];
	}
	else if (index == DRT_CLASS)
	{
		pszType = szReqType[2];
	}
	else if (index == DRT_VENDOR)
	{
		pszType = szReqType[3];
	}
	index = (USB_ControlRequest->bmRequestType & DRR_MASK);
	pszRecipient = szRecipient[0];
	if (index == DRR_DEVICE)
	{
		pszRecipient = szRecipient[1];
	}
	else if (index == DRR_INTERFACE)
	{
		pszRecipient = szRecipient[2];
	}
	else if (index == DRR_ENDPOINT)
	{
		pszRecipient = szRecipient[3];
	}
	// 
	if ((USB_ControlRequest->bmRequestType & DRR_MASK) == DRR_INTERFACE)	       
	{
		// 5.2.2 PP91 UAC2
		// interface is low byte of wIndex, entity is high byte
		sprintf(buffer, "Typ:0b%s Req:0x%X Typ:0x%X %s %s %s %s Val:0x%X Ndx:0x%X Len:0x%X IF:%d Entity:%d",
			itoa(USB_ControlRequest->bmRequestType, iBuffer, 2),
			USB_ControlRequest->bRequest,
			USB_ControlRequest->bmRequestType,
			pszRequest, pszDirection, pszType, pszRecipient,
			USB_ControlRequest->wValue,
			USB_ControlRequest->wIndex,
			USB_ControlRequest->wLength,
			wb.bval.lobyte,
			wb.bval.hibyte
			);
	}
	else
	{
		sprintf(buffer, "Typ:0b%s Req:0x%X Typ:0x%X %s %s %s %s Val:0x%X Ndx:0x%X Len:0x%X",
			itoa(USB_ControlRequest->bmRequestType, iBuffer, 2),
			USB_ControlRequest->bRequest,
			USB_ControlRequest->bmRequestType,
			pszRequest, pszDirection, pszType, pszRecipient,
			USB_ControlRequest->wValue,
			USB_ControlRequest->wIndex,
			USB_ControlRequest->wLength
			);
	}
	//
	return buffer;
}

//-----------------------------------------------------------------------------
// UART thread. Check queue and see if we got any data from ISR stuff
// If so format it and push out onto serial port @115K
void UARTTask(void* pvParameters)
{
	int tickCnt = 0;
	//
	DEBUGOUT("Config size %d 0x%X\n", GetConfigStructSize(), REQDIR_HOSTTODEVICE | REQREC_ENDPOINT);
	for (;;)
	{
		if (uxQueueMessagesWaiting(logQueueHandle))
		{
			dbg_message dbm;
			while (xQueueReceive(logQueueHandle, &dbm, 0))
			{
				if (dbm.file)
				{
					DEBUGOUT("%s(%d) [%d] %s 0x%X 0x%X 0x%X 0x%X 0x%X\r\n", 
						dbm.file,
						dbm.line,
						tickCnt, (dbm.psz ? dbm.psz : "<null>"), dbm.v1, dbm.v2, dbm.v3, dbm.v4, dbm.v5);
				}
				else
				{
					DEBUGOUT("[%d] %s 0x%X 0x%X 0x%X 0x%X 0x%X\r\n", tickCnt, (dbm.psz ? dbm.psz : "<null>"), dbm.v1, dbm.v2, dbm.v3, dbm.v4, dbm.v5);
				}
			}
		}
		if (uxQueueMessagesWaiting(usbQueueHandle))
		{
			USBMessage usbm;
			while (xQueueReceive(usbQueueHandle, &usbm, 0))
			{
				// decode the message. agggg.
				const char* p = Decode_Setup(&usbm.request, szBuffer);
				// dump
				DEBUGOUT("%s(%d) [%d] %s\r\n",(usbm.file ? usbm.file : "<null>"),usbm.line,tickCnt,p);
				//DEBUGOUT("[%d] %s\r\n", tickCnt, p);
			}
		}
		tickCnt++;
		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}

