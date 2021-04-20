/*
 * Copyright (c) 2014-2015, 2017-2019, Arm Limited and affiliates.
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
#ifdef HAVE_THREAD
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
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_beacon.h"
#include "6LoWPAN/Thread/thread_diagnostic.h"
#include "6LoWPAN/Thread/thread_bbr_commercial.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_lowpower_private_api.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_resolution_server.h"
#include "6LoWPAN/Thread/thread_resolution_client.h"
#include "6LoWPAN/Thread/thread_ccm.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "MLE/mle.h"
#include "Service_Libs/mle_service/mle_service_security.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "Service_Libs/etx/etx.h"
#include "Core/include/ns_address_internal.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"

#define TRACE_GROUP "thrd"

#define ID_MASK_UPDATE_MIN (10 * 10) /* 1 minute */
#define LEADER_DATA_UPDATE_MIN (10 * 10) /* 10 seconds */

/* Parent priority bits in Connectivity TLV flags byte */
#define CONNECTIVITY_PP_MASK     0xC0    // Parent priority
#define CONNECTIVITY_PP_LOW      0xC0
#define CONNECTIVITY_PP_MEDIUM   0x00
#define CONNECTIVITY_PP_HIGH     0x40
#define CONNECTIVITY_PP_INVALID  0x80

uint8_t thread_version = THREAD_PROTOCOL_VERSION;

thread_leader_data_t *thread_leader_data_generate(void);
thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info);
static uint8_t *thread_joining_port_tlv_write(uint16_t port, uint8_t *ptr);
static uint8_t *thread_commissioner_port_tlv_write(uint16_t port, uint8_t *ptr);
static void thread_tx_failure_handler(int8_t nwk_id, uint8_t accumulated_failures, uint8_t attribute_index);
static void thread_address_notification_cb(struct protocol_interface_info_entry *interface, const struct if_address_entry *addr, if_address_callback_t reason);

/* Helper functions*/
/* Ceil log2 function */
uint16_t thread_log2_aprx(uint32_t n)
{
    if (n == 0) {
        return 0;
    }
    uint16_t val = 0;
    --n;
    while (n > 0 && val < 255) {
        ++val;
        n >>= 1;
    }
    return val;
}



void thread_anycast_address_policy_update(const thread_info_t *thread_info, bool addPolicy)
{
    uint8_t ipv6_address[16] = {0};

    thread_addr_write_mesh_local_16(ipv6_address, 0xfc00, thread_info);

    if (addPolicy) {
        tr_debug("Thread Add AnycastAddress Policy");
        addr_policy_table_add_entry(ipv6_address, 118, 20, 40);
    } else {
        addr_policy_table_delete_entry(ipv6_address, 118);
    }
}

/**
 * Thread key request for MLE Message
 *
 */
uint8_t *thread_management_key_request_with_sequence(int8_t interface_id, uint8_t keyId, uint32_t keySequnce)
{
    protocol_interface_info_entry_t *cur;
    uint8_t *keyPtr = NULL;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return NULL;
    }
    //tr_debug("MLE key request by sequence id %"PRIu8" seq %"PRIu32, keyId, keySequnce);

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return NULL;
    }
    if (!cur->thread_info->masterSecretMaterial.valid_Info) {
        return NULL;
    }
    if (keySequnce == linkConfiguration->key_sequence) {
        if (mle_service_security_default_key_id_get(interface_id) == keyId) {
            keyPtr =  mle_service_security_default_key_get(interface_id);
        }
    } else if (keySequnce == (linkConfiguration->key_sequence + 1)) {
        if (mle_service_security_next_key_id_get(interface_id) == keyId) {
            keyPtr =  mle_service_security_next_key_get(interface_id);
        }
    }

    if (!keyPtr) {
        tr_debug("Gen temporary key id %"PRIu8" seq %"PRIu32, keyId, keySequnce);
        thread_key_get(linkConfiguration->master_key, cur->thread_info->masterSecretMaterial.historyKey, keySequnce);
        cur->thread_info->masterSecretMaterial.historyKeyId = keyId;
        cur->thread_info->masterSecretMaterial.historyKeyValid = false;
        keyPtr = cur->thread_info->masterSecretMaterial.historyKey;
    }
    return keyPtr;
}
uint8_t *thread_mle_service_security_notify_cb(int8_t interface_id, mle_security_event_t event, uint8_t keyId)
{
    (void)keyId;
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return NULL;
    }
    switch (event) {
        case MLE_SEC_MAX_FRAME_COUNTER_REACHED:

            break;
        case MLE_SEC_KEY_UPDATE_NOTIFY:

            mac_helper_security_key_swap_next_to_default(interface);

            if (thread_info(interface)) {
                thread_security_update_from_mac(interface);
            }
            break;

        case MLE_SEC_UNKNOWN_KEY:
            return NULL;
    }
    return NULL;
}
int8_t thread_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;

    if (!cur) {
        return -1;
    }

    if ((cur->configure_flags & INTERFACE_SETUP_MASK) != INTERFACE_SETUP_READY) {
        tr_debug("Interface not yet fully configured");
        return -5;
    }

    protocol_6lowpan_register_handlers(cur);
    addr_interface_set_ll64(cur, NULL);
    thread_interface_up(cur);
    ret_val = nwk_6lowpan_up(cur);

    cur->nwk_nd_re_scan_count = 0;
    cur->thread_info->link_sync_allowed = true;

    return ret_val;
}

int8_t thread_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    tr_debug("SET thread Idle");
    //stop polling
    mac_data_poll_disable(cur);
    // Reset MAC for safe upper layer memory free
    protocol_mac_reset(cur);
    //Clean mle table
    thread_neighbor_list_clean(cur);
    // store frame counters
    if (cur->thread_info) {
        thread_nvm_fast_data_t fast_data;
        memset(&fast_data, 0, sizeof(thread_nvm_fast_data_t));
        link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);
        if (linkConfiguration) {
            fast_data.seq_counter = linkConfiguration->key_sequence;
        }
        mac_helper_link_frame_counter_read(cur->id, &fast_data.mac_frame_counter);
        fast_data.mle_frame_counter = mle_service_security_get_frame_counter(cur->id);
        thread_nvm_store_fast_data_write(&fast_data);
        thread_joiner_application_configuration_nvm_save(cur->id);
        mac_pairwise_key_flush_list(cur->id);
        thread_discovery_reset(cur->id);
        thread_leader_mleid_rloc_map_to_nvm_write(cur);
        thread_bootstrap_stop(cur);
        mle_service_interface_unregister(cur->id);
        thread_diagnostic_delete(cur->id); // delete before thread_management_server_delete as they share same coap_service id
        thread_management_client_delete(cur->id); // delete before thread_management_server_delete as they share same coap_service id
        thread_nd_service_disable(cur->id); // delete before thread_management_server_delete as they share same coap_service id
        thread_management_server_delete(cur->id);
        thread_joiner_application_deinit(cur->id);
        //free network Data
        thread_network_data_free_and_clean(&cur->thread_info->networkDataStorage);
        //free local also here
        thread_network_local_data_free_and_clean(&cur->thread_info->localServerDataBase, cur->id);
        thread_network_data_base_init(&cur->thread_info->networkDataStorage);
        cur->thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
    }

    if (nd_proxy_downstream_interface_unregister(cur->id) == -1) {
        tr_warn("nd proxy unregister failed");
    }
    return nwk_6lowpan_down(cur);
}


bool thread_addr_is_mesh_local(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    thread_info_t *info = cur ? cur->thread_info : NULL;
    if (info && info->threadPrivatePrefixInfo.ulaValid) {
        return memcmp(info->threadPrivatePrefixInfo.ulaPrefix, addr, 8) == 0;
    }
    return false;
}

bool thread_on_mesh_route_possible_add(thread_attach_device_mode_e threadMode)
{
    bool addRoute;
    if ((threadMode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) || (threadMode == THREAD_DEVICE_MODE_END_DEVICE)) {
        addRoute = false;
    } else {
        addRoute = true;
    }

    return addRoute;
}

