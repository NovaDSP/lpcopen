/*
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
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
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "string.h"

#include "lpc_phy_smsc87x0.c"
#include "retarget.c"

/** @ingroup BOARD_NGX_XPLORER_18304330
 * @{
 */

#ifdef _USE_4357
#define LED4_BIT 18  
#define LED4_PORT 5
#define LED5_BIT 11 
#define LED5_PORT 4
#define PIN_INT_PORT			4   
#define PIN_INT_BIT				0
#else
#define LED2_BIT			11
#define LED2_PORT			1
#define LED3_BIT			12
#define LED3_PORT			1
#endif

void Board_UART_Init(LPC_USART_T *pUART)
{
#ifdef _USE_4357
	// Initialize UART0 pin connect	Xplorer++ board
	Chip_SCU_PinMux(0xF ,10 , MD_PDN|MD_EZI, FUNC1); 	// UART0_TXD
	Chip_SCU_PinMux(0xF ,11 , MD_PDN|MD_EZI, FUNC1); 	// UART0_RXD
#else	
	if (pUART == LPC_USART0) {
		Chip_SCU_PinMuxSet(0x6, 4, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC2));					/* P6.5 : UART0_TXD */
		Chip_SCU_PinMuxSet(0x6, 5, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));/* P6.4 : UART0_RXD */
	}
	else if (pUART == LPC_UART1) {
		Chip_SCU_PinMuxSet(0x1, 13, (SCU_MODE_MODE_REPEATER | SCU_MODE_FUNC2));				/* P1.13 : UART1_TXD */
		Chip_SCU_PinMuxSet(0x1, 14, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));	/* P1.14 : UART1_RX */
	}
#endif	
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaud(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_DATABIT_8, UART_PARITY_NONE, UART_STOPBIT_1);

	/* Enable UART Transmit */
	Chip_UART_TxCmd(DEBUG_UART, ENABLE);
#endif
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	while (Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch) == ERROR) 
	{
		// NOP
	}
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	uint8_t data;

	if (Chip_UART_ReceiveByte(DEBUG_UART, &data) == SUCCESS) {
		return (int) data;
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(const char* str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}

static void Board_LED_Init()
{
#ifdef _USE_4357

	// User LED 4
	Chip_SCU_PinMux(0x9 ,5 , MD_PDN, SCU_MODE_FUNC4); 					
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT,LED4_PORT,LED4_BIT, true); 			

	// User LED 5
	Chip_SCU_PinMux(0x9 ,6 , MD_PDN, SCU_MODE_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT,LED5_PORT,LED5_BIT, true); 

	/* P2.12 : LED D2 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED4_PORT, LED4_BIT, true);
	/* P2.11 : LED D3 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED5_PORT, LED5_BIT, true);
	/* Set initial states to off (true to disable) */
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED4_PORT, LED4_BIT, (bool) true);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED5_PORT, LED5_BIT, (bool) true);
#else
	/* P2.12 : LED D2 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED2_PORT, LED2_BIT, true);
	/* P2.11 : LED D3 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED3_PORT, LED3_BIT, true);
	/* Set initial states to off (true to disable) */
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED2_PORT, LED2_BIT, (bool) true);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED3_PORT, LED3_BIT, (bool) true);
#endif	
}

void Board_LED_Set(uint8_t LEDNumber, bool On)
{
#ifdef _USE_4357
	if (LEDNumber == 0) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED4_PORT, LED4_BIT, (bool) !On);
	}
	else if (LEDNumber == 1) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED5_PORT, LED5_BIT, (bool) !On);
	}
#else
	if (LEDNumber == 0) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED2_PORT, LED2_BIT, (bool) !On);
	}
	else if (LEDNumber == 1) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED3_PORT, LED3_BIT, (bool) !On);
	}
#endif	
}

bool Board_LED_Test(uint8_t LEDNumber)
{
#ifdef _USE_4357
	if (LEDNumber == 0) {
		return (bool) !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, LED4_PORT, LED4_BIT);
	}
	else if (LEDNumber == 1) {
		return (bool) !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, LED5_PORT, LED5_BIT);
	}
#else
	if (LEDNumber == 0) {
		return (bool) !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, LED2_PORT, LED2_BIT);
	}
	else if (LEDNumber == 1) {
		return (bool) !Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, LED3_PORT, LED3_BIT);
	}
#endif
	return false;
}

