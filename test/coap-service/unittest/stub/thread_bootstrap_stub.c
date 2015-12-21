/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */

/*
 * \file thread_bootstrap.c
 * \brief Add short description about this file!!!
 *
 */
#include "config.h"
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
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routerid_server.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_dynamic_reed.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "dhcp_service_api.h"
#include "thread_dhcpv6_client.h"


#define TRACE_GROUP "thbs"

void threa_ula_address_generate_and_push_to_stack(protocol_interface_info_entry_t *cur);
int thread_bootstrap_reset(protocol_interface_info_entry_t *cur);
static void  thread_mle_challenge_trig(int8_t interface_id, uint8_t *LinkId);

void thread_bootstrap_attached_active_router(protocol_interface_info_entry_t *cur);
void thread_attach_router_id_request(protocol_interface_info_entry_t *cur);
void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur);
void thread_bootstrap_child_id_handler(protocol_interface_info_entry_t *cur);
void thread_attach_router_id_release_done(protocol_interface_info_entry_t *cur);
void thread_attach_router_id_release(protocol_interface_info_entry_t *cur);

int8_t nwk_thread_host_control(protocol_interface_info_entry_t *cur, net_host_mode_t mode)
{
    return 0;
}

int thread_link_configuration_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    return 0;
}

void thread_interface_init(protocol_interface_info_entry_t *cur)
{
}

void thread_tasklet(arm_event_s *event)
{
}

int thread_bootstrap_tasklet_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_clean_old_16_bit_address_based_addresses(protocol_interface_info_entry_t *cur)
{
}


void thread_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
}

int thread_leader_configure_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int thread_bootstrap_attach_start(int8_t nwk_interface_id, thread_bootsrap_state_type_e state)
{
    return 0;
}

void thread_bootstrap_leader_data_free(thread_info_t *thread_info, int8_t bootstrap)
{
}

void thread_bootsrap_generate_thread_partitition(int8_t interface_id, bool newPartition)
{
}

void thread_bootstrap_generate_leader_and_link(int8_t interface_id)
{
}

void thread_bootstrap_connection_error(int8_t interface_id, nwk_connect_error_types errorType, uint8_t *LinkId)
{
}

void thread_bootsrap_router_id_cb(int8_t interface_id, uint8_t *ml16_address, uint8_t router_id,  const uint8_t *router_mask_ptr)
{
}

void thread_interface_up(protocol_interface_info_entry_t *cur)
{
}

int thread_bootstrap_reset(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_network_attach_start(protocol_interface_info_entry_t *cur)
{
}

void threa_ula_address_generate_and_push_to_stack(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_ready(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_downgrade_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attched_upgrade_reed(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attched_active_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_router_id_release_ready(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_router_id_get_fail(protocol_interface_info_entry_t *cur)
{
}

int8_t thread_bootstrap_child_id_request(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_bootstrap_child_id_handler(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_active_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur)
{
}

void thread_attach_router_id_release(protocol_interface_info_entry_t *cur)
{
}


void thread_attach_router_id_release_done(protocol_interface_info_entry_t *cur)
{
}

void thread_attach_router_id_request(protocol_interface_info_entry_t *cur)
{
}

bool thread_bootstrap_trig_reed_upgrade(protocol_interface_info_entry_t *cur)
{
    return false;
}

bool thread_bootstrap_trig_router_downgrade(protocol_interface_info_entry_t *cur)
{
    return false;
}

void thread_parent_scan(protocol_interface_info_entry_t *cur)
{
}

int thread_set_link_for_commission(protocol_interface_info_entry_t *interface, nwk_pan_descriptor_t *best)
{
    return 0;
}

void thread_mac_active_scan_response(uint8_t *data)
{
}

void thread_bootstrap_state_machine(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_stop(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_address_register(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_child_update_trig(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_global_leader_data(thread_info_t *thread_info)
{

}

bool thread_tlv_request(int8_t interface_id, uint8_t *address, uint8_t *req_tlv, uint8_t req_len)
{
    return false;
}

int thread_router_mle_advertise(struct protocol_interface_info_entry *cur)
{
    return 0;
}

uint8_t *thread_leader_data_tlv_write(uint8_t *ptr, struct thread_leader_data_s *leader_data)
{
    return NULL;
}
