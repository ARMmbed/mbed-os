/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "fvp_emac.h"
#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"


/********************************************************************************
 * Internal data
 ********************************************************************************/

#define THREAD_STACKSIZE          512

/* Flags for worker thread */
#define FLAG_TX  (0x1u << 0)
#define FLAG_RX  (0x1u << 1)

/** \brief Driver thread priority */
#define THREAD_PRIORITY (osPriorityNormal)

#define PHY_TASK_PERIOD_MS      200


fvp_EMAC::fvp_EMAC() : _thread(THREAD_PRIORITY, THREAD_STACKSIZE, NULL, "fvp_emac_thread")

{
}

void fvp_EMAC::ethernet_callback(lan91_event_t event, void *param)
{
    fvp_EMAC *enet = static_cast<fvp_EMAC *>(param);
    switch (event) {
        case LAN91_RxEvent:
            enet->rx_isr();
            break;
        case LAN91_TxEvent:
            enet->tx_isr();
            break;
        default:
            break;
    }
}

/** \brief Ethernet receive interrupt handler */
void fvp_EMAC::rx_isr()
{
    _thread.flags_set(FLAG_RX);
}

/** \brief Ethernet transmit interrupt handler */
void fvp_EMAC::tx_isr()
{
    _thread.flags_set(FLAG_TX);
}

/** \brief Low level init of the MAC and PHY. */
bool fvp_EMAC::low_level_init_successful()
{
    LAN91_init();
    LAN91_SetCallback(&fvp_EMAC::ethernet_callback, this);
    return true;
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
void fvp_EMAC::thread_function(void *pvParameters)
{
    struct fvp_EMAC *fvp_enet = static_cast<fvp_EMAC *>(pvParameters);

    for (;;) {
        uint32_t flags = ThisThread::flags_wait_any(FLAG_RX | FLAG_TX);
        if (flags & FLAG_RX) {
            fvp_enet->packet_rx();
        }
    }
}


/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 */
void fvp_EMAC::packet_rx()
{
    while (!LAN91_RxFIFOEmpty()) {
        emac_mem_buf_t *temp_rxbuf = NULL;
        uint32_t *rx_payload_ptr;
        uint32_t rx_length = 0;

        temp_rxbuf = _memory_manager->alloc_heap(FVP_ETH_MAX_FLEN, LAN91_BUFF_ALIGNMENT);

        /* no memory been allocated*/
        if (NULL != temp_rxbuf) {

            rx_payload_ptr = (uint32_t *)_memory_manager->get_ptr(temp_rxbuf);
            rx_length = _memory_manager->get_len(temp_rxbuf);
            bool state;

#ifdef LOCK_RX_THREAD
            /* Get exclusive access */
            _TXLockMutex.lock();
#endif
            state = LAN91_receive_frame(rx_payload_ptr, &rx_length);

#ifdef LOCK_RX_THREAD
            _TXLockMutex.unlock();
#endif
            if (!state) {
                _memory_manager->free(temp_rxbuf);
                continue;
            } else {
                _memory_manager->set_len(temp_rxbuf, rx_length);
            }
            _emac_link_input_cb(temp_rxbuf);
        }
    }
    LAN91_SetInterruptMasks(MSK_RCV);
}


/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] buf      the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
bool fvp_EMAC::link_out(emac_mem_buf_t *buf)
{
    // If buffer is chained or not aligned then make a contiguous aligned copy of it
    if (_memory_manager->get_next(buf) ||
            reinterpret_cast<uint32_t>(_memory_manager->get_ptr(buf)) % LAN91_BUFF_ALIGNMENT) {
        emac_mem_buf_t *copy_buf;
        copy_buf = _memory_manager->alloc_heap(_memory_manager->get_total_len(buf), LAN91_BUFF_ALIGNMENT);
        if (NULL == copy_buf) {
            _memory_manager->free(buf);
            return false;
        }

        // Copy to new buffer and free original
        _memory_manager->copy(copy_buf, buf);
        _memory_manager->free(buf);
        buf = copy_buf;
    }

    /* Save the buffer so that it can be freed when transmit is done */
    uint32_t *buffer;
    uint32_t tx_length = 0;
    bool state;
    buffer = (uint32_t *)(_memory_manager->get_ptr(buf));
    tx_length = _memory_manager->get_len(buf);

    /* Get exclusive access */
    _TXLockMutex.lock();

    /* Setup transfers */
    state = LAN91_send_frame(buffer, &tx_length);
    _TXLockMutex.unlock();
    /* Restore access */


    if (!state) {
        return false;
    }
    /* Free the buffer */
    _memory_manager->free(buf);

    return true;
}

/** \brief  PHY task monitoring the link */
void fvp_EMAC::phy_task()
{
    // Get current status
    lan91_phy_status_t connection_status;
    connection_status = LAN91_GetLinkStatus();

    if (connection_status != _prev_state && _emac_link_state_cb) {
        _emac_link_state_cb(connection_status);
    }
    _prev_state = connection_status;
}

bool fvp_EMAC::power_up()
{
    /* Initialize the hardware */
    if (!low_level_init_successful()) {
        return false;
    }

    /* Start ethernet Worker thread */
    _thread.start(callback(&fvp_EMAC::thread_function, this));

    /* Trigger thread to deal with any RX packets that arrived before thread was started */
    rx_isr();

    /* PHY monitoring task */
    _prev_state = STATE_LINK_DOWN;

    mbed::mbed_event_queue()->call(mbed::callback(this, &fvp_EMAC::phy_task));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    ThisThread::sleep_for(10);

    _phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD_MS, mbed::callback(this, &fvp_EMAC::phy_task));

    return true;
}

uint32_t fvp_EMAC::get_mtu_size() const
{
    return LAN91_ETH_MTU_SIZE;
}

uint32_t fvp_EMAC::get_align_preference() const
{
    return LAN91_BUFF_ALIGNMENT;
}

void fvp_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, FVP_ETH_IF_NAME, (size < sizeof(FVP_ETH_IF_NAME)) ? size : sizeof(FVP_ETH_IF_NAME));
}

uint8_t fvp_EMAC::get_hwaddr_size() const
{
    return FVP_HWADDR_SIZE;
}

bool fvp_EMAC::get_hwaddr(uint8_t *addr) const
{
    read_MACaddr(addr);
    return true;
}

void fvp_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

void fvp_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    _emac_link_input_cb = input_cb;
}

void fvp_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    _emac_link_state_cb = state_cb;
}

void fvp_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void fvp_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void fvp_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void fvp_EMAC::power_down()
{
    /* No-op at this stage */
}

void fvp_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    _memory_manager = &mem_mngr;
}


fvp_EMAC &fvp_EMAC::get_instance()
{
    static fvp_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return fvp_EMAC::get_instance();
}

/** @} */

/* --------------------------------- End Of File ------------------------------ */
