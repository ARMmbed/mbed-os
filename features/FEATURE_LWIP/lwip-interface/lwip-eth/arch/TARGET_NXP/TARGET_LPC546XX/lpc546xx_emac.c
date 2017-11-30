/* mbed Microcontroller Library
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

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

#include "eth_arch.h"
#include "sys_arch.h"

#include "fsl_phy.h"
#include "lpc546xx_emac_config.h"
#include <string.h>

#include "mbed_interface.h"

#if LWIP_ARP || LWIP_ETHERNET

enet_handle_t g_handle;
// RX packet buffer pointers
struct pbuf *rx_buff[ENET_RX_RING_LEN];
// TX packet buffer pointers
uint8_t *tx_buff[ENET_TX_RING_LEN];
// RX packet payload pointers
uint32_t rx_ptr[ENET_RX_RING_LEN];

/** \brief  Debug output formatter lock define
 *
 * When using FreeRTOS and with LWIP_DEBUG enabled, enabling this
 * define will allow RX debug messages to not interleave with the
 * TX messages (so they are actually readable). Not enabling this
 * define when the system is under load will cause the output to
 * be unreadable. There is a small tradeoff in performance for this
 * so use it only for debug. */
//#define LOCK_RX_THREAD

/* LPC546XX EMAC driver data structure */
struct lpc_enetdata {
  struct netif *netif;  /**< Reference back to LWIP parent netif */
  osThreadId_t thread; /**< Processing thread */
  sys_mutex_t TXLockMutex; /**< TX critical section mutex */
  sys_sem_t xTXDCountSem; /**< TX free buffer counting semaphore */
};

static struct lpc_enetdata lpc_enetdata;

/* \brief Flags for worker thread */
#define FLAG_TX  1
#define FLAG_RX  2

/** \brief  Driver thread priority */
#define THREAD_PRIORITY (osPriorityNormal)

#ifdef LWIP_DEBUG
#define THREAD_STACKSIZE (DEFAULT_THREAD_STACKSIZE * 5)
#else
#define THREAD_STACKSIZE DEFAULT_THREAD_STACKSIZE
#endif

static void lpc546xx_phy_task(void *data);
static void lpc546xx_packet_rx(struct lpc_enetdata *lpc_enet);
static void lpc546xx_packet_tx(struct lpc_enetdata *lpc_enet);

#define PHY_TASK_PERIOD_MS      200

/********************************************************************************
 * Buffer management
 ********************************************************************************/
/*
 * This function will queue a new receive buffer
 */
static void update_read_buffer(enet_rx_bd_struct_t *rxDesc, uint8_t *buf)
{
    enet_rx_bd_ring_t *rxBdRing = (enet_rx_bd_ring_t *)&g_handle.rxBdRing[0];
    uint32_t control = ENET_RXDESCRIP_RD_OWN_MASK | ENET_RXDESCRIP_RD_BUFF1VALID_MASK | ENET_RXDESCRIP_RD_IOC_MASK;
    uint32_t index = rxBdRing->rxGenIdx;
    /* Increase the index. */
    index++;
    if (index >= ENET_RX_RING_LEN) {
        index = 0;
    }

    rxBdRing->rxGenIdx = index;
    if (buf != NULL) {
        rxDesc->buff1Addr = (uint32_t)buf;
    }

    rxDesc->buff2Addr = 0;
    rxDesc->reserved = 0;
    rxDesc->control = control;
}

/** \brief  Free TX buffers that are complete
 *
 *  \param[in] lpc_enet  Pointer to driver data structure
 */
static void lpc546xx_tx_reclaim(struct lpc_enetdata *lpc_enet)
{
    static uint8_t consume_index = 0;
    enet_tx_bd_ring_t *txBdRing = (enet_tx_bd_ring_t *)&g_handle.txBdRing[0];

    while (consume_index != txBdRing->txConsumIdx) {
        /* Free the transmit buffer */
        free(tx_buff[consume_index]);
        tx_buff[consume_index] = NULL;

        osSemaphoreRelease(lpc_enetdata.xTXDCountSem.id);

        consume_index++;
        if (consume_index >= ENET_TX_RING_LEN) {
            consume_index = 0;
        }
    }
}

