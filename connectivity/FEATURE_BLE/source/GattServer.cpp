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

#include "ble/GattServer.h"
#include "source/GattServerImpl.h"

namespace ble {

void GattServer::setEventHandler(EventHandler *handler)
{
    return impl->setEventHandler(handler);
}

ble_error_t GattServer::reset()
{
    return impl->reset(this);
}

ble_error_t GattServer::addService(GattService &service)
{
    return impl->addService(service);
}

ble_error_t GattServer::read(
    GattAttribute::Handle_t attributeHandle,
    uint8_t buffer[],
    uint16_t *lengthP
)
{
    return impl->read(attributeHandle, buffer, lengthP);
}

ble_error_t GattServer::read(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    uint8_t *buffer,
    uint16_t *lengthP
)
{
    return impl->read(connectionHandle, attributeHandle, buffer, lengthP);
}

ble_error_t GattServer::write(
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
)
{
    return impl->write(attributeHandle, value, size, localOnly);
}

ble_error_t GattServer::write(
    ble::connection_handle_t connectionHandle,
    GattAttribute::Handle_t attributeHandle,
    const uint8_t *value,
    uint16_t size,
    bool localOnly
)
{
    return impl->write(connectionHandle, attributeHandle, value, size, localOnly);
}

ble_error_t GattServer::areUpdatesEnabled(
    const GattCharacteristic &characteristic,
    bool *enabledP
)
{
    return impl->areUpdatesEnabled(characteristic, enabledP);
}

ble_error_t GattServer::areUpdatesEnabled(
    ble::connection_handle_t connectionHandle,
    const GattCharacteristic &characteristic,
    bool *enabledP
)
{
    return impl->areUpdatesEnabled(connectionHandle, characteristic, enabledP);
}

bool GattServer::isOnDataReadAvailable() const
{
    return impl->isOnDataReadAvailable();
}

void GattServer::onDataSent(const DataSentCallback_t &callback)
{
    return impl->onDataSent(callback);
}

GattServer::DataSentCallbackChain_t &GattServer::onDataSent()
{
    return impl->onDataSent();
}

void GattServer::onDataWritten(const DataWrittenCallback_t &callback)
{
    return impl->onDataWritten(callback);
}

GattServer::DataWrittenCallbackChain_t &GattServer::onDataWritten()
{
    return impl->onDataWritten();
}

ble_error_t GattServer::onDataRead(const DataReadCallback_t &callback)
{
    return impl->onDataRead(callback);
}

GattServer::DataReadCallbackChain_t &GattServer::onDataRead()
{
    return impl->onDataRead();
}

void GattServer::onShutdown(const GattServerShutdownCallback_t &callback)
{
    return impl->onShutdown(callback);
}

GattServer::GattServerShutdownCallbackChain_t& GattServer::onShutdown()
{
    return impl->onShutdown();
}

void GattServer::onUpdatesEnabled(EventCallback_t callback)
{
    impl->onUpdatesEnabled(callback);
}

void GattServer::onUpdatesDisabled(EventCallback_t callback)
{
    impl->onUpdatesDisabled(callback);
}

void GattServer::onConfirmationReceived(EventCallback_t callback)
{
    impl->onConfirmationReceived(callback);
}

} // ble
