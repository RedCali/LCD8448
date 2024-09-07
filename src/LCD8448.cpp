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

#include "LCD8448_ArduinoLogo.h"
#include "LCD8448_font.h"
#include "LCD8448_font_big.h"
#include "LCD8448_font_h.h"
#include "LCD8448_symbols.h"

#pragma region PRIVATE
#pragma region INTERNAL METHODS

void LCD8448::convertFloat(char *buf, float num, int width, uint8_t prec) {
  char format[10];
  sprintf(format, "%%%i.%if", width, prec);
  sprintf(buf, format, num);
}

#pragma endregion INTERNAL METHODS
#pragma endregion PRIVATE

#pragma region PUBLIC
LCD8448::LCD8448(LCD_Mode initMode, LCD_Display backlightInverted) {
  // Set initial startup mode for Display controller
  _initMode = initMode;
  // Set backlight inversion flag
  _backlightInverted = backlightInverted;
}

#pragma region GENERAL METHODS
/**************************************************************************************/
void LCD8448::setContrast(uint8_t contrast) {
  if (contrast > LCD_CONTRAST_MAX)
    contrast = LCD_CONTRAST_MAX;
  if (contrast < LCD_CONTRAST_MIN)
    contrast = LCD_CONTRAST_MIN;
  writeCommand(PCD8544_FUNCTION_SET | PCD8544_EXTENDED_INSTRUCTION);
  writeCommand(PCD8544_SET_VOP | contrast);
  writeCommand(PCD8544_FUNCTION_SET);
  _contrast = contrast;
}

void LCD8448::enableSleep() {
  _sleep = true;
  writeCommand(PCD8544_SET_XY_COLUM_X);
  writeCommand(PCD8544_SET_XY_COLUM_X);
  for (int i = 0; i < 504; ++i)
    writeCommand(0);
  writeCommand(PCD8544_FUNCTION_SET | PCD8544_POWERDOWN);
}

void LCD8448::disableSleep() {
  writeCommand(PCD8544_FUNCTION_SET | PCD8544_EXTENDED_INSTRUCTION);
  writeCommand(PCD8544_SET_VOP | _contrast);
  writeCommand(PCD8544_SET_TEMP | LCD_TEMP);
  writeCommand(PCD8544_SET_BIAS | LCD_BIAS);
  writeCommand(PCD8544_FUNCTION_SET);
  writeCommand(PCD8544_DISPLAY_CONTROL | PCD8544_DISPLAY_NORMAL);
  _sleep = false;
}

void LCD8448::clear(void) {
  if (_sleep) return;

  writeCommand(0x0c);
  // setXY(0, 0);
  writeCommand(PCD8544_SET_XY_COLUM_X);  // X / column
  writeCommand(PCD8544_SET_XY_ROW_Y);    // Y / row
  for (int i = 0; i < 504; i++)          // 6*84
    writeData(0x00);
}

void LCD8448::clearRow(int row, int startX, int endX) {
  if (_sleep) return;

  setXY(startX, row);
  for (int c = startX; c <= endX; c++)
    writeData(0x00);
}
/**************************************************************************************/
#pragma endregion GENERAL METHODS

#pragma region DIRECT METHODS
/**************************************************************************************/
void LCD8448::drawArduinoLogo() {
  if (_sleep) return;
  unsigned char *pLogo = (unsigned char *)arduino_logo;
  setXY(0, 0);
  for (uint16_t i = 0; i < 504; i++) {
    writeData(pgm_read_byte(pLogo + i));
  }
}

void LCD8448::draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map) {
  if (_sleep) return;
  draw_bmp_pixel(X, Y, map, 84, 48);
}

void LCD8448::draw_bmp_pixel(uint8_t X, uint8_t Y, const unsigned char *map, uint8_t pixelsWidth, uint8_t pixelHeight) {
  if (_sleep) return;
  uint8_t row;

  if (pixelHeight % 8 == 0) {
    row = pixelHeight / 8;  // row from 1 to 6;Pix_y from R0 to R47
  } else {
    row = pixelHeight / 8 + 1;  // Quotient + 1
  }
  for (uint8_t n = 0; n < row; n++) {
    setXY(X, Y);
    for (uint8_t i = 0; i < pixelsWidth; i++) {
      writeData(map[i + n * pixelsWidth]);
    }
    Y++;
  }
}

