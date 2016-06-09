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

#include "mbed.h"
#include "LWIPInterface.h"

#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "netif/etharp.h"
#include "eth_arch.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/timers.h"
#include "lwip/dns.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"


/* TCP/IP and Network Interface Initialisation */
static struct netif netif;

static char ip_addr[NSAPI_IP_SIZE] = "\0";
static char mac_addr[NSAPI_MAC_SIZE] = "\0";

static Semaphore tcpip_inited(0);
static Semaphore netif_linked(0);
static Semaphore netif_up(0);

static void tcpip_init_irq(void *)
{
    tcpip_inited.release();
}

static void netif_link_irq(struct netif *netif)
{
    if (netif_is_link_up(netif)) {
        netif_linked.release();
    }
}

static void netif_status_irq(struct netif *netif)
{
    if (netif_is_up(netif)) {
        strcpy(ip_addr, inet_ntoa(netif->ip_addr));
        netif_up.release();
    }
}

static void init_netif(ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gw)
{
    tcpip_init(tcpip_init_irq, NULL);
    tcpip_inited.wait();

    memset((void*) &netif, 0, sizeof(netif));
    netif_add(&netif, ipaddr, netmask, gw, NULL, eth_arch_enetif_init, tcpip_input);
    netif_set_default(&netif);

    netif_set_link_callback  (&netif, netif_link_irq);
    netif_set_status_callback(&netif, netif_status_irq);
}

static void set_mac_address(void)
{
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    snprintf(mac_addr, 19, "%02x:%02x:%02x:%02x:%02x:%02x", MBED_MAC_ADDR_0, MBED_MAC_ADDR_1, MBED_MAC_ADDR_2,
             MBED_MAC_ADDR_3, MBED_MAC_ADDR_4, MBED_MAC_ADDR_5);
#else
    char mac[6];
    mbed_mac_address(mac);
    snprintf(mac_addr, 19, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
}


/* Interface implementation */
int LWIPInterface::connect()
{
    // Check if we've already connected
    if (get_ip_address()) {
        return 0;
    }

    // Set up network
    set_mac_address();
    init_netif(0, 0, 0);

    // Connect to network
    eth_arch_enable_interrupts();

    dhcp_start(&netif);

    // Wait for an IP Address
    // -1: error, 0: timeout
    if (netif_up.wait(2500) < 0) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    return 0;
}

int LWIPInterface::disconnect()
{
    dhcp_release(&netif);
    dhcp_stop(&netif);

    eth_arch_disable_interrupts();
    ip_addr[0] = '\0';
    mac_addr[0] = '\0';

    return 0;
}

const char *LWIPInterface::get_ip_address()
{
    return ip_addr[0] ? ip_addr : 0;
}

const char *LWIPInterface::get_mac_address()
{
    return mac_addr[0] ? mac_addr : 0;
}

struct lwip_socket {
    nsapi_protocol_t proto;
    union {
        struct udp_pcb *udp;
        struct tcp_pcb *tcp;
    };

    struct tcp_pcb *npcb;
    struct pbuf *rx_chain;
    Semaphore *sem;

    void (*callback)(void *);
    void *data;
};

static void udp_recv_irq(
        void *arg, struct udp_pcb *upcb, struct pbuf *p,
        struct ip_addr *addr, uint16_t port);

int LWIPInterface::socket_open(void **handle, nsapi_protocol_t proto)
{
    struct lwip_socket *s = new struct lwip_socket;
    if (!s) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    memset(s, 0, sizeof *s);

    switch (proto) {
        case NSAPI_UDP:
            s->proto = proto;
            s->udp = udp_new();
            if (!s->udp) {
                return NSAPI_ERROR_NO_SOCKET;
            }

            udp_recv(s->udp, udp_recv_irq, s);
            *handle = s;
            return 0;

        case NSAPI_TCP:
            s->proto = proto;
            s->tcp = tcp_new();
            if (!s->tcp) {
                return NSAPI_ERROR_NO_SOCKET;
            }

            tcp_arg(s->tcp, s);
            //tcp_err(s->tcp, tcp_error_irq);
            *handle = s;
            return 0;
    }

    return NSAPI_ERROR_DEVICE_ERROR;
}

int LWIPInterface::socket_close(void *handle)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    int err = 0;

    switch (s->proto) {
        case NSAPI_UDP:
            udp_disconnect(s->udp);
            udp_remove(s->udp);
            break;

        case NSAPI_TCP:
            if (tcp_close(s->tcp)) {
                err = NSAPI_ERROR_DEVICE_ERROR;
            }
            break;
    }

    if (s->rx_chain) {
        pbuf_free(s->rx_chain);
        s->rx_chain = 0;
    }

    delete s;

    return err;
}


