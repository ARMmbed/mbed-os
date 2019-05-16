/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "ble/GattClient.h"

namespace ble {
namespace interface {

template<class Impl>
ble_error_t GattClient<Impl>::launchServiceDiscovery(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t sc,
    ServiceDiscovery::CharacteristicCallback_t cc,
    const UUID &matchingServiceUUID,
    const UUID &matchingCharacteristicUUIDIn
) {
    return impl()->launchServiceDiscovery_(
        connectionHandle,
        sc,
        cc,
        matchingServiceUUID,
        matchingCharacteristicUUIDIn
    );
}

template<class Impl>
ble_error_t GattClient<Impl>::discoverServices(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t callback,
    GattAttribute::Handle_t startHandle,
    GattAttribute::Handle_t endHandle
) {
    return impl()->discoverServices_(
        connectionHandle,
        callback,
        startHandle,
        endHandle
    );
}

template<class Impl>
bool GattClient<Impl>::isServiceDiscoveryActive(void) const
{
    return impl()->isServiceDiscoveryActive_();
}

template<class Impl>
void GattClient<Impl>::terminateServiceDiscovery(void)
{
    return impl()->terminateServiceDiscovery_();
}

template<class Impl>
ble_error_t GattClient<Impl>::read(
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    uint16_t offset
) const {
    return impl()->read_(connHandle, attributeHandle, offset);
}

template<class Impl>
ble_error_t GattClient<Impl>::write(
    GattClient::WriteOp_t cmd,
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    size_t length,
    const uint8_t *value
) const {
    return impl()->write_(
        cmd,
        connHandle,
        attributeHandle,
        length,
        value
    );
}

template<class Impl>
void GattClient<Impl>::onServiceDiscoveryTermination(
    ServiceDiscovery::TerminationCallback_t callback
) {
    return impl()->onServiceDiscoveryTermination_(callback);
}

template<class Impl>
ble_error_t GattClient<Impl>::discoverCharacteristicDescriptors(
    const DiscoveredCharacteristic& characteristic,
    const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
    const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
) {
    return impl()->discoverCharacteristicDescriptors_(
        characteristic,
        discoveryCallback,
        terminationCallback
    );
}

template<class Impl>
bool GattClient<Impl>::isCharacteristicDescriptorDiscoveryActive(
    const DiscoveredCharacteristic& characteristic
) const {
    return impl()->isCharacteristicDescriptorDiscoveryActive_(characteristic);
}

template<class Impl>
void GattClient<Impl>::terminateCharacteristicDescriptorDiscovery(
    const DiscoveredCharacteristic& characteristic
) {
    return impl()->terminateCharacteristicDescriptorDiscovery_(characteristic);
}

template<class Impl>
ble_error_t GattClient<Impl>::negotiateAttMtu(
    ble::connection_handle_t connHandle
) {
    return impl()->negotiateAttMtu_(connHandle);
}

template<class Impl>
ble_error_t GattClient<Impl>::reset(void)
{
    return impl()->reset_();
}

/* ------------------------ Default implementations ------------------------- */


template<class Impl>
ble_error_t GattClient<Impl>::reset_(void)
{
    /* Notify that the instance is about to shut down. */
    shutdownCallChain.call(this);
    shutdownCallChain.clear();

    onDataReadCallbackChain.clear();
    onDataWriteCallbackChain.clear();
    onHVXCallbackChain.clear();

    return BLE_ERROR_NONE;
}

template<class Impl>
ble_error_t GattClient<Impl>::discoverServices_(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t callback,
    GattAttribute::Handle_t startHandle,
    GattAttribute::Handle_t endHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattClient<Impl>::launchServiceDiscovery_(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t sc,
    ServiceDiscovery::CharacteristicCallback_t cc,
    const UUID &matchingServiceUUID,
    const UUID &matchingCharacteristicUUIDIn
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
bool GattClient<Impl>::isServiceDiscoveryActive_(void) const
{
    return false;
}

template<class Impl>
void GattClient<Impl>::terminateServiceDiscovery_(void)
{
}

template<class Impl>
ble_error_t GattClient<Impl>::negotiateAttMtu_(
    ble::connection_handle_t connHandle
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattClient<Impl>::read_(
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    uint16_t offset
) const {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattClient<Impl>::write_(
    GattClient::WriteOp_t cmd,
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    size_t length,
    const uint8_t *value
) const {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
void GattClient<Impl>::onServiceDiscoveryTermination_(
    ServiceDiscovery::TerminationCallback_t callback
) {
}

template<class Impl>
ble_error_t GattClient<Impl>::discoverCharacteristicDescriptors_(
    const DiscoveredCharacteristic& characteristic,
    const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
    const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
bool GattClient<Impl>::isCharacteristicDescriptorDiscoveryActive_(
    const DiscoveredCharacteristic& characteristic
) const {
    return false;
}

template<class Impl>
void GattClient<Impl>::terminateCharacteristicDescriptorDiscovery_(
    const DiscoveredCharacteristic& characteristic
) {
}

} // interface
} // ble
