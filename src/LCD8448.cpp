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
#include "LCD8448.h"

#include <math.h>

#include "LCD8448_font.h"
#include "LCD8448_font_big.h"
#include "LCD8448_font_h.h"
#include "LCD8448_symbols.h"

LCD8448 lcd = LCD8448();

#pragma region PUBLIC
#pragma region GENERAL METHODS
/**************************************************************************************/
void LCD8448::clear(void) {
    writeCommand(0x0c);
    writeCommand(0x80);
    set_XY(0, 0);
    for (int i = 0; i < 504; i++)  // 6*84
        writeData(0x00);
}

void LCD8448::set_XY(uint8_t X, uint8_t Y) {
    writeCommand(0x40 | (Y & 0x07));  // column
    writeCommand(0x80 | (X & 0x3F));  // row
}
/**************************************************************************************/
#pragma endregion GENERAL METHODS

#pragma region DIRECT DISPLAY METHODS
/**************************************************************************************/
void LCD8448::draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map) {
    draw_bmp_pixel(X, Y, map, 84, 48);
}

void LCD8448::draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map, uint8_t Pix_x, uint8_t Pix_y) {
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0)
        row = Pix_y / 8;  // row from 1 to 6;Pix_y from R0 to R47
    else
        row = Pix_y / 8 + 1;  // Quotient+1

    for (n = 0; n < row; n++) {
        set_XY(X, Y);
        for (i = 0; i < Pix_x; i++) {
            writeData(map[i + n * Pix_x]);  // D/C=1:write data to display RAM
        }
        Y++;
    }
}

void LCD8448::draw_bmp_pixel_P(uint8_t X, uint8_t Y, const unsigned char *map) {
    set_XY(X, Y);
    for (int i = 0; i < 504; i++)
        writeData(pgm_read_byte(map + i));
}

void LCD8448::write_char(unsigned char c, LCD_Display mode) {
    unsigned char line, ch;
    unsigned char *pFont;

    pFont = (unsigned char *)font6_8;
    c -= 32;  // the ASCII of "SP" is 32

    for (line = 0; line < 6; line++) {
        ch = pgm_read_byte(pFont + c * 6 + line);        // pgm_read_byte(address_short),read c from the font6_8[][6](the detail information is in the "font6x8.h")
        writeData((mode == NORMAL) ? ch : (ch ^ 0xff));  // NORMAL=0,True:return ch;False:return ch^0xff
    }
}

void LCD8448::write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
    set_XY(X, Y);
    while (*str) {
        write_char(*str, mode);
        str++;
    }
}

void LCD8448::write_char_big(uint8_t X, uint8_t Y, unsigned char c, LCD_Display mode) {
    unsigned char i, j;
    unsigned char *pFont;
    unsigned char ch_dat;

    pFont = (unsigned char *)big_number;

    if (c == '.')
        c = 10;
    else if (c == '+')
        c = 11;
    else if (c == '-')
        c = 12;
    else
        c = c & 0x0f;

    for (i = 0; i < 3; i++) {
        set_XY(X, Y + i);

        for (j = 0; j < 16; j++) {
            ch_dat = pgm_read_byte(pFont + c * 48 + i * 16 + j);
            writeData((mode == NORMAL) ? ch_dat : (ch_dat ^ 0xff));
        }
    }
}

void LCD8448::write_string_big(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
    while (*str) {
        write_char_big(X, Y, *str, mode);

        if (*str++ == '.')
            X += 5;
        else
            X += 12;
    }
}
/**************************************************************************************/
#pragma endregion DIRECT DISPLAY METHODS

#pragma region SPECIAL DISPLAY METHODS
/**************************************************************************************/
void LCD8448::write_chinese(uint8_t X, uint8_t Y, const unsigned char *c, uint8_t ch_with, uint8_t num, uint8_t line, uint8_t row) {
    unsigned char i, n;

    set_XY(X, Y);
    for (i = 0; i < num;) {
        for (n = 0; n < ch_with * 2; n++) {
            if (n == ch_with) {
                if (i == 0)
                    set_XY(X, Y + 1);
                else {
                    set_XY((X + (ch_with + row) * i), Y + 1);
                }
            }
            writeData(c[(i * ch_with * 2) + n]);
        }
        i++;
        set_XY((X + (ch_with + row) * i), Y);
    }
}

