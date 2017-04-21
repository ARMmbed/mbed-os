
/** \addtogroup netsocket */
/** @{*/
/* NetworkStack
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

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "netsocket/nsapi_types.h"
#include "netsocket/SocketAddress.h"

// Predeclared class
class NetworkStack;


/** NetworkInterface class
 *
 *  Common interface that is shared between network devices
 */
class NetworkInterface {
public:
    virtual ~NetworkInterface() {};

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

    /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_gateway();

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
    virtual nsapi_error_t set_network(
            const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Enables DHCP on connecting the network. Defaults to enabled unless
     *  a static IP address has been assigned. Requires that the network is
     *  disconnected.
     *
     *  @param dhcp     True to enable DHCP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

    /** Start the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Translates a hostname to an IP address with specific version
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  @param address  Destination for the host SocketAddress
     *  @param host     Hostname to resolve
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC)
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t gethostbyname(const char *host,
            SocketAddress *address, nsapi_version_t version = NSAPI_UNSPEC);

    /** Add a domain name server to list of servers to query
     *
     *  @param addr     Destination for the host address
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t add_dns_server(const SocketAddress &address);

protected:
    friend class Socket;
    friend class UDPSocket;
    friend class TCPSocket;
    friend class TCPServer;
    friend class SocketAddress;
    template <typename IF>
    friend NetworkStack *nsapi_create_stack(IF *iface);

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack() = 0;
};


#endif

/** @}*/