//-----------------------------------------------------------------------------
void Board_Buttons_Init()
{
#ifdef _USE_4357
	// JME RA please check this makes sense?
	Chip_SCU_PinMux(0x8 ,0 ,(MD_PLN|MD_EZI|MD_ZI), SCU_MODE_FUNC0);		
	// Set direction for GPIO port to input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT,PIN_INT_PORT, (1 << PIN_INT_BIT), false);	
#else
	Chip_SCU_PinMuxSet(0x2, 7, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));		// P2_7 as GPIO0[7]
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, (1 << BUTTONS_BUTTON1_GPIO_BIT_NUM), false);	// input
#endif		
}

//-----------------------------------------------------------------------------
uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
#ifdef _USE_4357
	// JME RA please check this makes sense?
	if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, PIN_INT_PORT, PIN_INT_BIT) == 0) 
	{
		ret |= BUTTONS_BUTTON1;
	}
#else	
	if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0) 
	{
		ret |= BUTTONS_BUTTON1;
	}
#endif	
	return ret;
}

//-----------------------------------------------------------------------------
void Board_Joystick_Init(void)
{

}

//-----------------------------------------------------------------------------
uint8_t Joystick_GetStatus(void)
{
	return NO_BUTTON_PRESSED;
}

//-----------------------------------------------------------------------------
/*!< System Clock Frequency (Core Clock)*/
uint32_t SystemCoreClock;

//-----------------------------------------------------------------------------
/* Update system core clock rate, should be called if the system has
   a clock rate change */
void SystemCoreClockUpdate(void)
{
	/* CPU core speed */
	SystemCoreClock = Chip_Clock_GetRate(CLK_MX_MXCORE);
}

//-----------------------------------------------------------------------------
/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	// JME audit: nasty !!!!!!!!
	uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

//-----------------------------------------------------------------------------
// Set up and initialize all required blocks and functions related to the
//   board hardware
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	// JME No. There are no clocks at this point ...
	// DEBUGINIT();

	/* Updates SystemCoreClock global var with current clock speed */
	SystemCoreClockUpdate();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Setup GPIOs for USB demos */
	Chip_SCU_PinMuxSet(0x2, 6, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC4));			/* P2_6 USB1_PWR_EN, USB1 VBus function */
	Chip_SCU_PinMuxSet(0x2, 5, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));	/* P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION */
	Chip_SCU_PinMuxSet(0x1, 7, (SCU_MODE_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC4));			/* P1_7 USB0_PWR_EN, USB0 VBus function Xplorer */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 5, 6, true);							/* GPIO5[6] = USB1_PWR_EN */
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 5, 6, true);							/* GPIO5[6] output high */

	/* Initialize LEDs */
	Board_LED_Init();

	// Sets up DEBUG UART  but after all main clocks are set!
	DEBUGINIT();

}

//-----------------------------------------------------------------------------
void Board_I2C_Init(I2C_ID_T id)
{
	if (id == I2C1) {
		/* Configure pin function for I2C1*/
		Chip_SCU_PinMuxSet(0x2, 3, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));		/* P2.3 : I2C1_SDA */
		Chip_SCU_PinMuxSet(0x2, 4, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));		/* P2.4 : I2C1_SCL */
	}
	else {
		Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);
	}
}

#ifndef CORE_M0
/* PIN0 Interrupt not available in M0 core */
void GPIO0_IRQHandler(void)
{
	static bool On;

	if (Chip_GPIO_IntGetStatus(LPC_GPIO_PIN_INT, 0, 0, 0)) {
		Chip_GPIO_IntClear(LPC_GPIO_PIN_INT, 0, 0);
		On = (bool) !On;
		Board_LED_Set(1, On);
	}
}

void Board_GPIO_Int_Init()
{
	Chip_SCU_PinMuxSet(0xF, 9, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));	/* PF.9 : POTI button */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 7, 23, false);						/* PF.9 -> GPIO7[23] : input */
	Chip_SCU_GPIOIntPinSel(0, 7, 23);
	Chip_GPIO_IntCmd(LPC_GPIO_PIN_INT, 0, 0, GPIOPININT_FALLING_EDGE);			/* Configure GPIO0[7] to interrupt pin (SW2 switch) */

	NVIC_EnableIRQ(PIN_INT0_IRQn);	/* enable GPIO interrupt 0 */
}

#endif

