/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef MBED_CHAINABLEGATTSERVEREVENTHANDLER_H_
#define MBED_CHAINABLEGATTSERVEREVENTHANDLER_H_

#include "ble/GattServer.h"
#include "ble/common/ChainableEventHandler.h"

/**
 * GattServer::EventHandler implementation that allows the application
 * to register multiple separate EventHandlers to be called when
 * GattServer events happen.
 */
class ChainableGattServerEventHandler : public ble::GattServer::EventHandler,
                                        public ChainableEventHandler<ble::GattServer::EventHandler>
{

public:

    ChainableGattServerEventHandler() { }

    ~ChainableGattServerEventHandler() { }

    void onAttMtuChange(ble::connection_handle_t connectionHandle, uint16_t attMtuSize) override {
        execute_on_all(&ble::GattServer::EventHandler::onAttMtuChange,
                connectionHandle, attMtuSize);
    }

    void onDataSent(const GattDataSentCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataSent, params);
    }

    void onDataWritten(const GattWriteCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataWritten, params);
    }

    void onDataRead(const GattReadCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataRead, params);
    }

    void onShutdown(const GattServer &server) override {
        execute_on_all(&ble::GattServer::EventHandler::onShutdown, server);
    }

    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onUpdatesEnabled, params);
    }

    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onUpdatesDisabled, params);
    }

    void onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params) override {
        execute_on_all(&ble::GattServer::EventHandler::onConfirmationReceived, params);
    }

};

#endif /* MBED_CHAINABLEGATTSERVEREVENTHANDLER_H_ */
