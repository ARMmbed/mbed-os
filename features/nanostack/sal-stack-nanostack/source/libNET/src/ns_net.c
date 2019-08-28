/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
/**
 * \file net.c
 * \brief Network API for library model
 *
 * The network API functions for library model
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_scheduler.h"
#include "string.h"
#include "ns_trace.h"
#include "socket_api.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Core/include/ns_socket.h"
#ifdef HAVE_RPL
#include "RPL/rpl_of0.h"
#include "RPL/rpl_mrhof.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#endif
#include "ccmLIB.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/beacon_handler.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "net_pana_parameters_api.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Security/Common/sec_lib_definitions.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "net_thread_test.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#ifdef HAVE_WS
#include "6LoWPAN/ws/ws_pae_controller.h"
#endif
#include "BorderRouter/border_router.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "sw_mac.h"
#include "mac_api.h"
#include "ethernet_mac_api.h"
#include <stdarg.h>


#define TRACE_GROUP "lNet"
/**
 * \brief A function checks that the channel list is not empty. Channel pages 9 and 10 can have eight 32-bit channel masks.
 * \param scan_list is a pointer to the channel list structure given by the application.
 * \return 0 on success.
 * \return -1 if channel list is empty.
 */
static int arm_channel_list_validation(const channel_list_s *scan_list)
{
    uint8_t i = 1;
    if (scan_list) {
        if (scan_list->channel_page == CHANNEL_PAGE_9 || scan_list->channel_page == CHANNEL_PAGE_10) {
            i = 8;
        }
        while (i--)
            if (scan_list->channel_mask[i]) {
                return 0;
            }
    }
    return -1;
}


/* Energy & Active Scan API */
int8_t arm_net_energy_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t *conf), uint8_t energy_tresshold)
{
    (void)interface_id;
    (void)scan_list;
    (void)passed_fptr;
    (void)energy_tresshold;
    int8_t ret_val = -3;
#ifdef HAVE_RF_TUNNEL
    (void)interface_id;
    (void)scan_list;
    (void)passed_fptr;
    (void)energy_tresshold;
#else
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            ret_val = -1;
        } else {

            nwk_scan_params_t *scan_params = 0;
            if (cur->mac_parameters) {
                scan_params = &cur->mac_parameters->nwk_scan_params;
                scan_params->stack_chan_list = *scan_list;
                scan_params->energy_treshold = energy_tresshold;

                mlme_scan_t req;
                mac_create_scan_request(MAC_ED_SCAN_TYPE, &cur->mac_parameters->nwk_scan_params.stack_chan_list, 5, &req);
                if (cur->mac_api) {
                    cur->scan_cb = passed_fptr;
                    cur->mac_api->mlme_req(cur->mac_api, MLME_SCAN, &req);
                    ret_val = 0;
                }
            }
        }
    }
#endif
    return ret_val;

}

int8_t arm_net_nwk_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t *conf), uint8_t scan_level)
{
    (void)interface_id;
    (void)scan_list;
    (void)passed_fptr;
    (void)scan_level;
    int8_t ret_val = -3;
#ifdef HAVE_RF_TUNNEL
    (void)interface_id;
    (void)scan_list;
    (void)passed_fptr;
    (void)scan_level;
#else
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (cur) {
        cur->scan_cb = passed_fptr;
        if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
            if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
                ret_val = -1;
            } else if (arm_channel_list_validation(scan_list)) {
                tr_debug("Given channel mask is empty!\n");
                ret_val = -2;
            } else {
                nwk_scan_params_t *scan_params = &cur->mac_parameters->nwk_scan_params;
                scan_params->stack_chan_list = *scan_list;

                mlme_scan_t req;
                mac_create_scan_request(MAC_ACTIVE_SCAN, &scan_params->stack_chan_list, 5, &req);
                if (cur->mac_api) {
                    cur->scan_cb = passed_fptr;
                    scan_params->active_scan_active = true;
                    cur->mac_api->mlme_req(cur->mac_api, MLME_SCAN, &req);
                }
                filter->nwk_active_scan_level = scan_level;
                mac_helper_nwk_id_filter_set(0, filter);
                mac_helper_mac16_address_set(cur, 0xffff);
                protocol_6lowpan_register_handlers(cur);
                ret_val = 0;

            }
        }
    }
#endif
    return ret_val;
}

nwk_pan_descriptor_t *arm_net_get_scanned_nwk_list(int8_t interface_id)
{
    nwk_pan_descriptor_t *ret_val = 0;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->mac_parameters) {
            ret_val = cur->mac_parameters->nwk_scan_params.nwk_response_info;
        }
    }
    return ret_val;
}

/**
 * \brief A function to read pan ID filter.
 * \return 16-bit value indicating a pan ID filter.
 */
uint16_t arm_net_get_nwk_pan_id_filter(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
            return filter->net_pan_id_filter;
        }
    }
    return 0;
}

/**
 * \brief A function to read network layer configurations.
 * \param network_params is a pointer to the structure to where the network layer configs are written to.
 * \return 0 on success.
 * \return Negative value if interface id or PAN coordinator is not known.
 */
