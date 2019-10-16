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
#include "mbed.h"
#include "CordioHCITransportDriver.h"
#include "drivers/DigitalInOut.h"

namespace ble {
namespace vendor {
namespace cypress_ble {

using namespace ble::vendor;

/**
 * Implementation of the H4 driver over Cypress based chips.
 */
class CyH4TransportDriver : public cordio::CordioHCITransportDriver {
public:
    /**
     * Initialize the transport driver.
     *
     */
	CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name,
                            uint8_t host_wake_irq = 0, uint8_t dev_wake_irq = 0);
        CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud);

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

    void bt_host_wake_irq_handler();

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    void on_host_stack_inactivity();
#endif

    void update_uart_baud_rate(int baud);

    bool get_enabled_powersave();
    uint8_t get_host_wake_irq_event();
    uint8_t get_dev_wake_irq_event();

private:
    void on_controller_irq();
    void assert_bt_dev_wake();
    void deassert_bt_dev_wake();

    // Use RawSerial as opposed to Serial as we don't require the locking primitives
    // provided by the Serial class (access to the UART should be exclusive to this driver)
    // Furthermore, we access the peripheral in interrupt context which would clash
    // with Serial's locking facilities
    RawSerial uart;
    PinName cts;
    PinName rts;
    PinName bt_host_wake_name;
    PinName bt_device_wake_name;

    DigitalInOut bt_host_wake;
    DigitalInOut bt_device_wake;

    bool     enabled_powersave;
    uint8_t  host_wake_irq_event;
    uint8_t  dev_wake_irq_event;

    bool     holding_deep_sleep_lock;

};

} // namespace cypress
} // namespace vendor
} // namespace ble

#define DEF_BT_BAUD_RATE    (115200)
#define DEF_BT_3M_BAUD_RATE (3000000)     /* Both Host and BT device have to be adapt to this */

#define WAKE_EVENT_ACTIVE_HIGH ( 1 )      /* Interrupt Rising Edge  */
#define WAKE_EVENT_ACTIVE_LOW  ( 0 )      /* Interrupt Falling Edge */

#if (defined(TARGET_CY8CPROTO_062_4343W))
#define BT_UART_NO_3M_SUPPORT  ( 1 )
#endif

ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_default_h4_transport_driver();
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver();
#endif
#endif /* CY_H4TRANSPORT_DRIVER_H_ */
