/*


*/

#include <stdint.h>
#include "USB.h"
#include "apptypes.h"

// helper macro for identification
#define  STRUCT(Type,Name)
#define  KV(Key,Value)	(Value)

#define  AUDIO_INTERFACE_PROTOCOL_UNDEFINED         0x00
#define  AUDIO_INTERFACE_IP_VERSION_02_00           0x20
#define  IP_VERSION_02_00	AUDIO_INTERFACE_IP_VERSION_02_00

#define MSK_EP_NBR                            0x0F
#define MSK_EP_DIR                            0x80

#define USB_CONFIG_ATTRIBUTES_RESERVED    0x80
#define USB_CONFIG_BUSPOWERED            (USB_CONFIG_ATTRIBUTES_RESERVED | 0x00)
#define USB_CONFIG_SELFPOWERED           (USB_CONFIG_ATTRIBUTES_RESERVED | 0x40)
#define USB_CONFIG_REMOTEWAKEUP          (USB_CONFIG_ATTRIBUTES_RESERVED | 0x20)

#define Usb_format_mcu_to_usb_data(arg1,arg2) (arg2)

#define GET_STATUS                            0x00
#define GET_DEVICE                            0x01
#define CLEAR_FEATURE                         0x01
#define GET_STRING                            0x03
#define SET_FEATURE                           0x03
#define SET_ADDRESS                           0x05
#define GET_DESCRIPTOR                        0x06
#define SET_DESCRIPTOR                        0x07
#define GET_CONFIGURATION                     0x08
#define SET_CONFIGURATION                     0x09
#define GET_INTERFACE                         0x0A
#define SET_INTERFACE                         0x0B
#define SYNCH_FRAME                           0x0C

#define GET_DEVICE_DESCRIPTOR                    1
#define GET_CONFIGURATION_DESCRIPTOR             4

#define REQUEST_DEVICE_STATUS                 0x80
#define REQUEST_INTERFACE_STATUS              0x81
#define REQUEST_ENDPOINT_STATUS               0x82
#define DEVICE_TYPE                           0x00
#define INTERFACE_TYPE                        0x01
#define ENDPOINT_TYPE                         0x02

// Descriptor Types
#define DEVICE_DESCRIPTOR                     0x01
#define CONFIGURATION_DESCRIPTOR              0x02
#define STRING_DESCRIPTOR                     0x03
#define INTERFACE_DESCRIPTOR                  0x04
#define ENDPOINT_DESCRIPTOR                   0x05
#define DEVICE_QUALIFIER_DESCRIPTOR           0x06
#define OTHER_SPEED_CONFIGURATION_DESCRIPTOR  0x07
#define DESCRIPTOR_IAD 0x0B
#define CS_INTERFACE 0x24
#define CS_ENDPOINT                     0x25

#define GENERAL_SUB_TYPE                0x01
#define FORMAT_SUB_TYPE                 0x02
#define HEADER_SUB_TYPE                 0x01
#define MIXER_UNIT_SUB_TYPE             0x04
#define FEATURE_UNIT_SUB_TYPE           0x06
#define INPUT_TERMINAL_SUB_TYPE         0x02
#define OUTPUT_TERMINAL_SUB_TYPE        0x03


#define  AUDIO_INTERFACE_SUBCLASS_UNDEFINED         0x00
#define  AUDIO_INTERFACE_SUBCLASS_AUDIOCONTROL      0x01
#define  AUDIO_INTERFACE_SUBCLASS_AUDIOSTREAMING    0x02
#define  AUDIO_INTERFACE_SUBCLASS_MIDISTREAMING     0x03

#define NO_CLASS                              0x00
#define NO_SUBCLASS                                0x00
#define NO_PROTOCOL                                     0x00
#define AUDIO_CLASS                           0x01
#define AUDIOCONTROL_SUBCLASS                      0x01
#define AUDIOSTREAMING_SUBCLASS                    0x02
#define MIDISTREAMING_SUBCLASS                     0x03

#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_UNDEFINED              0x00
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_HEADER                 0x01
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_INPUT_TERMINAL         0x02
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_OUTPUT_TERMINAL        0x03
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_MIXER_UNIT             0x04
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_SELECTOR_UNIT          0x05
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_FEATURE_UNIT           0x06
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_EFFECT_UNIT            0x07
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_PROCESSING_UNIT        0x08
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_EXTENSION_UNIT         0x09
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SOURCE           0x0A
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SELECTOR         0x0B
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_MULTIPLIER       0x0C
#define  DESCRIPTOR_SUBTYPE_AUDIO_AC_SAMPLE_RATE_CONVERTE   0x0D

#define HEADER_SUB_TYPE 0x01
#define DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SOURCE 0x0A
// CONFIGURATION
#define NB_INTERFACE	   2	//!  DG8SAQ, Audio (3)
#define CONF_NB            1     //! Number of this configuration
#define CONF_INDEX         0
#define CONF_ATTRIBUTES    USB_CONFIG_SELFPOWERED
#define MAX_POWER          250    // 500 mA

// IAD for Audio
#define FIRST_INTERFACE1	1
#define INTERFACE_COUNT1	3						//!  Audio Control, Audio In, Audio Out
#define FUNCTION_CLASS		AUDIO_CLASS
#define FUNCTION_SUB_CLASS  0
#define FUNCTION_PROTOCOL	IP_VERSION_02_00
#define FUNCTION_INDEX		0