int8_t arm_nwk_param_read(int8_t interface_id, link_layer_setups_s *network_params)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    addrtype_t addrType = mac_helper_coordinator_address_get(cur, network_params->address);
    if (addrType == ADDR_NONE) {
        return -2;
    }
    network_params->PANId = mac_helper_panid_get(cur);
    if (addrType == ADDR_802_15_4_SHORT) {
        network_params->addr_mode = ADDR_MAC_SHORT16;
    } else {
        network_params->addr_mode = ADDR_MAC_LONG64;
    }

    network_params->LogicalChannel = cur->mac_parameters->mac_channel;
    network_params->sf = 0xff;
    return 0;
}

/**
 * \brief A function to read MAC PAN-ID, Short address & EUID64
 * \param mac_params is a pointer to the structure to where the mac address are written to.
 * \return 0 on success.
 * \return Negative value if interface id is not known.
 */
int8_t arm_nwk_mac_address_read(int8_t interface_id, link_layer_address_s *mac_params)
{
    int8_t ret_val = -2;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        ret_val = 0;
        memcpy(mac_params->mac_long, cur->mac, 8);
        memcpy(mac_params->iid_eui64, cur->iid_eui64, 8);
        if (cur->mac_parameters) {
            mac_params->PANId = cur->mac_parameters->pan_id;
            mac_params->mac_short = cur->mac_parameters->mac_short_address;
        } else {
            mac_params->PANId = 0xffff;
            mac_params->mac_short = 0xffff;
        }
    }
    return ret_val;
}

/**
 * \brief A function to read 6LoWPAN ND border router address and NWK prefix
 * \param mac_params is a pointer to the structure to where the mac address are written to.
 * \return 0 on success.
 * \return -1 .
 */
int8_t arm_nwk_nd_address_read(int8_t interface_id, network_layer_address_s *nd_addr_info)
{
    (void)interface_id;
    (void)nd_addr_info;
    int8_t ret_val = -2;
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if ((cur->lowpan_info & (INTERFACE_NWK_ACTIVE | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) == (INTERFACE_NWK_ACTIVE | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
            uint8_t *adr_ptr = protocol_6lowpan_nd_border_router_address_get(cur->nwk_id);
            if (adr_ptr) {
                ret_val = 0;
                memcpy(nd_addr_info->border_router, adr_ptr, 16);
                memcpy(nd_addr_info->prefix, adr_ptr, 8);
            }
        }
    }
#else
    (void)interface_id;
    (void)nd_addr_info;
#endif
    return ret_val;
}

/**
  * \brief Get current used channel.
  *
  * \return Active channel
  * \return -1 if invalid network interface ID is given
  */
int16_t arm_net_get_current_channel(int8_t interface_id)
{
    int16_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            ret_val = cur->mac_parameters->mac_channel;
        }
    }

    return ret_val;
}

/**
 * \brief A function to set sleep mode of a host.
 * \param state equals to 1 if the sleep mode is to be enabled, 0 if the sleep mode is to be disabled.
 */
void arm_net_host_enter_sleep_state_set(int8_t interface_id, uint8_t state)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->rfd_poll_info) {

        if (state) {
            cur->rfd_poll_info->macDeepSleepEnabled = true;
        } else {
            cur->rfd_poll_info->macDeepSleepEnabled = false;
        }
    }
}


/**
 * \brief A function to read library version information.
 * \param ptr is a pointer to an array to where the version information is read to.
 */
void net_get_version_information(uint8_t *ptr)
{
    (void)ptr;
}

/**
 * \brief Set configured network interface Global address mode (Border router bootstrap mode can't set this).
 *
 * \param interface_id Network interface ID
 * \param mode efine 6LoWPAN Global Address register mode::
 *      * NET_6LOWPAN_GP64_ADDRESS, Interface register only GP64
 *      * NET_6LOWPAN_GP16_ADDRESS, Interface register only GP16
 *      * NET_6LOWPAN_MULTI_GP_ADDRESS, Interface register GP16 and GP64 addresses. GP16 is primary address and GP64 is secondary.
 *
 * \param short_address_base Short address base. If application defines value 0-0xfffd 6LoWPAN try to register GP16 address using that address. 0xfffe and 0xffff will generate random 16-bit short address.
 *
 * \param define_new_short_address_at_DAD This parameter is only checked when mode is not NET_6LOWPAN_GP64_ADDRESS and short_address_base is 0-0xfffd. Recommend value is 1 that will enable automatic new address definition at Duplicate Address Detection(DAD). Value 0 will generate Duplicate Adreress Detection error for interface bootstrap.
Border Router Device will not check that part.
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Illegal for Border Router
 * \return -3 Bootsrap not defined yet.
 */
