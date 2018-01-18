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
#include "nrf_ble.h"
#include "nrf_ble_gap.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

namespace {
static ble_error_t convert_sd_error(uint32_t err) {
    // TODO: implementation
    return err ? BLE_ERROR_UNSPECIFIED : BLE_ERROR_NONE;
}
}


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

// FIXME: on nordic, the irk is passed in sd_ble_gap_scan_start where whitelist
// and resolving list are all mixed up.

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
    bool &enabled
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t nRF5xSecurityManager::set_authentication_timeout(
    connection_handle_t, uint16_t timeout_in_10ms
) {
    // NOTE: Nothing in the Nordic API to manipulate the authentication timeout
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
    // use sd_ble_gap_encrypt it requires:
    //  - ediv
    //  - rand
    //  - ltk
    //  - flag indicating if the key was generated with lesc
    //  - flag indicated if the key is an authenticated key
    //  - flag indicating the length of the ltk ???
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::disable_encryption(connection_handle_t connection)
{
    // NO FUNCTION to disable encryption
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
    // usefulness ?
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::set_irk(const irk_t& irk)
{
    memcpy(_irk, irk, sizeof(_irk));
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::set_csrk(const csrk_t& csrk)
{
    memcpy(_csrk, csrk, sizeof(_csrk));
    return BLE_ERROR_NONE;
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
    ble_gap_sec_params_t security_params = {
        /* bond */ static_cast<uint8_t>((authentication_requirements >> 0) & 3),
        /* mitm */ static_cast<uint8_t>((authentication_requirements >> 2) & 1),
        /* lesc */ static_cast<uint8_t>((authentication_requirements >> 3) & 1),
        /* keypress */ static_cast<uint8_t>((authentication_requirements >> 4) & 1),
        /* io_caps */ io_capability.value(),
        /* oob */ oob_data_flag,
        /* min_key_size */ 7, // FIXME!
        /* max_key_size */ maximum_encryption_key_size,
        /* kdist_periph */ {
            /* enc */ static_cast<uint8_t>((responder_dist >> 0) & 1),
            /* id */ static_cast<uint8_t>((responder_dist >> 1) & 1),
            /* sign */ static_cast<uint8_t>((responder_dist >> 2) & 1),
            /* link */ static_cast<uint8_t>((responder_dist >> 3) & 1)
        },
        /* kdist_central */ {
            /* enc */ static_cast<uint8_t>((initiator_dist >> 0) & 1),
            /* id */ static_cast<uint8_t>((initiator_dist >> 1) & 1),
            /* sign */ static_cast<uint8_t>((initiator_dist >> 2) & 1),
            /* link */ static_cast<uint8_t>((initiator_dist >> 3) & 1)
        }
    };

    uint32_t err = sd_ble_gap_authenticate(
        connection,
        &security_params
    );

    return convert_sd_error(err);
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
    ble_gap_sec_params_t security_params = {
        /* bond */ static_cast<uint8_t>((authentication_requirements >> 0) & 3),
        /* mitm */static_cast<uint8_t>((authentication_requirements >> 2) & 1),
        /* lesc */ static_cast<uint8_t>((authentication_requirements >> 3) & 1),
        /* keypress */ static_cast<uint8_t>((authentication_requirements >> 4) & 1),
        /* io_caps */ io_capability.value(),
        /* oob */ oob_data_flag,
        /* min_key_size */ 7, // FIXME!
        /* max_key_size */ maximum_encryption_key_size,
        /* kdist_periph */ {
            /* enc */ static_cast<uint8_t>((responder_dist >> 0) & 1),
            /* id */ static_cast<uint8_t>((responder_dist >> 1) & 1),
            /* sign */ static_cast<uint8_t>((responder_dist >> 2) & 1),
            /* link */ static_cast<uint8_t>((responder_dist >> 3) & 1)
        },
        /* kdist_central */ {
            /* enc */ static_cast<uint8_t>((initiator_dist >> 0) & 1),
            /* id */ static_cast<uint8_t>((initiator_dist >> 1) & 1),
            /* sign */ static_cast<uint8_t>((initiator_dist >> 2) & 1),
            /* link */ static_cast<uint8_t>((initiator_dist >> 3) & 1)
        }
    };

    ble_gap_sec_keyset_t keyset = {
        /* keys_own */ {
            /* encryption key */ NULL,
            /* id key */ NULL, /* FIXME: pass in the IRK mixed up with address ?!?*/
            /* signing key */ NULL, /* FIXME: put _csrk in the type of data structure used by nordic */
            /* P-256 Public Key */ NULL
        },
        /* keys_peer */ {
            // FIXME: The softdevice requires the application to maintain this memory chunk ...
        }
    };

    uint32_t err = sd_ble_gap_sec_params_reply(
        connection,
        /* status */ BLE_GAP_SEC_STATUS_SUCCESS,
        /* params */ &security_params,
        /* keys ... */ &keyset
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::cancel_pairing(
    connection_handle_t connection, pairing_failure_t reason
) {
    // FIXME: there is no fixed function that can be used to cancel pairing all
    // the time. However sd_ble_gap_sec_params_reply should be used to cancel a
    // pairing after a pairing request.

    // sd_ble_gap_auth_key_reply should be used to cancel pairing when a key
    // entered by the user is required.

    uint32_t err = sd_ble_gap_sec_params_reply(
        connection,
        /* status */ reason.value() | 0x80,
        /* params */ NULL,
        /* keys ... */ NULL
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::request_authentication(connection_handle_t connection)
{
    uint8_t authentication_requirements;
    // FIXME: need authentication requirements from the caller
    ble_gap_sec_params_t security_params = {
        /* bond */ static_cast<uint8_t>((authentication_requirements >> 0) & 3),
        /* mitm */static_cast<uint8_t>((authentication_requirements >> 2) & 1),
        /* lesc */ static_cast<uint8_t>((authentication_requirements >> 3) & 1),
        /* keypress */ static_cast<uint8_t>((authentication_requirements >> 4) & 1)
        /* other parameters ignored ??? TODO: check*/
    };

    uint32_t err = sd_ble_gap_authenticate(
        connection,
        &security_params
    );

    return convert_sd_error(err);
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
    uint32_t err = sd_ble_gap_auth_key_reply(
        connection,
        BLE_GAP_AUTH_KEY_TYPE_PASSKEY,
        /* FIXME: convert passkey_num into and asci key */ NULL
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::oob_data_request_reply(
    connection_handle_t connection, const oob_data_t& oob_data
) {
    uint32_t err = sd_ble_gap_auth_key_reply(
        connection,
        BLE_GAP_AUTH_KEY_TYPE_OOB,
        oob_data
    );

    return convert_sd_error(err);
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

bool nRF5xSecurityManager::sm_handler(const ble_evt_t *evt)
{
    SecurityManagerEventHandler* handler =
        get_security_manager().get_event_handler();

    if ((evt == NULL) || (handler == NULL)) {
        return false;
    }

    const ble_gap_evt_t& gap_evt = evt->evt.gap_evt;
    uint16_t connection = gap_evt.conn_handle;

    switch (evt->header.evt_id) {
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST: {
            const ble_gap_sec_params_t& params =
                gap_evt.params.sec_params_request.peer_params;

            authentication_t authentication_requirements =
                params.bond |
                params.mitm << 2 |
                params.lesc << 3 |
                params.keypress << 4;

            key_distribution_t initiator_dist =
                params.kdist_peer.enc |
                params.kdist_peer.id << 1 |
                params.kdist_peer.sign << 2 |
                params.kdist_peer.link << 3;

            key_distribution_t responder_dist =
                params.kdist_own.enc |
                params.kdist_own.id << 1 |
                params.kdist_own.sign << 2 |
                params.kdist_own.link << 3;

            // FIXME: pass min key size
            handler->on_pairing_request(
                connection,
                (io_capability_t::type) params.io_caps,
                params.oob,
                authentication_requirements,
                params.max_key_size,
                initiator_dist,
                responder_dist
            );
            return true;
        }

        case BLE_GAP_EVT_SEC_INFO_REQUEST: {
            const ble_gap_evt_sec_info_request_t& req =
                gap_evt.params.sec_info_request;

            handler->on_ltk_request(
                connection,
                req.master_id.ediv,
                req.master_id.rand
            );

            return true;
        }

        case BLE_GAP_EVT_PASSKEY_DISPLAY: {
            const ble_gap_evt_passkey_display_t& req =
                gap_evt.params.passkey_display;

            if (req.match_request == 0) {
                handler->on_passkey_display(
                    connection,
                    0 /* TODO: conversion of req.passkey into a numerical passkey */
                );
            } else {
                // handle this case for secure pairing
            }

            return true;
        }

        case BLE_GAP_EVT_KEY_PRESSED:
            // TODO: add with LESC support
            return true;

        case BLE_GAP_EVT_AUTH_KEY_REQUEST: {
            uint8_t key_type = gap_evt.params.auth_key_request.key_type;

            switch (key_type) {
                case BLE_GAP_AUTH_KEY_TYPE_NONE:
                    break;

                case BLE_GAP_AUTH_KEY_TYPE_PASSKEY:
                    handler->on_passkey_request(connection);
                    break;

                case BLE_GAP_AUTH_KEY_TYPE_OOB:
                    handler->on_oob_data_request(connection);
                    break;
            }

            return true;
        }

        case BLE_GAP_EVT_LESC_DHKEY_REQUEST:
            // TODO: Add with LESC support
            return true;

        case BLE_GAP_EVT_AUTH_STATUS: {
            const ble_gap_evt_auth_status_t& status =
                gap_evt.params.auth_status;

            switch (status.auth_status) {
                case BLE_GAP_SEC_STATUS_SUCCESS:
                    // FIXME: needs success handler
                    break;
                case BLE_GAP_SEC_STATUS_TIMEOUT:
                    // FIXME: needs timeout handler
                    break;

                case BLE_GAP_SEC_STATUS_PASSKEY_ENTRY_FAILED:
                case BLE_GAP_SEC_STATUS_OOB_NOT_AVAILABLE:
                case BLE_GAP_SEC_STATUS_AUTH_REQ:
                case BLE_GAP_SEC_STATUS_CONFIRM_VALUE:
                case BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP:
                case BLE_GAP_SEC_STATUS_ENC_KEY_SIZE:
                case BLE_GAP_SEC_STATUS_SMP_CMD_UNSUPPORTED:
                case BLE_GAP_SEC_STATUS_UNSPECIFIED:
                case BLE_GAP_SEC_STATUS_REPEATED_ATTEMPTS:
                case BLE_GAP_SEC_STATUS_INVALID_PARAMS:
                case BLE_GAP_SEC_STATUS_DHKEY_FAILURE:
                case BLE_GAP_SEC_STATUS_NUM_COMP_FAILURE:
                case BLE_GAP_SEC_STATUS_BR_EDR_IN_PROG:
                case BLE_GAP_SEC_STATUS_X_TRANS_KEY_DISALLOWED:
                    handler->on_pairing_error(
                        connection,
                        (pairing_failure_t::type) (status.auth_status & 0xF)
                    );
                    break;

                default:
                    break;
            }


            return true;
        }

        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            // FIXME: Invoke handler indicating the link encryption
            return true;

        case BLE_GAP_EVT_TIMEOUT:
            // FIXME: forward event when available
            return true;

        case BLE_GAP_EVT_SEC_REQUEST:
            // FIXME: forward event when available
            return true;

        default:
            return false;
    }
}

} // nordic
} // vendor
} // pal
} // ble

