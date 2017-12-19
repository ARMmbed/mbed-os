/* LWIP implementation of NetworkInterfaceAPI
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

#ifndef EMAC_INTERFACE_H
#define EMAC_INTERFACE_H

#include "nsapi.h"
#include "rtos.h"
#include "EMAC.h"
#include "OnboardNetworkStack.h"


/** EMACInterface class
 *  Implementation of the NetworkInterface for an EMAC-based driver
 */
class EMACInterface : public virtual NetworkInterface
{
public:
    /** Create an EMAC-based network interface.
     *
     * The default arguments obtain the default EMAC, which will be target-
     * dependent (and the target may have some JSON option to choose which
     * is the default, if there are multiple). The default stack is configured
     * by JSON option nsapi.default-stack.
     *
     * Due to inability to return errors from the constructor, no real
     * work is done until the first call to connect().
     *
     * @param emac  Reference to EMAC to use
     * @param stack Reference to onboard-network stack to use
     */
    EMACInterface(
            EMAC &emac = EMAC::get_default_instance(),
            OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param ip_address  Null-terminated representation of the local IP address
     *  @param netmask     Null-terminated representation of the local network mask
     *  @param gateway     Null-terminated representation of the local gateway
     *  @return            0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(
            const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

    /** Start the interface
     *  @return             0 on success, negative on failure
     */
    virtual nsapi_error_t connect();

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    virtual nsapi_error_t disconnect();

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

    virtual EMACInterface *emacInterface() { return this; }

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack 
     */
    virtual NetworkStack *get_stack();

    EMAC &_emac;
    OnboardNetworkStack &_stack;
    OnboardNetworkStack::Interface *_interface;
    bool _dhcp;
    char _mac_address[NSAPI_MAC_SIZE];
    char _ip_address[NSAPI_IPv6_SIZE];
    char _netmask[NSAPI_IPv4_SIZE];
    char _gateway[NSAPI_IPv4_SIZE];
};

#endif