bool thread_addr_is_mesh_local_16(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return thread_addr_is_mesh_local(addr, cur) && memcmp(addr + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0;
}

uint8_t *thread_addr_write_mesh_local_16(uint8_t ip_addr_out[16], uint16_t addr16, const thread_info_t *info)
{
    if (!info->threadPrivatePrefixInfo.ulaValid) {
        return NULL;
    }
    memcpy(ip_addr_out + 8, ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(addr16, ip_addr_out + 14);
    return memcpy(ip_addr_out, info->threadPrivatePrefixInfo.ulaPrefix, 8);
}

bool thread_leader_data_parse(uint8_t *ptr, uint16_t dataLength, thread_leader_data_t *leaderDataBuf)
{
    mle_tlv_info_t mle_tlv_info;
    if (mle_tlv_option_discover(ptr, dataLength, MLE_TYPE_LEADER_DATA, &mle_tlv_info) == 8) {
        uint8_t *t_ptr = mle_tlv_info.dataPtr;
        leaderDataBuf->partitionId = common_read_32_bit(t_ptr);
        t_ptr += 4;
        leaderDataBuf->weighting = *t_ptr++;
        leaderDataBuf->dataVersion = *t_ptr++;
        leaderDataBuf->stableDataVersion = *t_ptr++;
        leaderDataBuf->leaderRouterId = *t_ptr;
        return true;
    }
    return false;
}

bool thread_connectivity_tlv_parse(uint8_t *ptr, uint16_t dataLength, thread_connectivity_t *connectivityTlv)
{
    mle_tlv_info_t mle_tlv_info;
    int tlv_length = mle_tlv_option_discover(ptr, dataLength, MLE_TYPE_CONNECTIVITY, &mle_tlv_info);
    if (tlv_length >= 7) {
        uint8_t *t_ptr = mle_tlv_info.dataPtr;
        uint8_t flags = *t_ptr++;
        switch (flags & CONNECTIVITY_PP_MASK)  {
            case CONNECTIVITY_PP_LOW:
                connectivityTlv->parentPriority = THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_LOW;
                break;
            case CONNECTIVITY_PP_HIGH:
                connectivityTlv->parentPriority = THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_HIGH;
                break;
            case CONNECTIVITY_PP_MEDIUM:
            default:
                connectivityTlv->parentPriority = THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_MEDIUM;
                break;
        }
        connectivityTlv->linkQuality3 = *t_ptr++;
        connectivityTlv->linkQuality2 = *t_ptr++;
        connectivityTlv->linkQuality1 = *t_ptr++;
        connectivityTlv->leaderCost = *t_ptr++;
        connectivityTlv->idSequence = *t_ptr++;
        connectivityTlv->activeRouters = *t_ptr++;
        if (tlv_length >= 10) {
            connectivityTlv->SEDBufferSize = common_read_16_bit(t_ptr);
            t_ptr += 2;
            connectivityTlv->SEDDatagramCount = *t_ptr++;
        } else {
            connectivityTlv->SEDBufferSize = THREAD_SED_BUFFER_MIN_SIZE;
            connectivityTlv->SEDDatagramCount = THREAD_SED_DATAGRAM_MIN_COUNT;
        }
        return true;
    }
    return false;
}

void thread_key_guard_timer_calculate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, bool is_init)
{
    uint32_t key_rotation = linkConfiguration ? linkConfiguration->key_rotation : 0;

    if (is_init && key_rotation < 1) {
        tr_warn("Attempted to set key rotation time smaller than 1 hour.");
        key_rotation = 1;
    }

    cur->thread_info->masterSecretMaterial.keyRotation = key_rotation * 3600; // setting value is hours converting to seconds
    cur->thread_info->masterSecretMaterial.keySwitchGuardTimer = is_init ? 0 : (key_rotation * 3600 * 93 / 100);
}

void thread_key_guard_timer_reset(protocol_interface_info_entry_t *cur)
{
    cur->thread_info->masterSecretMaterial.keySwitchGuardTimer = 0;
}

thread_leader_data_t *thread_leader_data_generate(void)
{
    thread_leader_data_t *leader_data;
    leader_data = ns_dyn_mem_alloc(sizeof(thread_leader_data_t));
    if (leader_data) {
        memset(leader_data, 0, sizeof(thread_leader_data_t));
    }
    return leader_data;
}

thread_leader_data_t *thread_leader_data_get(thread_info_t *info)
{
    if (info->thread_leader_data == NULL) {
        info->thread_leader_data = thread_leader_data_generate();
    }
    return info->thread_leader_data;
}

thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info)
{
    thread_parent_info_t *parent_data;
    if (!info->thread_endnode_parent) {
        info->thread_endnode_parent = ns_dyn_mem_alloc(sizeof(thread_parent_info_t));
    }

    parent_data = info->thread_endnode_parent;
    if (parent_data) {
        memset(parent_data, 0, sizeof(thread_parent_info_t));
    }
    return parent_data;
}

void thread_dynamic_reed_initialize(thread_router_select_t *routerSelect)
{
    routerSelect->jitterTimerActive = false;
    routerSelect->possibleDefaultParent = 0xff;
    routerSelect->routerDowngradeThresHold = ROUTER_DOWNGRADE_THRESHOLD;
    routerSelect->routerUpgradeThresHold = ROUTER_UPGRADE_THRESHOLD;
    routerSelect->reedAdvertisementInterval = REED_ADVERTISEMENT_INTERVAL;
    routerSelect->reedAdvertisementJitterInterval = REED_ADVERTISEMENT_MAX_JITTER;
    routerSelect->reedAdvertisementTimeout = NULL;
}

bool thread_leader_commissioner_create(thread_info_t *thread_info)
{
    thread_info->registered_commissioner.commissioner_valid = false;
    thread_info->registered_commissioner.session_id = randLIB_get_16bit();
    ns_dyn_mem_free(thread_info->registered_commissioner.commissioner_id_ptr);
    thread_info->registered_commissioner.commissioner_id_ptr = NULL;
    eventOS_timeout_cancel(thread_info->registered_commissioner.commissioner_timeout);
    thread_info->registered_commissioner.commissioner_timeout = NULL;
    thread_info->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_NOT_REGISTERED;
    thread_info->registered_commissioner.steering_data_len = 0;
    return true;
}


void thread_data_base_init(thread_info_t *thread_info, int8_t interfaceId)
{
    if (!thread_info) {
        return;
    }
    (void) interfaceId;

    thread_leader_commissioner_create(thread_info);
    thread_info->rfc6775 = false;
    thread_info->threadPrivatePrefixInfo.ulaValid = false;
    thread_info->routerIdRequested = false;
    thread_info->networkDataRequested = false;
    thread_info->proactive_an_timer = 0;

    thread_info->thread_device_mode = THREAD_DEVICE_MODE_END_DEVICE;
    thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
    thread_routing_reset(&thread_info->routing);
}

int thread_info_allocate_and_init(protocol_interface_info_entry_t *cur)
{
    if (thread_bootstrap_tasklet_init(cur) != 0) {
        return -1;
    }

    if (!cur->thread_info) {
        cur->thread_info = ns_dyn_mem_alloc(sizeof(thread_info_t));
        if (!cur->thread_info) {
            return -1;
        }
        memset(cur->thread_info, 0, sizeof(thread_info_t));

        cur->thread_info->interface_id = cur->id;
        cur->thread_info->testMaxActiveRouterIdLimit = 32;
        cur->thread_info->version = thread_version; // Default implementation version
        cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_END_DEVICE;
        cur->thread_info->childUpdateReqTimer = -1;
        cur->thread_info->parent_priority = CONNECTIVITY_PP_INVALID; // default invalid - calculated using child count

        thread_maintenance_timer_set(cur);
        thread_routing_init(&cur->thread_info->routing);
        thread_network_local_server_data_base_init(&cur->thread_info->localServerDataBase);
        memset(&cur->thread_info->registered_commissioner, 0, sizeof(thread_commissioner_t));
        thread_dynamic_reed_initialize(&cur->thread_info->routerSelectParameters);
        thread_common_ccm_allocate(cur);
        ns_list_init(&cur->thread_info->childIdReqPending);
        ns_list_init(&cur->thread_info->child_mcast_list);
        if (!thread_leader_data_get(cur->thread_info)) {
            return -1;
        }
    } else {
        thread_network_data_free_and_clean(&cur->thread_info->networkDataStorage);
        thread_network_local_data_free_and_clean(&cur->thread_info->localServerDataBase, cur->id);
        thread_leader_service_leader_data_free(cur->thread_info);
        thread_data_base_init(cur->thread_info, cur->id);
        thread_dynamic_reed_initialize(&cur->thread_info->routerSelectParameters);
    }
    thread_network_data_base_init(&cur->thread_info->networkDataStorage);
    //SET Thread Bootstrap Up & Down
    cur->if_up = thread_bootstrap_up;
    cur->if_down = thread_bootstrap_down;
    cur->mac_parameters->beacon_ind = thread_beacon_indication;
    cur->mac_parameters->mac_in_direct_entry_timeout = 30000;
    cur->thread_info->routerShortAddress = 0xfffe; //Default value Not Router
    //memset(cur->thread_info->lastValidRouteMask, 0, (N_THREAD_ROUTERS+7)/8);
    cur->thread_info->releaseRouterId = false;

    // Test data
    cur->thread_info->testRandomPartitionId = 0;

    return 0;
}

void thread_info_deallocate(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info) {
        //Release DHCPv6 leasequery Service
        thread_nd_service_delete(cur->id);
        thread_network_data_free_and_clean(&cur->thread_info->networkDataStorage);
        thread_network_local_data_free_and_clean(&cur->thread_info->localServerDataBase, cur->id);
        thread_leader_service_leader_data_free(cur->thread_info);
        thread_data_base_init(cur->thread_info, cur->id);
        thread_routing_free(&cur->thread_info->routing);
        thread_common_ccm_free(cur);
        thread_ccm_free(cur);
        if (cur->thread_info->thread_endnode_parent) {
            ns_dyn_mem_free(cur->thread_info->thread_endnode_parent);
            cur->thread_info->thread_endnode_parent = NULL;
        }
        ns_dyn_mem_free(cur->thread_info);
        cur->thread_info = 0;
        cur->mesh_callbacks = NULL;
    }
}

thread_leader_info_t *thread_allocate_and_init_leader_private_data(void)
{
    thread_leader_info_t *leader_info = ns_dyn_mem_alloc(sizeof(thread_leader_info_t));
    if (leader_info) {
        leader_info->leader_id_seq_timer = ID_SEQUENCE_PERIOD;
        leader_info->leader_nvm_sync_timer = 0;
    }
    return leader_info;
}

thread_route_cost_t thread_link_quality_to_cost(thread_link_quality_e quality)
{
    switch (quality) {
        case QUALITY_20dB:
            return 1;
        case QUALITY_10dB:
            return 2;
        case QUALITY_2dB:
            return 4;
        default:
        case QUALITY_BAD:
            return THREAD_COST_INFINITE;
    }
}

thread_route_cost_t thread_link_cost_sum(thread_route_cost_t a, thread_route_cost_t b)
{
    if (a == THREAD_COST_INFINITE || b == THREAD_COST_INFINITE) {
        return THREAD_COST_INFINITE;
    }

    if (a + b > THREAD_MAX_ROUTE_COST) {
        return THREAD_COST_INFINITE;
    }

    return a + b;
}

