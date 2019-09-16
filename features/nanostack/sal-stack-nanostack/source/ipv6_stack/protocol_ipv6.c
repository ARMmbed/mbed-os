/*
 * Copyright (c) 2012-2019, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/ns_socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/nd_router_object.h" // for gp_address_ functions - better place?
#include "ipv6_stack/ipv6_routing_table.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Common_Protocols/tcp.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "ethernet_mac_api.h"

#ifdef HAVE_ETHERNET

#define TRACE_GROUP "pIP6"

typedef struct ipv6_interface_prefix_on_link_t {
    uint8_t                             prefix[16];                 /*!< destination address */
    uint8_t                             prefix_len;
    uint32_t                            prefix_valid_ttl;
    ns_list_link_t                      link;
} ipv6_interface_prefix_on_link_t;

typedef struct ipv6_interface_route_on_link_t {
    uint8_t                             prefix[16];                 /*!< destination address */
    uint8_t                             prefix_len;
    uint8_t                             routePrefer;
    uint32_t                            prefix_valid_ttl;
    ns_list_link_t                      link;
} ipv6_interface_route_on_link_t;

#define WB_UPDATE_PERIOD_SECONDS            23

#define ROUTER_SOL_MAX_COUNTER              4

#define TRACE_GROUP_PROTOCOL_IPv6  "ip6s"

static void ipv6_stack(buffer_t *b);

static uint8_t ipv6_nd_rs(protocol_interface_info_entry_t *cur);

static void ipv6_stack_prefix_on_link_update(protocol_interface_info_entry_t *cur, uint8_t *address);
static void ipv6_nd_bootstrap(protocol_interface_info_entry_t *cur);
static void ipv6_interface_address_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason);
int ipv6_interface_route_validate(int8_t interface_id, uint8_t *address);
/* These are the advertised on-link prefixes */
static NS_LIST_DEFINE(prefix_on_link, ipv6_interface_prefix_on_link_t, link);
/* These are the advertised Route prefixes */
static NS_LIST_DEFINE(route_on_link, ipv6_interface_route_on_link_t, link);
static prefix_list_t ipv6_prefixs = NS_LIST_INIT(ipv6_prefixs);

bool tunnel_in_use = false;

static void ethernet_data_conf_cb(const eth_mac_api_t *api, const eth_data_conf_t *data)
{
    (void)data;
    (void)api;
    //Currently not handled
}

static void ethernet_data_ind_cb(const eth_mac_api_t *api, const eth_data_ind_t *data)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(api->parent_id);
    if (!cur || !cur->if_stack_buffer_handler || !data) {
        return;
    }

    /* Fast exit for Ethernet - we will tend to get quite a lot of broadcast
     * IPv4 traffic, so filter frame type before allocating a buffer.
     */
    if (data->etehernet_type != ETHERTYPE_IPV6 || data->msduLength == 0) {
        return;
    }

    buffer_t *buffer = buffer_get_minimal(data->msduLength);
    if (!buffer) {
        tr_error("RX: out of memory");
        return;
    }

    buffer->buf_ptr = buffer->buf_end = 0;
    // Upward direction functions assume no headroom and are trusting that removed bytes are still valid.
    // see mac.c:655

    /* Set default flags */
    buffer->options.dbm = data->dbm;
    buffer->options.lqi = data->link_quality;
    buffer->interface = cur;
    buffer->info = (buffer_info_t)(B_TO_IPV6_TXRX | B_FROM_MAC | B_DIR_UP);

    if (cur->eth_mac_api->address_resolution_needed) {
        buffer->dst_sa.addr_type = ADDR_EUI_48;
        memcpy(buffer->dst_sa.address, data->dstAddress, 6);

        buffer->src_sa.addr_type = ADDR_EUI_48;
        memcpy(buffer->src_sa.address, data->srcAddress, 6);
    }

    /* Copy packet content */
    buffer_data_add(buffer, data->msdu, data->msduLength);

    protocol_push(buffer);
}

void ipv6_interface_phy_sap_register(protocol_interface_info_entry_t *cur)
{
    if (cur->eth_mac_api) {
        cur->eth_mac_api->mac_initialize(cur->eth_mac_api, &ethernet_data_conf_cb, &ethernet_data_ind_cb, cur->id);
    }
}

