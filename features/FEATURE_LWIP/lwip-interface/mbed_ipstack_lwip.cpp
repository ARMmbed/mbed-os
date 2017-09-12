/* Copyright (c) 2017 ARM Limited
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

#include "mbed_ipstack_lwip.h"
#include "nsapi_stack_lwip.h"

extern "C" {
#include "lwip_tools.h"

static bool mbed_lwip_inited = false;

static os_semaphore_t tcpip_inited_sem = {0};
static const osSemaphoreAttr_t tcpip_inited_sem_attr = {
  .name = "",
  .attr_bits = 0,
  .cb_mem = &tcpip_inited_sem,
  .cb_size = sizeof(tcpip_inited_sem),
};

static os_semaphore_t linked_sem = {0};
static const osSemaphoreAttr_t linked_sem_attr = {
  .name = "",
  .attr_bits = 0,
  .cb_mem = &linked_sem,
  .cb_size = sizeof(linked_sem),
};

static os_semaphore_t has_addr_sem = {0};
static const osSemaphoreAttr_t has_addr_sem_attr = {
  .name = "",
  .attr_bits = 0,
  .cb_mem = &has_addr_sem,
  .cb_size = sizeof(has_addr_sem),
};

static osSemaphoreId_t tcpip_inited;

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
    osSemaphoreRelease(tcpip_inited);
}

static void mbed_lwip_netif_link_irq(struct netif *netif)
{
    mbed_ipstack_interface_t *interface = (mbed_ipstack_interface_t *)netif->state;
    if (netif_is_link_up(netif)) {
        osSemaphoreRelease(interface->linked);
    }
}

static void mbed_lwip_netif_status_irq(struct netif *netif)
{
    static bool any_addr = true;
    mbed_ipstack_interface_t *interface = (mbed_ipstack_interface_t *)netif->state;

    if (netif_is_up(netif)) {
        // Indicates that has address
        if (any_addr == true && mbed_lwip_get_ip_addr(true, netif)) {
            osSemaphoreRelease(interface->has_addr);
            any_addr = false;
            return;
        }

        // Indicates that has preferred address
        if (mbed_lwip_get_ip_addr(false, netif)) {
            osSemaphoreRelease(interface->has_addr);
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

char *mbed_ipstack_get_mac_address(mbed_ipstack_interface_t *interface)
{
    return interface->hwaddr;
}

char *mbed_ipstack_get_ip_address(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = mbed_lwip_get_ip_addr(true, &interface->netif);
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

char *mbed_ipstack_get_netmask(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_netmask(&interface->netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

char *mbed_ipstack_get_gateway(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_gw(&interface->netif);
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

    tcpip_inited = osSemaphoreNew(UINT16_MAX, 0, &tcpip_inited_sem_attr);
    tcpip_init(mbed_lwip_tcpip_init_irq, NULL);
    osSemaphoreAcquire(tcpip_inited, 0);

    // Zero out socket set
    mbed_lwip_arena_init();

    mbed_lwip_inited = true;
}

nsapi_error_t mbed_ipstack_add_ethernet_interface(const emac_interface_ops_t *emac_ops, void *hw, bool default_if, mbed_ipstack_interface_t **interface_out)
{
    mbed_ipstack_interface_t *interface = (mbed_ipstack_interface_t *)malloc(sizeof(mbed_ipstack_interface_t));
    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    memset(interface, 0, sizeof(mbed_ipstack_interface_t));
    interface->ops = emac_ops;
    interface->hw = hw;
    interface->connected = false;
    interface->dhcp = true;
    interface->linked = osSemaphoreNew(UINT16_MAX, 0, &linked_sem_attr);
    interface->has_addr = osSemaphoreNew(UINT16_MAX, 0, &has_addr_sem_attr);
    mbed_mac_address(interface->hwaddr);

    interface->netif.state = interface;

    if (!netif_add(&interface->netif,
#if LWIP_IPV4
            0, 0, 0,
#endif
            interface, emac_lwip_if_init, tcpip_input)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (default_if)
        netif_set_default(&interface->netif);

    netif_set_link_callback(&interface->netif, mbed_lwip_netif_link_irq);
    netif_set_status_callback(&interface->netif, mbed_lwip_netif_status_irq);

    *interface_out = interface;

    return NSAPI_ERROR_OK;
}

// TODO - PPP equivalent of above

nsapi_error_t mbed_ipstack_bringup(mbed_ipstack_interface_t *interface, bool dhcp, const char *ip, const char *netmask, const char *gw)
{
    // Check if we've already connected
    if (interface->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    mbed_ipstack_init();

#if LWIP_IPV6
    netif_create_ip6_linklocal_address(interface->netif, 1/*from MAC*/);
#if LWIP_IPV6_MLD
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if ((interface->netif)->mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    (interface->netif)->mld_mac_filter(interface->netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6_MLD */

#if LWIP_IPV6_AUTOCONFIG
    /* IPv6 address autoconfiguration not enabled by default */
  interface->netif->ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */

#endif /* LWIP_IPV6 */

    u32_t ret;

    if (!netif_is_link_up(&interface->netif)) {
        ret = osSemaphoreAcquire(interface->linked, 15000);
        if (ret != osOK) {
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

        netif_set_addr(&interface->netif, &ip_addr, &netmask_addr, &gw_addr);
    }
#endif

    netif_set_up(&interface->netif);

#if LWIP_IPV4
    // Connect to the network
    interface->dhcp = dhcp;

    if (dhcp) {
        err_t err = dhcp_start(&interface->netif);
        if (err) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }
    }
#endif

    // If doesn't have address
    if (!mbed_lwip_get_ip_addr(true, &interface->netif)) {
        ret = osSemaphoreAcquire(interface->has_addr, 15000);
        if (ret != osOK) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }
        interface->connected = true;
    }

#if ADDR_TIMEOUT
    // If address is not for preferred stack waits a while to see
    // if preferred stack address is acquired
    if (!mbed_lwip_get_ip_addr(false, interface->netif)) {
        ret = sys_arch_sem_wait(&interface->has_addr, ADDR_TIMEOUT * 1000);
    }
#endif

    add_dns_addr(&interface->netif);

    return 0;
}

nsapi_error_t mbed_ipstack_bringdown(mbed_ipstack_interface_t *interface)
{
    // Check if we've connected
    if (!interface->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

#if LWIP_IPV4
    // Disconnect from the network
    if (interface->dhcp) {
        dhcp_release(&interface->netif);
        dhcp_stop(&interface->netif);
        interface->dhcp = false;
    }
#endif

    netif_set_down(&interface->netif);

#if LWIP_IPV6
    mbed_lwip_clear_ipv6_addresses(interface->netif);
#endif

    interface->has_addr = osSemaphoreNew(UINT16_MAX, 0, &has_addr_sem_attr);
    interface->connected = false;
    return 0;
}

} // extern "C"

NetworkStack *mbed_ipstack_get_stack() {
	return nsapi_create_stack(&lwip_stack);
}
