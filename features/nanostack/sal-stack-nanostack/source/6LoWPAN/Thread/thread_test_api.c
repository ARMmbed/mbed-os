/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "nsconfig.h"

#include <string.h>
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include <net_thread_test.h>
#include "randLIB.h"

#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "6LoWPAN/Thread/thread_bbr_commercial.h"
#include "6LoWPAN/Thread/thread_ccm.h"
#include "MLE/mle.h"
#include "thread_meshcop_lib.h"
#include "thread_diagcop_lib.h"
#include "coap_service_api.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "tapi"


int_fast8_t arm_nwk_6lowpan_thread_test_add_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr,
    uint_fast8_t link_margin_db,
    uint8_t id_sequence,
    const uint8_t *id_mask,
    const uint8_t *route_data)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    return thread_routing_add_link(cur, neighbour_short_addr, link_margin_db, id_sequence, id_mask, route_data, true);
#else
    (void)interface_id;
    (void)neighbour_short_addr;
    (void)link_margin_db;
    (void)id_sequence;
    (void)id_mask;
    (void)route_data;
    return -1;
#endif
}

int_fast8_t arm_nwk_6lowpan_thread_test_remove_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    return thread_routing_remove_link(cur, neighbour_short_addr);
#else
    (void)interface_id;
    (void)neighbour_short_addr;
    return -1;
#endif

}

void arm_nwk_6lowpan_thread_test_print_routing_database(int8_t interface_id)
{
#if defined(FEA_TRACE_SUPPORT) && defined(HAVE_THREAD)
    const protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return;
    }

    const thread_info_t *thread = cur->thread_info;
    if (!thread) {
        tr_warn("Thread not active");
        return;
    }

    //tr_debug("Thread routing database");
    if (thread_i_am_router(cur)) {
        tr_info("I am Thread Router (ID: %d)", thread_router_id_from_addr(mac_helper_mac16_address_get(cur)));
    } else {
        tr_info("I am not a Thread Router");
    }
    //tr_debug("Router ID Set + Current Route:");
    if (thread_i_am_router(cur)) {
        for (int r = 0; r < N_THREAD_ROUTERS; r++) {
            if (thread->routing.fast_route_table[r] != FAST_ROUTE_INVALID_ID) {
                if (thread->routing.fast_route_table[r] == FAST_ROUTE_NO_ROUTE) {
                    tr_debug("Current Route - fast no route: %d -> ?", r);
                } else {
                    tr_debug("Current Route - fast route: %d -> %d", r, thread->routing.fast_route_table[r]);
                }
            }
        }
    }

    ns_list_foreach(const thread_router_link_t, n, &thread->routing.link_set) {
        tr_debug("Link Set - Id:%d Margin:%d.%x InQ:%d OutQ:%d As good:%d Age:%ds",
                 n->router_id,
                 n->link_margin >> THREAD_LINK_MARGIN_SCALING,
                 n->link_margin & ((1 << THREAD_LINK_MARGIN_SCALING) - 1),
                 n->incoming_quality,
                 n->outgoing_quality,
                 n->as_good,
                 n->age / 10);
    }

    if (thread_i_am_router(cur)) {
        ns_list_foreach(const thread_route_t, route, &thread->routing.route_set) {
            tr_debug("Route Set - Dest:%d Next hop:%d Cost:%d",
                     route->destination,
                     route->next_hop,
                     route->route_cost);
        }
    }
#else
    (void)interface_id;
#endif
}

int8_t thread_routing_set_network_id_timeout(int8_t interface_id, uint16_t network_id_timeout)
{
#ifdef HAVE_THREAD
    const protocol_interface_info_entry_t *cur;

    if (network_id_timeout < 5) {
        tr_warn("Too small timeout value, min: 5");
        return -3;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    if (!cur->thread_info) {
        tr_warn("Thread not active");
        return -2;
    }
    cur->thread_info->routing.networkIdTimeout = network_id_timeout;
    return 0;
#else
    (void)interface_id;
    (void)network_id_timeout;
    return -1;
#endif
}

int8_t thread_routing_get_network_id_timeout(int8_t interface_id, uint16_t *network_id_timeout)
{
#ifdef HAVE_THREAD
    const protocol_interface_info_entry_t *cur;

    if (!network_id_timeout) {
        tr_warn("Invalid input ptr");
        return -3;
    }
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    if (!cur->thread_info) {
        tr_warn("Thread not active");
        return -2;
    }
    *network_id_timeout = cur->thread_info->routing.networkIdTimeout;
    return 0;
#else
    (void)interface_id;
    (void)network_id_timeout;
    return -1;
#endif
}

int thread_test_set_context_id_reuse_timeout(
    int8_t interface_id,
    uint32_t timeout)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    if (timeout < 60 || timeout > THREAD_CONTEXT_ID_REUSE_TIMEOUT) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }

    cur->thread_info->networkDataStorage.contex_id_reuse_timeout = timeout;
    return 0;
