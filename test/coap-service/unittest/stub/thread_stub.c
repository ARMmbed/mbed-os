/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "randLIB.h"
#include "common_functions.h"

#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"

#define TRACE_GROUP "thrd"

#define ID_MASK_UPDATE_MIN (10 * 10) /* 1 minute */
#define LEADER_DATA_UPDATE_MIN (10 * 10) /* 10 seconds */

void thread_leader_private_data_clean(thread_leader_info_t *private);
thread_leader_data_t *thread_leader_data_generate(void);
thread_leader_info_t *thread_allocate_and_init_leader_private_data(void);
thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info);
int thread_leader_router_id_deallocate(const uint8_t *eui64, protocol_interface_info_entry_t *cur);

int thread_proxy_state_update(int8_t caller_interface_id , int8_t handler_interface_id, bool status)
{

}

void thread_anycast_address_policy_update(const uint8_t *meshLocalPrefix, bool addPolicy)
{

}

int8_t thread_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int8_t thread_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int thread_leader_data_base_allocate(protocol_interface_info_entry_t *cur)
{
    return 0;
}

bool thread_addr_is_mesh_local(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return false;
}

bool thread_addr_is_mesh_local_16(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return false;
}

bool thread_addr_is_mesh_local_64(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return false;
}

void thread_leader_data_initialize(thread_leader_data_t *leader_data, uint8_t routerId)
{
}

void thread_leader_private_routemask_init(thread_leader_info_t *leader_info)
{
}

void thread_leader_private_routemask_init_by_last_partition(thread_leader_info_t *leader_info, uint8_t *lastValidRouteMask, uint8_t leaderId)
{
}

void thread_router_child_info_init(thread_child_info_t *child_info)
{
}

bool thread_leader_data_parse(uint8_t *ptr, uint16_t dataLength, thread_leader_data_t *leaderDataBuf)
{
    return false;
}

bool thread_connectivity_tlv_parse(uint8_t *ptr, uint16_t dataLength, thread_connectivity_t *connectivityTlv)
{
    return false;
}



thread_leader_data_t *thread_leader_data_generate(void)
{
    return NULL;
}

thread_leader_data_t *thread_leader_data_allocate(thread_info_t *info)
{
    return NULL;
}

thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info)
{
    return NULL;
}

int thread_leader_ul16_address_get(thread_info_t *thread_info, uint8_t *address_ptr)
{
    return -1;
}

void thread_leader_setup_delete(link_configuration_s *leader_setup_ptr)
{
}

bool thread_leader_commissioner_create(thread_info_t *thread_info)
{
    return false;
}

void thread_leader_commissioner_free(thread_info_t *thread_info)
{
}


void thread_data_base_init(thread_info_t *thread_info, int8_t interfaceId)
{
}


int thread_info_allocate_and_init(protocol_interface_info_entry_t *cur)
{
    return -1;
}

void thread_child_info_free(thread_info_t *thread_info)
{
}

void thread_info_deallocate(protocol_interface_info_entry_t *cur)
{
}

int thread_leader_route_mask_update(thread_leader_info_t *info, uint8_t router_id, bool setActive)
{
    return -1;
}

/* Called by thread_routing.c as a result of updates to routing table - allows
 * leader to monitor a router being available (having a finite route cost).
 */
void thread_leader_router_state_changed(thread_leader_info_t *info, uint8_t router_id, bool available, int8_t interface_id)
{
}

int thread_validate_own_routeid_from_new_mask(const uint8_t *master_router_id_mask, uint8_t router_id)
{
    return -1;
}




void thread_leader_allocate_router_id_by_allocated_id(thread_leader_info_t *info, uint8_t router_id, const uint8_t *eui64)
{
}

int thread_generate_leasequery(int8_t interfaceID, uint8_t *addressRequested)
{
    return -1;
}

thread_leader_info_t *thread_allocate_and_init_leader_private_data(void)
{
    return NULL;
}

