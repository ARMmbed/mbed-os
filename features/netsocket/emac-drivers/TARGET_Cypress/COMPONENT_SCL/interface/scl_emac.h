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
 * code solely for use in connection with Cypress' integrated circuit products.
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
 * including Cypress' product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

#ifndef SCL_EMAC_H_
#define SCL_EMAC_H_

/** @file
 *  Provides EMAC interface functions to be used with the SCL_EMAC object
 *
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

    /**
     * Get the EMAC instance
     *
     * @return     Reference to SCL_EMAC object.
     */
    static SCL_EMAC &get_instance();

    /**
     * Get the EMAC instance
     *
     * @param  role    Interface type.
     *
     * @return     Reference to SCL_EMAC object.
     */
    static SCL_EMAC &get_instance(scl_interface_role_t role);

    /**
     * Returns the maximum transmission unit
     *
     * @return     MTU in bytes.
     */
    virtual uint32_t get_mtu_size() const;

    /**
     * Gets the memory buffer alignment preference
     *
     * Gets the preferred memory buffer alignment of the EMAC device. IP stack may or may not
     * align with the link out memory buffer chains using the alignment.
     *
     * @return         Memory alignment requirement in bytes.
     */
    virtual uint32_t get_align_preference() const;

    /**
     * Returns the interface name
     *
     * @param name		Pointer to the location where the name should be written.
     * @param size 		Maximum number of characters to copy.
     */
    virtual void get_ifname(char *name, uint8_t size) const;

    /**
     * Returns the size of the underlying interface HW address size.
     *
     * @return		HW address size in bytes.
     */
    virtual uint8_t get_hwaddr_size() const;

    /**
     * Returns the interface supplied HW address
     * Copies the HW address to the provided memory
     * @param addr		HW address of the underlying interface. It must be of correct size. See @a get_hwaddr_size.
     * @return     		True if HW address is available.
     */
    virtual bool get_hwaddr(uint8_t *addr) const;

    /**
     * Set HW address for the interface
     *
     * Provided address must be of correct size. See @a get_hwaddr_size.
     *
     * Called to set the MAC address to be used - if @a get_hwaddr is provided
     * the stack would normally use that, but it could be overridden for test
     * purposes.
     *
     * @param addr		Address to be set
     */
    virtual void set_hwaddr(const uint8_t *addr);

    /**
     * Sends the packet over the link
     *
     * This cannot be called from an interrupt context.
     *
     * @param buf		Packet to be sent.
     * @return     		True if the packet was sent successfully. False otherwise.
     */
    virtual bool link_out(emac_mem_buf_t *buf);

    /**
     * Initializes the HW
     *
     * @return True on success. False in case of an error.
     */
    virtual bool power_up();

    /**
     * De-initializes the HW
     */
    virtual void power_down();

    /**
     * Sets a callback that is called for packets received for a given interface
     *
     * @param input_cb		Function to be registered as a callback.
     */
    virtual void set_link_input_cb(emac_link_input_cb_t input_cb);

    /**
     * Sets a callback that is called on changes in the link status for a given interface
     *
     * @param state_cb		Function to be registered as a callback.
     */
    virtual void set_link_state_cb(emac_link_state_change_cb_t state_cb);

    /** Adds a device to a multicast group
     *
     * @param address		A multicast group hardware address.
     */
    virtual void add_multicast_group(const uint8_t *address);

    /** Removes a device from a multicast group
     *
     * @param address		A multicast group hardware address.
     */
    virtual void remove_multicast_group(const uint8_t *address);

    /** Requests reception of all multicast packets
     *
     * @param all		True to receive all multicasts.
     *            		False to receive only multicasts addressed to specified groups.
     */
    virtual void set_all_multicast(bool all);

    /** Sets memory manager used to handle memory buffers
     *
     * @param mem_mngr	Pointer to memory manager.
     */
    virtual void set_memory_manager(EMACMemoryManager &mem_mngr);

    /** Sets callback to receive EMAC activity events
     *
     * @param activity_cb	The callback for activity events.
     */
    virtual void set_activity_cb(mbed::Callback<void(bool is_tx_activity)> activity_cb);

    emac_link_input_cb_t emac_link_input_cb = NULL; /**< Callback for incoming data */
    emac_link_state_change_cb_t emac_link_state_cb = NULL; /**< Callback for network connection status */
    EMACMemoryManager *memory_manager; /**< Pointer to hold memory manager object */
    bool powered_up = false; /**< Flag for Wi-Fi power on status */
    bool link_state = false; /**< Flag for network connection status */
    scl_interface_role_t interface_type; /**< Type of the interface */
    scl_mac_t multicast_addr; /**< Multicast address */
    mbed::Callback<void(bool)> activity_cb; /**< Callback for activity on network */

};
/** Sends the change in network connection state to network stack
*
* @param state_up	Connection status.
*/
extern "C" void scl_emac_wifi_link_state_changed(bool state_up);

#endif /* SCL_EMAC_H_ */