int8_t arm_nwk_6lowpan_gp_address_mode(int8_t interface_id, net_6lowpan_gp_address_mode_e mode, uint16_t short_address_base, uint8_t define_new_short_address_at_DAD)
{
#ifdef HAVE_6LOWPAN_ND
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return -2;
    }
    if (!(cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED)) {
        return -3;
    }

    if (thread_info(cur)) {
        return -2;
    }

    if (short_address_base < 0xfffe) {
        cur->lowpan_desired_short_address = short_address_base;
    } else {
        protocol_6lowpan_allocate_mac16(cur); //Allocate Random init value
    }
    cur->reallocate_short_address_if_duplicate = define_new_short_address_at_DAD;
    cur->lowpan_address_mode = mode;

    return 0;
#else
    return -2;
#endif
}

/**
 * \brief A function to read networking address informations.
 * \param addr_id identifies the address information type to be read.
 * \param address is a pointer to a buffer to where the address information is written to.
 * \return zero on success, -1 on errors.
 */
int8_t arm_net_address_get(int8_t interface_id, net_address_t addr_id, uint8_t *address)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    const uint8_t *addr;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->global_address_available) { //Should also check Check Bootstrap state
        return -1;
    }

    switch (addr_id) {
        case ADDR_IPV6_LL:
            ret_val = addr_interface_get_ll_address(cur, address, 0);
            break;

        case ADDR_IPV6_GP:
            addr = addr_select_with_prefix(cur, NULL, 0, SOCKET_IPV6_PREFER_SRC_PUBLIC | SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT);
            if (addr) {
                memcpy(address, addr, 16);
                ret_val = 0;
            }
            break;

        case ADDR_IPV6_GP_SEC:
            addr = addr_select_with_prefix(cur, NULL, 0, SOCKET_IPV6_PREFER_SRC_PUBLIC | SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG);
            /* Return if the "prefer long" gives a different answer to the default "prefer short". Pointer comparison is
             * sufficient as addr_select returns a pointer into the address list. */
            if (addr && addr != addr_select_with_prefix(cur, NULL, 0, SOCKET_IPV6_PREFER_SRC_PUBLIC | SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT)) {
                memcpy(address, addr, 16);
                ret_val = 0;
            }
            break;
    }
    return ret_val;
}

/**
 * \brief A function to read network Interface address count.
 * \param interface_id Id to interface.
 * \param address_count Pointer where address count will be saved.
 * \return zero on success, -1 on errors.
 */
int8_t arm_net_interface_address_list_size(int8_t interface_id, uint16_t *address_count)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    *address_count = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        ns_list_foreach(if_address_entry_t, addr, &cur->ip_addresses) {
            if (!addr->tentative) {
                (*address_count)++;
            }
        }

        ret_val = 0;
    }
    return ret_val;
}

/**
 * \brief A function to set interface metric.
 * \param interface_id Network interface ID.
 * \param metric Used to rank otherwise-equivalent routes. Lower is preferred and default is 0. The metric value is added to metric provided by the arm_net_route_add() function.
 * \return 0 On success, -1 on errors.
 */
int8_t arm_net_interface_set_metric(int8_t interface_id, uint16_t metric)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (cur) {
        cur->ipv6_neighbour_cache.route_if_info.metric = metric;
        ret_val = 0;
    }

    return ret_val;
}

/**
 * \brief A function to read the interface metric value on an interface.
 * \param interface_id Network interface ID.
 * \param metric A pointer to the variable where the interface metric value is saved.
 * \return 0 On success, -1 on errors.
 */
int8_t arm_net_interface_get_metric(int8_t interface_id, uint16_t *metric)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (cur) {
        *metric = cur->ipv6_neighbour_cache.route_if_info.metric;
        ret_val = 0;
    }

    return ret_val;
}

/**
 * \brief A function to read network Interface.
 * \param interface_id Id to interface.
 * \param address_buf_size Indicate buffer size in bytes minimal is 16 bytes.
 * \param address_buffer pointer where stack save address one by one.
 * \param writed_address_count pointer where stack save how many address is writed behind address_buffer.
 *
 * \return zero on success, -1 on errors.
 */
int8_t arm_net_address_list_get(int8_t interface_id, uint8_t address_buf_size, uint8_t *address_buffer, int *writed_address_count)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    int address_count = 0;


    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (address_buf_size >= 16) {
        int loop_counter = 0;
        bool save_address;
        while (loop_counter < 2) {
            ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
                if (e->tentative) {
                    continue;
                }

                save_address = false;
                if (loop_counter) {
                    if (!addr_is_ipv6_link_local(e->address)) {
                        save_address = true;
                    }
                } else {
                    if (addr_is_ipv6_link_local(e->address)) {
                        save_address = true;
                    }
                }
                if (save_address) {
                    memcpy(address_buffer, e->address, 16);
                    address_buf_size -= 16;
                    ret_val = 0;
                    address_count++;
                    if (address_buf_size >= 16) {
                        address_buffer += 16;
                    } else {
                        *writed_address_count = address_count;
                        return ret_val;
                    }
                }
            }
            loop_counter++;
        }
        //Save writed address count to Pointer
        *writed_address_count = address_count;
    }
    return ret_val;
}

