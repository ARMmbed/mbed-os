/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef SCL_STA_INTERFACE_H
#define SCL_STA_INTERFACE_H


/** @file
 *  Provides SCL interface functions to be used with WiFiInterface or NetworkInterface Objects
 */

#include "netsocket/WiFiInterface.h"
#include "netsocket/EMACInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "scl_emac.h"
#include "scl_wifi_api.h"
#include "scl_types.h"
#include "SclAccessPoint.h"
#include "scl_interface.h"
#define MAX_SSID_LENGTH                       (33) /**< Maximum ssid length */
#define MAX_PASSWORD_LENGTH                   (64) /**< Maximum password length */

/** SclSTAInterface class
 *  Implementation of the Network Stack for the SCL
 */
class SclSTAInterface : public WiFiInterface, public EMACInterface {
public:

    SclSTAInterface(
        SCL_EMAC &emac = SCL_EMAC::get_instance(),
        OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance(),
        scl_interface_shared_info_t &shared = scl_iface_shared);

    /** Gets the current instance of the SclSTAInterface
     *
     *  @return         Pointer to the object of class SclSTAInterface.
     */
    static SclSTAInterface *get_default_instance();

    /** Turns on the Wi-Fi device
    *
    *  @return         void
    */
    void wifi_on();

    /** Starts the interface
     *
     *  Attempts to connect to a Wi-Fi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t connect();

    /** Starts the interface
     *
     *  Attempts to connect to a Wi-Fi network.
     *
     *  @param ssid      Name of the network to connect to.
     *  @param pass      Security passphrase to connect to the network.
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE).
     *  @param channel   This parameter is not supported, setting it to a value other than 0 will result in NSAPI_ERROR_UNSUPPORTED.
     *  @return          0 on success, negative error code on failure.
     */
    nsapi_error_t connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);

    /** Disconnects the interface
     *
     *  @return             0 on success, negative error code on failure.
     */
    nsapi_error_t disconnect();

    /** Set the Wi-Fi network credentials
     *
     *  @param ssid      Name of the network to connect to.
     *  @param pass      Security passphrase to connect to the network.
     *  @param security  Type of encryption for connection.
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, negative error code on failure.
     */
    nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Sets the Wi-Fi network channel - NOT SUPPORTED
     *
     *  This function is not supported and will return NSAPI_ERROR_UNSUPPORTED.
     *
     *  @param channel   Channel on which the connection is to be made (Default: 0).
     *  @return          Not supported, returns NSAPI_ERROR_UNSUPPORTED.
     */
    nsapi_error_t set_channel(uint8_t channel)
    {
        if (channel != 0) {
            return NSAPI_ERROR_UNSUPPORTED;
        }
        return 0;
    }

    /** Set blocking status of interface.
     *  Nonblocking mode is not supported.
     *
     *  @param blocking  True if connect is blocking
     *  @return          0 on success, negative error code on failure
     */
    nsapi_error_t set_blocking(bool blocking)
    {
        if (blocking) {
            _blocking = blocking;
            return NSAPI_ERROR_OK;
        } else {
            return NSAPI_ERROR_UNSUPPORTED;
        }
    }
    /** Gets the current radio signal strength for active connection
     *
     *  @return          Connection strength in dBm (negative value).
     */
    int8_t get_rssi();

    /** Scan for available networks in WiFiAccessPoint format
     *
     * This function will block.
     *
     * @param  aps      Pointer to allocated array of WiFiAccessPoint format for discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan(WiFiAccessPoint *aps, unsigned count);

    /** This function is used to indicate if the device is connected to the network.
     *
     *  @return          SCL_SUCCESS if device is connected.
     */
    int is_interface_connected();

    /** Gets the BSSID (MAC address of device connected to).
     *
     *  @param bssid   Pointer to the BSSID value.
     *  @return        SCL_SUCCESS if BSSID is obtained successfully.
     *  @return        SCL_BADARG if input parameter is NULL.
     *  @return        SCL_ERROR if unable to fetch BSSID.
     */
    int get_bssid(uint8_t *bssid);

    /** This function is used to set up the Wi-Fi interface.
     *  This function should be used after the wifi_on.
     *
     * @return          SCL_SUCCESS if the Wi-Fi interface is set up successfully.
     */
    int wifi_set_up(void);
protected:
    int internal_scan(WiFiAccessPoint *aps, unsigned count, scan_result_type sres_type);

private:

    char _ssid[MAX_SSID_LENGTH]; /**< The longest possible name (defined in 802.11) +1 for the \0 */
    char _pass[MAX_PASSWORD_LENGTH]; /**< The longest allowed passphrase + 1 */
    nsapi_security_t _security; /**< Security type */
    SCL_EMAC &_scl_emac; /**< SCL_EMAC object */
    scl_interface_shared_info_t &_iface_shared;
};
#endif /* ifndef SCL_STA_INTERFACE_H */
