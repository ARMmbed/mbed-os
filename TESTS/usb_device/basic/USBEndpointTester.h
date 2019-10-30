/*
 * Copyright (c) 2018-2018, ARM Limited, All Rights Reserved
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

#ifndef USB_ENDPOINT_TESTER_H
#define USB_ENDPOINT_TESTER_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"
#include "EventQueue.h"
#include "EventFlags.h"
#include "platform/NonCopyable.h"

#include "USBDevice.h"

#define NUM_ENDPOINTS 6 // Not including CTRL OUT/IN

class USBEndpointTester: public USBDevice, private mbed::NonCopyable<USBEndpointTester> {

public:
    USBEndpointTester(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release,
                      bool abort_transfer_test);
    virtual ~USBEndpointTester();
    const char *get_serial_desc_string();
    void start_ep_in_abort_test();

    uint32_t get_cnt_cb_set_conf() const
    {
        return _cnt_cb_set_conf;
    }
    uint32_t get_cnt_cb_set_intf() const
    {
        return _cnt_cb_set_intf;
    }
    uint32_t get_cnt_cb_bulk_out() const
    {
        return _cnt_cb_bulk_out;
    }
    uint32_t get_cnt_cb_bulk_in() const
    {
        return _cnt_cb_bulk_in;
    }
    uint32_t get_cnt_cb_int_out() const
    {
        return _cnt_cb_int_out;
    }
    uint32_t get_cnt_cb_int_in() const
    {
        return _cnt_cb_int_in;
    }
    uint32_t get_cnt_cb_iso_out() const
    {
        return _cnt_cb_iso_out;
    }
    uint32_t get_cnt_cb_iso_in() const
    {
        return _cnt_cb_iso_in;
    }

    struct ep_config_t {
        bool dir_in;
        usb_ep_type_t type;
        uint32_t max_packet;
        ep_cb_t callback;
    };

protected:
    events::EventQueue *queue;
    rtos::EventFlags flags;
    uint8_t *ctrl_buf;

    bool _abort_transfer_test;
    usb_ep_t _endpoints[NUM_ENDPOINTS];
    uint8_t *_endpoint_buffs[NUM_ENDPOINTS];
    ep_config_t (*_endpoint_configs)[NUM_ENDPOINTS];

    static ep_config_t _intf_config_max[NUM_ENDPOINTS];
    static ep_config_t _intf_config0[NUM_ENDPOINTS];
    static ep_config_t _intf_config1[NUM_ENDPOINTS];
    static ep_config_t _intf_config2[NUM_ENDPOINTS];
    static ep_config_t _intf_config3[NUM_ENDPOINTS];
    static ep_config_t _intf_config4[NUM_ENDPOINTS];

    volatile uint32_t _cnt_cb_set_conf;
    volatile uint32_t _cnt_cb_set_intf;
    volatile uint32_t _cnt_cb_bulk_out;
    volatile uint32_t _cnt_cb_bulk_in;
    volatile uint32_t _cnt_cb_int_out;
    volatile uint32_t _cnt_cb_int_in;
    volatile uint32_t _cnt_cb_iso_out;
    volatile uint32_t _cnt_cb_iso_in;

    volatile uint32_t _num_packets_bulk_out_abort;
    volatile uint32_t _num_packets_bulk_in_abort;
    volatile uint32_t _num_packets_int_out_abort;
    volatile uint32_t _num_packets_int_in_abort;

    virtual const uint8_t *configuration_desc(uint8_t index);
    virtual void callback_state_change(DeviceState new_state);
    virtual void callback_request(const setup_packet_t *setup);
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);
    virtual void callback_set_configuration(uint8_t configuration);
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);

    void _setup_non_zero_endpoints();
    bool _setup_interface(uint16_t interface, uint8_t alternate);

    virtual void _cb_bulk_out();
    virtual void _cb_bulk_in();
    virtual void _cb_int_out();
    virtual void _cb_int_in();
    virtual void _cb_iso_out();
    virtual void _cb_iso_in();

private:
    const char *get_desc_string(const uint8_t *desc);
    bool _request_rw_restart(const setup_packet_t *setup);
    bool _request_abort_buff_check(const setup_packet_t *setup);
};

#endif
