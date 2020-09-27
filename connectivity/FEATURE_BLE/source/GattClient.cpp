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

#include "ble/GattClient.h"
#include "source/generic/GattClientImpl.h"

namespace ble {

void GattClient::setEventHandler(EventHandler *handler)
{
    impl->setEventHandler(handler);
}

ble_error_t GattClient::launchServiceDiscovery(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t sc,
    ServiceDiscovery::CharacteristicCallback_t  cc,
    const UUID &matchingServiceUUID,
    const UUID &matchingCharacteristicUUIDIn
)
{
    return impl->launchServiceDiscovery(
        connectionHandle, sc, cc, matchingServiceUUID, matchingCharacteristicUUIDIn
    );
}

ble_error_t GattClient::discoverServices(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t callback,
    const UUID &matchingServiceUUID
)
{
    return impl->discoverServices(connectionHandle, callback, matchingServiceUUID);
}

ble_error_t GattClient::discoverServices(
    ble::connection_handle_t connectionHandle,
    ServiceDiscovery::ServiceCallback_t callback,
    GattAttribute::Handle_t startHandle,
    GattAttribute::Handle_t endHandle
)
{
    return impl->discoverServices(connectionHandle, callback, startHandle, endHandle);
}

bool GattClient::isServiceDiscoveryActive(void) const
{
    return impl->isServiceDiscoveryActive();
}

void GattClient::terminateServiceDiscovery(void)
{
    return impl->terminateServiceDiscovery();
}

ble_error_t GattClient::read(
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    uint16_t offset
) const
{
    return impl->read(connHandle, attributeHandle, offset);
}

ble_error_t GattClient::write(
    GattClient::WriteOp_t cmd,
    ble::connection_handle_t connHandle,
    GattAttribute::Handle_t attributeHandle,
    size_t length,
    const uint8_t *value
) const
{
    return impl->write(cmd, connHandle, attributeHandle, length, value);
}

/* Event callback handlers. */

void GattClient::onDataRead(ble::ReadCallback_t callback)
{
    return impl->onDataRead(callback);
}

ble::ReadCallbackChain_t& GattClient::onDataRead()
{
    return impl->onDataRead();
}

void GattClient::onDataWritten(ble::WriteCallback_t callback)
{
    impl->onDataWritten(callback);
}

ble::WriteCallbackChain_t& GattClient::onDataWritten()
{
    return impl->onDataWritten();
}

void GattClient::onServiceDiscoveryTermination(
    ServiceDiscovery::TerminationCallback_t callback
)
{
    return impl->onServiceDiscoveryTermination(callback);
}

ble_error_t GattClient::discoverCharacteristicDescriptors(
    const DiscoveredCharacteristic& characteristic,
    const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
    const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
)
{
    return impl->discoverCharacteristicDescriptors(
        characteristic,
        discoveryCallback,
        terminationCallback
    );
}

bool GattClient::isCharacteristicDescriptorDiscoveryActive(
    const DiscoveredCharacteristic& characteristic
) const
{
    return impl->isCharacteristicDescriptorDiscoveryActive(characteristic);
}

void GattClient::terminateCharacteristicDescriptorDiscovery(
    const DiscoveredCharacteristic& characteristic
)
{
    return impl->terminateCharacteristicDescriptorDiscovery(characteristic);
}

ble_error_t GattClient::negotiateAttMtu(ble::connection_handle_t connection)
{
    return impl->negotiateAttMtu(connection);
}

void GattClient::onHVX(HVXCallback_t callback)
{
    return impl->onHVX(callback);
}


void GattClient::onShutdown(const GattClientShutdownCallback_t& callback)
{
    return impl->onShutdown(callback);
}

GattClient::GattClientShutdownCallbackChain_t& GattClient::onShutdown()
{
    return impl->onShutdown();
}

GattClient::HVXCallbackChain_t& GattClient::onHVX()
{
    return impl->onHVX();
}

ble_error_t GattClient::reset(void)
{
    return impl->reset();
}

void GattClient::processReadResponse(const GattReadCallbackParams *params)
{
    return impl->processReadResponse(params);
}

void GattClient::processWriteResponse(const GattWriteCallbackParams *params)
{
    return impl->processWriteResponse(params);
}

void GattClient::processHVXEvent(const GattHVXCallbackParams *params)
{
    return impl->processHVXEvent(params);
}

} // namespace ble

