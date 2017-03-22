#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "fsl_phy.h"

#include "k64f_emac_config.h"

#include "mbed_interface.h"
#include "emac_api.h"
#include "emac_stack_mem.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nsapi_types.h"

enet_handle_t g_handle;
// TX Buffer descriptors
uint8_t *tx_desc_start_addr;
// RX Buffer descriptors
uint8_t *rx_desc_start_addr;
// RX packet buffer pointers
emac_stack_mem_t *rx_buff[ENET_RX_RING_LEN];
// TX packet buffer pointers
emac_stack_mem_t *tx_buff[ENET_RX_RING_LEN];
// RX packet payload pointers
uint32_t *rx_ptr[ENET_RX_RING_LEN];

#define K64_ETH_MTU_SIZE 1500
#define K64_ETH_IF_NAME "en"

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
  osSemaphoreId    RxReadySem; /**< RX packet ready semaphore */
  osSemaphoreDef_t RxReadySem_def; /**< RX semaphore: definition struct */
  uint32_t         RxReadySem_data[2]; /**< RX semaphore: object mem */

  osSemaphoreId    TxCleanSem; /**< TX cleanup thread wakeup semaphore */
  osSemaphoreDef_t TxCleanSem_def; /**< TX semaphore: definition struct */
  uint32_t         TxCleanSem_data[2]; /**< TX semaphore: object mem */

  osMutexId TXLockMutex; /**< TX critical section mutex */
  osMutexDef_t TXLockMutex_def; /**< TX mutex: definition struct */
  int32_t TXLockMutex_data[4]; /**< TX mutex: object mem */

  osSemaphoreId    xTXDCountSem; /**< TX free buffer counting semaphore */
  osSemaphoreDef_t xTXDCountSem_def; /**< TX free buffer semaphore: definition struct */
  uint32_t         xTXDCountSem_data[2]; /**< TX free buffer semaphore: object mem */

  osThreadId    RxThread; /**< Packet reception thread */
  osThreadDef_t RxThread_def; /**< Packet reception thread: definition struct */

  osThreadId    TxCleanThread; /**< Packet transmission cleanup thread */
  osThreadDef_t TxCleanThread_def; /**< Packet transmission cleanup thread: definition struct */

  osThreadId    PhyThread; /**< PHY monitoring thread */
  osThreadDef_t PhyThread_def; /**< PHY monitoring thread: definition struct */

  uint8_t tx_consume_index, tx_produce_index; /**< TX buffers ring */
  emac_link_input_fn emac_link_input_cb; /**< Callback for incoming data */
  void *emac_link_input_cb_data; /**< Data to be passed to input cb */
  emac_link_state_change_fn emac_link_state_cb; /**< Link state change callback */
  void *emac_link_state_cb_data; /**< Data to be passed to link state cb */
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
    } else {
        g_handle.rxBdCurrent++;
    }

    /* Actives the receive buffer descriptor. */
    ENET->RDAR = ENET_RDAR_RDAR_MASK;
}

/** \brief  Free TX buffers that are complete
 *
 *  \param[in] k64f  Pointer to driver data structure
 */
