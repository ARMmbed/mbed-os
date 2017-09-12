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

#ifndef MBED_EMAC_API_H
#define MBED_EMAC_API_H

#include <stdbool.h>
#include "emac_stack_mem.h"
#include "cmsis_os.h"

/**
 * emac_interface_ops_t
 *
 * This interface should be used to abstract low level access to networking hardware
 * All operations receive a `void *` hw pointer which an emac device provides when
 * it is registered with a stack.
 */


/**
 * Callback to be register with Emac interface and to be called fore received packets
 *
 * @param data Arbitrary user data (IP stack)
 * @param buf  Received data
 */
typedef void (*emac_link_input_fn)(void *data, emac_stack_mem_chain_t *buf);

/**
 * Callback to be register with Emac interface and to be called for link status changes
 *
 * @param  data Arbitrary user data (IP stack)
 * @param  up   Link status
 */
typedef void (*emac_link_state_change_fn)(void *data, bool up);

/**
 * Return maximum transmission unit
 *
 * @param hw   Emac interface's handle
 * @return     MTU in bytes
 */
typedef uint32_t (*emac_get_mtu_size_fn)(void *hw);

/**
 * Return interface name
 *
 * @param hw   Emac interface's handle
 * @param name Pointer to where the name should be written
 * @param size Maximum number of character to copy
 */
typedef void (*emac_get_ifname_fn)(void *hw, char *name, uint8_t size);

/**
 * Returns size of the underlying interface HW address size
 *
 * @param hw   Emac interface's handle
 * @return     HW address size in bytes
 */
typedef uint8_t (*emac_get_hwaddr_size_fn)(void *hw);

/**
 * Return interface hw address
 *
 * Copies HW address to provided memory, @param addr has to be of correct size see @a get_hwaddr_size
 *
 * @param hw   Emac interface's handle
 * @param addr HW address for underlying interface
 */
typedef void (*emac_get_hwaddr_fn)(void *hw, uint8_t *addr);

/**
 * Set HW address for interface
 *
 * Provided address has to be of correct size, see @a get_hwaddr_size
 *
 * @param hw   Emac interface's handle
 * @param addr Address to be set
 */
typedef void (*emac_set_hwaddr_fn)(void *hw, const uint8_t *addr);

/**
 * Sends the packet over the link
 *
 * That can not be called from an interrupt context.
 *
 * @param hw   Emac interface's handle
 * @param buf  Packet to be send
 * @return     True if the packet was send successfully, False otherwise
 */
typedef bool (*emac_link_out_fn)(void *hw, emac_stack_mem_chain_t *buf);

/**
 * Initializes the HW
 *
 * @param hw   Emac interface's handle
 * @return True on success, False in case of an error.
 */
typedef bool (*emac_power_up_fn)(void *hw);

/**
 * Deinitializes the HW
 *
 * @param hw   Emac interface's handle
 */
typedef void (*emac_power_down_fn)(void *hw);

/**
 * Sets a callback that needs to be called for packets received for that interface
 *
 * @param hw       Emac interface's handle
 * @param input_cb Function to be register as a callback
 * @param data     Arbitrary user data to be passed to the callback
 */
typedef void (*emac_set_link_input_cb_fn)(void *hw,  emac_link_input_fn input_cb, void *data);

/**
 * Sets a callback that needs to be called on link status changes for given interface
 *
 * @param hw       Emac interface's handle
 * @param state_cb Function to be register as a callback
 * @param data     Arbitrary user data to be passed to the callback
 */
typedef void (*emac_set_link_state_cb_fn)(void *hw, emac_link_state_change_fn state_cb, void *data);

/** Add device to a multicast group
 *
 * @param hw       Emac interface's handle
 * @param address  A multicast group hardware address
 */
typedef void (*emac_add_multicast_group)(void *hw,  uint8_t *address);

typedef struct emac_interface_ops {
    emac_get_mtu_size_fn        get_mtu_size;
    emac_get_ifname_fn          get_ifname;
    emac_get_hwaddr_size_fn     get_hwaddr_size;
    emac_get_hwaddr_fn          get_hwaddr;
    emac_set_hwaddr_fn          set_hwaddr;
    emac_link_out_fn            link_out;
    emac_power_up_fn            power_up;
    emac_power_down_fn          power_down;
    emac_set_link_input_cb_fn   set_link_input_cb;
    emac_set_link_state_cb_fn   set_link_state_cb;
    emac_add_multicast_group    add_multicast_group;
} emac_interface_ops_t;

/** These need to be defined by targets wishing to provide an Ethernet driver using EMAC interface. It will
 *  be used by the EthernetInterface class's default constructor to initialise the networking subsystem.
 */
extern const emac_interface_ops_t mbed_emac_eth_ops_default;
extern void *mbed_emac_eth_hw_default;

#endif  /* MBED_EMAC_API_H */
