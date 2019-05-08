/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NANOSTACKPPPINTERFACE_H
#define NANOSTACKPPPINTERFACE_H

#include "nsapi.h"
#include "NetworkInterface.h"
#include "Nanostack.h"
#include "OnboardNetworkStack.h"
#include "nsdynmemLIB.h"

extern "C" { // "pppos.h" is missing extern C
#include "pppapi.h"
}
//#include "ppp.h"
//#include "netif/ppp/pppapi.h"

class NanostackPppInterface : public OnboardNetworkStack::Interface {
public:

	NanostackPppInterface(NanostackMemoryManager &mem, void *hw, bool default_if, nsapi_ip_stack_t stack);
	
    ~NanostackPppInterface();

    nsapi_error_t ppp_if_init();
    virtual nsapi_error_t connect();
    virtual nsapi_error_t disconnect();
    virtual nsapi_error_t bringup(bool dhcp, const char *ip,
                                  const char *netmask, const char *gw,
                                  nsapi_ip_stack_t stack = DEFAULT_STACK,
                                  bool blocking = true
                                 );

    virtual nsapi_error_t bringdown();
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);
    virtual nsapi_connection_status_t get_connection_status() const;
    virtual char *get_interface_name(char *buf);
    virtual char *get_mac_address(char *buf, nsapi_size_t buflen);
    virtual char *get_netmask(char *buf, nsapi_size_t buflen);
    virtual char *get_gateway(char *buf, nsapi_size_t buflen);
    virtual char *get_ip_address(char *buf, nsapi_size_t buflen);
    virtual char *get_ip_address_if(char *buf, nsapi_size_t buflen, const char *interface_name);

    struct netif netif;

private:
    virtual NetworkStack *get_stack();
    //OnboardNetworkStack &_stack;
    void *hw; /**< alternative implementation pointer - used for PPP */
    char _interface_name[NSAPI_INTERFACE_NAME_MAX_SIZE];
    char has_addr_state;
    nsapi_connection_status_t connected;
    bool dhcp_started;
    bool dhcp_has_to_be_set;
    bool blocking;
    bool ppp;
    mbed::Callback<void(nsapi_event_t, intptr_t)> client_callback;
    nsapi_ip_stack_t stack_type;
    mbed_rtos_storage_semaphore_t linked_sem;
    osSemaphoreId_t linked;
    mbed_rtos_storage_semaphore_t unlinked_sem;
    osSemaphoreId_t unlinked;
    mbed_rtos_storage_semaphore_t has_any_addr_sem;
    osSemaphoreId_t has_any_addr;
    NanostackMemoryManager &memory_manager;
    rtos::Semaphore connect_semaphore;
};


#ifdef __cplusplus
extern "C" {
#endif
    nsapi_error_t ppp_if_init(void *pcb, struct netif *netif,  nsapi_ip_stack_t stack);
    err_t ppp_if_connect(void *pcb);
    err_t ppp_if_disconnect(void *pcb);

#ifdef __cplusplus
}
#endif

#endif
