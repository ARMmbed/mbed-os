
#include "gpio_api.h"
#include "wait_api.h"

 // called before main
void mbed_sdk_init()
{
    gpio_t modemEn, modemRst, modemPwrOn, modemLvlOe, modemILvlOe, modemUsbDet;
    gpio_t gpsEn, gpsRst, led, modemRts;
    
    // start with modem disabled 
    gpio_init_out_ex(&modemEn, 	    MDMEN, 		PullNone, 0);
    gpio_init_out_ex(&modemRst, 	MDMRST, 	PullNone, 1);
    gpio_init_out_ex(&modemPwrOn, 	MDMPWRON, 	PullNone, 1);
    gpio_init_out_ex(&modemLvlOe, 	MDMLVLOE, 	PullNone, 1);
    gpio_init_out_ex(&modemILvlOe,  MDMILVLOE, 	PullNone, 0);
    gpio_init_out_ex(&modemUsbDet,  MDMUSBDET, 	PullNone, 0);
    gpio_init_out_ex(&modemRts, 	MDMRTS, 	PullNone, 0);
    // start with gps disabled 
    gpio_init_out_ex(&gpsEn, 		GPSEN, 		PullNone, 0);
    gpio_init_out_ex(&gpsRst, 		GPSRST, 	PullNone, 1);
    // led should be off
    gpio_init_out_ex(&led, 		    LED, 		PullNone, 0);
      
    wait_ms(50); // when USB cable is inserted the interface chip issues 
    // multiple resets to the target CPU We wait here for a short period to 
    // prevent those resets from propagating to the modem and other 
    // components. 
}
