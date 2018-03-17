/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef USBCDC_H
#define USBCDC_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#include "USBDevice.h"
#include "LinkedList.h"

class AsyncOp;

class USBCDC: public USBDevice {
public:

    /*
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    * @param connect_blocking define if the connection must be blocked if USB not plugged in
    */
    USBCDC(uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect_blocking);

    /*
    * Constructor
    *
    * @param phy USB phy to use
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    * @param connect_blocking define if the connection must be blocked if USB not plugged in
    */
    USBCDC(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect_blocking);

    /**
     * Block until the terminal is connected
     */
    void wait_connected();

    /*
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
     * Send what there is room for
     *
     * @param buffer data to send
     * @param size maximum number of bytes to send
     * @param actual a pointer to where to store the number of bytes sent
     * @param now true to start data transmission, false to wait
     */
    void send_nb(uint8_t *buffer, uint32_t size, uint32_t *actual, bool now = true);

    /*
    * Read a buffer from a certain endpoint. Warning: blocking
    *
    * Blocks until at least one byte of data has been read (actual != NULL) or
    * until the full size has been read (actual == NULL).
    *
    * @param buffer buffer where will be stored bytes
    * @param size the maximum number of bytes to read
    * @param actual A pointer to where to store the number of bytes actually read
    *   or NULL to read the full size
    * @returns true if successful false if interrupted due to a state change
    */
    bool receive(uint8_t *buffer, uint32_t size, uint32_t *actual=NULL);

    /**
     * Read from the receive buffer
     *
     * @param buffer buffer to fill with data
     * @param size maximum number of bytes read
     * @param actual a pointer to where to store the number of bytes actually received
     */
    void receive_nb(uint8_t *buffer, uint32_t size, uint32_t *actual);

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
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc();

    /*
    * Called by USBCallback_requestCompleted when CDC line coding is changed
    * Warning: Called in ISR
    *
    * @param baud The baud rate
    * @param bits The number of bits in a word (5-8)
    * @param parity The parity
    * @param stop The number of stop bits (1 or 2)
    */
    virtual void line_coding_changed(int baud, int bits, int parity, int stop) {};

    /*
     * Called when there is data that can be read
     */
    virtual void data_rx() {}

    /*
     * Called when there is space in the TX buffer
     */
    virtual void data_tx() {}

protected:

    class AsyncWrite;
    class AsyncRead;

    virtual void callback_reset();
    virtual void callback_state_change(DeviceState new_state);
    virtual void callback_request(const setup_packet_t *setup);
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);
    virtual void callback_set_configuration(uint8_t configuration);
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);

    void _init(bool connect_blocking);

    void _change_terminal_connected(bool connected);
    void _connect_wake_all();

    void _send_next();
    void _send_abort_all();
    void _send_isr_start();
    void _send_isr(usb_ep_t endpoint);

    void _receive_next();
    void _receive_abort_all();
    void _receive_isr_start();
    void _receive_isr(usb_ep_t endpoint);

    usb_ep_t _bulk_in;
    usb_ep_t _bulk_out;
    usb_ep_t _int_in;

    uint8_t _cdc_line_coding[7];
    uint8_t _cdc_new_line_coding[7];
    uint8_t _config_descriptor[75];

    LinkedList<AsyncOp> _connected_list;
    bool _terminal_connected;

    LinkedList<AsyncWrite> _tx_list;
    bool _tx_in_progress;
    uint8_t _tx_buffer[64];
    uint8_t *_tx_buf;
    uint32_t _tx_size;

    LinkedList<AsyncRead> _rx_list;
    bool _rx_in_progress;
    uint8_t _rx_buffer[64];
    uint8_t *_rx_buf;
    uint32_t _rx_size;
};

#endif
