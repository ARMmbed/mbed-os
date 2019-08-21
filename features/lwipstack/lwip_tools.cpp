/* LWIP common helpers
 * Copyright (c) 2017 ARM Limited
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

#include <stdbool.h>
#include <string.h>

#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/api.h"

#include "LWIPStack.h"
#include "lwip_tools.h"

#include "netsocket/nsapi_types.h"

#if !LWIP_IPV4 || !LWIP_IPV6
static bool all_zeros(const uint8_t *p, int len);
#endif

/* LWIP error remapping */
nsapi_error_t LWIP::err_remap(err_t err)
{
    switch (err) {
        case ERR_OK:
        case ERR_CLSD:
            return 0;
        case ERR_MEM:
        case ERR_BUF:
            return NSAPI_ERROR_NO_MEMORY;
        case ERR_CONN:
        case ERR_RST:
        case ERR_ABRT:
            return NSAPI_ERROR_NO_CONNECTION;
        case ERR_TIMEOUT:
        case ERR_RTE:
        case ERR_WOULDBLOCK:
            return NSAPI_ERROR_WOULD_BLOCK;
        case ERR_VAL:
        case ERR_USE:
        case ERR_ARG:
            return NSAPI_ERROR_PARAMETER;
        case ERR_INPROGRESS:
            return NSAPI_ERROR_IN_PROGRESS;
        case ERR_ALREADY:
            return NSAPI_ERROR_ALREADY;
        case ERR_ISCONN:
            return NSAPI_ERROR_IS_CONNECTED;
        default:
            return NSAPI_ERROR_DEVICE_ERROR;
    }
}


const ip_addr_t *LWIP::get_ipv4_addr(const struct netif *netif)
{
#if LWIP_IPV4
    if (!netif_is_up(netif)) {
        return NULL;
    }

    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        return netif_ip_addr4(netif);
    }
#endif
    return NULL;
}

const ip_addr_t *LWIP::get_ipv6_addr(const struct netif *netif)
{
#if LWIP_IPV6
    if (!netif_is_up(netif)) {
        return NULL;
    }

    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) &&
                !ip6_addr_islinklocal(netif_ip6_addr(netif, i))) {
            return netif_ip_addr6(netif, i);
        }
    }
#endif
    return NULL;
}

bool LWIP::is_local_addr(const ip_addr_t *ip_addr)
{
    struct netif *netif;

    for (netif = netif_list; netif != NULL; netif = netif->next) {
        if (!netif_is_up(netif)) {
            continue;
        }
#if LWIP_IPV6
        if (IP_IS_V6(ip_addr)) {
            for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
                if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) &&
                        ip6_addr_cmp(netif_ip6_addr(netif, i), ip_2_ip6(ip_addr))) {
                    return true;
                }
            }
        }
#endif

#if LWIP_IPV4
        if (IP_IS_V4(ip_addr)) {
            if (!ip4_addr_isany(netif_ip4_addr(netif)) &&
                    ip4_addr_cmp(netif_ip4_addr(netif), ip_2_ip4(ip_addr))) {
                return true;
            }
        }
#endif
    }
    return false;
}

const ip_addr_t *LWIP::get_ip_addr(bool any_addr, const struct netif *netif)
{
    const ip_addr_t *pref_ip_addr = 0;
    const ip_addr_t *npref_ip_addr = 0;

#if LWIP_IPV4 && LWIP_IPV6
#if IP_VERSION_PREF == PREF_IPV4
    pref_ip_addr = get_ipv4_addr(netif);
    npref_ip_addr = get_ipv6_addr(netif);
#else
    pref_ip_addr = get_ipv6_addr(netif);
    npref_ip_addr = get_ipv4_addr(netif);
#endif
#elif LWIP_IPV6
    pref_ip_addr = get_ipv6_addr(netif);
#elif LWIP_IPV4
    pref_ip_addr = get_ipv4_addr(netif);
#endif

    if (pref_ip_addr) {
        return pref_ip_addr;
    } else if (npref_ip_addr && any_addr) {
        return npref_ip_addr;
    }

    return NULL;
}

