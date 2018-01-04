/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "DmMaster.h"

/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define DISPLAY_REFRESH_INTERVAL_MS		250

	
/******************************************************
___________________IMPLEMENTATION______________________
******************************************************/

DmMaster::DmMaster(PinName mosi, PinName miso, PinName sck, PinName csDisplay, PinName csPeripheral, PinName oledA0, PinName wakeup) :
	Display(mosi, sck, csDisplay, oledA0), UI(mosi, miso, sck, csPeripheral, wakeup), _refreshTicker() {
		Display.init();
		//_refreshTicker.attach(callback(this, &DmMaster::refreshDisplay), ((float) DISPLAY_REFRESH_INTERVAL_MS)/1000.0f );
}

void DmMaster::printLogo(void) {
	Display.loadLogo();
	this->refreshDisplay();
}

void DmMaster::cls(void) {
	Display.clearDisplay();
	Display.setTextCursor(0,0);
	this->refreshDisplay();
}

void DmMaster::startDemo(void) {
	// demo will allow control of LEDs with buttons and print info on display
	UI.attach(callback(this, &DmMaster::toggleLedSelection), DmUI::Button1);
	UI.attach(callback(this, &DmMaster::_toggleLedColor_Red), DmUI::Button2);
	UI.attach(callback(this, &DmMaster::_toggleLedColor_Green), DmUI::Button3);	
	UI.attach(callback(this, &DmMaster::_toggleLedColor_Blue), DmUI::Button4);	
	
//	UI.startPolling(300);
}

void DmMaster::stopDemo(void) {
//	UI.stopPolling();
	UI.detach(DmUI::Button1);
	UI.detach(DmUI::Button2);
	UI.detach(DmUI::Button3);
	UI.detach(DmUI::Button4);
	cls();
	printLogo();
}

bool DmMaster::isConnected(void) {
	
	return (this->UI.getHwVersion() < 3);	// we will read 0xFF on SPI if NC
	
}

uint32_t led1_color = 0;
uint32_t led2_color = 0;
void DmMaster::toggleLedColor(uint32_t color) {
	if(Led1Active){
		led1_color ^= color;
		UI.led1_on(led1_color);
	} else {
		led2_color ^= color;
		UI.led2_on(led2_color);
	}
}

void DmMaster::toggleLedSelection(void) {
	Led1Active = !Led1Active;
	Display.clearDisplay();
	Display.setTextCursor(0,0);
	Display.printf("LED %d selected\n", Led1Active ? 1 : 2);
	uint8_t ls = UI.getLightSensorState();
	Display.printf("LIGHT SENSOR value: %d", ls);
	this->refreshDisplay();
}

void DmMaster::scroll(ScrollDirection direction) {
	switch(direction){
		case Right:
			Display.startScrollRight(0, 7);
			break;
		case Left:
			Display.startScrollLeft(0, 7);
			break;
		case DiagonalRight:
			Display.startScrollDiagRight(0, 7);
			break;
		case DiagonalLeft:
			Display.startScrollDiagLeft(0, 7);
			break;
		case Off:
		default:
			Display.stopScroll();
			Display.refresh();
			break;
	}
}

void DmMaster::refreshDisplay(void) {
	Display.refresh();
}