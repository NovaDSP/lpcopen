/*
 * @brief USB device standard request management
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * Copyright(C) Dean Camera, 2011, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */


#define  __INCLUDE_FROM_USB_DRIVER
#include "USBMode.h"

#if defined(USB_CAN_BE_DEVICE)

#define  __INCLUDE_FROM_DEVICESTDREQ_C
#include "DeviceStandardReq.h"

uint8_t USB_Device_ConfigurationNumber;

#if !defined(NO_DEVICE_SELF_POWER)
bool    USB_Device_CurrentlySelfPowered;
#endif

#if !defined(NO_DEVICE_REMOTE_WAKEUP)
bool    USB_Device_RemoteWakeupEnabled;
#endif

#include <logger.h>

const char* psz[] =
{
/*0*/ "REQ_GetStatus",
		"REQ_ClearFeature",
/*2*/ "REQ_SetFeature",
	"REQ_SetAddress",
/*4*/ "REQ_GetDescriptor",
		"REQ_GetConfiguration",
/*6*/ "REQ_SetConfiguration",
		"Unknown EP0 request (request,type,ep#)",
/*8*/ "Unknown bRequest",
	"Raw USB packet (Req,Typ,Ndx,MSB,LSB)",
/*10*/	"Stalling EP (Req,Typ,Ndx,MSB,LSB)",
	"Request: Get_Interface",
/*11*/	"Request: Set_Interface",
};

//
const char* requests[] = {
	"REQ_GetStatus 0",
	"REQ_ClearFeature 1",
	"Unknown 2",
	"REQ_SetFeature 3",
	"Unknown 4",
	"REQ_SetAddress 5",
	"REQ_GetDescriptor 6",
	"REQ_SetDescriptor 7",
	"REQ_GetConfiguration 8",
	"REQ_SetConfiguration 9",
	"REQ_GetInterface 10",
	"REQ_SetInterface 11",
	"REQ_SynchFrame 12",
	"Unknown 13",
	"Unknown 14"
};

const char* stall = "stalled previous request!";

//
void USB_Device_ProcessControlRequest(uint8_t corenum)
{
//	USB_ControlRequest.bmRequestType = Endpoint_Read_8();
//	USB_ControlRequest.bRequest      = Endpoint_Read_8();
//	USB_ControlRequest.wValue        = Endpoint_Read_16_LE();
//	USB_ControlRequest.wIndex        = Endpoint_Read_16_LE();
//	USB_ControlRequest.wLength       = Endpoint_Read_16_LE();
	WordByte wb;
	uint8_t type = 0;
	uint8_t request = 0;
	uint16_t wIndex = 0;
	uint8_t wValue_lsb = 0;
	uint8_t wValue_msb = 0;
	
	Endpoint_GetSetupPackage(corenum, (uint8_t*) &USB_ControlRequest);

	// call into application code
	EVENT_USB_Device_ControlRequest();

	if (Endpoint_IsSETUPReceived(corenum))
	{
		// Bit 7: Request direction (0=Host to device – Out, 1=Device to host – In).
		// Bits 5-6: Request type (0=standard, 1=class, 2=vendor, 3=reserved).
		// Bits 0-4: Recipient (0=device, 1=interface, 2=endpoint,3=other).
		type = USB_ControlRequest.bmRequestType;
		
		// 
		request = USB_ControlRequest.bRequest;
		wIndex = USB_ControlRequest.wIndex;
		wb.wval = USB_ControlRequest.wValue;
		wValue_lsb = wb.bval.lobyte;
		wValue_msb = wb.bval.hibyte;

		switch (USB_ControlRequest.bRequest)
		{
			case REQ_GetStatus:
				if ((type == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)) ||
					(type == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_ENDPOINT)))
				{
//					LogS(psz[0],eREQ_GetStatus,type,corenum);
					USB_Device_GetStatus(corenum);
				}
				break;
			case REQ_ClearFeature:
				if ((type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE)) ||
					(type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_ENDPOINT))
					)
				{
//					LogS(psz[1],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_ClearSetFeature(corenum);
					//
					LogUSB(__FILE__,__LINE__,&USB_ControlRequest,sizeof(USB_ControlRequest));
				}
				// OSX sends this ... we did not handle it
				else if (type == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
				{
					LogUSB(__FILE__,__LINE__,&USB_ControlRequest,sizeof(USB_ControlRequest));
				}
				else if (type == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
				{
					LogUSB(__FILE__,__LINE__,&USB_ControlRequest,sizeof(USB_ControlRequest));
				}
				else
				{
					LogUSB(__FILE__,__LINE__,&USB_ControlRequest,sizeof(USB_ControlRequest));
				}
				break;
			case REQ_SetFeature:
				if ((type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE)) ||
					(type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_ENDPOINT)))
				{
//					LogS(psz[2],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_ClearSetFeature(corenum);
				}
				//
				LogUSB(__FILE__,__LINE__,&USB_ControlRequest,sizeof(USB_ControlRequest));
				// 
				break;
			case REQ_SetAddress:
				if (type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE))
				{
//					LogS(psz[3],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_SetAddress(corenum);
				}
				break;
			case REQ_GetDescriptor:
				if ((type == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)) ||
					(type == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_INTERFACE)))
				{
//					LogS(psz[4],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_GetDescriptor(corenum);
				}
				break;
			case REQ_GetConfiguration:
				if (type == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE))
				{
//					LogS(psz[5],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_GetConfiguration(corenum);
				}
				break;
			case REQ_SetConfiguration:
				if (type == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE))
				{
//					LogS(psz[6],USB_ControlRequest.bRequest,type,corenum);
					USB_Device_SetConfiguration(corenum);
				}
				break;
			default:
				;
		}
	}
	
	//
	if (Endpoint_IsSETUPReceived(corenum))
	{
		LogReq(requests[request],
			request,
			type,
			wIndex,
			wValue_msb,
			wValue_lsb
			);
		Log3(stall,0,0,0);
		Endpoint_ClearSETUP(corenum);
		Endpoint_StallTransaction(corenum);
	}
}

