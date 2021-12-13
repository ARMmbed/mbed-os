/*
 * Copyright (c) 2021, Pelion and affiliates.
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
#ifdef HAVE_WS
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "net_interface.h"
#include "eventOS_event.h"
#include "randLIB.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "sw_mac.h"
#include "ccmLIB.h"
#include "Core/include/ns_monitor.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/MAC/mac_ie_lib.h"
#include "MPL/mpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "RPL/rpl_policy.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/ip.h"
#include "Service_Libs/Trickle/trickle.h"
#include "Service_Libs/fhss/channel_list.h"
#include "Service_Libs/utils/ns_time.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_llc.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "6LoWPAN/ws/ws_ie_lib.h"
#include "6LoWPAN/ws/ws_stats.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_bootstrap_ffn.h"
#include "6LoWPAN/ws/ws_bootstrap_6lbr.h"
#include "6LoWPAN/ws/ws_bootstrap_6lr.h"
#include "6LoWPAN/ws/ws_bootstrap_6ln.h"
#include "6LoWPAN/ws/ws_phy.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "platform/topo_trace.h"
#include "dhcp_service_api.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_vendordata.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "ws_management_api.h"
#include "net_rpl.h"
#include "mac_api.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"
#include "6LoWPAN/ws/ws_eapol_auth_relay.h"
#include "6LoWPAN/ws/ws_eapol_relay.h"
#include "libNET/src/net_dns_internal.h"
#include "Service_Libs/random_early_detection/random_early_detection_api.h"

#define TRACE_GROUP "wsbs"

static void ws_bootstrap_ffn_dhcp_neighbour_update_cb(int8_t interface_id, uint8_t ll_addr[static 16]);
static void ws_bootstrap_ffn_dhcp_info_notify_cb(int8_t interface, dhcp_option_notify_t *options, dhcp_server_notify_info_t *server_info);

int8_t ws_bootstrap_ffn_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;

    if (!cur) {
        return -1;
    }

    if ((cur->configure_flags & INTERFACE_SETUP_MASK) != INTERFACE_SETUP_READY) {
        tr_error("Interface not yet fully configured");
        return -2;
    }
    if (ws_bootstrap_fhss_initialize(cur) != 0) {
        tr_error("fhss initialization failed");
        return -3;
    }
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //BBR init like NVM read
        ws_bbr_init(cur);
    }
    // Save FHSS api
    cur->ws_info->fhss_api = ns_sw_mac_get_fhss_api(cur->mac_api);

    ws_bootstrap_ll_address_validate(cur);

    addr_interface_set_ll64(cur, NULL);
    cur->nwk_nd_re_scan_count = 0;
    // Trigger discovery for bootstrap
    ret_val = nwk_6lowpan_up(cur);
    if (ret_val) {
        goto cleanup;
    }

    /* Wi-sun will trig event for stamechine this timer must be zero on init */
    cur->bootsrap_state_machine_cnt = 0;
    /* Disable SLLAO send/mandatory receive with the ARO */
    cur->ipv6_neighbour_cache.use_eui64_as_slla_in_aro = true;
    /* Omit sending of NA if ARO SUCCESS */
    cur->ipv6_neighbour_cache.omit_na_aro_success = true;
    /* Omit sending of NA and consider ACK to be success */
    cur->ipv6_neighbour_cache.omit_na = true;
    // do not process AROs from NA. This is overriden by Wi-SUN specific failure handling
    cur->ipv6_neighbour_cache.recv_na_aro = false;
    /* Disable NUD Probes */
    cur->ipv6_neighbour_cache.send_nud_probes = false;
    cur->ipv6_neighbour_cache.probe_avoided_routers = true;
    /*Replace NS handler to disable multicast address queries */
    cur->if_ns_transmit = ws_bootstrap_nd_ns_transmit;

    dhcp_client_init(cur->id, DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE);
    dhcp_service_link_local_rx_cb_set(cur->id, ws_bootstrap_ffn_dhcp_neighbour_update_cb);
    dhcp_client_configure(cur->id, true, true, true); //RENEW uses SOLICIT, Interface will use 1 instance for address get, IAID address hint is not used.

    dhcp_client_solicit_timeout_set(cur->id, WS_DHCP_SOLICIT_TIMEOUT, WS_DHCP_SOLICIT_MAX_RT, WS_DHCP_SOLICIT_MAX_RC, WS_DHCP_SOLICIT_MAX_DELAY);
    dhcp_client_option_notification_cb_set(cur->id, ws_bootstrap_ffn_dhcp_info_notify_cb);

    // Configure memory limits and garbage collection values;
    ws_bootstrap_memory_configuration();
    ws_nud_table_reset(cur);

    ws_bootstrap_candidate_table_reset(cur);
    // Zero uptime counters
    cur->ws_info->uptime = 0;
    cur->ws_info->authentication_time = 0;
    cur->ws_info->connected_time = 0;

    blacklist_params_set(
        WS_BLACKLIST_ENTRY_LIFETIME,
        WS_BLACKLIST_TIMER_MAX_TIMEOUT,
        WS_BLACKLIST_TIMER_TIMEOUT,
        WS_BLACKLIST_ENTRY_MAX_NBR,
        WS_BLACKLIST_PURGE_NBR,
        WS_BLACKLIST_PURGE_TIMER_TIMEOUT);

    ws_bootstrap_event_discovery_start(cur);

    return 0;