thread_link_quality_e thread_link_margin_to_quality(thread_link_margin_t margin)
{
    if (margin > (20 << THREAD_LINK_MARGIN_SCALING)) {
        return QUALITY_20dB;
    } else if (margin > (10 << THREAD_LINK_MARGIN_SCALING)) {
        return QUALITY_10dB;
    } else if (margin > (2 << THREAD_LINK_MARGIN_SCALING)) {
        return QUALITY_2dB;
    } else {
        return QUALITY_BAD;
    }
}

uint_fast8_t thread_sum_rx_path_cost_and_link_cost(uint8_t inMargim, uint8_t outMargin, uint8_t pathCost)
{
    thread_route_cost_t linkCost, rxCost;
    if (inMargim < outMargin) {
        linkCost = thread_link_quality_to_cost(thread_link_margin_to_quality(inMargim));
    } else {
        linkCost = thread_link_quality_to_cost(thread_link_margin_to_quality(outMargin));
    }
    rxCost = pathCost;

    return thread_link_cost_sum(linkCost, rxCost);
}

thread_registered_mcast_addr_t *thread_registered_mcast_addr_entry_allocate(void)
{
    thread_registered_mcast_addr_t *addr = ns_dyn_mem_alloc(sizeof(thread_registered_mcast_addr_t));

    if (addr) {
        memset(addr, 0, sizeof(thread_registered_mcast_addr_t));
        ns_list_init(&addr->children);
    }

    return addr;
}

thread_mcast_child_t *thread_mcast_addr_child_entry_allocate(void)
{
    thread_mcast_child_t *child = ns_dyn_mem_alloc(sizeof(thread_mcast_child_t));

    if (child) {
        memset(child, 0, sizeof(thread_mcast_child_t));
    }

    return child;
}

void thread_registered_mcast_addr_entry_clean(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach_safe(thread_registered_mcast_addr_t, entry, &cur->thread_info->child_mcast_list) {
        ns_list_remove(&cur->thread_info->child_mcast_list, entry);

        ns_list_foreach_safe(thread_mcast_child_t, child, &entry->children) {
            ns_list_remove(&entry->children, child);
            ns_dyn_mem_free(child);
        }

        ns_dyn_mem_free(entry);
    }
}

void thread_child_mcast_entries_remove(protocol_interface_info_entry_t *cur, const uint8_t *mac64)
{
    ns_list_foreach_safe(thread_registered_mcast_addr_t, entry, &cur->thread_info->child_mcast_list) {
        ns_list_foreach_safe(thread_mcast_child_t, child, &entry->children) {
            if (memcmp(child->mac64, mac64, 8) == 0) {
                ns_list_remove(&entry->children, child);
                ns_dyn_mem_free(child);
            }
        }
    }

    // Remove empty multicast address entries
    ns_list_foreach_safe(thread_registered_mcast_addr_t, entry, &cur->thread_info->child_mcast_list) {
        if (ns_list_is_empty(&entry->children)) {
            ns_list_remove(&cur->thread_info->child_mcast_list, entry);
            ns_dyn_mem_free(entry);
        }
    }
}

thread_registered_mcast_addr_t *thread_registered_mcast_addr_entry_find(protocol_interface_info_entry_t *cur, const uint8_t *mcast_addr)
{
    ns_list_foreach(thread_registered_mcast_addr_t, entry, &cur->thread_info->child_mcast_list) {
        if (memcmp(entry->address, mcast_addr, 16) == 0) {
            return entry;
        }
    }

    return NULL;
}

thread_mcast_child_t *thread_child_mcast_entry_find(thread_mcast_children_list_t *children, const uint8_t *mac64)
{
    ns_list_foreach(thread_mcast_child_t, entry, children) {
        if (memcmp(entry->mac64, mac64, 8) == 0) {
            return entry;
        }
    }

    return NULL;
}

bool thread_stable_context_check(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur), buf->dst_sa.address + 2, buf->dst_sa.addr_type);
    if (entry && thread_addr_is_child(mac_helper_mac16_address_get(cur), entry->mac16)) {

        /* Check if the child can handle only stable network data (e.g. sleepy device) */
        return !(thread_neighbor_class_request_full_data_setup(&cur->thread_info->neighbor_class, entry->index));
    }
    return false;
}

thread_mcast_child_t *thread_child_mcast_entry_get(protocol_interface_info_entry_t *cur, const uint8_t *mcast_addr, const uint8_t *mac64)
{
    mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur), mac64, ADDR_802_15_4_LONG);

    if (!entry) {
        tr_error("No MLE entry.");
        return NULL;
    }

    if (entry->rx_on_idle) {
        /* Not a sleepy child */
        tr_debug("Not a sleepy child");
        return NULL;
    }

    thread_registered_mcast_addr_t *addr = thread_registered_mcast_addr_entry_find(cur, mcast_addr);

    if (!addr) {
        addr = thread_registered_mcast_addr_entry_allocate();

        if (addr) {
            memcpy(addr->address, mcast_addr, 16);
            ns_list_add_to_end(&cur->thread_info->child_mcast_list, addr);
        } else {
            return NULL;
        }
    }

    thread_mcast_child_t *child = thread_child_mcast_entry_find(&addr->children, mac64);

    if (!child) {
        child = thread_mcast_addr_child_entry_allocate();

        if (child) {
            memcpy(child->mac64, mac64, 8);
            ns_list_add_to_end(&addr->children, child);
        } else {
            return NULL;
        }
    }

    return child;
}

void thread_child_id_request_info_init(thread_pending_child_id_req_t *child_info)
{
    if (child_info) {
        child_info->routeReq = false;
        child_info->networkDataReq = false;
        child_info->shortAddressReq = false;
    }
}

thread_pending_child_id_req_t *thread_child_id_request_allocate(void)
{
    thread_pending_child_id_req_t *req = ns_dyn_mem_alloc(sizeof(thread_pending_child_id_req_t));
    memset(req->eiid, 0, 8);
    thread_child_id_request_info_init(req);
    return req;
}

void thread_child_id_request_entry_clean(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach_safe(thread_pending_child_id_req_t, entry, &cur->thread_info->childIdReqPending) {
        ns_list_remove(&cur->thread_info->childIdReqPending, entry);
        ns_dyn_mem_free(entry);
    }
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get(protocol_interface_info_entry_t *cur, uint8_t *euid64)
{
    thread_pending_child_id_req_t *req;
    ns_list_foreach(thread_pending_child_id_req_t, entry, &cur->thread_info->childIdReqPending) {
        if (memcmp(entry->euid64, euid64, 8) == 0) {
            thread_child_id_request_info_init(entry);
            return entry;
        }
    }
    req = thread_child_id_request_allocate();
    if (req) {
        tr_debug("Add to list ID REQ");
        memcpy(req->euid64, euid64, 8);
        ns_list_add_to_end(&cur->thread_info->childIdReqPending, req);
    }
    return req;
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get_from_the_list(protocol_interface_info_entry_t *cur)
{
    thread_pending_child_id_req_t *req;

    req = ns_list_get_first(&cur->thread_info->childIdReqPending);
    if (req) {
        ns_list_remove(&cur->thread_info->childIdReqPending, req);
    }

    return req;

}

void thread_child_id_request_entry_remove(protocol_interface_info_entry_t *cur, thread_pending_child_id_req_t *entry)
{
    if (entry) {
        ns_list_remove(&cur->thread_info->childIdReqPending, entry);
        ns_dyn_mem_free(entry);
    }
}

int thread_init(protocol_interface_info_entry_t *cur)
{
    if (!cur->thread_info) {
        return -1;
    }

    if (thread_mle_class_init(cur->id) != 0) {
        return -1;
    }

    // set mle security - first allocate instance and then set security
    mle_service_security_instance_allocate(cur->id);
    if (mle_service_security_init(cur->id, 5,
                                  0,
                                  thread_management_key_request_with_sequence,
                                  thread_mle_service_security_notify_cb) != 0) {
        tr_error("Mle Service security init Fail");
        return -1;
    }

    if (etx_accum_failures_callback_register(cur->nwk_id, cur->id, 1, thread_tx_failure_handler) != 1) {
        return -1;
    }

    addr_notification_register(thread_address_notification_cb);

    thread_leader_service_leader_data_free(cur->thread_info);
    thread_data_base_init(cur->thread_info, cur->id);
    mac_helper_pib_boolean_set(cur, macThreadForceLongAddressForBeacon, true);
    mac_helper_mac16_address_set(cur, 0xffff);
    return 0;
}

int thread_attach_ready(protocol_interface_info_entry_t *cur)
{
    if (!cur->thread_info) {
        return -1;
    }

    switch (cur->thread_info->thread_attached_state) {
        case THREAD_STATE_CONNECTED:
        case THREAD_STATE_CONNECTED_ROUTER:
            return 0;
        /* break; */
        default:
            break;
    }

    return -1;
}

bool thread_attach_active_router(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        return true;
    }

    return false;
}

bool thread_scan_mask_validation(protocol_interface_info_entry_t *cur, uint8_t mask)
{
    uint8_t maskCompare = 0;

    if (cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER) {
        return false;
    }

    switch (cur->thread_info->thread_attached_state) {
        case THREAD_STATE_CONNECTED_ROUTER:
            maskCompare  = 0x80;
            break;
        case THREAD_STATE_CONNECTED:
            maskCompare = 0x40;
            break;
        default:
            maskCompare = 0;
            break;
    }
    return (mask & maskCompare);
}

int thread_route_ready_to_leader(protocol_interface_info_entry_t *cur)
{
    int retVal = -1;
    switch (cur->thread_info->thread_attached_state) {
        case THREAD_STATE_CONNECTED_ROUTER:
            if (cur->thread_info->leader_private_data) {
                //We are the leader
                retVal = 0;
            } else if (thread_routing_cost_get_by_router_id(&cur->thread_info->routing, cur->thread_info->thread_leader_data->leaderRouterId) != 0) {
                retVal = 0;
            }
            break;
        case THREAD_STATE_CONNECTED:
            if (cur->thread_info->thread_endnode_parent) {
                retVal = 0;
            }
            break;
        default:
            break;
    }
    return retVal;
}
static void thread_child_update_req_timer(protocol_interface_info_entry_t *cur, uint16_t seconds)
{
    if (cur->thread_info->childUpdateReqTimer == -1) {
        return;
    }

    if (cur->thread_info->childUpdateReqTimer > seconds) {
        cur->thread_info->childUpdateReqTimer -= seconds;
    } else {
        cur->thread_info->childUpdateReqTimer = 0;
    }

    if (cur->thread_info->childUpdateReqTimer == 0) {
        thread_bootstrap_child_update_trig(cur);
        cur->thread_info->childUpdateReqTimer = -1; // disable
    }
}

static void thread_key_switch_timer(protocol_interface_info_entry_t *cur, uint16_t seconds)
{
    if (cur->thread_info->masterSecretMaterial.keySwitchGuardTimer > seconds) {
        cur->thread_info->masterSecretMaterial.keySwitchGuardTimer -= seconds;
    } else {
        cur->thread_info->masterSecretMaterial.keySwitchGuardTimer = 0;
    }

    if (cur->thread_info->masterSecretMaterial.keyRotation > seconds) {
        cur->thread_info->masterSecretMaterial.keyRotation -= seconds;
    } else {
        cur->thread_info->masterSecretMaterial.keyRotation = 0;
    }

    if (cur->thread_info->masterSecretMaterial.keyRotation == 0) {
        link_configuration_s *linkConfiguration;
        linkConfiguration = thread_joiner_application_get_config(cur->id);

        if (!linkConfiguration) {
            return;
        }

        tr_debug("thrKeyRotation == 0: sync key material by %"PRIu32, linkConfiguration->key_sequence + 1);
        thread_management_key_sets_calc(cur, linkConfiguration, linkConfiguration->key_sequence + 1);
        thread_key_guard_timer_calculate(cur, linkConfiguration, false);
    }
}

static void thread_maintenance_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    if (thread_info(cur)->thread_maintenance_timer) {
        if (thread_info(cur)->thread_maintenance_timer > seconds) {
            thread_info(cur)->thread_maintenance_timer -= seconds;
            return;
        }
    }

    thread_maintenance_timer_set(cur);

    tr_debug("NWK data maintenance scan");

    thread_bootstrap_network_data_activate(cur);
}

