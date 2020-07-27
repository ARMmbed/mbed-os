/*
 * Copyright (c) 2019 Arm Limited
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

#include "mbed_interface.h"
#include "mbed_wait_api.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"

#include "smsc9220_emac.h"
#include "smsc9220_eth_drv.h"

#ifndef SMSC9220_ETH
#error "SMSC9220_ETH should be defined, check device_cfg.h!"
#endif

#ifndef SMSC9220_Ethernet_Interrupt_Handler
#error "SMSC9220_Ethernet_Interrupt_Handler should be defined to platform's \
Ethernet IRQ handler!"
#endif

static SMSC9220_EMAC *board_emac_pointer = NULL;
const struct smsc9220_eth_dev_t* SMSC9220_EMAC::dev = &SMSC9220_ETH_DEV;

extern "C" void SMSC9220_Ethernet_Interrupt_Handler(void)
{
    if (smsc9220_get_interrupt(SMSC9220_EMAC::dev,
                                 SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL)) {
        board_emac_pointer->rx_isr();
        smsc9220_clear_interrupt(SMSC9220_EMAC::dev,
                                 SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL);
        smsc9220_disable_interrupt(SMSC9220_EMAC::dev,
                                   SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL);
    }
}

SMSC9220_EMAC::SMSC9220_EMAC() : receiver_thread(LINK_STATUS_THREAD_PRIORITY,
                                       (uint32_t)LINK_STATUS_THREAD_STACKSIZE)
{
}

/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt.
 */
void SMSC9220_EMAC::rx_isr()
{
    receiver_thread.flags_set(FLAG_RX);
}

/** \brief  Allocates a emac_mem_buf_t and returns the data from the incoming
 * packet.
 *
 *  \return a emac_mem_buf_t filled with the received packet
 * (including MAC header)
 */
emac_mem_buf_t *SMSC9220_EMAC::low_level_input()
{
    emac_mem_buf_t *p = NULL;
    uint32_t message_length = 0;
    uint32_t received_bytes = 0;

    message_length = smsc9220_peek_next_packet_size(dev);
    if (message_length == 0) {
        return p;
    } else {
        /* The Ethernet controller cannot remove CRC from the end of the
         * incoming packet, thus it should be taken into account when
         * calculating the actual message length.*/
        message_length -= CRC_LENGTH_BYTES;
    }

    p = _memory_manager->alloc_heap(SMSC9220_ETH_MAX_FRAME_SIZE,
                                    SMSC9220_BUFF_ALIGNMENT);

    if (p != NULL) {
        _RXLockMutex.lock();
        received_bytes = smsc9220_receive_by_chunks(dev,
                                  (char*)_memory_manager->get_ptr(p),
                                  _memory_manager->get_len(p));
        if(received_bytes == 0){
            _memory_manager->free(p);
            p = nullptr;
        } else {
            _memory_manager->set_len(p, received_bytes);
        }
        _RXLockMutex.unlock();
    }

    return p;
}

/** \brief  Receiver thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] params pointer to the interface data
 */
void SMSC9220_EMAC::receiver_thread_function(void* params)
{
    struct SMSC9220_EMAC *smsc9220_enet = static_cast<SMSC9220_EMAC *>(params);

    while(1) {
        uint32_t flags = ThisThread::flags_wait_any(FLAG_RX);

        if (flags & FLAG_RX) {
            smsc9220_enet->packet_rx();
        }
    }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the Network Stack.
 */
void SMSC9220_EMAC::packet_rx()
{
    emac_mem_buf_t *p;
    p = low_level_input();
    if(p != NULL) {
        _emac_link_input_cb(p);
    }
    smsc9220_enable_interrupt(dev, SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL);
}

bool SMSC9220_EMAC::link_out(emac_mem_buf_t *buf)
{
    if(buf == NULL) {
        return false;
    } else {
        uint32_t buffer_chain_length = 0;
        enum smsc9220_error_t error = SMSC9220_ERROR_NONE;
        /* If buffer is chained or not aligned then
         * make a contiguous aligned copy of it */
        if (_memory_manager->get_next(buf) ||
            reinterpret_cast<uint32_t>(_memory_manager->get_ptr(buf)) %
                                                      SMSC9220_BUFF_ALIGNMENT) {
            emac_mem_buf_t *copy_buf;
            copy_buf = _memory_manager->alloc_heap(
                                             _memory_manager->get_total_len(buf),
                                             SMSC9220_BUFF_ALIGNMENT);
            if (NULL == copy_buf) {
                _memory_manager->free(buf);
                return false;
            }

            /* Copy to new buffer and free original */
            _memory_manager->copy(copy_buf, buf);
            _memory_manager->free(buf);
            buf = copy_buf;
        }

        buffer_chain_length = _memory_manager->get_total_len(buf);

        _TXLockMutex.lock();
        error = smsc9220_send_by_chunks(dev,
                                      buffer_chain_length,
                                      true,
                                      (const char*)_memory_manager->get_ptr(buf),
                                      _memory_manager->get_len(buf));
        _memory_manager->free(buf);
        _TXLockMutex.unlock();
        return (error == SMSC9220_ERROR_NONE);
    }
}

void SMSC9220_EMAC::link_status_task()
{
    uint32_t phy_basic_status_reg_value = 0;
    bool current_link_status_up = false;

    /* Get current status */
    smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_BSTATUS,
                         &phy_basic_status_reg_value);

    current_link_status_up = (bool)(phy_basic_status_reg_value &
                          (1ul << (PHY_REG_BSTATUS_LINK_STATUS_INDEX)));

    /* Compare with the previous state */
    if (current_link_status_up != _prev_link_status_up) {
        _emac_link_state_cb(current_link_status_up);
        _prev_link_status_up = current_link_status_up;
    }

}

