/*


*/


#ifndef APPENUM_H
#define APPENUM_H

//-----------------------------------------------------------------------------
// abstract as much as possible
#include "logger.h"

typedef enum 
{
	eTIDNoTerminal,
	eTIDInput,
	eTIDOutput,
	eTIDClock = 9,
	
} TerminalID;

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

#endif
