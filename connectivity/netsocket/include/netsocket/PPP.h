/*
 * Copyright (c) 2019 ARM Limited
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

#ifndef PPP_H_
#define PPP_H_

#include <stdbool.h>
#include "Callback.h"
#include "FileHandle.h"
#include "NetStackMemoryManager.h"

class PPP {
public:

    /** Return the default on-board PPP
     *
     * Returns the default on-board PPP - this will be target-specific, and
     * may not be available on all targets.
     */
    static PPP &get_default_instance();

    virtual ~PPP() = default;

    /**
     * Callback to be registered with PPP interface and to be called for received packets
     *
     * @param buf  Received data
     */
    //typedef void (*ppp_link_input_fn)(void *data, net_stack_mem_buf_t *buf);
    typedef mbed::Callback<void (net_stack_mem_buf_t *buf)> ppp_link_input_cb_t;

    /**
     * Callback to be registered with PPP interface and to be called for link status changes
     *
     * @param  up   Link status
     */
    //typedef void (*ppp_link_state_change_fn)(void *data, bool up);
    typedef mbed::Callback<void (bool up)> ppp_link_state_change_cb_t;

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() = 0;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the cellular device.
     * @return         Memory alignment requirement in bytes
     */
    virtual uint32_t get_align_preference() const = 0;

    /**
     * Return interface name
     *
     * @param name Pointer to where the name should be written
     * @param size Maximum number of characters to copy
     */
    virtual void get_ifname(char *name, uint8_t size) const = 0;

    /**
     * Sends the packet over the link
     *
     * That cannot be called from an interrupt context.
     *
     * @param buf  Packet to be sent
     * @return     True if the packet was sent, false otherwise
     */
    virtual bool link_out(net_stack_mem_buf_t *buf, nsapi_ip_stack_t ip_stack) = 0;

    /**
     * Initializes the PPP
     *
     * @return True on success, False in case of an error.
     */
    virtual bool power_up() = 0;

    /**
     * Deinitializes the PPP
     *
     */
    virtual void power_down() = 0;

    /**
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(ppp_link_input_cb_t input_cb) = 0;

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(ppp_link_state_change_cb_t state_cb) = 0;

    /** Sets memory manager that is used to handle memory buffers
     *
     * @param mem_mngr Pointer to memory manager
     */
    virtual void set_memory_manager(NetStackMemoryManager &mem_mngr) = 0;

    /** Sets file stream used to communicate with modem
     *
     * @param stream Pointer to file handle
     */
    virtual void set_stream(mbed::FileHandle *stream) = 0;

    /** Sets IP protocol versions of IP stack
     *
     * @param ip_stack IP protocol version
     */
    virtual void set_ip_stack(nsapi_ip_stack_t ip_stack) = 0;

    /** Sets user name and password for PPP protocol
     *
     * @param uname    User name
     * @param password Password
     */
    virtual void set_credentials(const char *uname, const char *password) = 0;

    /** Gets local IP address
     *
     * @param version IP address version
     * @return        IP address
     */
    virtual const nsapi_addr_t *get_ip_address(nsapi_version_t version) = 0;

    /** Get the local network mask.
     *
     *  @return         Local network mask or null if no network mask has been received.
     */
    virtual const nsapi_addr_t *get_netmask() = 0;

    /** Get the local gateway.
     *
     *  @return         Local gateway or null if no network mask has been received.
     */
    virtual const nsapi_addr_t *get_gateway() = 0;

    /** Gets DNS server address
     *
     * @param index Server index
     */
    virtual const nsapi_addr_t *get_dns_server(uint8_t index) = 0;
};

#endif /* PPP_H_ */
