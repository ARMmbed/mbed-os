/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "platform/arm_hal_interrupt.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "common_functions.h"
#include "randLIB.h"
#include "platform/arm_hal_phy.h"
#include "platform/arm_hal_interrupt.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "net_pana_parameters_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/mld.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "MPL/mpl.h"
#include "RPL/rpl_control.h"
#include "libDHCPv6/libDHCPv6.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Service_Libs/whiteboard/whiteboard.h"

#include "6LoWPAN/MAC/beacon_handler.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_response_handler.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/NVM/nwk_nvm.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "Service_Libs/load_balance/load_balance_api.h"
#include "Service_Libs/pan_blacklist/pan_blacklist_api.h"

#include "mac_api.h"
#include "ethernet_mac_api.h"

#define TRACE_GROUP_CORE "core"

#define TRACE_GROUP "core"

#ifndef SEC_LIB_X_100MS_COUNTER
#define SEC_LIB_X_100MS_COUNTER 1 //Default scaller is 100ms tick
#endif

// RFC 4861 says we only have to reroll ReachableTime every couple of hours, but
// to make sure the code is regularly exercised, let's make it 10 minutes.
#define REACHABLE_TIME_UPDATE_SECONDS       600

/** Quick monotonic time for simple timestamp comparisons; 100ms ticks.
 * This can of course wrap, so to handle this correctly comparisons must be
 * expressed like:
 *
 * "if (time_now - time_then < 200)"
 * NOT
 * "if (time_now < time_then + 200)"
 */
uint32_t protocol_core_monotonic_time;
static int8_t protocol_root_tasklet_ID = -1;

int protocol_core_buffers_in_event_queue;

protocol_interface_info_entry_t *protocol_core_multicast_upstream;

typedef struct {
    uint8_t core_timer_ticks;
    bool core_timer_event;
    uint16_t core_security_ticks_counter;
} lowpan_core_timer_structures_s;

protocol_interface_list_t NS_LIST_NAME_INIT(protocol_interface_info_list);

static lowpan_core_timer_structures_s protocol_core_timer_info;

/** Cores Power Save Varibale whic indicate States  */
volatile uint8_t power_save_state =  0;

void core_timer_event_handle(uint16_t ticksUpdate);
static void protocol_buffer_poll(buffer_t *b);

static int8_t net_interface_get_free_id(void);

static void nwk_net_event_post(arm_nwk_interface_status_type_e posted_event, int8_t net_tasklet, int8_t nwk_id);

static uint16_t  protocol_core_seconds_timer = 10;

int8_t protocol_read_tasklet_id(void)
{
    return protocol_root_tasklet_ID;
}

uint8_t check_power_state(uint8_t mode)
{
    uint8_t ret_val = power_save_state & mode;
    return ret_val;
}


void set_power_state(uint8_t mode)
{
    power_save_state |= mode;
}

void clear_power_state(uint8_t mode)
{
    power_save_state &= ~mode;
}

void arm_net_protocol_packet_handler(buffer_t *buf, protocol_interface_info_entry_t *cur_interface)
{
    if (cur_interface->if_stack_buffer_handler) {
        cur_interface->if_stack_buffer_handler(buf);
        //buf = 0;
    } else {
        buffer_free(buf);
    }
}

void protocol_root_tasklet(arm_event_t *event)
{
    arm_internal_event_type_e event_type;
    event_type = (arm_internal_event_type_e)event->event_type;

    switch (event_type) {
        case ARM_LIB_TASKLET_INIT_EVENT:
            tr_debug("NS Root task Init");
            break;

        case ARM_IN_PROTOCOL_TIMER_EVENT: {
            uint16_t tick_update = (uint16_t)event->event_data;
            /* This event is delivered as "user-allocated", so finish reading
             * before "freeing" */
            protocol_timer_event_lock_free();
            protocol_timer_cb(tick_update);
            break;
        }
        case ARM_IN_INTERFACE_BOOTSTRAP_CB:
            net_bootsrap_cb_run(event->event_id);
            break;
        case ARM_IN_INTERFACE_CORE_TIMER_CB:
            /* This event is delivered as "user-allocated", so finish reading
             * before "freeing" */
            core_timer_event_handle((uint16_t)event->event_data);
            break;
        case ARM_IN_INTERFACE_PROTOCOL_HANDLE: {
            buffer_t *buf = event->data_ptr;
            protocol_buffer_poll(buf);
            break;
        }
        case ARM_IN_SECURITY_ECC_CALLER:
            sec_ecc_sceduler();
            break;
        default:
            break;
    }
}
void protocol_core_security_tick_update(uint16_t tick_update)
{
    if (protocol_core_timer_info.core_security_ticks_counter <= tick_update) {
        sec_timer_handle();
        protocol_core_timer_info.core_security_ticks_counter = SEC_LIB_X_100MS_COUNTER;
    }
}

