#include "C027.h"
 
void mbed_die(void)
{
    DigitalOut led(LED);
    led = 0;
    // Send SOS in Morse Code: // "... --- ...  "
    const char sos[] = "=.=.=...===.===.===...=.=.=......"; 
    const int  dot = 100; // length of the dot in milliseconds (typical: 50-100 ms)
    // lengths of symbols: 
    // di         =   dot 
    // dah        = 3 dot 
    // char space = 3 dot 
    // word space = 6 dot 
    for (int i = 0; /* nothing */; i = sos[i+1] ? i+1 : 0)
    {
        led = (sos[i] == '=');
        wait_ms(dot);
    }
}
 
C027::C027() :
    mdmEn(MDMEN),       mdmRst(MDMRST),         mdmPwrOn(MDMPWRON),   // MDM CTRL
    mdmLvlOe(MDMLVLOE), mdmILvlOe(MDMILVLOE),   mdmUsbDet(MDMUSBDET), // MDM IF
    gpsEn(GPSEN),       gpsRst(GPSRST)                                // GPS CTRL
{
    DigitalOut led(LED);
    DigitalOut mdmRts(MDMRTS);
    led       = 0;  // LED1:   0=off
    mdmRts    = 0;  // RTS:    0=ready to send 
    // we start with the gps disabled
    gpsEn     = 0;  // LDOEN:  1=on,0=off
    gpsRst    = 0;  // RESET:  0=reset,1=operating
    gpsIsEnabled = false;
    // we start with the modem disabled
    mdmLvlOe  = 1;  // LVLEN:  1=disabled (uart/gpio)
    mdmILvlOe = 0;  // ILVLEN: 0=disabled (i2c)
    mdmUsbDet = 0;  // USBDET: 0=disabled
    mdmPwrOn  = 1;  // PWRON:  1=idle, 0=action
    mdmEn     = 0;  // LDOEN:  1=on, 0=off
    mdmRst    = 0;  // RESET:  0=reset, 1=operating
    mdmIsEnabled = false;
    mdmUseUsb = false;
    wait_ms(50); // when USB cable is inserted the interface chip issues 
    // multiple resets to the target CPU We wait here for a short period to 
    // prevent those resets from propagating to the modem and other 
    // components. 
}
 
void C027::mdmPower(bool enable)
{
    if (!mdmIsEnabled && enable) // enable modem
    {
#ifdef C027_REVA
        // we need the gps ldo for the level shifters of the modem
        gpsEn     = 1;  // LDOEN(gps): 0=off -> 1=on 
#endif
        mdmPwrOn  = 0;  // PWRON:  0=turn on
        mdmEn     = 0;  // LDOEN:  0=off
        mdmRst    = 0;  // RESET:  0=reset
        mdmLvlOe  = 1;  // LVLEN:  1=disabled
        mdmILvlOe = 0;  // ILVLEN: 0=disabled
        mdmUsbDet = mdmUseUsb ? 1 : 0;  // USBDET: 0=disabled, 1=enabled
        wait_ms( 10);   //         wait until supply switched off
        mdmEn     = 1;  // LDOEN:  0=off -> 1=on
        wait_ms( 10);   //         wait until supply stable and on
        wait_ms(300);   //         power on sequence is triggered by keeping reset 50ms-300ms low
        mdmRst    = 1;  // RESET:  0=reset -> 1=operating
        wait_ms(150);   //         LISA-C requires 150ms low to turn on
        mdmPwrOn  = 1;  // PWRON:  0=turn on -> 1=idle
        mdmIsEnabled = true;
        // the interfaces of the modem should be fully ready after a certain period
        // LISA-U200  5000ms
        // SARA-G350  3000ms
        // LISA-C200       
        wait_ms(1000);
        mdmLvlOe  = 0;  // LVLEN:  0=enabled (uart/gpio)
        if (gpsIsEnabled) 
            mdmILvlOe = 1; // ILVLEN: 1=enabled (i2c)
    }
    else if (mdmIsEnabled && !enable) // disable modem
    {
        mdmIsEnabled = false;
        // initiate power off sequence by setting PwnOn low for >1s
        mdmILvlOe = 0;  // ILVLEN: 0=disabled (i2c)
        mdmLvlOe  = 1;  // LVLEN:  1=disabled (uart/gpio)
        mdmUsbDet = 0;  // USBDET: 0=disabled
        mdmPwrOn  = 0;  // PWRON:  0=active
        wait_ms(1000);  //         send to sleep
        mdmPwrOn  = 1;  // PWRON:  0=active -> 1=idle
        // now we can savely switch off the ldo
        mdmRst    = 0;  // RESET:  1=operating -> 0=reset
        mdmEn     = 0;  // LDOEN:  1=on -> 0=off
#ifdef C027_REVA
        // the gps ldo may no longer be needed level shifters of the modem
        // so we switch it off if the gps is not enabled
        if (!gpsIsEnabled)
            gpsEn = 0;  // LDOEN(gps): 1=on -> 0=off
#endif
    }
        
}
 
