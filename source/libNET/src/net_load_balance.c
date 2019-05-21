/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include <string.h>
#include "net_load_balance_api.h"
#include "net_interface.h"
#include "NWK_INTERFACE/Include/protocol.h"
#ifdef HAVE_6LOWPAN_ND
#include "net_rpl.h"
#include "Service_Libs/load_balance/load_balance_api.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "mlme.h"
#include "mac_api.h"
#include "sw_mac.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "libNET/src/net_load_balance_internal.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#ifdef PANA
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#endif
#include "6LoWPAN/ND/nd_router_object.h"
#ifdef HAVE_RPL
#include "RPL/rpl_control.h"
#endif

#define TRACE_GROUP "6lb"

static uint8_t net_load_balance_priority_get(const void *load_balance_user)
{
    const protocol_interface_info_entry_t *cur = load_balance_user;
    if (!cur->rpl_domain) {
        return 0;
    }
    int16_t priority = protocol_6lowpan_rpl_global_priority_get();
    if (priority < 0) {
        priority = 0;
    } else if (priority > 255) {
        priority = 255;
    }

    return priority;
}

static void net_load_balance_beacon_tx(const void *load_balance_user)
{
    const protocol_interface_info_entry_t *cur = load_balance_user;
    mlme_set_t set_req;
    set_req.attr = macLoadBalancingBeaconTx;
    set_req.attr_index = 0;
    set_req.value_pointer = NULL;
    set_req.value_size = 0;
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);

}

static bool net_load_balance_network_switch_req(void *load_balance_user, struct mlme_pan_descriptor_s *PANDescriptor, const uint8_t *beacon_payload, uint16_t beacon_payload_length)
{
    protocol_interface_info_entry_t *interface = load_balance_user;
    //Run down current interface setup and do start
    if (interface->if_down(interface) != 0) {
        return false;
    }
#ifdef PANA
    pana_reset_client_session();
#endif
    if (interface->if_up(interface) != 0) {
        return false;
    }

    if (!protocol_6lowpan_bootsrap_link_set(interface, PANDescriptor, beacon_payload, beacon_payload_length)) {
        return false;
    }

    //Start configured bootstrap
    return protocol_6lowpan_bootsrap_start(interface);
}

static void net_load_balance_link_reject(const protocol_interface_info_entry_t *cur)
{
    uint8_t ll_all_nodes[16];
    memcpy(ll_all_nodes, ADDR_LINK_LOCAL_ALL_NODES, 16);
    mle_service_interface_receiver_handler_update(cur->id, NULL);
    mle_service_reject_message_build(cur->id, ll_all_nodes, false);

    nd_router_t *nd_router = nd_get_object_by_nwk_id(cur->nwk_id);
    if (nd_router) {
        nd_router->nd_timer = 0;
        nd_router->ns_forward_timer = 0;
        nd_router->mle_advert_timer = 0;
    }
}

static bool net_load_balance_network_switch_notify_cb(const void *load_balance_user, load_balance_nwk_switch_operation operation, uint16_t *timeout)
{
    const protocol_interface_info_entry_t *cur = load_balance_user;
    switch (operation) {
        case LB_ROUTER_LEAVE:
            if (arm_nwk_6lowpan_rpl_dodag_poison(cur->id) == 0) {
                *timeout = 8;
            } else {
                *timeout = 0;
            }
            break;

        case LB_NEIGHBOUR_LEAVE:
            net_load_balance_link_reject(cur);
            *timeout = 4;
            break;
    }
    return true;
}
#endif

int8_t net_load_balance_network_switch_cb_set(int8_t interface_id, net_load_balance_network_switch_notify *network_switch_notify)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    return load_balance_network_switch_cb_set(interface_ptr->lb_api, network_switch_notify);
#else
    return -1;
#endif
}


int8_t net_load_balance_create(int8_t interface_id, bool  enable_periodic_beacon_interval)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr || !interface_ptr->mac_api) {
        return -1;
    } else if (interface_ptr->lb_api) {
        return -3;
    }
    //Allocate load balance user class
    load_balance_api_t *lb_api = load_balance_create(net_load_balance_network_switch_notify_cb, enable_periodic_beacon_interval);
    if (!lb_api) {
        return -2;
    }
    //Here initialize load balance based on current mac max beacon data size
    uint16_t beacon_payload_size;
    if (interface_ptr->mac_api->phyMTU > MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE) {
        beacon_payload_size = MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD;
    } else {
        beacon_payload_size = interface_ptr->mac_api->phyMTU - MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD;
    }

    if (lb_api->lb_initialize(lb_api, net_load_balance_beacon_tx, net_load_balance_priority_get, net_load_balance_network_switch_req, beacon_payload_size, interface_ptr) != 0) {
        load_balance_delete(lb_api);
        return -2;
    }

    //Store here load balance class pointer pointer
    interface_ptr->lb_api = lb_api;

    //Enable if
    if ((interface_ptr->lowpan_info & (INTERFACE_NWK_ACTIVE | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) == (INTERFACE_NWK_ACTIVE | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
        net_load_balance_internal_state_activate(interface_ptr, true);
    }

    return 0;
#else
    return -1;
#endif
}


int8_t net_load_balance_delete(int8_t interface_id)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }
    load_balance_api_t *lb_api = interface_ptr->lb_api;
    interface_ptr->lb_api = NULL;

    return load_balance_delete(lb_api);