int ipv6_generate_static_gp_setup(protocol_interface_info_entry_t *cur, bool temporaryAddress)
{
    int ret_val = -1;
    if_address_entry_t *address_entry;
    uint8_t static_address[16];

    ns_list_foreach(prefix_entry_t, prefix_ptr, &ipv6_prefixs) {
        if (prefix_ptr->options & PIO_A) {
            memcpy(static_address, prefix_ptr->prefix, 8);
            memcpy(static_address + 8, cur->iid_eui64, 8);
            address_entry = addr_add(cur, static_address, 64, ADDR_SOURCE_STATIC, 0xffffffff, 0xffffffff, false);
            if (address_entry) {
                address_entry->temporary = temporaryAddress;
                address_entry->cb = ipv6_interface_address_cb;
#ifdef WHITEBOARD
                whiteboard_interface_register(static_address, cur->id);
#endif
                if (cur->ipv6_configure->IPv6_ND_state != IPV6_GP_CONFIG) {
                    cur->ipv6_configure->IPv6_ND_state = IPV6_GP_CONFIG;
                    cur->global_address_available = true;
                }
                ret_val = 0;
            }
        }
    }
    return ret_val;
}

int ipv6_prefix_router_flag_activate(uint8_t *ipv6_address)
{
    prefix_entry_t *entry = icmpv6_prefix_compare(&ipv6_prefixs, ipv6_address, 64);

    if (entry) {
        entry->options |= PIO_R;
        memcpy(&entry->prefix[8], ipv6_address + 8, 8);
        return 0;
    }
    return -1;
}

static buffer_t *ethernet_header_build_push(buffer_t *buf, uint16_t ethernet_type, int8_t *status)
{
    protocol_interface_info_entry_t *cur = buf->interface;
    *status = -1;
    if (cur->nwk_id == IF_IPV6) {
        eth_data_req_t req;
        req.msduLength = buffer_data_length(buf);
        req.msdu = buffer_data_pointer(buf);
        req.srcAddress = buf->src_sa.address;
        req.dstAddress = buf->dst_sa.address;
        req.etehernet_type = ethernet_type;
        if (cur->eth_mac_api) {
            cur->eth_mac_api->data_req(cur->eth_mac_api, &req);
            *status = 0;
        } else {
            *status = -1;
        }
    }
    return buf;
}

/* Input: Final IP packet for transmission on link.
 *        Buffer destination = final destination
 *        Buffer source undefined.
 *        Route next hop address set.
 * Output: Buffer destination+source = link-layer addresses
 *         Sent to MAC layer
 */
buffer_t *ethernet_down(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;

    if (!buf->route) {
        tr_debug("ethernet_down route");
        return buffer_free(buf);
    }

    /* Check if we have tunnel interface that does not need address resolution */
    if (cur->eth_mac_api->address_resolution_needed) {
        const uint8_t *ip_dst = buf->route->route_info.next_hop_addr;

        /* First, check routing to get next hop */
        if (addr_is_ipv6_multicast(ip_dst)) {
            buf->dst_sa.addr_type = ADDR_EUI_48;
            buf->dst_sa.address[0] = 0x33;
            buf->dst_sa.address[1] = 0x33;
            memcpy(&buf->dst_sa.address[2], ip_dst + 12, 4);
        } else { /* unicast */
            ipv6_neighbour_t *n = ipv6_interface_resolve_new(cur, buf);
            if (!n) {
                return NULL;
            }
        }
        buf->src_sa.addr_type = ADDR_EUI_48;
        memcpy(buf->src_sa.address, cur->mac, 6);
    }

    buf->info = (buffer_info_t)(B_FROM_IPV6_TXRX | B_TO_MAC | B_DIR_DOWN);

    return buf;

}

/* Return length of option always, and write option if opt_out != NULL */
static uint8_t ethernet_llao_write(protocol_interface_info_entry_t *cur, uint8_t *opt_out, uint8_t opt_type, bool must, const uint8_t *ip_addr)
{
    (void) must;
    (void) ip_addr;

    if (memcmp(cur->mac, ADDR_EUI64_ZERO, 8) == 0) {
        return 0;
    }

    if (opt_out) {
        opt_out[0] = opt_type;
        opt_out[1] = 1;
        memcpy(opt_out + 2, cur->mac, 6);
    }
    return 8;
}

/* Parse, and return actual size, or 0 if error */
static uint8_t ethernet_llao_parse(protocol_interface_info_entry_t *cur, const uint8_t *opt_in, sockaddr_t *ll_addr_out)
{
    (void) cur;
    if (opt_in[1] != 1) {
        return 0;
    }

    ll_addr_out->addr_type = ADDR_EUI_48;
    memcpy(ll_addr_out->address, opt_in + 2, 6);

    return 6;
}

/**
 * \fn ethernet_up
 *
 * \brief Parse IPv6 header received over the Backhaul link
 *
 */
