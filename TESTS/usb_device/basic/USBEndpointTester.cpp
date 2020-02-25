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
#include "stdlib.h"
#include "USBEndpointTester.h"
#include "mbed_shared_queues.h"
#include "EndpointResolver.h"

#define DEFAULT_CONFIGURATION       (1)

#define NUM_PACKETS_UNTIL_ABORT     2
#define NUM_PACKETS_AFTER_ABORT     8
#define EP_ABORT_BUFF_VALUE         0xff

/* If the host ever receives a payload with any byte set to this value,
 * the device does not handle abort operation correctly. The buffer
 * passed to aborted operation must not be used after call to abort().
 */
#define FORBIDDEN_PAYLOAD_VALUE     (NUM_PACKETS_AFTER_ABORT + 1)

#define VENDOR_TEST_CTRL_IN             1
#define VENDOR_TEST_CTRL_OUT            2
#define VENDOR_TEST_CTRL_IN_SIZES       9
#define VENDOR_TEST_CTRL_OUT_SIZES      10
#define VENDOR_TEST_RW_RESTART          11
#define VENDOR_TEST_ABORT_BUFF_CHECK    12

#define CTRL_BUF_SIZE (2048)

#define EVENT_READY (1 << 0)

#define TEST_SIZE_EP_BULK_MAX   (64)
#define TEST_SIZE_EP_BULK_MIN   (8)
#define TEST_SIZE_EP_BULK_0     (16)
#define TEST_SIZE_EP_BULK_1     TEST_SIZE_EP_BULK_MAX
#define TEST_SIZE_EP_BULK_2     (32)
#define TEST_SIZE_EP_BULK_3     (16)
#define TEST_SIZE_EP_BULK_4     TEST_SIZE_EP_BULK_MIN

#define TEST_SIZE_EP_INT_MAX    (64)
#define TEST_SIZE_EP_INT_MIN    (1)
#define TEST_SIZE_EP_INT_0      (16)
#define TEST_SIZE_EP_INT_1      TEST_SIZE_EP_INT_MAX
#define TEST_SIZE_EP_INT_2      (32)
#define TEST_SIZE_EP_INT_3      (16)
#define TEST_SIZE_EP_INT_4      TEST_SIZE_EP_INT_MIN


/* According to USB spec, the wMaxPacketSize for FS isochronous endpoints
 * is 1023 B. There are a couple of reasons this value is not used in tests:
 * - some of the boards supported by Mbed OS have too little RAM dedicated
 *   for USB, making EndpointResolve::valid() fail when all the endpoints (2x
 *   bulk, 2x interrupt, 2x isochronous, 2x control) are configured to use
 *   the max value of wMaxPacketSize
 *   (e.g. NUCLEO_F207ZG has 1.25K of endpoint RAM),
 * - given a test host with other USB devices on the bus, it is unlikely
 *   for the test device to be able to reserve the bandwidth associated with
 *   high wMaxPacketSize for iso endpoints.
 */
#define TEST_SIZE_EP_ISO_MAX    (256)
#define TEST_SIZE_EP_ISO_MIN    (1)
#define TEST_SIZE_EP_ISO_0      (0)
#define TEST_SIZE_EP_ISO_1      (0)
#define TEST_SIZE_EP_ISO_2      (0)
#define TEST_SIZE_EP_ISO_3      (0)
#define TEST_SIZE_EP_ISO_4      (0)

#define EP_BULK_OUT 0
#define EP_BULK_IN  1
#define EP_INT_OUT  2
#define EP_INT_IN   3
#define EP_ISO_OUT  4
#define EP_ISO_IN   5

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config_max[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_out) },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_in) },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_out) },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_in) },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_out) },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_MAX, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_in) },
};

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config0[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_0, NULL },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_0, NULL },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_0, NULL },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_0, NULL },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_0, NULL },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_0, NULL },
};

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config1[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_out) },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_in) },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_out) },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_in) },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_out) },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_1, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_in) },
};

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config2[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_out) },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_in) },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_out) },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_in) },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_out) },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_2, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_in) },
};

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config3[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_out) },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_in) },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_out) },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_in) },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_out) },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_3, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_in) },
};

USBEndpointTester::ep_config_t USBEndpointTester::_intf_config4[NUM_ENDPOINTS] = {
    { false, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_out) },
    { true, USB_EP_TYPE_BULK, TEST_SIZE_EP_BULK_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_bulk_in) },
    { false, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_out) },
    { true, USB_EP_TYPE_INT, TEST_SIZE_EP_INT_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_int_in) },
    { false, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_out) },
    { true, USB_EP_TYPE_ISO, TEST_SIZE_EP_ISO_4, static_cast<ep_cb_t>(&USBEndpointTester::_cb_iso_in) },
};

