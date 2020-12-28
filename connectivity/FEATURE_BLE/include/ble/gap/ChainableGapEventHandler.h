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

#ifndef MBED_CHAINABLEGAPEVENTHANDLER_H_
#define MBED_CHAINABLEGAPEVENTHANDLER_H_

#include "ble/Gap.h"
#include "ble/common/ChainableEventHandler.h"

/**
 * Gap::EventHandler implementation that allows the application
 * to register multiple separate EventHandlers to be called when
 * Gap events happen.
 */
class ChainableGapEventHandler : public ble::Gap::EventHandler,
                                 public ChainableEventHandler<ble::Gap::EventHandler>
{

public:

    ChainableGapEventHandler() { }

    ~ChainableGapEventHandler() { }

    void onScanRequestReceived(const ble::ScanRequestEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onScanRequestReceived, event);
    }

    void onAdvertisingStart(const ble::AdvertisingStartEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onAdvertisingStart, event);
    }

    void onAdvertisingEnd(const ble::AdvertisingEndEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onAdvertisingEnd, event);
    }
    
    void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onAdvertisingReport, event);
    }

    void onScanTimeout(const ble::ScanTimeoutEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onScanTimeout, event);
    }

    void onPeriodicAdvertisingSyncEstablished(
        const ble::PeriodicAdvertisingSyncEstablishedEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onPeriodicAdvertisingSyncEstablished, event);
    }
    
    void onPeriodicAdvertisingReport(
        const ble::PeriodicAdvertisingReportEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onPeriodicAdvertisingReport, event);
    }

    void onPeriodicAdvertisingSyncLoss(
        const ble::PeriodicAdvertisingSyncLoss &event) override {
        execute_on_all(&ble::Gap::EventHandler::onPeriodicAdvertisingSyncLoss, event);
    }

    void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onConnectionComplete, event);
    }

    void onUpdateConnectionParametersRequest(
        const ble::UpdateConnectionParametersRequestEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onUpdateConnectionParametersRequest, event);
    }

    void onConnectionParametersUpdateComplete(
        const ble::ConnectionParametersUpdateCompleteEvent &event) override {
        execute_on_all(&ble::Gap::EventHandler::onConnectionParametersUpdateComplete, event);
    }

    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override {
       execute_on_all(&ble::Gap::EventHandler::onDisconnectionComplete, event);
    }

    void onReadPhy(
        ble_error_t status,
        ble::connection_handle_t connectionHandle,
        ble::phy_t txPhy,
        ble::phy_t rxPhy) override {
        execute_on_all(&ble::Gap::EventHandler::onReadPhy, status,
                connectionHandle, txPhy, rxPhy);
    }

    void onPhyUpdateComplete(
        ble_error_t status,
        ble::connection_handle_t connectionHandle,
        ble::phy_t txPhy,
        ble::phy_t rxPhy) override {
        execute_on_all(&ble::Gap::EventHandler::onPhyUpdateComplete, status,
                connectionHandle, txPhy, rxPhy);
    }

    void onDataLengthChange(
        ble::connection_handle_t connectionHandle,
        uint16_t txSize,
        uint16_t rxSize) override {
        execute_on_all(&ble::Gap::EventHandler::onDataLengthChange,
                connectionHandle, txSize, rxSize);
    }

    void onPrivacyEnabled() override {
        execute_on_all(&ble::Gap::EventHandler::onPrivacyEnabled);
    }

};

#endif /* MBED_CHAINABLEGAPEVENTHANDLER_H_ */