int8_t arm_net_address_list_get_next(int8_t interface_id, int *n, uint8_t address_buffer[16])
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur;
    int address_count = 0;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    int loop_counter = 0;
    bool save_address;
    while (loop_counter < 2) {
        ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
            if (e->tentative) {
                continue;
            }

            save_address = false;
            if (loop_counter) {
                if (!addr_is_ipv6_link_local(e->address)) {
                    save_address = true;
                }
            } else {
                if (addr_is_ipv6_link_local(e->address)) {
                    save_address = true;
                }
            }
            if (save_address) {
                if (*n == address_count) {
                    memcpy(address_buffer, e->address, 16);
                    *n = *n + 1;
                    return 0;
                }
                address_count++;
            }
        }
        loop_counter++;
    }
    return ret_val;
}

int8_t arm_net_address_add_to_interface(int8_t interface_id, const uint8_t address[16], uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime)
{
    protocol_interface_info_entry_t *cur;
    if_address_entry_t *entry;

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return -1;
    }

    entry = addr_add(cur, address, prefix_len, ADDR_SOURCE_STATIC, valid_lifetime, preferred_lifetime, false);

    if (!entry) {
        return -1;
    }

    return 0;
}

int8_t arm_net_address_delete_from_interface(int8_t interface_id, const uint8_t address[16])
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return -1;
    }

    return addr_delete(cur, address);
}

int8_t arm_net_route_add(const uint8_t *prefix, uint8_t prefix_len, const uint8_t *next_hop, uint32_t lifetime, uint8_t metric, int8_t interface_id)
{
    ipv6_route_t *entry;

    if (prefix_len > 128 || (prefix == NULL && prefix_len != 0)) {
        return -2;
    }

    entry = ipv6_route_add_metric(prefix, prefix_len, interface_id, next_hop, ROUTE_USER, NULL, 0, lifetime, metric);

    if (!entry) {
        return -1;
    }

    return 0;
}

int8_t arm_net_route_delete(const uint8_t *prefix, uint8_t prefix_len, const uint8_t *next_hop, int8_t interface_id)
{
    if (prefix_len > 128 || (prefix == NULL && prefix_len != 0)) {
        return -2;
    }

    return ipv6_route_delete(prefix, prefix_len, interface_id, next_hop, ROUTE_USER);
}


int8_t arm_nwk_interface_ethernet_init(eth_mac_api_t *api, const char *interface_name_ptr)
{
#ifdef HAVE_ETHERNET
    if (!api) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_generate_ethernet(api);
    if (!cur) {
        return -3;
    }

    cur->if_up = ipv6_interface_up;
    cur->if_down = ipv6_interface_down;
    cur->interface_name = interface_name_ptr;
    return cur->id;
#else
    (void)api;
    (void)interface_name_ptr;
    return -2;
#endif
}

extern int8_t arm_nwk_interface_ppp_init(struct eth_mac_api_s *api, const char *interface_name_ptr)
{
#ifdef HAVE_ETHERNET
    if (!api) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_generate_ppp(api);
    if (!cur) {
        return -3;
    }

    cur->if_up = ipv6_interface_up;
    cur->if_down = ipv6_interface_down;
    cur->interface_name = interface_name_ptr;
    return cur->id;
#else
    (void)api;
    (void)interface_name_ptr;
    return -2;
#endif
}

int8_t arm_nwk_interface_lowpan_init(mac_api_t *api, char *interface_name_ptr)
{
    if (!api) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_generate_lowpan(api);
    if (!cur) {
        return -3;
    }
    protocol_6lowpan_configure_core(cur);
    cur->interface_name = interface_name_ptr;
    return cur->id;
}

static int arm_net_channel_bit_mask_to_number(const uint32_t *channel_mask)
{
    int i, j;

    for (j = 0; j < 8; j++) {
        for (i = 0; i < 32; i++) {
            if (channel_mask[j] & ((uint32_t)1 << i)) {
                break;
            }
        }
        if (i < 32) {
            break;
        }
    }
    if (j > 7) {
        return -1;
    }
    return i + (j * 32);
}

/**
 * \brief Set network interface link layer parameters.
 *
 * \param interface_id Network interface ID
 * \param tun_driver_id Driver id FOR PHY data IN & OUT
 * \param channel define network link channel
 * \param link_setup Link layer parameters for NET_6LOWPAN_NETWORK_DRIVER defines NetworkID, PAN-ID Short Address
 *

 * \return >=0 Config set OK.
 * \return -1 Unknown network ID or tun driver.
 * \return -2 Interface is active, Bootsrap mode not selected or is not NET_6LOWPAN_NETWORK_DRIVER or NET_6LOWPAN_SNIFFER.
 * \return -3 No Memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter
 * \return -5 Channel list empty
 */
