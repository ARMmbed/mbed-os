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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "eth_arch.h"
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

#if DEVICE_EMAC
    #define MBED_NETIF_INIT_FN emac_lwip_if_init
#else
    #define MBED_NETIF_INIT_FN eth_arch_enetif_init
#endif

#define DHCP_TIMEOUT 15000

/* Static arena of sockets */
static struct lwip_socket {
    bool in_use;

    struct netconn *conn;
    struct netbuf *buf;
    u16_t offset;

    void (*cb)(void *);
    void *data;
} lwip_arena[MEMP_NUM_NETCONN];

static bool lwip_connected = false;

static void mbed_lwip_arena_init(void)
{
    memset(lwip_arena, 0, sizeof lwip_arena);
}

static struct lwip_socket *mbed_lwip_arena_alloc(void)
{
    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (!lwip_arena[i].in_use) {
            struct lwip_socket *s = &lwip_arena[i];
            memset(s, 0, sizeof *s);
            s->in_use = true;
            sys_arch_unprotect(prot);
            return s;
        }
    }

    sys_arch_unprotect(prot);
    return 0;
}

static void mbed_lwip_arena_dealloc(struct lwip_socket *s)
{
    s->in_use = false;
}

static void mbed_lwip_socket_callback(struct netconn *nc, enum netconn_evt eh, u16_t len)
{
    // Filter send minus events
    if (eh == NETCONN_EVT_SENDMINUS && nc->state == NETCONN_WRITE) {
        return;
    }

    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (lwip_arena[i].in_use
            && lwip_arena[i].conn == nc
            && lwip_arena[i].cb) {
            lwip_arena[i].cb(lwip_arena[i].data);
        }
    }

    sys_arch_unprotect(prot);
}


/* TCP/IP and Network Interface Initialisation */
static struct netif lwip_netif;
static bool lwip_dhcp = false;
static char lwip_mac_address[NSAPI_MAC_SIZE];

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

