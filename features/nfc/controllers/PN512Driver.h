/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_PN512_DRIVER_H
#define MBED_PN512_DRIVER_H

#include <stdint.h>

#include "nfc/NFCControllerDriver.h"

namespace mbed {
namespace nfc {

    struct PN512TransportDriver;
    class PN512Driver : NFCControllerDriver {
    public:
        PN512Driver(PN512TransportDriver* transport_driver);

    private:
        virtual void initialize(scheduler_timer_t* pTimer) = 0;
        virtual transceiver_t* get_transceiver() const;

        pn512_t _pn512;
    };

} // namespace nfc
} // namespace mbed

#endif
