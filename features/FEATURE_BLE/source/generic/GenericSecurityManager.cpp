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

namespace ble {
namespace generic {

static const uint8_t NUMBER_OFFSET = '0';

class GenericSecurityManager : public SecurityManager {
public:
    virtual ble_error_t init(bool                     enableBonding  = true,
                             bool                     requireMITM    = true,
                             SecurityIOCapabilities_t iocaps         = IO_CAPS_NONE,
                             const Passkey_t          passkey        = NULL) {
        (void)requireMITM;
        loadState();
        pal.set_io_capabilities(iocaps);
        pal.set_bondable(enableBonding);
        setPasskey(passkey);

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
        return pal.set_security_mode(connectionHandle, securityMode);
    }

    ble_error_t purgeAllBondingState(void) {
        ble::pal::bonded_list_t empty_list = { NULL, 0, 0 };
        return pal.set_bonded_list(empty_list);
    }

    ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        return pal.get_whitelist(addresses);
    }

    ble_error_t setOOBDataUsage(Gap::Handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM = false) {
        return pal.set_oob_data_usage(connectionHandle, useOOB, OOBProvidesMITM);
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

    void setSecurityManagerEventHandler(SecurityManagerEventHandler* handler) {
        SecurityManager::setSecurityManagerEventHandler(handler);
        /* handler is always a valid pointer */
        pal.set_event_handler(*handler);
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl) : pal(palImpl), saveStateEnabled(false) {
        pal.set_event_handler(defaultEventHandler);
    }

public:
    ble_error_t reset(void) {
        saveState();

        SecurityManager::reset();

        pal.set_event_handler(*eventHandler);

        return BLE_ERROR_NONE;
    }

private:
    ble::pal::SecurityManager& pal;
    bool saveStateEnabled;
};

} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
