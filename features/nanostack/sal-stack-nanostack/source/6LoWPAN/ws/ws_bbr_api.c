/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include <string.h>
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "net_interface.h"
#include "eventOS_event.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "ws_management_api.h"
#include "net_rpl.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"

#include "ws_bbr_api.h"

#define TRACE_GROUP "BBRw"

#define RPL_INSTANCE_ID 1

#ifdef HAVE_WS_BORDER_ROUTER

#define WS_ULA_LIFETIME 24*3600
#define WS_ROUTE_LIFETIME WS_ULA_LIFETIME
#define WS_DHCP_ADDRESS_LIFETIME 2*3600
#define BBR_CHECK_INTERVAL 60
#define BBR_BACKUP_ULA_DELAY 300

/* when creating BBR make ULA dodag ID always and when network becomes available add prefix to DHCP
 *
 *
 */
static int8_t backbone_interface_id = -1; // BBR backbone information
static uint16_t configuration = 0;

static uint8_t static_dodag_prefix[8] = {0xfd, 0x00, 0x61, 0x72, 0x6d};
static uint8_t current_dodag_id[16] = {0};
static uint8_t current_local_prefix[8] = {0};
static uint8_t current_global_prefix[8] = {0};
static uint32_t bbr_delay_timer = BBR_CHECK_INTERVAL; // initial delay.
static uint32_t global_prefix_unavailable_timer = 0; // initial delay.

static rpl_dodag_conf_t rpl_conf = {
    // Lifetime values
    .default_lifetime = 120,
    .lifetime_unit = 60,
    .objective_code_point = 1, // MRHOF algorithm used
    .authentication = 0,
    .path_control_size = 7,
    .dag_max_rank_increase = 2048,
    .min_hop_rank_increase = 196,
    // DIO configuration
    .dio_interval_min = WS_RPL_DIO_IMIN,
    .dio_interval_doublings = WS_RPL_DIO_DOUBLING,
    .dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY
};

void ws_bbr_rpl_config(uint8_t imin, uint8_t doubling, uint8_t redundancy)
{
    if (imin == 0 || doubling == 0) {
        // use default values
        imin = WS_RPL_DIO_IMIN;
        doubling = WS_RPL_DIO_DOUBLING;
        redundancy = WS_RPL_DIO_REDUNDANCY;
    }
    if (rpl_conf.dio_interval_min == imin &&
            rpl_conf.dio_interval_doublings == doubling &&
            rpl_conf.dio_redundancy_constant == redundancy) {
        // Same values no update needed
        return;
    }
    rpl_conf.dio_interval_min = imin;
    rpl_conf.dio_interval_doublings = doubling;
    rpl_conf.dio_redundancy_constant = redundancy;
    if (protocol_6lowpan_rpl_root_dodag) {
        rpl_control_update_dodag_config(protocol_6lowpan_rpl_root_dodag, &rpl_conf);
        rpl_control_increment_dodag_version(protocol_6lowpan_rpl_root_dodag);
    }
}

static void ws_bbr_rpl_root_start(uint8_t *dodag_id)
{
    tr_info("RPL root start");
    rpl_data_init_root();

    if (protocol_6lowpan_rpl_root_dodag) {
        rpl_control_delete_dodag_root(protocol_6lowpan_rpl_domain, protocol_6lowpan_rpl_root_dodag);
        protocol_6lowpan_rpl_root_dodag = NULL;
    }

    protocol_6lowpan_rpl_root_dodag = rpl_control_create_dodag_root(protocol_6lowpan_rpl_domain, RPL_INSTANCE_ID, dodag_id, &rpl_conf, rpl_conf.min_hop_rank_increase, RPL_GROUNDED | RPL_MODE_NON_STORING | RPL_DODAG_PREF(0));
    if (!protocol_6lowpan_rpl_root_dodag) {
        tr_err("RPL dodag init failed");
        return;
    }
    // RPL memory limits set larger for Border router
    rpl_control_set_memory_limits(64 * 1024, 0);
}

static void ws_bbr_rpl_root_stop(void)
{
    tr_info("RPL root stop");
    if (protocol_6lowpan_rpl_root_dodag) {
        rpl_control_delete_dodag_root(protocol_6lowpan_rpl_domain, protocol_6lowpan_rpl_root_dodag);
        protocol_6lowpan_rpl_root_dodag = NULL;
    }
    memset(current_local_prefix, 0, 8);
    memset(current_global_prefix, 0, 8);
    memset(current_dodag_id, 0, 16);
}