#else
    (void)interface_id;
    (void)timeout;
    return -1;
#endif
}

int thread_test_remove_router_by_id(int8_t interface_id, uint8_t routerId)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (!cur->thread_info->leader_private_data) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (routerId >= 64) {
        return -1;
    }

    if (!thread_leader_service_route_mask_bit_clear(cur->thread_info->leader_private_data, routerId)) {
        return -1;
    }

    tr_debug("Release Routerid %x", routerId);
    thread_leader_service_update_id_set(cur);

    return 0;
#else
    (void)interface_id;
    (void)routerId;
    return -1;
#endif
}

int thread_test_router_downgrade(int8_t interface_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    tr_debug("Trigger REED router downgrade process");
    thread_bootstrap_attached_downgrade_router(cur);

    return 0;
#else
    (void)interface_id;
    return -1;
#endif
}

int thread_test_print_network_data(int8_t interface_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }


    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    thread_nd_network_data_print(&cur->thread_info->networkDataStorage, mac_helper_mac16_address_get(cur));

    return 0;
#else
    (void)interface_id;
    return -1;
#endif
}


int8_t thread_reed_set_advertisement_interval(int8_t interface_id, uint16_t advertisement_interval, uint16_t jitter_interval)
{
#ifdef HAVE_THREAD
    const protocol_interface_info_entry_t *cur;

    if (!advertisement_interval) {
        tr_warn("Invalid input ptr");
        return -3;
    }
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    if (!cur->thread_info) {
        tr_warn("Thread not active");
        return -2;
    }
    if (!jitter_interval) {
        jitter_interval = 0;
    }
    cur->thread_info->routerSelectParameters.reedAdvertisementInterval = (uint16_t) advertisement_interval;
    cur->thread_info->routerSelectParameters.reedAdvertisementJitterInterval = (uint16_t) jitter_interval;
    tr_debug("new advertisement interval set to %d seconds and jitter %d seconds", (uint16_t) advertisement_interval, (uint16_t) jitter_interval);
    return 0;
#else
    (void)interface_id;
    (void)advertisement_interval;
    (void)jitter_interval;
    return -1;
#endif
}

int thread_test_key_sequence_counter_update(int8_t interface_id, uint32_t thrKeySequenceCounter)
{
#ifdef HAVE_THREAD
    int ret_val = -1;
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info) {
        /* Not sure if this check is needed - not sure exactly what the flag means! */
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            if (cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED) {
                if (thrKeySequenceCounter > linkConfiguration->key_sequence) {
                    ret_val = thread_management_key_sets_calc(cur, linkConfiguration, thrKeySequenceCounter);
                }
            }
        }
    }
    return ret_val;
#else
    (void)interface_id;
    (void)thrKeySequenceCounter;
    return -1;
#endif
}

int thread_test_stack_cache_reset(int8_t interface_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    // Reset link information
    (void) thread_nvm_store_link_info_clear();

    return 0;
#else
    (void)interface_id;
    return -1;
#endif
}

int thread_test_key_rotation_update(int8_t interface_id, uint32_t thrKeyRotation)
{
#ifdef HAVE_THREAD
    int ret_val = -1;
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info) {
        /* Not sure if this check is needed - not sure exactly what the flag means! */
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            if (cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED) {
                linkConfiguration->key_rotation = thrKeyRotation;
                thread_key_guard_timer_calculate(cur, linkConfiguration, false);
                ret_val = 0;
            }
        }
    }
    return ret_val;
#else
    (void)interface_id;
    (void)thrKeyRotation;
    return -1;
#endif
}

