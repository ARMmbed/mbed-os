/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#ifndef MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_
#define MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_

#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "ble/BLETypes.h"
#include "ble/SafeEnum.h"
#include "ble/BLEProtocol.h"
#include "ble/SecurityManager.h"

namespace ble {
namespace pal {

using SecurityManager::SecurityIOCapabilities_t;
using SecurityManager::IO_CAPS_NONE;
using SecurityManager::SecurityCompletionStatus_t;
using SecurityManager::SecurityMode_t;
using SecurityManager::LinkSecurityStatus_t;
using SecurityManager::Keypress_t;

/* please use typedef for porting not the types directly */

typedef SecurityManager::Passkey_t passkey_t;
typedef SecurityManager::C192_t c192_t;
typedef SecurityManager::R192_t r192_t;
typedef SecurityManager::C256_t c256_t;
typedef SecurityManager::R256_t r256_t;
typedef BLEProtocol::AddressBytes_t address_t;

typedef uint8_t irk_t[16];
typedef uint8_t csrk_t[16];
typedef uint8_t ltk_t[16];
typedef uint8_t ediv_t[8];
typedef uint8_t rand_t[2];
typedef uint32_t passkey_num_t;

typedef uint8_t key_distribution_t;

enum KeyDistributionFlags_t : uint8_t {
    KEY_DISTRIBUTION_NONE       = 0x00,
    KEY_DISTRIBUTION_ENCRYPTION = 0x01,
    KEY_DISTRIBUTION_IDENTITY   = 0x02,
    KEY_DISTRIBUTION_SIGNING    = 0x04,
    KEY_DISTRIBUTION_LINK       = 0x08,
    KEY_DISTRIBUTION_ALL        = 0x0F
};

typedef uint8_t authentication_t;

enum AuthenticationFlags_t : uint8_t {
    AUTHENTICATION_BONDING                = 0x01,
    AUTHENTICATION_MITM                   = 0x04, /* 0x02 missing because bonding uses two bits */
    AUTHENTICATION_SECURE_CONNECTIONS     = 0x08,
    AUTHENTICATION_KEYPRESS_NOTIFICATION  = 0x10
};

struct pairing_request_t {
    SecurityManager::SecurityIOCapabilities_t iocaps;
    bool use_oob;
    authentication_t auth;
    uint8_t max_key_size;
    key_distribution_t initiator_dist;
    key_distribution_t responder_dist;
};

struct bonded_list_entry_t {
    address_t peer_address;
    ediv_t ediv;
    rand_t rand;
    ltk_t ltk;
    csrk_t csrk;
};

struct resolving_list_entry_t {
    address_t peer_address;
    irk_t peer_irk;
    irk_t local_irk;
};

/** Representation of a resolving list. */
struct resolving_list_t {
    resolving_list_entry_t *entries; /**< pointer to array storing the entries */
    uint8_t size; /**< actual number of entries */
    uint8_t capacity;  /**< number of entries that can be stored */
};

/** Representation of a bonded list. */
struct bonded_list_t {
    bonded_list_entry_t *entries; /**< pointer to array storing the entries */
    uint8_t size; /**< actual number of entries */
    uint8_t capacity;  /**< number of entries that can be stored */
};

class SecurityManagerEventHandler {
    SecurityManagerEventHandler() : _app_event_handler(NULL) { };
    virtual void security_setup_initiated(connection_handle_t handle, bool allowBonding,
                                          bool requireMITM, SecurityManager::SecurityIOCapabilities_t iocaps) {
        if (_app_event_handler) {
            _app_event_handler->securitySetupInitiated(handle, allowBonding, requireMITM, iocaps);
        }
    }
    virtual void security_setup_completed(connection_handle_t handle,
                                          SecurityManager::SecurityCompletionStatus_t status) {
        if (_app_event_handler) {
            _app_event_handler->securitySetupCompleted(handle, status);
        }
    }
    virtual void link_secured(connection_handle_t handle, SecurityManager::SecurityMode_t securityMode) {
        if (_app_event_handler) {
            _app_event_handler->linkSecured(handle, securityMode);
        }
    }