static int ws_border_router_proxy_validate(int8_t interface_id, uint8_t *address)
{

    /* Could also check route type, but I don't think it really matters */
    ipv6_route_t *route;
    route = ipv6_route_choose_next_hop(address, interface_id, NULL);
    if (!route || route->prefix_len < 128) {
        return -1;
    }

    return 0;
}

int ws_border_router_proxy_state_update(int8_t caller_interface_id, int8_t handler_interface_id, bool status)
{
    (void)caller_interface_id;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(handler_interface_id);
    if (!cur) {
        tr_error("No Interface");
        return -1;
    }

    tr_debug("Border router Backhaul link %s", status ? "ready" : "down");
    return 0;
}

static int ws_bbr_static_dodagid_create(protocol_interface_info_entry_t *cur)
{
    if (memcmp(current_dodag_id, ADDR_UNSPECIFIED, 16) != 0) {
        // address generated
        return 0;
    }
    // This address is only used if no other address available.
    if_address_entry_t *add_entry = icmpv6_slaac_address_add(cur, static_dodag_prefix, 64, 0xffffffff, 0xffffffff, true, SLAAC_IID_FIXED);
    if (!add_entry) {
        tr_err("dodagid create failed");
        return -1;
    }
    memcpy(current_dodag_id, add_entry->address, 16);
    tr_info("BBR generate DODAGID %s", trace_ipv6(current_dodag_id));
    addr_policy_table_add_entry(static_dodag_prefix, 64, 2, WS_NON_PREFFRED_LABEL);

    return 0;
}

/*
 * 0 static non rooted self generated own address
 * 1 static address with backbone connectivity
 */
static void ws_bbr_bb_static_prefix_get(uint8_t *dodag_id_ptr)
{

    /* Get static ULA prefix if we have configuration in backbone and there is address we use that.
     *
     * If there is no address we can use our own generated ULA as a backup ULA
     */

    protocol_interface_info_entry_t *bb_interface = protocol_stack_interface_info_get_by_id(backbone_interface_id);

    if (bb_interface && bb_interface->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
        if (protocol_address_prefix_cmp(bb_interface, bb_interface->ipv6_configure->static_prefix64, 64)) {
            memcpy(dodag_id_ptr, bb_interface->ipv6_configure->static_prefix64, 8);
        }
    }
    return;
}


static void ws_bbr_dodag_get(uint8_t *local_prefix_ptr, uint8_t *global_prefix_ptr)
{
    uint8_t global_address[16];

    memset(global_prefix_ptr, 0, 8);

    // By default static dodagID prefix is used as local prefix
    memcpy(local_prefix_ptr, current_dodag_id, 8);
    ws_bbr_bb_static_prefix_get(local_prefix_ptr);

    if (arm_net_address_get(backbone_interface_id, ADDR_IPV6_GP, global_address) != 0) {
        // No global prefix available
        return;
    }
    protocol_interface_info_entry_t *bb_interface = protocol_stack_interface_info_get_by_id(backbone_interface_id);
    if_address_entry_t *addr_entry = addr_get_entry(bb_interface, global_address);

    if (!addr_entry || addr_entry->preferred_lifetime == 0) {
        return;
    }
    //tr_debug("BBR address %s lifetime %d pref %d", trace_ipv6(addr_entry->address), addr_entry->valid_lifetime, addr_entry->preferred_lifetime);

    if (memcmp(global_address, local_prefix_ptr, 8) == 0) {
        // static prefix is same
        return;
    }
    memcpy(global_prefix_ptr, global_address, 8);
    return;
}
static void wisun_bbr_na_send(int8_t interface_id, const uint8_t target[static 16])
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }
    // Send NA only if it is enabled for the backhaul
    if (!cur->send_na) {
        return;
    }

    buffer_t *buffer = icmpv6_build_na(cur, false, true, true, target, NULL, ADDR_UNSPECIFIED);
    protocol_push(buffer);
    return;

}

static bool wisun_dhcp_address_add_cb(int8_t interfaceId, dhcp_address_cache_update_t *address_info, void *route_src)
{
    (void)route_src;
    protocol_interface_info_entry_t *curPtr = protocol_stack_interface_info_get_by_id(interfaceId);
    if (!curPtr) {
        return false;
    }

    // When address is allocated we send NA to backbone to notify the new address and flush from other BBRs
    wisun_bbr_na_send(backbone_interface_id, address_info->allocatedAddress);
    return true;
}

