#include "ssd1351.h"
#include <string.h>

DisplayRAM *DRAM;
#define DRAM_16 DRAM->halfw
#define DRAM_8 DRAM->byte

/* Screen cursor for printing */
cursor SSD1351_cursor;

/**
 * @brief  Writes command to the SSD1351 OLED Display
 * @param  cmd: command to send
 * @retval None
 */
void SSD1351_write_command(uint8_t cmd)
{
    SSD1351_ClearDCPin();
    SSD1351_ClearCSPin();
    SSD1351_SendByte(cmd);
    SSD1351_SetCSPin();
}

/**
 * @brief  Writes single byte data to the SSD1351 OLED Display
 * @param  data: data byte to send
 * @retval None
 */
void SSD1351_write_data(uint8_t data)
{
    SSD1351_SetDCPin();
    SSD1351_ClearCSPin();
    SSD1351_SendByte(data);
    SSD1351_SetCSPin();
}

/**
 * @brief  Writes a data buffer of bytes to SSD1351 display
 * @param  data: pointer to data buffer to send
 * @param  len: integer with length of buffer to send
 * @retval None
 */
void SSD1351_write_data_buffer(uint8_t *data, uint32_t len)
{
    SSD1351_SetDCPin();
    SSD1351_ClearCSPin();
    SSD1351_SendBuffer(DRAM_8, DRAM_SIZE_8);
    SSD1351_SetCSPin();
}

/*
 * @brief Converts from RGB to a single 16bit value
 * @param r: starting x coordinate
 * @para g: starting y coordinate
 * @param b: width of the rectangle
 * @reval 16bit value with the rgb color for display
 */
uint16_t SSD1351_get_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t rgb_color = 0;
    rgb_color |= ((r / 8) << 8);
    rgb_color |= ((g / 4) >> 3);
    rgb_color |= (((g / 4) % 0x07) << 13);
    rgb_color |= ((b / 8) << 3);
    return rgb_color;
}

/**
 * @brief  Initializes the SSD1351 OLED Display
 * @retval None
 */
void SSD1351_init(void)
{
    DRAM = new DisplayRAM;
    memset(DRAM, 0, DRAM_SIZE_8);

    SSD1351_SetResetPin();
    SSD1351_DelayMs(10);
    SSD1351_ClearResetPin();
    SSD1351_DelayMs(10);
    SSD1351_SetResetPin();
    SSD1351_DelayMs(40);

    SSD1351_write_command(SSD1351_CMD_COMMANDLOCK);
    SSD1351_write_data(0x12);
    SSD1351_write_command(SSD1351_CMD_COMMANDLOCK);
    SSD1351_write_data(0xB1);

    SSD1351_write_command(SSD1351_CMD_DISPLAYOFF);
    SSD1351_write_command(SSD1351_CMD_CLOCKDIV);
    SSD1351_write_data(0xF1);
    SSD1351_write_command(SSD1351_CMD_MUXRATIO);
    SSD1351_write_data(127);

    SSD1351_write_command(SSD1351_CMD_SETREMAP);
    SSD1351_write_data(0x70);

    SSD1351_write_command(SSD1351_CMD_SETCOLUMN);
    SSD1351_write_data(0x00);
    SSD1351_write_data(0x7F);

    SSD1351_write_command(SSD1351_CMD_SETROW);
    SSD1351_write_data(0x00);
    SSD1351_write_data(0x7F);

    SSD1351_write_command(SSD1351_CMD_STARTLINE);
    SSD1351_write_data(0x00);

    SSD1351_write_command(SSD1351_CMD_DISPLAYOFFSET);
    SSD1351_write_data(0x00);

    SSD1351_write_command(SSD1351_CMD_SETGPIO);
    SSD1351_write_data(0x00);

    SSD1351_write_command(SSD1351_CMD_FUNCTIONSELECT);
    SSD1351_write_data(0x01);

    SSD1351_write_command(SSD1351_CMD_PRECHARGE);
    SSD1351_write_data(0x32);

    SSD1351_write_command(SSD1351_CMD_VCOMH);
    SSD1351_write_data(0x05);

    SSD1351_write_command(SSD1351_CMD_NORMALDISPLAY);

    SSD1351_write_command(SSD1351_CMD_CONTRASTABC);
    SSD1351_write_data(0x8A); // Color A: Blue
    SSD1351_write_data(0x51); // Color B: Green
    SSD1351_write_data(0x8A); // Color C: Red

    SSD1351_write_command(SSD1351_CMD_CONTRASTMASTER);
    SSD1351_write_data(0x0F);

    SSD1351_write_command(SSD1351_CMD_SETVSL);
    SSD1351_write_data(0xA0);
    SSD1351_write_data(0xB5);
    SSD1351_write_data(0x55);

    SSD1351_write_command(SSD1351_CMD_PRECHARGE2);
    SSD1351_write_data(0x01);

    SSD1351_write_command(SSD1351_CMD_WRITERAM);
    SSD1351_write_data_buffer(DRAM_8, DRAM_SIZE_8);

    SSD1351_write_command(SSD1351_CMD_DISPLAYON);
}