// USB DG8SAQ Interface descriptor
#define INTERFACE_NB0			    0
#define ALTERNATE_NB0	            0                  //! The alt setting nb of this interface
#define NB_ENDPOINT0			    0                  //! The number of endpoints this interface has
#define INTERFACE_CLASS0		    NO_CLASS          //! No Class
#define INTERFACE_SUB_CLASS0        NO_SUBCLASS        //! No Subclass
#define INTERFACE_PROTOCOL0    		NO_PROTOCOL		   //! No Protocol
#define INTERFACE_INDEX0       		0

#define DSC_INTERFACE_DG8SAQ		INTERFACE_NB0

#define WL_INDEX		0x07
#define AIT_INDEX		0x08
#define AOT_INDEX		0x09
#define AIN_INDEX		0x0A
#define AIA_INDEX		0x0B

// Audio Class V2.0 descriptor values

// Standard Audio Control (AC) interface descriptor
#define INTERFACE_NB1       1
#define ALTERNATE_NB1       0
#define NB_ENDPOINT1        0			     //! No endpoint for AC interface
#define INTERFACE_CLASS1    AUDIO_CLASS  	//! Audio Class
#define INTERFACE_SUB_CLASS1 AUDIO_INTERFACE_SUBCLASS_AUDIOCONTROL
#define INTERFACE_PROTOCOL1  IP_VERSION_02_00		     //! IP_VERSION_02_00 ie UAC V2
#define INTERFACE_INDEX1     WL_INDEX

#define DSC_INTERFACE_AUDIO			INTERFACE_NB1

// endpoint indexes
#define UAC2_EP_AUDIO_IN		1
#define UAC2_EP_AUDIO_OUT		2
#define UAC2_EP_AUDIO_OUT_FB	3

// USB Endpoint 1 descriptor
#define ENDPOINT_NB_1       ( UAC2_EP_AUDIO_IN | MSK_EP_DIR )
#define EP_ATTRIBUTES_1		0x0
#define EP_IN_LENGTH_1_FS	294				// 3 bytes * 49 samples * stereo
#define EP_IN_LENGTH_1_HS	294
#define EP_SIZE_1_FS		EP_IN_LENGTH_1_FS
#define EP_SIZE_1_HS        EP_IN_LENGTH_1_HS
#define EP_INTERVAL_1_FS	0x01			 // one packet per uframe, each uF 1ms, so only 48khz
#define EP_INTERVAL_1_HS    0x02			 // One packet per 2 uframe, each uF 125us, so 192khz


// USB Endpoint 2 descriptor
#define ENDPOINT_NB_2       ( UAC2_EP_AUDIO_OUT )
#define EP_ATTRIBUTES_2     0x0
#define EP_OUT_LENGTH_2_HS   294				// 3 bytes * 49 samples * stereo
#define EP_OUT_LENGTH_2_FS	294
#define EP_SIZE_2_FS		EP_OUT_LENGTH_2_FS
#define EP_SIZE_2_HS        EP_OUT_LENGTH_2_HS
#define EP_INTERVAL_2_FS	0x01			 // one packet per uframe
#define EP_INTERVAL_2_HS    0x02			 // One packet per 2 uframe


// USB Endpoint 3 descriptor
#define ENDPOINT_NB_3       ( UAC2_EP_AUDIO_OUT_FB | MSK_EP_DIR )		// 0x83
#define EP_ATTRIBUTES_3     0x0
#define EP_IN_LENGTH_3_FS   64				//
#define EP_IN_LENGTH_3_HS	64				//
#define EP_SIZE_3_FS		EP_IN_LENGTH_3_FS
#define EP_SIZE_3_HS        EP_IN_LENGTH_3_HS
#define EP_INTERVAL_3_FS	0x01
#define EP_INTERVAL_3_HS    0x04
#define EP_REFRESH_3_FS		0x05			 //  16ms
#define EP_REFRESH_3_HS		0x08			 // 2^(8-1) = 128 uframe = 16ms

// AC interface descriptor Audio specific
#define AUDIO_CLASS_REVISION_2          0x0200
#define MIC_CATEGORY					AUDIO_FUNCTION_SUBCLASS_MICROPHONE
#define HEADSET_CATEGORY 				0x04
#define MIC_LATENCY_CONTROL				0x0

// Clock Source descriptor
#define CLOCK_SOURCE_1_INDEX 0x04
#define CSD_ID_1						0x04
#define CSD_ID_1_TYPE					0x01	// fixed freq internal clock
#define CSD_ID_1_CONTROL				0x05	// freq r/w, validity r

#define CLOCK_SOURCE_2_INDEX 0x05
#define CSD_ID_2						0x05
#define CSD_ID_2_TYPE					0x1	// fixed freq internal clock
#define CSD_ID_2_CONTROL				0x05	// freq r/w, validity r

// Clock Selector descriptor
#define CSX_ID							0x06
#define CSX_INPUT_PINS					0x02
#define CSX_SOURCE_1					CSD_ID_1
#define CSX_SOURCE_2					CSD_ID_2
#define CSX_CONTROL						0x02	// clock selector is readable and writable


// Input Terminal descriptor
#define INPUT_TERMINAL_ID				0x01
#define INPUT_TERMINAL_TYPE				0x0201 	// Terminal is microphone
#define INPUT_TERMINAL_ASSOCIATION		0x00   	// No association
#define INPUT_TERMINAL_NB_CHANNELS		0x02   	// Two channels for input terminal
#define INPUT_TERMINAL_CHANNEL_CONF		0x00000003 	// Two channels at front left and front right positions
//#define INPUT_TERMINAL_CONTROLS			0x0040	// D7-6 Cluster control - readonly
#define INPUT_TERMINAL_CONTROLS			0x0000	// none
#define INPUT_TERMINAL_CH_NAME_ID		0x00	// No channel name
#define INPUT_TERMINAL_STRING_DESC	    0x00	// No string descriptor

