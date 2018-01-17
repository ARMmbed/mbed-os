/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#include "nRF5xPalSecurityManager.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

nRF5xSecurityManager::nRF5xSecurityManager() : ::ble::pal::SecurityManager()
{

}

nRF5xSecurityManager::~nRF5xSecurityManager()
{

}

////////////////////////////////////////////////////////////////////////////
// SM lifecycle management
//

ble_error_t nRF5xSecurityManager::initialize()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::terminate()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::reset()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Resolving list management
//

uint8_t nRF5xSecurityManager::read_resolving_list_capacity()
{
    // FIXME: implement
    return 0;
}

ble_error_t nRF5xSecurityManager::add_device_to_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    address_t peer_identity_address,
    irk_t peer_irk,
    irk_t local_irk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::remove_device_from_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    address_t peer_identity_address
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::clear_resolving_list()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Feature support
//

ble_error_t nRF5xSecurityManager::set_secure_connections_support(
    bool enabled, bool secure_connections_only
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_secure_connections_support(
    bool &enabled, bool &secure_connections_only
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t nRF5xSecurityManager::set_authentication_timeout(
    connection_handle_t, uint16_t timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_authentication_timeout(
    connection_handle_t, uint16_t &timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

ble_error_t nRF5xSecurityManager::enable_encryption(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::disable_encryption(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_encryption_status(
    connection_handle_t connection, LinkSecurityStatus_t &status
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_encryption_key_size(
    connection_handle_t, uint8_t &bitsize
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::refresh_encryption_key(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Privacy
//

ble_error_t nRF5xSecurityManager::set_private_address_timeout(
    uint16_t timeout_in_seconds
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

ble_error_t nRF5xSecurityManager::set_ltk(
    connection_handle_t connection, ltk_t ltk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::set_irk(const irk_t& irk)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::set_csrk(const csrk_t& csrk)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::generate_irk()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::generate_csrk()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

ble_error_t nRF5xSecurityManager::send_pairing_request(
    connection_handle_t connection,
    io_capability_t io_capability,
    bool oob_data_flag,
    authentication_t authentication_requirements,
    uint8_t maximum_encryption_key_size,
    key_distribution_t initiator_dist,
    key_distribution_t responder_dist
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::send_pairing_response(
    connection_handle_t connection,
    io_capability_t io_capability,
    bool oob_data_flag,
    authentication_t authentication_requirements,
    uint8_t maximum_encryption_key_size,
    key_distribution_t initiator_dist,
    key_distribution_t responder_dist
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::cancel_pairing(
    connection_handle_t connection, pairing_failure_t reason
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::request_authentication(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_random_data(random_data_t &random_data)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

ble_error_t nRF5xSecurityManager::passkey_request_reply(
    connection_handle_t connection, const passkey_num_t passkey
) {
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::oob_data_request_reply(
    connection_handle_t connection, const oob_data_t& oob_data
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::confirmation_entered(
    connection_handle_t connection, bool confirmation
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::send_keypress_notification(
    connection_handle_t connection, Keypress_t keypress
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

nRF5xSecurityManager& nRF5xSecurityManager::get_security_manager()
{
    static nRF5xSecurityManager _security_manager;
    return _security_manager;
}

bool nRF5xSecurityManager::sm_handler()
{

}

} // nordic
} // vendor
} // pal
} // ble