thread_child_info_t *thread_router_child_info_allocate_and_init(void)
{
    return NULL;
}



void thread_child_id_request_info_init(thread_pending_child_id_req_t *child_info)
{
}

thread_pending_child_id_req_t *thread_child_id_request_allocate(void)
{
    return NULL;
}

void thread_child_id_request_entry_clean(protocol_interface_info_entry_t *cur)
{
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get(protocol_interface_info_entry_t *cur, uint8_t *euid64)
{
    return NULL;
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get_from_the_list(protocol_interface_info_entry_t *cur)
{
    return NULL;
}

void thread_child_id_request_entry_remove(protocol_interface_info_entry_t *cur, thread_pending_child_id_req_t *entry)
{
}

int thread_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int thread_route_tlv_push(protocol_interface_info_entry_t *cur, uint8_t *route_tlv, uint8_t route_len, uint8_t linkMargin, uint16_t shortAddress)
{
    return 0;
}

void thread_leader_update_id_set(protocol_interface_info_entry_t *cur)
{
}

int thread_routerid_request_handler(int8_t interfaceId, const uint8_t *clientId, thread_router_id_response_t *router_id_response, bool reNew)
{
    return -1;
}


int thread_routerid_release_handler(int8_t interfaceId, uint8_t *clientId)
{
    return -1;
}

void thread_leader_router_id_valid_lifetime_update(protocol_interface_info_entry_t *cur, uint32_t tickUpdate)
{
}

int thread_leader_router_id_deallocate(const uint8_t *eui64, protocol_interface_info_entry_t *cur)
{
    return -1;
}

int thread_leader_router_id_allocate(const uint8_t *eui64, protocol_interface_info_entry_t *cur, thread_router_id_response_t *reponse, bool renew)
{
    return -1;
}

int thread_discover_mode_active(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int thread_attach_mode_active(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int thread_attach_ready(protocol_interface_info_entry_t *cur)
{
    return -1;
}

bool thread_attach_active_router(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int thread_router_ready(protocol_interface_info_entry_t *cur)
{
    return -1;
}

bool thread_scan_mask_validation(protocol_interface_info_entry_t *cur, uint8_t mask)
{
    return false;
}

int thread_route_ready_to_leader(protocol_interface_info_entry_t *cur)
{
    return -1;
}

void thread_leader_private_data_clean(thread_leader_info_t *pr)
{
}

bool thread_route_possible_add(thread_attach_device_mode_e threadMode)
{
    return false;
}

bool thread_on_mesh_route_possible_add(thread_attach_device_mode_e threadMode)
{
    return false;
}

bool thread_on_mesh_address_valid(thread_network_server_data_entry_t *curRoute)
{
    return false;
}

void thread_dhcp_client_gua_error_cb(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status)
{
}


void thread_network_data_service_learn(protocol_interface_info_entry_t *cur)
{
}

void thread_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
}

void thread_timer(protocol_interface_info_entry_t *cur, uint8_t ticks)
{
}

void thread_network_data_update(protocol_interface_info_entry_t *cur)
{
}

bool thread_insist_that_mesh_isnt_a_link(const protocol_interface_info_entry_t *cur)
{
    return false;
}

/* If it's a 16-bit src address, only pass it if it's on the list */
bool thread_test_addr_filter(const buffer_t *buf, const protocol_interface_info_entry_t *cur)
{
    return false;
}

uint8_t *thread_beacon_create_payload(const link_configuration_s *leader_link_setup,thread_commissioner_t *registered_commissioner, uint8_t *return_length)
{
    return NULL;
}

uint8_t thread_beacon_indication(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_network_data_request_disable_trig(protocol_interface_info_entry_t *cur)
{

}

void thread_network_data_request_trig(protocol_interface_info_entry_t *cur, uint8_t *requestDstAddress, bool delaydTrig)
{

}