cleanup:
    return ret_val;
}

int8_t ws_bootstrap_ffn_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    tr_info("Wi-SUN ifdown");
    // Reset MAC for safe upper layer memory free
    protocol_mac_reset(cur);
    ns_sw_mac_fhss_unregister(cur->mac_api);
    ns_fhss_delete(cur->ws_info->fhss_api);
    cur->ws_info->fhss_api = NULL;
    // Reset WS information
    ws_bootstrap_asynch_trickle_stop(cur);
    ws_llc_reset(cur);
    if (nd_proxy_downstream_interface_unregister(cur->id) != 0) {
        tr_warn("nd proxy unregister failed");
    }
    ws_nud_table_reset(cur);
    dhcp_client_delete(cur->id);
    ws_eapol_relay_delete(cur);
    ws_eapol_auth_relay_delete(cur);
    ws_pae_controller_stop(cur);
    ws_bootstrap_candidate_table_reset(cur);
    blacklist_clear();
    cur->if_common_forwarding_out_cb = NULL;

    return nwk_6lowpan_down(cur);
}

static void ws_bootstrap_ffn_dhcp_neighbour_update_cb(int8_t interface_id, uint8_t ll_addr[static 16])
{
    if (memcmp(ll_addr, ADDR_LINK_LOCAL_PREFIX, 8)) {
        return;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    uint8_t mac64[8];
    memcpy(mac64, ll_addr + 8, 8);
    mac64[0] ^= 2;
    ws_bootstrap_mac_neighbor_short_time_set(cur, mac64, WS_NEIGHBOUR_DHCP_ENTRY_LIFETIME);
}

static void ws_bootstrap_ffn_dhcp_info_notify_cb(int8_t interface, dhcp_option_notify_t *options, dhcp_server_notify_info_t *server_info)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface);
    if (!cur) {
        return;
    }
    uint8_t server_ll64[16];
    memcpy(server_ll64, ADDR_LINK_LOCAL_PREFIX, 8);

    if (server_info->duid_length == 8) {
        memcpy(server_ll64 + 8, server_info->duid, 8);
    } else {
        server_ll64[8] = server_info->duid[0];
        server_ll64[9] = server_info->duid[1];
        server_ll64[10] = server_info->duid[2];
        server_ll64[11] = 0xff;
        server_ll64[12] = 0xfe;
        server_ll64[13] = server_info->duid[3];
        server_ll64[14] = server_info->duid[4];
        server_ll64[15] = server_info->duid[5];
    }
    server_ll64[8] ^= 2;

    switch (options->option_type) {
        case DHCPV6_OPTION_VENDOR_SPECIFIC_INFO:
            if (options->option.vendor_spesific.enterprise_number != ARM_ENTERPRISE_NUMBER) {
                break;
            }
            while (options->option.vendor_spesific.data_length) {
                uint16_t option_type;
                char *domain;
                uint8_t *address;
                uint16_t option_len;
                option_len = net_dns_option_vendor_option_data_get_next(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &option_type);
                tr_debug("DHCP vendor specific data type:%u length %d", option_type, option_len);
                //tr_debug("DHCP vendor specific data %s", trace_array(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length));

                if (option_len == 0) {
                    // Option fields were corrupted
                    break;
                }
                if (option_type == ARM_DHCP_VENDOR_DATA_DNS_QUERY_RESULT) {
                    // Process ARM DNS query result
                    domain = NULL;
                    address = NULL;
                    if (net_dns_option_vendor_option_data_dns_query_read(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &address, &domain) > 0 ||
                            domain || address) {
                        // Valid ARM DNS query entry
                        net_dns_query_result_set(interface, address, domain, server_info->life_time);
                    }
                }
                if (option_type == ARM_DHCP_VENDOR_DATA_TIME_CONFIGURATION) {
                    timezone_info_t time_configuration;
                    if (net_vendor_option_time_configuration_read(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &time_configuration.timestamp, &time_configuration.timezone, &time_configuration.deviation, &time_configuration.status)) {
                        int ret = ns_time_system_timezone_info_notify(&time_configuration);
                        tr_info("Network Time configuration %s status:%"PRIu16" time stamp: %"PRIu64" deviation: %"PRId16" Time Zone: %"PRId16, ret == 0 ? "notified" : "notify FAILED", time_configuration.status, time_configuration.timestamp, time_configuration.deviation, time_configuration.timezone);
                    }
                }
                if (option_type == ARM_DHCP_VENDOR_DATA_NETWORK_TIME) {
                    // Process ARM Network Time
                    // Get Current time
                    // Get Round trip time of the DHCP request
                    // Estimated error is elapsed time of request
                    // If current time difference is larger than estimated error update current time
                    //     set the time for server time + *.5 RTT
                    int32_t era;
                    uint32_t offset;
                    if (net_vendor_option_current_time_read(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &era, &offset, NULL)) {
                        uint64_t current_time;
                        uint64_t network_time = (era * (uint64_t)(4294967296)) + offset - 2208988800; //Convert to First day of Unix (1 Jan 1970)

                        tr_debug("Network Time option Era:%"PRId32" Offset:%"PRIu32" rtt: %"PRId32" time: %"PRIu64,  era, offset, server_info->rtt, network_time);
                        if (0 == ns_time_system_time_read(&current_time)) {
                            uint64_t difference;
                            // We only adjust clock if time has drifted more than 10 seconds to avoid constant changing of time
                            // If Round trip time is very high the accuracy is reduced.
                            uint32_t estimated_error = 10 + server_info->rtt / 10;
                            // Take into account the round trip time it took the response to arrive from the time server Write the time.
                            network_time += server_info->rtt / 20;

                            if (current_time > network_time) {
                                difference = current_time - network_time;
                            } else {
                                difference = network_time - current_time;
                            }
                            if (difference > estimated_error) {
                                // Larger than 10 second difference update the time
                                int ret = ns_time_system_time_write(network_time);
                                tr_info("Network Time %s: Era:%"PRId32" Offset:%"PRIu32" old time: %"PRIu64" time: %"PRIu64, ret == 0 ? "updated" : "update FAILED", era, offset, current_time, network_time);
                            }
                            // System time has been acquired
                            ns_time_system_time_acquired_set();
                        }
                    }
                }

                options->option.vendor_spesific.data_length -= option_len;
                options->option.vendor_spesific.data += option_len;
            }
            break;

        case DHCPV6_OPTION_DNS_SERVERS:
            while (options->option.generic.data_length && options->option.generic.data_length >= 16 && options->option.generic.data_length % 16 == 0) {
                // Validate payload to have full 16 byte length addresses without any extra bytes
                net_dns_server_address_set(interface, server_ll64, options->option.generic.data, server_info->life_time);
                options->option.generic.data_length -= 16;
                options->option.generic.data += 16;
            }
            break;
        case DHCPV6_OPTION_DOMAIN_LIST:
            net_dns_server_search_list_set(interface, server_ll64, options->option.generic.data, options->option.generic.data_length, server_info->life_time);
            break;
        default:
            break;
    }

}

