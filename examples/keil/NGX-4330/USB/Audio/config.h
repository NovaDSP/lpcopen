/*

	Config file for commonly modified settings

*/

#ifndef CONFIG_H
#define CONFIG_H

// if 1 then we will set to full-speed (12MBS) 
// if 0 then we run at default hispeed (480MBS)

// Mac
//#define USE_FULL_SPEED 1
//#define CHANNEL_COUNT 6

// Windows/Linux
#define USE_FULL_SPEED 0
#define CHANNEL_COUNT 12

// the default is 12 channels ...

// verified working W7x64 WASAPI USB3 hub
// #define CHANNEL_COUNT 12

// verified working W7x64 WASAPI USB3 hub
//#define CHANNEL_COUNT 24

// verified working W7x64 WASAPI USB3 hub
// #define CHANNEL_COUNT 36

// verified working W7x64 WASAPI USB3 hub
// #define CHANNEL_COUNT 48

// valid sizes are 2 and 3 for 16 and 24 bit respectively
// do not change for purposes of resolving channel mask issue
#define BYTES_PER_SAMPLE 2

// sets the isoch endpoint size
#define EP_SIZE_BYTES 1024

// how many sample rates do we support?
// we need this! if we fail to expose any user-configurable properties
// then Window will never display an advanced tab in Sound applet ...
#define SUPPORTED_SAMPLE_RATES 5

// set to 1 to use channel mask, set to 0 to zero it out
#define USE_CHANNEL_MASK 0

// set this to 1 to experiment with device controls
// like volume etc.
// Do not modify. This will break things/
#define USE_FEATURE 0

#endif

