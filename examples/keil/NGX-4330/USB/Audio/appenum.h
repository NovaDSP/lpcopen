/*


*/


#ifndef APPENUM_H
#define APPENUM_H

//-----------------------------------------------------------------------------
//
typedef enum 
{ 
	eLanguage, 
	eManufacturer,
	eProduct, 
	eSerial,
	eINTERFACE_STRING_ALT0,
	eINTERFACE_STRING_ALT1,
	eINTERFACE_STRING_AC0,
	eINTERFACE_STRING_IF0,
	eINTERFACE_STRING_IAD,
	eINTERFACE_STRING_CLOCK_SOURCE,
	eINTERFACE_STRING_CLOCK_SELECTOR,
	eINTERFACE_STRING_INPUT_TERMINAL,
	eINTERFACE_STRING_OUTPUT_TERMINAL,
	eINTERFACE_STRING_ACINTERFACEDESCRIPTOR,
	eINTERFACE_STRING_MAX
} StringDescriptors;

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


extern const char* states[MaxStates];


#endif
