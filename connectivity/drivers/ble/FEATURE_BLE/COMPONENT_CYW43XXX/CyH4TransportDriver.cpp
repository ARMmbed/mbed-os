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
#include "mbed_power_mgmt.h"
#include "drivers/InterruptIn.h"
#if !defined(CYW43XXX_UNBUFFERED_UART)
#include "cybsp_types.h"
#else
#include "mbed_wait_api.h"
#endif
#include "Callback.h"
#include "rtos/ThisThread.h"
#include <chrono>

// BT settling time after power on
#if !defined (CY_BT_POWER_ON_SETTLING_TIME)
    #define CY_BT_POWER_ON_SETTLING_TIME                    (500ms)
#endif /* !defined (CY_BT_POWER_ON_SETTLING_TIME) */

// Power on reset time 
#if !defined (CY_BT_POWER_ON_RESET_TIME)
    #define CY_BT_POWER_ON_RESET_TIME                       (1ms)      
#endif /* !defined (CY_BT_POWER_ON_RESET_TIME) */


namespace ble {
namespace vendor {
namespace cypress_ble {

using namespace std::chrono_literals;

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, PinName bt_power_name, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name, uint8_t host_wake_irq, uint8_t dev_wake_irq) :
#if defined(CYW43XXX_UNBUFFERED_UART)
    uart(tx, rx),
#else
    tx(tx), rx(rx),
#endif
    cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_power(bt_power_name, PIN_OUTPUT, PullUp, 0),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullNone, 1),
    host_wake_irq_event(host_wake_irq),
    dev_wake_irq_event(dev_wake_irq)
{
    enabled_powersave = true;
}

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, PinName bt_power_name, int baud) :
#if defined(CYW43XXX_UNBUFFERED_UART)
    uart(tx, rx),
#else
    tx(tx), rx(rx),
#endif
    cts(cts), rts(rts),
    bt_host_wake_name(NC),
    bt_device_wake_name(NC),
    bt_power(bt_power_name, PIN_OUTPUT, PullNone, 0),
    bt_host_wake(bt_host_wake_name),
    bt_device_wake(bt_device_wake_name)

{
    enabled_powersave = false;
}

CyH4TransportDriver::~CyH4TransportDriver()
{

}

void CyH4TransportDriver::bt_host_wake_rise_irq_handler(void)
{
    if (host_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
        if(bt_host_wake_active == true)
        {
            /* lock MCU Deep Sleep entry as long as host_wake is asserted */
            sleep_manager_unlock_deep_sleep();
            bt_host_wake_active = false;
        }
    } else {
        /* lock MCU Deep Sleep entry as long as host_wake is asserted */
        sleep_manager_lock_deep_sleep();
        bt_host_wake_active = true;
    }
}

void CyH4TransportDriver::bt_host_wake_fall_irq_handler(void)
{
    if (host_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
        /* lock MCU Deep Sleep entry as long as host_wake is asserted */
        sleep_manager_lock_deep_sleep();
        bt_host_wake_active = true;
    } else {
        if(bt_host_wake_active == true)
        {
            /* lock MCU Deep Sleep entry as long as host_wake is asserted */
            sleep_manager_unlock_deep_sleep();
            bt_host_wake_active = false;
        }
    }
}