USBEndpointTester::USBEndpointTester(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release,
                                     bool abort_transfer_test) :
    USBDevice(phy, vendor_id, product_id, product_release), _abort_transfer_test(abort_transfer_test), _endpoint_configs(
        &_intf_config_max)
{
    _cnt_cb_set_conf = 0;
    _cnt_cb_set_intf = 0;
    _cnt_cb_bulk_out = 0;
    _cnt_cb_bulk_in = 0;
    _cnt_cb_int_out = 0;
    _cnt_cb_int_in = 0;
    _cnt_cb_iso_out = 0;
    _cnt_cb_iso_in = 0;
    _num_packets_bulk_out_abort = 0;
    _num_packets_bulk_in_abort = 0;
    _num_packets_int_out_abort = 0;
    _num_packets_int_in_abort = 0;

    EndpointResolver resolver(endpoint_table());
    resolver.endpoint_ctrl(64);
    ep_config_t *epc = NULL;
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        epc = &((*_endpoint_configs)[i]);
        _endpoints[i] = resolver.next_free_endpoint(epc->dir_in, epc->type, epc->max_packet);
        _endpoint_buffs[i] = (uint8_t *) calloc(epc->max_packet, sizeof(uint8_t));
        MBED_ASSERT(_endpoint_buffs[i] != NULL);
    }
    MBED_ASSERT(resolver.valid());
    queue = mbed::mbed_highprio_event_queue();
    configuration_desc(0);
    ctrl_buf = new uint8_t[CTRL_BUF_SIZE];
    init();
    USBDevice::connect();
    flags.wait_any(EVENT_READY, osWaitForever, false);
}

USBEndpointTester::~USBEndpointTester()
{
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        if (_endpoint_buffs[i] != NULL) {
            free(_endpoint_buffs[i]);
        }
    }
    deinit();
    delete[] ctrl_buf;
}

const char *USBEndpointTester::get_desc_string(const uint8_t *desc)
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

const char *USBEndpointTester::get_serial_desc_string()
{
    return get_desc_string(string_iserial_desc());
}

void USBEndpointTester::callback_state_change(DeviceState new_state)
{
    if (new_state == Configured) {
        flags.set(EVENT_READY);
    } else {
        flags.clear(EVENT_READY);
    }
}

void USBEndpointTester::callback_request(const setup_packet_t *setup)
{
    /* Called in ISR context */
    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;

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
            case VENDOR_TEST_RW_RESTART:
                result = (_request_rw_restart(setup)) ? Success : Failure;
                break;
            case VENDOR_TEST_ABORT_BUFF_CHECK:
                result = Send;
                ctrl_buf[0] = _request_abort_buff_check(setup);
                data = ctrl_buf;
                size = 1;
                break;
            default:
                result = PassThrough;
                break;
        }
    }
    complete_request(result, data, size);
}

bool USBEndpointTester::_request_rw_restart(const setup_packet_t *setup)
{
    assert_locked();
    ep_config_t *epc = NULL;
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        epc = &((*_endpoint_configs)[i]);
        endpoint_abort(_endpoints[i]);
        if (epc->dir_in == false) {
            // Wait for data on every OUT endpoint
            read_start(_endpoints[i], _endpoint_buffs[i], epc->max_packet);
        }
    }
    return true;
}

bool USBEndpointTester::_request_abort_buff_check(const setup_packet_t *setup)
{
    assert_locked();
    if (setup->bmRequestType.Recipient != ENDPOINT_RECIPIENT) {
        return false;
    }
    size_t ep_index = NUM_ENDPOINTS;
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        if (_endpoints[i] == setup->wIndex) {
            ep_index = i;
            break;
        }
    }
    if (ep_index == NUM_ENDPOINTS) {
        return false;
    }
    if (_endpoint_buffs[ep_index] == NULL) {
        return false;
    }
    for (size_t i = 0; i < (*_endpoint_configs)[ep_index].max_packet; i++) {
        if (_endpoint_buffs[ep_index][i] != EP_ABORT_BUFF_VALUE) {
            return false;
        }
    }
    return true;
}

void USBEndpointTester::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
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
            case VENDOR_TEST_ABORT_BUFF_CHECK:
                result = true;
                break;
            default:
                result = false;
                break;
        }
    }
    complete_request_xfer_done(result);
}

