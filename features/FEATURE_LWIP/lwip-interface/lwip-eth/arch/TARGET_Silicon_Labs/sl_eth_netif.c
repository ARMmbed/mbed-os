/***************************************************************************//**
 * @file sl_eth_netif.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#include "sl_eth_netif.h"
#include "sl_eth_hw.h"
#include "device.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "lwip/ethip6.h"
#include "lwip/igmp.h"
#include "lwip/mld6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

#include "eth_arch.h"
#include "sys_arch.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mbed_interface.h"

// -----------------------------------------------------------------------------
// Datatypes
// -----------------------------------------------------------------------------
typedef struct {
      struct netif *netif;      /** Context pointer to lwip driver instance */
      osThreadId_t thread;      /** Ethernet driver thread */
      sys_sem_t    tx_sem;      /** Semaphore protecting the TX state. 
                                  * Not a mutex since we're posting from IRQ */
      uint8_t      phy_addr;    /** (R)MII address where the PHY is residing */
      uint8_t      rx_idx;      /** Index in RX queue for next packet to read */
} sl_eth_inst_data_t;

typedef struct {
  uint32_t addr;
  uint32_t status;
} sl_eth_bd_t;

#define FLAG_TX  1
#define FLAG_RX  2

#define THREAD_PRIORITY (osPriorityNormal)

#ifdef LWIP_DEBUG
#define THREAD_STACKSIZE (DEFAULT_THREAD_STACKSIZE * 5)
#else
#define THREAD_STACKSIZE DEFAULT_THREAD_STACKSIZE
#endif

// -----------------------------------------------------------------------------
// Function signatures
// -----------------------------------------------------------------------------
/**
 * This function checks the PHY for the current link status
 */
static void sl_eth_link_state_poll(void *data);

/**
 * This function sets up all data structures
 */
static void sl_eth_data_init(void);

/**
 * This function resets and enables the PHY
 */
static void sl_eth_phy_init(void);

/**
 * Calculate broadcast hash of MAC address
 */
static uint8_t sl_eth_calc_hash(uint8_t* const mac);

/**
 * Add MAC address to listen for
 */
static void sl_eth_add_mcast(uint8_t* const addr);

/**
 * Remove previously added MAC address from address filter
 */
static void sl_eth_del_mcast(uint8_t* const addr);

#if LWIP_IGMP
/**
 * IPv4 address filtering setup.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] group IPv4 group to modify
 * \param[in] action
 * \return ERR_OK or error code
 */
static err_t sl_eth_igmp_filter(struct netif *netif,
                                const ip4_addr_t *group,
                                enum netif_mac_filter_action action);
#endif

#if LWIP_IPV6_MLD
/**
 * IPv6 address filtering setup.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] group IPv6 group to modify
 * \param[in] action
 * \return ERR_OK or error code
 */
static err_t sl_eth_mld_filter(struct netif *netif,
                               const ip6_addr_t *group,
                               enum netif_mac_filter_action action);
#endif

/**
 * Frees the transmit pbuf's after packet transmission IRQ
 */
static void sl_eth_tx_done_cb(struct netif *netif);

/**
 * Passes received packet(s) into lwip after packet reception IRQ
 */
static void sl_eth_rx_done_cb(struct netif *netif);

/**
 * Write to PHY register
 */
static void sl_eth_write_phy(uint8_t phy_addr, uint8_t reg_addr, uint16_t data);

/**
 * Read from PHY register
 */
static void sl_eth_read_phy(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data);

/**
 * This function is the ipv4 ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV4
static err_t sl_eth_output_ipv4(struct netif *netif,
                                struct pbuf *q,
                                const ip4_addr_t *ipaddr);
#endif

/**
 * This function is the ipv6 ethernet packet send function. It calls
 * ethip6_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV6
static err_t sl_eth_output_ipv6(struct netif *netif,
                                struct pbuf *q,
                                const ip6_addr_t *ipaddr);
#endif

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until an in-progress TX
 *          finishes.
 *
 *  \param[in] netif the lwip network interface structure for this netif
 *  \param[in] p the MAC packet to send
 *  \return ERR_OK if the packet could be sent or an err_t value
 */