/**
 * @brief  Turns off the SSD1351 OLED Display
 * @retval None
 */
void SSD1351_turn_off(void)
{
    SSD1351_write_command(SSD1351_CMD_DISPLAYOFF);
}

/**
 * @brief  Turns on the SSD1351 OLED Display
 * @retval None
 */
void SSD1351_turn_on(void)
{
    SSD1351_write_command(SSD1351_CMD_DISPLAYON);
}

/**
 * @brief  Turns off the SSD1351 OLED Display
 * @param  color: Unsigned int16 containing color code
 * @retval None
 */
void SSD1351_fill(uint16_t color)
{
    for (int i = 0; i < DRAM_SIZE_16; i++)
        DRAM_16[i] = color;
    // SSD1351_write_command(SSD1351_CMD_STOPSCROLL);
}

/**
 * @brief  Updates the screen RAM
 * @retval None
 */
void SSD1351_update(void)
{
    SSD1351_write_command(SSD1351_CMD_WRITERAM);
    SSD1351_write_data_buffer(DRAM_8, DRAM_SIZE_8);
}

/**
 * @brief  Updates a specific area within the display
 * @retval None
 */
/*void SSD1351_update_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
  SSD1351_write_command(SSD1351_CMD_SETCOLUMN);
  SSD1351_write_data(x0);
  SSD1351_write_data(y0);

  SSD1351_write_command(SSD1351_CMD_SETROW);
  SSD1351_write_data(x1);
  SSD1351_write_data(x0);

  int a0 = x0 + (y0 * 128);
  int a1 = x1 + (y1 * 128);

  SSD1351_write_command(SSD1351_CMD_WRITERAM);
  for (int i = a0; i < a1 * 2; i++){
    SSD1351_write_data(DRAM_8[i]);
  }

  // Back to default settings

  SSD1351_write_command(SSD1351_CMD_SETCOLUMN);
  SSD1351_write_data(x0);
  SSD1351_write_data(y0);

  SSD1351_write_command(SSD1351_CMD_SETROW);
  SSD1351_write_data(x1);
  SSD1351_write_data(x0);
}*/

/**
 * @brief Writes a pixel data to the screen RAM buffer
 * @param color: Unsigned int16 containing color code
 * @param x: Pixel's horizontal position
 * @param y: Pixel's vertical position
 * @retval None
 */
void SSD1351_write_pixel(int16_t x, int16_t y, uint16_t color)
{
    DRAM_16[x + COLUMNS * y] = color;
}

/*  LINE DRAWING FUNCTIONS */

void SSD1351_draw_line_low(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int16_t D = 2 * dy - dx;
    int16_t y = y0;

    if (x0 < x1)
    {
        for (int16_t x = x0; x <= x1; x++)
        {
            SSD1351_write_pixel(x, y, color);
            if (D > 0)
            {
                y = y + yi;
                D = D - 2 * dx;
            }
            D = D + 2 * dy;
        }
    }
    else
    {
        for (int16_t x = x0; x >= x1; x--)
        {
            SSD1351_write_pixel(x, y, color);
            if (D > 0)
            {
                y = y + yi;
                D = D - 2 * dx;
            }
            D = D + 2 * dy;
        }
    }
}

