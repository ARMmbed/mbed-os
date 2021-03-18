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
#include "dm_api.h"
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

template<size_t size>
const char* to_string(const ble::byte_array_t<size> &array)
{
    return (mbed_trace_array)(array.data(), array.size());
}

const char* to_string(const UUID& uuid);

const char *to_string(bool v);

const char *to_string(ble::SecurityManager::SecurityIOCapabilities_t capabilities);

const char *to_string(ble::io_capability_t capabilities);

const char *to_string(ble::SecurityManager::SecurityMode_t security_mode);

const char *passkey_str(const uint8_t* passkey);

const char *to_string(const ble::address_t& address);

const char *to_string(const ble::link_encryption_t encryption);

const char *to_string(Keypress_t keypress);

const char *to_string(ble::pairing_failure_t reason);

const char *to_string(target_peer_address_type_t type);

const char *to_string(privacy_mode_t mode);

const char *to_string(ble::advertising_type_t advType);

const char *to_string(ble::own_address_type_t addressType);

const char *to_string(ble::advertising_filter_policy_t policy);

const char *to_string(ble::phy_t phy);

const char *to_string(ble::scanning_filter_policy_t policy);

const char *to_string(ble::duplicates_filter_t filtering);

const char *to_string(ble::peer_address_type_t addressType);

const char *to_string(ble::initiator_filter_policy_t policy);

const char *to_string(ble::coded_symbol_per_bit_t sym);

const char *to_string(ble::local_disconnection_reason_t reason);

const char *to_string(ble::advertising_channel_map_t map);

const char *to_string(ble::connection_peer_address_type_t connection_peer_address_type);

const char *to_string(ble::whitelist_address_type_t whitelist_address_type);

const char *to_string(ble::hci_error_code_t error);

const char *to_string(ble::advertising_fragment_description_t fragment_description);

const char *to_string(ble::peripheral_privacy_configuration_t::resolution_strategy_t resolution_strategy);

const char *to_string(ble::GapEventType event_type);

const char *to_string(ble::received_advertising_type_t type);

const char *to_string(ble::connection_role_t role);

const char *to_string(ble::direct_address_type_t direct_address_type);

const char *to_string(ble::advertising_data_status_t data_status);

const char *to_string(ble::att_security_requirement_t security_requirement);

const char *to_string(GattAuthCallbackReply_t authorization_reply);

} // namespace ble

#endif //BLE_CLIAPP_BLE_TRACE_HELPERS_H
