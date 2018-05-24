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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"

#include "fsl_phy.h"

#include "lpc546xx_emac_config.h"
#include "lpc546xx_emac.h"

enet_handle_t g_handle;
// RX packet buffer pointers
emac_mem_buf_t *rx_buff[ENET_RX_RING_LEN];
// TX packet buffer pointers
emac_mem_buf_t *tx_buff[ENET_TX_RING_LEN];
// RX packet payload pointers
uint32_t rx_ptr[ENET_RX_RING_LEN];

/********************************************************************************
 * Internal data
 ********************************************************************************/
#define ENET_BuffSizeAlign(n) ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_ALIGN(x,align)   ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)- 1)))

extern "C" void lpc546xx_init_eth_hardware(void);

/* \brief Flags for worker thread */
#define FLAG_TX  1
#define FLAG_RX  2

/** \brief  Driver thread priority */
#define THREAD_PRIORITY (osPriorityNormal)

#define PHY_TASK_PERIOD_MS      200

LPC546XX_EMAC::LPC546XX_EMAC() : xTXDCountSem(ENET_TX_RING_LEN, ENET_TX_RING_LEN), hwaddr()
{
}

static osThreadId_t create_new_thread(const char *threadName, void (*thread)(void *arg), void *arg, int stacksize, osPriority_t priority, os_thread_t *thread_cb)
{
    osThreadAttr_t attr = {0};
    attr.name = threadName;
    attr.stack_mem  = malloc(stacksize);
    attr.cb_mem  = thread_cb;
    attr.stack_size = stacksize;
    attr.cb_size = sizeof(os_thread_t);
    attr.priority = priority;
    return osThreadNew(thread, arg, &attr);
}

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
 */
void LPC546XX_EMAC::tx_reclaim()
{
    /* Get exclusive access */
    TXLockMutex.lock();

    static uint8_t consume_index = 0;
    enet_tx_bd_ring_t *txBdRing = (enet_tx_bd_ring_t *)&g_handle.txBdRing[0];

    while (consume_index != txBdRing->txConsumIdx) {
        /* Free the transmit buffer */
        memory_manager->free(tx_buff[consume_index]);
        tx_buff[consume_index] = NULL;

        xTXDCountSem.release();

        consume_index++;
        if (consume_index >= ENET_TX_RING_LEN) {
            consume_index = 0;
        }
    }
    /* Restore access */
    TXLockMutex.unlock();
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt of LPC546XX.
 */
void LPC546XX_EMAC::rx_isr()
{
    osThreadFlagsSet(thread, FLAG_RX);
}

void LPC546XX_EMAC::tx_isr()
{
    osThreadFlagsSet(thread, FLAG_TX);
}

void LPC546XX_EMAC::ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, uint8_t channel, void *param)
{
    LPC546XX_EMAC *enet = static_cast<LPC546XX_EMAC *>(param);

    switch (event)
    {
        case kENET_RxIntEvent:
            enet->rx_isr();
            break;
        case kENET_TxIntEvent:
            enet->tx_isr();
            break;
        default:
            break;
    }
}

/** \brief  Low level init of the MAC and PHY.
 */
bool LPC546XX_EMAC::low_level_init_successful()
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
        return false;
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
        return false;
    }

    AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rx_desc_start_addr[ENET_RX_RING_LEN], ENET_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t tx_desc_start_addr[ENET_TX_RING_LEN], ENET_BUFF_ALIGNMENT);

    /* prepare the buffer configuration. */
   enet_buffer_config_t buffCfg = {
       ENET_RX_RING_LEN,
       ENET_TX_RING_LEN,
       &tx_desc_start_addr[0],
       &tx_desc_start_addr[0],
       &rx_desc_start_addr[0],
       &rx_desc_start_addr[ENET_RX_RING_LEN],
       rx_ptr,
       ENET_BuffSizeAlign(ENET_ETH_MAX_FLEN),
   };

    /* Create buffers for each receive BD */
    for (i = 0; i < ENET_RX_RING_LEN; i++) {
        rx_buff[i] = memory_manager->alloc_heap(buffCfg.rxBuffSizeAlign, ENET_BUFF_ALIGNMENT);
        if (NULL == rx_buff[i])
            return false;

        rx_ptr[i] = (uint32_t)memory_manager->get_ptr(rx_buff[i]);
    }

    ENET_Init(ENET, &config, hwaddr, refClock);

    /* Enable the tx & rx interrupt. */
    ENET_EnableInterrupts(ENET, kENET_DmaTx | kENET_DmaRx);

    /* Create the handler. */
    ENET_CreateHandler(ENET, &g_handle, &config, &buffCfg, &LPC546XX_EMAC::ethernet_callback, this);

    /* Initialize Descriptor. */
    ENET_DescriptorInit(ENET, &config, &buffCfg);

    return true;
}

