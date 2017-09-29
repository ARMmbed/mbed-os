#include "mbed.h"

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
#elif TARGET_REALTEK_RTL8195AM
#include "RTWInterface.h"
#else
#include "ESP8266Interface.h"
#endif

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface)
        delete interface;

#if TARGET_UBLOX_EVK_ODIN_W2
    interface = new OdinWiFiInterface();
#elif TARGET_REALTEK_RTL8195AM
    interface = new RTWInterface();
#else
    interface = new ESP8266Interface(D1, D0);
#endif
    return interface;
}
