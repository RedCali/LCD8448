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
// Text Background definition
LCD8448::LCD_Display _displayHighlight = LCD8448::NORMAL;
// Nummeric counter
unsigned long _counter = 0;
// Char array for numeric convertion
char _bufferCounter[15];

// Storage for Runtime measuring for interval control
unsigned long _millisPrevious;

void setup() {
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
  // Init LCD Display
  initLCD();

  // Write constant strings into the display
  lcd.write_string(1 * 6 + 3, 0, "Hello", LCD8448::NORMAL);
  lcd.write_string(8 * 6 - 3, 0, "World", LCD8448::INVERTED);
// Switch on the Backlight of the LCD
  lcd.setBacklightON();
}

void loop() {
  // Everytime the runtime exeeds the update intervll, we update the display
  if ((millis() - _millisPrevious) > UPDATE_INTERVAL) {
    // Write LED Pin
    digitalWrite(LED_PIN, _pinState);
    // Toggle LED Pin state
    _pinState = (_pinState == 0) ? 0x01 : 0x00;

    // Convert numeric value into string using "sprintf", incremnt the counter afterwarts and write to display
    sprintf(_bufferCounter, "%3d.%d", (int)(abs(_counter) / 10), (int)(abs(_counter) % 10));
    _counter++;

    lcd.write_string(24, 4, _bufferCounter, LCD8448::NORMAL);

    // Just a simple string where the background highlighting toggles
    lcd.write_string(1 * 6 + 3, 2, "The LCD8448", _displayHighlight);
    _displayHighlight = (_displayHighlight == LCD8448::NORMAL) ? LCD8448::INVERTED : LCD8448::NORMAL;

    // Store actull time counter
    _millisPrevious = millis();
    // Reset the counter if reaches 1000
    if (_counter >= 1000) _counter = 0;
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