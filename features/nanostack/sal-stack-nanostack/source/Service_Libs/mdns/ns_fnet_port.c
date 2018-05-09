/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
 * FNET porting to Nanostack environment
 */

#include "fnet.h"
#include "fnet_netif.h"
#include "fnet_netif_prv.h"
#include "fnet_timer.h"
#include "fnet_serial.h"
#include "fnet_socket.h"

#include "ns_types.h"
#include "ns_list.h"
#include "common_functions.h" // common_write
#include "ns_trace.h"
#include "socket_api.h"
#include "net_interface.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"

#include "ns_fnet_events.h"

#define TRACE_GROUP "mDNS"


fnet_time_t fnet_timer_get_ms( void )
{
    return ns_fnet_time_in_ms_get();
}

fnet_scope_id_t fnet_netif_get_scope_id(fnet_netif_desc_t netif_desc)
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;
    FNET_DEBUG("fnet_netif_get_scope_id() scope_id=%d", (int)netif->scope_id);
    return netif->scope_id;
}

fnet_ip4_addr_t fnet_netif_get_ip4_addr( fnet_netif_desc_t netif_desc )
{
    (void)netif_desc;
    return 0u;
}

fnet_bool_t fnet_netif_get_ip6_addr (fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_addr_info_t *addr_info)
{
    fnet_bool_t     result = FNET_FALSE;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;
    uint8_t         global_address[16] = {0};
    (void)n;

    if(netif && addr_info) {
        if (0 == arm_net_address_get((int8_t)netif->scope_id, ADDR_IPV6_GP, global_address)) {
            memcpy(&addr_info->address.addr, global_address, 16);   /* IPv6 address.*/
            addr_info->state = FNET_NETIF_IP6_ADDR_STATE_PREFERRED;     /* Address current state.*/
            addr_info->type = FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE; /* How the address was acquired.*/
            result = FNET_TRUE;
        }
    }

    FNET_DEBUG("fnet_netif_get_ip6_addr(), if=%d: %s", (int8_t)netif->scope_id, trace_ipv6(addr_info->address.addr));

    return result;
}

fnet_bool_t fnet_netif_is_my_ip6_addr(fnet_netif_t *netif, const fnet_ip6_addr_t *ip_addr)
{
    int i = 0;
    uint8_t address_buffer[16];
    int8_t interface_id = (int8_t)netif->scope_id;
    int8_t addr_available = 0;

    do {
        addr_available = arm_net_address_list_get_next(interface_id, &i, address_buffer);
        if (addr_available < 0) {
            return FNET_FALSE;
        }
        if (addr_ipv6_equal(address_buffer, ip_addr->addr)) {
            return FNET_TRUE;
        }
    } while (addr_available);

    return FNET_FALSE;
}

static void fnet_nanostack_port_ns_socket_recv(void *socket_cb)
{
    (void)socket_cb;
    // socket event received, run service again to achieve faster response time
    ns_fnet_events_fast_poll();
}

fnet_socket_t fnet_socket(fnet_address_family_t family, fnet_socket_type_t type, fnet_uint32_t protocol)
{
    int8_t socket_id = 0;
    (void)protocol;

    if (type != SOCK_DGRAM || family != AF_INET6) {
        return FNET_NULL;
    }

    socket_id = socket_open(SOCKET_UDP, 0, fnet_nanostack_port_ns_socket_recv);
    if (socket_id < 0) {
        return FNET_NULL;
    }

    // Socket contains buffer for the received data, enable the feature using socket options
    static const int32_t rcvbuf_size = 1024*4;
    socket_setsockopt(socket_id, SOCKET_SOL_SOCKET, SOCKET_SO_RCVBUF, &rcvbuf_size, sizeof rcvbuf_size);

    FNET_DEBUG("fnet_socket_open, socket:%d", socket_id);
    return (fnet_socket_t)(long)socket_id;
}

fnet_return_t fnet_socket_bind( fnet_socket_t s, const struct sockaddr *name, fnet_size_t namelen )
{
    (void)namelen;
    ns_address_t ns_source_addr = {0};
    int8_t socket_id = (int8_t)(long)s;
    fnet_return_t fnet_ret_val = FNET_ERR;
    const struct sockaddr_in6 *namein6 = (const struct sockaddr_in6 *) name;
    memcpy(ns_source_addr.address, &namein6->sin6_addr, 16);
    ns_source_addr.identifier = FNET_NTOHS(namein6->sin6_port);
    ns_source_addr.type = ADDRESS_IPV6;

    FNET_DEBUG("fnet_socket_bind socket:%d to port %d address %s", socket_id, (int)ns_source_addr.identifier, trace_ipv6(ns_source_addr.address));

    int8_t status = socket_bind(socket_id, &ns_source_addr);
    if (status == 0) {
        fnet_ret_val = FNET_OK;
    }

    return fnet_ret_val;
}

