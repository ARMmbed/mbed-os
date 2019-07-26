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

#include <stdint.h>
#include "USBCDC_ECM.h"
#include "EndpointResolver.h"
#include "usb_phy_api.h"
#include "mbed_interface.h"
#include "mbed_assert.h"

#ifndef MAX_SEGMENT_SIZE
#define MAX_SEGMENT_SIZE        (1514)
#endif

#define FLAG_WRITE_DONE         (1 << 0)
#define FLAG_DISCONNECT         (1 << 1)
#define FLAG_CONNECT            (1 << 2)
#define FLAG_INT_DONE           (1 << 3)

#define SET_ETHERNET_MULTICAST_FILTERS                  0x40
#define SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER    0x41
#define GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER    0x42
#define SET_ETHERNET_PACKET_FILTER                      0x43
#define GET_ETHERNET_STATISTIC                          0x44

#define CS_INTERFACE                0x24
#define NETWORK_CONNECTION          0x00
#define CONNECTION_SPEED_CHANGE     0x2A
#define LINK_SPEED                  (10000000)

USBCDC_ECM::USBCDC_ECM(bool connect_blocking, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(get_usb_phy(), vendor_id, product_id, product_release), _packet_filter(0), _queue(4 * EVENTS_EVENT_SIZE)
{
    _init();

    if (connect_blocking) {
        init();
        USBDevice::connect();
        wait_ready();
    } else {
        init();
    }
}

USBCDC_ECM::USBCDC_ECM(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(phy, vendor_id, product_id, product_release), _packet_filter(0), _queue(4 * EVENTS_EVENT_SIZE)
{

    _init();
}

USBCDC_ECM::~USBCDC_ECM()
{
    deinit();
}

void USBCDC_ECM::_init()
{
    EndpointResolver resolver(endpoint_table());
    resolver.endpoint_ctrl(MAX_PACKET_SIZE_EP0);
    _int_in = resolver.endpoint_in(USB_EP_TYPE_INT, MAX_PACKET_SIZE_INT);
    _bulk_in = resolver.endpoint_in(USB_EP_TYPE_BULK, MAX_PACKET_SIZE_BULK);
    _bulk_out = resolver.endpoint_out(USB_EP_TYPE_BULK, MAX_PACKET_SIZE_BULK);

    MBED_ASSERT(resolver.valid());

    _thread.start(callback(&_queue, &events::EventQueue::dispatch_forever));
    _rx_queue.resize(MAX_SEGMENT_SIZE);
}

void USBCDC_ECM::callback_reset()
{
    assert_locked();
    /* Called in ISR context */
}

void USBCDC_ECM::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
{
    assert_locked();
    /* Called in ISR context */

    complete_request_xfer_done(false);
}

void USBCDC_ECM::callback_set_configuration(uint8_t configuration)
{
    assert_locked();
    /* Called in ISR context */

    bool ret = false;
    if (configuration == DEFAULT_CONFIGURATION) {
        ret = true;
    }

    complete_set_configuration(ret);
}

bool USBCDC_ECM::ready()
{
    return _flags.get() & FLAG_CONNECT ? true : false;
}

void USBCDC_ECM::wait_ready()
{
    _flags.wait_any(FLAG_CONNECT, osWaitForever, false);
}

bool USBCDC_ECM::_notify_network_connection(uint8_t value)
{
    _write_mutex.lock();

    bool ret = true;
    uint8_t request[8] = {0};

    request[0] = 0xA1;
    request[1] = NETWORK_CONNECTION;
    request[2] = value;
    request[3] = 0x00;

    _flags.clear(FLAG_INT_DONE);
    USBDevice::write_start(_int_in, request, sizeof(request));
    uint32_t flags = _flags.wait_any(FLAG_INT_DONE | FLAG_DISCONNECT, osWaitForever, false);
    if (flags & FLAG_DISCONNECT) {
        ret = false;
    }
    USBDevice::write_finish(_int_in);

    _write_mutex.unlock();
    return ret;
}

bool USBCDC_ECM::_notify_connection_speed_change(uint32_t up, uint32_t down)
{
    _write_mutex.lock();

    bool ret = true;
    struct notification_t {
        uint8_t request[8];
        uint32_t up;
        uint32_t down;
    };

    notification_t notification;
    memset(&notification, 0, sizeof(notification));

    notification.request[0] = 0xA1;
    notification.request[1] = CONNECTION_SPEED_CHANGE;
    notification.request[2] = 0x00;
    notification.request[3] = 0x00;
    notification.request[6] = 0x08;
    notification.up = up;
    notification.down = down;

    _flags.clear(FLAG_INT_DONE);
    USBDevice::write_start(_int_in, (uint8_t *)&notification, sizeof(notification));
    uint32_t flags = _flags.wait_any(FLAG_INT_DONE | FLAG_DISCONNECT, osWaitForever, false);
    if (flags & FLAG_DISCONNECT) {
        ret = false;
    }
    USBDevice::write_finish(_int_in);

    _write_mutex.unlock();
    return ret;
}

void USBCDC_ECM::_notify_connect()
{
    _notify_network_connection(1);
    _notify_connection_speed_change(LINK_SPEED, LINK_SPEED);
}

bool USBCDC_ECM::_write_bulk(uint8_t *buffer, uint32_t size)
{
    bool ret = true;

    _flags.clear(FLAG_WRITE_DONE);
    USBDevice::write_start(_bulk_in, buffer, size);
    uint32_t flags = _flags.wait_any(FLAG_WRITE_DONE | FLAG_DISCONNECT, osWaitForever, false);
    if (flags & FLAG_DISCONNECT) {
        ret = false;
    }

    USBDevice::write_finish(_bulk_in);
    return ret;
}

bool USBCDC_ECM::send(uint8_t *buffer, uint32_t size)
{
    _write_mutex.lock();
    bool ret = true;
    uint32_t sent = 0;
    uint32_t data_size = 0;

    if (size > MAX_SEGMENT_SIZE) {
        _write_mutex.unlock();
        mbed_error_printf("Buffer size is too large\n");
        return false;
    }

    uint32_t max_packet = USBDevice::endpoint_max_packet_size(_bulk_in);

    while (size - sent > 0) {
        data_size = (size - sent > max_packet) ? max_packet : size - sent;
        if (_write_bulk(buffer + sent, data_size)) {
            sent += data_size;
        } else {
            _write_mutex.unlock();
            return false;
        }
    }

    /* Send zero length packet */
    if (size % max_packet == 0) {
        uint8_t buf = 0;
        ret = _write_bulk(&buf, 0);
    }

    _write_mutex.unlock();
    return ret;
}

void USBCDC_ECM::receive_nb(uint8_t *buffer, uint32_t size, uint32_t *actual)
{
    lock();

    uint32_t available = _rx_queue.size();
    uint32_t copy_size = available > size ? size : available;
    _rx_queue.read(buffer, copy_size);
    *actual = copy_size;

    unlock();
}

void USBCDC_ECM::attach_rx(mbed::Callback<void()> cb)
{
    lock();

    _callback_rx = cb;

    unlock();
}

void USBCDC_ECM::attach_filter(mbed::Callback<void()> cb)
{
    lock();

    _callback_filter = cb;

    unlock();
}

uint16_t USBCDC_ECM::read_packet_filter()
{
    return _packet_filter;
}

void USBCDC_ECM::callback_request(const setup_packet_t *setup)
{
    assert_locked();

    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;

    if (setup->bmRequestType.Type == CLASS_TYPE) {
        //printf("In USBCallback_request: CLASS specific Request: %02x\n", setup->bRequest);
        switch (setup->bRequest) {
            case SET_ETHERNET_MULTICAST_FILTERS:
                /* TODO: Support is optional, not implemented here */
                break;
            case SET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER:
                /* TODO: Support is optional, not implemented here */
                break;
            case GET_ETHERNET_POWER_MANAGEMENT_PATTERN_FILTER:
                /* TODO: Support is optional, not implemented here */
                break;
            case SET_ETHERNET_PACKET_FILTER:
                if (_packet_filter != setup->wValue) {
                    _packet_filter = setup->wValue;
                    // Signal that packet filter configuration is changed
                    if (_callback_filter) {
                        _callback_filter();
                    }
                }
                result = Success;
                break;
            case GET_ETHERNET_STATISTIC:
                /* TODO: Support is optional, not implemented here */
                break;
            default:
                result = Failure;
                break;
        }
    }

    complete_request(result, data, size);
}

void USBCDC_ECM::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    assert_locked();
    /* Called in ISR context */

    if (alternate) {
        _packet_filter = 0;

        endpoint_add(_int_in, MAX_PACKET_SIZE_INT, USB_EP_TYPE_INT, &USBCDC_ECM::_int_callback);
        endpoint_add(_bulk_in, MAX_PACKET_SIZE_BULK, USB_EP_TYPE_BULK, &USBCDC_ECM::_bulk_in_callback);
        endpoint_add(_bulk_out, MAX_PACKET_SIZE_BULK, USB_EP_TYPE_BULK, &USBCDC_ECM::_bulk_out_callback);

        read_start(_bulk_out, _bulk_buf, MAX_PACKET_SIZE_BULK);

        _queue.call(static_cast<USBCDC_ECM *>(this), &USBCDC_ECM::_notify_connect);
    }

    complete_set_interface(true);
}

void USBCDC_ECM::callback_state_change(DeviceState new_state)
{
    assert_locked();
    /* Called in ISR context */

    if (new_state == Configured) {
        _flags.set(FLAG_CONNECT);
        _flags.clear(FLAG_DISCONNECT);
    } else {
        _flags.set(FLAG_DISCONNECT);
        _flags.clear(FLAG_CONNECT | FLAG_WRITE_DONE | FLAG_INT_DONE);
    }
}

const uint8_t *USBCDC_ECM::device_desc()
{
    uint8_t ep0_size = endpoint_max_packet_size(0x00);
    uint8_t device_descriptor_temp[] = {
        DEVICE_DESCRIPTOR_LENGTH,       // bLength
        DEVICE_DESCRIPTOR,              // bDescriptorType
        0x00, 0x02,                     // bcdUSB 2.0
        0x02,                           // bDeviceClass
        0x00,                           // bDeviceSubClass
        0x00,                           // bDeviceProtocol
        ep0_size,                       // bMaxPacketSize0
        (uint8_t)(LSB(vendor_id)),
        (uint8_t)(MSB(vendor_id)),      // idVendor
        (uint8_t)(LSB(product_id)),
        (uint8_t)(MSB(product_id)),     // idProduct
        (uint8_t)(LSB(product_release)),
        (uint8_t)(MSB(product_release)),// bcdDevice
        STRING_OFFSET_IMANUFACTURER,    // iManufacturer
        STRING_OFFSET_IPRODUCT,         // iProduct
        STRING_OFFSET_ISERIAL,          // iSerialNumber
        0x01                            // bNumConfigurations
    };
    MBED_ASSERT(sizeof(device_descriptor_temp) == sizeof(device_descriptor));
    memcpy(device_descriptor, device_descriptor_temp, sizeof(device_descriptor));
    return device_descriptor;
}

const uint8_t *USBCDC_ECM::string_iproduct_desc()
{
    static const uint8_t string_iproduct_descriptor[] = {
        26,
        STRING_DESCRIPTOR,
        'U', 0, 'S', 0, 'B', 0, ' ', 0, 'E', 0, 't', 0, 'h', 0, 'e', 0, 'r', 0, 'n', 0, 'e', 0, 't', 0
    };
    return string_iproduct_descriptor;
}

const uint8_t *USBCDC_ECM::string_iconfiguration_desc()
{
    uint8_t string_imac_addr_temp[26] = {0};
    const char unicodes[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                             'A', 'B', 'C', 'D', 'E', 'F'
                            };
    char mac[6];

    mbed_mac_address(mac);

    string_imac_addr_temp[0] = 26;
    string_imac_addr_temp[1] = STRING_DESCRIPTOR;
    /* Convert MAC address to USB CDC string format */
    for (int i = 0; i < 6; i++) {
        string_imac_addr_temp[i * 4 + 2] = unicodes[mac[i] >> 4];
        string_imac_addr_temp[i * 4 + 4] = unicodes[mac[i] & 0xF];
    }

    MBED_ASSERT(sizeof(string_imac_addr_temp) == sizeof(_string_imac_addr));
    memcpy(_string_imac_addr, string_imac_addr_temp, sizeof(string_imac_addr_temp));
    return _string_imac_addr;
}

const uint8_t *USBCDC_ECM::string_iserial_desc()
{
    static const uint8_t string_iserial_descriptor[] = {
        26,
        STRING_DESCRIPTOR,
        '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0, '9', 0, 'A', 0, 'B', 0
    };
    return string_iserial_descriptor;
}

#define CONFIG_DESC_SIZE (9+9+5+5+13+7+9+9+7+7)

const uint8_t *USBCDC_ECM::configuration_desc(uint8_t index)
{
    if (index != 0) {
        return NULL;
    }

    uint8_t config_descriptor_temp[] = {
        // configuration descriptor, USB spec 9.6.3, page 264-265, Table 9-10
        0x09,                           // bLength
        CONFIGURATION_DESCRIPTOR,       // bDescriptorType
        LSB(CONFIG_DESC_SIZE),          // wTotalLength (LSB)
        MSB(CONFIG_DESC_SIZE),          // wTotalLength (MSB)
        2,                              // bNumInterfaces
        1,                              // bConfigurationValue
        0,                              // iConfiguration
        0xC0,                           // bmAttributes
        50,                             // bMaxPower

        // Communication interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        0x09,                   // bLength
        INTERFACE_DESCRIPTOR,   // bDescriptorType
        0,                      // bInterfaceNumber
        0,                      // bAlternateSetting
        1,                      // bNumEndpoints
        0x02,                   // bInterfaceClass
        0x06,                   // bInterfaceSubClass
        0x00,                   // bInterfaceProtocol
        0,                      // iInterface

        // CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 15
        0x05,                   // bFunctionLength
        CS_INTERFACE,           // bDescriptorType
        0x00,                   // bDescriptorSubtype
        0x20, 0x01,             // bcdCDC CDC 1.20

        // CDC Union Functional Descriptor, CDC Spec 5.2.3.2, Table 16
        0x05,                   // bFunctionLength
        CS_INTERFACE,           // bDescriptorType
        0x06,                   // bDescriptorSubType
        0,                      // bControlInterface
        1,                      // bSubordinateInterface0

        // CDC Ethernet Networking Functional Descriptor, ECM Spec 5.4, Table 3
        0x0D,                           // bFunctionLenght
        CS_INTERFACE,                   // bDescriptorType
        0x0F,                           // bDescriptorSubtype
        STRING_OFFSET_ICONFIGURATION,   // iMacAddress
        0, 0, 0, 0,                     // bmEthernetStatistics
        (uint8_t) LSB(MAX_SEGMENT_SIZE),    // wMaxSegmentSize (LSB)
        (uint8_t) MSB(MAX_SEGMENT_SIZE),    // wMaxSegmentSize (MSB)
        0, 0,                           // wNumberMCFilters
        0,                              // bNumberPowerFilters

        // Endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH,     // bLength
        ENDPOINT_DESCRIPTOR,            // bDescriptorType
        _int_in,                        // bEndpointAddress
        E_INTERRUPT,                    // bmAttributes (0x03=intr)
        (uint8_t) LSB(MAX_PACKET_SIZE_INT),     // wMaxPacketSize (LSB)
        (uint8_t) MSB(MAX_PACKET_SIZE_INT),     // wMaxPacketSize (MSB)
        16,                             // bInterval

        // Default data interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        0x09,                       // bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        0,                          // bAlternateSetting
        0,                          // bNumEndpoints
        0x0A,                       // bInterfaceClass
        0x00,                       // bInterfaceSubClass
        0x00,                       // bInterfaceProtocol
        0,                          // iInterface

        // Data interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        0x09,                       // bLength
        INTERFACE_DESCRIPTOR,       // bDescriptorType
        1,                          // bInterfaceNumber
        1,                          // bAlternateSetting
        2,                          // bNumEndpoints
        0x0A,                       // bInterfaceClass
        0x00,                       // bInterfaceSubClass
        0x00,                       // bInterfaceProtocol
        0,                          // iInterface

        // Endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _bulk_in,                   // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        (uint8_t) LSB(MAX_PACKET_SIZE_BULK),  // wMaxPacketSize (LSB)
        (uint8_t) MSB(MAX_PACKET_SIZE_BULK),  // wMaxPacketSize (MSB)
        0,                          // bInterval

        // Endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _bulk_out,                  // bEndpointAddress
        E_BULK,                     // bmAttributes (0x02=bulk)
        (uint8_t) LSB(MAX_PACKET_SIZE_BULK),  // wMaxPacketSize (LSB)
        (uint8_t) MSB(MAX_PACKET_SIZE_BULK),  // wMaxPacketSize (MSB)
        0                           // bInterval
    };

    MBED_ASSERT(sizeof(config_descriptor_temp) == sizeof(_config_descriptor));
    memcpy(_config_descriptor, config_descriptor_temp, sizeof(config_descriptor_temp));
    return _config_descriptor;
}

void USBCDC_ECM::_int_callback()
{
    assert_locked();

    _flags.set(FLAG_INT_DONE);
}

void USBCDC_ECM::_bulk_in_callback()
{
    assert_locked();

    _flags.set(FLAG_WRITE_DONE);
}

void USBCDC_ECM::_bulk_out_callback()
{
    assert_locked();

    uint32_t read_size = read_finish(_bulk_out);

    if (read_size <= _rx_queue.free()) {
        // Copy data over
        _rx_queue.write(_bulk_buf, read_size);
    }

    // Signal that there is ethernet packet available
    if (_callback_rx && (read_size < USBDevice::endpoint_max_packet_size(_bulk_out))) {
        _callback_rx();
    }

    read_start(_bulk_out, _bulk_buf, MAX_PACKET_SIZE_BULK);
}