static void k64f_tx_reclaim(struct k64f_enetdata *enet)
{
  /* Get exclusive access */
  osMutexWait(enet->TXLockMutex, osWaitForever);

  // Traverse all descriptors, looking for the ones modified by the uDMA
  while((enet->tx_consume_index != enet->tx_produce_index) &&
       (!(g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK))) {
    emac_stack_mem_free(tx_buff[enet->tx_consume_index % ENET_TX_RING_LEN]);
    if (g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
      g_handle.txBdDirty = g_handle.txBdBase;
    else
      g_handle.txBdDirty++;

    enet->tx_consume_index += 1;
    osSemaphoreRelease(enet->xTXDCountSem);
  }

  /* Restore access */
  osMutexRelease(enet->TXLockMutex);
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt of K64F.
 */
void enet_mac_rx_isr(struct k64f_enetdata *enet)
{
  osSemaphoreRelease(enet->RxReadySem);
}

void enet_mac_tx_isr(struct k64f_enetdata *enet)
{
  osSemaphoreRelease(enet->TxCleanSem);
}

void ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
{
  struct k64f_enetdata *enet = param;
  switch (event)
  {
    case kENET_RxEvent:
      enet_mac_rx_isr(enet);
      break;
    case kENET_TxEvent:
      enet_mac_tx_isr(enet);
      break;
    default:
      break;
  }
}

/** \brief  Low level init of the MAC and PHY.
 *
 *  \param[in]      enet       Pointer to K64F enet structure
 *  \param[in]      hwaddr     MAC address
 */
static err_t low_level_init(struct k64f_enetdata *enet, char *hwaddr)
{
  uint8_t i;
  uint32_t sysClock;
  phy_speed_t phy_speed;
  phy_duplex_t phy_duplex;
  uint32_t phyAddr = 0;
  bool link = false;
  enet_config_t config;
  void *payload;

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
    rx_buff[i] = emac_stack_mem_alloc(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT);
    if (NULL == rx_buff[i])
      return ERR_MEM;

    /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
       RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
       a data structure which is internal to lwIP. This might not prove to be a good idea
       in the long run, but a better fix would probably involve modifying lwIP itself */
    payload = emac_stack_mem_ptr(rx_buff[i]);
    payload = (void*)ENET_ALIGN((uint32_t)payload, ENET_BUFF_ALIGNMENT);
    rx_ptr[i] = payload;
  }

  enet->tx_consume_index = enet->tx_produce_index = 0;

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
  ENET_Init(ENET, &g_handle, &config, &buffCfg, (uint8_t*)hwaddr, sysClock);
  ENET_SetCallback(&g_handle, ethernet_callback, enet);
  ENET_ActiveRead(ENET);

  return ERR_OK;
}

/** \brief  Allocates a emac_stack_mem_t and returns the data from the incoming packet.
 *
 *  \param[in] enet  Pointer to K64F enet structure
 *  \param[in] idx   index of packet to be read
 *  \return a emac_stack_mem_t filled with the received packet (including MAC header)
 */
static emac_stack_mem_t *k64f_low_level_input(struct k64f_enetdata *enet, int idx)
{
  volatile enet_rx_bd_struct_t *bdPtr = g_handle.rxBdCurrent;
  emac_stack_mem_t *p = NULL;
  emac_stack_mem_t *temp_rxbuf = NULL;
  u32_t length = 0;
  const u16_t err_mask = ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK | ENET_BUFFDESCRIPTOR_RX_CRC_MASK |
                         ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK | ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK;
void *payload;

#ifdef LOCK_RX_THREAD
  /* Get exclusive access */
  osMutexWait(enet->TXLockMutex, osWaitForever);
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
    emac_stack_mem_set_len(p, length);

    /* Attempt to queue new buffer */
    temp_rxbuf = emac_stack_mem_alloc(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT);
    if (NULL == temp_rxbuf) {
      /* Drop frame (out of memory) */
      LINK_STATS_INC(link.drop);

      /* Re-queue the same buffer */
      update_read_buffer(NULL);

      LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
        ("k64f_low_level_input: Packet index %d dropped for OOM\n",
        idx));
#ifdef LOCK_RX_THREAD
      osMutexRelease(enet->TXLockMutex);
#endif

      return NULL;
    }

    rx_buff[idx] = temp_rxbuf;
    /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
       RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
       a data structure which is internal to lwIP. This might not prove to be a good idea
       in the long run, but a better fix would probably involve modifying lwIP itself */
    payload = emac_stack_mem_ptr(rx_buff[idx]);
    payload = (void*)ENET_ALIGN((uint32_t)payload, ENET_BUFF_ALIGNMENT);
    rx_ptr[idx] = payload;

    update_read_buffer(payload);
    LWIP_DEBUGF(UDP_LPC_EMAC | LWIP_DBG_TRACE,
      ("k64f_low_level_input: Packet received: %p, size %"PRIu32" (index=%d)\n",
      p, length, idx));

    /* Save size */
    emac_stack_mem_set_chain_len(p, length);
    LINK_STATS_INC(link.recv);
  }

#ifdef LOCK_RX_THREAD
  osMutexRelease(enet->TXLockMutex);
#endif

  return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] idx   index of packet to be read
 */