static void nwk_bootsrap_timer(protocol_interface_info_entry_t *cur)
{
    if (cur->bootsrap_state_machine_cnt) {
        if (cur->bootsrap_state_machine_cnt-- == 1) {
            arm_event_s event = {
                .receiver = protocol_root_tasklet_ID,
                .sender = 0,
                .event_id = (uint8_t)cur->id,
                .event_type = ARM_IN_INTERFACE_BOOTSTRAP_CB,
                .data_ptr = NULL,
                .priority = ARM_LIB_LOW_PRIORITY_EVENT,
            };
            if (eventOS_event_send(&event) != 0) {
                cur->bootsrap_state_machine_cnt = 1; // Try again next tick
                tr_error("nwk_bootsrap_timer(): event send failed");
            }
        }
    }
}

void core_timer_event_handle(uint16_t ticksUpdate)
{
    protocol_core_monotonic_time += ticksUpdate;

    if (protocol_core_seconds_timer <= ticksUpdate) {
        uint16_t extra_ticks = ticksUpdate - protocol_core_seconds_timer;
        uint16_t seconds = 1 + extra_ticks / 10;
        protocol_core_seconds_timer = (10 - extra_ticks % 10);
        // TODO: make this lot use "seconds", not 1

        ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
            if (cur->nwk_id == IF_6LoWPAN) {
                if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
                    if (thread_info(cur)) {
                        thread_seconds_timer(cur, seconds);
                    } else if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
                        beacon_join_priority_update(cur->id);
                    }

                    if (cur->nwk_wpan_nvm_api) {
                        cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, false);
                    }
                }
            } else if (cur->nwk_id == IF_IPV6) {
                //Slow Pointer Update
                ipv6_core_slow_timer_event_handle(cur);
            }

            if (cur->lb_api) {
                cur->lb_api->lb_seconds_tick_update(cur->lb_api);
            }



            addr_slow_timer(cur, seconds);
            mld_slow_timer(cur, seconds);
            ipv6_neighbour_cache_slow_timer(&cur->ipv6_neighbour_cache, seconds);
            pan_blacklist_time_update(&cur->pan_blaclist_cache, seconds);
            pan_coordinator_blacklist_time_update(&cur->pan_cordinator_black_list, seconds);
            if (cur->reachable_time_ttl > seconds) {
                cur->reachable_time_ttl -= seconds;
            } else {
                protocol_stack_interface_set_reachable_time(cur, cur->base_reachable_time);
            }

            cur->icmp_ra_tokens += seconds;
            if (cur->icmp_ra_tokens > 3) {
                cur->icmp_ra_tokens = 3;
            }

            cur->mle_link_reject_tokens += seconds;
            if (cur->mle_link_reject_tokens > 2) {
                cur->mle_link_reject_tokens = 2;
            }
        }

        mpl_slow_timer(seconds);
        rpl_control_slow_timer(seconds);
        ipv6_route_table_ttl_update(seconds);
        ipv6_destination_cache_timer(seconds);
        ipv6_frag_timer(seconds);
        cipv6_frag_timer(seconds);
        protocol_6lowpan_mle_timer(seconds);
        /* This limit bad behaviour device's MLE link reject generation */

    } else {
        protocol_core_seconds_timer -= ticksUpdate;
    }

    //Fast Timer
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->nwk_id == IF_6LoWPAN) {
            if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
                nwk_bootsrap_timer(cur);
                nd_object_timer(cur,ticksUpdate);
                if (thread_info(cur)) {
                    thread_timer(cur, ticksUpdate);
                }
                lowpan_context_timer(&cur->lowpan_contexts, ticksUpdate);
            }
        } else if (cur->nwk_id == IF_IPV6) {
            ipv6_core_timer_event_handle(cur, ticksUpdate);
        }

        ipv6_neighbour_cache_fast_timer(&cur->ipv6_neighbour_cache, ticksUpdate);
        addr_fast_timer(cur, ticksUpdate);
        mld_fast_timer(cur, ticksUpdate);

        /* This gives us the RFC 4443 default (10 tokens/s, bucket size 10) */
        cur->icmp_tokens += ticksUpdate;
        if (cur->icmp_tokens > 10) {
            cur->icmp_tokens = 10;
        }
    }

    rpl_control_fast_timer(ticksUpdate);
    icmpv6_radv_timer(ticksUpdate);
    protocol_core_security_tick_update(ticksUpdate);
    platform_enter_critical();
    protocol_core_timer_info.core_timer_event = false;
    platform_exit_critical();
}

