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

#ifndef _BTLE_SECURITY_H_
#define _BTLE_SECURITY_H_

#include "ble/Gap.h"
#include "ble/SecurityManager.h"

/**
 * Function to test whether the SecurityManager has been initialized.
 * Possible by a call to @ref btle_initializeSecurity().
 *
 * @return True if the SecurityManager was previously initialized, false
 *         otherwise.
 */
bool btle_hasInitializedSecurity(void);

/**
 * Enable Nordic's Device Manager, which brings in functionality from the
 * stack's Security Manager. The Security Manager implements the actual
 * cryptographic algorithms and protocol exchanges that allow two devices to
 * securely exchange data and privately detect each other.
 *
 * @param[in]  enableBonding Allow for bonding.
 * @param[in]  requireMITM   Require protection for man-in-the-middle attacks.
 * @param[in]  iocaps        To specify IO capabilities of this peripheral,
 *                           such as availability of a display or keyboard to
 *                           support out-of-band exchanges of security data.
 * @param[in]  passkey       To specify a static passkey.
 *
 * @return BLE_ERROR_NONE on success.
 */
ble_error_t btle_initializeSecurity(bool                                      enableBonding = true,
                                    bool                                      requireMITM   = true,
                                    SecurityManager::SecurityIOCapabilities_t iocaps        = SecurityManager::IO_CAPS_NONE,
                                    const SecurityManager::Passkey_t          passkey       = NULL);

/**
 * Get the security status of a link.
 *
 * @param[in]  connectionHandle
 *               Handle to identify the connection.
 * @param[out] securityStatusP
 *               security status.
 *
 * @return BLE_ERROR_NONE Or appropriate error code indicating reason for failure.
 */
ble_error_t btle_getLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::LinkSecurityStatus_t *securityStatusP);

/**
 * Set the security mode on a connection. Useful for elevating the security mode
 * once certain conditions are met, e.g., a particular service is found.
 *
 * @param[in]  connectionHandle
 *               Handle to identify the connection.
 * @param[in]  securityMode
 *               security mode.
 *
 * @return BLE_ERROR_NONE Or appropriate error code indicating reason for failure.
 */
ble_error_t btle_setLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::SecurityMode_t securityMode);

/**
 * Function for deleting all peer device context and all related bonding
 * information from the database.
 *
 * @retval BLE_ERROR_NONE             On success, else an error code indicating reason for failure.
 * @retval BLE_ERROR_INVALID_STATE    If the API is called without module initialization and/or
 *                                    application registration.
 */
ble_error_t btle_purgeAllBondingState(void);

/**
 * Query the SoftDevice bond table to extract a whitelist containing the BLE
 * addresses and IRKs of bonded devices.
 *
 * @param[in/out]  p_whitelist
 *                  (on input) p_whitelist->addr_count and
 *                  p_whitelist->irk_count specify the maximum number of
 *                  addresses and IRKs added to the whitelist structure.
 *                  (on output) *p_whitelist is a whitelist containing the
 *                  addresses and IRKs of the bonded devices.
 *
 * @return BLE_ERROR_NONE Or appropriate error code indicating reason for failure.
 */
ble_error_t btle_createWhitelistFromBondTable(ble_gap_whitelist_t *p_whitelist);

/**
 * Function to test whether a BLE address is generated using an IRK.
 *
 * @param[in]   p_addr
 *                  Pointer to a BLE address.
 * @param[in]   p_irk
 *                  Pointer to an IRK.
 *
 * @return True if p_addr can be generated using p_irk, false otherwise.
 */
bool btle_matchAddressAndIrk(ble_gap_addr_t const * p_addr, ble_gap_irk_t const * p_irk);

/**
 * Function to generate a private resolvable BLE address.
 *
 * @param[out]  p_addr
 *                  The output address.
 * @param[in]   p_irk
 *                  A reference to a IRK.
 *
 * @note This function does not generate a secure address since the prand number in the
 *       resolvable address is not truly random. Therefore, the output of this function
 *       is only meant to be used by the application internally but never exported.
 */
void btle_generateResolvableAddress(const ble_gap_irk_t &irk, ble_gap_addr_t &address);

#endif /* _BTLE_SECURITY_H_ */
