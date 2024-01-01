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

#include "LCD8448_Config.h"

#define LCD8448_VERSION "1.0.0"  // software version of this library

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
// #include "WProgram.h"
#include <stdlib.h>
#endif

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

// Constant definition
#define MENU_NORMAL 1
#define MENU_HIGHLIGHT 0
#define OFF 0
#define ON 1
#define COMMAND 0
#define DATA 1

class LCD8448 {
   private:
#pragma region PRIVATE
    unsigned char virtuelldisp[504];
    unsigned char virtuelldisp_temp[504];

#pragma region INTERNAL METHODS
    inline void chipSelect() {
        // Chip Select/Enable: Active LOW
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(SPI_CS, LOW);
#else
        LCD_PORT &= ~(1 << SPI_CS);
#endif
    }
    inline void chipDeSelect() {
        // Chip DeSelect/Disable: InActive HIGH
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(SPI_CS, HIGH);
#else
        LCD_PORT |= (1 << SPI_CS);
#endif
    }

    inline void chipWriteCommand() {
        // D/C=0:the current data byte is interpreted as command byte
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(LCD_DC, COMMAND);
#else
        LCD_PORT &= ~(1 << LCD_DC);
#endif
    }
    inline void chipWriteData() {
        // D/C=1:write data to display RAM
#if defined(ARDUINO) && ARDUINO >= 100
        digitalWrite(LCD_DC, DATA);
#else
        LCD_PORT |= (1 << LCD_DC);
#endif
    }

    void writeCommand(uint8_t data) {  // Sending Routine Master Mode (polling)
        // Chip Select/Enable: Active LOW
        chipSelect();
        // D/C=0:the current data byte is interpreted as command byte
        chipWriteCommand();
#if defined(ARDUINO) && ARDUINO >= 100
        // Run trough the single bits to transfer
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
#else
        SPDR = data;  // send Character

        waitForTransmissionReady();
#endif
        // Chip DeSelect/Disable: InActive HIGH
        chipDeSelect();
    }
    void writeData(uint8_t data) {  // Sending Routine Master Mode (polling)
        // Chip Select/Enable: Active LOW
        chipSelect();
        //  D/C=1:write data to display RAM
        chipWriteData();
#if defined(ARDUINO) && ARDUINO >= 100
        // Run trough the single bits to transfer
        for (uint8_t i = 0; i < 8; i++) {
            if (data & 0x80) {                 // 1000 0000
                digitalWrite(SPI_MOSI, HIGH);  //
            } else {
                digitalWrite(SPI_MOSI, LOW);  //
            }
            digitalWrite(SPI_SCK, LOW);
            data = data << 1;
            digitalWrite(SPI_SCK, HIGH);  //
        }
#else
        SPDR = data;  // send Character

        waitForTransmissionReady();
#endif
        // Chip DeSelect/Disable: InActive HIGH
        chipDeSelect();
    }

#if defined(ARDUINO) && ARDUINO >= 100

#else
    inline void waitForTransmissionReady(void) {
        while (!(SPSR & (1 << SPIF)))
            ;  // wait until Char is sent
    }
#endif
#pragma endregion INTERNAL METHODS
#pragma endregion PRIVATE

   public:
#pragma region PUBLIC
#pragma region GENERAL METHODS
    /**************************************************************************************/
    LCD8448() {}
    ~LCD8448() {}
    void init(void) {
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
#ifdef BACKLIGHT_INVERTED
        if (dat != 0)
            digitalWrite(LCD_BL, LOW);
        else
            digitalWrite(LCD_BL, HIGH);
#else
        if (dat != 0)
            digitalWrite(LCD_BL, HIGH);
        else
            digitalWrite(LCD_BL, LOW);

#endif
#else
#ifdef BACKLIGHT_INVERTED
        if (dat != 0)
            LCD_BL_PORT &= ~(1 << LCD_BL);
        else
            LCD_BL_PORT &= (1 << LCD_BL);
#else
        if (dat != 0)
            LCD_BL_PORT &= (1 << LCD_BL);
        else
            LCD_BL_PORT &= ~(1 << LCD_BL);
#endif
#endif
    }