int LWIPInterface::socket_bind(void *handle, const SocketAddress &address)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    ip_addr_t ip_addr = ip_addr_any; // TODO use address

    switch (s->proto) {
        case NSAPI_UDP:
            if (udp_bind(s->udp, &ip_addr, address.get_port())) {
                return NSAPI_ERROR_DEVICE_ERROR;
            }
            return 0;

        case NSAPI_TCP:
            if (tcp_bind(s->tcp, &ip_addr, address.get_port())) {
                return NSAPI_ERROR_DEVICE_ERROR;
            }
            return 0;
    }

    return NSAPI_ERROR_DEVICE_ERROR;
}

static err_t tcp_accept_irq(void *arg, struct tcp_pcb *tpcb, err_t err);

int LWIPInterface::socket_listen(void *handle, int backlog)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    if (s->tcp->state != LISTEN) {
        struct tcp_pcb *server = tcp_listen(s->tcp);
        if (!server) {
            return NSAPI_ERROR_NO_SOCKET;
        }

        s->tcp = server;
        s->npcb = 0;
    }

    tcp_arg(s->tcp, s);
    tcp_accept(s->tcp, tcp_accept_irq);
    return 0;
}

static err_t tcp_sent_irq(void *arg, struct tcp_pcb *tpcb, uint16_t len);
static err_t tcp_recv_irq(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

static err_t tcp_connect_irq(void *handle, struct tcp_pcb *tpcb, err_t err)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    tcp_sent(tpcb, tcp_sent_irq);
    tcp_recv(tpcb, tcp_recv_irq);

    s->sem->release();

    return ERR_OK;
}

int LWIPInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    ip_addr_t ip_addr;
    inet_aton(addr.get_ip_address(), &ip_addr);

    Semaphore connected(0);
    s->sem = &connected;

    tcp_connect(s->tcp, &ip_addr, addr.get_port(), tcp_connect_irq);

    // Wait for connection
    if (connected.wait(1500) < 0) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    return 0;
}

static err_t tcp_refuse_irq(void *handle, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    return ERR_WOULDBLOCK;
}

static err_t tcp_accept_irq(void *handle, struct tcp_pcb *npcb, err_t err)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    if (s->npcb) {
        tcp_abort(npcb);
        return ERR_ABRT;
    }

    tcp_recv(npcb, tcp_refuse_irq);
    s->npcb = npcb;

    if (s->callback) {
        s->callback(s->data);
    }

    return ERR_OK;
}

int LWIPInterface::socket_accept(void **handle, void *server)
{
    struct lwip_socket *s = (struct lwip_socket *)server;
    if (!s->npcb) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    struct lwip_socket *ns = new struct lwip_socket;
    if (!ns) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    memset(ns, 0, sizeof *ns);

    ns->tcp = s->npcb;
    s->npcb = 0;

    tcp_accepted(ns->tcp);
    tcp_arg(ns->tcp, ns);
    //tcp_err(ns->tcp, tcp_error_irq);
    tcp_sent(ns->tcp, tcp_sent_irq);
    tcp_recv(ns->tcp, tcp_recv_irq);
    *handle = ns;
    return 0;
}

static struct pbuf *pbuf_consume(struct pbuf *p, size_t consume, bool free_partial)
{
    do {
        if (consume <= p->len) {
            // advance the payload pointer by the number of bytes copied
            p->payload = (char *)p->payload + consume;
            // reduce the length by the number of bytes copied
            p->len -= consume;
            // break out of the loop
            consume = 0;
        }
        if (p->len == 0 || consume > p->len || (consume == 0 && free_partial)) {
            struct pbuf *q;
            q = p->next;
            // decrement the number of bytes copied by the length of the buffer
            if(consume > p->len)
                consume -= p->len;
            // Free the current pbuf
            // NOTE: This operation is interrupt safe, but not thread safe.
            if (q != NULL) {
                pbuf_ref(q);
            }
            pbuf_free(p);
            p = q;
        }
    } while (consume);
    return p;
}

static err_t tcp_sent_irq(void *handle, struct tcp_pcb *tpcb, uint16_t len)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    if (s->callback) {
        s->callback(s->data);
    }

    return ERR_OK;
}

