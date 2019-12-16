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
#include "EMAC.h"
#include "OnboardNetworkStack.h"


/** EMACInterface class
 *  Implementation of the NetworkInterface for an EMAC-based driver
 *
 * This class provides the necessary glue logic to create a NetworkInterface
 * based on an EMAC and an OnboardNetworkStack. EthernetInterface and
 * EMAC-based Wi-Fi drivers derive from it.
 *
 * Drivers derived from EMACInterface should be constructed so that their
 * EMAC is functional without the need to call `connect()`. For example
 * a Wi-Fi driver should permit `WiFi::get_emac().power_up()` as soon as
 * the credentials have been set. This is necessary to support specialized
 * applications such as 6LoWPAN mesh border routers.
 */
class EMACInterface : public virtual NetworkInterface {
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
    EMACInterface(EMAC &emac = EMAC::get_default_instance(),
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
    virtual nsapi_error_t set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual nsapi_error_t set_network(const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @retval         NSAPI_ERROR_OK on success.
     *  @retval         NSAPI_ERROR_UNSUPPORTED if operation is not supported.
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

    /** @copydoc NetworkInterface::connect */
    virtual nsapi_error_t connect();

    /** @copydoc NetworkInterface::disconnect */
    virtual nsapi_error_t disconnect();

    /** @copydoc NetworkInterface::get_mac_address */
    virtual const char *get_mac_address();

    /** @copydoc NetworkInterface::get_ip_address */
    virtual nsapi_error_t get_ip_address(SocketAddress *address);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_ip_address();

    /** @copydoc NetworkInterface::get_ipv6_link_local_address */
    virtual nsapi_error_t get_ipv6_link_local_address(SocketAddress *address);

    /** @copydoc NetworkInterface::get_netmask */
    virtual nsapi_error_t get_netmask(SocketAddress *address);

    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_netmask();

    /** @copydoc NetworkInterface::get_gateway */
    virtual nsapi_error_t get_gateway(SocketAddress *address);

    /** @copydoc NetworkInterface::get_gateway */
    MBED_DEPRECATED_SINCE("mbed-os-5.15", "String-based APIs are deprecated")
    virtual const char *get_gateway();

    /** @copydoc NetworkInterface::get_interface_name */
    virtual char *get_interface_name(char *interface_name);

    /** @copydoc NetworkInterface::set_as_default */
    virtual void set_as_default();

    /** @copydoc NetworkInterface::attach */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** @copydoc NetworkInterface::get_connection_status */
    virtual nsapi_connection_status_t get_connection_status() const;

    /** @copydoc NetworkInterface::set_blocking */
    virtual nsapi_error_t set_blocking(bool blocking);

    /** Provide access to the EMAC
     *
     * This should be used with care - normally the network stack would
     * control the EMAC, so manipulating the EMAC while the stack
     * is also using it (ie after connect) will likely cause problems.
     *
     * @return          Reference to the EMAC in use
     */
    EMAC &get_emac() const
    {
        return _emac;
    }

    virtual EMACInterface *emacInterface()
    {
        return this;
    }

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
    bool _blocking;
    char _mac_address[NSAPI_MAC_SIZE];
    char _ip_address[NSAPI_IPv6_SIZE];
    char _netmask[NSAPI_IPv4_SIZE];
    char _gateway[NSAPI_IPv4_SIZE];
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
};

#endif
