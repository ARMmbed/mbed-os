/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "mbed_shared_queues.h"
#include "netsocket/nsapi_types.h"

#include "gd32xx_emac.h"

/* \brief Flags for worker thread */
#define _ENET_FLAG_RX                       (1)

/** \brief  Driver thread priority */
#define _THREAD_STACKSIZE                   (512)
#define _THREAD_PRIORITY                    (osPriorityHigh)

#define _PHY_TASK_PERIOD_MS                 (200)

#define _ENET_HW_ADDR_SIZE                  (6)
#define _ENET_MTU_SIZE                      (1500)
#define _ENET_IF_NAME                       "gd"

#define _ENET_BOARD_PHY_ADDRESS             (0x01)
#define _ENET_HARDWARE_CHECKSUM             (0)

#define _GD_MAC_ADDR0                       0x02
#define _GD_MAC_ADDR1                       0xaa
#define _GD_MAC_ADDR2                       0xbb
#define _GD32_ID_ADDR                       0x1FFFF7E8
/* ENET RxDMA/TxDMA descriptor */
extern enet_descriptors_struct  rxdesc_tab[ENET_RXBUF_NUM], txdesc_tab[ENET_TXBUF_NUM];
/* ENET receive buffer  */
extern uint8_t rx_buff[ENET_RXBUF_NUM][ENET_RXBUF_SIZE];
/* ENET transmit buffer */
extern uint8_t tx_buff[ENET_TXBUF_NUM][ENET_TXBUF_SIZE];
/*global transmit and receive descriptors pointers */
extern enet_descriptors_struct  *dma_current_txdesc;
extern enet_descriptors_struct  *dma_current_rxdesc;

#ifdef __cplusplus
extern "C" {
#endif

void ENET_IRQHandler(void);
void enet_bsp_init(void);
#ifdef __cplusplus
}
#endif

/**
 * Ethernet IRQ Handler
 *
 */
void ENET_IRQHandler(void)
{
    /* frame received */
    if (SET == enet_interrupt_flag_get(ENET_DMA_INT_FLAG_RS)) {
        /* clear the enet DMA Rx interrupt pending bits */
        enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_RS_CLR);
        enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_NI_CLR);
        /* Ethernet Rx Transfer completed callback */
        GD32_EMAC &emac = GD32_EMAC::get_instance();
        if (emac.rx_thread) {
            osThreadFlagsSet(emac.rx_thread, _ENET_FLAG_RX);
        }
    }
}

GD32_EMAC::GD32_EMAC()
    : rx_thread(0),
      phy_status(0)
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

/** \brief  Low level init of the MAC and PHY.
 *
 */
bool GD32_EMAC::low_level_init()
{
    /* Init ETH */
    uint8_t macaddr[6];
    uint32_t i;

#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    MACAddr[0] = MBED_MAC_ADDR_0;
    MACAddr[1] = MBED_MAC_ADDR_1;
    MACAddr[2] = MBED_MAC_ADDR_2;
    MACAddr[3] = MBED_MAC_ADDR_3;
    MACAddr[4] = MBED_MAC_ADDR_4;
    MACAddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)macaddr);
#endif

    enet_bsp_init();
    /* reset ethernet on AHB bus */
    enet_deinit();

    if (ERROR == enet_software_reset()) {
        while (1);
    }

#if (1 == _ENET_HARDWARE_CHECKSUM)
    if (ERROR == enet_init(ENET_AUTO_NEGOTIATION, ENET_AUTOCHECKSUM_DROP_FAILFRAMES, ENET_BROADCAST_FRAMES_PASS)) {
        while (1);
    }
#else
    if (ERROR == enet_init(ENET_AUTO_NEGOTIATION, ENET_NO_AUTOCHECKSUM, ENET_BROADCAST_FRAMES_PASS)) {
        while (1);
    }
#endif
    /* initialize MAC address in ethernet MAC */
    enet_mac_address_set(ENET_MAC_ADDRESS0, macaddr);

    enet_interrupt_enable(ENET_DMA_INT_NIE);
    enet_interrupt_enable(ENET_DMA_INT_RIE);

    /* Initialize Tx Descriptors list: Chain Mode */
    enet_descriptors_chain_init(ENET_DMA_TX);