void protocol_core_cb(uint16_t ticksUpdate)
{
    protocol_timer_start(PROTOCOL_TIMER_STACK_TIM, protocol_core_cb, 100);
    protocol_core_timer_info.core_timer_ticks += ticksUpdate;
    if (!protocol_core_timer_info.core_timer_event) {
        protocol_core_timer_info.core_timer_event = true;
        /* Static initialised, constant values */
        static arm_event_storage_t event = {
            .data = {
                .sender = 0,
                .data_ptr = NULL,
                .event_type = ARM_IN_INTERFACE_CORE_TIMER_CB,
                .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
            }
        };
        event.data.receiver = protocol_root_tasklet_ID;
        event.data.event_data = protocol_core_timer_info.core_timer_ticks;
        eventOS_event_send_user_allocated(&event);
        protocol_core_timer_info.core_timer_ticks = 0;
    }
}


void protocol_core_init(void)
{
    protocol_root_tasklet_ID = eventOS_event_handler_create(&protocol_root_tasklet, ARM_LIB_TASKLET_INIT_EVENT);
    tr_debug("Allocate Root Tasklet");
    if (protocol_timer_init() == -1) {
        tr_error("Protocol timer init failed");
    }
    protocol_core_monotonic_time = 0;
    protocol_core_timer_info.core_timer_event = false;
    protocol_core_timer_info.core_timer_ticks = 0;
    protocol_core_timer_info.core_security_ticks_counter = SEC_LIB_X_100MS_COUNTER;

    protocol_timer_start(PROTOCOL_TIMER_STACK_TIM, protocol_core_cb, 100);

}

void protocol_core_interface_info_reset(protocol_interface_info_entry_t *entry)
{
    if (entry) {
        entry->global_address_available = false;
        icmpv6_radv_disable(entry);
        icmpv6_stop_router_advertisements(entry, NULL);
        lowpan_context_list_free(&entry->lowpan_contexts);
        ipv6_neighbour_cache_flush(&entry->ipv6_neighbour_cache);
        entry->if_stack_buffer_handler = 0;
        entry->if_6lowpan_dad_process.active = false;
        //Clean
        ns_list_foreach_safe(if_address_entry_t, addr, &entry->ip_addresses) {
            addr_delete_entry(entry, addr);
        }
#ifdef HAVE_RPL
        /* This is done after address deletion, so RPL can act on them */
        rpl_control_remove_domain_from_interface(entry);
#endif
    }
}

void bootsrap_next_state_kick(icmp_state_t new_state, protocol_interface_info_entry_t *cur)
{
    cur->bootsrap_state_machine_cnt = 0;
    cur->nwk_bootstrap_state = new_state;
    arm_event_s event = {
        .receiver = protocol_root_tasklet_ID,
        .sender = 0,
        .event_id = (uint8_t)cur->id,
        .event_type = ARM_IN_INTERFACE_BOOTSTRAP_CB,
        .data_ptr = NULL,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };
    if (eventOS_event_send(&event) != 0) {
        tr_error("bootsrap_next_state_kick(): event send failed");
    }
}

uint32_t protocol_stack_interface_set_reachable_time(protocol_interface_info_entry_t *cur, uint32_t base_reachable_time)
{
    cur->base_reachable_time = base_reachable_time;
    cur->reachable_time_ttl = REACHABLE_TIME_UPDATE_SECONDS;

    return cur->ipv6_neighbour_cache.reachable_time = randLIB_randomise_base(base_reachable_time, 0x4000, 0xBFFF);
}


