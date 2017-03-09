/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#include "lwipopts_conf.h"

// Workaround for Linux timeval
#if defined (TOOLCHAIN_GCC)
#define LWIP_TIMEVAL_PRIVATE 0
#include <sys/time.h>
#endif

// Operating System 
#define NO_SYS                      0

#if MBED_CONF_LWIP_IPV4_ENABLED
#define LWIP_IPV4                   1
#else
#define LWIP_IPV4                   0
#endif
#if MBED_CONF_LWIP_IPV6_ENABLED
#define LWIP_IPV6                   1
#else
#define LWIP_IPV6                   0
#endif
#if !MBED_CONF_LWIP_IPV4_ENABLED && !MBED_CONF_LWIP_IPV6_ENABLED
#error "Either IPv4 or IPv6 must be enabled."
#endif

// On dual stack configuration how long wait for preferred stack
// before selecting either IPv6 or IPv4
#if LWIP_IPV4 && LWIP_IPV6
#define ADDR_TIMEOUT                MBED_CONF_LWIP_ADDR_TIMEOUT
#else
#define ADDR_TIMEOUT                0
#endif

#define PREF_IPV4                   1
#define PREF_IPV6                   2

#if MBED_CONF_LWIP_IP_VER_PREF == 4
#define IP_VERSION_PREF             PREF_IPV4
#endif
#if MBED_CONF_LWIP_IP_VER_PREF == 6
#define IP_VERSION_PREF             PREF_IPV6
#endif
#ifndef IP_VERSION_PREF
#error "Either IPv4 or IPv6 must be preferred."
#endif

//#define LWIP_DEBUG

#if NO_SYS == 0
#include "cmsis_os.h"

#define SYS_LIGHTWEIGHT_PROT        1

#define LWIP_RAW                    0

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#ifdef LWIP_DEBUG
#define TCPIP_THREAD_STACKSIZE      1200*2
#else
#define TCPIP_THREAD_STACKSIZE      1200
#endif

#define TCPIP_THREAD_PRIO           (osPriorityNormal)

#ifdef LWIP_DEBUG
#define DEFAULT_THREAD_STACKSIZE    512*2
#else
#define DEFAULT_THREAD_STACKSIZE    512
#endif

#define MEMP_NUM_SYS_TIMEOUT        16

#define sys_msleep(ms) sys_msleep(ms)

#endif

// 32-bit alignment
#define MEM_ALIGNMENT               4

#define LWIP_RAM_HEAP_POINTER       lwip_ram_heap

// Number of pool pbufs.
// Each requires 684 bytes of RAM.
#ifndef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE              5
#endif

// One tcp_pcb_listen is needed for each TCPServer.
// Each requires 72 bytes of RAM.
#ifdef MBED_CONF_LWIP_TCP_SERVER_MAX
#define MEMP_NUM_TCP_PCB_LISTEN     MBED_CONF_LWIP_TCP_SERVER_MAX
#else
#define MEMP_NUM_TCP_PCB_LISTEN     4
#endif

// One is tcp_pcb needed for each TCPSocket.
// Each requires 196 bytes of RAM.
#ifdef MBED_CONF_LWIP_TCP_SOCKET_MAX
#define MEMP_NUM_TCP_PCB            MBED_CONF_LWIP_TCP_SOCKET_MAX
#else
#define MEMP_NUM_TCP_PCB            4
#endif

// One udp_pcb is needed for each UDPSocket.
// Each requires 84 bytes of RAM (total rounded to multiple of 512).
#ifdef MBED_CONF_LWIP_UDP_SOCKET_MAX
#define MEMP_NUM_UDP_PCB            MBED_CONF_LWIP_UDP_SOCKET_MAX
#else
#define MEMP_NUM_UDP_PCB            4
#endif

// Number of non-pool pbufs.
// Each requires 92 bytes of RAM.
#ifndef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF               8
#endif

// Each netbuf requires 64 bytes of RAM.
#ifndef MEMP_NUM_NETBUF
#define MEMP_NUM_NETBUF             8
#endif

// One netconn is needed for each UDPSocket, TCPSocket or TCPServer.
// Each requires 236 bytes of RAM (total rounded to multiple of 512).
#ifdef MBED_CONF_LWIP_SOCKET_MAX
#define MEMP_NUM_NETCONN            MBED_CONF_LWIP_SOCKET_MAX
#else
#define MEMP_NUM_NETCONN            4
#endif

#define TCP_QUEUE_OOSEQ             0
#define TCP_OVERSIZE                0

#define LWIP_DHCP                   LWIP_IPV4
#define LWIP_DNS                    1
#define LWIP_SOCKET                 0

#define SO_REUSE                    1

// Support Multicast
#include "stdlib.h"
#define LWIP_IGMP                   LWIP_IPV4
#define LWIP_RAND()                 rand()

#define LWIP_COMPAT_SOCKETS         0
#define LWIP_POSIX_SOCKETS_IO_NAMES 0
#define LWIP_SO_RCVTIMEO            1
#define LWIP_TCP_KEEPALIVE          1

// Fragmentation on, as per IPv4 default
#define LWIP_IPV6_FRAG              LWIP_IPV6

// Queuing "disabled", as per IPv4 default (so actually queues 1)
#define LWIP_ND6_QUEUEING           0

// Debug Options
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define IGMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define TIMERS_DEBUG                LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define SLIP_DEBUG                  LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF
#define AUTOIP_DEBUG                LWIP_DBG_OFF
#define DNS_DEBUG                   LWIP_DBG_OFF
#define IP6_DEBUG                   LWIP_DBG_OFF

#define PPP_DEBUG                   LWIP_DBG_OFF
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define UDP_LPC_EMAC                LWIP_DBG_OFF

#ifdef LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK         1
#define MEMP_SANITY_CHECK           1
#else
#define LWIP_NOASSERT               1
#define LWIP_STATS                  0
#endif

#define LWIP_DBG_TYPES_ON           LWIP_DBG_ON
#define LWIP_DBG_MIN_LEVEL          LWIP_DBG_LEVEL_ALL

#define LWIP_PLATFORM_BYTESWAP      1

#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 1

#if LWIP_TRANSPORT_ETHERNET

// Broadcast
#define IP_SOF_BROADCAST            0
#define IP_SOF_BROADCAST_RECV       0

#define LWIP_BROADCAST_PING         1

#define LWIP_CHECKSUM_ON_COPY       1

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#elif LWIP_TRANSPORT_PPP

#define TCP_SND_BUF                     (3 * 536)
#define TCP_WND                         (2 * 536)

#define LWIP_ARP 0

#define PPP_SUPPORT 1
#define CHAP_SUPPORT                    1
#define PAP_SUPPORT                     1
#define PPP_THREAD_STACKSIZE            4*192
#define PPP_THREAD_PRIO 0

#define MAXNAMELEN                      64     /* max length of hostname or name for auth */
#define MAXSECRETLEN                    64

#else
#error A transport mechanism (Ethernet or PPP) must be defined
#endif

#endif /* LWIPOPTS_H_ */
