/*

	Config file for commonly modified settings

*/

#ifndef CONFIG_H
#define CONFIG_H

//#define CHANNEL_COUNT 32
#define CHANNEL_COUNT 2
// set to 1 to use channel mask, set to 0 to zero it out
#define USE_CHANNEL_MASK 0

// JME this is channel independent
// do not change for purposes of resolving channel mask issue
#define SAMPLE_COUNT 6

// valid sizes are 2 and 3 for 16 and 24 bit respectively
// do not change for purposes of resolving channel mask issue
#define BYTES_PER_SAMPLE 2

// sets the isoch endpoint size
#define EP_SIZE_BYTES 1024

// set this to 1 to experiment with device controls
// like volume etc.
// Do not modify. This will break things/
#define USE_FEATURE 0

#endif