static void USB_Device_SetAddress(uint8_t corenum)
{
	uint8_t    DeviceAddress    = (USB_ControlRequest.wValue & 0x7F);
	uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
	GlobalInterruptDisable();
				
	Endpoint_ClearSETUP(corenum);

	Endpoint_ClearStatusStage(corenum);

	while (!(Endpoint_IsINReady(corenum)));

	USB_Device_SetDeviceAddress(corenum, DeviceAddress);
	USB_DeviceState[corenum] = (DeviceAddress) ? DEVICE_STATE_Addressed : DEVICE_STATE_Default;
	
	SetGlobalInterruptMask(CurrentGlobalInt);

	// JME
	Log(eREQ_SetAddress,corenum,DeviceAddress,0);

}

static void USB_Device_SetConfiguration(uint8_t corenum)
{
	#if defined(FIXED_NUM_CONFIGURATIONS)
	if ((uint8_t)USB_ControlRequest.wValue > FIXED_NUM_CONFIGURATIONS)
	  return;
	#else
	USB_Descriptor_Device_t* DevDescriptorPtr;

	#if defined(ARCH_HAS_MULTI_ADDRESS_SPACE)
		#if defined(USE_FLASH_DESCRIPTORS)
			#define MemoryAddressSpace  MEMSPACE_FLASH
		#elif defined(USE_EEPROM_DESCRIPTORS)
			#define MemoryAddressSpace  MEMSPACE_EEPROM
		#elif defined(USE_RAM_DESCRIPTORS)
			#define MemoryAddressSpace  MEMSPACE_SRAM
		#else
			uint8_t MemoryAddressSpace;
		#endif
	#endif
	
	if (CALLBACK_USB_GetDescriptor(corenum, (DTYPE_Device << 8), 0, (void*)&DevDescriptorPtr
	#if defined(ARCH_HAS_MULTI_ADDRESS_SPACE) && \
	    !(defined(USE_FLASH_DESCRIPTORS) || defined(USE_EEPROM_DESCRIPTORS) || defined(USE_RAM_DESCRIPTORS))
	                               , &MemoryAddressSpace
	#endif
	                               ) == NO_DESCRIPTOR)
	{
		return;
	}

	#if defined(ARCH_HAS_MULTI_ADDRESS_SPACE)
	if (MemoryAddressSpace == MEMSPACE_FLASH)
	{
		if (((uint8_t)USB_ControlRequest.wValue > pgm_read_byte(&DevDescriptorPtr->NumberOfConfigurations)))
		  return;
	}
	else if (MemoryAddressSpace == MEMSPACE_EEPROM)
	{
		if (((uint8_t)USB_ControlRequest.wValue > eeprom_read_byte(&DevDescriptorPtr->NumberOfConfigurations)))
		  return;
	}
	else
	{
		if ((uint8_t)USB_ControlRequest.wValue > DevDescriptorPtr->NumberOfConfigurations)
		  return;
	}
	#else
	if ((uint8_t)USB_ControlRequest.wValue > DevDescriptorPtr->NumberOfConfigurations)
	  return;	
	#endif
	#endif

	Endpoint_ClearSETUP(corenum);

	USB_Device_ConfigurationNumber = (uint8_t)USB_ControlRequest.wValue;

	Endpoint_ClearStatusStage(corenum);

	if (USB_Device_ConfigurationNumber)
	  USB_DeviceState[corenum] = DEVICE_STATE_Configured;
	else
	  USB_DeviceState[corenum] = (USB_Device_IsAddressSet()) ? DEVICE_STATE_Configured : DEVICE_STATE_Powered;

	// call into application code
	EVENT_USB_Device_ConfigurationChanged();
}

