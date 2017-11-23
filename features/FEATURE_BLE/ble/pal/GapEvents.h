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

#ifndef BLE_PAL_GAP_MESSAGE_H_
#define BLE_PAL_GAP_MESSAGE_H_

#include "GapTypes.h"
#include "ble/ArrayView.h"

namespace ble {
namespace pal {

/**
 * Enumeration of GAP event types
 */
struct GapEventType : SafeEnum<GapEventType, uint8_t> {
    enum type {
        /**
         * Event type used by GapUnexpectedErrorEvent
         */
        UNEXPECTED_ERROR,

        /**
         * Event type used by GapConnectionCompleteEvent
         */
        CONNECTION_COMPLETE,

        /**
         * Event type used by GapAdvertisingReportEvent
         */
        ADVERTISING_REPORT,

        /**
         * Event type used by GapConnectionUpdateEvent
         * */
        CONNECTION_UPDATE,

        /**
         * Event type used by GapRemoteConnectionParameterRequestEvent
         */
        REMOTE_CONNECTION_PARAMETER_REQUEST,

        /**
         * Event type used by GapDisconnectionCompleteEvent
         */
        DISCONNECTION_COMPLETE
    };

    GapEventType(type event_type) : SafeEnum<GapEventType, uint8_t>(event_type) { }
};


/**
 * Base class of a Gap Event.
 *
 * Client should use the field type to deduce the actual type of the event.
 */
struct GapEvent {

    const GapEventType type;

protected:
    GapEvent(GapEventType type) : type(type) { }

    // Disable copy construction and copy assignement operations.
    GapEvent(const GapEvent&);
    GapEvent& operator=(const GapEvent&);
};


/**
 * Model an unexpected error that happen during a gap procedure.
 *
 * This class is mainly used to notify user code of an unexpected error returned
 * in an HCI command complete event.
 */
struct GapUnexpectedErrorEvent : public GapEvent {
    GapUnexpectedErrorEvent(uint16_t opcode, uint8_t error_code) :
        GapEvent(GapEventType::UNEXPECTED_ERROR),
        opcode(opcode), error_code(error_code) { }

    /**
     * Opcode composed of the OCF and OGF of the command which has returned an
     * error.
     */
    const uint16_t opcode;

    /**
     * Error code
     */
    const uint8_t error_code;
};


/**
 * Indicate to both ends (slave or master) the end of the connection process.
 *
 * This structure should be used for Connection Complete Events and Enhanced
 * Connection Complete Event.
 *
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.1 LE Connection Complete Event
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.10 LE Enhanced Connection
 */
struct GapConnectionCompleteEvent : public GapEvent {
    /**
     * Construct a new GapConnectionCompleteEvent.
     *
     * @param status Status of the operation: 0x00 in case of success otherwise
     * the error code associated with the failure.
     *
     * @param connection_handle handle of the connection created. This handle
     * will be used to address the connection in any connection oriented
     * operation.
     *
     * @param role Role of the LE subsystem in the connection.
     *
     * @param address_type Type of address used by the peer for this connection.
     *
     * @param address Address of the peer used to establish the connection.
     *
     * @param connection_interval Connection interval used on this connection.
     * It shall be in a range [0x0006 : 0x0C80]. A unit is equal to 1.25ms.
     *
     * @param connection_latency Number of connection events the slave can
     * drop.
     *
     * @param supervision_timeout Supervision timeout of the connection. It
     * shall be in the range [0x000A : 0x0C80] where a unit represent 10ms.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.1 LE Connection Complete Event
     * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.10 LE Enhanced Connection
     * Complete Event
     */
    GapConnectionCompleteEvent(
        uint8_t status,
        connection_handle_t connection_handle,
        connection_role_t role,
        advertising_peer_address_type_t peer_address_type,
        const address_t& peer_address,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    ) :
        GapEvent(GapEventType::CONNECTION_COMPLETE),
        status(status),
        connection_handle(connection_handle),
        role(role),
        peer_address_type(peer_address_type),
        peer_address(peer_address),
        connection_interval(connection_interval),
        connection_latency(connection_latency),
        supervision_timeout(supervision_timeout) {
    }

    /*
     * @param status Indicate if the connection succesfully completed or not:
     *   - 0: Connection successfuly completed
     *   - [0x01 : 0xFF] Connection failed to complete, the value represent
     *     the code for the error.
     */
    const uint8_t status;

    /**
     * Handle of the connection created, valid if status is equal to 0.
     * @important Valid if status is equal to 0.
     */
    const connection_handle_t connection_handle;

    /**
     * Role of the device in the connection
     * @important Valid if status is equal to 0.
     */
    const connection_role_t role;

    /**
     * Peer address type.
     */
    const advertising_peer_address_type_t peer_address_type;

    /**
     * Peer address.
     */
    const address_t peer_address;

    /**
     * Connection interval used in this connection.
     * It shall be in a range [0x0006 : 0x0C80]. A unit is equal to 1.25ms.
     */
    const uint16_t connection_interval;

    /**
     * Number of connection events the slave can drop.
     */
    const uint16_t connection_latency;

    /**
     * Supervision timeout of the connection
     * It shall be in the range [0x000A : 0x0C80] where a unit represent 10ms.
     */
    const uint16_t supervision_timeout;
};


/**
 * Report advertising from one or more LE device.
 *
 * @important This class has to be implemented by the BLE port.
 *
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.2 LE Advertising Report Event
 */
struct GapAdvertisingReportEvent : public GapEvent {

    /**
     * POD representing an advertising captured by the LE subsystem.
     */
    struct advertising_t {
        received_advertising_type_t type;
        connection_peer_address_type_t address_type;
        const address_t& address;
        ArrayView<const uint8_t> data;
        int8_t rssi;
    };