int thread_test_router_select_threshold_values_set(
    int8_t interface_id,
    uint8_t upgradeThreshold,
    uint8_t downgradeThreshold)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    if (downgradeThreshold == 0) {
        tr_warn("Invalid Threshold value");
        return -1;
    }

    if (upgradeThreshold >= downgradeThreshold) {
        tr_warn("Invalid Threshold value %u >= %u", upgradeThreshold, downgradeThreshold);
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }

    cur->thread_info->routerSelectParameters.routerUpgradeThresHold = upgradeThreshold;
    cur->thread_info->routerSelectParameters.routerDowngradeThresHold = downgradeThreshold;

    return 0;
#else
    (void)interface_id;
    (void)upgradeThreshold;
    (void)downgradeThreshold;
    return -1;
#endif
}
int thread_test_max_accepted_router_id_limit_set(
    int8_t interface_id,
    uint8_t maxRouterLimit)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    if ((maxRouterLimit < 1) || (maxRouterLimit > 32)) {
        tr_error("Accept values are between 1-32 for max Router Id assign limit");
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }
    cur->thread_info->testMaxActiveRouterIdLimit = maxRouterLimit;
    tr_debug("New max Router Id assign limit is %u", maxRouterLimit);
    return 0;
#else
    (void)interface_id;
    (void)maxRouterLimit;
    return -1;
#endif
}


/**
 * Set Thread Security Master Key and Key Index
 *
 * \param interface_id Network Interface
 * \param enableSecurity Boolean for enable security or disable
 * \param threadMasterKey Thread Master Key material which will be used for generating new key
 * \param threadMasterKeyIndex Thread key material key index which will be increment periodically
 * \param keyRollPeriod Define Key index & key update process
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_test_security_material_set(int8_t interface_id, bool enableSecurity, uint8_t *thrMasterKey, uint32_t thrKeySequenceCounter, uint32_t thrKeyRotation)
{
#ifdef HAVE_THREAD
    int ret_val = -1;
    uint8_t key_material[32];
    uint8_t key_index;
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
        return -4;
    }

    if (cur->thread_info) {

        if (cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED) {
            mac_helper_link_frame_counter_set(cur->id, 0);
            if (enableSecurity) {
                if (thrMasterKey) {
                    ret_val = 0;
                    linkConfiguration->key_rotation = thrKeyRotation;
                    linkConfiguration->key_sequence = thrKeySequenceCounter;
                    cur->if_lowpan_security_params->nwk_security_mode = NET_SEC_MODE_PSK_LINK_SECURITY;
                    cur->mac_parameters->mac_configured_sec_level = 5;
                    cur->thread_info->masterSecretMaterial.historyKeyValid = false;
                    cur->thread_info->masterSecretMaterial.valid_Info = true;
                    memcpy(linkConfiguration->master_key, thrMasterKey, 16);
                    cur->thread_info->masterSecretMaterial.keyRotation = thrKeyRotation;
                    //Define KEY's
                    thread_key_get(thrMasterKey, key_material, thrKeySequenceCounter);
                    key_index = THREAD_KEY_INDEX(thrKeySequenceCounter);
                    //Set Keys
                    mac_helper_security_default_key_set(cur, &key_material[16], key_index, MAC_KEY_ID_MODE_IDX);
                    mle_service_security_set_security_key(cur->id, key_material, key_index, true);
                    //Gen also Next Key
                    thread_security_next_key_generate(cur, linkConfiguration->master_key, thrKeySequenceCounter);
                    thread_key_guard_timer_calculate(cur, linkConfiguration, false);
                }
            } else {
                ret_val = 0;
                cur->if_lowpan_security_params->nwk_security_mode = NET_SEC_MODE_NO_LINK_SECURITY;
                cur->mac_parameters->mac_configured_sec_level = 0;
                cur->thread_info->masterSecretMaterial.valid_Info = false;
            }
            if (ret_val == 0) {
                cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
                cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
            }
        }
    }
    return ret_val;
#else
    (void)interface_id;
    (void)enableSecurity;
    (void)thrMasterKey;
    (void)thrKeySequenceCounter;
    (void)thrKeyRotation;
    return -1;
#endif
}

int thread_test_version_set(int8_t interface_id, uint8_t version)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    thread_version = version;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        /*We already stored the new Thread version above, so even if cur is NULL the version is updated.*/
        return 0;
    }
    cur->thread_info->version = version;
    return 0;

