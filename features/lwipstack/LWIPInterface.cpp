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

#define __STDC_LIMIT_MACROS

#include "nsapi.h"
#include "mbed_interface.h"
#include "mbed_assert.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <new>
#include <stdint.h>

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

#include "ppp_lwip.h"

#include "LWIPStack.h"

LWIP::Interface *LWIP::Interface::list;

LWIP::Interface *LWIP::Interface::our_if_from_netif(struct netif *netif)
{
    for (Interface *interface = list; interface; interface = interface->next) {
        if (netif == &interface->netif) {
            return interface;
        }
    }

    return NULL;
}

static void add_dns_addr_to_dns_list_index(const u8_t addr_type, const u8_t index)
{
#if LWIP_IPV6
    if (addr_type == IPADDR_TYPE_V6) {
        /* 2001:4860:4860::8888 google */
        ip_addr_t ipv6_dns_addr = IPADDR6_INIT(
                PP_HTONL(0x20014860UL),
                PP_HTONL(0x48600000UL),
                PP_HTONL(0x00000000UL),
                PP_HTONL(0x00008888UL));
        dns_setserver(index, &ipv6_dns_addr);
    }
#endif
#if LWIP_IPV4
    if (addr_type == IPADDR_TYPE_V4) {
        /* 8.8.8.8 google */
        ip_addr_t ipv4_dns_addr = IPADDR4_INIT(0x08080808);
        dns_setserver(index, &ipv4_dns_addr);
    }
#endif
}

static int get_ip_addr_type(const ip_addr_t *ip_addr)
{
#if LWIP_IPV6
    if (IP_IS_V6(ip_addr)) {
        return IPADDR_TYPE_V6;
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(ip_addr)) {
        return IPADDR_TYPE_V4;
    }
#endif
#if LWIP_IPV6 && LWIP_IPV4
    return IPADDR_TYPE_ANY;
#endif
}

void LWIP::add_dns_addr(struct netif *lwip_netif)
{
    // Check for existing dns address
    for (char numdns = 0; numdns < DNS_MAX_SERVERS; numdns++) {
        const ip_addr_t *dns_ip_addr = dns_getserver(numdns);
        if (!ip_addr_isany(dns_ip_addr)) {
            return;
        }
    }

    // Get preferred ip version
    const ip_addr_t *ip_addr = get_ip_addr(false, lwip_netif);
    u8_t addr_type = IPADDR_TYPE_ANY;

    // Add preferred ip version dns address to index 0
    if (ip_addr) {
        addr_type = get_ip_addr_type(ip_addr);
        add_dns_addr_to_dns_list_index(addr_type, 0);
    }

#if LWIP_IPV4 && LWIP_IPV6
    if (!ip_addr) {
        // Get address for any ip version
        ip_addr = get_ip_addr(true, lwip_netif);
        if (!ip_addr) {
            return;
        }
        addr_type = get_ip_addr_type(ip_addr);
        // Add the dns address to index 0
        add_dns_addr_to_dns_list_index(addr_type, 0);
    }

    if (addr_type == IPADDR_TYPE_V4) {
        // If ipv4 is preferred and ipv6 is available add ipv6 dns address to index 1
        ip_addr = get_ipv6_addr(lwip_netif);
    } else if (addr_type == IPADDR_TYPE_V6) {
        // If ipv6 is preferred and ipv4 is available add ipv4 dns address to index 1
        ip_addr = get_ipv4_addr(lwip_netif);
    } else {
        ip_addr = NULL;
    }

    if (ip_addr) {
        addr_type = get_ip_addr_type(ip_addr);
        add_dns_addr_to_dns_list_index(addr_type, 1);
    }
#endif
}

nsapi_error_t LWIP::Interface::set_dhcp()
{
    netif_set_up(&netif);

#if LWIP_DHCP
    if (dhcp_has_to_be_set) {
        err_t err = dhcp_start(&netif);
        dhcp_has_to_be_set = false;
        if (err) {
            connected = NSAPI_STATUS_DISCONNECTED;
            if (client_callback) {
                client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
            }
            return NSAPI_ERROR_DHCP_FAILURE;
        }
        dhcp_started = true;
    }
#endif

    return NSAPI_ERROR_OK;
}

