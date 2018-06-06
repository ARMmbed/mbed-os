
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
#include "netsocket/NetworkInterface.h"
#include "netsocket/WiFiAccessPoint.h"

/** WiFiInterface class
 *
 *  Common interface that is shared between WiFi devices
 *  @addtogroup netsocket
 */
class WiFiInterface: public virtual NetworkInterface
{
public:
    /** Get the default WiFi interface.
     *
     * This is provided as a weak method so applications can override.
     * Default behaviour is to get the target's default interface, if
     * any.
     *
     * @return pointer to interface, if any
     */
    static WiFiInterface *get_default_instance();

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *ssid, const char *pass,
            nsapi_security_t security = NSAPI_SECURITY_NONE) = 0;

    /** Set the WiFi network channel
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_channel(uint8_t channel) = 0;

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
    virtual nsapi_error_t connect(const char *ssid, const char *pass,
            nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0) = 0;

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Scan for available networks
     *
     *  This function will block. If the @a count is 0, function will only return count of available networks, so that
     *  user can allocated necessary memory. If the \p count is grater than 0 and the a \p res is not NULL it'll be populated
     *  with discovered networks up to value of \p count.
     *
     *  @param  res      Pointer to allocated array to store discovered AP
     *  @param  count    Size of allocated @a res array, or 0 to only count available AP
     *  @return          Number of entries in \p count, or if \p count was 0 number of available networks,
     *                   negative on error see @a nsapi_error
     */
    virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, nsapi_size_t count) = 0;

    virtual WiFiInterface *wifiInterface() {
        return this;
    }

protected:

    /** Get the target's default WiFi interface.
     *
     * This is provided as a weak method so targets can override. The
     * default implementation returns NULL.
     *
     * @return pointer to interface, if any
     */
    static WiFiInterface *get_target_default_instance();
};

#endif

/** @}*/
