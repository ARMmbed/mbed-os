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

#include "LWIPInterface.h"

#include "mbed.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "netif/etharp.h"
#include "eth_arch.h"


#define LWIP_TIMEOUT 15000


/* TCP/IP and Network Interface Initialisation */
static struct netif netif;

static char ip_addr[NS_IP_SIZE] = "\0";
static char mac_addr[NS_MAC_SIZE] = "\0";

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


// LWIPInterface implementation
int32_t LWIPInterface::connect()
{
    // Set up network
    set_mac_address();
    init_netif(0, 0, 0);

    // Connect to network
    eth_arch_enable_interrupts();

    dhcp_start(&netif);

    // Wait for an IP Address
    // -1: error, 0: timeout
    if (netif_up.wait(LWIP_TIMEOUT) < 0) {
        return NS_ERROR_TIMEOUT;
    }

    return 0;
}

int32_t LWIPInterface::disconnect()
{
    dhcp_release(&netif);
    dhcp_stop(&netif);

    eth_arch_disable_interrupts();

    return 0;
}

const char *LWIPInterface::getIPAddress()
{
    return ip_addr;
}

const char *LWIPInterface::getMACAddress()
{
    return mac_addr;
}

SocketInterface *LWIPInterface::createSocket(ns_protocol_t proto)
{
    int type = (proto == NS_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    int fd = lwip_socket(AF_INET, type, 0);
    if (fd < 0) {
        return 0;
    }

    return new LWIPSocket(fd);
}

void LWIPInterface::destroySocket(SocketInterface *siface)
{
    LWIPSocket *socket = (LWIPSocket *)siface;
    lwip_close(socket->fd);

    delete socket;
}


// TCP SocketInterface implementation
int32_t LWIPInterface::LWIPSocket::open(const char *ip, uint16_t port)
{
    struct sockaddr_in host;
    memset(&host, 0, sizeof host);
    inet_aton(ip, &host.sin_addr);
    host.sin_family = AF_INET;
    host.sin_port = htons(port);

    if (lwip_connect(fd, (const struct sockaddr *)&host, sizeof host) < 0) {
        return NS_ERROR_NO_CONNECTION;
    }

    return 0;
}

int32_t LWIPInterface::LWIPSocket::close()
{
    return 0;
}

int32_t LWIPInterface::LWIPSocket::send(const void *voiddata, uint32_t size)
{
    uint8_t *data = (uint8_t *)voiddata;
    uint32_t writtenLen = 0;

    while (writtenLen < size) {
        int ret = lwip_send(fd, data + writtenLen, size - writtenLen, 0);

        if (ret > 0) {
            writtenLen += ret;
        } else if (ret == 0) {
            return NS_ERROR_NO_CONNECTION;
        } else {
            return NS_ERROR_DEVICE_ERROR;
        }
    }

    return writtenLen;
}

int32_t LWIPInterface::LWIPSocket::recv(void *data, uint32_t size)
{
    int ret = lwip_recv(fd, data, size, MSG_DONTWAIT);

    if (ret > 0) {
        return ret;
    } else if (ret == 0) {
        return NS_ERROR_NO_CONNECTION;
    } else if (ret == -1) {
        return NS_ERROR_WOULD_BLOCK;
    } else {
        return NS_ERROR_DEVICE_ERROR;
    }
}