static void USB_Device_GetConfiguration(uint8_t corenum)
{
	Endpoint_ClearSETUP(corenum);

	Endpoint_Write_8(corenum, USB_Device_ConfigurationNumber);
	Endpoint_ClearIN(corenum);

	Endpoint_ClearStatusStage(corenum);
}

#if !defined(NO_INTERNAL_SERIAL) && (USE_INTERNAL_SERIAL != NO_DESCRIPTOR)
static void USB_Device_GetInternalSerialDescriptor(uint8_t corenum)
{
	struct
	{
		USB_Descriptor_Header_t Header;
		uint16_t                UnicodeString[INTERNAL_SERIAL_LENGTH_BITS / 4];
	} SignatureDescriptor;

	SignatureDescriptor.Header.Type = DTYPE_String;
	SignatureDescriptor.Header.Size = USB_STRING_LEN(INTERNAL_SERIAL_LENGTH_BITS / 4);
	
	USB_Device_GetSerialString(SignatureDescriptor.UnicodeString);

	Endpoint_ClearSETUP(corenum);

	Endpoint_Write_Control_Stream_LE(corenum, &SignatureDescriptor, sizeof(SignatureDescriptor));
	Endpoint_ClearOUT(corenum);
}
#endif

static void USB_Device_GetDescriptor(uint8_t corenum)
{
	const void* DescriptorPointer;
	uint16_t    DescriptorSize;

	#if defined(ARCH_HAS_MULTI_ADDRESS_SPACE) && \
	    !(defined(USE_FLASH_DESCRIPTORS) || defined(USE_EEPROM_DESCRIPTORS) || defined(USE_RAM_DESCRIPTORS))
	uint8_t DescriptorAddressSpace;
	#endif

	#if !defined(NO_INTERNAL_SERIAL) && (USE_INTERNAL_SERIAL != NO_DESCRIPTOR)
	if (USB_ControlRequest.wValue == ((DTYPE_String << 8) | USE_INTERNAL_SERIAL))
	{
		USB_Device_GetInternalSerialDescriptor(corenum);
		return;
	}
	#endif

	if ((DescriptorSize = CALLBACK_USB_GetDescriptor(corenum, USB_ControlRequest.wValue, USB_ControlRequest.wIndex,
	                                                 &DescriptorPointer
	#if defined(ARCH_HAS_MULTI_ADDRESS_SPACE) && \
	    !(defined(USE_FLASH_DESCRIPTORS) || defined(USE_EEPROM_DESCRIPTORS) || defined(USE_RAM_DESCRIPTORS))
	                                                 , &DescriptorAddressSpace
	#endif
													 )) == NO_DESCRIPTOR)
	{
		return;
	}

	Endpoint_ClearSETUP(corenum);

	#if defined(USE_RAM_DESCRIPTORS) || !defined(ARCH_HAS_MULTI_ADDRESS_SPACE)
	Endpoint_Write_Control_Stream_LE(corenum, DescriptorPointer, DescriptorSize);
	#elif defined(USE_EEPROM_DESCRIPTORS)
	Endpoint_Write_Control_EStream_LE(DescriptorPointer, DescriptorSize);
	#elif defined(USE_FLASH_DESCRIPTORS)
	Endpoint_Write_Control_PStream_LE(DescriptorPointer, DescriptorSize);
	#else
	if (DescriptorAddressSpace == MEMSPACE_FLASH)
	  Endpoint_Write_Control_PStream_LE(DescriptorPointer, DescriptorSize);
	else if (DescriptorAddressSpace == MEMSPACE_EEPROM)
	  Endpoint_Write_Control_EStream_LE(DescriptorPointer, DescriptorSize);
	else
	  Endpoint_Write_Control_Stream_LE(corenum, DescriptorPointer, DescriptorSize);
	#endif

	Endpoint_ClearOUT(corenum);
}

