/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
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

#ifndef BLE_CLIAPP_BLE_TO_STRING_H
#define BLE_CLIAPP_BLE_TO_STRING_H

#include "ble/SecurityManager.h"
#include "mbed-trace/mbed_trace.h"

namespace ble {

static inline constexpr const char* to_string(bool v)
{
    if (v) {
        return "true";
    } else {
        return "false";
    }
}

static inline constexpr const char* to_string(ble::SecurityManager::SecurityIOCapabilities_t capabilities)
{
    switch(capabilities) {
        case ble::SecurityManager::IO_CAPS_DISPLAY_ONLY:
            return "IO_CAPS_DISPLAY_ONLY";
        case ble::SecurityManager::IO_CAPS_DISPLAY_YESNO:
            return "IO_CAPS_DISPLAY_YESNO";
        case ble::SecurityManager::IO_CAPS_KEYBOARD_DISPLAY:
            return "IO_CAPS_KEYBOARD_DISPLAY";
        case ble::SecurityManager::IO_CAPS_KEYBOARD_ONLY:
            return "IO_CAPS_KEYBOARD_ONLY";
        case ble::SecurityManager::IO_CAPS_NONE:
            return "IO_CAPS_NONE";
        default:
            return "unknown";
    }
}

static inline constexpr const char* to_string(ble::SecurityManager::SecurityMode_t security_mode)
{
    switch (security_mode) {
        case ble::SecurityManager::SECURITY_MODE_NO_ACCESS:
            return "SECURITY_MODE_NO_ACCESS";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            return "SECURITY_MODE_ENCRYPTION_OPEN_LINK";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM:
            return "SECURITY_MODE_ENCRYPTION_NO_MITM";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM:
            return "SECURITY_MODE_ENCRYPTION_WITH_MITM";
        case ble::SecurityManager::SECURITY_MODE_SIGNED_NO_MITM:
            return "SECURITY_MODE_SIGNED_NO_MITM";
        case ble::SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM:
            return "SECURITY_MODE_SIGNED_WITH_MITM";
        default:
            return "Unknown";
    }
}

static inline constexpr const char* passkey_str(const uint8_t* passkey)
{
    if (!passkey) {
        return "0";
    } else {
        return (mbed_trace_array)(passkey, 6);
    }
}

static inline const char* to_string(const ble::address_t& address)
{
    return (mbed_trace_array)(address.data(), address.size());
}

template<size_t size>
const char* to_string(const ble::byte_array_t<size> &array)
{
    return (mbed_trace_array)(array.data(), array.size());
}

static inline const char* to_string(const ble::link_encryption_t encryption)
{
    using link_encryption_t = ble::link_encryption_t;

    switch (encryption.value()) {
        case link_encryption_t::NOT_ENCRYPTED:
            return "NOT_ENCRYPTED";
        case link_encryption_t::ENCRYPTION_IN_PROGRESS:
            return "ENCRYPTION_IN_PROGRESS";
        case link_encryption_t::ENCRYPTED:
            return "ENCRYPTED";
        case link_encryption_t::ENCRYPTED_WITH_MITM:
            return "ENCRYPTED_WITH_MITM";
        case link_encryption_t::ENCRYPTED_WITH_SC_AND_MITM:
            return "ENCRYPTED_WITH_SC_AND_MITM";
        default:
            return "Unknown";
    }
}

static inline const char* to_string(Keypress_t keypress)
{
    switch (keypress) {
        case KEYPRESS_STARTED:
            return "KEYPRESS_STARTED";
        case KEYPRESS_ENTERED:
            return "KEYPRESS_ENTERED";
        case KEYPRESS_ERASED:
            return "KEYPRESS_ERASED";
        case KEYPRESS_CLEARED:
            return "KEYPRESS_CLEARED";
        case KEYPRESS_COMPLETED:
            return "KEYPRESS_COMPLETED";
        default:
            return "Unknown";
    }
}

} // namespace ble

#endif //BLE_CLIAPP_BLE_TO_STRING_H
