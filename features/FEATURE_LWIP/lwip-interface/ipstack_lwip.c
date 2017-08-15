/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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

#include "nsapi.h"
#include "mbed_interface.h"
#include "mbed_assert.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/mld6.h"
#include "lwip/dns.h"
#include "lwip/udp.h"

#include "emac_api.h"
#include "emac_lwip.h"
#include "mbed_ipstack.h"
#include "lwip_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

static bool mbed_lwip_inited = false;
static sys_sem_t lwip_tcpip_inited;

static void add_dns_addr(struct netif *lwip_netif)
{
    // Do nothing if not brought up
    const ip_addr_t *ip_addr = mbed_lwip_get_ip_addr(true, lwip_netif);
    if (!ip_addr) {
        return;
    }

    // Check for existing dns server
    for (char numdns = 0; numdns < DNS_MAX_SERVERS; numdns++) {
        const ip_addr_t *dns_ip_addr = dns_getserver(numdns);
        if (!ip_addr_isany(dns_ip_addr)) {
            return;
        }
    }

#if LWIP_IPV6
    if (IP_IS_V6(ip_addr)) {
        /* 2001:4860:4860::8888 google */
        ip_addr_t ipv6_dns_addr = IPADDR6_INIT(
                PP_HTONL(0x20014860UL),
                PP_HTONL(0x48600000UL),
                PP_HTONL(0x00000000UL),
                PP_HTONL(0x00008888UL));
        dns_setserver(0, &ipv6_dns_addr);
    }
#endif

#if LWIP_IPV4
    if (IP_IS_V4(ip_addr)) {
        /* 8.8.8.8 google */
        ip_addr_t ipv4_dns_addr = IPADDR4_INIT(0x08080808);
        dns_setserver(0, &ipv4_dns_addr);
    }
#endif
}

static void mbed_lwip_tcpip_init_irq(void *eh)
{
    sys_sem_signal(&lwip_tcpip_inited);
}

static void mbed_lwip_netif_link_irq(struct netif *netif)
{
    if (netif_is_link_up(netif)) {
        emac_interface_t *emac = netif->state;
        sys_sem_signal(&emac->linked);
    }
}

static void mbed_lwip_netif_status_irq(struct netif *netif)
{
    static bool any_addr = true;
    emac_interface_t *emac = netif->state;

    if (netif_is_up(netif)) {
        // Indicates that has address
        if (any_addr == true && mbed_lwip_get_ip_addr(true, netif)) {
            sys_sem_signal(&emac->has_addr);
            any_addr = false;
            return;
        }

        // Indicates that has preferred address
        if (mbed_lwip_get_ip_addr(false, netif)) {
            sys_sem_signal(&emac->has_addr);
        }
    } else {
        any_addr = true;
    }
}

#if LWIP_IPV6
static void mbed_lwip_clear_ipv6_addresses(struct netif *netif)
{
    for (u8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        netif_ip6_addr_set_state(netif, i, IP6_ADDR_INVALID);
    }
}
#endif

char *mbed_ipstack_get_mac_address(emac_interface_t *emac)
{
    return emac->hwaddr;
}

char *mbed_ipstack_get_ip_address(emac_interface_t *emac, char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = mbed_lwip_get_ip_addr(true, &emac->netif);
    if (!addr) {
        return NULL;
    }
#if LWIP_IPV6
    if (IP_IS_V6(addr)) {
        return ip6addr_ntoa_r(ip_2_ip6(addr), buf, buflen);
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(addr)) {
        return ip4addr_ntoa_r(ip_2_ip4(addr), buf, buflen);
    }
#endif
    return NULL;
}

