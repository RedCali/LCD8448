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
********** Regular FONTS *************
* index = ASCII - 32
*/
#pragma once
#ifndef LCD8448_FONT_H_
#define LCD8448_FONT_H_

#include <avr/pgmspace.h>

const unsigned char font6_8[][6] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // sp --> 32
    {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00},  // !
    {0x00, 0x00, 0x07, 0x00, 0x07, 0x00},  // "
    {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14},  // #
    {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12},  // $
    {0x00, 0x62, 0x64, 0x08, 0x13, 0x23},  // %
    {0x00, 0x36, 0x49, 0x55, 0x22, 0x50},  // &
    {0x00, 0x00, 0x05, 0x03, 0x00, 0x00},  // '
    {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00},  // (
    {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00},  // )
    {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14},  // * --> 42
    {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},  // +
    {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00},  // ,
    {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},  // -
    {0x00, 0x00, 0x60, 0x60, 0x00, 0x00},  // .
    {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},  // /
    {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0
    {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},  // 1
    {0x00, 0x42, 0x61, 0x51, 0x49, 0x46},  // 2
    {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31},  // 3
    {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},  // 4 --> 52
    {0x00, 0x27, 0x45, 0x45, 0x45, 0x39},  // 5
    {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},  // 6
    {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},  // 7
    {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},  // 8
    {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},  // 9
    {0x00, 0x00, 0x36, 0x36, 0x00, 0x00},  // :
    {0x00, 0x00, 0x56, 0x36, 0x00, 0x00},  // ;
    {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},  // <
    {0x00, 0x14, 0x14, 0x14, 0x14, 0x14},  // =
    {0x00, 0x00, 0x41, 0x22, 0x14, 0x08},  // > --> 62
    {0x00, 0x02, 0x01, 0x51, 0x09, 0x06},  // ?
    {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E},  // @
    {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C},  // A
    {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},  // B
    {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},  // C
    {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C},  // D
    {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},  // E
    {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},  // F
    {0x00, 0x3E, 0x41, 0x49, 0x29, 0x7A},  // G		{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},  // H --> 72
    {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00},  // I
    {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01},  // J
    {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},  // K
    {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},  // L
    {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F},  // M
    {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F},  // N
    {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
    {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},  // P
    {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},  // Q
    {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46},  // R --> 82
    {0x00, 0x46, 0x49, 0x49, 0x49, 0x31},  // S
    {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},  // T
    {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},  // U
    {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},  // V
    {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F},  // W
    {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},  // X
    {0x00, 0x07, 0x08, 0x70, 0x08, 0x07},  // Y
    {0x00, 0x61, 0x51, 0x49, 0x45, 0x43},  // Z
    {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00},  // [
    {0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55},  // / --> 92
    {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00},  // ]
    {0x00, 0x04, 0x02, 0x01, 0x02, 0x04},  // ^
    {0x00, 0x40, 0x40, 0x40, 0x40, 0x40},  // _
    {0x00, 0x00, 0x01, 0x02, 0x04, 0x00},  // '
    {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},  // a
    {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38},  // b
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x20},  // c
    {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F},  // d
    {0x00, 0x38, 0x54, 0x54, 0x54, 0x18},  // e
    {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02},  // f --> 102
    {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},  // g
    {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78},  // h
    {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00},  // i
    {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00},  // j
    {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},  // k
    {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00},  // l
    {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},  // m
    {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78},  // n
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x38},  // o
    {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18},  // p --> 112
    {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC},  // q
    {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08},  // r
    {0x00, 0x48, 0x54, 0x54, 0x54, 0x20},  // s
    {0x00, 0x04, 0x3F, 0x44, 0x40, 0x20},  // t
    {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C},  // u
    {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},  // v
    {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C},  // w
    {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},  // x
    {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C},  // y
    {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44},  // z --> 122
    {0x00, 0x00, 0x06, 0x09, 0x09, 0x06},  // � grad Zeichen		TASTE --> {
    {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00},  // |					TASTE --> |
    {0x00, 0x48, 0x48, 0x7E, 0x48, 0x48},  // +- �				TASTE --> }
    {0x00, 0x12, 0x12, 0x7E, 0x12, 0x12}   // -+ �				TASTE --> ~
};

