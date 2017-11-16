/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include "MLE/mle.h"

#ifndef NO_MLE

#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "socket_api.h"
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "shalib.h"
#include "randLIB.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include <net_thread_test.h>
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/topo_trace.h"
#include "common_functions.h"
#include "MLE/mle_tlv.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6.h"

#include "mle_stub.h"

#define MLE_UNICAST_CHALLENGE_TIMEOUT 20

#define TRACE_GROUP "mle"

mle_stub_def mle_stub;

typedef enum {
    ARM_MLE_INIT = 0,
    ARM_MLE_TTL_TIMER
} arm_mle_event_id_e;


/**
 * Generic Type/Length beader, no value
 */
typedef struct {
    uint8_t type;
    uint8_t length;
} mle_tlv_tl;

/**
 * Source address structure
 */
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t src_address[2];
} mle_tlv_src_address_s;

/**
 * Mode structure
 */
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t mode;
} mle_tlv_mode_s;

/**
 * Timeout structure
 */
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t timeout;
} mle_tlv_timeout_s;

/**
 * Link layer frame counter structure
 */
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t ll_frm_counter[4];
} mle_tlv_ll_frame_counter_s;

/**
 * MLE frame counter structure
 */
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t mle_frm_counter[4];
} mle_tlv_mle_frame_counter_s;

typedef union {
    uint8_t *u8ptr;
    mle_tlv_tl                  *tl;
    mle_tlv_src_address_s       *src_address;
    mle_tlv_mode_s              *mode;
    mle_tlv_timeout_s           *timeout;
    mle_tlv_ll_frame_counter_s  *ll_frm_counter;
    mle_tlv_mle_frame_counter_s *mle_frm_counter;
} tlv_value_ptr_u;

void mle_table_ttl(uint16_t ticks, protocol_interface_info_entry_t *cur_interface);

NS_LARGE uint8_t mle_update_timer = 0;

typedef struct {
    uint8_t  queu_update_event;
    uint8_t  active_params;
    uint16_t pan_id;     /*!< PAN-ID */
    uint32_t pan_time;
    uint8_t  channel;       /*!< Channel */
    uint32_t chan_time;
    uint8_t  allow_join;    /*!< Permit Join */
    uint32_t allow_time;
    uint8_t *beacon_payload;
    uint8_t  beacon_payload_len;
    uint32_t beacon_payload_delay;
} mle_nwk_param_t;

void mle_set_max_allow_join_time(uint16_t new_max_allow_join_time)
{
}

void mle_event_handler(arm_event_s *event)
{
}

uint8_t mle_mode_get_by_interface_ptr(protocol_interface_info_entry_t *cur)
{
    return 0;
}

uint8_t mle_challenge(uint8_t *addr, protocol_interface_info_entry_t *cur)
{
    return 1;
}

int mle_child_update_generation(uint8_t *parentAddress, struct protocol_interface_info_entry *cur, uint8_t mode)
{
    return 0;
}

int8_t mle_host_link_configuration(bool rx_on_idle, protocol_interface_info_entry_t *cur)
{
    return -1;
}

uint8_t mle_router_synch(protocol_interface_info_entry_t *cur, const uint8_t *destAddress)
{
    return 1;
}

uint8_t mle_neigh_advertise(protocol_interface_info_entry_t *cur)
{
    return 1;
}

uint8_t mle_reed_advertise (protocol_interface_info_entry_t *cur)
{
    return 0;
}

int mle_attach_parent_request_build(protocol_interface_info_entry_t *cur, uint8_t scanMask)
{
    return -1;
}


uint8_t *mle_linkquality_write(uint8_t *buffer)
{
    return NULL;
}

int mle_attach_parent_response_build(protocol_interface_info_entry_t *cur, sockaddr_t *dstAddress, uint8_t *challenge, uint8_t chalLen, uint8_t linkMargin)
{
    return -1;
}
void mle_network_data_push_to_sleep_child(protocol_interface_info_entry_t *cur, bool stableDataUpdate)
{
}

void mle_multicast_push_to_sleep_child(protocol_interface_info_entry_t *cur, buffer_t *buf)
{

}

int mle_network_data_propagation(protocol_interface_info_entry_t *cur, uint8_t *childUnicastAddress, bool fullList)
{
}