void Board_SDMMC_Init(void)
{
	Chip_SCU_PinMuxSet(0x1, 9, (SCU_PINIO_FAST | SCU_MODE_FUNC7));	/* P1.9 connected to SDIO_D0 */
	Chip_SCU_PinMuxSet(0x1, 10, (SCU_PINIO_FAST | SCU_MODE_FUNC7));	/* P1.10 connected to SDIO_D1 */
	Chip_SCU_PinMuxSet(0x1, 11, (SCU_PINIO_FAST | SCU_MODE_FUNC7));	/* P1.11 connected to SDIO_D2 */
	Chip_SCU_PinMuxSet(0x1, 12, (SCU_PINIO_FAST | SCU_MODE_FUNC7));	/* P1.12 connected to SDIO_D3 */

	Chip_SCU_ClockPinMuxSet(2, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC4));	/* CLK2 connected to SDIO_CLK */
	Chip_SCU_PinMuxSet(0x1, 6, (SCU_PINIO_FAST | SCU_MODE_FUNC7));	/* P1.6 connected to SDIO_CMD */
}

void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP1) {
		/* Set up clock and power for SSP1 module */
		/* Configure SSP1 pins*/
		//this is for 4330 xplorer board
		/* SCLK comes out pin CLK0 */
		//Chip_SCU_ClockPinMuxSet(0, (SCU_PINIO_FAST | SCU_MODE_FUNC6));		/* CLK0 connected to CLK	SCU_MODE_FUNC6=SSP1 CLK1  */
		//Chip_SCU_PinMuxSet(0x1, 5, (SCU_PINIO_FAST | SCU_MODE_FUNC5));			/* P1.5 connected to nCS	SCU_MODE_FUNC5=SSP1 SSEL1 */
		//Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5));/* P1.3 connected to SO		SCU_MODE_FUNC5=SSP1 MISO1 */
		//Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5));/* P1.4 connected to nSI	SCU_MODE_FUNC5=SSP1 MOSI1 */

		//this is for 4357 xplorer board
		//setup the Audio clock pin and clock source
		Chip_SCU_ClockPinMuxSet(1, (SCU_PINIO_FAST | SCU_MODE_FUNC5));			/* T10, CLK1 connected to CGU_OUT1  */
		
		//setup the SSP IO pins
		Chip_SCU_PinMuxSet(0xf, 4, (SCU_PINIO_FAST | SCU_MODE_FUNC0));			/* D10, Pf.4 connected to CLK	SCU_MODE_FUNC0=SSP1 CLK1  */
		Chip_SCU_PinMuxSet(0xf, 5, (SCU_PINIO_FAST | SCU_MODE_FUNC2));			/* E9, Pf.5 connected to nCS	SCU_MODE_FUNC5=SSP1 SSEL1 */
		Chip_SCU_PinMuxSet(0x1, 4, (SCU_PINIO_FAST | SCU_MODE_FUNC5));			/* T3, P1.4 connected to nSI	SCU_MODE_FUNC5=SSP1 MOSI1 */
		Chip_SCU_PinMuxSet(0xf, 6, (SCU_MODE_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));/* E7, Pf.6 connected to SO		SCU_MODE_FUNC2=SSP1 MISO1 */
	}
	else {
		return;
	}
}

static void delay(uint32_t i) {
	while (i--) {}
}

/* Initialize Audio Codec */
static Status Board_Audio_CodecInit(int micIn)
{
	/* Reset UDA1380 on board NGX Xplorer */
	Chip_SCU_PinMuxSet(0x2, 10, (SCU_MODE_MODE_INACT | SCU_MODE_FUNC0));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0, 14, true);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0, 14, true);
	// delay 1us
	delay(100000);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0, 14, false);
	delay(100000);

	if (!UDA1380_Init(UDA1380_MIC_IN_LR & - (micIn != 0))) {
		return ERROR;
	}

	return SUCCESS;
}

/* Board Audio initialization */
void Board_Audio_Init(LPC_I2S_T *pI2S, int micIn)
{
	Chip_I2S_Audio_Format_T I2S_Config;

	I2S_Config.SampleRate = 48000;
	I2S_Config.ChannelNumber = 2;		/* 1 is mono, 2 is stereo */
	I2S_Config.WordWidth =  16;			/* 8, 16 or 32 bits */
	Chip_I2S_Init(pI2S);
	Chip_I2S_Config(pI2S, I2S_TX_MODE, &I2S_Config);

	/* Init UDA1380 CODEC */
	while (Board_Audio_CodecInit(micIn) != SUCCESS) {}
}

/* FIXME Should we remove this function? */
void Serial_CreateStream(void *Stream)
{}

/**
 * @}
 */
