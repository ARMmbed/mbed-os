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
#include "ppp_lwip.h"
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
    } else {
        osSemaphoreRelease(interface->unlinked);
    }
}

static void mbed_lwip_netif_status_irq(struct netif *netif)
{
    mbed_ipstack_interface_t *interface = (mbed_ipstack_interface_t *)netif->state;

    if (netif_is_up(&interface->netif)) {
        if (!(interface->has_addr_state & HAS_ANY_ADDR) && mbed_lwip_get_ip_addr(true, netif)) {
            osSemaphoreRelease(interface->has_any_addr);
            interface->has_addr_state |= HAS_ANY_ADDR;
        }
#if PREF_ADDR_TIMEOUT
        if (!(interface->has_addr_state & HAS_PREF_ADDR) && mbed_lwip_get_ip_addr(false, netif)) {
            osSemaphoreRelease(interface->has_pref_addr);
            interface->has_addr_state |= HAS_PREF_ADDR;
        }
#endif
#if BOTH_ADDR_TIMEOUT
        if (!(interface->has_addr_state & HAS_BOTH_ADDR) && mbed_lwip_get_ipv4_addr(netif) && mbed_lwip_get_ipv6_addr(netif)) {
            osSemaphoreRelease(interface->has_both_addr);
            interface->has_addr_state |= HAS_BOTH_ADDR;
        }
#endif
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

char *mbed_ipstack_get_mac_address(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen)
{
    (void) snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x",
             interface->netif.hwaddr[0], interface->netif.hwaddr[1], interface->netif.hwaddr[2],
             interface->netif.hwaddr[3], interface->netif.hwaddr[4], interface->netif.hwaddr[5]);
    return buf;
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
#if LWIP_IPV6 && LWIP_IPV4
    return NULL;
#endif
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

    // Seed lwip random
    lwip_seed_random();

    // Initialise TCP sequence number
    uint32_t tcp_isn_secret[4];
    for (int i = 0; i < 4; i++) {
        tcp_isn_secret[i] = LWIP_RAND();
    }
    lwip_init_tcp_isn(0, (u8_t *) &tcp_isn_secret);

    tcpip_inited = osSemaphoreNew(UINT16_MAX, 0, &tcpip_inited_sem_attr);
    tcpip_init(mbed_lwip_tcpip_init_irq, NULL);
    osSemaphoreAcquire(tcpip_inited, 0);

    // Zero out socket set
    mbed_lwip_arena_init();

    mbed_lwip_inited = true;
}

static mbed_ipstack_interface_t *mbed_ipstack_allocate_interface(void *hw)
{
    mbed_ipstack_interface_t *interface = (mbed_ipstack_interface_t *)malloc(sizeof(mbed_ipstack_interface_t));
    if (!interface) {
        return NULL;
    }
    memset(interface, 0, sizeof(mbed_ipstack_interface_t));
    osSemaphoreAttr_t attr;
    attr.name = NULL;
    attr.attr_bits = 0;

    attr.cb_mem = &interface->linked_sem;
    attr.cb_size = sizeof interface->linked_sem;
    interface->linked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &interface->unlinked_sem;
    attr.cb_size = sizeof interface->unlinked_sem;
    interface->unlinked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &interface->has_any_addr_sem;
    attr.cb_size = sizeof interface->has_any_addr_sem;
    interface->has_any_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#if PREF_ADDR_TIMEOUT
    attr.cb_mem = &interface->has_pref_addr_sem;
    attr.cb_size = sizeof interface->has_pref_addr_sem;
    interface->has_pref_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
#if BOTH_ADDR_TIMEOUT
    attr.cb_mem = &interface->has_both_addr_sem;
    attr.cb_size = sizeof interface->has_both_addr_sem;
    interface->has_both_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif

    interface->netif.state = interface;
    interface->hw = hw;

    return interface;
}

nsapi_error_t mbed_ipstack_add_ethernet_interface(const emac_interface_ops_t *emac_ops, void *hw, bool default_if, mbed_ipstack_interface_t **interface_out)
{
#if LWIP_ETHERNET
    mbed_ipstack_interface_t *interface = mbed_ipstack_allocate_interface(hw);
    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    interface->ops = emac_ops;
    interface->dhcp = true;
    interface->ppp = false;

#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    netif->interface.hwaddr[0] = MBED_MAC_ADDR_0;
    netif->interface.hwaddr[1] = MBED_MAC_ADDR_1;
    netif->interface.hwaddr[2] = MBED_MAC_ADDR_2;
    netif->interface.hwaddr[3] = MBED_MAC_ADDR_3;
    netif->interface.hwaddr[4] = MBED_MAC_ADDR_4;
    netif->interface.hwaddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *) interface->netif.hwaddr);
#endif

    interface->netif.hwaddr_len = 6;

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

    /* Use mac address as additional seed to random number generator */
    uint64_t seed = interface->netif.hwaddr[0];
    for (uint8_t i = 1; i < 8; i++) {
        seed <<= 8;
        seed |= interface->netif.hwaddr[i % 6];
    }
    lwip_add_random_seed(seed);

    return NSAPI_ERROR_OK;
#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //LWIP_ETHERNET
}

