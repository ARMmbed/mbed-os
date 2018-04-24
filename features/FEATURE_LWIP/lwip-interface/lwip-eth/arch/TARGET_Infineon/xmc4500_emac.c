/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
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
#include "xmc_eth_mac.h"
#include "xmc_eth_phy.h"
#include "xmc_gpio.h"
#include "eth_arch.h"
#include "sys_arch.h"

/****************************************************************** Defines **/
#define RXD1     P2_3
#define RXD0     P2_2
#define RXER     P2_4
#define CLK_RMII P15_8
#define TX_EN    P2_5
#define TXD1     P2_9
#define TXD0     P2_8
#define CRS_DV   P15_9
#define MDIO     P2_0
#define MDC      P2_7

#define PHY_ADDR                0
#define STATE_UNKNOWN           (-1)

#define XMC_ETH_MAC_NUM_RX_BUF (4)
#define XMC_ETH_MAC_NUM_TX_BUF (4)

/*Maximum retry iterations for phy auto-negotiation*/
#define ETH_LWIP_PHY_MAX_RETRIES  0xfffffU

/* \brief Flags for worker thread */
#define FLAG_TX  1
#define FLAG_RX  2

#define PHY_TASK_PERIOD_MS      200

/****************************************************************** Typedef **/

/* XMC4500 EMAC driver data structure */
typedef struct lpc_enetdata_s {
    struct netif *netif;      ///> Reference back to LWIP parent netif
    osThreadId_t thread;      ///>Processing thread
    sys_mutex_t TXLockMutex;  ///> TX critical section mutex
    sys_sem_t xTXDCountSem;   ///> TX free buffer counting semaphore
}xmc_enetdata_t;

typedef struct {
    int connected;
    int speed;
    int duplex;
} PHY_STATE;

