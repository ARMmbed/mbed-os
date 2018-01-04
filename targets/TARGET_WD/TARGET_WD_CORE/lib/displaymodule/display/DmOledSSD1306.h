/*
 * DmLed.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created:	12.05.2017
 * Author:	Patrick Frischmann
 * EMail:	patrick.frischmann@world-direct.at
 *
 * Description:
 *        Library for Monochrome OLEDs based on SSD1306 driver.
 *
 */ 
#ifndef DMOLEDSSL1306_H
#define DMOLEDSSL1306_H

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "DmOledBase.h"


/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

//#define SSD1306_SPI_USE_NSCK			// uncomment if negated SCK signal is used

// Colors
#define BLACK		0
#define WHITE		1
#define INVERSE		2

// Commands
#define SSD1306_SETCONTRAST								0x81
#define SSD1306_DISPLAYALLON_RESUME						0xA4
#define SSD1306_DISPLAYALLON							0xA5
#define SSD1306_NORMALDISPLAY							0xA6
#define SSD1306_INVERTDISPLAY							0xA7
#define SSD1306_DISPLAYOFF								0xAE
#define SSD1306_DISPLAYON								0xAF

#define SSD1306_SETDISPLAYOFFSET						0xD3
#define SSD1306_SETCOMPINS								0xDA

#define SSD1306_SETVCOMDESELECT							0xDB

#define SSD1306_SETDISPLAYCLOCKDIV						0xD5
#define SSD1306_SETPRECHARGE							0xD9

#define SSD1306_SETMULTIPLEX							0xA8

#define SSD1306_SETLOWCOLUMN							0x00
#define SSD1306_SETHIGHCOLUMN							0x10

#define SSD1306_SETSTARTLINE							0x40

#define SSD1306_MEMORYMODE								0x20
#define SSD1306_COLUMNADDR								0x21
#define SSD1306_PAGEADDR								0x22

#define SSD1306_COMSCANINC								0xC0
#define SSD1306_COMSCANDEC								0xC8

#define SSD1306_SEGREMAP								0xA0

#define SSD1306_CHARGEPUMP								0x8D

#define SSD1306_EXTERNALVCC								0x1
#define SSD1306_SWITCHCAPVCC							0x2

// Scrolling commands
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL					0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL					0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A


/******************************************************
___________________DECLARATION_________________________
******************************************************/
class DmOledSSD1306 : public DmOledBase {
public:
	DmOledSSD1306(PinName mosi, PinName sck, PinName cs, PinName dc, PinName rst = NC);
	virtual ~DmOledSSD1306();
	
	inline bool isDimmed(void) { return _isDimmed; };

	/** @brief Initialize display functionality (needs to be called before any further access).
	 *
     */
	virtual void init(void);
	
	/** @brief Refresh the display, i.e. load previously modified buffer contents into GDDRAM.
	 *
     */
	void refresh(void);
	
	/** @brief Clear the display buffer contents (needs @ref refresh() afterwards).
	 *
     */
	void clearDisplay(void);
	
	/** @brief Load the company logo into the buffer (needs @ref refresh() afterwards).
	 *
     */
	void loadLogo(void);
	
	/** @brief Invert the display colors.
	 *
	 * @param invert Specify if invert or revert should be performed.
     */
	void invertDisplay(bool invert = true);
	
	/** @brief Dim the display brightness (Reduce contrast).
	 *
	 * @param dim Specify if dim or revert should be performed.
     */
	void dim(bool dim = true);
	
	/** @brief Start scrolling to the right.
	 *
	 *	Overall, we have 8 pages to scroll through (0-7).
	 *
	 * @param start Row where to start.
	 * @param stop Row where to stop.
     */
	void startScrollRight(uint8_t start, uint8_t stop);
	
	/** @brief Start scrolling to the left.
	 *
	 *	Overall, we have 8 pages to scroll through (0-7).
	 *
	 * @param start Row where to start.
	 * @param stop Row where to stop.
     */
	void startScrollLeft(uint8_t start, uint8_t stop);

	/** @brief Start a diagonal scroll to the right.
	 *
	 *	Overall, we have 8 pages to scroll through (0-7).
	 *
	 * @param start Row where to start.
	 * @param stop Row where to stop.
     */
	void startScrollDiagRight(uint8_t start, uint8_t stop);
	
	/** @brief Start a diagonal scroll to the left.
	 *
	 *	Overall, we have 8 pages to scroll through (0-7).
	 *
	 * @param start Row where to start.
	 * @param stop Row where to stop.
     */
	void startScrollDiagLeft(uint8_t start, uint8_t stop);
	
	/** @brief Stop scrolling. After execution RAM data needs to be rewritten.
	 *
     */
	void stopScroll(void);
	
	/** @brief Set a pixel to a specified color. This is the basic operation for more advanced operations.
	 *
	 * @param x Distance on x-axis for coordinate discription.
	 * @param y Distance on y-axis for coordinate discription.
	 * @param color Color code (for monochromatic display we may only use black or white).
     */
	void setPixel(uint16_t x, uint16_t y, uint16_t color);
	
	/** @brief Draw a line in vertical direction.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param length Length of the line to be drawn.
	 * @param color Color code (for monochromatic display we may only use black or white).
     */
	virtual void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
	
	/** @brief Draw a line in horizontal direction.
	 *
	 * @param x Distance of starting point on x-axis for coordinate discription.
	 * @param y Distance of starting point on y-axis for coordinate discription.
	 * @param length Length of the line to be drawn.
	 * @param color Color code (for monochromatic display we may only use black or white).
     */
	virtual void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
	
private:
	void select(void);
	void deSelect(void);
	void enterCommandMode(void);
	void enterDataMode(void);
	
	void writeBus(uint8_t data);
	void sendCommand(uint8_t index);
	void sendData(uint16_t data);
	
	inline void drawVerticalLineInternal(uint16_t x, uint16_t y, uint16_t length, uint16_t color) __attribute__((always_inline));
	inline void drawHorizontalLineInternal(uint16_t x, uint16_t y, uint16_t length, uint16_t color) __attribute__((always_inline));
	
	bool _isDimmed;

	bool _useReset;
	DigitalOut* _pinCS;
	DigitalOut* _pinDC;
	DigitalOut* _pinRST;
	SPI* _spi;
};

#endif // !DMOLEDSSL1306_H