// Output Terminal descriptor
#define OUTPUT_TERMINAL_ID				0x03
#define OUTPUT_TERMINAL_TYPE			0x0101 	// USB Streaming
#define OUTPUT_TERMINAL_ASSOCIATION		0x00   	// No association
#define OUTPUT_TERMINAL_SOURCE_ID		MIC_FEATURE_UNIT_ID
#define OUTPUT_TERMINAL_CONTROLS		0x0000	// no controls


//MIC Feature Unit descriptor
#define MIC_FEATURE_UNIT_ID            0x02
#define MIC_FEATURE_UNIT_SOURCE_ID     INPUT_TERMINAL_ID
#define MIC_BMA_CONTROLS               0x00000003 	// Mute readable and writable
#define MIC_BMA_CONTROLS_CH_1		   0x00000003	//
#define MIC_BMA_CONTROLS_CH_2		   0x00000003

// Speaker Input Terminal
#define SPK_INPUT_TERMINAL_ID			0x11
#define SPK_INPUT_TERMINAL_TYPE			0x0101	// USB Streaming
#define SPK_INPUT_TERMINAL_ASSOCIATION	0x00	// No association
#define SPK_INPUT_TERMINAL_NB_CHANNELS	0x02
#define SPK_INPUT_TERMINAL_CHANNEL_CONF	0x0003	// left front and right front
#define SPK_INPUT_TERMINAL_CH_NAME_ID	0x00	// No channel name

//SPK Feature Unit descriptor
#define SPK_FEATURE_UNIT_ID            0x12
#define SPK_FEATURE_UNIT_SOURCE_ID     SPK_INPUT_TERMINAL_ID
#define SPK_BMA_CONTROLS               0x00000003 	// Mute readable and writable
#define SPK_BMA_CONTROLS_CH_1		   0x00000003	//
#define SPK_BMA_CONTROLS_CH_2		   0x00000003

// SPK Output Terminal descriptor
#define SPK_OUTPUT_TERMINAL_ID				0x13
#define SPK_OUTPUT_TERMINAL_TYPE		0x0302	// Headphones
#define SPK_OUTPUT_TERMINAL_ASSOCIATION		0x00   	// No association
#define SPK_OUTPUT_TERMINAL_SOURCE_ID		SPK_FEATURE_UNIT_ID
#define SPK_OUTPUT_TERMINAL_CONTROLS		0x0000	// no controls

//Audio Streaming (AS) interface descriptor
#define STD_AS_INTERFACE_IN				0x02   // Index of Std AS Interface for Audio In
#define STD_AS_INTERFACE_OUT			0x03   // Index of Std AS Interface for Audio Out

#define DSC_INTERFACE_AS				STD_AS_INTERFACE_IN
#define DSC_INTERFACE_AS_OUT			STD_AS_INTERFACE_OUT


//Alternate O Audio Streaming (AS) interface descriptor
#define ALT0_AS_INTERFACE_INDEX			0x00   // Index of Std AS interface Alt0
#define ALT0_AS_NB_ENDPOINT				0x00   // Nb of endpoints for alt0 interface
#define ALT0_AS_INTERFACE_CLASS			0x01   // Audio class
#define ALT0_AS_INTERFACE_SUB_CLASS 	0x02   // Audio streamn sub class
#define ALT0_AS_INTERFACE_PROTOCOL		IP_VERSION_02_00


//Alternate 1 Audio Streaming (AS) interface descriptor
#define ALT1_AS_INTERFACE_INDEX			0x01   // Index of Std AS interface Alt1
#define ALT1_AS_NB_ENDPOINT				0x01   // Nb of endpoints for alt1 interface
#define ALT1_AS_INTERFACE_CLASS			0x01   // Audio class
#define ALT1_AS_INTERFACE_SUB_CLASS 	0x02   // Audio streamn sub class
#define ALT1_AS_INTERFACE_PROTOCOL		IP_VERSION_02_00


//Alternate 2 Audio Streaming (AS) interface descriptor
#define ALT2_AS_INTERFACE_INDEX			0x02   // Index of Std AS interface Alt2
#define ALT2_AS_NB_ENDPOINT				0x01   // Nb of endpoints for alt2 interface
#define ALT2_AS_INTERFACE_CLASS			0x01   // Audio class
#define ALT2_AS_INTERFACE_SUB_CLASS 	0x02   // Audio streamn sub class
#define ALT2_AS_INTERFACE_PROTOCOL		IP_VERSION_02_00


//Class Specific AS (general) Interface descriptor
#define AS_TERMINAL_LINK					OUTPUT_TERMINAL_ID		// Unit Id of the output terminal
#define AS_DELAY							0x01		// Interface delay
#define AS_FORMAT_TYPE						0x01		// PCM Format
#define AS_FORMATS							0x01	// PCM only
#define AS_CONTROLS							0x05	// active alt settings r/w, valid alt settings r
#define AS_NB_CHANNELS						0x02
#define AS_CHAN_CONFIG						0x00000003	// L+R front

// Format type for ALT1
#define FORMAT_TYPE_1						0x01	// Format TypeI
#define FORMAT_SUBSLOT_SIZE_1				0x03	// Number of bytes per subslot
#define FORMAT_BIT_RESOLUTION_1				0x18	// 24 bits per sample

//Audio endpoint specific descriptor field
#define AUDIO_EP_ATRIBUTES				0x0	 	// No sampling freq, no pitch, no pading
#define AUDIO_EP_CONTROLS				0x0
#define AUDIO_EP_DELAY_UNIT				0x00	 	// Unused
#define AUDIO_EP_LOCK_DELAY				0x0000		// Unused

