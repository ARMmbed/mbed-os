/*
 * Copyright (c) 2014-2015, Arm Limited and affiliates.
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

/**
* \file \test_libTrace\Test.c
*
* \brief Unit tests for libTrace
*/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "unity.h"
#include "ns_address.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"

#include "unity.h"
#include "unity_improved.h"

typedef enum {
    IF_6LoWPAN,
    IF_IPV6,
    IF_LOCAL_SOCKET,
} nwk_interface_id;

typedef struct protocol_interface_info_entry {
    nwk_interface_id nwk_id;
    int8_t id;
    int8_t net_start_tasklet;
    char *interface_name;
} protocol_interface_info_entry_t;

typedef enum if_address_source_e {
    ADDR_SOURCE_UNKNOWN,
    ADDR_SOURCE_SLAAC,
    ADDR_SOURCE_DHCP,
    ADDR_SOURCE_STATIC,
} if_address_source_t;

typedef enum if_address_callback_e {
    ADDR_CALLBACK_DAD_COMPLETE, // Duplicate Address Detection complete - address now valid
    ADDR_CALLBACK_DAD_FAILED,   // Duplicate Address Detection failed - address is a duplicate, about to be deleted
    ADDR_CALLBACK_TIMER,        // state_timer reached 0
    ADDR_CALLBACK_DEPRECATED,   // preferred lifetime reached 0
    ADDR_CALLBACK_INVALIDATED,  // valid lifetime reached 0 - about to be deleted
} if_address_callback_t;

struct if_address_entry;
typedef void if_address_callback_fn(struct protocol_interface_info_entry *interface, struct if_address_entry *addr, if_address_callback_t reason);

typedef struct if_address_entry {
    uint8_t address[16];        // IPv6 (or IPv4-mapped IPv6 in future)
    uint8_t prefix_len;         // length of prefix part
    uint32_t valid_lifetime;    // seconds remaining; 0xffffffff = infinite
    uint32_t preferred_lifetime; // seconds remaining; 0 if deprecated, 0xffffffff = infinite
    uint32_t state_timer;       // ticks to state change - used by DAD, then can be used by protocol
    uint8_t count;              // general count field - used by DAD, then can be used by protocol
    bool temporary;             // RFC 4941 temporary address
    bool tentative;             // Tentative address (Duplicate Address Detection running)
    if_address_source_t source; //
    if_address_callback_fn *cb; // Address protocol callback function
    void *data;                 // Address protocol data
    ns_list_link_t link;
} if_address_entry_t;
protocol_interface_info_entry_t piie;

typedef NS_LIST_HEAD(if_address_entry_t, link) if_address_list_t;

if_address_list_t addr_list;

int_fast8_t addr_delete(protocol_interface_info_entry_t *cur, const uint8_t address[static 16])
{
    uint8_t *address_ptr = malloc(16);
    if_address_entry_t *addr_ptr = NULL;
    TEST_METHOD_CALLED("addr_delete", address_ptr);
    memcpy(address_ptr, address, 16);
    print_buf(address, 16);
    ns_list_foreach(if_address_entry_t, cur_ptr, &addr_list) {
        if (memcmp(cur_ptr->address, address, 16) == 0) {
            addr_ptr = cur_ptr;
        }
    }
    if (addr_ptr != NULL) {
        printf("  Address found and deleted.\n");
        ns_list_remove(&addr_list, addr_ptr);
        free(addr_ptr);
    } else {
        printf("  Address NOT found.\n");
    }
    return 0;
}
void lt_test_addr_delete(const uint8_t address[static 16])
{
    uint8_t *address_ptr;
    address_ptr = TEST_METHOD_CHECK("addr_delete");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(address, address_ptr, 16, "Incorrect address deleted");
    free(address_ptr);
}

if_address_entry_t *addr_add(protocol_interface_info_entry_t *cur, const uint8_t address[static 16], uint_fast8_t prefix_len, if_address_source_t source, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad)
{
    if_address_entry_t *addr_ptr = malloc(sizeof(if_address_entry_t));

    memcpy(addr_ptr->address, address, 16);
    ns_list_add_to_start(&addr_list, addr_ptr);

    TEST_METHOD_CALLED("addr_add", addr_ptr);
    printf("  addr = ");
    print_buf(addr_ptr->address, 16);
    return addr_ptr;
}
void lt_test_addr_add(const uint8_t address[static 16])
{
    if_address_entry_t *addr_ptr;
    addr_ptr = TEST_METHOD_CHECK("addr_add");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(address, addr_ptr->address, 16, "Incorrect address");
}

if_address_entry_t *addr_get_entry(const protocol_interface_info_entry_t *interface, const uint8_t addr[static 16])
{
    if_address_entry_t *addr_ptr = NULL;

    printf("SIM Address addr_get_entry()\n  addr = ");
    print_buf(addr, 16);
    ns_list_foreach(if_address_entry_t, cur_ptr, &addr_list) {
        if (memcmp(cur_ptr->address, addr, 16) == 0) {
            addr_ptr = cur_ptr;
        }
    }
    if (addr_ptr != NULL) {
        printf("  Address found.\n");
    } else {
        printf("  Address NOT found.\n");
    }
    return addr_ptr;
}

void lt_timeout_address(int8_t interface_id, const uint8_t address[static 16])
{
    if_address_entry_t *addr_ptr;
    ns_list_foreach(if_address_entry_t, cur_ptr, &addr_list) {
        if (memcmp(cur_ptr->address, address, 16) == 0) {
            addr_ptr = cur_ptr;
        }
    }
    printf("lt_timeout_address()\n");
    if (addr_ptr != NULL) {
        printf("  Address found calling_timeout.\n");
        piie.id = interface_id;
        addr_ptr->cb(&piie, addr_ptr, ADDR_CALLBACK_TIMER);
    } else {
        printf("  Address NOT found.\n");
    }
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_id(int8_t nwk_id)
{
    printf("SIM Protocol protocol_stack_interface_info_get_by_id()\n");
    return &piie;
}

