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

#ifndef LWIPOPTS_CONF_H
#define LWIPOPTS_CONF_H

#define LWIP_TRANSPORT_ETHERNET       1
#define NO_SYS                  0
#define LWIP_SOCKET             1
#define LWIP_NETCONN            1
#define SYS_LIGHTWEIGHT_PROT    1

#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#define ETH_PAD_SIZE            2

#define MEM_ALIGNMENT           4

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#define TCPIP_THREAD_STACKSIZE  1024
#define DEFAULT_THREAD_STACKSIZE 1024

#define MEM_SIZE                8 * 1024

#define PBUF_POOL_SIZE          8
#define PBUF_POOL_BUFSIZE       1536
#define MEMP_NUM_PBUF           8

#define LWIP_UDP                1
#define LWIP_TCP                1
#define TCP_MSS                 1460
#define TCP_WND                 (4 * TCP_MSS)
#define TCP_SND_BUF             (4 * TCP_MSS)
#define TCP_SND_QUEUELEN        8

#define IP_OPTIONS_ALLOWED      0
#define IP_FRAG                 0
#define IP_REASSEMBLY           0

#define CHECKSUM_GEN_IP         0
#define CHECKSUM_GEN_UDP        0
#define CHECKSUM_GEN_TCP        0
#define CHECKSUM_GEN_ICMP       0
#define CHECKSUM_CHECK_IP       0
#define CHECKSUM_CHECK_UDP      0
#define CHECKSUM_CHECK_TCP      0
#define CHECKSUM_CHECK_ICMP       0

#define MEMP_NUM_SYS_TIMEOUT    (LWIP_TCP + IP_REASSEMBLY + LWIP_ARP + (2*LWIP_DHCP) + LWIP_AUTOIP + LWIP_IGMP + LWIP_DNS + (PPP_SUPPORT*6*MEMP_NUM_PPP_PCB) + (LWIP_IPV6 ? (1 + LWIP_IPV6_REASS + LWIP_IPV6_MLD) : 0) + 5)


#endif