/* Input: MAC payload received from Ethernet or other link (frame type was IPv6)
 *        Buffer destination = MAC destination (if actually was Ethernet)
 *        Buffer source = MAC source
 * Output: LL broadcast/multicast flags set based on addresses.
 *         Sent to IPv6 forwarding up.
 */
static buffer_t *ethernet_up(buffer_t *buf)
{
    if (buf->dst_sa.addr_type == ADDR_EUI_48 && (buf->dst_sa.address[0] & 1)) {
        if (memcmp(buf->dst_sa.address, (const uint8_t[]) {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    }, 6) == 0)
        buf->options.ll_broadcast_rx = true;
        else {
            buf->options.ll_multicast_rx = true;
        }
    }
    buf->ip_routed_up = true;
    buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_IPV6_TXRX | B_TO_IPV6_FWD);
    return buf;
}

static void ipv6_nd_bootstrap(protocol_interface_info_entry_t *cur)
{
    switch (cur->ipv6_configure->IPv6_ND_state) {
        case IPV6_LL_CONFIG:
            if (addr_interface_set_ll64(cur, ipv6_interface_address_cb) != 0) {
                cur->ipv6_configure->ND_TIMER = 1;
            }
            break;

        case IPV6_ROUTER_SOLICATION:
            tr_debug("Waiting for ICMPv6 Router Advertisement");
            if (ipv6_nd_rs(cur)) {
                if (cur->ipv6_configure->routerSolicationRetryCounter != ROUTER_SOL_MAX_COUNTER) {
                    cur->ipv6_configure->routerSolicationRetryCounter++;
                }
                cur->ipv6_configure->ND_TIMER = (cur->ipv6_configure->routerSolicationRetryCounter * 25);

            } else {
                cur->ipv6_configure->ND_TIMER = 1;
            }
            break;

        case IPV6_GP_GEN:
            if (ipv6_generate_static_gp_setup(cur, false) != 0) {
                cur->ipv6_configure->ND_TIMER = 1;
            }
            break;
        case IPV6_GP_CONFIG:
            break;

        case IPV6_DHCPV6_SOLICATION:
//              if(dhcpv6_non_temporal_address_solication(cur) == 0)
//              {
//                  cur->ipv6_configure->ND_TIMER = 0;
//              }
//              else
//              {
//                  cur->ipv6_configure->ND_TIMER = 1;
//              }
            break;

        case IPV6_DHCPV6_ADDRESS_REQUEST:
            break;
        case IPV6_DHCPV6_ADDRESS_REQ_FAIL:
            tr_warn("DHCP ADDRESS_REQ_FAIL");
            /*XXX Do what here? */
            break;

        case IPV6_READY:
            break;
    }
}


void ipv6_prefix_on_link_list_add_by_interface_address_list(protocol_interface_info_entry_t *cur_Ipv6, protocol_interface_info_entry_t *curRegisteredInterface)
{
    ns_list_foreach(if_address_entry_t, e, &curRegisteredInterface->ip_addresses) {
        if (addr_ipv6_scope(e->address, curRegisteredInterface) == IPV6_SCOPE_GLOBAL) {
            ipv6_stack_prefix_on_link_update(cur_Ipv6, e->address);
        }
    }
}

static void ipv6_stack_prefix_on_link_update(protocol_interface_info_entry_t *cur, uint8_t *address)
{
    if (!cur->global_address_available) {
        return;
    }

    if (addr_interface_gp_prefix_compare(cur, address) != 0) {
        ipv6_interface_prefix_on_link_t *new_entry = 0;
        ns_list_foreach(ipv6_interface_prefix_on_link_t, tmp_prefix, &prefix_on_link) {
            if (memcmp(tmp_prefix->prefix, address, 8) == 0) {
                return;
            }
        }

        new_entry = ns_dyn_mem_alloc(sizeof(ipv6_interface_prefix_on_link_t));
        if (new_entry) {
            memset(new_entry->prefix, 0, 16);
            memcpy(new_entry->prefix, address, 8);

            new_entry->prefix_len = 64;
            // Treat the prefix like a "route", so 3 * advert interval, not 30 days
            new_entry->prefix_valid_ttl = (UINT32_C(3) * icmpv6_radv_max_rtr_adv_interval(cur)) / 10;
            ns_list_add_to_end(&prefix_on_link, new_entry);
            icmpv6_restart_router_advertisements(cur, ADDR_UNSPECIFIED);
        }
    }
}

