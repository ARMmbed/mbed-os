/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_PN512_TRANSPORT_DRIVER_H
#define MBED_PN512_TRANSPORT_DRIVER_H

#include <stdint.h>
#include "stack/platform/nfc_transport.h"


namespace mbed {
namespace nfc {
/**
 * The PN512 supports multiple transport mechanisms (SPI, I2C, UART): this class provides a unified API across these transports
 */
class PN512TransportDriver {
public:
    /**
     * The PN512TransportDriver delegate
     */
    struct Delegate {
        /**
         * Called when the PN512 asserts the interrupt line
         */
        virtual void on_hw_interrupt() {}

    protected:
        ~Delegate() {}
    };

    /**
     * Create a PN512TransportDriver instance
     */
    PN512TransportDriver();

    /**
    * PN512TransportDriver destructor.
    */
    virtual ~PN512TransportDriver();

    /**
     * Initialize transport driver and perform a chip reset
     */
    virtual void initialize() = 0;

    /**
     * Retrieve the nfc_transport_t struct for the stack to use
     *
     * @return a pointer to a nfc_transport_t struct
     */
    virtual nfc_transport_t *get_transport() = 0;

    /**
     * Set this instance's delegate
     *
     * @param[in] delegate the delegate instance to use
     */
    void set_delegate(Delegate *delegate);
protected:

    /**
     * An implementation must call this function (can be called from interrupt context)
     * when the PN512 asserts its interrupt line
     */
    void hw_interrupt();
private:
    Delegate *_delegate;
};

} // namespace nfc
} // namespace mbed

#endif
