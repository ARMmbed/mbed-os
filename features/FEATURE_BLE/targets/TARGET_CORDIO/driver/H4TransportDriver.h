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

#ifndef CORDIO_H4_TRANSPORT_DRIVER_H_
#define CORDIO_H4_TRANSPORT_DRIVER_H_

#if (DEVICE_SERIAL && DEVICE_SERIAL_FC) || defined(DOXYGEN_ONLY)

#include <stdint.h>
#include "mbed.h"
#include "CordioHCITransportDriver.h"

namespace ble {
namespace vendor {
namespace cordio {

/**
 * Implementation of the H4 driver.
 *
 * @note This HCI transport implementation is not accessible to devices that do
 * not expose serial flow control.
 */
class H4TransportDriver : public CordioHCITransportDriver {
public:
    /**
     * Initialize the transport driver.
     *
     * @param tx tx pin name.
     * @param rx rx pin name
     * @param cts cts pin name
     * @param rts rts pin name.
     * @param baud baud use to communicate with the ble module
     */
    H4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud);

    /**
     * Destructor
     */
    virtual ~H4TransportDriver() { }

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

private:
    void on_controller_irq();

    // Use UnbufferedSerial as we don't require locking primitives.
    // We access the peripheral in interrupt context.
    UnbufferedSerial uart;
    PinName cts;
    PinName rts;
};

} // namespace cordio
} // namespace vendor
} // namespace ble

#endif

#endif /* CORDIO_H4_TRANSPORT_DRIVER_H_ */
