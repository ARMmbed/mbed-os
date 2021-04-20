/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
#include "nsconfig.h"
#ifdef HAVE_THREAD
#include "ns_types.h"
#include <string.h>
#include <nsdynmemLIB.h>
#include "eventOS_event_timer.h"
#include <ns_list.h>
#include <net_thread_test.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "net_interface.h" // TODO remove when fixing properly
#include "thread_commissioning_if.h"
#include "thread_management_if.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"

#define TRACE_GROUP "thrC"

/* Local function declarations */

int thread_commissioning_if_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, uint8_t eui64[static 8], uint8_t key[static 16])
{
    int ret;
    if (!eui64 || !key) {
        return -1;
    }
// TODO this is now IID either change to full LL64 address or sumthing
    tr_debug("pairwise key:%s, interface:%d, iid: %s", trace_array(key, 16), interface_id, trace_array(eui64, 8));
    eui64[0] ^= 2;
    ret = mac_pairwise_key_add(interface_id, valid_life_time, eui64, key);
    eui64[0] ^= 2;
    return ret;
}

int thread_commissioning_if_pairwise_key_del(int8_t interface_id, uint8_t eui64[static 8])
{
    int ret;
    if (!eui64) {
        return -1;
    }
    tr_debug("pairwise key delete interface:%d, iid: %s", interface_id, trace_array(eui64, 8));
    eui64[0] ^= 2;
    ret = mac_pairwise_key_del(interface_id, eui64);
    eui64[0] ^= 2;
    return ret;
}

int thread_commissioning_if_pairwise_key_delete_all(int8_t interface_id)
{
    return mac_pairwise_key_flush_list(interface_id);
}

int thread_commissioning_if_border_router_locator_get(int8_t interface_id, uint8_t *address_ptr)
{
    protocol_interface_info_entry_t *cur;
    if (!address_ptr) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    if (!cur->thread_info->registered_commissioner.commissioner_valid) {
        return -3;
    }

    memcpy(address_ptr, cur->thread_info->registered_commissioner.border_router_address, 16);

    return 0;
}

int thread_commissioning_if_enable_security(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->mac_parameters) {
        return -1;
    }
    cur->mac_parameters->mac_configured_sec_level = SEC_ENC_MIC32;
    mac_helper_default_security_level_set(cur, cur->mac_parameters->mac_configured_sec_level);
    mac_helper_default_security_key_id_mode_set(cur, MAC_KEY_ID_MODE_IDX);
    return 0;
}

#endif


