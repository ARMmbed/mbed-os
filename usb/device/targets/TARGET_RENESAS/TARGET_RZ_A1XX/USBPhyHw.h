/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited, All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef USBPHYHW_H
#define USBPHYHW_H

#include "mbed.h"
#include "USBPhy.h"


class USBPhyHw : public USBPhy {
public:
    USBPhyHw();
    virtual ~USBPhyHw();
    virtual void init(USBPhyEvents *events);
    virtual void deinit();
    virtual bool powered();
    virtual void connect();
    virtual void disconnect();
    virtual void configure();
    virtual void unconfigure();
    virtual void sof_enable();
    virtual void sof_disable();
    virtual void set_address(uint8_t address);
    virtual void remote_wakeup();
    virtual const usb_ep_table_t *endpoint_table();

    virtual uint32_t ep0_set_max_packet(uint32_t max_packet);
    virtual void ep0_setup_read_result(uint8_t *buffer, uint32_t size);
    virtual void ep0_read(uint8_t *data, uint32_t size);
    virtual uint32_t ep0_read_result();
    virtual void ep0_write(uint8_t *buffer, uint32_t size);
    virtual void ep0_stall();

    virtual bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type);
    virtual void endpoint_remove(usb_ep_t endpoint);
    virtual void endpoint_stall(usb_ep_t endpoint);
    virtual void endpoint_unstall(usb_ep_t endpoint);

    virtual bool endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size);
    virtual uint32_t endpoint_read_result(usb_ep_t endpoint);
    virtual bool endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size);
    virtual void endpoint_abort(usb_ep_t endpoint);

    virtual void process();

private:
#define PIPE_NUM      (16)

    typedef struct {
        bool        enable;
        uint16_t    status;
        uint32_t    req_size;
        uint32_t    data_cnt;
        uint8_t     *p_data;
    } pipe_ctrl_t;

    USBPhyEvents *events;
    pipe_ctrl_t pipe_ctrl[PIPE_NUM];
    uint16_t setup_buffer[32];

    static void _usbisr(void);
    void chg_curpipe(uint16_t pipe, uint16_t isel);
    uint16_t is_set_frdy(uint16_t pipe, uint16_t isel);
    uint8_t *read_fifo(uint16_t pipe, uint16_t count, uint8_t *read_p);
    uint16_t read_data(uint16_t pipe);
    void fifo_to_buf(uint16_t pipe);
    uint8_t *write_fifo(uint16_t pipe, uint16_t count, uint8_t *write_p);
    uint16_t write_data(uint16_t pipe);
    void buf_to_fifo(uint16_t pipe);
    uint16_t *get_pipectr_reg(uint16_t pipe);
    uint16_t *get_pipetre_reg(uint16_t pipe);
    uint16_t *get_pipetrn_reg(uint16_t pipe);
    uint16_t *get_fifoctr_reg(uint16_t pipe);
    uint16_t *get_fifosel_reg(uint16_t pipe);
    uint32_t *get_fifo_reg(uint16_t pipe);
    uint16_t get_pid(uint16_t pipe);
    void set_mbw(uint16_t pipe, uint16_t data);
    void set_pid(uint16_t pipe, uint16_t new_pid);
    void cpu_delay_1us(uint16_t time);
    uint16_t EP2PIPE(uint16_t endpoint);
    uint16_t PIPE2EP(uint16_t pipe);
    uint16_t PIPE2FIFO(uint16_t pipe);
    void reset_usb(uint16_t clockmode);
    bool chk_vbsts(void);
    void ctrl_end(uint16_t status);
    void data_end(uint16_t pipe, uint16_t status);
    void forced_termination(uint16_t pipe, uint16_t status);

};

#endif
