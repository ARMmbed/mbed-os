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
#include "USBSerial.h"
#include "usb_phy_api.h"


USBSerial::USBSerial(bool connect_blocking, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBCDC(get_usb_phy(), vendor_id, product_id, product_release)
{
    _settings_changed_callback = 0;

    if (connect_blocking) {
        connect();
        wait_ready();
    } else {
        init();
    }
}

USBSerial::USBSerial(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBCDC(phy, vendor_id, product_id, product_release)
{
    _settings_changed_callback = 0;
}

USBSerial::~USBSerial()
{
    deinit();
}

int USBSerial::_putc(int c)
{
    if (send((uint8_t *)&c, 1)) {
        return c;
    } else {
        return -1;
    }
}

int USBSerial::_getc()
{
    uint8_t c = 0;
    if (receive(&c, sizeof(c))) {
        return c;
    } else {
        return -1;
    }
}

void USBSerial::data_rx()
{
    assert_locked();

    //call a potential handler
    if (rx) {
        rx.call();
    }
}

uint8_t USBSerial::available()
{
    USBCDC::lock();

    uint8_t size = 0;
    if (!_rx_in_progress) {
        size = _rx_size > 0xFF ? 0xFF : _rx_size;
    }

    USBCDC::unlock();
    return size;
}

bool USBSerial::connected()
{
    return _terminal_connected;
}
