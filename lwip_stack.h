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
 */

#ifndef LWIP_STACK_H
#define LWIP_STACK_H

#include "nsapi.h"

#ifdef __cplusplus
extern "C" {
#endif


// Access to lwip through the nsapi
int lwip_bringup(void);
void lwip_bringdown(void);

extern nsapi_stack_t lwip_stack;

const char *lwip_get_mac_address(void);
const char *lwip_get_ip_address(void);


#ifdef __cplusplus
}
#endif

#endif