const unsigned char font20_24[] PROGMEM = {
    0x00, 0x00, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0x3F, 0x1F, 0x0F, 0x0F, 0x0F,
    0x1F, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xF0, 0x00, 0x00, 0x30, 0x38, 0x3C, 0x7E, 0x7E, 0x7F, 0x1F,
    0x0F, 0x0F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0xC0, 0xF0, 0xF8, 0xFC, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x1F,
    0x1F, 0x1F, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0x7F, 0x1F, 0x0F, 0x0F, 0x0F,
    0x1F, 0x3F, 0x3F, 0x3E, 0x3C, 0x3C, 0x38, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x1F, 0x1F, 0x9F, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x00, 0x00, 0x00, 0xF8, 0xFC,
    0xFE, 0xFE, 0xFF, 0xFF, 0x9F, 0x0F, 0x0F, 0x9F, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0x00, 0x00,
    0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0x1F, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0xFE, 0xFE, 0xFC,
    0xF8, 0xF0, 0x80, 0x00, 0x00, 0xC0, 0xF0, 0xFC, 0xFE, 0xFE, 0xFE, 0x3F, 0x0F, 0x0F, 0x0F, 0x1F,
    0x3F, 0xFF, 0xFE, 0xFE, 0xFC, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x70, 0xE0,
    0xDE, 0xFE, 0xDE, 0xE0, 0xF0, 0x70, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0x3F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC,
    0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1F, 0x3F, 0xFF, 0xFF, 0xFB, 0xF9, 0xF0, 0xE0, 0x00, 0xE0, 0xF0, 0xF8, 0xFE,
    0xFF, 0xDF, 0xC7, 0xC3, 0xC1, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0,
    0x00, 0x00, 0x0E, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F, 0x0F, 0x07, 0x07, 0x07, 0x0F, 0xFF, 0xFF, 0xFF,
    0xFE, 0xFC, 0xF8, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x0E, 0x0F, 0x0F, 0x0F,
    0x1F, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0,
    0xF8, 0xFE, 0xFF, 0xFF, 0x7F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xF9,
    0xFF, 0xFF, 0xFF, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xF9, 0xF8, 0xE0, 0x00,
    0x00, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0x7C, 0x78, 0x78, 0x78, 0x38, 0x1E, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x1F,
    0x07, 0x01, 0x07, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE0, 0xFC, 0xFF, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x70,
    0x70, 0x70, 0x70, 0x00, 0x00, 0x60, 0x7C, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7D, 0x7C,
    0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x00, 0x00, 0x02, 0x0E, 0x1F, 0x3F, 0x3F, 0x7F, 0x7C,
    0x78, 0x78, 0x78, 0x78, 0x7C, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03, 0x00, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x07, 0x07, 0x07,
    0x00, 0x02, 0x0E, 0x1F, 0x3F, 0x3F, 0x7F, 0x7C, 0x78, 0x78, 0x78, 0x78, 0x7E, 0x7F, 0x3F, 0x3F,
    0x1F, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7E, 0x7C, 0x78, 0x78, 0x78,
    0x7C, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F,
    0x3F, 0x3F, 0x7F, 0x7E, 0x78, 0x78, 0x78, 0x78, 0x7E, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03, 0x00,
    0x00, 0x04, 0x1E, 0x1E, 0x3E, 0x7E, 0x7E, 0x7C, 0x78, 0x78, 0x78, 0x7C, 0x7F, 0x3F, 0x3F, 0x1F,
    0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1F, 0x1F, 0x3F, 0x7F, 0x7E, 0x7C, 0x78, 0x78, 0x7C,
    0x7E, 0x7F, 0x3F, 0x3F, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E,
    0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x23, 0xE3, 0x73, 0x7F, 0x3F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0x7E, 0x7F, 0x1F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char v[] PROGMEM = {
    0x02,
    0x02,
    0x06,
    0x1E,
    0xFE,
    0xFE,
    0xFE,
    0xFE,
    0xE2,
    0x02,
    0x02,
    0x00,
    0x00,
    0x00,
    0x02,
    0x02,
    0xC6,
    0xFE,
    0x3E,
    0x0E,
    0x06,
    0x02,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x0F,
    0x7F,
    0xFF,
    0xFF,
    0xFF,
    0xF8,
    0xE0,
    0x00,
    0xC0,
    0xF8,
    0x3E,
    0x07,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x07,
    0x3F,
    0x7F,
    0x7F,
    0x3F,
    0x07,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

#endif /* LCD8448_FONT_H_ */