#if (1 == _ENET_HARDWARE_CHECKSUM)
    /* enable the TCP, UDP and ICMP checksum insertion for the Tx frames */
    for (i = 0; i < ENET_TXBUF_NUM; i++) {
        enet_transmit_checksum_config(&txdesc_tab[i], ENET_CHECKSUM_TCPUDPICMP_FULL);
    }
#endif

    /* Initialize Rx Descriptors list: Chain Mode  */
    enet_descriptors_chain_init(ENET_DMA_RX);

    /* enable ethernet Rx interrrupt */
    for (i = 0; i < ENET_RXBUF_NUM; i++) {
        enet_rx_desc_immediate_receive_complete_interrupt(&rxdesc_tab[i]);
    }

    /* enable MAC and DMA transmission and reception */
    enet_enable();

    return true;
}

/**
 * Sends the packet over the link
 *
 * That can not be called from an interrupt context.
 *
 * @param buf  Packet to be send
 * @return     True if the packet was send successfully, False otherwise
 */
bool GD32_EMAC::link_out(emac_mem_buf_t *buf)
{
    emac_mem_buf_t *q;
    uint8_t *buffer;
    uint16_t framelength = 0;

    /* Get exclusive access */
    TXLockMutex.lock();

    while ((uint32_t)RESET != (dma_current_txdesc->status & ENET_TDES0_DAV)) {}

    /* copy frame from pbufs to driver buffers */
    buffer = reinterpret_cast<uint8_t *>(enet_desc_information_get(dma_current_txdesc, TXDESC_BUFFER_1_ADDR));

    for (q = buf; q != NULL; q = memory_manager->get_next(q)) {
        memcpy(static_cast<uint8_t *>(&buffer[framelength]), static_cast<uint8_t *>(memory_manager->get_ptr(q)), memory_manager->get_len(q));
        framelength = framelength + memory_manager->get_len(q);
    }

    /* Prepare transmit descriptors to give to DMA */
    if (SUCCESS != ENET_NOCOPY_FRAME_TRANSMIT(framelength)) {
        while (1);
    }

    memory_manager->free(buf);
    /* Restore access */
    TXLockMutex.unlock();

    return true;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 */
emac_mem_buf_t *GD32_EMAC::low_level_input(void)
{
    emac_mem_buf_t *p = NULL, *q;
    uint32_t l = 0;
    uint16_t len;
    uint8_t *buffer;

    /* obtain the size of the packet and put it into the "len" variable. */
    len = enet_desc_information_get(dma_current_rxdesc, RXDESC_FRAME_LENGTH);
    buffer = reinterpret_cast<uint8_t *>(enet_desc_information_get(dma_current_rxdesc, RXDESC_BUFFER_1_ADDR));

    if (len > 0) {
        /* Allocate a memory buffer chain from buffer pool */
        p = memory_manager->alloc_pool(len, 0);
    } else {
        return p;
    }

    if (p != NULL) {
        for (q = p; q != NULL; q = memory_manager->get_next(q)) {
            memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(q)), static_cast<uint8_t *>(&buffer[l]), memory_manager->get_len(q));
            l = l + memory_manager->get_len(q);
        }
    }
    ENET_NOCOPY_FRAME_RECEIVE();

    return p;
}


/** \brief  Attempt to read a packet from the EMAC interface.
 *
 */
