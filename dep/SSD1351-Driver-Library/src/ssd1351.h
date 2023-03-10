//MIT License
//Copyright (c) 2022 Jaime Centeno
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and 
// to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef SSD1351_H
#define SSD1351_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include "fonts.h"

#define OLED_128x128

/*------------------- END OF HARDWARE CONFIGURATION ------------------------------- */

// Static definition for testing purposes on Ceedling

#ifdef OLED_128x128
#define DRAM_SIZE_8 32768
#define DRAM_SIZE_16 16384
#define COLUMNS 128
#define ROWS 128
#endif // OLED_128x128

union DisplayRAM{
  uint8_t byte[DRAM_SIZE_8];
  uint16_t halfw[DRAM_SIZE_16];
};

struct cursor
{
    uint8_t x;
    uint8_t y;
} extern SSD1351_cursor;

// SSD1351 Commands
#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET   0xA2
#define SSD1351_CMD_DISPLAYALLOFF   0xA4
#define SSD1351_CMD_DISPLAYALLON    0xA5
#define SSD1351_CMD_NORMALDISPLAY   0xA6
#define SSD1351_CMD_INVERTDISPLAY   0xA7
#define SSD1351_CMD_FUNCTIONSELECT  0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON       0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL          0xB4
#define SSD1351_CMD_SETGPIO         0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT          0xB9
#define SSD1351_CMD_PRECHARGELEVEL  0xBB
#define SSD1351_CMD_VCOMH           0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO        0xCA
#define SSD1351_CMD_COMMANDLOCK     0xFD
#define SSD1351_CMD_HORIZSCROLL     0x96
#define SSD1351_CMD_STOPSCROLL      0x9E
#define SSD1351_CMD_STARTSCROLL     0x9F

// Some color definitions
#define COLOR_BLUE 0x00F8
#define COLOR_RED 0x1F00
#define COLOR_GREEN 0xE007
#define COLOR_YELLOW 0xFF07
#define COLOR_PURPLE 0x1FF8
#define COLOR_AQUA 0xE0FF
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 3

uint16_t SSD1351_get_rgb(uint8_t r, uint8_t g, uint8_t b);

void SSD1351_ClearDCPin();
void SSD1351_ClearCSPin();
void SSD1351_ClearResetPin();

void SSD1351_DelayMs(uint32_t ms);
void SSD1351_SendBuffer(uint8_t *buffer, uint32_t size);
void SSD1351_SendByte(uint8_t val);

void SSD1351_SetDCPin();
void SSD1351_SetCSPin();
void SSD1351_SetResetPin();

void SSD1351_init(void);

void SSD1351_turn_off(void);

void SSD1351_turn_on(void);

void SSD1351_fill(uint16_t color);

void SSD1351_write_pixel(uint8_t x, uint8_t y, uint16_t color);

void SSD1351_update(void);

void SSD1351_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);

void SSD1351_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void SSD1351_draw_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);

void SSD1351_draw_filled_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void SSD1351_draw_circle(uint8_t xc, uint8_t yc, uint8_t r, uint16_t color);

void SSD1351_draw_filled_circle(uint8_t xc, uint8_t yc, uint8_t r, uint16_t color);

void SSD1351_printf(uint16_t color, font_t font, const char *format, ...);
void SSD1351_write_string(uint16_t color, font_t &font, const char *line, uint8_t alignment = ALIGN_LEFT);

void SSD1351_set_cursor(uint8_t x, uint8_t y);

void SSD1351_draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *data, uint16_t color);

#endif //SSD1351_H