void thread_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
    uint8_t leader_address[16];
    uint8_t commissioner_address[16];
    uint16_t commissioner_port;
    thread_info_t *thread_info = cur->thread_info;

    if (!thread_info) {
        return;
    }

    blacklist_ttl_update(ticks);

    // if we have pending configurations timer is made for it.
    thread_joiner_application_seconds_timer(cur->id, ticks);
    thread_resolution_client_timer(cur->id, ticks);
    thread_key_switch_timer(cur, ticks);
    thread_child_update_req_timer(cur, ticks);
    thread_bootstrap_address_registration_timer(cur, ticks);

    if (thread_attach_ready(cur) != 0) {
        return;
    }
    // Store all Dynamic information periodically. Now saved every 1s
    thread_bootstrap_dynamic_configuration_save(cur);

    // TODO should this be somewhere else? in joiner application?
    if (thread_joiner_application_next_active_config_exists(cur->id)) {
        thread_management_get_leader_address(cur->id, leader_address);
        thread_management_client_active_set(cur->id, leader_address);
        thread_joiner_application_next_active_config_delete(cur->id);
    }

    if (thread_joiner_application_next_pending_config_exists(cur->id)) {
        thread_management_get_leader_address(cur->id, leader_address);
        thread_management_client_pending_set(cur->id, leader_address);
        thread_joiner_application_next_pending_config_delete(cur->id);
    }

    // Check if we need to make application provisioning
    if (PROVISIONING_STATUS_NOT_DONE == thread_joiner_application_provisioning_get(cur->id) &&
            thread_management_get_commissioner_address(cur->id, commissioner_address, &commissioner_port) == 0) {
        // Provisioning not done and commissioner is present
        thread_management_client_provision_request(cur->id,  commissioner_address, commissioner_port);
    }


    // add more checks here when to become router
    // If we are doing attach to new partition, do not upgrade
    if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY) {
        return;
    }

    thread_router_bootstrap_timer(cur, ticks);
    thread_maintenance_timer(cur, ticks);
    thread_border_router_seconds_timer(cur->id, ticks);
    thread_bbr_seconds_timer(cur->id, ticks);
    thread_lowpower_timer(cur, ticks);
    thread_nvm_store_seconds_timer(ticks);

    if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {
        nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);
    }
}

void thread_network_data_request_send(protocol_interface_info_entry_t *cur, uint8_t *requestDstAddress, bool delaydTrig)
{
    thread_info_t *thread_info = cur->thread_info;
    uint8_t req_tlv = MLE_TYPE_NETWORK_DATA;

    if (!cur->thread_info) {
        return;
    }

    tr_debug("Send MLE network data request");

    if (cur->thread_info->networkDataRequested) {
        tr_debug("Pending data request found");
        return;
    }

    cur->thread_info->networkDataRequested = true;

    thread_tlv_request(thread_info->interface_id, requestDstAddress, delaydTrig, &req_tlv, 1);
}

void thread_timer(protocol_interface_info_entry_t *cur, uint8_t ticks)
{
    thread_info_t *thread_info = cur->thread_info;
    if (!thread_info) {
        return;
    }

    if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY) {
        /* Own attach is ongoing, do not send advertisements */
        return;
    }

    if (thread_i_am_router(cur)) {
        if (thread_routing_timer(thread_info, ticks)) {
            thread_router_bootstrap_mle_advertise(cur);
        }
    }
}

/* This test is used to indicate that we're following the Thread 1.0 oddities
 * arising from not having LL16 addresses on the mesh. For example, LL16
 * addresses go through the MAC and only one radio hop, and UL16/GP16 addresses
 * have special semantics on the PAN.
 */
bool thread_insist_that_mesh_isnt_a_link(const protocol_interface_info_entry_t *cur)
{
    return cur->thread_info && !cur->thread_info->rfc6775;
}

int8_t thread_beacon_create_payload(struct protocol_interface_info_entry *cur)
{
    uint8_t *ptr;
    uint8_t payload_len;
    link_configuration_s *leader_link_setup;
    thread_management_server_data_t server_data;

    leader_link_setup = thread_joiner_application_get_config(cur->id);

    if (!leader_link_setup) {
        return -1;
    }

    if (!(leader_link_setup->securityPolicy & SECURITY_POLICY_BEACON_PAYLOAD_ENABLED)) {
        mac_helper_beacon_payload_reallocate(cur, 0);
        return mac_helper_beacon_payload_register(cur);
    }
    if (thread_management_server_commisoner_data_get(cur->id, &server_data) != 0) {
        return mac_helper_beacon_payload_register(cur);
    }

    payload_len = 2/*header*/ + 16/*Network id*/ + 8/*extented PANID*/;

    if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len) {
        payload_len += cur->thread_info->registered_commissioner.steering_data_len + 2;
    }

    if (server_data.joiner_router_enabled) {
        payload_len +=  4/*Joiner UDP port*/;
    }
    if ((leader_link_setup->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED)) {
        payload_len +=  4/*Commissioner UDP port*/;
    }

    ptr = mac_helper_beacon_payload_reallocate(cur, payload_len);
    if (!ptr) {
        return -1;
    }

    *ptr++ = THREAD_BEACON_PROTOCOL_ID;
    *ptr = THREAD_BEACON_PROTOCOL_VERSION << THREAD_BEACON_VERSION_SHIFT;

    if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len > 0) {
        *ptr |= THREAD_BEACON_JOINING_PERMITTED_BIT;    // permit join bit set on
    }
    if ((leader_link_setup->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED)) {
        *ptr |= THREAD_BEACON_NATIVE_COMMISSIONER_BIT;
    }
    ptr++;
    memcpy(ptr, leader_link_setup->name, 16);
    ptr += 16;
    memcpy(ptr, leader_link_setup->extented_pan_id, 8);
    ptr += 8;

    if (server_data.joiner_router_enabled) {
        /* MESHCOP_TLV_JOINER_UDP_PORT */
        ptr = thread_joining_port_tlv_write(server_data.joiner_router_port, ptr);
    }
    if ((leader_link_setup->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED)) {
        /* MESHCOP_TLV_COMMISSIONER_UDP_PORT */
        ptr = thread_commissioner_port_tlv_write(server_data.commissioner_port, ptr);
    }


    if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len > 0) {
        ptr = thread_nd_commission_data_write_steering_data(ptr, cur->thread_info->registered_commissioner.steering_data, cur->thread_info->registered_commissioner.steering_data_len);
    }

    return mac_helper_beacon_payload_register(cur);
}

uint8_t thread_beacon_indication(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    (void)ptr;
    (void)len;
    (void)cur;
    return 1;
}