static err_t sl_eth_output(struct netif *netif, struct pbuf *p);

/**
 * Thread to de-escalate ETH IRQ's
 */
static void sl_eth_thread(void* unused);

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------
static sl_eth_inst_data_t sl_eth_inst;

static sl_eth_bd_t      sl_eth_rx_bds[SL_ETH_NUM_RX_BD];
static struct pbuf *    sl_eth_rx_pbufs[SL_ETH_NUM_RX_BD];
static sl_eth_bd_t      sl_eth_tx_bds[SL_ETH_NUM_TX_BD];
static struct pbuf *    sl_eth_tx_pbufs[SL_ETH_NUM_TX_BD];

#if defined(LWIP_IGMP) || defined(LWIP_IPV6_MLD)
static uint8_t            sl_eth_hash_refcnt[64] = {0};
#endif

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
static void sl_eth_data_init(void)
{
    size_t i;

    /* Allocate a full-frame pbuf for each RX BD and set up said BD */
    for(i = 0; i < SL_ETH_NUM_RX_BD; i++) {
        sl_eth_rx_pbufs[i] = pbuf_alloc(PBUF_RAW, SL_ETH_RX_BUF_SIZE, PBUF_RAM);
        sl_eth_rx_bds[i].addr = ((uint32_t)sl_eth_rx_pbufs[i]->payload) & ~0x3;
        if(i == SL_ETH_NUM_RX_BD-1) {
            sl_eth_rx_bds[i].addr |= 0x2;
    }
    sl_eth_rx_bds[i].status = 0;
    }

    /* Set BD RX list address in DMA */
    ETH->RXQPTR = (uint32_t)sl_eth_rx_bds;
    ETH->DMACFG = (ETH->DMACFG & ~_ETH_DMACFG_RXBUFSIZE_MASK) 
                  | ((SL_ETH_RX_BUF_SIZE/64) << _ETH_DMACFG_RXBUFSIZE_SHIFT);

    /* Set up TX BDs */
    for(i = 0; i < SL_ETH_NUM_TX_BD; i++) {
        sl_eth_tx_pbufs[i] = (struct pbuf*)NULL;
        sl_eth_tx_bds[i].addr = 0;
        sl_eth_tx_bds[i].status = 0;
        if(i == SL_ETH_NUM_TX_BD-1) {
            sl_eth_tx_bds[i].status |= (0x1 << 30);
        }
    }

    /* Start RX reception at index 0 */
    sl_eth_inst.rx_idx = 0;
}

static void sl_eth_phy_init(void)
{
    uint8_t phy_addr = 0;
    uint16_t regid1, regid2;
    /* PHY has been enabled by hw_init. Figure out address first */
    for(; phy_addr < 32; phy_addr++) {
        sl_eth_read_phy(phy_addr, PHY_PHYSID1, &regid1);
        sl_eth_read_phy(phy_addr, PHY_PHYSID2, &regid2);

        if (((regid1 == 0x0000u) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x0000u) && (regid2 == 0x3FFFu)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0x3FFFu)) ||
            ((regid1 == 0xFFFFu) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x0000u) && (regid2 == 0xFFFFu)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0xFFFFu)) ||
            ((regid1 == 0xFFFFu) && (regid2 == 0xFFFFu))) {
            continue;
        } else {
            break;
        }
    }

    if(phy_addr >= 32) {
        /* no PHY found */
        return;
    } else {
        sl_eth_inst.phy_addr = phy_addr;
    }

    /* Reset PHY */
    sl_eth_write_phy(phy_addr, PHY_BMCR, BMCR_RESET);
    sl_eth_read_phy(phy_addr, PHY_BMCR, &regid1);

    /* wait for PHY to come out of reset */
    while(regid1 & BMCR_RESET) {
        osDelay(2);
        sl_eth_read_phy(phy_addr, PHY_BMCR, &regid1);
    }

    /* Enable PHY */
    if(regid1 & BMCR_PDOWN) {
        sl_eth_write_phy(phy_addr, PHY_BMCR, regid1 & (~BMCR_PDOWN));
    }

    /* Set up auto-negotiation */
    sl_eth_read_phy(phy_addr, PHY_BMCR, &regid1);
    regid1 |= BMCR_ANENABLE | BMCR_ANRESTART;
    sl_eth_write_phy(phy_addr, PHY_BMCR, regid1);
}

