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
#include "NFCNDEFCapable.h"

#include "platform/Span.h"

namespace mbed {
namespace nfc {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * This class represents a NFC target (either a remote target when the local controller in in initiator mode, or a target connected through a wired connection).
 *
 * A target can be a NFC tag/card, a NFC-enabled phone or other NFC device capable of modulating a RF field.
 */
class NFCTarget : public NFCNDEFCapable {
public:
    /**
     * Create a NFCTarget.
     *
     * @param[in] buffer a bytes array used to store NDEF messages
     */
    NFCTarget(const Span<uint8_t> &buffer);

    /**
     * NFCTarget destructor
     */
    virtual ~NFCTarget();

    struct Delegate : NFCNDEFCapable::Delegate {
        /**
         * The NDEF message erasing request completed.
         *
         * @param[in] result NFC_OK or an error code on failure
         */
        virtual void on_ndef_message_erased(nfc_err_t result) {}

        /**
         * The NDEF message writing request completed.
         *
         * @param[in] result NFC_OK or an error code on failure
         */
        virtual void on_ndef_message_written(nfc_err_t result) {}

        /**
         * The NDEF message reading request completed.
         *
         * @param[in] result NFC_OK or an error code on failure
         */
        virtual void on_ndef_message_read(nfc_err_t result) {}

    protected:
        ~Delegate() {}
    };

    /**
     * Write a NDEF message to the target.
     *
     * on_ndef_message_written() will be called on completion.
     */
    virtual void write_ndef_message() = 0;

    /**
     * Read a NDEF message from the target.
     *
     * on_ndef_message_read() will be called on completion.
     */
    virtual void read_ndef_message() = 0;

    /**
     * Erase the NDEF message in the target.
     *
     * on_ndef_message_erased() will be called on completion.
     */
    virtual void erase_ndef_message() = 0;
};

/**
 * @}
 */

} // namespace nfc
} // namespace mbed

#endif
