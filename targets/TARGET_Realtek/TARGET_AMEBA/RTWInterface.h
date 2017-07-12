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
#include "lwip/netif.h"

// Forward declaration
class NetworkStack;

/** Realtek Wlan (RTW) interface class
 *  Implementation of the NetworkStack for Ameba
 */
class RTWInterface: public WiFiInterface
{
public:
    /** RTWWlanInterface lifetime
     */
    RTWInterface(bool debug=false);
    ~RTWInterface();

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param address  Null-terminated representation of the local IP address
     *  @param netmask  Null-terminated representation of the local network mask
     *  @param gateway  Null-terminated representation of the local gateway
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

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

    /** Get the local MAC address
     *
     *  Provided MAC address is intended for info or debug purposes and
     *  may not be provided if the underlying network interface does not
     *  provide a MAC address
     *  
     *  @return         Null-terminated representation of the local MAC address
     *                  or null if no MAC address is available
     */
    virtual const char *get_mac_address();

    /** Get the local IP address
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if no IP address has been recieved
     */
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_netmask();

    /** Get the local gateways
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_gateway();

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack 
     */
    virtual NetworkStack *get_stack();

    bool _dhcp;
    char _ssid[256];
    char _pass[256];
    nsapi_security_t _security;
    uint8_t _channel;
    char _ip_address[IPADDR_STRLEN_MAX];
    char _netmask[NSAPI_IPv4_SIZE];
    char _gateway[NSAPI_IPv4_SIZE];
};

#endif
