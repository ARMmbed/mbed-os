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

#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/BLETypes.h"

namespace ble {
namespace pal {

/**
 * Implemented by classes that are reacting to connection changes.
 * @see ConnectionEventMonitor
 */
template<class Impl>
class ConnectionEventMonitorEventHandler {
    Impl* self() {
        return static_cast<Impl*>(this);
    }
public:
    /**
     * Inform the Security manager of a new connection. This will create
     * or retrieve an existing security manager entry for the connected device.
     * Called by GAP.
     *
     * @param[in] connection Handle to identify the connection.
     * @param[in] role indicate if the device is central or peripheral.
     * @param[in] peer_address_type type of address.
     * @param[in] peer_address Address of the connected device.
     * @param[in] local_address_type type of address of the local device.
     * @param[in] local_address Address of the local device that was used during connection.
     * @param[in] connection_params connection parameters like interval, latency and timeout.
     * @param[in] resolved_peer_address resolved address of the peer; may
     * be NULL.
     */
    void on_connected(
        connection_handle_t connection,
        ::Gap::Role_t role,
        ble::peer_address_type_t peer_address_type,
        const BLEProtocol::AddressBytes_t peer_address,
        BLEProtocol::AddressType_t local_address_type,
        const BLEProtocol::AddressBytes_t local_address,
        const ::Gap::ConnectionParams_t *connection_params
    ) {
        self()->on_connected_(
            connection,
            role,
            peer_address_type,
            peer_address,
            local_address_type,
            local_address,
            connection_params
        );
    }

    /**
     * Inform the monitor about a disconnection.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] reason Reason for the disconnection.
     */
    void on_disconnected(
        connection_handle_t connection,
        ::Gap::DisconnectionReason_t reason
    ) {
        self()->on_disconnected_(connection, reason);
    }
};


/**
 * Implemented by classes that need to be notified of connection changes.
 * Notification is done by calling functions in the passed in event handler
 */
template<class EventHandler>
class ConnectionEventMonitor {

protected:
    ConnectionEventMonitor() : _connection_event_handler(NULL) { }

    EventHandler* _connection_event_handler;

public:
    /**
     * Register a handler for connection events to be used internally and serviced first.
     *
     * @param[in] connection_event_handler Event handler being registered.
     */
    void set_connection_event_handler(EventHandler *connection_event_handler) {
        _connection_event_handler = connection_event_handler;
    }
};

} // namespace pal
} // namespace ble

#endif /* MBED_BLE_CONNECTION_EVENT_MONITOR */
