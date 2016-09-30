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

#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include <string.h>
#include "Callback.h"
#include "network-socket/NetworkInterface.h"
#include "nsapi.h"

/** WiFiInterface class
 *
 *  Common interface that is shared between WiFi devices
 */
class WiFiInterface: public NetworkInterface
{
public:
    typedef struct wifi_ap {
        char ssid[33]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
        uint8_t bssid[6];
        nsapi_security_t security;
        int8_t rssi;
        uint8_t channel;
    } wifi_ap_t;

    /** WiFiInterface lifetime
     */
    virtual ~WiFiInterface() {};

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual int set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE) = 0;

    /** Set the WiFi network channel
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual int set_channel(uint8_t channel) = 0;

    /** Gets the current radio signal strength for active connection
     *
     *  @return         Connection strength in dBm (negative value),
     *                  or 0 if measurement impossible
     */
    virtual int8_t get_rssi() = 0;

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual int connect(const char *ssid, const char *pass,
            nsapi_security_t security = NSAPI_SECURITY_NONE,
            uint8_t channel = 0);

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int connect() = 0;

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual int disconnect() = 0;

    /** Scan for available networks
     *
     *  The scan will 
     *  If the network interface is set to non-blocking mode, scan will attempt to scan
     *  for WiFi networks asynchronously and return NSAPI_ERROR_WOULD_BLOCK. If a callback
     *  is attached, the callback will be called when the operation has completed.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    virtual int scan(wifi_ap_t *res, unsigned count) = 0;
};

#endif