static void sl_eth_link_state_poll(void *data)
{
    struct netif *netif = data;
    static bool connected = false;
    uint16_t phy_val, link_val;

    /* read BMSR twice, since it needs to latch */
    sl_eth_read_phy(sl_eth_inst.phy_addr, PHY_BMSR, &phy_val);
    sl_eth_read_phy(sl_eth_inst.phy_addr, PHY_BMSR, &phy_val);

    if((phy_val & BMSR_LSTATUS) == 0) {
        /* link is down */
        if(connected) {
            netif_set_link_down(netif);
            /* TODO: Reset all buffers here */
            /* For now, this is not a problem. In-transit buffers will
             * still be sent the next time the link comes up, so the
             * only impact is that we'd be sending stale packets. */
            connected = false;
        }
        return;
    }

    /* link is up, get speed and duplex status */
    sl_eth_read_phy(sl_eth_inst.phy_addr, PHY_ANAR, &phy_val);
    sl_eth_read_phy(sl_eth_inst.phy_addr, PHY_ANLPAR, &link_val);

    link_val &= (ANLPAR_100BASE4 |
                 ANLPAR_100FULL  |
                 ANLPAR_100HALF  |
                 ANLPAR_10FULL   |
                 ANLPAR_10HALF);

    phy_val &= link_val;

    if (phy_val >= ANLPAR_100FULL) {
        /* 100 mbit full duplex */
        ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x3;
    } else if (phy_val >= ANLPAR_100HALF) {
        /* 100 mbit half duplex */
        ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x1;
    } else if (phy_val >= ANLPAR_10FULL) {
        /* 10 mbit full duplex */
        ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x2;
    } else {
        /* 10 mbit half duplex */
        ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x0;
    }

    if(!connected) {
        netif_set_link_up(netif);
        connected = true;
    }
}

static void sl_eth_write_phy(uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
    unsigned int timeout;

    ETH->PHYMNGMNT = ETH_PHYMNGMNT_WRITE0_DEFAULT
                   | ETH_PHYMNGMNT_WRITE1
                   | (0x01 << _ETH_PHYMNGMNT_OPERATION_SHIFT)
                   | ((phy_addr << _ETH_PHYMNGMNT_PHYADDR_SHIFT)
                      & _ETH_PHYMNGMNT_PHYADDR_MASK)
                   | ((reg_addr << _ETH_PHYMNGMNT_REGADDR_SHIFT)
                      & _ETH_PHYMNGMNT_REGADDR_MASK)
                   | (0x2 << _ETH_PHYMNGMNT_WRITE10_SHIFT)
                   | (data & _ETH_PHYMNGMNT_PHYRWDATA_MASK);

    for(timeout = 0; timeout < 10000u; timeout++) {
        if(ETH->NETWORKSTATUS & ETH_NETWORKSTATUS_MANDONE) {
            break;
        }
    }
}