unsigned char LCD8448::prop_write_char(char c, LCD_Display mode) {
    int line, line_s = 0, line_e = 2;
    unsigned char *pFont;
    unsigned char ch;

    pFont = (unsigned char *)font6_8;

    if (c -= 32) {
        for (line_s = 0; line_s < 6; line_s++) {
            if ((unsigned char)(*pFont + c * 6 + line_s))
                break;
        }
        for (line_e = 5; line_e < 0; line_e--) {
            if ((unsigned char)(*pFont + c * 6 + line_e))
                break;
        }
    }
    for (line = line_s; line < line_e + 1; line++) {
        ch = (unsigned char)(*(pFont + c * 6 + line));
        writeData((mode == NORMAL) ? ch : (ch ^ 0xff));
    }
    writeData((mode == NORMAL) ? 0 : 0xff);
    return ((unsigned char)(line_e + 2 - line_s));
}

void LCD8448::prop_write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
    set_XY(X, Y);
    while (*str) {
        prop_write_char(*str, mode);
        str++;
    }
}
/**************************************************************************************/
void LCD8448::write_number_big(uint8_t X, uint8_t Y, int number, uint8_t comma, uint8_t digits, LCD_Display mode) {
    unsigned char lauf = 0;
    char numberInArray[digits + 2];

    if (comma > 0)
        digits += 1;

    while (lauf < digits) {
        numberInArray[digits - lauf - 1] = '0' + number % 10;
        lauf++;
        if ((lauf == comma - 1) && comma != 0) {
            numberInArray[digits - lauf - 1] = '.';
            lauf++;
        }
        number /= 10;
    }
    numberInArray[digits] = '\0';

    write_string_big(10, 1, numberInArray, NORMAL);
}

void LCD8448::write_number_big2(uint8_t X, uint8_t Y, uint8_t number) {
    unsigned char *ptr = (unsigned char *)font20_24;
    for (char i = 0; i < 3; i++) {
        set_XY(X, i + Y);
        for (char j = 0; j < 20; j++)
            writeData(pgm_read_byte(ptr + j + i * 320 + number * 20));
    }
}
/*************************************************************************************/
#pragma endregion SPECIAL DISPLAY METHODS

#pragma region VIRTUAL DISPLAY METHODS
/**************************************************************************************/
/*additional drawing functionality*/
void LCD8448::vd_clear(void) {
    for (int i = 0; i < 504; i++) {
        virtuelldisp[i] = 0;
    }
}

void LCD8448::vd_print(void) {
    draw_bmp_pixel(0, 0, virtuelldisp, 84, 48);
}

void LCD8448::vd_set_pixel(uint8_t X0, uint8_t Y0) {
    virtuelldisp[X0 + (Y0 / 8) * 84] |= (1 << Y0 % 8);
}

void LCD8448::vd_set_pixel_byte(uint8_t X0, uint8_t Y0, uint8_t c) {
    virtuelldisp[X0 + Y0 * 84] |= c;
}

void LCD8448::vd_set_pixel_byte_any(uint8_t X0, uint8_t Y0, uint8_t c) {
    unsigned char posi = Y0 % 8;
    unsigned char offset = Y0 / 8;

    virtuelldisp[X0 + offset * 84] |= (c << posi);
    offset++;
    if (offset <= 5)
        virtuelldisp[X0 + offset * 84] |= (c >> (8 - posi));
}

void LCD8448::vd_write_char(uint8_t X, uint8_t Y, char c, LCD_Display mode) {
    unsigned char line;
    unsigned char *pFont;
    unsigned char ch1;

    pFont = (unsigned char *)font6_8;
    c -= 32;

    if (Y % 8 == 0) {
        for (line = 0; line < 6; line++) {
            ch1 = pgm_read_byte(pFont + c * 6 + line);
            vd_set_pixel_byte(X + line, Y / 8, (mode == NORMAL) ? ch1 : (ch1 ^ 0xff));
        }
    } else {
        unsigned char ch2;
        unsigned char posi = Y % 8;

        for (line = 0; line < 6; line++) {
            ch1 = pgm_read_byte(pFont + c * 6 + line);
            ch2 = (ch1 << posi);
            ch1 = (ch1 >> (8 - posi));
            vd_set_pixel_byte(X + line, Y / 8, (mode == NORMAL) ? ch2 : (ch2 ^ (0xff << posi)));
            if (Y != 5) {
                vd_set_pixel_byte(X + line, Y / 8 + 1, (mode == NORMAL) ? ch1 : (ch1 ^ (0xff >> (8 - posi))));
            }
        }
    }
}