    inline void setBacklightOFF(void) {
#if defined(ARDUINO) && ARDUINO >= 100
#ifdef BACKLIGHT_INVERTED
        digitalWrite(LCD_BL, HIGH);
#else
        digitalWrite(LCD_BL, LOW);
#endif
#else
#ifdef BACKLIGHT_INVERTED
        LCD_BL_PORT |= (1 << LCD_BL);
#else
        LCD_BL_PORT &= ~(1 << LCD_BL);
#endif
#endif
    }

    inline void setBacklightON(void) {
#if defined(ARDUINO) && ARDUINO >= 100
#ifdef BACKLIGHT_INVERTED
        digitalWrite(LCD_BL, LOW);
#else
        digitalWrite(LCD_BL, HIGH);
#endif
#else
#ifdef BACKLIGHT_INVERTED
        LCD_BL_PORT &= ~(1 << LCD_BL);
#else
        LCD_BL_PORT |= (1 << LCD_BL);
#endif
#endif
    }

    void clear(void);
    void set_XY(uint8_t X, uint8_t Y);
    /**************************************************************************************/
#pragma endregion GENERAL METHODS

#pragma region DIRECT DISPLAY METHODS
    /**************************************************************************************/
    void draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map);
    void draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map, uint8_t Pix_x, uint8_t Pix_y);
    void draw_bmp_pixel_P(uint8_t X, uint8_t Y, const unsigned char *map);
    void write_char(unsigned char c, uint8_t mode);
    void write_string(uint8_t X, uint8_t Y, const char *str, uint8_t mode);
    void write_char_big(uint8_t X, uint8_t Y, unsigned char c, uint8_t mode);
    void write_string_big(uint8_t X, uint8_t Y, const char *str, uint8_t mode);
    /**************************************************************************************/
#pragma endregion DIRECT DISPLAY METHODS

#pragma region SPECIAL DISPLAY METHODS
    /**************************************************************************************/
    void write_chinese(uint8_t X, uint8_t Y, const unsigned char *c, uint8_t ch_with, uint8_t num, uint8_t line, uint8_t row);
    unsigned char prop_write_char(char c, uint8_t mode);
    void prop_write_string(uint8_t X, uint8_t Y, const char *str, uint8_t mode);
    /*************************************************************************************/
    void write_number_big(uint8_t X, uint8_t Y, int number, uint8_t comma, uint8_t digits, uint8_t mode);
    void write_number_big2(uint8_t X, uint8_t Y, uint8_t number);
    /*************************************************************************************/
#pragma endregion SPECIAL DISPLAY METHODS

#pragma region VIRTUAL DISPLAY METHODS
    /**************************************************************************************/
    void vd_clear(void);
    void vd_print(void);
    void vd_set_pixel(uint8_t X0, uint8_t Y0);
    void vd_set_pixel_byte(uint8_t X0, uint8_t Y0, uint8_t c);
    void vd_set_pixel_byte_any(uint8_t X0, uint8_t Y0, uint8_t c);
    void vd_write_char(uint8_t X, uint8_t Y, char c, uint8_t mode);
    void vd_write_char_v(uint8_t X, uint8_t Y, char c, uint8_t mode);
    void vd_write_string(uint8_t X, uint8_t Y, const char *str, uint8_t mode);
    void vd_write_string_v(uint8_t X, uint8_t Y, const char *str, uint8_t mode);
    void vd_write_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);
    void vd_write_rect(uint8_t X0, uint8_t Y0, uint8_t a, uint8_t b);
    void vd_write_circle(uint8_t X0, uint8_t Y0, uint8_t radius);
#pragma region SPECIAL DISPLAY METHODS
    /**************************************************************************************/
    void vd_write_framework(char *head, uint8_t mode);
    void vd_alert(const char *text);
    void vd_question(const char *, uint8_t);
    void vd_overlayON(void);
    void vd_overlayOFF(void);
    void vd_battery(uint8_t X0, uint8_t Y0, uint8_t state, uint8_t mode);
    /**************************************************************************************/
#pragma endregion SPECIAL DISPLAY METHODS
    /**************************************************************************************/
#pragma endregion VIRTUAL DISPLAY METHODS
#pragma endregion PUBLIC
};
extern LCD8448 lcd;

#endif /* LCD8448_H_ */