static void sl_eth_read_phy(uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
{
    unsigned int timeout;

    ETH->PHYMNGMNT = ETH_PHYMNGMNT_WRITE0_DEFAULT
                   | ETH_PHYMNGMNT_WRITE1
                   | (0x02 << _ETH_PHYMNGMNT_OPERATION_SHIFT)
                   | ((phy_addr << _ETH_PHYMNGMNT_PHYADDR_SHIFT)
                      & _ETH_PHYMNGMNT_PHYADDR_MASK)
                   | ((reg_addr << _ETH_PHYMNGMNT_REGADDR_SHIFT)
                      & _ETH_PHYMNGMNT_REGADDR_MASK)
                   | (0x2 << _ETH_PHYMNGMNT_WRITE10_SHIFT);

    for(timeout = 0; timeout < 10000u; timeout++) {
        if(ETH->NETWORKSTATUS & ETH_NETWORKSTATUS_MANDONE) {
        break;
        }
    }

    *data = ETH->PHYMNGMNT & _ETH_PHYMNGMNT_PHYRWDATA_MASK;
}

static err_t sl_eth_output(struct netif *netif, struct pbuf *p)
{
    size_t num_bufs = 1, i;
    struct pbuf * pnext = p;
    /* Figure out how many buffers the pbuf consists of */
    while(pnext->next != (struct pbuf*)NULL) {
        num_bufs++;
        pnext = pnext->next;
    }

    if(num_bufs >= SL_ETH_NUM_TX_BD) {
        /* lwip is passing us more chained pbufs than we can handle */
        return ERR_BUF;
    }

    /* Wait for previous packet to finish transmitting */
    osStatus_t stat = osSemaphoreAcquire(((sl_eth_inst_data_t*)netif->state)->tx_sem.id, 100);
    if (stat != osOK) {
        return ERR_IF;
    }

    /* Set up TX descriptors with pbufs, keep track of pbuf reference */
    for(i = 0; i < num_bufs; i++) {
        sl_eth_tx_pbufs[i] = p;

        uint32_t statusword = p->len & 0x3FFF;
        if(i == (SL_ETH_NUM_TX_BD-1)) {
            /* Mark as last BD in list */
            statusword |= (0x1 << 30);
        }
        if(i == num_bufs - 1) {
            /* Mark as last BD for this frame */
            statusword |= (0x1 << 15);
        }

        sl_eth_tx_bds[i].addr = (uint32_t)p->payload;
        sl_eth_tx_bds[i].status = statusword;

        pbuf_ref(p);
        p = p->next;
    }

    /* (Re-)Kick off ETH TX */
    ETH->TXQPTR = (uint32_t)sl_eth_tx_bds;
    ETH->NETWORKCTRL |= ETH_NETWORKCTRL_TXSTRT;
    return ERR_OK;
}

static void sl_eth_tx_done_cb(struct netif *netif)
{
    /* Free the pbuf's */
    for(size_t i = 0; i < SL_ETH_NUM_TX_BD; i++) {
        if(sl_eth_tx_pbufs[i] != NULL) {
            pbuf_free(sl_eth_tx_pbufs[i]);
            sl_eth_tx_pbufs[i] = NULL;
        }
    }

    /* Signal TX thread(s) we're ready to start TX'ing */
    osSemaphoreRelease(((sl_eth_inst_data_t*)netif->state)->tx_sem.id);
}

static void sl_eth_rx_done_cb(struct netif *netif)
{
    /* Find packets in the RX BD chain which have been fully received. Feed the
     * corresponding pbuf into lwip, and attach a new pbuf to the BD. Lwip is
     * done with the pbuf when input returns. */
    sl_eth_inst_data_t* state = netif->state;
    while(sl_eth_rx_bds[state->rx_idx].addr & 0x1) {
        /* DMA has relinquished control over this packet */
        struct pbuf* p = sl_eth_rx_pbufs[state->rx_idx];
        p->len = sl_eth_rx_bds[state->rx_idx].status & 0x1FFF;
        p->tot_len = p->len;

        /* Attempt to queue new buffer */
        struct pbuf* temp_rxbuf = pbuf_alloc(PBUF_RAW, SL_ETH_RX_BUF_SIZE, PBUF_RAM);
        if (NULL == temp_rxbuf) {
            /* out of memory, drop packet */
            LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
                        ("sl_eth_rx_done_cb: Packet index %d dropped for OOM\n",
                        state->rx_idx));
            sl_eth_rx_bds[state->rx_idx].addr &= ~0x1;
        } else {
            sl_eth_rx_pbufs[state->rx_idx] = temp_rxbuf;

            sl_eth_rx_bds[state->rx_idx].status = 0;
            if(sl_eth_rx_bds[state->rx_idx].addr & 0x2) {
                sl_eth_rx_bds[state->rx_idx].addr = (uint32_t)temp_rxbuf->payload | 0x2;
            } else {
                sl_eth_rx_bds[state->rx_idx].addr = (uint32_t)temp_rxbuf->payload;
            }

            /* pass all packets to ethernet_input, which decides what packets it supports */
            if (netif->input(p, netif) != ERR_OK) {
                LWIP_DEBUGF(NETIF_DEBUG, ("sl_eth_rx_done_cb: input error\n"));
                pbuf_free(p);
            }
        }
        state->rx_idx = (state->rx_idx + 1) % SL_ETH_NUM_RX_BD;
    }
}

