#include "netsocket/WiFiAccessPoint.h"

WiFiAccessPoint::WiFiAccessPoint() = default;

WiFiAccessPoint::WiFiAccessPoint(nsapi_wifi_ap_t ap)
{
    _ap = ap;
}

const char *WiFiAccessPoint::get_ssid() const
{
    return _ap.ssid;
}

const uint8_t *WiFiAccessPoint::get_bssid() const
{
    return _ap.bssid;
}

nsapi_security_t WiFiAccessPoint::get_security() const
{
    return _ap.security;
}

int8_t WiFiAccessPoint::get_rssi() const
{
    return _ap.rssi;
}

uint8_t WiFiAccessPoint::get_channel() const
{
    return _ap.channel;
}