int LWIPInterface::socket_send(void *handle, const void *buf, unsigned size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    if (tcp_write(s->tcp, buf, size, TCP_WRITE_FLAG_COPY)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    tcp_output(s->tcp);

    return size;
}

static err_t tcp_recv_irq(void *handle, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    // Check for disconnect
    if (!p) {
        // Zero pcb during disconnect, since disconnect will cause a free
        switch (tpcb->state) {
            case FIN_WAIT_1:
            case FIN_WAIT_2:
            case TIME_WAIT:
                s->tcp = 0;
                break;
            default:
                break;
        }
        return ERR_OK;
    }

    __disable_irq();
    if (!s->rx_chain) {
        s->rx_chain = p;
    } else {
        pbuf_cat(s->rx_chain, p);
    }
    __enable_irq();

    if (s->callback) {
        s->callback(s->data);
    }

    return ERR_OK;
}

int LWIPInterface::socket_recv(void *handle, void *buf, unsigned size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    // Disconnected
    if (!s->tcp && !s->rx_chain) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // Nothing ready
    if (!s->rx_chain) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    // Copy out pbuf
    struct pbuf *p = s->rx_chain;
    int copied = pbuf_copy_partial(p, buf, size, 0);
    s->rx_chain = pbuf_consume(p, copied, false);

    // Update TCP window
    tcp_recved(s->tcp, copied);
    return copied;
}

int LWIPInterface::socket_sendto(void *handle, const SocketAddress &addr, const void *buf, unsigned size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    struct pbuf *pb = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    if (pbuf_take(pb, buf, size)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    ip_addr_t id_addr;
    inet_aton(addr.get_ip_address(), &id_addr);

    err_t err = udp_sendto(s->udp, pb, &id_addr, addr.get_port());
    pbuf_free(pb);
    if (err) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (s->callback) {
        s->callback(s->data);
    }

    return size;
}

static void udp_recv_irq(
    void *handle, struct udp_pcb *upcb, struct pbuf *p,
    struct ip_addr *addr, uint16_t port)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    __disable_irq();
    if (!s->rx_chain) {
        s->rx_chain = p;
    } else {
        // Attach p to buffer chain without changing the tot_len
        // NOTE: This is not how pbufs are intended to work, but it is necessary to deal with
        // a) fragmentation and b) packet queueing
        struct pbuf *q = s->rx_chain;
        while (q->next) { q = q->next; }
        q->next = p;
    }
    __enable_irq();

    if (s->callback) {
        s->callback(s->data);
    }
}

int LWIPInterface::socket_recvfrom(void *handle, SocketAddress *addr, void *buf, unsigned size)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;

    // Disconnected
    if (!s->udp && !s->rx_chain) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // Nothing ready
    if (!s->rx_chain) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    struct pbuf *p = s->rx_chain;

    if (addr) {
        struct udp_hdr *udphdr;
        struct ip_hdr *iphdr;

        // roll back the pbuf by udp_hdr to find the source port
        pbuf_header(p, UDP_HLEN);
        udphdr = (struct udp_hdr *)p->payload;

        // roll back the pbuf by ip_hdr to find the source IP
        pbuf_header(p, IP_HLEN);
        iphdr = (struct ip_hdr *)p->payload;

        // put the pbuf back where it was
        pbuf_header(p, -UDP_HLEN - IP_HLEN);

        addr->set_ip_address(inet_ntoa(iphdr->src));
        addr->set_port(ntohs(udphdr->src));
    }

    // Copy out pbuf
    size = size < p->tot_len ? size : p->tot_len;
    int copied = pbuf_copy_partial(p, buf, size, 0);
    s->rx_chain = pbuf_consume(p, p->tot_len, true);

    return copied;
}

int LWIPInterface::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen) {
    struct lwip_socket *s = (struct lwip_socket *)handle;

    switch (optname) {
        case NSAPI_KEEPALIVE:
            if (optlen != sizeof(int) || s->proto != NSAPI_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->tcp->so_options |= SOF_KEEPALIVE;
            return 0;

        case NSAPI_KEEPIDLE:
            if (optlen != sizeof(int) || s->proto != NSAPI_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->tcp->keep_idle = *(int*)optval;
            return 0;

        case NSAPI_KEEPINTVL:
            if (optlen != sizeof(int) || s->proto != NSAPI_TCP) {
                return NSAPI_ERROR_UNSUPPORTED;
            }

            s->tcp->keep_intvl = *(int*)optval;
            return 0;
            
        default:
            return NSAPI_ERROR_UNSUPPORTED;
    }
}

void LWIPInterface::socket_attach(void *handle, void (*callback)(void *), void *data)
{
    struct lwip_socket *s = (struct lwip_socket *)handle;
    s->callback = callback;
    s->data = data;
}
