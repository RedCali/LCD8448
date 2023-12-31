/*
* LCD Display Library for the 8448 LCD Display (Nokia 5110 display)
* Base		: Lauren from DFRobot / version 0.3
* Date     : 06.01.2012
* E-mail   : Lauran.pan@gmail.com

* modified	: Dominik and Bastian
* E-mail	: mail@bastiangeier.de

* Any suggestions are welcome.
*/
#pragma once
#ifndef LCD8448_PIN_CONFIG_H_
#define LCD8448_PIN_CONFIG_H_

// LCD PIN definition
#if defined(ARDUINO) && ARDUINO >= 100  // SPI Interface --- (using Arduino Digital Pin 2,3,4,5,6)
#define LCD_BL 7                        // Back light control
#define SPI_SCK 6                       // CLK - Serial Clock(Master Output)
#define SPI_MOSI 5                      // DI - Master Output,Slave Input
#define LCD_DC 4                        // DC - Data/Command(command active low)
#define SPI_CS 3                        // CE - Chip Select,Slave Transmit Enable(active low,Master Output)
#define LCD_RST 2                       // Reset

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

#endif /* LCD8448_PIN_CONFIG_H_ */