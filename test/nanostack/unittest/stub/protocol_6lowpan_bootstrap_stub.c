/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/*
 * \file protocol_6lowpan_bootstrap.c
 *
 */
#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#ifndef NO_MLE
#include "MLE/mle.h"
//#include "MLE/mle_challenge_data.h"
#endif
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#ifndef NO_TCP
#ifndef NO_TLS
#include "ccmLIB.h"
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#endif
#include "Common_Protocols/tcp.h"
#endif
#include "6LoWPAN/ND/nd_router_object.h"
#include "common_functions.h"

#define TRACE_GROUP_LOWPAN_BOOT "6Bo"

#include "protocol_6lowpan_bootstrap_stub.h"

protocol_6lowpan_bootstrap_def protocol_6lowpan_bootstrap_stub;

int8_t arm_6lowpan_bootsrap_down(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.int8_value;
}

int8_t arm_6lowpan_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.int8_value;
}

void mle_6lowpan_message_handler(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{

}

int8_t arm_6lowpan_mle_service_ready_for_security_init(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.int8_value;
}

mle_6lowpan_data_t *protocol_6lowpan_mle_data_get(void)
{
    return NULL;
}

void protocol_6lowpan_mle_blacklist_clear(void)
{

}

void arm_6lowpan_bootstrap_init(protocol_interface_info_entry_t *cur)
{
}

int8_t arm_network_processor_up(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.int8_value;
}

int8_t arm_6lowpan_bootstarp_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension)
{
    return protocol_6lowpan_bootstrap_stub.int8_value;
}

void nwk_6lowpan_router_scan_state(protocol_interface_info_entry_t *cur)
{
}

void nwk_6lowpan_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_link_advertise_handle(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t tick)
{
}

uint8_t nwk_bootstrap_icmp_rpl_dis_msg_tx(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.uint8_value;
}

void protocol_6lowpan_bootstrap_nd_ready(protocol_interface_info_entry_t *cur_interface)
{
}


void nwk_6lowpan_rpl_router_result_check(protocol_interface_info_entry_t *cur)
{
}


void nwk_6lowpan_nd_address_registartion_ready(protocol_interface_info_entry_t *cur)
{
}

#ifdef PANA
void nwk_6lowpan_pana_key_pull(protocol_interface_info_entry_t *cur)
{
}
#endif

#ifndef NO_MLE

#ifdef PANA
void nwk_6lowpan_bootsrap_pana_authentication_cb(bool processSuccesfully, protocol_interface_info_entry_t *cur)
{

}

#endif

#endif

void nwk_6lowpan_network_authentication_fail(protocol_interface_info_entry_t *cur)
{
}

uint8_t *protocol_6lowpan_mle_service_security_notify_cb(int8_t interface_id, mle_security_event_t event, uint8_t keyId)
{
    return NULL;
}

void protocol_6lowpan_mac_scan_confirm(uint8_t *data)
{
}

void protocol_6lowpan_bootstrap(protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_mac_clean_mac16(protocol_interface_info_entry_t *interface)
{
}

void protocol_6lowpan_nd_borderrouter_connection_down(protocol_interface_info_entry_t *interface)
{
}

void protocol_6lowpan_bootstrap_re_start(protocol_interface_info_entry_t *interface)
{
}

uint8_t *protocol_6lowpan_nd_border_router_address_get(nwk_interface_id nwk_id)
{
    return protocol_6lowpan_bootstrap_stub.uint8_ptr;
}

uint8_t protocol_6lowpan_rf_link_scalability_from_lqi(uint8_t lqi)
{
    return protocol_6lowpan_bootstrap_stub.uint8_value;
}

int protocol_6lowpan_del_ll16(protocol_interface_info_entry_t *cur, uint16_t mac_short_address)
{
    return 0;
}

int protocol_6lowpan_set_ll16(protocol_interface_info_entry_t *cur, uint16_t mac_short_address)
{
    return 0;
}

int protocol_6lowpan_child_update(protocol_interface_info_entry_t *cur)
{
    return protocol_6lowpan_bootstrap_stub.int_value;
}

int protocol_6lowpan_parent_link_req(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int protocol_6lowpan_router_synch_to_new_router(protocol_interface_info_entry_t *cur, uint8_t *ll64, uint8_t incoming_idr, bool retrans)
{
    return 0;
}

bool lowpan_neighbour_data_clean(int8_t interface_id, const uint8_t *link_local_address)
{
    return false;
}

void arm_6lowpan_security_init_ifup(protocol_interface_info_entry_t *cur)
{

}

void protocol_6lowpan_mle_timer(uint16_t ticks_update)
{

}

void bootstrap_timer_handle(uint16_t ticks)
{

}