#if LWIP_IPV4
static err_t sl_eth_output_ipv4(struct netif *netif,
                                struct pbuf *q,
                                const ip4_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return etharp_output(netif, q, ipaddr);
    } else {
        return ERR_CONN;
    }
}
#endif

#if LWIP_IPV6
static err_t sl_eth_output_ipv6(struct netif *netif,
                                struct pbuf *q,
                                const ip6_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return ethip6_output(netif, q, ipaddr);
    } else {
        return ERR_CONN;
    }
}
#endif

static uint8_t sl_eth_calc_hash(uint8_t* const mac)
{
    return (uint8_t)((    (mac[0]       & 0x3F)                         & 0x3F)
                     ^ ((((mac[0] >> 6) & 0x3) | ((mac[1] & 0xF) << 2)) & 0x3F)
                     ^ ((((mac[1] >> 4) & 0xF) | ((mac[2] & 0x3) << 4)) & 0x3F)
                     ^ ((((mac[2] >> 2) & 0x3F))                        & 0x3F)
                     ^   ((mac[3]       & 0x3F)                         & 0x3F)
                     ^ ((((mac[3] >> 6) & 0x3) | ((mac[4] & 0xF) << 2)) & 0x3F)
                     ^ ((((mac[4] >> 4) & 0xF) | ((mac[5] & 0x3) << 4)) & 0x3F)
                     ^ ((((mac[5] >> 2) & 0x3F))                        & 0x3F));
}

#if LWIP_IGMP
static err_t sl_eth_igmp_filter(struct netif *netif,
                                const ip4_addr_t *group,
                                enum netif_mac_filter_action action)
{
    uint32_t group23 = ntohl(group->addr) & 0x007FFFFF;
    uint8_t addr[6];
    addr[0] = LL_IP4_MULTICAST_ADDR_0;
    addr[1] = LL_IP4_MULTICAST_ADDR_1;
    addr[2] = LL_IP4_MULTICAST_ADDR_2;
    addr[3] = group23 >> 16;
    addr[4] = group23 >> 8;
    addr[5] = group23;
    switch (action) {
        case NETIF_ADD_MAC_FILTER:
            sl_eth_add_mcast(addr);
            return ERR_OK;
        case NETIF_DEL_MAC_FILTER:
            sl_eth_del_mcast(addr);
            return ERR_OK;
        default:
            return ERR_ARG;
    }
}
#endif

#if LWIP_IPV6_MLD
static err_t sl_eth_mld_filter(struct netif *netif,
                               const ip6_addr_t *group,
                               enum netif_mac_filter_action action)
{
    uint32_t group32 = ntohl(group->addr[3]);
    uint8_t addr[6];
    addr[0] = LL_IP6_MULTICAST_ADDR_0;
    addr[1] = LL_IP6_MULTICAST_ADDR_1;
    addr[2] = group32 >> 24;
    addr[3] = group32 >> 16;
    addr[4] = group32 >> 8;
    addr[5] = group32;
    switch (action) {
        case NETIF_ADD_MAC_FILTER:
            sl_eth_add_mcast(addr);
            return ERR_OK;
        case NETIF_DEL_MAC_FILTER:
            sl_eth_del_mcast(addr);
            return ERR_OK;
        default:
            return ERR_ARG;
    }
}
#endif

static void sl_eth_add_mcast(uint8_t* const addr)
{
    uint8_t bitnr;
    /* Calculate bit number for hash of this address */
    bitnr = sl_eth_calc_hash(addr);
    /* Increment refcnt */
    if (sl_eth_hash_refcnt[bitnr] == 0) {
        if(bitnr > 31) {
            ETH->HASHTOP |= (0x1 << (bitnr - 32));
        } else {
            ETH->HASHBOTTOM |= (0x1 << bitnr);
        }
    }
    sl_eth_hash_refcnt[bitnr]++;
}