void ipv6_stack_route_advert_update(uint8_t *address, uint8_t prefixLength, uint8_t routePrefer)
{
    protocol_interface_info_entry_t *cur = nwk_interface_get_ipv6_ptr();
    if (!cur) {
        return;
    }
    if (cur->ipv6_configure->ipv6_stack_mode != NET_IPV6_BOOTSTRAP_STATIC) {
        return;
    }

    if (addr_interface_gp_prefix_compare(cur, address) == 0) {
        return;
    }

    ns_list_foreach(ipv6_interface_route_on_link_t, cur_prefix, &route_on_link) {
        if ((cur_prefix->prefix_len == prefixLength) && bitsequal(cur_prefix->prefix, address, prefixLength)) {
            cur_prefix->routePrefer = routePrefer;
            return;
        }
    }

    ipv6_interface_route_on_link_t *new_entry = ns_dyn_mem_alloc(sizeof(ipv6_interface_route_on_link_t));
    if (new_entry) {
        memset(new_entry->prefix, 0, 16);
        memcpy(new_entry->prefix, address, 16);

        new_entry->prefix_len = prefixLength;
        new_entry->routePrefer = routePrefer;
        // Treat the prefix like a "route", so 3 * advert interval, not 30 days
        new_entry->prefix_valid_ttl = (UINT32_C(3) * icmpv6_radv_max_rtr_adv_interval(cur)) / 10;
        ns_list_add_to_end(&route_on_link, new_entry);
        icmpv6_restart_router_advertisements(cur, ADDR_UNSPECIFIED);
    }
}

void ipv6_prefix_on_link_update(uint8_t *address)
{
    protocol_interface_info_entry_t *cur = nwk_interface_get_ipv6_ptr();
    if (cur) {
        //Call IPv6 Onlink Update
        if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
            ipv6_stack_prefix_on_link_update(cur, address);
        }
    }
}

static void ipv6_stack_prefix_on_link_remove(protocol_interface_info_entry_t *cur, uint8_t *address)
{
    ns_list_foreach(ipv6_interface_prefix_on_link_t, cur_prefix, &prefix_on_link) {
        if (memcmp(cur_prefix->prefix, address, 8) == 0) {
            cur_prefix->prefix_valid_ttl = 0;
            icmpv6_restart_router_advertisements(cur, ADDR_UNSPECIFIED);
            break;
        }
    }
}

void ipv6_prefix_on_link_remove(uint8_t *address)
{
    protocol_interface_info_entry_t *cur = nwk_interface_get_ipv6_ptr();
    if (cur) {
        if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
            ipv6_stack_prefix_on_link_remove(cur, address);
        }
    }
}

void ipv6_stack_route_advert_remove(uint8_t *address, uint8_t prefixLength)
{
    protocol_interface_info_entry_t *cur = nwk_interface_get_ipv6_ptr();
    if (!cur) {
        return;
    }
    if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
        ns_list_foreach(ipv6_interface_route_on_link_t, cur_prefix, &route_on_link) {
            if ((cur_prefix->prefix_len == prefixLength) && bitsequal(cur_prefix->prefix, address, prefixLength)) {
                cur_prefix->prefix_valid_ttl = 0;
                icmpv6_restart_router_advertisements(cur, ADDR_UNSPECIFIED);
                break;
            }
        }
    }
}

void ipv6_prefix_online_list_free(void)
{
    ns_list_foreach_safe(ipv6_interface_prefix_on_link_t, cur, &prefix_on_link) {
        ns_list_remove(&prefix_on_link, cur);
        ns_dyn_mem_free(cur);
    }
}

void ipv6_rote_advert_list_free(void)
{
    ns_list_foreach_safe(ipv6_interface_route_on_link_t, cur, &route_on_link) {
        ns_list_remove(&route_on_link, cur);
        ns_dyn_mem_free(cur);
    }
}

void ipv6_core_slow_timer_event_handle(struct protocol_interface_info_entry *cur)
{
    if (cur->ipv6_configure) {
        protocol_interface_info_entry_t *curRegisteredInterface;
        if (cur->ipv6_configure->wb_table_ttl-- == 1) {
            //tr_debug("WB Table TTL");
            whiteboard_ttl_update(WB_UPDATE_PERIOD_SECONDS);
            cur->ipv6_configure->wb_table_ttl   = WB_UPDATE_PERIOD_SECONDS;
        }

        if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {

            if (cur->global_address_available) {
                curRegisteredInterface = protocol_stack_interface_info_get(IF_6LoWPAN);
                if (curRegisteredInterface) {

                    ipv6_prefix_on_link_list_add_by_interface_address_list(cur, curRegisteredInterface);
                }
            }
        }
    }
}

