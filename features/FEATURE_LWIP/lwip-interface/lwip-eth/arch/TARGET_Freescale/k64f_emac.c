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

#include "fsl_phy.h"
#include "k64f_emac_config.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mbed_interface.h"

enet_handle_t g_handle;
// TX Buffer descriptors
uint8_t *tx_desc_start_addr;
// RX Buffer descriptors
uint8_t *rx_desc_start_addr;
// RX packet buffer pointers
struct pbuf *rx_buff[ENET_RX_RING_LEN];
// TX packet buffer pointers
struct pbuf *tx_buff[ENET_RX_RING_LEN];
// RX packet payload pointers
uint32_t *rx_ptr[ENET_RX_RING_LEN];

/********************************************************************************
 * Internal data
 ********************************************************************************/
#define ENET_BuffSizeAlign(n) ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_ALIGN(x,align)   ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)- 1)))
#if (defined(TARGET_K64F) && (defined(TARGET_FRDM)))
extern void k64f_init_eth_hardware(void);
#endif

#if (defined(TARGET_K66F) && (defined(TARGET_FRDM)))
extern void k66f_init_eth_hardware(void);
#endif

/* K64F EMAC driver data structure */
struct k64f_enetdata {
  struct netif *netif;  /**< Reference back to LWIP parent netif */
  osThreadId_t thread; /**< Processing thread */
  sys_mutex_t TXLockMutex; /**< TX critical section mutex */
  sys_sem_t xTXDCountSem; /**< TX free buffer counting semaphore */
  uint8_t tx_consume_index, tx_produce_index; /**< TX buffers ring */
};

static struct k64f_enetdata k64f_enetdata;

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

static void k64f_phy_task(void *data);
static void packet_rx(struct k64f_enetdata *k64f_enet);
static void packet_tx(struct k64f_enetdata *k64f_enet);

#define PHY_TASK_PERIOD_MS      200

/********************************************************************************
 * Buffer management
 ********************************************************************************/
/*
 * This function will queue a new receive buffer
 */
static void update_read_buffer(uint8_t *buf)
{
    if (buf != NULL) {
        g_handle.rxBdCurrent->buffer = buf;
    }

    /* Clears status. */
    g_handle.rxBdCurrent->control &= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;

    /* Sets the receive buffer descriptor with the empty flag. */
    g_handle.rxBdCurrent->control |= ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK;

    /* Increases the buffer descriptor to the next one. */
    if (g_handle.rxBdCurrent->control & ENET_BUFFDESCRIPTOR_RX_WRAP_MASK) {
        g_handle.rxBdCurrent = g_handle.rxBdBase;
    } else {
        g_handle.rxBdCurrent++;
    }

    /* Actives the receive buffer descriptor. */
    ENET->RDAR = ENET_RDAR_RDAR_MASK;
}

/** \brief  Free TX buffers that are complete
 *
 *  \param[in] k64f_enet  Pointer to driver data structure
 */
