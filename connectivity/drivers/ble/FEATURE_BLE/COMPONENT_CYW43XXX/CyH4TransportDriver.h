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

#ifndef CY_H4TRANSPORT_DRIVER_H_
#define CY_H4TRANSPORT_DRIVER_H_

#if (DEVICE_SERIAL && DEVICE_SERIAL_FC) || defined(DOXYGEN_ONLY)

#include <stdint.h>
#include "ble/driver/CordioHCITransportDriver.h"
#include "drivers/DigitalInOut.h"
#include "drivers/InterruptIn.h"

#if defined(CYW43XXX_UNBUFFERED_UART)
#include "drivers/UnbufferedSerial.h"
#else
#include "cyhal_uart.h"
#endif

namespace ble {
namespace vendor {
namespace cypress_ble {

using namespace ble::vendor;

/**
 * Implementation of the H4 driver over Cypress based chips.
 */
class CyH4TransportDriver : public CordioHCITransportDriver {
public:
    /**
     * Initialize the transport driver.
     *
     */
    CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, PinName bt_power_name, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name,
                        uint8_t host_wake_irq = 0, uint8_t dev_wake_irq = 0);
    CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts,  PinName bt_power_name, int baud);

    /**
     * Destructor
     */
    virtual ~CyH4TransportDriver();

    /**
     * @see CordioHCITransportDriver::initialize
     */
    virtual void initialize();

    /**
     * @see CordioHCITransportDriver::terminate
     */
    virtual void terminate();

    /**
     * @see CordioHCITransportDriver::write
     */
    virtual uint16_t write(uint8_t type, uint16_t len, uint8_t *pData);

    void bt_host_wake_rise_irq_handler();
    void bt_host_wake_fall_irq_handler();

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    void on_host_stack_inactivity();
#endif

    void update_uart_baud_rate(int baud);

    bool get_enabled_powersave();
    uint8_t get_host_wake_irq_event();
    uint8_t get_dev_wake_irq_event();

private:
    void assert_bt_dev_wake();
    void deassert_bt_dev_wake();
	
#if defined(CYW43XXX_UNBUFFERED_UART)
    void on_controller_irq();
#endif

    // Use HAL serial because Cypress UART is buffered.
    // The PUTC function does not actually blocks until data is fully transmitted,
    // it only blocks until data gets into HW buffer.
    // The UART APIs prevents sleep while there are data in the HW buffer.
    // However UART APIs does not prevent the BT radio from going to sleep.
    // Use the HAL APIs to prevent the radio from going to sleep until UART transmition is complete.
    // Mbed layer has no API that distinguish between data in HW buffer v.s. data already transmitted.
#if defined(CYW43XXX_UNBUFFERED_UART)
    mbed::UnbufferedSerial uart;
#else
    cyhal_uart_t uart;
    PinName tx;
    PinName rx;
#endif
    PinName cts;
    PinName rts;
    PinName bt_host_wake_name;
    PinName bt_device_wake_name;

    mbed::DigitalInOut bt_power;
    mbed::DigitalInOut bt_host_wake;
    mbed::DigitalInOut bt_device_wake;

    bool bt_host_wake_active;

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    mbed::InterruptIn *host_wake_pin;
#endif

    bool     enabled_powersave;
    uint8_t  host_wake_irq_event;
    uint8_t  dev_wake_irq_event;
};

} // namespace cypress
} // namespace vendor
} // namespace ble

#define DEF_BT_BAUD_RATE    (115200)
#define DEF_BT_3M_BAUD_RATE (3000000)     /* Both Host and BT device have to be adapt to this */

#define WAKE_EVENT_ACTIVE_HIGH ( 1 )      /* Interrupt Rising Edge  */
#define WAKE_EVENT_ACTIVE_LOW  ( 0 )      /* Interrupt Falling Edge */

ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_default_h4_transport_driver();
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver();
#endif
#endif /* CY_H4TRANSPORT_DRIVER_H_ */