/****************************************************************** Globals **/
static xmc_enetdata_t xmc_enetdata;
#if defined(__GNUC__)
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t rx_desc[XMC_ETH_MAC_NUM_RX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t tx_desc[XMC_ETH_MAC_NUM_TX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t rx_buf[XMC_ETH_MAC_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t tx_buf[XMC_ETH_MAC_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
#endif

XMC_ETH_MAC_t eth_mac =
{
        .regs = ETH0,
        .address = 0,
        .rx_desc = rx_desc,
        .tx_desc = tx_desc,
        .rx_buf = &rx_buf[0][0],
        .tx_buf = &tx_buf[0][0],
        .num_rx_buf = XMC_ETH_MAC_NUM_RX_BUF,
        .num_tx_buf = XMC_ETH_MAC_NUM_TX_BUF
};

static XMC_ETH_PHY_CONFIG_t eth_phy_config =
{
        .interface = XMC_ETH_LINK_INTERFACE_RMII,
        .enable_auto_negotiate = true
};

/****************************************************** Function prototypes **/
static void xmc_phy_task(void *data);
static void packet_rx(xmc_enetdata_t *xmc_enet);
static void packet_tx(xmc_enetdata_t *xmc_enet);
extern void mbed_mac_address(char *mac);

/**************************************************************** Functions **/

/**
 * Check PHY link status
 */
static void PHY_GetLinkStatus(XMC_ETH_MAC_t *eth_mac, uint32_t phyAddr, bool *status)
{

    if (XMC_ETH_PHY_GetLinkStatus(eth_mac, phyAddr) == XMC_ETH_LINK_STATUS_DOWN){
        /* link down */
        *status = false;
    }else{
        /* link up */
        *status = true;
    }
}

/**
 * Get PHY Link speed and duplex
 */
static void PHY_GetLinkSpeedDuplex(XMC_ETH_MAC_t *eth_mac, uint32_t phyAddr, XMC_ETH_LINK_SPEED_t *speed, XMC_ETH_LINK_DUPLEX_t *duplex)
{

    /* Get speed and duplex from PHY driver */
    *speed = XMC_ETH_PHY_GetLinkSpeed(eth_mac, phyAddr);
    *duplex = XMC_ETH_PHY_GetLinkDuplex(eth_mac, phyAddr);
}

/**
 * Init PHY
 */
static int PHY_Init(XMC_ETH_MAC_t *eth_mac, uint32_t phyAddr, XMC_ETH_PHY_CONFIG_t *eth_phy_config){

    bool phy_autoneg_state;
    uint32_t retries = 0U;
    int32_t status;

    if((status = XMC_ETH_PHY_Init(eth_mac, phyAddr, eth_phy_config)) != XMC_ETH_PHY_STATUS_OK)
    {
        return 1;
    }

    /* If autonegotiation is enabled */
    do {
        phy_autoneg_state = XMC_ETH_PHY_IsAutonegotiationCompleted(eth_mac, phyAddr);
        retries++;
    } while ((phy_autoneg_state == false) && (retries < ETH_LWIP_PHY_MAX_RETRIES));

    if(phy_autoneg_state == false)
    {
        return 2;
    }

    return 0;
}

/**
 * Low level Ethernet init
 */
static err_t low_level_init(struct netif *netif)
{
    uint32_t phyAddr = 0;
    bool link = false;
    XMC_ETH_LINK_SPEED_t speed;
    XMC_ETH_LINK_DUPLEX_t duplex;
    uint64_t mac_addr = 0;

    XMC_ETH_MAC_PORT_CTRL_t port_control;
    XMC_GPIO_CONFIG_t gpio_config;

    /* Do whatever else is needed to initialize interface. */
    gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
    XMC_GPIO_Init(RXD0, &gpio_config);
    XMC_GPIO_Init(RXD1, &gpio_config);
    XMC_GPIO_Init(CLK_RMII, &gpio_config);
    XMC_GPIO_Init(CRS_DV, &gpio_config);
    XMC_GPIO_Init(RXER, &gpio_config);
    XMC_GPIO_Init(MDIO, &gpio_config);

    port_control.mode = XMC_ETH_MAC_PORT_CTRL_MODE_RMII;
    port_control.rxd0 = XMC_ETH_MAC_PORT_CTRL_RXD0_P2_2;
    port_control.rxd1 = XMC_ETH_MAC_PORT_CTRL_RXD1_P2_3;
    port_control.clk_rmii = XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P15_8;
    port_control.crs_dv = XMC_ETH_MAC_PORT_CTRL_CRS_DV_P15_9;
    port_control.rxer = XMC_ETH_MAC_PORT_CTRL_RXER_P2_4;
    port_control.mdio = XMC_ETH_MAC_PORT_CTRL_MDIO_P2_0;
    mac_addr = (((uint64_t)netif->hwaddr[0] << 0U) | ((uint64_t)netif->hwaddr[1] << 8U) | ((uint64_t)netif->hwaddr[2] << 16U) | ((uint64_t)netif->hwaddr[3] << 24U) | ((uint64_t)netif->hwaddr[4] << 32U) | ((uint64_t)netif->hwaddr[5] << 40U));
    eth_mac.address = mac_addr;
    XMC_ETH_MAC_SetPortControl(&eth_mac, port_control);

    XMC_ETH_MAC_Init(&eth_mac);

    XMC_ETH_MAC_DisableJumboFrame(&eth_mac);
    XMC_ETH_MAC_EnableReceptionBroadcastFrames(&eth_mac);

    gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
    gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE;
    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL |P2_8_AF_ETH0_TXD0);
    XMC_GPIO_Init(TXD0, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_9_AF_ETH0_TXD1);
    XMC_GPIO_Init(TXD1, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_5_AF_ETH0_TX_EN);
    XMC_GPIO_Init(TX_EN, &gpio_config);

    gpio_config.mode = (XMC_GPIO_MODE_t)((uint32_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL | P2_7_AF_ETH0_MDC);
    XMC_GPIO_Init(MDC, &gpio_config);

    XMC_GPIO_SetHardwareControl(MDIO, XMC_GPIO_HWCTRL_PERIPHERAL1);

    /* Initialize PHY */
    PHY_Init(&eth_mac, 0, &eth_phy_config);
    PHY_GetLinkStatus(&eth_mac, phyAddr, &link);

    if(link){
        PHY_GetLinkSpeedDuplex(&eth_mac, phyAddr, &speed, &duplex);

        XMC_ETH_MAC_SetLink(&eth_mac, speed, duplex);
        /* Enable ethernet interrupts */
        XMC_ETH_MAC_EnableEvent(&eth_mac, (uint32_t)XMC_ETH_MAC_EVENT_RECEIVE);

        NVIC_SetPriority(ETH0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63U, 0U));
        NVIC_ClearPendingIRQ(ETH0_0_IRQn);
        NVIC_EnableIRQ(ETH0_0_IRQn);
        XMC_ETH_MAC_EnableTx(&eth_mac);
        XMC_ETH_MAC_EnableRx(&eth_mac);
    }else{
        /* Enable ethernet interrupts */
        XMC_ETH_MAC_DisableEvent(&eth_mac, (uint32_t)XMC_ETH_MAC_EVENT_RECEIVE);
        NVIC_DisableIRQ(ETH0_0_IRQn);

        XMC_ETH_MAC_DisableTx(&eth_mac);
        XMC_ETH_MAC_DisableRx(&eth_mac);
    }

    return ERR_OK;
}

/**
 * This function is the ipv4 ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
err_t xmc_etharp_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return etharp_output(netif, q, ipaddr);
    }

    return ERR_CONN;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(void)
{
    struct pbuf *p = NULL;
    struct pbuf *q;
    uint32_t len;
    uint8_t *buffer;

    if (XMC_ETH_MAC_IsRxDescriptorOwnedByDma(&eth_mac) == false)
    {
        len = XMC_ETH_MAC_GetRxFrameSize(&eth_mac);

        if ((len > 0U) && (len <= (uint32_t)XMC_ETH_MAC_BUF_SIZE))
        {
#if ETH_PAD_SIZE
            len += ETH_PAD_SIZE;    /* allow room for Ethernet padding */
#endif

            /* We allocate a pbuf chain of pbufs from the pool. */
            p = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_POOL);

            if (p != NULL)
            {
#if ETH_PAD_SIZE
                pbuf_header(p, -ETH_PAD_SIZE);  /* drop the padding word */
#endif

                buffer = XMC_ETH_MAC_GetRxBuffer(&eth_mac);

                len = 0U;
                /* We iterate over the pbuf chain until we have read the entire
                 * packet into the pbuf. */
                for (q = p; q != NULL; q = q->next)
                {
                    /* Read enough bytes to fill this pbuf in the chain. The
                     * available data in the pbuf is given by the q->len
                     * variable.
                     * This does not necessarily have to be a memcpy, you can also preallocate
                     * pbufs for a DMA-enabled MAC and after receiving truncate it to the
                     * actually received size. In this case, ensure the tot_len member of the
                     * pbuf is the sum of the chained pbuf len members.
                     */
                    MEMCPY(q->payload, &buffer[len], q->len);
                    len += q->len;
                }
#if ETH_PAD_SIZE
                pbuf_header(p, ETH_PAD_SIZE);    /* Reclaim the padding word */
#endif
                XMC_ETH_MAC_ReturnRxDescriptor(&eth_mac);
            }
        }
        else
        {
            XMC_ETH_MAC_ReturnRxDescriptor(&eth_mac);
        }
    }
    XMC_ETH_MAC_ResumeRx(&eth_mac);
    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] netif the lwip network interface structure
 *  \param[in] idx   index of packet to be read
 */