void USBEndpointTester::callback_set_configuration(uint8_t configuration)
{
    _cnt_cb_set_conf++;
    if (configuration != DEFAULT_CONFIGURATION) {
        complete_set_configuration(false);
        return;
    }

    // Configure endpoints > 0
    bool status = _setup_interface(0, 0);
    complete_set_configuration(status);
}

bool USBEndpointTester::_setup_interface(uint16_t interface, uint8_t alternate)
{
    if (interface != 0) {
        return false;
    }

    switch (alternate) {
        case 0:
            _endpoint_configs = &_intf_config0;
            break;
        case 1:
            _endpoint_configs = &_intf_config1;
            break;
        case 2:
            _endpoint_configs = &_intf_config2;
            break;
        case 3:
            _endpoint_configs = &_intf_config3;
            break;
        case 4:
            _endpoint_configs = &_intf_config4;
            break;
        default:
            return false;
    }

    _setup_non_zero_endpoints();

    if (_abort_transfer_test && alternate >= 1) {
        _num_packets_bulk_out_abort = 0;
        _num_packets_bulk_in_abort = 0;
        _num_packets_int_out_abort = 0;
        _num_packets_int_in_abort = 0;
        start_ep_in_abort_test();
    }
    return true;
}

void USBEndpointTester::_setup_non_zero_endpoints()
{
    ep_config_t *epc = NULL;
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        epc = &((*_endpoint_configs)[i]);
        endpoint_add(_endpoints[i], epc->max_packet, epc->type, epc->callback);
        if (epc->callback == NULL) {
            continue;
        }
        if (epc->dir_in == false) {
            // Wait for data on every OUT endpoint
            read_start(_endpoints[i], _endpoint_buffs[i], epc->max_packet);
        }
    }
}

void USBEndpointTester::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    _cnt_cb_set_intf++;
    if (interface != 0 || alternate > 4) {
        complete_set_interface(false);
        return;
    }
    for (size_t i = 0; i < NUM_ENDPOINTS; i++) {
        endpoint_abort(_endpoints[i]);
        endpoint_remove(_endpoints[i]);
    }
    bool status = _setup_interface(interface, alternate);
    complete_set_interface(status);
}

#define CONFIG1_DESC_SIZE (CONFIGURATION_DESCRIPTOR_LENGTH \
        + 5 * (INTERFACE_DESCRIPTOR_LENGTH + NUM_ENDPOINTS * ENDPOINT_DESCRIPTOR_LENGTH) )

const uint8_t *USBEndpointTester::configuration_desc(uint8_t index)
{
    static const uint8_t config_1_descriptor[] = {
        // configuration descriptor
        CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
        CONFIGURATION_DESCRIPTOR,           // bDescriptorType
        LSB(CONFIG1_DESC_SIZE),             // wTotalLength (LSB)
        MSB(CONFIG1_DESC_SIZE),             // wTotalLength (MSB)
        1,                                  // bNumInterfaces
        1,                                  // bConfigurationValue
        0,                                  // iConfiguration
        0x80,                               // bmAttributes
        50,                                 // bMaxPower

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        NUM_ENDPOINTS,              // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_OUT],    // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_IN],     // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_BULK_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_BULK_IN].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_OUT],     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_INT_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_INT_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_IN],      // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_INT_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_INT_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_OUT],     // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_IN],      // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config0[EP_ISO_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config0[EP_ISO_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        NUM_ENDPOINTS,              // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_OUT],    // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_IN],     // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_BULK_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_BULK_IN].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_OUT],     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_INT_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_INT_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_IN],      // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_INT_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_INT_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_OUT],     // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_IN],      // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config1[EP_ISO_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config1[EP_ISO_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        2,                          // bAlternateSetting
        NUM_ENDPOINTS,              // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_OUT],    // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_IN],     // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_BULK_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_BULK_IN].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_OUT],     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_INT_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_INT_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_IN],      // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_INT_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_INT_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_OUT],     // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_IN],      // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config2[EP_ISO_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config2[EP_ISO_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        3,                          // bAlternateSetting
        NUM_ENDPOINTS,              // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_OUT],    // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_IN],     // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_BULK_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_BULK_IN].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_OUT],     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_INT_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_INT_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_IN],      // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_INT_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_INT_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_OUT],     // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_IN],      // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config3[EP_ISO_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config3[EP_ISO_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval

        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        INTERFACE_DESCRIPTOR_LENGTH,// bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        0,                          // bInterfaceNumber
        4,                          // bAlternateSetting
        NUM_ENDPOINTS,              // bNumEndpoints
        0xFF,                       // bInterfaceClass
        0xFF,                       // bInterfaceSubClass
        0xFF,                       // bInterfaceProtocol
        0,                          // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_OUT],    // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_BULK_OUT].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_BULK_IN],     // bEndpointAddress
        E_BULK,                     // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_BULK_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_BULK_IN].max_packet)),  // wMaxPacketSize (MSB)
        0,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_OUT],     // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_INT_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_INT_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_INT_IN],      // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_INT_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_INT_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_OUT],     // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_ISO_OUT].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _endpoints[EP_ISO_IN],      // bEndpointAddress
        E_ISOCHRONOUS,              // bmAttributes
        (uint8_t)(LSB(_intf_config4[EP_ISO_IN].max_packet)),  // wMaxPacketSize (LSB)
        (uint8_t)(MSB(_intf_config4[EP_ISO_IN].max_packet)),  // wMaxPacketSize (MSB)
        1,                          // bInterval
    };
    if (index == 0) {
        return config_1_descriptor;
    } else {
        return NULL;
    }
}

