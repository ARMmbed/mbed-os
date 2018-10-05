/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#if DEVICE_SERIAL && DEVICE_SERIAL_FC

#include "H4TransportDriver.h"

namespace ble {
namespace vendor {
namespace cordio {

H4TransportDriver::H4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud) :
    uart(tx, rx, baud), cts(cts), rts(rts) { }

void H4TransportDriver::initialize()
{
    uart.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    uart.set_flow_control(
        /* flow */ SerialBase::RTSCTS,
        /* rts */ rts,
        /* cts */ cts
    );

    uart.attach(
        callback(this, &H4TransportDriver::on_controller_irq),
        SerialBase::RxIrq
    );
}

void H4TransportDriver::terminate() {  }

uint16_t H4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;
    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.putc(to_write);
        ++i;
    }
    return len;
}

void H4TransportDriver::on_controller_irq()
{
    while (uart.readable()) {
        uint8_t char_received = uart.getc();
        on_data_received(&char_received, 1);
    }
}

} // namespace cordio
} // namespace vendor
} // namespace ble

#endif