/********************************************************************************
 * Internal data
 ********************************************************************************/
#define ENET_BuffSizeAlign(n) ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_ALIGN(x,align)   ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)- 1)))
extern void lpc546xx_init_eth_hardware(void);

/** \brief  Allocates a pbuf and returns the data from the incoming packet.
 *
 *  \param[in] netif the lwip network interface structure for this lpc_enetif
 *  \return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *lpc546xx_low_level_input(struct netif *netif)
{
    enet_rx_bd_ring_t *rxBdRing = (enet_rx_bd_ring_t *)&g_handle.rxBdRing[0];
    enet_rx_bd_struct_t *bdPtr = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
#ifdef LOCK_RX_THREAD
    struct lpc_enetdata *lpc_enet = netif->state;
#endif
    struct pbuf *p = NULL;
    struct pbuf *temp_rxbuf = NULL;
    u32_t length = 0;

    /* Determine if a frame has been received */
    if ((bdPtr->control & ENET_RXDESCRIP_WR_OWN_MASK) != 0) {
        return p;
    }

#ifdef LOCK_RX_THREAD
    /* Get exclusive access */
    sys_mutex_lock(&lpc_enet->TXLockMutex);
#endif

    /* Determine if the received frame has an error */
    if ((bdPtr->control & ENET_RXDESCRIP_WR_ERRSUM_MASK) != 0) {
        LINK_STATS_INC(link.err);
        LINK_STATS_INC(link.drop);
        /* Re-use the same buffer in case of error */
        update_read_buffer(bdPtr, NULL);
    } else {
        if (bdPtr->control & ENET_RXDESCRIP_WR_LD_MASK) {
            length = (bdPtr->control & ENET_RXDESCRIP_WR_PACKETLEN_MASK);
        } else {
            length = rxBdRing->rxBuffSizeAlign;
        }

        /* Zero-copy */
        p = rx_buff[rxBdRing->rxGenIdx];
        p->len = length;

        /* Attempt to queue new buffer */
        temp_rxbuf = pbuf_alloc(PBUF_RAW, ENET_ALIGN(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT), PBUF_RAM);
        if (NULL == temp_rxbuf) {
            /* Drop frame (out of memory) */
            LINK_STATS_INC(link.drop);

            LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
                       ("lpc546xx_low_level_input: Packet index %d dropped for OOM\n", rxBdRing->rxGenIdx));

            /* Re-queue the same buffer */
            update_read_buffer(bdPtr, NULL);

#ifdef LOCK_RX_THREAD
            sys_mutex_unlock(&lpc_enet->TXLockMutex);
#endif

            return NULL;
        }

        rx_buff[rxBdRing->rxGenIdx] = temp_rxbuf;
        rx_ptr[rxBdRing->rxGenIdx] = (uint32_t)rx_buff[rxBdRing->rxGenIdx]->payload;

        LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
                    ("lpc_low_level_input: Packet received: %p, size %"PRIu32" (index=%d)\n", p, length, rxBdRing->rxGenIdx));

        update_read_buffer(bdPtr, rx_buff[rxBdRing->rxGenIdx]->payload);

        /* Save size */
        p->tot_len = (u16_t)length;
        LINK_STATS_INC(link.recv);
    }

#ifdef LOCK_RX_THREAD
    sys_mutex_unlock(&lpc_enet->TXLockMutex);
#endif

    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] netif the lwip network interface structure
 */
