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

#ifndef USB_HID_H
#define USB_HID_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice.h"

#include "USBHID_Types.h"
#include "AsyncOp.h"
#include "LinkedList.h"



/**
 * USBHID example
 * @code
 * #include "mbed.h"
 * #include "USBHID.h"
 *
 * USBHID hid;
 * HID_REPORT recv;
 * BusOut leds(LED1,LED2,LED3,LED4);
 *
 * int main(void) {
 *    while (1) {
 *        hid.read(&recv);
 *        leds = recv.data[0];
 *    }
 * }
 * @endcode
 */

class USBHID: public USBDevice {
public:

    /**
    * Constructor
    *
    * @param output_report_length Maximum length of a sent report (up to 64 bytes) (default: 64 bytes)
    * @param input_report_length Maximum length of a received report (up to 64 bytes) (default: 64 bytes)
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    * @param connect Connect the device
    */
    USBHID(uint8_t output_report_length = 64, uint8_t input_report_length = 64, uint16_t vendor_id = 0x1234, uint16_t product_id = 0x0006, uint16_t product_release = 0x0001, bool connect = true);

    /**
    * Constructor
    *
    * @param phy USBPhy to use as the backend
    * @param output_report_length Maximum length of a sent report (up to 64 bytes) (default: 64 bytes)
    * @param input_report_length Maximum length of a received report (up to 64 bytes) (default: 64 bytes)
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    * @param connect Connect the device
    */
    USBHID(USBPhy *phy, uint8_t output_report_length, uint8_t input_report_length, uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect);

    /**
     * Block until this HID device is configured
     */
    void wait_connected();

    /**
    * Send a Report. warning: blocking
    *
    * @param report Report which will be sent (a report is defined by all data and the length)
    * @returns true if successful
    */
    bool send(const HID_REPORT *report);


    /**
    * Send a Report. warning: non blocking
    *
    * @param report Report which will be sent (a report is defined by all data and the length)
    * @returns true if successful
    */
    bool send_nb(const HID_REPORT *report);

    /**
    * Read a report: blocking
    *
    * @param report pointer to the report to fill
    * @returns true if successful
    */
    bool read(HID_REPORT *report);

    /**
    * Read a report: non blocking
    *
    * @param report pointer to the report to fill
    * @returns true if successful
    */
    bool read_nb(HID_REPORT *report);

protected:
    uint16_t reportLength;
    uint8_t reportDescriptor[27];

    /*
    * Get the Report descriptor
    *
    * @returns pointer to the report descriptor
    */
    virtual const uint8_t *report_desc();

    /*
    * Get the length of the report descriptor
    *
    * @returns the length of the report descriptor
    */
    virtual uint16_t report_desc_length();

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
    * HID Report received by SET_REPORT request. Warning: Called in ISR context
    * First byte of data will be the report ID
    *
    * @param report Data and length received
    */
    virtual void HID_callbackSetReport(HID_REPORT *report) {};

    /**
    * Called when USB changes state
    *
    * @param new_state The new state of the USBDevice
    *
    * Warning: Called in ISR context
    */
    virtual void callback_state_change(DeviceState new_state);

    /*
    * This is used to handle extensions to standard requests
    * and class specific requests
    */
    virtual void callback_request(const setup_packet_t *setup);

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
     * Called when there is a hid report that can be read
     */
    virtual void report_rx() {}

    /*
     * Called when there is space to send a hid report
     */
    virtual void report_tx() {}

protected:
    usb_ep_t _int_in;
    usb_ep_t _int_out;

private:
    void _init(uint8_t output_report_length, uint8_t input_report_length, bool connect);
    void _send_isr(usb_ep_t endpoint);
    void _read_isr(usb_ep_t endpoint);

    void _connect_wake_all();
    void _send_abort_all();
    void _read_abort_all();

    class AsyncSend;
    class AsyncRead;

    LinkedList<AsyncOp> _connect_list;
    LinkedList<AsyncSend> _send_list;
    bool _send_idle;
    LinkedList<AsyncRead> _read_list;
    bool _read_idle;

    uint8_t _configuration_descriptor[41];
    HID_REPORT _input_report;
    HID_REPORT _output_report;
    uint8_t _output_length;
    uint8_t _input_length;


};

#endif