    GapAdvertisingReportEvent() : GapEvent(GapEventType::ADVERTISING_REPORT) { }

    virtual ~GapAdvertisingReportEvent() { }

    /**
     * Count of advertising in this event.
     */
    virtual uint8_t size() const = 0;

    /**
     * Access the advertising at index i.
     */
    virtual advertising_t operator[](uint8_t i) const = 0;
};


/**
 * Indicates the connection update process completion.
 *
 * If no parameters are updated after a connection update request from the peer
 * then this event shall not be emmited.
 *
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.3 LE Connection Update Complete
 * Event.
 */
struct GapConnectionUpdateEvent : public GapEvent {

    /**
     * Construct a connection update event for a successful process.
     *
     * @param status Status of the connection update event operation. If equal
     * to 0x00 then the process was successful, otherwise the status indicates
     * the reason of the faillure.
     *
     * @param connection_handle Handle of the connection updated.
     *
     * @param connection_interval New connection interval used by the connection.
     *
     * @param Connection_latency New connection latency used by the connection.
     *
     * @param supervision_timeout New connection supervision timeout.
     */
    GapConnectionUpdateEvent(
        uint8_t status,
        connection_handle_t connection_handle,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    ) :
        GapEvent(GapEventType::CONNECTION_UPDATE),
        status(status),
        connection_handle(connection_handle),
        connection_interval(connection_interval),
        connection_latency(connection_latency),
        supervision_timeout(supervision_timeout) {
    }

    /**
     * If equal to 0, the connection update has succesfully completed otherwise
     * the process has failled and this field represent the error associated to
     * the faillure.
     */
    const uint8_t status;

    /**
     * Handle of the connection which has completed the connection update
     * process.
     */
    const connection_handle_t connection_handle;

    /**
     * New connection interval used by the connection.
     * It shall be in a range [0x0006 : 0x0C80]. A unit is equal to 1.25ms.
     */
    const uint16_t connection_interval;

    /*
     * New number of connection events the slave can drop.
     */
    const uint16_t connection_latency;

    /*
     * New supervision timeout of the connection.
     * It shall be in the range [0x000A : 0x0C80] where a unit represent 10ms.
     */
    const uint16_t supervision_timeout;
};


/**
 * indicate a request from the peer to change the connection parameters.
 *
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.65.6 LE Remote Connection Parameter
 * Request Event.
 */
struct GapRemoteConnectionParameterRequestEvent : public GapEvent {
    /**
     * Construct a new remote connection parameter request event.
     *
     * @param connection_handle Handle of the connection with the peer
     * requesting the parameter update
     *
     * @param min_connection_interval Minimum value of the connection interval
     * requested by the peer.
     *
     * @param max_connection_interval Maximum value of the connection interval
     * requested by the peer.
     *
     * @param connection_latency Slave latency requested by the peer.
     *
     * @param supervision_timeout Supervision timeout requested by the peer.
     */
    GapRemoteConnectionParameterRequestEvent(
        connection_handle_t connection_handle,
        uint16_t min_connection_interval,
        uint16_t max_connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    ) : GapEvent(GapEventType::REMOTE_CONNECTION_PARAMETER_REQUEST),
        connection_handle(connection_handle),
        min_connection_interval(min_connection_interval),
        max_connection_interval(max_connection_interval),
        connection_latency(connection_latency),
        supervision_timeout(supervision_timeout) {
    }

    /**
     * Handle of the connection with the peer requesting the parameter update.
     */
    const connection_handle_t connection_handle;

    /**
     * Minimum value of the connection interval requested by the peer.
     * It shall be in a range [0x0006 : 0x0C80]. A unit is equal to 1.25ms.
     */
    const uint16_t min_connection_interval;

    /**
     * Maximum value of the connection interval requested by the peer.
     * It shall be in a range [0x0006 : 0x0C80]. A unit is equal to 1.25ms.
     */
    const uint16_t max_connection_interval;

    /*
     * Slave latency requested by the peer.
     */
    const uint16_t connection_latency;

    /*
     * Supervision timeout requested by the peer.
     * It shall be in the range [0x000A : 0x0C80] where a unit represent 10ms.
     */
    const uint16_t supervision_timeout;
};


/**
 * Indicate the end of a disconnection process.
 *
 * @note: See Bluetooth 5 Vol 2 PartE: 7.7.5 Disconnection Complete Event.
 */
struct GapDisconnectionCompleteEvent : public GapEvent {
    /**
     * Construct a disconnection complete event.
     *
     * @param status Status of the procedure. If equal to 0 then the
     * disconnection process complete successfully. Otherwise it represents the
     * error code associated with the faillure.
     *
     * @param connection_handle Handle of the connection disconnected.
     *
     * @param reason Reason of the disconnection
     */
    GapDisconnectionCompleteEvent(
        uint8_t status,
        connection_handle_t connection_handle,
        uint8_t reason
    ) : GapEvent(GapEventType::DISCONNECTION_COMPLETE),
        status(status),
        connection_handle(connection_handle),
        reason(reason) {
    }

    /**
     * Status of the procedure. If equal to 0 then the procedure was a success
     * otherwise this variable contains the error code associated with the
     * faillure.
     */
    const uint8_t status;

    /**
     * Handle of the connection used for the procedure.
     */
    const connection_handle_t connection_handle;

    /**
     * Reason for disconnection.
     *
     * @important ignored in case of faillure.
     */
    const uint8_t reason;
};

} // namespace pal
} // namespace ble

#endif /* BLE_PAL_GAP_MESSAGE_H_ */
