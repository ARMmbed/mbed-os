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

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::authentication_t;
using ble::pal::key_distribution_t;
using ble::pal::irk_t;
using ble::pal::csrk_t;
using ble::pal::ltk_t;
using ble::pal::ediv_t;
using ble::pal::rand_t;
using SecurityManager::SecurityIOCapabilities_t;

static const uint8_t NUMBER_OFFSET = '0';

struct SecurityEntry_t {
    connection_handle_t handle;
    uint8_t peer_address_is_public:1;
    uint8_t mitm_protection:1;
    uint8_t is_authenticated:1;
    uint8_t is_connected:1;
};

struct SecurityEntryKeys_t {
    ltk_t  ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    address_t identity_address;
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

    void get_entry(SecurityEntryDbCb_t cb, ediv_t ediv, rand_t rand);
    void get_entry(SecurityEntryDbCb_t cb, address_t identity_address);
    void get_entry(SecurityEntryDbCb_t cb, connection_handle_t handle);

    void update_entry(SecurityEntry_t&);
    void update_entry(connection_handle_t handle, address_t &peer_address, ediv_t &ediv,
                      rand_t &rand, ltk_t &ltk, csrk_t &csrk);

    void remove_entry(SecurityEntry_t&);
    void clear_entries(SecurityEntry_t&);

    void get_whitelist(WhitelistDbCb_t cb);

    void update_whitelist(Gap::Whitelist_t&);
    void add_whitelist_entry(address_t);

    void remove_whitelist_entry(address_t);
    void clear_whitelist();
private:

};

class GenericSecurityManager : public SecurityManager {
public:
    virtual ble_error_t init(bool                     enableBonding  = true,
                             bool                     requireMITM    = true,
                             SecurityIOCapabilities_t initIocaps     = IO_CAPS_NONE,
                             const Passkey_t          defaultPasskey = NULL) {
        loadState();
        bonding = enableBonding;
        mitm = requireMITM;
        iocaps = initIocaps;
        passkey = defaultPasskey;

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
        //return pal.set_security_mode(connectionHandle, securityMode);
    }

    ble_error_t purgeAllBondingState(void) {
        ble::pal::bonded_list_t empty_list = { NULL, 0, 0 };
        return pal.clear_bonded_list();
    }

    ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        return pal.get_whitelist(addresses);
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

    DbCbAction_t setLtkCb(SecurityEntry_t& entry, SecurityEntryKeys_t& entryKeys) {
        pal.set_ltk(entry.handle, entryKeys.ltk);
        return DB_CB_ACTION_UPDATE;
    }

    void setSecurityManagerEventHandler(SecurityManagerEventHandler* handler) {
        SecurityManager::setSecurityManagerEventHandler(handler);
        /* handler is always a valid pointer */
        palEventHandler.set_app_event_handler(eventHandler);
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl) : pal(palImpl), saveStateEnabled(false) {
        palEventHandler.set_app_event_handler(&defaultEventHandler);
        pal.set_event_handler(&palEventHandler);
    }

public:
    ble_error_t reset(void) {
        saveState();

        SecurityManager::reset();

        return BLE_ERROR_NONE;
    }

private:
    ble::pal::SecurityManager& pal;
    GenericSecurityManagerEventHandler palEventHandler;
    bool saveStateEnabled;

    SecurityDb db;

    SecurityIOCapabilities_t iocaps;
    Passkey_t                passkey;
    bool                     mitm;
    bool                     bonding;

    bool                useOob;
    authentication_t    authentication;
    uint8_t             minKeySize;
    uint8_t             maxKeySize;
    key_distribution_t  initiatorDist;
    key_distribution_t  responderDist;
};


} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
