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

#ifndef BLE_CLIAPP_BLE_TRACE_HELPERS_H
#define BLE_CLIAPP_BLE_TRACE_HELPERS_H

#include "ble/SecurityManager.h"
#include "mbed-trace/mbed_trace.h"
#include "pal/GapTypes.h"
#include "ble-host/include/dm_api.h"
#include "common/UUID.h"

namespace ble {

#if MBED_CONF_MBED_TRACE_ENABLE
void trace_le_supported_features(uint64_t feat);

const char* dm_callback_event_to_string(uint8_t event);
const char* hci_opcode_to_string(uint16_t opcode);
const char* attribute_error_code_to_string(uint8_t error);
const char* attribute_opcode_to_string(uint8_t error);
const char* ble_error_to_string(ble_error_t error);
const char* att_client_callback_event_to_string(uint8_t code);
const char* att_error_code_to_string(uint8_t code);
#endif //MBED_CONF_MBED_TRACE_ENABLE

template<typename T>
static inline const char* tr_as_array(T item)
{
    return (mbed_trace_array)((const uint8_t*)&item, sizeof(item));
}

static inline const char* to_string(UUID uuid)
{
    return (mbed_trace_array)(uuid.getBaseUUID(), uuid.getLen());
}

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

static inline const char* to_string(ble::io_capability_t capabilities)
{
    switch (capabilities.value()) {
        case ble::io_capability_t::DISPLAY_ONLY:
            return "DISPLAY_ONLY";
        case ble::io_capability_t::DISPLAY_YES_NO:
            return "IO_CAPS_DISPLAY_DISPLAY_YES_NOYESNO";
        case ble::io_capability_t::KEYBOARD_ONLY:
            return "KEYBOARD_ONLY";
        case ble::io_capability_t::NO_INPUT_NO_OUTPUT:
            return "NO_INPUT_NO_OUTPUT";
        case ble::io_capability_t::KEYBOARD_DISPLAY:
            return "KEYBOARD_DISPLAY";
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

static inline const char *to_string(ble::pairing_failure_t reason)
{
    switch (reason.value()) {
        case ble::pairing_failure_t::PASSKEY_ENTRY_FAILED:
            return "PASSKEY_ENTRY_FAILED";
        case ble::pairing_failure_t::OOB_NOT_AVAILABLE:
            return "OOB_NOT_AVAILABLE";
        case ble::pairing_failure_t::AUTHENTICATION_REQUIREMENTS:
            return "AUTHENTICATION_REQUIREMENTS";
        case ble::pairing_failure_t::CONFIRM_VALUE_FAILED:
            return "CONFIRM_VALUE_FAILED";
        case ble::pairing_failure_t::PAIRING_NOT_SUPPORTED:
            return "PAIRING_NOT_SUPPORTED";
        case ble::pairing_failure_t::ENCRYPTION_KEY_SIZE:
            return "ENCRYPTION_KEY_SIZE";
        case ble::pairing_failure_t::COMMAND_NOT_SUPPORTED:
            return "COMMAND_NOT_SUPPORTED";
        case ble::pairing_failure_t::UNSPECIFIED_REASON:
            return "UNSPECIFIED_REASON";
        case ble::pairing_failure_t::REPEATED_ATTEMPTS:
            return "REPEATED_ATTEMPTS";
        case ble::pairing_failure_t::INVALID_PARAMETERS:
            return "INVALID_PARAMETERS";
        case ble::pairing_failure_t::DHKEY_CHECK_FAILED:
            return "DHKEY_CHECK_FAILED";
        case ble::pairing_failure_t::NUMERIC_COMPARISON_FAILED:
            return "NUMERIC_COMPARISON_FAILED";
        case ble::pairing_failure_t::BR_EDR_PAIRING_IN_PROGRESS:
            return "BR_EDR_PAIRING_IN_PROGRESS";
        case ble::pairing_failure_t::CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED:
            return "CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED";
        default:
            return "Unknown";
    }
}

static inline const char *to_string(target_peer_address_type_t type)
{
    if (type == target_peer_address_type_t::PUBLIC) {
        return "PUBLIC";
    } else {
        return "RANDOM";
    }
}

static inline const char *to_string(privacy_mode_t mode)
{
    if (mode == privacy_mode_t::NETWORK) {
        return "NETWORK";
    } else {
        return "DEVICE";
    }
}

static inline const char* to_string(ble::advertising_type_t advType) {
    switch (advType.value()) {
        case ble::advertising_type_t::CONNECTABLE_UNDIRECTED:
            return "CONNECTABLE_UNDIRECTED";
        case ble::advertising_type_t::CONNECTABLE_DIRECTED:
            return "CONNECTABLE_DIRECTED";
        case ble::advertising_type_t::SCANNABLE_UNDIRECTED:
            return "SCANNABLE_UNDIRECTED";
        case ble::advertising_type_t::NON_CONNECTABLE_UNDIRECTED:
            return "NON_CONNECTABLE_UNDIRECTED";
        case ble::advertising_type_t::CONNECTABLE_DIRECTED_LOW_DUTY:
            return "CONNECTABLE_DIRECTED_LOW_DUTY";
        case ble::advertising_type_t::CONNECTABLE_NON_SCANNABLE_UNDIRECTED:
            return "CONNECTABLE_NON_SCANNABLE_UNDIRECTED";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::own_address_type_t addressType) {
    switch (addressType.value()) {
        case ble::own_address_type_t::PUBLIC:
            return "PUBLIC";
        case ble::own_address_type_t::RANDOM:
            return "RANDOM";
        case ble::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK:
            return "RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK";
        case ble::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK:
            return "RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::advertising_filter_policy_t policy) {
    switch (policy.value()) {
        case ble::advertising_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::advertising_filter_policy_t::FILTER_SCAN_REQUESTS:
            return "FILTER_SCAN_REQUESTS";
        case ble::advertising_filter_policy_t::FILTER_CONNECTION_REQUEST:
            return "FILTER_CONNECTION_REQUEST";
        case ble::advertising_filter_policy_t::FILTER_SCAN_AND_CONNECTION_REQUESTS:
            return "FILTER_SCAN_AND_CONNECTION_REQUESTS";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::phy_t phy) {
    switch (phy.value()) {
        case ble::phy_t::NONE:
            return "NONE";
        case ble::phy_t::LE_1M:
            return "LE_1M";
        case ble::phy_t::LE_2M:
            return "LE_2M";
        case ble::phy_t::LE_CODED:
            return "LE_CODED";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::scanning_filter_policy_t policy) {
    switch (policy.value()) {
        case ble::scanning_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::scanning_filter_policy_t::FILTER_ADVERTISING:
            return "FILTER_ADVERTISING";
        case ble::scanning_filter_policy_t::NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED:
            return "NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED";
        case ble::scanning_filter_policy_t::FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED:
            return "FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::duplicates_filter_t filtering) {
    switch (filtering.value()) {
        case ble::duplicates_filter_t::ENABLE:
            return "ENABLE";
        case ble::duplicates_filter_t::DISABLE:
            return "DISABLE";
        case ble::duplicates_filter_t::PERIODIC_RESET:
            return "PERIODIC_RESET";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::peer_address_type_t addressType) {
    switch (addressType.value()) {
        case ble::peer_address_type_t::PUBLIC:
            return "PUBLIC";
        case ble::peer_address_type_t::RANDOM:
            return "RANDOM";
        case ble::peer_address_type_t::PUBLIC_IDENTITY:
            return "PUBLIC_IDENTITY";
        case ble::peer_address_type_t::RANDOM_STATIC_IDENTITY:
            return "RANDOM_STATIC_IDENTITY";
        case ble::peer_address_type_t::ANONYMOUS:
            return "ANONYMOUS";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::initiator_filter_policy_t policy) {
    switch(policy.value()) {
        case ble::initiator_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::initiator_filter_policy_t::USE_WHITE_LIST:
            return "USE_WHITE_LIST";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::coded_symbol_per_bit_t sym) {
    switch (sym.value()) {
        case ble::coded_symbol_per_bit_t::UNDEFINED:
            return "UNDEFINED";
        case ble::coded_symbol_per_bit_t::S2:
            return "S2";
        case ble::coded_symbol_per_bit_t::S8:
            return "S8";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::local_disconnection_reason_t reason) {
    switch (reason.value()) {
        case ble::local_disconnection_reason_t::AUTHENTICATION_FAILURE:
            return "AUTHENTICATION_FAILURE";
        case ble::local_disconnection_reason_t::USER_TERMINATION:
            return "USER_TERMINATION";
        case ble::local_disconnection_reason_t::LOW_RESOURCES:
            return "LOW_RESOURCES";
        case ble::local_disconnection_reason_t::POWER_OFF:
            return "POWER OFF";
        case ble::local_disconnection_reason_t::UNSUPPORTED_REMOTE_FEATURE:
            return "UNSUPPORTED_REMOTE_FEATURE";
        case ble::local_disconnection_reason_t::PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
            return "PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED";
        case ble::local_disconnection_reason_t::UNACCEPTABLE_CONNECTION_PARAMETERS:
            return "UNACCEPTABLE_CONNECTION_PARAMETERS";
        default:
            return "unknown";
    }
}

} // namespace ble

#endif //BLE_CLIAPP_BLE_TRACE_HELPERS_H
