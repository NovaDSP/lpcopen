/*
 * @brief USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *        computer-readable structures which the host requests upon device enumeration, to determine
 *        the device's capabilities and functions
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


#include "Descriptors.h"
#include "apptypes.h"
#include "appenum.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
USB_Descriptor_Device_t DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

// full speed. Drop back to flagging as USB 1.1 (?)	
#if USE_FULL_SPEED == 1	
	.USBSpecification       = VERSION_BCD(01.10),
#else	
	.USBSpecification       = VERSION_BCD(02.00),
#endif	

#ifdef _USE_AC2
	.Class                  = USB_CSCP_IADDeviceClass,
	.SubClass               = USB_CSCP_IADDeviceSubclass,
	.Protocol               = USB_CSCP_IADDeviceProtocol,
#else
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,
#endif
	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	// this belongs to OMG ...
	.VendorID               = 0xFACE,
	
	// persuade Windows to re-install drivers when we change configuration
#ifdef _USE_AC2
	.ProductID              = 1909,
#elif BYTES_PER_SAMPLE == 2 && USE_FULL_SPEED == 1
	.ProductID              = 1601 + CHANNEL_COUNT,
#elif BYTES_PER_SAMPLE == 2 && USE_FULL_SPEED == 0
	.ProductID              = 1602 + CHANNEL_COUNT,
#elif BYTES_PER_SAMPLE == 3	 && USE_FULL_SPEED == 1	
	.ProductID              = 2401 + CHANNEL_COUNT,
#elif BYTES_PER_SAMPLE == 3	 && USE_FULL_SPEED == 0
	.ProductID              = 2402 + CHANNEL_COUNT,
#else
	#error unsupported sample size
#endif	
	.ReleaseNumber          = VERSION_BCD(02.18),

	.ManufacturerStrIndex   = eManufacturer,
	.ProductStrIndex        = eProduct,
	.SerialNumStrIndex      = eSerial,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
USB_Descriptor_Configuration_t ConfigurationDescriptor =
{
	.Config = 
	{
		.Header                   = 
		{
		.Size = sizeof(USB_Descriptor_Configuration_Header_t), 
		.Type = DTYPE_Configuration
		},
		// termination byte not included in size
		.TotalConfigurationSize   = sizeof(USB_Descriptor_Configuration_t) - 1,
		.TotalInterfaces          = 2,
		.ConfigurationNumber      = 1,
		.ConfigurationStrIndex    = NO_DESCRIPTOR,
		.ConfigAttributes         = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),
		.MaxPowerConsumption      = USB_CONFIG_POWER_MA(200)
	},

	.Audio_ControlInterface = 
	{
		.Header = 
		{
		.Size = sizeof(USB_Descriptor_Interface_t), 
		.Type = DTYPE_Interface
		},
		.InterfaceNumber          = 0,
		.AlternateSetting         = 0,
		.TotalEndpoints           = 0,
		.Class                    = AUDIO_CSCP_AudioClass,
		.SubClass                 = AUDIO_CSCP_ControlSubclass,
		.Protocol                 = AUDIO_CSCP_ControlProtocol,
		.InterfaceStrIndex        = NO_DESCRIPTOR
	},

	/*.Audio_ControlInterface_SPC = */
	{
		/*.Header = */
		{
		/*.Size = */ sizeof(ConfigurationDescriptor.Audio_ControlInterface_SPC),
		/*.Type = */ DTYPE_CSInterface
		},
		.Subtype = AUDIO_DSUBTYPE_CSInterface_Header,
		.ACSpecification = VERSION_BCD(01.00),
		.TotalLength = (sizeof(USB_Audio_Descriptor_Interface_AC_t) +
						sizeof(USB_Audio_Descriptor_InputTerminal_t) +
						sizeof(USB_Audio_Descriptor_OutputTerminal_t)),
		.InCollection             = 1,
		.InterfaceNumber          = 1,
	},

	/*.Audio_InputTerminal = */
	{
		/*.Header = */
		{
		/* .Size = */ sizeof(ConfigurationDescriptor.Audio_InputTerminal),
		/* .Type = */ DTYPE_CSInterface
		},
		.Subtype                  = AUDIO_DSUBTYPE_CSInterface_InputTerminal,
		.TerminalID               = 0x01,
		.TerminalType             = DIGITAL_AUDIO_INTERFACE,
		/*.AssociatedOutputTerminal = */ 0x00,
		// define channel count
		.TotalChannels            = CHANNEL_COUNT,
		// this is set at runtime. determines if the device gives hints for spatial location
		.ChannelConfig            = 0,
		// JME
		.ChannelStrIndex          = NO_DESCRIPTOR,
		.TerminalStrIndex         = NO_DESCRIPTOR
	},


	.Audio_OutputTerminal = 
	{
		.Header                   = 
		{
		.Size = sizeof(USB_Audio_Descriptor_OutputTerminal_t), 
		.Type = DTYPE_CSInterface
		},
		.Subtype                  = AUDIO_DSUBTYPE_CSInterface_OutputTerminal,
		.TerminalID               = 0x02,
		.TerminalType             = AUDIO_TERMINAL_STREAMING,
		.AssociatedInputTerminal  = 0x00,
		.SourceID                 = 0x01,
		.TerminalStrIndex         = NO_DESCRIPTOR
	},

	// selected when device is told to stop streaming. isoch packets no longer sent
	.Audio_StreamInterface_Alt0 = 
	{
		.Header                   = 
		{
			.Size = sizeof(USB_Descriptor_Interface_t), 
			.Type = DTYPE_Interface
		},

		.InterfaceNumber          = 1,
		.AlternateSetting         = 0,
		.TotalEndpoints           = 0,
		.Class                    = AUDIO_CSCP_AudioClass,
		.SubClass                 = AUDIO_CSCP_AudioStreamingSubclass,
		.Protocol                 = AUDIO_CSCP_StreamingProtocol,
		.InterfaceStrIndex        = NO_DESCRIPTOR
	},

	// selected when device is told to start streaming. isoch packets required
	.Audio_StreamInterface_Alt1 = 
	{
		.Header = 
		{
		.Size = sizeof(USB_Descriptor_Interface_t), 
		.Type = DTYPE_Interface
		},

		.InterfaceNumber          = 1,
		.AlternateSetting         = 1,
		.TotalEndpoints           = 1,
		.Class                    = AUDIO_CSCP_AudioClass,
		.SubClass                 = AUDIO_CSCP_AudioStreamingSubclass,
		.Protocol                 = AUDIO_CSCP_StreamingProtocol,
		.InterfaceStrIndex        = NO_DESCRIPTOR
	},

	//
	.Audio_StreamInterface_SPC = 
	{
		.Header = 
		{
		.Size = sizeof(USB_Audio_Descriptor_Interface_AS_t), 
		.Type = DTYPE_CSInterface
		},
		.Subtype                  = AUDIO_DSUBTYPE_CSInterface_General,
		.TerminalLink             = 0x02,
		.FrameDelay               = 1,
		.AudioFormat              = 0x0001
	},

	.Audio_AudioFormat = 
	{
		// .Header = 
		{
			/* .Size = */ sizeof(USB_Audio_Descriptor_Format_t) + sizeof(USB_Audio_SampleFreq_t) * SUPPORTED_SAMPLE_RATES,
			/* .Type = */ DTYPE_CSInterface
		},
		/*.Subtype = */ AUDIO_DSUBTYPE_CSInterface_FormatType,
		/* .FormatType = */ FORMAT_TYPE_1,
		/* .Channels = */ CHANNEL_COUNT,
		// 2 bytes per subframe for 16bit, 3 for 24 bit
		/* .SubFrameSize = */ BYTES_PER_SAMPLE,
		// Bits per sample, i.e 3 * 8 for 24 bit audio
		/* .BitResolution = */ BYTES_PER_SAMPLE * 8,
		// how many sample rates do we support?
		/* .TotalDiscreteSampleRates = */ SUPPORTED_SAMPLE_RATES,
	},

	//.Audio_AudioFormatSampleRates = 
	{
//		JME integral sample rates only
//		No, keep these for testing control interface is working
//		AUDIO_SAMPLE_FREQ(8000),
//		AUDIO_SAMPLE_FREQ(11025),
//		AUDIO_SAMPLE_FREQ(22050),
//		AUDIO_SAMPLE_FREQ(44100),
		AUDIO_SAMPLE_FREQ(48000),
		// this is just to persuade Windows to display
		// an 'advanced' tab ...
		AUDIO_SAMPLE_FREQ(44100),	
//		AUDIO_SAMPLE_FREQ(22050),
//		AUDIO_SAMPLE_FREQ(11025),
//		AUDIO_SAMPLE_FREQ(8000),		
	},

	.Audio_StreamEndpoint = 
	{
		/*.Endpoint = */
		{
			/*.Header = */
			{
			/*.Size = */ sizeof(USB_Audio_Descriptor_StreamEndpoint_Std_t), 
			/*.Type = */ DTYPE_Endpoint
			},
			/*.EndpointAddress = */ (ENDPOINT_DIR_IN | AUDIO_STREAM_EPNUM),
			/* .Attributes     = */ (EP_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC | ENDPOINT_USAGE_DATA),
			/*.EndpointSize = */ EP_SIZE_BYTES,
			/*.PollingIntervalMS = */ 0x01
		},
		/*.Refresh            = */ 0,
		/*.SyncEndpointNumber = */ 0
	},

	.Audio_StreamEndpoint_SPC = 
	{
		.Header = 
		{
		.Size = sizeof(USB_Audio_Descriptor_StreamEndpoint_Spc_t), 
		.Type = DTYPE_CSEndpoint
		},
		.Subtype = AUDIO_DSUBTYPE_CSEndpoint_General,
		.Attributes = (AUDIO_EP_ACCEPTS_SMALL_PACKETS | AUDIO_EP_SAMPLE_FREQ_CONTROL),
		// USBAudio1.0 P62
		// The bLockDelayUnitsand wLockDelayfields are only applicable for synchronous and adaptive
		// endpoints. For asynchronous endpoints, the clock is generated internally in the audio function and is
		// completely independent. In this case, bLockDelayUnits and wLockDelay must be set to zero
		// Indicates the units used for the	wLockDelay field: 0: Undefined 1: Milliseconds	2: Decoded PCM samples 3..255: Reserved
		.LockDelayUnits = ePCMSamples,
		// table 4.21 in USB1.0. i.e. 2 samples for lock
		.LockDelay = 2
	},
	.Audio_Termination = 0x00
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
uint8_t LanguageString[] =
{
	USB_STRING_LEN(1),
	DTYPE_String,
	WBVAL(LANGUAGE_ID_ENG),
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t ManufacturerString[] =
{
	USB_STRING_LEN(7),
	DTYPE_String,
	WBVAL('C'),
	WBVAL('h'),
	WBVAL('o'),
	WBVAL('r'),
	WBVAL('d'),
	WBVAL('i'),
	WBVAL('a'),
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t ProductString[] =
{
#if (CHANNEL_COUNT >= 10)
	USB_STRING_LEN(15),
#else
	USB_STRING_LEN(14),
#endif	
	DTYPE_String,
	WBVAL('C'),
	WBVAL('h'),
	WBVAL('o'),
	WBVAL('r'),
	WBVAL('d'),
	WBVAL('i'),
	WBVAL('a'),
	WBVAL(' '),
#if (CHANNEL_COUNT == 1)
	WBVAL('1'),
#elif (CHANNEL_COUNT == 2)
	WBVAL('2'),
#elif (CHANNEL_COUNT == 4)	
	WBVAL('4'),
#elif (CHANNEL_COUNT == 6)	
	WBVAL('6'),
#elif (CHANNEL_COUNT == 8)	
	WBVAL('8'),
#elif (CHANNEL_COUNT == 10)	
	WBVAL('1'),
	WBVAL('0'),
#elif (CHANNEL_COUNT == 12)	
	WBVAL('1'),
	WBVAL('2'),
#elif (CHANNEL_COUNT == 24)	
	WBVAL('2'),
	WBVAL('4'),
#elif (CHANNEL_COUNT == 36)	
	WBVAL('3'),
	WBVAL('6'),
#elif (CHANNEL_COUNT == 48)	
	WBVAL('4'),
	WBVAL('8'),
#else	
	WBVAL('N'),
	WBVAL('N'),
#endif
	WBVAL('C'),
	WBVAL('H'),
	WBVAL(' '),
#if USE_FULL_SPEED == 1
	WBVAL('F'),
	WBVAL('S'),
#else	
	WBVAL('H'),
	WBVAL('S'),
#endif	
};

uint8_t ChannelStrings[] =
{
	USB_STRING_LEN(5),
	DTYPE_String,
	WBVAL('I'),
	WBVAL('P'),
	WBVAL(' '),
	WBVAL('0'),
	WBVAL('1'),
};

uint8_t AltMan[] =
{
	USB_STRING_LEN(7),
	DTYPE_String,
	WBVAL('C'),
	WBVAL('h'),
	WBVAL('o'),
	WBVAL('r'),
	WBVAL('d'),
	WBVAL('i'),
	WBVAL('a'),
};

uint8_t DescSerial[] =
{
#if USE_FULL_SPEED == 0
	USB_STRING_LEN(5),
	DTYPE_String,
	WBVAL('4'),
	WBVAL('8'),
	WBVAL('0'),
	WBVAL('.'),
	WBVAL('0'),
#else 
	USB_STRING_LEN(4),
	DTYPE_String,
	WBVAL('1'),
	WBVAL('2'),
	WBVAL('.'),
	WBVAL('0'),
#endif	
};

uint8_t DescAlt0[] =
{
	USB_STRING_LEN(4),
	DTYPE_String,
	WBVAL('A'),
	WBVAL('L'),
	WBVAL('T'),
	WBVAL('0'),
};

uint8_t DescAlt1[] =
{
	USB_STRING_LEN(4),
	DTYPE_String,
	WBVAL('A'),
	WBVAL('L'),
	WBVAL('T'),
	WBVAL('1'),
};

uint8_t DescAC0[] =
{
	USB_STRING_LEN(3),
	DTYPE_String,
	WBVAL('A'),
	WBVAL('C'),
	WBVAL('1'),
};

uint8_t DescIF0[] =
{
	USB_STRING_LEN(3),
	DTYPE_String,
	WBVAL('I'),
	WBVAL('F'),
	WBVAL('0'),
};

uint8_t DescIAD[] =
{
	USB_STRING_LEN(3),
	DTYPE_String,
	WBVAL('I'),
	WBVAL('A'),
	WBVAL('D'),
};

//-----------------------------------------------------------------------------
uint8_t ClockSource[] = 
{
	USB_STRING_LEN(6),DTYPE_String,WBVAL('C'),WBVAL('L'),WBVAL('K'),WBVAL('S'),WBVAL('R'),WBVAL('C'),
};

//-----------------------------------------------------------------------------
uint8_t ClockSelector[] = 
{
	USB_STRING_LEN(6),DTYPE_String,WBVAL('C'),WBVAL('L'),WBVAL('K'),WBVAL('S'),WBVAL('E'),WBVAL('L'),
};

//-----------------------------------------------------------------------------
uint8_t InputTerminal[] = 
{
	USB_STRING_LEN(6),DTYPE_String,WBVAL('I'),WBVAL('P'),WBVAL('T'),WBVAL('E'),WBVAL('R'),WBVAL('M'),
};

//-----------------------------------------------------------------------------
uint8_t OutputTerminal[] = 
{
	USB_STRING_LEN(6),DTYPE_String,WBVAL('O'),WBVAL('P'),WBVAL('T'),WBVAL('E'),WBVAL('R'),WBVAL('M'),
};

//-----------------------------------------------------------------------------
uint8_t acInterfaceDescriptor[] = 
{
	USB_STRING_LEN(5),DTYPE_String,WBVAL('A'),WBVAL('C'),WBVAL('I'),WBVAL('F'),WBVAL('D'),
};

//-----------------------------------------------------------------------------
USB_Descriptor_String_t* descriptor_string[] = 
{
	(USB_Descriptor_String_t*) LanguageString,
	(USB_Descriptor_String_t*) ManufacturerString,
	(USB_Descriptor_String_t*) ProductString,
	(USB_Descriptor_String_t*) DescSerial,
	(USB_Descriptor_String_t*) DescAlt0,
	(USB_Descriptor_String_t*) DescAlt1,
	(USB_Descriptor_String_t*) DescAC0,
	(USB_Descriptor_String_t*) DescIF0,
	(USB_Descriptor_String_t*) DescIAD,
	(USB_Descriptor_String_t*) ClockSource,
	(USB_Descriptor_String_t*) ClockSelector,
	(USB_Descriptor_String_t*) InputTerminal,
	(USB_Descriptor_String_t*) OutputTerminal,
	(USB_Descriptor_String_t*) acInterfaceDescriptor
};

//-----------------------------------------------------------------------------
/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */

//-----------------------------------------------------------------------------
//
uint16_t CALLBACK_USB_GetDescriptor(uint8_t corenum,
                                    const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void * *const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);
	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;
	
	portBASE_TYPE xHigherPriorityTaskWoken;
	dbg_message dbm = { 0, 0 };
	dbm.psz = states[eGetDescriptor];
	dbm.flags = DescriptorType;
	dbm.value = DescriptorNumber;
	
	switch (DescriptorType)
	{
	case DTYPE_Device:
		Address = &DeviceDescriptor;
		Size    = sizeof(USB_Descriptor_Device_t);
		break;
	case DTYPE_Configuration:
#ifdef _USE_AC2	
		Address = GetConfigStruct();
		Size    = GetConfigStructSize();
#else		
		Address = &ConfigurationDescriptor;
		Size    = sizeof(USB_Descriptor_Configuration_t);
#endif		
		break;
	case DTYPE_String:
		if (DescriptorNumber < eINTERFACE_STRING_MAX)
		{
			USB_Descriptor_String_t* p = descriptor_string[DescriptorNumber];
			Address = p;
			Size    = pgm_read_byte(&p->Header.Size);
		}
		break;
	}
	// queue the details for later logging
	dbm.v2 = Size;
	xQueueSendFromISR(xqh,&dbm,&xHigherPriorityTaskWoken);
	// set the address of the string 'struct'
	*DescriptorAddress = Address;
	// return the number of bytes ...
	return Size;
}
