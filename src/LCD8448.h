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
#ifndef LCD8448_H_
#define LCD8448_H_

#include "LCD8448_Config.h"

#define LCD8448_VERSION "1.0.0"  // software version of this library

#if defined(ARDUINO) && ARDUINO >= 100
#pragma message("Compiling for Arduino Framework Architecture...")
#include "Arduino.h"
#else
#pragma message("Compiling for AVR ATMEGA Framework Architecture...")
// #include "WProgram.h"
#include <stdlib.h>
#include <stdio.h>
#endif

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#pragma region Constant definition
// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDED_INSTRUCTION 0x01
#define PCD8544_DISPLAY_BLANK 0x00
#define PCD8544_DISPLAY_NORMAL 0x04
#define PCD8544_DISPLAY_ALLON 0x01
#define PCD8544_DISPLAY_INVERTED 0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAY_CONTROL 0x08
#define PCD8544_SET_XY_COLUM_X 0x80
#define PCD8544_SET_XY_COLUM_X_MASK 0x3f
#define PCD8544_SET_XY_ROW_Y 0x40
#define PCD8544_SET_XY_ROW_Y_MASK 0x07
// Extended instruction set
#define PCD8544_SET_TEMP 0x04
#define PCD8544_SET_BIAS 0x10
#define PCD8544_SET_VOP 0x80
// Display presets
#define LCD_BIAS 0x03              // Range: 0-7 (0x00-0x07)
#define LCD_TEMP 0x02              // Range: 0-3 (0x00-0x03)
#define LCD_CONTRAST_DEFAULT 0x46  // Range: 0-127 (0x00-0x7F)
#define LCD_CONTRAST_MAX 0x7F      // Range: 0-127 (0x00-0x7F)
#define LCD_CONTRAST_MIN 0x01      // Range: 0-127 (0x00-0x7F)
// Display Command / Data
#define LCD_COMMAND 0
#define LCD_DATA 1
#pragma endregion Constant definition

class LCD8448 {
  public:
  #pragma region PUBLIC

  enum LCD_Mode : uint8_t {
    // Display Control Instruction,  bit DB3 = 1
    //                    // 3D0E
    MODE_BLANK = PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_BLANK,       // 1000 - D=0 / E=0
    MODE_ALL_ON = PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_ALLON,      // 1001 - D=0 / E=1
    MODE_REGULAR = PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_NORMAL,    // 1100 - D=1 / E=0
    MODE_INVERTED = PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_INVERTED  // 1101 - D=1 / E=1
  };

  enum LCD_Display : uint8_t {
    NORMAL = 0x01,
    INVERTED = 0x00
  };


  #pragma region GENERAL METHODS
  /**************************************************************************************/
  LCD8448() {
    _initMode = MODE_REGULAR;
    _contrast = LCD_CONTRAST_DEFAULT;
    _backlightInverted = NORMAL;
  }

  ~LCD8448() {}

  void init(LCD_Mode initMode = MODE_REGULAR, LCD_Display backlightInverted = NORMAL) {
    #if defined(ARDUINO) && ARDUINO >= 100
    #ifdef LCD_DEBUG
    Serial.print("LCD PINS --> SPI SCK: ");
    Serial.print(SPI_SCK);
    Serial.print(" / SPI MOSI: ");
    Serial.print(SPI_MOSI);
    Serial.print(" / SPI CS: ");
    Serial.print(SPI_CS);
    Serial.print(" / SPI DC: ");
    Serial.print(LCD_DC);
    Serial.print(" / SPI RST: ");
    Serial.print(LCD_RST);
    Serial.print(" / SPI BL: ");
    Serial.println(LCD_BL);
    #else

    #endif

    pinMode(SPI_SCK, OUTPUT);
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_CS, OUTPUT);

    digitalWrite(SPI_SCK, LOW);
    digitalWrite(SPI_MOSI, LOW);
    digitalWrite(SPI_CS, LOW);

    pinMode(LCD_DC, OUTPUT);
    pinMode(LCD_RST, OUTPUT);
    pinMode(LCD_BL, OUTPUT);

