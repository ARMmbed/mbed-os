
/* UbloxWiFiSoftAPInterface
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

#ifndef UBLOX_WIFI_SOFTAPINTERFACE_H
#define UBLOX_WIFI_SOFTAPINTERFACE_H

#include <string.h>
#include "netsocket/WiFiAccessPoint.h"

/** UbloxWiFiSoftAPInterface class
 *
 *  Common interface that is shared between WiFi devices supporting SoftAP mode
 */
class UbloxWiFiSoftAPInterface
{
public:
    /** UbloxWiFiSoftAPInterface lifetime
     */
    virtual ~UbloxWiFiSoftAPInterface() {};

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_ap_credentials(const char *ssid, const char *pass = 0,
            nsapi_security_t security = NSAPI_SECURITY_NONE) = 0;

    /** Set the WiFi network channel
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_ap_channel(uint8_t channel) = 0;

    /** Gets the current number of active connections
     *
     *  @return         number of active connections
     */
    virtual int get_ap_connection_count() = 0;

    /** Gets the max supported number of active connections
     *
     *  @return         maximum number of active connections
     */
    virtual int get_ap_max_connection_count() = 0;

    /** Enable or disable DHCP on the network access point
     *
     *  Enables DHCP in SoftAP mode. Defaults to enabled unless
     *  a static IP address has been assigned. Requires that the network is
     *  service stopped.
     *
     *  @param dhcp     True to enable DHCP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_ap_dhcp(bool dhcp) = 0;

    /** Start the interface
     *
     *  Attempts to serve a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   Channel on which the connection is to be made.
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_start(const char *ssid, const char *pass = 0,
            nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0) = 0;

    /** Start the interface
     *
     *  Attempts to serve a WiFi network. Requires ssid to be set.
     *  passphrase is optional.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t ap_start() = 0;

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_stop() = 0;

};

#endif

