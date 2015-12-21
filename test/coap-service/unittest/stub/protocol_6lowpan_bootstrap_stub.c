/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

/*
 * \file protocol_6lowpan_bootstrap.c
 *
 */
#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#ifdef HAVE_RPL
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#else
/* This sort of thing should be handled in rpl.h itself */
#define rpl_object_poisons() 0
#endif
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

int8_t arm_6lowpan_bootsrap_down(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int8_t arm_6lowpan_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    return -1;
}

void arm_6lowpan_bootstrap_init(protocol_interface_info_entry_t *cur)
{
}

void nwk_6lowpan_router_scan_state(protocol_interface_info_entry_t *cur)
{
}

void nwk_6lowpan_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_rpl_external_advertisment(protocol_interface_info_entry_t *cur_interface , rpl_dodag_t *cur)
{
}

void protocol_6lowpan_link_advertise_handle(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t tick)
{
}

uint8_t nwk_bootstrap_icmp_rpl_dis_msg_tx(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void protocol_6lowpan_bootstrap_nd_ready(protocol_interface_info_entry_t *cur_interface, nd_router_t *cur)
{
}

void nwk_6lowpan_rpl_router_discover(protocol_interface_info_entry_t *cur)
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

void nwk_6lowpan_network_authentication_done(protocol_interface_info_entry_t *cur)
{
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
    return NULL;
}

uint8_t protocol_6lowpan_rf_link_scalability_from_lqi(uint8_t lqi)
{
    return 0;
}