void xmc_enetif_input(struct netif *netif, int idx)
{
    struct pbuf *p;
    struct eth_hdr *ethhdr;

    NVIC_DisableIRQ(ETH0_0_IRQn);

    p = low_level_input();

    while (p != NULL)
    {
        ethhdr = p->payload;
        switch (htons(ethhdr->type))
        {
        case ETHTYPE_IP:
        case ETHTYPE_ARP:
            /* full packet send to tcpip_thread to process */
            if (netif->input( p, netif) != ERR_OK)
            {
                pbuf_free(p);
            }

            break;

        default:
            pbuf_free(p);
            break;
        }

        p = low_level_input();
    }

    NVIC_ClearPendingIRQ(ETH0_0_IRQn);
    NVIC_EnableIRQ(ETH0_0_IRQn);
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void emac_thread(void* pvParameters)
{
    xmc_enetdata_t *xmc_enet = pvParameters;

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX|FLAG_TX, osFlagsWaitAny, PHY_TASK_PERIOD_MS);
        if (flags == osFlagsErrorTimeout) {
            // Rather than calling strictly every period, we call when idle
            // for that period - hopefully good enough. We run this task
            // from lwIP's thread rather than our RX/TX thread, as PHY reads can
            // be slow, and we don't want them to interfere with data pumping.
            // This is analogous to the way the PHY polling works in the Nanostack
            // version of the driver
            tcpip_callback_with_block(xmc_phy_task, &xmc_enet->netif, 0);
            continue;
        }

        LWIP_ASSERT("osThreadFlagsWait error", !(flags & osFlagsError));

        if (flags & FLAG_RX) {
            packet_rx(xmc_enet);
        }

        if (flags & FLAG_TX) {
            packet_tx(xmc_enet);
        }
    }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 *
 *  \param[in] k64f_enet pointer to the interface data
 */
