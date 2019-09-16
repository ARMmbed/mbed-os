/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright (c) 2017 ARM Limited
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include "kinetis_emac_config.h"
#include "kinetis_emac.h"
#include "mbed_power_mgmt.h"

enet_handle_t g_handle;
// TX Buffer descriptors
uint8_t *tx_desc_start_addr;
// RX Buffer descriptors
uint8_t *rx_desc_start_addr;
// RX packet buffer pointers
emac_mem_buf_t *rx_buff[ENET_RX_RING_LEN];
// TX packet buffer pointers
emac_mem_buf_t *tx_buff[ENET_TX_RING_LEN];
// RX packet payload pointers
uint32_t *rx_ptr[ENET_RX_RING_LEN];

/********************************************************************************
 * Internal data
 ********************************************************************************/
#define ENET_BuffSizeAlign(n) ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_ALIGN(x,align)   ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)- 1)))

extern "C" void kinetis_init_eth_hardware(void);

/* \brief Flags for worker thread */
#define FLAG_TX  1
#define FLAG_RX  2

/** \brief  Driver thread priority */
#define THREAD_PRIORITY (osPriorityNormal)

#define PHY_TASK_PERIOD_MS      200

Kinetis_EMAC::Kinetis_EMAC() : xTXDCountSem(ENET_TX_RING_LEN, ENET_TX_RING_LEN), hwaddr()
{
}

static osThreadId_t create_new_thread(const char *threadName, void (*thread)(void *arg), void *arg, int stacksize, osPriority_t priority, mbed_rtos_storage_thread_t *thread_cb)
{
    osThreadAttr_t attr = {0};
    attr.name = threadName;
    attr.stack_mem  = malloc(stacksize);
    attr.cb_mem  = thread_cb;
    attr.stack_size = stacksize;
    attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
    attr.priority = priority;
    return osThreadNew(thread, arg, &attr);
}
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
 */
