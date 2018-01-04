/*
 * DmUI.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created:	12.05.2017
 * Author:	Patrick Frischmann
 * EMail:	patrick.frischmann@world-direct.at
 *
 * Description:
 *        Enables Button functionality and LED control of WD Display Module.
 *
 */ 

#ifndef DMUI_H
#define DMUI_H

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "mbed.h"
#include "BitIO.h"
#include "mbed_events.h"
#include "rtos.h"
#include "platform/Callback.h"


/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define LED_COLOR_RED				0xFF0000
#define LED_COLOR_GREEN				0x00FF00
#define LED_COLOR_BLUE				0x0000FF
#define LED_COLOR_BLACK				0x000000
#define LED_COLOR_WHITE				(LED_COLOR_RED | LED_COLOR_GREEN | LED_COLOR_BLUE)
#define LED_COLOR_YELLOW			(LED_COLOR_RED | LED_COLOR_GREEN)
#define LED_COLOR_MAGENTA			(LED_COLOR_RED | LED_COLOR_BLUE)
#define LED_COLOR_CYAN				(LED_COLOR_GREEN | LED_COLOR_BLUE)

#define PULSE_WIDTH_USEC	5


/******************************************************
___________________DECLARATION_________________________
******************************************************/
class DmUI {
public:
	DmUI(PinName mosi, PinName miso, PinName sck, PinName cs, PinName irq);
	~DmUI(){};
	
	enum IrqSource {
        Button1 = 0,
        Button2,
        Button3,
        Button4,

        IrqSourceCnt
    };
	
	/** @brief Attach a function that is invoked after a button press.
	 *
	 * @param func The callback
     * @param type The type of event (button number) to which the execution is bound.
	 */
	void attach(Callback<void()> func, IrqSource type=Button1);
	template<typename T, typename M> void attach(T *obj, M method, IrqSource type=Button1) {
        core_util_critical_section_enter();
        attach(callback(obj, method), type);
        core_util_critical_section_exit();
    }
	
	/** @brief Detach a previously attached function.
	 *
     * @param type The type of event (button number) to which the execution is bound.
     */
	void detach(IrqSource type=Button1);
	
	/** @brief Start polling for a button press (if no interrupt can be used).
	 *
	 * @param ms The polling interval in milliseconds.
	 */
	void startPolling(uint16_t ms);
	
	/** @brief Stop polling for a button press.
	 */
	void stopPolling(void);
	
	/** @brief Turn on LED 1.
	 *
	 * @param color A 24 bit hexcode that describes the color.
	 */
	void led1_on(uint32_t color);
	
	/** @brief Turn off LED 1.
	 *
	 */
	void led1_off(void);
	
	/** @brief Turn on LED 2.
	 *
	 * @param color A 24 bit hexcode that describes the color.
	 */
	void led2_on(uint32_t color);
	
	/** @brief Turn off LED 2.
	 *
	 */
	void led2_off(void);
	
	uint8_t getHwVersion(void) { return _hwVersion; };
	uint8_t getLightSensorState(void);
	
protected:
	void onButtonPressed(void);
	
private:
	void led_on(uint8_t led, uint32_t color);
	void led_off(uint8_t led);
	int shift(void);
	
	InterruptIn _pinIRQ;
	DigitalOut _pinCS;
	SPI _spi;
	Callback<void()> _irq[IrqSourceCnt];
	Ticker _timer;
	
	EventQueue _queue;
	Thread _eventThread;
	
	uint8_t _srStateCache = 0xFF;
	uint8_t _hwVersion;
};	

#endif // !DMUI_H