void GD32_EMAC::packet_rx()
{
    /* move received packet into a new buf */
    while (1) {
        emac_mem_buf_t *p = NULL;
        p = low_level_input();

        if (p) {
            emac_link_input_cb(p);
        } else {
            break;
        }
    }
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
void GD32_EMAC::thread_function(void *pvParameters)
{
    static struct GD32_EMAC *gd32_enet = static_cast<GD32_EMAC *>(pvParameters);

    while (1) {
        uint32_t flags = osThreadFlagsWait(_ENET_FLAG_RX, osFlagsWaitAny, osWaitForever);

        if (flags & _ENET_FLAG_RX) {
            gd32_enet->packet_rx();
        }
    }
}

/**
 * This task checks phy link status and updates net status
 */
void GD32_EMAC::phy_task()
{
    uint16_t regval;

    enet_phy_write_read(ENET_PHY_READ, _ENET_BOARD_PHY_ADDRESS, PHY_REG_BSR, &regval);
    if (emac_link_state_cb) {
        regval &= PHY_LINKED_STATUS;

        if (phy_status != regval) {
            if (regval == PHY_LINKED_STATUS) {
                emac_link_state_cb(true);;
            } else {
                emac_link_state_cb(false);
            }
        }
    }

    phy_status = regval;
}

void GD32_EMAC::eth_arch_enable_interrupts(void)
{
    nvic_irq_enable(ENET_IRQn, 7, 0);
}

void GD32_EMAC::eth_arch_disable_interrupts(void)
{
    nvic_irq_disable(ENET_IRQn);
}

/** This returns a unique 6-byte MAC address, based on the device UID
*  This function overrides hal/common/mbed_interface.c function
*  @param mac A 6-byte array to write the MAC address
*/
void mbed_mac_address(char *mac)
{
    uint32_t unique_id;

    unique_id = *(uint32_t *)_GD32_ID_ADDR;
    mac[0] = _GD_MAC_ADDR0;
    mac[1] = _GD_MAC_ADDR1;
    mac[2] = _GD_MAC_ADDR2;
    mac[3] = (unique_id & 0x00ff0000) >> 16;
    mac[4] = (unique_id & 0x0000ff00) >> 8;
    mac[5] = (unique_id & 0x000000ff);
}

/**
 * Initializes the HW
 *
 * @return True on success, False in case of an error.
 */
bool GD32_EMAC::power_up()
{
    /* Initialize the hardware */
    if (true != low_level_init()) {
        return false;
    }

    /* Worker thread */
    rx_thread = create_new_thread("gd32_emac_thread", &GD32_EMAC::thread_function, this, _THREAD_STACKSIZE, _THREAD_PRIORITY, &rx_thread_cb);

    phy_task_handle = mbed::mbed_event_queue()->call_every(_PHY_TASK_PERIOD_MS, mbed::callback(this, &GD32_EMAC::phy_task));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    eth_arch_enable_interrupts();

    return true;
}

/**
 * Return maximum transmission unit
 *
 * @return     MTU in bytes
 */
uint32_t GD32_EMAC::get_mtu_size() const
{
    return _ENET_MTU_SIZE;
}

/**
 * Gets memory buffer alignment preference
 *
 * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
 * align link out memory buffer chains using the alignment.
 *
 * @return         Memory alignment requirement in bytes
 */
uint32_t GD32_EMAC::get_align_preference() const
{
    return 0;
}

/**
 * Return interface name
 *
 * @param name Pointer to where the name should be written
 * @param size Maximum number of character to copy
 */
void GD32_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, _ENET_IF_NAME, (size < sizeof(_ENET_IF_NAME)) ? size : sizeof(_ENET_IF_NAME));
}

/**
 * Returns size of the underlying interface HW address size.
 *
 * @return     HW address size in bytes
 */
uint8_t GD32_EMAC::get_hwaddr_size() const
{
    return _ENET_HW_ADDR_SIZE;
}

/**
 * Returns size of the underlying interface HW address size.
 *
 * @return     HW address size in bytes
 */
bool GD32_EMAC::get_hwaddr(uint8_t *addr) const
{
    mbed_mac_address((char *)addr);
    return true;
}

/**
 * Set HW address for interface
 *
 * Provided address has to be of correct size, see @a get_hwaddr_size
 *
 * Called to set the MAC address to actually use - if @a get_hwaddr is provided
 * the stack would normally use that, but it could be overridden, eg for test
 * purposes.
 *
 * @param addr Address to be set
 */
void GD32_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

/**
 * Sets a callback that needs to be called for packets received for that interface
 *
 * @param input_cb Function to be register as a callback
 */
void GD32_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

/**
 * Sets a callback that needs to be called on link status changes for given interface
 *
 * @param state_cb Function to be register as a callback
 */
void GD32_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

/** Add device to a multicast group
 *
 * @param address  A multicast group hardware address
 */
void GD32_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

/** Remove device from a multicast group
 *
 * @param address  A multicast group hardware address
 */
void GD32_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

/** Request reception of all multicast packets
 *
 * @param all True to receive all multicasts
 *            False to receive only multicasts addressed to specified groups
 */
void GD32_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

/**
 * Deinitializes the HW
 *
 */
void GD32_EMAC::power_down()
{
    /* No-op at this stage */
}

/** Sets memory manager that is used to handle memory buffers
 *
 * @param mem_mngr Pointer to memory manager
 */
void GD32_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

GD32_EMAC &GD32_EMAC::get_instance()
{
    static GD32_EMAC emac;
    return emac;
}

/* Weak so a module can override */
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return GD32_EMAC::get_instance();
}