static void USB_Device_GetStatus(uint8_t corenum)
{
	uint8_t CurrentStatus = 0;

	switch (USB_ControlRequest.bmRequestType)
	{
		#if !defined(NO_DEVICE_SELF_POWER) || !defined(NO_DEVICE_REMOTE_WAKEUP)
		case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE):
			#if !defined(NO_DEVICE_SELF_POWER)
			if (USB_Device_CurrentlySelfPowered)
			  CurrentStatus |= FEATURE_SELFPOWERED_ENABLED;
			#endif

			#if !defined(NO_DEVICE_REMOTE_WAKEUP)
			if (USB_Device_RemoteWakeupEnabled)
			  CurrentStatus |= FEATURE_REMOTE_WAKEUP_ENABLED;
			#endif
			break;
		#endif
		#if !defined(CONTROL_ONLY_DEVICE)
		case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_ENDPOINT):
			Endpoint_SelectEndpoint(corenum, (uint8_t)USB_ControlRequest.wIndex & ENDPOINT_EPNUM_MASK);

			CurrentStatus = Endpoint_IsStalled(corenum);

			Endpoint_SelectEndpoint(corenum, ENDPOINT_CONTROLEP);

			break;
		#endif
		default:
			return;
	}

	Endpoint_ClearSETUP(corenum);

	Endpoint_Write_16_LE(corenum, CurrentStatus);
	Endpoint_ClearIN(corenum);

	Endpoint_ClearStatusStage(corenum);
}

const char* features[] = {
"REQREC_DEVICE    (0 << 0)",
"REQREC_INTERFACE (1 << 0)",
"REQREC_ENDPOINT  (2 << 0)",
"REQREC_OTHER     (3 << 0)",
};

const char* not_handled = "not_handled";

static void USB_Device_ClearSetFeature(uint8_t corenum)
{
	uint8_t val = (USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT);
	//
	Log3(features[val],val,USB_ControlRequest.bmRequestType,0);
	//switch (USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT)
	switch (val)
	{
		#if !defined(NO_DEVICE_REMOTE_WAKEUP)
		case REQREC_DEVICE:
			if ((uint8_t)USB_ControlRequest.wValue == FEATURE_SEL_DeviceRemoteWakeup)
			{
			  USB_Device_RemoteWakeupEnabled = (USB_ControlRequest.bRequest == REQ_SetFeature);
			}
			else
			  return;

			break;
		#endif
		#if !defined(CONTROL_ONLY_DEVICE)
		case REQREC_ENDPOINT:
			if ((uint8_t)USB_ControlRequest.wValue == FEATURE_SEL_EndpointHalt)
			{
				uint8_t EndpointIndex = ((uint8_t)USB_ControlRequest.wIndex & ENDPOINT_EPNUM_MASK);

				if (EndpointIndex == ENDPOINT_CONTROLEP)
				  return;

				Endpoint_SelectEndpoint(corenum, EndpointIndex);

				if (Endpoint_IsEnabled())
				{
					if (USB_ControlRequest.bRequest == REQ_SetFeature)
					{
						Endpoint_StallTransaction(corenum);
					}
					else
					{
						Endpoint_ClearStall(corenum);
						Endpoint_ResetEndpoint(EndpointIndex);
						Endpoint_ResetDataToggle(corenum);
					}
				}
			}

			break;
		#endif
		default:
			Log3(not_handled,val,USB_ControlRequest.bmRequestType,0);
			return;
	}

	Endpoint_SelectEndpoint(corenum, ENDPOINT_CONTROLEP);

	Endpoint_ClearSETUP(corenum);

	Endpoint_ClearStatusStage(corenum);
}

#endif

