/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

/*
 * \file thread_host_bootstrap.c
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
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#define TRACE_GROUP "tebs"



void thread_child_set_default_route(protocol_interface_info_entry_t *cur)
{

}

void thread_network_attach_start(protocol_interface_info_entry_t *cur)
{

}


void thread_bootstrap_end_device_synch_finish(protocol_interface_info_entry_t *cur)
{

}


void thread_mle_parent_discover_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mac_aux_security_header_t *security_headers)
{

}

void thread_mle_connected_host_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mac_aux_security_header_t *security_headers)
{

}


void thread_endevice_synch_start(protocol_interface_info_entry_t *cur)
{
}

int8_t thread_bootstrap_child_update(protocol_interface_info_entry_t *cur)
{
    return 0;
}