static uint8_t *thread_linkquality_write(int8_t interface_id, uint8_t *buffer)
{
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr && !interface_ptr->thread_info) {
        return buffer;
    }

    uint8_t lqi1 = 0, lqi2 = 0, lqi3 = 0;
    thread_link_quality_e thread_link_quality;
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(interface_ptr)->neighbour_list;

    ns_list_foreach(mac_neighbor_table_entry_t, cur, mac_table_list) {
        if (thread_is_router_addr(cur->mac16)) {
            // Only count routers to link quality
            uint16_t link_margin = thread_neighbor_entry_linkmargin_get(&interface_ptr->thread_info->neighbor_class, cur->index);
            thread_link_quality = thread_link_margin_to_quality(link_margin);
            switch (thread_link_quality) {
                case QUALITY_20dB:
                    lqi3++;
                    break;
                case QUALITY_10dB:
                    lqi2++;
                    break;
                case QUALITY_2dB:
                    lqi1++;
                    break;
                case QUALITY_BAD:
                    break;

            }
        }
    }

    *buffer++ = lqi3;
    *buffer++ = lqi2;
    *buffer++ = lqi1;
    return buffer;
}

uint8_t thread_route_option_size(protocol_interface_info_entry_t *cur)
{
    return 2 + MLE_ROUTE_MIN_OPTION_LEN + thread_routing_get_route_data_size(cur);

}
uint8_t *thread_route_option_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    uint8_t *len_ptr;
    uint8_t *saved_ptr;

    saved_ptr = ptr;
    *ptr++ = MLE_TYPE_ROUTE;
    len_ptr = ptr++;
    /* ptr now points to ID sequence */

    /* Can write the data straight into the buffer */
    if (thread_routing_get_route_data(cur,
                                      ptr, /* ptr to ID sequence (1 byte) */
                                      ptr + 1, /* ptr to ID mask (MLE_ROUTE_ID_MASK_SIZE bytes) */
                                      ptr + MLE_ROUTE_MIN_OPTION_LEN, /* ptr to router table data */
                                      len_ptr) != 0) { /* ptr to length */
        /* 0 -> SUCCESS */
        /* Point to beginning of buffer again */
        ptr = saved_ptr;
    } else {
        /* Function sets length to router table length - adjust for ID sequence and ID mask length */
        *len_ptr += MLE_ROUTE_MIN_OPTION_LEN;
        /* Advance buffer pointer past value field */
        ptr += *len_ptr;
    }

    return ptr;
}

uint8_t *thread_connectivity_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur, uint8_t mode)
{
    thread_info_t *thread = thread_info(cur);
    //Set Connectivity
    *ptr++ = MLE_TYPE_CONNECTIVITY;
    *ptr++ = 10;

    // determine parent priority
    if ((thread->parent_priority & CONNECTIVITY_PP_MASK) != CONNECTIVITY_PP_INVALID) {
        *ptr++ = thread->parent_priority & CONNECTIVITY_PP_MASK;
    } else if ((mode & MLE_DEV_MASK) == MLE_RFD_DEV && (3 * mle_class_rfd_entry_count_get(cur) > 2 * THREAD_MAX_MTD_CHILDREN)) {
        *ptr++ = CONNECTIVITY_PP_LOW;
    } else if (!(mode & MLE_RX_ON_IDLE) && (3 * mle_class_sleepy_entry_count_get(cur) > 2 * THREAD_MAX_SED_CHILDREN)) {
        *ptr++ = CONNECTIVITY_PP_LOW;
    } else if (3 * thread_router_bootstrap_child_count_get(cur) > 2 * thread->maxChildCount) {
        // 1/3 of the child capacity remaining, PP=low
        *ptr++ = CONNECTIVITY_PP_LOW;
    } else if (mle_class_free_entry_count_get(cur) < THREAD_FREE_MLE_ENTRY_THRESHOLD) {
        // If only few entries available in the MLE table, change priority to low
        *ptr++ = CONNECTIVITY_PP_LOW;
    } else {
        *ptr++ = CONNECTIVITY_PP_MEDIUM;
    }

    ptr = thread_linkquality_write(cur->id, ptr);

    // Route Cost To leader
    if (thread->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        // Leader cost
        *ptr++ = thread_routing_cost_get_by_router_id(&thread->routing, thread->thread_leader_data->leaderRouterId);
        //Router ID sequence
        *ptr++ = thread->routing.router_id_sequence;
    } else {
        // Leader cost when attached as child (REED,FED,SED,MED)
        *ptr++ = thread->thread_endnode_parent->pathCostToLeader;
        //Router ID sequence
        if (thread->routing.router_id_sequence_valid) {
            *ptr++ = thread->routing.router_id_sequence;
        } else {// We dont know the correct value
            *ptr++ = 0;
        }
    }

    // active Routers
    uint8_t activeRouters = 0;
    if (cur->thread_info->leader_private_data) {
        activeRouters = thread_routing_count_active_routers_from_mask(cur->thread_info->leader_private_data->master_router_id_mask);
    } else if (thread->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        activeRouters = thread_routing_count_active_routers(&thread->routing);
    }
    *ptr++ = activeRouters;

    //Set SED Buffer size and datagram count
    ptr = common_write_16_bit(THREAD_SED_BUFFER_SIZE, ptr);
    *ptr++ = THREAD_SED_DATAGRAM_COUNT;

    return ptr;
}

uint16_t thread_network_data_tlv_length(protocol_interface_info_entry_t *cur, bool fullist)
{
    if (fullist) {
        return cur->thread_info->networkDataStorage.network_data_len;
    }

    return thread_network_data_generate_stable_set(cur, NULL);
}

uint16_t thread_network_data_generate_stable_set(protocol_interface_info_entry_t *cur, uint8_t *result_ptr)
{
    uint8_t *dptr;
    uint16_t network_data_len = 0;
    uint8_t length;
    uint8_t type;

    dptr = cur->thread_info->networkDataStorage.network_data;
    uint16_t network_data_length = cur->thread_info->networkDataStorage.network_data_len;

    tr_debug("Generating stable set from network data of size %d",
             cur->thread_info->networkDataStorage.network_data_len);

    while (network_data_length) {
        if (network_data_length >= 2) {
            type = *dptr++;
            length = *dptr++;

            if (length == 0) {
                // 0 is not valid length for TLV
                return 0;
            }

            if (!(type & THREAD_NWK_STABLE_DATA)) {
                // Skip this TLV altogether...
                network_data_length -= 2 + length;
                dptr += length;
                continue;
            }

            type &= THREAD_NWK_DATA_TYPE_MASK;
            network_data_length -= 2;

            if (network_data_length < length) {
                tr_error("Length error");
                return 0;
            }

            // Set length ready for next check
            network_data_length -= length;

            if (type == THREAD_NWK_DATA_TYPE_PREFIX) {
                uint8_t *length_ptr = NULL;
                length -= 2;

                uint8_t prefix_bytes_len = prefixBits_to_bytes(*(dptr + 1));

                if (prefix_bytes_len > length) {
                    return 0;
                }

                length -= prefix_bytes_len;

                if (result_ptr) {
                    *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                    length_ptr = result_ptr;
                    result_ptr++;
                    memcpy(result_ptr, dptr, 2 + prefix_bytes_len);
                    result_ptr += 2 + prefix_bytes_len;
                }
                dptr += 2 + prefix_bytes_len;

                network_data_len += 2 + 2 + prefix_bytes_len;
                uint16_t total_tlv_length = 2 + prefix_bytes_len;

                uint8_t context_id = thread_border_router_prefix_context_id(dptr, length);
                tr_debug("Resolved Context ID: %d", context_id);

                while (length > 2) {
                    type = *dptr++;
                    uint8_t subLength = *dptr++;
                    thread_border_router_tlv_entry_t genericService;

                    if (!(type & THREAD_NWK_STABLE_DATA)) {
                        // Skip this sub-TLV altogether...
                        dptr += subLength;
                        length -= 2 + subLength;
                        continue;
                    }

                    type &= THREAD_NWK_DATA_TYPE_MASK;
                    length -= 2;

                    tr_debug("SubType: %02x, %s", type, trace_array(dptr, subLength));
                    total_tlv_length += 2 + subLength;
                    network_data_len += 2 + subLength;
                    tr_debug("Total TLV length: %d", total_tlv_length);
                    if (subLength <= length) {
                        length -= subLength;
                        if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                            if (result_ptr) {
                                *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                                *result_ptr++ = subLength;
                            }
                            while (subLength) {
                                dptr += 2;
                                uint16_t flags = common_read_16_bit(dptr);
                                dptr += 2;
                                subLength -= THREAD_BORDER_ROUTER_TLV_LENGTH;
                                genericService.P_dhcp = ((flags >> THREAD_P_DHCP_BIT_MOVE) & 1);
                                if (result_ptr) {
                                    if (genericService.P_dhcp) {
                                        result_ptr = common_write_16_bit(0xfc00 | context_id, result_ptr);
                                    } else {
                                        result_ptr = common_write_16_bit(0xfffe, result_ptr);
                                    }
                                    result_ptr = common_write_16_bit(flags, result_ptr);
                                }
                            }
                        } else if (type == THREAD_NWK_DATA_TYPE_ROUTE) {
                            if (result_ptr) {
                                *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                                *result_ptr++ = subLength;
                            }
                            while (subLength) {
                                dptr += 2;
                                uint8_t preference = *dptr++;
                                subLength -= THREAD_HAS_ROUTE_TLV_LENGTH;
                                if (result_ptr) {
                                    result_ptr = common_write_16_bit(0xfffe, result_ptr);
                                    *result_ptr++ = preference;
                                }
                            }
                        } else {
                            if (result_ptr) {
                                *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                                *result_ptr++ = subLength;
                                memcpy(result_ptr, dptr, subLength);
                                result_ptr += subLength;
                            }

                            dptr += subLength;
                        }

                    } else {
                        tr_error("Length fail");
                        return 0;
                    }
                }

                if (result_ptr) {
                    *length_ptr = total_tlv_length;
                }

            } else if (type == THREAD_NWK_DATA_TYPE_SERVICE_DATA) {
                uint8_t *length_ptr = NULL;
                uint16_t total_tlv_length = 0;
                uint16_t copy_length = 1;
                uint8_t T = (*dptr) >> 7;
                uint8_t S_id = *dptr & 0x0f;

                if (!T) {
                    network_data_len += 4;
                    total_tlv_length += 4;
                    copy_length += 4;
                    length -= 4;
                }

                uint16_t service_data_length = *(dptr + copy_length);

                if (result_ptr) {
                    *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                    length_ptr = result_ptr;
                    result_ptr++;
                    memcpy(result_ptr, dptr, 1 + copy_length + service_data_length);
                    result_ptr += 1 + copy_length + service_data_length;
                }
                network_data_len += 2 + 2 + service_data_length;
                total_tlv_length += 2 + service_data_length;
                dptr += 1 + copy_length + service_data_length;
                length -= 2 + service_data_length;

                while (length > 2) {
                    type = *dptr++;
                    uint8_t subLength = *dptr++;

                    if (!(type & THREAD_NWK_STABLE_DATA)) {
                        // Skip this sub-TLV altogether...
                        length -= 2 + subLength;
                        dptr += subLength;
                        continue;
                    }

                    length -= 2;
                    type &= THREAD_NWK_DATA_TYPE_MASK;

                    if (subLength <= length) {
                        tr_debug("SubType: %02x, length: %d, data: %s", type, length, trace_array(dptr, subLength));
                        total_tlv_length += 2 + subLength;
                        network_data_len += 2 + subLength;
                        length -= subLength;
                        if (result_ptr) {
                            *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                            *result_ptr++ = subLength;
                            uint8_t *rloc_ptr = result_ptr;
                            // Copy the remaining
                            memcpy(result_ptr, dptr, subLength);
                            result_ptr += subLength;
                            // Copy RLOC but replace with ALOC
                            common_write_16_bit(0xfc10 | S_id, rloc_ptr);
                        }
                        dptr += subLength;
                    } else {
                        tr_debug("Server sub-TLV parse fail!");
                        return 0;
                    }
                }

                if (result_ptr) {
                    *length_ptr = total_tlv_length;
                }
            } else {
                tr_debug("Unknown TLV: %d, length: %d", type, length);
                network_data_len += 2 + length;
                if (result_ptr) {
                    *result_ptr++ = type | THREAD_NWK_STABLE_DATA;
                    *result_ptr++ = length;
                    memcpy(result_ptr, dptr, length);
                    result_ptr += length;
                }
                dptr += length;
            }
        } else {
            tr_error("Length failure");
            return 0;
        }
    }

    return network_data_len;
}