static void packet_rx(xmc_enetdata_t *xmc_enet)
{
    int idx = 0;

    xmc_enetif_input(xmc_enet->netif, idx);
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the pbuf and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] k64f_enet pointer to the interface data
 */
static void packet_tx(xmc_enetdata_t *xmc_enet)
{
    // Not supported now
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] netif the lwip network interface structure for this netif
 *  \param[in] p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
static err_t xmc_low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    uint32_t framelen = 0U;
    uint8_t *buffer;

    if (p->tot_len > (u16_t)XMC_ETH_MAC_BUF_SIZE) {
        return ERR_BUF;
    }

    if (XMC_ETH_MAC_IsTxDescriptorOwnedByDma(&eth_mac))
    {
        XMC_ETH_MAC_ResumeTx(&eth_mac);

        return ERR_BUF;
    }
    else
    {
        buffer = XMC_ETH_MAC_GetTxBuffer(&eth_mac);

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE);    /* Drop the padding word */
#endif

        for(q = p; q != NULL; q = q->next)
        {
            /* Send the data from the pbuf to the interface, one pbuf at a
           time. The size of the data in each pbuf is kept in the ->len
           variable. */
            MEMCPY(buffer, q->payload, q->len);
            framelen += (uint32_t)q->len;
            buffer += q->len;
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE);    /* Reclaim the padding word */
#endif

        XMC_ETH_MAC_SetTxBufferSize(&eth_mac, framelen);

        XMC_ETH_MAC_ReturnTxDescriptor(&eth_mac);
        XMC_ETH_MAC_ResumeTx(&eth_mac);

        return ERR_OK;
    }
}

/**
 * Physical Ethernet task (Check link status)
 */
static void xmc_phy_task(void *data)
{
    struct netif *netif = data;
    static PHY_STATE prev_state = {STATE_UNKNOWN, STATE_UNKNOWN, STATE_UNKNOWN};
    uint32_t phyAddr = 0;

    /* Get current status */
    PHY_STATE crt_state;
    bool connection_status;
    PHY_GetLinkStatus(&eth_mac, phyAddr, &connection_status);
    crt_state.connected = connection_status;

    /* Get the actual PHY link speed */
    PHY_GetLinkSpeedDuplex(&eth_mac, phyAddr, (XMC_ETH_LINK_SPEED_t *)&crt_state.speed, (XMC_ETH_LINK_DUPLEX_t *)&crt_state.duplex);

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
        XMC_ETH_MAC_SetLink(&eth_mac, (XMC_ETH_LINK_SPEED_t)crt_state.speed, (XMC_ETH_LINK_DUPLEX_t)crt_state.duplex);
    }

    prev_state = crt_state;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    err_t err;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    xmc_enetdata.netif = netif;

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
    netif->state = &xmc_enetdata;
    err = low_level_init(netif);
    if (err != ERR_OK)
        return err;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "xmc4500";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

    netif->output = xmc_etharp_output_ipv4;

    netif->linkoutput = xmc_low_level_output;

    LWIP_ASSERT("xTXDCountSem creation error", (xmc_enetdata.xTXDCountSem.id != NULL));

    err = sys_mutex_new(&xmc_enetdata.TXLockMutex);
    LWIP_ASSERT("TXLockMutex creation error", (err == ERR_OK));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    tcpip_callback_with_block(xmc_phy_task, netif, 1);
    osDelay(10);

    /* Worker thread */
    xmc_enetdata.thread = sys_thread_new("xmc_emac_thread", emac_thread, netif->state, DEFAULT_THREAD_STACKSIZE, osPriorityNormal)->id;

    /* Trigger thread to deal with any RX packets that arrived before thread was started */
    packet_rx(&xmc_enetdata);
    return ERR_OK;
}

/**
 * Enable Ethernet Interrupts
 */
void eth_arch_enable_interrupts(void) {

    NVIC_EnableIRQ(ETH0_0_IRQn);
}

/**
 * Disable Ethernet Interrupts
 */
void eth_arch_disable_interrupts(void) {

    NVIC_DisableIRQ(ETH0_0_IRQn);
}

/**
 * Ethernet IRQ Handler
 */
void ETH0_0_IRQHandler(void)
{
    XMC_ETH_MAC_ClearEventStatus(&eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);

    if (xmc_enetdata.thread) {
        osThreadFlagsSet(xmc_enetdata.thread, FLAG_RX);
    }
}

/*EOF*/