void k64f_enetif_input(struct k64f_enetdata *enet, int idx)
{
  emac_stack_mem_t *p;

  /* move received packet into a new buf */
  p = k64f_low_level_input(enet, idx);
  if (p == NULL)
    return;

  enet->emac_link_input_cb(enet->emac_link_input_cb_data, p);
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void packet_rx(void* pvParameters) {
  struct k64f_enetdata *enet = pvParameters;
  int idx = 0;

  while (1) {
    /* Wait for receive task to wakeup */
    osSemaphoreWait(enet->RxReadySem, osWaitForever);

    while ((g_handle.rxBdCurrent->control & ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK) == 0) {
      k64f_enetif_input(enet, idx);
      idx = (idx + 1) % ENET_RX_RING_LEN;
    }
  }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the buffer and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] pvParameters pointer to the interface data
 */
static void packet_tx(void* pvParameters) {
  struct k64f_enetdata *enet = pvParameters;

  while (1) {
    /* Wait for transmit cleanup task to wakeup */
    osSemaphoreWait(enet->TxCleanSem, osWaitForever);
    k64f_tx_reclaim(enet);
  }
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] emac     Emac driver for the network interface
 *  \param[in] buf      the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
static bool k64f_eth_link_out(emac_interface_t *emac, emac_stack_mem_chain_t *chain)
{
  struct k64f_enetdata *enet = emac->hw;
  emac_stack_mem_t *q;
  emac_stack_mem_t *temp_buf;
  uint8_t *psend = NULL, *dst;

  temp_buf = emac_stack_mem_alloc(emac_stack_mem_chain_len(chain), ENET_BUFF_ALIGNMENT);
  if (NULL == temp_buf)
    return false;

  /* K64F note: the next line ensures that the RX buffer is properly aligned for the K64F
     RX descriptors (16 bytes alignment). However, by doing so, we're effectively changing
     a data structure which is internal to lwIP. This might not prove to be a good idea
     in the long run, but a better fix would probably involve modifying lwIP itself */
  psend = (uint8_t *)ENET_ALIGN((uint32_t)emac_stack_mem_ptr(temp_buf), ENET_BUFF_ALIGNMENT);

  for (q = emac_stack_mem_chain_dequeue(&chain), dst = psend; q != NULL; q = emac_stack_mem_chain_dequeue(&chain)) {
    memcpy(dst, emac_stack_mem_ptr(q), emac_stack_mem_len(q));
    dst += emac_stack_mem_len(q);
  }

  /* Check if a descriptor is available for the transfer. */
  int32_t count = osSemaphoreWait(enet->xTXDCountSem, 0);
  if (count < 1)
    return false;

  /* Get exclusive access */
  osMutexWait(enet->TXLockMutex, osWaitForever);

  /* Save the buffer so that it can be freed when transmit is done */
  tx_buff[enet->tx_produce_index % ENET_TX_RING_LEN] = temp_buf;
  enet->tx_produce_index += 1;

  /* Setup transfers */
  g_handle.txBdCurrent->buffer = psend;
  g_handle.txBdCurrent->length = emac_stack_mem_len(temp_buf);

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
  osMutexRelease(enet->TXLockMutex);

  return true;
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
  struct k64f_enetdata *enet = data;
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
      enet->emac_link_state_cb(enet->emac_link_state_cb_data, crt_state.connected);
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

static bool k64f_eth_power_up(emac_interface_t *emac)
{
  err_t err;

  /* Initialize the hardware */
  err = low_level_init(&k64f_enetdata, emac->hwaddr);
  if (err != ERR_OK)
    return false;

  /* CMSIS-RTOS, start tasks */
  memset(k64f_enetdata.xTXDCountSem_data, 0, sizeof(k64f_enetdata.xTXDCountSem_data));
  k64f_enetdata.xTXDCountSem_def.semaphore = k64f_enetdata.xTXDCountSem_data;
  k64f_enetdata.xTXDCountSem = osSemaphoreCreate(&k64f_enetdata.xTXDCountSem_def, ENET_TX_RING_LEN);
  MBED_ASSERT(k64f_enetdata.xTXDCountSem != NULL);

  /* Transmission lock mutex */
  memset(k64f_enetdata.TXLockMutex_data, 0, sizeof(k64f_enetdata.TXLockMutex_data));
  k64f_enetdata.TXLockMutex_def.mutex = k64f_enetdata.TXLockMutex_data;
  k64f_enetdata.TXLockMutex = osMutexCreate(&k64f_enetdata.TXLockMutex_def);
  MBED_ASSERT(k64f_enetdata.TXLockMutex != NULL);

  /* Packet receive task */
  memset(k64f_enetdata.RxReadySem_data, 0, sizeof(k64f_enetdata.RxReadySem_data));
  k64f_enetdata.RxReadySem_def.semaphore = k64f_enetdata.RxReadySem_data;
  k64f_enetdata.RxReadySem = osSemaphoreCreate(&k64f_enetdata.RxReadySem_def, 0);
  MBED_ASSERT(k64f_enetdata.RxReadySem != NULL);

  /* Packet reception thread */
  k64f_enetdata.RxThread_def.pthread = (os_pthread)packet_rx;
  k64f_enetdata.RxThread_def.tpriority = RX_PRIORITY;
#ifdef LWIP_DEBUG
  k64f_enetdata.RxThread_def.stacksize = DEFAULT_THREAD_STACKSIZE*5;
#else
  k64f_enetdata.RxThread_def.stacksize = DEFAULT_THREAD_STACKSIZE;
#endif
  k64f_enetdata.RxThread_def.stack_pointer = (uint32_t*)malloc(k64f_enetdata.RxThread_def.stacksize);
  if (k64f_enetdata.RxThread_def.stack_pointer == NULL)
    error("RxThread: Error allocating the stack memory");
  k64f_enetdata.RxThread = osThreadCreate(&k64f_enetdata.RxThread_def, &k64f_enetdata);
  if (k64f_enetdata.RxThread == NULL)
    error("RxThread: create error\n");

  /* Transmit cleanup task */
  memset(k64f_enetdata.TxCleanSem_data, 0, sizeof(k64f_enetdata.TxCleanSem_data));
  k64f_enetdata.TxCleanSem_def.semaphore = k64f_enetdata.TxCleanSem_data;
  k64f_enetdata.TxCleanSem = osSemaphoreCreate(&k64f_enetdata.TxCleanSem_def, 0);
  MBED_ASSERT(k64f_enetdata.TxCleanSem != NULL);

  /* Transmission cleanup thread */
  k64f_enetdata.TxCleanThread_def.pthread = (os_pthread)packet_tx;
  k64f_enetdata.TxCleanThread_def.tpriority = TX_PRIORITY;
  k64f_enetdata.TxCleanThread_def.stacksize = DEFAULT_THREAD_STACKSIZE;
  k64f_enetdata.TxCleanThread_def.stack_pointer = (uint32_t*)malloc(k64f_enetdata.TxCleanThread_def.stacksize);
  if (k64f_enetdata.TxCleanThread_def.stack_pointer == NULL)
    error("TxCleanThread: Error allocating the stack memory");
  k64f_enetdata.TxCleanThread = osThreadCreate(&k64f_enetdata.TxCleanThread_def, &k64f_enetdata);
  if (k64f_enetdata.TxCleanThread == NULL)
    error("TxCleanThread: create error\n");

  /* PHY monitoring task */
  k64f_enetdata.PhyThread_def.pthread = (os_pthread)k64f_phy_task;
  k64f_enetdata.PhyThread_def.tpriority = PHY_PRIORITY;
  k64f_enetdata.PhyThread_def.stacksize = DEFAULT_THREAD_STACKSIZE;
  k64f_enetdata.PhyThread_def.stack_pointer = (uint32_t*)malloc(k64f_enetdata.PhyThread_def.stacksize);
  if (k64f_enetdata.PhyThread_def.stack_pointer == NULL)
    error("PhyThread: Error allocating the stack memory");
  k64f_enetdata.PhyThread = osThreadCreate(&k64f_enetdata.PhyThread_def, &k64f_enetdata);
  if (k64f_enetdata.PhyThread == NULL)
    error("PhyThread: create error\n");

  /* Allow the PHY task to detect the initial link state and set up the proper flags */
  osDelay(10);

  return true;
}

static uint32_t k64f_eth_get_mtu_size(emac_interface_t *emac)
{
  return K64_ETH_MTU_SIZE;
}

static void k64f_eth_get_ifname(emac_interface_t *emac, char *name, uint8_t size)
{
  memcpy(name, K64_ETH_IF_NAME, (size < sizeof(K64_ETH_IF_NAME)) ? size : sizeof(K64_ETH_IF_NAME));
}

static uint8_t k64f_eth_get_hwaddr_size(emac_interface_t *emac)
{
    return NSAPI_MAC_BYTES;
}

static void k64f_eth_get_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
  mbed_mac_address((char*)addr);
}

static void k64f_eth_set_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
  /* No-op at this stage */
}