static void sl_eth_del_mcast(uint8_t* const addr)
{
    uint8_t bitnr;
    /* Calculate bit number for hash of this address */
    bitnr = sl_eth_calc_hash(addr);
    /* Decrement refcnt, remove bit if 0 */
    if(sl_eth_hash_refcnt[bitnr] == 1) {
        sl_eth_hash_refcnt[bitnr] = 0;
        if(bitnr > 31) {
            ETH->HASHTOP &= ~(0x1 << (bitnr - 32));
        } else {
            ETH->HASHBOTTOM &= ~(0x1 << bitnr);
        }
    } else {
        sl_eth_hash_refcnt[bitnr]--;
    }
}

// Override mbed_mac_address of mbed_interface.c to provide ethernet devices with a unique MAC address
void mbed_mac_address(char *mac)
{
    if (DEVINFO->EUI48L != 0xFFFFFFFF) {
        mac[0] = DEVINFO->EUI48H >> 8;
        mac[1] = DEVINFO->EUI48H >> 0;
        mac[2] = DEVINFO->EUI48L >> 24;
        mac[3] = DEVINFO->EUI48L >> 16;
        mac[4] = DEVINFO->EUI48L >> 8;
        mac[5] = DEVINFO->EUI48L >> 0;
    } else {
        mac[0] = DEVINFO->UNIQUEH >> 24;
        mac[1] = DEVINFO->UNIQUEH >> 16;
        mac[2] = DEVINFO->UNIQUEH >> 8;
        mac[3] = DEVINFO->UNIQUEL >> 16;
        mac[4] = DEVINFO->UNIQUEL >> 8;
        mac[5] = DEVINFO->UNIQUEL >> 0;
    }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param[in] netif the lwip network interface structure for this netif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    err_t err = ERR_OK;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    sl_eth_inst.netif = netif;

    /* set MAC hardware address */
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    netif->hwaddr[0] = MBED_MAC_ADDR_0;
    netif->hwaddr[1] = MBED_MAC_ADDR_1;
    netif->hwaddr[2] = MBED_MAC_ADDR_2;
    netif->hwaddr[3] = MBED_MAC_ADDR_3;
    netif->hwaddr[4] = MBED_MAC_ADDR_4;
    netif->hwaddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)netif->hwaddr);
#endif

    /* Ethernet address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

    /* Initialize the hardware */
    netif->state = &sl_eth_inst;
    sl_eth_hw_init();
    sl_eth_data_init();

    ETH->SPECADDR1BOTTOM = ((uint32_t)netif->hwaddr[0] << (0)) |
                         ((uint32_t)netif->hwaddr[1] << (8)) |
                         ((uint32_t)netif->hwaddr[2] << (16))|
                         ((uint32_t)netif->hwaddr[3] << (24));

    ETH->SPECADDR1TOP = ((uint32_t)netif->hwaddr[4] << (0)) |
                      ((uint32_t)netif->hwaddr[5] << (8));

    ETH->IFCR |= _ETH_IFCR_MASK;
    ETH->RXSTATUS = 0xFFFFFFFF;
    ETH->TXSTATUS = 0xFFFFFFFF;
    ETH->IENS = ETH_IENS_RXCMPLT |
              ETH_IENS_TXCMPLT |
              ETH_IENS_TXUNDERRUN |
              ETH_IENS_RTRYLMTORLATECOL |
              ETH_IENS_TXUSEDBITREAD |
              ETH_IENS_AMBAERR |
              ETH_IENS_MNGMNTDONE;

    ETH->NETWORKCFG |= ETH_NETWORKCFG_FCSREMOVE |
                     ETH_NETWORKCFG_UNICASTHASHEN |
                     ETH_NETWORKCFG_MULTICASTHASHEN |
                     ETH_NETWORKCFG_RXCHKSUMOFFLOADEN;

    ETH->NETWORKCFG |= ETH_NETWORKCFG_FULLDUPLEX |
                     ETH_NETWORKCFG_SPEED;

    ETH->DMACFG |= _ETH_DMACFG_AMBABRSTLEN_MASK |
                 ETH_DMACFG_FRCDISCARDONERR |
                 ETH_DMACFG_TXPBUFTCPEN;

    ETH->DMACFG &= ~ETH_DMACFG_HDRDATASPLITEN;

    ETH->NETWORKCTRL |= ETH_NETWORKCTRL_ENBTX |
                      ETH_NETWORKCTRL_ENBRX |
                      ETH_NETWORKCTRL_MANPORTEN;

    sl_eth_phy_init();

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "efm32gg11-mbed";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

