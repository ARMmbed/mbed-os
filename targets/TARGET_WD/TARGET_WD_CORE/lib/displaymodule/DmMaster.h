/*
 * DmMaster.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created:	17.05.2017
 * Author:	Patrick Frischmann
 * EMail:	patrick.frischmann@world-direct.at
 *
 * Description:
 *        Enables control of WD Display Module.
 *
 */ 

#ifndef DMMASTER_H
#define DMMASTER_H
				
/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "DmUI.h"
#include "DmOledSSD1306.h"


/******************************************************
___________________DEFINES_____________________________
******************************************************/



/******************************************************
___________________DECLARATION_________________________
******************************************************/
class DmMaster {
public:
	DmMaster(PinName mosi, PinName miso, PinName sck, PinName csDisplay, PinName csPeripheral, PinName oledA0, PinName wakeup);
	~DmMaster(){};

	/** @brief Show the company logo.
	 *
     */
	void printLogo(void);
	
	/** @brief Clear the screen.
	 *
     */
	void cls(void);
	
	enum ScrollDirection {
        Right = 0,
        Left,
		DiagonalRight,
		DiagonalLeft,
        Off
    };
	
	/** @brief Enable scrolling.
	 *
	 * @param direction Scrolling direction.
     */
	void scroll(ScrollDirection direction);
	
	/** @brief Start the display demo.
	 *
     */
	void startDemo(void);
	
	/** @brief Stop the display demo.
	 *
     */
	void stopDemo(void);
	
	/** @brief Check module connection
	 *
     */
	bool isConnected(void);
	
	DmOledSSD1306 Display;
	DmUI UI;
	
protected:
	// for Demo purposes
	void toggleLedSelection(void);
	void toggleLedColor(uint32_t color);

	// cb stubs
	void _toggleLedColor_Red(void) { toggleLedColor(LED_COLOR_RED); }
	void _toggleLedColor_Green(void) { toggleLedColor(LED_COLOR_GREEN); }
	void _toggleLedColor_Blue(void) { toggleLedColor(LED_COLOR_BLUE); }

private:
	bool Led1Active = true;
	Ticker _refreshTicker;
	
	void refreshDisplay(void);
};
	
#endif // !DMMASTER_H


