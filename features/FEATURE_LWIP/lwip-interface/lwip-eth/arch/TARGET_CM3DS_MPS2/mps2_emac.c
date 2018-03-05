/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ethernet_api.h"
#include "eth_arch.h"

#include "lwip/def.h"
#include "lwip/ethip6.h"
#include "lwip/igmp.h"
#include "lwip/mem.h"
#include "lwip/mld6.h"
#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip/snmp.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"

#include "mbed_interface.h"
#include "mbed_wait_api.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "smsc9220_eth.h"
#include "sys_arch.h"

/**
 * @file mps2_emac.c
 *
 * @brief Connects the lwIP stack to the SMSC9220 Ethernet controller driver,
 *        by implementing the low-level Ethernet interface layer of lwIP,
 *        according to the skeleton file:
 *        \mbed-os\features\FEATURE_LWIP\lwip-interface\lwip\src\netif\lwip_ethernetif.c
 *
 */

#define HOSTNAME_STRING "lwip_mps2"

static sys_sem_t rx_ready_sem;

struct ethernetif {
    const struct eth_addr *ethaddr;
    int is_enabled;
    sys_mutex_t tx_mutex;
};

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p, *q;
    uint16_t len;
    int unread_bytes = 0;

    len = ETH_MAX_FLEN;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {
#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* It iterates over the pbuf chain until it has read the entire
         * packet into the pbuf. */
        for (q = p; q != NULL; q = q->next) {
            /* Read enough bytes to fill this pbuf in the chain. The
             * available data in the pbuf is given by the q->len
             * variable.
             * This does not necessarily have to be a memcpy, you can also preallocate
             * pbufs for a DMA-enabled MAC and after receiving truncate it to the
             * actually received size. In this case, ensure the tot_len member of the
             * pbuf is the sum of the chained pbuf len members.
             */
            unread_bytes = ethernet_read(q->payload, q->len);
            if (unread_bytes == 0) {
                break;
            }
        }
        /* package has been read */

        MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
        if (((u8_t*)p->payload)[0] & 1) {
            /* broadcast or multicast packet */
            MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
        } else {
            /* unicast packet */
            MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
        }
#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
         /* drop packet */
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
    }

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void ethernetif_input(struct netif *netif)
{
    struct pbuf *p;

    /* move received packet into a new pbuf */
    p = low_level_input(netif);
    /* if no packet could be read, silently ignore this */
    if (p != NULL) {
        /* pass all packets to ethernet_input, which decides what packets it supports */
        if (netif->input(p, netif) != ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
        }
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: buffer null pointer error\n"));
    }
}

/**
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 *
 * @param pvParameters pointer to the interface data
 */
static void packet_rx(void* pvParameters)
{
    struct netif *netif = pvParameters;

    while (1) {
        sys_arch_sem_wait(&rx_ready_sem, 0);
        ethernetif_input(netif);
        smsc9220_enable_interrupt(enum_smsc9220_interrupt_rxstatus_fifo_level);
    }
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    err_t error = ERR_OK;
    unsigned int is_new_packet = 1;
    const unsigned int packet_length = p->tot_len;

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* fragmented packet sending should not be used concurrently */
    sys_mutex_lock(&(((struct ethernetif*)(netif->state))->tx_mutex));

    for (q = p; (q != NULL) && (error == ERR_OK); q = q->next) {
        /* Send the data from the pbuf to the interface, one pbuf at a
         * time.
         * The SMSC9220 driver should be used directly for sending by
         * buffer chain, because the SMSC9220 Ethernet API cannot support
         * ethernet_send and ethernet_write functions without additional
         * SW FIFO, what would increase the memory footprint.
         */
        error = smsc9220_send_by_chunks(packet_length, is_new_packet,
                                        q->payload, q->len);
        if (error != ERR_OK) {
            LWIP_ASSERT("smsc9220_send_by_chunks error", 0);
            return error;
        }
        is_new_packet = 0;
    }
    sys_mutex_unlock(&(((struct ethernetif*)(netif->state))->tx_mutex));

    /* packet should be sent now */

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
        /* broadcast or multicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    } else {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return error;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static err_t low_level_init(struct netif *netif)
{
    err_t error = ERR_OK;
    int low_level_error = 0;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    netif->hwaddr[0] = MBED_MAC_ADDR_0;
    netif->hwaddr[1] = MBED_MAC_ADDR_1;
    netif->hwaddr[2] = MBED_MAC_ADDR_2;
    netif->hwaddr[3] = MBED_MAC_ADDR_3;
    netif->hwaddr[4] = MBED_MAC_ADDR_4;
    netif->hwaddr[5] = MBED_MAC_ADDR_5;
#else
    ethernet_address((char *)netif->hwaddr);
#endif

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

#if LWIP_IPV6 && LWIP_IPV6_MLD
    /*
     * For hardware/netifs that implement MAC filtering.
     * All-nodes link-local is handled by default, so it must let the hardware know
     * to allow multicast packets in.
     * Should set mld_mac_filter previously. */
    if (netif->mld_mac_filter != NULL) {
        ip6_addr_t ip6_allnodes_ll;
        ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
        netif->mld_mac_filter(netif, &ip6_allnodes_ll, MLD6_ADD_MAC_FILTER);
    }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */

    low_level_error =  ethernet_init();
    if (low_level_error != 0) {
        return ERR_IF;
    }

    error = sys_mutex_new(&(((struct ethernetif*)(netif->state))->tx_mutex));

    /* Maximum Transfer Unit */
    netif->mtu = ETH_MAX_PAYLOAD_LEN;

    return error;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    err_t error = ERR_OK;
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));
    ethernetif = mem_malloc(sizeof(struct ethernetif));

    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

    ethernetif->is_enabled = 0;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = HOSTNAME_STRING;
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd,
                    LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = ethernetif;
    /* Two-character name, "en" for Ethernet.
     * This can be used to get a netif by name, via netif_find */
    netif->name[0] = 'e';
    netif->name[1] = 'n';
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /* initialize the hardware */
    error = low_level_init(netif);

    if (error == ERR_OK) {
        sys_thread_new("receive_thread", packet_rx, netif,
                       DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
        sys_sem_new(&rx_ready_sem, 0);
        ethernetif->is_enabled = 1;
    }

    return error;
}

void eth_arch_enable_interrupts(void)
{
    smsc9220_enable_interrupt(enum_smsc9220_interrupt_rxstatus_fifo_level);
    NVIC_EnableIRQ(ETHERNET_IRQn);
}

void eth_arch_disable_interrupts(void)
{
    NVIC_DisableIRQ(ETHERNET_IRQn);
}

void ETHERNET_IRQHandler(void)
{
    if (smsc9220_get_interrupt(enum_smsc9220_interrupt_rxstatus_fifo_level)) {
        sys_sem_signal(&rx_ready_sem);
        smsc9220_clear_interrupt(enum_smsc9220_interrupt_rxstatus_fifo_level);
        smsc9220_disable_interrupt(enum_smsc9220_interrupt_rxstatus_fifo_level);
    }
}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