static void protocol_core_base_init(protocol_interface_info_entry_t *entry,nwk_interface_id nwk_id)
{
    entry->nwk_id = nwk_id;
    switch (nwk_id) {
        case IF_IPV6:
            entry->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_ETHERNET_ROUTER;
            break;
       default:
            entry->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER;
            break;
    }
    entry->bootStrapId = -1;
    entry->lowpan_address_mode = NET_6LOWPAN_GP64_ADDRESS;
    entry->ipv6_configure = NULL;
    entry->if_lowpan_security_params = NULL;
    entry->if_ns_transmit = NULL;
    entry->if_special_forwarding = NULL;
    entry->if_snoop = NULL;
    entry->if_icmp_handler = NULL;
    entry->if_map_ip_to_link_addr = NULL;
    entry->if_map_link_addr_to_ip = NULL;
    entry->if_6lowpan_dad_process.active = false;
    entry->lowpan_desired_short_address = 0xfffe;
    entry->lowpan_info = 0;
    entry->rpl_domain = NULL;
    entry->if_down = NULL;
    entry->if_up = NULL;
}

static void protocol_core_base_finish_init(protocol_interface_info_entry_t *entry)
{
    entry->configure_flags = 0;
    entry->bootsrap_state_machine_cnt = 0;
    entry->pana_sec_info_temp = NULL;
    entry->lb_api = NULL;
    entry->global_address_available = false;
    entry->reallocate_short_address_if_duplicate = true;
    entry->iids_map_to_mac = false;
    entry->opaque_slaac_iids = true;
    entry->ip_multicast_as_mac_unicast_to_parent = false;
    entry->dad_failures = 0;
    entry->icmp_tokens = 10;
    entry->mle_link_reject_tokens = 2;
    entry->ip_forwarding = true; /* Default to on for now... */
    entry->ip_multicast_forwarding = true; /* Default to on for now... */
#ifdef HAVE_IPV6_ND
    entry->recv_ra_routes = true;
    entry->recv_ra_prefixes = true;
#endif
    entry->send_mld = true;
#ifdef HAVE_MPL
    entry->mpl_seed = false;
    entry->mpl_control_trickle_params = rfc7731_default_control_message_trickle_params;
    entry->mpl_data_trickle_params = rfc7731_default_data_message_trickle_params;
    entry->mpl_seed_set_entry_lifetime = RFC7731_DEFAULT_SEED_SET_ENTRY_LIFETIME;
    entry->mpl_proactive_forwarding = true;
    entry->mpl_seed_id_mode = MULTICAST_MPL_SEED_ID_IPV6_SRC_FOR_DOMAIN;
#endif
    entry->cur_hop_limit = UNICAST_HOP_LIMIT_DEFAULT;
    protocol_stack_interface_set_reachable_time(entry, 30000);
    entry->dup_addr_detect_transmits = 1;
    entry->ipv6_neighbour_cache.link_mtu = IPV6_MIN_LINK_MTU;
    entry->max_link_mtu = IPV6_MIN_LINK_MTU;
    entry->pmtu_lifetime = 10 * 60; // RFC 1981 default - 10 minutes
    icmpv6_radv_init(entry);
    ns_list_link_init(entry, link);
    entry->if_stack_buffer_handler = NULL;
    entry->interface_name = 0;
    entry->border_router_setup = NULL;
#ifdef HAVE_THREAD
    entry->thread_info = NULL;
#endif
    entry->mesh_callbacks = NULL;
    entry->ip_addresses_max_slaac_entries = 0;
    ns_list_init(&entry->lowpan_contexts);
    ns_list_init(&entry->ip_addresses);
    ns_list_init(&entry->ip_groups);
#ifdef MULTICAST_FORWARDING
    ns_list_init(&entry->ip_groups_fwd);
    entry->ip_mcast_fwd_for_scope = IPV6_SCOPE_SITE_LOCAL; // Default for backwards compatibility
#endif
    ns_list_init(&entry->ipv6_neighbour_cache.list);
}



static int lowpan_security_parameters_allocate(protocol_interface_info_entry_t *entry)
{
    entry->if_lowpan_security_params = ns_dyn_mem_alloc(sizeof(if_6lowpan_security_info_t));
    if (!entry->if_lowpan_security_params) {
        return -1;
    }

    entry->if_lowpan_security_params->security_level = 0;
    entry->if_lowpan_security_params->mle_security_frame_counter = 0;
    entry->if_lowpan_security_params->pana_params = NULL;
    entry->if_lowpan_security_params->nwk_security_mode = NET_SEC_MODE_NO_LINK_SECURITY;
    return 0;
}

static void lowpan_security_parameters_deallocate(protocol_interface_info_entry_t *entry)
{
    ns_dyn_mem_free(entry->if_lowpan_security_params);
    entry->if_lowpan_security_params = NULL;

}

