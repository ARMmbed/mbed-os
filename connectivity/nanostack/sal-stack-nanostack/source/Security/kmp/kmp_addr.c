/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/kmp/kmp_addr.h"

#ifdef HAVE_WS

#define TRACE_GROUP "kmar"

void kmp_address_init(kmp_addr_e type, kmp_addr_t *addr, const uint8_t *eui_64)
{
    memset(addr, 0, sizeof(kmp_addr_t));
    addr->type = type;
    if (eui_64) {
        memcpy(addr->eui_64, eui_64, 8);
    }
}


const uint8_t *kmp_address_eui_64_get(const kmp_addr_t *addr)
{
    if (!addr) {
        return NULL;
    }

    return addr->eui_64;
}

const uint8_t *kmp_address_ip_get(const kmp_addr_t *addr)
{
    if (!addr || addr->type != KMP_ADDR_EUI_64_AND_IP) {
        return NULL;
    }

    return addr->relay_address;
}

int8_t kmp_address_eui_64_set(kmp_addr_t *addr, const uint8_t *eui64)
{
    if (!addr) {
        return -1;
    }

    memcpy(addr->eui_64, eui64, 8);
    return 0;
}

int8_t kmp_address_copy(kmp_addr_t *to_addr, const kmp_addr_t *from_addr)
{
    if (!to_addr || !from_addr) {
        return -1;
    }

    memcpy(to_addr->eui_64, from_addr->eui_64, 8);

    if (to_addr->type == KMP_ADDR_EUI_64_AND_IP
            && from_addr->type == KMP_ADDR_EUI_64_AND_IP) {
        memcpy(to_addr->relay_address, from_addr->relay_address, sizeof(address_t));
        to_addr->port = from_addr->port;
    } else if (to_addr->type == KMP_ADDR_EUI_64_AND_IP) {
        memset(to_addr->relay_address, 0, sizeof(address_t));
        to_addr->port = 0;
    }

    return 0;
}

#endif /* HAVE_WS */