static void k64f_tx_reclaim(struct k64f_enetdata *k64f_enet)
{
  /* Get exclusive access */
  sys_mutex_lock(&k64f_enet->TXLockMutex);

  // Traverse all descriptors, looking for the ones modified by the uDMA
  while((k64f_enet->tx_consume_index != k64f_enet->tx_produce_index) &&
        (!(g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK))) {
      pbuf_free(tx_buff[k64f_enet->tx_consume_index % ENET_TX_RING_LEN]);
      if (g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
        g_handle.txBdDirty = g_handle.txBdBase;
      else
        g_handle.txBdDirty++;

      k64f_enet->tx_consume_index += 1;
      osSemaphoreRelease(k64f_enet->xTXDCountSem.id);
  }

  /* Restore access */
  sys_mutex_unlock(&k64f_enet->TXLockMutex);
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt of K64F.
 */
void enet_mac_rx_isr()
{
    osThreadFlagsSet(k64f_enetdata.thread, FLAG_RX);
}

void enet_mac_tx_isr()
{
    osThreadFlagsSet(k64f_enetdata.thread, FLAG_TX);
}

void ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
{
    switch (event)
    {
      case kENET_RxEvent:
        enet_mac_rx_isr();
        break;
      case kENET_TxEvent:
        enet_mac_tx_isr();
        break;
      default:
        break;
    }
}

/** \brief  Low level init of the MAC and PHY.
 *
 *  \param[in]      netif  Pointer to LWIP netif structure
 */
static err_t low_level_init(struct netif *netif)
{
  struct k64f_enetdata *k64f_enet = netif->state;
  uint8_t i;
  uint32_t sysClock;
  phy_speed_t phy_speed;
  phy_duplex_t phy_duplex;
  uint32_t phyAddr = 0;
  bool link = false;
  enet_config_t config;

  // Allocate RX descriptors
  rx_desc_start_addr = (uint8_t *)calloc(1, sizeof(enet_rx_bd_struct_t) * ENET_RX_RING_LEN + ENET_BUFF_ALIGNMENT);
  if(!rx_desc_start_addr)
    return ERR_MEM;

  // Allocate TX descriptors
  tx_desc_start_addr = (uint8_t *)calloc(1, sizeof(enet_tx_bd_struct_t) * ENET_TX_RING_LEN + ENET_BUFF_ALIGNMENT);
  if(!tx_desc_start_addr)
    return ERR_MEM;

  rx_desc_start_addr = (uint8_t *)ENET_ALIGN(rx_desc_start_addr, ENET_BUFF_ALIGNMENT);
  tx_desc_start_addr = (uint8_t *)ENET_ALIGN(tx_desc_start_addr, ENET_BUFF_ALIGNMENT);

  /* Create buffers for each receive BD */
  for (i = 0; i < ENET_RX_RING_LEN; i++) {
    rx_buff[i] = pbuf_alloc(PBUF_RAW, ENET_ETH_MAX_FLEN + ENET_BUFF_ALIGNMENT, PBUF_RAM);
    if (NULL == rx_buff[i])
      return ERR_MEM;

    /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
       RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
       a data structure which is internal to lwIP. This might not prove to be a good idea
       in the long run, but a better fix would probably involve modifying lwIP itself */
    rx_buff[i]->payload = (void*)ENET_ALIGN((uint32_t)rx_buff[i]->payload, ENET_BUFF_ALIGNMENT);
    rx_ptr[i] = rx_buff[i]->payload;
  }

  k64f_enet->tx_consume_index = k64f_enet->tx_produce_index = 0;

  /* prepare the buffer configuration. */
  enet_buffer_config_t buffCfg = {
    ENET_RX_RING_LEN,
    ENET_TX_RING_LEN,
    ENET_ALIGN(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT),
    0,
    (volatile enet_rx_bd_struct_t *)rx_desc_start_addr,
    (volatile enet_tx_bd_struct_t *)tx_desc_start_addr,
    (uint8_t *)&rx_ptr,
    NULL,
  };
#if (defined(TARGET_K64F) && (defined(TARGET_FRDM)))
  k64f_init_eth_hardware();
#endif

#if (defined(TARGET_K66F) && (defined(TARGET_FRDM)))
  k66f_init_eth_hardware();
#endif

  sysClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

  ENET_GetDefaultConfig(&config);

  PHY_Init(ENET, 0, sysClock);
  PHY_GetLinkStatus(ENET, phyAddr, &link);
  if (link)
  {
    /* Get link information from PHY */
    PHY_GetLinkSpeedDuplex(ENET, phyAddr, &phy_speed, &phy_duplex);
    /* Change the MII speed and duplex for actual link status. */
    config.miiSpeed = (enet_mii_speed_t)phy_speed;
    config.miiDuplex = (enet_mii_duplex_t)phy_duplex;
    config.interrupt = kENET_RxFrameInterrupt | kENET_TxFrameInterrupt;
  }
  config.rxMaxFrameLen = ENET_ETH_MAX_FLEN;
  config.macSpecialConfig = kENET_ControlFlowControlEnable;
  config.txAccelerConfig = kENET_TxAccelIsShift16Enabled;
  config.rxAccelerConfig = kENET_RxAccelisShift16Enabled | kENET_RxAccelMacCheckEnabled;
  ENET_Init(ENET, &g_handle, &config, &buffCfg, netif->hwaddr, sysClock);
  ENET_SetCallback(&g_handle, ethernet_callback, netif);
  ENET_ActiveRead(ENET);

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
#if LWIP_IPV4
err_t k64f_etharp_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
  /* Only send packet is link is up */
  if (netif->flags & NETIF_FLAG_LINK_UP) {
    return etharp_output(netif, q, ipaddr);
  }

  return ERR_CONN;
}
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
err_t k64f_etharp_output_ipv6(struct netif *netif, struct pbuf *q, const ip6_addr_t *ipaddr)
{
  /* Only send packet is link is up */
  if (netif->flags & NETIF_FLAG_LINK_UP) {
    return ethip6_output(netif, q, ipaddr);
  }

  return ERR_CONN;
}
#endif

#if LWIP_IGMP
/**
 * IPv4 address filtering setup.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] group IPv4 group to modify
 * \param[in] action
 * \return ERR_OK or error code
 */
err_t igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
    switch (action) {
        case NETIF_ADD_MAC_FILTER:
        {
            uint32_t group23 = ntohl(group->addr) & 0x007FFFFF;
            uint8_t addr[6];
            addr[0] = LL_IP4_MULTICAST_ADDR_0;
            addr[1] = LL_IP4_MULTICAST_ADDR_1;
            addr[2] = LL_IP4_MULTICAST_ADDR_2;
            addr[3] = group23 >> 16;
            addr[4] = group23 >> 8;
            addr[5] = group23;
            ENET_AddMulticastGroup(ENET, addr);
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            /* As we don't reference count, silently ignore delete requests */
            return ERR_OK;
        default:
            return ERR_ARG;
    }
}
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
err_t mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
    switch (action) {
        case NETIF_ADD_MAC_FILTER:
        {
            uint32_t group32 = ntohl(group->addr[3]);
            uint8_t addr[6];
            addr[0] = LL_IP6_MULTICAST_ADDR_0;
            addr[1] = LL_IP6_MULTICAST_ADDR_1;
            addr[2] = group32 >> 24;
            addr[3] = group32 >> 16;
            addr[4] = group32 >> 8;
            addr[5] = group32;
            ENET_AddMulticastGroup(ENET, addr);
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            /* As we don't reference count, silently ignore delete requests */
            return ERR_OK;
        default:
            return ERR_ARG;
    }
}
#endif

/** \brief  Allocates a pbuf and returns the data from the incoming packet.
 *
 *  \param[in] netif the lwip network interface structure
 *  \param[in] idx   index of packet to be read
 *  \return a pbuf filled with the received packet (including MAC header)
 */
static struct pbuf *k64f_low_level_input(struct netif *netif, int idx)
{
  volatile enet_rx_bd_struct_t *bdPtr = g_handle.rxBdCurrent;
  struct pbuf *p = NULL;
  struct pbuf *temp_rxbuf = NULL;
  u32_t length = 0;
  const u16_t err_mask = ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK | ENET_BUFFDESCRIPTOR_RX_CRC_MASK |
                         ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK | ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK;


#ifdef LOCK_RX_THREAD
  /* Get exclusive access */
  sys_mutex_lock(&k64f_enet->TXLockMutex);
#endif

  /* Determine if a frame has been received */
  if ((bdPtr->control & err_mask) != 0) {
#if LINK_STATS
    if ((bdPtr->control & ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK) != 0)
      LINK_STATS_INC(link.lenerr);
    else
      LINK_STATS_INC(link.chkerr);
#endif
    LINK_STATS_INC(link.drop);
    /* Re-use the same buffer in case of error */
    update_read_buffer(NULL);
  } else {
    /* A packet is waiting, get length */
    length = bdPtr->length;

    /* Zero-copy */
    p = rx_buff[idx];
    p->len = length;

    /* Attempt to queue new buffer */
    temp_rxbuf = pbuf_alloc(PBUF_RAW, ENET_ETH_MAX_FLEN + ENET_BUFF_ALIGNMENT, PBUF_RAM);
    if (NULL == temp_rxbuf) {
      /* Drop frame (out of memory) */
      LINK_STATS_INC(link.drop);

      /* Re-queue the same buffer */
      update_read_buffer(NULL);

      LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
        ("k64f_low_level_input: Packet index %d dropped for OOM\n",
        idx));
#ifdef LOCK_RX_THREAD
      sys_mutex_unlock(&k64f_enet->TXLockMutex);
#endif

      return NULL;
    }

    rx_buff[idx] = temp_rxbuf;
    /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
       RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
       a data structure which is internal to lwIP. This might not prove to be a good idea
       in the long run, but a better fix would probably involve modifying lwIP itself */
    rx_buff[idx]->payload = (void*)ENET_ALIGN((uint32_t)rx_buff[idx]->payload, ENET_BUFF_ALIGNMENT);
    rx_ptr[idx] = rx_buff[idx]->payload;

    update_read_buffer(rx_buff[idx]->payload);
    LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
      ("k64f_low_level_input: Packet received: %p, size %"PRIu32" (index=%d)\n",
      p, length, idx));

    /* Save size */
    p->tot_len = (u16_t) length;
    LINK_STATS_INC(link.recv);
  }

#ifdef LOCK_RX_THREAD
  sys_mutex_unlock(&k64f_enet->TXLockMutex);
#endif

  return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] netif the lwip network interface structure
 *  \param[in] idx   index of packet to be read
 */