void LWIP::Interface::netif_link_irq(struct netif *netif)
{
    LWIP::Interface *interface = our_if_from_netif(netif);

    if (netif_is_link_up(&interface->netif)) {
        nsapi_error_t dhcp_status = interface->set_dhcp();

        if (interface->blocking && dhcp_status == NSAPI_ERROR_OK) {
            osSemaphoreRelease(interface->linked);
        } else if (dhcp_status != NSAPI_ERROR_OK) {
            netif_set_down(&interface->netif);
        }
    } else {
        osSemaphoreRelease(interface->unlinked);
        netif_set_down(&interface->netif);
    }
}

void LWIP::Interface::netif_status_irq(struct netif *netif)
{
    LWIP::Interface *interface = our_if_from_netif(netif);

    if (netif_is_up(&interface->netif)) {
        bool dns_addr_has_to_be_added = false;
        if (!(interface->has_addr_state & HAS_ANY_ADDR) && LWIP::get_ip_addr(true, netif)) {
            if (interface->blocking) {
                osSemaphoreRelease(interface->has_any_addr);
            }
            interface->has_addr_state |= HAS_ANY_ADDR;
            dns_addr_has_to_be_added = true;
        }
#if PREF_ADDR_TIMEOUT
        if (!(interface->has_addr_state & HAS_PREF_ADDR) && LWIP::get_ip_addr(false, netif)) {
            if (interface->blocking) {
                osSemaphoreRelease(interface->has_pref_addr);
            }
            interface->has_addr_state |= HAS_PREF_ADDR;
            dns_addr_has_to_be_added = true;
        }
#endif
#if BOTH_ADDR_TIMEOUT
        if (!(interface->has_addr_state & HAS_BOTH_ADDR) && LWIP::get_ipv4_addr(netif) && LWIP::get_ipv6_addr(netif)) {
            if (interface->blocking) {
                osSemaphoreRelease(interface->has_both_addr);
            }
            interface->has_addr_state |= HAS_BOTH_ADDR;
            dns_addr_has_to_be_added = true;
        }
#endif
       if (dns_addr_has_to_be_added && !interface->blocking) {
            add_dns_addr(&interface->netif);
        }


        if (interface->has_addr_state & HAS_ANY_ADDR) {
            interface->connected = NSAPI_STATUS_GLOBAL_UP;
        }
    } else {
        interface->connected = NSAPI_STATUS_DISCONNECTED;
    }

    if (interface->client_callback) {
        interface->client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, interface->connected);
    }
}

void LWIP::Interface::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    client_callback = status_cb;
}

nsapi_connection_status_t LWIP::Interface::get_connection_status() const
{
    return connected;
}

#if LWIP_IPV6
static void mbed_lwip_clear_ipv6_addresses(struct netif *netif)
{
    for (u8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        netif_ip6_addr_set_state(netif, i, IP6_ADDR_INVALID);
    }
}
#endif

char *LWIP::Interface::get_mac_address(char *buf, nsapi_size_t buflen)
{
    (void) snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x",
             netif.hwaddr[0], netif.hwaddr[1], netif.hwaddr[2],
             netif.hwaddr[3], netif.hwaddr[4], netif.hwaddr[5]);
    return buf;
}

