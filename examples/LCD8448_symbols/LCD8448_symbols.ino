// Include LCD Library
#include <LCD8448.h>
/*
// PIN Definitions for SPI Interface --- using Arduino NOKIA LCD shield
LCD_BL   7 // Backlight control (Arduino DIO Pin 7)
SPI_SCK  2 // Serial Clock(Master Output)
SPI_MOSI 3 // Master Output,Slave Input
LCD_DC   4 // Data/Command(command active low)
SPI_CS   5 // Chip Select,Slave Transmit Enable(active low,Master Output)
LCD_RST  6 // Reset & Reset Button
*/

// Define the update rate of the display and the LED
#define UPDATE_INTERVAL 500

// LED Definitions
#define LED_PIN 13
unsigned char _pinState = 0x00;

// Diaplay Variables
// Text Backfound definition
LCD8448::LCD_Display _displayHighlight = LCD8448::NORMAL;
// Storage for Runtime measuring for interval control
unsigned long _millisPrevious;
unsigned int _counter = 0;

void setup() {
  // Init Serial interface
  Serial.begin(115200);
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
  // Init LCD Display
  lcd.init();
  // Switch on the Backlight of the LCD
  lcd.setBacklightON();
  lcd.vd_clear();
}

void loop() {
  // Everytime the runtime exeeds the update intervll, we update the display
  if ((millis() - _millisPrevious) > UPDATE_INTERVAL) {
    // Write LED Pin
    digitalWrite(LED_PIN, _pinState);
    // Toggle LED Pin state
    _pinState = (_pinState == 0) ? 0x01 : 0x00;

    // Clear the display
    lcd.vd_clear();
    // Headline
    //lcd.vd_write_string(0, 0, "LCD8448 Symbol", LCD8448::NORMAL);
    lcd.vd_write_framework("LCD8448 Symbol", _displayHighlight);

    // Symbols
    lcd.vd_battery(10, 2, _counter % 11, LCD8448::NORMAL);
    lcd.vd_wireless(20, 2, _counter % 4, LCD8448::NORMAL);

    lcd.vd_network(10, 3, _counter % 3, LCD8448::NORMAL);
    lcd.vd_antenna(20, 3, _counter % 4, LCD8448::NORMAL);
    lcd.vd_sdCard(30, 3, _counter % 3, LCD8448::NORMAL);

    // Print the display contents
    lcd.vd_print();

    _counter++;
    _displayHighlight = (_displayHighlight == LCD8448::NORMAL) ? LCD8448::INVERTED : LCD8448::NORMAL;

    // Store actull time _
    _millisPrevious = millis();
  }
}


void initLCD() {
  Serial.println("Init Display....");
  lcd.init();
}

void stopLCD() {
  Serial.println("Stop Display....");
  lcd.off();
}