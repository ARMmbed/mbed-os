#include "mbed.h"
#include "ESP8266Interface.h"

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface)
        delete interface;

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
    interface = new OdinWiFiInterface();
#elif TARGET_REALTEK_RTL8195AM
#include "RTWInterface.h"
    interface = new RTWInterface();
#else
    interface = new ESP8266Interface(D1, D0);
#endif
    return interface;
}