#if defined(CYW43XXX_UNBUFFERED_UART)
void CyH4TransportDriver::on_controller_irq()
#else
static void on_controller_irq(void *callback_arg, cyhal_uart_event_t event)
#endif
{
#if !defined(CYW43XXX_UNBUFFERED_UART)
    (void)(event);
    cyhal_uart_t *uart_obj = (cyhal_uart_t *)callback_arg;
#endif

    sleep_manager_lock_deep_sleep();

#if defined(CYW43XXX_UNBUFFERED_UART)
    while (uart.readable()) {
        uint8_t char_received;
        if (uart.read(&char_received, 1)) {
            CordioHCITransportDriver::on_data_received(&char_received, 1);
        }
#else
    while (cyhal_uart_readable(uart_obj)) {
        uint8_t char_received;
        cyhal_uart_getc(uart_obj, &char_received, 0);
        CyH4TransportDriver::on_data_received(&char_received, 1);
#endif
    }

    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::initialize()
{
    // Initial MCU Deep Sleep locking. CyH4TransportDriver has the following MCU Deep Sleep locking
    // scenarios:
    // a) A BT device or MCU does not support Low Power mode (MBED configuration does not include
    //    MBED_TICKLESS, DEVICE_SLEEP, DEVICE_LPTICKER or CYCFG_BT_LP_ENABLED features).
    //    In this case, CyH4TransportDriver locks Deep Sleep in the initialize() function and
    //    unlocks the terminate() function.
    // b) A BT device and MCU support Low Power mode. 
    //    In this case, the control of the unlock/lock of the Deep Sleep
    //    functionality will be done in bt_host_wake_rise_irq_handler and bt_host_wake_fall_irq_handler 
    //    handlers. Finally, CyH4TransportDriver unlocks the Deep Sleep in terminate() function 
    //    (if it was locked before) by checking the bt_host_wake_active flag.
    bt_host_wake_active = true;
    sleep_manager_lock_deep_sleep();

    // Keep the bt_power line in the low level to ensure that the device resets.
    bt_power = 0;
    rtos::ThisThread::sleep_for(CY_BT_POWER_ON_RESET_TIME);

#if defined(CYW43XXX_UNBUFFERED_UART)
    uart.baud(DEF_BT_BAUD_RATE);

    uart.format(
        /* bits */ 8,
        /* parity */ mbed::SerialBase::None,
        /* stop bit */ 1
    );

    uart.set_flow_control(
        /* flow */ mbed::SerialBase::RTSCTS,
        /* rts */ rts,
        /* cts */ cts
    );

    uart.attach(
        mbed::callback(this, &CyH4TransportDriver::on_controller_irq),
        mbed::SerialBase::RxIrq
    );
#else
    cyhal_uart_init(&uart, tx, rx, NULL, NULL);

    const cyhal_uart_cfg_t uart_cfg = { .data_bits = 8, .stop_bits = 1, .parity = CYHAL_UART_PARITY_NONE, .rx_buffer = NULL, .rx_buffer_size = 0 };
    cyhal_uart_configure(&uart, &uart_cfg);
    cyhal_uart_set_flow_control(&uart, cts, rts);
    cyhal_uart_clear(&uart);
    cyhal_uart_register_callback(&uart, &on_controller_irq, &uart);
    cyhal_uart_enable_event(&uart, CYHAL_UART_IRQ_RX_NOT_EMPTY, CYHAL_ISR_PRIORITY_DEFAULT, true);
#endif

    // Power up BT
    bt_power = 1;
    rtos::ThisThread::sleep_for(CY_BT_POWER_ON_SETTLING_TIME);

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (bt_host_wake_name != NC) {
        //Register IRQ for Host WAKE
        host_wake_pin = new mbed::InterruptIn(bt_host_wake_name);
        host_wake_pin->fall(mbed::callback(this, &CyH4TransportDriver::bt_host_wake_fall_irq_handler));
        host_wake_pin->rise(mbed::callback(this, &CyH4TransportDriver::bt_host_wake_rise_irq_handler));
    }
#endif
    if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
    } else {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
    }
}

void CyH4TransportDriver::terminate()
{
#if !defined(CYW43XXX_UNBUFFERED_UART)
    cyhal_uart_event_t enable_irq_event = (cyhal_uart_event_t)(CYHAL_UART_IRQ_RX_DONE
                                       | CYHAL_UART_IRQ_TX_DONE
                                       | CYHAL_UART_IRQ_RX_NOT_EMPTY
                                      );

    cyhal_uart_enable_event(&uart,
                            enable_irq_event,
                            CYHAL_ISR_PRIORITY_DEFAULT,
                            false
                        );
#endif

    if(bt_host_wake.is_connected())
    {
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
        delete host_wake_pin;
#endif
    }

    deassert_bt_dev_wake();

    // Power down BT
    bt_power = 0; //BT_POWER is an output, should not be freed only set inactive

#if defined(CYW43XXX_UNBUFFERED_UART)
    uart.close();
#else
    cyhal_uart_free(&uart);
#endif

    // Unlock Deep Sleep if it was locked by CyH4TransportDriver before.
    if (bt_host_wake_active == true)
    {
       sleep_manager_unlock_deep_sleep();
       bt_host_wake_active = false;
    }
}

uint16_t CyH4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    sleep_manager_lock_deep_sleep();
    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
#if defined(CYW43XXX_UNBUFFERED_UART)
        while (uart.writeable() == 0);
        uart.write(&to_write, 1);
#else
        while (cyhal_uart_writable(&uart) == 0);
        cyhal_uart_putc(&uart, to_write);
#endif
        ++i;
    }
#if defined(CYW43XXX_UNBUFFERED_UART)
/* Assuming a 16 byte FIFO as worst case this will ensure all bytes are sent before deasserting bt_dev_wake */
#ifndef BT_UART_NO_3M_SUPPORT
    wait_us(50); // 3000000 bps
#else
    rtos::ThisThread::sleep_for(2ms); // 115200 bps
#endif
#else
    while(cyhal_uart_is_tx_active(&uart));
#endif

    deassert_bt_dev_wake();
    sleep_manager_unlock_deep_sleep();
    return len;
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
#if defined(CYW43XXX_UNBUFFERED_UART)
    uart.baud((uint32_t)baud);
#else
    uint32_t ignore;
    cyhal_uart_set_baud(&uart, (uint32_t)baud, &ignore);
#endif
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
       /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, CYBSP_BT_POWER, DEF_BT_BAUD_RATE,
       CYBSP_BT_HOST_WAKE, CYBSP_BT_DEVICE_WAKE
    );

#else
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
        /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
        /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, CYBSP_BT_POWER, DEF_BT_BAUD_RATE);
#endif
    return s_transport_driver;
}

MBED_WEAK
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver()
{
    return (ble_cordio_get_default_h4_transport_driver());
}

#endif