static void k64f_eth_set_link_input_cb(emac_interface_t *emac, emac_link_input_fn input_cb, void *data)
{
  struct k64f_enetdata *enet = emac->hw;

  enet->emac_link_input_cb = input_cb;
  enet->emac_link_input_cb_data = data;
}

static void k64f_eth_set_link_state_cb(emac_interface_t *emac, emac_link_state_change_fn state_cb, void *data)
{
  struct k64f_enetdata *enet = emac->hw;

  enet->emac_link_state_cb = state_cb;
  enet->emac_link_state_cb_data = data;
}

static void k64f_eth_add_multicast_group(emac_interface_t *emac, uint8_t *addr)
{
  ENET_AddMulticastGroup(ENET, addr);
}

static void k64f_eth_power_down(emac_interface_t *emac)
{
  /* No-op at this stage */
}


const emac_interface_ops_t k64f_eth_emac_ops = {
    .get_mtu_size = k64f_eth_get_mtu_size,
    .get_ifname = k64f_eth_get_ifname,
    .get_hwaddr_size = k64f_eth_get_hwaddr_size,
    .get_hwaddr = k64f_eth_get_hwaddr,
    .set_hwaddr = k64f_eth_set_hwaddr,
    .link_out = k64f_eth_link_out,
    .power_up = k64f_eth_power_up,
    .power_down = k64f_eth_power_down,
    .set_link_input_cb = k64f_eth_set_link_input_cb,
    .set_link_state_cb = k64f_eth_set_link_state_cb,
    .add_multicast_group = k64f_eth_add_multicast_group
};

emac_interface_t mbed_emac_eth_default = {&k64f_eth_emac_ops, &k64f_enetdata};

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