void USBEndpointTester::_cb_bulk_out()
{
    _cnt_cb_bulk_out++;
    uint32_t rx_size = read_finish(_endpoints[EP_BULK_OUT]);
    if (_abort_transfer_test == false) {
        // Send data back to host using the IN endpoint.
        memset(_endpoint_buffs[EP_BULK_IN], 0, (*_endpoint_configs)[EP_BULK_IN].max_packet);
        memcpy(_endpoint_buffs[EP_BULK_IN], _endpoint_buffs[EP_BULK_OUT], rx_size);
        write_start(_endpoints[EP_BULK_IN], _endpoint_buffs[EP_BULK_IN], rx_size);
    } else {
        // Abort the transfer if enough data was received.
        _num_packets_bulk_out_abort++;
        if (_num_packets_bulk_out_abort == NUM_PACKETS_UNTIL_ABORT) {
            // Set every byte of the buffer to a known value.
            memset(_endpoint_buffs[EP_BULK_OUT], EP_ABORT_BUFF_VALUE, (*_endpoint_configs)[EP_BULK_OUT].max_packet);
        }
        read_start(_endpoints[EP_BULK_OUT], _endpoint_buffs[EP_BULK_OUT], (*_endpoint_configs)[EP_BULK_OUT].max_packet);
        if (_num_packets_bulk_out_abort == NUM_PACKETS_UNTIL_ABORT) {
            endpoint_abort(_endpoints[EP_BULK_OUT]);
        }
    }
}

void USBEndpointTester::_cb_bulk_in()
{
    _cnt_cb_bulk_in++;
    write_finish(_endpoints[EP_BULK_IN]);
    if (_abort_transfer_test == false) {
        // Receive more data from the host using the OUT endpoint.
        read_start(_endpoints[EP_BULK_OUT], _endpoint_buffs[EP_BULK_OUT], (*_endpoint_configs)[EP_BULK_OUT].max_packet);
    } else {
        _num_packets_bulk_in_abort++;
        if (_num_packets_bulk_in_abort >= NUM_PACKETS_UNTIL_ABORT + NUM_PACKETS_AFTER_ABORT) {
            return;
        }
        // Abort the transfer if enough data was sent.
        memset(_endpoint_buffs[EP_BULK_IN], _num_packets_bulk_in_abort, (*_endpoint_configs)[EP_BULK_IN].max_packet);
        write_start(_endpoints[EP_BULK_IN], _endpoint_buffs[EP_BULK_IN], (*_endpoint_configs)[EP_BULK_IN].max_packet);
        if (_num_packets_bulk_in_abort == NUM_PACKETS_UNTIL_ABORT) {
            endpoint_abort(_endpoints[EP_BULK_IN]);
            // Verify that buffer given in write_start is not used after the
            // call to endpoint_abort(), by changing the buffer contents.
            // The test will fail if the host receives new buffer content.
            memset(_endpoint_buffs[EP_BULK_IN], FORBIDDEN_PAYLOAD_VALUE, (*_endpoint_configs)[EP_BULK_IN].max_packet);
        }
    }
}

