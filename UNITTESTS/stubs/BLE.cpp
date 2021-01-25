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

#include "ble/BLE.h"
#include "GattServerImpl_mock.h"
#include "GattClientImpl_mock.h"
#include "GapImpl_mock.h"
#include "SecurityManagerImpl_mock.h"
#include "ble/GattClient.h"
#include "ble/GattServer.h"
#include "ble/SecurityManager.h"
#include "ble/Gap.h"

namespace ble {

class BLEInstanceBase {
};

BLE::BLE(ble::BLEInstanceBase &transport) : transport(transport)
{
}

BLE& BLE::Instance()
{
    static ble::BLEInstanceBase transport;
    static BLE instance(transport);
    return instance;
}

ble::Gap &BLE::gap()
{
    static GapMock impl;
    static ble::Gap gap(&impl);
    return gap;
}

const ble::Gap &BLE::gap() const
{
    auto &self = const_cast<BLE &>(*this);
    return const_cast<const ble::Gap &>(self.gap());
}

ble::GattServer &BLE::gattServer()
{
    static GattServerMock impl;
    static ble::GattServer server(&impl);
    return server;
}

const ble::GattServer &BLE::gattServer() const
{
    auto &self = const_cast<BLE &>(*this);
    return const_cast<const ble::GattServer &>(self.gattServer());
}

ble::GattClient &BLE::gattClient()
{
    static GattClientMock impl;
    static ble::GattClient  gatt_client(&impl);
    return gatt_client;
}

const ble::GattClient &BLE::gattClient() const
{
    auto &self = const_cast<BLE &>(*this);
    return const_cast<const ble::GattClient &>(self.gattClient());
}

ble::SecurityManager &BLE::securityManager()
{
    static SecurityManagerMock impl;
    static ble::SecurityManager sm(&impl);
    return sm;
}

const ble::SecurityManager &BLE::securityManager() const
{
    auto &self = const_cast<BLE &>(*this);
    return const_cast<const ble::SecurityManager &>(self.securityManager());
}

void BLE::processEvents()
{

}

}