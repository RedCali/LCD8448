// Include LCD definitions - Overwrite the Library LCD Config File
#include "LCD8448_Config.h"
// Include LCD Library
#include <LCD8448.h>

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

void setup() {
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

    // Headline
    lcd.write_string(0, 0, "LCD8448 Symbol", LCD8448::NORMAL);

    // Symbols
    for (int i = 0; i < 8; i++) {
      lcd.vd_battery(10, 2, i, LCD8448::NORMAL);
      lcd.vd_wireless(20, 2, i, LCD8448::NORMAL);
    }

    for (int i = 0; i < 8; i++) {
      lcd.vd_network(10, 3, i, LCD8448::NORMAL);
      lcd.vd_antenna(20, 3, i, LCD8448::NORMAL);
      lcd.vd_sdCard(30, 3, i, LCD8448::NORMAL);
    }

    // Store actull time counter
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