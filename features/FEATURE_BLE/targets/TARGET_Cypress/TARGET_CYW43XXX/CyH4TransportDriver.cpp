/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#if DEVICE_SERIAL && DEVICE_SERIAL_FC

#include "CyH4TransportDriver.h"
#include "cycfg_pins.h"

namespace ble {
namespace vendor {
namespace cypress_ble {

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name) :
    uart(tx, rx, baud), cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullDefault, 1)
{
}

void CyH4TransportDriver::bt_host_wake_irq_handler(void)
{
	sleep_manager_lock_deep_sleep();
	CyH4TransportDriver::on_controller_irq();
	sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::initialize()
{
	InterruptIn *host_wake_pin;

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
        callback(this, &CyH4TransportDriver::on_controller_irq),
        SerialBase::RxIrq
    );

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    //Register IRQ for Host WAKE
    host_wake_pin = new InterruptIn(bt_host_wake_name);
    host_wake_pin->fall(callback(this, &CyH4TransportDriver::bt_host_wake_irq_handler));

#endif
    bt_device_wake = 0;
    wait_ms(500);
}

void CyH4TransportDriver::terminate() {  }

uint16_t CyH4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.putc(to_write);
        ++i;
    }

    deassert_bt_dev_wake();
    return len;
}

void CyH4TransportDriver::on_controller_irq()
{
	assert_bt_dev_wake();

	while (uart.readable()) {
        uint8_t char_received = uart.getc();
        on_data_received(&char_received, 1);
    }

	deassert_bt_dev_wake();
}

void CyH4TransportDriver::assert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    bt_device_wake = 0;
#endif
}

void CyH4TransportDriver::deassert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
	//De-assert bt_device_wake
	bt_device_wake = 1;
#endif
}

} // namespace cypress_ble
} // namespace vendor
} // namespace ble

#endif
