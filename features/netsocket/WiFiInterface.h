/*
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

/** @file WifiInterface.h Common interface between Wi-Fi devices */
/** @addtogroup netinterface
 * @{
 */

#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include <string.h>
#include "netsocket/NetworkInterface.h"
#include "netsocket/WiFiAccessPoint.h"

/** Common interface between Wi-Fi devices.
 */
class WiFiInterface: public virtual NetworkInterface {
public:
    /** Get the default Wi-Fi interface.
     *
     * This is provided as a weak method so applications can override it.
     * Default behavior is to get the target's default interface, if
     * any.
     *
     * @return pointer to interface, if any.
     */
    static WiFiInterface *get_default_instance();

    /** Set the Wi-Fi network credentials.
     *
     *  @param ssid      Name of the network to connect to.
     *  @param pass      Security passphrase to connect to the network.
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE).
     *  @return          NSAPI_ERROR_OK on success, or error code on failure.
     */
    virtual nsapi_error_t set_credentials(const char *ssid, const char *pass,
                                          nsapi_security_t security = NSAPI_SECURITY_NONE) = 0;

    /** Set the Wi-Fi network channel.
     *
     *  @param channel   Channel to make the connection, or 0 for any (Default: 0).
     *  @return          NSAPI_ERROR_OK on success, or error code on failure.
     */
    virtual nsapi_error_t set_channel(uint8_t channel) = 0;

    /** Get the current radio signal strength for active connection.
     *
     *  @return         Connection strength in dBm (negative value),
     *                  or 0 if measurement impossible.
     */
    virtual int8_t get_rssi() = 0;

    /** Attempt to connect to a Wi-Fi network.
     *
     *  @param ssid      Name of the network to connect to.
     *  @param pass      Security passphrase to connect to the network.
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE).
     *  @param channel   Channel to make the connection, or 0 for any (Default: 0).
     *  @return          NSAPI_ERROR_OK on success, or error code on failure.
     */
    virtual nsapi_error_t connect(const char *ssid, const char *pass,
                                  nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0) = 0;

    /** Attempt to connect to a Wi-Fi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface.
     *
     *  @return         NSAPI_ERROR_OK on success, or error code on failure.
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Scan for available networks.
     *
     *  @note This is a blocking function.
     *
     *  If the \p count is 0, the function only returns the number of available networks.
     *  If the \p count is greater than 0 and the \p res is not NULL, the array of discovered APs is populated
     *  with discovered networks up to the value of the \p count.
     *
     *  @param  res      Pointer to allocated array to store discovered APs.
     *  @param  count    Size of allocated res array, or 0 to only count available APs.
     *  @return          Number of entries in res, or if count was 0, number of available networks.
     *                   Negative on error (@see nsapi_types.h for nsapi_error).
     */
    virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, nsapi_size_t count) = 0;

    /** @copydoc NetworkInterface::wifiInterface
     */
    virtual WiFiInterface *wifiInterface()
    {
        return this;
    }

#if !defined(DOXYGEN_ONLY)
protected:

    /** Get the target's default Wi-Fi interface.
     *
     * This is provided as a weak method so targets can override it. The
     * default implementation returns NULL.
     *
     * @return pointer to interface, if any.
     */
    static WiFiInterface *get_target_default_instance();
#endif //!defined(DOXYGEN_ONLY)

public:
    /** Set default parameters on a Wi-Fi interface.
     *
     * A Wi-Fi interface instantiated directly or using
     * WiFiInterface::get_default_instance() is initially unconfigured.
     * This call can be used to set the default parameters that would
     * have been set if the interface had been requested using
     * NetworkInterface::get_default_instance() (see nsapi JSON
     * configuration).
     */
    virtual void set_default_parameters();
};

#endif

/** @}*/
