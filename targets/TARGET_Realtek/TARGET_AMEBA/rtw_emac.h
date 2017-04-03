/* mbed Microcontroller Library
 * Copyright (c) 2016 Realtek Semiconductor Corp.
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
#ifndef MBED_WLAN_EMAC_H
#define MBED_WLAN_EMAC_H

#include "emac_api.h"

extern void wlan_emac_link_change(bool up);
extern emac_interface_t *wlan_emac_init_interface();
extern void wlan_emac_recv(struct netif *netif, uint32_t len);

#endif
