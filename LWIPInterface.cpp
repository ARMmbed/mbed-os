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

/* TCP/IP and Network Interface Initialisation */
static struct netif netif;

static char ip_addr[NSAPI_IP_SIZE] = "\0";
static char mac_addr[NSAPI_MAC_SIZE] = "\0";

static Semaphore tcpip_inited(0);
static Semaphore netif_linked(0);
static Semaphore netif_up(0);

static void tcpip_init_done(void *)
{
    tcpip_inited.release();
}

static void netif_link_callback(struct netif *netif)
{
    if (netif_is_link_up(netif)) {
        netif_linked.release();
    }
}

static void netif_status_callback(struct netif *netif)
{
    if (netif_is_up(netif)) {
        strcpy(ip_addr, inet_ntoa(netif->ip_addr));
        netif_up.release();
    }
}

static void init_netif(ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gw)
{
    tcpip_init(tcpip_init_done, NULL);
    tcpip_inited.wait();

    memset((void*) &netif, 0, sizeof(netif));
    netif_add(&netif, ipaddr, netmask, gw, NULL, eth_arch_enetif_init, tcpip_input);
    netif_set_default(&netif);

    netif_set_link_callback  (&netif, netif_link_callback);
    netif_set_status_callback(&netif, netif_status_callback);
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


int LWIPInterface::connect()
{
    // Set up network
    set_mac_address();
    init_netif(0, 0, 0);

    // Connect to network
    eth_arch_enable_interrupts();

    dhcp_start(&netif);

    // Wait for an IP Address
    // -1: error, 0: timeout
    if (netif_up.wait(1500) < 0) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    return 0;
}

int LWIPInterface::disconnect()
{
    dhcp_release(&netif);
    dhcp_stop(&netif);

    eth_arch_disable_interrupts();

    return 0;
}

const char *LWIPInterface::get_ip_address()
{
    return ip_addr;
}

const char *LWIPInterface::get_mac_address()
{
    return mac_addr;
}

void *LWIPInterface::socket_create(nsapi_protocol_t proto)
{
    int type = (proto == NSAPI_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    int fd = lwip_socket(AF_INET, type, 0);
    if (fd < 0) {
        return 0;
    }

    return (void *)(fd+1);
}

void LWIPInterface::socket_destroy(void *handle)
{
    int fd = (int)handle-1;
    lwip_close(fd);
    
}

int LWIPInterface::socket_set_option(void *handle, int optname, const void *optval, unsigned optlen)
{
    int fd = (int)handle-1;
    return lwip_setsockopt(fd, SOL_SOCKET, optname, optval, (socklen_t)optlen);
}

int LWIPInterface::socket_get_option(void *handle, int optname, void *optval, unsigned *optlen)
{
    int fd = (int)handle-1;
    return lwip_getsockopt(fd, SOL_SOCKET, optname, optval, (socklen_t*)optlen);
}

int LWIPInterface::socket_bind(void *handle, int port)
{
    int fd = (int)handle-1;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;
    
    if (lwip_bind(fd, (const struct sockaddr *)&sa, sizeof sa) < 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return 0;
}

int LWIPInterface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int LWIPInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    int fd = (int)handle-1;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    inet_aton(addr.get_ip_address(), &sa.sin_addr);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(addr.get_port());

    if (lwip_connect(fd, (const struct sockaddr *)&sa, sizeof sa) < 0) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    return 0;
}
    
bool LWIPInterface::socket_is_connected(void *handle)
{
    return true;
}

int LWIPInterface::socket_accept(void *handle, void **connection)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int LWIPInterface::socket_send(void *handle, const void *p, unsigned size)
{
    int fd = (int)handle-1;
    uint8_t *data = (uint8_t *)p;
    unsigned written = 0;

    while (written < size) {
        int ret = lwip_send(fd, data + written, size - written, 0);

        if (ret > 0) {
            written += ret;
        } else if (ret == 0) {
            return NSAPI_ERROR_NO_CONNECTION;
        } else {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    return written;
}

int LWIPInterface::socket_recv(void *handle, void *data, unsigned size)
{
    int fd = (int)handle-1;
    int ret = lwip_recv(fd, data, size, MSG_DONTWAIT);

    if (ret > 0) {
        return ret;
    } else if (ret == 0) {
        return NSAPI_ERROR_NO_CONNECTION;
    } else if (ret == -1) {
        return NSAPI_ERROR_WOULD_BLOCK;
    } else {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

int LWIPInterface::socket_sendto(void *handle, const SocketAddress &addr, const void *p, unsigned size)
{
    int fd = (int)handle-1;
    uint8_t *data = (uint8_t *)p;
    unsigned written = 0;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    inet_aton(addr.get_ip_address(), &sa.sin_addr);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(addr.get_port());

    while (written < size) {
        int ret = lwip_sendto(fd, data + written, size - written, 0,
            (const struct sockaddr *)&sa, sizeof sa);

        if (ret > 0) {
            written += ret;
        } else if (ret == 0) {
            return NSAPI_ERROR_NO_CONNECTION;
        } else {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    return written;
}

int LWIPInterface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    int fd = (int)handle-1;
    struct sockaddr_in sa;
    socklen_t sa_len = sizeof sa;

    int ret = lwip_recvfrom(fd, data, size, MSG_DONTWAIT, 
        (struct sockaddr *)&sa, &sa_len);

    if (ret > 0 && addr) {
        addr->set_ip_address(inet_ntoa(sa.sin_addr));
        addr->set_port(ntohs(sa.sin_port));
    }

    if (ret > 0) {    
        return ret;
    } else if (ret == 0) {
        return NSAPI_ERROR_NO_CONNECTION;
    } else if (ret == -1) {
        return NSAPI_ERROR_WOULD_BLOCK;
    } else {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
}

int LWIPInterface::socket_close(void *handle, bool shutdown)
{
    int fd = (int)handle-1;
    if (shutdown) {
        lwip_shutdown(fd, SHUT_RDWR);
    }

    lwip_close(fd);
    return 0;
}

void LWIPInterface::socket_attach_accept(void *handle, void (*callback)(void *), void *id)
{
}

void LWIPInterface::socket_attach_send(void *handle, void (*callback)(void *), void *id)
{
}

void LWIPInterface::socket_attach_recv(void *handle, void (*callback)(void *), void *id)
{
}

