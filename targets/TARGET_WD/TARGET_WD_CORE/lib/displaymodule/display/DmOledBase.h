/*
 * DmOledBase.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created:	11.05.2017
 * Author:	Patrick Frischmann
 * EMail:	patrick.frischmann@world-direct.at
 *
 * Description:
 *        Enables control of WD Display Module.
 *
 */ 

#ifndef DMOLEDBASE_H
#define DMOLEDBASE_H

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "mbed.h"


/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define BLACK		0
#define WHITE		1


/******************************************************
___________________DECLARATION_________________________
******************************************************/
class DmOledBase : public Stream {

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define swap_pixel(a, b) { int16_t t = a; a = b; b = t; }
	
public:
	DmOledBase(const uint16_t width, const uint16_t height) :
		_rawWidth(width), 
		_rawHeight(height),
		_width(width), 
		_height(height),
		rotation(0),
		cursorX(0),
		cursorY(0),
		_bgColor(BLACK),
		_fgColor(WHITE),
		textsize(1),
		wrap(true)
		{};
	virtual ~DmOledBase() {};
	
	virtual void init(void) = 0;
	
	inline uint16_t width(void) { return _width; }
	inline uint16_t height(void) { return _height; }
	void setWidth(uint16_t width) { _width = width; }
	void setHeight(uint16_t height) { _height = height; }

	inline int16_t x_coordinate(void) { return cursorX; };
	inline int16_t y_coordinate(void) { return cursorY; };
	
	/** @brief Set the color of the text.
	 *
	 * @param background Background color.
	 * @param foreground Foreground color.
     */
	void setTextColor(uint16_t background, uint16_t foreground) { _bgColor = background; _fgColor = foreground; }

	/** @brief Set the cursor for text start position.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
     */
	inline void setTextCursor(int16_t x, int16_t y) { cursorX = x; cursorY = y; };
	
	/** @brief Set the size of the text.
	 *
	 * @param s Size discriptor (default = 1).
     */
	inline void setTextSize(uint8_t s) { textsize = (s > 0) ? s : 1; };
	
	/** @brief Set a pixel to a specified color. This is the basic operation for more advanced operations. Needs to be implemented in derived class.
	 *
	 * @param x Distance on x-axis for coordinate discription.
	 * @param y Distance on y-axis for coordinate discription.
	 * @param color Color code (for monochromatic display we may only use black or white).
     */
	virtual void setPixel(uint16_t x, uint16_t y, uint16_t color) = 0;
	
	/** @brief Invert the display colors. Implementation is optional
	 *
	 * @param invert Specify if invert or revert should be performed.
     */
	virtual void invertDisplay(bool invert = true) {};
	
	// Stream implementation - provides printf() interface
	virtual int _putc(int value) { return writeChar(value); };
    virtual int _getc() { return -1; };
	
	/** @brief Draw a line.
	 *
	 * @param x0 Distance of starting point on x-axis for coordinate discription.
	 * @param y0 Distance of starting point on y-axis for coordinate discription.
	 * @param x1 Distance of end point on x-axis for coordinate discription.
	 * @param y1 Distance of end point on y-axis for coordinate discription.
	 * @param color Color code.
     */
	virtual void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	
	/** @brief Draw a line in vertical direction. Improved implementation may be provided in derived class.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param length Length of the line to be drawn.
	 * @param color Color code.
     */
	virtual void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
	
	/** @brief Draw a line in hoizontal direction. Improved implementation may be provided in derived class.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param length Length of the line to be drawn.
	 * @param color Color code.
     */
	virtual void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
 
	/** @brief Draw a rectangle. Improved implementation may be provided in derived class.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param width Width of the rectangle.
	 * @param height Height of the rectangle.
	 * @param color Color code.
     */
	virtual void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
	
	/** @brief Draw and fill rectangle. Improved implementation may be provided in derived class.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param width Width of the rectangle.
	 * @param height Height of the rectangle.
	 * @param color Color code.
     */
	virtual void fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
 
	/** @brief Fill the whole screen with a specified color.
	 *
	 * @param color Color code.
     */
	virtual void fillScreen(uint16_t color);
	
	/** @brief Draw a character.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param ch The character to be drawn.
	 * @param color Color code.
	 * @param bg The background color.
	 * @param size The size of the character.
     */
	void drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg, uint8_t size);
	
	/** @brief Write a character.
	 *
	 * @param c The character to write.
	 *
	 * @retval 1.
     */
	size_t writeChar(uint8_t c);
	
	/** @brief Draw a bitmap.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param width Width of the bitmap.
	 * @param height Height of the bitmap.
	 * @param bitmap The bitmap to draw.
	 * @param color Color code.
     */
	void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* bitmap, uint16_t color);
 
	/** @brief Set the display rotation.
	 *
	 * @param r Rotation specifier (1, 2, 3 or 4)
     */
    void setRotation(uint8_t r);
    inline uint8_t getRotation(void) { rotation %= 4; return rotation; };
	
protected:
	uint16_t _rawWidth, _rawHeight;	// never changes
	uint16_t _width, _height;		// dependent on rotation
	
	int16_t  cursorX, cursorY;
    uint8_t  textsize;
	bool wrap; // If set, 'wrap' text at right edge of display
	
	uint8_t rotation;
	
	uint16_t _bgColor, _fgColor;
};

#endif // !DMOLEDBASE_H