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

#ifndef MBED_NFC_CONTROLLER_H
#define MBED_NFC_CONTROLLER_H

#include <stdint.h>
#include "events/EventQueue.h"

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

    class NFCRemoteInitiator;
    class NFCRemoteTarget;
    class NFCControllerDriver;

    class NFCController {
    public:
        struct Delegate {
            enum nfc_discovery_terminated_reason_t {
                nfc_discovery_terminated_completed = 0,
                nfc_discovery_terminated_canceled,
                nfc_discovery_terminated_rf_error
            };

            virtual void on_discovery_terminated(nfc_discovery_terminated_reason_t reason) {}

            virtual void on_nfc_initiator_discovered(const NFCRemoteInitiator& nfc_initiator) {}

            virtual void on_nfc_target_discovered(const NFCRemoteTarget& nfc_target) {}
        };

        NFCController(NFCControllerDriver* driver, events::EventQueue* queue);

        void set_delegate(Delegate* delegate);

        nfc_rf_protocols_bitmask_t get_supported_rf_protocols() const;

        nfc_err_t configure_rf_protocols(nfc_rf_protocols_bitmask_t rf_protocols);

        nfc_err_t start_discovery();

        nfc_err_t cancel_discovery();

    private:
        NFCControllerDriver* _driver;
        Delegate* _delegate;
    };

} // namespace nfc
} // namespace mbed

#endif
