/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "DmUI.h"


/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define BUTTON1_BIT			0x0
#define BUTTON2_BIT			0x1
#define BUTTON3_BIT			0x2
#define BUTTON4_BIT			0x3
#define LIGHTSENSOR_MASK	0x30
#define HW_VERSION_MASK		0xC0

#define LED1	0x0
#define LED2	0x1

#define LED1_GREEN_BIT		0x0
#define LED1_BLUE_BIT		0x1
#define LED1_RED_BIT		0x2
#define LED2_GREEN_BIT		0x3
#define LED2_BLUE_BIT		0x4
#define LED2_RED_BIT		0x5

/******************************************************
___________________IMPLEMENTATION______________________
******************************************************/
static void donothing() {}

DmUI::DmUI(PinName mosi, PinName miso, PinName sck, PinName cs, PinName irq) : _pinCS(cs, 0), _pinIRQ(irq), _spi(mosi, miso, sck), _timer(), _queue(4 * EVENTS_EVENT_SIZE) {
	
	// start event queue dispatch thread
	//this->_eventThread.start(callback(&_queue, &EventQueue::dispatch_forever));
	
	_spi.format(8, 0);
	_spi.frequency(1000000);
	
	led1_off();
	led2_off();
	
	// read hw version with 3 retries
	for (int i=0; i<3; i++) {
		int inputRegisterState = shift();
		if (inputRegisterState >= 0) {
			this->_hwVersion = (uint8_t)((inputRegisterState & HW_VERSION_MASK) >> 6);
			break;
		}		
	}
	
	_pinIRQ.fall(callback(this, &DmUI::onButtonPressed));
	_pinIRQ.enable_irq();
	
	for (int i = 0; i < sizeof _irq / sizeof _irq[0]; i++) {
        _irq[i].attach(donothing);
    }
}

void DmUI::onButtonPressed(void) {
	
	// Read register contents
	int result = shift();
	
	if (result < 0) return; // read failed
	
	// call attached function
	uint8_t res = ~((uint8_t) result); // active low
	
	if (BIT_CHECK(&res, BUTTON1_BIT)) {
		_irq[Button1].call();
	} else if (BIT_CHECK(&res, BUTTON2_BIT)) {
		_irq[Button2].call();
	} else if (BIT_CHECK(&res, BUTTON3_BIT)) {
		_irq[Button3].call();
	} if (BIT_CHECK(&res, BUTTON4_BIT)) {
		_irq[Button4].call();
	}
}

void DmUI::attach(Callback<void()> func, IrqSource type /* = Button1 */) {
	if(func){
		_irq[type].attach(_queue.event(func));
	} else {
		_irq[type].attach(donothing);
	}
}

void DmUI::detach(IrqSource type /* = Button1 */) {
	_irq[type].attach(donothing);
}

void DmUI::startPolling(uint16_t ms) {
	_timer.attach(this, &DmUI::onButtonPressed, ((float) ms)/1000.0f );
}

void DmUI::stopPolling(void) {
	_timer.detach();
}

int DmUI::shift(void) {
	
	// Trigger a parallel Load to latch the state of the data lines from the button shift-register
	_pinCS.write(0);
	wait_us(PULSE_WIDTH_USEC);
	
	// R/W contents of both shift-registers
	int result = _spi.write(_srStateCache);
	
	// Trigger a store for the LED shift-register
	wait_us(PULSE_WIDTH_USEC);
	_pinCS.write(1);
		
	return result;
}

void DmUI::led_on(uint8_t led, uint32_t color) {
	char red = (color & LED_COLOR_RED) >> 16;
	char green = (color & LED_COLOR_GREEN) >> 8;
	char blue = (color & LED_COLOR_BLUE);
	
	switch(led) {
		case LED1:
			BIT_BOOL_SET(&_srStateCache, LED1_RED_BIT, red==0);
			BIT_BOOL_SET(&_srStateCache, LED1_GREEN_BIT, green==0);
			BIT_BOOL_SET(&_srStateCache, LED1_BLUE_BIT, blue==0);
			break;
		
		case LED2:
			BIT_BOOL_SET(&_srStateCache, LED2_RED_BIT, red==0);
			BIT_BOOL_SET(&_srStateCache, LED2_GREEN_BIT, green==0);
			BIT_BOOL_SET(&_srStateCache, LED2_BLUE_BIT, blue==0);
			break;
		
		default: 
			break;
	}
	shift();
}

void DmUI::led_off(uint8_t led) {
	led_on(led, LED_COLOR_BLACK);
}

void DmUI::led1_on(uint32_t color) {
	led_on(LED1, color);
}

void DmUI::led1_off(void) {
	led_off(LED1);
}

void DmUI::led2_on(uint32_t color) {
	led_on(LED2, color);
}

void DmUI::led2_off(void) {
	led_off(LED2);
}

uint8_t DmUI::getLightSensorState(void){
	
	// read light sensor State with 3 retries
	for (int i=0; i<3; i++) {
		int inputRegisterState = shift();
		if (inputRegisterState >= 0) {
			return (uint8_t)((inputRegisterState & LIGHTSENSOR_MASK) >> 4);
		}		
	}
	
}