/*
 * Copyright (c) 2018 ARM Limited
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

#ifndef L3IP_INTERFACE_H
#define L3IP_INTERFACE_H

#include "nsapi.h"
#include "L3IP.h"
#include "OnboardNetworkStack.h"
#include "SocketAddress.h"

/** L3IPInterface class
 *  Implementation of the NetworkInterface for an IP-based driver
 *
 * This class provides the necessary glue logic to create a NetworkInterface
 * based on an L3IP and an OnboardNetworkStack. Cellular Interface  drivers derive from it.
 *
 * Drivers derived from L3IPInterface should be constructed so that their
 * L3IP is functional without the need to call `connect()`.
 */
class L3IPInterface : public virtual NetworkInterface {
public:
    /** Create an L3IP-based network interface.
     *
     * The default arguments obtain the default L3IP, which will be target-
     * dependent (and the target may have some JSON option to choose which
     * is the default, if there are multiple). The default stack is configured
     * by JSON option nsapi.default-stack.
     *
     * Due to inability to return errors from the constructor, no real
     * work is done until the first call to connect().
     *
     * @param l3ip  Reference to L3IP to use
     * @param stack Reference to onboard-network stack to use
     */
    L3IPInterface(L3IP &l3ip = L3IP::get_default_instance(),
                  OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());
    ~L3IPInterface() override;
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
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t set_dhcp(bool dhcp) override;

    /** Start the interface
     *  @return             0 on success, negative on failure
     */
    nsapi_error_t connect() override;

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    nsapi_error_t disconnect() override;

    /** @copydoc NetworkInterface::get_ip_address */
    nsapi_error_t get_ip_address(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_netmask */
    nsapi_error_t get_netmask(SocketAddress *address) override;

    /** @copydoc NetworkInterface::get_gateway */
    nsapi_error_t get_gateway(SocketAddress *address) override;

    /** Get the network interface name
     *
     *  @return         Null-terminated representation of the network interface name
     *                  or null if  interface not exists
     */
    char *get_interface_name(char *interface_name) override;

    /** Set the network interface as default one
      */
    void set_as_default() override;

    /** Register callback for status reporting
     *
     *  @param status_cb The callback for status changes
     */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) override;

    /** Get the connection status
     *
     *  @return         The connection status according to nsapi_connection_status_t
     */
    nsapi_connection_status_t get_connection_status() const override;

    /** Set blocking status of connect() which by default should be blocking
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t set_blocking(bool blocking) override;

    /** Provide access to the L3IP
     *
     * This should be used with care - normally the network stack would
     * control the L3IP, so manipulating the L3IP while the stack
     * is also using it (ie after connect) will likely cause problems.
     *
     * @return          Reference to the L3IP in use
     */
    L3IP &getl3ip() const
    {
        return _l3ip;
    }

#if 0
    /* NetworkInterface does not currently have l3ipInterface, so this
     * "dynamic cast" is non-functional.
     */
    L3IPInterface *l3ipInterface() final
    {
        return this;
    }
#endif

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack
     */
    NetworkStack *get_stack() override;

    L3IP &_l3ip;
    OnboardNetworkStack &_stack;
    OnboardNetworkStack::Interface *_interface = nullptr;
    bool _dhcp = true;
    bool _blocking = true;
    SocketAddress _ip_address;
    SocketAddress _netmask;
    SocketAddress _gateway;
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
};

#endif
