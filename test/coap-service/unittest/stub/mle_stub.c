/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
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
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "shalib.h"
#include "randLIB.h"
#ifdef NVM_BORDER_ROUTER
#include "br_list_nvm_api.h"
#endif
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread.h"
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

/* removes child id when becoming router*/
int thread_clear_child_id_by_address(protocol_interface_info_entry_t *cur, mle_neigh_table_entry_t *entry_temp);

void mle_entry_rm(mle_neigh_table_entry_t *entry);

#ifdef NVM_BORDER_ROUTER
uint16_t (*mle_nvm_storage_cb)(br_nvm_update_process_t) = 0;

int8_t mle_nvm_callback_set(uint16_t (*passed_fptr)(br_nvm_update_process_t), uint8_t *nvm_static_buffer)
{
}


int8_t mle_entry_store_from_nvm(const uint8_t *nvm_data)
{
    return -1;
}


void mle_nvm_update(mle_neigh_table_entry_t *entry, br_nvm_update_process_t event)
{
}

#endif

void mle_set_max_allow_join_time(uint16_t new_max_allow_join_time)
{
}

int8_t arm_mle_prepare_update(int8_t nwk_id, uint8_t type, uint32_t time, uint16_t value)
{
    return 0;
}

int8_t arm_mle_update_beacon_payload(int8_t nwk_id, uint8_t *optional_fields_data, uint8_t optional_fields_len, uint32_t delay)
{
    return 0;
}

uint8_t mle_update_process(uint8_t event)
{
    return 0;
}

void mle_init(void)
{
}

void mle_event_handler(arm_event_s *event)
{
}

void protocol_core_mle_init(void)
{
}

void mle_entry_frame_counter_update(mle_neigh_table_entry_t *entry_temp, uint32_t counter, uint8_t keyId)
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

void mle_entry_timeout_update(mle_neigh_table_entry_t *entry_temp, uint32_t timeout_tlv)
{
}

void mle_neigh_entry_update_by_mle_tlv_list(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, uint8_t lqi, uint8_t keyId)
{
}

uint8_t mle_indirect_check(const uint8_t *ptr, addrtype_t type)
{
    return 0;
}

static mle_neigh_table_entry_t *mle_entry_scan_long(const uint8_t *address)
{
    return NULL;
}

static mle_neigh_table_entry_t *mle_entry_scan_short(uint16_t address)
{
    return NULL;
}

void mle_key_synch_request(mle_neigh_table_entry_t *info)
{
}

mle_neigh_table_entry_t *mle_table_rpl_advert(void)
{
    return NULL;
}

void mle_entry_rm(mle_neigh_table_entry_t *entry)
{
}

void mle_table_ttl(uint16_t ticks, protocol_interface_info_entry_t *cur_interface)
{
}

mle_neigh_table_entry_t *mle_refresh_entry_timeout(int8_t interfaceId, const uint8_t *addressPtr, addrtype_t addressType, bool dataPollConfirmation)
{
    return NULL;
}

mle_neigh_table_entry_t *mle_entry_get_by_link_address(const uint8_t *address, addrtype_t type)
{
    mle_stub.expectedUint8ptr = address;
    mle_stub.expectedUint8 = type;

    return mle_stub.mle_neigh_table_entry_ptr;
}

mle_neigh_table_list_t *mle_list_get(void)
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

void mle_tables_frame_counter_state_reset_for_nw_key(uint8_t keyId)
{

}

#endif /* NO_MLE */
