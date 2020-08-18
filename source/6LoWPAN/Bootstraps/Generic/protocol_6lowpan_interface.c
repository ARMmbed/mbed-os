/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
 * \file protocol_6lowpan_interface.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "mac_api.h"

#ifdef HAVE_RPL
#include "RPL/rpl_control.h"
#endif
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread_common.h"

#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "ccmLIB.h"
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "Common_Protocols/tcp.h"

#include "6LoWPAN/ND/nd_router_object.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "libNET/src/net_load_balance_internal.h"

void protocol_mac_reset(protocol_interface_info_entry_t *cur)
{
    if (cur->mac_api) {
        mlme_reset_t reset;
        reset.SetDefaultPIB = true;
        cur->mac_api->mlme_req(cur->mac_api, MLME_RESET, &reset);
    }
}



static int8_t set_6lowpan_nwk_down(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;
    // Check first current state
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        /* Change Active -> Idle */
        /* Disable Protocols Timers */
        if (!thread_info(cur)) {
            mac_neighbor_table_neighbor_list_clean(mac_neighbor_info(cur));
#ifndef NO_MLE
            if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {

                blacklist_clear();

            }
#endif
        }
        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {
            pana_reset_values(cur->mac_parameters->pan_id);
        }

        if (cur->interface_mode == INTERFACE_UP) {
            cur->mac_parameters->pan_id = 0xffff;
            cur->mac_parameters->SecurityEnabled = false;
            cur->mac_parameters->security_frame_counter = 0;
            cur->mac_parameters->mac_security_level = 0;
            protocol_mac_reset(cur);
            cur->interface_mode = INTERFACE_IDLE;
            net_load_balance_internal_state_activate(cur, false);
        }
        lowpan_adaptation_interface_reset(cur->id);
        reassembly_interface_reset(cur->id);

        icmp_nd_routers_init();

        if (cur->pana_sec_info_temp) {
            ns_dyn_mem_free(cur->pana_sec_info_temp);
            cur->pana_sec_info_temp = 0;
        }
        /* Init RPL Timers */
        cur->bootsrap_state_machine_cnt = 0;
        mac_helper_free_scan_confirm(&cur->mac_parameters->nwk_scan_params);

        cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
        cur->lowpan_info &= ~(INTERFACE_NWK_BOOTSRAP_ACTIVE | INTERFACE_NWK_ACTIVE);
        cur->interface_mode = INTERFACE_IDLE;
        ret_val = 0;
    }
    return ret_val;
}

static int8_t set_6lowpan_nwk_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = 1;

    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
        /* Change Idle-> Active */
        icmp_nd_routers_init();
        cur->nwk_bootstrap_state = ER_ACTIVE_SCAN;
        cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE | INTERFACE_NWK_ACTIVE; //Set Active Bootsrap
        cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY; //Clear Bind
        cur->bootsrap_state_machine_cnt = 2;
        //Possible mac_mlme_start_req(call)
        mac_helper_panid_set(cur, 0xffff);
        mac_helper_mac16_address_set(cur, 0xffff);

        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {
            mac_helper_default_security_level_set(cur, SEC_NONE);
        }
        cur->interface_mode = INTERFACE_UP;
        ret_val = 0;
    }
    return ret_val;
}

int8_t nwk_6lowpan_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val;

    ret_val = set_6lowpan_nwk_up(cur);
    if (ret_val == 0) {
        protocol_6lowpan_interface_common_init(cur);

        cur->nwk_mode = ARM_NWK_GP_IP_MODE;
        nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
        filter->nwk_active_scan_level = 2;
    }

    return ret_val;
}

int8_t nwk_6lowpan_down(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val;
    neighbor_cache_flush(&cur->neigh_cache);
    ret_val = set_6lowpan_nwk_down(cur);
    protocol_core_interface_info_reset(cur);
    return ret_val;
}
