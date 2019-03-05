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

#include "ble/GattServer.h"

namespace ble {
namespace interface {

template<class Impl>
GattServer<Impl>::GattServer() :
    serviceCount(0),
    characteristicCount(0),
    eventHandler(NULL),    
    dataSentCallChain(),
    dataWrittenCallChain(),
    dataReadCallChain(),
    updatesEnabledCallback(NULL),
    updatesDisabledCallback(NULL),
    confirmationReceivedCallback(NULL) {
}

template<class Impl>
ble_error_t GattServer<Impl>::addService(GattService &service)
{
    return impl()->addService_(service);
}

template<class Impl>
ble_error_t GattServer<Impl>::read(
    GattAttribute::Handle_t attributeHandle,
    uint8_t buffer[],
    uint16_t *lengthP
) {
    return impl()->read_(attributeHandle, buffer, lengthP);
}

template<class Impl>
ble_error_t GattServer<Impl>::read(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    uint8_t *buffer,
    uint16_t *lengthP
) {
    return impl()->read_(connectionHandle, attributeHandle, buffer, lengthP);
}

template<class Impl>
ble_error_t GattServer<Impl>::write(
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
) {
    return impl()->write_(attributeHandle, value, size, localOnly);
}

template<class Impl>
ble_error_t GattServer<Impl>::write(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
) {
    return impl()->write_(
        connectionHandle,
        attributeHandle,
        value,
        size,
        localOnly
    );
}

template<class Impl>
ble_error_t GattServer<Impl>::areUpdatesEnabled(
    const GattCharacteristic &characteristic,
    bool *enabledP
) {
    return impl()->areUpdatesEnabled_(characteristic, enabledP);
}

template<class Impl>
ble_error_t GattServer<Impl>::areUpdatesEnabled(
    ble::connection_handle_t connectionHandle,
    const GattCharacteristic &characteristic,
    bool *enabledP
) {
    return impl()->areUpdatesEnabled_(connectionHandle, characteristic, enabledP);
}

template<class Impl>
bool GattServer<Impl>::isOnDataReadAvailable() const
{
    return impl()->isOnDataReadAvailable_();
}

template<class Impl>
ble_error_t GattServer<Impl>::reset(void)
{
    return impl()->reset_();
}

template<class Impl>
ble_error_t GattServer<Impl>::reset_(void)
{
    /* Notify that the instance is about to shutdown */
    shutdownCallChain.call(this);
    shutdownCallChain.clear();

    serviceCount = 0;
    characteristicCount = 0;

    dataSentCallChain.clear();
    dataWrittenCallChain.clear();
    dataReadCallChain.clear();
    updatesEnabledCallback       = NULL;
    updatesDisabledCallback      = NULL;
    confirmationReceivedCallback = NULL;

    return BLE_ERROR_NONE;
}

/* -------------------------- Dummy implementation -------------------------- */

template<class Impl>
ble_error_t GattServer<Impl>::addService_(GattService &service)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::read_(
    GattAttribute::Handle_t attributeHandle,
    uint8_t buffer[],
    uint16_t *lengthP
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::read_(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    uint8_t *buffer,
    uint16_t *lengthP
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::write_(
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::write_(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::areUpdatesEnabled_(
    const GattCharacteristic &characteristic,
    bool *enabledP
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t GattServer<Impl>::areUpdatesEnabled_(
    ble::connection_handle_t connectionHandle,
    const GattCharacteristic &characteristic,
    bool *enabledP
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
bool GattServer<Impl>::isOnDataReadAvailable_() const
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // interface
} // ble
