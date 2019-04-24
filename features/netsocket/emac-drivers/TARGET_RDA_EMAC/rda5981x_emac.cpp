/* Copyright (c) 2019 Unisoc Communications Inc.
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
#include "lwip/arch.h"
#include "lwip/pbuf.h"
#include "rda5991h_wland.h"
#include "rda5981x_emac_config.h"
#include "rda5981x_emac.h"
#include "rda_sys_wrapper.h"
#include "maclib_task.h"

#define RDA_HWADDR_SIZE         (6)
#define RDA_ETH_MTU_SIZE        1500
#define RDA_ETH_IF_NAME         "st"

#define RX_PRIORITY   (osPriorityNormal)
#define TX_PRIORITY   (osPriorityNormal)
#define PHY_PRIORITY  (osPriorityNormal)

extern void *packet_rx_queue;

RDA5981x_EMAC::RDA5981x_EMAC()
{
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the memory buffer chain that is passed to the function.
 *
 * @param buf the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return true if the packet could be sent
 *         false value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
bool RDA5981x_EMAC::link_out(emac_mem_buf_t *buf)
{
    emac_mem_buf_t *q, *p = buf;

    u32_t actual_txlen = 0;
    u8_t **data = NULL;
    u16_t retry = 400;

    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output enter, p:%08x\n", p));

    while ((data == NULL) && (retry-- > 0)) {
        data = (u8_t**)wland_get_databuf();
        osThreadYield();
    }
    if (data == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("rda91h_low_level_output, no PKT buf\r\n"));
        memory_manager->free(buf);
        return false;
    }

    for (q = p; q != NULL; q = memory_manager->get_next(q)) {
        /* Send the data from the pbuf to the interface, one pbuf at a
           time. The size of the data in each pbuf is kept in the ->len
           variable. */
        memcpy(&((*data)[actual_txlen+2]), memory_manager->get_ptr(q), memory_manager->get_len(q));//reserve wid header length
        actual_txlen += memory_manager->get_len(q);
        if (actual_txlen > 1514 || actual_txlen > memory_manager->get_total_len(p)) {
            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output err, actual_txlen:%d, tot_len%d\n", actual_txlen, memory_manager->get_total_len(p)));
            memory_manager->free(buf);
            return false;
        }
    }

    /* Signal rda5996 that packet should be sent */
    if (actual_txlen == memory_manager->get_total_len(p)) {
        wland_txip_data((void*)data, actual_txlen, 0);
        memory_manager->free(buf);
        return true;
    }

    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output pkt len mismatch, actual_txlen:%d, tot_len%d\n",
            actual_txlen, memory_manager->get_total_len(p)));

    memory_manager->free(buf);
    return false;
}

/**
 * Should allocate a contiguous memory buffer and transfer the bytes of the incoming
 * packet to the buffer.
 *
 * @param buf If a frame was received and the memory buffer allocation was successful, a memory
 *            buffer filled with the received packet (including MAC header)
 * @return negative value when no more frames,
 *         zero when frame is received
 */
emac_mem_buf_t * RDA5981x_EMAC::low_level_input(u8_t *data, int len)
{
    emac_mem_buf_t *p, *q;
    u16_t index = 0;

    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input enter, rxlen:%d\n", len));

    /* Obtain the size of the packet and put it into the "len"
     variable. */
    if (!len) {
        return NULL;
    }

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = memory_manager->alloc_pool(len, 0);
    if (p != NULL) {
        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
        for (q = p; q != NULL; q = memory_manager->get_next(q)) {
            /* Read enough bytes to fill this pbuf in the chain. The
            * available data in the pbuf is given by the q->len
            * variable.
            * This does not necessarily have to be a memcpy, you can also preallocate
            * pbufs for a DMA-enabled MAC and after receiving truncate it to the
            * actually received size. In this case, ensure the tot_len member of the
            * pbuf is the sum of the chained pbuf len members.
            */
            /* load rx data from 96 to local mem_pool */
            memcpy(memory_manager->get_ptr(q), &data[index], memory_manager->get_len(q));
            index += memory_manager->get_len(q);

            if (index >= len) {
                break;
            }
        }

    } else {
        /* Drop this packet */
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input pbuf_alloc fail, rxlen:%d\n", len));

        return NULL;
    }
    return p;
}


/** \brief  Attempt to read a packet from the EMAC interface.
 *
 */
void RDA5981x_EMAC::packet_rx()
{
    rda_msg msg;
    packet_rx_queue = rda_mail_create(10, sizeof(unsigned int)*4);
    /* move received packet into a new buf */
    while (1) {
        emac_mem_buf_t *p = NULL;
        rda_mail_get(packet_rx_queue, (void*)&msg, osWaitForever);
        switch(msg.type) {
            case 0:
                p = low_level_input((unsigned char*)msg.arg1, msg.arg2);
                if (p == NULL) {
                    break;
                }
                if (p) {
                    emac_link_input_cb(p);
                }
                break;
            case 1:
                emac_link_state_cb(msg.arg1);
                break;
            default:
                break;
        }
    }
}

void RDA5981x_EMAC::thread_function(void *pvParameters)
{
    static struct RDA5981x_EMAC *rda5981x_enet = static_cast<RDA5981x_EMAC *>(pvParameters);
    rda5981x_enet->packet_rx();
}

bool RDA5981x_EMAC::power_up()
{
    /* Initialize the hardware */
    static int init_flag = 0;
    if (init_flag == 0) {
        wland_reg_func();
        rda_thread_new("maclib_thread", maclib_task, NULL, DEFAULT_THREAD_STACKSIZE*8, PHY_PRIORITY);
        rda_thread_new("wland_thread", wland_task, NULL, DEFAULT_THREAD_STACKSIZE*5, PHY_PRIORITY);
        rda_thread_new("packet_rx", RDA5981x_EMAC::thread_function, this, DEFAULT_THREAD_STACKSIZE*5, PHY_PRIORITY);
        /* Allow the PHY task to detect the initial link state and set up the proper flags */
        osDelay(100);
        wland_sta_init();
        init_flag = 1;
    }

    return true;
}

uint32_t RDA5981x_EMAC::get_mtu_size() const
{
    return RDA_ETH_MTU_SIZE;
}

uint32_t RDA5981x_EMAC::get_align_preference() const
{
    return 0;
}

void RDA5981x_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, RDA_ETH_IF_NAME, (size < sizeof(RDA_ETH_IF_NAME)) ? size : sizeof(RDA_ETH_IF_NAME));
}

uint8_t RDA5981x_EMAC::get_hwaddr_size() const
{
    return RDA_HWADDR_SIZE;
}

bool RDA5981x_EMAC::get_hwaddr(uint8_t *addr) const
{
    mbed_mac_address((char *)addr);
    return true;
}

void RDA5981x_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

void RDA5981x_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void RDA5981x_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void RDA5981x_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void RDA5981x_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void RDA5981x_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void RDA5981x_EMAC::power_down()
{
    /* No-op at this stage */
}

void RDA5981x_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

RDA5981x_EMAC &RDA5981x_EMAC::get_instance()
{
    static RDA5981x_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return RDA5981x_EMAC::get_instance();
}