void C027::mdmUsbEnable(bool enable)
{
    mdmUseUsb = enable;
    if (mdmIsEnabled)
        mdmUsbDet = mdmUseUsb ? 1 : 0;
}
 
void C027::mdmReset(void)
{
    if (mdmIsEnabled)
    {
        mdmRst   = 0;   // RESET: 0=reset
        // power on sequence is triggered by reset low
        // LISA-U200   50ms
        // SARA-G350   50ms
        // LISA-C200  300ms
        wait_ms(300);
        mdmRst   = 1;   // RESET: 0=reset -> 1=operating
    }
}
 
void C027::mdmWakeup(void)
{
    if (mdmIsEnabled)
    {
        mdmPwrOn = 0;
        // wakeup the device by low pulse: 
        // LISA-U200 50-80us
        // SARA-G350     5ms
        // LISA-C200   150ms 
        wait_ms(150);   
        mdmPwrOn = 1;
    }
}
 
void C027::mdmSleep(void)
{
    if (mdmIsEnabled)
    {
        mdmPwrOn = 0;
        // going to sleep is triggerd by low pulse: 
        // LISA-U200  1000ms
        // SARA-G350     n/a
        // LISA-C200     n/a
        wait_ms(1000);  
        mdmPwrOn = 1;
    }
}
 
void C027::gpsPower(bool enable)
{
    if (!gpsIsEnabled && enable) // enable gps
    {
        gpsRst   = 0;   // RESET: 0=reset
        gpsEn    = 1;   // LDOEN: 0=off -> 1=on
        wait_ms(1);     //        wait until stable
        gpsRst   = 1;   // RESET: 0=reset -> 1=operating
        gpsIsEnabled = true;
        // the gps schould be fully ready after 50ms
        if (mdmIsEnabled) 
            mdmILvlOe = 1; // ILVLEN: 0=enabled (i2c)
    }
    else if (gpsIsEnabled && !enable) // diasble gps
    {
        gpsIsEnabled = false;
        mdmILvlOe = 0;   // ILVLEN: 0=disabled (i2c)
        gpsRst    = 0;   // RESET: 1=operating -> 0=reset
#ifdef C027_REVA
        // the gps ldo may have to remain enabled for the level shifters of the modem
        if (!mdmIsEnabled) 
            gpsEn = 0;   // LDOEN: 1=on -> 0=off 
#else
        gpsEn     = 0;   // LDOEN: 1=on -> 0=off 
#endif
    }
}
 
void C027::gpsReset(void)
{
    if (gpsIsEnabled)
    {
        gpsRst   = 0;    // RESET: 0=reset,1=operating
        wait_ms(1);
        gpsRst   = 1;    // RESET: 1=operating,0=reset
    }
}