void Kinetis_EMAC::tx_reclaim()
{
  /* Get exclusive access */
  TXLockMutex.lock();

  // Traverse all descriptors, looking for the ones modified by the uDMA
  while((tx_consume_index != tx_produce_index) &&
        (!(g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK))) {
      memory_manager->free(tx_buff[tx_consume_index % ENET_TX_RING_LEN]);
      if (g_handle.txBdDirty->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
        g_handle.txBdDirty = g_handle.txBdBase;
      else
        g_handle.txBdDirty++;

      tx_consume_index += 1;
      xTXDCountSem.release();
  }

  /* Restore access */
  TXLockMutex.unlock();
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt.
 */
void Kinetis_EMAC::rx_isr()
{
    if (thread) {
        osThreadFlagsSet(thread, FLAG_RX);
    }
}

void Kinetis_EMAC::tx_isr()
{
    osThreadFlagsSet(thread, FLAG_TX);
}

void Kinetis_EMAC::ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
{
    Kinetis_EMAC *enet = static_cast<Kinetis_EMAC *>(param);
    switch (event)
    {
      case kENET_RxEvent:
        enet->rx_isr();
        break;
      case kENET_TxEvent:
        enet->tx_isr();
        break;
      default:
        break;
    }
}


/** \brief  Low level init of the MAC and PHY.
 */
bool Kinetis_EMAC::low_level_init_successful()
{
    uint8_t i;
    uint32_t sysClock;
    phy_speed_t phy_speed;
    phy_duplex_t phy_duplex;
    uint32_t phyAddr = 0;
    enet_config_t config;

    // Allocate RX descriptors
    rx_desc_start_addr = (uint8_t *)calloc(1, sizeof(enet_rx_bd_struct_t) * ENET_RX_RING_LEN + ENET_BUFF_ALIGNMENT);
    if(!rx_desc_start_addr)
        return false;

    // Allocate TX descriptors
    tx_desc_start_addr = (uint8_t *)calloc(1, sizeof(enet_tx_bd_struct_t) * ENET_TX_RING_LEN + ENET_BUFF_ALIGNMENT);
    if(!tx_desc_start_addr)
        return false;

    rx_desc_start_addr = (uint8_t *)ENET_ALIGN(rx_desc_start_addr, ENET_BUFF_ALIGNMENT);
    tx_desc_start_addr = (uint8_t *)ENET_ALIGN(tx_desc_start_addr, ENET_BUFF_ALIGNMENT);

    /* Create buffers for each receive BD */
    for (i = 0; i < ENET_RX_RING_LEN; i++) {
        rx_buff[i] = memory_manager->alloc_heap(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT);
        if (NULL == rx_buff[i])
            return false;

        rx_ptr[i] = (uint32_t*)memory_manager->get_ptr(rx_buff[i]);
    }

    tx_consume_index = tx_produce_index = 0;

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

    kinetis_init_eth_hardware();

    sysClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    ENET_GetDefaultConfig(&config);

    if (PHY_Init(ENET, phyAddr, sysClock) != kStatus_Success) {
        return false;
    }

    /* Get link information from PHY */
    PHY_GetLinkSpeedDuplex(ENET, phyAddr, &phy_speed, &phy_duplex);
    /* Change the MII speed and duplex for actual link status. */
    config.miiSpeed = (enet_mii_speed_t)phy_speed;
    config.miiDuplex = (enet_mii_duplex_t)phy_duplex;
    config.interrupt = kENET_RxFrameInterrupt | kENET_TxFrameInterrupt;
    config.rxMaxFrameLen = ENET_ETH_MAX_FLEN;
    config.macSpecialConfig = kENET_ControlFlowControlEnable;
    config.txAccelerConfig = 0;
    config.rxAccelerConfig = kENET_RxAccelMacCheckEnabled;
    ENET_Init(ENET, &g_handle, &config, &buffCfg, hwaddr, sysClock);

#if defined(TOOLCHAIN_ARM)
#if defined(__OPTIMISE_TIME) && (__ARMCC_VERSION < 5060750)
    /* Add multicast groups
       work around for https://github.com/ARMmbed/mbed-os/issues/4372 */
    ENET->GAUR = 0xFFFFFFFFu;
    ENET->GALR = 0xFFFFFFFFu;
#endif
#endif

    ENET_SetCallback(&g_handle, &Kinetis_EMAC::ethernet_callback, this);
    ENET_ActiveRead(ENET);

    return true;
}

/** \brief  Allocates a emac_mem_buf_t and returns the data from the incoming packet.
 *
 *  \param[in] idx   index of packet to be read
 *  \return a emac_mem_buf_t filled with the received packet (including MAC header)
 */
emac_mem_buf_t *Kinetis_EMAC::low_level_input(int idx)
{
    volatile enet_rx_bd_struct_t *bdPtr = g_handle.rxBdCurrent;
    emac_mem_buf_t *p = NULL;
    emac_mem_buf_t *temp_rxbuf = NULL;
    uint32_t length = 0;
    const uint16_t err_mask = ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK | ENET_BUFFDESCRIPTOR_RX_CRC_MASK |
                              ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK | ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK;

#ifdef LOCK_RX_THREAD
    /* Get exclusive access */
    TXLockMutex.lock();
#endif

    /* Determine if a frame has been received */
    if ((bdPtr->control & err_mask) != 0) {
        /* Re-use the same buffer in case of error */
        update_read_buffer(NULL);
    } else {
        /* A packet is waiting, get length */
        length = bdPtr->length;

        /* Zero-copy */
        p = rx_buff[idx];
        memory_manager->set_len(p, length);

        /* Attempt to queue new buffer */
        temp_rxbuf = memory_manager->alloc_heap(ENET_ETH_MAX_FLEN, ENET_BUFF_ALIGNMENT);
        if (NULL == temp_rxbuf) {
            /* Re-queue the same buffer */
            update_read_buffer(NULL);

#ifdef LOCK_RX_THREAD
      TXLockMutex.unlock();
#endif

            return NULL;
        }

        rx_buff[idx] = temp_rxbuf;
        rx_ptr[idx] = (uint32_t*)memory_manager->get_ptr(rx_buff[idx]);

        update_read_buffer((uint8_t*)rx_ptr[idx]);
    }

#ifdef LOCK_RX_THREAD
    osMutexRelease(TXLockMutex);
#endif

    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] idx   index of packet to be read
 */
void Kinetis_EMAC::input(int idx)
{
    emac_mem_buf_t *p;

    /* move received packet into a new buf */
    p = low_level_input(idx);
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
void Kinetis_EMAC::thread_function(void* pvParameters)
{
    struct Kinetis_EMAC *kinetis_enet = static_cast<Kinetis_EMAC *>(pvParameters);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX|FLAG_TX, osFlagsWaitAny, osWaitForever);

        MBED_ASSERT(!(flags & osFlagsError));

        if (flags & FLAG_RX) {
            kinetis_enet->packet_rx();
        }

        if (flags & FLAG_TX) {
            kinetis_enet->packet_tx();
        }
    }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 */
void Kinetis_EMAC::packet_rx()
{
    static int idx = 0;

    while ((g_handle.rxBdCurrent->control & ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK) == 0) {
        input(idx);
        idx = (idx + 1) % ENET_RX_RING_LEN;
    }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the buffer and descriptor used for the packet once
 * the packet has been transferred.
 */
void Kinetis_EMAC::packet_tx()
{
    tx_reclaim();
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] buf      the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
bool Kinetis_EMAC::link_out(emac_mem_buf_t *buf)
{
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

    /* Check if a descriptor is available for the transfer (wait 10ms before dropping the buffer) */
    if (!xTXDCountSem.try_acquire_for(10)) {
        memory_manager->free(buf);
        return false;
    }

    /* Get exclusive access */
    TXLockMutex.lock();

    /* Save the buffer so that it can be freed when transmit is done */
    tx_buff[tx_produce_index % ENET_TX_RING_LEN] = buf;
    tx_produce_index += 1;

    /* Setup transfers */
    g_handle.txBdCurrent->buffer = static_cast<uint8_t *>(memory_manager->get_ptr(buf));
    g_handle.txBdCurrent->length = memory_manager->get_len(buf);
    g_handle.txBdCurrent->control |= (ENET_BUFFDESCRIPTOR_TX_READY_MASK | ENET_BUFFDESCRIPTOR_TX_LAST_MASK);

    /* Increase the buffer descriptor address. */
    if (g_handle.txBdCurrent->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK) {
        g_handle.txBdCurrent = g_handle.txBdBase;
    } else {
        g_handle.txBdCurrent++;
    }

    /* Active the transmit buffer descriptor. */
    ENET->TDAR = ENET_TDAR_TDAR_MASK;

    /* Restore access */
    TXLockMutex.unlock();

    return true;
}

/*******************************************************************************
 * PHY task: monitor link
*******************************************************************************/

#define STATE_UNKNOWN           (-1)
#define STATE_LINK_DOWN         (0)
#define STATE_LINK_UP           (1)

void Kinetis_EMAC::phy_task()
{
    uint32_t phyAddr = 0;

    // Get current status
    PHY_STATE crt_state;
    bool connection_status;
    PHY_GetLinkStatus(ENET, phyAddr, &connection_status);

    if (connection_status) {
        crt_state.connected = STATE_LINK_UP;
    } else {
        crt_state.connected = STATE_LINK_DOWN;
    }

    if (crt_state.connected == STATE_LINK_UP) {
        if (prev_state.connected != STATE_LINK_UP) {
            PHY_AutoNegotiation(ENET, phyAddr);
        }

        PHY_GetLinkSpeedDuplex(ENET, phyAddr, &crt_state.speed, &crt_state.duplex);

        if (prev_state.connected != STATE_LINK_UP || crt_state.speed != prev_state.speed) {
            /* Poke the registers*/
            ENET_SetMII(ENET, (enet_mii_speed_t)crt_state.speed, (enet_mii_duplex_t)crt_state.duplex);
        }
    }

    // Compare with previous state
    if (crt_state.connected != prev_state.connected && emac_link_state_cb) {
        emac_link_state_cb(crt_state.connected);
    }

    prev_state = crt_state;
}

bool Kinetis_EMAC::power_up()
{
    /* Initialize the hardware */
    if (!low_level_init_successful()) {
        return false;
    }

    // Can't enter deep sleep as long as Ethernet is active
    sleep_manager_lock_deep_sleep();

    /* Worker thread */
    thread = create_new_thread("Kinetis_EMAC_thread", &Kinetis_EMAC::thread_function, this, THREAD_STACKSIZE, THREAD_PRIORITY, &thread_cb);

    /* Trigger thread to deal with any RX packets that arrived before thread was started */
    rx_isr();

    /* PHY monitoring task */
    prev_state.connected = STATE_LINK_DOWN;
    prev_state.speed = (phy_speed_t)STATE_UNKNOWN;
    prev_state.duplex = (phy_duplex_t)STATE_UNKNOWN;

    mbed::mbed_event_queue()->call(mbed::callback(this, &Kinetis_EMAC::phy_task));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD_MS, mbed::callback(this, &Kinetis_EMAC::phy_task));

    return true;
}

uint32_t Kinetis_EMAC::get_mtu_size() const
{
    return KINETIS_ETH_MTU_SIZE;
}

uint32_t Kinetis_EMAC::get_align_preference() const
{
    return ENET_BUFF_ALIGNMENT;
}

void Kinetis_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, KINETIS_ETH_IF_NAME, (size < sizeof(KINETIS_ETH_IF_NAME)) ? size : sizeof(KINETIS_ETH_IF_NAME));
}

