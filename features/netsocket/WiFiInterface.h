
/** \addtogroup netsocket */
/** @{*/
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
 *  Common interface implemented by all supported WiFi modules. Provides both station and access point modes.
 */
class WiFiInterface: public NetworkInterface
{
public:
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
     *  user can allocated necessary memory. If the @count is grater than 0 and the @a ap is not NULL it'll be populated
     *  with discovered networks up to value of @a count.
     *
     *  @param  ap       Pointer to allocated array to store discovered AP
     *  @param  count    Size of allocated @a res array, or 0 to only count available AP
     *  @return          Number of entries in @a, or if @a count was 0 number of available networks,
     *                   negative on error see @a nsapi_error
     */
    virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, nsapi_size_t count) = 0;

    /** WiFi Access Point API
     *
     *  WiFi Access Point functionality. Support for AP mode is not mandatory in which case AP mode API functions will
     *  return @a NSAPI_ERROR_UNSUPPORTED or NULL.
     */

    /** Set IP config for access point
     *
     *  This function has to be called before the access point is started.
     *
     *  @param gateway     Null-terminated representation of the local gateway
     *  @param netmask     Null-terminated representation of the network mask
     *  @return            0 on success, negative error code on failure
     */
    virtual nsapi_error_t ap_set_network(const char *gateway, const char *netmask)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Set the WiFi access point network SSID and credentials
     *
     *  This function has to be called before the access point is started.
     *
     *  @param ssid      Name of the network to create
     *  @param pass      Security passphrase to protect the network with (Default: No password)
     *  @param security  Type of encryption for connection (Default: Open network)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_set_credentials(const char *ssid, const char *pass = NULL,
                                             nsapi_security_t security = NSAPI_SECURITY_NONE)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Set the WiFi access point discoverability
     *
     *  @param hidden    true to hide the SSID, false to advertise the SSID
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_set_hidden(bool hidden)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Set the WiFi access point network channel
     *
     *  @param channel   Channel on which the network is to be created, or 0 for auto
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_set_channel(uint8_t channel)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Set up the access point and start accepting connections
     *
     *  Before this function is called @a ap_set_credentials and @a ap_set_network needs to be called to set up the
     *  basic details, like BSSID and IP addresses. Security can be selected using @a ap_set_credentials, defaults to
     *  open network if not set. Channel of operation can be supplied using @a ap_set_channel, it will be selected
     *  automatically if not set. BSSID can be hidden using @a ap_set_hidden, it will be discoverable by default.
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_start()
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Disconnect all clients and stop the access point
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_stop()
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Return number of connected clients
     *
     *  @return         Number of connected WiFi clients, or negative on error
     */
    virtual nsapi_size_or_error_t ap_connected()
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    /** Get the local MAC address for access point mode
     *
     *  Provided MAC address is intended for info or debug purposes and
     *  may not be provided if the underlying network interface does not
     *  provide a MAC address
     *
     *  @return         Null-terminated representation of the local AP MAC address
     *                  or NULL if no MAC address is available
     */
    virtual const char *ap_get_mac_address()
    {
        return NULL;
    }

    /** Set the local MAC address for access point mode
     *
     *  WiFi module may not support changing MAC address in which case @NSAPI_ERROR_UNSUPPORTED is returned.
     *
     *  @param address      Null-terminated representation of the local MAC address for AP mode
     *  @return             0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_set_mac_address(const char *mac_address)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }
};

#endif

/** @}*/
