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
#ifndef LCD8448_H_
#define LCD8448_H_

#define _LCD_VERSION "1.0.0"  // software version of this library

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

// LCD PIN definition

#if defined(ARDUINO) && ARDUINO >= 100  // SPI Interface --- (using Arduino Digital Pin 2,3,4,5,6)
#ifndef LCD_BL
#define LCD_BL 7  // Backlight control
#endif
#ifndef SPI_SCK
#define SPI_SCK 6  // CLK - Serial Clock(Master Output)
#endif
#ifndef SPI_MOSI
#define SPI_MOSI 5  // DI - Master Output,Slave Input
#endif
#ifndef LCD_DC
#define LCD_DC 4  // DC - Data/Command(command active low)
#endif
#ifndef SPI_CS
#define SPI_CS 3  // CE - Chip Select,Slave Transmit Enable(active low,Master Output)
#endif
#ifndef LCD_RST
#define LCD_RST 2  // Reset
#endif

#else
#define LCD_PORT PORTD
#define LCD_DDR DDRD
#define LCD_RST PIND6   // RST	Reset
#define SPI_CS PIND5    // CE	Chip Select,Slave Transmit Enable(active low,Master Output)
#define LCD_DC PIND4    // DC	Data/Command	(command active low)
#define SPI_MOSI PIND3  // Din	Master Output,	Slave Input
// #define	SPI_MISO	PINB4		// nc	Master Input,	Slave Output
#define SPI_SCK PIND2   // CLK	Serial Clock(Master Output)

// Back light definition
#define LCD_BL_PORT PORTD
#define LCD_BL_DDR DDRD
#define LCD_BL PIND7  // Backlight control
#endif

// constant definition
#define MENU_NORMAL 1
#define MENU_HIGHLIGHT 0
#define OFF 0
#define ON 1
#define COMMAND 0
#define DATA 1

class LCD8448 {
   private:
    bool backLightInverted;
    unsigned char virtuelldisp[504];
    unsigned char virtuelldisp_temp[504];

    inline void chipSelect() {
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(LCD_BL, LOW);
#else
        LCD_PORT &= ~(1 << SPI_CS);
#endif
    }
    inline void chipDeSelect() {
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(LCD_BL, HIGH);
#else
        LCD_PORT |= (1 << SPI_CS);
#endif
    }

    void writeCommand(uint8_t data)  // Sending Routine Master Mode (polling)
    {
#if defined(ARDUINO) && ARDUINO >= 100
        // Chip Enable: Active LOW
        digitalWrite(SPI_CS, LOW);
        // D/C=0:the current data byte is interpreted as command byte
        digitalWrite(LCD_DC, COMMAND);

        //
        for (uint8_t i = 0; i < 8; i++) {
            if (data & 0x80)  // 1000 0000
            {
                digitalWrite(SPI_MOSI, HIGH);  //
            } else {
                digitalWrite(SPI_MOSI, LOW);  //
            }
            digitalWrite(SPI_SCK, LOW);
            data = data << 1;
            digitalWrite(SPI_SCK, HIGH);  //
        }
        digitalWrite(SPI_CS, HIGH);
#else
        LCD_PORT &= ~(1 << SPI_CS);
        LCD_PORT &= ~(1 << LCD_DC);

        SPDR = data;  // send Character

        waitForTransmissionReady();
#endif
    }
    void writeData(uint8_t data)  // Sending Routine Master Mode (polling)
    {
#if defined(ARDUINO) && ARDUINO >= 100
        // Chip Enable: Active LOW
        digitalWrite(SPI_CS, LOW);
        // D/C=1:write data to display RAM
        digitalWrite(LCD_DC, DATA);

        //
        for (uint8_t i = 0; i < 8; i++) {
            if (data & 0x80)  // 1000 0000
            {
                digitalWrite(SPI_MOSI, HIGH);  //
            } else {
                digitalWrite(SPI_MOSI, LOW);  //
            }
            digitalWrite(SPI_SCK, LOW);
            data = data << 1;
            digitalWrite(SPI_SCK, HIGH);  //
        }
        digitalWrite(SPI_CS, HIGH);
#else
        LCD_PORT &= ~(1 << SPI_CS);
        LCD_PORT |= (1 << LCD_DC);

        SPDR = data;  // send Character

        waitForTransmissionReady();
#endif
    }

#if defined(ARDUINO) && ARDUINO >= 100

#else
    inline void waitForTransmissionReady(void) {
        while (!(SPSR & (1 << SPIF)))
            ;  // wait until Char is sent
        LCD_PORT |= (1 << SPI_CS);
    }
#endif