int8_t arm_nwk_interface_network_driver_set(int8_t interface_id, const channel_list_s *nwk_channel_list, network_driver_setup_s *link_setup)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;

    if (arm_channel_list_validation(nwk_channel_list)) {
        tr_debug("Given channel mask is empty!\n");
        return -5;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->mac_api) {
        return -1;
    }


    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        ret_val = -2;
    } else if ((cur->configure_flags &  INTERFACE_BOOTSTRAP_DEFINED) == 0) {
        ret_val = -2;
    } else if (link_setup && (link_setup->beacon_payload_tlv_length && link_setup->beacon_payload_tlv_ptr == NULL)) {
        ret_val = -4;
    } else if (link_setup && (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT || cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_SNIFFER)) {

        ret_val = 0;

        if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT) {
            //Configure setup
            uint8_t *beaon_payload = mac_helper_beacon_payload_reallocate(cur, 18);
            if (beaon_payload) {
                *beaon_payload++ = link_setup->beacon_protocol_id;
                *beaon_payload++ = 7; //Accept Join / Host & Router
                memcpy(beaon_payload, link_setup->network_id, 16);
                ret_val = mac_helper_beacon_payload_register(cur);
            } else {
                ret_val = -3;
            }
            cur->mac_parameters->mac_channel_list = *nwk_channel_list;
        } else {

        }

        if (ret_val == 0) {
            if (link_setup->mac_short_adr < 0xfffe) {
                cur->lowpan_address_mode = NET_6LOWPAN_GP16_ADDRESS;
            } else {
                cur->lowpan_address_mode = NET_6LOWPAN_GP64_ADDRESS;
            }
            mac_helper_panid_set(cur, link_setup->mac_panid);
            mac_helper_mac16_address_set(cur, link_setup->mac_short_adr);

            int channel_number = arm_net_channel_bit_mask_to_number(nwk_channel_list->channel_mask);

            if (channel_number >= 0) {
                // copy the channel list information, which is needed by FHSS
                //Set Channel
                mac_helper_mac_channel_set(cur, channel_number);
                cur->configure_flags |= INTERFACE_NETWORK_DRIVER_SETUP_DEFINED;
            }
        } else {
            mac_helper_beacon_payload_reallocate(cur, 0);
        }
    } else {
        ret_val = -2;
    }
    return ret_val;
}

int8_t arm_nwk_interface_up(int8_t interface_id)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) && cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return -4;
    }

    if (!cur->if_up || !cur->if_down) {
        return -5;
    }

    cur->net_start_tasklet = eventOS_scheduler_get_active_tasklet();
    ret_val = cur->if_up(cur);


    return ret_val;
}

int8_t arm_nwk_interface_down(int8_t interface_id)
{

    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {

        if (!(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
            ret_val = -4;
        } else if (!cur->if_up || !cur->if_down) {
            return -5;
        } else {
            ret_val = cur->if_down(cur);
        }

    }
    return ret_val;
}

int8_t arm_pana_client_key_pull(int8_t interface_id)
{
#ifndef PANA
    (void)interface_id;
#endif
    return pana_client_key_pull(interface_id);
}

int8_t arm_nwk_link_layer_security_mode(int8_t interface_id, net_6lowpan_link_layer_sec_mode_e  mode, uint8_t sec_level, const net_link_layer_psk_security_info_s *psk_key_info)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || thread_info(cur) || !cur->mac_parameters || (cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED) == 0) {
        return -1;
    }

#ifndef HAVE_6LOWPAN_ND
    return -1;
#else
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        return -4;
    }

    //Verify MLE Service
    if (arm_6lowpan_mle_service_ready_for_security_init(cur) != 0) {
        return -1;
    }

    cur->if_lowpan_security_params->nwk_security_mode = mode;
    mac_helper_link_frame_counter_set(cur->id, 0); //This is maybe mistake

    if (mode == NET_SEC_MODE_NO_LINK_SECURITY) {
        cur->mac_parameters->mac_configured_sec_level = 0;
        cur->if_lowpan_security_params->security_level = 0;
        cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
        cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
    } else {
        if (sec_level == 0 || sec_level > 7) {
            return -2;
        }

        cur->mac_parameters->mac_configured_sec_level = sec_level;
        cur->if_lowpan_security_params->security_level = sec_level;

        if (mode == NET_SEC_MODE_PSK_LINK_SECURITY) {
            if (!psk_key_info) {
                return -2;
            }
            //SET PSK KEY
            cur->if_lowpan_security_params->psk_key_info = *psk_key_info;
            cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
            cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
        } else {
            if (!cur->if_lowpan_security_params->pana_params) {
                cur->if_lowpan_security_params->pana_params = pana_client_parameter_allocate();
            }

            if (!cur->if_lowpan_security_params->pana_params) {
                return -2;
            }
            cur->lowpan_info |= (INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION);
        }
    }
    return 0;
#endif
}

int8_t arm_network_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info)
{
#if !defined(PANA) && !defined(HAVE_WS)
    (void)chain_info;
#endif

#ifdef HAVE_WS
    ws_pae_controller_certificate_chain_set(chain_info);
#endif

    return pana_interface_certificate_chain_set(chain_info);
}

int8_t arm_network_trusted_certificate_add(const arm_certificate_entry_s *cert)
{
#ifdef HAVE_WS
    return ws_pae_controller_trusted_certificate_add(cert);
#else
    (void) cert;
    return -1;
#endif
}

