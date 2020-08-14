/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#ifndef Test_USBMSD_H
#define Test_USBMSD_H

#include "USBMSD.h"


#define USB_DEV_SN_LEN (32) // 32 hex digit UUID
#define USB_DEV_SN_DESC_SIZE (USB_DEV_SN_LEN * 2 + 2)

/**
 * Convert a C style ASCII to a USB string descriptor
 *
 * @param usb_desc output buffer for the USB string descriptor
 * @param str ASCII string
 * @param n size of usb_desc buffer, even number
 * @returns number of bytes returned in usb_desc or -1 on failure
 */
int ascii2usb_string_desc(uint8_t *usb_desc, const char *str, size_t n)
{
    if (str == NULL || usb_desc == NULL || n < 4) {
        return -1;
    }
    if (n % 2 != 0) {
        return -1;
    }
    size_t s, d;
    // set bString (@ offset 2 onwards) as a UNICODE UTF-16LE string
    memset(usb_desc, 0, n);
    for (s = 0, d = 2; str[s] != '\0' && d < n; s++, d += 2) {
        usb_desc[d] = str[s];
    }
    // set bLength @ offset 0
    usb_desc[0] = d;
    // set bDescriptorType @ offset 1
    usb_desc[1] = STRING_DESCRIPTOR;
    return d;
}

class TestUSBMSD: public USBMSD {
public:
    TestUSBMSD(BlockDevice *bd, bool connect_blocking = true, uint16_t vendor_id = 0x0703, uint16_t product_id = 0x0104,
               uint16_t product_release = 0x0001)
        : USBMSD(bd, connect_blocking, vendor_id, product_id, product_release)
    {

    }

    virtual ~TestUSBMSD()
    {

    }

    uint32_t get_read_counter()
    {
        return read_counter;
    }

    uint32_t get_program_counter()
    {
        return program_counter;
    }

    void reset_counters()
    {
        read_counter = program_counter = erase_counter = 0;
    }

    static void setup_serial_number()
    {
        char _key[128] = { 0 };
        char _value[128] = { 0 };

        greentea_send_kv("get_serial_number", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("serial_number", _key);
        usb_dev_sn[USB_DEV_SN_LEN] = '\0';
        memcpy(usb_dev_sn, _value, USB_DEV_SN_LEN);
        ascii2usb_string_desc(_serial_num_descriptor, usb_dev_sn, USB_DEV_SN_DESC_SIZE);
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *)_serial_num_descriptor;
    }

    static volatile uint32_t read_counter;
    static volatile uint32_t program_counter;
    static volatile uint32_t erase_counter;

protected:
    virtual int disk_read(uint8_t *data, uint64_t block, uint8_t count)
    {
        read_counter++;
        return USBMSD::disk_read(data, block, count);
    }

    virtual int disk_write(const uint8_t *data, uint64_t block, uint8_t count)
    {
        erase_counter++;
        program_counter++;

        return USBMSD::disk_write(data, block, count);
    }
private:
    static uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
    static char usb_dev_sn[USB_DEV_SN_LEN + 1];
};

uint8_t TestUSBMSD::_serial_num_descriptor[USB_DEV_SN_DESC_SIZE] = { 0 };
char  TestUSBMSD::usb_dev_sn[USB_DEV_SN_LEN + 1] = { 0 };


volatile uint32_t TestUSBMSD::read_counter = 0;
volatile uint32_t TestUSBMSD::program_counter = 0;
volatile uint32_t TestUSBMSD::erase_counter = 0;

#endif // Test_USBMSD_H