   public:
    LCD8448();
    ~LCD8448();
    void init(void) {
        init(false);
    }
    void init(bool blInverted) {
        backLightInverted = blInverted;

#if defined(ARDUINO) && ARDUINO >= 100
        pinMode(SPI_SCK, OUTPUT);
        digitalWrite(SPI_SCK, LOW);
        pinMode(SPI_MOSI, OUTPUT);
        digitalWrite(SPI_MOSI, LOW);
        pinMode(LCD_DC, OUTPUT);
        digitalWrite(LCD_DC, LOW);
        pinMode(SPI_CS, OUTPUT);
        digitalWrite(SPI_CS, LOW);
        pinMode(LCD_RST, OUTPUT);
        digitalWrite(LCD_RST, LOW);

        pinMode(LCD_BL, OUTPUT);
        digitalWrite(LCD_BL, LOW);

        digitalWrite(LCD_RST, LOW);
        delayMicroseconds(1);
        digitalWrite(LCD_RST, HIGH);

        digitalWrite(SPI_CS, LOW);  // Chip Select,Slave Transmit Enable(active low,Master Output)
        delayMicroseconds(1);
        digitalWrite(SPI_CS, HIGH);
        delayMicroseconds(1);

        setBacklightOFF();

#else
        LCD_BL_DDR |= (1 << LCD_BL);
        LCD_DDR |= (1 << LCD_RST) | (1 << LCD_DC);
        LCD_PORT &= ~(1 << LCD_RST);

        LCD_DDR |= (1 << SPI_CS) | (1 << SPI_MOSI) | (1 << SPI_SCK);

        SPCR = (1 << SPE) | (1 << MSTR);  //|(1<<SPR0);	// enable SPI in Master Mode with SCK = CK/4
        SPSR = (1 << SPI2X);
        // IOReg   = SPSR;                 			// clear SPIF bit in SPSR
        // SPSR
        // SPDR = 0;

        for (int i = 0; i < 1000; i++) {
        };

        LCD_PORT |= (1 << LCD_RST);
#endif

        // data_type=0, all are command bytes
        writeCommand(0x21);  // Function Set:0 0 1 0 0 PD V H=0010 0001;PD=0,V=0,H=1;
        writeCommand(0xc0);  // Set Vop:1 Vop6 Vop5 Vop4 Vop3 Vop2 Vop1 Vop0=1100 0000
        writeCommand(0x06);  // Set Temperature Coefficient:0 0 0 0 0 1 Tc1 Tc0=0000 0110;Tc1=1,Tc0=0(Vlcd temperature coefficient 2)
        writeCommand(0x13);  // Set Bias System (BSx):0 0 0 1 0 BS2 BS1 BS0=0001 0011;BS2=0, BS1=1, BS0=1==>N=4,MUX RATE=1:48

        writeCommand(0x20);  // Function Set:0 0 1 0 0 PD V H=0010 0000;PD=0,V=0,H=0;

        clear();
        vd_clear();

        writeCommand(0x0c);
        setBacklightOFF();
        chipDeSelect();
    }

    inline void off(void) {
        setBacklightOFF();
        clear();
    }

