/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef EMAC_H
#define EMAC_H

#include <stdbool.h>
#include "Callback.h"
#include "EMACMemoryManager.h"

// Nuvoton platform headers define EMAC - avoid the collision
#undef EMAC

/**
 * This interface should be used to abstract low level access to networking hardware
 * All operations receive a `void *` hw pointer which an emac device provides when
 * it is registered with a stack.
 */
class EMAC {
public:

    /** Return the default on-board EMAC
     *
     * Returns the default on-board EMAC - this will be target-specific, and
     * may not be available on all targets.
     */
    static EMAC &get_default_instance();

    /**
     * Callback to be register with Emac interface and to be called for received packets
     *
     * @param buf  Received data
     */
    //typedef void (*emac_link_input_fn)(void *data, emac_mem_buf_t *buf);
    typedef mbed::Callback<void (emac_mem_buf_t *buf)> emac_link_input_cb_t;

    /**
     * Callback to be register with Emac interface and to be called for link status changes
     *
     * @param  up   Link status
     */
    //typedef void (*emac_link_state_change_fn)(void *data, bool up);
    typedef mbed::Callback<void (bool up)> emac_link_state_change_cb_t;

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const = 0;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
     * align link out memory buffer chains using the alignment.
     *
     * @return         Memory alignment requirement in bytes
     */
    virtual uint32_t get_align_preference() const = 0;

    /**
     * Return interface name
     *
     * @param name Pointer to where the name should be written
     * @param size Maximum number of character to copy
     */
    virtual void get_ifname(char *name, uint8_t size) const = 0;

    /**
     * Returns size of the underlying interface HW address size.
     *
     * @return     HW address size in bytes
     */
    virtual uint8_t get_hwaddr_size() const = 0;

    /**
     * Return interface-supplied HW address
     *
     * Copies HW address to provided memory, @param addr has to be of correct size see @a get_hwaddr_size
     *
     * HW address need not be provided if this interface does not have its own HW
     * address configuration; stack will choose address from central system
     * configuration if the function returns false and does not write to addr.
     *
     * @param addr HW address for underlying interface
     * @return     true if HW address is available
     */
    virtual bool get_hwaddr(uint8_t *addr) const = 0;

    /**
     * Set HW address for interface
     *
     * Provided address has to be of correct size, see @a get_hwaddr_size
     *
     * Called to set the MAC address to actually use - if @a get_hwaddr is provided
     * the stack would normally use that, but it could be overridden, eg for test
     * purposes.
     *
     * @param addr Address to be set
     */
    virtual void set_hwaddr(const uint8_t *addr) = 0;

    /**
     * Sends the packet over the link
     *
     * That can not be called from an interrupt context.
     *
     * @param buf  Packet to be send
     * @return     True if the packet was send successfully, False otherwise
     */
    virtual bool link_out(emac_mem_buf_t *buf) = 0;

    /**
     * Initializes the HW
     *
     * @return True on success, False in case of an error.
     */
    virtual bool power_up() = 0;

    /**
     * Deinitializes the HW
     *
     */
    virtual void power_down() = 0;

    /**
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(emac_link_input_cb_t input_cb) = 0;

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(emac_link_state_change_cb_t state_cb) = 0;

    /** Add device to a multicast group
     *
     * @param address  A multicast group hardware address
     */
    virtual void add_multicast_group(const uint8_t *address) = 0;

    /** Remove device from a multicast group
     *
     * @param address  A multicast group hardware address
     */
    virtual void remove_multicast_group(const uint8_t *address) = 0;

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
    virtual void set_memory_manager(EMACMemoryManager &mem_mngr) = 0;
};


/** These need to be defined by targets wishing to provide an Ethernet driver using EMAC interface. It will
 *  be used by the EMACInterface class's default constructor to initialise the networking subsystem.
 */
//extern const emac_interface_ops_t mbed_emac_eth_ops_default;
//extern void *mbed_emac_eth_hw_default;

#endif  /* EMAC_H */
