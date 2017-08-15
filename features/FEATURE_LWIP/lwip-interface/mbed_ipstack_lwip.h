/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef MBED_IPSTACK_LWIP_H_
#define MBED_IPSTACK_LWIP_H_

#ifdef __cplusplus
extern "C" {
#endif

struct emac_interface_ops;

struct mbed_ipstack_interface {

    /* Members implemented by vendor */
    const struct emac_interface_ops *ops; /**< HW specific emac implementation */
    void *hw; /**< EMAC implementation specific user data */

    /* Private members used by the stack */
    os_semaphore_t linked_sem;
    osSemaphoreId_t linked;
    os_semaphore_t unlinked_sem;
    osSemaphoreId_t unlinked;
    os_semaphore_t has_any_addr_sem;
    osSemaphoreId_t has_any_addr;
#define HAS_ANY_ADDR 1
#if PREF_ADDR_TIMEOUT
    os_semaphore_t has_pref_addr_sem;
    osSemaphoreId_t has_pref_addr;
#define HAS_PREF_ADDR 2
#endif
#if BOTH_ADDR_TIMEOUT
    os_semaphore_t has_both_addr_sem;
    osSemaphoreId_t has_both_addr;
#define HAS_BOTH_ADDR 4
#endif
    char has_addr_state;
    bool connected;
    bool dhcp;
    bool ppp;
    struct netif netif;
};

/** Register a PPP interface with the IP stack
 *
 * Connects PPP layer with the IP stack and initializes all the required infrastructure.
 * This function should be called only once for each available interface.
 *
 * This is an internal function that links ppp_lwip.cpp to mbed_ipstack_lwip.cpp,
 * once a driver starts it via the nsapi_ppp.h API.
 *
 * Ultimately the nsapi_ppp.h API will be deprecated, and there will be a
 * mbed_ipstack_add_ppp_interface() replacing nsapi_ppp_connect().
 *
 * @param      pcb              PPP implementation specific user data; will be passed to PPP callbacks
 * @param      default_if       true if the interface should be treated as the default one
 * @param[out] interface_out    set to interface handle that must be passed to subsequent mbed_stack calls
 * @return                      NSAPI_ERROR_OK on success, or error code
 */
nsapi_error_t mbed_ipstack_lwip_add_ppp_interface(void *pcb, bool default_if, struct mbed_ipstack_interface **interface_out);


#ifdef __cplusplus
}
#endif


#endif /* MBED_IPSTACK_LWIP_H_ */
