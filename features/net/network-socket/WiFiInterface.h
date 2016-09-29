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
    WiFiInterface();
    virtual ~WiFiInterface();

    /** Set the WiFi network credentials
     *
     *  @param ssid     Name of the network to connect to
     *  @param pass     Security passphrase to connect to the network
     *  @param security Type of encryption for connection
     *                  (defaults to NSAPI_SECURITY_NONE)
     */
    virtual int set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @param timeout   Timeout in milliseconds; 0 for no timeout (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual int connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE,
                                  uint8_t channel = 0, unsigned timeout = 0) = 0;

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network asynchronously, the call will return straight away. If the @a cb was NULL
     *  you'll need to query @a get_state until it's in NSAPI_IF_STATE_CONNECTED state, otherwise the @a cb will be
     *  called with connection results, connected AP details and user data.
     *
     *  Note: @a ssid and @a pass must be kept until the connection is made, that is the callback has been called or the
     *        state changed to @a NSAPI_IF_STATE_CONNECTED as they are passed by value.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param cb        Function to be called when the connect finishes (Default: NULL)
     *  @param data      Arbitrary user data to pass to @a cb function (Default: NULL)
     *  @param timeout   Timeout in milliseconds; 0 for no timeout (Default: 0)
     */
    virtual void connect_async(const char *ssid, const char *pass,nsapi_security_t security = NSAPI_SECURITY_NONE,
                               uint8_t channel = 0, mbed::Callback<void(nsapi_error_t, wifi_ap_t*, void*)> cb = NULL, void *data = NULL,
                               unsigned timeout = 0) = 0;

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
     *  @return          0 on success, or error code on failure
     */
    virtual int disconnect() = 0;

    /** Get the current WiFi interface state
     *
     * @returns         Interface state enum
     */
    virtual nsapi_if_state_t get_state() = 0;

    /** Gets the current radio signal strength for active connection
     *
     * @return Connection strength in dBm (negative value), or 0 if measurement impossible
     */
    virtual int8_t get_rssi() = 0;

    /** Scan for available networks
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    virtual int scan(wifi_ap_t *res, unsigned count, unsigned timeout = 0) = 0;

    /** Scan for available networks
     *
     * This function won't block, it'll return immediately and call provided callback for each discovered network with
     * AP data and user @a data. After the last discovered network @a cb will be called with NULL as @a ap, so it
     * will be always called at least once.
     *
     * @param  cb    Function to be called for every discovered network
     * @param  data  User handle that will be passed to @a cb along with the AP data (Default: NULL)
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     */
    virtual void scan_async(mbed::Callback<void(wifi_ap_t*, void*)> cb, void *data = NULL, unsigned timeout = 0) = 0;

private:
    char *_ssid;
    char *_pass;
    nsapi_security_t _security;
    char _ip_address[16]; //IPv4 only thus 16
};

#endif