int8_t arm_network_trusted_certificate_remove(const arm_certificate_entry_s *cert)
{
#ifdef HAVE_WS
    return ws_pae_controller_trusted_certificate_remove(cert);
#else
    (void) cert;
    return -1;
#endif
}

int8_t arm_network_trusted_certificates_remove(void)
{
    return -1;
}

int8_t arm_network_own_certificate_add(const arm_certificate_entry_s *cert)
{
    (void) cert;
    return -1;
}

extern int8_t arm_network_own_certificates_remove(void)
{
    return -1;
}

int8_t arm_network_certificate_revocation_list_add(const arm_cert_revocation_list_entry_s *crl)
{
#ifdef HAVE_WS
    return ws_pae_controller_certificate_revocation_list_add(crl);
#else
    (void) crl;
    return -1;
#endif
}

int8_t arm_network_certificate_revocation_list_remove(const arm_cert_revocation_list_entry_s *crl)
{
#ifdef HAVE_WS
    return ws_pae_controller_certificate_revocation_list_remove(crl);
#else
    (void) crl;
    return -1;
#endif
}

/**
 * \brief Read Pana server security key material
 *
 * previous_active_network_key is information is only valid when current_active_key_index is bigger than 1.
 *
 *\param key pointer for store keymaterial information.
 *
 * \return 0 Key Read OK
 * \return -1 Pana server key material not available
 */
int8_t arm_network_key_get(int8_t interface_id, ns_keys_t *key)
{
#ifndef PANA
    (void)interface_id;
    (void)key;
#endif
    return pana_network_key_get(interface_id, key);
}

int8_t arm_pana_server_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key)
{
#ifndef PANA
    (void)interface_id;
    (void)cipher_mode;
    (void)key_material;
    (void)time_period_before_activate_key;
#endif
    return pana_server_interface_init(interface_id, cipher_mode, key_material, time_period_before_activate_key);
}

int8_t arm_pana_activate_new_key(int8_t interface_id)
{
#ifndef PANA
    (void)interface_id;
#endif
    return  pana_server_trig_new_key(interface_id);
}

int8_t arm_pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material)
{
#ifndef PANA
    (void)interface_id;
    (void)network_key_material;
#endif
    return pana_server_key_update(interface_id, network_key_material);
}

int8_t net_pana_parameter_set(const pana_lib_parameters_s *parameter_ptr)
{
#ifndef PANA
    (void)parameter_ptr;
#endif
    return pana_set_params(parameter_ptr);
}


/**
 * \brief API to read PANA library parameters.
 *
 * \param parameter_ptr Output pointer for Pana parameters
 *
 */
int8_t net_pana_parameter_read(pana_lib_parameters_s *parameter_ptr)
{
#ifndef PANA
    (void)parameter_ptr;
#endif
    return pana_get_params(parameter_ptr);
}

int8_t arm_pana_client_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id)
{
#ifndef PANA
    (void)interface_id;
    (void)cipher_mode;
    (void)psk_key_id;
#endif
    return pana_client_interface_init(interface_id, cipher_mode, psk_key_id);
}

int8_t arm_nwk_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer)
{
#ifndef HAVE_ETHERNET
    (void)interface_id;
    (void)bootstrap_mode;
    (void)ipv6_prefix_pointer;
#endif
    return ipv6_interface_configure_ipv6_bootstrap_set(interface_id, bootstrap_mode, ipv6_prefix_pointer);
}

int8_t arm_nwk_interface_accept_ipv6_ra(int8_t interface_id, net_ipv6_accept_ra_e accept_ra)
{
#ifndef HAVE_ETHERNET
    (void)interface_id;
    (void)accept_ra;
#endif
    return ipv6_interface_accept_ra(interface_id, accept_ra);
}

int8_t arm_6lowpan_bootsrap_set_for_selected_interface(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE || cur->interface_mode == INTERFACE_UP) {
        return -4;
    }

    if (cur->nwk_id != IF_6LoWPAN) {
        return -1;
    }
    return 0;
}

/**
 * \brief Set network interface bootstrap setup.
 *
 * \param interface_id Network interface ID
 * \param bootstrap_mode Selected Bootstrap mode:
 *      * NET_6LOWPAN_BORDER_ROUTER, Initialise Border router basic setup
 *      * NET_6LOWPAN_ROUTER, Enable normal 6LoWPAN ND and RPL to bootstrap
 *      * NET_6LOWPAN_HOST, Enable normal 6LoWPAN ND only to bootstrap
 *      * NET_6LOWPAN_SLEEPY_HOST, Enable normal 6LoWPAN ND only to bootstrap
 *
 * \param net_6lowpan_mode_extension Define MLE protocol use and 6LoWPAN mode
 *
 * \return >=0 Bootstrap mode set OK.
 * \return -1 Unknown network ID.
 * \return -2 Unsupported bootstrap type or extension in this library.
 * \return -3 No Memory for 6LoWPAN stack.
 * \return -4 Null pointer parameter
 */