void LCD8448::vd_write_char_v(uint8_t X, uint8_t Y, char c, LCD_Display mode) {
    unsigned char ch1;
    unsigned char *pFont;

    pFont = (unsigned char *)font6_8_h;

    Y -= 6;
    if (c >= '0' && c <= '9')
        c -= '0';
    if (c == '+')
        c = 10;
    if (c == 'c')
        c = 11;
    if (c == ',')
        c = 12;
    for (unsigned char i = 0; i < 8; i++) {
        ch1 = pgm_read_byte(pFont + c * 8 + i);
        vd_set_pixel_byte_any(X + i, Y, (mode == NORMAL) ? ch1 : (ch1 ^ 0xfc));
    }
}

void LCD8448::vd_write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
    while (*str) {
        vd_write_char(X, Y, *str, mode);
        X += 6;
        str++;
    }
}

void LCD8448::vd_write_string_v(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
    while (*str && Y > 0) {
        vd_write_char_v(X, Y, *str, mode);
        Y -= 6;
        str++;
    }
}

void LCD8448::vd_write_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1) {
    signed char dx = abs(X1 - X0), sx = X0 < X1 ? 1 : -1;
    signed char dy = -abs(Y1 - Y0), sy = Y0 < Y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (!(X0 == X1 && Y0 == Y1)) {
        vd_set_pixel(X0, Y0);
        // if (X0==X1 && Y0==Y1) break;
        e2 = 2 * err;
        if (e2 > dy) {
            err += dy;
            X0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            Y0 += sy;
        }
    }
}

void LCD8448::vd_write_rect(uint8_t X0, uint8_t Y0, uint8_t a, uint8_t b) {
    vd_write_line(X0, Y0, X0 + a, Y0);
    vd_write_line(X0 + a, Y0, X0 + a, Y0 + b);
    vd_write_line(X0 + a, Y0 + b, X0, Y0 + b);
    vd_write_line(X0, Y0 + b, X0, Y0);
}

void LCD8448::vd_write_circle(uint8_t X0, uint8_t Y0, uint8_t radius) {
    signed char f = 1 - radius;
    signed char ddF_x = 0;
    signed char ddF_y = -2 * radius;
    signed char x = 0;
    signed char y = radius;

    vd_set_pixel(X0, Y0 + radius);
    vd_set_pixel(X0, Y0 - radius);
    vd_set_pixel(X0 + radius, Y0);
    vd_set_pixel(X0 - radius, Y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        vd_set_pixel(X0 + x, Y0 + y);
        vd_set_pixel(X0 - x, Y0 + y);
        vd_set_pixel(X0 + x, Y0 - y);
        vd_set_pixel(X0 - x, Y0 - y);
        vd_set_pixel(X0 + y, Y0 + x);
        vd_set_pixel(X0 - y, Y0 + x);
        vd_set_pixel(X0 + y, Y0 - x);
        vd_set_pixel(X0 - y, Y0 - x);
    }
}

#pragma region SPECIAL DISPLAY METHODS
/*************************************************************************************/
void LCD8448::vd_write_framework(char *head, LCD_Display mode) {
    // head in center position
    uint8_t lkopf = strlen(head);
    uint8_t headCenter = (14 - lkopf) * 3;

    vd_write_string(headCenter, 0, head, mode);

    for (uint8_t i = 0; i < headCenter; i++)
        virtuelldisp[i] = 0xff;

    for (uint8_t i = headCenter + lkopf * 6; i < 84; i++)
        virtuelldisp[i] = 0xff;

    // Frame around the display
    for (uint8_t i = 0; i < 6; i++) {
        virtuelldisp[84 * i] = 0xff;
        virtuelldisp[83 + 84 * i] = 0xff;
    }
    vd_write_line(1, 47, 83, 47);

    //	vd_battery(10,5,9,NORMAL);
    //	vd_write_line(1,38,83,38);
    //	vd_write_string(21,39,"22%",NORMAL);
}

void LCD8448::vd_alert(const char *text) {
    for (uint16_t i = 0; i < 504; i++) {
        if (i % 2)
            virtuelldisp[i] |= 0x55;
    }

    uint8_t len = strlen(text);
    uint16_t offset = (14 - len) * 3;

    for (uint16_t i = offset + 1 * 84 - 4; i < offset + 1 * 84 + len * 6 + 4; i++) {
        virtuelldisp[i] |= 0xf0;
        virtuelldisp[i + 1 * 84] = 0x00;
        virtuelldisp[i + 2 * 84] |= 0x0f;
    }

    for (uint16_t i = offset + 2 * 84 - 4; i < offset + 2 * 84; i++) {
        virtuelldisp[i] = 0xff;
        virtuelldisp[i + len * 6 + 4] = 0xff;
    }

    vd_write_string(offset, 2 * 8, text, INVERTED);

    draw_bmp_pixel(0, 0, virtuelldisp, 84, 48);
}