fnet_return_t fnet_socket_setopt( fnet_socket_t s, fnet_protocol_t level, fnet_socket_options_t optname, const void *optval, fnet_size_t optvallen )
{
    (void) level;
    (void) optvallen;
    int8_t socket_id;
    int8_t ret_val = -1;

    socket_id = (int8_t)(long)s;

    if (optname == IPV6_JOIN_GROUP) {
        const struct ipv6_mreq *mreq6 = optval; /* Multicast group information.*/

        int8_t interface_id = (int8_t)mreq6->ipv6imr_interface; // scope_id holds interface_id
        ret_val = socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &interface_id, sizeof(interface_id));

        if (ret_val == 0) {
            ns_ipv6_mreq_t ns_ipv6_mreq;
            ns_ipv6_mreq.ipv6mr_interface = interface_id;
            memcpy(ns_ipv6_mreq.ipv6mr_multiaddr, ADDR_LINK_LOCAL_MDNS, 16);
            // Join multicast group
            ret_val = socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_JOIN_GROUP, &ns_ipv6_mreq, sizeof(ns_ipv6_mreq));
        }

        if (ret_val == 0) {
            bool loopback = false;
            // Ignore loopback from own multicasts
            ret_val = socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_LOOP, &loopback, sizeof(loopback));
        }
    }
    else if (optname == IPV6_MULTICAST_HOPS) {
        // Both FNET and Nanostack use int16_t, so can pass straight through
        ret_val = socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, optval, optvallen);
    }

    if (ret_val == 0) {
        return FNET_OK;
    }

    return FNET_ERR;
}

fnet_int32_t fnet_socket_sendto( fnet_socket_t s, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, const struct sockaddr *to, fnet_size_t tolen )
{
    (void)tolen; // ipv6 address expected
    (void)flags; //
    int8_t socket_id = (int8_t)(long)s;
    ns_address_t ns_address;

    ns_address.type = ADDRESS_IPV6;
    memcpy(ns_address.address, &((struct sockaddr_in6 *)to)->sin6_addr.s6_addr, 16);
    // Note! Port is already in network byte order, transfer it back as nanostack will convert it again to network byte order
    ns_address.identifier = FNET_NTOHS(to->sa_port);
    FNET_DEBUG("fnet_socket_sendto, %d bytes, port:%d, socket:%d", (int)len, (int)ns_address.identifier, socket_id);
    return (fnet_int32_t)socket_sendto(socket_id, &ns_address, buf, len);
}

fnet_int32_t fnet_socket_recvfrom( fnet_socket_t s, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, struct sockaddr *from, fnet_size_t *fromlen )
{
    (void)flags; // mdns does not use flags
    (void)fromlen;  // address is ipv6
    int8_t socket_id = (int8_t)(long)s;
    int16_t data_len;
    ns_address_t source_address;
    data_len = socket_read(socket_id, &source_address, buf, len);

    if (data_len >= 0) {
        if (from) {
            struct sockaddr_in6 *fromin6 = (struct sockaddr_in6 *) from;
            memcpy(&fromin6->sin6_addr.s6_addr, source_address.address, 16);
            //addrin6.sin6_scope_id = interface_id; where to get interface_id here?
            fromin6->sin6_family = AF_INET6;
            // port is in native byte order,, convert to network byte order
            fromin6->sin6_port = FNET_HTONS(source_address.identifier);
            *fromlen = sizeof(struct sockaddr_in6);
        }
    } else {
        tr_error("Socket recv failed: socket:%d, err:%d", socket_id, data_len);
        data_len = (int16_t) FNET_ERR;
    }

    return data_len;
}

fnet_return_t fnet_socket_close( fnet_socket_t s )
{
    int8_t socket_id = (int8_t)(long)s;

    FNET_DEBUG("fnet_socket_close, socket:%d", socket_id);

    socket_close(socket_id);

    return FNET_OK;
}
