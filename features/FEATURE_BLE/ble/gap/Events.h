/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef BLE_GAP_EVENTS_H
#define BLE_GAP_EVENTS_H

#include "ble/blecommon.h"
#include "ble/BLETypes.h"
#include "platform/Span.h"

/** Event generated when an advertising packet is seen during passive scanning
 *  or a scan response is received during active scanning.
 */
struct AdvertisingReportEvent {
    /** Create a advertising report event.
     *
     * @param type Type of advertising used.
     * @param peerAddressType Peer address type of advertiser.
     * @param peerAddress Peer address of advertiser.
     * @param primaryPhy PHY used on the primary channels.
     * @param secondaryPhy PHY used on secondary channels.
     * @param SID Set identification number.
     * @param txPower Transmission power reported by the packet.
     * @param rssi Measured signal strength.
     * @param periodicInterval Interval of periodic advertising.
     * @param directAddressType Directed advertising target address type.
     * @param directAddress Directed advertising target address.
     * @param advertisingData Advertising payload.
     */
    AdvertisingReportEvent(
        const ble::advertising_event_t &type,
        const ble::peer_address_type_t &peerAddressType,
        const ble::address_t &peerAddress,
        const ble::phy_t &primaryPhy,
        const ble::phy_t &secondaryPhy,
        ble::advertising_sid_t SID,
        ble::advertising_power_t txPower,
        ble::rssi_t rssi,
        ble::periodic_interval_t periodicInterval,
        const ble::peer_address_type_t &directAddressType,
        const ble::address_t &directAddress,
        const mbed::Span<const uint8_t> &advertisingData
    ) :
        type(type),
        peerAddressType(peerAddressType),
        peerAddress(peerAddress),
        primaryPhy(primaryPhy),
        secondaryPhy(secondaryPhy),
        SID(SID),
        txPower(txPower),
        rssi(rssi),
        periodicInterval(periodicInterval),
        directAddressType(directAddressType),
        directAddress(directAddress),
        advertisingData(advertisingData) { }

    /** Get event type. */
    const ble::advertising_event_t &getType() const {
        return type;
    }

    /** Get peer address type. */
    const ble::peer_address_type_t &getPeerAddressType() const {
        return peerAddressType;
    }

    /** Get peer address. */
    const ble::address_t &getPeerAddress() const {
        return peerAddress;
    }

    /** Get primary PHY. */
    const ble::phy_t &getPrimaryPhy() const {
        return primaryPhy;
    }

    /** Get secondary PHY. */
    const ble::phy_t &getSecondaryPhy() const {
        return secondaryPhy;
    }

    /** Get advertising set identifier. */
    ble::advertising_sid_t getSID() const {
        return SID;
    }

    /** Get TX power. */
    ble::advertising_power_t getTxPower() const {
        return txPower;
    }

    /** Get received signal strength. */
    ble::rssi_t getRssi() const {
        return rssi;
    }

    /** Get interval. */
    ble::periodic_interval_t getPeriodicInterval() const {
        return periodicInterval;
    }

    /** Get target address type in directed advertising. */
    const ble::peer_address_type_t &getDirectAddressType() const {
        return directAddressType;
    }

    /** Get target address in directed advertising. */
    const ble::address_t &getDirectAddress() const {
        return directAddress;
    }

    /** Get payload. */
    const mbed::Span<const uint8_t> &getAdvertisingData() const {
        return advertisingData;
    }

private:
    ble::advertising_event_t    type;
    ble::peer_address_type_t         peerAddressType;
    ble::address_t const     &peerAddress;
    ble::phy_t                     primaryPhy;
    ble::phy_t                     secondaryPhy;
    ble::advertising_sid_t    SID;
    ble::advertising_power_t  txPower;
    ble::rssi_t               rssi;
    ble::periodic_interval_t    periodicInterval;
    ble::peer_address_type_t         directAddressType;
    const ble::address_t     &directAddress;
    mbed::Span<const uint8_t> advertisingData;
};

/** Event generated when a connection initiation end (successfully or not).
 */
struct ConnectionCompleteEvent {
    /** Create a connection complete event.
     *
     * @param success BLE_ERROR_NONE if connection succeeded.
     * @param connectionHandle Connection handle if successful.
     * @param ownRole Role of the local device.
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     * @param localResolvablePrivateAddress Local address type if privacy enabled.
     * @param peerResolvablePrivateAddress Peer address type if privacy enabled.
     * @param connectionInterval Connection interval.
     * @param connectionLatency Connection latency in events.
     * @param supervisionTimeout Supervision timeout.
     * @param masterClockAccuracy Peer clock accuracy in parts per million.
     */
    ConnectionCompleteEvent(
        ble_error_t status,
        ble::connection_handle_t connectionHandle,
        ble::connection_role_t ownRole,
        const ble::peer_address_type_t &peerAddressType,
        const ble::address_t &peerAddress,
        const ble::address_t &localResolvablePrivateAddress,
        const ble::address_t &peerResolvablePrivateAddress,
        ble::conn_interval_t connectionInterval,
        ble::slave_latency_t connectionLatency,
        ble::supervision_timeout_t supervisionTimeout,
        uint16_t masterClockAccuracy
    ) :
        status(status),
        connectionHandle(connectionHandle),
        ownRole(ownRole),
        peerAddressType(peerAddressType),
        peerAddress(peerAddress),
        localResolvablePrivateAddress(localResolvablePrivateAddress),
        peerResolvablePrivateAddress(peerResolvablePrivateAddress),
        connectionInterval(connectionInterval),
        connectionLatency(connectionLatency),
        supervisionTimeout(supervisionTimeout),
        masterClockAccuracy(masterClockAccuracy) { }

