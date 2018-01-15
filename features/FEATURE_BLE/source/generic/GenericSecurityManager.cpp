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

#ifndef __GENERIC_SECURITY_MANAGER_H__
#define __GENERIC_SECURITY_MANAGER_H__

#include "SecurityManager.h"
#include "PalSecurityManager.h"
#include "Callback.h"
#include "ble/pal/GapTypes.h"

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::advertising_peer_address_type_t;
using ble::pal::authentication_t;
using ble::pal::key_distribution_t;
using ble::pal::irk_t;
using ble::pal::csrk_t;
using ble::pal::ltk_t;
using ble::pal::ediv_t;
using ble::pal::rand_t;
using SecurityManager::SecurityIOCapabilities_t;

static const uint8_t NUMBER_OFFSET = '0';

/* separate structs to allow db implementation to minimise memory usage */

struct SecurityEntry_t {
    connection_handle_t handle;
    address_t identity_address;
    uint8_t peer_address_is_public:1;
    uint8_t mitm_protection:1; /**< does the key provide mitm */
    uint8_t is_authenticated:1; /**< have we authenticated during this connection */
    uint8_t is_connected:1;
};

struct SecurityEntryKeys_t {
    ltk_t  ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    irk_t  irk;
    csrk_t csrk;
};

enum DbCbAction_t {
    DB_CB_ACTION_UPDATE,
    DB_CB_ACTION_NO_UPDATE_REQUIRED, /* does not guarantee discarding changes if you made any */
    DB_CB_ACTION_REMOVE
};

typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&)> SecurityEntryDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryKeys_t&)> SecurityEntryKeysDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryIdentity_t&)> SecurityEntryIdentityDbCb_t;
typedef mbed::Callback<DbCbAction_t(Gap::Whitelist_t&)> WhitelistDbCb_t;

class GenericSecurityManagerEventHandler;

class SecurityDb {
public:
    SecurityDb() {};
    ~SecurityDb() {};

    void get_entry(SecurityEntryDbCb_t cb, connection_handle_t handle);
    void get_entry(SecurityEntryKeysDbCb_t cb, ediv_t ediv, rand_t rand);
    void get_entry(SecurityEntryIdentityDbCb_t cb, address_t identity_address);

    void update_entry(SecurityEntry_t&);
    void update_entry(connection_handle_t handle,
                      bool address_is_public,
                      address_t &peer_address,
                      ediv_t &ediv,
                      rand_t &rand,
                      ltk_t &ltk,
                      irk_t &irk,
                      csrk_t &csrk);

    void remove_entry(SecurityEntry_t&);
    void clear_entries(SecurityEntry_t&);

    void get_whitelist(WhitelistDbCb_t cb);

    void update_whitelist(Gap::Whitelist_t&);
    void add_whitelist_entry(address_t);

    void remove_whitelist_entry(address_t);
    void clear_whitelist();
private:

};

