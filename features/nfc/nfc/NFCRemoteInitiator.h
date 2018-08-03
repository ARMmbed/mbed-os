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

#ifndef MBED_NFC_REMOTE_INITIATOR_H
#define MBED_NFC_REMOTE_INITIATOR_H

#include <stdint.h>

#include "NFCDefinitions.h"
#include "NFCEndpoint.h"
#include "NFCNDEFCapable.h"

namespace mbed {
namespace nfc {
    class NFCRemoteInitiator : public NFCEndpoint, public NFCNDEFCapable {
    public:
        NFCRemoteInitiator();
        virtual ~NFCRemoteInitiator();

        struct Delegate : NFCEndpoint::Delegate {
            virtual void on_selected() {}
            virtual void on_deselected() {}

            virtual void on_before_ndef_message_read() {}
            virtual void on_after_ndef_message_write() {}
        };

        void set_delegate(Delegate* delegate);

        nfc_tag_type_t nfc_tag_type() const;
        bool is_iso7816_supported() const;
        void add_iso7816_application(ISO7816App* application);
    };

} // namespace nfc
} // namespace mbed

#endif