static void ws_bbr_dhcp_server_start(protocol_interface_info_entry_t *cur, uint8_t *global_id)
{
    uint8_t ll[16];
    memcpy(ll, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&ll[8], cur->mac, 8);
    ll[8] ^= 2;

    tr_debug("DHCP server activate %s", trace_ipv6_prefix(global_id, 64));

    if (DHCPv6_server_service_init(cur->id, global_id, cur->mac, DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) != 0) {
        tr_error("DHCPv6 Server create fail");
        return;
    }
    DHCPv6_server_service_callback_set(cur->id, global_id, NULL, wisun_dhcp_address_add_cb);

    DHCPv6_server_service_set_address_autonous_flag(cur->id, global_id, true);
    DHCPv6_server_service_set_address_validlifetime(cur->id, global_id, WS_DHCP_ADDRESS_LIFETIME);

    ws_dhcp_client_address_request(cur, global_id, ll);
}
static void ws_bbr_dhcp_server_stop(protocol_interface_info_entry_t *cur, uint8_t *global_id)
{
    tr_debug("DHCP server deactivate %s", trace_ipv6(global_id));
    DHCPv6_server_service_delete(cur->id, global_id, false);

}

static void ws_bbr_rpl_status_check(protocol_interface_info_entry_t *cur)
{

    uint8_t local_prefix[8] = {0};
    uint8_t global_prefix[8] = {0};

    //tr_info("BBR status check");

    /*
     * Start RPL Root
     */
    if (!protocol_6lowpan_rpl_root_dodag) {
        // Generate DODAGID
        if (ws_bbr_static_dodagid_create(cur) == 0) {
            ws_bbr_rpl_root_start(current_dodag_id);
        }
    }

    if (!protocol_6lowpan_rpl_root_dodag) {
        // Failed to start
        tr_err("BBR failed to start");
        return;
    }

    /*
     * Check that DODAGID is still valid
     */
    if (protocol_interface_address_compare(current_dodag_id) != 0) {
        //DODAGID is lost need to restart
        tr_warn("DODAGID lost restart RPL");
        memset(current_dodag_id, 0, 16);
        ws_bbr_rpl_root_stop();
        return;
    }

    ws_bbr_dodag_get(local_prefix, global_prefix);
    tr_debug("BBR global %s, local %s", trace_ipv6_prefix(global_prefix, 64), trace_ipv6_prefix(local_prefix, 64));

    /*
     * Add default route to RPL
     */
    rpl_control_update_dodag_route(protocol_6lowpan_rpl_root_dodag, NULL, 0, 0, WS_ROUTE_LIFETIME, false);

    /*
     * Create static ULA configuration or modify if needed
     */
    if ((configuration & BBR_ULA_C) &&
            memcmp(current_local_prefix, local_prefix, 8) != 0) {
        // Generate Static ULA
        // Start static ULA prefix and routing always
        if (memcmp(current_local_prefix, ADDR_UNSPECIFIED, 8) != 0) {
            // Remove Old ULA prefix
            rpl_control_update_dodag_prefix(protocol_6lowpan_rpl_root_dodag, current_local_prefix, 64, PIO_A, 0, 0, true);
        }
        if (memcmp(local_prefix, ADDR_UNSPECIFIED, 8) != 0) {
            tr_info("RPL Local prefix activate %s", trace_ipv6_prefix(local_prefix, 64));
            rpl_control_update_dodag_prefix(protocol_6lowpan_rpl_root_dodag, local_prefix, 64, PIO_A, WS_ULA_LIFETIME, WS_ULA_LIFETIME, false);
            memcpy(current_local_prefix, local_prefix, 8);
        }
    }

    /*
     * Check if backup ULA prefix is needed
     */
    if (memcmp(global_prefix, ADDR_UNSPECIFIED, 8) == 0) {
        //Global prefix not available count if backup ULA should be created
        global_prefix_unavailable_timer += BBR_CHECK_INTERVAL;
        tr_debug("Check for backup prefix %"PRIu32"", global_prefix_unavailable_timer);
        if (global_prefix_unavailable_timer >= BBR_BACKUP_ULA_DELAY) {
            if (memcmp(current_global_prefix, ADDR_UNSPECIFIED, 8) == 0) {
                tr_info("start using backup prefix %s", trace_ipv6_prefix(local_prefix, 64));
            }
            memcpy(global_prefix, local_prefix, 8);
        }
    } else {
        //Global connection OK
        global_prefix_unavailable_timer = 0;
    }

    /*
     * Check for Global prefix
     */
    if (memcmp(current_global_prefix, global_prefix, 8) != 0) {
        // Global prefix changed
        if (memcmp(current_global_prefix, ADDR_UNSPECIFIED, 8) != 0) {
            // TODO remove old global prefix
            tr_info("RPL global prefix deactivate %s", trace_ipv6_prefix(current_global_prefix, 64));

            // Old backbone information is deleted after 120 seconds
            rpl_control_update_dodag_prefix(protocol_6lowpan_rpl_root_dodag, current_global_prefix, 64, 0, 0, 0, true);
            if (configuration & BBR_GUA_ROUTE) {
                rpl_control_update_dodag_route(protocol_6lowpan_rpl_root_dodag, current_global_prefix, 64, 0, 0, true);
            }
            ipv6_route_add_with_info(current_global_prefix, 64, backbone_interface_id, NULL, ROUTE_THREAD_BBR, NULL, 0, 120, 0);

            ws_bbr_dhcp_server_stop(cur, current_global_prefix);
        }
        // TODO add global prefix
        if (memcmp(global_prefix, ADDR_UNSPECIFIED, 8) != 0) {

            tr_info("RPL global prefix activate %s", trace_ipv6_prefix(global_prefix, 64));
            // Add default route to RPL
            // Enable default routing to backbone
            if (ipv6_route_add_with_info(global_prefix, 64, backbone_interface_id, NULL, ROUTE_THREAD_BBR, NULL, 0, 0xffffffff, 0) == NULL) {
                tr_err("global route add failed");
                return;
            }
            ws_bbr_dhcp_server_start(cur, global_prefix);
            rpl_control_update_dodag_prefix(protocol_6lowpan_rpl_root_dodag, global_prefix, 64, 0, 0, 0, false);
            // no check for failure should have

            if (configuration & BBR_GUA_ROUTE) {
                // Add also global prefix and route to RPL
                rpl_control_update_dodag_route(protocol_6lowpan_rpl_root_dodag, global_prefix, 64, 0, WS_ROUTE_LIFETIME, false);
            }

        }
        memcpy(current_global_prefix, global_prefix, 8);
        rpl_control_increment_dodag_version(protocol_6lowpan_rpl_root_dodag);
        nd_proxy_downstream_interface_register(cur->id, ws_border_router_proxy_validate, ws_border_router_proxy_state_update);
    } else if (memcmp(current_global_prefix, ADDR_UNSPECIFIED, 8) != 0) {
        /*
         *  This is a keep alive validation RPL is updated to hold the real info.
         *  There is no status checks on prefix adds so this makes sure they are not lost
         *  DHCP validation should be done also
         */
        rpl_control_update_dodag_prefix(protocol_6lowpan_rpl_root_dodag, current_global_prefix, 64, 0, 0, 0, false);

        if (configuration & BBR_GUA_ROUTE) {
            // Add also global prefix and route to RPL
            rpl_control_update_dodag_route(protocol_6lowpan_rpl_root_dodag, current_global_prefix, 64, 0, WS_ROUTE_LIFETIME, false);
        }
    }
}

