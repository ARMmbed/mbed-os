/*
 * (c) (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
 *
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress's integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

#ifndef SCL_EMAC_H_
#define SCL_EMAC_H_

/** @file
 *  Provides EMAC interface functions to be used with SCL_EMAC Object
 */
#include "EMAC.h"
#include "EMACInterface.h"
#include "WiFiInterface.h"
#include "scl_common.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"
#include "scl_wifi_api.h"
class SCL_EMAC : public EMAC {
public:
    SCL_EMAC();
    SCL_EMAC(scl_interface_role_t itype);

    static SCL_EMAC &get_instance();

    static SCL_EMAC &get_instance(scl_interface_role_t role);

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const;

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
     * align link out memory buffer chains using the alignment.
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
     * Copies HW address to provided memory
     * @param addr HW address for underlying interface, it has to be of correct size see @a get_hwaddr_size
     * @return     true if HW address is available
     */
    virtual bool get_hwaddr(uint8_t *addr) const;

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
     * Sets a callback that needs to be called for packets received for that interface
     *
     * @param input_cb Function to be register as a callback
     */
    virtual void set_link_input_cb(emac_link_input_cb_t input_cb);

    /**
     * Sets a callback that needs to be called on link status changes for given interface
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

    /** Set callback to receive EMAC activity events
     *
     * @param activity_cb The callback for activity events
     */
    virtual void set_activity_cb(mbed::Callback<void(bool is_tx_activity)> activity_cb);

    emac_link_input_cb_t emac_link_input_cb = NULL; /* Callback for incoming data */
    emac_link_state_change_cb_t emac_link_state_cb = NULL;
    EMACMemoryManager *memory_manager;
    bool powered_up = false;
    bool link_state = false;
    scl_interface_role_t interface_type;

    scl_mac_t multicast_addr;
    mbed::Callback<void(bool)> activity_cb;

};
extern "C" void scl_emac_wifi_link_state_changed(bool state_up);

#endif /* SCL_EMAC_H_ */