void LCD8448::vd_question(const char *question, uint8_t aktiv) {
    for (uint16_t i = 0; i < 504; i++) {
        if (i % 2)
            virtuelldisp[i] |= 0x55;
    }

    uint8_t len = strlen(question);
    uint8_t offset = (14 - len) * 3;

    uint16_t start = offset - 4;
    uint16_t ende = offset + 4 + len * 6;

    if (start > 9)
        start = 9;
    if (ende < 75)
        ende = 75;

    for (uint16_t i = start + 1 * 84; i < ende + 1 * 84; i++) {
        virtuelldisp[i] |= 0xf0;
        virtuelldisp[i + 1 * 84] = 0x00;
        virtuelldisp[i + 2 * 84] = 0x7F;
        virtuelldisp[i + 3 * 84] = 0x00;
        virtuelldisp[i + 4 * 84] |= 0x0f;
    }

    for (uint16_t i = start; i < offset; i++) {
        virtuelldisp[i + 2 * 84] = 0xFF;
        virtuelldisp[i + 2 * 84 + ende - offset] = 0xFF;
    }

    for (uint16_t i = start; i < 12; i++) {
        virtuelldisp[i + 3 * 84] = 0xff;
        virtuelldisp[i + 4 * 84] = 0xff;
    }
    for (uint16_t i = 72; i < ende; i++) {
        virtuelldisp[i + 3 * 84] = 0xff;
        virtuelldisp[i + 4 * 84] = 0xff;
    }

    vd_write_string(offset, 2 * 8, question, INVERTED);

    if (aktiv == 1) {
        vd_write_line(12, 31, 42, 31);

        for (uint16_t i = 12 + 4 * 84; i < 15 + 4 * 84; i++) {
            virtuelldisp[i] = 0xff;
            virtuelldisp[i + 27] = 0xff;
        }
    } else if (aktiv == 2) {
        vd_write_line(42, 31, 72, 31);
    }

    vd_write_string(15, 4 * 8, " NO ", (aktiv == 1) ? INVERTED : NORMAL);
    vd_write_string(42, 4 * 8, " YES ", (aktiv == 2) ? INVERTED : NORMAL);

    draw_bmp_pixel(0, 0, virtuelldisp, 84, 48);
}

void LCD8448::vd_overlayON(void) {
    for (uint16_t i = 0; i < 504; i++)
        virtuelldisp_temp[i] = virtuelldisp[i];
}

void LCD8448::vd_overlayOFF(void) {
    for (uint16_t i = 0; i < 504; i++)
        virtuelldisp[i] = virtuelldisp_temp[i];

    draw_bmp_pixel(0, 0, virtuelldisp, 84, 48);
}

void LCD8448::vd_battery(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
    // Battery symbol
    unsigned char ch;
    unsigned char *pBatterie;
    pBatterie = (unsigned char *)battery;
    // set_XY(0,5);
    for (char i = 0; i < 8; i++) {
        ch = pgm_read_byte(pBatterie + i + 8 * state);
        vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
    }
}

void LCD8448::vd_wireless(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
    // Network symbol
    unsigned char ch;
    unsigned char *pWireless;
    pWireless = (unsigned char *)wireless;
    // set_XY(0,5);
    for (char i = 0; i < 8; i++) {
        ch = pgm_read_byte(pWireless + i + 8 * state);
        vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
    }
}

void LCD8448::vd_network(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
    // Network symbol
    unsigned char ch;
    unsigned char *pNetwork;
    pNetwork = (unsigned char *)network;
    // set_XY(0,5);
    for (char i = 0; i < 7; i++) {
        ch = pgm_read_byte(pNetwork + i + 7 * state);
        vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
    }
}

void LCD8448::vd_antenna(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
    // Network symbol
    unsigned char ch;
    unsigned char *pAntenna;
    pAntenna = (unsigned char *)antenna;
    // set_XY(0,5);
    for (char i = 0; i < 7; i++) {
        ch = pgm_read_byte(pAntenna + i + 7 * state);
        vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
    }
}
/**************************************************************************************/
#pragma endregion SPECIAL DISPLAY METHODS
/**************************************************************************************/
#pragma endregion VIRTUAL DISPLAY METHODS
#pragma endregion PUBLIC