    virtual void security_context_stored(connection_handle_t handle) {
        if (_app_event_handler) {
            _app_event_handler->securityContextStored(handle);
        }
    }
    virtual void passkey_display(connection_handle_t handle, const SecurityManager::Passkey_t passkey) {
        if (_app_event_handler) {
            _app_event_handler->passkeyDisplay(handle, passkey);
        }
    }

    virtual void valid_mic_timeout(connection_handle_t handle) {
        if (_app_event_handler) {
            _app_event_handler->validMicTimeout(handle);
        }
    }

    virtual void link_key_failure(connection_handle_t handle) {
        if (_app_event_handler) {
            _app_event_handler->linkKeyFailure(handle);
        }
    }

    virtual void keypress_notification(connection_handle_t handle, SecurityManager::Keypress_t keypress) {
        if (_app_event_handler) {
            _app_event_handler->keypressNotification(handle, keypress);
        }
    }

    virtual void legacy_pariring_oob_request(connection_handle_t handle) {
        if (_app_event_handler) {
            _app_event_handler->legacyPairingOobRequest(handle);
        }
    }

    virtual void oob_request(connection_handle_t handle) {
        if (_app_event_handler) {
            _app_event_handler->oobRequest(handle);
        }
    }
    virtual void pin_request(connection_handle_t handle) {

        if (_app_event_handler) {
            _app_event_handler->pinRequest(handle);
        }
    }
    virtual void passkey_request(connection_handle_t handle) {

        if (_app_event_handler) {
            _app_event_handler->passkeyRequest(handle);
        }
    }
    virtual void confirmation_request(connection_handle_t handle) {

        if (_app_event_handler) {
            _app_event_handler->confirmationRequest(handle);
        }
    }
    virtual void accept_pairing_request(connection_handle_t handle, pairing_request_t &params) {
        if (_app_event_handler) {
            _app_event_handler->acceptPairingRequest(handle);
        }
    }

    virtual void keys_exchanged(connection_handle_t handle, address_t &peer_address, ediv_t &ediv,
                                rand_t &rand, ltk_t &ltk, csrk_t &csrk);
    virtual void ltk_request(connection_handle_t handle, ediv_t &ediv, rand_t &rand);

    virtual void set_app_event_handler(::SecurityManagerEventHandler *app_event_handler) {
        _app_event_handler = app_event_handler;
    }
private:
    ::SecurityManagerEventHandler *_app_event_handler;
};


class SecurityManager : private mbed::NonCopyable<SecurityManager> {
public:
    SecurityManager() : _pal_event_handler(NULL) { };
    virtual ~SecurityManager() { };

    virtual ble_error_t initialize() {
        return BLE_ERROR_NONE;
    }
    virtual ble_error_t terminate() {
        return BLE_ERROR_NONE;
    }
    virtual ble_error_t reset() {
        return BLE_ERROR_NONE;
    }

    /* persistence */

