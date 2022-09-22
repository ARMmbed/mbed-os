/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#if defined(MBED_CONF_RTOS_PRESENT) || defined(DOXYGEN_ONLY)
#include "USBDescriptor.h"
#include "USBDevice.h"
#include "ByteBuffer.h"
#include "rtos/Mutex.h"
#include "EventFlags.h"
#include "events/EventQueue.h"
#include "rtos/Thread.h"
#include "Callback.h"

#define MAX_PACKET_SIZE_INT     (64)
#define MAX_PACKET_SIZE_BULK    (64)
#define MAX_PACKET_SIZE_EP0     (64)
#define DEFAULT_CONFIGURATION   (1)

#define PACKET_TYPE_PROMISCUOUS     (1<<0)
#define PACKET_TYPE_ALL_MULTICAST   (1<<1)
#define PACKET_TYPE_DIRECTED        (1<<2)
#define PACKET_TYPE_BROADCAST       (1<<3)
#define PACKET_TYPE_MULTICAST       (1<<4)

/**
 * \defgroup drivers_USBCDC_ECM USBCDC_ECM class
 * \ingroup drivers-public-api-usb
 * @{
 * @note Bare metal profile: This class is not supported.
 */

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

    /**
     * Read from the receive buffer
     *
     * @param buffer buffer to fill with data
     * @param size maximum number of bytes read
     * @param actual a pointer to where to store the number of bytes actually received
     */
    void receive_nb(uint8_t *buffer, uint32_t size, uint32_t *actual);

    /**
     * Return ethernet packet filter bitmap
     *
     * The Packet Filter is the inclusive OR of the bitmap
     * D0:   PACKET_TYPE_PROMISCUOUS
     * D1:   PACKET_TYPE_ALL_MULTICAST
     * D2:   PACKET_TYPE_DIRECTED
     * D3:   PACKET_TYPE_BROADCAST
     * D4:   PACKET_TYPE_MULTICAST
     * D5-D15: Reserved (zero)
     *
     * @return ethernet packet filter bitmap
     */
    uint16_t read_packet_filter();

    /**
     * Attach a callback for when an ethernet packet is received
     *
     * @param cb code to call when a packet is received
     */
    void attach_rx(mbed::Callback<void()> cb);

    /**
     * Attach a callback for when a request to configure device ethernet
     * packet filter is received
     *
     * @param cb code to call when a packet filter request is received
     */
    void attach_filter(mbed::Callback<void()> cb);

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

    uint8_t device_descriptor[18];

private:

    usb_ep_t _int_in;
    usb_ep_t _bulk_in;
    usb_ep_t _bulk_out;

    uint8_t _config_descriptor[80];
    uint8_t _string_imac_addr[26];

    uint8_t _bulk_buf[MAX_PACKET_SIZE_BULK];
    uint16_t _packet_filter;
    ByteBuffer _rx_queue;

    rtos::EventFlags _flags;
    rtos::Mutex _write_mutex;

    events::EventQueue _queue;
    rtos::Thread _thread;
    mbed::Callback<void()> _callback_rx;
    mbed::Callback<void()> _callback_filter;

    void _init();
    void _int_callback();
    void _bulk_in_callback();
    void _bulk_out_callback();
    bool _notify_network_connection(uint8_t value);
    bool _notify_connection_speed_change(uint32_t up, uint32_t down);
    bool _write_bulk(uint8_t *buffer, uint32_t size);
    void _notify_connect();
};

/** @}*/
#endif // defined(MBED_CONF_RTOS_PRESENT)
#endif