uint8_t Kinetis_EMAC::get_hwaddr_size() const
{
    return KINETIS_HWADDR_SIZE;
}

bool Kinetis_EMAC::get_hwaddr(uint8_t *addr) const
{
    return false;
}

void Kinetis_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(hwaddr, addr, sizeof hwaddr);
    ENET_SetMacAddr(ENET, const_cast<uint8_t*>(addr));
}

void Kinetis_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void Kinetis_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void Kinetis_EMAC::add_multicast_group(const uint8_t *addr)
{
    ENET_AddMulticastGroup(ENET, const_cast<uint8_t *>(addr));
}

void Kinetis_EMAC::remove_multicast_group(const uint8_t *addr)
{
    // ENET HAL doesn't reference count - ENET_LeaveMulticastGroup just maps
    // address to filter bit, and clears that bit, even if shared by other
    // addresses. So don't attempt anything for now.
}

void Kinetis_EMAC::set_all_multicast(bool all)
{
    if (all) {
        ENET->GAUR = 0xFFFFFFFFu;
        ENET->GALR = 0xFFFFFFFFu;
    }
}

void Kinetis_EMAC::power_down()
{
    // Ethernet went down, can enter deep sleep
    sleep_manager_unlock_deep_sleep();
}

void Kinetis_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}


Kinetis_EMAC &Kinetis_EMAC::get_instance() {
    static Kinetis_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return Kinetis_EMAC::get_instance();
}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