#else
    (void)version;
    (void)interface_id;
    return -1;
#endif
}

int thread_test_pbbr_response_override_set(int8_t interface_id, uint8_t dua_status, uint8_t dua_count, uint8_t ba_failure_count)
{
#ifdef HAVE_THREAD
    (void)interface_id;
    thread_bbr_commercial_status_override_set(dua_status, dua_count, ba_failure_count);
    return 0;

#else
    (void)interface_id;
    (void)dua_status;
    (void)dua_count;
    (void)ba_failure_count;
    return -1;
#endif
}

int thread_test_router_selection_jitter_set(int8_t interface_id, uint32_t jitter)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    if (jitter < 1) {
        return -1;
    }
    thread_router_selection_jitter = jitter;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info && cur->thread_info->reedJitterTimer > jitter) {
        cur->thread_info->reedJitterTimer = thread_router_bootstrap_random_upgrade_jitter();
    }
    return 0;

#else
    (void)interface_id;
    (void)jitter;
    return -1;
#endif
}

int thread_test_min_delay_timer_set(int8_t interface_id, uint32_t delay_timer_value)
{
#ifdef HAVE_THREAD
    (void)interface_id;
    if (delay_timer_value < 1) {
        return -1;
    }
    thread_delay_timer_default = delay_timer_value;
    return 0;

#else
    (void)interface_id;
    (void)delay_timer_value;
    return -1;
#endif
}

int thread_test_increment_key_sequence_counter(int8_t interface_id)
{
#ifdef HAVE_THREAD
    return thread_management_increment_key_sequence_counter(interface_id);
#else
    (void)interface_id;
    return -1;
#endif
}
#ifdef HAVE_THREAD
static int thread_test_panid_conflict_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;
    uint16_t panid;
    uint8_t *ptr;
    uint16_t len MAYBE_UNUSED;
    tr_debug("Thread test panid conflict request");
    if (2 <= thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PANID, &panid)) {
        tr_info("PANID TLV %02x", panid);
    }
    if ((len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL_MASK, &ptr)) > 0) {
        tr_info("Channel mask TLV %s", trace_array(ptr, len));
    }
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
    return 0;
}
#endif

#ifdef HAVE_THREAD
static int thread_test_energy_scan_result_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;
    uint8_t *ptr;
    uint16_t len MAYBE_UNUSED;

    tr_debug("Thread test energy scan result");
    if ((len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL_MASK, &ptr)) > 0) {
        tr_info("Channel mask TLV %s", trace_array(ptr, len));
    }
    if ((len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_ENERGY_LIST, &ptr)) > 0) {
        tr_info("Energy list TLV %s", trace_array(ptr, len));
    }
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
    return 0;
}
#ifdef HAVE_THREAD
static uint8_t coap_service = 0;
static response_cb *generic_response_cb_ptr = NULL;
static coap_response_cb *coap_response_cb_ptr = NULL;

static int thread_test_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void)source_address;
    (void)source_port;

    tr_debug("Thread test response received");
    if (!response_ptr) {
        return -1;
    }
    /* Call callback function when receiving response, but do not call when receiving message code "empty" (Just ACK - not piggybacked response) */
    if (generic_response_cb_ptr && (response_ptr->msg_code != COAP_MSG_CODE_EMPTY)) {
        generic_response_cb_ptr(service_id, response_ptr->payload_ptr, response_ptr->payload_len);
    }

    if (response_ptr->uri_path_ptr && (!memcmp(response_ptr->uri_path_ptr, THREAD_URI_DIAGNOSTIC_ANSWER, strlen(THREAD_URI_DIAGNOSTIC_ANSWER)))) {
        coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, response_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
    }
    return 0;
}

static int thread_coap_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
#ifndef HAVE_DEBUG
    (void) source_address;
    (void) source_port;
#endif

    tr_debug("Thread coap response received %s :%d", trace_ipv6(source_address), source_port);
    if (!response_ptr) {
        if (coap_response_cb_ptr) {
            coap_response_cb_ptr(service_id, 0xff, 0xff, NULL, 0);
        }
        return 0;
    }
    /* Call callback function when receiving response, but do not call when receiving message code "empty" (Just ACK - not piggybacked response) */
    if (coap_response_cb_ptr && (response_ptr->msg_code != COAP_MSG_CODE_EMPTY)) {
        coap_response_cb_ptr(service_id, (uint8_t)(response_ptr->msg_code), (uint8_t)(response_ptr->msg_type), response_ptr->payload_ptr, response_ptr->payload_len);
    }

    return 0;
}

