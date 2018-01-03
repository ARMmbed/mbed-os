/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "PalSm.h"

class GenericSecurityManager : public SecurityManager {
public:
    
    virtual ble_error_t init(bool                     enableBonding = true,
                             bool                     requireMITM   = true,
                             SecurityIOCapabilities_t iocaps        = IO_CAPS_NONE,
                             const Passkey_t          passkey       = NULL) {
        /* Avoid compiler warnings about unused variables. */
        (void)enableBonding;
        (void)requireMITM;
        (void)iocaps;
        (void)passkey;

        loadState();

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
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

    virtual ble_error_t getLinkSecurity(Gap::Handle_t connectionHandle, LinkSecurityStatus_t *securityStatusP) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)securityStatusP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
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

    ble_error_t setOOBDataUsage(Gap::Handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM) {
        /*
        [].useOOB = useOOB;
        [].OOBProvidesMITM = OOBProvidesMITM;
        */

        return BLE_ERROR_NONE;
    }

    ble_error_t preserveBondingStateOnReset(bool enabled) {
        saveStateEnabled = enabled;
        return BLE_ERROR_NONE;
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl) : pal(palImpl), saveStateEnabled(false) {
        eventHandler = new SecurityManagerEventHandler();
        pal.setSecurityManagerEventHandler(eventHandler);
    }
public:
    
    ble_error_t reset(void) {
        saveState();
        SecurityManager::reset();
        pal.setSecurityManagerEventHandler(eventHandler);

        return BLE_ERROR_NONE;
    }

private:
    ble::pal::SecurityManager& pal;
    bool saveStateEnabled;
};

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