void SSD1351_draw_line_high(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int16_t D = 2 * dx - dy;
    int16_t x = x0;

    if (y0 < y1)
    {
        for (int16_t y = y0; y <= y1; y++)
        {
            SSD1351_write_pixel(x, y, color);
            if (D > 0)
            {
                x = x + xi;
                D = D - 2 * dy;
            }
            D = D + 2 * dx;
        }
    }
    else
    {
        for (int16_t y = y0; y >= y1; y--)
        {
            SSD1351_write_pixel(x, y, color);
            if (D > 0)
            {
                x = x + xi;
                D = D - 2 * dy;
            }
            D = D + 2 * dx;
        }
    }
}

/*
 * @brief Draws a line from specified parameters into display RAM
 * @param x0: starting x coordinate
 * @para y0: starting y coordinate
 * @param x1: ending x coordinate
 * @param y1: ending y coordinate
 * @color: color to use to draw the line
 * @reval None
 */
void SSD1351_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            SSD1351_draw_line_low(x1, y1, x0, y0, color);
        }
        else
        {
            SSD1351_draw_line_low(x0, y0, x1, y1, color);
        }
    }
    else
    {
        if (y0 > y1)
        {
            SSD1351_draw_line_high(x1, y1, x0, y0, color);
        }
        else
        {
            SSD1351_draw_line_high(x0, y0, x1, y1, color);
        }
    }
    return;
}

/*
 * @brief Draws a rectangle with specified dimensions into display RAM
 * @param x0: starting x coordinate
 * @para y0: starting y coordinate
 * @param w: width of the rectangle
 * @oaram h: height of the rectangle
 * @color: color for the border
 * @reval None
 */
void SSD1351_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    SSD1351_draw_line(x, y, x + w, y, color);
    SSD1351_draw_line(x + w, y, x + w, y + h, color);
    SSD1351_draw_line(x + w, y + h, x, y + h, color);
    SSD1351_draw_line(x, y + h, x, y, color);
}

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r,
                      uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4)
        {
            SSD1351_write_pixel(x0 + x, y0 + y, color);
            SSD1351_write_pixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2)
        {
            SSD1351_write_pixel(x0 + x, y0 - y, color);
            SSD1351_write_pixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8)
        {
            SSD1351_write_pixel(x0 - y, y0 + x, color);
            SSD1351_write_pixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1)
        {
            SSD1351_write_pixel(x0 - y, y0 - x, color);
            SSD1351_write_pixel(x0 - x, y0 - y, color);
        }
    }
}
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Trying to optimize line drawing in buffered mode is pretty pointless, it's stupid fast anyway.
    SSD1351_draw_line(x, y, x, y + h - 1, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Trying to optimize line drawing in buffered mode is pretty pointless, it's stupid fast anyway.
    SSD1351_draw_line(x, y, x + w - 1, y, color);
}
/*
 * @brief Draws a rounded rectangle with specified dimensions into display RAM
 * @param x0: starting x coordinate
 * @para y0: starting y coordinate
 * @param w: width of the rectangle
 * @oaram h: height of the rectangle
 * @oaram r: radius of the rectangle's corners
 * @color: color for the border
 * @reval None
 */
void SSD1351_draw_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    drawFastHLine(x + r, y, w - 2 * r, color);         // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right

    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/*
 * @brief Draws a filled rectangle with specified dimensions into display RAM
 * @param x0: starting x coordinate
 * @para y0: starting y coordinate
 * @param w: width of the rectangle
 * @oaram h: height of the rectangle
 * @oaram color: color for the rectangle
 * @reval None
 */
void SSD1351_draw_filled_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    for (int16_t i = x; i < x + w; i++)
        drawFastVLine(i, y, h, color);
}

/*
 * @brief Draws a rotated filled rectangle with specified dimensions into display RAM
 * @param xc: center x coordinate
 * @para yc: center y coordinate
 * @param w: width of the rectangle
 * @param h: height of the rectangle
 * @param r: rotation in degrees
 * @oaram color: color for the rectangle
 * @reval None
 */