static int thread_coap_multicast_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
#ifndef HAVE_DEBUG
    (void) source_address;
    (void) source_port;
#endif

    tr_debug("Thread coap multicast response received %s :%d", trace_ipv6(source_address), source_port);
    /* Call callback function when receiving response, but do not call when receiving message code "empty" (Just ACK - not piggybacked response) */

    if (coap_response_cb_ptr) {
        coap_response_cb_ptr(service_id, (uint8_t)(response_ptr->msg_code), (uint8_t)(response_ptr->msg_type), response_ptr->payload_ptr, response_ptr->payload_len);
    }
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, response_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
    return 0;
}
#endif

static int thread_test_service_init(int8_t interface_id)
{
    if (coap_service == 0) {
        coap_service = coap_service_initialize(interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
        coap_service_register_uri(coap_service, THREAD_URI_PANID_CONFLICT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_test_panid_conflict_cb);
        coap_service_register_uri(coap_service, THREAD_URI_ED_REPORT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_test_energy_scan_result_cb);
        coap_service_register_uri(coap_service, THREAD_URI_DIAGNOSTIC_ANSWER, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_coap_multicast_response_cb);
    }
    return 0;
}

static uint8_t *channel_mask_write(uint8_t *ptr, uint8_t channel_page, uint8_t *mask_ptr)
{
    uint8_t mask_len = 0;
    uint8_t mask_tlv[6];//
    if (channel_page == 0) {
        mask_len = 6;
        mask_tlv[0] = channel_page;
        mask_tlv[1] = 4;
        memcpy(&mask_tlv[2], mask_ptr, 4);
    }
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_CHANNEL_MASK, mask_len, mask_tlv);
    return ptr;
}
#endif
int thread_test_panid_query_send(int8_t interface_id, uint8_t *address_ptr, uint16_t session_id, uint16_t panid, uint8_t channel_page, uint8_t *mask_ptr)
{
#ifdef HAVE_THREAD
    sn_coap_msg_type_e msg_type;
    uint8_t payload[16];// 2+6 + 2+2 + 2+2
    uint8_t *ptr;
    thread_test_service_init(interface_id);
    if (address_ptr[0] == 0xff) {
        msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    } else {
        msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    }
    ptr = payload;
    ptr = channel_mask_write(ptr, channel_page, mask_ptr);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_PANID, panid);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);
    coap_service_request_send(coap_service, COAP_REQUEST_OPTIONS_NONE, address_ptr, THREAD_MANAGEMENT_PORT,
                              msg_type, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_PANID_QUERY, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_test_response_cb);

    return 0;
#else
    (void)interface_id;
    (void)address_ptr;
    (void)session_id;
    (void)panid;
    (void)channel_page;
    (void)mask_ptr;
    return -1;
#endif
}
int thread_test_energy_scan_send(int8_t interface_id, uint8_t *address_ptr, uint16_t session_id, uint8_t channel_page, uint8_t *mask_ptr, uint16_t period, uint8_t count, uint16_t duration)
{
#ifdef HAVE_THREAD
    sn_coap_msg_type_e msg_type;
    uint8_t payload[2 + 2 + 2 + 1 + 2 + 1 + 2 + 6 + 2 + 2];
    uint8_t *ptr;
    thread_test_service_init(interface_id);
    if (address_ptr[0] == 0xff) {
        msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    } else {
        msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    }
    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_PERIOD, period);
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_COUNT, count);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_DURATION, duration);

    ptr = channel_mask_write(ptr, channel_page, mask_ptr);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);
    coap_service_request_send(coap_service, COAP_REQUEST_OPTIONS_NONE, address_ptr, THREAD_MANAGEMENT_PORT,
                              msg_type, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ED_SCAN, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_test_response_cb);

    return 0;
#else
    (void)interface_id;
    (void)address_ptr;
    (void)session_id;
    (void)channel_page;
    (void)mask_ptr;
    (void)period;
    (void)count;
    (void)duration;
    return -1;
#endif
}