void ipv6_core_timer_event_handle(protocol_interface_info_entry_t *cur, const uint8_t event)
{
    (void)event;
    if (cur->ipv6_configure->ND_TIMER) {
        if (cur->ipv6_configure->ND_TIMER-- == 1) {
            //Call ND State Machine
            ipv6_nd_bootstrap(cur);
        }
    }
}

int ipv6_prefix_register(uint8_t *prefix_64, uint32_t lifetime, uint32_t prefer_lifetime)
{
    prefix_entry_t *new_entry = icmpv6_prefix_add(&ipv6_prefixs, prefix_64, 64, lifetime, prefer_lifetime, (PIO_L | PIO_A));
    if (new_entry) {
        memset(&new_entry->prefix[8], 0, 8);
        new_entry->options = PIO_L | PIO_A;
        return 0;
    }
    return -1;
}

int ipv6_interface_route_validate(int8_t interface_id, uint8_t *address)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    } else if (!cur->ipv6_configure) {
        return -1;
    } else if (cur->ipv6_configure->IPv6_ND_state != IPV6_READY) {
        return -1;
    }

    //Validate route OnLink
    ipv6_route_t *route;
    route = ipv6_route_choose_next_hop(address, interface_id, NULL);

    //Respond default route only when we have other interface for route
    if (route && route->on_link) {
        return 0;
    }

    return -1;

}

int8_t ipv6_interface_up(protocol_interface_info_entry_t *cur)
{
    uint8_t ipv6_interface_adr[16];

    if (cur->if_stack_buffer_handler) {
        return -1;
    }

    if (cur->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
        uint32_t lifeTime, prefLifetime;
        uint8_t pre_options;
        memcpy(&ipv6_interface_adr[0], cur->ipv6_configure->static_prefix64, 8);
        memcpy(&ipv6_interface_adr[8], cur->iid_eui64, 8);
        prefix_entry_t *new_entry = 0;
        // Treat the prefix like a "route", so 3 * advert interval, not 30 days
        lifeTime = (UINT32_C(3) * icmpv6_radv_max_rtr_adv_interval(cur)) / 10;
        prefLifetime = lifeTime / 2;
        pre_options = PIO_L | PIO_A | PIO_R;

        new_entry = icmpv6_prefix_add(&ipv6_prefixs, ipv6_interface_adr, 64, lifeTime, prefLifetime, pre_options);

        if (!new_entry) {
            return -1;
        }
        memcpy(&new_entry->prefix[8], &ipv6_interface_adr[8], 8);
        new_entry->options = pre_options;

        ipv6_route_add(ipv6_interface_adr, 64, cur->id, NULL, ROUTE_STATIC, 0xffffffff, 1);

        // Disable RA route and prefix processing until static address has been added to interface
        icmpv6_recv_ra_routes(cur, false);
        icmpv6_recv_ra_prefixes(cur, false);
    }

    cur->ipv6_configure->IPv6_ND_state = IPV6_LL_CONFIG;
    cur->ipv6_configure->ND_TIMER = 1;

    cur->ipv6_configure->routerSolicationRetryCounter = 0;
    cur->ipv6_configure->wb_table_ttl       = WB_UPDATE_PERIOD_SECONDS;
    icmpv6_radv_disable(cur);

    tr_debug("IPV6 interface Base Ready");
    cur->if_stack_buffer_handler = ipv6_stack;
    cur->if_llao_write = ethernet_llao_write;
    cur->if_llao_parse = ethernet_llao_parse;
    cur->ipv6_neighbour_cache.max_ll_len = 6;
    cur->ipv6_neighbour_cache.link_mtu = cur->max_link_mtu = 1500;
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;
    cur->lowpan_info |= INTERFACE_NWK_ACTIVE;

    // Always want fe80::/64 to be an on-link prefix.
    ipv6_route_add(ADDR_LINK_LOCAL_PREFIX, 64, cur->id, NULL, ROUTE_STATIC, 0xFFFFFFFF, 0);

    // Putting a multicast route to ff00::/8 makes sure we can always transmit multicast.
    // Interface metric will determine which interface is actually used, if we have multiple.
    ipv6_route_add(ADDR_LINK_LOCAL_ALL_NODES, 8, cur->id, NULL, ROUTE_STATIC, 0xFFFFFFFF, -1);

    return 0;
}

