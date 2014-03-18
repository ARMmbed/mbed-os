
#include "gpio_api.h"
#include "wait_api.h"

 // called before main
void mbed_sdk_init()
{
    gpio_t modemEn, modemRst, modemPwrOn, modemLvlOe, modemILvlOe, modemUsbDet;
    gpio_t gpsEn, gpsRst, led, modemRts;
    
    // start with modem disabled 
    gpio_init_out_ex(&modemEn,     MDMEN,     0);
    gpio_init_out_ex(&modemRst,    MDMRST,    1);
    gpio_init_out_ex(&modemPwrOn,  MDMPWRON,  1);
    gpio_init_out_ex(&modemLvlOe,  MDMLVLOE,  1);
    gpio_init_out_ex(&modemILvlOe, MDMILVLOE, 0);
    gpio_init_out_ex(&modemUsbDet, MDMUSBDET, 0);
    gpio_init_out_ex(&modemRts,    MDMRTS,    0);
    // start with gps disabled 
    gpio_init_out_ex(&gpsEn,       GPSEN,     0);
    gpio_init_out_ex(&gpsRst,      GPSRST,    1);
    // led should be off
    gpio_init_out_ex(&led,         LED,       0);
    
    wait_ms(50); // when USB cable is inserted the interface chip issues 
    // multiple resets to the target CPU We wait here for a short period to 
    // prevent those resets from propagating to the modem and other 
    // components. 
}