/*
void SSD1351_draw_rotated_rect(int16_t xc, int16_t yc, int16_t w, int16_t h, int16_t r, uint16_t color){
  r = r % 90;
  int16_t hyp = sqrt((h * h)/4 + (w * w)/4);
  float wr = 45  + r;
  float rad = (wr/360)*M_PI;
  int16_t x0 = xc - (hyp * sin(rad));
  int16_t y0 = yc + (hyp * cos(rad));
}*/

void draw_circle(int16_t xc, int16_t yc, int16_t x, int16_t y, uint16_t color)
{
    SSD1351_write_pixel(xc + x, yc + y, color);
    SSD1351_write_pixel(xc - x, yc + y, color);
    SSD1351_write_pixel(xc + x, yc - y, color);
    SSD1351_write_pixel(xc - x, yc - y, color);
    SSD1351_write_pixel(xc + y, yc + x, color);
    SSD1351_write_pixel(xc - y, yc + x, color);
    SSD1351_write_pixel(xc + y, yc - x, color);
    SSD1351_write_pixel(xc - y, yc - x, color);
}

void draw_filled_circle(int16_t xc, int16_t yc, int16_t x, int16_t y, uint16_t color)
{
    SSD1351_draw_line(xc - x, yc + y, xc + x, yc + y, color);
    SSD1351_draw_line(xc - x, yc - y, xc + x, yc - y, color);
    SSD1351_draw_line(xc - y, yc + x, xc + y, yc + x, color);
    SSD1351_draw_line(xc - y, yc - x, xc + y, yc - x, color);
}

/*
 * @brief Draws a cicle with specified origin and radius into display RAM
 * @param xc: integer for the x origin coordinate
 * @param yc: integer for the y origin coordinate
 * @param color: color for the border
 * @reval None
 */
void SSD1351_draw_circle(int16_t xc, int16_t yc, uint16_t r, uint16_t color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    draw_circle(xc, yc, x, y, color);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        draw_circle(xc, yc, x, y, color);
    }
}

/*
 * @brief Draws a cicle with specified origin and radius into display RAM
 * @param xc: integer for the x origin coordinate
 * @param yc: integer for the y origin coordinate
 * @param color: color for the circle
 * @reval None
 */
void SSD1351_draw_filled_circle(int16_t xc, int16_t yc, uint16_t r, uint16_t color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    draw_filled_circle(xc, yc, x, y, color);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        draw_filled_circle(xc, yc, x, y, color);
    }
}

void SSD1351_write_char(uint16_t color, font_t &font, char c)
{
    uint16_t fd;
    if (c == '\n')
    {
        SSD1351_cursor.y = SSD1351_cursor.y + font.height;
        SSD1351_cursor.x = 0;
    }
    else
    {
        for (int i = 0; i < font.height; i++)
        {
            fd = font.data[(c - font.first) * font.height + i];

            for (int j = 0; j < font.width; j++)
            {
                if ((fd << j) & 0x80)
                    SSD1351_write_pixel(SSD1351_cursor.x + j, SSD1351_cursor.y + i, color);
            }
        }

        SSD1351_cursor.x += font.width;
    }

    return;
}

void SSD1351_write_string(uint16_t color, font_t &font, const char *line)
{
    while (*line != 0)
    {
        SSD1351_write_char(color, font, *line);
        line++;
    }
}

/*
 * @brief Sets the printing cursor to a positioin
 * @param x: integer for the x position for the cursor
 * @param y: integer for the y position for the cursor
 */

void SSD1351_set_cursor(uint8_t x, uint8_t y)
{
    SSD1351_cursor.x = x;
    SSD1351_cursor.y = y;
}

/*
 * @brief Draws a bitmap
 * @param sp: pointer to struct holding sprite data
 */

void SSD1351_draw_bitmap(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t *data, uint16_t color)
{
    int16_t i, j, byteWidth = (width + 7) / 8;

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            if (data[j * byteWidth + i / 8] & (128 >> (i & 7)))
                SSD1351_write_pixel(x + i, y + j, color);
        }
    }
}