void ws_bbr_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    (void)seconds;

    if (!ws_info(cur)) {
        return;
    }
    if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        // Not a border router
        return;
    }
    if (!cur->rpl_domain) {
        // RPL not started
        return;
    }

    if (bbr_delay_timer > seconds) {
        bbr_delay_timer -= seconds;
    } else {
        bbr_delay_timer = BBR_CHECK_INTERVAL; // 20 second interval between status checks

        // prequisists
        // Wi-SUN network configuration started without RPL

        // RPL configured simple
        // 1. Wait for backend connection
        // 2. When address becomes available in backend start RPL dodag
        // 3. if address removed remove dodag

        // RPL configured Advanced
        // 1. Add ULA DODAG and and start ROOT even without backend
        //   a. If static prefix configured use it.
        //   b. generate random ULA and publish it to backend
        // 2. if GUA prefix becomes available in backend add new prefix to DODAG
        // 3. if GUA prefix is removed remove the prefix.

        ws_bbr_rpl_status_check(cur);

    }
    // Normal BBR operation
    if (protocol_6lowpan_rpl_root_dodag) {
        if (cur->ws_info->pan_version_timer > seconds) {
            cur->ws_info->pan_version_timer -= seconds;
        } else {
            // Border router has timed out
            tr_debug("Border router version number update");
            cur->ws_info->pan_version_timer = ws_common_version_lifetime_get(cur->ws_info->network_size_config);
            cur->ws_info->pan_information.pan_version++;
            // Inconsistent for border router to make information distribute faster
            ws_bootstrap_configuration_trickle_reset(cur);

            if (cur->ws_info->network_size_config == NETWORK_SIZE_AUTOMATIC) {
                ws_common_network_size_configure(cur, cur->ws_info->pan_information.pan_size);
            }
            // We update the RPL version in same time to allow nodes to reselect parent
            // As configuration is made so that devices cant move downward in dodag this allows it
            // Version number update is only done if DoDAG MAX Rank Increase parameter is 0
            if (rpl_conf.dag_max_rank_increase == 0 && cur->ws_info->pan_information.pan_version && cur->ws_info->pan_information.pan_version % RPL_VERSION_LIFETIME / cur->ws_info->pan_version_timer  == 0) {
                // Third the rate of configuration version change at default 5 hours
                rpl_control_increment_dodag_version(protocol_6lowpan_rpl_root_dodag);
            }
        }

    }

}

