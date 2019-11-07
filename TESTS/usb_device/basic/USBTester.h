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

#ifndef USB_TESTER_H
#define USB_TESTER_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"
#include "EventQueue.h"
#include "EventFlags.h"
#include "platform/NonCopyable.h"

#include "USBDevice.h"

class USBTester: public USBDevice, private mbed::NonCopyable<USBTester> {
public:

    /*
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBTester(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    virtual ~USBTester();

    /*
    *
    * @returns descriptor string in ASCII
    */
    const char *get_serial_desc_string();
    const char *get_iproduct_desc_string();
    const char *get_iinterface_desc_string();
    uint32_t get_reset_count() const
    {
        return reset_count;
    }
    uint32_t get_suspend_count() const
    {
        return suspend_count;
    }
    uint32_t get_resume_count() const
    {
        return resume_count;
    }
    void clear_reset_count()
    {
        reset_count = 0;
    }
    void clear_suspend_count()
    {
        suspend_count = 0;
    }
    void clear_resume_count()
    {
        resume_count = 0;
    }

private:

    enum { NONE = -1 };
    const char *get_desc_string(const uint8_t *desc);
    virtual void suspend(bool suspended);
    bool set_configuration(uint16_t configuration);
    bool set_interface(uint16_t interface, uint16_t alternate);
    bool setup_iterface(uint8_t ep_in, uint8_t ep_out, uint32_t ep_size, usb_ep_type_t ep_type,
                        uint8_t *buf, uint32_t buf_size, void (USBTester::*callback)());
    void remove_iterface(uint16_t interface);
    int16_t interface_0_alt_set;
    int16_t interface_1_alt_set;
    int16_t configuration_set;

protected:

    /*
    * Get device descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the device descriptor
    */
    virtual const uint8_t *device_desc();

    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual const uint8_t *string_iproduct_desc();

    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual const uint8_t *string_iinterface_desc();

    /*
    * Get configuration descriptor
    *
    * @param index descriptor index
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc(uint8_t index);

protected:
    uint8_t bulk_in;
    uint8_t bulk_out;
    uint8_t bulk_buf[64];
    uint8_t int_in;
    uint8_t int_out;
    uint8_t int_buf[64];
    events::EventQueue *queue;
    rtos::EventFlags flags;
    volatile uint32_t reset_count;
    volatile uint32_t suspend_count;
    volatile uint32_t resume_count;

    virtual void callback_state_change(DeviceState new_state);
    virtual void callback_request(const setup_packet_t *setup);
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);
    virtual void callback_set_configuration(uint8_t configuration);
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);
    virtual void epbulk_out_callback();
    virtual void epint_out_callback();
    virtual void callback_reset();
    uint8_t *ctrl_buf;

};

#endif