uint8_t *mle_general_write_link_layer_framecounter(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

bool mle_neigh_entry_frame_counter_update(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, protocol_interface_info_entry_t *cur, uint8_t key_id)
{
    return false;
}

void mle_entry_timeout_update(mle_neigh_table_entry_t *entry_temp, uint32_t timeout_tlv)
{
}

void mle_entry_timeout_refresh(mle_neigh_table_entry_t *entry_temp)
{

}

void mle_neigh_entry_update_by_mle_tlv_list(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, uint8_t lqi)
{
}

static mle_neigh_table_entry_t *mle_entry_scan_long(const uint8_t *address)
{
    return NULL;
}

mle_neigh_table_entry_t *mle_table_rpl_advert(void)
{
    return NULL;
}

void mle_table_ttl(uint16_t ticks, protocol_interface_info_entry_t *cur_interface)
{
}

mle_neigh_table_entry_t *mle_refresh_entry_timeout(int8_t interfaceId, const uint8_t *addressPtr, addrtype_t addressType, bool dataPollConfirmation)
{
    return NULL;
}

mle_neigh_table_entry_t *mle_class_get_by_link_address(int8_t interface_id, const uint8_t *address, addrtype_t type)
{
    mle_stub.expectedUint8ptr = address;
    mle_stub.expectedUint8 = type;

    return mle_stub.mle_neigh_table_entry_ptr;
}

mle_neigh_table_list_t *mle_class_active_list_get(int8_t interface_id)
{
    return NULL;
}

uint8_t mle_thread_end_device_synch_to_router(protocol_interface_info_entry_t *cur, uint8_t *router_ll64)
{
    return 0;
}

uint8_t mle_thread_end_device_synch(protocol_interface_info_entry_t *cur)
{
    return 0;
}

uint8_t *mle_general_write_source_address(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    return 0;
}

mle_neigh_table_entry_t *mle_class_get_entry_by_mac64(int8_t interface_id, uint8_t linkMargin, const uint8_t *mac64, bool allocateNew)
{
    return mle_stub.mle_neigh_table_entry_ptr;
}

mle_neigh_table_entry_t *mle_class_get_by_device_attribute_id(int8_t interface_id, uint8_t device_attibute)
{
    return mle_stub.mle_neigh_table_entry_ptr;
}

uint16_t mle_class_active_neigh_counter(int8_t interface_id)
{
    return 0;
}

int8_t mle_class_mode_set(int8_t interface_id,mle_class_user_mode mode)
{
    return 0;
}

int16_t mle_class_free_entry_count_get(int8_t interface_id)
{
    return 0;
}
mle_neigh_table_entry_t *mle_class_get_entry_by_ll64(int8_t interface_id, uint8_t linkMargin, const uint8_t *ipv6Address, bool allocateNew)
{
    return mle_stub.mle_neigh_table_entry_ptr;
}

int8_t mle_class_remove_entry(int8_t interface_id, mle_neigh_table_entry_t *entry)
{
    return 0;
}

int8_t mle_class_remove_neighbour(int8_t interface_id, const uint8_t *address, addrtype_t type)
{
    return 0;
}

int8_t mle_class_deallocate(int8_t interface_id)
{
    return 0;
}

int8_t mle_class_list_clean(int8_t interface_id)
{
    return 0;
}

int8_t mle_class_init(int8_t interface_id, uint8_t table_size, mle_entry_user_entry_remove_notify *remove_cb, mle_entry_link_keep_alive *keep_alive_cb, mle_entry_interface_activate *interface_is_active)
{
    return 0;
}

int8_t mle_class_router_challenge(int8_t interface_id,mle_entry_link_keep_alive *challenge_cb)
{
    return 0;
}

bool mle_class_exists_for_interface(int8_t interface_id)
{
    return mle_stub.bool_value;
}

mle_neigh_table_entry_t *mle_class_discover_entry_by_ll64(int8_t interface_id, const uint8_t *ipv6Address)
{
    return NULL;
}

int16_t mle_class_sleepy_entry_count_get(int8_t interface_id)
{
    return 0;
}

int16_t mle_class_rfd_entry_count_get(int8_t interface_id)
{
    return 0;
}

#endif /* NO_MLE */