uint16_t test_pan_size_override = 0xffff;

uint16_t ws_bbr_pan_size(protocol_interface_info_entry_t *cur)
{
    uint16_t result = 0;
    if (test_pan_size_override != 0xffff) {
        return test_pan_size_override;
    }
    //
    const uint8_t *prefix_ptr;
    if (memcmp(current_global_prefix, ADDR_UNSPECIFIED, 8) != 0) {
        //Use GUA Prefix
        prefix_ptr = current_global_prefix;
    } else {
        //Use ULA for indentifier
        prefix_ptr = current_local_prefix;
    }

    rpl_control_get_instance_dao_target_count(cur->rpl_domain, RPL_INSTANCE_ID, NULL, prefix_ptr, &result);
    if (result > 0) {
        // remove the Border router from the PAN size
        result--;
    }
    return result;
}

bool ws_bbr_ready_to_start(protocol_interface_info_entry_t *cur)
{

    (void)cur;
    uint8_t global_address[16];

    if (backbone_interface_id < 0) {
        // No need to wait for backbone
        return true;
    }

    if ((configuration & BBR_BB_WAIT) != BBR_BB_WAIT) {
        // No need to wait for backbone
        return true;
    }

    if (arm_net_address_get(backbone_interface_id, ADDR_IPV6_GP, global_address) != 0) {
        // No global prefix available
        return false;
    }

    return true;
}
#endif //HAVE_WS_BORDER_ROUTER

/* Public APIs
 *
 */

int ws_bbr_start(int8_t interface_id, int8_t bb_interface_id)
{
#ifdef HAVE_WS_BORDER_ROUTER

    (void)interface_id;
    protocol_interface_info_entry_t *bb_interface = protocol_stack_interface_info_get_by_id(bb_interface_id);

    if (!bb_interface) {
        return -1;
    }
    // TODO make bb configurations

    backbone_interface_id = bb_interface_id;

    return 0;
#else
    (void)interface_id;
    (void)bb_interface_id;
    return -1;
#endif
}
void ws_bbr_stop(int8_t interface_id)
{
#ifdef HAVE_WS_BORDER_ROUTER

    (void)interface_id;
    backbone_interface_id = -1;

    if (!protocol_6lowpan_rpl_domain) {
        return;
    }

    rpl_control_delete_dodag_root(protocol_6lowpan_rpl_domain, protocol_6lowpan_rpl_root_dodag);
    protocol_6lowpan_rpl_root_dodag = NULL;

#else
    (void)interface_id;
#endif
}
int ws_bbr_configure(int8_t interface_id, uint16_t options)
{
#ifdef HAVE_WS_BORDER_ROUTER

    (void)interface_id;
    if (protocol_6lowpan_rpl_root_dodag &&
            options != configuration) {
        //Configuration changed delete previus setup
        ws_bbr_rpl_root_stop();
    }
    configuration = options;
    return 0;
#else
    (void)interface_id;
    (void)options;
    return -1;
#endif
}

int ws_bbr_node_keys_remove(int8_t interface_id, uint8_t *eui64)
{
    (void) interface_id;
    (void) eui64;
#ifdef HAVE_WS_BORDER_ROUTER
    return ws_pae_controller_node_keys_remove(interface_id, eui64);
#else
    return -1;
#endif
}

int ws_bbr_node_access_revoke_start(int8_t interface_id)
{
    (void) interface_id;
#ifdef HAVE_WS_BORDER_ROUTER
    return ws_pae_controller_node_access_revoke_start(interface_id);
#else
    return -1;
#endif
}

int ws_bbr_eapol_node_limit_set(int8_t interface_id, uint16_t limit)
{
    (void) interface_id;
#ifdef HAVE_WS_BORDER_ROUTER
    return ws_pae_controller_node_limit_set(interface_id, limit);
#else
    (void) limit;
    return -1;
#endif
}
