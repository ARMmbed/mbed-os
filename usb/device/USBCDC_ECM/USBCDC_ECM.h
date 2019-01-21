/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef USBCDC_ECM_H
#define USBCDC_ECM_H

#include "USBDescriptor.h"
#include "USBDevice.h"

#define MAX_PACKET_SIZE_INT     (64)
#define MAX_PACKET_SIZE_BULK    (64)
#define MAX_PACKET_SIZE_EP0     (64)
#define DEFAULT_CONFIGURATION   (1)

class USBCDC_ECM: public USBDevice {
public:

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */

    USBCDC_ECM(bool connect_blocking = true, uint16_t vendor_id = 0x0700, uint16_t product_id = 0x0101, uint16_t product_release = 0x0001);

    /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBCDC_ECM(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBCDC_ECM();

    /**
     * Check if this class is ready
     *
     * @return true if configured, false otherwise
     */
    bool ready();

    /**
     * Block until this device is configured
     */
    void wait_ready();

    /**
    * Send a buffer
    *
    * This function blocks until the full contents have been sent.
    *
    * @param buffer buffer to be sent
    * @param size length of the buffer
    * @returns true if successful false if interrupted due to a state change
    */
    bool send(uint8_t *buffer, uint32_t size);

protected:

    /*
    * Called when USB changes state
    *
    * @param new_state The new state of the USBDevice
    *
    * Warning: Called in ISR context
    */
    virtual void callback_state_change(DeviceState new_state);

    /*
    * This is used to handle extensions to standard requests
    * and class specific requests with a data phase
    */
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);

    /*
    * Called by USBDevice layer. Set configuration of the device.
    * For instance, you can add all endpoints that you need on this function.
    *
    * @param configuration Number of the configuration
    * @returns true if class handles this request
    */
    virtual void callback_set_configuration(uint8_t configuration);

    /*
    * Called by USBDevice layer in response to set_interface.
    *
    * Upon reception of this command endpoints of any previous interface
    * if any must be removed with endpoint_remove and new endpoint added with
    * endpoint_add.
    *
    * @param configuration Number of the configuration
    *
    * Warning: Called in ISR context
    */
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);

    /*
    * Get device descriptor.
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
    * Get string configuration descriptor
    *
    * @returns pointer to the string configuration descriptor
    */
    virtual const uint8_t *string_iconfiguration_desc();

    /*
    * Get string serial descriptor
    *
    * @returns pointer to the string serial descriptor
    */
    virtual const uint8_t *string_iserial_desc();

    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc(uint8_t index);

    /*
    * This is used to handle extensions to standard requests
    * and class specific requests
    */
    virtual void callback_request(const setup_packet_t *setup);

    /*
    * Called by USBDevice layer on bus reset.
    *
    * complete_reset must be called after
    * the device is fully reset.
    *
    * Warning: Called in ISR context
    */
    virtual void callback_reset();

    uint8_t deviceDescriptor[18];

private:

    usb_ep_t _int_in;
    usb_ep_t _bulk_in;
    usb_ep_t _bulk_out;

    uint8_t _bulk_buf[MAX_PACKET_SIZE_BULK];
    uint32_t _bulk_buf_size;

    rtos::EventFlags _flags;
    rtos::Mutex _write_mutex;

    void _init();
    void _int_callback();
    void _bulk_in_callback();
    void _bulk_out_callback();
    bool _notify_network_connection(uint8_t value);
    bool _notify_connection_speed_change(uint32_t up, uint32_t down);
    bool _write_bulk(uint8_t *buffer, uint32_t size);
};

#endif
