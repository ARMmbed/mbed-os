
#include "gpio_api.h"
#include "wait_api.h"

 // called before main
void mbed_sdk_init()
{
    gpio_t modemEn, modemRst, modemPwrOn, modemLvlOe, modemILvlOe, modemUsbDet;
    gpio_t gpsEn, gpsRst, led, modemRts;
    
    // start with modem disabled 
    GPIO_INIT_OUT(&modemEn,     MDMEN,      PullNone, 0);
    GPIO_INIT_OUT(&modemRst,    MDMRST,     PullNone, 1);
    GPIO_INIT_OUT(&modemPwrOn,  MDMPWRON,   PullNone, 1);
    GPIO_INIT_OUT(&modemLvlOe,  MDMLVLOE,   PullNone, 1);
    GPIO_INIT_OUT(&modemILvlOe, MDMILVLOE,  PullNone, 0);
    GPIO_INIT_OUT(&modemUsbDet, MDMUSBDET,  PullNone, 0);
    GPIO_INIT_OUT(&modemRts,    MDMRTS,     PullNone, 0);
    // start with gps disabled 
    GPIO_INIT_OUT(&gpsEn,       GPSEN,      PullNone, 0);
    GPIO_INIT_OUT(&gpsRst,      GPSRST,     PullNone, 1);
    // led should be off
    GPIO_INIT_OUT(&led,         LED,        PullNone, 0);
    
    wait_ms(50); // when USB cable is inserted the interface chip issues 
    // multiple resets to the target CPU We wait here for a short period to 
    // prevent those resets from propagating to the modem and other 
    // components. 
}
