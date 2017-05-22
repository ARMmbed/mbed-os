#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

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
extern void k64f_init_eth_hardware(void);

/* K64F EMAC driver data structure */
struct k64f_enetdata {
  struct netif *netif;  /**< Reference back to LWIP parent netif */
  sys_sem_t RxReadySem; /**< RX packet ready semaphore */
  sys_sem_t TxCleanSem; /**< TX cleanup thread wakeup semaphore */
  sys_mutex_t TXLockMutex; /**< TX critical section mutex */
  sys_sem_t xTXDCountSem; /**< TX free buffer counting semaphore */
  uint8_t tx_consume_index, tx_produce_index; /**< TX buffers ring */
};

static struct k64f_enetdata k64f_enetdata;

/** \brief  Driver transmit and receive thread priorities
 *
 * Thread priorities for receive thread and TX cleanup thread. Alter
 * to prioritize receive or transmit bandwidth. In a heavily loaded
 * system or with LEIP_DEBUG enabled, the priorities might be better
 * the same. */
#define RX_PRIORITY   (osPriorityNormal)
#define TX_PRIORITY   (osPriorityNormal)
#define PHY_PRIORITY  (osPriorityNormal)

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
        g_handle.rxBdDirty = g_handle.rxBdBase;
    } else {
        g_handle.rxBdCurrent++;
        g_handle.rxBdDirty++;
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
  uint8_t i = 0 ;

  /* Get exclusive access */
  sys_mutex_lock(&k64f_enet->TXLockMutex);

  i = k64f_enet->tx_consume_index;
  // Traverse all descriptors, looking for the ones modified by the uDMA
  while((i != k64f_enet->tx_produce_index) && (!(g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK))) {
      pbuf_free(tx_buff[i]);
      if (g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
        g_handle.txBdDirty = g_handle.txBdBase;
      else
        g_handle.txBdDirty++;

      i = (i + 1) % ENET_TX_RING_LEN;
  }

  k64f_enet->tx_consume_index = i;
  /* Restore access */
  sys_mutex_unlock(&k64f_enet->TXLockMutex);
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt of K64F.
 */
void enet_mac_rx_isr()
{
  sys_sem_signal(&k64f_enetdata.RxReadySem);
}

void enet_mac_tx_isr()
{
  sys_sem_signal(&k64f_enetdata.TxCleanSem);
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

  k64f_init_eth_hardware();

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
 * This function is the ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
err_t k64f_etharp_output(struct netif *netif, struct pbuf *q, ip_addr_t *ipaddr)
{
  /* Only send packet is link is up */
  if (netif->flags & NETIF_FLAG_LINK_UP)
    return etharp_output(netif, q, ipaddr);

  return ERR_CONN;
}

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
      ("k64f_low_level_input: Packet received: %p, size %d (index=%d)\n",
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
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = k64f_low_level_input(netif, idx);
  if (p == NULL)
    return;

  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = (struct eth_hdr*)p->payload;

  switch (htons(ethhdr->type)) {
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
      /* full packet send to tcpip_thread to process */
      if (netif->input(p, netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("k64f_enetif_input: IP input error\n"));
        /* Free buffer */
        pbuf_free(p);
      }
      break;

    default:
      /* Return buffer */
      pbuf_free(p);
      break;
  }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void packet_rx(void* pvParameters) {
  struct k64f_enetdata *k64f_enet = pvParameters;
  int idx = 0;

  while (1) {
    /* Wait for receive task to wakeup */
    sys_arch_sem_wait(&k64f_enet->RxReadySem, 0);

    while ((g_handle.rxBdCurrent->control & ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK) == 0) {
      k64f_enetif_input(k64f_enet->netif, idx);
      idx = (idx + 1) % ENET_RX_RING_LEN;
    }
  }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the pbuf and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void packet_tx(void* pvParameters) {
  struct k64f_enetdata *k64f_enet = pvParameters;

  while (1) {
    /* Wait for transmit cleanup task to wakeup */
    sys_arch_sem_wait(&k64f_enet->TxCleanSem, 0);
    k64f_tx_reclaim(k64f_enet);
  }
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

  /* Wait until a descriptor is available for the transfer. */
  /* THIS WILL BLOCK UNTIL THERE ARE A DESCRIPTOR AVAILABLE */
  while (g_handle.txBdCurrent->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
    osSemaphoreWait(k64f_enet->xTXDCountSem.id, osWaitForever);

  /* Get exclusive access */
  sys_mutex_lock(&k64f_enet->TXLockMutex);

  /* Save the buffer so that it can be freed when transmit is done */
  tx_buff[k64f_enet->tx_produce_index] = temp_pbuf;
  k64f_enet->tx_produce_index = (k64f_enet->tx_produce_index + 1) % ENET_TX_RING_LEN;

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

#define PHY_TASK_PERIOD_MS      200
#define STATE_UNKNOWN           (-1)

typedef struct {
    int connected;
    phy_speed_t speed;
    phy_duplex_t duplex;
} PHY_STATE;

int phy_link_status() {
    bool connection_status;
    uint32_t phyAddr = 0;

    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);
    return (int)connection_status;
}

static void k64f_phy_task(void *data) {
  struct netif *netif = (struct netif*)data;
  bool connection_status;
  PHY_STATE crt_state = {STATE_UNKNOWN, (phy_speed_t)STATE_UNKNOWN, (phy_duplex_t)STATE_UNKNOWN};
  PHY_STATE prev_state;
  uint32_t phyAddr = 0;
  uint32_t rcr = 0;

  prev_state = crt_state;
  while (true) {
    // Get current status
    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);
    crt_state.connected = connection_status ? 1 : 0;
    // Get the actual PHY link speed
    PHY_GetLinkSpeedDuplex(ENET, phyAddr, &crt_state.speed, &crt_state.duplex);

    // Compare with previous state
    if (crt_state.connected != prev_state.connected) {
      if (crt_state.connected)
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, (void*) netif, 1);
      else
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1);
    }

    if (crt_state.speed != prev_state.speed) {
      rcr = ENET->RCR;
      rcr &= ~ENET_RCR_RMII_10T_MASK;
      rcr |= ENET_RCR_RMII_10T(!crt_state.speed);
      ENET->RCR = rcr;
    }

    prev_state = crt_state;
    osDelay(PHY_TASK_PERIOD_MS);
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
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  // TODOETH: check if the flags are correct below
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP;

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

  netif->output = k64f_etharp_output;
  netif->linkoutput = k64f_low_level_output;

  /* CMSIS-RTOS, start tasks */
#ifdef CMSIS_OS_RTX
  memset(k64f_enetdata.xTXDCountSem.data, 0, sizeof(k64f_enetdata.xTXDCountSem.data));
  k64f_enetdata.xTXDCountSem.def.semaphore = k64f_enetdata.xTXDCountSem.data;
#endif
  k64f_enetdata.xTXDCountSem.id = osSemaphoreCreate(&k64f_enetdata.xTXDCountSem.def, ENET_TX_RING_LEN);

  LWIP_ASSERT("xTXDCountSem creation error", (k64f_enetdata.xTXDCountSem.id != NULL));

  err = sys_mutex_new(&k64f_enetdata.TXLockMutex);
  LWIP_ASSERT("TXLockMutex creation error", (err == ERR_OK));

  /* Packet receive task */
  err = sys_sem_new(&k64f_enetdata.RxReadySem, 0);
  LWIP_ASSERT("RxReadySem creation error", (err == ERR_OK));
  sys_thread_new("receive_thread", packet_rx, netif->state, DEFAULT_THREAD_STACKSIZE, RX_PRIORITY);

  /* Transmit cleanup task */
  err = sys_sem_new(&k64f_enetdata.TxCleanSem, 0);
  LWIP_ASSERT("TxCleanSem creation error", (err == ERR_OK));
  sys_thread_new("txclean_thread", packet_tx, netif->state, DEFAULT_THREAD_STACKSIZE, TX_PRIORITY);

  /* PHY monitoring task */
  sys_thread_new("phy_thread", k64f_phy_task, netif, DEFAULT_THREAD_STACKSIZE, PHY_PRIORITY);

  /* Allow the PHY task to detect the initial link state and set up the proper flags */
  osDelay(10);

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