    virtual ble_error_t get_bonded_list(bonded_list_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_bonded_list(bonded_list_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t get_resolving_list(resolving_list_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_resolving_list(resolving_list_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t get_whitelist(Gap::Whitelist_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_whitelist(Gap::Whitelist_t &list) {
        (void)list;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* feature support */

    virtual ble_error_t set_secure_connections_support(bool enabled, bool secure_connections_only = false) {
        (void)enabled;
        (void)secure_connections_only;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t get_secure_connections_support(bool &enabled, bool &secure_connections_only) {
        (void)enabled;
        (void)secure_connections_only;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* security settings */

    virtual ble_error_t set_io_capabilities(SecurityIOCapabilities_t iocaps = IO_CAPS_NONE) {
        (void)iocaps;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_bondable(bool bondable = true) {
        (void)bondable;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_keypress_notification(bool send_keypresses = true) {
        (void)send_keypresses;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_pin_code(uint8_t pin_length, uint8_t *pin_code, bool static_pin = false) {
        (void)pin_length;
        (void)pin_code;
        (void)static_pin;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_passkey(passkey_num_t passkey) {
        (void)passkey;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_oob_data_usage(connection_handle_t handle, bool useOOB, bool OOBProvidesMITM) {
        (void)handle;
        (void)useOOB;
        (void)OOBProvidesMITM;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_encryption_key_size(uint8_t minimum_bitsize, uint8_t maximum_bitsize = 128) {
        (void)minimum_bitsize;
        (void)maximum_bitsize;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t get_encryption_key_size(connection_handle_t, uint8_t &bitsize) {
        (void)bitsize;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_authentication_timeout(connection_handle_t, uint16_t timeout_in_10ms) {
        (void)timeout_in_10ms;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t get_authentication_timeout(connection_handle_t, uint16_t &timeout_in_10ms) {
        (void)timeout_in_10ms;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t refresh_encryption_key(connection_handle_t handle) {
        (void)handle;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* privacy */

    virtual ble_error_t set_private_address_timeout(uint16_t timeout_in_seconds) {
        (void)timeout_in_seconds;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* keys */

    virtual ble_error_t set_ltk(connection_handle_t handle, ltk_t ltk) {
        (void)ltk;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_irk(irk_t irk) {
        (void)irk;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_csrk(csrk_t csrk) {
        (void)csrk;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t generate_irk() {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t generate_csrk() {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* authentication */

    virtual ble_error_t get_encryption_status(connection_handle_t handle, LinkSecurityStatus_t &status) {
        (void)handle;
        (void)status;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t request_pairing(connection_handle_t handle, pairing_request_t &params) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t accept_pairing(connection_handle_t handle, pairing_request_t &params) {
        (void)handle;
        (void)params;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t reject_pairing(connection_handle_t handle) {
        (void)handle;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t cancel_pairing(connection_handle_t handle) {
        (void)handle;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_pairing_request_authorisation(bool authorisation_required = true) {
        (void)authorisation_required;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t request_authentication() {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* MITM */

    virtual ble_error_t confirmation_entered(connection_handle_t handle, bool confirmation) {
        (void)handle;
        (void)confirmation;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t passkey_entered(connection_handle_t handle, passkey_t passkey) {
        (void)handle;
        (void)passkey;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t send_keypress_notification(connection_handle_t handle, Keypress_t keypress) {
        (void)handle;
        (void)keypress;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t set_oob(connection_handle_t handle, c192_t& c192, r192_t& r192) {
        (void)handle;
        (void)c192;
        (void)r192;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t set_extended_oob(connection_handle_t handle,
                                         c192_t& c192, r192_t& r192, c256_t& c256, r256_t& r256) {
        (void)handle;
        (void)c192;
        (void)r192;
        (void)c256;
        (void)r256;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t get_local_oob_data(connection_handle_t handle, c192_t& c192, r192_t& r192) {
        (void)handle;
        (void)c192;
        (void)r192;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    virtual ble_error_t get_local_extended_oob_data(connection_handle_t handle,
                                                    c192_t& c192, r192_t& r192, c256_t& c256, r256_t& r256) {
        (void)handle;
        (void)c192;
        (void)r192;
        (void)c256;
        (void)r256;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Entry points for the underlying stack to report events back to the user. */
 public:
    SecurityManagerEventHandler& get_event_handler() {
        /* guaranteed to be a valid pointer */
        return _pal_event_handler;
    }
    void set_app_event_handler(::SecurityManagerEventHandler *app_event_handler) {
        _pal_event_handler->set_app_event_handler(app_event_handler);
    }
    void set_event_handler(SecurityManagerEventHandler *event_handler) {
        _pal_event_handler = event_handler;
    }

private:
    SecurityManagerEventHandler *_pal_event_handler;

};

} /* namespace pal */
} /* namespace ble */

#endif /* MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_ */
