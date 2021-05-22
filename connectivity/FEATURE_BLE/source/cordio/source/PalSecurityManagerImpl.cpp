/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#if BLE_FEATURE_SECURITY

#include <cstring>

#include "ble/common/BLERoles.h"
#include "ble/common/blecommon.h"
#include "source/PalSecurityManagerImpl.h"
#include "source/PalAttClientImpl.h"
#include "dm_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "wsf_os.h"
#include "hci_core.h"

#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLSM"

namespace ble {
namespace impl {

PalSecurityManager::PalSecurityManager() :
    _pal_event_handler(nullptr),
    _use_default_passkey(false),
    _default_passkey(0)
{
}

PalSecurityManager::~PalSecurityManager()
{
}

////////////////////////////////////////////////////////////////////////////
// SM lifecycle management
//


ble_error_t PalSecurityManager::initialize()
{
    tr_info("Initialize PAL SM");
    // reset local state
    _use_default_passkey = false;
    _default_passkey = 0;
#if BLE_FEATURE_SECURE_CONNECTIONS
    _lesc_keys_generated = false;
#endif // BLE_FEATURE_SECURE_CONNECTIONS
#if BLE_FEATURE_SIGNING
    memset(_peer_csrks, 0, sizeof(_peer_csrks));
#endif

#if 0
    // FIXME: need help from the stack or local calculation
    // generate a new set of keys
    DmSecGenerateEccKeyReq();
#endif

    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::terminate()
{
    tr_info("Terminate PAL SM");
#if BLE_FEATURE_SIGNING
    cleanup_peer_csrks();
#endif // BLE_FEATURE_SIGNING
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::reset()
{
    tr_info("Reset PAL SM");
#if BLE_FEATURE_SIGNING
    cleanup_peer_csrks();
#endif // BLE_FEATURE_SIGNING
    initialize();
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Feature support
//

// FIXME: Enable when new function available in the pal.
#if 0

ble_error_t PalSecurityManager::set_secure_connections_support(
    bool enabled, bool secure_connections_only
)
{
    // secure connection support is enabled automatically at the stack level.
    if (secure_connections_only) {
        pSmpCfg->auth |= SMP_AUTH_SC_FLAG;
    } else {
        pSmpCfg->auth &= ~SMP_AUTH_SC_FLAG;
    }
    return BLE_ERROR_NONE;
}
#endif


ble_error_t PalSecurityManager::get_secure_connections_support(
    bool &enabled
)
{
#if BLE_FEATURE_SECURE_CONNECTIONS
    // FIXME: should depend of the controller
    enabled = false;
    return BLE_ERROR_NONE;
#else
    enabled = false;
    return BLE_ERROR_NONE;
#endif
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//


ble_error_t PalSecurityManager::set_authentication_timeout(
    connection_handle_t connection, uint16_t timeout_in_10ms
)
{
    tr_info("Connection %d - pass authentication timeout %d to the stack", connection, timeout_in_10ms);
    DmWriteAuthPayloadTimeout(connection, timeout_in_10ms);
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::get_authentication_timeout(
    connection_handle_t connection, uint16_t &timeout_in_10ms
)
{
    // FIXME: Is it usefull to add dynamic timeout management for all connections ?
    return BLE_ERROR_NOT_IMPLEMENTED;
}

#if BLE_ROLE_PERIPHERAL
ble_error_t PalSecurityManager::slave_security_request(
    connection_handle_t connection,
    AuthenticationMask authentication
)
{
    tr_info("Connection %d - Pass slave security to the stack: authentication = %02x", connection, authentication.value());
    DmSecSlaveReq(connection, authentication.value());
    return BLE_ERROR_NONE;
}
#endif // BLE_ROLE_PERIPHERAL

////////////////////////////////////////////////////////////////////////////
// Encryption
//

#if BLE_ROLE_CENTRAL
ble_error_t PalSecurityManager::enable_encryption(
    connection_handle_t connection,
    const ltk_t &ltk,
    const rand_t &rand,
    const ediv_t &ediv,
    bool mitm
)
{
    tr_info("Connection %d - Enable encryption: lth = %s, rand = %s, ediv = %s, mitm = %s",
        connection, to_string(ltk), to_string(rand), to_string(ediv), to_string(mitm));
    dmSecLtk_t sec_ltk;
    memcpy(sec_ltk.key, ltk.data(), ltk.size());
    memcpy(sec_ltk.rand, rand.data(), rand.size());
    memcpy(&sec_ltk.ediv, ediv.data(), ediv.size());

    DmSecEncryptReq(
        connection,
        mitm ? DM_SEC_LEVEL_ENC_AUTH : DM_SEC_LEVEL_ENC,
        &sec_ltk
    );

    return BLE_ERROR_NONE;
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t PalSecurityManager::enable_encryption(
    connection_handle_t connection,
    const ltk_t &ltk,
    bool mitm
)
{
    tr_info("Connection %d - Enable SC encryption: ltk = %s, mitm = %s",
        connection, to_string(ltk), to_string(mitm));
    dmSecLtk_t sec_ltk = {0};
    memcpy(sec_ltk.key, ltk.data(), ltk.size());

    DmSecEncryptReq(
        connection,
        DM_SEC_LEVEL_ENC_LESC,
        &sec_ltk
    );


    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_SECURE_CONNECTIONS
#endif // BLE_ROLE_CENTRAL


ble_error_t PalSecurityManager::encrypt_data(
    const byte_array_t<16> &key,
    encryption_block_t &data
)
{
    tr_warning("PAL Encrypt data not implemented");
    return BLE_ERROR_NOT_IMPLEMENTED;
}


////////////////////////////////////////////////////////////////////////////
// Keys
//


ble_error_t PalSecurityManager::set_ltk(
    connection_handle_t connection,
    const ltk_t &ltk,
    bool mitm,
    bool secure_connections
)
{
    tr_info("Connection %d - send ltk response: ltk = %s, mitm = %s, sc = %s ",
        connection, to_string(ltk), to_string(mitm), to_string(secure_connections));
    uint8_t security_level = DM_SEC_LEVEL_NONE;
    if (secure_connections) {
        security_level = DM_SEC_LEVEL_ENC_LESC;
    } else if (mitm) {
        security_level = DM_SEC_LEVEL_ENC_AUTH;
    } else {
        security_level = DM_SEC_LEVEL_ENC;
    }

    DmSecLtkRsp(
        connection,
        /* key found */ true,
        /* sec level */ security_level,
        const_cast<uint8_t *>(ltk.data())
    );
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::set_ltk_not_found(
    connection_handle_t connection
)
{
    tr_info("Connection %d - Reply LTK not found", connection);
    DmSecLtkRsp(
        connection,
        /* key found */ false,
        /* sec level */ DM_SEC_LEVEL_NONE,
        nullptr
    );

    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::set_irk(const irk_t &irk)
{
    tr_info("Set stack local IRK: %s", to_string(irk));
    _irk = irk;
    DmSecSetLocalIrk(_irk.data());
    return BLE_ERROR_NONE;
}

ble_error_t PalSecurityManager::set_identity_address(
    const address_t &address,
    bool public_address
)
{
    tr_info("Set stack local identity address: address = %s, public = %s", to_string(address), to_string(public_address));
    DmSecSetLocalIdentityAddr(
        address.data(),
        public_address ? DM_ADDR_PUBLIC : DM_ADDR_RANDOM
    );
    return BLE_ERROR_NONE;
}

#if BLE_FEATURE_SIGNING
ble_error_t PalSecurityManager::set_csrk(
    const csrk_t &csrk,
    sign_count_t sign_counter
)
{
    tr_info("Set stack local CSRK: CSRK = %s, sign counter = %" PRIu32, to_string(csrk), sign_counter);
    _csrk = csrk;
    DmSecSetLocalCsrk(_csrk.data());
#if BLE_FEATURE_GATT_CLIENT
    // extra set the sign counter used by the client
    impl::PalAttClient::get_client().set_sign_counter(sign_counter);
#endif

    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::set_peer_csrk(
    connection_handle_t connection,
    const csrk_t &csrk,
    bool authenticated,
    sign_count_t sign_counter
)
{
    if (connection == 0 || connection > DM_CONN_MAX) {
        tr_error("Connection %d - Imposible to set stack peer CSRK, invalid connection ID", connection);
        return BLE_ERROR_INVALID_PARAM;
    }

    tr_info("Connection %d - Set stack peer CSRK: CSRK = %s, authenticated = %s, sign_counter = %" PRIu32,
        connection, to_string(csrk), to_string(authenticated), sign_counter);

    size_t connection_index = connection - 1;

    if (_peer_csrks[connection_index]) {
        *_peer_csrks[connection_index] = csrk;
    } else {
        _peer_csrks[connection_index] = new(std::nothrow) csrk_t(csrk);
        if (_peer_csrks[connection_index] == nullptr) {
            return BLE_ERROR_NO_MEM;
        }
    }

    AttsSetCsrk(connection, _peer_csrks[connection_index]->data(), authenticated);
    AttsSetSignCounter(connection, sign_counter);
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::remove_peer_csrk(connection_handle_t connection)
{
    if (connection == 0 || connection > DM_CONN_MAX) {
        tr_error("Connection %d - Imposible to set stack peer CSRK, invalid connection ID", connection);
        return BLE_ERROR_INVALID_PARAM;
    }

    tr_info("Connection %d - Remove stack peer CSRK", connection);

    size_t connection_index = connection - 1;

    if (_peer_csrks[connection_index]) {
        delete _peer_csrks[connection_index];
        _peer_csrks[connection_index] = nullptr;
    }

    AttsSetCsrk(connection, nullptr, false);
    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_SIGNING

////////////////////////////////////////////////////////////////////////////
// Global parameters
//


ble_error_t PalSecurityManager::set_display_passkey(passkey_num_t passkey)
{
    PasskeyAscii ascii_passkey(passkey);
    tr_info("Set stack display passkey: %s", passkey_str(ascii_passkey.value()));

    if (passkey) {
        _use_default_passkey = true;
        _default_passkey = passkey;
    } else {
        _use_default_passkey = false;
    }
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::set_io_capability(io_capability_t io_capability)
{
    tr_info("Set stack I/O capabilities: %s", to_string(io_capability));
    pSmpCfg->ioCap = io_capability.value();
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::set_encryption_key_requirements(
    uint8_t min_encryption_key_size,
    uint8_t max_encryption_key_size
)
{
    tr_info("Set stack encryption key requirements: [%d : %d]", min_encryption_key_size, max_encryption_key_size);
    if ((min_encryption_key_size < 7) || (min_encryption_key_size > 16) ||
        (min_encryption_key_size > max_encryption_key_size)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    pSmpCfg->minKeyLen = min_encryption_key_size;
    pSmpCfg->maxKeyLen = max_encryption_key_size;

    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

#if BLE_ROLE_CENTRAL
ble_error_t PalSecurityManager::send_pairing_request(
    connection_handle_t connection,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
)
{
    tr_info("Connection %d - send pairing request: oob = %s, auth = %02X, initiator dist = %02x, responder dist = %02X",
        connection, to_string(oob_data_flag), authentication_requirements.value(), initiator_dist.value(), responder_dist.value());
    DmSecPairReq(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}
#endif // BLE_ROLE_CENTRAL

#if BLE_ROLE_PERIPHERAL
ble_error_t PalSecurityManager::send_pairing_response(
    connection_handle_t connection,
    bool oob_data_flag,
    AuthenticationMask authentication_requirements,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
)
{
    tr_info("Connection %d - Send pairing response: oob = %s, auth = %02X, initiator dist = %02x, responder dist = %02X",
        connection, to_string(oob_data_flag), authentication_requirements.value(), initiator_dist.value(), responder_dist.value());
    DmSecPairRsp(
        connection,
        oob_data_flag,
        authentication_requirements.value(),
        initiator_dist.value(),
        responder_dist.value()
    );

    return BLE_ERROR_NONE;
}
#endif // BLE_ROLE_PERIPHERAL


ble_error_t PalSecurityManager::cancel_pairing(
    connection_handle_t connection, pairing_failure_t reason
)
{
    tr_info("Connection %d - cancel pairing: reason = %s", connection, to_string(reason));
    DmSecCancelReq(connection, reason.value());
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::get_random_data(byte_array_t<8> &random_data)
{
    SecRand(random_data.data(), random_data.size());
    tr_info("Stack generated random data: %s", to_string(random_data));
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// MITM
//


ble_error_t PalSecurityManager::passkey_request_reply(
    connection_handle_t connection, passkey_num_t passkey
)
{
    tr_info("Connection %d - Reply with passkey %s", connection, passkey_str(PasskeyAscii(passkey).value()));
    DmSecAuthRsp(
        connection,
        /* datalength */ 3,
        reinterpret_cast<uint8_t *>(&passkey)
    );

    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::legacy_pairing_oob_request_reply(
    connection_handle_t connection,
    const oob_tk_t &oob_data
)
{
    tr_info("Connection %d - Reply with legacy OOB data %s", connection, to_string(oob_data));
    DmSecAuthRsp(
        connection,
        /* data length */16,
        const_cast<uint8_t *>(oob_data.data())
    );

    return BLE_ERROR_NONE;
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t PalSecurityManager::confirmation_entered(
    connection_handle_t connection, bool confirmation
)
{
    tr_info("Connection %d - Send confirmation entered: %s", connection, to_string(confirmation));
    DmSecCompareRsp(connection, confirmation);
    return BLE_ERROR_NONE;
}

// FIXME: remove when declaration from the stack is available
extern "C" void DmSecKeypressReq(dmConnId_t connId, uint8_t keypressType);


ble_error_t PalSecurityManager::send_keypress_notification(
    connection_handle_t connection, ble::Keypress_t keypress
)
{
    tr_info("Connection %d - Send keypress notification: %s", connection, to_string(keypress));
    DmSecKeypressReq(connection, keypress);
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::generate_secure_connections_oob()
{
    tr_info("Start generation of SC OOB");
    uint8_t oobLocalRandom[SMP_RAND_LEN];
    SecRand(oobLocalRandom, SMP_RAND_LEN);
    DmSecCalcOobReq(oobLocalRandom, _public_key_x);
    return BLE_ERROR_NONE;
}


ble_error_t PalSecurityManager::secure_connections_oob_request_reply(
    connection_handle_t connection,
    const oob_lesc_value_t &local_random,
    const oob_lesc_value_t &peer_random,
    const oob_confirm_t &peer_confirm
)
{
    tr_info("Connection %d - Reply to SC OOB request: local_random = %s, peer_random = %s, peer_confirm = %s",
        connection, to_string(local_random), to_string(peer_random), to_string(peer_confirm));
    dmSecLescOobCfg_t oob_config = {0};

    memcpy(oob_config.localRandom, local_random.data(), local_random.size());
    // FIXME:
    // memcpy(oob_config.localConfirm, ?, ?);
    memcpy(oob_config.peerRandom, peer_random.data(), peer_random.size());
    memcpy(oob_config.peerConfirm, peer_confirm.data(), peer_confirm.size());

    DmSecSetOob(connection, &oob_config);
    DmSecAuthRsp(connection, 0, nullptr);

    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_SECURE_CONNECTIONS


PalSecurityManager &PalSecurityManager::get_security_manager()
{
    static PalSecurityManager _security_manager;
    return _security_manager;
}


bool PalSecurityManager::sm_handler(const wsfMsgHdr_t *msg)
{
    PalSecurityManager &self = get_security_manager();
    PalSecurityManagerEventHandler *handler = self.get_event_handler();

    if (handler == nullptr) {
        return false;
    }

    MBED_ASSERT(msg);

    switch (msg->event) {
        case DM_SEC_PAIR_CMPL_IND: {
            tr_info("Handling event DM_SEC_PAIR_CMPL_IND");
            auto *evt = (dmSecPairCmplIndEvt_t *) msg;
            // Note: authentication and bonding flags present in the auth field
            handler->on_pairing_completed(evt->hdr.param);
            return true;
        }

        case DM_SEC_PAIR_FAIL_IND: {
            tr_info("Handling event DM_SEC_PAIR_FAIL_IND");
            connection_handle_t connection = msg->param;
            uint8_t status = msg->status;

            if (status >= pairing_failure_t::PASSKEY_ENTRY_FAILED &&
                status <= pairing_failure_t::CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED) {
                handler->on_pairing_error(
                    connection,
                    (pairing_failure_t::type) status
                );
            } else if (status == SMP_ERR_MEMORY) {
                // note: report the error as an unspecified error
                handler->on_pairing_error(
                    connection,
                    pairing_failure_t::UNSPECIFIED_REASON
                );
            } else if (msg->status == SMP_ERR_TIMEOUT) {
                handler->on_pairing_timed_out(connection);
            } else {
                // note: report the error as an unspecified error
                handler->on_pairing_error(
                    connection,
                    pairing_failure_t::UNSPECIFIED_REASON
                );
            }
            return true;
        }

        case DM_SEC_ENCRYPT_IND: {
            tr_info("Handling event DM_SEC_ENCRYPT_IND");
            auto *evt = (dmSecEncryptIndEvt_t *) msg;
            // note: the field usingLtk of the message indicates if an LTK was
            // used to encrypt the link
            // FIXME: How to indicate the level of encryption ?
            handler->on_link_encryption_result(evt->hdr.param, link_encryption_t::ENCRYPTED);
            return true;
        }

        case DM_SEC_ENCRYPT_FAIL_IND: {
            tr_info("Handling event DM_SEC_ENCRYPT_FAIL_IND");
            // note: msg->status contains the encryption failure status
            handler->on_link_encryption_result(msg->param, link_encryption_t::NOT_ENCRYPTED);
            return true;
        }

        case DM_SEC_AUTH_REQ_IND: {
            tr_info("Handling event DM_SEC_AUTH_REQ_IND");
            auto *evt = (dmSecAuthReqIndEvt_t *) msg;
            connection_handle_t connection = evt->hdr.param;

            if (evt->oob) {
                // FIXME: Nothing in the API indicates if smp or sc OOB are
                // requested.
                // To set secure connection OOB:
                //   - DmSecSetOob(connection, oob_data)
                //   - DmSecAuthRsp(connection, 0, nullptr)
                handler->on_legacy_pairing_oob_request(connection);
            } else if (evt->display) {
                if (get_security_manager()._use_default_passkey) {
                    handler->on_passkey_display(
                        connection,
                        get_security_manager()._default_passkey
                    );
                    tr_info("Use default passkey");
                    DmSecAuthRsp(
                        connection,
                        /* data length */ SMP_PIN_LEN,
                        reinterpret_cast<uint8_t *>(&(get_security_manager()._default_passkey))
                    );
                } else {
                    /* generate random passkey, limit to 6 digit max */
                    tr_info("Generate random passkey");
                    passkey_num_t passkey;
                    SecRand((uint8_t *) &passkey, sizeof(passkey));
                    passkey %= 1000000;
                    handler->on_passkey_display(connection, passkey);
                    DmSecAuthRsp(
                        connection, SMP_PIN_LEN, reinterpret_cast<uint8_t *>(&passkey)
                    );
                }
            } else {
                handler->on_passkey_request(connection);
            }
            return true;
        }

        case DM_SEC_KEY_IND: {
            tr_info("Handling event DM_SEC_KEY_IND");
            // NOTE: also report security level and encryption key len
            auto *evt = (dmSecKeyIndEvt_t *) msg;
            connection_handle_t connection = evt->hdr.param;

            switch (evt->type) {
                case DM_KEY_LOCAL_LTK:
                    handler->on_keys_distributed_local_ltk(
                        connection,
                        ltk_t(reinterpret_cast<uint8_t *>(evt->keyData.ltk.key))
                    );

                    handler->on_keys_distributed_local_ediv_rand(
                        connection,
                        ediv_t(reinterpret_cast<uint8_t *>(&(evt->keyData.ltk.ediv))),
                        evt->keyData.ltk.rand
                    );
                    break;

                case DM_KEY_PEER_LTK:
                    handler->on_keys_distributed_ltk(
                        connection,
                        ltk_t(reinterpret_cast<uint8_t *>(evt->keyData.ltk.key))
                    );

                    handler->on_keys_distributed_ediv_rand(
                        connection,
                        ediv_t(reinterpret_cast<uint8_t *>(&(evt->keyData.ltk.ediv))),
                        evt->keyData.ltk.rand
                    );
                    break;
#if BLE_FEATURE_PRIVACY
                case DM_KEY_IRK:
                    handler->on_keys_distributed_bdaddr(
                        connection,
                        (advertising_peer_address_type_t::type) evt->keyData.irk.addrType,
                        evt->keyData.irk.bdAddr
                    );

                    handler->on_keys_distributed_irk(
                        connection,
                        irk_t(reinterpret_cast<uint8_t *>(evt->keyData.irk.key))
                    );
                    break;
#endif // BLE_FEATURE_PRIVACY
#if BLE_FEATURE_SIGNING
                case DM_KEY_CSRK:
                    handler->on_keys_distributed_csrk(
                        connection, evt->keyData.csrk.key
                    );
                    break;
#endif // BLE_FEATURE_SIGNING
            }

            return true;
        }

        case DM_SEC_LTK_REQ_IND: {
            tr_info("Handling event DM_SEC_LTK_REQ_IND");
            uint8_t null_rand[HCI_RAND_LEN] = {0};
            auto *evt = (hciLeLtkReqEvt_t *) msg;

            if (evt->encDiversifier == 0 &&
                memcmp(evt->randNum, null_rand, sizeof(null_rand))) {
                handler->on_ltk_request(
                    evt->hdr.param
                );
            } else {
                handler->on_ltk_request(
                    evt->hdr.param,
                    reinterpret_cast<uint8_t *>(&evt->encDiversifier),
                    evt->randNum
                );
            }
            return true;
        }

#if BLE_ROLE_PERIPHERAL
        case DM_SEC_PAIR_IND: {
            tr_info("Handling event DM_SEC_PAIR_IND");
            auto *evt = (dmSecPairIndEvt_t *) msg;
            handler->on_pairing_request(
                /* connection */ evt->hdr.param,
                evt->oob,
                evt->auth,
                evt->iKeyDist,
                evt->rKeyDist
            );
            return true;
        }
#endif // BLE_ROLE_PERIPHERAL

#if BLE_ROLE_CENTRAL
        case DM_SEC_SLAVE_REQ_IND: {
            tr_info("Handling event DM_SEC_SLAVE_REQ_IND");
            auto *evt = (dmSecPairIndEvt_t *) msg;
            handler->on_slave_security_request(
                /* connection */ evt->hdr.param,
                evt->auth
            );
            return true;
        }
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_SECURE_CONNECTIONS
        case DM_SEC_CALC_OOB_IND: {
            tr_info("Handling event DM_SEC_CALC_OOB_IND");
            auto *evt = (dmSecOobCalcIndEvt_t *) msg;
            handler->on_secure_connections_oob_generated(
                evt->random,
                evt->confirm
            );
            return true;
        }

        case DM_SEC_ECC_KEY_IND: {
            tr_info("Handling event DM_SEC_ECC_KEY_IND");
            auto *evt = (secEccMsg_t *) msg;
            tr_info("ECC Keys generated: public key x=%s, public key y=%s, private key=%s",
                tr_array(evt->data.key.pubKey_x, SEC_ECC_KEY_LEN),
                tr_array(evt->data.key.pubKey_y, SEC_ECC_KEY_LEN),
                tr_array(evt->data.key.privKey, SEC_ECC_KEY_LEN));
            DmSecSetEccKey(&evt->data.key);
            memcpy(self._public_key_x, evt->data.key.pubKey_x, sizeof(self._public_key_x));
            self._lesc_keys_generated = true;
            return true;
        }

        case DM_SEC_COMPARE_IND: {
            tr_info("Handling event DM_SEC_COMPARE_IND");
            auto *evt = (dmSecCnfIndEvt_t *) msg;
            handler->on_passkey_display(
                /* connection */ evt->hdr.param,
                DmSecGetCompareValue(evt->confirm)
            );
            handler->on_confirmation_request(/* connection */ evt->hdr.param);
            return true;
        }

        case DM_SEC_KEYPRESS_IND: {
            tr_info("Handling event DM_SEC_KEYPRESS_IND");
            auto *evt = (dmSecKeypressIndEvt_t *) msg;
            handler->on_keypress_notification(
                /* connection */ evt->hdr.param,
                (ble::Keypress_t) evt->notificationType
            );
            return true;
        }
#endif // BLE_FEATURE_SECURE_CONNECTIONS

        default:
            return false;
    }
}

// Helper functions for privacy

#if BLE_FEATURE_SIGNING
void PalSecurityManager::cleanup_peer_csrks()
{
    for (auto & peer_csrk : _peer_csrks) {
        if (peer_csrk) {
            delete peer_csrk;
            peer_csrk = nullptr;
        }
    }
}
#endif // BLE_FEATURE_SIGNING

void PalSecurityManager::set_event_handler(
    PalSecurityManagerEventHandler *event_handler
)
{
    _pal_event_handler = event_handler;
}

PalSecurityManagerEventHandler *PalSecurityManager::get_event_handler()
{
    return _pal_event_handler;
}

} // namespace impl
} // namespace ble

#endif // BLE_FEATURE_SECURITY
