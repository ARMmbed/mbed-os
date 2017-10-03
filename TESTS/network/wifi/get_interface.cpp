#include "mbed.h"

// Pick the correct driver based on mbed_app.json
#define INTERNAL            1
#define WIFI_ESP8266        2
#define X_NUCLEO_IDW01M1    3

#if MBED_CONF_APP_WIFI_DRIVER == INTERNAL

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
#define DRIVER OdinWiFiInterface

#elif TARGET_REALTEK_RTL8195AM
#include "RTWInterface.h"
#define DRIVER RTWInterface
#else
#error [NOT_SUPPORTED] Unsupported Wifi driver
#endif

#elif MBED_CONF_APP_WIFI_DRIVER == WIFI_ESP8266
#include "ESP8266Interface.h"
#define DRIVER ESP8266Interface

#elif MBED_CONF_APP_WIFI_DRIVER == X_NUCLEO_IDW01M1
#include "SpwfSAInterface.h"
#define DRIVER SpwfSAInterface
#else
#error [NOT_SUPPORTED] Unsupported Wifi driver
#endif

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface)
        delete interface;

#if MBED_CONF_APP_WIFI_DRIVER == INTERNAL
    interface = new DRIVER();
#else
    interface = new DRIVER(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif
    return interface;
}
