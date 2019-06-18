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

#ifndef PPP_SERVICE_H
#define PPP_SERVICE_H

#include <stdbool.h>
#include "Callback.h"
#include "Semaphore.h"
#include "NetStackMemoryManager.h"
#include "FileHandle.h"
#include "events/EventQueue.h"
#include "netsocket/PPP.h"

/**
 * This interface should be used to abstract low level access to networking hardware
 * All operations receive a `void *` hardware pointer which an ppp device provides when
 * it is registered with a stack.
 */
class ppp_service : public PPP {
public:
    ppp_service();

    static ppp_service &get_instance();

    /**
     * Callback to be registered with PPP interface and to be called for received packets
     *
     * @param buf  Received data
     */
    //typedef void (*ppp_link_input_fn)(void *data, net_stack_mem_buf_t *buf);
    typedef mbed::Callback<void (net_stack_mem_buf_t *buf)> ppp_link_input_cb_t;

    /**
     * Callback to be registered with PPP interface and to be called for link status changes
     *
     * @param  up   Link status
     */
    //typedef void (*ppp_link_state_change_fn)(void *data, bool up);
    typedef mbed::Callback<void (bool up)> ppp_link_state_change_cb_t;

    /**
     * Return maximum transmission unit
     *
     * @return     MTU in bytes
     */
    virtual uint32_t get_mtu_size();

    /**
     * Gets memory buffer alignment preference
     *
     * Gets preferred memory buffer alignment of the ppp device.
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
     * @param buf  Packet to be send
     * @return     True if the packet was send successfully, false otherwise
     */
    virtual bool link_out(net_stack_mem_buf_t *buf, nsapi_ip_stack_t ip_stack);

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
    virtual void set_link_input_cb(ppp_link_input_cb_t input_cb);

    /**
     * Sets a callback that needs to be called on link status changes for given interface
     *
     * @param state_cb Function to be register as a callback
     */
    virtual void set_link_state_cb(ppp_link_state_change_cb_t state_cb);

    /** Sets memory manager that is used to handle memory buffers
     *
     * @param mem_mngr Pointer to memory manager
     */
    virtual void set_memory_manager(NetStackMemoryManager &mem_mngr);

    /** Sets file stream used to communicate with modem
     *
     * @param stream Pointer to file handle
     */
    virtual void set_stream(mbed::FileHandle *stream);

    /** Sets IP protocol versions of IP stack
     *
     * @param ip_stack IP protocol version
     */
    virtual void set_ip_stack(nsapi_ip_stack_t ip_stack);

    /** Sets user name and password for PPP protocol
     *
     * @param uname    User name
     * @param password Password
     */
    virtual void set_credentials(const char *uname, const char *password);

    /** Gets local IP address
     *
     * @param version IP address version
     * @return        IP address
     */
    virtual const nsapi_addr_t *get_ip_address(nsapi_version_t version);

    /** Get the local network mask.
     *
     *  @return    Local network mask or null if no network mask has been received.
     */
    virtual const nsapi_addr_t *get_netmask();

    /** Get the local gateway.
     *
     *  @return    Local gateway or null if no network mask has been received.
     */
    virtual const nsapi_addr_t *get_gateway();

    /** Gets DNS server address
     *
     * @param index Server index
     */
    virtual const nsapi_addr_t *get_dns_server(uint8_t index);

    /** Link state indication from PPP
     *
     * @param  up       Link status
     */
    virtual void link_state(bool up);

    /** Received IP packet from PPP to stack
     *
     * @param buf  Received packet
     */
    virtual void link_input(net_stack_mem_buf_t *buf);

    /** Handle to PPP event queue
     *
     *  @return    Event queue
     */
    virtual events::EventQueue *event_queue_get();

    /** Lock PPP resource
     *
     */
    virtual void resource_lock();

    /** Unlock PPP resource
     *
     */
    virtual void resource_unlock();

private:

    // PPP library interface to service
    bool prepare_event_queue();
    void ppp_input();
    void ppp_stream_sigio_callback();
    void ppp_handle_modem_hangup();
    static void ppp_link_status(struct ppp_pcb_s *pcb, int err_code, void *ctx);
    void power_up_call();
    void link_state_call(bool up);

    // PPP status functions
    nsapi_error_t ppp_stack_if_init();
    nsapi_error_t ppp_if_connect();
    nsapi_error_t ppp_if_disconnect();

    // Internal data
    mbed::FileHandle *ppp_service_stream;
    rtos::Semaphore ppp_service_close_sem;
    rtos::Mutex ppp_service_mutex;
    events::EventQueue *ppp_service_event_queue;
    NetStackMemoryManager *memory_manager; /**< Memory manager */
    ppp_link_input_cb_t ppp_link_input_cb; /**< Callback for incoming data */
    ppp_link_state_change_cb_t ppp_link_state_cb; /**< Link state change callback */

    struct netif *ppp_service_netif;
    void *ppp_service_pcb;
    nsapi_ip_stack_t ppp_service_stack;
    const char *ppp_service_uname;
    const char *ppp_service_password;
    nsapi_error_t ppp_service_connect_error_code;
    bool ppp_service_active : 1;
    bool ppp_service_event_queued : 1;
    bool ppp_service_terminating : 1;
    bool ppp_link_state_up : 1;
};

#endif  /* PPP_SERVICE_H */
