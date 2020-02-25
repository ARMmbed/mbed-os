/*
 * Copyright (c) 2018-2020, ARM Limited, All Rights Reserved
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

#if USB_DEVICE_TESTS

#if defined(MBED_CONF_RTOS_PRESENT)

#include "stdint.h"
#include "USBTester.h"
#include "mbed_shared_queues.h"
#include "EndpointResolver.h"

#define DEFAULT_CONFIGURATION (1)
#define LAST_CONFIGURATION    (2)

#define VENDOR_TEST_CTRL_IN                 1
#define VENDOR_TEST_CTRL_OUT                2
#define VENDOR_TEST_CTRL_NONE               3
#define VENDOR_TEST_CTRL_IN_DELAY           4
#define VENDOR_TEST_CTRL_OUT_DELAY          5
#define VENDOR_TEST_CTRL_NONE_DELAY         6
#define VENDOR_TEST_CTRL_IN_STATUS_DELAY    7
#define VENDOR_TEST_CTRL_OUT_STATUS_DELAY   8
#define VENDOR_TEST_CTRL_IN_SIZES           9
#define VENDOR_TEST_CTRL_OUT_SIZES          10

#define MAX_EP_SIZE 64
#define MIN_EP_SIZE 8

#define CTRL_BUF_SIZE (2048)

#define EVENT_READY (1 << 0)


USBTester::USBTester(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBDevice(phy, vendor_id, product_id, product_release), interface_0_alt_set(NONE),
    interface_1_alt_set(NONE), configuration_set(NONE), reset_count(0),
    suspend_count(0), resume_count(0)
{

    EndpointResolver resolver(endpoint_table());

    resolver.endpoint_ctrl(64);
    bulk_in = resolver.endpoint_in(USB_EP_TYPE_BULK, 64);
    bulk_out = resolver.endpoint_out(USB_EP_TYPE_BULK, 64);
    int_in = resolver.endpoint_in(USB_EP_TYPE_INT, 64);
    int_out = resolver.endpoint_out(USB_EP_TYPE_INT, 64);
    MBED_ASSERT(resolver.valid());
    queue = mbed::mbed_highprio_event_queue();
    configuration_desc(0);
    ctrl_buf = new uint8_t[CTRL_BUF_SIZE];
    init();
    USBDevice::connect();
    flags.wait_any(EVENT_READY, osWaitForever, false);

}

USBTester::~USBTester()
{
    deinit();
    delete[] ctrl_buf;
}


const char *USBTester::get_desc_string(const uint8_t *desc)
{
    static char ret_string[128] = { };
    const uint8_t desc_size = desc[0] - 2;
    const uint8_t *desc_str = &desc[2];
    uint32_t j = 0;
    for (uint32_t i = 0; i < desc_size; i += 2, j++) {
        ret_string[j] = desc_str[i];
    }
    ret_string[j] = '\0';
    return ret_string;
}

void USBTester::suspend(bool suspended)
{
    if (suspended) {
        ++suspend_count;
    } else {
        ++resume_count;
    }
}

const char *USBTester::get_serial_desc_string()
{
    return get_desc_string(string_iserial_desc());
}

const char *USBTester::get_iinterface_desc_string()
{
    return get_desc_string(string_iserial_desc());
}

const char *USBTester::get_iproduct_desc_string()
{
    return get_desc_string(string_iserial_desc());
}

void USBTester::callback_state_change(DeviceState new_state)
{
    if (new_state == Configured) {
        flags.set(EVENT_READY);
    } else {
        flags.clear(EVENT_READY);
        configuration_set = NONE;
        interface_0_alt_set = NONE;
        interface_1_alt_set = NONE;
    }
}

void USBTester::callback_reset()
{
    ++reset_count;
}

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
                size = setup->wValue < CTRL_BUF_SIZE ? setup->wValue : CTRL_BUF_SIZE;
                break;
            case VENDOR_TEST_CTRL_OUT:
                result = Receive;
                data = ctrl_buf;
                size = setup->wValue < 8 ? setup->wValue : 8;
                break;
            case VENDOR_TEST_CTRL_NONE:
                result = Success;
                break;
            case VENDOR_TEST_CTRL_NONE_DELAY:
                result = Success;
                delay = 2000;
                break;
            case VENDOR_TEST_CTRL_IN_SIZES:
                result = Send;
                data = ctrl_buf;
                size = setup->wLength;
                break;
            case VENDOR_TEST_CTRL_OUT_SIZES:
                result = Receive;
                data = ctrl_buf;
                size = setup->wValue;
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

void USBTester::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
{
    if (aborted) {
        complete_request_xfer_done(false);
        return;
    }

    bool result = false;
    if (setup->bmRequestType.Type == VENDOR_TYPE) {
        switch (setup->bRequest) {
            case VENDOR_TEST_CTRL_IN:
                result = true;
                break;
            case VENDOR_TEST_CTRL_OUT:
                result = true;
                break;
            case VENDOR_TEST_CTRL_OUT_SIZES:
                result = true;
                break;
            case VENDOR_TEST_CTRL_IN_SIZES:
                result = true;
                break;
            default:
                result = false;
                break;
        }
    }
    complete_request_xfer_done(result);
}

// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported.
void USBTester::callback_set_configuration(uint8_t configuration)
{
    bool ret = false;

    if (configuration >= DEFAULT_CONFIGURATION && configuration <= LAST_CONFIGURATION) {
        endpoint_remove_all();
        ret = set_configuration(configuration);
    }

    complete_set_configuration(ret);
}

bool USBTester::setup_iterface(uint8_t ep_in, uint8_t ep_out, uint32_t ep_size, usb_ep_type_t ep_type,
                               uint8_t *buf, uint32_t buf_size, void (USBTester::*callback)())
{
    bool success = false;

    success = endpoint_add(ep_in, ep_size, ep_type);
    success &= endpoint_add(ep_out, ep_size, ep_type, callback);
    success &= read_start(ep_out, buf, buf_size);
    return success;
}

void USBTester::remove_iterface(uint16_t interface)
{
    if (configuration_set == 1) {
        if (interface == 0) {
            endpoint_remove(bulk_in);
            endpoint_remove(bulk_out);
            interface_0_alt_set = NONE;
        }
        if (interface == 1) {
            endpoint_remove(int_in);
            endpoint_remove(int_out);
            interface_1_alt_set = NONE;
        }
    }
    if (configuration_set == 2) {
        if (interface == 0) {
            endpoint_remove(int_in);
            endpoint_remove(int_out);
            interface_0_alt_set = NONE;
        }
        if (interface == 1) {
            endpoint_remove(bulk_in);
            endpoint_remove(bulk_out);
            interface_1_alt_set = NONE;
        }
    }
}

bool USBTester::set_configuration(uint16_t configuration)
{
    bool success = false;
    // set 0 alt setting for each interface
    if (configuration == 1) {
        // interface 0 alternate 0
        success = setup_iterface(bulk_in, bulk_out, MAX_EP_SIZE, USB_EP_TYPE_BULK,
                                 bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
        // interface 1 alternate 0
        success &= setup_iterface(int_in, int_out, MAX_EP_SIZE, USB_EP_TYPE_INT,
                                  int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
    } else if (configuration == 2) {
        // interface 0 alternate 0
        success = setup_iterface(int_in, int_out, MIN_EP_SIZE, USB_EP_TYPE_INT,
                                 int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
        // interface 1 alternate 0
        success &= setup_iterface(bulk_in, bulk_out, MIN_EP_SIZE, USB_EP_TYPE_BULK,
                                  bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
    }
    if (success) {
        configuration_set = configuration;
        interface_0_alt_set = interface_1_alt_set = 0;
    }
    return success;
}

void USBTester::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    complete_set_interface(set_interface(interface, alternate));
}

bool USBTester::set_interface(uint16_t interface, uint16_t alternate)
{
    bool success = false;

    if (interface == 0) {
        if (configuration_set == 1) {
            if (alternate == 0) {
                remove_iterface(interface);
                success = setup_iterface(bulk_in, bulk_out, MAX_EP_SIZE, USB_EP_TYPE_BULK,
                                         bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
            }
            if (alternate == 1) {
                remove_iterface(interface);
                success = setup_iterface(bulk_in, bulk_out, MIN_EP_SIZE, USB_EP_TYPE_BULK,
                                         bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
            }
        }
        if (configuration_set == 2) {
            if (alternate == 0) {
                remove_iterface(interface);
                success = setup_iterface(int_in, int_out, MIN_EP_SIZE, USB_EP_TYPE_INT,
                                         int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
            }
            if (alternate == 1) {
                remove_iterface(interface);
                success = setup_iterface(int_in, int_out, MAX_EP_SIZE, USB_EP_TYPE_INT,
                                         int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
            }
        }
        if (success) {
            interface_0_alt_set = alternate;
        }
    }
    if (interface == 1) {
        if (configuration_set == 1) {
            if (alternate == 0) {
                remove_iterface(interface);
                success = setup_iterface(int_in, int_out, MAX_EP_SIZE, USB_EP_TYPE_INT,
                                         int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
            }
            if (alternate == 1) {
                remove_iterface(interface);
                success = setup_iterface(int_in, int_out, MIN_EP_SIZE, USB_EP_TYPE_INT,
                                         int_buf, sizeof(int_buf), &USBTester::epint_out_callback);
            }
        }
        if (configuration_set == 2) {
            if (alternate == 0) {
                remove_iterface(interface);
                success = setup_iterface(bulk_in, bulk_out, MIN_EP_SIZE, USB_EP_TYPE_BULK,
                                         bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
            }
            if (alternate == 1) {
                remove_iterface(interface);
                success = setup_iterface(bulk_in, bulk_out, MAX_EP_SIZE, USB_EP_TYPE_BULK,
                                         bulk_buf, sizeof(bulk_buf), &USBTester::epbulk_out_callback);
            }
        }
        if (success) {
            interface_1_alt_set = alternate;
        }
    }
    return success;
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
        2                     // bNumConfigurations
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
        0x22,
        STRING_DESCRIPTOR,
        'M', 0, 'B', 0, 'E', 0, 'D', 0, ' ', 0,
        'T', 0, 'E', 0, 'S', 0, 'T', 0, ' ', 0,
        'D', 0, 'E', 0, 'V', 0, 'I', 0, 'C', 0, 'E', 0,
    };
    return string_iproduct_descriptor;
}


#define CONFIG_1_DESC_SIZE (9+9+7+7 + 9+7+7 + 9+7+7 + 9+7+7)
#define CONFIG_2_DESC_SIZE (9+9+7+7 + 9+7+7 + 9+7+7 + 9+7+7)

const uint8_t *USBTester::configuration_desc(uint8_t index)
{
    static const uint8_t config_1_descriptor[] = {
        // configuration descriptor
        CONFIGURATION_DESCRIPTOR_LENGTH,// bLength
        CONFIGURATION_DESCRIPTOR,   // bDescriptorType
        LSB(CONFIG_1_DESC_SIZE),    // wTotalLength
        MSB(CONFIG_1_DESC_SIZE),
        2,                          // bNumInterfaces
        1,                          // bConfigurationValue
        0,                          // iConfiguration
        0x80,                       // bmAttributes
        50,                         // bMaxPower

        // Interface 0 setting 0

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_in,                    // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 0 setting 1

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        2,                          // bNumEndpoints
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
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 1 setting 0

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_in,                     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_out,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 1 setting 1

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

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

    static const uint8_t config_2_descriptor[] = {
        // configuration descriptor
        CONFIGURATION_DESCRIPTOR_LENGTH,// bLength
        CONFIGURATION_DESCRIPTOR,   // bDescriptorType
        LSB(CONFIG_2_DESC_SIZE),    // wTotalLength
        MSB(CONFIG_2_DESC_SIZE),
        2,                          // bNumInterfaces
        2,                          // bConfigurationValue
        0,                          // iConfiguration
        0x80,                       // bmAttributes
        50,                         // bMaxPower

        // Interface 0 setting 0

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

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
        1,                          // bInterval

        // Interface 0 setting 1

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_in,                     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        int_out,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes (0x03=interrupt)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 1 setting 0

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        2,                          // bNumEndpoints
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
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MIN_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MIN_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // Interface 1 setting 1

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_in,                    // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        bulk_out,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        LSB(MAX_EP_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_EP_SIZE),           // wMaxPacketSize (MSB)
        1                           // bInterval
    };

    if (index == 0) {
        return config_1_descriptor;
    } else if (index == 1) {
        return config_2_descriptor;
    } else {
        return NULL;
    }
}

void USBTester::epint_out_callback()
{
    read_finish(int_out);
    read_start(int_out, int_buf, sizeof(int_buf));
}
void USBTester::epbulk_out_callback()
{
    read_finish(bulk_out);
    read_start(bulk_out, bulk_buf, sizeof(bulk_buf));
}
#endif
#endif //USB_DEVICE_TESTS
