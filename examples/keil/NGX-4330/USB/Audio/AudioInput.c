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

#include "AudioInput.h"

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
// JME
//uint16_t sample_buffer[512*CHANNEL_COUNT] ATTR_ALIGNED(4) __BSS(USBRAM_SECTION);
//uint32_t sample_buffer_size = 512*CHANNEL_COUNT*sizeof(uint16_t);

// waveform test
/*
const uint32_t check = CHANNEL_COUNT * 4;
uint16_t sample_buffer[check] ATTR_ALIGNED(4) __BSS(USBRAM_SECTION) =
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	5461,5461,5461,5461,5461,5461,5461,5461,5461,5461,5461,5461,
	10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,
	16383,16383,16383,16383,16383,16383,16383,16383,16383,16383,16383,16383,
};
*/

// Sample rate: 48000hz  Sine frequency 1000hz 1024 samples
const int buffer_size = 1024;
int16_t sample_buffer[buffer_size] ATTR_ALIGNED(4) __BSS(USBRAM_SECTION) = {
4210,8351,12539,16325,19840,23169,25954,28309,30195,31633,32468,32766,32495,31684,30272,28410,
26077,23169,20000,16499,12539,8545,4409,0,-4210,-8351,-12539,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,28410,
26077,23311,20000,16499,12724,8545,4409,201,-4210,-8351,-12353,-16325,-19840,-23027,-25954,-28309,
-30195,-31633,-32468,-32766,-32495,-31684,-30349,-28410,-26077,-23311,-20000,-16499,-12724,-8545,-4409,-201,
4210,8351,12353,16325,19840,23027,25954,28309,30195,31633,32468,32766,32495,31684,30349,-22099,
};
//
uint32_t sample_buffer_size = buffer_size;
//
int modulo = 6 * 500;

//-----------------------------------------------------------------------------
/* Configures the board hardware and chip peripherals for the demo's
   functionality. */
static void SetupHardware(void)
{
	Board_Init();
	Board_Buttons_Init();
	USB_Init(Microphone_Audio_Interface.Config.PortNumber, USB_MODE_Device);
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

int16_t data[CHANNEL_COUNT] = { 0 };

//-----------------------------------------------------------------------------
// this is actually returning a pointer to the data buffer to be transferred
uint32_t CALLBACK_HAL_GetISOBufferAddress(const uint32_t EPNum, uint32_t* packet_size)
{
	// how large is my buffer in *bytes*
	*packet_size = CHANNEL_COUNT * sizeof(int16_t);
	// Check if this is audio stream endpoint
	if ((EPNum & 0x7F) == AUDIO_STREAM_EPNUM)
	{
		iso_packets++;
		if (iso_packets % modulo == 0)
		{
			iso_state = !iso_state;
			// should flash at sub-multiple of blue SOF LED
			Board_LED_Set(GREENLED, iso_state);
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
void TIMER1_IRQHandler(void)
{
	static bool On = false;

	if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) 
	{
		Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
		On = (bool) !On;
		Board_LED_Set(BLUELED, On);
	}
}

//-----------------------------------------------------------------------------
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
/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	uint32_t Button_State = 0;
	SetupHardware();
	
	InitTimer();
	
	Board_LED_Set(GREENLED, 0);
	
	//Board_Debug_Init();
	
	//Board_UARTPutSTR("------------Main-------------\n");
	USB_Device_EnableSOFEvents();
	
	// sample_buffer = (uint16_t*)Audio_Get_ISO_Buffer_Address(0);
#if defined(USB_DEVICE_ROM_DRIVER)
	UsbdAdc_Init(&Microphone_Audio_Interface);
#endif
	for (;; )
	{
		/* Only generate audio if the board button is being pressed */
		/* Generate Square Wave at 1kHz */
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