int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension)
{
    int8_t ret_val;

    ret_val = arm_6lowpan_bootsrap_set_for_selected_interface(interface_id);

    if (ret_val == 0) {

        if (net_6lowpan_mode_extension == NET_6LOWPAN_THREAD) {
            ret_val = thread_node_bootstrap_init(interface_id, bootstrap_mode);
        } else if (net_6lowpan_mode_extension == NET_6LOWPAN_WS) {
            ret_val = ws_bootstrap_init(interface_id, bootstrap_mode);
        } else {
            ret_val = arm_6lowpan_bootstarp_bootstrap_set(interface_id, bootstrap_mode, net_6lowpan_mode_extension);
        }
    }

    return ret_val;
}

int8_t arm_nwk_set_channel_list(int8_t interface_id, const channel_list_s *nwk_channel_list)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_parameters) {
        return -1;
    }

    if (arm_channel_list_validation(nwk_channel_list)) {
        tr_debug("Given channel mask is empty!\n");
        return -2;
    }

    //CHECK Value
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        return -4;
    }

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        if (!cur->border_router_setup) {
            return -2;
        }

        const int channel_number = arm_net_channel_bit_mask_to_number(nwk_channel_list->channel_mask);
        if (channel_number < 0) {
            return -3;
        }
        cur->mac_parameters->mac_channel_list = *nwk_channel_list;
        cur->mac_parameters->mac_channel = channel_number;
        cur->border_router_setup->chanlist = &cur->mac_parameters->mac_channel_list;
        ret_val = 0;
    } else {
        // copy the channel information and store one internal pointer to it
        cur->mac_parameters->mac_channel_list = *nwk_channel_list;
        ret_val = 0;
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_link_scan_parameter_set(int8_t interface_id, uint8_t scan_time)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            return -4;
        }

        if (cur->mac_parameters) {
            if (scan_time > 14) {
                ret_val = -5;
            } else {
                nwk_scan_params_t *scan_params = 0;
                scan_params = &cur->mac_parameters->nwk_scan_params;
                scan_params->scan_duration = scan_time;
                ret_val = 0;
            }
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t interface_id, uint16_t pan_id_filter)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            ret_val =  -2;
        } else if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
            filter->net_pan_id_filter = pan_id_filter;
            ret_val = 0;
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t interface_id, const uint8_t *nwk_id_filter)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {

        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            ret_val =  -2;
        } else if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
            ret_val = mac_helper_nwk_id_filter_set(nwk_id_filter, filter);
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t interface_id, uint8_t protocol_ID)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {

        if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
            ret_val =  -4;
        } else if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);
            filter->beacon_protocol_id_filter = protocol_ID;
            ret_val = 0;
        }
    }
    return ret_val;
}

/* Don't have a loopback interface we can optimise for, but we do still need a route so we
 * can talk to ourself at all, in case our address isn't in an on-link prefix.
 */
static void net_automatic_loopback_route_update(protocol_interface_info_entry_t *interface, const if_address_entry_t *addr, if_address_callback_t reason)
{
    /* Don't care about link-local addresses - we know they're on-link */
    if (addr_is_ipv6_link_local(addr->address)) {
        return;
    }

    /* TODO: When/if we have a real loopback interface, these routes would use it instead of interface->id */
    switch (reason) {
        case ADDR_CALLBACK_DAD_COMPLETE:
            ipv6_route_add(addr->address, 128, interface->id, NULL, ROUTE_LOOPBACK, 0xFFFFFFFF, 0);
            break;
        case ADDR_CALLBACK_DELETED:
            ipv6_route_delete(addr->address, 128, interface->id, NULL, ROUTE_LOOPBACK);
            break;
        default:
            break;
    }
}

int8_t arm_nwk_6lowpan_beacon_join_priority_tx_callback_set(int8_t interface_id,
                                                            beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr)
{
    return (mac_beacon_link_beacon_join_priority_tx_callback_set(interface_id, beacon_join_priority_tx_cb_ptr));
}

int8_t arm_nwk_6lowpan_beacon_compare_rx_callback_set(int8_t interface_id,
                                                      beacon_compare_rx_cb *beacon_compare_rx_cb_ptr)
{
    return (mac_beacon_link_beacon_compare_rx_callback_set(interface_id, beacon_compare_rx_cb_ptr));
}

/**
  * \brief A function to initialize core elements of NanoStack library.
  *
  * \param core_idle is a function pointer to a function that is called whenever NanoStack is idle.
  * \return 0 on success.
  * \return -1 if a null pointer is given.
  */
int8_t net_init_core(void)
{
    /* Reset Protocol_stats */
    protocol_stats_init();
    protocol_core_init();
#ifdef HAVE_RPL
    rpl_data_init();
    // XXX application should call these!
    rpl_of0_init();
    rpl_mrhof_init();
#endif
    network_library_init();
    addr_notification_register(net_automatic_loopback_route_update);
    return 0;
}

