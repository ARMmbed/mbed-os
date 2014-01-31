
#include "gpio_api.h"
#include "wait_api.h"

 // called before main
void mbed_sdk_init()
{
    gpio_t modemEn, modemRst, modemPwrOn, modemLvlOe, modemILvlOe, modemUsbDet;
    gpio_t gpsEn, gpsRst, led, modemRts;
    
    gpio_init(&modemEn, MDMEN, PIN_OUTPUT);
    gpio_init(&modemRst, MDMRST, PIN_OUTPUT);
    gpio_init(&modemPwrOn, MDMPWRON, PIN_OUTPUT);
    gpio_init(&modemLvlOe, MDMLVLOE, PIN_OUTPUT);
    gpio_init(&modemILvlOe, MDMILVLOE, PIN_OUTPUT);
    gpio_init(&modemUsbDet, MDMUSBDET, PIN_OUTPUT);
    gpio_init(&gpsEn, GPSEN, PIN_OUTPUT);
    gpio_init(&gpsRst, GPSRST, PIN_OUTPUT);
    gpio_init(&led, LED, PIN_OUTPUT);
    gpio_init(&modemRts, MDMRTS, PIN_OUTPUT);
    
    gpio_write(&led, 0);  			// LED1:   0=off
    gpio_write(&modemRts, 0);		// RTS:    0=ready to send 
    // we start with the gps disabled
    gpio_write(&gpsEn, 0);  		// LDOEN:  1=on,0=off
    gpio_write(&gpsRst, 0); 		// RESET:  0=reset,1=operating
    // we start with the modem disabled
    gpio_write(&modemLvlOe, 1);		// LVLEN:  1=disabled (uart/gpio)
    gpio_write(&modemILvlOe, 0);  	// ILVLEN: 0=disabled (i2c)
    gpio_write(&modemUsbDet, 0); 	// USBDET: 0=disabled
    gpio_write(&modemPwrOn, 1);		// PWRON:  1=idle, 0=action
    gpio_write(&modemEn, 0);  		// LDOEN:  1=on, 0=off
    gpio_write(&modemRst, 0);  		// RESET:  0=reset, 1=operating
    
    wait_ms(50); // when USB cable is inserted the interface chip issues 
    // multiple resets to the target CPU We wait here for a short period to 
    // prevent those resets from propagating to the modem and other 
    // components. 
}