int8_t ipv6_interface_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    whiteboard_interface_unregister_all_address(cur->id);
    icmpv6_prefix_list_free(&ipv6_prefixs);
    ipv6_prefix_online_list_free();
    ipv6_rote_advert_list_free();
    neighbor_cache_flush(&cur->neigh_cache);
    ipv6_route_table_remove_interface(cur->id);
    protocol_core_interface_info_reset(cur);
    cur->ipv6_configure->wb_table_ttl = 0;
    cur->lowpan_info &= ~INTERFACE_NWK_ACTIVE;
    cur->global_address_available = false;
    //Clear all generated proxies
    nd_proxy_upstream_interface_unregister(cur->id);

    return 0;
}


static uint8_t ipv6_nd_rs(protocol_interface_info_entry_t *cur)
{
    buffer_t *buf = icmpv6_build_rs(cur, NULL);
    if (buf) {
        tr_debug("Push RS to IPV6");
        arm_net_protocol_packet_handler(buf, cur);
        return 1;
    }
    return 0;
}

#ifdef RADV_TX
void ipv6_nd_ra_advert(protocol_interface_info_entry_t *cur, const uint8_t *dest)
{
    /* 12 base size, plus allow 8 for MTU and 16 for SLLA */
    uint16_t length = 12 + 8 + 16;
    length += 32 * ns_list_count(&ipv6_prefixs);
    length += 32 * ns_list_count(&prefix_on_link);

    ns_list_foreach(ipv6_interface_route_on_link_t, tmp_route, &route_on_link) {
        if (tmp_route->prefix_len < 65) {
            length += 16;    // add upper 64-bit
        } else {
            length += 24;    // add full address
        }
    }

    buffer_t *buf = buffer_get(length);
    if (buf) {
        uint8_t *ptr = buffer_data_pointer(buf);

        buf->options.hop_limit = 255;
        buf->options.type = ICMPV6_TYPE_INFO_RA;
        buf->options.code = 0x00;
        *ptr++ = cur->adv_cur_hop_limit;
        *ptr++ = cur->rtr_adv_flags;
        //Do not advertise default route: set Router Lifetime to 0
        ptr = common_write_16_bit(0, ptr);
        ptr = common_write_32_bit(cur->adv_reachable_time, ptr);
        ptr = common_write_32_bit(cur->adv_retrans_timer, ptr);

        // We don't include our Source Link-Layer address - we're not
        // advertising routes, so we don't expect anyone to be talking directly
        // to us; we're just acting as an ND proxy. If we put in the SLLAO, we
        // add ourselves to their Neighbour Cache unnecessarily. This can be
        // reactivated if we start advertising actual routes
#if 0
        // Set the source Link-Layer address
        ptr = icmpv6_write_icmp_lla(cur, ptr, ICMPV6_OPT_SRC_LL_ADDR);
#endif

        //MTU
        if (cur->adv_link_mtu != 0) {
            ptr = icmpv6_write_mtu_option(cur->adv_link_mtu, ptr);
        }

        //IPv6 Prefixes
        ptr = icmpv6_write_prefix_option(&ipv6_prefixs,  ptr, 0, cur);

        // Update our own routing table prefixes in sync with what we're advertising above
        ns_list_foreach(prefix_entry_t, pfx, &ipv6_prefixs) {
            if (pfx->options & PIO_L) {
                ipv6_route_add(pfx->prefix, pfx->prefix_len, cur->id, NULL, ROUTE_STATIC, pfx->lifetime, 1);
            }
        }

        ns_list_foreach_safe(ipv6_interface_prefix_on_link_t, pfx, &prefix_on_link) {
            ipv6_route_add(pfx->prefix, pfx->prefix_len, cur->id, NULL, ROUTE_STATIC, pfx->prefix_valid_ttl, 0);

            *ptr++ = ICMPV6_OPT_PREFIX_INFO;
            *ptr++ = 4; // length
            *ptr++ = pfx->prefix_len;
            *ptr++ = PIO_L;
            ptr = common_write_32_bit(pfx->prefix_valid_ttl, ptr); // Valid Lifetime
            ptr = common_write_32_bit(pfx->prefix_valid_ttl >> 1, ptr); // Preferred Lifetime
            ptr = common_write_32_bit(0, ptr);
            memcpy(ptr, pfx->prefix, 16);
            ptr += 16;
            if (pfx->prefix_valid_ttl == 0) {
                ns_list_remove(&prefix_on_link, pfx);
                ns_dyn_mem_free(pfx);
            }
        }

        ns_list_foreach_safe(ipv6_interface_route_on_link_t, tmp_route, &route_on_link) {
            uint8_t prefix_bytes;
            *ptr++ = ICMPV6_OPT_ROUTE_INFO;
            if (tmp_route->prefix_len < 65) {
                *ptr++ = 2;
                prefix_bytes = 8;
            } else {
                *ptr++ = 3;
                prefix_bytes = 16;
            }
            *ptr++ = tmp_route->prefix_len;
            *ptr++ = tmp_route->routePrefer;
            ptr = common_write_32_bit(tmp_route->prefix_valid_ttl, ptr);
            memcpy(ptr, tmp_route->prefix, prefix_bytes);
            ptr += prefix_bytes;

            if (tmp_route->prefix_valid_ttl == 0) {
                ns_list_remove(&route_on_link, tmp_route);
                ns_dyn_mem_free(tmp_route);
            }
        }

        buffer_data_end_set(buf, ptr);
        memcpy(buf->dst_sa.address, dest, 16);
        /* Source must be LL address (even if non-LL dest) */
        if (addr_interface_get_ll_address(cur, buf->src_sa.address, 0)) {
            tr_debug("No address defined");
            buffer_free(buf);
            return;
        }
        buf->dst_sa.addr_type = ADDR_IPV6;
        buf->src_sa.addr_type = ADDR_IPV6;

        buf->interface = cur;
        buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
        arm_net_protocol_packet_handler(buf, cur);
    }
}
#endif

