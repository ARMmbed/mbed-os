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

#include "stdint.h"
#include "USBMouse.h"
#include "ThisThread.h"
#include "usb_phy_api.h"


USBMouse::USBMouse(bool connect_blocking, MOUSE_TYPE mouse_type, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{
    _button = 0;
    _mouse_type = mouse_type;

    if (connect_blocking) {
        USBDevice::connect();
        wait_ready();
    } else {
        init();
    }
}

USBMouse::USBMouse(USBPhy *phy, MOUSE_TYPE mouse_type, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{
    _button = 0;
    _mouse_type = mouse_type;
}

USBMouse::~USBMouse()
{
    deinit();
}

bool USBMouse::update(int16_t x, int16_t y, uint8_t button, int8_t z)
{
    bool ret;
    switch (_mouse_type) {
        case REL_MOUSE:
            _mutex.lock();

            while (x > 127) {
                if (!mouse_send(127, 0, button, z)) {
                    _mutex.unlock();
                    return false;
                }
                x = x - 127;
            }
            while (x < -128) {
                if (!mouse_send(-128, 0, button, z)) {
                    _mutex.unlock();
                    return false;
                }
                x = x + 128;
            }
            while (y > 127) {
                if (!mouse_send(0, 127, button, z)) {
                    _mutex.unlock();
                    return false;
                }
                y = y - 127;
            }
            while (y < -128) {
                if (!mouse_send(0, -128, button, z)) {
                    _mutex.unlock();
                    return false;
                }
                y = y + 128;
            }
            ret = mouse_send(x, y, button, z);

            _mutex.unlock();
            return ret;
        case ABS_MOUSE:
            _mutex.lock();

            HID_REPORT report;

            report.data[0] = x & 0xff;
            report.data[1] = (x >> 8) & 0xff;
            report.data[2] = y & 0xff;
            report.data[3] = (y >> 8) & 0xff;
            report.data[4] = -z;
            report.data[5] = button & 0x07;

            report.length = 6;

            ret = send(&report);

            _mutex.unlock();
            return ret;
        default:
            return false;
    }
}

bool USBMouse::mouse_send(int8_t x, int8_t y, uint8_t buttons, int8_t z)
{
    _mutex.lock();

    HID_REPORT report;
    report.data[0] = buttons & 0x07;
    report.data[1] = x;
    report.data[2] = y;
    report.data[3] = -z; // >0 to scroll down, <0 to scroll up

    report.length = 4;

    bool ret = send(&report);

    _mutex.unlock();
    return ret;
}

bool USBMouse::move(int16_t x, int16_t y)
{
    _mutex.lock();

    bool ret = update(x, y, _button, 0);

    _mutex.unlock();
    return ret;
}

bool USBMouse::scroll(int8_t z)
{
    _mutex.lock();

    bool ret = update(0, 0, _button, z);

    _mutex.unlock();
    return ret;
}


bool USBMouse::double_click()
{
    _mutex.lock();

    if (!click(MOUSE_LEFT)) {
        _mutex.unlock();
        return false;
    }
    rtos::ThisThread::sleep_for(100);
    bool ret = click(MOUSE_LEFT);

    _mutex.unlock();
    return ret;
}

bool USBMouse::click(uint8_t button)
{
    _mutex.lock();

    if (!update(0, 0, button, 0)) {
        _mutex.unlock();
        return false;
    }
    rtos::ThisThread::sleep_for(10);
    bool ret = update(0, 0, 0, 0);

    _mutex.unlock();
    return ret;
}

bool USBMouse::press(uint8_t button)
{
    _mutex.lock();

    _button = button & 0x07;
    bool ret = update(0, 0, _button, 0);

    _mutex.unlock();
    return ret;
}

bool USBMouse::release(uint8_t button)
{
    _mutex.lock();

    _button = (_button & (~button)) & 0x07;
    bool ret = update(0, 0, _button, 0);

    _mutex.unlock();
    return ret;
}


const uint8_t *USBMouse::report_desc()
{

    if (_mouse_type == REL_MOUSE) {
        static const uint8_t report_descriptor[] = {
            USAGE_PAGE(1),      0x01,       // Genric Desktop
            USAGE(1),           0x02,       // Mouse
            COLLECTION(1),      0x01,       // Application
            USAGE(1),           0x01,       // Pointer
            COLLECTION(1),      0x00,       // Physical

            REPORT_COUNT(1),    0x03,
            REPORT_SIZE(1),     0x01,
            USAGE_PAGE(1),      0x09,       // Buttons
            USAGE_MINIMUM(1),       0x1,
            USAGE_MAXIMUM(1),       0x3,
            LOGICAL_MINIMUM(1),     0x00,
            LOGICAL_MAXIMUM(1),     0x01,
            INPUT(1),           0x02,
            REPORT_COUNT(1),    0x01,
            REPORT_SIZE(1),     0x05,
            INPUT(1),           0x01,

            REPORT_COUNT(1),    0x03,
            REPORT_SIZE(1),     0x08,
            USAGE_PAGE(1),      0x01,
            USAGE(1),           0x30,       // X
            USAGE(1),           0x31,       // Y
            USAGE(1),           0x38,       // scroll
            LOGICAL_MINIMUM(1),     0x81,
            LOGICAL_MAXIMUM(1),     0x7f,
            INPUT(1),           0x06,       // Relative data

            END_COLLECTION(0),
            END_COLLECTION(0),
        };
        reportLength = sizeof(report_descriptor);
        return report_descriptor;
    } else if (_mouse_type == ABS_MOUSE) {
        static const uint8_t report_descriptor[] = {
            USAGE_PAGE(1), 0x01,           // Generic Desktop
            USAGE(1), 0x02,                // Mouse
            COLLECTION(1), 0x01,           // Application
            USAGE(1), 0x01,                // Pointer
            COLLECTION(1), 0x00,           // Physical

            USAGE_PAGE(1), 0x01,            // Generic Desktop
            USAGE(1), 0x30,                 // X
            USAGE(1), 0x31,                 // Y
            LOGICAL_MINIMUM(1), 0x00,       // 0
            LOGICAL_MAXIMUM(2), 0xff, 0x7f, // 32767
            REPORT_SIZE(1), 0x10,
            REPORT_COUNT(1), 0x02,
            INPUT(1), 0x02,                 // Data, Variable, Absolute

            USAGE_PAGE(1), 0x01,            // Generic Desktop
            USAGE(1), 0x38,                 // scroll
            LOGICAL_MINIMUM(1), 0x81,       // -127
            LOGICAL_MAXIMUM(1), 0x7f,       // 127
            REPORT_SIZE(1), 0x08,
            REPORT_COUNT(1), 0x01,
            INPUT(1), 0x06,                 // Data, Variable, Relative

            USAGE_PAGE(1), 0x09,            // Buttons
            USAGE_MINIMUM(1), 0x01,
            USAGE_MAXIMUM(1), 0x03,
            LOGICAL_MINIMUM(1), 0x00,       // 0
            LOGICAL_MAXIMUM(1), 0x01,       // 1
            REPORT_COUNT(1), 0x03,
            REPORT_SIZE(1), 0x01,
            INPUT(1), 0x02,                 // Data, Variable, Absolute
            REPORT_COUNT(1), 0x01,
            REPORT_SIZE(1), 0x05,
            INPUT(1), 0x01,                 // Constant

            END_COLLECTION(0),
            END_COLLECTION(0)
        };
        reportLength = sizeof(report_descriptor);
        return report_descriptor;
    }
    return NULL;
}

#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (1 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (1 * HID_DESCRIPTOR_LENGTH) \
                               + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *USBMouse::configuration_desc(uint8_t index)
{
    if (index != 0) {
        return NULL;
    }
    uint8_t configuration_descriptor_temp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
        CONFIGURATION_DESCRIPTOR,           // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (MSB)
        0x01,                               // bNumInterfaces
        DEFAULT_CONFIGURATION,              // bConfigurationValue
        0x00,                               // iConfiguration
        C_RESERVED | C_SELF_POWERED,        // bmAttributes
        C_POWER(0),                         // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR,               // bDescriptorType
        0x00,                               // bInterfaceNumber
        0x00,                               // bAlternateSetting
        0x02,                               // bNumEndpoints
        HID_CLASS,                          // bInterfaceClass
        HID_SUBCLASS_BOOT,                  // bInterfaceSubClass
        HID_PROTOCOL_MOUSE,                 // bInterfaceProtocol
        0x00,                               // iInterface

        HID_DESCRIPTOR_LENGTH,              // bLength
        HID_DESCRIPTOR,                     // bDescriptorType
        LSB(HID_VERSION_1_11),              // bcdHID (LSB)
        MSB(HID_VERSION_1_11),              // bcdHID (MSB)
        0x00,                               // bCountryCode
        0x01,                               // bNumDescriptors
        REPORT_DESCRIPTOR,                  // bDescriptorType
        (uint8_t)(LSB(report_desc_length())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        _int_in,                            // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        _int_out,                           // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configuration_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configuration_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}
