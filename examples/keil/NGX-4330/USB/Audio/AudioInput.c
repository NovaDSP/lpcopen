/*
 * @brief Make your board becomes a USB microphone
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

#pragma diag_suppress 368

#include "AudioInput.h"
#include <limits.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/** Audio Class driver interface configuration and state information. This structure is
 *  passed to all Audio Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_Audio_Device_t Microphone_Audio_Interface =
{
	.Config = {
		.StreamingInterfaceNumber = 1,

		.DataINEndpointNumber     = AUDIO_STREAM_EPNUM,
		.DataINEndpointSize       = AUDIO_STREAM_EPSIZE,
		.PortNumber             = 0,
	},
};

// for runtime modification
extern USB_Descriptor_Configuration_t ConfigurationDescriptor;
//
#define TICKRATE_HZ 1

#define BLUELED 1
#define GREENLED 0

/** Max Sample Frequency. */
#define AUDIO_MAX_SAMPLE_FREQ   48000
/** Current audio sampling frequency of the streaming audio endpoint. */
uint32_t CurrentAudioSampleFrequency = AUDIO_MAX_SAMPLE_FREQ;
/* Sample Buffer */
// uint16_t* sample_buffer = NULL;
PRAGMA_ALIGN_4
//
int modulo = 6 * 500;

//-----------------------------------------------------------------------------
/* Configures the board hardware and chip peripherals for the demo's
   functionality. */
static void SetupHardware(void)
{
	Board_Init();
	Board_Buttons_Init();
}

//-----------------------------------------------------------------------------
void Audio_Reset_Data_Buffer(void)
{}

/** This callback function provides iso buffer address for HAL iso transfer processing.
 * for ISO In EP, this function also returns the size of buffer, depend on SampleFrequency.
 */

// LED state 
bool iso_state = false; 
// ISO packet callback counter
int iso_packets = 0; 
//
int iso_index = 0;

//-----------------------------------------------------------------------------
// we assume that on SOF interrput we send N channels * sizeof sample to isoch EP
#ifdef FULL_SPEED
// 48 samples every microframe
const int SAMPLE_COUNT = 48;
#else
// 6 samples per 125ns microframe
const int SAMPLE_COUNT = 6;
#endif

//-----------------------------------------------------------------------------
// JME fix sample size at 16 bits for the time being. Keeps init code simple
// in any other configuration simply send zeroes
int16_t data[CHANNEL_COUNT * SAMPLE_COUNT] = { 0 };


//-----------------------------------------------------------------------------
// Called on every SOF interrupt. Thus 8KHz at high-speed and 1KHz at full-speed
// this is actually returning a pointer to the data buffer to be transferred
//
uint32_t CALLBACK_HAL_GetISOBufferAddress(const uint32_t EPNum, uint32_t* packet_size)
{
	*packet_size = CHANNEL_COUNT * SAMPLE_COUNT * BYTES_PER_SAMPLE;
	// Check if this is audio stream endpoint
	if ((EPNum & 0x7F) == AUDIO_STREAM_EPNUM)
	{
		iso_packets++;
		if (iso_packets % modulo == 0)
		{
			iso_state = !iso_state;
			// should flash at sub-multiple of blue SOF LED
			// Board_LED_Set(GREENLED, iso_state);
		}
		// return the audio data buffer
		return (uint32_t) &data[0];
	}
	else
	{
		return 0;
	}
}
//-----------------------------------------------------------------------------
// timer runs and ticks green LED so we know we are waiting for SW2
void TIMER1_IRQHandler(void)
{
	static bool On = false;

	if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) 
	{
		Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
		On = (bool) !On;
		Board_LED_Set(GREENLED, On);
	}
}

//-----------------------------------------------------------------------------
// Set up all the initial timer IRQ stuff
void InitTimer()
{
	//
	uint32_t timerFreq = 0;
	/* Enable timer 1 clock and reset it */
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_RGU_TriggerReset(RGU_TIMER1_RST);
	while (Chip_RGU_InReset(RGU_TIMER1_RST)) 
	{
		// NOP
	}

	/* Get timer 1 peripheral clock rate */
	timerFreq = Chip_Clock_GetRate(CLK_MX_TIMER1);

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (timerFreq / TICKRATE_HZ));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
	Chip_TIMER_Enable(LPC_TIMER1);

	/* Enable timer interrupt */
//	NVIC_EnableIRQ(TIMER1_IRQn);
//	NVIC_ClearPendingIRQ(TIMER1_IRQn);
}

//-----------------------------------------------------------------------------
// Main program entry point. This routine contains the overall program flow, including initial
 // setup of all components and the main program loop.
