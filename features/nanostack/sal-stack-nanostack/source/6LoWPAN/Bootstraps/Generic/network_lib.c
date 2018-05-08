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
#include "ns_types.h"
#include <string.h>
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "MLE/mle.h"
#include "shalib.h"
#include "Common_Protocols/ipv6_constants.h"

#ifdef PANA
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "ccmLIB.h"
#endif
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "net_nwk_scan.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#ifdef HAVE_RPL
#include "RPL/rpl_data.h"
#endif

#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"

#define MAC_MLME_SACN_PERIO_PARAM 5

#define TRACE_GROUP "nw"

/**
 * \brief Check That Network Have IPv6 Address Ready.
 *
 * \return 1 when IPv6 address is valid and 0 when it is not valid
 */

uint8_t nwk_ready(nwk_interface_id id)
{
    uint8_t ret_val = 0;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get(id);
    if (cur) {
        if (thread_info(cur)) {
            if (thread_attach_ready(cur) == 0) {
                ret_val = 1;
            }
        } else {
            if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
                ret_val =  1;
            }
        }
    }
    return ret_val;

}

void nwk_parent_poll_fail_cb(int8_t id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(id);
    if (!cur) {
        tr_error("Data Poll Fail Event: No interface");
        return;
    }
    if (thread_info(cur)) {
        //Initialize Bootsrap
        thread_bootstrap_connection_error(cur->id, CON_ERROR_POLL, NULL);
    } else {
        nwk_bootsrap_state_update(ARM_NWK_NWK_PARENT_POLL_FAIL, cur);
    }

}

void network_library_init(void)
{
    socket_init();
    address_module_init();
    //SET MAC_MLME Handler
    protocol_init();
}

buffer_t *nwk_udp_rx_security_check(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;
    uint8_t drop_unsecured = 0;

    // Hack for PANA and MLE. PANA socket is not unsecured, need to allow unsecured link local traffic.
    // MLE need to allow joiner request, that is not secured.
    // TODO: Check if there is better fix for these.
    if (buf->src_sa.port == UDP_PORT_PANA || buf->dst_sa.port == UDP_PORT_PANA) {
        if ((buf->dst_sa.address[0] != 0xfe)  && (buf->options.ll_security_bypass_rx)) {
            drop_unsecured = 1;
        }
    } else if (buf->dst_sa.port == UDP_PORT_MLE) {
        // OK
    } else if (buf->options.ll_security_bypass_rx) {
        if (addr_ipv6_scope(buf->src_sa.address, cur) > IPV6_SCOPE_LINK_LOCAL) {
            drop_unsecured = 1;
        } else {
            if (!buf->socket) {
                buffer_socket_set(buf, socket_lookup_ipv6(IPV6_NH_UDP, &buf->dst_sa, &buf->src_sa, true));
            }
            if (buf->socket && buf->socket->inet_pcb->link_layer_security == 0) {
                // non-secure okay if it's for a socket whose security flag is clear.
            }
            else {
                drop_unsecured = 1;
            }
        }
    }

    if (drop_unsecured) {
        tr_warn("Drop UDP Unsecured");
        buf = buffer_free(buf);
    }

    return buf;
}