    /** Get connection complete event status. */
    ble_error_t getStatus() const {
        return status;
    }

    /** Get connection handle (valid only when successful). */
    ble::connection_handle_t getConnectionHandle() const {
        return connectionHandle;
    }

    /** Get own role. */
    ble::connection_role_t getOwnRole() const {
        return ownRole;
    }

    /** Get peer address type. */
    const ble::peer_address_type_t &getPeerAddressType() const {
        return peerAddressType;
    }

    /** Get peer address. */
    const ble::address_t &getPeerAddress() const {
        return peerAddress;
    }

    /** Get get local resolvable random address if privacy is used. */
    const ble::address_t &getLocalResolvablePrivateAddress() const {
        return localResolvablePrivateAddress;
    }

    /** Get peer resolvable private address if privacy is used. */
    const ble::address_t &getPeerResolvablePrivateAddress() const {
        return peerResolvablePrivateAddress;
    }

    /** Get connection interval. */
    ble::conn_interval_t getConnectionInterval() const {
        return connectionInterval;
    }

    /** Get connection latency. */
    ble::slave_latency_t getConnectionLatency() const {
        return connectionLatency;
    }

    /** Get supervision timeout. */
    ble::supervision_timeout_t getSupervisionTimeout() const {
        return supervisionTimeout;
    }

    /** Get clock accuracy in parts per million. */
    uint16_t getMasterClockAccuracy() const {
        return masterClockAccuracy;
    }

private:
    ble_error_t              status;
    ble::connection_handle_t                 connectionHandle;
    ble::connection_role_t                   ownRole;
    ble::peer_address_type_t        peerAddressType;
    const ble::address_t    &peerAddress;
    const ble::address_t    &localResolvablePrivateAddress;
    const ble::address_t    &peerResolvablePrivateAddress;
    ble::conn_interval_t       connectionInterval;
    ble::slave_latency_t       connectionLatency;
    ble::supervision_timeout_t supervisionTimeout;
    uint16_t                 masterClockAccuracy;
};

/** Event generated when we first receive a periodic advertisement.
 */
struct PeriodicAdvertisingSyncEstablishedEvent {
    /** Create advertising sync event.
     *
     * @param success BLE_ERROR_NONE if synchronisation was achieved.
     * @param syncHandle Advertising sync handle.
     * @param sid Advertising set identifier.
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     * @param peerPhy PHY used for advertisements.
     * @param advertisingInterval Periodic advertising interval.
     * @param masterClockAccuracy Peer clock accuracy in parts per million.
     */
    PeriodicAdvertisingSyncEstablishedEvent(
        ble_error_t status,
        ble::periodic_sync_handle_t syncHandle,
        ble::advertising_sid_t sid,
        const ble::peer_address_type_t &peerAddressType,
        const ble::address_t &peerAddress,
        const ble::phy_t &peerPhy,
        uint16_t advertisingInterval,
        const ble::clock_accuracy_t &peerClockAccuracy
    ) :
        status(status),
        syncHandle(syncHandle),
        sid(sid),
        peerAddressType(peerAddressType),
        peerAddress(peerAddress),
        peerPhy(peerPhy),
        advertisingInterval(advertisingInterval),
        peerClockAccuracy(peerClockAccuracy) { }

    /** Get sync establishment status. */
    ble_error_t getStatus() const {
        return status;
    }

    /** Get periodic advertising sync handle. */
    ble::periodic_sync_handle_t getSyncHandle() const {
        return syncHandle;
    }

    /** Get advertising set identifier. */
    ble::advertising_sid_t getSid() const {
        return sid;
    }

    /** Get peer address type. */
    const ble::peer_address_type_t &getPeerAddressType() const {
        return peerAddressType;
    }

    /** Get peer address. */
    const ble::address_t &getPeerAddress() const {
        return peerAddress;
    }

    /** Get PHY used. */
    const ble::phy_t &getPeerPhy() const {
        return peerPhy;
    }

    /** Get interval. */
    uint16_t getAdvertisingInterval() const {
        return advertisingInterval;
    }

