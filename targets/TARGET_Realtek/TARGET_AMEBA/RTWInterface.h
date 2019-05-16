/* Ameba implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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

#ifndef RTW_INTERFACE_H
#define RTW_INTERFACE_H

#include "netsocket/NetworkInterface.h"
#include "netsocket/WiFiInterface.h"
#include "nsapi.h"
#include "rtos.h"
#include "netif.h"
#include "rtw_emac.h"
#include "OnboardNetworkStack.h"
#include "EMACInterface.h"
#include "LWIPStack.h"

// Forward declaration
class NetworkStack;

/** Realtek Wlan (RTW) interface class
 *  Implementation of the NetworkStack for Ameba
 */
class RTWInterface: public WiFiInterface, public EMACInterface
{
public:
    /** RTWWlanInterface lifetime
     */
    RTWInterface(
        RTW_EMAC &rtw_emac = RTW_EMAC::get_instance(),
        OnboardNetworkStack &rtw_lwip_stack = OnboardNetworkStack::get_default_instance(), bool debug=false);

    ~RTWInterface();

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

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
                         nsapi_security_t security = NSAPI_SECURITY_NONE,
                         uint8_t channel = 0);

    /** Start the interface
     *  @return          0 on success, negative on failure
     */
    virtual nsapi_error_t connect();

    /** Stop the interface
     *  @return          0 on success, negative on failure
     */
    virtual nsapi_error_t disconnect();
    virtual int is_connected();

    /** Scan for available networks
     *
     *  The scan will
     *  If the network interface is set to non-blocking mode, scan will attempt to scan
     *  for WiFi networks asynchronously and return NSAPI_ERROR_WOULD_BLOCK. If a callback
     *  is attached, the callback will be called when the operation has completed.
     *  @param  ap       Pointer to allocated array to store discovered AP
     *  @param  count    Size of allocated @a res array, or 0 to only count available AP
     *  @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     *  @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *  see @a nsapi_error
     */
    virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, unsigned count);

    virtual nsapi_error_t set_channel(uint8_t channel);
    virtual int8_t get_rssi();

    RTW_EMAC &get_emac() const { return rtw_emac; }

    virtual RTWInterface *rtwInterface() { return this; }

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack 
     */
    virtual NetworkStack *get_stack();
    RTW_EMAC &rtw_emac;
    OnboardNetworkStack &rtw_obn_stack;
    char _ssid[256];
    char _pass[256];
    nsapi_security_t _security;
    uint8_t _channel;
    static const int SSID_MAX_LENGTH = 32; //The longest ssid
    static const int PASSPHRASE_MAX_LENGTH = 63; //The longest passphrase
    static const int PASSPHRASE_MIN_LENGTH = 8; // The shortest passphrase
};
#endif