#if LWIP_IPV4
    netif->output = sl_eth_output_ipv4;
#if LWIP_IGMP
    netif->igmp_mac_filter = sl_eth_igmp_filter;
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#endif
#if LWIP_IPV6
    netif->output_ip6 = sl_eth_output_ipv6;
#if LWIP_IPV6_MLD
    netif->mld_mac_filter = sl_eth_mld6_filter;
    netif->flags |= NETIF_FLAG_MLD6;
#else
#error "IPv6 multicasts won't be received if LWIP_IPV6_MLD is disabled, breaking the system"
#endif
#endif
    netif->linkoutput = sl_eth_output;

    /* CMSIS-RTOS, start tasks */
    memset(&sl_eth_inst.tx_sem.data, 0, sizeof(sl_eth_inst.tx_sem.data));
    sl_eth_inst.tx_sem.attr.cb_mem = &sl_eth_inst.tx_sem.data;
    sl_eth_inst.tx_sem.attr.cb_size = sizeof(sl_eth_inst.tx_sem.data);
    sl_eth_inst.tx_sem.id = osSemaphoreNew(1, 1, &sl_eth_inst.tx_sem.attr);

    LWIP_ASSERT("tx_sem creation error", (sl_eth_inst.tx_sem.id != NULL));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    tcpip_callback_with_block(sl_eth_link_state_poll, netif, 1);
    osDelay(10);

    /* Worker thread */
    sl_eth_inst.thread = sys_thread_new("sl_eth", sl_eth_thread, netif->state, THREAD_STACKSIZE, THREAD_PRIORITY)->id;

    eth_arch_enable_interrupts();

    return err;
}

static void sl_eth_thread(void* unused)
{
    sl_eth_inst_data_t *sl_eth_inst_ptr = unused;

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX|FLAG_TX, osFlagsWaitAny, SL_ETH_LINK_POLL_PERIOD_MS);
        if (flags == osFlagsErrorTimeout) {
            // Rather than calling strictly every period, we call when idle
            // for that period - hopefully good enough. We run this task
            // from lwIP's thread rather than our RX/TX thread, as PHY reads can
            // be slow, and we don't want them to interfere with data pumping.
            // This is analogous to the way the PHY polling works in the Nanostack
            // version of the driver
            tcpip_callback_with_block(sl_eth_link_state_poll, sl_eth_inst_ptr->netif, 0);
            continue;
        }

        LWIP_ASSERT("osThreadFlagsWait error", !(flags & osFlagsError));

        if (flags & FLAG_RX) {
            sl_eth_rx_done_cb(sl_eth_inst_ptr->netif);
        }

        if (flags & FLAG_TX) {
            sl_eth_tx_done_cb(sl_eth_inst_ptr->netif);
        }
    }
}

void eth_arch_enable_interrupts(void)
{
    NVIC_EnableIRQ(ETH_IRQn);
}

void eth_arch_disable_interrupts(void)
{
    NVIC_DisableIRQ(ETH_IRQn);
}

/* IRQ handler for ethernet interrupts */
void ETH_IRQHandler(void)
{
    uint32_t int_clr = 0;
    uint32_t int_status = ETH->IFCR;
    uint32_t txdone_mask = ETH_IFCR_TXCMPLT |
                           ETH_IFCR_TXUNDERRUN |
                           ETH_IFCR_RTRYLMTORLATECOL |
                           ETH_IFCR_TXUSEDBITREAD |
                           ETH_IFCR_AMBAERR;

    if(int_status & ETH_IFCR_RXCMPLT) {
        osThreadFlagsSet(sl_eth_inst.thread, FLAG_RX);
        int_clr |= ETH_IFCR_RXCMPLT;
    }
    if(int_status & txdone_mask) {
        osThreadFlagsSet(sl_eth_inst.thread, FLAG_TX);
        int_clr |= txdone_mask;
    }

    int_clr |= ETH_IFCR_MNGMNTDONE;
    ETH->IFCR = int_clr;
}
