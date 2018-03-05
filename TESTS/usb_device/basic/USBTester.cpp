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

#include "stdint.h"
#include "USBTester.h"
#include "mbed_shared_queues.h"
#include "EndpointResolver.h"

#define DEFAULT_CONFIGURATION (1)

#define VENDOR_TEST_CTRL_IN                 1
#define VENDOR_TEST_CTRL_OUT                2
#define VENDOR_TEST_CTRL_NONE               3
#define VENDOR_TEST_CTRL_IN_DELAY           4
#define VENDOR_TEST_CTRL_OUT_DELAY          5
#define VENDOR_TEST_CTRL_NONE_DELAY         6
#define VENDOR_TEST_CTRL_IN_STATUS_DELAY    7
#define VENDOR_TEST_CTRL_OUT_STATUS_DELAY   8

#define MAX_EP_SIZE 64
#define MIN_EP_SIZE 8


USBTester::USBTester(uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect_blocking): USBDevice(vendor_id, product_id, product_release)
{

    EndpointResolver resolver(endpoint_table());

    resolver.endpoint_ctrl(64);
    bulk_in = resolver.endpoint_in(USB_EP_TYPE_BULK, 64);
    bulk_out = resolver.endpoint_out(USB_EP_TYPE_BULK, 64);
    int_in = resolver.endpoint_in(USB_EP_TYPE_INT, 64);
    int_out = resolver.endpoint_out(USB_EP_TYPE_INT, 64);
    MBED_ASSERT(resolver.valid());
    queue = mbed_highprio_event_queue();

    configuration_desc();

    init();
    USBDevice::connect(connect_blocking);

}

USBTester::~USBTester()
{
    deinit();
}

void USBTester::callback_state_change(DeviceState new_state)
{
    // Nothing to do
};

void USBTester::callback_request(const setup_packet_t *setup)
{
    /* Called in ISR context */
    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;
    uint32_t delay = 0;

    /* Process vendor-specific requests */
    if (setup->bmRequestType.Type == VENDOR_TYPE) {
        switch (setup->bRequest) {
            case VENDOR_TEST_CTRL_IN:
                result = Send;
                data = ctrl_buf;
                size = setup->wValue < sizeof(ctrl_buf) ? setup->wValue : sizeof(ctrl_buf);
                break;
            case VENDOR_TEST_CTRL_OUT:
                result = Receive;
                data = ctrl_buf;
                size = setup->wValue < 8 ? setup->wValue  : 8;
                break;
            case VENDOR_TEST_CTRL_NONE:
                result = Success;
                break;
            case VENDOR_TEST_CTRL_NONE_DELAY:
                result = Success;
                delay = 2000;
                break;
            default:
                result = PassThrough;
                break;
        }
    }

    if (delay) {
        queue->call_in(delay, static_cast<USBDevice *>(this), &USBTester::complete_request, Success, data, size);
    } else {
        complete_request(result, data, size);
    }


}

void USBTester::callback_request_xfer_done(const setup_packet_t *setup)
{
    bool result = false;
    if (setup->bmRequestType.Type == VENDOR_TYPE) {
        switch (setup->bRequest) {
            case VENDOR_TEST_CTRL_IN:
                result = true;
                break;
            case VENDOR_TEST_CTRL_OUT:
                result = true;
                break;
            default:
                result = false;
                break;
        }
    }
    complete_request_xfer_done(true);
}

// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported.
void USBTester::callback_set_configuration(uint8_t configuration)
{
    if (configuration != DEFAULT_CONFIGURATION) {
        complete_set_configuration(false);
        return;
    }

    // Configure endpoints > 0
    endpoint_add(int_in, MAX_EP_SIZE, USB_EP_TYPE_INT);
    endpoint_add(int_out, MAX_EP_SIZE, USB_EP_TYPE_INT, &USBTester::epint_out_callback);
    endpoint_add(bulk_in, MAX_EP_SIZE, USB_EP_TYPE_BULK);
    endpoint_add(bulk_out, MAX_EP_SIZE, USB_EP_TYPE_BULK, &USBTester::epbulk_out_callback);

    read_start(int_out);
    read_start(bulk_out);

    complete_set_configuration(true);
}

