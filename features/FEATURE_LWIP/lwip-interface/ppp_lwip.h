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
#ifndef PPP_LWIP_H_
#define PPP_LWIP_H_
#include "netif/ppp/pppapi.h"
#ifdef __cplusplus
extern "C" {
#endif
#if LWIP_PPP_API

/** Initializes LWIP PPP interface
 *
 *  Starts a private thread for LWIP PPP and turns the PPP interface
 *  up.
 *
 *  @param netif  LWIP netif struct
 *
 *  @return 0 for success and negative error codes for failure
 */
nsapi_error_t ppp_lwip_if_init(void *pcb, struct netif *netif,  nsapi_ip_stack_t stack);

/** Connects to a PPP pipe
 *
 *  Called during LWIP interface bringup
 *
 *  @return 0 for success and negative error codes for failure
 */
err_t ppp_lwip_connect(void *pcb);

/** Disconnects from a PPP pipe
 *
 *  Can be called from multiple places. If LWIP bringup fails after initializing
 *  PPP interface, this API kicks in. Formal disconnection occurs when LWIP
 *  interface is brought down.
 *
 *  @return 0 for success and negative error codes for failure
 */
err_t ppp_lwip_disconnect(void *pcb);
#else
/**
 * Stubs in case LWIP PPP is not enabled
 */
#define ppp_lwip_if_init(pcb, netif, stack) NSAPI_ERROR_UNSUPPORTED
#define ppp_lwip_connect(pcb)               ERR_IF
#define ppp_lwip_disconnect(pcb)            ERR_IF
#endif //LWIP_PPP_API
#ifdef __cplusplus
}
#endif

#endif /* PPP_LWIP_H_ */