void lpc546xx_enetif_input(struct netif *netif)
{
    struct pbuf *p;

    /* move received packet into a new pbuf */
    p = lpc546xx_low_level_input(netif);
    if (p == NULL)
        return;

    /* pass all packets to ethernet_input, which decides what packets it supports */
    if (netif->input(p, netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("lpc546xx_enetif_input: input error\n"));
        /* Free buffer */
        pbuf_free(p);
    }
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void lpc546xx_emac_thread(void* pvParameters)
{
    struct lpc_enetdata *lpc_enet = pvParameters;

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX | FLAG_TX, osFlagsWaitAny, PHY_TASK_PERIOD_MS);
        if (flags == osFlagsErrorTimeout) {
            // Rather than calling strictly every period, we call when idle
            // for that period - hopefully good enough. We run this task
            // from lwIP's thread rather than our RX/TX thread, as PHY reads can
            // be slow, and we don't want them to interfere with data pumping.
            // This is analogous to the way the PHY polling works in the Nanostack
            // version of the driver
            tcpip_callback_with_block(lpc546xx_phy_task, lpc_enet->netif, 0);
            continue;
        }

        LWIP_ASSERT("osThreadFlagsWait error", !(flags & osFlagsError));
        if (flags & FLAG_RX) {
            lpc546xx_packet_rx(lpc_enet);
        }

        if (flags & FLAG_TX) {
            lpc546xx_packet_tx(lpc_enet);
        }
    }
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] netif the lwip network interface structure for this lpc_enetif
 *  \param[in] p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
static err_t lpc546xx_low_level_output(struct netif *netif, struct pbuf *p)
{
    struct lpc_enetdata *lpc_enet = netif->state;
    struct pbuf *q;
    uint8_t *psend = NULL, *dst;
    enet_tx_bd_ring_t *txBdRing = (enet_tx_bd_ring_t *)&g_handle.txBdRing[0];
    uint32_t index = txBdRing->txGenIdx;

    psend = (uint8_t *)calloc(1, ENET_ALIGN(p->tot_len, ENET_BUFF_ALIGNMENT));
    if (NULL == psend)
        return ERR_MEM;

    if (p->len == p->tot_len) {
        MEMCPY(psend, p->payload, p->len);
    } else {
        for (q = p, dst = psend; q != NULL; q = q->next) {
            MEMCPY(dst, q->payload, q->len);
            dst += q->len;
        }
    }
    /* Check if a descriptor is available for the transfer. */
    osStatus_t stat = osSemaphoreAcquire(lpc_enet->xTXDCountSem.id, 0);
    if (stat != osOK)
        return ERR_BUF;

    /* Save the buffer so that it can be freed when transmit is done */
    tx_buff[index] = psend;

    if (ENET_SendFrame(ENET, &g_handle, psend, p->tot_len) != kStatus_Success) {
        free(psend);
        tx_buff[index] = NULL;
        return ERR_IF;
    }

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt of LPC546XX.
 */
void enet_mac_rx_isr()
{
    osThreadFlagsSet(lpc_enetdata.thread, FLAG_RX);
}

void enet_mac_tx_isr()
{
    osThreadFlagsSet(lpc_enetdata.thread, FLAG_TX);
}

void ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, uint8_t channel, void *param)
{
    switch (event)
    {
        case kENET_RxIntEvent:
            enet_mac_rx_isr();
            break;
        case kENET_TxIntEvent:
            enet_mac_tx_isr();
            break;
        default:
            break;
    }
}

#if NO_SYS == 0

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 *
 *  \param[in] lpc_enet pointer to the interface data
 */
static void lpc546xx_packet_rx(struct lpc_enetdata *lpc_enet)
{
    enet_rx_bd_ring_t *rxBdRing = (enet_rx_bd_ring_t *)&g_handle.rxBdRing[0];
    enet_rx_bd_struct_t *bdPtr = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
    bool suspend = false;

    /* Suspend and command for rx. */
    if (ENET->DMA_CH[0].DMA_CHX_STAT & ENET_DMA_CH_DMA_CHX_STAT_RBU_MASK) {
        suspend = true;
    }

    while ((bdPtr->control & ENET_RXDESCRIP_WR_OWN_MASK) == 0) {
        lpc546xx_enetif_input(lpc_enet->netif);
        /* rxGenIdx gets updated, gets the next receive buffer descriptor */
        bdPtr = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
    }

    /* Set command for rx when it is suspend. */
    if (suspend)
    {
        ENET->DMA_CH[0].DMA_CHX_RXDESC_TAIL_PTR = ENET->DMA_CH[0].DMA_CHX_RXDESC_TAIL_PTR;
    }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the pbuf and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] lpc_enet pointer to the interface data
 */