    /** Get clock accuracy in parts per million. */
    const ble::clock_accuracy_t &getPeerClockAccuracy() const {
        return peerClockAccuracy;
    }

private:
    ble_error_t status;
    ble::periodic_sync_handle_t syncHandle;
    ble::advertising_sid_t sid;
    ble::peer_address_type_t peerAddressType;
    const ble::address_t& peerAddress;
    ble::phy_t peerPhy;
    uint16_t advertisingInterval;
    ble::clock_accuracy_t peerClockAccuracy;
};

/** Event generated when periodic advertising packet is received.
 */
struct PeriodicAdvertisingReportEvent {
    /** Create periodic advertising report event.
     *
     * @param syncHandle Periodic advertising sync handle
     * @param txPower TX power.
     * @param rssi Received signal strength.
     * @param dataStatus Status to indicate the completeness of the payload.
     * @param payload Periodic advertisement payload.
     */
    PeriodicAdvertisingReportEvent(
        ble::periodic_sync_handle_t syncHandle,
        ble::advertising_power_t txPower,
        ble::rssi_t rssi,
        ble::advertising_data_status_t dataStatus,
        const mbed::Span<const uint8_t> &payload
    ) :
        syncHandle(syncHandle),
        txPower(txPower),
        rssi(rssi),
        dataStatus(dataStatus),
        payload(payload) { }

    /** Get periodic advertising sync handle. */
    ble::periodic_sync_handle_t getSyncHandle() const {
        return syncHandle;
    }

    /** Get TX power as reported by the advertising packet. */
    ble::advertising_power_t getTxPower() const {
        return txPower;
    }

    /** Get received signal strength. */
    ble::rssi_t getRssi() const {
        return rssi;
    }

    /** Get data completeness status. */
    const ble::advertising_data_status_t &getDataStatus() const {
        return dataStatus;
    }

    /** Get payload. */
    const mbed::Span<const uint8_t> &getPayload() const {
        return payload;
    }

private:
    ble::periodic_sync_handle_t syncHandle;
    ble::advertising_power_t txPower;
    ble::rssi_t rssi;
    ble::advertising_data_status_t dataStatus;
    mbed::Span<const uint8_t> payload;
};

/** Event generated when periodic advertising sync is lost.
 */
struct PeriodicAdvertisingSyncLoss {
    /** Create periodic advertising sync loss event.
     *
     * @param syncHandle Periodic advertising sync handle.
     */
    PeriodicAdvertisingSyncLoss(
        ble::periodic_sync_handle_t syncHandle
    ) :
        syncHandle(syncHandle) { }

    /** Get periodic sync handle. */
    ble::periodic_sync_handle_t getSyncHandle() const {
        return syncHandle;
    }

private:
    ble::periodic_sync_handle_t syncHandle;
};

/** Event generated when scan times out.
 */
struct ScanTimeoutEvent { };

/** Event produced when advertising ends.
 */
struct AdvertisingEndEvent_t {
    /** Create advertising end event.
     *
     * @param advHandle Advertising set handle.
     * @param connection Connection handle.
     * @param completed_events Number of events created during before advertising end.
     * @param connected True if connection has been established.
     */
    AdvertisingEndEvent_t(
        ble::advertising_handle_t advHandle,
        ble::connection_handle_t connection,
        uint8_t completed_events,
        bool connected
    ) :
        advHandle(advHandle),
        connection(connection),
        completed_events(completed_events),
        connected(connected) { }

    /** Get advertising handle. */
    ble::advertising_handle_t getAdvHandle() const {
        return advHandle;
    }

    /** Get connection handle (valid only if connected successfully). */
    ble::connection_handle_t getConnection() const {
        return connection;
    }

    /** Get how many events advertising created. */
    uint8_t getCompleted_events() const {
        return completed_events;
    }

    /** Has the advertising ended with a connection. */
    bool isConnected() const {
        return connected;
    }

private:
    ble::advertising_handle_t advHandle;
    ble::connection_handle_t connection;
    uint8_t completed_events;
    bool connected;
};

/** Event produced when a peer requests a scan response from the advertiser.
 */
struct ScanRequestEvent_t {
    /** Create scan request event.
     *
     * @param advHandle Advertising handle.
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     */
    ScanRequestEvent_t(
        ble::advertising_handle_t advHandle,
        const ble::peer_address_type_t &peerAddressType,
        const ble::address_t &peerAddress
    ) :
        advHandle(advHandle),
        peerAddressType(peerAddressType),
        peerAddress(peerAddress) { }

    /** Get advertising handle. */
    ble::advertising_handle_t getAdvHandle() const {
        return advHandle;
    }

    /** Get peer address type. */
    const ble::peer_address_type_t &getPeerAddressType() const {
        return peerAddressType;
    }

    /** Get peer address. */
    const ble::address_t &getPeerAddress() const {
        return peerAddress;
    }

private:
    ble::advertising_handle_t advHandle;
    ble::peer_address_type_t peerAddressType;
    const ble::address_t &peerAddress;
};

#endif //BLE_GAP_EVENTS_H
