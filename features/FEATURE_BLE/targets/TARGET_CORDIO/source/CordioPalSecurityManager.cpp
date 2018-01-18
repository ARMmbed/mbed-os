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

#include "CordioPalSecurityManager.h"
#include "dm_api.h"
#include "smp_api.h"
#include "wsf_os.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

CordioSecurityManager::CordioSecurityManager() : ::ble::pal::SecurityManager()
{

}

CordioSecurityManager::~CordioSecurityManager()
{

}

////////////////////////////////////////////////////////////////////////////
// SM lifecycle management
//

ble_error_t CordioSecurityManager::initialize()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::terminate()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::reset()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Resolving list management
//

uint8_t CordioSecurityManager::read_resolving_list_capacity()
{
    // FIXME: implement
    return 0;
}

ble_error_t CordioSecurityManager::add_device_to_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    address_t peer_identity_address,
    irk_t peer_irk,
    irk_t local_irk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::remove_device_from_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    address_t peer_identity_address
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::clear_resolving_list()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Feature support
//

ble_error_t CordioSecurityManager::set_secure_connections_support(
    bool enabled, bool secure_connections_only
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_secure_connections_support(
    bool &enabled
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t CordioSecurityManager::set_authentication_timeout(
    connection_handle_t, uint16_t timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_authentication_timeout(
    connection_handle_t, uint16_t &timeout_in_10ms
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

ble_error_t CordioSecurityManager::enable_encryption(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::disable_encryption(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_encryption_status(
    connection_handle_t connection, LinkSecurityStatus_t &status
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_encryption_key_size(
    connection_handle_t, uint8_t &bitsize
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::refresh_encryption_key(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Privacy
//

ble_error_t CordioSecurityManager::set_private_address_timeout(
    uint16_t timeout_in_seconds
) {
    DmPrivSetResolvablePrivateAddrTimeout(timeout_in_seconds);
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

ble_error_t CordioSecurityManager::set_ltk(
    connection_handle_t connection, ltk_t ltk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::set_irk(const irk_t& irk)
{
    DmSecSetLocalIrk(const_cast<uint8_t*>(irk));
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::set_csrk(const csrk_t& csrk)
{
    DmSecSetLocalCsrk(const_cast<uint8_t*>(csrk));
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::generate_irk()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::generate_csrk()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

ble_error_t CordioSecurityManager::send_pairing_request(
    connection_handle_t connection,
    io_capability_t io_capability,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    uint8_t maximum_encryption_key_size,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {
    if ((maximum_encryption_key_size > 16) ||
        (maximum_encryption_key_size < pSmpCfg->minKeyLen)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    pSmpCfg->maxKeyLen = maximum_encryption_key_size;
    pSmpCfg->ioCap = io_capability.value();

    DmSecPairReq(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::send_pairing_response(
    connection_handle_t connection,
    io_capability_t io_capability,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    uint8_t maximum_encryption_key_size,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {

    if ((maximum_encryption_key_size > 16) ||
        (maximum_encryption_key_size < pSmpCfg->minKeyLen)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    pSmpCfg->maxKeyLen = maximum_encryption_key_size;
    pSmpCfg->ioCap = io_capability.value();

    DmSecPairRsp(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::cancel_pairing(
    connection_handle_t connection, pairing_failure_t reason
) {
    DmSecCancelReq(connection, reason.value());
    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::request_authentication(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::get_random_data(random_data_t &random_data)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

ble_error_t CordioSecurityManager::passkey_request_reply(
    connection_handle_t connection, const passkey_num_t passkey
) {
    DmSecAuthRsp(
        connection,
        3,
        reinterpret_cast<uint8_t*>(const_cast<passkey_num_t*>(&passkey))
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::oob_data_request_reply(
    connection_handle_t connection, const oob_data_t& oob_data
) {
    DmSecAuthRsp(
        connection,
        16,
        const_cast<uint8_t*>(oob_data)
    );

    return BLE_ERROR_NONE;
}

ble_error_t CordioSecurityManager::confirmation_entered(
    connection_handle_t connection, bool confirmation
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t CordioSecurityManager::send_keypress_notification(
    connection_handle_t connection, Keypress_t keypress
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

CordioSecurityManager& CordioSecurityManager::get_security_manager()
{
    static CordioSecurityManager _security_manager;
    return _security_manager;
}

bool CordioSecurityManager::sm_handler(const wsfMsgHdr_t* msg) {
    SecurityManagerEventHandler* handler =
        get_security_manager().get_event_handler();

    if ((msg == NULL) || (handler == NULL)) {
        return false;
    }

    switch (msg->event) {
        case DM_SEC_PAIR_CMPL_IND:
            // TODO
            return true;

        case DM_SEC_PAIR_FAIL_IND: {
            uint8_t status = msg->status;
            if (status >= pairing_failure_t::PASSKEY_ENTRY_FAILED &&
                status <= pairing_failure_t::CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED) {
                handler->on_pairing_error(
                    msg->param,
                    (pairing_failure_t::type) msg->status
                );
            } else if (status == SMP_ERR_MEMORY) {
                // FIXME: report to user
            } else if (msg->status == SMP_ERR_TIMEOUT) {
                // FIXME: forward to timeout handler
            } else {
                // TODO: log error
            }
            return true;
        }

        case DM_SEC_ENCRYPT_IND:
            // TODO: Indicate link encryption
            return true;

        case DM_SEC_ENCRYPT_FAIL_IND:
            // TODO: indicate link encryption failure
            return true;

        case DM_SEC_AUTH_REQ_IND: {
            dmSecAuthReqIndEvt_t* evt = (dmSecAuthReqIndEvt_t*) msg;
            connection_handle_t connection = evt->hdr.param;

            if (evt->oob) {
                handler->on_oob_data_request(connection);
            } else if (evt->display) {
                // FIXME: generate the passkey to display or query the default one
                passkey_num_t passkey;
                handler->on_passkey_display(connection, passkey);
                DmSecAuthRsp(connection, 3, reinterpret_cast<uint8_t*>(&passkey));
            } else {
                handler->on_passkey_request(connection);
            }
            return true;
        }

        case DM_SEC_KEY_IND: {
            dmSecKeyIndEvt_t* evt = (dmSecKeyIndEvt_t*) msg;
            connection_handle_t connection = evt->hdr.param;

            switch(evt->type) {
                case DM_KEY_LOCAL_LTK:
                    // TODO: usefull ???
                    break;

                case DM_KEY_PEER_LTK:
                    handler->on_keys_distributed_ltk(connection, evt->keyData.ltk.key);
                    handler->on_keys_distributed_ediv_rand(
                        connection, evt->keyData.ltk.ediv, evt->keyData.ltk.rand
                    );
                    break;

                case DM_KEY_IRK:
                    handler->on_keys_distributed_bdaddr(
                        connection,
                        evt->keyData.irk.addrType,
                        evt->keyData.irk.bdAddr
                    );
                    handler->on_keys_distributed_irk(connection, evt->keyData.irk.key);
                    break;

                case DM_KEY_CSRK:
                    handler->on_keys_distributed_csrk(connection, evt->keyData.csrk);
                    break;
            }

            // TODO: what to do with the security level and encryption key len ???

            return true;
        }

        case DM_SEC_LTK_REQ_IND: {
            hciLeLtkReqEvt_t* evt = (hciLeLtkReqEvt_t*) msg;
            handler->on_ltk_request(
                evt->hdr.param,
                evt->encDiversifier,
                evt->randNum
            );
            return true;
        }

        case DM_SEC_PAIR_IND: {
            dmSecPairIndEvt_t* evt = (dmSecPairIndEvt_t*) msg;
            handler->on_pairing_request(
                evt->hdr.param,
                0, // io capability missing,
                evt->oob,
                evt->auth,
                0, // FIXME: maximum encryption key size missing
                evt->iKeyDist,
                evt->rKeyDist
            )        ;
            return true;
        }

        case DM_SEC_SLAVE_REQ_IND:
            return true;

        case DM_SEC_CALC_OOB_IND:
            return true;

        case DM_SEC_ECC_KEY_IND:
            return true;

        case DM_SEC_COMPARE_IND:
            return true;

        case DM_SEC_KEYPRESS_IND:
            return true;
    }

}


} // cordio
} // vendor
} // pal
} // ble

