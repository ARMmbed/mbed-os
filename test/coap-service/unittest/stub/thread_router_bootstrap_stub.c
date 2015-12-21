/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

/*
 * \file thread_router_bootstrap.c
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
#include "shalib.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MulticastTrigle/multicast.h"
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_routerid_server.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_dynamic_reed.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_beacon.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "multicast_api.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "thread_dhcpv6_client.h"

#define TRACE_GROUP "trbs"

int thread_router_mle_advertise(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int thread_reed_advertise (protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_set_meshlocal_route(protocol_interface_info_entry_t *cur)
{
}

uint8_t *thread_write_route_option(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}


void thread_multicast_forward_to_child(void *buf)
{
}

void thread_attach_router_id_request(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_child_id_handler(protocol_interface_info_entry_t *cur)
{
}

void thread_mle_connected_reed_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mac_aux_security_header_t *security_headers)
{
}


void thread_reed_synch_start(protocol_interface_info_entry_t *cur)
{
}

bool thread_bootstrap_trig_router_downgrade(protocol_interface_info_entry_t *cur)
{
    return false;
}

bool thread_bootstrap_trig_reed_upgrade(protocol_interface_info_entry_t *cur)
{
    return false;
}

void thread_bootstrap_child_id_reject(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_active_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_router_synch_finish(protocol_interface_info_entry_t *cur)
{
}

void thread_bootsrap_generate_thread_partitition(int8_t interface_id, bool newPartition)
{
}

void thread_attach_router_id_release(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_leader_data_free(thread_info_t *thread_info, int8_t bootstrap)
{
}

void thread_bootstrap_global_leader_data(thread_info_t *thread_info)
{

}