void LWIP::arena_init(void)
{
    memset(arena, 0, sizeof(arena));
}

struct LWIP::mbed_lwip_socket *LWIP::arena_alloc()
{
    LWIP &lwip = LWIP::get_instance();

    lwip.adaptation.lock();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (!arena[i].in_use) {
            struct mbed_lwip_socket *s = &arena[i];
            memset(s, 0, sizeof(*s));
            s->in_use = true;
            lwip.adaptation.unlock();
            return s;
        }
    }

    lwip.adaptation.unlock();

    return 0;
}

void LWIP::arena_dealloc(struct mbed_lwip_socket *s)
{
    s->in_use = false;

    while (s->multicast_memberships_count > 0) {
        uint32_t index = 0;
        index = next_registered_multicast_member(s, index);

        setsockopt(s, NSAPI_SOCKET, NSAPI_DROP_MEMBERSHIP, &s->multicast_memberships[index],
                   sizeof(s->multicast_memberships[index]));
        index++;
    }

    free(s->multicast_memberships);
    s->multicast_memberships = NULL;
}

bool convert_lwip_addr_to_mbed(nsapi_addr_t *out, const ip_addr_t *in)
{
#if LWIP_IPV6
    if (IP_IS_V6(in)) {
        out->version = NSAPI_IPv6;
        SMEMCPY(out->bytes, ip_2_ip6(in), sizeof(ip6_addr_t));
        return true;
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(in)) {
        out->version = NSAPI_IPv4;
        SMEMCPY(out->bytes, ip_2_ip4(in), sizeof(ip4_addr_t));
        return true;
    }
#endif
#if LWIP_IPV6 && LWIP_IPV4
    return false;
#endif
}

bool convert_mbed_addr_to_lwip(ip_addr_t *out, const nsapi_addr_t *in)
{
#if LWIP_IPV6
    if (in->version == NSAPI_IPv6) {
        IP_SET_TYPE(out, IPADDR_TYPE_V6);
        SMEMCPY(ip_2_ip6(out), in->bytes, sizeof(ip6_addr_t));
        return true;
    }
#if !LWIP_IPV4
    /* For bind() and other purposes, need to accept "null" of other type */
    /* (People use IPv4 0.0.0.0 as a general null) */
    if (in->version == NSAPI_UNSPEC ||
            (in->version == NSAPI_IPv4 && all_zeros(in->bytes, 4))) {
        ip_addr_set_zero_ip6(out);
        return true;
    }
#endif
#endif

#if LWIP_IPV4
    if (in->version == NSAPI_IPv4) {
        IP_SET_TYPE(out, IPADDR_TYPE_V4);
        SMEMCPY(ip_2_ip4(out), in->bytes, sizeof(ip4_addr_t));
        return true;
    }
#if !LWIP_IPV6
    /* For symmetry with above, accept IPv6 :: as a general null */
    if (in->version == NSAPI_UNSPEC ||
            (in->version == NSAPI_IPv6 && all_zeros(in->bytes, 16))) {
        ip_addr_set_zero_ip4(out);
        return true;
    }
#endif
#endif

#if LWIP_IPV4 && LWIP_IPV6
    if (in->version == NSAPI_UNSPEC) {
#if IP_VERSION_PREF == PREF_IPV4
        ip_addr_set_zero_ip4(out);
#else
        ip_addr_set_zero_ip6(out);
#endif
        return true;
    }
#endif

    return false;
}

#if !LWIP_IPV4 || !LWIP_IPV6
static bool all_zeros(const uint8_t *p, int len)
{
    for (int i = 0; i < len; i++) {
        if (p[i]) {
            return false;
        }
    }

    return true;
}
#endif