static bool convert_mbed_addr_to_lwip(ip_addr_t *out, const nsapi_addr_t *in)
{
#if LWIP_IPV6
    if (in->version == NSAPI_IPv6) {
         IP_SET_TYPE(out, IPADDR_TYPE_V6);
         MEMCPY(ip_2_ip6(out), in->bytes, sizeof(ip6_addr_t));
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
         MEMCPY(ip_2_ip4(out), in->bytes, sizeof(ip4_addr_t));
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

static bool convert_lwip_addr_to_mbed(nsapi_addr_t *out, const ip_addr_t *in)
{
#if LWIP_IPV6
    if (IP_IS_V6(in)) {
        out->version = NSAPI_IPv6;
        MEMCPY(out->bytes, ip_2_ip6(in), sizeof(ip6_addr_t));
        return true;
    }
#endif
#if LWIP_IPV4
    if (IP_IS_V4(in)) {
        out->version = NSAPI_IPv4;
        MEMCPY(out->bytes, ip_2_ip4(in), sizeof(ip4_addr_t));
        return true;
    }
#endif
    return false;
}

static const ip_addr_t *mbed_lwip_get_ipv4_addr(const struct netif *netif)
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

static const ip_addr_t *mbed_lwip_get_ipv6_addr(const struct netif *netif)
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

const ip_addr_t *mbed_lwip_get_ip_addr(bool any_addr, const struct netif *netif)
{
    const ip_addr_t *pref_ip_addr = 0;
    const ip_addr_t *npref_ip_addr = 0;

#if IP_VERSION_PREF == PREF_IPV4
    pref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
#else
    pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
    npref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif

    if (pref_ip_addr) {
        return pref_ip_addr;
    } else if (npref_ip_addr && any_addr) {
        return npref_ip_addr;
    }

    return NULL;
}

void add_dns_addr(struct netif *lwip_netif)
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

static sys_sem_t lwip_tcpip_inited;
static void mbed_lwip_tcpip_init_irq(void *eh)
{
    sys_sem_signal(&lwip_tcpip_inited);
}

static sys_sem_t lwip_netif_linked;
static void mbed_lwip_netif_link_irq(struct netif *lwip_netif)
{
    if (netif_is_link_up(lwip_netif)) {
        sys_sem_signal(&lwip_netif_linked);
    }
}

static sys_sem_t lwip_netif_has_addr;
static void mbed_lwip_netif_status_irq(struct netif *lwip_netif)
{
    static bool any_addr = true;

    if (netif_is_up(lwip_netif)) {
        // Indicates that has address
        if (any_addr == true && mbed_lwip_get_ip_addr(true, lwip_netif)) {
            sys_sem_signal(&lwip_netif_has_addr);
            any_addr = false;
            return;
        }

        // Indicates that has preferred address
        if (mbed_lwip_get_ip_addr(false, lwip_netif)) {
            sys_sem_signal(&lwip_netif_has_addr);
        }
    } else {
        any_addr = true;
    }
}

static void mbed_lwip_set_mac_address(void)
{
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    snprintf(lwip_mac_address, NSAPI_MAC_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
            MBED_MAC_ADDR_0, MBED_MAC_ADDR_1, MBED_MAC_ADDR_2,
            MBED_MAC_ADDR_3, MBED_MAC_ADDR_4, MBED_MAC_ADDR_5);
#else
    char mac[6];
    mbed_mac_address(mac);
    snprintf(lwip_mac_address, NSAPI_MAC_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
}

/* LWIP interface implementation */
const char *mbed_lwip_get_mac_address(void)
{
    return lwip_mac_address[0] ? lwip_mac_address : 0;
}

char *mbed_lwip_get_ip_address(char *buf, nsapi_size_t buflen)
{
    const ip_addr_t *addr = mbed_lwip_get_ip_addr(true, &lwip_netif);
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

const char *mbed_lwip_get_netmask(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_netmask(&lwip_netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

char *mbed_lwip_get_gateway(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
    const ip4_addr_t *addr = netif_ip4_gw(&lwip_netif);
    if (!ip4_addr_isany(addr)) {
        return ip4addr_ntoa_r(addr, buf, buflen);
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}

nsapi_error_t mbed_lwip_init(emac_interface_t *emac)
{
    // Check if we've already brought up lwip
    if (!mbed_lwip_get_mac_address()) {
        // Set up network
        mbed_lwip_set_mac_address();

        sys_sem_new(&lwip_tcpip_inited, 0);
        sys_sem_new(&lwip_netif_linked, 0);
        sys_sem_new(&lwip_netif_has_addr, 0);

        tcpip_init(mbed_lwip_tcpip_init_irq, NULL);
        sys_arch_sem_wait(&lwip_tcpip_inited, 0);

        memset(&lwip_netif, 0, sizeof lwip_netif);
        if (!netif_add(&lwip_netif,
#if LWIP_IPV4
                0, 0, 0,
#endif
                emac, MBED_NETIF_INIT_FN, tcpip_input)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        netif_set_default(&lwip_netif);

        netif_set_link_callback(&lwip_netif, mbed_lwip_netif_link_irq);
        netif_set_status_callback(&lwip_netif, mbed_lwip_netif_status_irq);

#if !DEVICE_EMAC
        eth_arch_enable_interrupts();
#endif
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t mbed_lwip_bringup(bool dhcp, const char *ip, const char *netmask, const char *gw)
{
    // Check if we've already connected
    if (lwip_connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    if(mbed_lwip_init(NULL) != NSAPI_ERROR_OK) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    // Zero out socket set
    mbed_lwip_arena_init();

#if LWIP_IPV6
    netif_create_ip6_linklocal_address(&lwip_netif, 1/*from MAC*/);
#if LWIP_IPV6_MLD
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if (lwip_netif.mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    lwip_netif.mld_mac_filter(&lwip_netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6_MLD */

#if LWIP_IPV6_AUTOCONFIG
    /* IPv6 address autoconfiguration not enabled by default */
  lwip_netif.ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */

#endif

    u32_t ret;

    if (!netif_is_link_up(&lwip_netif)) {
        ret = sys_arch_sem_wait(&lwip_netif_linked, 15000);

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

        netif_set_addr(&lwip_netif, &ip_addr, &netmask_addr, &gw_addr);
    }
#endif

    netif_set_up(&lwip_netif);

#if LWIP_IPV4
    // Connect to the network
    lwip_dhcp = dhcp;

    if (lwip_dhcp) {
        err_t err = dhcp_start(&lwip_netif);
        if (err) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }
    }
#endif

    // If doesn't have address
    if (!mbed_lwip_get_ip_addr(true, &lwip_netif)) {
        ret = sys_arch_sem_wait(&lwip_netif_has_addr, 15000);
        if (ret == SYS_ARCH_TIMEOUT) {
            return NSAPI_ERROR_DHCP_FAILURE;
        }
    }

#if ADDR_TIMEOUT
    // If address is not for preferred stack waits a while to see
    // if preferred stack address is acquired
    if (!mbed_lwip_get_ip_addr(false, &lwip_netif)) {
        ret = sys_arch_sem_wait(&lwip_netif_has_addr, ADDR_TIMEOUT * 1000);
    }
#endif

    add_dns_addr(&lwip_netif);

    lwip_connected = true;
    return 0;
}

#if LWIP_IPV6
void mbed_lwip_clear_ipv6_addresses(struct netif *lwip_netif)
{
    for (u8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        netif_ip6_addr_set_state(lwip_netif, i, IP6_ADDR_INVALID);
    }
}
#endif

nsapi_error_t mbed_lwip_bringdown(void)
{
    // Check if we've connected
    if (!lwip_connected) {
        return NSAPI_ERROR_PARAMETER;
    }

#if LWIP_IPV4
    // Disconnect from the network
    if (lwip_dhcp) {
        dhcp_release(&lwip_netif);
        dhcp_stop(&lwip_netif);
        lwip_dhcp = false;
    }
#endif

    netif_set_down(&lwip_netif);

#if LWIP_IPV6
    mbed_lwip_clear_ipv6_addresses(&lwip_netif);
#endif

    sys_sem_free(&lwip_netif_has_addr);
    sys_sem_new(&lwip_netif_has_addr, 0);
    lwip_connected = false;
    return 0;
}

/* LWIP error remapping */
static nsapi_error_t mbed_lwip_err_remap(err_t err) {
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
        case ERR_INPROGRESS:
        case ERR_WOULDBLOCK:
            return NSAPI_ERROR_WOULD_BLOCK;
        case ERR_VAL:
        case ERR_USE:
        case ERR_ISCONN:
        case ERR_ARG:
            return NSAPI_ERROR_PARAMETER;
        default:
            return NSAPI_ERROR_DEVICE_ERROR;
    }
}

/* LWIP network stack implementation */
static nsapi_error_t mbed_lwip_gethostbyname(nsapi_stack_t *stack, const char *host, nsapi_addr_t *addr, nsapi_version_t version)
{
    ip_addr_t lwip_addr;

#if LWIP_IPV4 && LWIP_IPV6
    u8_t addr_type;
    if (version == NSAPI_UNSPEC) {
        const ip_addr_t *ip_addr;
        ip_addr = mbed_lwip_get_ip_addr(true, &lwip_netif);
        if (IP_IS_V6(ip_addr)) {
            addr_type = NETCONN_DNS_IPV6;
        } else {
            addr_type = NETCONN_DNS_IPV4;
        }
    } else if (version == NSAPI_IPv4) {
        addr_type = NETCONN_DNS_IPV4;
    } else if (version == NSAPI_IPv6) {
        addr_type = NETCONN_DNS_IPV6;
    }
    err_t err = netconn_gethostbyname_addrtype(host, &lwip_addr, addr_type);
#elif LWIP_IPV4
    if (version != NSAPI_IPv4 && version != NSAPI_UNSPEC) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    err_t err = netconn_gethostbyname(host, &lwip_addr);
#elif LWIP_IPV6
    if (version != NSAPI_IPv6 && version != NSAPI_UNSPEC) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    err_t err = netconn_gethostbyname(host, &lwip_addr);
#endif

    if (err != ERR_OK) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    convert_lwip_addr_to_mbed(addr, &lwip_addr);

    return 0;
}

static nsapi_error_t mbed_lwip_add_dns_server(nsapi_stack_t *stack, nsapi_addr_t addr)
{
    // Shift all dns servers down to give precedence to new server
    for (int i = DNS_MAX_SERVERS-1; i > 0; i--) {
        dns_setserver(i, dns_getserver(i-1));
    }

    ip_addr_t ip_addr;
    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    dns_setserver(0, &ip_addr);
    return 0;
}

static nsapi_error_t mbed_lwip_socket_open(nsapi_stack_t *stack, nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    // check if network is connected
    if (!lwip_connected) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // allocate a socket
    struct lwip_socket *s = mbed_lwip_arena_alloc();
    if (!s) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    u8_t lwip_proto = proto == NSAPI_TCP ? NETCONN_TCP : NETCONN_UDP;

#if LWIP_IPV6 && LWIP_IPV4
    const ip_addr_t *ip_addr;
    ip_addr = mbed_lwip_get_ip_addr(true, &lwip_netif);

    if (IP_IS_V6(ip_addr)) {
        // Enable IPv6 (or dual-stack). LWIP dual-stack support is
        // currently incomplete as of 2.0.0rc2 - eg we will only be able
        // to do a UDP sendto to an address matching the type selected
        // here. Matching "get_ip_addr" and DNS logic, use v4 if
        // available.
        lwip_proto |= NETCONN_TYPE_IPV6;
    }
#elif LWIP_IPV6
    lwip_proto |= NETCONN_TYPE_IPV6;
#endif

    s->conn = netconn_new_with_callback(lwip_proto, mbed_lwip_socket_callback);

    if (!s->conn) {
        mbed_lwip_arena_dealloc(s);
        return NSAPI_ERROR_NO_SOCKET;
    }

    netconn_set_recvtimeout(s->conn, 1);
    *(struct lwip_socket **)handle = s;
    return 0;
}

static nsapi_error_t mbed_lwip_socket_close(nsapi_stack_t *stack, nsapi_socket_t handle)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    netbuf_delete(s->buf);
    err_t err = netconn_delete(s->conn);
    mbed_lwip_arena_dealloc(s);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_bind(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if ((s->conn->type == NETCONN_TCP && s->conn->pcb.tcp->local_port != 0) ||
        (s->conn->type == NETCONN_UDP && s->conn->pcb.udp->local_port != 0)) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    err_t err = netconn_bind(s->conn, &ip_addr, port);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_listen(nsapi_stack_t *stack, nsapi_socket_t handle, int backlog)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    err_t err = netconn_listen_with_backlog(s->conn, backlog);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_connect(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    netconn_set_nonblocking(s->conn, false);
    err_t err = netconn_connect(s->conn, &ip_addr, port);
    netconn_set_nonblocking(s->conn, true);

    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_accept(nsapi_stack_t *stack, nsapi_socket_t server, nsapi_socket_t *handle, nsapi_addr_t *addr, uint16_t *port)
{
    struct lwip_socket *s = (struct lwip_socket *)server;
    struct lwip_socket *ns = mbed_lwip_arena_alloc();
    if (!ns) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    err_t err = netconn_accept(s->conn, &ns->conn);
    if (err != ERR_OK) {
        mbed_lwip_arena_dealloc(ns);
        return mbed_lwip_err_remap(err);
    }

    netconn_set_recvtimeout(ns->conn, 1);
    *(struct lwip_socket **)handle = ns;

    ip_addr_t peer_addr;
    (void) netconn_peer(ns->conn, &peer_addr, port);
    convert_lwip_addr_to_mbed(addr, &peer_addr);

    netconn_set_nonblocking(ns->conn, true);

    return 0;
}

static nsapi_size_or_error_t mbed_lwip_socket_send(nsapi_stack_t *stack, nsapi_socket_t handle, const void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    size_t bytes_written = 0;

    err_t err = netconn_write_partly(s->conn, data, size, NETCONN_COPY, &bytes_written);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    return (nsapi_size_or_error_t)bytes_written;
}

static nsapi_size_or_error_t mbed_lwip_socket_recv(nsapi_stack_t *stack, nsapi_socket_t handle, void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    if (!s->buf) {
        err_t err = netconn_recv(s->conn, &s->buf);
        s->offset = 0;

        if (err != ERR_OK) {
            return mbed_lwip_err_remap(err);
        }
    }

    u16_t recv = netbuf_copy_partial(s->buf, data, (u16_t)size, s->offset);
    s->offset += recv;

    if (s->offset >= netbuf_len(s->buf)) {
        netbuf_delete(s->buf);
        s->buf = 0;
    }

    return recv;
}

static nsapi_size_or_error_t mbed_lwip_socket_sendto(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port, const void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr;

    if (!convert_mbed_addr_to_lwip(&ip_addr, &addr)) {
        return NSAPI_ERROR_PARAMETER;
    }

    struct netbuf *buf = netbuf_new();
    err_t err = netbuf_ref(buf, data, (u16_t)size);
    if (err != ERR_OK) {
        netbuf_free(buf);
        return mbed_lwip_err_remap(err);
    }

    err = netconn_sendto(s->conn, buf, &ip_addr, port);
    netbuf_delete(buf);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    return size;
}

static nsapi_size_or_error_t mbed_lwip_socket_recvfrom(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t *addr, uint16_t *port, void *data, nsapi_size_t size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    struct netbuf *buf;

    err_t err = netconn_recv(s->conn, &buf);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    convert_lwip_addr_to_mbed(addr, netbuf_fromaddr(buf));
    *port = netbuf_fromport(buf);

    u16_t recv = netbuf_copy(buf, data, (u16_t)size);
    netbuf_delete(buf);

    return recv;
}

static nsapi_error_t mbed_lwip_setsockopt(nsapi_stack_t *stack, nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    switch (optname) {
        case NSAPI_KEEPALIVE:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->so_options |= SOF_KEEPALIVE;
            return 0;

        case NSAPI_KEEPIDLE:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->keep_idle = *(int*)optval;
            return 0;

        case NSAPI_KEEPINTVL:
            if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->conn->pcb.tcp->keep_intvl = *(int*)optval;
            return 0;

        case NSAPI_REUSEADDR:
            if (optlen != sizeof(int)) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            if (*(int *)optval) {
                s->conn->pcb.tcp->so_options |= SOF_REUSEADDR;
            } else {
                s->conn->pcb.tcp->so_options &= ~SOF_REUSEADDR;
            }
            return 0;

        default:
            return NSAPI_ERROR_UNSUPPORTED;
    }
}

static void mbed_lwip_socket_attach(nsapi_stack_t *stack, nsapi_socket_t handle, void (*callback)(void *), void *data)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    s->cb = callback;
    s->data = data;
}

/* LWIP network stack */
const nsapi_stack_api_t lwip_stack_api = {
    .gethostbyname      = mbed_lwip_gethostbyname,
    .add_dns_server     = mbed_lwip_add_dns_server,
    .socket_open        = mbed_lwip_socket_open,
    .socket_close       = mbed_lwip_socket_close,
    .socket_bind        = mbed_lwip_socket_bind,
    .socket_listen      = mbed_lwip_socket_listen,
    .socket_connect     = mbed_lwip_socket_connect,
    .socket_accept      = mbed_lwip_socket_accept,
    .socket_send        = mbed_lwip_socket_send,
    .socket_recv        = mbed_lwip_socket_recv,
    .socket_sendto      = mbed_lwip_socket_sendto,
    .socket_recvfrom    = mbed_lwip_socket_recvfrom,
    .setsockopt         = mbed_lwip_setsockopt,
    .socket_attach      = mbed_lwip_socket_attach,
};

nsapi_stack_t lwip_stack = {
    .stack_api = &lwip_stack_api,
};
