/*
* LCD Display Library for the 8448 LCD Display (Nokia 5110 display)
* Base		: Lauren from DFRobot / version 0.3
* Date      : 06.01.2012
* E-mail    : Lauran.pan@gmail.com

* Modified	: Dominik Schubert
* Date      : 06.01.2013

* Modified	: Bastian Geier
* Date      : 30.12.2023
* E-mail	: mail@bastiangeier.de
* Repo      : https://github.com/RedCali/LCD8448.git

* Any suggestions are welcome.
*/
#pragma once
#ifndef LCD8448_CONFIG_H_
#define LCD8448_CONFIG_H_

// Commend / uncomment the define for Debugging
#define LCD_DEBUG

// Define if the Arduino LCD Shield is used
//#define LCD_SHIELD

// LCD PIN definition
#if defined(ARDUINO) && (ARDUINO >= 100) // SPI Interface --- (using Arduino Digital Pins)
#ifdef LCD_SHIELD
// PIN Definitions for SPI Interface --- using Arduino NOKIA LCD shield
#define LCD_BL   7 // Backlight control (Arduino DIO Pin 7)
#define SPI_SCK  2 // Serial Clock(Master Output)
#define SPI_MOSI 3 // Master Output,Slave Input
#define LCD_DC   4 // Data/Command(command active low)
#define SPI_CS   5 // Chip Select,Slave Transmit Enable(active low,Master Output)
#define LCD_RST  6 // Reset & Reset Button
#else
#define LCD_BL   7 // Back light control
#define SPI_SCK  6 // CLK - Serial Clock(Master Output)
#define SPI_MOSI 5 // DI - Master Output,Slave Input
#define LCD_DC   4 // DC - Data/Command(command active low)
#define SPI_CS   3 // CE - Chip Select,Slave Transmit Enable(active low,Master Output)
#define LCD_RST  2 // Reset
#endif // ARDUINO && (ARDUINO >= 100) // SPI Interface --- (using Arduino Digital Pins)

#else
#define LCD_PORT PORTB
#define LCD_DDR DDRB
#define LCD_RST PINB0   // RST	Reset
#define SPI_CS PINB1    // CE	Chip Select,Slave Transmit Enable(active low,Master Output)
#define LCD_DC PINB2    // DC	Data/Command	(command active low)
#define SPI_MOSI PINB3  // Din	Master Output,	Slave Input
// #define	SPI_MISO	PINB4		// nc	Master Input,	Slave Output
#define SPI_SCK PINB5   // CLK	Serial Clock(Master Output)

// Back light definition
#define LCD_BL_PORT PORTD
#define LCD_BL_DDR DDRD
#define LCD_BL PIND7  // Back light control
#endif

#endif /* LCD8448_CONFIG_H_ */
