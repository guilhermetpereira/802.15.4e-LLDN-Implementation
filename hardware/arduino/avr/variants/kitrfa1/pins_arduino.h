/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

/*
	This version of pins_arduino.h is for the WM-100-Duino
	Gerson Fernando Budke 2014 Abr 14
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// PORTLIST	Functions
// -------------------------------------------	
// PORT	   ID	1° Function				Digital	PWM		PCINT
// PD 0 ** 0 **	I2C_SCL					D0		
// PD 1 ** 1 ** I2C_SDA					D1
// PD 2 ** 2 ** USART1_RX				D2
// PD 3 ** 3 **	USART1_TX				D3
// PD 4 ** 4 **	(I2C_EVENT MCP)			D4
// PD 5 ** 5 **	(RS-485 DE)				D5
// PD 6 ** 6 **	(RS-485 #RE)			D6
// PD 7 ** 7 **							D7

// PB 0 ** 8 **  (SPI_SS FLASH #SEL)	D8				PCINT
// PB 1 ** 9 **  SPI_SCK				D9				PCINT
// PB 2 ** 10 ** SPI_MOSI				D10				PCINT
// PB 3 ** 11 ** SPI_MISO				D11				PCINT
// PB 4 ** 12 **						D12		PWM		PCINT
// PB 5 ** 13 **						D13		PWM		PCINT
// PB 6 ** 14 **						D14		PWM		PCINT
// PB 7 ** 15 **						D15		PWM		PCINT

// PE 0 ** 16 ** USART2_RX				D16
// PE 1 ** 17 ** USART2_TX				D17
// PE 2 ** 18 ** 						D18				
// PE 3 ** 19 **						D19		PWM		
// PE 4 ** 20 **						D20		PWM		
// PE 5 ** 21 **						D21		PWM		
// PE 6 ** 22 **						D22				
// PE 7 ** 23 **						D23				

// PF 0 ** 24 ** A0						D24
// PF 1 ** 25 ** A1						D25
// PF 2 ** 26 ** A5						D26
// PF 3 ** 27 ** A7						D27
// PF 4 ** 28 ** A4						D28
// PF 5 ** 29 ** A6						D29
// PF 6 ** 30 ** A7						D30
// PF 7 ** 31 ** A8						D31

// PG 0 ** 32 ** LED0					D32
// PG 1 ** 33 ** LED1					D33
// PG 5 ** 34 ** LED2					D34		PWM
// PG 2 ** 35 ** SWITCH0				D35

#define NUM_DIGITAL_PINS				32
#define NUM_ANALOG_INPUTS				8
#define analogInputToDigitalPin(p)		(((p) < 8) ? (p) + 24 : -1)
#define digitalPinHasPWM(p)				((p) == 12 || (p) == 13 || (p) == 14 || (p) == 15 || \
										 (p) == 19 || (p) == 20 || (p) == 21 )

const static uint8_t SS					= 8;
const static uint8_t MOSI				= 10;
const static uint8_t MISO				= 11;
const static uint8_t SCK				= 9;

const static uint8_t A0					= 24;
const static uint8_t A1					= 25;
const static uint8_t A2					= 26;
const static uint8_t A3					= 27;
const static uint8_t A4					= 28;
const static uint8_t A5					= 29;
const static uint8_t A6					= 30;
const static uint8_t A7					= 31;

const static uint8_t SDA				= 0;
const static uint8_t SCL				= 1;
const static uint8_t LED0				= 32;
const static uint8_t LED1				= 33;
const static uint8_t LED2				= 34;

// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):

#define digitalPinToPCICR(p)			( (((p) >= 8) && ((p) <= 15)) ? (&PCICR) : ((uint8_t *) 0) )
#define digitalPinToPCICRbit(p)			( 0 )
#define digitalPinToPCMSK(p)			( (((p) >= 8) && ((p) <= 15)) ? (&PCMSK0) : ((uint8_t *) 0) )
#define digitalPinToPCMSKbit(p)			( (((p) >= 8) && ((p) <= 15)) ? (p) - 8 : 0 )

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&DDRB,
	NOT_A_PORT,
	(uint16_t)&DDRD,
	(uint16_t)&DDRE,
	(uint16_t)&DDRF,
	(uint16_t)&DDRG,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PORTB,
	NOT_A_PORT,
	(uint16_t)&PORTD,
	(uint16_t)&PORTE,
	(uint16_t)&PORTF,
	(uint16_t)&PORTG,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PIN,
	NOT_A_PIN,
	(uint16_t)&PINB,
	NOT_A_PIN,
	(uint16_t)&PIND,
	(uint16_t)&PINE,
	(uint16_t)&PINF,
	(uint16_t)&PING,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
	NOT_A_PIN,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	// PORTLIST		
	// -------------------------------------------		
	PD	, // PD 0 ** 0 **  I2C_SCL
	PD	, // PD 1 ** 1 **  I2C_SDA
	PD	, // PD 2 ** 2 **  USART1_RX	
	PD	, // PD 3 ** 3 **  USART1_TX	
	PD	, // PD 4 ** 4 ** 
	PD	, // PD 5 ** 5 ** 
	PD	, // PD 6 ** 6 ** 
	PD	, // PD 7 ** 7 ** 
	
	PB	, // PB 0 ** 8 **  SPI_SS
	PB	, // PB 1 ** 9 **  SPI_SCK
	PB	, // PB 2 ** 10 ** SPI_MOSI
	PB	, // PB 3 ** 11 ** SPI_MISO
	PB	, // PB 4 ** 12 ** 
	PB	, // PB 5 ** 13 ** 
	PB	, // PB 6 ** 14 ** 
	PB	, // PB 7 ** 15 ** 

	PE	, // PE 0 ** 16 ** USART2_RX
	PE	, // PE 1 ** 17 ** USART2_TX
	PE	, // PE 2 ** 18 ** 
	PE	, // PE 3 ** 19 ** 
	PE	, // PE 4 ** 20 ** 
	PE	, // PE 5 ** 21 ** 
	PE	, // PE 6 ** 22 ** 
	PE	, // PE 7 ** 23 ** 

	PF	, // PF 0 ** 24 ** A0
	PF	, // PF 1 ** 25 ** A1
	PF	, // PF 2 ** 26 ** A2
	PF	, // PF 3 ** 27 ** A3
	PF	, // PF 4 ** 28 ** A4
	PF	, // PF 5 ** 29 ** A5
	PF	, // PF 6 ** 30 ** A6
	PF	, // PF 7 ** 31 ** A7

	PG  , // PG 0 ** 32 ** LED_0
	PG  , // PG 1 ** 33 ** LED_1
	PG	, // PG 5 ** 34 ** LED_2
	PG	, // PG 2 ** 35 ** SWITCH_0
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PD 0 ** 0 **  I2C_SCL
	_BV( 1 )	, // PD 1 ** 1 **  I2C_SDA
	_BV( 2 )	, // PD 2 ** 2 **  USART1_RX
	_BV( 3 )	, // PD 3 ** 3 **  USART1_TX
	_BV( 4 )	, // PD 4 ** 4 **
	_BV( 5 )	, // PD 5 ** 5 **
	_BV( 6 )	, // PD 6 ** 6 **
	_BV( 7 )	, // PD 7 ** 7 **
	
	_BV( 0 )	, // PB 0 ** 8 **  SPI_SS
	_BV( 1 )	, // PB 1 ** 9 **  SPI_SCK
	_BV( 2 )	, // PB 2 ** 10 ** SPI_MOSI
	_BV( 3 )	, // PB 3 ** 11 ** SPI_MISO
	_BV( 4 )	, // PB 4 ** 12 **
	_BV( 5 )	, // PB 5 ** 13 **
	_BV( 6 )	, // PB 6 ** 14 **
	_BV( 7 )	, // PB 7 ** 15 **

	_BV( 0 )	, // PE 0 ** 16 ** USART2_RX
	_BV( 1 )	, // PE 1 ** 17 ** USART2_TX
	_BV( 2 )	, // PE 2 ** 18 **
	_BV( 3 )	, // PE 3 ** 19 **
	_BV( 4 )	, // PE 4 ** 20 **
	_BV( 5 )	, // PE 5 ** 21 **
	_BV( 6 )	, // PE 6 ** 22 **
	_BV( 7 )	, // PE 7 ** 23 **

	_BV( 0 )	, // PF 0 ** 24 ** A0
	_BV( 1 )	, // PF 1 ** 25 ** A1
	_BV( 2 )	, // PF 2 ** 26 ** A2
	_BV( 3 )	, // PF 3 ** 27 ** A3
	_BV( 4 )	, // PF 4 ** 28 ** A4
	_BV( 5 )	, // PF 5 ** 29 ** A5
	_BV( 6 )	, // PF 6 ** 30 ** A6
	_BV( 7 )	, // PF 7 ** 31 ** A7

	_BV( 0 )	, // PG 0 ** 32 ** LED_0
	_BV( 1 )	, // PG 1 ** 33 ** LED_1
	_BV( 5 )	, // PG 5 ** 34 ** LED_2
	_BV( 2 )	, // PG 2 ** 35 ** SWITCH_0
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	// TIMERS		
	// -------------------------------------------
	NOT_ON_TIMER	, // PD 0 ** 0 **  I2C_SCL
	NOT_ON_TIMER	, // PD 1 ** 1 **  I2C_SDA
	NOT_ON_TIMER	, // PD 2 ** 2 **  USART1_RX
	NOT_ON_TIMER	, // PD 3 ** 3 **  USART1_TX
	NOT_ON_TIMER	, // PD 4 ** 4 **
	NOT_ON_TIMER	, // PD 5 ** 5 **
	NOT_ON_TIMER	, // PD 6 ** 6 **
	NOT_ON_TIMER	, // PD 7 ** 7 **
	
	NOT_ON_TIMER	, // PB 0 ** 8 **  SPI_SS
	NOT_ON_TIMER	, // PB 1 ** 9 **  SPI_SCK
	NOT_ON_TIMER	, // PB 2 ** 10 ** SPI_MOSI
	NOT_ON_TIMER	, // PB 3 ** 11 ** SPI_MISO
	TIMER2A			, // PB 4 ** 12 **
	TIMER1A			, // PB 5 ** 13 **
	TIMER1B			, // PB 6 ** 14 **
	TIMER0A			, // PB 7 ** 15 **

	NOT_ON_TIMER	, // PE 0 ** 16 ** USART2_RX
	NOT_ON_TIMER	, // PE 1 ** 17 ** USART2_TX
	NOT_ON_TIMER	, // PE 2 ** 18 **
	TIMER3A			, // PE 3 ** 19 **
	TIMER3B			, // PE 4 ** 20 **
	TIMER3C			, // PE 5 ** 21 **
	NOT_ON_TIMER	, // PE 6 ** 22 **
	NOT_ON_TIMER	, // PE 7 ** 23 **

	NOT_ON_TIMER	, // PF 0 ** 24 ** A0
	NOT_ON_TIMER	, // PF 1 ** 25 ** A1
	NOT_ON_TIMER	, // PF 2 ** 26 ** A2
	NOT_ON_TIMER	, // PF 3 ** 27 ** A3
	NOT_ON_TIMER	, // PF 4 ** 28 ** A4
	NOT_ON_TIMER	, // PF 5 ** 29 ** A5
	NOT_ON_TIMER	, // PF 6 ** 30 ** A6
	NOT_ON_TIMER	, // PF 7 ** 31 ** A7

	NOT_ON_TIMER	, // PG 0 ** 32 ** LED_0
	NOT_ON_TIMER	, // PG 1 ** 33 ** LED_1
	TIMER0B			, // PG 5 ** 34 ** LED_2
	NOT_ON_TIMER	, // PG 2 ** 35 ** SWITCH_0
};	

#endif

#endif