int thread_test_diagnostic_command_send(int8_t interface_id, uint8_t *address_ptr, const char *uri_ptr, uint8_t request_length, uint8_t *request_ptr, response_cb *resp_cb)
{
#ifdef HAVE_THREAD
    uint8_t payload[17 + 2] = {0};
    uint8_t *ptr = NULL;
    generic_response_cb_ptr = resp_cb;
    sn_coap_msg_type_e msg_type;
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_REQUEST_GET;

    thread_test_service_init(interface_id);

    if (request_length > 17 || !address_ptr) {
        return -1;
    }

    if (address_ptr[0] == 0xff) {
        msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    } else {
        msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    }

    if (!strcmp(uri_ptr, "d/dr")) {
        msg_code = COAP_MSG_CODE_REQUEST_POST;
    }

    ptr = payload;
    if (request_length > 0 && request_ptr) {
        ptr = thread_diagcop_tlv_data_write(ptr, DIAGCOP_TLV_TYPE_LIST, request_length, request_ptr);
    }
    coap_service_request_send(coap_service, COAP_REQUEST_OPTIONS_NONE, address_ptr, THREAD_MANAGEMENT_PORT,
                              msg_type, msg_code, uri_ptr, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_test_response_cb);

    return 0;
#else
    (void)interface_id;
    (void)address_ptr;
    (void)uri_ptr;
    (void)request_length;
    (void)request_ptr;
    (void)resp_cb;
    return -1;
#endif
}
int thread_test_coap_request_send(int8_t interface_id, uint8_t *address_ptr, uint16_t port, uint8_t msg_type, uint8_t msg_code, uint16_t content_format, const char *uri_ptr, uint8_t *request_ptr, uint8_t request_length, coap_response_cb *resp_cb)
{
#ifdef HAVE_THREAD
    coap_response_cb_ptr = resp_cb;

    thread_test_service_init(interface_id);

    coap_service_request_send(coap_service, COAP_REQUEST_OPTIONS_NONE, address_ptr, port,
                              (sn_coap_msg_type_e)msg_type, (sn_coap_msg_code_e)msg_code, uri_ptr, (sn_coap_content_format_e) content_format, request_ptr, request_length, thread_coap_response_cb);

    return 0;
#else
    (void)interface_id;
    (void)address_ptr;
    (void)port;
    (void)msg_type;
    (void)msg_code;
    (void)content_format;
    (void)uri_ptr;
    (void)request_length;
    (void)request_ptr;
    (void)resp_cb;
    return -1;
#endif
}


int thread_test_announce_ntf_send(int8_t interface_id, uint8_t *address_ptr, uint32_t channel, uint16_t panid, uint64_t timestamp)
{
#ifdef HAVE_THREAD

    (void)address_ptr;
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    return thread_bootstrap_announce_send(cur, (uint8_t)(channel >> 16), (uint16_t) channel, panid, timestamp, (uint16_t) channel);
#else
    (void)interface_id;
    (void)address_ptr;
    (void)channel;
    (void)panid;
    (void)timestamp;
    return -1;
#endif
}

int thread_test_announce_begin_send(int8_t interface_id, uint8_t *address_ptr, uint16_t session_id, uint8_t channel_page, uint8_t *mask_ptr, uint16_t period, uint8_t count)
{
#ifdef HAVE_THREAD
    uint8_t payload[ 2 + 2 + 2 + 2 + 2 + 1 + 2 + 6];
    uint8_t *ptr;
    sn_coap_msg_type_e msg_type;
    thread_test_service_init(interface_id);

    if (address_ptr[0] == 0xff) {
        msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    } else {
        msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    }

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_PERIOD, period);
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_COUNT, count);
    ptr = channel_mask_write(ptr, channel_page, mask_ptr);
    coap_service_request_send(coap_service, COAP_REQUEST_OPTIONS_NONE, address_ptr, THREAD_MANAGEMENT_PORT,
                              msg_type, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_MANAGEMENT_ANNOUNCE_BEGIN, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_test_response_cb);

    return 0;
#else
    (void)interface_id;
    (void)address_ptr;
    (void)session_id;
    (void)channel_page;
    (void)mask_ptr;
    (void)period;
    (void)count;
    return -1;
#endif
}