static void lpc546xx_packet_tx(struct lpc_enetdata *lpc_enet)
{
    lpc546xx_tx_reclaim(lpc_enet);
}

#endif

/** \brief  Low level init of the MAC and PHY.
 *
 *  \param[in]      netif  Pointer to LWIP netif structure
 */
static err_t low_level_init(struct netif *netif)
{
    status_t status;
    uint8_t i;
    uint32_t refClock;
    phy_speed_t phy_speed;
    phy_duplex_t phy_duplex;
    uint32_t phyAddr = 0;
    bool link = false;
    enet_config_t config;
    uint32_t timeout = 0xFFFU;

    lpc546xx_init_eth_hardware();

    refClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    status = PHY_Init(ENET, phyAddr, refClock);
    if (status != kStatus_Success) {
        return ERR_IF;
    }

    ENET_GetDefaultConfig(&config);
    config.multiqueueCfg = NULL;

    while ((!link) && timeout) {
        PHY_GetLinkStatus(ENET, phyAddr, &link);
        if (link) {
            /* Get link information from PHY */
            PHY_GetLinkSpeedDuplex(ENET, phyAddr, &phy_speed, &phy_duplex);

            /* Change the MII speed and duplex for actual link status. */
            config.miiSpeed = (enet_mii_speed_t)phy_speed;
            config.miiDuplex = (enet_mii_duplex_t)phy_duplex;
        }
        timeout--;
    }

    if (!link) {
        return ERR_CONN;
    }

    AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rx_desc_start_addr[ENET_RX_RING_LEN], ENET_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t tx_desc_start_addr[ENET_TX_RING_LEN], ENET_BUFF_ALIGNMENT);

    /* Create buffers for each receive BD */
    for (i = 0; i < ENET_RX_RING_LEN; i++) {
        rx_buff[i] = pbuf_alloc(PBUF_RAW, ENET_ALIGN(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT), PBUF_RAM);
        if (NULL == rx_buff[i])
            return ERR_MEM;

        rx_ptr[i] = (uint32_t)rx_buff[i]->payload;
    }

     /* prepare the buffer configuration. */
    enet_buffer_config_t buffCfg = {
        ENET_RX_RING_LEN,
        ENET_TX_RING_LEN,
        &tx_desc_start_addr[0],
        &tx_desc_start_addr[0],
        &rx_desc_start_addr[0],
        &rx_desc_start_addr[ENET_RX_RING_LEN],
        rx_ptr,
        ENET_ALIGN(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT),
    };

    ENET_Init(ENET, &config, netif->hwaddr, refClock);

    /* Enable the tx & rx interrupt. */
    ENET_EnableInterrupts(ENET, kENET_DmaTx | kENET_DmaRx);

    /* Create the handler. */
    ENET_CreateHandler(ENET, &g_handle, &config, &buffCfg, ethernet_callback, netif);

    /* Initialize Descriptor. */
    ENET_DescriptorInit(ENET, &config, &buffCfg);

    return ERR_OK;
}

/**
 * This function is the ethernet IPv4 packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this lpc_enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV4
err_t lpc546xx_etharp_output_ipv4(struct netif *netif, struct pbuf *q,
    const ip4_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP)
        return etharp_output(netif, q, ipaddr);

    return ERR_CONN;
}
#endif

/**
 * This function is the ethernet IPv6 packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this lpc_enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV6
err_t lpc546xx_etharp_output_ipv6(struct netif *netif, struct pbuf *q,
    const ip6_addr_t *ipaddr)
{
    /* Only send packet is link is up */s
    if (netif->flags & NETIF_FLAG_LINK_UP)
        return ethip6_output(netif, q, ipaddr);

    return ERR_CONN;
}
#endif

/*******************************************************************************
 * PHY task: monitor link
*******************************************************************************/

#define STATE_UNKNOWN           (-1)

