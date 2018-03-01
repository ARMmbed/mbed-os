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
#include "nrf_soc.h"

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


nRF5xSecurityManager::nRF5xSecurityManager()
    : ::ble::pal::SecurityManager(),
    _use_legacy_pairing(true),
    _use_secure_connections(true),
    _use_default_passkey(false),
    _default_passkey(0),
    _io_capability(io_capability_t::NO_INPUT_NO_OUTPUT),
    _min_encryption_key_size(7),
    _max_encryption_key_size(16)
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
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::terminate()
{
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::reset()
{
    return BLE_ERROR_NONE;
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
    const address_t &peer_identity_address,
    const irk_t &peer_irk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::remove_device_from_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    const address_t &peer_identity_address
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
    _use_secure_connections = enabled;
    _use_legacy_pairing = !enabled || (enabled && !secure_connections_only);
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::get_secure_connections_support(
    bool &enabled
) {
    // NRF5x platforms support secure connections
    _use_secure_connections = true;
    return BLE_ERROR_NONE;
}


////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t nRF5xSecurityManager::set_authentication_timeout(
    connection_handle_t connection, uint16_t timeout_in_10ms
) {
    // NOTE: Nothing in the Nordic API to manipulate the authentication timeout
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_authentication_timeout(
    connection_handle_t connection, uint16_t &timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::slave_security_request(
    connection_handle_t connection,
    AuthenticationMask authentication
) {
    // In the peripheral role, only the bond, mitm, lesc and keypress fields of this structure are used.

    ble_gap_sec_params_t security_params = {
        /* bond */ authentication.get_bondable(),
        /* mitm */ authentication.get_mitm(),
        /* lesc */ authentication.get_secure_connections(),
        /* keypress */ authentication.get_keypress_notification(),
        0
    };

    uint32_t err = sd_ble_gap_authenticate(
        connection,
        &security_params
    );

    return convert_sd_error(err);
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

ble_error_t nRF5xSecurityManager::enable_encryption(
    connection_handle_t connection,
    const ltk_t &ltk,
    const rand_t &rand,
    const ediv_t &ediv,
    bool mitm
) {
    ble_gap_master_id_t master_id;
    ble_gap_enc_info_t enc_info;

    memcpy(master_id.rand, rand.data(), rand.size());
    memcpy(&master_id.ediv, ediv.data(), ediv.size()); 

    memcpy(enc_info.ltk, ltk.data(), ltk.size());
    enc_info.lesc = _use_secure_connections;
    enc_info.auth = mitm;
    enc_info.ltk_len = ltk.size();

    uint32_t err = sd_ble_gap_encrypt(
        connection,
        &master_id,
        &enc_info
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::encrypt_data(
    const byte_array_t<16> &key,
    encryption_block_t &data
) {
    // NO FUNCTION to disable encryption
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
    connection_handle_t connection,
    const ltk_t& ltk,
    bool mitm,
    bool secure_connections
) {
    ble_gap_enc_info_t enc_info;

    memcpy(enc_info.ltk, ltk.data(), ltk.size());
    enc_info.lesc = secure_connections;
    enc_info.auth = mitm;
    enc_info.ltk_len = ltk.size();

    uint32_t err = sd_ble_gap_sec_info_reply(
        connection,
        &enc_info,
        NULL,
        NULL // Not supported
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::set_ltk_not_found(
    connection_handle_t connection
) {
    uint32_t err = sd_ble_gap_sec_info_reply(
        connection,
        NULL,
        NULL,
        NULL // Not supported
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::set_irk(const irk_t& irk)
{
    // TODO
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::set_csrk(const csrk_t& csrk)
{
    // TODO
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::generate_public_key()
{
    // FIXME
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Global parameters
//

ble_error_t nRF5xSecurityManager::set_display_passkey(passkey_num_t passkey)
{
    if (passkey) {
        _use_default_passkey = true;
        _default_passkey = passkey;
    } else {
        _use_default_passkey = false;
    }
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::set_io_capability(io_capability_t io_capability)
{
    _io_capability = io_capability;
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xSecurityManager::set_encryption_key_requirements(
    uint8_t min_encryption_key_size,
    uint8_t max_encryption_key_size
) {
    if ((min_encryption_key_size < 7) || (min_encryption_key_size > 16) ||
        (min_encryption_key_size > max_encryption_key_size)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _min_encryption_key_size = min_encryption_key_size;
    _max_encryption_key_size = max_encryption_key_size;

    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

ble_error_t nRF5xSecurityManager::send_pairing_request(
    connection_handle_t connection,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
)  {
    ble_gap_sec_params_t security_params = {
        /* bond */ authentication_requirements.get_bondable(),
        /* mitm */ authentication_requirements.get_mitm(),
        /* lesc */ authentication_requirements.get_secure_connections(),
        /* keypress */ authentication_requirements.get_keypress_notification(),
        /* io_caps */ _io_capability.value(),
        /* oob */ oob_data_flag,
        /* min_key_size */ _min_encryption_key_size,
        /* max_key_size */ _max_encryption_key_size,
        /* kdist_periph */ {
            /* enc */ responder_dist.get_encryption(),
            /* id */ responder_dist.get_identity(),
            /* sign */ responder_dist.get_signing(),
            /* link */ responder_dist.get_link()
        },
        /* kdist_central */ {
            /* enc */ initiator_dist.get_encryption(),
            /* id */ initiator_dist.get_identity(),
            /* sign */ initiator_dist.get_signing(),
            /* link */ initiator_dist.get_link()
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
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {
    ble_gap_sec_params_t security_params = {
        /* bond */ authentication_requirements.get_bondable(),
        /* mitm */ authentication_requirements.get_mitm(),
        /* lesc */ authentication_requirements.get_secure_connections(),
        /* keypress */ authentication_requirements.get_keypress_notification(),
        /* io_caps */ _io_capability.value(),
        /* oob */ oob_data_flag,
        /* min_key_size */ _min_encryption_key_size,
        /* max_key_size */ _max_encryption_key_size,
        /* kdist_periph */ {
            /* enc */ responder_dist.get_encryption(),
            /* id */ responder_dist.get_identity(),
            /* sign */ responder_dist.get_signing(),
            /* link */ responder_dist.get_link()
        },
        /* kdist_central */ {
            /* enc */ initiator_dist.get_encryption(),
            /* id */ initiator_dist.get_identity(),
            /* sign */ initiator_dist.get_signing(),
            /* link */ initiator_dist.get_link()
        }
    };

    ble_gap_sec_keyset_t keyset = {
        /* keys_own */ {
            /* encryption key */ &_sp_own_enc_key,
            /* id key */ &_sp_own_id_key,
            /* signing key */ &_sp_own_sign_key,
            /* P-256 Public Key */ &_sp_own_pk
        },
        /* keys_peer */ {
            /* encryption key */ &_sp_peer_enc_key,
            /* id key */ &_sp_peer_id_key,
            /* signing key */ &_sp_peer_sign_key,
            /* P-256 Public Key */ &_sp_peer_pk
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

    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xSecurityManager::get_random_data(byte_array_t<8> &random_data)
{
    uint32_t err = sd_rand_application_vector_get(random_data.buffer(), random_data.size());
    return convert_sd_error(err);
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

ble_error_t nRF5xSecurityManager::passkey_request_reply(
    connection_handle_t connection, const passkey_num_t passkey
) {
    PasskeyAscii pkasc(passkey);
    uint32_t err = sd_ble_gap_auth_key_reply(
        connection,
        BLE_GAP_AUTH_KEY_TYPE_PASSKEY,
        pkasc.value()
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::legacy_pairing_oob_data_request_reply(
    connection_handle_t connection, 
    const oob_tk_t& oob_data
) {
    uint32_t err = sd_ble_gap_auth_key_reply(
        connection,
        BLE_GAP_AUTH_KEY_TYPE_OOB,
        oob_data.data()
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::confirmation_entered(
    connection_handle_t connection, bool confirmation
) {
    uint32_t err;
    
    if(confirmation) {
        // Accept
        err = sd_ble_gap_auth_key_reply(
            connection,
            BLE_GAP_AUTH_KEY_TYPE_PASSKEY,
            NULL
        );
    }
    else {
        // Reject
        err = sd_ble_gap_auth_key_reply(
            connection,
            BLE_GAP_AUTH_KEY_TYPE_NONE,
            NULL
        );
    }

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::send_keypress_notification(
    connection_handle_t connection, Keypress_t keypress
) {
    uint8_t kp_not = static_cast<uint8_t>(keypress);

    uint32_t err = sd_ble_gap_keypress_notify(
        connection,
        kp_not
    );

    return convert_sd_error(err);
}

ble_error_t nRF5xSecurityManager::oob_data_verified(
    connection_handle_t connection,
    const oob_lesc_value_t &local_random,
    const oob_lesc_value_t &peer_random
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
    SecurityManager::EventHandler* handler =
        get_security_manager().get_event_handler();
    nRF5xSecurityManager* inst = &nRF5xSecurityManager::get_security_manager();

    if ((evt == NULL) || (handler == NULL)) {
        return false;
    }

    const ble_gap_evt_t& gap_evt = evt->evt.gap_evt;
    uint16_t connection = gap_evt.conn_handle;

    switch (evt->header.evt_id) {
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST: {
            const ble_gap_sec_params_t& params =
                gap_evt.params.sec_params_request.peer_params;

            AuthenticationMask authentication_requirements(
                params.bond,
                params.mitm,
                params.lesc,
                params.keypress
            );

            KeyDistribution initiator_dist(
                params.kdist_peer.enc,
                params.kdist_peer.id,
                params.kdist_peer.sign,
                params.kdist_peer.link
            );

            KeyDistribution responder_dist(
                params.kdist_own.enc,
                params.kdist_own.id,
                params.kdist_own.sign,
                params.kdist_own.link
            );

            // FIXME: pass min key size
            handler->on_pairing_request(
                connection,
                params.oob,
                authentication_requirements,
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
                ediv_t((uint8_t*)(&req.master_id.ediv)),
                rand_t(req.master_id.rand)
            );

            return true;
        }

        case BLE_GAP_EVT_PASSKEY_DISPLAY: {
            const ble_gap_evt_passkey_display_t& req =
                gap_evt.params.passkey_display;

            if (req.match_request == 0) {
                handler->on_passkey_display(
                    connection,
                    PasskeyAscii::to_num(req.passkey)
                );
            } else {
                // handle this case for secure pairing
            }

            return true;
        }

        case BLE_GAP_EVT_KEY_PRESSED: {
            const ble_gap_evt_key_pressed_t& notf = gap_evt.params.key_pressed;
            handler->on_keypress_notification(
                connection,
                (Keypress_t)notf.kp_not
            );
            return true;
        }
        case BLE_GAP_EVT_AUTH_KEY_REQUEST: {
            uint8_t key_type = gap_evt.params.auth_key_request.key_type;

            switch (key_type) {
                case BLE_GAP_AUTH_KEY_TYPE_NONE: // Illegal
                    break;

                case BLE_GAP_AUTH_KEY_TYPE_PASSKEY:
                    handler->on_passkey_request(connection);
                    break;

                case BLE_GAP_AUTH_KEY_TYPE_OOB:
                    handler->on_legacy_pairing_oob_request(connection);
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
                    // Keys stored (_sp*) are valid
                    handler->on_keys_distributed_local_ltk(
                        connection,
                        ltk_t(inst->_sp_own_enc_key.enc_info.ltk)
                    );

                    handler->on_keys_distributed_local_ediv_rand(
                        connection,
                        ediv_t(reinterpret_cast<uint8_t*>(&inst->_sp_own_enc_key.master_id.ediv)),
                        inst->_sp_own_enc_key.master_id.rand
                    );

                    handler->on_keys_distributed_ltk(
                        connection,
                        ltk_t(inst->_sp_peer_enc_key.enc_info.ltk)
                    );

                    handler->on_keys_distributed_ediv_rand(
                        connection,
                        ediv_t(reinterpret_cast<uint8_t*>(&inst->_sp_peer_enc_key.master_id.ediv)),
                        inst->_sp_peer_enc_key.master_id.rand
                    );
                    break;

                    // TODO add signing & privacy keys here

                case BLE_GAP_SEC_STATUS_TIMEOUT:
                    handler->on_pairing_timed_out(connection);
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
        {
        const ble_gap_evt_conn_sec_update_t& req =
                gap_evt.params.conn_sec_update;
            if( (req.conn_sec.sec_mode.sm == 1) && (req.conn_sec.sec_mode.lv >= 2) )
            {
                handler->on_link_encryption_result(connection, link_encryption_t::ENCRYPTED);
            }
            else
            {
                handler->on_link_encryption_result(connection, link_encryption_t::NOT_ENCRYPTED);
            }
            return true;
        }
        case BLE_GAP_EVT_TIMEOUT:
            // FIXME: forward event when available
            return true;

        case BLE_GAP_EVT_SEC_REQUEST:
        {
            const ble_gap_evt_sec_request_t& req =
                gap_evt.params.sec_request;
            AuthenticationMask auth_mask(req.bond, req.mitm, req.lesc, req.keypress);
            handler->on_slave_security_request(
                            connection,
                            auth_mask
                        );
            return true;
        }
        default:
            return false;
    }
}

} // nordic
} // vendor
} // pal
} // ble