static int8_t mac_data_poll_host_polling_state_change_check(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = 0;
    if (cur->lowpan_info  & INTERFACE_NWK_ROUTER_DEVICE) {
        tr_warn("Host Control not accepted for Router");
        ret_val = -1;
    } else if (nwk_bootsrap_ready(cur) == 0) {
        tr_debug("Bootsrap Active");
        ret_val = -2;
    }
    return ret_val;
}


/**
  * \brief Set new Host state.
  *
  * \param mode new host state
  * \param poll_time poll time in seconds only handled when NET_HOST_SLOW_POLL_MODE is enabled
  *
  * Valid poll time for NET_HOST_SLOW_POLL_MODE is 0 < poll_time poll_time < 864001 (1 Day)
  *
  * \return 0, State update OK
  * \return -1, unknown state
  * \return -2, invalid time
  * \return -3 MLE handshake trig Fail
  *
  */
int8_t arm_nwk_host_mode_set(int8_t interface_id, net_host_mode_t mode, uint32_t poll_time)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (mac_data_poll_host_polling_state_change_check(cur) != 0) {
        return -3;
    }

    net_host_mode_t old_mode;

    if (mac_data_poll_host_mode_get(cur, &old_mode) != 0) {
        return -1;
    }

    if (thread_info(cur)) {
        //save polltime for later use, polltime is zero for fast poll mode
        thread_info(cur)->sleepy_host_poll_time = 0;
        if (mode == NET_HOST_SLOW_POLL_MODE) {
            thread_info(cur)->sleepy_host_poll_time = poll_time;
        }
        if (old_mode == NET_HOST_RX_ON_IDLE && mode != old_mode
                && thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE) {
            tr_debug("End device changing to SED");
            thread_management_device_type_set(cur->id, THREAD_DEVICE_SED);
            return 0;
        }
    }
    return mac_data_poll_host_mode_set(cur, mode, poll_time);
}

/**
  * \brief Read Current Host State.
  *
  * \param mode pointer where host state will be saved

  * \return 0, State Read update OK
  * \return -1, Net Role is Router or stack is idle
  *
  */
int8_t arm_nwk_host_mode_get(int8_t interface_id, net_host_mode_t *mode)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    return mac_data_poll_host_mode_get(cur, mode);
}


int8_t net_nvm_data_clean(int8_t interface_id)
{
    int8_t ret_val = -2; // Not ative
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if (cur->mac_parameters) {
            nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);

            if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
                mac_helper_nwk_id_filter_set(0, filter);
                mac_helper_panid_set(cur, 0xffff);
                mac_helper_mac16_address_set(cur, 0xffff);
                pana_reset_client_session();
                ret_val = 0;
            } else {
                ret_val = 0;
            }
        }
    }

    return ret_val;
}


static void trace_cmd_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vtracef(TRACE_LEVEL_CMD, TRACE_GROUP, fmt, ap);
    va_end(ap);
}
void arm_print_routing_table(void)
{
    arm_print_routing_table2(trace_cmd_print);
}

void arm_print_routing_table2(void (*print_fn)(const char *fmt, ...))
{
    ipv6_destination_cache_print(print_fn);
    ipv6_route_table_print(print_fn);
#ifdef HAVE_RPL
    rpl_control_print(print_fn);
#endif
}

void arm_print_neigh_cache(void)
{
    arm_print_neigh_cache2(trace_cmd_print);
}

void arm_print_neigh_cache2(void (*print_fn)(const char *fmt, ...))
{
    nwk_interface_print_neigh_cache(print_fn);
}

void arm_print_protocols(void)
{
    arm_print_protocols2(trace_cmd_print, ' ');
}

void arm_print_protocols2(void (*print_fn)(const char *fmt, ...), char sep)
{
    socket_list_print(print_fn, sep);
}

void arm_ncache_flush(void)
{
    nwk_interface_flush_neigh_cache();
}

int arm_nwk_sleepy_device_parent_buffer_size_set(int8_t interface_id, uint16_t big_packet_threshold, uint16_t small_packets_per_child_count, uint16_t big_packets_total_count)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        return lowpan_adaptation_indirect_queue_params_set(cur, big_packet_threshold,
                                                           big_packets_total_count, small_packets_per_child_count);
    }
    return -1;
}

int8_t arm_nwk_set_cca_threshold(int8_t interface_id, uint8_t cca_threshold)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->mac_api || (cca_threshold > 100)) {
        return -1;
    }
    mlme_set_t set_req;
    set_req.attr = macCCAThreshold;
    set_req.attr_index = 0;
    set_req.value_pointer = &cca_threshold;
    set_req.value_size = sizeof(cca_threshold);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);
    return 0;
}

int8_t arm_nwk_set_tx_output_power(int8_t interface_id, uint8_t tx_power)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->mac_api || (tx_power > 100)) {
        return -1;
    }
    mlme_set_t set_req;
    set_req.attr = macTXPower;
    set_req.attr_index = 0;
    set_req.value_pointer = &tx_power;
    set_req.value_size = sizeof(tx_power);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);
    return 0;
}