int thread_test_partition_info_get(int8_t interface_id, uint32_t *partition_id, uint8_t *weighting, uint8_t *data_version, uint8_t *stable_data_version, uint8_t *leader_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->thread_leader_data) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (partition_id) {
        *partition_id = cur->thread_info->thread_leader_data->partitionId;
    }
    if (weighting) {
        *weighting = cur->thread_info->thread_leader_data->weighting;
    }
    if (data_version) {
        *data_version = cur->thread_info->thread_leader_data->dataVersion;
    }
    if (stable_data_version) {
        *stable_data_version = cur->thread_info->thread_leader_data->stableDataVersion;
    }
    if (leader_id) {
        *leader_id = cur->thread_info->thread_leader_data->leaderRouterId;
    }
    return 0;
#else
    (void)interface_id;
    (void)partition_id;
    (void)weighting;
    (void)data_version;
    (void)stable_data_version;
    (void)leader_id;
    return -1;
#endif
}
int thread_test_partition_info_set(int8_t interface_id, uint32_t partition_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    cur->thread_info->testRandomPartitionId = partition_id;
    return 0;

#else
    (void)interface_id;
    (void)partition_id;
    return -1;
#endif
}
int8_t thread_test_thread_information_get(int8_t interface_id, uint16_t *short_addr, uint8_t *router_count, bool *network_stable)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    if (short_addr) {
        *short_addr = mac_helper_mac16_address_get(cur);
    }
    if (router_count) {
        *router_count = thread_routing_count_active_routers(&cur->thread_info->routing);
    }
    if (network_stable) {
        if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE ||
                cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE ||
                cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE) {
            *network_stable = true;
        } else {
            *network_stable = !(thread_router_bootstrap_reed_upgrade(cur) ||
                                thread_router_bootstrap_router_downgrade(cur));
        }

    }
    return 0;
#else
    (void)interface_id;
    (void)short_addr;
    (void)router_count;
    (void)network_stable;
    return -1;
#endif
}


int8_t thread_test_child_count_get(int8_t interface_id)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !(cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER)) {
        return -1;
    }
    return thread_router_bootstrap_child_count_get(cur);
#else
    (void)interface_id;
    return -1;
#endif
}

int8_t thread_test_child_info_get(int8_t interface_id, uint8_t index, uint16_t *short_addr, bool *sleepy, uint8_t *mac64, uint8_t *margin)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    uint8_t n = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->mac_parameters || !cur->thread_info || cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER) {
        return -1;
    }
    uint16_t mac16 = mac_helper_mac16_address_get(cur);
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;
    if (!mac_table_list) {
        return -1;
    }

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (thread_router_addr_from_addr(cur_entry->mac16) == thread_router_addr_from_addr(mac16)) {
            if (n == index) {
                *short_addr = cur_entry->mac16;
                memcpy(mac64, cur_entry->mac64, 8);
                *sleepy = cur_entry->rx_on_idle != true;
                *margin = thread_neighbor_entry_linkmargin_get(&cur->thread_info->neighbor_class, cur_entry->index);
                return 0;
            }
            n++;
        }
    }
    return -1;
#else
    (void)interface_id;
    (void)index;
    (void)short_addr;
    (void)sleepy;
    (void)mac64;
    (void)margin;
    return -1;
#endif
}
int8_t thread_test_neighbour_info_get(int8_t interface_id, uint8_t index, uint16_t *short_addr, uint8_t *mac64, uint8_t *margin)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    uint8_t n = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    uint16_t mac16 = mac_helper_mac16_address_get(cur);
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;
    if (!mac_table_list) {
        return -1;
    }

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (thread_router_addr_from_addr(cur_entry->mac16) != thread_router_addr_from_addr(mac16)) {
            if (n == index) {
                *short_addr = cur_entry->mac16;
                memcpy(mac64, cur_entry->mac64, 8);
                *margin = thread_neighbor_entry_linkmargin_get(&cur->thread_info->neighbor_class, cur_entry->index);
                return 0;
            }
            n++;
        }
    }
    return -1;
#else
    (void)interface_id;
    (void)index;
    (void)short_addr;
    (void)mac64;
    (void)margin;
    return -1;
#endif
}

int8_t thread_test_initial_slaac_iid_set(int8_t interface_id, uint8_t *iid)
{
#ifdef HAVE_THREAD
    (void)interface_id;// I somehow feel this might be needed later
    return addr_opaque_initial_iid_set(iid);
#else
    (void)interface_id;
    (void)iid;
    return -1;
#endif
}


