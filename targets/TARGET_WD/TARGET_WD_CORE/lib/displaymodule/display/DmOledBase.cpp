/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "DmOledBase.h"
#include "BitIO.h"
#include "font.h"

/******************************************************
___________________DEFINES_____________________________
******************************************************/
//#define FONT_CHAR_WIDTH		8
//#define FONT_CHAR_HEIGHT	16
//extern uint8_t font[];
//
//#define read_font_line(__char, __line) \
//      font[((uint16_t)(__char))*FONT_CHAR_HEIGHT+(__line)]

	
/******************************************************
___________________IMPLEMENTATION______________________
******************************************************/
void DmOledBase::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    
    if (steep)
    {
        swap_pixel(x0, y0);
        swap_pixel(x1, y1);
    }
    
    if (x0 > x1)
    {
        swap_pixel(x0, x1);
        swap_pixel(y0, y1);
    }
    
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int16_t ystep;
    
    if (y0 < y1)
        ystep = 1;
    else
        ystep = -1;
    
    for (; x0<=x1; x0++)
    {
        if (steep)
            setPixel(y0, x0, color);
        else
            setPixel(x0, y0, color);
 
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void DmOledBase::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color) {
	drawLine(x, y, x+length-1, y, color);
}
 
void DmOledBase::drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color) {
	drawLine(x, y, x, y+length-1, color);
}

void DmOledBase::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
	drawHorizontalLine(x, y, width, color);
	drawHorizontalLine(x, y+height-1, width, color);
	drawVerticalLine(x, y, height, color);
	drawVerticalLine(x+width-1, y, height, color);
}
 
void DmOledBase::fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
	for (int16_t i=x; i<x+width; i++)
        drawVerticalLine(i, y, height, color); 
}
 
void DmOledBase::fillScreen(uint16_t color) {
	fillRectangle(0, 0, _width, _height, color);
}

size_t DmOledBase::writeChar(uint8_t c) {
	
	if (c == '\n')
    {
        cursorY += textsize*8;
        cursorX = 0;
    }
    else if (c == '\r')
        cursorX = 0;
    else
    {
        drawChar(cursorX, cursorY, c, _fgColor, _bgColor, textsize);
        cursorX += textsize*6;
        if (wrap && (cursorX > (_width - textsize*6)))
        {
            cursorY += textsize*8;
            cursorX = 0;
        }
    }
    return 1;
}

void DmOledBase::drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg, uint8_t size) {
	if(
        (x >= _width) ||			// Clip right
        (y >= _height) ||			// Clip bottom
        ((x + 5 * size - 1) < 0) || // Clip left
        ((y + 8 * size - 1) < 0)	// Clip top
        )
    return;
    
    for (int8_t i=0; i<6; i++ )
    {
        uint8_t line = 0;
 
        if (i == 5) 
            line = 0x0;
        else 
            line = font[(ch*5)+i];
            
        for (int8_t j = 0; j<8; j++)
        {
            if (line & 0x1)
            {
                if (size == 1) // default size
                    setPixel(x+i, y+j, color);
                else // big size
                    fillRectangle(x+(i*size), y+(j*size), size, size, color);
            }
            line >>= 1;
        }
    }
}
 
void DmOledBase::drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* bitmap, uint16_t color) {
	for (int16_t j=0; j<height; j++)
	{
		for (int16_t i=0; i<width; i++ )
		{
			if (bitmap[i + (j/8)*width] & BIT_VALUE(j%8))
				setPixel(x+i, y+j, color);
		}
	}
}

void DmOledBase::setRotation(uint8_t r) {
	r %= 4;  // cant be higher than 3
    rotation = r;
    switch (r)
    {
        case 0:
        case 2:
            _width = _rawWidth;
            _height = _rawHeight;
            break;
        case 1:
        case 3:
            _width = _rawHeight;
            _height = _rawWidth;
            break;
    }
}