//For playback
#define ALT1_AS_NB_ENDPOINT_OUT			0x02   // two EP,  OUT and OUT_FB
#define AS_TERMINAL_LINK_OUT		    SPK_INPUT_TERMINAL_ID

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

//! USB Configuration Descriptor
typedef struct _S_usb_configuration_descriptor
{
	U8      bLength;              //!< Size of this descriptor in bytes
	U8      bDescriptorType;      //!< CONFIGURATION descriptor type
	U16     wTotalLength;         //!< Total length of data returned
	U8      bNumInterfaces;       //!< Number of interfaces for this conf.
	U8      bConfigurationValue;  //!< Value for SetConfiguration resquest
	U8      iConfiguration;       //!< Index of string descriptor
	U8      bmAttributes;         //!< Configuration characteristics
	U8      MaxPower;             //!< Maximum power consumption
}
ATTR_PACKED S_usb_configuration_descriptor;


//! USB Interface Descriptor
typedef
struct _S_usb_interface_descriptor
{
	U8      bLength;              //!< Size of this descriptor in bytes
	U8      bDescriptorType;      //!< INTERFACE descriptor type
	U8      bInterfaceNumber;     //!< Number of interface
	U8      bAlternateSetting;    //!< Value to select alternate setting
	U8      bNumEndpoints;        //!< Number of EP except EP 0
	U8      bInterfaceClass;      //!< Class code assigned by the USB
	U8      bInterfaceSubClass;   //!< Subclass code assigned by the USB
	U8      bInterfaceProtocol;   //!< Protocol code assigned by the USB
	U8      iInterface;           //!< Index of string descriptor
}
ATTR_PACKED S_usb_interface_descriptor;

//! USB Interface Association Descriptor
typedef
struct _S_usb_interface_association_descriptor
{
	U8 bLength;						// Size of this Descriptor in BYTEs
	U8 bDescriptorType;				// INTERFACE_ASSOCIATION Descriptor Type (0x0B)
	U8 bFirstInterface;				// Interface number of the first one associated with this function
	U8 bInterfaceCount;				// Number of contiguous interface associated with this function
	U8 bFunctionClass;				// The class triad of this interface,
	U8 bFunctionSubClass;			//   usually same as the triad of the first interface
	U8 bFunctionProcotol;
	U8 iInterface;					// Index of String Desc for this function
}
ATTR_PACKED S_usb_interface_association_descriptor;

//! A U D I O Class V2.0 Specific paragraph 4.7
//! Audio AC interface descriptor pp 4.7.2
typedef
struct _S_usb_ac_interface_descriptor_2
{
	U8  bLength;               /* Size of this descriptor in bytes */
	U8  bDescriptorType;       /* CS_INTERFACE descriptor type */
	U8  bDescritorSubtype;     /* HEADER subtype */
	U16 bcdADC;          		  /* Revision of class spec */
	U8  bCategory;				/* Primary use of this function */
	U16 wTotalLength;       	  /* Total size of class specific descriptor */
	U8  bmControls;		     /* Latency Control Bitmap */
} ATTR_PACKED S_usb_ac_interface_descriptor_2;


//! Clock Source descriptor  pp 4.7.2.1
typedef
struct _S_usb_clock_source_descriptor
{
	U8  bLength;               /* Size of this descriptor in bytes */
	U8  bDescriptorType;       /* CS_INTERFACE descriptor type */
	U8 	bDescritorSubtype;     /* CLOCK_SOURCE subtype */
	U8  bClockID;       	  /* Clock Source ID */
	U8  bmAttributes;		     /* Clock Type Bitmap */
	U8  bmControls;			/* Clock control bitmap */
	U8  bAssocTerminal;		/* Terminal ID associated with this source */
	U8  iClockSource;			/* String descriptor of this clock source */
} ATTR_PACKED S_usb_clock_source_descriptor;


//! Clock Selector descriptor pp 4.7.2.2
typedef
struct _S_usb_clock_selector_descriptor
{
	U8  bLength;               /* Size of this descriptor in bytes */
	U8  bDescriptorType;       /* CS_INTERFACE descriptor type */
	U8 	bDescritorSubtype;     /* CLOCK_SELECTOR subtype */
	U8  bClockID;       	  /* Clock Selector ID */
	U8  bNrInPins;		     /* Number of Input Pins */
	U8  baCSourceID1;			/* variable length */
	U8  baCSourceID2;
	U8  bmControls;			/* Clock selector control bitmap  */
	U8  iClockSelector;			/* String descriptor of this clock selector */
} ATTR_PACKED S_usb_clock_selector_descriptor;

//! USB INPUT Terminal Descriptor pp 4.7.2.4
typedef
struct _S_usb_in_ter_descriptor_2
{
	U8		bLength;		/* Size of this descriptor in bytes */
	U8 	bDescriptorType;	/* CS_INTERFACE descriptor type */
	U8 	bDescriptorSubType;	/* INPUT_TERMINAL subtype */
	U8		bTerminalID;	/* Input Terminal ID */
	U16	wTerminalType;		/* Terminal type */
	U8		bAssocTerminal;	/* Output terminal this input is associated with */
	U8		bCSourceID;		/* ID of Clock entity to which this terminal is connected */
	U8		bNrChannels;	/* Number of Logical output channels */
	U32	bmChannelConfig;	/* Spatial location of logical channels */
	U8		iChannelNames;	/* String descriptor of first logical channel */
	U16  bmControls;		/* Paired Bitmap of controls */
	U8		iTerminal;		/* String descriptor of this Input Terminal */
} ATTR_PACKED S_usb_in_ter_descriptor_2;


