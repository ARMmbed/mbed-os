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

#ifndef IMPL_H4_TRANSPORT_DRIVER_H_
#define IMPL_H4_TRANSPORT_DRIVER_H_

#if (DEVICE_SERIAL && DEVICE_SERIAL_FC) || defined(DOXYGEN_ONLY)

#include <cstdint>
#include "drivers/UnbufferedSerial.h"
#include "ble/driver/CordioHCITransportDriver.h"

namespace ble {

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
    ~H4TransportDriver() override = default;

    /**
     * @see CordioHCITransportDriver::initialize
     */
    void initialize() override;

    /**
     * @see CordioHCITransportDriver::terminate
     */
    void terminate() override;

    /**
     * @see CordioHCITransportDriver::write
     */
    uint16_t write(uint8_t type, uint16_t len, uint8_t *pData) override;

private:
    void on_controller_irq();

    // Use UnbufferedSerial as we don't require locking primitives.
    // We access the peripheral in interrupt context.
    mbed::UnbufferedSerial uart;
    PinName cts;
    PinName rts;
};

} // namespace ble

#endif

#endif /* IMPL_H4_TRANSPORT_DRIVER_H_ */
