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

#ifndef MBED_NFC_NDEF_CAPABLE_H
#define MBED_NFC_NDEF_CAPABLE_H

#include <stdint.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */
    
    /**
     * The base class for all endpoints that can support NDEF content.
     */ 
    class NFCNDEFCapable {
    public:
        /**
         * Check if this instance actually supports NDEF content.
         * @return whether NDEF content is supported
         */ 
        virtual bool is_ndef_supported() const { return false; }

        /**
         * Set the instance of the parser which should handle incoming NDEF messages.
         * If set to NULL, incoming NDEF messages will be rejected
         * @param[in] parser a pointer to the parser to use, or NULL
         */ 
        void set_ndef_message_parser(ndef::MessageParser* parser) const;

        /**
         * Set the instance of the build which should generate outgoing NDEF messages.
         * If set to NULL, outgoing NDEF messages will not be produced
         * @param[in] builder a pointer to the builder to use, or NULL
         */
        void set_ndef_message_builder(ndef::MessageBuilder* builder) const;

    protected:
        /**
         * Get the NDEF message parser to use, or NULL.
         * @return a pointer to a parser, or NULL
         */ 
        ndef::MessageParser* ndef_message_parser();

        /**
         * Get the NDEF message builder to use, or NULL.
         * @return a pointer to a builder, or NULL
         */ 
        ndef::MessageBuilder* ndef_message_builder();

    private:
        ndef::MessageParser* _message_parser;
        ndef::MessageBuilder* _message_builder;
    };

    /**
     * @}
     */

} // namespace nfc
} // namespace mbed

#endif
