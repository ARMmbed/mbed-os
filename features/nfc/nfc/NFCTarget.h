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

#ifndef MBED_NFC_TARGET_H
#define MBED_NFC_TARGET_H

#include <stdint.h>

#include "NFCDefinitions.h"
#include "NFCEndpoint.h"
#include "NFCNDEFCapable.h"

namespace mbed {
namespace nfc {

    class NFCTarget : public NFCEndpoint, public NFCNDEFCapable {
    public:
        NFCTarget();
        virtual ~NFCTarget();

        struct Delegate {
            virtual void on_ndef_message_erased(nfc_err_t result) {}
            virtual void on_ndef_message_written(nfc_err_t result) {}
            virtual void on_ndef_message_read(nfc_err_t result) {}
        };

        void set_delegate(Delegate* delegate);

        void write_ndef_message();
        void read_ndef_message();
        void erase_ndef_message();
    };

} // namespace nfc
} // namespace mbed

#endif