void LCD8448::draw_bmp_pixel_P(uint8_t X, uint8_t Y, const unsigned char *map) {
  if (_sleep) return;
  setXY(X, Y);
  for (uint16_t i = 0; i < 504; i++) {
    writeData(pgm_read_byte(map + i));
  }
}

void LCD8448::write_char(unsigned char c, LCD_Display mode) {
  if (_sleep) return;
  unsigned char *pFont = (unsigned char *)font6_8;
  c -= 32;  // the ASCII of "SP" is 32

  for (uint8_t line = 0; line < 6; line++) {
    unsigned char ch = pgm_read_byte(pFont + c * 6 + line);  // pgm_read_byte(address_short),read c from the font6_8[][6](the detail information is in the "font6x8.h")
    writeData((mode == NORMAL) ? ch : (ch ^ 0xff));          // NORMAL=0,True:return ch;False:return ch^0xff
  }
}

void LCD8448::write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
  if (_sleep) return;
#if defined(LCD_DEBUG)
#if defined(ARDUINO) && ARDUINO >= 100
  Serial.print("LCD writeString: ");
  Serial.println(str);
#endif
#endif

  setXY(X, Y);
  while (*str) {
    write_char(*str, mode);
    str++;
  }
}

void LCD8448::write_char_big(uint8_t X, uint8_t Y, unsigned char c, LCD_Display mode) {
  if (_sleep) return;
  unsigned char *pFont = (unsigned char *)big_number;

  if (c == '.') {
    c = 10;
  } else if (c == '+') {
    c = 11;
  } else if (c == '-') {
    c = 12;
  } else {
    c = c & 0x0f;
  }

  for (uint8_t i = 0; i < 3; i++) {
    setXY(X, Y + i);

    for (uint8_t j = 0; j < 16; j++) {
      unsigned char ch = pgm_read_byte(pFont + c * 48 + i * 16 + j);
      writeData((mode == NORMAL) ? ch : (ch ^ 0xff));
    }
  }
}

void LCD8448::write_string_big(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
  if (_sleep) return;
  while (*str) {
    write_char_big(X, Y, *str, mode);

    if (*str++ == '.') {
      X += 5;
    } else {
      X += 12;
    }
  }
}

void LCD8448::write_num_integer(uint8_t X, uint8_t Y, long num, int length, char filler, LCD_Display mode) {
  if (_sleep) return;
  char buf[25];
  char st[27];
  int c = 0;

  if (num == 0) {
    if (length != 0) {
      for (c = 0; c < (length - 1); c++)
        st[c] = filler;
      st[c] = 48;
      st[c + 1] = 0;
    } else {
      st[0] = 48;
      st[1] = 0;
    }
  } else {
    int f = 0;
    uint8_t neg = (num < 0);
    if (neg) {
      num = -num;
    }

    while (num > 0) {
      buf[c] = 48 + (num % 10);
      c++;
      num = (num - (num % 10)) / 10;
    }
    buf[c] = 0;

    if (neg) {
      st[0] = 45;
    }

    if (length > (c + neg)) {
      for (int i = 0; i < (length - c - neg); i++) {
        st[i + neg] = filler;
        f++;
      }
    }

    for (int i = 0; i < c; i++) {
      st[i + neg + f] = buf[c - i - 1];
    }
    st[c + neg + f] = 0;
  }
  write_string(X, Y, st, mode);
}

void LCD8448::write_num_float(uint8_t X, uint8_t Y, float num, int length, uint8_t dec, char divider, char filler, LCD_Display mode) {
  if (_sleep) return;
  char st[27];

  convertFloat(st, num, length, dec);
  // Replace divider sign
  if (divider != '.') {
    for (uint8_t i = 0; i < sizeof(st); i++)
      if (st[i] == '.')
        st[i] = divider;
  }

  if (filler != ' ') {
    if (num < 0) {  // Number is negative
      st[0] = '-';
      for (uint8_t i = 1; i < sizeof(st); i++)
        if ((st[i] == ' ') || (st[i] == '-'))
          st[i] = filler;
    } else {
      for (uint8_t i = 0; i < sizeof(st); i++)
        if (st[i] == ' ')
          st[i] = filler;
    }
  }
  write_string(X, Y, st, mode);
}
/**************************************************************************************/
#pragma endregion DIRECT METHODS

