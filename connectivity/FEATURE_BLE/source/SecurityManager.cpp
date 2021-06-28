/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#include "ble/SecurityManager.h"
#include "source/generic/SecurityManagerImpl.h"

namespace ble {

ble_error_t SecurityManager::init(
    bool                     enableBonding,
    bool                     requireMITM,
    SecurityIOCapabilities_t iocaps,
    const Passkey_t          passkey,
    bool                     signing,
    const char              *dbFilepath
)
{
    return impl->init(enableBonding, requireMITM, iocaps, passkey, signing, dbFilepath);
}

ble_error_t SecurityManager::setDatabaseFilepath(const char *dbFilepath)
{
    return impl->setDatabaseFilepath(dbFilepath);
}

ble_error_t SecurityManager::reset()
{
    return impl->reset();
}

ble_error_t SecurityManager::preserveBondingStateOnReset(bool enable)
{
    return impl->preserveBondingStateOnReset(enable);
}

ble_error_t SecurityManager::writeBondingStateToPersistentStorage()
{
    ble_error_t err = impl->preserveBondingStateOnReset(true);
    if (err) {
        return err;
    }
    return impl->writeBondingStateToPersistentStorage();
}

ble_error_t SecurityManager::purgeAllBondingState()
{
    return impl->purgeAllBondingState();
}

ble_error_t SecurityManager::generateWhitelistFromBondTable(::ble::whitelist_t *whitelist) const
{
    return impl->generateWhitelistFromBondTable(whitelist);
}

#if BLE_ROLE_CENTRAL
ble_error_t SecurityManager::requestPairing(ble::connection_handle_t connectionHandle)
{
    return impl->requestPairing(connectionHandle);
}
#endif

#if BLE_ROLE_PERIPHERAL
ble_error_t SecurityManager::acceptPairingRequest(ble::connection_handle_t connectionHandle)
{
    return impl->acceptPairingRequest(connectionHandle);
}
#endif

ble_error_t SecurityManager::cancelPairingRequest(ble::connection_handle_t connectionHandle)
{
    return impl->cancelPairingRequest(connectionHandle);
}

ble_error_t SecurityManager::setPairingRequestAuthorisation(bool required)
{
    return impl->setPairingRequestAuthorisation(required);
}

ble_error_t SecurityManager::getPeerIdentity(ble::connection_handle_t connectionHandle)
{
    return impl->getPeerIdentity(connectionHandle);
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t SecurityManager::allowLegacyPairing(bool allow)
{
    return impl->allowLegacyPairing(allow);
}

ble_error_t SecurityManager::getSecureConnectionsSupport(bool *enabled)
{
    return impl->getSecureConnectionsSupport(enabled);
}
#endif

ble_error_t SecurityManager::setIoCapability(SecurityIOCapabilities_t iocaps)
{
    return impl->setIoCapability(iocaps);
}

ble_error_t SecurityManager::setDisplayPasskey(const Passkey_t passkey)
{
    return impl->setDisplayPasskey(passkey);
}

ble_error_t SecurityManager::setLinkSecurity(ble::connection_handle_t connectionHandle, SecurityMode_t securityMode)
{
    return impl->setLinkSecurity(connectionHandle, securityMode);
}

ble_error_t SecurityManager::setKeypressNotification(bool enabled)
{
    return impl->setKeypressNotification(enabled);
}

#if BLE_FEATURE_SIGNING
ble_error_t SecurityManager::enableSigning(ble::connection_handle_t connectionHandle, bool enabled)
{
    return impl->enableSigning(connectionHandle, enabled);
}
#endif // BLE_FEATURE_SIGNING

ble_error_t SecurityManager::setHintFutureRoleReversal(bool enable)
{
    return impl->setHintFutureRoleReversal(enable);
}

ble_error_t SecurityManager::getLinkEncryption(ble::connection_handle_t connectionHandle, ble::link_encryption_t *encryption)
{
    return impl->getLinkEncryption(connectionHandle, encryption);
}

ble_error_t SecurityManager::setLinkEncryption(ble::connection_handle_t connectionHandle, ble::link_encryption_t encryption)
{
    return impl->setLinkEncryption(connectionHandle, encryption);
}

ble_error_t SecurityManager::setEncryptionKeyRequirements(uint8_t minimumByteSize, uint8_t maximumByteSize)
{
    return impl->setEncryptionKeyRequirements(minimumByteSize, maximumByteSize);
}

ble_error_t SecurityManager::getEncryptionKeySize(
    connection_handle_t connectionHandle,
    uint8_t *size
)
{
    return impl->getEncryptionKeySize(connectionHandle, size);
}

ble_error_t SecurityManager::requestAuthentication(ble::connection_handle_t connectionHandle)
{
    return impl->requestAuthentication(connectionHandle);
}

ble_error_t SecurityManager::generateOOB(const ble::address_t *address)
{
    return impl->generateOOB(address);
}

ble_error_t SecurityManager::setOOBDataUsage(ble::connection_handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM)
{
    return impl->setOOBDataUsage(connectionHandle, useOOB, OOBProvidesMITM);
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t SecurityManager::confirmationEntered(ble::connection_handle_t connectionHandle, bool confirmation)
{
    return impl->confirmationEntered(connectionHandle, confirmation);
}
#endif // BLE_FEATURE_SECURE_CONNECTIONS

ble_error_t SecurityManager::passkeyEntered(ble::connection_handle_t connectionHandle, Passkey_t passkey)
{
    return impl->passkeyEntered(connectionHandle, passkey);
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t SecurityManager::sendKeypressNotification(ble::connection_handle_t connectionHandle, ble::Keypress_t keypress)
{
    return impl->sendKeypressNotification(connectionHandle, keypress);
}
#endif // BLE_FEATURE_SECURE_CONNECTIONS

ble_error_t SecurityManager::legacyPairingOobReceived(const ble::address_t *address, const ble::oob_tk_t *tk)
{
    return impl->legacyPairingOobReceived(address, tk);
}

#if BLE_FEATURE_SECURE_CONNECTIONS
ble_error_t SecurityManager::oobReceived(const ble::address_t *address, const ble::oob_lesc_value_t *random, const ble::oob_confirm_t *confirm)
{
    return impl->oobReceived(address, random, confirm);
}
#endif // BLE_FEATURE_SECURE_CONNECTIONS

#if BLE_FEATURE_SIGNING
ble_error_t SecurityManager::getSigningKey(ble::connection_handle_t connectionHandle, bool authenticated)
{
    return impl->getSigningKey(connectionHandle, authenticated);
}
#endif // BLE_FEATURE_SIGNING

#if BLE_FEATURE_PRIVACY
ble_error_t SecurityManager::setPrivateAddressTimeout(
    uint16_t timeout_in_seconds
)
{
    return impl->setPrivateAddressTimeout(timeout_in_seconds);
}
#endif // BLE_FEATURE_PRIVACY

void SecurityManager::onShutdown(const SecurityManagerShutdownCallback_t& callback)
{
    return impl->onShutdown(callback);
}

SecurityManager::SecurityManagerShutdownCallbackChain_t& SecurityManager::onShutdown()
{
    return impl->onShutdown();
}

void SecurityManager::setSecurityManagerEventHandler(EventHandler* handler)
{
    return impl->setSecurityManagerEventHandler(handler);
}

} // ble