uint16_t thread_network_data_tlv_size(protocol_interface_info_entry_t *cur, bool fullist)
{
    uint16_t length = thread_network_data_tlv_length(cur, fullist);

    if (length > 254) {
        // 2 extra bytes for extended TLV format
        length += 2;
    }

    return length;
}

uint8_t *thread_network_data_tlv_write(protocol_interface_info_entry_t *cur, uint8_t *ptr, bool fulllist)
{
    uint16_t length = thread_network_data_tlv_length(cur, fulllist);
    *ptr++ = MLE_TYPE_NETWORK_DATA;

    if (length > 254) {
        *ptr++ = 0xff;
        ptr = common_write_16_bit(length, ptr);
    } else {
        *ptr++ = length;
    }

    if (fulllist) {
        if (cur->thread_info->networkDataStorage.network_data_len > 0) {
            memcpy(ptr, cur->thread_info->networkDataStorage.network_data,
                   cur->thread_info->networkDataStorage.network_data_len);
            ptr += cur->thread_info->networkDataStorage.network_data_len;
        }
        return ptr;
    }

    uint16_t size = thread_network_data_generate_stable_set(cur, ptr);
    return ptr + size;
}

uint8_t *thread_active_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    link_configuration_s *configuration;

    configuration = thread_joiner_application_get_config(cur->id);
    if (!configuration) {
        tr_error("No configuration");
        return ptr;
    }
    *ptr++ = MLE_TYPE_ACTIVE_TIMESTAMP;
    *ptr++ = 8;
    ptr = common_write_64_bit(configuration->timestamp, ptr);
    return ptr;
}

uint16_t thread_active_operational_dataset_size(protocol_interface_info_entry_t *cur)
{
    return 2 + thread_joiner_application_active_config_length(cur->id, NULL, 0, mle_active_configuration_dataset_ignore_tlvs, mle_active_configuration_dataset_ignore_tlvs_size());
}

uint8_t *thread_active_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    int length;
    length = thread_joiner_application_active_config_length(cur->id, NULL, 0, mle_active_configuration_dataset_ignore_tlvs, mle_active_configuration_dataset_ignore_tlvs_size());
    if (length < 1) {
        return ptr;
    }
    *ptr++ = MLE_TYPE_OPERATIONAL_DATASET;
    *ptr++ = length;
    ptr = thread_joiner_application_active_config_write(cur->id, ptr, NULL, 0, mle_active_configuration_dataset_ignore_tlvs, mle_active_configuration_dataset_ignore_tlvs_size());

    return ptr;
}
bool thread_active_operational_dataset_process(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t len, uint64_t dataset_timestamp)
{
    //make a copy of the incoming timestamp
    uint64_t timestamp;
    link_configuration_s *link_configuration;

    if (!cur || !cur->thread_info || !ptr || !len) {
        return false;
    }
    link_configuration = thread_joiner_application_get_config(cur->id);
    if (!link_configuration) {
        return false;
    }
    tr_debug("process Active dataset");
    timestamp = thread_joiner_application_active_timestamp_get(cur->id);

    if (timestamp > dataset_timestamp) {
        tr_debug("We have newer timestamp");
        thread_joiner_application_next_active_config_save(cur->id);
    }
    if (timestamp == dataset_timestamp) {
        // No changes required
        return false;
    }
    tr_debug("Update Active dataset");
    // New active operational dataset received;
    thread_joiner_application_update_configuration(cur->id, ptr, len, false);
    thread_joiner_application_active_timestamp_set(cur->id, dataset_timestamp);
    thread_configuration_thread_activate(cur, link_configuration);
    thread_joiner_application_configuration_nvm_save(cur->id);
    return true;
}

uint8_t thread_pending_timestamp_tlv_size(protocol_interface_info_entry_t *cur)
{
    if (!thread_joiner_application_pending_config_timestamp_get(cur->id)) {
        return 0;
    }
    return 2 + 8;
}

uint8_t *thread_pending_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    uint64_t pending_timestamp;

    pending_timestamp = thread_joiner_application_pending_config_timestamp_get(cur->id);
    if (!pending_timestamp) {
        return ptr;
    }
    *ptr++ = MLE_TYPE_PENDING_TIMESTAMP;
    *ptr++ = 8;
    ptr = common_write_64_bit(pending_timestamp, ptr);
    return ptr;
}

uint16_t thread_pending_operational_dataset_size(protocol_interface_info_entry_t *cur)
{

    if (!thread_joiner_application_pending_config_exists(cur->id)) {
        return 0;
    }
    // Pending set always includes delay timer but not pending timestamp
    return thread_joiner_application_pending_config_length(cur->id, NULL, 0, mle_pending_configuration_dataset_ignore_tlvs, mle_pending_configuration_dataset_ignore_tlvs_size());
}

uint8_t *thread_pending_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    int dataset_length;
    if (!thread_joiner_application_pending_config_exists(cur->id)) {
        return ptr;
    }

    dataset_length = thread_joiner_application_pending_config_length(cur->id, NULL, 0, mle_pending_configuration_dataset_ignore_tlvs, mle_pending_configuration_dataset_ignore_tlvs_size());
    if (dataset_length < 1) {
        return ptr;
    }
    *ptr++ = MLE_TYPE_PENDING_OPERATIONAL_DATASET;
    *ptr++ = dataset_length;
    ptr = thread_joiner_application_pending_config_build(cur->id, ptr, NULL, 0, mle_pending_configuration_dataset_ignore_tlvs, mle_pending_configuration_dataset_ignore_tlvs_size());
    return ptr;
}

bool thread_pending_operational_dataset_process(protocol_interface_info_entry_t *cur, uint64_t mle_pending_timestamp, uint8_t *ptr, uint16_t len)
{
    uint32_t delay_timer;

    if (!cur || !cur->thread_info) {
        return false;
    }
    tr_debug("process pending dataset");
    if (!ptr || !len) {
        // No pending set received
        return false;
    }

    if (4 > thread_meshcop_tlv_data_get_uint32(ptr, len, MESHCOP_TLV_DELAY_TIMER, &delay_timer)) {
        tr_warn("Delay timer not present");
        return false;
    }

    if (mle_pending_timestamp < thread_joiner_application_pending_config_timestamp_get(cur->id)) {
        // Saving this config for later use first we get the current active
        tr_debug("save pending set for future");
        thread_joiner_application_next_pending_config_save(cur->id);
    }

    if (0 != thread_joiner_application_pending_config_create(cur->id, ptr, len)) {
        tr_error("pending set creation failed");
        return false;
    }
    tr_debug("updating pending dataset");
    thread_joiner_application_pending_config_timestamp_set(cur->id, mle_pending_timestamp);
    thread_joiner_application_pending_config_enable(cur->id, delay_timer);
    return true;
}

uint8_t thread_leader_data_tlv_size(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->thread_info || !cur->thread_info->thread_leader_data) {
        return 0;
    }
    return 9;
}

uint8_t *thread_leader_data_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->thread_info || !cur->thread_info->thread_leader_data) {
        return ptr;
    }
    *ptr++ = MLE_TYPE_LEADER_DATA; /* MLE TLV Type */
    *ptr++ = 8; /* MLE TLV Value: NWD:LDR TLV Length (7) */
    ptr = common_write_32_bit(cur->thread_info->thread_leader_data->partitionId, ptr);
    *ptr++ = cur->thread_info->thread_leader_data->weighting;
    *ptr++ = cur->thread_info->thread_leader_data->dataVersion;
    *ptr++ = cur->thread_info->thread_leader_data->stableDataVersion;
    *ptr++ = cur->thread_info->thread_leader_data->leaderRouterId;
    return ptr;
}