void USBEndpointTester::_cb_int_out()
{
    _cnt_cb_int_out++;
    uint32_t rx_size = read_finish(_endpoints[EP_INT_OUT]);
    if (_abort_transfer_test == false) {
        // Send data back to host using the IN endpoint.
        memset(_endpoint_buffs[EP_INT_IN], 0, (*_endpoint_configs)[EP_INT_IN].max_packet);
        memcpy(_endpoint_buffs[EP_INT_IN], _endpoint_buffs[EP_INT_OUT], rx_size);
        write_start(_endpoints[EP_INT_IN], _endpoint_buffs[EP_INT_IN], rx_size);
    } else {
        // Abort the transfer if enough data was received.
        _num_packets_int_out_abort++;
        if (_num_packets_int_out_abort == NUM_PACKETS_UNTIL_ABORT) {
            // Set every byte of the buffer to a known value.
            memset(_endpoint_buffs[EP_INT_OUT], EP_ABORT_BUFF_VALUE, (*_endpoint_configs)[EP_INT_OUT].max_packet);
        }
        read_start(_endpoints[EP_INT_OUT], _endpoint_buffs[EP_INT_OUT], (*_endpoint_configs)[EP_INT_OUT].max_packet);
        if (_num_packets_int_out_abort == NUM_PACKETS_UNTIL_ABORT) {
            endpoint_abort(_endpoints[EP_INT_OUT]);
        }
    }
}

void USBEndpointTester::_cb_int_in()
{
    _cnt_cb_int_in++;
    write_finish(_endpoints[EP_INT_IN]);
    if (_abort_transfer_test == false) {
        // Receive more data from the host using the OUT endpoint.
        read_start(_endpoints[EP_INT_OUT], _endpoint_buffs[EP_INT_OUT], (*_endpoint_configs)[EP_INT_OUT].max_packet);
    } else {
        _num_packets_int_in_abort++;
        if (_num_packets_int_in_abort >= NUM_PACKETS_UNTIL_ABORT + NUM_PACKETS_AFTER_ABORT) {
            return;
        }
        // Abort the transfer if enough data was sent.
        memset(_endpoint_buffs[EP_INT_IN], _num_packets_int_in_abort, (*_endpoint_configs)[EP_INT_IN].max_packet);
        write_start(_endpoints[EP_INT_IN], _endpoint_buffs[EP_INT_IN], (*_endpoint_configs)[EP_INT_IN].max_packet);
        if (_num_packets_int_in_abort == NUM_PACKETS_UNTIL_ABORT) {
            endpoint_abort(_endpoints[EP_INT_IN]);
            // Verify that buffer given in write_start is not used after the
            // call to endpoint_abort(), by changing the buffer contents.
            // The test will fail if the host receives new buffer content.
            memset(_endpoint_buffs[EP_INT_IN], FORBIDDEN_PAYLOAD_VALUE, (*_endpoint_configs)[EP_INT_IN].max_packet);
        }
    }
}

void USBEndpointTester::_cb_iso_out()
{
    _cnt_cb_iso_out++;
    uint32_t rx_size = read_finish(_endpoints[EP_ISO_OUT]);
    // Send data back to host using the IN endpoint.
    memset(_endpoint_buffs[EP_ISO_IN], 0, (*_endpoint_configs)[EP_ISO_IN].max_packet);
    memcpy(_endpoint_buffs[EP_ISO_IN], _endpoint_buffs[EP_ISO_OUT], rx_size);
    write_start(_endpoints[EP_ISO_IN], _endpoint_buffs[EP_ISO_IN], rx_size);
}

void USBEndpointTester::_cb_iso_in()
{
    _cnt_cb_iso_in++;
    write_finish(_endpoints[EP_ISO_IN]);
    // Receive more data from the host using the OUT endpoint.
    read_start(_endpoints[EP_ISO_OUT], _endpoint_buffs[EP_ISO_OUT], (*_endpoint_configs)[EP_ISO_OUT].max_packet);
}

void USBEndpointTester::start_ep_in_abort_test()
{
    memset(_endpoint_buffs[EP_BULK_IN], 0, (*_endpoint_configs)[EP_BULK_IN].max_packet);
    memset(_endpoint_buffs[EP_INT_IN], 0, (*_endpoint_configs)[EP_INT_IN].max_packet);

    write_start(_endpoints[EP_BULK_IN], _endpoint_buffs[EP_BULK_IN], (*_endpoint_configs)[EP_BULK_IN].max_packet);
    write_start(_endpoints[EP_INT_IN], _endpoint_buffs[EP_INT_IN], (*_endpoint_configs)[EP_INT_IN].max_packet);
}
#endif
#endif //USB_DEVICE_TESTS
