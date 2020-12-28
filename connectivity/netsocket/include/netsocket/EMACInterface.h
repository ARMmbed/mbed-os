/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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
     *  @param ip_address  SocketAddress representation of the local IP address
     *  @param netmask     SocketAddress representation of the local network mask
     *  @param gateway     SocketAddress representation of the local gateway
     *  @return            0 on success, negative error code on failure
     */
    nsapi_error_t set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway) override;

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @retval         NSAPI_ERROR_OK on success.
     *  @retval         NSAPI_ERROR_UNSUPPORTED if operation is not supported.
     */
    nsapi_error_t set_dhcp(bool dhcp) override;

    /** @copydoc NetworkInterface::connect */
    nsapi_error_t connect() override;

    /** @copydoc NetworkInterface::disconnect */
    nsapi_error_t disconnect() override;

    /** @copydoc NetworkInterface::get_mac_address */
    const char *get_mac_address() override;

    /** @copydoc NetworkInterface::set_mac_address */
    nsapi_error_t set_mac_address(uint8_t *mac_addr, nsapi_size_t addr_len) override;

    /** @copydoc NetworkInterface::get_ip_address */
    nsapi_error_t get_ip_address(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_ipv6_link_local_address */
    nsapi_error_t get_ipv6_link_local_address(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_netmask */
    nsapi_error_t get_netmask(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_gateway */
    nsapi_error_t get_gateway(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_interface_name */
    char *get_interface_name(char *interface_name) override;

    /** @copydoc NetworkInterface::set_as_default */
    void set_as_default() override;

    /** @copydoc NetworkInterface::attach */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) override;

    /** @copydoc NetworkInterface::get_connection_status */
    nsapi_connection_status_t get_connection_status() const override;

    /** @copydoc NetworkInterface::set_blocking */
    nsapi_error_t set_blocking(bool blocking) override;

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

    EMACInterface *emacInterface() final
    {
        return this;
    }

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack
     */
    NetworkStack *get_stack() final;

    EMAC &_emac;
    OnboardNetworkStack &_stack;
    OnboardNetworkStack::Interface *_interface = nullptr;
    bool _dhcp = true;
    bool _blocking = true;
    bool _hw_mac_addr_set = false;
    char _mac_address[NSAPI_MAC_SIZE];
    char _ip_address[NSAPI_IPv6_SIZE] {};
    char _netmask[NSAPI_IPv4_SIZE] {};
    char _gateway[NSAPI_IPv4_SIZE] {};
    uint8_t _hw_mac_addr[NSAPI_MAC_BYTES] {};
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
};

#endif