#pragma region SPECIAL METHODS
/**************************************************************************************/
void LCD8448::write_chinese(uint8_t X, uint8_t Y, const unsigned char *c, uint8_t charWith, uint8_t num, uint8_t row, LCD_Display mode) {
  if (_sleep) return;
  unsigned char i, n;

  setXY(X, Y);
  for (i = 0; i < num;) {
    for (n = 0; n < charWith * 2; n++) {
      if (n == charWith) {
        if (i == 0) {
          setXY(X, Y + 1);
        } else {
          setXY((X + (charWith + row) * i), Y + 1);
        }
      }

      char ch = c[(i * charWith * 2) + n];
      writeData((mode == NORMAL) ? ch : (ch ^ 0xff));
    }
    i++;
    setXY((X + (charWith + row) * i), Y);
  }
}

unsigned char LCD8448::prop_write_char(char c, LCD_Display mode) {
  if (_sleep) return ' ';
  int line, line_s = 0, line_e = 2;
  unsigned char *pFont = (unsigned char *)font6_8;

  if (c -= 32) {
    for (line_s = 0; line_s < 6; line_s++) {
      if ((unsigned char)(*pFont + c * 6 + line_s)) {
        break;
      }
    }
    for (line_e = 5; line_e < 0; line_e--) {
      if ((unsigned char)(*pFont + c * 6 + line_e)) {
        break;
      }
    }
  }

  for (line = line_s; line < line_e + 1; line++) {
    unsigned char ch = (unsigned char)(*(pFont + c * 6 + line));
    writeData((mode == NORMAL) ? ch : (ch ^ 0xff));
  }

  writeData((mode == NORMAL) ? 0 : 0xff);
  return ((unsigned char)(line_e + 2 - line_s));
}

void LCD8448::prop_write_string(uint8_t X, uint8_t Y, const char *str, LCD_Display mode) {
  if (_sleep) return;
  setXY(X, Y);
  while (*str) {
    prop_write_char(*str, mode);
    str++;
  }
}
/**************************************************************************************/
void LCD8448::write_number_big(uint8_t X, uint8_t Y, int number, uint8_t comma, uint8_t digits, LCD_Display mode) {
  if (_sleep) return;
  uint8_t index = 0;
  char numberInArray[digits + 2];

  if (comma > 0) {
    digits += 1;
  }

  while (index < digits) {
    numberInArray[digits - index - 1] = '0' + number % 10;
    index++;

    if ((index == comma - 1) && comma != 0) {
      numberInArray[digits - index - 1] = '.';
      index++;
    }
    number /= 10;
  }
  numberInArray[digits] = '\0';

  write_string_big(X, Y, numberInArray, mode);
}

void LCD8448::write_number_big2(uint8_t X, uint8_t Y, uint8_t number, LCD_Display mode) {
  if (_sleep) return;
  unsigned char *ptr = (unsigned char *)font20_24;

  for (uint8_t i = 0; i < 3; i++) {
    setXY(X, i + Y);
    for (uint8_t j = 0; j < 20; j++) {
      unsigned char ch = pgm_read_byte(ptr + j + i * 320 + number * 20);
      writeData((mode == NORMAL) ? ch : (ch ^ 0xff));
    }
  }
}
/*************************************************************************************/
#pragma endregion SPECIAL METHODS

#pragma region VIRTUAL LCD METHODS
/**************************************************************************************/
/*additional drawing functionality*/
void LCD8448::vd_clear(void) {
  for (int i = 0; i < 504; i++) {
    _virtualDisplay[i] = 0;
  }
}

void LCD8448::vd_print(void) {
  if (_sleep) return;
  draw_bmp_pixel(0, 0, _virtualDisplay, 84, 48);
}

void LCD8448::vd_set_pixel(uint8_t X0, uint8_t Y0) {
  _virtualDisplay[X0 + (Y0 / 8) * 84] |= (1 << Y0 % 8);
}

void LCD8448::vd_set_pixel_byte(uint8_t X0, uint8_t Y0, uint8_t c) {
  _virtualDisplay[X0 + Y0 * 84] |= c;
}

void LCD8448::vd_set_pixel_byte_any(uint8_t X0, uint8_t Y0, uint8_t c) {
  unsigned char position = Y0 % 8;
  unsigned char offset = Y0 / 8;

  _virtualDisplay[X0 + offset * 84] |= (c << position);
  offset++;
  if (offset <= 5) {
    _virtualDisplay[X0 + offset * 84] |= (c >> (8 - position));
  }
}

