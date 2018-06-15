/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
 */

#ifndef KINETIS_EMAC_H_
#define KINETIS_EMAC_H_

#include "EMAC.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

class Kinetis_EMAC : public EMAC {
public:
    Kinetis_EMAC();

    static Kinetis_EMAC &get_instance();

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

private:
    bool low_level_init_successful();
    void rx_isr();
    void tx_isr();
    void packet_rx();
    void packet_tx();
    void tx_reclaim();
    void input(int idx);
    emac_mem_buf_t *low_level_input(int idx);
    static void thread_function(void* pvParameters);
    void phy_task();
    static void ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param);

    os_thread_t thread_cb;
    osThreadId_t thread; /**< Processing thread */
    rtos::Mutex TXLockMutex;/**< TX critical section mutex */
    rtos::Semaphore xTXDCountSem; /**< TX free buffer counting semaphore */
    uint8_t tx_consume_index, tx_produce_index; /**< TX buffers ring */
    emac_link_input_cb_t emac_link_input_cb; /**< Callback for incoming data */
    emac_link_state_change_cb_t emac_link_state_cb; /**< Link state change callback */
    EMACMemoryManager *memory_manager; /**< Memory manager */
    int phy_task_handle; /**< Handle for phy task event */
    struct PHY_STATE {
        int connected;
        phy_speed_t speed;
        phy_duplex_t duplex;
    };
    PHY_STATE prev_state;
    uint8_t hwaddr[KINETIS_HWADDR_SIZE];
};

#endif /* KINETIS_EMAC_H_ */
