/* Copyright (c) 2018-2020 Renesas Electronics Corporation.
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"

#include "r_ether_rza2_if.h"
#include "rza2_emac.h"
#include "r_ether_rza2_config.h"

#define RZ_A2_ETH_IF_NAME    "en"
#define PHY_TASK_PERIOD      200ms

using namespace std::chrono;

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance()
{
#if (ETHER_CH0_EN == 1)
    return RZ_A2_EMAC::get_instance(ETHER_CHANNEL_0);
#elif (ETHER_CH1_EN == 1)
    return RZ_A2_EMAC::get_instance(ETHER_CHANNEL_1);
#else
#error "Set ETHER_CH0_EN or ETHER_CH1_EN to 1."
#endif
}

RZ_A2_EMAC &RZ_A2_EMAC::get_instance(uint32_t channel)
{
#if (ETHER_CH0_EN == 1) && (ETHER_CH1_EN == 1)
    static RZ_A2_EMAC emac_0(ETHER_CHANNEL_0);
    static RZ_A2_EMAC emac_1(ETHER_CHANNEL_1);

    if (channel == 0) {
        return emac_0;
    } else {
        return emac_1;
    }
#else
    static RZ_A2_EMAC emac(channel);
    return emac;
#endif
}

RZ_A2_EMAC::RZ_A2_EMAC(uint32_t channel) : _channel(channel), hwaddr(), hwaddr_set(false), power_on(false),
    recvThread(osPriorityNormal, 896), sem_recv(0)
{
}

uint32_t RZ_A2_EMAC::get_mtu_size() const
{
    return 1500;
}

uint32_t RZ_A2_EMAC::get_align_preference() const
{
    return 0;
}

void RZ_A2_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, RZ_A2_ETH_IF_NAME, (size < sizeof(RZ_A2_ETH_IF_NAME)) ? size : sizeof(RZ_A2_ETH_IF_NAME));
}

uint8_t RZ_A2_EMAC::get_hwaddr_size() const
{
    return 6;
}

bool RZ_A2_EMAC::get_hwaddr(uint8_t *addr) const
{
    return false;
}

void RZ_A2_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(hwaddr, addr, sizeof(hwaddr));
    hwaddr_set = true;

    /* Reconnect */
    if (power_on != false) {
        R_ETHER_Open_ZC2(_channel, hwaddr, ETHER_FLAG_OFF);
    }
}

bool RZ_A2_EMAC::link_out(emac_mem_buf_t *buf)
{
    emac_mem_buf_t *copy_buf = buf;
    uint32_t       retry_cnt = 0;
    uint16_t       write_buf_size;
    int            total_write_size = 0;
    uint8_t       *pwrite_buffer_address;

    while (1) {
        if (R_ETHER_Write_ZC2_GetBuf(_channel, (void **) &pwrite_buffer_address, &write_buf_size) == ETHER_SUCCESS) {
            break;
        }
        retry_cnt++;
        if (retry_cnt > 200) {
            memory_manager->free(buf);
            return false;
        }
        osDelay(1);
    }

    while ((copy_buf != NULL) && (memory_manager->get_ptr(copy_buf) != NULL) && (memory_manager->get_len(copy_buf) != 0)) {
        memcpy(&pwrite_buffer_address[total_write_size], memory_manager->get_ptr(copy_buf), memory_manager->get_len(copy_buf));
        total_write_size +=  memory_manager->get_len(copy_buf);
        copy_buf = memory_manager->get_next(copy_buf);
    }
    memory_manager->free(buf);

    if (total_write_size > 0) {
        if (total_write_size < 60) {
            memset(&pwrite_buffer_address[total_write_size], 0, 60 - total_write_size);
            total_write_size = 60;
        }
        if (R_ETHER_Write_ZC2_SetBuf(_channel, total_write_size) == ETHER_SUCCESS) {
            return true;
        }
    }

    return false;
}

bool RZ_A2_EMAC::power_up()
{
    ether_param_t param;

    if (power_on != false) {
        return true;
    }

    /* Initialize memory which ETHERC/EDMAC is used */
    R_ETHER_Initial();

    /* Set the callback function */
    param.ether_callback.pcb_func    = &_callback_pcb;
    R_ETHER_Control(CONTROL_SET_CALLBACK, param);

    /* Set the callback function */
    param.ether_callback.pcb_int_hnd    = &_callback_hnd;
    R_ETHER_Control(CONTROL_SET_INT_HANDLER, param);

    param.channel = _channel;
    R_ETHER_Control(CONTROL_POWER_ON, param);

    if (hwaddr_set != false) {
        R_ETHER_Open_ZC2(_channel, hwaddr, ETHER_FLAG_OFF);
    }

    /* task */
    recvThread.start(mbed::callback(this, &RZ_A2_EMAC::recv_task));
    phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD, mbed::callback(this, &RZ_A2_EMAC::phy_task));

    power_on = true;
    return true;
}

void RZ_A2_EMAC::power_down()
{
    power_on = false;
}

void RZ_A2_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void RZ_A2_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void RZ_A2_EMAC::add_multicast_group(const uint8_t *addr)
{
    // Not supported
}

void RZ_A2_EMAC::remove_multicast_group(const uint8_t *addr)
{
    // Not supported
}

void RZ_A2_EMAC::set_all_multicast(bool all)
{
    // Not supported
}

void RZ_A2_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

void RZ_A2_EMAC::_callback_pcb(void *arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)arg;
    get_instance(p_cb_arg->channel).callback_pcb(arg);
}

void RZ_A2_EMAC::_callback_hnd(void *arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)arg;
    get_instance(p_cb_arg->channel).callback_hnd(arg);
}

void RZ_A2_EMAC::callback_pcb(void *arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)arg;

    if (p_cb_arg->event_id == ETHER_CB_EVENT_ID_LINK_ON) {
        emac_link_state_cb(true);
    } else if (p_cb_arg->event_id == ETHER_CB_EVENT_ID_LINK_OFF) {
        emac_link_state_cb(false);
    } else {
        // do nothing
    }
}

void RZ_A2_EMAC::callback_hnd(void *arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)arg;

    if (p_cb_arg->status_eesr & 0x00040000) {
        sem_recv.release();
    }
}

void RZ_A2_EMAC::recv_task(void)
{
    int32_t ret;
    emac_mem_buf_t *buf;
    uint8_t *pread_buffer_address;

    while (1) {
        sem_recv.acquire();
        while (1) {
            /* (1) Retrieve the receive buffer location controlled by the  descriptor. */
            ret = R_ETHER_Read_ZC2(_channel, (void **)&pread_buffer_address);
            if (ret <= ETHER_NO_DATA) {
                break;
            }

            /* When there is data to receive */
            while (1) {
                buf = memory_manager->alloc_heap(ret, 0);
                if (buf != NULL) {
                    /* (2) Copy the data read from the receive buffer which is controlled by the descriptor to
                     the buffer which is specified by the user (up to 1024 bytes). */
                    memcpy(memory_manager->get_ptr(buf), pread_buffer_address, (uint32_t)memory_manager->get_len(buf));

                    /* (3) Read the receive data from the receive buffer controlled by the descriptor,
                     and then release the receive buffer. */
                    R_ETHER_Read_ZC2_BufRelease(_channel);

                    emac_link_input_cb(buf);
                    break;
                }
                osDelay(5);
            }
        }
    }
}

void RZ_A2_EMAC::phy_task(void)
{
    R_ETHER_LinkProcess(_channel);
}

