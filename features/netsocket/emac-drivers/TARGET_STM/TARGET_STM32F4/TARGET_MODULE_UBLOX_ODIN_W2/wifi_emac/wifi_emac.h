/* Copyright (c) 2017 ARM Limited
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

#ifndef WIFI_EMAC_H_
#define WIFI_EMAC_H_

#include "EMAC.h"
#include "cb_wlan_target_data.h"
#include "cb_wlan.h"

class OdinWiFiEMAC : public EMAC {
public:
    OdinWiFiEMAC();

    static OdinWiFiEMAC &get_instance();

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size() const;

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
     * Copies HW address to provided memory, @param addr has to be of correct size see @a get_hwaddr_size
     *
     * HW address need not be provided if this interface does not have its own HW
     * address configuration; stack will choose address from central system
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

    /** Sets memory manager that is used to handle memory buffers
     *
     * @param mem_mngr Pointer to memory manager
     */
    virtual void set_memory_manager(EMACMemoryManager &mem_mngr);

    virtual uint32_t get_align_preference() const;
    virtual void add_multicast_group(const uint8_t *address);
    virtual void remove_multicast_group(const uint8_t *address);
    virtual void set_all_multicast(bool all);

    cbWLAN_Handle get_wifi_emac_handle();
    void set_wifi_emac_handle(cbWLAN_Handle _handle);

private:

    cbWLAN_Handle handle = cbWLAN_DEFAULT_HANDLE;

    emac_link_input_cb_t emac_link_input_cb; /**< Callback for incoming data */
    emac_link_state_change_cb_t emac_link_state_cb; /**< Link state change callback */
    EMACMemoryManager *memory_manager;

    friend cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame);
    friend cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame);
    friend cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size);
    friend void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame);
    friend cb_uint32 handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame);

    friend void handleWlanStatusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data);
    friend void handleWlanPacketIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo);
    friend void send_wlan_packet(OdinWiFiEMAC *ptr, void *buf);
};

#endif /* WIFI_EMAC_H_ */
