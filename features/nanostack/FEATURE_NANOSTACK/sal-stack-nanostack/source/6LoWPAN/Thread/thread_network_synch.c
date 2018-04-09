/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * \file thread_network_synch.c
 *
 */
#include "nsconfig.h"
#ifdef HAVE_THREAD
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "thread_management_if.h"
#include "thread_config.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "tsyn"

typedef struct thread_sync_child_info {
    uint32_t mle_frame_counter;
    uint32_t mac_frame_counter;
    uint8_t long_addr[8];
    uint16_t short_addr;
    uint8_t mode;
} thread_sync_child_info_t;

typedef struct thread_network_dynamic_data_store {
    thread_leader_info_t *leader_private_data;
    thread_parent_info_t *thread_endnode_parent;
    thread_attach_state_e thread_attached_state;
    uint16_t shortAddress;
    thread_sync_child_info_t children[THREAD_MAX_CHILD_COUNT];
} thread_network_dynamic_data_store_t;


typedef struct thread_network_dynamic_data_entry {
    int8_t interfaceId;
    ns_list_link_t link; /*!< List link entry */
    thread_network_dynamic_data_store_t networ_dynamic_data_parameters;
} thread_network_dynamic_data_entry_t;

static thread_sync_child_info_t *thread_dynamic_storage_free_child_find(int8_t interface_id);
static thread_sync_child_info_t *thread_dynamic_storage_child_info_find(int8_t interface_id, mle_neigh_table_entry_t *child);
static NS_LIST_DEFINE(thread_network_dynamic_data_info, thread_network_dynamic_data_entry_t, link);

thread_network_dynamic_data_entry_t *thread_network_synch_create(int8_t interfaceId)
{
    thread_network_dynamic_data_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(thread_network_dynamic_data_entry_t));
    if (newEntry) {
        tr_debug("Allocate New Store");
        newEntry->interfaceId = interfaceId;
        newEntry->networ_dynamic_data_parameters.thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
        newEntry->networ_dynamic_data_parameters.shortAddress = 0xfffe;
        newEntry->networ_dynamic_data_parameters.leader_private_data = NULL;
        newEntry->networ_dynamic_data_parameters.thread_endnode_parent = NULL;
        for (int i = 0; i < THREAD_MAX_CHILD_COUNT; ++i) {
            memset(&newEntry->networ_dynamic_data_parameters.children[i], 0, sizeof(thread_sync_child_info_t));
        }
        ns_list_add_to_end(&thread_network_dynamic_data_info, newEntry);
    }
    return newEntry;
}

thread_network_dynamic_data_entry_t *thread_network_synch_find(int8_t interfaceId)
{
    //Check current  List
    ns_list_foreach(thread_network_dynamic_data_entry_t, cur, &thread_network_dynamic_data_info) {
        if (interfaceId == cur->interfaceId) {
            return cur;
        }
    }
    return NULL;
}

int thread_network_synch_delete(int8_t interfaceId)
{
    thread_network_dynamic_data_entry_t *newEntry = thread_network_synch_find(interfaceId);
    if (newEntry) {
        //Free
        if (newEntry->networ_dynamic_data_parameters.leader_private_data) {
            ns_dyn_mem_free(newEntry->networ_dynamic_data_parameters.leader_private_data);
        }

        if (newEntry->networ_dynamic_data_parameters.thread_endnode_parent) {
            ns_dyn_mem_free(newEntry->networ_dynamic_data_parameters.thread_endnode_parent);
        }

        newEntry->networ_dynamic_data_parameters.thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
        newEntry->networ_dynamic_data_parameters.shortAddress = 0xfffe;
        newEntry->networ_dynamic_data_parameters.leader_private_data = NULL;
        newEntry->networ_dynamic_data_parameters.thread_endnode_parent = NULL;

        return 0;
    }
    return -1;
}

//Call This when clean synched networkdata
int thread_network_synch_data_free(int8_t interface_id)
{
    return thread_network_synch_delete(interface_id);
}

/*
 * Dynamic network data storage.
 */