/** \brief  Allocates a emac_mem_buf_t and returns the data from the incoming packet.
 *
 *  \return a emac_mem_buf_t filled with the received packet (including MAC header)
 *         NULL on memory error
 */
emac_mem_buf_t *LPC546XX_EMAC::low_level_input()
{
    enet_rx_bd_ring_t *rxBdRing = (enet_rx_bd_ring_t *)&g_handle.rxBdRing[0];
    enet_rx_bd_struct_t *bdPtr = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
    emac_mem_buf_t *p = NULL;
    emac_mem_buf_t *temp_rxbuf = NULL;
    uint32_t length = 0;

    /* Determine if a frame has been received */
    if ((bdPtr->control & ENET_RXDESCRIP_WR_OWN_MASK) != 0) {
        return p;
    }

#ifdef LOCK_RX_THREAD
    /* Get exclusive access */
    TXLockMutex.lock();
#endif

    /* Determine if the received frame has an error */
    if ((bdPtr->control & ENET_RXDESCRIP_WR_ERRSUM_MASK) != 0) {
        /* Re-use the same buffer in case of error */
        update_read_buffer(bdPtr, NULL);
    } else {
        if (bdPtr->control & ENET_RXDESCRIP_WR_LD_MASK) {
            length = (bdPtr->control & ENET_RXDESCRIP_WR_PACKETLEN_MASK) - 4;
        } else {
            length = rxBdRing->rxBuffSizeAlign;
        }

        /* Zero-copy */
        p = rx_buff[rxBdRing->rxGenIdx];
        memory_manager->set_len(p, length);

        /* Attempt to queue new buffer */
        temp_rxbuf = memory_manager->alloc_heap(rxBdRing->rxBuffSizeAlign, ENET_BUFF_ALIGNMENT);
        if (NULL == temp_rxbuf) {
            /* Re-queue the same buffer */
            update_read_buffer(bdPtr, NULL);

#ifdef LOCK_RX_THREAD
            TXLockMutex.unlock();
#endif

            return NULL;
        }

        rx_buff[rxBdRing->rxGenIdx] = temp_rxbuf;
        rx_ptr[rxBdRing->rxGenIdx] = (uint32_t)memory_manager->get_ptr(rx_buff[rxBdRing->rxGenIdx]);

        update_read_buffer(bdPtr, (uint8_t*)rx_ptr[rxBdRing->rxGenIdx]);
    }

#ifdef LOCK_RX_THREAD
    osMutexRelease(TXLockMutex);
#endif

    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 */
void LPC546XX_EMAC::input()
{
    emac_mem_buf_t *p;

    /* move received packet into a new pbuf */
    p = low_level_input();
    if (p == NULL) {
        return;
    }

    emac_link_input_cb(p);
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
void LPC546XX_EMAC::thread_function(void* pvParameters)
{
    struct LPC546XX_EMAC *lpc_enet = static_cast<LPC546XX_EMAC *>(pvParameters);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX | FLAG_TX, osFlagsWaitAny, osWaitForever);

        MBED_ASSERT(!(flags & osFlagsError));

        if (flags & FLAG_RX) {
            lpc_enet->packet_rx();
        }

        if (flags & FLAG_TX) {
            lpc_enet->packet_tx();
        }
    }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 */
void LPC546XX_EMAC::packet_rx()
{
    enet_rx_bd_ring_t *rxBdRing = (enet_rx_bd_ring_t *)&g_handle.rxBdRing[0];
    enet_rx_bd_struct_t *bdPtr = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
    bool suspend = false;

    /* Suspend and command for rx. */
    if (ENET->DMA_CH[0].DMA_CHX_STAT & ENET_DMA_CH_DMA_CHX_STAT_RBU_MASK) {
        suspend = true;
    }

    while ((bdPtr->control & ENET_RXDESCRIP_WR_OWN_MASK) == 0) {
        input();
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
 * reclaims the buffer and descriptor used for the packet once
 * the packet has been transferred.
 */
void LPC546XX_EMAC::packet_tx()
{
    tx_reclaim();
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] buf the MAC packet to send (e.g. IP packet including MAC addresses and type)
 */
bool LPC546XX_EMAC::link_out(emac_mem_buf_t *buf)
{
    enet_tx_bd_ring_t *txBdRing = (enet_tx_bd_ring_t *)&g_handle.txBdRing[0];
    uint32_t index = txBdRing->txGenIdx;

    // If buffer is chained or not aligned then make a contiguous aligned copy of it
    if (memory_manager->get_next(buf) ||
        reinterpret_cast<uint32_t>(memory_manager->get_ptr(buf)) % ENET_BUFF_ALIGNMENT) {
        emac_mem_buf_t *copy_buf;
        copy_buf = memory_manager->alloc_heap(memory_manager->get_total_len(buf), ENET_BUFF_ALIGNMENT);
        if (NULL == copy_buf) {
            memory_manager->free(buf);
            return false;
        }

        // Copy to new buffer and free original
        memory_manager->copy(copy_buf, buf);
        memory_manager->free(buf);
        buf = copy_buf;
    }

    /* Check if a descriptor is available for the transfer. */
    if (xTXDCountSem.wait(0) == 0) {
        memory_manager->free(buf);
        return false;
    }

    /* Get exclusive access */
    TXLockMutex.lock();

    /* Save the buffer so that it can be freed when transmit is done */
    tx_buff[index] = buf;

    if (ENET_SendFrame(ENET, &g_handle, static_cast<uint8_t *>(memory_manager->get_ptr(buf)), memory_manager->get_len(buf)) != kStatus_Success) {
        memory_manager->free(buf);
        tx_buff[index] = NULL;
        return false;
    }

    /* Restore access */
    TXLockMutex.unlock();

    return true;
}

/*******************************************************************************
 * PHY task: monitor link
*******************************************************************************/

#define STATE_UNKNOWN           (-1)

int phy_link_status(void) {
    bool connection_status;
    uint32_t phyAddr = 0;

    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);
    return (int)connection_status;
}

void LPC546XX_EMAC::phy_task()
{
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
    if (crt_state.connected != prev_state.connected && emac_link_state_cb) {
        emac_link_state_cb(crt_state.connected);
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

bool LPC546XX_EMAC::power_up()
{
    /* Initialize the hardware */
    if (!low_level_init_successful()) {
        return false;
    }

    /* Worker thread */
    thread = create_new_thread("LPC546XX_EMAC_thread", &LPC546XX_EMAC::thread_function, this, THREAD_STACKSIZE, THREAD_PRIORITY, &thread_cb);

    /* Trigger thread to deal with any RX packets that arrived before thread was started */
    rx_isr();

    /* PHY monitoring task */
    prev_state.connected = STATE_UNKNOWN;
    prev_state.speed = (phy_speed_t)STATE_UNKNOWN;
    prev_state.duplex = (phy_duplex_t)STATE_UNKNOWN;

    phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD_MS, mbed::callback(this, &LPC546XX_EMAC::phy_task));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    /* Active TX/RX. */
    ENET_StartRxTx(ENET, 1, 1);

    return true;
}

uint32_t LPC546XX_EMAC::get_mtu_size() const
{
    return LPC546XX_ETH_MTU_SIZE;
}

uint32_t LPC546XX_EMAC::get_align_preference() const
{
    return ENET_BUFF_ALIGNMENT;
}

void LPC546XX_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, LPC546XX_ETH_IF_NAME, (size < sizeof(LPC546XX_ETH_IF_NAME)) ? size : sizeof(LPC546XX_ETH_IF_NAME));
}

uint8_t LPC546XX_EMAC::get_hwaddr_size() const
{
    return LPC546XX_HWADDR_SIZE;
}

bool LPC546XX_EMAC::get_hwaddr(uint8_t *addr) const
{
    return false;
}

void LPC546XX_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(hwaddr, addr, sizeof hwaddr);
    ENET_SetMacAddr(ENET, const_cast<uint8_t*>(addr));
}

void LPC546XX_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void LPC546XX_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void LPC546XX_EMAC::add_multicast_group(const uint8_t *addr)
{
    set_all_multicast(true);
}

void LPC546XX_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void LPC546XX_EMAC::set_all_multicast(bool all)
{
    uint32_t reg = ENET->MAC_FRAME_FILTER;

    if (all) {
        reg |= ENET_MAC_FRAME_FILTER_PM_MASK;
    } else {
        reg &= ~ENET_MAC_FRAME_FILTER_PM_MASK;
    }

    ENET->MAC_FRAME_FILTER = reg;
}

void LPC546XX_EMAC::power_down()
{
    /* No-op at this stage */
}

void LPC546XX_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}


LPC546XX_EMAC &LPC546XX_EMAC::get_instance() {
    static LPC546XX_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return LPC546XX_EMAC::get_instance();
}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