char *mbed_ipstack_get_netmask(emac_interface_t *emac, char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_netmask(&emac->netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

char *mbed_ipstack_get_gateway(emac_interface_t *emac, char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_gw(&emac->netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

void mbed_ipstack_init(void)
{
    if(mbed_lwip_inited)
        return;

    sys_sem_new(&lwip_tcpip_inited, 0);
    tcpip_init(mbed_lwip_tcpip_init_irq, NULL);
    sys_arch_sem_wait(&lwip_tcpip_inited, 0);

    // Zero out socket set
    mbed_lwip_arena_init();

    mbed_lwip_inited = true;
}

nsapi_error_t mbed_ipstack_add_netif(emac_interface_t *emac, bool default_if)
{
    emac->connected = false;
    emac->dhcp = true;
    sys_sem_new(&emac->linked, 0);
    sys_sem_new(&emac->has_addr, 0);
    memset(&emac->netif, 0, sizeof(emac->netif));
    emac->netif.state = emac;

    mbed_mac_address(emac->hwaddr);

    if (!netif_add(&emac->netif,
#if LWIP_IPV4
            0, 0, 0,
#endif
            emac, emac_lwip_if_init, tcpip_input)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (default_if)
        netif_set_default(&emac->netif);

    netif_set_link_callback(&emac->netif, mbed_lwip_netif_link_irq);
    netif_set_status_callback(&emac->netif, mbed_lwip_netif_status_irq);

    return NSAPI_ERROR_OK;
}

nsapi_error_t mbed_ipstack_bringup(emac_interface_t *emac, bool dhcp, const char *ip, const char *netmask, const char *gw)
{
    // Check if we've already connected
    if (emac->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    mbed_ipstack_init();

#if LWIP_IPV6
    netif_create_ip6_linklocal_address(&emac->netif, 1/*from MAC*/);
#if LWIP_IPV6_MLD
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if (emac->netif.mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    emac->netif.mld_mac_filter(&emac->netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6_MLD */

#if LWIP_IPV6_AUTOCONFIG
    /* IPv6 address autoconfiguration not enabled by default */
  emac->netif.ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */

#endif /* LWIP_IPV6 */

    u32_t ret;

    if (!netif_is_link_up(&emac->netif)) {
        ret = sys_arch_sem_wait(&emac->linked, 15000);

        if (ret == SYS_ARCH_TIMEOUT) {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

#if LWIP_IPV4
    if (!dhcp) {
        ip4_addr_t ip_addr;
        ip4_addr_t netmask_addr;
        ip4_addr_t gw_addr;

        if (!inet_aton(ip, &ip_addr) ||
            !inet_aton(netmask, &netmask_addr) ||
            !inet_aton(gw, &gw_addr)) {
            return NSAPI_ERROR_PARAMETER;
        }

        netif_set_addr(&emac->netif, &ip_addr, &netmask_addr, &gw_addr);
    }
#endif

    netif_set_up(&emac->netif);

#if LWIP_IPV4
    // Connect to the network
    emac->dhcp = dhcp;

    if (dhcp) {
        err_t err = dhcp_start(&emac->netif);
        if (err) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }
    }
#endif

    // If doesn't have address
    if (!mbed_lwip_get_ip_addr(true, &emac->netif)) {
        ret = sys_arch_sem_wait(&emac->has_addr, 15000);
        if (ret == SYS_ARCH_TIMEOUT) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }

        emac->connected = true;
    }

#if ADDR_TIMEOUT
    // If address is not for preferred stack waits a while to see
    // if preferred stack address is acquired
    if (!mbed_lwip_get_ip_addr(false, &emac->netif)) {
        ret = sys_arch_sem_wait(&emac->has_addr, ADDR_TIMEOUT * 1000);
    }
#endif

    add_dns_addr(&emac->netif);

    return 0;
}

nsapi_error_t mbed_ipstack_bringdown(emac_interface_t *emac)
{
    // Check if we've connected
    if (!emac->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

#if LWIP_IPV4
    // Disconnect from the network
    if (emac->dhcp) {
        dhcp_release(&emac->netif);
        dhcp_stop(&emac->netif);
        emac->dhcp = false;
    }
#endif

    netif_set_down(&emac->netif);

#if LWIP_IPV6
    mbed_lwip_clear_ipv6_addresses(&emac->netif);
#endif

    sys_sem_free(&emac->has_addr);
    sys_sem_new(&emac->has_addr, 0);
    emac->connected = false;
    return 0;
}

extern const nsapi_stack_api_t lwip_stack_api; /* Defined in nsapi_stack_lwip.c */

void mbed_ipstack_set_stack(emac_interface_t *emac, nsapi_stack_t *stack)
{
    stack->emac = emac;
    stack->stack_api = &lwip_stack_api;
}

#ifdef __cplusplus
}
#endif
