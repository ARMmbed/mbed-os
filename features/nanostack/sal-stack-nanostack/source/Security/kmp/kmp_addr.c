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

#define KMP_ADDR_DYN_ALLOC        0x80
#define KMP_ADDR_TYPE_MASK        0x0F

typedef struct {
    uint8_t type;
    uint8_t eui_64[8];
    address_t ip_addr;
    uint16_t port;
} kmp_eui_64_ip_addr_t;

kmp_addr_t *kmp_address_create(kmp_addr_e type, const uint8_t *eui_64)
{
    uint8_t size;
    if (type == KMP_ADDR_EUI_64) {
        size = sizeof(kmp_addr_t);
    } else if (type == KMP_ADDR_EUI_64_AND_IP) {
        size = sizeof(kmp_eui_64_ip_addr_t);
    } else {
        return 0;
    }

    kmp_addr_t *addr = ns_dyn_mem_alloc(size);
    if (!addr) {
        return 0;
    }

    kmp_address_init(type, addr, eui_64);

    addr->type |= KMP_ADDR_DYN_ALLOC;

    return addr;
}

void kmp_address_init(kmp_addr_e type, kmp_addr_t *addr, const uint8_t *eui_64)
{
    uint8_t size;
    if (type == KMP_ADDR_EUI_64) {
        size = sizeof(kmp_addr_t);
    } else if (type == KMP_ADDR_EUI_64_AND_IP) {
        size = sizeof(kmp_eui_64_ip_addr_t);
    } else {
        return;
    }

    kmp_addr_t *kmp_addr = addr;

    memset(addr, 0, size);
    kmp_addr->type = type;
    if (eui_64) {
        memcpy(kmp_addr->eui_64, eui_64, 8);
    }
}

void kmp_address_delete(kmp_addr_t *addr)
{
    if (addr && (addr->type & KMP_ADDR_DYN_ALLOC)) {
        ns_dyn_mem_free(addr);
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
    if (!addr || (addr->type & KMP_ADDR_TYPE_MASK) != KMP_ADDR_EUI_64_AND_IP) {
        return NULL;
    }

    return ((kmp_eui_64_ip_addr_t *)addr)->ip_addr;
}

uint16_t kmp_address_port_get(const kmp_addr_t *addr)
{
    if (!addr || (addr->type & KMP_ADDR_TYPE_MASK) != KMP_ADDR_EUI_64_AND_IP) {
        return 0;
    }

    return ((kmp_eui_64_ip_addr_t *)addr)->port;
}

int8_t kmp_address_eui_64_set(kmp_addr_t *addr, const uint8_t *eui64)
{
    if (!addr) {
        return -1;
    }

    memcpy(addr->eui_64, eui64, 8);
    return 0;
}

int8_t kmp_address_ip_set(kmp_addr_t *addr, const uint8_t *ip_addr)
{
    if (!addr || !ip_addr || (addr->type & KMP_ADDR_TYPE_MASK) != KMP_ADDR_EUI_64_AND_IP) {
        return -1;
    }

    memcpy(((kmp_eui_64_ip_addr_t *)addr)->ip_addr, ip_addr, sizeof(address_t));
    return 0;
}

int8_t kmp_address_port_set(kmp_addr_t *addr, const uint16_t port)
{
    if (!addr || (addr->type & KMP_ADDR_TYPE_MASK) != KMP_ADDR_EUI_64_AND_IP) {
        return -1;
    }

    ((kmp_eui_64_ip_addr_t *)addr)->port = port;
    return 0;
}

int8_t kmp_address_copy(kmp_addr_t *to_addr, const kmp_addr_t *from_addr)
{
    if (!to_addr || !from_addr) {
        return -1;
    }

    memcpy(to_addr->eui_64, from_addr->eui_64, 8);

    kmp_eui_64_ip_addr_t *to_ip_addr = (kmp_eui_64_ip_addr_t *) to_addr;
    kmp_eui_64_ip_addr_t *from_ip_addr = (kmp_eui_64_ip_addr_t *) from_addr;

    if ((to_ip_addr->type & KMP_ADDR_TYPE_MASK) == KMP_ADDR_EUI_64_AND_IP
            && (from_ip_addr->type & KMP_ADDR_TYPE_MASK) == KMP_ADDR_EUI_64_AND_IP) {
        memcpy(to_ip_addr->ip_addr, from_ip_addr->ip_addr, sizeof(address_t));
        to_ip_addr->port = from_ip_addr->port;
    } else if ((to_ip_addr->type & KMP_ADDR_TYPE_MASK) == KMP_ADDR_EUI_64_AND_IP) {
        memset(to_ip_addr->ip_addr, 0, sizeof(address_t));
        to_ip_addr->port = 0;
    }

    return 0;
}

#endif /* HAVE_WS */

