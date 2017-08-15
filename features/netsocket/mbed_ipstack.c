/* LWIP implementation of NSAPI NetworkStack
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
#include "lwip_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

static void mbed_lwip_socket_callback(struct netconn *nc, enum netconn_evt eh, u16_t len)
{
    // Filter send minus events
    if (eh == NETCONN_EVT_SENDMINUS && nc->state == NETCONN_WRITE) {
        return;
    }

    sys_prot_t prot = sys_arch_protect();

    for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
        if (mbed_lwip_arena[i].in_use
            && mbed_lwip_arena[i].conn == nc
            && mbed_lwip_arena[i].cb) {
            mbed_lwip_arena[i].cb(mbed_lwip_arena[i].data);
        }
    }

    sys_arch_unprotect(prot);
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

/* LWIP network stack implementation */
static nsapi_error_t mbed_lwip_gethostbyname(nsapi_stack_t *stack, const char *host, nsapi_addr_t *addr, nsapi_version_t version)
{
    ip_addr_t lwip_addr;

#if LWIP_IPV4 && LWIP_IPV6
    u8_t addr_type;
    if (version == NSAPI_UNSPEC) {
        const ip_addr_t *ip_addr;
        ip_addr = mbed_lwip_get_ip_addr(true, &stack->emac->netif);
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
    if (!stack->emac->connected) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // allocate a socket
    struct mbed_lwip_socket *s = mbed_lwip_arena_alloc();
    if (!s) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    u8_t lwip_proto = proto == NSAPI_TCP ? NETCONN_TCP : NETCONN_UDP;

#if LWIP_IPV6 && LWIP_IPV4
    const ip_addr_t *ip_addr;
    ip_addr = mbed_lwip_get_ip_addr(true, &stack->emac->netif);

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
    *(struct mbed_lwip_socket **)handle = s;
    return 0;
}

static nsapi_error_t mbed_lwip_socket_close(nsapi_stack_t *stack, nsapi_socket_t handle)
{
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;

    netbuf_delete(s->buf);
    err_t err = netconn_delete(s->conn);
    mbed_lwip_arena_dealloc(s);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_bind(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;
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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;

    err_t err = netconn_listen_with_backlog(s->conn, backlog);
    return mbed_lwip_err_remap(err);
}

static nsapi_error_t mbed_lwip_socket_connect(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;
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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)server;
    struct mbed_lwip_socket *ns = mbed_lwip_arena_alloc();
    if (!ns) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    err_t err = netconn_accept(s->conn, &ns->conn);
    if (err != ERR_OK) {
        mbed_lwip_arena_dealloc(ns);
        return mbed_lwip_err_remap(err);
    }

    netconn_set_recvtimeout(ns->conn, 1);
    *(struct mbed_lwip_socket **)handle = ns;

    ip_addr_t peer_addr;
    (void) netconn_peer(ns->conn, &peer_addr, port);
    convert_lwip_addr_to_mbed(addr, &peer_addr);

    netconn_set_nonblocking(ns->conn, true);

    return 0;
}

static nsapi_size_or_error_t mbed_lwip_socket_send(nsapi_stack_t *stack, nsapi_socket_t handle, const void *data, nsapi_size_t size)
{
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;
    size_t bytes_written = 0;

    err_t err = netconn_write_partly(s->conn, data, size, NETCONN_COPY, &bytes_written);
    if (err != ERR_OK) {
        return mbed_lwip_err_remap(err);
    }

    return (nsapi_size_or_error_t)bytes_written;
}

static nsapi_size_or_error_t mbed_lwip_socket_recv(nsapi_stack_t *stack, nsapi_socket_t handle, void *data, nsapi_size_t size)
{
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;

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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;
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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;
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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;

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
    struct mbed_lwip_socket *s = (struct mbed_lwip_socket *)handle;

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

#ifdef __cplusplus
}
#endif
