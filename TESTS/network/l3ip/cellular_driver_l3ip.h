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

#ifndef CELLULAR_DRIVER_L3IP_H_
#define CELLULAR_DRIVER_L3IP_H_

#include "L3IP.h"


class Cellular_driver_L3IP : public L3IP {
public:
    Cellular_driver_L3IP();

    static Cellular_driver_L3IP &get_instance();

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the cellular device.
     * @return         Memory alignment requirement in bytes
     */
    virtual uint32_t get_align_preference() const;

    /**
     * Return interface name
     *
     * @param name Pointer to where the name should be written
     * @param size Maximum number of characters to copy
     */
    virtual void get_ifname(char *name, uint8_t size) const;

    /**
     * Sends the packet over the link
     *
     * That cannot be called from an interrupt context.
     *
     * @param buf  Packet to be sent
     * @return     True if the packet was sent, false otherwise
     */
    virtual bool link_out(net_stack_mem_buf_t *buf);

    /**
     * Initializes the hardware
     *
     * @return True on success, False in case of an error.
     */
    virtual bool power_up();

    /**
     * Deinitializes the hardware
     *
     */
    virtual void power_down();

    /**
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(l3ip_link_input_cb_t input_cb);

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(l3ip_link_state_change_cb_t state_cb);

    /** Add device to an IP4 multicast group
     *
     * @param address an IP4 multicast group address
     */
    virtual void add_ipv4_multicast_group(const SocketAddress &address);

    /** Add device to an IP6 multicast group
    *
    * @param address  an IP6 multicast group address
    */
    virtual void add_ipv6_multicast_group(const SocketAddress &address);

    /** Remove device from an IPV4 multicast group
     *
     * @param address  An IPV4 multicast group address
     */
    virtual void remove_ipv4_multicast_group(const SocketAddress &address);

    /** Remove device from an IPV6 multicast group
    *
    * @param address  An IPV6 multicast group address
    */
    virtual void remove_ipv6_multicast_group(const SocketAddress &address);

    /** Request reception of all multicast packets
     *
     * @param all True to receive all multicasts
     *            False to receive only multicasts addressed to specified groups
     */
    virtual void set_all_multicast(bool all);

    /** Sets memory manager that is used to handle memory buffers
     *
     * @param mem_mngr Pointer to memory manager
     */
    virtual void set_memory_manager(NetStackMemoryManager &mem_mngr);

private:

    l3ip_link_input_cb_t l3ip_link_input_cb; /**< Callback for incoming data */
    l3ip_link_state_change_cb_t l3ip_link_state_cb; /**< Link state change callback */
    NetStackMemoryManager *memory_manager; /**< Memory manager */

};

#endif /* CELLULAR_DRIVER_L3IP_H_ */