bool thread_addresses_needs_to_be_registered(protocol_interface_info_entry_t *cur)
{
    lowpan_context_t *ctx;
    uint8_t thread_realm_local_mcast_addr[16];
    uint8_t thread_ll_unicast_prefix_based_mcast_addr[16];
    if (thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_SLEEPY_END_DEVICE &&
            thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_END_DEVICE) {
        // No address registration for others than MED or SED
        return false;
    }

    // check for addresses
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (addr_ipv6_scope(e->address, cur) == IPV6_SCOPE_GLOBAL || (addr_ipv6_scope(e->address, cur) == IPV6_SCOPE_REALM_LOCAL
                                                                      && !thread_addr_is_mesh_local_16(e->address, cur))) {
            ctx = lowpan_context_get_by_address(&cur->lowpan_contexts, e->address);
            if (!ctx) {
                return true;
            }
            if (ctx->cid != 0) {
                return true;

            }
        }
    }

    // check for multicast groups
    thread_bootstrap_all_nodes_address_generate(thread_realm_local_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_REALM_LOCAL);
    thread_bootstrap_all_nodes_address_generate(thread_ll_unicast_prefix_based_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_LINK_LOCAL);
    ns_list_foreach(if_group_entry_t, entry, &cur->ip_groups) {
        if (!memcmp((entry->group), ADDR_MULTICAST_SOLICITED, 13)) {
            /* Skip solicited node multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, thread_realm_local_mcast_addr)) {
            /* Skip well-known realm-local all Thread nodes multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, thread_ll_unicast_prefix_based_mcast_addr)) {
            /* Skip well-known link-local all Thread nodes multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_ALL_MPL_FORWARDERS)) {
            /* Skip All MPL Forwarders address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_REALM_LOCAL_ALL_NODES)) {
            /* Skip Mesh local all nodes */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_REALM_LOCAL_ALL_ROUTERS)) {
            /* Skip Mesh local all routers */
            continue;
        }
        return true;
    }
    return false;
}

uint8_t *thread_ml_address_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    lowpan_context_t *ctx;
    uint8_t *address_len_ptr;

    if (thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_SLEEPY_END_DEVICE &&
            thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_END_DEVICE) {
        // No address registration for others than MED or SED
        return ptr;
    }
    *ptr++ = MLE_TYPE_ADDRESS_REGISTRATION;
    address_len_ptr = ptr++;

    *address_len_ptr = 0;

    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {

        if (*address_len_ptr > 148) {
            // Maximum length of address registrations
            continue;
        }
        if (!thread_addr_is_mesh_local_16(e->address, cur)) {
            ctx = lowpan_context_get_by_address(&cur->lowpan_contexts, e->address);
            if (ctx && ctx->cid == 0) {
                //Write TLV to list
                *ptr++ = (ctx->cid | 0x80);
                memcpy(ptr, e->address + 8, 8);
                ptr += 8;
                *address_len_ptr += 9;
            }
        }
    }
    return ptr;
}

uint8_t *thread_address_registration_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    uint8_t thread_realm_local_mcast_addr[16];
    uint8_t thread_ll_unicast_prefix_based_mcast_addr[16];
    lowpan_context_t *ctx;
    uint8_t *address_len_ptr;

    if (thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_SLEEPY_END_DEVICE &&
            thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_END_DEVICE) {
        // No address registration for others than MED or SED
        return ptr;
    }
    *ptr++ = MLE_TYPE_ADDRESS_REGISTRATION;
    address_len_ptr = ptr++;

    *address_len_ptr = 0;

    // Register all global addressess
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (*address_len_ptr > 148) {
            // Maximum length of address registrations
            continue;
        }
        if (addr_ipv6_scope(e->address, cur) == IPV6_SCOPE_GLOBAL || (addr_ipv6_scope(e->address, cur) == IPV6_SCOPE_REALM_LOCAL
                                                                      && !thread_addr_is_mesh_local_16(e->address, cur))) {
            ctx = lowpan_context_get_by_address(&cur->lowpan_contexts, e->address);
            if (ctx) {
                //Write TLV to list
                *ptr++ = (ctx->cid | 0x80);
                memcpy(ptr, e->address + 8, 8);
                ptr += 8;
                *address_len_ptr += 9;
            } else {
                *ptr++ = 0;
                memcpy(ptr, e->address, 16);
                ptr += 16;
                *address_len_ptr += 17;
            }
        }
    }

    /* Registers multicast addresses to the parent */
    thread_bootstrap_all_nodes_address_generate(thread_realm_local_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_REALM_LOCAL);
    thread_bootstrap_all_nodes_address_generate(thread_ll_unicast_prefix_based_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_LINK_LOCAL);
    ns_list_foreach(if_group_entry_t, entry, &cur->ip_groups) {
        if (*address_len_ptr > 148) {
            // Maximum length of address registrations
            continue;
        }

        if (!memcmp((entry->group), ADDR_MULTICAST_SOLICITED, 13)) {
            /* Skip solicited node multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, thread_realm_local_mcast_addr)) {
            /* Skip well-known realm-local all Thread nodes multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, thread_ll_unicast_prefix_based_mcast_addr)) {
            /* Skip well-known link-local all Thread nodes multicast address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_ALL_MPL_FORWARDERS)) {
            /* Skip All MPL Forwarders address */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_REALM_LOCAL_ALL_NODES)) {
            /* Skip Mesh local all nodes */
            continue;
        }
        if (addr_ipv6_equal(entry->group, ADDR_REALM_LOCAL_ALL_ROUTERS)) {
            /* Skip Mesh local all routers */
            continue;
        }

        *ptr++ = 0;
        memcpy(ptr, entry->group, 16);
        ptr += 16;
        *address_len_ptr += 17;
    }
    if (*address_len_ptr == 0) {
        // No address added remove type and length
        ptr -= 2;
    }
    return ptr;

}

int thread_link_reject_send(protocol_interface_info_entry_t *interface, const uint8_t *ll64)
{
    uint16_t buf_id;
    uint32_t keySequence;
    uint8_t *ptr;
    mle_message_timeout_params_t timeout;

    buf_id = mle_service_msg_allocate(interface->id, 32, false, MLE_COMMAND_REJECT);
    if (buf_id == 0) {
        return -1;
    }

    thread_management_get_current_keysequence(interface->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);

    mle_service_set_msg_destination_address(buf_id, ll64);

    ptr = mle_service_get_data_pointer(buf_id);

    // write status TLV
    *ptr++ = MLE_TYPE_STATUS;
    *ptr++ = 1;
    *ptr++ = MLE_STATUS_ERROR;

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);

    return mle_service_send_message(buf_id);

}

static uint8_t *thread_joining_port_tlv_write(uint16_t port, uint8_t *ptr)
{
    *ptr++ = MESHCOP_TLV_JOINER_UDP_PORT;
    *ptr++ = 2;
    return common_write_16_bit(port, ptr);
}

static uint8_t *thread_commissioner_port_tlv_write(uint16_t port, uint8_t *ptr)
{
    *ptr++ = MESHCOP_TLV_COMMISSIONER_UDP_PORT;
    *ptr++ = 2;
    return common_write_16_bit(port, ptr);
}

static void thread_tx_failure_handler(int8_t nwk_id, uint8_t accumulated_failures, uint8_t attribute_index)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(nwk_id);

    tr_debug("In Thread TX_FAIL handler, accumulated_failures=%d", accumulated_failures);

    if (!cur || !cur->thread_info) {
        return;
    }

    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_attribute_discover(mac_neighbor_info(cur), attribute_index);
    if (!neighbor) {
        return;
    }

    if (accumulated_failures >= THREAD_MAC_TRANSMISSIONS * THREAD_FAILED_CHILD_TRANSMISSIONS) {
        mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), neighbor);
    }
}

/* Called when MLE link to neighbour lost, or ETX callback says link is bad */
void thread_reset_neighbour_info(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *neighbour)
{
    thread_parent_info_t *thread_endnode_parent = thread_info(cur)->thread_endnode_parent;

    if (!thread_i_am_router(cur) && thread_endnode_parent && thread_endnode_parent->shortAddress == neighbour->mac16) {
        if (cur->nwk_bootstrap_state != ER_CHILD_ID_REQ) {
            tr_warn("End device lost parent, reset!\n");
            thread_bootstrap_connection_error(cur->id, CON_PARENT_CONNECT_DOWN, NULL);
        }
    }

    thread_routing_remove_link(cur, neighbour->mac16);
    thread_router_bootstrap_reset_child_info(cur, neighbour);
    protocol_6lowpan_release_long_link_address_from_neighcache(cur, neighbour->mac64);
    mac_helper_devicetable_remove(cur->mac_api, neighbour->index, neighbour->mac64);
    thread_neighbor_class_entry_remove(&cur->thread_info->neighbor_class, neighbour->index);
}

uint8_t thread_get_router_count_from_route_tlv(mle_tlv_info_t *routeTlv)
{
    if (!routeTlv) {
        return 0;
    }

    if (routeTlv->tlvLen < (MLE_ROUTE_ID_MASK_SIZE + 1)) {
        return 0;
    }

    if (!routeTlv->dataPtr) {
        return 0;
    }

    return routeTlv->tlvLen - MLE_ROUTE_ID_MASK_SIZE - 1;
}