void k64f_enetif_input(struct netif *netif, int idx)
{
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = k64f_low_level_input(netif, idx);
  if (p == NULL)
    return;

  /* pass all packets to ethernet_input, which decides what packets it supports */
  if (netif->input(p, netif) != ERR_OK) {
      LWIP_DEBUGF(NETIF_DEBUG, ("k64f_enetif_input: input error\n"));
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
static void emac_thread(void* pvParameters)
{
    struct k64f_enetdata *k64f_enet = pvParameters;

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX|FLAG_TX, osFlagsWaitAny, PHY_TASK_PERIOD_MS);
        if (flags == osFlagsErrorTimeout) {
            // Rather than calling strictly every period, we call when idle
            // for that period - hopefully good enough. We run this task
            // from lwIP's thread rather than our RX/TX thread, as PHY reads can
            // be slow, and we don't want them to interfere with data pumping.
            // This is analogous to the way the PHY polling works in the Nanostack
            // version of the driver
            tcpip_callback_with_block(k64f_phy_task, k64f_enet->netif, 0);
            continue;
        }

        LWIP_ASSERT("osThreadFlagsWait error", !(flags & osFlagsError));

        if (flags & FLAG_RX) {
            packet_rx(k64f_enet);
        }

        if (flags & FLAG_TX) {
            packet_tx(k64f_enet);
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
static void packet_rx(struct k64f_enetdata *k64f_enet)
{
    static int idx = 0;

    while ((g_handle.rxBdCurrent->control & ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK) == 0) {
        k64f_enetif_input(k64f_enet->netif, idx);
        idx = (idx + 1) % ENET_RX_RING_LEN;
    }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the pbuf and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] k64f_enet pointer to the interface data
 */
static void packet_tx(struct k64f_enetdata *k64f_enet)
{
    k64f_tx_reclaim(k64f_enet);
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] netif the lwip network interface structure for this netif
 *  \param[in] p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
static err_t k64f_low_level_output(struct netif *netif, struct pbuf *p)
{
  struct k64f_enetdata *k64f_enet = netif->state;
  struct pbuf *q;
  struct pbuf *temp_pbuf;
  uint8_t *psend = NULL, *dst;

  temp_pbuf = pbuf_alloc(PBUF_RAW, p->tot_len + ENET_BUFF_ALIGNMENT, PBUF_RAM);
  if (NULL == temp_pbuf)
    return ERR_MEM;

  /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
     RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
     a data structure which is internal to lwIP. This might not prove to be a good idea
     in the long run, but a better fix would probably involve modifying lwIP itself */
  psend = (uint8_t *)ENET_ALIGN((uint32_t)temp_pbuf->payload, ENET_BUFF_ALIGNMENT);

  for (q = p, dst = psend; q != NULL; q = q->next) {
    MEMCPY(dst, q->payload, q->len);
    dst += q->len;
  }

  /* Check if a descriptor is available for the transfer. */
  osStatus_t stat = osSemaphoreAcquire(k64f_enet->xTXDCountSem.id, 0);
  if (stat != osOK)
    return ERR_BUF;

  /* Get exclusive access */
  sys_mutex_lock(&k64f_enet->TXLockMutex);

  /* Save the buffer so that it can be freed when transmit is done */
  tx_buff[k64f_enet->tx_produce_index % ENET_TX_RING_LEN] = temp_pbuf;
  k64f_enet->tx_produce_index += 1;

  /* Setup transfers */
  g_handle.txBdCurrent->buffer = psend;
  g_handle.txBdCurrent->length = p->tot_len;
  g_handle.txBdCurrent->control |= (ENET_BUFFDESCRIPTOR_TX_READY_MASK | ENET_BUFFDESCRIPTOR_TX_LAST_MASK);

  /* Increase the buffer descriptor address. */
  if (g_handle.txBdCurrent->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
    g_handle.txBdCurrent = g_handle.txBdBase;
  else
    g_handle.txBdCurrent++;

  /* Active the transmit buffer descriptor. */
  ENET->TDAR = ENET_TDAR_TDAR_MASK;

  LINK_STATS_INC(link.xmit);

  /* Restore access */
  sys_mutex_unlock(&k64f_enet->TXLockMutex);

  return ERR_OK;
}

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

static void k64f_phy_task(void *data)
{
    struct netif *netif = data;

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
      uint32_t rcr = ENET->RCR;
      rcr &= ~ENET_RCR_RMII_10T_MASK;
      rcr |= ENET_RCR_RMII_10T(!crt_state.speed);
      ENET->RCR = rcr;
    }

    prev_state = crt_state;
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
  err_t err;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

  k64f_enetdata.netif = netif;

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
  // TODOETH: check if the flags are correct below
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

  /* Initialize the hardware */
  netif->state = &k64f_enetdata;
  err = low_level_init(netif);
  if (err != ERR_OK)
    return err;

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwipk64f";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = 'e';
  netif->name[1] = 'n';

#if LWIP_IPV4
  netif->output = k64f_etharp_output_ipv4;
#if LWIP_IGMP
  netif->igmp_mac_filter = igmp_mac_filter;
  netif->flags |= NETIF_FLAG_IGMP;
#endif
#endif
#if LWIP_IPV6
  netif->output_ip6 = k64f_etharp_output_ipv6;
#if LWIP_IPV6_MLD
  netif->mld_mac_filter = mld_mac_filter;
  netif->flags |= NETIF_FLAG_MLD6;
#else
  // Would need to enable all multicasts here - no API in fsl_enet to do that
  #error "IPv6 multicasts won't be received if LWIP_IPV6_MLD is disabled, breaking the system"
#endif
#endif
  netif->linkoutput = k64f_low_level_output;

  /* CMSIS-RTOS, start tasks */
  memset(&k64f_enetdata.xTXDCountSem.data, 0, sizeof(k64f_enetdata.xTXDCountSem.data));
  k64f_enetdata.xTXDCountSem.attr.cb_mem = &k64f_enetdata.xTXDCountSem.data;
  k64f_enetdata.xTXDCountSem.attr.cb_size = sizeof(k64f_enetdata.xTXDCountSem.data);
  k64f_enetdata.xTXDCountSem.id = osSemaphoreNew(ENET_TX_RING_LEN, ENET_TX_RING_LEN, &k64f_enetdata.xTXDCountSem.attr);

  LWIP_ASSERT("xTXDCountSem creation error", (k64f_enetdata.xTXDCountSem.id != NULL));

  err = sys_mutex_new(&k64f_enetdata.TXLockMutex);
  LWIP_ASSERT("TXLockMutex creation error", (err == ERR_OK));

  /* Allow the PHY task to detect the initial link state and set up the proper flags */
  tcpip_callback_with_block(k64f_phy_task, netif, 1);
  osDelay(10);

  /* Worker thread */
  k64f_enetdata.thread = sys_thread_new("k64f_emac_thread", emac_thread, netif->state, THREAD_STACKSIZE, THREAD_PRIORITY)->id;

  return ERR_OK;
}

void eth_arch_enable_interrupts(void) {
  //NVIC_SetPriority(ENET_Receive_IRQn, 6U);
  //NVIC_SetPriority(ENET_Transmit_IRQn, 6U);
}

void eth_arch_disable_interrupts(void) {

}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