//! USB OUTPUT Terminal Descriptor pp 4.7.2.5
typedef
struct _S_usb_out_ter_descriptor_2
{
	U8		bLength;		/* Size of this descriptor in bytes */
	U8 	bDescriptorType;	/* CS_INTERFACE descriptor type */
	U8 	bDescriptorSubType;	/* OUTPUT_TERMINAL subtype */
	U8		bTerminalID;	/* Output Terminal ID */
	U16	wTerminalType;		/* Terminal type */
	U8		bAssocTerminal;	/* Input Terminal this output is associated with */
	U8		bSourceID;		/* ID of the Unit or Terminal to which this teminal is connected to */
	U8      bCSourceID;		/* ID od the Clock Entity to which this terminal is connected */
	U16  bmControls;		/* Paired Bitmap of controls */
	U8		iTerminal;		/* String descriptor of this Output Terminal */
} ATTR_PACKED S_usb_out_ter_descriptor_2;

//! USB Mixer Unit descriptor pp 4.7.2.6
//! USB Selector Unit descriptor pp 4.7.2.7


//! USB Audio Feature Unit descriptor pp 4.7.2.8
typedef
struct _S_usb_feature_unit_descriptor_2
{
	U8		bLength;			/* Size of this descriptor in bytes */
	U8 	bDescriptorType;        /* CS_INTERFACE descriptor type */
	U8 	bDescriptorSubType; 	/* FEATURE_UNIT  subtype */
	U8		bUnitID;			/* Feature unit ID */
	U8		bSourceID;			/* ID of the Unit or Terminal to which this teminal is connected to */
	U32		bmaControls;	/* Master Channel 0*/
	U32	    bmaControls_1;  // Channel 1
	U32     bmaControls_2;  // Channel 2
	U8	iFeature;  /* String Descriptor of this Feature Unit */
} ATTR_PACKED S_usb_feature_unit_descriptor_2;

//! USB Standard AS interface Descriptor pp 4.9.1
typedef
struct _S_usb_as_interface_descriptor
{
	U8		bLength;			/* Size of this descriptor in bytes */
	U8 		bDescriptorType;		/* INTERFACE descriptor type */
	U8		bInterfaceNumber;	/* Number of the interface (0 based) */
	U8		bAlternateSetting;
	U8		bNumEndpoints;		/* Number of endpoints in this interface */
	U8		bInterfaceClass;	/* AUDIO Interface class code */
	U8		bInterfaceSubclass;	/* AUDIO_STREAMING Interface subclass code */
	U8		bInterfaceProtocol;	/* IP_VERSION_02_00 Interface protocol code */
	U8		iInterface;			/* String descriptor of this Interface */
} ATTR_PACKED S_usb_as_interface_descriptor;


//! USB Class-Specific AS general interface descriptor pp 4.9.2
typedef
struct _S_usb_as_g_interface_descriptor_2
{
	U8		bLength;			/* Size of this descriptor in bytes */
	U8 	bDescriptorType;		/* CS_INTERFACE descriptor type */
	U8 	bDescriptorSubType;		/* AS_GENERAL subtype */
	U8		bTerminalLink;		/* Terminal ID to which this interface is connected */
	U8  	bmControls;			/* Bitmap of controls */
	U8  bFormatType;			/* Format type the interface is using */
	U32 bmFormats;				/* Bitmap of Formats this interface supports */
	U8  bNrChannels;			/* Number of Physical channels in this interface cluster */
	U32 bmChannelConfig;		/* Bitmap of spatial locations of the physical channels */
	U8  iChannelNames;			/* String descriptor of the first physical channel */
} ATTR_PACKED S_usb_as_g_interface_descriptor_2;


//! Class-Specific Audio Format Type descriptor pp 4.9.3 -> 2.3.1.6 Type I Format
typedef
struct _S_usb_format_type_2
{
	U8		bLength;			/* Size of this descriptor in bytes */
	U8 	bDescriptorType;		/* CS_INTERFACE descriptor type */
	U8 	bDescriptorSubType;		/* FORMAT_TYPE subtype */
	U8		bFormatType;		/* Format Type this streaming interface is using */
	U8		bSubslotSize;		/* Number of bytes in one audio subslot */
	U8		bBitResolution;		/* Number of bits used from the available bits in the subslot */
} ATTR_PACKED S_usb_format_type_2;


//! USB Class-Specific AS Encoder Descriptor pp 4.9.4
//! USB Encoder Descriptor pp 4.9.4.1
//! USB Class-Specific AS Decoder Descriptor pp 4.9.5
//! USB MPEG Decoder Descriptor pp 4.9.5.1
//! USB AC-3 Decoder Descriptor pp 4.9.5.2
//! USB WMA Decoder Descriptor pp 4.9.5.3
//! USB DTS Decoder Descriptor pp 4.9.5.4


//! Usb Standard AS Isochronous Audio Data Endpoint Descriptors pp 4.10.1.1
typedef
struct _S_usb_endpoint_audio_descriptor_2
{
	U8      bLength;               /* Size of this descriptor in bytes */
	U8      bDescriptorType;       /* CS_ENDPOINT descriptor type */
	U8      bEndpointAddress;      /* Address of the endpoint */
	U8      bmAttributes;          /* Endpoint's attributes */
	U16     wMaxPacketSize;        /* Maximum packet size for this EP */
	U8      bInterval;             /* Interval for polling EP in ms */
} ATTR_PACKED S_usb_endpoint_audio_descriptor_2;


