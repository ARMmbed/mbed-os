/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "ble/SecurityManager.h"


namespace ble {
namespace interface {

template <class Impl>
ble_error_t SecurityManager<Impl>::init(
    bool enableBonding,
    bool requireMITM,
    SecurityIOCapabilities_t iocaps,
    const Passkey_t passkey,
    bool signing,
    const char *dbFilepath
) {
    return impl()->init_(
        enableBonding,
        requireMITM,
        iocaps,
        passkey,
        signing,
        dbFilepath
    );
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setDatabaseFilepath(const char *dbFilepath) {
    return impl()->setDatabaseFilepath_(dbFilepath);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::reset(void) {
    return impl()->reset_();
}

template <class Impl>
ble_error_t SecurityManager<Impl>::reset_() {
    /* Notify that the instance is about to shutdown */
    shutdownCallChain.call(this);
    shutdownCallChain.clear();
    eventHandler = &defaultEventHandler;

    return BLE_ERROR_NONE;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::preserveBondingStateOnReset(bool enable) {
    return impl()->preserveBondingStateOnReset_(enable);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::purgeAllBondingState(void) {
    return impl()->purgeAllBondingState_();
}

template <class Impl>
ble_error_t SecurityManager<Impl>::generateWhitelistFromBondTable(
    ::Gap::Whitelist_t *whitelist
) const {
    return impl()->generateWhitelistFromBondTable_(whitelist);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::requestPairing(
    ble::connection_handle_t connectionHandle
) {
    return impl()->requestPairing_(connectionHandle);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::acceptPairingRequest(
    ble::connection_handle_t connectionHandle
) {
    return impl()->acceptPairingRequest_(connectionHandle);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::cancelPairingRequest(
    ble::connection_handle_t connectionHandle
) {
    return impl()->cancelPairingRequest_(connectionHandle);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setPairingRequestAuthorisation(
    bool required
) {
    return impl()->setPairingRequestAuthorisation_(required);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::allowLegacyPairing(bool allow) {
    return impl()->allowLegacyPairing_(allow);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getSecureConnectionsSupport(bool *enabled) {
    return impl()->getSecureConnectionsSupport_(enabled);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setIoCapability(SecurityIOCapabilities_t iocaps) {
    return impl()->setIoCapability_(iocaps);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setDisplayPasskey(const Passkey_t passkey) {
    return impl()->setDisplayPasskey_(passkey);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setLinkSecurity(
    ble::connection_handle_t connectionHandle,
    SecurityMode_t securityMode
) {
    return impl()->setLinkSecurity_(connectionHandle, securityMode);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setKeypressNotification(bool enabled) {
    return impl()->setKeypressNotification_(enabled);
}

#if BLE_FEATURE_SIGNING
template <class Impl>
ble_error_t SecurityManager<Impl>::enableSigning(
    ble::connection_handle_t connectionHandle,
    bool enabled
) {
    return impl()->enableSigning_(connectionHandle, enabled);
}
#endif // BLE_FEATURE_SIGNING

template <class Impl>
ble_error_t SecurityManager<Impl>::setHintFutureRoleReversal(bool enable) {
    return impl()->setHintFutureRoleReversal_(enable);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getLinkEncryption(
    ble::connection_handle_t connectionHandle,
    ble::link_encryption_t *encryption
) {
    return impl()->getLinkEncryption_(connectionHandle, encryption);
}


template <class Impl>
ble_error_t SecurityManager<Impl>::setLinkEncryption(
    ble::connection_handle_t connectionHandle,
    ble::link_encryption_t encryption
) {
    return impl()->setLinkEncryption_(connectionHandle, encryption);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setEncryptionKeyRequirements(
    uint8_t minimumByteSize,
    uint8_t maximumByteSize
) {
    return impl()->setEncryptionKeyRequirements_(minimumByteSize, maximumByteSize);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::requestAuthentication(
    ble::connection_handle_t connectionHandle
) {
    return impl()->requestAuthentication_(connectionHandle);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::generateOOB(const ble::address_t *address) {
    return impl()->generateOOB_(address);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setOOBDataUsage(
    ble::connection_handle_t connectionHandle,
    bool useOOB,
    bool OOBProvidesMITM
) {
    return impl()->setOOBDataUsage_(connectionHandle, useOOB, OOBProvidesMITM);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::confirmationEntered(
    ble::connection_handle_t connectionHandle,
    bool confirmation
) {
    return impl()->confirmationEntered_(connectionHandle, confirmation);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::passkeyEntered(
    ble::connection_handle_t connectionHandle,
    Passkey_t passkey
) {
    return impl()->passkeyEntered_(connectionHandle, passkey);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::sendKeypressNotification(
    ble::connection_handle_t connectionHandle,
    Keypress_t keypress
) {
    return impl()->sendKeypressNotification_(connectionHandle, keypress);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::legacyPairingOobReceived(
    const ble::address_t *address,
    const ble::oob_tk_t *tk
) {
    return impl()->legacyPairingOobReceived_(address, tk);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::oobReceived(
    const ble::address_t *address,
    const ble::oob_lesc_value_t *random,
    const ble::oob_confirm_t *confirm
) {
    return impl()->oobReceived_(address, random, confirm);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getSigningKey(
    ble::connection_handle_t connectionHandle,
    bool authenticated
) {
    return impl()->getSigningKey_(connectionHandle, authenticated);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getAddressesFromBondTable(
    ::Gap::Whitelist_t &addresses
) const {
    return impl()->getAddressesFromBondTable_(addresses);
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getAddressesFromBondTable_(
    ::Gap::Whitelist_t &addresses
) const {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

/* ------------------------ Dummy implementations --------------------------- */

template <class Impl>
ble_error_t SecurityManager<Impl>::init_(
    bool enableBonding,
    bool requireMITM,
    SecurityIOCapabilities_t iocaps,
    const Passkey_t passkey,
    bool signing,
    const char *dbFilepath
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setDatabaseFilepath_(const char *dbFilepath) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::preserveBondingStateOnReset_(bool enable) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::purgeAllBondingState_(void) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::generateWhitelistFromBondTable_(
    ::Gap::Whitelist_t *whitelist
) const {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::requestPairing_(
    ble::connection_handle_t connectionHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::acceptPairingRequest_(
    ble::connection_handle_t connectionHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::cancelPairingRequest_(
    ble::connection_handle_t connectionHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setPairingRequestAuthorisation_(
    bool required
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::allowLegacyPairing_(bool allow) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getSecureConnectionsSupport_(bool *enabled) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setIoCapability_(SecurityIOCapabilities_t iocaps) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setDisplayPasskey_(const Passkey_t passkey) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setLinkSecurity_(
    ble::connection_handle_t connectionHandle,
    SecurityMode_t securityMode
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setKeypressNotification_(bool enabled) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::enableSigning_(
    ble::connection_handle_t connectionHandle,
    bool enabled
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setHintFutureRoleReversal_(bool enable) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getLinkEncryption_(
    ble::connection_handle_t connectionHandle,
    ble::link_encryption_t *encryption
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}


template <class Impl>
ble_error_t SecurityManager<Impl>::setLinkEncryption_(
    ble::connection_handle_t connectionHandle,
    ble::link_encryption_t encryption
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setEncryptionKeyRequirements_(
    uint8_t minimumByteSize,
    uint8_t maximumByteSize
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::requestAuthentication_(
    ble::connection_handle_t connectionHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::generateOOB_(const ble::address_t *address) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::setOOBDataUsage_(
    ble::connection_handle_t connectionHandle,
    bool useOOB,
    bool OOBProvidesMITM
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::confirmationEntered_(
    ble::connection_handle_t connectionHandle,
    bool confirmation
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::passkeyEntered_(
    ble::connection_handle_t connectionHandle,
    Passkey_t passkey
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::sendKeypressNotification_(
    ble::connection_handle_t connectionHandle,
    Keypress_t keypress
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::legacyPairingOobReceived_(
    const ble::address_t *address,
    const ble::oob_tk_t *tk
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::oobReceived_(
    const ble::address_t *address,
    const ble::oob_lesc_value_t *random,
    const ble::oob_confirm_t *confirm
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template <class Impl>
ble_error_t SecurityManager<Impl>::getSigningKey_(
    ble::connection_handle_t connectionHandle,
    bool authenticated
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // interface
} // ble
