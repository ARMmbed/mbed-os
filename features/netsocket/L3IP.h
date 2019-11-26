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

#ifndef L3IP_H
#define L3IP_H

#include <stdbool.h>
#include "Callback.h"
#include "NetStackMemoryManager.h"
#include "SocketAddress.h"

/**
 * This interface should be used to abstract low level access to networking hardware
 * All operations receive a `void *` hardware pointer which an l3ip device provides when
 * it is registered with a stack.
 */
class L3IP {
public:

    /** Return the default on-board L3IP
     *
     * Returns the default on-board L3IP - this will be target-specific, and
     * may not be available on all targets.
     */
    static L3IP &get_default_instance();

    /**
     * Callback to be registered with L3IP interface and to be called for received packets
     *
     * @param buf  Received data
     */
    //typedef void (*l3ip_link_input_fn)(void *data, net_stack_mem_buf_t *buf);
    typedef mbed::Callback<void (net_stack_mem_buf_t *buf)> l3ip_link_input_cb_t;

    /**
     * Callback to be registered with L3IP interface and to be called for link status changes
     *
     * @param  up   Link status
     */
    //typedef void (*l3ip_link_state_change_fn)(void *data, bool up);
    typedef mbed::Callback<void (bool up)> l3ip_link_state_change_cb_t;

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const = 0;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the l3ip device.
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
     * @param buf  Packet to be send
     * @return     True if the packet was send successfully, false otherwise
     */
    virtual bool link_out(net_stack_mem_buf_t *buf) = 0;

    /**
     * Initializes the hardware
     *
     * @return True on success, False in case of an error.
     */
    virtual bool power_up() = 0;

    /**
     * Deinitializes the hardware
     *
     */
    virtual void power_down() = 0;

    /**
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(l3ip_link_input_cb_t input_cb) = 0;

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(l3ip_link_state_change_cb_t state_cb) = 0;

    /** Add device to an IP4 multicast group
     *
     * @param address  An IP4 multicast group address
     */
    virtual void add_ipv4_multicast_group(const SocketAddress &address) = 0;

    /** Add device to an IP6 multicast group
     *
     * @param address  An IP6 multicast group address
     */
    virtual void add_ipv6_multicast_group(const SocketAddress &address) = 0;

    /** Remove device from an IPV4 multicast group
     *
     * @param address  An IPV4 multicast group address
     */
    virtual void remove_ipv4_multicast_group(const SocketAddress &address) = 0;

    /** Remove device from an IPV6 multicast group
    *
    * @param address  An IPV6 multicast group address
    */
    virtual void remove_ipv6_multicast_group(const SocketAddress &address) = 0;

    /** Request reception of all multicast packets
     *
     * @param all True to receive all multicasts
     *            False to receive only multicasts addressed to specified groups
     */
    virtual void set_all_multicast(bool all) = 0;

    /** Sets memory manager that is used to handle memory buffers
     *
     * @param mem_mngr Pointer to memory manager
     */
    virtual void set_memory_manager(NetStackMemoryManager &mem_mngr) = 0;
};




#endif  /* L3IP_H */
