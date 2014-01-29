/*  Platform header file, for the u-blox C27-C20/U20/G35 
*   mbed Internet of Things Starter Kit
*   http://mbed.org/platforms/u-blox-C027/
*   
*/
 
#ifndef C027_H
#define C027_H
 
#include "mbed.h"
 
// Override the weak mbed die function that flashes the leds. 
// We do not have fancy leds that can be flashed on the C027.
void mbed_die(void);
 
//#define C027_REVA // remove this define for the revision B boards
 
class C027
{
public:
    C027();
 
    void mdmPower(bool enable);
    void mdmUsbEnable(bool enable); // enable the USB interface
    void mdmReset(void);
    void mdmWakeup(void);
    void mdmSleep(void);
    void gpsPower(bool enable);
    void gpsReset(void);
 
private:
    // modem pins 
    DigitalOut      mdmEn;
    DigitalOut      mdmRst;
    DigitalOut      mdmPwrOn;
    DigitalOut      mdmLvlOe;
    DigitalOut      mdmILvlOe;
    DigitalOut      mdmUsbDet;
    bool            mdmIsEnabled;
    bool            mdmUseUsb;
    // gps pins
    DigitalOut      gpsEn;
    DigitalOut      gpsRst;
    bool            gpsIsEnabled;
};

#endif