void USBTester::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    if (interface == 0 && alternate == 0) {
        endpoint_remove(int_in);
        endpoint_remove(int_out);
        endpoint_remove(bulk_in);
        endpoint_remove(bulk_out);

        endpoint_add(int_in, MAX_EP_SIZE, USB_EP_TYPE_INT);
        endpoint_add(int_out, MAX_EP_SIZE, USB_EP_TYPE_INT, &USBTester::epint_out_callback);
        endpoint_add(bulk_in, MAX_EP_SIZE, USB_EP_TYPE_BULK);
        endpoint_add(bulk_out, MAX_EP_SIZE, USB_EP_TYPE_BULK, &USBTester::epbulk_out_callback);

        read_start(int_out);
        read_start(bulk_out);

        complete_set_interface(true);
        return;
    }
    if (interface == 0 && alternate == 1) {
        endpoint_remove(int_in);
        endpoint_remove(int_out);
        endpoint_remove(bulk_in);
        endpoint_remove(bulk_out);

        endpoint_add(int_in, MIN_EP_SIZE, USB_EP_TYPE_INT);
        endpoint_add(int_out, MIN_EP_SIZE, USB_EP_TYPE_INT, &USBTester::epint_out_callback);
        endpoint_add(bulk_in, MIN_EP_SIZE, USB_EP_TYPE_BULK);
        endpoint_add(bulk_out, MIN_EP_SIZE, USB_EP_TYPE_BULK, &USBTester::epbulk_out_callback);

        read_start(int_out);
        read_start(bulk_out);

        complete_set_interface(true);
        return;
    }
    complete_set_interface(false);
}

const uint8_t *USBTester::device_desc()
{
    uint8_t ep0_size = endpoint_max_packet_size(0x00);
    uint8_t device_descriptor_temp[] = {
        18,                   // bLength
        1,                    // bDescriptorType
        0x10, 0x01,           // bcdUSB
        0,                    // bDeviceClass
        0,                    // bDeviceSubClass
        0,                    // bDeviceProtocol
        ep0_size,             // bMaxPacketSize0
        (uint8_t)(LSB(vendor_id)), (uint8_t)(MSB(vendor_id)),  // idVendor
        (uint8_t)(LSB(product_id)), (uint8_t)(MSB(product_id)),// idProduct
        0x00, 0x01,           // bcdDevice
        1,                    // iManufacturer
        2,                    // iProduct
        3,                    // iSerialNumber
        1                     // bNumConfigurations
    };
    MBED_ASSERT(sizeof(device_descriptor_temp) == sizeof(device_descriptor));
    memcpy(device_descriptor, device_descriptor_temp, sizeof(device_descriptor));
    return device_descriptor;
}

const uint8_t *USBTester::string_iinterface_desc()
{
    static const uint8_t string_iinterface_descriptor[] = {
        0x08,
        STRING_DESCRIPTOR,
        'C', 0, 'D', 0, 'C', 0,
    };
    return string_iinterface_descriptor;
}

const uint8_t *USBTester::string_iproduct_desc()
{
    static const uint8_t string_iproduct_descriptor[] = {
        0x16,
        STRING_DESCRIPTOR,
        'C', 0, 'D', 0, 'C', 0, ' ', 0, 'D', 0, 'E', 0, 'V', 0, 'I', 0, 'C', 0, 'E', 0
    };
    return string_iproduct_descriptor;
}


#define CONFIG1_DESC_SIZE (9+9+7+7+7+7 + 9+7+7+7+7)

const uint8_t *USBTester::configuration_desc()
{
    static const uint8_t config_descriptor[] = {
        // configuration descriptor
        9,                      // bLength
        2,                      // bDescriptorType
        LSB(CONFIG1_DESC_SIZE), // wTotalLength
        MSB(CONFIG1_DESC_SIZE),
        1,                      // bNumInterfaces
        1,                      // bConfigurationValue
        0,                      // iConfiguration
        0x80,                   // bmAttributes
        50,                     // bMaxPower

        // Interface 0 setting 0

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                          // bLength
        4,                          // bDescriptorType
        0,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        4,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_in,                    // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),// wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),// wMaxPacketSize (MSB)
        0,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),// wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),// wMaxPacketSize (MSB)
        0,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_in,                     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE), // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE), // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_out,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE), // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE), // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 0 setting 1

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                          // bLength
        4,                          // bDescriptorType
        0,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        4,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_in,                    // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        0,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        0,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_in,                     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_out,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        1                           // bInterval

    };
    return config_descriptor;
}


void USBTester::epint_out_callback(usb_ep_t endpoint)
{
    uint8_t buffer[65];
    uint32_t size = 0;

    if (!read_finish(endpoint, buffer, sizeof(buffer), &size)) {
        return;
    }
    if (!read_start(endpoint)) {
        return;
    }
}
void USBTester::epbulk_out_callback(usb_ep_t endpoint)
{
    uint8_t buffer[65];
    uint32_t size = 0;

    if (!read_finish(endpoint, buffer, sizeof(buffer), &size)) {
        return;
    }
    if (!read_start(endpoint)) {
        return;
    }
}