int main(void)
{
	int i = 0;
	
	// we loop at startup until SW2 is pressed
	uint32_t Button_State = 0;
	
	// set up hardware
	SetupHardware();
	
	// and ticker timer at 1hz
	InitTimer();
	
	// switch off green LED
	Board_LED_Set(GREENLED, false);
	
	// this can be played with in the debugger ...
	bool use_channel_mask = (USE_CHANNEL_MASK == 1);
	// the default mask according to the spec can be zero
	uint32_t mask = 0;
	// set the channel mask at run-time
	if (use_channel_mask)
	{
		// set a bit for each channel
		for (i = 0; i < CHANNEL_COUNT; i++)
		{
			mask |= (1 << i);
		}
	}
	// now set the mask.
	ConfigurationDescriptor.Audio_InputTerminal.ChannelConfig = mask;

	Board_Debug_Init();
	Board_UARTPutChar('\n');
	Board_UARTPutChar('.');
	Board_UARTPutChar('.');
	Board_UARTPutChar('.');
	Board_UARTPutChar('\n');
	
	// Enable timer interrupt
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	
	int16_t value = 0;
	int16_t step = SHRT_MAX / (SAMPLE_COUNT / 2);
	int index = 0;
	// set up the sample pattern in the USB xfer buffer
	// depending on how many channels and bus speed
	// this is a really simple full range saw wave
	int sample = 0;
	int channel = 0;
	for (sample = 0; sample < SAMPLE_COUNT; sample++)
	{
		// apply same values to all channels
		for (channel = 0; channel < CHANNEL_COUNT; channel++)
		{
			data[index++] = value;
		}
		// next value
		value += step;
	}

	// wait. do not connect until button 1 is pressed
	for (;;)
	{
		if ((Buttons_GetStatus() & BUTTONS_BUTTON1) != Button_State)
		{
			Board_LED_Set(GREENLED, 1);
			break;
		}
	}

	// stop the timer IRQ
	NVIC_DisableIRQ(TIMER1_IRQn);		
	//
	// Initialize the USB audio driver
	USB_Init(Microphone_Audio_Interface.Config.PortNumber, USB_MODE_Device,USE_FULL_SPEED);

	// enable SOF interrupt
	USB_Device_EnableSOFEvents();

#if defined(USB_DEVICE_ROM_DRIVER)
	UsbdAdc_Init(&Microphone_Audio_Interface);
#endif
	for (;;)
	{
		//
		if ((Buttons_GetStatus() & BUTTONS_BUTTON1) != Button_State)
		{
		
		}
		
#if !defined(USB_DEVICE_ROM_DRIVER)
		Audio_Device_USBTask(&Microphone_Audio_Interface);
		USB_USBTask(Microphone_Audio_Interface.Config.PortNumber, USB_MODE_Device);
#endif
	}
}

//-----------------------------------------------------------------------------
/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	Board_LED_Set(GREENLED, 1);
}

//-----------------------------------------------------------------------------
/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	Board_LED_Set(GREENLED, 0);
}

//-----------------------------------------------------------------------------
/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= Audio_Device_ConfigureEndpoints(&Microphone_Audio_Interface);
	//
/*
	if (USB_Device_ConfigurationNumber == 0)
	{
		Board_LED_Set(GREENLED, false);	
	}
	else
	{
		Board_LED_Set(GREENLED, true);
	}
*/
}

//-----------------------------------------------------------------------------
/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	Audio_Device_ProcessControlRequest(&Microphone_Audio_Interface);
}

//-----------------------------------------------------------------------------
// Indicates if streaming has started ...
void EVENT_Audio_Device_StreamStartStop(USB_ClassInfo_Audio_Device_t *const AudioInterfaceInfo)
{
	if (AudioInterfaceInfo->State.InterfaceEnabled)
	{
		Board_LED_Set(GREENLED, 1);
	}
	else
	{
		Board_LED_Set(GREENLED, 0);
	}
}

//-----------------------------------------------------------------------------
// void USB_Event_Stub(void)

static uint32_t counter = 0;
static bool lit = false;

void EVENT_USB_Device_StartOfFrame(void)
{
	counter++;
	if (counter % modulo == 0)
	{
		lit = !lit;
		Board_LED_Set(BLUELED, lit);
	}
}

//-----------------------------------------------------------------------------
/** Audio class driver callback for the setting and retrieval of streaming endpoint properties. This callback must be implemented
 *  in the user application to handle property manipulations on streaming audio endpoints.
 */
bool CALLBACK_Audio_Device_GetSetEndpointProperty(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
        const uint8_t EndpointProperty,
        const uint8_t EndpointAddress,
        const uint8_t EndpointControl,
        uint16_t* const DataLength,
        uint8_t* Data)
{
	/* Check the requested endpoint to see if a supported endpoint is being manipulated */
	if (EndpointAddress == (ENDPOINT_DIR_IN | Microphone_Audio_Interface.Config.DataINEndpointNumber))
	{
		/* Check the requested control to see if a supported control is being manipulated */
		if (EndpointControl == AUDIO_EPCONTROL_SamplingFreq)
		{
			switch (EndpointProperty)
			{
			case AUDIO_REQ_SetCurrent:
				/* Check if we are just testing for a valid property, or actually adjusting it */
				if (DataLength != NULL)
				{
					/* Set the new sampling frequency to the value given by the host */
					CurrentAudioSampleFrequency =
					    (((uint32_t) Data[2] << 16) | ((uint32_t) Data[1] << 8) | (uint32_t) Data[0]);
					if (CurrentAudioSampleFrequency > AUDIO_MAX_SAMPLE_FREQ)
					{
						return false;
					}
					// JME adjust buffer size here.
				}
				return true;
			case AUDIO_REQ_GetCurrent:
				/* Check if we are just testing for a valid property, or actually reading it */
				if (DataLength != NULL)
				{
					*DataLength = 3;
					Data[2] = (CurrentAudioSampleFrequency >> 16);
					Data[1] = (CurrentAudioSampleFrequency >> 8);
					Data[0] = (CurrentAudioSampleFrequency &  0xFF);
				}
				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
/** Audio class driver callback for the setting and retrieval of streaming interface properties. This callback must be implemented
 *  in the user application to handle property manipulations on streaming audio interfaces.
 *
 */
bool CALLBACK_Audio_Device_GetSetInterfaceProperty(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
        const uint8_t Property,
        const uint8_t EntityAddress,
        const uint16_t Parameter,
        uint16_t* const DataLength,
        uint8_t* Data)
{
	/* No audio interface entities in the device descriptor, thus no properties to get or set. */
	return false;
}