typedef struct {
    int connected;
    phy_speed_t speed;
    phy_duplex_t duplex;
} PHY_STATE;

int phy_link_status(void) {
    bool connection_status;
    uint32_t phyAddr = 0;

    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);
    return (int)connection_status;
}

static void lpc546xx_phy_task(void *data) {
    struct netif *netif = (struct netif*)data;
    static PHY_STATE prev_state = {STATE_UNKNOWN, (phy_speed_t)STATE_UNKNOWN, (phy_duplex_t)STATE_UNKNOWN};
    uint32_t phyAddr = 0;
    // Get current status
    PHY_STATE crt_state;
    bool connection_status;

    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);
    crt_state.connected = connection_status;
    // Get the actual PHY link speed
    PHY_GetLinkSpeedDuplex(ENET, phyAddr, &crt_state.speed, &crt_state.duplex);

    // Compare with previous state
    if (crt_state.connected != prev_state.connected) {
        // We're called from lwIP's tcpip thread, so can call link functions directly
        if (crt_state.connected) {
            netif_set_link_up(netif);
        } else {
            netif_set_link_down(netif);
        }
    }

    if (crt_state.speed != prev_state.speed) {
        uint32_t fes = ENET->MAC_CONFIG;
        fes &= ~ENET_MAC_CONFIG_FES_MASK;
        if (prev_state.speed != (phy_speed_t)STATE_UNKNOWN) {
            fes |= ENET_MAC_CONFIG_FES(!crt_state.speed);
        } else {
            fes |= ENET_MAC_CONFIG_FES(crt_state.speed);
        }

        ENET->MAC_CONFIG = fes;
    }

    prev_state = crt_state;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param[in] netif the lwip network interface structure for this lpc_enetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    err_t err;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    lpc_enetdata.netif = netif;

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

    netif->hwaddr_len = ETH_HWADDR_LEN;

     /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;
#ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#if LWIP_IPV6_MLD
    netif->flags |= NETIF_FLAG_MLD6;
#endif

    /* Initialize the hardware */
    netif->state = &lpc_enetdata;
    err = low_level_init(netif);
    if (err != ERR_OK)
        return err;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwiplpc546xx";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

#if LWIP_IPV4
    netif->output = lpc546xx_etharp_output_ipv4;
#endif
#if LWIP_IPV6
    netif->output_ip6 = lpc546xx_etharp_output_ipv6;
#endif

    netif->linkoutput = lpc546xx_low_level_output;

    /* CMSIS-RTOS, start tasks */
#if NO_SYS == 0
    memset(&lpc_enetdata.xTXDCountSem.data, 0, sizeof(lpc_enetdata.xTXDCountSem.data));
    lpc_enetdata.xTXDCountSem.attr.cb_mem = &lpc_enetdata.xTXDCountSem.data;
    lpc_enetdata.xTXDCountSem.attr.cb_size = sizeof(lpc_enetdata.xTXDCountSem.data);
    lpc_enetdata.xTXDCountSem.id = osSemaphoreNew(ENET_TX_RING_LEN, ENET_TX_RING_LEN, &lpc_enetdata.xTXDCountSem.attr);

    LWIP_ASSERT("xTXDCountSem creation error", (lpc_enetdata.xTXDCountSem.id != NULL));

    err = sys_mutex_new(&lpc_enetdata.TXLockMutex);
    LWIP_ASSERT("TXLockMutex creation error", (err == ERR_OK));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    tcpip_callback_with_block(lpc546xx_phy_task, netif, 1);
    osDelay(10);

    /* Worker thread */
    lpc_enetdata.thread = sys_thread_new("lpc546xx_emac_thread", lpc546xx_emac_thread, netif->state, THREAD_STACKSIZE, THREAD_PRIORITY)->id;
#endif

    /* Active TX/RX. */
    ENET_StartRxTx(ENET, 1, 1);

    return ERR_OK;
}

void eth_arch_enable_interrupts(void) {

}

void eth_arch_disable_interrupts(void) {

}

#endif /* LWIP_ARP || LWIP_ETHERNET */