//! Usb Class_Specific AS Isochronous Audio Data Endpoint Descriptors pp 4.10.1.2
typedef
struct _S_usb_endpoint_audio_specific_2
{
	U8		bLength;			/* Size of this descriptor in bytes */
	U8 		bDescriptorType;	/* CS_ENDPOINT descriptor type*/
	U8 		bDescriptorSubType;	/* EP_GENERAL subtype */
	U8		bmAttributes;		/* Bitmap of attributes 8 */
	U8      bmControls;			/* Paired bitmap of controls */
	U8    	bLockDelayUnits;		/* units for wLockDelay */
	U16		wLockDelay;				/* time to lock endpoint */
} ATTR_PACKED S_usb_endpoint_audio_specific_2;

typedef
struct _InterfaceCollectionDescriptor
{
	S_usb_ac_interface_descriptor_2			audioac;
	S_usb_clock_source_descriptor			audio_cs1;
	S_usb_clock_source_descriptor			audio_cs2;
	S_usb_clock_selector_descriptor			audio_csel;
	//			S_usb_clock_multiplier_descriptor		audio_cmul;
	S_usb_in_ter_descriptor_2 				mic_in_ter;
	S_usb_feature_unit_descriptor_2			mic_fea_unit;
	S_usb_out_ter_descriptor_2				mic_out_ter;
	S_usb_in_ter_descriptor_2				spk_in_ter;
	S_usb_feature_unit_descriptor_2			spk_fea_unit;
	S_usb_out_ter_descriptor_2				spk_out_ter;
} 
ATTR_PACKED InterfaceCollectionDescriptor;

typedef
struct _S_usb_user_configuration_descriptor
{
	S_usb_configuration_descriptor			cfg;
	S_usb_interface_descriptor	 			ifc0;

	//! Audio descriptors Class 2

	S_usb_interface_association_descriptor	iad1;
	S_usb_interface_descriptor				ifc1;
	//-------------------------------------------
	InterfaceCollectionDescriptor ifcd;
	//-------------------------------------------
	S_usb_as_interface_descriptor	 		mic_as_alt0;
	S_usb_as_interface_descriptor	 		mic_as_alt1;
	S_usb_as_g_interface_descriptor_2		mic_g_as;
	S_usb_format_type_2						mic_format_type;
	S_usb_endpoint_audio_descriptor_2 		ep1;
	S_usb_endpoint_audio_specific_2			ep1_s;
	S_usb_as_interface_descriptor	 		spk_as_alt0;
	S_usb_as_interface_descriptor	 		spk_as_alt1;
	S_usb_as_g_interface_descriptor_2		spk_g_as;
	S_usb_format_type_2						spk_format_type;
	S_usb_endpoint_audio_descriptor_2 		ep2;
	S_usb_endpoint_audio_specific_2			ep2_s;
}
ATTR_PACKED S_usb_user_configuration_descriptor;