static protocol_interface_info_entry_t * protocol_interface_class_allocate(nwk_interface_id nwk_id)
{
    protocol_interface_info_entry_t *entry = ns_dyn_mem_alloc(sizeof(protocol_interface_info_entry_t));
    if (entry) {
        memset(entry, 0, sizeof(protocol_interface_info_entry_t));
        /* We assume for now zone indexes for interface, link and realm all equal interface id */
        int8_t id = net_interface_get_free_id();
        entry->id = id;
        entry->zone_index[IPV6_SCOPE_INTERFACE_LOCAL] = id;
        entry->zone_index[IPV6_SCOPE_LINK_LOCAL] = id;
        entry->zone_index[IPV6_SCOPE_REALM_LOCAL] = id;
        protocol_core_base_init(entry,nwk_id);
    }
    return entry;
}

static protocol_interface_info_entry_t *protocol_core_interface_6lowpan_entry_get_with_mac(mac_api_t *api)
{
    if( !api ){
        return NULL;
    }
    protocol_interface_info_entry_t *entry = protocol_interface_class_allocate(IF_6LoWPAN);
    if (!entry) {
        return NULL;
    }

    if (lowpan_adaptation_interface_init(entry->id, api->phyMTU) != 0) {
        goto interface_failure;
    }

    if (reassembly_interface_init(entry->id, 8, 5) != 0) {
        goto interface_failure;
    }

    if (lowpan_security_parameters_allocate(entry) != 0) {
        goto interface_failure;
    }

    entry->mac_parameters = ns_dyn_mem_alloc(sizeof(arm_15_4_mac_parameters_t));
    if (!entry->mac_parameters) {
        goto interface_failure;
    }
    memset(entry->mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    entry->mac_parameters->MacUnsusecured_2003_cab = mac_unsecured_2003_compatibility;
    entry->mac_parameters->mac_short_address = 0xffff;
    entry->mac_parameters->pan_id = 0xffff;
    entry->mac_parameters->nwk_filter_params.beacon_protocol_id_filter = 0xff;
    entry->mac_parameters->nwk_filter_params.net_pan_id_filter = 0xffff;
    entry->mac_parameters->mac_in_direct_entry_timeout = 7000; //default timeout

    entry->mac_parameters->mac_prev_key_attribute_id = 0;
    entry->mac_parameters->mac_default_key_attribute_id = 1;
    entry->mac_parameters->mac_next_key_attribute_id = 2;

    entry->beacon_cb = beacon_received;

    entry->mac_api = api;
    int8_t err = entry->mac_api->mac_initialize( entry->mac_api, &mcps_data_confirm_handler, &mcps_data_indication_handler,
                                                 &mcps_purge_confirm_handler, &mlme_confirm_handler, &mlme_indication_handler,
                                                 entry->id );
    if( err < 0 ){
        goto interface_failure;
    }

    //Set default key source
    mac_helper_set_default_key_source(entry);

    protocol_core_base_finish_init(entry);
    return entry;

interface_failure:
    lowpan_adaptation_interface_free(entry->id);
    reassembly_interface_free(entry->id);
    ns_dyn_mem_free(entry->mac_parameters);
    lowpan_security_parameters_deallocate(entry);
    ns_dyn_mem_free(entry);
    entry = NULL;
    return NULL;
}

static void protocol_6lowpan_mac_set(protocol_interface_info_entry_t *cur, const uint8_t *mac)
{
    memcpy(cur->iid_eui64, mac, 8);
    /* Invert U/L Bit */
    cur->iid_eui64[0] ^= 2;

    mac_helper_mac64_set(cur, mac);
}

#ifdef HAVE_ETHERNET
static bool protocol_ipv6_setup_allocate(protocol_interface_info_entry_t *entry)
{
    entry->ipv6_configure = ns_dyn_mem_alloc(sizeof(ipv6_interface_info_t));
    if (entry->ipv6_configure) {
        entry->lowpan_info = INTERFACE_NWK_ROUTER_DEVICE;
        memset(entry->ipv6_configure, 0, sizeof(ipv6_interface_info_t));
        entry->ipv6_configure->temporaryUlaAddressState = false;
        return true;
    }
    return false;
}

static protocol_interface_info_entry_t *protocol_core_interface_ethernet_entry_get(eth_mac_api_t *api)
{
    protocol_interface_info_entry_t *entry = protocol_interface_class_allocate(IF_IPV6);
    if (!entry) {
        return NULL;
    }
    if (!protocol_ipv6_setup_allocate(entry)) {
        ns_dyn_mem_free(entry);
        entry = NULL;
    } else {
        entry->eth_mac_api = api;
        ipv6_interface_phy_sap_register(entry);
        protocol_core_base_finish_init(entry);
    }

    return entry;
}

static void protocol_ethernet_mac_set(protocol_interface_info_entry_t *cur, const uint8_t *mac)
{
    if( !cur || !cur->eth_mac_api ) {
        return;
    }

    memcpy(cur->mac, mac, 6);
    cur->mac[6] = 0;
    cur->mac[7] = 0;
    cur->iid_eui64[0] = mac[0] ^ 2;
    cur->iid_eui64[1] = mac[1];
    cur->iid_eui64[2] = mac[2];
    cur->iid_eui64[3] = 0xff;
    cur->iid_eui64[4] = 0xfe;
    cur->iid_eui64[5] = mac[3];
    cur->iid_eui64[6] = mac[4];
    cur->iid_eui64[7] = mac[5];

    cur->eth_mac_api->mac48_set(cur->eth_mac_api, cur->mac);
}

#else
protocol_interface_info_entry_t *protocol_core_interface_ethernet_entry_get(eth_mac_api_t *api)
{
    (void)api;
    return NULL;
}

static void protocol_ethernet_mac_set(protocol_interface_info_entry_t *cur, const uint8_t *mac)
{
    (void)cur;
    (void)mac;
}
#endif

static void protocol_stack_interface_iid_eui64_generate(protocol_interface_info_entry_t *cur , const uint8_t *mac)
{
    if (cur->nwk_id == IF_6LoWPAN) {
        protocol_6lowpan_mac_set(cur, mac);
    } else {
        protocol_ethernet_mac_set(cur, mac);
    }
    //By default use this EUI-64-based IID for SLAAC
    memcpy(cur->iid_slaac, cur->iid_eui64, 8);
    //And why not feed it into the random seed too?
    randLIB_add_seed(common_read_64_bit(cur->mac));
}

protocol_interface_info_entry_t *nwk_interface_get_ipv6_ptr(void)
{
#ifdef HAVE_ETHERNET
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->nwk_id == IF_IPV6) {
            return cur;
        }
    }