class GenericSecurityManager : public SecurityManager,
                               public ble::pal::SecurityManagerEventHandler {
public:
    virtual ble_error_t init(bool                     initBondable = true,
                             bool                     initMITM     = true,
                             SecurityIOCapabilities_t initIocaps   = IO_CAPS_NONE,
                             const Passkey_t          initPasskey  = NULL) {
        loadState();
        bondable = initBondable;
        mitm = initMITM;
        iocaps = initIocaps;
        passkey = initPasskey;

        return BLE_ERROR_NONE;
    }

    void saveState() {
        if (saveStateEnabled) {
            /*save lists */
        }
    }

    void loadState() {
        if (saveStateEnabled) {
            /*load lists */
        }
    }

    virtual ble_error_t setAuthenticationTimeout(connection_handle_t handle, uint32_t timeout_in_ms) {
        return pal.set_authentication_timeout(handle, timeout_in_ms / 10);
    }
    virtual ble_error_t getAuthenticationTimeout(connection_handle_t handle, uint32_t *timeout_in_ms) {
        uint16_t timeout_in_10ms;
        ble_error_t status = pal.get_authentication_timeout(handle, timeout_in_10ms);
        timeout_in_ms = 10 * timeout_in_10ms;
        return status;
    }

    virtual ble_error_t getLinkSecurity(Gap::Handle_t connectionHandle, LinkSecurityStatus_t *securityStatusP) {
        return pal.get_encryption_status(connectionHandle, *securityStatusP);
    }

    ble_error_t setLinkSecurity(Gap::Handle_t connectionHandle, SecurityMode_t securityMode) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    ble_error_t purgeAllBondingState(void) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    ble_error_t setOOBDataUsage(Gap::Handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM = false) {
        return BLE_ERROR_NONE;
    }

    ble_error_t preserveBondingStateOnReset(bool enabled) {
        saveStateEnabled = enabled;
        return BLE_ERROR_NONE;
    }

    ble_error_t setPinCode(uint8_t pinLength, uint8_t *pinCode, bool isStatic = false) {
        return pal.set_pin_code(pinLength, pinCode, isStatic);
    }

    ble_error_t setPasskey(const Passkey_t passkeyASCI, bool isStatic = false) {
        uint32_t passkey = 0;
        for (int i = 0, m = 1; i < 6; ++i, m *= 10) {
            passkey += (passkeyASCI[i] - NUMBER_OFFSET) * m;
        }
        return pal.set_passkey(passkey);
    }

    virtual ble_error_t setPairingRequestAuthorisation(bool required = true) {
        authorisationRequired = required;
        return BLE_ERROR_NONE;
    }

    DbCbAction_t setLtkCb(SecurityEntry_t& entry, SecurityEntryKeys_t& entryKeys) {
        pal.set_ltk(entry.handle, entryKeys.ltk);
        return DB_CB_ACTION_UPDATE;
    }

    void setSecurityManagerEventHandler(::SecurityManagerEventHandler* handler) {
        SecurityManager::setSecurityManagerEventHandler(handler);
        if (handler) {
            _app_event_handler = handler;
        }
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl) : pal(palImpl), saveStateEnabled(false) {
        _app_event_handler = &defaultEventHandler;
        pal.set_event_handler(this);
    }

public:
    ble_error_t reset(void) {
        saveState();

        SecurityManager::reset();

        return BLE_ERROR_NONE;
    }

private:
    ble::pal::SecurityManager& pal;
    bool saveStateEnabled;

    SecurityDb db;

    SecurityIOCapabilities_t iocaps;
    Passkey_t                passkey;
    bool                     mitm;
    bool                     bondable;
    bool                     authorisationRequired;

    bool                useOob;
    authentication_t    authentication;
    uint8_t             minKeySize;
    uint8_t             maxKeySize;
    key_distribution_t  initiatorDist;
    key_distribution_t  responderDist;

    /*  implements ble::pal::SecurityManagerEventHandler */
public:
    virtual void security_setup_initiated(connection_handle_t handle, bool allow_bonding,
                                          bool require_mitm, SecurityIOCapabilities_t iocaps) {
        if (_app_event_handler) {
            _app_event_handler->securitySetupInitiated(handle, allow_bonding, require_mitm, iocaps);
        }
    }
    virtual void security_setup_completed(connection_handle_t handle,
                                          SecurityManager::SecurityCompletionStatus_t status) {
        if (_app_event_handler) {
            _app_event_handler->securitySetupCompleted(handle, status);
        }
    }
    virtual void link_secured(connection_handle_t handle, SecurityManager::SecurityMode_t security_mode) {
        if (_app_event_handler) {
            _app_event_handler->linkSecured(handle, security_mode);
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
    virtual void accept_pairing_request(connection_handle_t handle,
                                        SecurityIOCapabilities_t iocaps,
                                        bool use_oob,
                                        authentication_t authentication,
                                        uint8_t max_key_size,
                                        key_distribution_t initiator_dist,
                                        key_distribution_t responder_dist) {
        if (_app_event_handler && authorisationRequired) {
            _app_event_handler->acceptPairingRequest(handle);
        }
    }

    void keys_exchanged(connection_handle_t handle,
                        advertising_peer_address_type_t peer_address_type,
                        address_t &peer_address,
                        ediv_t &ediv,
                        rand_t &rand,
                        ltk_t &ltk,
                        irk_t &irk,
                        csrk_t &csrk) {
        db.update_entry(
            handle,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_address,
            ediv,
            rand,
            ltk,
            irk,
            csrk
        );

    }
    void ltk_request(connection_handle_t handle, ediv_t &ediv, rand_t &rand) {
        db.get_entry(
            mbed::callback(this, &GenericSecurityManager::setLtkCb),
            ediv,
            rand
        );
    }

private:
    /* handler is always a valid pointer */
    ::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