static void ipv6_stack(buffer_t *b)
{
    /* Protocol Buffer Handle */
    int8_t status;
    nwk_interface_id nwk_id = IF_IPV6;
    if (b) {
        //Save Original Interface id
        nwk_id = b->interface->nwk_id;
    }
    while (b) {
        /* Buffer Direction Select Switch */
        if ((b->info & B_DIR_MASK) == B_DIR_DOWN) {
            /* Direction DOWN */
            switch (b->info & B_TO_MASK) {
                case B_TO_ICMP:
                    /* Build ICMP Header */
                    b = icmpv6_down(b);
                    break;

                case B_TO_UDP:
                    /* Build UDP Header */
                    b = udp_down(b);
                    break;

                case B_TO_IPV6:
                    /* Build IP Header */
                    b = ipv6_down(b);
                    break;

                case B_TO_IPV6_FWD:
                    /* IPv6 forwarding */
                    b = ipv6_forwarding_down(b);
                    break;

                case B_TO_IPV6_TXRX:
                    /* Resolution, preparing MAC payload */
                    b = ethernet_down(b);
                    break;

                case B_TO_MAC:
                    b = ethernet_header_build_push(b, ETHERTYPE_IPV6, &status);
                    if (b) {
                        //GET Interface Pointer
                        //uint8_t *ptr = b->buf + b->buf_ptr;
                        //uint16_t len = b->buf_end - b->buf_ptr;
                        //ethernet_tx(ptr, len);
                        if (status == 0) {
                            socket_tx_buffer_event_and_free(b, SOCKET_TX_DONE);
                        } else {
                            socket_tx_buffer_event_and_free(b, SOCKET_TX_FAIL);
                        }
                    }
                    return;

                default:
                    b = buffer_free(b);
                    break;
            }
        } else {
            /* Direction UP */
            switch (b->info & B_TO_MASK) {
                case B_TO_APP:
                    socket_up(b);
                    b = NULL;
                    break;

                case B_TO_MAC:
                    /* Beacon and command frames */
                    //b = mac_up(b);
                    break;

                case B_TO_ICMP:
                    /* Parse ICMP Message */
                    b = icmpv6_up(b);
                    break;

                case B_TO_UDP:
                    /* Parse UDP Message */
                    b = udp_up(b);
                    break;

                case B_TO_IPV6_FWD:
                    /* Handle IP Payload */
                    b = ipv6_forwarding_up(b);
                    break;

                case B_TO_IPV6_TXRX:
                    /* Handle MAC Payload */
                    b = ethernet_up(b);
                    break;

                case B_TO_TCP:
                    //tr_debug("--> TCP_up()");
                    b = tcp_up(b);
                    break;

                default:
                    tr_debug("LLL");
                    b = buffer_free(b);
                    break;
            }
        }

        if (b) {
            //Check If Stack forward packet to different interface
            if (b->interface->nwk_id != nwk_id) {
                protocol_push(b);
                b = 0;
            }
        }
    }
}

