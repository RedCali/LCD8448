/*
* LCD Display Library for the 8448 LCD Display (Nokia 5110 display)
* Base      : Lauren from DFRobot / version 0.3
* Date      : 06.01.2012
* E-mail    : Lauran.pan@gmail.com

* Modified  : Dominik Schubert
* Date      : 06.01.2013

* Modified  : Bastian Geier
* Date      : 30.12.2023
* E-mail    : mail@bastiangeier.de
* Repo      : https://github.com/RedCali/LCD8448.git

* Any suggestions are welcome.
*/
/*
********** Symbols *******************
* The Symbol arrays created with: https://github.com/RedCali/Image2Bitmap
*/
/*
********** Ideas for new Symbols *****
* Box - ballot , Tick Box, Box, X-Box
* Unicode U+2610 U+2611 U+2612
*/
#pragma once
#ifndef LCD8448_SYMBOLS_H_
#define LCD8448_SYMBOLS_H_

#include <avr/pgmspace.h>

const unsigned char antenna1[][7] PROGMEM = {
    {0x00, 0x18, 0x30, 0xe0, 0x30, 0x18, 0x00},  //  0 - no connection
    {0x00, 0x18, 0x32, 0xe4, 0x32, 0x18, 0x00},  //  1 - connecting 1
    {0x01, 0x1a, 0x34, 0xe8, 0x34, 0x1a, 0x01},  //  1 - connecting 2
    {0x01, 0x1a, 0x35, 0xea, 0x35, 0x1a, 0x01}   //  2 - connecting 3
};

const unsigned char antenna2[][7] PROGMEM = {
    {0x02, 0x04, 0x08, 0xf0, 0x08, 0x04, 0x02},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0x05, 0x03}   //
};

const unsigned char antennaStrength1[][18] PROGMEM = {
    {0x03, 0x05, 0x09, 0xff, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0xff}   //
};

const unsigned char antennaStrength2[][12] PROGMEM = {
    {0x03, 0x05, 0x09, 0xff, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xf0, 0x00, 0xfc, 0x00, 0x00},  //
    {0x03, 0x05, 0x09, 0xff, 0x09, 0xc5, 0x03, 0xf0, 0x00, 0xfc, 0x00, 0xff}   //
};

const unsigned char arrowUpDown[][12] PROGMEM = {
    {0x04, 0x06, 0xff, 0xff, 0x06, 0x04, 0x20, 0x60, 0xff, 0xff, 0x60, 0x20},  //
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0xff, 0xff, 0x60, 0x20},  //
    {0x04, 0x06, 0xff, 0xff, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   //
};

const unsigned char barGraph[][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0x80, 0x80, 0x80, 0x80, 0x80},  //
    {0xc0, 0xc0, 0xc0, 0xc0, 0xc0},  //
    {0xe0, 0xe0, 0xe0, 0xe0, 0xe0},  //
    {0xf0, 0xf0, 0xf0, 0xf0, 0xf0},  //
    {0xf8, 0xf8, 0xf8, 0xf8, 0xf8},  //
    {0xfc, 0xfc, 0xfc, 0xfc, 0xfc},  //
    {0xfe, 0xfe, 0xfe, 0xfe, 0xfe},  //
    {0xff, 0xff, 0xff, 0xff, 0xff}   //
};

const unsigned char battery[][8] PROGMEM = {
    {0x3c, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3c, 0x18},  //  0 - empty
    {0x3c, 0x34, 0x24, 0x24, 0x24, 0x24, 0x3c, 0x18},  //  1 -  10%
    {0x3c, 0x3c, 0x24, 0x24, 0x24, 0x24, 0x3c, 0x18},  //  2 -  20%
    {0x3c, 0x3c, 0x34, 0x24, 0x24, 0x24, 0x3c, 0x18},  //  3 -  30%
    {0x3c, 0x3c, 0x3c, 0x24, 0x24, 0x24, 0x3c, 0x18},  //  4 -  40%
    {0x3c, 0x3c, 0x3c, 0x34, 0x24, 0x24, 0x3c, 0x18},  //  5 -  50%
    {0x3c, 0x3c, 0x3c, 0x3c, 0x24, 0x24, 0x3c, 0x18},  //  6 -  60%
    {0x3c, 0x3c, 0x3c, 0x3c, 0x34, 0x24, 0x3c, 0x18},  //  7 -  70%
    {0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x24, 0x3c, 0x18},  //  8 -  80%
    {0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x34, 0x3c, 0x18},  //  9 -  90%
    {0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x18}   // 10 - 100%
};

const unsigned char bluetooth[][6] PROGMEM = {
    {0x24, 0x18, 0xff, 0x91, 0x5a, 0x24},  //
    {0x3c, 0x18, 0xff, 0x91, 0x5a, 0x3c}   //
};

const unsigned char envelope[][12] PROGMEM = {
    {0xff, 0xc1, 0xa3, 0x95, 0x99, 0x91, 0x91, 0x99, 0x95, 0xa3, 0xc1, 0xff},  //
    {0xff, 0xc1, 0xa3, 0x95, 0x95, 0x95, 0x95, 0x95, 0x95, 0xa3, 0xc1, 0xff}   //
};

const unsigned char network[][7] PROGMEM = {
    {0x68, 0x60, 0x6b, 0x03, 0x6b, 0x60, 0x68},  //  0 - not connected
    {0x68, 0x70, 0x6b, 0x07, 0x6b, 0x70, 0x68},  //  1 - connecting
    {0x68, 0x78, 0x6b, 0x0f, 0x6b, 0x78, 0x68}   //  2 - connected
};

const unsigned char sdCard[][7] PROGMEM = {
    {0xfc, 0xc6, 0xab, 0x91, 0xa9, 0xc5, 0xff},  //  0 - no connection
    {0xfc, 0x96, 0xa3, 0xc1, 0xa1, 0x91, 0xff},  //  1 - connecting 1
    {0xfc, 0x86, 0x93, 0x89, 0xfd, 0x81, 0xff}   //  2 - connecting 2
};

const unsigned char signalStrength1[][13] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0x00},  //
    {0xc0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0xff}   //
};

const unsigned char signalStrength2[][7] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00},  //
    {0xc0, 0x00, 0xf0, 0x00, 0xfc, 0x00, 0x00},  //
    {0xc0, 0x00, 0xf0, 0x00, 0xfc, 0x00, 0xff}   //
};

const unsigned char wireless[][8] PROGMEM = {
    {0xc0, 0xc0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00},  //  0 - not connected
    {0xc0, 0xd0, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00},  //  1 - connecting 1
    {0xc0, 0xd4, 0x34, 0x6c, 0x18, 0x70, 0x00, 0x00},  //  2 - connecting 2
    {0xc0, 0xd5, 0x25, 0x4d, 0x19, 0x73, 0x06, 0x7c}   //  3 - connected
};

const unsigned char roundX1[] PROGMEM = {
    0x3e, 0x5d, 0x6b, 0x77, 0x6b, 0x5d, 0x3e  //
};

const unsigned char roundX2[] PROGMEM = {
    0x1c, 0x3e, 0x6b, 0x77, 0x6b, 0x3e, 0x1c  //
};

#endif /* LCD8448_SYMBOLS_H_ */
