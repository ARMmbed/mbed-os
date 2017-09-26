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

#include "netsocket/nsapi_types.h"

#include "lwip_tools.h"

/* Static arena of sockets */
struct mbed_lwip_socket mbed_lwip_arena[MEMP_NUM_NETCONN];

/* LWIP error remapping */
nsapi_error_t mbed_lwip_err_remap(err_t err) {
    switch (err) {
        case ERR_OK:
        case ERR_CLSD:
            return 0;
        case ERR_MEM:
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

#if LWIP_IPV4
static const ip_addr_t *mbed_lwip_get_ipv4_addr(const struct netif *netif)
{
    if (!netif_is_up(netif)) {
        return NULL;
    }

    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        return netif_ip_addr4(netif);
    }

    return NULL;
}
#endif

#if LWIP_IPV6
static const ip_addr_t *mbed_lwip_get_ipv6_addr(const struct netif *netif)
{
    if (!netif_is_up(netif)) {
        return NULL;
    }

    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) &&
                !ip6_addr_islinklocal(netif_ip6_addr(netif, i))) {
            return netif_ip_addr6(netif, i);
        }
    }

    return NULL;
}
#endif

const ip_addr_t *mbed_lwip_get_ip_addr(bool any_addr, const struct netif *netif)
{
    const ip_addr_t *pref_ip_addr = 0;
    const ip_addr_t *npref_ip_addr = 0;

#if LWIP_IPV4 && LWIP_IPV6
#if IP_VERSION_PREF == PREF_IPV4
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
#else
    pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif
#elif LWIP_IPV6
    pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
#elif LWIP_IPV4
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif

    if (pref_ip_addr) {
        return pref_ip_addr;
    } else if (npref_ip_addr && any_addr) {
        return npref_ip_addr;
    }

    return NULL;
}

void mbed_lwip_arena_init(void)
{
    memset(mbed_lwip_arena, 0, sizeof(mbed_lwip_arena));
}

struct mbed_lwip_socket *mbed_lwip_arena_alloc(void)
{
    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (!mbed_lwip_arena[i].in_use) {
            struct mbed_lwip_socket *s = &mbed_lwip_arena[i];
            memset(s, 0, sizeof(*s));
            s->in_use = true;
            sys_arch_unprotect(prot);
            return s;
        }
    }

    sys_arch_unprotect(prot);
    return 0;
}

void mbed_lwip_arena_dealloc(struct mbed_lwip_socket *s)
{
    s->in_use = false;
}
