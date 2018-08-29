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
#include "PN512TransportDriver.h"

#include "nfc/acore/acore/ac_buffer.h"
#include "nfc/acore/acore/ac_buffer_reader.h"
#include "nfc/acore/acore/ac_buffer_builder.h"
#include "nfc/stack/transceiver/pn512/pn512.h"

namespace mbed {
namespace nfc {

class PN512Driver : public NFCControllerDriver, private PN512TransportDriver::Delegate {
public:
    PN512Driver(PN512TransportDriver *transport_driver);

    virtual nfc_transceiver_t *initialize(nfc_scheduler_timer_t *scheduler_timer);
    virtual void get_supported_nfc_techs(nfc_tech_t *initiator, nfc_tech_t *target) const;

private:
    // PN512TransportDriver::Delegate implementation
    virtual void on_hw_interrupt();

    PN512TransportDriver *_transport_driver;
    pn512_t _pn512;
};

} // namespace nfc
} // namespace mbed

#endif