// usb_user_configuration_descriptor HS
const S_usb_user_configuration_descriptor uac2_usb_conf_desc_hs =
{
	STRUCT(S_usb_configuration_descriptor,cfg)
	{	
		KV(Size,sizeof(S_usb_configuration_descriptor)),
		KV(Type,DTYPE_Configuration),
		KV(TotalConfigurationSize,sizeof(S_usb_user_configuration_descriptor)),
		KV(TotalInterfaces,2),
		KV(ConfigurationNumber,1),
		KV(StringIndex,NO_DESCRIPTOR),
		KV(ConfigAttributes,USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),
		KV(MaxPower,USB_CONFIG_POWER_MA(200))
	},
	STRUCT(S_usb_interface_descriptor,ifc0)
	{
		KV(Size,sizeof(S_usb_interface_descriptor)),
		KV(Type,DTYPE_Interface),
		KV(InterfaceNumber,0),
		KV(AlternateSetting,0),
		KV(TotalEndpoints,0),
		INTERFACE_CLASS0,
		INTERFACE_SUB_CLASS0,
		INTERFACE_PROTOCOL0,
		INTERFACE_INDEX0
	},

	//! Here is where Audio Class 2 specific stuff is
	STRUCT(S_usb_interface_association_descriptor,iad1)
	{ 
		KV(Size,sizeof(S_usb_interface_association_descriptor)),
		KV(Type,DTYPE_InterfaceAssociation),
		KV(FirstInterfaceIndex,FIRST_INTERFACE1),
		KV(TotalInterfaces,INTERFACE_COUNT1),
		KV(Class,FUNCTION_CLASS),
		KV(SubClass,FUNCTION_SUB_CLASS),
		KV(Protocol,FUNCTION_PROTOCOL),
		KV(StringIndex,FUNCTION_INDEX)
	},

	STRUCT(S_usb_interface_descriptor,ifc1)
	{
		KV(Size,sizeof(S_usb_interface_descriptor)),
		KV(Type,DTYPE_Interface),
		INTERFACE_NB1,
		ALTERNATE_NB1,
		NB_ENDPOINT1,
		INTERFACE_CLASS1,
		INTERFACE_SUB_CLASS1,
		INTERFACE_PROTOCOL1,
		INTERFACE_INDEX1
	},
	STRUCT(InterfaceCollectionDescriptor,ifcd)
	{
		STRUCT(S_usb_ac_interface_descriptor_2,audioac)
		{  sizeof(S_usb_ac_interface_descriptor_2)
			,  CS_INTERFACE
			,  HEADER_SUB_TYPE
			,  Usb_format_mcu_to_usb_data(16, AUDIO_CLASS_REVISION_2)
			,  HEADSET_CATEGORY
			,  Usb_format_mcu_to_usb_data(16,sizeof(InterfaceCollectionDescriptor))
			,  MIC_LATENCY_CONTROL
		},
		STRUCT(S_usb_clock_source_descriptor,audio_cs1)
		{  
			sizeof (S_usb_clock_source_descriptor),
			CS_INTERFACE,
			DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SOURCE,
			CSD_ID_1,
			CSD_ID_1_TYPE,
			CSD_ID_1_CONTROL,
			OUTPUT_TERMINAL_ID,
			CLOCK_SOURCE_1_INDEX,
		},
		STRUCT(S_usb_clock_source_descriptor,audio_cs2)
		{  
			sizeof (S_usb_clock_source_descriptor),
			CS_INTERFACE,
			DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SOURCE,
			CSD_ID_2,
			CSD_ID_2_TYPE,
			CSD_ID_2_CONTROL,
			OUTPUT_TERMINAL_ID,
			CLOCK_SOURCE_2_INDEX,
		},
		STRUCT(S_usb_clock_selector_descriptor,audio_csel)
		{  
			sizeof (S_usb_clock_selector_descriptor),
			CS_INTERFACE,
			DESCRIPTOR_SUBTYPE_AUDIO_AC_CLOCK_SELECTOR,
			CSX_ID,
			CSX_INPUT_PINS,
			CSX_SOURCE_1,
			CSX_SOURCE_2,
			CSX_CONTROL,
			0,
		},
		STRUCT(S_usb_in_ter_descriptor_2,mic_in_ter)
		{  
			sizeof(S_usb_in_ter_descriptor_2),
			CS_INTERFACE,
			INPUT_TERMINAL_SUB_TYPE,
			INPUT_TERMINAL_ID,
			Usb_format_mcu_to_usb_data(16, INPUT_TERMINAL_TYPE),
			INPUT_TERMINAL_ASSOCIATION,
			CSX_ID,
			INPUT_TERMINAL_NB_CHANNELS,
			Usb_format_mcu_to_usb_data(32, INPUT_TERMINAL_CHANNEL_CONF),
			INPUT_TERMINAL_CH_NAME_ID,
			Usb_format_mcu_to_usb_data(16, INPUT_TERMINAL_CONTROLS),
			AIT_INDEX
		},
		STRUCT(S_usb_feature_unit_descriptor_2,mic_fea_unit)
		{  
			sizeof(S_usb_feature_unit_descriptor_2),
			CS_INTERFACE,
			FEATURE_UNIT_SUB_TYPE,
			MIC_FEATURE_UNIT_ID,
			MIC_FEATURE_UNIT_SOURCE_ID,
			Usb_format_mcu_to_usb_data(32, MIC_BMA_CONTROLS),
			Usb_format_mcu_to_usb_data(32, MIC_BMA_CONTROLS_CH_1),
			Usb_format_mcu_to_usb_data(32, MIC_BMA_CONTROLS_CH_2),
			0x00   //iFeature
		},
		STRUCT(S_usb_out_ter_descriptor_2,mic_out_ter)
		{  
			sizeof(S_usb_out_ter_descriptor_2)
			,  CS_INTERFACE
			,  OUTPUT_TERMINAL_SUB_TYPE
			,  OUTPUT_TERMINAL_ID
			,  Usb_format_mcu_to_usb_data(16, OUTPUT_TERMINAL_TYPE)
			,  OUTPUT_TERMINAL_ASSOCIATION
			,  OUTPUT_TERMINAL_SOURCE_ID
			,  CSX_ID
			,  Usb_format_mcu_to_usb_data(16, OUTPUT_TERMINAL_CONTROLS)
			, AOT_INDEX
		},
			{  sizeof(S_usb_in_ter_descriptor_2)
			,  CS_INTERFACE
			,  INPUT_TERMINAL_SUB_TYPE
			,  SPK_INPUT_TERMINAL_ID
			,  Usb_format_mcu_to_usb_data(16, SPK_INPUT_TERMINAL_TYPE)
			,  SPK_INPUT_TERMINAL_ASSOCIATION
			,  CSX_ID
			,  SPK_INPUT_TERMINAL_NB_CHANNELS
			,  Usb_format_mcu_to_usb_data(32, SPK_INPUT_TERMINAL_CHANNEL_CONF)
			,  INPUT_TERMINAL_CH_NAME_ID
			,  Usb_format_mcu_to_usb_data(16, INPUT_TERMINAL_CONTROLS)
			,  INPUT_TERMINAL_STRING_DESC
			}
			,
			{  sizeof(S_usb_feature_unit_descriptor_2)
			,  CS_INTERFACE
			,  FEATURE_UNIT_SUB_TYPE
			,  SPK_FEATURE_UNIT_ID
			,  SPK_FEATURE_UNIT_SOURCE_ID
			,  Usb_format_mcu_to_usb_data(32, SPK_BMA_CONTROLS)
			,  Usb_format_mcu_to_usb_data(32, SPK_BMA_CONTROLS_CH_1)
			,  Usb_format_mcu_to_usb_data(32, SPK_BMA_CONTROLS_CH_2)
			,  0x00
			}
			,
			{  sizeof(S_usb_out_ter_descriptor_2)
			,  CS_INTERFACE
			,  OUTPUT_TERMINAL_SUB_TYPE
			,  SPK_OUTPUT_TERMINAL_ID
			,  Usb_format_mcu_to_usb_data(16, SPK_OUTPUT_TERMINAL_TYPE)
			,  SPK_OUTPUT_TERMINAL_ASSOCIATION
			,  SPK_OUTPUT_TERMINAL_SOURCE_ID
			,  CSX_ID
			,  Usb_format_mcu_to_usb_data(16,SPK_OUTPUT_TERMINAL_CONTROLS)
			,  0x00
			},
		},
		//
		{  sizeof(S_usb_as_interface_descriptor)
		,  INTERFACE_DESCRIPTOR
		,  STD_AS_INTERFACE_IN
		,  ALT0_AS_INTERFACE_INDEX
		,  ALT0_AS_NB_ENDPOINT
		,  ALT0_AS_INTERFACE_CLASS
		,  ALT0_AS_INTERFACE_SUB_CLASS
		,  ALT0_AS_INTERFACE_PROTOCOL
		, AIN_INDEX
		},
		//
		{  sizeof(S_usb_as_interface_descriptor)
		,  INTERFACE_DESCRIPTOR
		,  STD_AS_INTERFACE_IN
		,  ALT1_AS_INTERFACE_INDEX
		,  ALT1_AS_NB_ENDPOINT
		,  ALT1_AS_INTERFACE_CLASS
		,  ALT1_AS_INTERFACE_SUB_CLASS
		,  ALT1_AS_INTERFACE_PROTOCOL
		, AIA_INDEX
		},
		//
		{  sizeof(S_usb_as_g_interface_descriptor_2)
		,  CS_INTERFACE
		,  GENERAL_SUB_TYPE
		,  AS_TERMINAL_LINK
		,  AS_CONTROLS
		,  AS_FORMAT_TYPE
		,  Usb_format_mcu_to_usb_data(32, AS_FORMATS)
		,  AS_NB_CHANNELS
		,  Usb_format_mcu_to_usb_data(32, AS_CHAN_CONFIG)
		},
		//
		{  sizeof(S_usb_format_type_2)
		,  CS_INTERFACE
		,  FORMAT_SUB_TYPE
		,  FORMAT_TYPE_1
		,  FORMAT_SUBSLOT_SIZE_1
		,  FORMAT_BIT_RESOLUTION_1
		},
		//
		{   sizeof(S_usb_endpoint_audio_descriptor_2)
		,   ENDPOINT_DESCRIPTOR
		,   ENDPOINT_NB_1
		,   EP_ATTRIBUTES_1
		,   Usb_format_mcu_to_usb_data(16, EP_SIZE_1_HS)
		,   EP_INTERVAL_1_HS
		},
		//
		{  sizeof(S_usb_endpoint_audio_specific_2)
		,  CS_ENDPOINT
		,  GENERAL_SUB_TYPE
		,  AUDIO_EP_ATRIBUTES
		,  AUDIO_EP_CONTROLS
		,  AUDIO_EP_DELAY_UNIT
		,  Usb_format_mcu_to_usb_data(16, AUDIO_EP_LOCK_DELAY)
		},
		//
		{  sizeof(S_usb_as_interface_descriptor)
		,  INTERFACE_DESCRIPTOR
		,  STD_AS_INTERFACE_OUT
		,  ALT0_AS_INTERFACE_INDEX
		,  ALT0_AS_NB_ENDPOINT
		,  ALT0_AS_INTERFACE_CLASS
		,  ALT0_AS_INTERFACE_SUB_CLASS
		,  ALT0_AS_INTERFACE_PROTOCOL
		,  0x00
		},
		//
		{  sizeof(S_usb_as_interface_descriptor)
		,  INTERFACE_DESCRIPTOR
		,  STD_AS_INTERFACE_OUT
		,  ALT1_AS_INTERFACE_INDEX
		,  ALT1_AS_NB_ENDPOINT_OUT
		,  ALT1_AS_INTERFACE_CLASS
		,  ALT1_AS_INTERFACE_SUB_CLASS
		,  ALT1_AS_INTERFACE_PROTOCOL
		,  0x00
		},
		//
		{  sizeof(S_usb_as_g_interface_descriptor_2)
		,  CS_INTERFACE
		,  GENERAL_SUB_TYPE
		,  SPK_INPUT_TERMINAL_ID
		,  AS_CONTROLS
		,  AS_FORMAT_TYPE
		,  Usb_format_mcu_to_usb_data(32, AS_FORMATS)
		,  AS_NB_CHANNELS
		,  Usb_format_mcu_to_usb_data(32,AS_CHAN_CONFIG)
		,  0x00
		},
		//
		{  sizeof(S_usb_format_type_2)
		,  CS_INTERFACE
		,  FORMAT_SUB_TYPE
		,  FORMAT_TYPE_1
		,  FORMAT_SUBSLOT_SIZE_1
		,  FORMAT_BIT_RESOLUTION_1
		},
		//
		{   sizeof(S_usb_endpoint_audio_descriptor_2)
		,   ENDPOINT_DESCRIPTOR
		,   ENDPOINT_NB_2
		,   EP_ATTRIBUTES_2
		,   Usb_format_mcu_to_usb_data(16, EP_SIZE_2_HS)
		,   EP_INTERVAL_2_HS
		},
		//
		{  
			KV(Size,sizeof(S_usb_endpoint_audio_specific_2)),
			KV(Type,DTYPE_CSEndpoint),
			KV(SubType,AUDIO_DSUBTYPE_CSEndpoint_General),
			KV(Attributes,AUDIO_EP_ATRIBUTES),
			KV(LockDelayUnits,ePCMSamples),
			KV(LockDelay,2)
		}
};

const int struct_size = sizeof(S_usb_user_configuration_descriptor);

int GetConfigStructSize() 
{
	return sizeof(S_usb_user_configuration_descriptor);
}

void* GetConfigStruct() 
{
	return (void*) &uac2_usb_conf_desc_hs;
}