static void ipv6_interface_address_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{
    tr_debug("Interface ID: %i, ipv6: %s", interface->id, trace_ipv6(addr->address));

    switch (reason) {
        case ADDR_CALLBACK_DAD_COMPLETE:
            tr_debug("DAD OK");
            switch (interface->ipv6_configure->IPv6_ND_state) {
                case IPV6_LL_CONFIG:
                    if (interface->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC) {
                        interface->ipv6_configure->IPv6_ND_state = IPV6_GP_GEN;
                    } else {
                        interface->ipv6_configure->IPv6_ND_state = IPV6_ROUTER_SOLICATION;
                    }

                    interface->ipv6_configure->ND_TIMER = 1;
                    break;

                case IPV6_GP_CONFIG:
                    if (addr_interface_all_address_ready(interface)) {
                        interface->global_address_available = true;
                        if (interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {
                            nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, interface);
                            /* We will need proxy both mode currently future static mode should not need proxy */
                            nd_proxy_upstream_interface_register(interface->id, ipv6_interface_route_validate);
                        }
                    }

                    // Enable RA route and prefix processing for static configuration mode if RA accept always is enabled
                    if (interface->ipv6_configure->ipv6_stack_mode == NET_IPV6_BOOTSTRAP_STATIC &&
                            interface->ipv6_configure->accept_ra == NET_IPV6_RA_ACCEPT_ALWAYS) {
                        icmpv6_recv_ra_routes(interface, true);
                        icmpv6_recv_ra_prefixes(interface, true);
                    }

                    if (addr->source == ADDR_SOURCE_SLAAC) {
                        ipv6_prefix_router_flag_activate(addr->address);
                    }
                    break;

                default:
                    /* No special action for subsequent addresses after initial bootstrap */
                    if (interface->global_address_available && interface->ipv6_configure->temporaryUlaAddressState) {
                        if (addr_ipv6_scope(addr->address, interface) == IPV6_SCOPE_GLOBAL) {
                            nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, interface);
                            tr_debug("Learn Real Global Scope");
                            interface->ipv6_configure->temporaryUlaAddressState = false;
                            /* We will need proxy both mode currently future static mode should not need proxy */
                            nd_proxy_upstream_interface_register(interface->id, ipv6_interface_route_validate);
                        }
                    }
                    break;
            }
            break;

        case ADDR_CALLBACK_PARENT_FULL:
        case ADDR_CALLBACK_DAD_FAILED:
            switch (interface->ipv6_configure->IPv6_ND_state) {
                case IPV6_LL_CONFIG:
                    tr_warn("No Valid LLaddress..Turn OFF Interface and Push DAD Event");
                    nwk_bootsrap_state_update(ARM_NWK_DUPLICATE_ADDRESS_DETECTED, interface);
                    break;

                case IPV6_GP_CONFIG:
                    if (interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {

                    }
                    break;

                default:
                    /* No special action for subsequent addresses after initial bootstrap */
                    break;
            }
            break;

        default:
            break;
    }
}

void ipv6_interface_slaac_handler(protocol_interface_info_entry_t *cur, const uint8_t *slaacPrefix, uint8_t prefixLen, uint32_t validLifeTime, uint32_t preferredLifeTime)
{
    if_address_entry_t *address_entry = icmpv6_slaac_address_add(cur, slaacPrefix, prefixLen, validLifeTime, preferredLifeTime, false, SLAAC_IID_DEFAULT);
    if (address_entry) {
        address_entry->cb = ipv6_interface_address_cb;
        if (cur->ipv6_configure && cur->ipv6_configure->IPv6_ND_state == IPV6_ROUTER_SOLICATION) {
            cur->ipv6_configure->IPv6_ND_state = IPV6_GP_CONFIG;
        }
        // If DAD not enabled address is valid right away
        if (cur->ipv6_configure && cur->dup_addr_detect_transmits == 0) {
            address_entry->cb(cur, address_entry, ADDR_CALLBACK_DAD_COMPLETE);
        }
    }
}

int8_t ipv6_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        return -4;
    }

    if (cur->nwk_id != IF_IPV6) {
        return -1;
    }

    switch (bootstrap_mode) {
        case NET_IPV6_BOOTSTRAP_STATIC:
            memcpy(cur->ipv6_configure->static_prefix64, ipv6_prefix_pointer, 8);
        /* fall through */
        case NET_IPV6_BOOTSTRAP_AUTONOMOUS:
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_ETHERNET_ROUTER;
            cur->ipv6_configure->ipv6_stack_mode = bootstrap_mode;
            break;
        default:
            break;
    }
    return 0;
}

int8_t ipv6_interface_accept_ra(int8_t interface_id, net_ipv6_accept_ra_e accept_ra)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->ipv6_configure) {
        return -1;
    }

    cur->ipv6_configure->accept_ra = accept_ra;

    return 0;
}

#endif
