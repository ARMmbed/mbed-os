/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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
 *
 * Additional contributors:
 * - Simon Pfeifhofer (simon.pfeifhofer@gmail.com)
 */

#ifndef LWIP_QUECTELM66_STACK_H
#define LWIP_QUECTELM66_STACK_H

#include "nsapi.h"
#include "emac_api.h"
#include "pppos_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

	#include "netif/ppp/pppapi.h"
	#include "netif/ppp/pppos.h"
	
	// Access to lwip through the nsapi
	nsapi_error_t mbed_lwip_quectelm66_init(emac_interface_t *emac, const char* username, const char* password, pppos_context_t* ctx_cb);
	nsapi_error_t mbed_lwip_quectelm66_bringup(const char *ip, const char *netmask, const char *gw, const char* username, const char* password, pppos_context_t* ctx_cb);
	nsapi_error_t mbed_lwip_quectelm66_bringdown(void);

	char *mbed_lwip_quectelm66_get_ip_address(char *buf, int buflen);
	char *mbed_lwip_quectelm66_get_netmask(char *buf, int buflen);
	char *mbed_lwip_quectelm66_get_gateway(char *buf, int buflen);

	ppp_pcb *mbed_get_ppp_pcb(void);
	
	extern nsapi_stack_t lwip_quectelm66_stack;


#ifdef __cplusplus
}
#endif

#endif
