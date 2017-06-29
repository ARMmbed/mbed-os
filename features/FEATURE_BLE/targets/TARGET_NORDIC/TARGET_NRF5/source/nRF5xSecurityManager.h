/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef __NRF51822_SECURITY_MANAGER_H__
#define __NRF51822_SECURITY_MANAGER_H__

#include <stddef.h>

#include "nRF5xGap.h"
#include "ble/SecurityManager.h"
#include "btle_security.h"

class nRF5xSecurityManager : public SecurityManager
{
public:
    /* Functions that must be implemented from SecurityManager */
    virtual ble_error_t init(bool                     enableBonding,
                             bool                     requireMITM,
                             SecurityIOCapabilities_t iocaps,
                             const Passkey_t          passkey) {
        return btle_initializeSecurity(enableBonding, requireMITM, iocaps, passkey);
    }

    virtual ble_error_t getLinkSecurity(Gap::Handle_t connectionHandle, LinkSecurityStatus_t *securityStatusP) {
        return btle_getLinkSecurity(connectionHandle, securityStatusP);
    }

    virtual ble_error_t setLinkSecurity(Gap::Handle_t connectionHandle, SecurityMode_t securityMode) {
        return btle_setLinkSecurity(connectionHandle, securityMode);
    }

    virtual ble_error_t purgeAllBondingState(void) {
        return btle_purgeAllBondingState();
    }
#if  (NRF_SD_BLE_API_VERSION <= 2)
    /**
     * @brief  Returns a list of addresses from peers in the stacks bond table.
     *
     * @param[in/out]   addresses
     *                  (on input) @ref Gap::Whitelist_t structure where at
     *                  most addresses.capacity addresses from bonded peers will
     *                  be stored.
     *                  (on output) A copy of the addresses from bonded peers.
     *
     * @return
     *           BLE_ERROR_NONE if successful.
     */
    virtual ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        uint8_t i;

        ble_gap_whitelist_t  whitelistFromBondTable;
        ble_gap_addr_t      *addressPtr[YOTTA_CFG_WHITELIST_MAX_SIZE];
        ble_gap_irk_t       *irkPtr[YOTTA_CFG_IRK_TABLE_MAX_SIZE];

        /* Initialize the structure so that we get as many addreses as the whitelist can hold */
        whitelistFromBondTable.addr_count = YOTTA_CFG_IRK_TABLE_MAX_SIZE;
        whitelistFromBondTable.pp_addrs   = addressPtr;
        whitelistFromBondTable.irk_count  = YOTTA_CFG_IRK_TABLE_MAX_SIZE;
        whitelistFromBondTable.pp_irks    = irkPtr;

        ble_error_t error = createWhitelistFromBondTable(whitelistFromBondTable);
        if (error != BLE_ERROR_NONE) {
            addresses.size = 0;
            return error;
        }

        /* Put all the addresses in the structure */
        for (i = 0; i < whitelistFromBondTable.addr_count; ++i) {
            if (i >= addresses.capacity) {
                /* Ran out of space in the output Gap::Whitelist_t */
                addresses.size = i;
                return BLE_ERROR_NONE;
            }
            memcpy(&addresses.addresses[i], whitelistFromBondTable.pp_addrs[i], sizeof(BLEProtocol::Address_t));
        }

        /* Update the current address count */
        addresses.size = i;

        /* The assumption here is that the underlying implementation of
         * createWhitelistFromBondTable()  will not return the private resolvable
         * addresses (which is the case in the SoftDevice). Rather it returns the
         * IRKs, so we need to generate the private resolvable address by ourselves.
         */
        for (i = 0; i < whitelistFromBondTable.irk_count; ++i) {
            if (i + addresses.size >= addresses.capacity) {
                /* Ran out of space in the output Gap::Whitelist_t */
                addresses.size += i;
                return BLE_ERROR_NONE;
            }
            btle_generateResolvableAddress(
                *whitelistFromBondTable.pp_irks[i],
                (ble_gap_addr_t &) addresses.addresses[i + addresses.size]
            );
        }

        /* Update the current address count */
        addresses.size += i;

        return BLE_ERROR_NONE;
    }
#else // -> NRF_SD_BLE_API_VERSION >= 3
    /**
     * @brief  Returns a list of addresses from peers in the stacks bond table.
     *
     * @param[in/out]   addresses
     *                  (on input) @ref Gap::Whitelist_t structure where at
     *                  most addresses.capacity addresses from bonded peers will
     *                  be stored.
     *                  (on output) A copy of the addresses from bonded peers.
     *
     * @retval BLE_ERROR_NONE         if successful.
     * @retval BLE_ERROR_UNSPECIFIED  Bond data could not be found in flash or is inconsistent.
     */
    virtual ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        return btle_getAddressesFromBondTable(addresses);
    }
#endif // #if  (NRF_SD_BLE_API_VERSION <= 2)



    /**
     * @brief  Clear nRF5xSecurityManager's state.
     *
     * @return
     *           BLE_ERROR_NONE if successful.
     */
    virtual ble_error_t reset(void)
    {
        if (SecurityManager::reset() != BLE_ERROR_NONE) {
            return BLE_ERROR_INVALID_STATE;
        }

        return BLE_ERROR_NONE;
    }

    bool hasInitialized(void) const {
        return btle_hasInitializedSecurity();
    }

public:
    /*
     * Allow instantiation from nRF5xn when required.
     */
    friend class nRF5xn;

    nRF5xSecurityManager() {
        /* empty */
    }

private:
    nRF5xSecurityManager(const nRF5xSecurityManager &);
    const nRF5xSecurityManager& operator=(const nRF5xSecurityManager &);

#if  (NRF_SD_BLE_API_VERSION <= 2)
    /*
     * Expose an interface that allows us to query the SoftDevice bond table
     * and extract a whitelist.
     */
    ble_error_t createWhitelistFromBondTable(ble_gap_whitelist_t &whitelistFromBondTable) const {
        return btle_createWhitelistFromBondTable(&whitelistFromBondTable);
    }
#endif
    /*
     * Given a BLE address and a IRK this function check whether the address
     * can be generated from the IRK. To do so, this function uses the hash
     * function and algorithm described in the Bluetooth low Energy
     * Specification. Internally, Nordic SDK functions are used.
     */
    bool matchAddressAndIrk(ble_gap_addr_t *address, ble_gap_irk_t *irk) const {
        return btle_matchAddressAndIrk(address, irk);
    }

    /*
     * Give nRF5xGap access to createWhitelistFromBondTable() and
     * matchAddressAndIrk()
     */
    friend class nRF5xGap;
};

#endif // ifndef __NRF51822_SECURITY_MANAGER_H__
