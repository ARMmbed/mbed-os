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

#include "ble/BLE.h"
#include "ble/SecurityManager.h"
#include "mbed-trace/mbed_trace.h"
#include "pal/GapTypes.h"
#include "pal/GapEvents.h"
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

static inline char* to_string(const UUID& uuid)
{
    const uint8_t* Buffer    = uuid.getBaseUUID();
    const uint8_t  Length    = uuid.getLen();
    const  size_t  Row_Count = 4;

    const char *Hex = "0123456789ABCDEF";

    static char string_buffer[Row_Count][UUID::LENGTH_OF_LONG_UUID + /* Number of hyphen delimiters =*/ 4];
    static uint8_t idx = 0;

    ++idx;

    if (idx == Row_Count) {
        idx= 0;
    }

    char* p1  = (char *)Buffer + Length - 1;
    char* p2  = string_buffer[idx];

    if (Length == UUID::LENGTH_OF_LONG_UUID) {
        const uint8_t format[] = {4, 2, 2, 2, 6};
        for ( uint8_t i: format) {
            size_t j = 0;
            for (; j < i; ++j) {
                *p2++ = Hex[(*p1 >> 4) & 0xF];
                *p2++ = Hex[(*p1--)    & 0xF];
            }
            *p2++ = '-';
        }
        *--p2 = 0;
    } else {
        size_t i = 0;
        for (; i < Length; ++i) {
            *p2++ = Hex[(*p1 >> 4) & 0xF];
            *p2++ = Hex[(*p1--)    & 0xF];
        }
        *p2 = 0;
    }

    return string_buffer[idx];
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

static inline const char* to_string(ble::pairing_failure_t reason)
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

static inline const char* to_string(target_peer_address_type_t type)
{
    if (type == target_peer_address_type_t::PUBLIC) {
        return "PUBLIC";
    } else {
        return "RANDOM";
    }
}

static inline const char* to_string(privacy_mode_t mode)
{
    if (mode == privacy_mode_t::NETWORK) {
        return "NETWORK";
    } else {
        return "DEVICE";
    }
}

static inline const char* to_string(ble::advertising_type_t advType)
{
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

static inline const char* to_string(ble::own_address_type_t addressType)
{
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

static inline const char* to_string(ble::advertising_filter_policy_t policy)
{
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

static inline const char* to_string(ble::phy_t phy)
{
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

static inline const char* to_string(ble::scanning_filter_policy_t policy)
{
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

static inline const char* to_string(ble::duplicates_filter_t filtering)
{
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

static inline const char* to_string(ble::peer_address_type_t addressType)
{
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

static inline const char* to_string(ble::initiator_filter_policy_t policy)
{
    switch(policy.value()) {
        case ble::initiator_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::initiator_filter_policy_t::USE_WHITE_LIST:
            return "USE_WHITE_LIST";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::coded_symbol_per_bit_t sym)
{
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

static inline const char* to_string(ble::local_disconnection_reason_t reason)
{
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

static inline const char* to_string(ble::advertising_channel_map_t map)
{
    switch (map.value()) {
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37:
            return "[001]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_38:
            return "[010]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37_AND_38:
            return "[011]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_39:
            return "[100]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37_AND_39:
            return "[101]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_38_AND_39:
            return "[110]";
        case ble::advertising_channel_map_t::ALL_ADVERTISING_CHANNELS:
            return "[111]";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::connection_peer_address_type_t connection_peer_address_type)
{
    switch (connection_peer_address_type.value()) {
        case ble::connection_peer_address_type_t::PUBLIC_ADDRESS:
            return "PUBLIC_ADDRESS";
        case ble::connection_peer_address_type_t::RANDOM_ADDRESS:
            return "RANDOM_ADDRESS";
        case ble::connection_peer_address_type_t::PUBLIC_IDENTITY_ADDRESS:
            return "PUBLIC_IDENTITY_ADDRESS";
        case ble::connection_peer_address_type_t::RANDOM_IDENTITY_ADDRESS:
            return "RANDOM_IDENTITY_ADDRESS";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::whitelist_address_type_t whitelist_address_type)
{
    if (whitelist_address_type.value() == ble::whitelist_address_type_t::PUBLIC_DEVICE_ADDRESS) {
        return "PUBLIC_DEVICE_ADDRESS";
    } else {
        return "RANDOM_DEVICE_ADDRESS";
    }
}

static inline const char* to_string(ble::hci_error_code_t error)
{
    switch (error.value()) {
        case ble::hci_error_code_t::SUCCESS:
            return "SUCCESS";
        case ble::hci_error_code_t::UNKNOWN_HCI_COMMAND:
            return "UNKNOWN_HCI_COMMAND";
        case ble::hci_error_code_t::UNKNOWN_CONNECTION_IDENTIFIER:
            return "UNKNOWN_CONNECTION_IDENTIFIER";
        case ble::hci_error_code_t::HARDWARE_FAILLURE:
            return "HARDWARE_FAILLURE";
        case ble::hci_error_code_t::PAGE_TIMEOUT:
            return "PAGE_TIMEOUT";
        case ble::hci_error_code_t::AUTHENTICATION_FAILLURE:
            return "AUTHENTICATION_FAILLURE";
        case ble::hci_error_code_t::PIN_OR_KEY_MISSING:
            return "PIN_OR_KEY_MISSING";
        case ble::hci_error_code_t::MEMORY_CAPACITY_EXCEEDED:
            return "MEMORY_CAPACITY_EXCEEDED";
        case ble::hci_error_code_t::CONNECTION_TIMEOUT:
            return "CONNECTION_TIMEOUT";
        case ble::hci_error_code_t::CONNECTION_LIMIT_EXCEEDED:
            return "CONNECTION_LIMIT_EXCEEDED";
        case ble::hci_error_code_t::SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED:
            return "SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED";
        case ble::hci_error_code_t::CONNECTION_ALREADY_EXIST:
            return "CONNECTION_ALREADY_EXIST";
        case ble::hci_error_code_t::COMMAND_DISALLOWED:
            return "COMMAND_DISALLOWED";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES:
            return "CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS:
            return "CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR:
            return "CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR";
        case ble::hci_error_code_t::CONNECTION_ACCEPT_TIMEOUT_EXCEEDED:
            return "CONNECTION_ACCEPT_TIMEOUT_EXCEEDED";
        case ble::hci_error_code_t::UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE:
            return "UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE";
        case ble::hci_error_code_t::INVALID_HCI_COMMAND_PARAMETERS:
            return "INVALID_HCI_COMMAND_PARAMETERS";
        case ble::hci_error_code_t::REMOTE_USER_TERMINATED_CONNECTION:
            return "REMOTE_USER_TERMINATED_CONNECTION";
        case ble::hci_error_code_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES:
            return "REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES";
        case ble::hci_error_code_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF:
            return "REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF";
        case ble::hci_error_code_t::CONNECTION_TERMINATED_BY_LOCAL_HOST:
            return "CONNECTION_TERMINATED_BY_LOCAL_HOST";
        case ble::hci_error_code_t::REPEATED_ATTEMPTS:
            return "REPEATED_ATTEMPTS";
        case ble::hci_error_code_t::PAIRING_NOT_ALLOWED:
            return "PAIRING_NOT_ALLOWED";
        case ble::hci_error_code_t::UNKNOWN_LMP_PDU:
            return "UNKNOWN_LMP_PDU";
        case ble::hci_error_code_t::UNSUPPORTED_REMOTE_FEATURE | ble::hci_error_code_t::UNSUPPORTED_LMP_FEATURE:
            return "UNSUPPORTED_REMOTE_FEATURE/UNSUPPORTED_LMP_FEATURE";
        case ble::hci_error_code_t::SCO_OFFSET_REJECTED:
            return "SCO_OFFSET_REJECTED";
        case ble::hci_error_code_t::SCO_INTERVAL_REJECTED:
            return "SCO_INTERVAL_REJECTED";
        case ble::hci_error_code_t::SCO_AIR_MODE_REJECTED:
            return "SCO_AIR_MODE_REJECTED";
        case ble::hci_error_code_t::INVALID_LMP_PARAMETERS | ble::hci_error_code_t::INVALID_LL_PARAMETERS:
            return "INVALID_LMP_PARAMETERS/INVALID_LL_PARAMETERS";
        case ble::hci_error_code_t::UNSPECIFIED_ERROR:
            return "UNSPECIFIED_ERROR";
        case ble::hci_error_code_t::UNSUPPORTED_LMP_PARAMETER_VALUE | ble::hci_error_code_t::UNSUPPORTED_LL_PARAMETER_VALUE:
            return "UNSUPPORTED_LMP_PARAMETER_VALUE/UNSUPPORTED_LL_PARAMETER_VALUE";
        case ble::hci_error_code_t::ROLE_CHANGE_NOT_ALLOWED:
            return "ROLE_CHANGE_NOT_ALLOWED";
        case ble::hci_error_code_t::LMP_RESPONSE_TIMEOUT | ble::hci_error_code_t::LL_RESPONSE_TIMEOUT:
            return "LMP_RESPONSE_TIMEOUT/LL_RESPONSE_TIMEOUT";
        case ble::hci_error_code_t::LMP_ERROR_TRANSACTION_COLLISION | ble::hci_error_code_t::LL_PROCEDURE_COLLISION:
            return "LMP_ERROR_TRANSACTION_COLLISION/LL_PROCEDURE_COLLISION";
        case ble::hci_error_code_t::LMP_PDU_NOT_ALLOWED:
            return "LMP_PDU_NOT_ALLOWED";
        case ble::hci_error_code_t::ENCRYPTION_MODE_NOT_ACCEPTABLE:
            return "ENCRYPTION_MODE_NOT_ACCEPTABLE";
        case ble::hci_error_code_t::LINK_KEY_CANNOT_BE_CHANGED:
            return "LINK_KEY_CANNOT_BE_CHANGED";
        case ble::hci_error_code_t::REQUESTED_QOS_NOT_SUPPORTED:
            return "REQUESTED_QOS_NOT_SUPPORTED";
        case ble::hci_error_code_t::INSTANT_PASSED:
            return "INSTANT_PASSED";
        case ble::hci_error_code_t::PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
            return "PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED";
        case ble::hci_error_code_t::DIFFERENT_TRANSACTION_COLLISION:
            return "DIFFERENT_TRANSACTION_COLLISION";
        case ble::hci_error_code_t::RESERVED_FOR_FUTURE_USE:
            return "RESERVED_FOR_FUTURE_USE";
        case ble::hci_error_code_t::QOS_UNACCEPTABLE_PARAMETER:
            return "QOS_UNACCEPTABLE_PARAMETER";
        case ble::hci_error_code_t::QOS_REJECTED:
            return "QOS_REJECTED";
        case ble::hci_error_code_t::CHANNEL_CLASSIFICATION_NOT_SUPPORTED:
            return "CHANNEL_CLASSIFICATION_NOT_SUPPORTED";
        case ble::hci_error_code_t::INSUFFICIENT_SECURITY:
            return "INSUFFICIENT_SECURITY";
        case ble::hci_error_code_t::PARAMETER_OUT_OF_MANDATORY_RANGE:
            return "PARAMETER_OUT_OF_MANDATORY_RANGE";
        case ble::hci_error_code_t::ROLE_SWITCH_PENDING:
            return "ROLE_SWITCH_PENDING";
        case ble::hci_error_code_t::RESERVED_SLOT_VIOLATION:
            return "RESERVED_SLOT_VIOLATION";
        case ble::hci_error_code_t::ROLE_SWITCH_FAILED:
            return "ROLE_SWITCH_FAILED";
        case ble::hci_error_code_t::EXTENDED_INQUIRY_RESPONSE_TOO_LARGE:
            return "EXTENDED_INQUIRY_RESPONSE_TOO_LARGE";
        case ble::hci_error_code_t::SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST:
            return "SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST";
        case ble::hci_error_code_t::HOST_BUSY_PAIRING:
            return "HOST_BUSY_PAIRING";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND:
            return "CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND";
        case ble::hci_error_code_t::CONTROLLER_BUSY:
            return "CONTROLLER_BUSY";
        case ble::hci_error_code_t::UNACCEPTABLE_CONNECTION_PARAMETERS:
            return "UNACCEPTABLE_CONNECTION_PARAMETERS";
        case ble::hci_error_code_t::ADVERTISING_TIMEOUT:
            return "ADVERTISING_TIMEOUT";
        case ble::hci_error_code_t::CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE:
            return "CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE";
        case ble::hci_error_code_t::CONNECTION_FAILED_TO_BE_ESTABLISHED:
            return "CONNECTION_FAILED_TO_BE_ESTABLISHED";
        case ble::hci_error_code_t::MAC_CONNECTION_FAILED:
            return "MAC_CONNECTION_FAILED";
        case ble::hci_error_code_t::COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING:
            return "COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING";
        case ble::hci_error_code_t::TYPE0_SUBMAP_NOT_DEFINED:
            return "TYPE0_SUBMAP_NOT_DEFINED";
        case ble::hci_error_code_t::UNKNOWN_ADVERTISING_IDENTIFIER:
            return "UNKNOWN_ADVERTISING_IDENTIFIER";
        case ble::hci_error_code_t::LIMIT_REACHED:
            return "LIMIT_REACHED";
        case ble::hci_error_code_t::OPERATION_CANCELLED_BY_HOST:
            return "OPERATION_CANCELLED_BY_HOST";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::advertising_fragment_description_t fragment_description)
{
    switch (fragment_description.value()) {
        case ble::advertising_fragment_description_t::INTERMEDIATE_FRAGMENT:
            return "INTERMEDIATE_FRAGMENT";
        case ble::advertising_fragment_description_t::FIRST_FRAGMENT:
            return "FIRST_FRAGMENT";
        case ble::advertising_fragment_description_t::LAST_FRAGMENT:
            return "LAST_FRAGMENT";
        case ble::advertising_fragment_description_t::COMPLETE_FRAGMENT:
            return "COMPLETE_FRAGMENT";
        case ble::advertising_fragment_description_t::UNCHANGED_DATA:
            return "UNCHANGED_DATA";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::peripheral_privacy_configuration_t::resolution_strategy_t resolution_strategy)
{
   switch (resolution_strategy) {
       case ble::peripheral_privacy_configuration_t::resolution_strategy_t::DO_NOT_RESOLVE:
           return "DO_NOT_RESOLVE";
       case ble::peripheral_privacy_configuration_t::resolution_strategy_t::REJECT_NON_RESOLVED_ADDRESS:
           return "REJECT_NON_RESOLVED_ADDRESS";
       case ble::peripheral_privacy_configuration_t::resolution_strategy_t::PERFORM_PAIRING_PROCEDURE:
           return "PERFORM_PAIRING_PROCEDURE";
       case ble::peripheral_privacy_configuration_t::resolution_strategy_t::PERFORM_AUTHENTICATION_PROCEDURE:
           return "PERFORM_AUTHENTICATION_PROCEDURE";
       default:
           return "unknown";
   }
}

static inline const char* to_string(ble::GapEventType event_type)
{
    switch (event_type.value()) {
        case ble::GapEventType::type::UNEXPECTED_ERROR:
            return "UNEXPECTED_ERROR";
        case ble::GapEventType::type::CONNECTION_COMPLETE:
            return "CONNECTION_COMPLETE";
        case ble::GapEventType::type::ADVERTISING_REPORT:
            return "ADVERTISING_REPORT";
        case ble::GapEventType::type::CONNECTION_UPDATE:
            return "CONNECTION_UPDATE";
        case ble::GapEventType::type::REMOTE_CONNECTION_PARAMETER_REQUEST:
            return "REMOTE_CONNECTION_PARAMETER_REQUEST";
        case ble::GapEventType::type::DISCONNECTION_COMPLETE:
            return "DISCONNECTION_COMPLETE";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::received_advertising_type_t type)
{
    switch (type.value()) {
        case ble::received_advertising_type_t::ADV_IND:
            return "ADV_IND";
        case ble::received_advertising_type_t::ADV_DIRECT_IND:
            return "ADV_DIRECT_IND";
        case ble::received_advertising_type_t::ADV_SCAN_IND:
            return "ADV_SCAN_IND";
        case ble::received_advertising_type_t::ADV_NONCONN_IND:
            return "ADV_NONCONN_IND";
        case ble::received_advertising_type_t::SCAN_RESPONSE:
            return "SCAN_RESPONSE";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::connection_role_t role)
{
    if (role.value() == ble::connection_role_t::CENTRAL) {
        return "CENTRAL";
    } else {
        return "PERIPHERAL";
    }
}

static inline const char* to_string(ble::direct_address_type_t direct_address_type)
{
    switch (direct_address_type.value()) {
        case ble::direct_address_type_t::PUBLIC_ADDRESS:
            return "PUBLIC_ADDRESS";
        case ble::direct_address_type_t::RANDOM_ADDRESS:
            return "RANDOM_ADDRESS";
        case ble::direct_address_type_t::PUBLIC_IDENTITY_ADDRESS:
            return "PUBLIC_IDENTITY_ADDRESS";
        case ble::direct_address_type_t::RANDOM_IDENTITY_ADDRESS:
            return "RANDOM_IDENTITY_ADDRESS";
        case ble::direct_address_type_t::RANDOM_DEVICE_ADDRESS:
            return "RANDOM_DEVICE_ADDRESS";
        default:
            return "unknown";
    }
}

static inline const char* to_string(ble::advertising_data_status_t data_status) {
    switch (data_status.value()) {
        case ble::advertising_data_status_t::COMPLETE:
            return "COMPLETE";
        case ble::advertising_data_status_t::INCOMPLETE_MORE_DATA:
            return "INCOMPLETE_MORE_DATA";
        case ble::advertising_data_status_t::INCOMPLETE_DATA_TRUNCATED:
            return "INCOMPLETE_DATA_TRUNCATED";
        default:
            return "unknown";
    }
}
static inline const char* to_string(ble::att_security_requirement_t security_requirement)
{
    switch (security_requirement.value()) {
        case ble::att_security_requirement_t::NONE:
            return "NONE";
        case ble::att_security_requirement_t::UNAUTHENTICATED:
            return "UNAUTHENTICATED";
        case ble::att_security_requirement_t::AUTHENTICATED:
            return "AUTHENTICATED";
        case ble::att_security_requirement_t::SC_AUTHENTICATED:
            return "SC_AUTHENTICATED";
        default:
            return "unknown";
    }
}

static inline const char* to_string(GattAuthCallbackReply_t authorization_reply)
{
    switch (authorization_reply) {
        case AUTH_CALLBACK_REPLY_SUCCESS:
            return "SUCCESS";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_HANDLE:
            return "INVALID_HANDLE";
        case AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED:
            return "READ_NOT_PERMITTED";
        case AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED:
            return "WRITE_NOT_PERMITTED";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_PDU:
            return "INVALID_PDU";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHENTICATION:
            return "INSUFFICIENT_AUTHENTICATION";
        case AUTH_CALLBACK_REPLY_ATTERR_REQUEST_NOT_SUPPORTED:
            return "REQUEST_NOT_SUPPORTED";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET:
            return "INVALID_OFFSET";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHORIZATION:
            return "INSUFFICIENT_AUTHORIZATION";
        case AUTH_CALLBACK_REPLY_ATTERR_PREPARE_QUEUE_FULL:
            return "PREPARE_QUEUE_FULL";
        case AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_FOUND:
            return "ATTRIBUTE_NOT_FOUND";
        case AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_LONG:
            return "ATTRIBUTE_NOT_LONG";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION_KEY_SIZE:
            return "INSUFFICIENT_ENCRYPTION_KEY_SIZE";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATTRIBUTE_VALUE_LENGTH:
            return "INVALID_ATTRIBUTE_VALUE_LENGTH";
        case AUTH_CALLBACK_REPLY_ATTERR_UNLIKELY_ERROR:
            return "UNLIKELY_ERROR";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION:
            return "INSUFFICIENT_ENCRYPTION";
        case AUTH_CALLBACK_REPLY_ATTERR_UNSUPPORTED_GROUP_TYPE:
            return "UNSUPPORTED_GROUP_TYPE";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_RESOURCES:
            return "INSUFFICIENT_RESOURCES";
        case AUTH_CALLBACK_REPLY_ATTERR_WRITE_REQUEST_REJECTED:
            return "WRITE_REQUEST_REJECTED";
        case AUTH_CALLBACK_REPLY_ATTERR_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED:
            return "CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED";
        case AUTH_CALLBACK_REPLY_ATTERR_PROCEDURE_ALREADY_IN_PROGRESS:
            return "PROCEDURE_ALREADY_IN_PROGRESS";
        case AUTH_CALLBACK_REPLY_ATTERR_OUT_OF_RANGE:
            return "OUT_OF_RANGE";
        default:
            return "unknown";
    }
}

} // namespace ble

#endif //BLE_CLIAPP_BLE_TRACE_HELPERS_H