static void thread_address_notification_cb(struct protocol_interface_info_entry *interface, const struct if_address_entry *addr, if_address_callback_t reason)
{
    if (thread_attach_ready(interface) != 0) {
        return;
    }

    if (reason != ADDR_CALLBACK_DAD_COMPLETE && reason != ADDR_CALLBACK_DELETED) {
        return;
    }

    if (addr_ipv6_scope(addr->address, interface) > IPV6_SCOPE_REALM_LOCAL) {
        tr_debug("Global address changed: %s", trace_ipv6(addr->address));

        if (thread_bootstrap_should_register_address(interface)) {
            interface->thread_info->childUpdateReqTimer = 1;
        } else {
            if (reason == ADDR_CALLBACK_DAD_COMPLETE) {
                /* Send address notification (our parent doesn't do that for us) */
                thread_bootstrap_address_registration(interface, addr->address, NULL, false, false);
            }
        }
    }
}

static bool thread_mcast_should_register_address(struct protocol_interface_info_entry *cur, uint8_t *addr)
{
    uint8_t thread_realm_local_mcast_addr[16];
    uint8_t thread_ll_unicast_prefix_based_mcast_addr[16];
    if (addr_ipv6_multicast_scope(addr) < IPV6_SCOPE_LINK_LOCAL) {
        return false;
    }
    if (memcmp(addr, ADDR_MULTICAST_SOLICITED, 13) == 0) {
        return false;
    }
    if (memcmp(addr, ADDR_LINK_LOCAL_ALL_NODES, 16) == 0) {
        return false;
    }
    if (memcmp(addr, ADDR_LINK_LOCAL_ALL_ROUTERS, 16) == 0) {
        return false;
    }
    thread_bootstrap_all_nodes_address_generate(thread_realm_local_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_REALM_LOCAL);
    if (memcmp(addr, thread_realm_local_mcast_addr, 16) == 0) {
        return false;
    }
    thread_bootstrap_all_nodes_address_generate(thread_ll_unicast_prefix_based_mcast_addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_LINK_LOCAL);
    if (memcmp(addr, thread_ll_unicast_prefix_based_mcast_addr, 16) == 0) {
        return false;
    }
    return true;
}

void thread_mcast_group_change(struct protocol_interface_info_entry *interface, if_group_entry_t *group, bool addr_added)
{
    if (thread_attach_ready(interface) != 0) {
        return;
    }

    tr_debug("Thread multicast address changed: %s", trace_ipv6(group->group));

    if (thread_bootstrap_should_register_address(interface)) {
        /* Trigger Child Update Request only if MTD child's multicast address change */
        if (thread_mcast_should_register_address(interface, group->group)) {
            interface->thread_info->childUpdateReqTimer = 1;
        }
    } else {
        if (addr_added) {
            thread_bootstrap_address_registration_timer_set(interface, 0, 1);
        }
    }
}

static void thread_old_partition_data_clean(int8_t interface_id)
{
    thread_management_client_old_partition_data_clean(interface_id);
    thread_border_router_old_partition_data_clean(interface_id);
}

void thread_partition_data_purge(protocol_interface_info_entry_t *cur)
{
    /* Partition has been changed. Wipe out data related to old partition */
    thread_old_partition_data_clean(cur->id);

    /* Reset previous routing information */
    thread_routing_reset(&cur->thread_info->routing);

    /* Flush address cache */
    ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);

    /* Remove linked neighbours for REEDs and FEDs */
    thread_reed_fed_neighbour_links_clean(cur);

}

bool thread_partition_match(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData)
{
    if (thread_info(cur)->thread_leader_data) {
        if ((thread_info(cur)->thread_leader_data->partitionId == leaderData->partitionId) &&
                (thread_info(cur)->thread_leader_data->weighting == leaderData->weighting)) {
            return true;
        }
    }
    return false;
}

void thread_partition_info_update(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData)
{
    /* Force network data update later when processing network data TLV */
    thread_info(cur)->thread_leader_data->dataVersion = leaderData->dataVersion - 1;
    thread_info(cur)->thread_leader_data->stableDataVersion = leaderData->stableDataVersion - 1;
    thread_info(cur)->thread_leader_data->leaderRouterId = leaderData->leaderRouterId;
    thread_info(cur)->thread_leader_data->partitionId = leaderData->partitionId;
    thread_info(cur)->thread_leader_data->weighting = leaderData->weighting;
    /* New network data learned, get rid of old partition data */
    thread_partition_data_purge(cur);
}

void thread_neighbor_communication_update(protocol_interface_info_entry_t *cur, uint8_t neighbor_attribute_index)
{
    thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, neighbor_attribute_index);
}

void thread_maintenance_timer_set(protocol_interface_info_entry_t *cur)
{
    thread_info(cur)->thread_maintenance_timer = THREAD_MAINTENANCE_TIMER_INTERVAL + randLIB_get_random_in_range(0, THREAD_MAINTENANCE_TIMER_INTERVAL / 10);
}

#ifdef HAVE_THREAD_V2

void thread_common_ccm_allocate(protocol_interface_info_entry_t *cur)
{
    cur->thread_info->ccm_info = ns_dyn_mem_alloc(sizeof(thread_ccm_info_t));
    if (!cur->thread_info->ccm_info) {
        return;
    }
    memset(cur->thread_info->ccm_info, 0, sizeof(thread_ccm_info_t));
    cur->thread_info->ccm_info->update_needed = true;
    cur->thread_info->ccm_info->listen_socket_ae = -1;
    cur->thread_info->ccm_info->listen_socket_nmkp = -1;

}

void thread_common_ccm_free(protocol_interface_info_entry_t *cur)
{
    ns_dyn_mem_free(cur->thread_info->ccm_info);
    cur->thread_info->ccm_info = NULL;
}

bool thread_common_ccm_enabled(protocol_interface_info_entry_t *cur)
{
    if (thread_info(cur)->version <= THREAD_PROTOCOL_VERSION) {
        // Thread 1.1 version devices dont check the extension
        return false;
    }
    uint16_t securityPolicy = thread_joiner_application_security_policy_get(cur->id);

    if (!(securityPolicy & THREAD_SECURITY_POLICY_CCM_DISABLED)) {
        return true;
    }

    return false;
}

static uint8_t *thread_common_server_tlv_list_get(uint8_t *service_tlv_ptr, uint16_t service_tlv_len, uint16_t *server_tlv_list_len)
{
    uint16_t tlv_length = 0;
    uint8_t service_id_len = 0;

    if (!(*service_tlv_ptr & 0x80)) {
        tlv_length += 4;
        service_tlv_ptr += 4;
    } else {
        tlv_length += 1;
        service_tlv_ptr++;
    }
    service_id_len = *service_tlv_ptr++;
    tlv_length++;

    tlv_length += service_id_len;
    service_tlv_ptr += service_id_len;

    if (tlv_length > service_tlv_len) {
        return NULL;
    }
    if (server_tlv_list_len) {
        *server_tlv_list_len = service_tlv_len - tlv_length;
    }

    return service_tlv_ptr;

}

int thread_common_primary_bbr_get(struct protocol_interface_info_entry *cur, uint8_t *addr_ptr, uint8_t *seq_ptr, uint32_t *mlr_timer_ptr, uint32_t *delay_timer_ptr)
{
    uint8_t *service_tlv_ptr = NULL;
    uint16_t service_tlv_len;
    //tr_debug("Search for Primary BBR info");

    do {
        service_tlv_len = thread_meshcop_tlv_find_next(cur->thread_info->networkDataStorage.network_data, cur->thread_info->networkDataStorage.network_data_len, THREAD_NWK_DATA_TYPE_SERVICE_DATA | THREAD_NWK_STABLE_DATA, &service_tlv_ptr);
        if (service_tlv_len > 3 &&
                (service_tlv_ptr[0] & 0x80) && // THREAD_ENTERPRISE_NUMBER
                service_tlv_ptr[1] == 1 && // length 1
                service_tlv_ptr[2] == THREAD_SERVICE_DATA_BBR) {
            //tr_info("BBR service TLV: %s\r\n", trace_array(service_tlv_ptr, service_tlv_len));
            // try to parse SUB-TLVs
            // network_data_server_tlv_parse(service_tlv_ptr, tlv_length);
            uint16_t server_tlv_len = 0;
            uint8_t *server_tlv_ptr = thread_common_server_tlv_list_get(service_tlv_ptr, service_tlv_len, &server_tlv_len);
            uint16_t found_tlv_len;
            uint8_t *found_tlv = NULL;
            //tr_info("BBR server TLV: %s\r\n", trace_array(server_tlv_ptr, server_tlv_len));
            do {
                found_tlv_len = thread_meshcop_tlv_find_next(server_tlv_ptr, server_tlv_len, THREAD_NWK_DATA_TYPE_SERVER_DATA | THREAD_NWK_STABLE_DATA, &found_tlv);
                if (found_tlv && found_tlv_len > 8) {
                    //tr_info("BBR server TLV: %s\r\n", trace_array(found_tlv, found_tlv_len));
                    if (addr_ptr) {
                        thread_addr_write_mesh_local_16(addr_ptr, common_read_16_bit(found_tlv), cur->thread_info);
                    }
                    if (seq_ptr) {
                        *seq_ptr = found_tlv[2];
                    }
                    if (delay_timer_ptr) {
                        *delay_timer_ptr = common_read_16_bit(&found_tlv[3]);
                    }
                    if (mlr_timer_ptr) {
                        *mlr_timer_ptr = common_read_32_bit(&found_tlv[5]);
                        if (*mlr_timer_ptr < THREAD_DEFAULT_MIN_MLR_TIMEOUT) {
                            *mlr_timer_ptr = THREAD_DEFAULT_MIN_MLR_TIMEOUT;
                        }
                    }
                    return 0;
                }
            } while (found_tlv_len > 0);
        }
    } while (service_tlv_len > 0);
    return -1;
}

#endif // HAVE_THREAD_V2
#endif

