/* WiFiInterface
 * Copyright (c) 2015 - 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WIFI_ACCESS_POINT_H
#define WIFI_ACCESS_POINT_H

#include <string.h>
#include "netsocket/nsapi_types.h"

/** WiFiAccessPoint class
 *
 *  Class that represents a WiFi Access Point
 *  Common interface that is shared between WiFi devices
 */
class WiFiAccessPoint {
    /** WiFiAccessPoint lifetime
     */
public:
    WiFiAccessPoint();
    WiFiAccessPoint(nsapi_wifi_ap_t ap);

    /** Get an access point's ssid
     *
     *  @return The ssid of the access point
     */
    const char *get_ssid() const;

    /** Get an access point's bssid
     *
     *  @return The bssid of the access point
     */
    const uint8_t *get_bssid() const;

    /** Get an access point's security
     *
     *  @return The security type of the access point
     */
    nsapi_security_t get_security() const;

    /** Gets the radio signal strength for the access point
     *
     *  @return         Connection strength in dBm (negative value),
     *                  or 0 if measurement impossible
     */
    int8_t get_rssi() const;

    /** Get the access point's channel
     *
     *  @return The channel of the access point
     */
    uint8_t get_channel() const;

private:
    nsapi_wifi_ap_t _ap {};
};

#endif