void thread_dynamic_storage_child_info_store(int8_t interface_id, mle_neigh_table_entry_t *child)
{
    thread_network_dynamic_data_entry_t *storeEntry = thread_network_synch_find(interface_id);

    if (!storeEntry) {
        storeEntry = thread_network_synch_create(interface_id);
    }

    if (!storeEntry) {
        return;
    }

    thread_sync_child_info_t *child_info = thread_dynamic_storage_child_info_find(interface_id, child);
    if (child_info) {
        child_info->mode = child->mode;
        child_info->short_addr = child->short_adr;
        child_info->mle_frame_counter = child->mle_frame_counter;
        child_info->mac_frame_counter = 0;
        memcpy(child_info->long_addr, child->mac64, 8);
        return;
    }

    child_info = thread_dynamic_storage_free_child_find(interface_id);

    if (child_info) {
        child_info->mode = child->mode;
        child_info->short_addr = child->short_adr;
        child_info->mle_frame_counter = child->mle_frame_counter;
        child_info->mac_frame_counter = 0;
        memcpy(child_info->long_addr, child->mac64, 8);
    }
    return;
}

void thread_dynamic_storage_child_info_clear(int8_t interface_id, mle_neigh_table_entry_t *child)
{
    thread_sync_child_info_t *child_info = thread_dynamic_storage_child_info_find(interface_id, child);

    if (child_info){
        // Clear child information
        memset (child_info,0,sizeof(thread_sync_child_info_t));
        tr_debug("Dynamic storage: cleared child; mac16=%04x", child->short_adr);
        return;
    }
    return;
}

static thread_sync_child_info_t *thread_dynamic_storage_child_info_find(int8_t interface_id, mle_neigh_table_entry_t *child)
{
    thread_network_dynamic_data_entry_t *storeEntry = thread_network_synch_find(interface_id);

    if (!storeEntry) {
        return NULL;
    }

    for (uint16_t i = 0; i < THREAD_MAX_CHILD_COUNT; ++i) {
        uint8_t *mac64 = storeEntry->networ_dynamic_data_parameters.children[i].long_addr;
        if (memcmp(mac64, child->mac64, 8) == 0) {
            return &storeEntry->networ_dynamic_data_parameters.children[i];
        }
    }
    return NULL;
}

static thread_sync_child_info_t *thread_dynamic_storage_free_child_find(int8_t interface_id)
{
    thread_network_dynamic_data_entry_t *storeEntry = thread_network_synch_find(interface_id);
    if (!storeEntry) {
        return NULL;
    }
    for (int i = 0; i < THREAD_MAX_CHILD_COUNT; ++i) {
        if (storeEntry->networ_dynamic_data_parameters.children[i].short_addr == 0) {
            return &storeEntry->networ_dynamic_data_parameters.children[i];
        }
    }
    return NULL;
}

void thread_dynamic_storage_build_mle_table(int8_t interface_id)
{
    tr_debug("Dynamic storage: building MLE table.");

    thread_network_dynamic_data_entry_t *storeEntry = thread_network_synch_find(interface_id);

    if (!storeEntry) {
        storeEntry = thread_network_synch_create(interface_id);
    }

    if (!storeEntry) {
        return;
    }

    mle_neigh_table_list_t *neig_list = mle_class_active_list_get(interface_id);

    if (!neig_list) {
        return;
    }

    int i;
    for (i = 0; i < THREAD_MAX_CHILD_COUNT; ++i) {
        if (storeEntry->networ_dynamic_data_parameters.children[i].short_addr == 0) {
            // Skip empty child entry
            continue;
        }
        uint8_t *mac64 = storeEntry->networ_dynamic_data_parameters.children[i].long_addr;
        tr_debug("Child: %04x, %s", storeEntry->networ_dynamic_data_parameters.children[i].short_addr, trace_array(mac64, 8));
        mle_neigh_table_entry_t *entry = mle_class_get_entry_by_mac64(interface_id, 64, mac64, true);
        if (entry) {
            entry->short_adr = storeEntry->networ_dynamic_data_parameters.children[i].short_addr;
            entry->mle_frame_counter = storeEntry->networ_dynamic_data_parameters.children[i].mle_frame_counter;
            entry->mode = storeEntry->networ_dynamic_data_parameters.children[i].mode;
            entry->threadNeighbor = true;

            protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

            if (cur && cur->mac_parameters) {
                // Set MAC layer frame counter for the child
                mac_helper_devicetable_set(entry, cur, storeEntry->networ_dynamic_data_parameters.children[i].mac_frame_counter, cur->mac_parameters->mac_default_key_index);
            }
        }
    }
}
#endif