#else
    return -1;
#endif
}

int8_t net_load_balance_threshold_set(int8_t interface_id, uint8_t threshold_min, uint8_t threshold_max)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    if (threshold_min > threshold_max) {
        return -1;
    }

    return load_balance_network_threshold_set(interface_ptr->lb_api, threshold_min, threshold_max);
#else
    return -1;
#endif
}

void net_load_balance_internal_state_activate(protocol_interface_info_entry_t *interface_ptr, bool state)
{
#ifdef HAVE_6LOWPAN_ND
    if (!interface_ptr || !interface_ptr->lb_api || !interface_ptr->mac_api) {
        return;
    }
    uint32_t trigle_period = 3600;
    uint32_t route_lifetime_period = 3600;
    //Enable Load Balance
#ifdef HAVE_RPL
    if (state && interface_ptr->rpl_domain) {
        struct rpl_instance *instance = rpl_control_lookup_instance(interface_ptr->rpl_domain, 1, NULL);
        if (instance) {
            const rpl_dodag_conf_t *dodag_config = rpl_control_get_dodag_config(instance);
            if (dodag_config) {
                //dio max Period caluclate in seconds
                uint32_t Imax_ms = (dodag_config->dio_interval_min + dodag_config->dio_interval_doublings) < 32 ?
                                   (1ul << (dodag_config->dio_interval_min + dodag_config->dio_interval_doublings)) : 0xfffffffful;
                trigle_period = Imax_ms / 1000;
                route_lifetime_period = (uint32_t)dodag_config->default_lifetime * dodag_config->lifetime_unit;

            }
        }
    }
#endif
    interface_ptr->lb_api->lb_enable(interface_ptr->lb_api, state, trigle_period, route_lifetime_period);
    mlme_set_t set_req;
    set_req.attr = macLoadBalancingAcceptAnyBeacon;
    set_req.attr_index = 0;
    set_req.value_pointer = &state;
    set_req.value_size = sizeof(bool);
    interface_ptr->mac_api->mlme_req(interface_ptr->mac_api, MLME_SET, &set_req);
#endif

}

#ifdef HAVE_RPL
#ifdef HAVE_6LOWPAN_BORDER_ROUTER
static int8_t net_load_balance_api_get_node_count_cb(void *lb_user, uint16_t *node_count)
{
    protocol_interface_info_entry_t *interface_ptr = lb_user;
    if (rpl_control_get_instance_dao_target_count(interface_ptr->rpl_domain, 1, NULL, NULL, node_count)) {
        return 0;
    }

    return -1;
}
#endif
#endif

#ifdef HAVE_RPL
#ifdef HAVE_6LOWPAN_BORDER_ROUTER
static int8_t net_load_balance_api_get_set_load_level_cb(void *lb_user, uint8_t load_level)
{
    //Call DODAG preference
    protocol_interface_info_entry_t *interface_ptr = lb_user;
    if (!interface_ptr->rpl_domain || interface_ptr->rpl_domain != protocol_6lowpan_rpl_domain || !protocol_6lowpan_rpl_root_dodag) {
        return -1;
    }

    if (load_level > RPL_DODAG_PREF_MASK) {
        load_level = RPL_DODAG_PREF_MASK;
    }

    rpl_control_set_dodag_pref(protocol_6lowpan_rpl_root_dodag, RPL_DODAG_PREF_MASK - load_level);
    return 0;
}
#endif
#endif

int8_t net_load_balance_load_level_update_enable(int8_t interface_id, uint16_t expected_device_count)
{
#ifdef HAVE_RPL
#ifdef HAVE_6LOWPAN_BORDER_ROUTER
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    uint16_t temp16 = expected_device_count % 8;
    if (temp16) {
        expected_device_count += (8 - temp16);
    }

    return load_balance_network_load_monitor_enable(interface_ptr->lb_api, expected_device_count, RPL_DODAG_PREF_MASK + 1, net_load_balance_api_get_node_count_cb, net_load_balance_api_get_set_load_level_cb);
#else
    (void)interface_id;
    (void)expected_device_count;
    return -1;
#endif
#else
    (void)interface_id;
    (void)expected_device_count;
    return -1;
#endif
}

/**
 * \brief Disable automatic network load level update
 *
 * \param interface_id interface id
 *
 * \return 0 process ok -1 Unknown interface id
 */
int8_t net_load_balance_load_level_update_disable(int8_t interface_id)
{
#ifdef HAVE_RPL
#ifdef HAVE_6LOWPAN_BORDER_ROUTER
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    return load_balance_network_load_monitor_disable(interface_ptr->lb_api);
#else
    (void)interface_id;
    return -1;
#endif
#else
    (void)interface_id;
    return -1;
#endif
}

int8_t net_load_balance_set_max_probability(int8_t interface_id, uint8_t max_p)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    return load_balance_set_max_probability(interface_ptr->lb_api, max_p);
#else
    return -1;
#endif
}
