/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

/*
 * \file protocol_6lowpan_interface.c
 * \brief Add short description about this file!!!
 *
 */
#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/icmp.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Core/include/routing_table.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#ifdef HAVE_RPL
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#endif
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#ifndef NO_MLE
#include "MLE/mle.h"
//#include "MLE/mle_challenge_data.h"
#endif
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread.h"
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
#include "MulticastTrigle/multicast.h"
#include "NAP/nap_config.h"
#include "NWK_INTERFACE/Include/protocol_interface_driver.h"

#include "6LoWPAN/ND/nd_router_object.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol_interface_driver.h"


int8_t nwk_6lowpan_up(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int8_t nwk_6lowpan_down(protocol_interface_info_entry_t *cur)
{
    return 0;
}
