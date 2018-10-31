/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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
     * Gets preferred memory buffer alignment of the cellular device. IP stack may or may not
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
     * Sends the packet over the link
     *
     * That can not be called from an interrupt context.
     *
     * @param buf  Packet to be send
     * @return     True if the packet was send successfully, False otherwise
     */
    virtual bool link_out(net_stack_mem_buf_t *buf);

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
    virtual void set_link_input_cb(l3ip_link_input_cb_t input_cb);

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(l3ip_link_state_change_cb_t state_cb);

    /** Add device to an IP4 multicast group
     *
     * @param address an IP4 multicast group  address
     */
    virtual void add_ipv4_multicast_group(const char *address);

     /** Add device to an IP6 multicast group
     *
     * @param address  an IP6 multicast group  address
     */
    virtual void add_ipv6_multicast_group(const char *address);

    /** Remove device from an IPV4 multicast group
     *
     * @param address  An IPV4 multicast group  address
     */
    virtual void remove_ipv4_multicast_group(const char *address);

    /** Remove device from an IPV6 multicast group
    *
    * @param address  An IPV6 multicast group  address
    */
    virtual void remove_ipv6_multicast_group(const char *address);

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