int8_t thread_test_router_id_request_send(int8_t interface_id, uint8_t status)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    tr_debug("Trigger REED router upgrade process with status %d", status);

    thread_router_bootstrap_router_id_request(cur, status);

    return 0;
#else
    (void)interface_id;
    (void)status;
    return -1;
#endif

}

int8_t thread_test_router_address_set(int8_t interface_id, uint16_t router_address)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    if ((router_address & THREAD_ROUTER_MASK) == THREAD_ROUTER_MASK) {
        tr_warn("address requested is reserved for anycast locator");
        return -2;
    }
    if (cur->thread_info->routerShortAddress != 0xfffe) {
        tr_debug("router short address already present");
        return -3;
    }
    cur->thread_info->routerShortAddress = router_address;
    return 0;
#else
    (void)interface_id;
    (void)router_address;
    return -1;
#endif
}

int8_t thread_test_joiner_router_joiner_port_set(uint16_t port)
{
#ifdef HAVE_THREAD
    /* If parameter is '0', then use default port */
    if (0 == port) {
        port = THREAD_DEFAULT_JOINER_PORT;
    }

    thread_joiner_port = port;
    return 0;
#else
    (void)port;
    return -1;
#endif

}

int8_t thread_test_mcast_address_per_message_set(uint8_t value)
{
#ifdef HAVE_THREAD
    if (value == 0 || value > 15) {
        tr_err("Value not in range. Valid range 1-15");
        return -1;
    }

    thread_max_mcast_addr = value;

    return 0;
#else
    (void)value;
    return -1;
#endif
}

int thread_test_mle_message_send(int8_t interface_id, uint8_t *dst_address, uint8_t msg_id, bool write_src_addr, bool write_leader_data, bool write_network_data, bool write_timestamp, bool write_operational_set, bool write_challenge, uint8_t *msg_ptr, uint8_t msg_len)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    uint16_t len = 32;
    uint32_t keySequence;
    uint8_t *ptr;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    len += msg_len;
    if (write_operational_set) {
        len += thread_active_operational_dataset_size(cur);
        len += thread_pending_operational_dataset_size(cur);
    }
    if (write_network_data) {
        len += thread_network_data_tlv_size(cur, true);
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, len, write_challenge, msg_id);

    if (bufId == 0) {
        return -1;
    }

    tr_debug("thread test send MLE message");

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    ptr = mle_service_get_data_pointer(bufId);

    //Set SRC
    if (write_src_addr) {
        ptr = mle_general_write_source_address(ptr, cur);
    }
    //SET leader data
    if (write_leader_data) {
        ptr = thread_leader_data_tlv_write(ptr, cur);
    }
    if (write_timestamp) {
        ptr = thread_active_timestamp_write(cur, ptr);
        ptr = thread_pending_operational_dataset_write(cur, ptr);
    }

    if (write_operational_set) {
        ptr = thread_active_operational_dataset_write(cur, ptr);
        ptr = thread_pending_operational_dataset_write(cur, ptr);
    }
    if (write_network_data) {
        ptr = thread_network_data_tlv_write(cur, ptr, true);
    }
    // Add any user decided TLVs
    if (msg_ptr && msg_len) {
        memcpy(ptr, msg_ptr, msg_len);
        ptr += msg_len;
    }

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, dst_address);
    mle_service_send_message(bufId);
    return 0;
#else
    (void)interface_id;
    (void)dst_address;
    (void)msg_id;
    (void)write_src_addr;
    (void)write_leader_data;
    (void)write_network_data;
    (void)write_timestamp;
    (void)write_operational_set;
    (void)write_challenge;
    (void)msg_ptr;
    (void)msg_len;
    return -1;
#endif
}


int thread_test_extension_name_set(int8_t interface_id, char extension_name[16])
{
    (void) interface_id;
    (void) extension_name;
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    return thread_ccm_thread_name_set(cur, extension_name);
#else
    return -1;
#endif
}

int thread_test_parent_priority_set(int8_t interface_id, uint8_t parent_priority)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -2;
    }
    cur->thread_info->parent_priority = parent_priority;
    return 0;
#else
    (void) interface_id;
    (void) parent_priority;
    return -1;
#endif
}