void LCD8448::vd_write_char(uint8_t X, uint8_t Y, char c, LCD_Display mode) {
  unsigned char *pFont = (unsigned char *)font6_8;
  c -= 32;  // the ASCII of "SP" is 32

  if (Y % 8 == 0) {
    for (uint8_t line = 0; line < 6; line++) {
      unsigned char ch1 = pgm_read_byte(pFont + c * 6 + line);
      vd_set_pixel_byte(X + line, Y / 8, (mode == NORMAL) ? ch1 : (ch1 ^ 0xff));
    }
  } else {
    unsigned char position = Y % 8;

    for (uint8_t line = 0; line < 6; line++) {
      unsigned char ch1 = pgm_read_byte(pFont + c * 6 + line);
      unsigned char ch2 = (ch1 << position);
      ch1 = (ch1 >> (8 - position));
      vd_set_pixel_byte(X + line, Y / 8, (mode == NORMAL) ? ch2 : (ch2 ^ (0xff << position)));
      if (Y != 5) {
        vd_set_pixel_byte(X + line, Y / 8 + 1, (mode == NORMAL) ? ch1 : (ch1 ^ (0xff >> (8 - position))));
      }
    }
  }
}

void LCD8448::vd_write_char_v(uint8_t X, uint8_t Y, char c, LCD_Display mode) {
  unsigned char *pFont = (unsigned char *)font6_8_h;

  Y -= 6;
  if (c >= '0' && c <= '9') {
    c -= '0';
  } else if (c == '+') {
    c = 10;
  } else if (c == 'c') {
    c = 11;
  } else if (c == ',') {
    c = 12;
  }

  for (unsigned char i = 0; i < 8; i++) {
    unsigned char ch = pgm_read_byte(pFont + c * 8 + i);
    vd_set_pixel_byte_any(X + i, Y, (mode == NORMAL) ? ch : (ch ^ 0xfc));
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
  int err = dx + dy;

  while (!(X0 == X1 && Y0 == Y1)) {
    vd_set_pixel(X0, Y0);
    // if (X0==X1 && Y0==Y1) break;
    int e2 = 2 * err;
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

void LCD8448::vd_write_rect(uint8_t X0, uint8_t Y0, uint8_t width, uint8_t height) {
  // Horizontal Top
  vd_write_line(X0, Y0, X0 + width, Y0);
  // Horizontal Bottom
  vd_write_line(X0, Y0 + height - 1, X0 + width, Y0 + height - 1);
  // Vertical Left
  vd_write_line(X0, Y0 + height, X0, Y0);
  // Vertical Right
  vd_write_line(X0 + width - 1, Y0, X0 + width - 1, Y0 + height);
}

void LCD8448::vd_write_round_rect(uint8_t X0, uint8_t Y0, uint8_t width, uint8_t height) {
  // Top / Left
  vd_set_pixel(X0 + 1, Y0 + 1);
  // Top / Right
  vd_set_pixel(X0 + width - 2, Y0 + 1);
  // Bottom / Left
  vd_set_pixel(X0 + 1, Y0 + height - 2);
  // Bottom / Right
  vd_set_pixel(X0 + width - 2, Y0 + height - 2);

  // Horizontal Top
  vd_write_line(X0 + 2, Y0, X0 + width - 2, Y0);
  // Horizontal Bottom
  vd_write_line(X0 + 2, Y0 + height - 1, X0 + width - 2, Y0 + height - 1);
  // Vertical Left
  vd_write_line(X0, Y0 + 2, X0, Y0 + height - 2);
  // Vertical Right
  vd_write_line(X0 + width - 1, Y0 + 2, X0 + width - 1, Y0 + height - 2);
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

#pragma region SPECIAL METHODS
/*************************************************************************************/
void LCD8448::vd_write_framework(const char *head, LCD_Display mode) {
  // head in center position
  uint8_t lengthHead = strlen(head);
  uint8_t headCenter = (14 - lengthHead) * 3;

  vd_write_string(headCenter, 0, head, mode);

  for (uint8_t i = 0; i < headCenter; i++) {
    _virtualDisplay[i] = 0xff;
  }

  for (uint8_t i = headCenter + lengthHead * 6; i < 84; i++) {
    _virtualDisplay[i] = 0xff;
  }

  // Frame around the display
  for (uint8_t i = 0; i < 6; i++) {
    _virtualDisplay[84 * i] = 0xff;
    _virtualDisplay[83 + 84 * i] = 0xff;
  }
  // vd_write_line(1, 0, 83, 0);
  vd_write_line(1, 47, 83, 47);

  // vd_battery(10,5,9,NORMAL);
  // vd_write_line(1,38,83,38);
  // vd_write_string(21,39,"22%",NORMAL);
}

void LCD8448::vd_alert(const char *text) {
  for (uint16_t i = 0; i < 504; i++) {
    if (i % 2) {
      _virtualDisplay[i] |= 0x55;
    }
  }

  uint8_t length = strlen(text);
  uint16_t offset = (14 - length) * 3;

  for (uint16_t i = offset + 1 * 84 - 4; i < offset + 1 * 84 + length * 6 + 4; i++) {
    _virtualDisplay[i] |= 0xf0;
    _virtualDisplay[i + 1 * 84] = 0x00;
    _virtualDisplay[i + 2 * 84] |= 0x0f;
  }

  for (uint16_t i = offset + 2 * 84 - 4; i < offset + 2 * 84; i++) {
    _virtualDisplay[i] = 0xff;
    _virtualDisplay[i + length * 6 + 4] = 0xff;
  }

  vd_write_string(offset, 2 * 8, text, INVERTED);

  draw_bmp_pixel(0, 0, _virtualDisplay, 84, 48);
}

void LCD8448::vd_question(const char *question, uint8_t active) {
  for (uint16_t i = 0; i < 504; i++) {
    if (i % 2) {
      _virtualDisplay[i] |= 0x55;
    }
  }

  uint8_t length = strlen(question);
  uint8_t offset = (14 - length) * 3;

  uint16_t start = offset - 4;
  uint16_t end = offset + 4 + length * 6;

  if (start > 9) {
    start = 9;
  }
  if (end < 75) {
    end = 75;
  }

  for (uint16_t i = start + 1 * 84; i < end + 1 * 84; i++) {
    _virtualDisplay[i] |= 0xf0;
    _virtualDisplay[i + 1 * 84] = 0x00;
    _virtualDisplay[i + 2 * 84] = 0x7F;
    _virtualDisplay[i + 3 * 84] = 0x00;
    _virtualDisplay[i + 4 * 84] |= 0x0f;
  }

  for (uint16_t i = start; i < offset; i++) {
    _virtualDisplay[i + 2 * 84] = 0xFF;
    _virtualDisplay[i + 2 * 84 + end - offset] = 0xFF;
  }

  for (uint16_t i = start; i < 12; i++) {
    _virtualDisplay[i + 3 * 84] = 0xff;
    _virtualDisplay[i + 4 * 84] = 0xff;
  }

  for (uint16_t i = 72; i < end; i++) {
    _virtualDisplay[i + 3 * 84] = 0xff;
    _virtualDisplay[i + 4 * 84] = 0xff;
  }

  vd_write_string(offset, 2 * 8, question, INVERTED);

  if (active == 1) {
    vd_write_line(12, 31, 42, 31);

    for (uint16_t i = 12 + 4 * 84; i < 15 + 4 * 84; i++) {
      _virtualDisplay[i] = 0xff;
      _virtualDisplay[i + 27] = 0xff;
    }
  } else if (active == 2) {
    vd_write_line(42, 31, 72, 31);
  }

  vd_write_string(15, 4 * 8, " NO ", (active == 1) ? INVERTED : NORMAL);
  vd_write_string(42, 4 * 8, " YES ", (active == 2) ? INVERTED : NORMAL);

  draw_bmp_pixel(0, 0, _virtualDisplay, 84, 48);
}

void LCD8448::vd_overlayON(void) {
  for (uint16_t i = 0; i < 504; i++) {
    _virtualDisplayTemp[i] = _virtualDisplay[i];
  }
}

void LCD8448::vd_overlayOFF(void) {
  for (uint16_t i = 0; i < 504; i++) {
    _virtualDisplay[i] = _virtualDisplayTemp[i];
  }

  draw_bmp_pixel(0, 0, _virtualDisplay, 84, 48);
}
/**************************************************************************************/
#pragma endregion SPECIAL METHODS

#pragma region SPECIAL SYMBOL METHODS
/**************************************************************************************/
void LCD8448::vd_symbol(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Symbols symbol, LCD_Display mode) {
  // Symbols
  unsigned char *pSymbol;
  uint8_t width;
  // GEt symbol pointer
  switch (symbol) {
    case ANTENNA_1: {
      pSymbol = (unsigned char *)antenna1;
      width = 7;
      break;
    }
    case ANTENNA_2: {
      pSymbol = (unsigned char *)antenna2;
      width = 7;
      break;
    }
    case ANTENNA_STRENGTH_1: {
      pSymbol = (unsigned char *)antennaStrength1;
      width = 18;
      break;
    }
    case ANTENNA_STRENGTH_2: {
      pSymbol = (unsigned char *)antennaStrength2;
      width = 12;
      break;
    }
    case ARROW_UP_DOWN: {
      pSymbol = (unsigned char *)arrowUpDown;
      width = 12;
      break;
    }
    case BAR_GRAPH: {
      pSymbol = (unsigned char *)barGraph;
      width = 5;
      break;
    }
    case BATTERY: {
      pSymbol = (unsigned char *)battery;
      width = 8;
      break;
    }
    case BLUETHOOTH: {
      pSymbol = (unsigned char *)bluetooth;
      width = 6;
      break;
    }
    case ENVELOPE: {
      pSymbol = (unsigned char *)envelope;
      width = 12;
      break;
    }
    case NETWORK: {
      pSymbol = (unsigned char *)network;
      width = 7;
      break;
    }
    case SD_CARD: {
      pSymbol = (unsigned char *)sdCard;
      width = 7;
      break;
    }
    case SIGNAL_STRENGTH_1: {
      pSymbol = (unsigned char *)signalStrength1;
      width = 13;
      break;
    }
    case SIGNAL_STRENGTH_2: {
      pSymbol = (unsigned char *)signalStrength2;
      width = 7;
      break;
    }
    case WIRELESS: {
      pSymbol = (unsigned char *)wireless;
      width = 8;
      break;
    }
    case ROUND_X1: {
      pSymbol = (unsigned char *)roundX1;
      width = 7;
      break;
    }
    case ROUND_X2: {
      pSymbol = (unsigned char *)roundX2;
      width = 7;
      break;
    }
    default: {
#if defined(LCD_DEBUG)
#if defined(ARDUINO) && ARDUINO >= 100
      Serial.print("Selected Symbol doesn't exist!\nSymbol Index: ");
      Serial.println(mode);
#endif
#endif
      return;
    }
  }

  // write symbol to VD Buffer
  for (char i = 0; i < width; i++) {
    unsigned char ch = pgm_read_byte(pSymbol + i + width * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_antenna1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)antenna1;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 7 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_antenna2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)antenna2;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 7 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_antennaStrength1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)antennaStrength1;
  for (char i = 0; i < 18; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 18 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_antennaStrength2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)antennaStrength2;
  for (char i = 0; i < 12; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 12 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_arrowUpDown(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)arrowUpDown;
  for (char i = 0; i < 12; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 12 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_barGraph(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)barGraph;
  for (char i = 0; i < 5; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 5 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_battery(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)battery;
  for (char i = 0; i < 8; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 8 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_bluetooth(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)bluetooth;
  for (char i = 0; i < 6; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 6 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_envelope(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Battery symbol
  unsigned char *pImages = (unsigned char *)envelope;
  for (char i = 0; i < 12; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 12 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_network(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)network;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 7 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_sdCard(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)sdCard;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 7 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_signalStrength1(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)signalStrength1;
  for (char i = 0; i < 13; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 13 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_signalStrength2(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)signalStrength2;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 7 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_wireless(uint8_t X0, uint8_t Y0, uint8_t state, LCD_Display mode) {
  // Network symbol
  unsigned char *pImages = (unsigned char *)wireless;
  for (char i = 0; i < 8; i++) {
    unsigned char ch = pgm_read_byte(pImages + i + 8 * state);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_roundX1(uint8_t X0, uint8_t Y0, LCD_Display mode) {
  // Round X Symbol symbol
  unsigned char *pImages = (unsigned char *)roundX1;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}

void LCD8448::vd_roundX2(uint8_t X0, uint8_t Y0, LCD_Display mode) {
  // Round X Symbol symbol
  unsigned char *pImages = (unsigned char *)roundX2;
  for (char i = 0; i < 7; i++) {
    unsigned char ch = pgm_read_byte(pImages + i);
    vd_set_pixel_byte(X0 + i, Y0, (mode == NORMAL) ? ch : (ch ^ 0xff));
  }
}
/**************************************************************************************/
#pragma endregion SPECIAL SYMBOL METHODS
/**************************************************************************************/
#pragma endregion VIRTUAL LCD METHODS
#pragma endregion PUBLIC
