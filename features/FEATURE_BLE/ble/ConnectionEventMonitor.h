/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef MBED_BLE_CONNECTION_EVENT_MONITOR
#define MBED_BLE_CONNECTION_EVENT_MONITOR

#include <algorithm>

#include "ble/BLE.h"
#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/pal/EventQueue.h"

namespace ble {

class ConnectionEventHandler {
public:
    virtual void on_connected(
        connection_handle_t connection,
        Gap::Role_t role,
        BLEProtocol::AddressType_t peer_address_type,
        const BLEProtocol::AddressBytes_t peer_address,
        BLEProtocol::AddressType_t local_address_type,
        const BLEProtocol::AddressBytes_t local_address,
        const Gap::ConnectionParams_t *connection_params
    ) = 0;

    virtual void on_disconnected(
        connection_handle_t connection,
        Gap::DisconnectionReason_t reason
    ) = 0;
};

class ConnectionEventMonitor {
public:
    /**
     * Register a handler for connection events to be used internally and serviced first.
     *
     * @param[in] connection_event_handler Event handler being registered.
     */
    virtual void set_connection_event_handler(ConnectionEventHandler *connection_event_handler) = 0;
};

}

#endif /* MBED_BLE_CONNECTION_EVENT_MONITOR */