#endif

    return NULL;
}

void nwk_interface_print_neigh_cache(route_print_fn_t *print_fn)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        ipv6_neighbour_cache_print(&cur->ipv6_neighbour_cache, print_fn);
    }
}

void nwk_interface_flush_neigh_cache(void)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);
    }
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list)
    if (cur->nwk_id == nwk_id) {
        return cur;
    }

    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_id(int8_t nwk_id)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list)
    if (cur->id == nwk_id) {
        return cur;
    }

    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_bootstrap_id(int8_t id)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list)
    if (cur->bootStrapId == id) {
        return cur;
    }

    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_rpl_domain(const struct rpl_domain *domain, int8_t last_id)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->id > last_id && cur->rpl_domain == domain) {
            return cur;
        }
    }

    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_sleep_possibility(void)
{
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (!cur->if_stack_buffer_handler) {
            continue;
        }

        if (cur->nwk_id == IF_IPV6) {
            return NULL;
        }

        /* Note that rf_mac_setup == NULL is okay */
        if (cur->mac_parameters && cur->rfd_poll_info == NULL) {
            return NULL;
        }
    }

    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->mac_parameters && cur->rfd_poll_info) {
            if (!cur->rfd_poll_info->pollActive && lowpan_adaptation_tx_active(cur->id)) {
                return cur;
            }
        }
    }

    return NULL;
}

uint8_t nwk_bootsrap_ready(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = 0;
    if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) == 0) {
        if (cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
            ret_val = 1;
        }
    }
    return ret_val;
}

static int8_t net_interface_get_free_id(void)
{
    uint_fast8_t id; // Must be unsigned for loop test to work...

    for (id = 1; id <= INT8_MAX; id++) {
        bool in_use = false;
        /* interface index == default zone index for link, interface and realm, so
         * ensure selected ID is not in use for any of those scopes */
        ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
            if (cur->id == (int8_t) id ||
                cur->zone_index[IPV6_SCOPE_INTERFACE_LOCAL] == id ||
                cur->zone_index[IPV6_SCOPE_LINK_LOCAL] == id ||
                cur->zone_index[IPV6_SCOPE_REALM_LOCAL] == id) {
                in_use = true;
                break;
            }
        }
        if (!in_use) {
            return id;
        }
    }

    return -1;
}