static void ws_dhcp_client_global_adress_cb(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status)
{
    (void)prefix;
    (void)interface;
    //TODO add handler for negative status
    tr_debug("DHCPv6 %s status %u with link %s", trace_ipv6(prefix), register_status, trace_ipv6(dhcp_addr));
    if (register_status) {
        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface);
        if (cur) {
            ws_address_reregister_trig(cur);
        }
    } else {
        //Delete dhcpv6 client
        dhcp_client_global_address_delete(interface, dhcp_addr, prefix);
    }
}


void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local)
{
    if (dhcp_client_get_global_address(cur->id, parent_link_local, prefix, ws_dhcp_client_global_adress_cb) != 0) {
        tr_error("DHCPp client request fail");
    }
}

void ws_dhcp_client_address_delete(protocol_interface_info_entry_t *cur, uint8_t *prefix)
{
    dhcp_client_global_address_delete(cur->id, NULL, prefix);
}

void ws_bootstrap_ffn_rpl_configure(protocol_interface_info_entry_t *cur)
{
    tr_debug("RPL Activate");
#ifdef HAVE_RPL
    bool downstream = true;
    bool leaf = false;
#endif

    addr_add_router_groups(cur);
#ifdef HAVE_RPL
    rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, downstream);
    // If i am router I Do this
    rpl_control_force_leaf(protocol_6lowpan_rpl_domain, leaf);
    rpl_control_process_routes(protocol_6lowpan_rpl_domain, false); // Wi-SUN assumes that no default route needed
    rpl_control_request_parent_link_confirmation(true);
    rpl_control_set_dio_multicast_min_config_advertisment_count(WS_MIN_DIO_MULTICAST_CONFIG_ADVERTISMENT_COUNT);
    rpl_control_set_address_registration_timeout((WS_NEIGHBOR_LINK_TIMEOUT / 60) + 1);
    rpl_control_set_dao_retry_count(WS_MAX_DAO_RETRIES);
    rpl_control_set_initial_dao_ack_wait(WS_MAX_DAO_INITIAL_TIMEOUT);
    rpl_control_set_mrhof_parent_set_size(WS_MAX_PARENT_SET_COUNT);
    rpl_control_set_force_tunnel(true);
    // Set RPL Link ETX Validation Threshold to 2.5 - 33.0
    // This setup will set ETX 0x800 to report ICMP error 18% probability
    // When ETX start go over 0x280 forward dropping probability start increase  linear to 100% at 0x2100
    rpl_policy_forward_link_etx_threshold_set(0x280, 0x2100);

    // Set the minimum target refresh to sen DAO registrations before pan timeout
    rpl_control_set_minimum_dao_target_refresh(WS_RPL_DAO_MAX_TIMOUT);
#endif // HAVE_RPL 

    cur->ws_info->rpl_state = 0xff; // Set invalid state and learn from event
}
#endif //HAVE_WS
