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

#ifndef MBED_NFC_EEPROM_DRIVER_H
#define MBED_NFC_EEPROM_DRIVER_H

#include <stdint.h>

#include "NFCDefinitions.h"
#include "NFCTarget.h"
#include "NFCEEPROMDriver.h"

namespace mbed {
namespace nfc {

    class NFCEEPROMDriver  {
    public:
        NFCEEPROMDriver();
        
        struct Delegate {
            virtual void has_started_session(bool success) = 0;
            virtual void has_read_bytes(bool success, const uint8_t* bytes) = 0;
            virtual void has_written_bytes(bool success) = 0;
            virtual void has_set_size(bool success) = 0;
            virtual void has_gotten_size(bool success, size_t size) = 0;
            virtual void has_erased_bytes(bool success) = 0;
        };

        void set_delegate(Delegate* delegate);

        virtual void reset() = 0;
        virtual size_t get_max_size() = 0;
        virtual void start_session() = 0; // This could lock the chip's RF interface
        virtual void end_session() = 0;
        virtual void read_bytes(uint32_t address, size_t count) = 0;
        virtual void write_bytes(uint32_t address, const uint8_t* bytes, size_t count) = 0;
        virtual void set_size(size_t count) = 0;
        virtual void get_size() = 0;
        virtual void erase_bytes(uint32_t address, size_t size) = 0;

    protected:  
        Delegate* delegate();

    private:
        Delegate* _delegate;
    };

} // namespace nfc
} // namespace mbed

#endif