protocol_interface_info_entry_t *protocol_stack_interface_generate_ethernet(eth_mac_api_t *api)
{
    if( !api ){
        return NULL;
    }
    protocol_interface_info_entry_t *new_entry = NULL;

    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->eth_mac_api == api) {
            return cur;
        }
    }

    if (api) {
        new_entry = protocol_core_interface_ethernet_entry_get(api);

        if (new_entry) {
            neighbor_cache_init(&(new_entry->neigh_cache));
            pan_blacklist_cache_init(&(new_entry->pan_blaclist_cache));
            pan_coordinator_blacklist_cache_init(&(new_entry->pan_cordinator_black_list));
            ipv6_neighbour_cache_init(&new_entry->ipv6_neighbour_cache, new_entry->id);
            addr_max_slaac_entries_set(new_entry, 16);
            uint8_t mac[6];
            int8_t error = api->mac48_get( api, mac );
            if(error){
                tr_error("mac_ext_mac64_address_get failed: %d", error);
                ns_dyn_mem_free(new_entry);
                return NULL;
            }

            protocol_stack_interface_iid_eui64_generate(new_entry, mac);
            ns_list_add_to_start(&protocol_interface_info_list, new_entry);

            (void) ipv6_route_table_set_max_entries(new_entry->id, ROUTE_RADV, 16);

            return new_entry;
        }
    }
    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_generate_lowpan(mac_api_t *api)
{
    if( !api ){
        return NULL;
    }
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->mac_api == api) {
            return cur;
        }
    }

    protocol_interface_info_entry_t *new_entry = protocol_core_interface_6lowpan_entry_get_with_mac(api);

    if (new_entry) {
        neighbor_cache_init(&(new_entry->neigh_cache));
        pan_blacklist_cache_init(&(new_entry->pan_blaclist_cache));
        pan_coordinator_blacklist_cache_init(&(new_entry->pan_cordinator_black_list));
        ipv6_neighbour_cache_init(&new_entry->ipv6_neighbour_cache, new_entry->id);

        uint8_t mac[8];
        int8_t error = api->mac64_get( api, MAC_EXTENDED_READ_ONLY, mac );
        if(error){
            tr_error("mac_ext_mac64_address_get failed: %d", error);
            ns_dyn_mem_free(new_entry);
            return NULL;
        }
        protocol_stack_interface_iid_eui64_generate(new_entry, mac);
        ns_list_add_to_start(&protocol_interface_info_list, new_entry);
        return new_entry;
    }
    return NULL;
}

bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t addr[/*addrlen*/])
{
    if (!cur) {
        return false;
    }

    switch (addrlen) {
        case 2:
            return cur->mac_parameters && cur->mac_parameters->mac_short_address == common_read_16_bit(addr);
        case 8:
            return memcmp(addr, cur->mac, 8) == 0;
        default:
            return false;
    }
}

/**
 * \brief Push Buffer to Protocol Core.
 *
 * \param buf pointer to buffer. NULL is accepted and ignored.
 */
void protocol_push(buffer_t *buf)
{
    /* Ignore NULL */
    if (!buf) {
        return;
    }

    arm_event_s event = {
            .receiver = protocol_root_tasklet_ID,
            .sender = 0,
            .event_type = ARM_IN_INTERFACE_PROTOCOL_HANDLE,
            .data_ptr = buf,
            .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) == 0) {
        protocol_core_buffers_in_event_queue++;
    } else {
        if ((buf->info & B_DIR_MASK) == B_DIR_DOWN) {
            buf = socket_tx_buffer_event(buf, SOCKET_NO_RAM);
        }
        if (buf) {
            buffer_free(buf);
        } else {
            tr_debug("TCP Allocated");
        }
    }
}

/*
 * A protocol poll function to be typically called from 'Event Core'. This function is registered to the call queue by using event_cb_send() from 'Event Core'.
 */
static void protocol_buffer_poll(buffer_t *b)
{
    protocol_core_buffers_in_event_queue--;

    // Avoid the danger of with route data becoming stale (including
    // dead info pointers) while the packet is in the queue.
    if (b->route && ipv6_route_table_source_was_invalidated(b->route->route_info.source)) {
        buffer_free_route(b);
        // Attempt re-route immediately - some layers assume routing already
        // performed by higher layers.
        if (!ipv6_buffer_route(b)) {
            goto error;
        }
    }

    // Once buffer queue is empty, clear the invalidation flags for above test
    if (protocol_core_buffers_in_event_queue == 0) {
        ipv6_route_table_source_invalidated_reset();
    } else if (protocol_core_buffers_in_event_queue < 0) {
        tr_err("protocol_core_buffers_in_event_queue negative");
    }

    // Call the actual handler
    protocol_interface_info_entry_t *cur = b->interface;
    if (cur && cur->if_stack_buffer_handler) {
        cur->if_stack_buffer_handler(b);
        return;
    }

error:
    socket_tx_buffer_event_and_free(b, SOCKET_TX_FAIL);
}

