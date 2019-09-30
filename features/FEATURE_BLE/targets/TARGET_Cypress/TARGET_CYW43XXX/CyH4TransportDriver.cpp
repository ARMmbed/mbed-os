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

namespace ble {
namespace vendor {
namespace cypress_ble {


CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name, uint8_t host_wake_irq, uint8_t dev_wake_irq) :
    uart(tx, rx, baud), cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullNone, 1),
    host_wake_irq_event(host_wake_irq),
    dev_wake_irq_event(dev_wake_irq)
{
    enabled_powersave = true;
}

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud) :
    uart(tx, rx, baud),
    cts(cts),
    rts(rts),
    bt_host_wake_name(NC),
    bt_device_wake_name(NC),
    bt_host_wake(bt_host_wake_name),
    bt_device_wake(bt_device_wake_name)
{
    enabled_powersave = false;
    sleep_manager_lock_deep_sleep();
    holding_deep_sleep_lock = true;
}

CyH4TransportDriver::~CyH4TransportDriver()
{
    if (holding_deep_sleep_lock)
    {
       sleep_manager_unlock_deep_sleep();
       holding_deep_sleep_lock = false;
    }
}

void CyH4TransportDriver::bt_host_wake_irq_handler(void)
{
    uart.attach(
        callback(this, &CyH4TransportDriver::on_controller_irq),
        SerialBase::RxIrq
    );
}

void CyH4TransportDriver::initialize()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
	InterruptIn *host_wake_pin;
#endif

    sleep_manager_lock_deep_sleep();

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
    if (bt_host_wake_name != NC) {
        //Register IRQ for Host WAKE
        host_wake_pin = new InterruptIn(bt_host_wake_name);
        if (host_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
           host_wake_pin->fall(callback(this, &CyH4TransportDriver::bt_host_wake_irq_handler));
        } else {
           host_wake_pin->rise(callback(this, &CyH4TransportDriver::bt_host_wake_irq_handler));
        }
    }

#endif
    if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
    } else {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
    }
    sleep_manager_unlock_deep_sleep();
    rtos::ThisThread::sleep_for(500);
}

void CyH4TransportDriver::terminate() {  }

uint16_t CyH4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    sleep_manager_lock_deep_sleep();
    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.putc(to_write);
        ++i;
    }

    deassert_bt_dev_wake();
    sleep_manager_unlock_deep_sleep();
    return len;
}

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
void CyH4TransportDriver::on_host_stack_inactivity()
{
    if (enabled_powersave) {
        uart.attach(NULL, SerialBase::RxIrq);
    }
}
#endif

void CyH4TransportDriver::on_controller_irq()
{
    sleep_manager_lock_deep_sleep();
    assert_bt_dev_wake();

    while (uart.readable()) {
        uint8_t char_received = uart.getc();
        on_data_received(&char_received, 1);
    }

    deassert_bt_dev_wake();
    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::assert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (enabled_powersave) {
        if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
        } else {
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
        }
    }
#endif
}

void CyH4TransportDriver::deassert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (enabled_powersave) {
        wait_us(5000); /* remove and replace when uart tx transmit complete api is available */
        //De-assert bt_device_wake
        if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
        } else {
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
        }
    }
#endif
}


void CyH4TransportDriver::update_uart_baud_rate(int baud)
{
    uart.baud(baud);
}

bool CyH4TransportDriver::get_enabled_powersave()
{
    return (enabled_powersave);
}

uint8_t CyH4TransportDriver::get_host_wake_irq_event()
{
    return (host_wake_irq_event);
}

uint8_t CyH4TransportDriver::get_dev_wake_irq_event()
{
    return (dev_wake_irq_event);
}


} // namespace cypress_ble
} // namespace vendor
} // namespace ble

ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_default_h4_transport_driver()
{
#if (defined(CYBSP_BT_HOST_WAKE) && defined(CYBSP_BT_DEVICE_WAKE))
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
       /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
       /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, DEF_BT_BAUD_RATE,
       CYBSP_BT_HOST_WAKE, CYBSP_BT_DEVICE_WAKE
    );

#else
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
        /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
        /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, DEF_BT_BAUD_RATE);
#endif
    return s_transport_driver;
}

MBED_WEAK
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver()
{
    return (ble_cordio_get_default_h4_transport_driver());
}

#endif
