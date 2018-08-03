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

#ifndef MBED_ISO7816_APP_H
#define MBED_ISO7816_APP_H

#include <stdint.h>
#include <stddef.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

    class ISO7816App {
        class Exchange {
        public:
            struct CAPDU
            {
                uint8_t cla;
                uint8_t ins;
                uint8_t p1;
                uint8_t p2;
                ac_buffer_t dataIn;
                size_t maxRespLength;
            };

            struct RAPDU
            {
                ac_buffer_t dataOut;
                uint16_t sw;
            };

            const CAPDU& command() const;
            RAPDU& response();

            nfc_err_t respond();

        private:
            Command _command;
            Response _response;
            ISO7816Application* _app;
        };

        struct Delegate {
            virtual void on_selected() {}

            virtual void on_deselected() {}

            virtual void on_exchange(Exchange* exchange) {}
        };
        
        void set_delegate(Delegate* delegate);
    };

} // namespace nfc
} // namespace mbed

#endif