static void nwk_net_event_post(arm_nwk_interface_status_type_e posted_event, int8_t net_tasklet, int8_t nwk_id)
{
    arm_event_s event = {
        .receiver = net_tasklet,
        .sender = protocol_read_tasklet_id(), /**< Event sender Tasklet ID */
        .event_type = ARM_LIB_NWK_INTERFACE_EVENT,
        .event_data = posted_event,
        .event_id = (int8_t) nwk_id,
        .data_ptr = NULL,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };
    if (eventOS_event_send(&event) != 0) {
        tr_error("nwk_net_event_post(): event send failed");
    }
}

void nwk_bootsrap_state_update(arm_nwk_interface_status_type_e posted_event, protocol_interface_info_entry_t *cur)
{
    //Clear Bootsrap Active Bit allways
    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_ACTIVE;
    cur->bootsrap_state_machine_cnt = 0;
    nwk_net_event_post(posted_event, cur->net_start_tasklet, cur->id);

    if (posted_event == ARM_NWK_BOOTSTRAP_READY) {

            switch (cur->bootsrap_mode) {

            case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT:
            case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_SNIFFER:
            case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER:
            case ARM_NWK_BOOTSRAP_MODE_ETHERNET_HOST:
            case ARM_NWK_BOOTSRAP_MODE_ETHERNET_ROUTER:
#ifdef HAVE_ETHERNET
                if (cur->ipv6_configure) {
                    cur->ipv6_configure->IPv6_ND_state = IPV6_READY;
                    if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
                        addr_add_router_groups(cur);
                        icmpv6_radv_enable(cur);//Activate RA send only with static enviroment
                        icmpv6_restart_router_advertisements(cur, ADDR_UNSPECIFIED);
                        if (cur->ipv6_configure->accept_ra != NET_IPV6_RA_ACCEPT_ALWAYS) {
                            icmpv6_recv_ra_routes(cur, false); // removes all existing RADV routes
                            icmpv6_recv_ra_prefixes(cur, false);
                        }
                    }
                }
#endif
                break;

            default:
                mac_data_poll_protocol_poll_mode_disable(cur);
                if (!cur->rpl_domain) {
                    tr_debug("NON RPL Ready");
                    //nwk_protocol_poll_mode_disable(cur->nwk_id, 0);
                } else {
                    tr_debug("RPL Ready");
                }
        }
    } else {
        if (cur->if_down) {
            cur->if_down(cur);
        } else {
            tr_debug("if_down() NULL");
        }
    }
}

void net_bootsrap_cb_run(uint8_t event)
{
    int8_t nwk_id = (int8_t) event;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(nwk_id);
    if (cur) {
        if (cur->nwk_id == IF_6LoWPAN) {
            //eventOS_scheduler_set_active_tasklet(protocol_read_tasklet_id());
            if (thread_info(cur)) {
                thread_bootstrap_state_machine(cur);
            } else {
                protocol_6lowpan_bootstrap(cur);
            }
        } else if (cur->nwk_id == IF_IPV6) {
            //IPV6 Bootsrap Run
        }
    }
}

void protocol_core_dhcpv6_allocated_address_remove(protocol_interface_info_entry_t *cur, uint8_t *guaPrefix)
{
    ipv6_stack_route_advert_remove(guaPrefix, 64);
    //Delete Address & Routes
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == ADDR_SOURCE_DHCP && (e->prefix_len == 64) && bitsequal(e->address, guaPrefix, 64)) {

            ipv6_stack_route_advert_remove(e->address, 128);

            ns_list_remove(&cur->ip_addresses, e);
            ns_dyn_mem_free(e);
            tr_debug("Delete DHCPv6 Allocated Address");
            break;
        }
    }
}

int8_t protcol_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    ns_list_foreach(protocol_interface_info_entry_t, other, &protocol_interface_info_list) {
        if (other != cur && addr_is_assigned_to_interface(other, addr)) {
            return 0;
        }
    }

    return -1;
}