char *LWIP::Interface::get_ip_address(char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = LWIP::get_ip_addr(true, &netif);
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

char *LWIP::Interface::get_netmask(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_netmask(&netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

char *LWIP::Interface::get_gateway(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_gw(&netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

LWIP::Interface::Interface() :
        hw(NULL), has_addr_state(0),
        connected(NSAPI_STATUS_DISCONNECTED),
        dhcp_started(false), dhcp_has_to_be_set(false), blocking(true), ppp(false)
{
    memset(&netif, 0, sizeof netif);

    osSemaphoreAttr_t attr;
    attr.name = NULL;
    attr.attr_bits = 0;

    attr.cb_mem = &linked_sem;
    attr.cb_size = sizeof linked_sem;
    linked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &unlinked_sem;
    attr.cb_size = sizeof unlinked_sem;
    unlinked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &has_any_addr_sem;
    attr.cb_size = sizeof has_any_addr_sem;
    has_any_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#if PREF_ADDR_TIMEOUT
    attr.cb_mem = &has_pref_addr_sem;
    attr.cb_size = sizeof has_pref_addr_sem;
    has_pref_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
#if BOTH_ADDR_TIMEOUT
    attr.cb_mem = &has_both_addr_sem;
    attr.cb_size = sizeof has_both_addr_sem;
    has_both_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif

    next = list;
    list = this;
}

nsapi_error_t LWIP::add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out)
{
#if LWIP_ETHERNET
    Interface *interface = new (std::nothrow) Interface();
    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    interface->emac = &emac;
    interface->memory_manager = &memory_manager;
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
            interface, &LWIP::Interface::emac_if_init, tcpip_input)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (default_if) {
        netif_set_default(&interface->netif);
        default_interface = interface;
    }

    netif_set_link_callback(&interface->netif, &LWIP::Interface::netif_link_irq);
    netif_set_status_callback(&interface->netif, &LWIP::Interface::netif_status_irq);

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
nsapi_error_t LWIP::_add_ppp_interface(void *hw, bool default_if, nsapi_ip_stack_t stack, LWIP::Interface **interface_out)
{
#if LWIP_PPP_API
    Interface *interface = new (std::nothrow) Interface();
    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    interface->hw = hw;
    interface->ppp = true;

    nsapi_error_t ret = ppp_lwip_if_init(hw, &interface->netif, stack);
    if (ret != NSAPI_ERROR_OK) {
        free(interface);
        return ret;
    }

    if (default_if) {
        netif_set_default(&interface->netif);
        default_interface = interface;
    }

    netif_set_link_callback(&interface->netif, &LWIP::Interface::netif_link_irq);
    netif_set_status_callback(&interface->netif, &LWIP::Interface::netif_status_irq);

    *interface_out = interface;

    return NSAPI_ERROR_OK;
#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //LWIP_PPP_API
}


nsapi_error_t LWIP::Interface::bringup(bool dhcp, const char *ip, const char *netmask, const char *gw, const nsapi_ip_stack_t stack, bool block)
{
    // Check if we've already connected
    if (connected == NSAPI_STATUS_GLOBAL_UP) {
        return NSAPI_ERROR_IS_CONNECTED;
    } else if (connected == NSAPI_STATUS_CONNECTING) {
        return NSAPI_ERROR_ALREADY;
    }

    connected = NSAPI_STATUS_CONNECTING;
    blocking = block;

#if LWIP_DHCP
    if (stack != IPV6_STACK && dhcp) {
        dhcp_has_to_be_set = true;
    }
#endif

#if LWIP_IPV6
    if (stack != IPV4_STACK) {
        if (netif.hwaddr_len == 6) {
            netif_create_ip6_linklocal_address(&netif, 1/*from MAC*/);
        }
#if LWIP_IPV6_MLD
        /*
         * For hardware/netifs that implement MAC filtering.
         * All-nodes link-local is handled by default, so we must let the hardware know
         * to allow multicast packets in.
         * Should set mld_mac_filter previously. */
        if (netif.mld_mac_filter != NULL) {
            ip6_addr_t ip6_allnodes_ll;
            ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
            netif.mld_mac_filter(&netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
        }
#endif /* LWIP_IPV6_MLD */

#if LWIP_IPV6_AUTOCONFIG
        /* IPv6 address autoconfiguration not enabled by default */
        netif.ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */
    } else {
        // Disable rourter solicitations
        netif.rs_count = 0;
    }
#endif /* LWIP_IPV6 */

#if LWIP_IPV4
    if (stack != IPV6_STACK) {
        if (!dhcp && !ppp) {
            ip4_addr_t ip_addr;
            ip4_addr_t netmask_addr;
            ip4_addr_t gw_addr;

            if (!inet_aton(ip, &ip_addr) ||
                !inet_aton(netmask, &netmask_addr) ||
                !inet_aton(gw, &gw_addr)) {
                return NSAPI_ERROR_PARAMETER;
            }

            netif_set_addr(&netif, &ip_addr, &netmask_addr, &gw_addr);
        }
    }
#endif

    if (client_callback) {
        client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_CONNECTING);
    }

    if (ppp) {
       err_t err = ppp_lwip_connect(hw);
       if (err) {
           connected = NSAPI_STATUS_DISCONNECTED;
           if (client_callback) {
               client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
           }
          return err_remap(err);
       }
    }

    if (!netif_is_link_up(&netif)) {
        if (blocking) {
            if (osSemaphoreAcquire(linked, 15000) != osOK) {
                if (ppp) {
                    (void) ppp_lwip_disconnect(hw);
                }
                return NSAPI_ERROR_NO_CONNECTION;
            }
        }
    } else {
        nsapi_error_t ret = set_dhcp();
        if (ret != NSAPI_ERROR_OK) {
            return ret;
        }
    }

    if (!blocking) {
        // Done enough - as addresses are acquired, there will be
        // connected callbacks.
        // XXX shouldn't this be NSAPI_ERROR_IN_PROGRESS if in CONNECTING state?
        return NSAPI_ERROR_OK;
    }

    // If doesn't have address
    if (!LWIP::get_ip_addr(true, &netif)) {
        if (osSemaphoreAcquire(has_any_addr, DHCP_TIMEOUT * 1000) != osOK) {
            if (ppp) {
                (void) ppp_lwip_disconnect(hw);
            }
            return NSAPI_ERROR_DHCP_FAILURE;
        }
    }

#if PREF_ADDR_TIMEOUT
    if (stack != IPV4_STACK && stack != IPV6_STACK) {
        // If address is not for preferred stack waits a while to see
        // if preferred stack address is acquired
        if (!LWIP::get_ip_addr(false, &netif)) {
            osSemaphoreAcquire(has_pref_addr, PREF_ADDR_TIMEOUT * 1000);
        }
    }
#endif
#if BOTH_ADDR_TIMEOUT
    if (stack != IPV4_STACK && stack != IPV6_STACK) {
        // If addresses for both stacks are not available waits a while to
        // see if address for both stacks are acquired
        if (!(LWIP::get_ipv4_addr(&netif) && LWIP::get_ipv6_addr(&netif))) {
            osSemaphoreAcquire(has_both_addr, BOTH_ADDR_TIMEOUT * 1000);
        }
    }
#endif

    add_dns_addr(&netif);

    return NSAPI_ERROR_OK;
}

nsapi_error_t LWIP::Interface::bringdown()
{
    // Check if we've connected
    if (connected == NSAPI_STATUS_DISCONNECTED) {
        return NSAPI_ERROR_PARAMETER;
    }

#if LWIP_DHCP
    // Disconnect from the network
    if (dhcp_started) {
        dhcp_release(&netif);
        dhcp_stop(&netif);
        dhcp_started = false;
        dhcp_has_to_be_set = false;
    }
#endif

    if (ppp) {
        /* this is a blocking call, returns when PPP is properly closed */
       err_t err = ppp_lwip_disconnect(hw);
       if (err) {
           return err_remap(err);
       }
       MBED_ASSERT(!netif_is_link_up(&netif));
       /*if (netif_is_link_up(&netif)) {
           if (sys_arch_sem_wait(&unlinked, 15000) == SYS_ARCH_TIMEOUT) {
               return NSAPI_ERROR_DEVICE_ERROR;
           }
       }*/
    } else {
        netif_set_down(&netif);
    }

#if LWIP_IPV6
    mbed_lwip_clear_ipv6_addresses(&netif);
#endif
#if LWIP_IPV4
    ip_addr_set_zero(&(netif.ip_addr));
    ip_addr_set_zero(&(netif.netmask));
    ip_addr_set_zero(&(netif.gw));
#endif

    osSemaphoreDelete(has_any_addr);
    osSemaphoreAttr_t attr;
    attr.name = NULL;
    attr.attr_bits = 0;
    attr.cb_mem = &has_any_addr_sem;
    attr.cb_size = sizeof has_any_addr_sem;
    has_any_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#if PREF_ADDR_TIMEOUT
    osSemaphoreDelete(has_pref_addr);
    attr.cb_mem = &has_pref_addr_sem;
    attr.cb_size = sizeof has_pref_addr_sem;
    has_pref_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
#if BOTH_ADDR_TIMEOUT
    osSemaphoreDelete(has_both_addr);
    attr.cb_mem = &has_both_addr_sem;
    attr.cb_size = sizeof has_both_addr_sem;
    has_both_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
#endif
    has_addr_state = 0;

    connected = NSAPI_STATUS_DISCONNECTED;
    return 0;
}
