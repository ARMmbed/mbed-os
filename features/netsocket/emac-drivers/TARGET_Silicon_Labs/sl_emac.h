/***************************************************************************//**
 * @file sl_eth_phy.h
 *******************************************************************************
 * @section License
 * <b>modifications (C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 * <b>original Copyright (c) 2015 ARM Limited</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef SL_EMAC_H_
#define SL_EMAC_H_

#include "EMAC.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

class SL_EMAC : public EMAC {
public:
    SL_EMAC();

    static SL_EMAC &get_instance();

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the Emac device.
     * IP stack may or may not align link out memory buffer chains
     * using the alignment.
     *
     * @return         Memory alignment requirement in bytes
     */
    virtual uint32_t get_align_preference() const;

    /**
     * Return interface name
     *
     * @param name Pointer to where the name should be written
     * @param size Maximum number of character to copy
     */
    virtual void get_ifname(char *name, uint8_t size) const;

    /**
     * Returns size of the underlying interface HW address size.
     *
     * @return     HW address size in bytes
     */
    virtual uint8_t get_hwaddr_size() const;

    /**
     * Return interface-supplied HW address
     *
     * Copies HW address to provided memory, @param addr has to be of correct
     * size see @a get_hwaddr_size
     *
     * HW address need not be provided if this interface does not have its own
     * HW address configuration; stack will choose address from central system
     * configuration if the function returns false and does not write to addr.
     *
     * @param addr HW address for underlying interface
     * @return     true if HW address is available
     */
    virtual bool get_hwaddr(uint8_t *addr) const;

    /**
     * Set HW address for interface
     *
     * Provided address has to be of correct size, see @a get_hwaddr_size
     *
     * Called to set the MAC address to actually use - if @a get_hwaddr is
     * provided the stack would normally use that, but it could be overridden,
     * eg for test purposes.
     *
     * @param addr Address to be set
     */
    virtual void set_hwaddr(const uint8_t *addr);

    /**
     * Sends the packet over the link
     *
     * That can not be called from an interrupt context.
     *
     * @param buf  Packet to be send
     * @return     True if the packet was send successfully, False otherwise
     */
    virtual bool link_out(emac_mem_buf_t *buf);

    /**
     * Initializes the HW
     *
     * @return True on success, False in case of an error.
     */
    virtual bool power_up();

    /**
     * Deinitializes the HW
     *
     */
    virtual void power_down();

    /**
     * Sets a callback that needs to be called for packets received for that
     * interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(emac_link_input_cb_t input_cb);

    /**
     * Sets a callback that needs to be called on link status changes for given
     * interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(emac_link_state_change_cb_t state_cb);

    /** Add device to a multicast group
     *
     * @param address  A multicast group hardware address
     */
    virtual void add_multicast_group(const uint8_t *address);

    /** Remove device from a multicast group
     *
     * @param address  A multicast group hardware address
     */
    virtual void remove_multicast_group(const uint8_t *address);

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
    virtual void set_memory_manager(EMACMemoryManager &mem_mngr);

    osThreadId_t thread;    /** Ethernet driver thread */

private:

    /* Instance variables */

    /** Semaphore protecting the TX state.
     *  Not a mutex since we're posting from IRQ
     */
    rtos::Semaphore tx_sem;
    /** (R)MII address where the detected PHY is residing */
    uint8_t phy_addr;
    /** Index in RX queue for next packet to read */
    uint8_t rx_idx;

    /** Multicast mask reference count. Multicast filtering is done using a hash
     *  bit, so multiple multicast addresses might map to the same bit. That's
     *  why there needs to be a reference count for every bit in the 64-bit mask
     */
    uint8_t mcast_hash_refcnt[64];

    /** Local reference to the buffer that's in the process of being sent */
    emac_mem_buf_t *tx_buf;
    /** List of current RX buffers, which autonomously get filled by the
     *  Ethernet peripheral.
     */
    emac_mem_buf_t *rx_bufs[SL_ETH_NUM_RX_BD];

    typedef struct {
        uint32_t addr;
        uint32_t status;
    } sl_eth_bd_t;

    /** Internal list of DMA descriptors for the RX buffer pool */
    sl_eth_bd_t rx_bds[SL_ETH_NUM_RX_BD];
    /** Internal list of DMA descriptors to point to the current buffer being
     *  sent */
    sl_eth_bd_t tx_bds[SL_ETH_NUM_TX_BD];

    /**< Processing thread */
    mbed_rtos_storage_thread_t thread_cb;

    /**< Callback for incoming data */
    emac_link_input_cb_t emac_link_input_cb;
    /**< Callback for link state change */
    emac_link_state_change_cb_t emac_link_state_cb;

    /**< Memory manager instance */
    EMACMemoryManager *memory_manager;

    bool connected;
    bool up;

    /* private functions */
    /**
     * Thread to de-escalate Ethernet peripheral IRQ's
     */
    static void eth_thread(void* instance);

    /**
     * This function polls the (R)MII bus for the first
     * available attached PHY chip, resets and enables the PHY
     * in auto-negotiation mode.
     */
    void phy_init(void);

    /**
     * Write to detected PHY register. Nop if no PHY initialized.
     */
    void write_phy(uint8_t reg_addr, uint16_t data);

    /**
     * Read from detected PHY register. Nop if no PHY initialized.
     */
    void read_phy(uint8_t reg_addr, uint16_t *data);

    /**
     * This function checks the detected PHY for its
     * current link status. Nop if no PHY was previously detected.
     * Fires callback set by set_link_state_cb on change in link state.
     */
    void link_state_poll(void);

    /**
     * Initializes buffer structures
     */
    void data_init(void);

    /**
     * De-initializes buffer structures
     */
    void data_deinit(void);
};

#endif /* SL_EMAC_H_ */
