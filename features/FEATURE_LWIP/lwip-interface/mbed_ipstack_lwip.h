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


struct mbed_ipstack_interface {

    /* Members implemented by vendor */
    const emac_interface_ops_t *ops; /**< HW specific emac implementation */
    void *hw; /**< EMAC implementation specific user data */

    /* Private members used by the stack */
    osSemaphoreId_t linked;
    osSemaphoreId_t has_addr;
    bool connected;
    bool dhcp;
    bool ppp;
    char hwaddr[6];
    struct netif netif;
};

#ifdef __cplusplus
}
#endif


#endif /* MBED_IPSTACK_LWIP_H_ */
