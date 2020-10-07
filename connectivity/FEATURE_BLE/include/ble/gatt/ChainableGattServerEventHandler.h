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

    virtual ~ChainableGattServerEventHandler() { }

    /**
     * Function invoked when the connections changes the ATT_MTU which controls
     * the maximum size of an attribute that can be read in a single L2CAP packet
     * which might be fragmented across multiple packets.
     *
     * @param connectionHandle The handle of the connection that changed the size.
     * @param attMtuSize
     */
    void onAttMtuChange(ble::connection_handle_t connectionHandle, uint16_t attMtuSize) override {
        execute_on_all(&ble::GattServer::EventHandler::onAttMtuChange,
                connectionHandle, attMtuSize);
    }

    /**
     * Function invoked when the server has sent data to a client as
     * part of a notification/indication.
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     */
    void onDataSent(const GattDataSentCallbackParams* params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataSent, params);
    }

    /**
     * Function invoked when a client writes an attribute
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     */
    void onDataWritten(const GattWriteCallbackParams *params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataWritten, params);
    }

    /**
     * Function invoked when a client reads an attribute
     *
     * @note  This functionality may not be available on all underlying stacks.
     * Application code may work around that limitation by monitoring read
     * requests instead of read events.
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     *
     * @see GattCharacteristic::setReadAuthorizationCallback()
     * @see isOnDataReadAvailable().
     */
    void onDataRead(const GattReadCallbackParams *params) override {
        execute_on_all(&ble::GattServer::EventHandler::onDataRead, params);
    }

    /**
     * Function invoked when the GattServer instance is about
     * to be shut down. This can result in a call to reset() or BLE::reset().
     */
    void onShutdown(const GattServer *server) override {
        execute_on_all(&ble::GattServer::EventHandler::onShutdown, server);
    }

    /**
     * Function invoked when the client has subscribed to characteristic updates
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     */
    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams* params) override {
        execute_on_all(&ble::GattServer::EventHandler::onUpdatesEnabled, params);
    }

    /**
     * Function invoked when the client has unsubscribed to characteristic updates
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     */
    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams* params) override {
        execute_on_all(&ble::GattServer::EventHandler::onUpdatesDisabled, params);
    }

    /**
     * Function invoked when an ACK has been received for an
     * indication sent to the client.
     *
     * @note params has a temporary scope and should be copied by the
     * application if needed later
     */
    void onConfirmationReceived(const GattConfirmationReceivedCallbackParams* params) override {
        execute_on_all(&ble::GattServer::EventHandler::onConfirmationReceived, params);
    }

};

#endif /* MBED_CHAINABLEGATTSERVEREVENTHANDLER_H_ */