    inline void backlight(unsigned char dat) {
#if defined(ARDUINO) && ARDUINO >= 100
        if ((dat == 1) != (backLightInverted == true))
            digitalWrite(LCD_BL, HIGH);
        else
            digitalWrite(LCD_BL, LOW);
#else
        if ((dat == 1) != (backLightInverted == true))
            LCD_BL_PORT &= ~(1 << LCD_BL);
        else
            LCD_BL_PORT &= (1 << LCD_BL);
#endif
    }
    inline void setBacklightOFF(void) {
#if defined(ARDUINO) && ARDUINO >= 100
        if (backLightInverted == true)
            digitalWrite(LCD_BL, HIGH);
        else
            digitalWrite(LCD_BL, LOW);
#else
        if ((dat == 1) != (backLightInverted == true))
            LCD_BL_PORT &= ~(1 << LCD_BL);
        else
            LCD_BL_PORT |= (1 << LCD_BL);
#endif
    }
    inline void setBacklightON(void) {
#if defined(ARDUINO) && ARDUINO >= 100
        if (backLightInverted == true)
            digitalWrite(LCD_BL, LOW);
        else
            digitalWrite(LCD_BL, HIGH);
#else
        if ((dat == 1) != (backLightInverted == true))
            LCD_BL_PORT |= (1 << LCD_BL);
        else
            LCD_BL_PORT &= ~(1 << LCD_BL);
#endif
    }

    void set_XY(uint8_t X, uint8_t Y);
    void clear(void);
    void draw_bmp_pixel(uint8_t X, uint8_t Y, unsigned char *map);
    void draw_bmp_pixel(uint8_t X, uint8_t Y, unsigned char *map, uint8_t Pix_x, uint8_t Pix_y);
    void draw_bmp_pixel_P(uint8_t X, uint8_t Y, const unsigned char *map);
    void write_string(uint8_t X, uint8_t Y, char *str, uint8_t mode);
    void write_char(unsigned char c, uint8_t mode);
    void write_string_big(uint8_t X, uint8_t Y, char *string, uint8_t mode);
    void write_char_big(uint8_t X, uint8_t Y, unsigned char ch, uint8_t mode);

    /**************************************************************************************/
    void write_chinese(uint8_t X, uint8_t Y, unsigned char *c, uint8_t ch_with, uint8_t num, uint8_t line, uint8_t row);
    void prop_write_string(uint8_t X, uint8_t Y, char *s, uint8_t mode);
    unsigned char prop_write_char(char c, uint8_t mode);
    /*************************************************************************************/
    void write_number_big(uint8_t X, uint8_t Y, int zahl, uint8_t komma, uint8_t stellen, uint8_t mode);
    void write_number_big2(uint8_t X, uint8_t Y, uint8_t zahl);
    /*************************************************************************************/
    void vd_clear(void);
    void vd_print(void);
    void vd_set_pixel(uint8_t X0, uint8_t Y0);
    void vd_set_pixel_byte(uint8_t X0, uint8_t Y0, uint8_t c);
    void vd_set_pixel_byte_any(uint8_t X0, uint8_t Y0, uint8_t c);
    void vd_write_char(uint8_t X, uint8_t Y, char c, uint8_t mode);
    void vd_write_char_v(uint8_t X, uint8_t Y, char c, uint8_t mode);
    void vd_write_string(uint8_t X, uint8_t Y, char *s, uint8_t mode);
    void vd_write_string_v(uint8_t X, uint8_t Y, char *s, uint8_t mode);
    void vd_write_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);
    void vd_write_rect(uint8_t X0, uint8_t Y0, uint8_t a, uint8_t b);
    void vd_write_circle(uint8_t X0, uint8_t Y0, uint8_t radius);
    /*************************************************************************************/
    void vd_write_framework(char *head, uint8_t mode);
    void vd_alert(char *text);
    void vd_question(char *, uint8_t);
    void vd_overlayON(void);
    void vd_overlayOFF(void);
    void vd_battery(uint8_t X0, uint8_t Y0, uint8_t state, uint8_t mode);
};
extern LCD8448 lcd;

#endif /* LCD8448_H_ */