/* Internal API to preserve existing PPP functionality - revise to better match mbed_ipstak_add_ethernet_interface later */
nsapi_error_t mbed_ipstack_lwip_add_ppp_interface(void *hw, bool default_if, mbed_ipstack_interface_t **interface_out)
{
#if LWIP_PPP
    mbed_ipstack_interface_t *interface = mbed_ipstack_allocate_interface(hw);
    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    interface->ops = NULL;
    interface->dhcp = true;
    interface->ppp = true;

    ret = ppp_lwip_if_init(hw, &interface->netif);
    if (ret != NSAPI_ERROR_OK) {
        free(interface);
        return ret;
    }

    if (default_if)
        netif_set_default(&interface->netif);

    netif_set_link_callback(&interface->netif, mbed_lwip_netif_link_irq);
    netif_set_status_callback(&interface->netif, mbed_lwip_netif_status_irq);

    *interface_out = interface;

#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //LWIP_PPP
}

nsapi_error_t mbed_ipstack_bringup(mbed_ipstack_interface_t *interface, bool dhcp, const char *ip, const char *netmask, const char *gw, const nsapi_ip_stack_t stack)
{
    // Check if we've already connected
    if (interface->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    mbed_ipstack_init();

#if LWIP_IPV6
    if (stack != IPV4_STACK) {
        if (interface->netif.hwaddr_len == 6) {
            netif_create_ip6_linklocal_address(interface->netif, 1/*from MAC*/);
        }
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
        interface->netif.ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */
    } else {
        // Disable rourter solicitations
        interface->netif.rs_count = 0;
    }
#endif /* LWIP_IPV6 */

#if LWIP_IPV4
    if (stack != IPV6_STACK) {
        if (!dhcp && !interface->ppp) {
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
    }
#endif

    netif_set_up(&interface->netif);
    if (interface->ppp) {
       err_t err = ppp_lwip_connect(interface->hw);
       if (err) {
           return mbed_lwip_err_remap(err);
       }
    }

    if (!netif_is_link_up(&interface->netif)) {
        if (osSemaphoreAcquire(interface->linked, 15000) != osOK) {
            if (interface->ppp) {
                ppp_lwip_disconnect(interface->hw);
            }
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    if (!interface->ppp) {
        netif_set_up(&interface->netif);
    }

#if LWIP_DHCP
    if (stack != IPV6_STACK) {
        // Connect to the network
        interface->dhcp = dhcp;

        if (dhcp) {
            err_t err = dhcp_start(&interface->netif);
            if (err) {
                return NSAPI_ERROR_DHCP_FAILURE;
            }
        }
    }
#endif

    // If doesn't have address
    if (!mbed_lwip_get_ip_addr(true, &interface->netif)) {
        if (osSemaphoreAcquire(interface->has_any_addr, DHCP_TIMEOUT * 1000) != osOK) {
            if (interface->ppp) {
                ppp_lwip_disconnect(interface->hw);
            }
            return NSAPI_ERROR_DHCP_FAILURE;
        }
        interface->connected = true;
    }

#if PREF_ADDR_TIMEOUT
    if (stack != IPV4_STACK && stack != IPV6_STACK) {
        // If address is not for preferred stack waits a while to see
        // if preferred stack address is acquired
        if (!mbed_lwip_get_ip_addr(false, &interface->netif)) {
            osSemaphoreAcquire(interface->has_pref_addr, PREF_ADDR_TIMEOUT * 1000);
        }
    }
#endif
#if BOTH_ADDR_TIMEOUT
    if (stack != IPV4_STACK && stack != IPV6_STACK) {
        // If addresses for both stacks are not available waits a while to
        // see if address for both stacks are acquired
        if (!(mbed_lwip_get_ipv4_addr(&interface->netif) && mbed_lwip_get_ipv6_addr(&interface->netif))) {
            osSemaphoreAcquire(interface->has_both_addr, BOTH_ADDR_TIMEOUT * 1000);
        }
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

#if LWIP_DHCP
    // Disconnect from the network
    if (interface->dhcp) {
        dhcp_release(&interface->netif);
        dhcp_stop(&interface->netif);
        interface->dhcp = false;
    }
#endif

    if (interface->ppp) {
        /* this is a blocking call, returns when PPP is properly closed */
       err_t err = ppp_lwip_disconnect(interface->hw);
       if (err) {
           return mbed_lwip_err_remap(err);
       }
       MBED_ASSERT(!netif_is_link_up(&interface->netif));
       /*if (netif_is_link_up(&interface->netif)) {
           if (sys_arch_sem_wait(&interface->unlinked, 15000) == SYS_ARCH_TIMEOUT) {
               return NSAPI_ERROR_DEVICE_ERROR;
           }
       }*/
    } else {
        netif_set_down(&interface->netif);
    }

#if LWIP_IPV6
    mbed_lwip_clear_ipv6_addresses(interface->netif);
#endif

    osSemaphoreDelete(interface->has_any_addr);
    osSemaphoreAttr_t attr;
    attr.name = NULL;
    attr.attr_bits = 0;
    attr.cb_mem = &interface->has_any_addr_sem;
    attr.cb_size = sizeof interface->has_any_addr_sem;
    interface->has_any_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#if PREF_ADDR_TIMEOUT
    osSemaphoreDelete(interface->has_pref_addr);
    attr.cb_mem = &interface->has_pref_addr_sem;
    attr.cb_size = sizeof interface->has_pref_addr_sem;
    interface->has_pref_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
#if BOTH_ADDR_TIMEOUT
    osSemaphoreDelete(interface->has_both_addr);
    attr.cb_mem = &interface->has_both_addr_sem;
    attr.cb_size = sizeof interface->has_both_addr_sem;
    interface->has_both_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
    interface->has_addr_state = 0;

    interface->connected = false;
    return 0;
}

} // extern "C"

NetworkStack *mbed_ipstack_get_stack() {
	return nsapi_create_stack(&lwip_stack);
}