    digitalWrite(LCD_DC, LOW);
    digitalWrite(LCD_RST, LOW);
    #else
    LCD_DDR |= (1 << SPI_CS) | (1 << SPI_MOSI) | (1 << SPI_SCK);
    LCD_DDR |= (1 << LCD_RST) | (1 << LCD_DC);
    LCD_BL_DDR |= (1 << LCD_BL);

    LCD_PORT &= ~((1 << LCD_RST) | (1 << LCD_DC));

    SPCR = (1 << SPE) | (1 << MSTR);  //|(1<<SPR0);	// enable SPI in Master Mode with SCK = CK/4
    SPSR = (1 << SPI2X);
    // IOReg   = SPSR;                 			// clear SPIF bit in SPSR
    // SPSR
    // SPDR = 0;
    #endif
    // Set backlight inversion flag
    _backlightInverted = backlightInverted;
    // Reset Sleep mode
    _sleep = false;
    // turn off the backlight
    setBacklightOFF();
    // reset Display
    reset();

    // toggle chip select
    chipSelect();  // Chip Select,Slave Transmit Enable(active low,Master Output)
    for (int i = 0; i < 1000; i++) {
    };
    chipDeSelect();
    for (int i = 0; i < 1000; i++) {
    };

    // data_type=0, all are command bytes
    writeCommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDED_INSTRUCTION);  // Function Set:0010 0001 --> PD=0, V=0, H=1;
    // writeCommand(0xc0);  // Set Vop:1 Vop6 Vop5 Vop4 Vop3 Vop2 Vop1 Vop0=1100 0000
    writeCommand(PCD8544_SET_VOP | _contrast);
    writeCommand(PCD8544_SET_TEMP | LCD_TEMP);  // Set Temperature Coefficient:0 0 0 0 0 1 Tc1 Tc0=0000 0110;Tc1=1,Tc0=0(Vlcd temperature coefficient 2)
    writeCommand(PCD8544_SET_BIAS | LCD_BIAS);  // Set Bias System (BSx):0 0 0 1 0 BS2 BS1 BS0=0001 0011;BS2=0, BS1=1, BS0=1==>N=4,MUX RATE=1:48
    writeCommand(PCD8544_FUNCTIONSET);          // Function Set:0 0 1 0 0 PD V H=0010 0000;PD=0,V=0,H=0;

    clearLCD();
    vd_clear();

    mode(initMode);
    chipDeSelect();
  }

  inline void on(void) {
    if (_sleep) {
      disableSleep();
      } else {
      init(_initMode);
    }
    setBacklightON();
  }

  inline void off(void) {
    setBacklightOFF();
    enableSleep();
  }

  inline void reset(void) {
    #if defined(ARDUINO) && ARDUINO >= 100
    digitalWrite(LCD_RST, LOW);
    delayMicroseconds(1);
    digitalWrite(LCD_RST, HIGH);
    delayMicroseconds(1);
    #else
    LCD_PORT |= (1 << LCD_RST);
    for (int i = 0; i < 1000; i++) {
    };
    LCD_PORT |= (1 << LCD_RST);
    for (int i = 0; i < 1000; i++) {
    };
    #endif
  }

  inline void backlight(uint8_t dat) {
    if (_sleep) return;
    #if defined(ARDUINO) && ARDUINO >= 100
    if (_backlightInverted == INVERTED) {
      if (dat != 0)
      digitalWrite(LCD_BL, LOW);
      else
      digitalWrite(LCD_BL, HIGH);
      } else {
      if (dat != 0)
      digitalWrite(LCD_BL, HIGH);
      else
      digitalWrite(LCD_BL, LOW);
    }
    #else
    if (_backlightInverted == INVERTED) {
      if (dat != 0)
      LCD_BL_PORT &= ~(1 << LCD_BL);
      else
      LCD_BL_PORT &= (1 << LCD_BL);
      } else {
      if (dat != 0)
      LCD_BL_PORT &= (1 << LCD_BL);
      else
      LCD_BL_PORT &= ~(1 << LCD_BL);
    }
    #endif
  }

  inline void setBacklightOFF(void) {
    #if defined(ARDUINO) && ARDUINO >= 100
    if (_backlightInverted == INVERTED) {
      digitalWrite(LCD_BL, HIGH);
      } else {
      digitalWrite(LCD_BL, LOW);
    }
    #else
    if (_backlightInverted == INVERTED) {
      LCD_BL_PORT |= (1 << LCD_BL);
      } else {
      LCD_BL_PORT &= ~(1 << LCD_BL);
    }
    #endif
  }

  inline void setBacklightON(void) {
    if (_sleep) return;
    #if defined(ARDUINO) && ARDUINO >= 100
    if (_backlightInverted == INVERTED) {
      digitalWrite(LCD_BL, LOW);
      } else {
      digitalWrite(LCD_BL, HIGH);
    }
    #else
    if (_backlightInverted == INVERTED) {
      LCD_BL_PORT &= ~(1 << LCD_BL);
      } else {
      LCD_BL_PORT |= (1 << LCD_BL);
    }
    #endif
  }

  // timed backlight
  // dimmed backlight
  // faded switch on/off in case of dimming is active

  inline void mode(LCD_Mode mode) {
    if (_sleep) return;
    #ifdef LCD_DEBUG
    #if defined(ARDUINO) && ARDUINO >= 100
    Serial.print("LCD MODE --> Selected mode: ");
    Serial.println(mode, HEX);
    #endif
    #endif
    _initMode = mode;
    writeCommand(_initMode);
  }

  inline void invert() {
    if (_sleep) return;
    if (_initMode == MODE_REGULAR)
    _initMode = MODE_INVERTED;
    else
    _initMode = MODE_REGULAR;

    #ifdef LCD_DEBUG
    #if defined(ARDUINO) && ARDUINO >= 100
    Serial.print("LCD MODE INVERTED --> Selected mode: ");
    Serial.println(_initMode, HEX);
    #endif
    #endif
    writeCommand(_initMode);
  }

  void setContrast(uint8_t contrast);

  void enableSleep();

  void disableSleep();

  void clearLCD(void);

  void clearRow(int row, int startX, int endX);

  inline void setXY(uint8_t X, uint8_t Y) {
    #ifdef LCD_DEBUG
    #if defined(ARDUINO) && (ARDUINO >= 100)
    Serial.print("LCD Set X: ");
    Serial.print(X);
    Serial.print(" / Y: ");
    Serial.println(Y);
    #endif
    #endif
    writeCommand(PCD8544_SET_XY_COLUM_X | (X & PCD8544_SET_XY_COLUM_X_MASK));  // X / column
    writeCommand(PCD8544_SET_XY_ROW_Y | (Y & PCD8544_SET_XY_ROW_Y_MASK));      // Y / row
  }
  /**************************************************************************************/
  #pragma endregion GENERAL METHODS

  #pragma region DIRECT METHODS
  /**************************************************************************************/
  void drawArduinoLogo();
  void draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map);
  void draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map, uint8_t pixelsWidth, uint8_t pixelHeight);
  void draw_bmp_pixel_P(uint8_t X, uint8_t Y, const unsigned char *map);
  void write_char(unsigned char c, LCD_Display mode = NORMAL);
  void write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode = NORMAL);
  void write_char_big(uint8_t X, uint8_t Y, unsigned char c, LCD_Display mode = NORMAL);
  void write_string_big(uint8_t X, uint8_t Y, const char *str, LCD_Display mode = NORMAL);
  void write_num_integer(uint8_t X, uint8_t Y, long num, int length, char filler, LCD_Display mode = NORMAL);
  void write_num_float(uint8_t X, uint8_t Y, float num, int length, uint8_t dec, char divider, char filler, LCD_Display mode = NORMAL);
  /**************************************************************************************/
  #pragma endregion DIRECT METHODS

  #pragma region SPECIAL METHODS
  /**************************************************************************************/
  void write_chinese(uint8_t X, uint8_t Y, const unsigned char *c, uint8_t charWith, uint8_t num, uint8_t row, LCD_Display mode = NORMAL);
  unsigned char prop_write_char(char c, LCD_Display mode = NORMAL);
  void prop_write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode = NORMAL);
  /*************************************************************************************/
  void write_number_big(uint8_t X, uint8_t Y, int number, uint8_t comma, uint8_t digits, LCD_Display mode = NORMAL);
  void write_number_big2(uint8_t X, uint8_t Y, uint8_t number, LCD_Display mode = NORMAL);
  /*************************************************************************************/
  #pragma endregion SPECIAL METHODS

  #pragma region VIRTUAL LCD METHODS
  /**************************************************************************************/
  void vd_clear(void);
  void vd_print(void);
  void vd_set_pixel(uint8_t X0, uint8_t Y0);
  void vd_set_pixel_byte(uint8_t X0, uint8_t Y0, uint8_t c);
  void vd_set_pixel_byte_any(uint8_t X0, uint8_t Y0, uint8_t c);
  void vd_write_char(uint8_t X, uint8_t Y, char c, LCD_Display mode = NORMAL);
  void vd_write_char_v(uint8_t X, uint8_t Y, char c, LCD_Display mode = NORMAL);
  void vd_write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode = NORMAL);
  void vd_write_string_v(uint8_t X, uint8_t Y, const char *str, LCD_Display mode = NORMAL);
  void vd_write_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);
  void vd_write_rect(uint8_t X0, uint8_t Y0, uint8_t width, uint8_t height);
  void vd_write_round_rect(uint8_t X0, uint8_t Y0, uint8_t width, uint8_t height);
  void vd_write_circle(uint8_t X0, uint8_t Y0, uint8_t radius);

  #pragma region SPECIAL METHODS
  /**************************************************************************************/
  void vd_write_framework(char *head, LCD_Display mode = NORMAL);
  void vd_alert(const char *text);
  void vd_question(const char *question, uint8_t active);
  void vd_overlayON(void);
  void vd_overlayOFF(void);
  /**************************************************************************************/
  #pragma endregion SPECIAL DISPLAY METHODS

  #pragma region SPECIAL SYMBOL METHODS
  /**************************************************************************************/
  enum LCD_Symbols : uint8_t {
    ANTENNA_1,
    ANTENNA_2,
    ANTENNA_STRENGTH_1,
    ANTENNA_STRENGTH_2,
    ARROW_UP_DOWN,
    BAR_GRAPH,
    BATTERY,
    BLUETHOOTH,
    ENVELOPE,
    NETWORK,
    SD_CARD,
    SIGNAL_STRENGTH_1,
    SIGNAL_STRENGTH_2,
    WIRELESS
  };

  void vd_symbol(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Symbols symbol, LCD_Display mode = NORMAL);

  void vd_antenna1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_antenna2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_antennaStrength1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_antennaStrength2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_arrowUpDown(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_barGraph(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_battery(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_bluetooth(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_envelope(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_network(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_sdCard(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_signalStrength1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_signalStrength2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  void vd_wireless(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode = NORMAL);
  /**************************************************************************************/
  #pragma endregion SPECIAL DISPLAY SYMBOL METHODS
  /**************************************************************************************/
  #pragma endregion VIRTUAL LCD METHODS
  #pragma endregion PUBLIC

  private:
  #pragma region PRIVATE
  LCD_Mode _initMode = MODE_REGULAR;
  LCD_Display _backlightInverted = NORMAL;
  uint8_t _contrast;
  uint8_t _sleep;
  unsigned char _virtualDisplay[504];
  unsigned char _virtualDisplayTemp[504];

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
    digitalWrite(LCD_DC, LCD_COMMAND);
    #else
    LCD_PORT &= ~(1 << LCD_DC);
    #endif
  }

  inline void chipWriteData() {
    // D/C=1:write data to display RAM
    #if defined(ARDUINO) && ARDUINO >= 100
    digitalWrite(LCD_DC, LCD_DATA);
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

  void convertFloat(char *buf, float num, int width, uint8_t prec);
  #pragma endregion INTERNAL METHODS
  #pragma endregion PRIVATE

};

extern LCD8448 lcd;
#endif /* LCD8448_H_ */