bool SMSC9220_EMAC::power_up()
{
    board_emac_pointer = this;
    receiver_thread.start(callback(&SMSC9220_EMAC::receiver_thread_function,
                                   this));

    /* Initialize the hardware */
    enum smsc9220_error_t init_successful = smsc9220_init(dev, &ThisThread::sleep_for);
    if (init_successful != SMSC9220_ERROR_NONE) {
        return false;
    }

    /* Init FIFO level interrupts: use Rx status level irq to trigger
     * interrupts for any non-processed packets, while Tx is not irq driven */
    smsc9220_set_fifo_level_irq(dev, SMSC9220_FIFO_LEVEL_IRQ_RX_STATUS_POS,
            SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MIN);
    smsc9220_set_fifo_level_irq(dev, SMSC9220_FIFO_LEVEL_IRQ_TX_STATUS_POS,
            SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MIN);
    smsc9220_set_fifo_level_irq(dev, SMSC9220_FIFO_LEVEL_IRQ_TX_DATA_POS,
            SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MAX);

    /* Enable Ethernet interrupts in NVIC */
    NVIC_EnableIRQ(ETHERNET_IRQn);
    smsc9220_enable_interrupt(dev, SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL);

    /* Trigger thread to deal with any RX packets that arrived
     * before receiver_thread was started */
    rx_isr();
    _prev_link_status_up = PHY_STATE_LINK_DOWN;
    mbed::mbed_event_queue()->call(mbed::callback(this,
                                             &SMSC9220_EMAC::link_status_task));

    /* Allow the Link Status task to detect the initial link state */
    ThisThread::sleep_for(10);
    _link_status_task_handle = mbed::mbed_event_queue()->call_every(
                              LINK_STATUS_TASK_PERIOD_MS,
                              mbed::callback(this,
                                             &SMSC9220_EMAC::link_status_task));

    return true;
}

uint32_t SMSC9220_EMAC::get_mtu_size() const
{
    return SMSC9220_ETH_MTU_SIZE;
}

uint32_t SMSC9220_EMAC::get_align_preference() const
{
    return SMSC9220_BUFF_ALIGNMENT;
}

void SMSC9220_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, SMSC9220_ETH_IF_NAME, (size < sizeof(SMSC9220_ETH_IF_NAME)) ?
                                           size : sizeof(SMSC9220_ETH_IF_NAME));
}

uint8_t SMSC9220_EMAC::get_hwaddr_size() const
{
    return SMSC9220_HWADDR_SIZE;
}

bool SMSC9220_EMAC::get_hwaddr(uint8_t *addr) const
{
    if(smsc9220_read_mac_address(dev, (char*)addr) == SMSC9220_ERROR_NONE) {
        return true;
    } else {
        return false;
    }
}

void SMSC9220_EMAC::set_hwaddr(const uint8_t *addr)
{
    if (!addr) {
        return;
    }

    memcpy(_hwaddr, addr, sizeof _hwaddr);
    uint32_t mac_low = 0;
    uint32_t mac_high = 0;

    /* Using local variables to make sure the right alignment is used */
    memcpy((void*)&mac_low, (void*)addr, 4);
    memcpy((void*)&mac_high, (void*)(addr+4), 2);

    if (smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_ADDRL, mac_low)) {
        return;
    }
    if (smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_ADDRH, mac_high)) {
        return;
    }
}

void SMSC9220_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    _emac_link_input_cb = input_cb;
}

void SMSC9220_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    _emac_link_state_cb = state_cb;
}

void SMSC9220_EMAC::add_multicast_group(const uint8_t *addr)
{
    // No action for now
}

void SMSC9220_EMAC::remove_multicast_group(const uint8_t *addr)
{
    // No action for now
}

void SMSC9220_EMAC::set_all_multicast(bool all)
{
    // No action for now
}

void SMSC9220_EMAC::power_down()
{
    // No action for now
}

void SMSC9220_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    _memory_manager = &mem_mngr;
}


SMSC9220_EMAC &SMSC9220_EMAC::get_instance() {
    static SMSC9220_EMAC emac;
    return emac;
}

/* Weak so a module can override */
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return SMSC9220_EMAC::get_instance();
}
