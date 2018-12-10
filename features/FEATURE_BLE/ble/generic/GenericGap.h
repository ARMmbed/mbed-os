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

#ifndef MBED_BLE_GENERIC_GAP
#define MBED_BLE_GENERIC_GAP

#include <algorithm>

#include "ble/BLE.h"
#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/PalSecurityManager.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/pal/EventQueue.h"
#include "ble/pal/ConnectionEventMonitor.h"

#include "drivers/Timeout.h"

namespace ble {
namespace generic {
/**
 * Generic implementation of the Gap class.
 * It requires a pal::Gap and a pal::GenericAccessService injected at
 * construction site.
 *
 * @attention: Not part of the public interface of BLE API.
 */
class GenericGap :
    public ::Gap,
    public pal::ConnectionEventMonitor,
    public pal::Gap::EventHandler {

public:
    /* TODO: move to config */
    static const uint8_t MAX_ADVERTISING_SETS = 15;
    static const size_t MAX_HCI_DATA_LENGTH = 251;

    /**
     * Construct a GenericGap.
     *
     * @param event_queue Event queue used to serialise execution.
     *
     * @param pal_gap GAP Platform abstraction instance containing the base GAP
     * primitives.
     *
     * @param generic_access_service Platform abstraction instance managing
     * the GATT generic access service.
     *
     * @param pal_sm Security Manager Platform abstraction instance containing the base
     * Security Manager primitives.
     */
    GenericGap(
        pal::EventQueue &event_queue,
        pal::Gap &pal_gap,
        pal::GenericAccessService &generic_access_service,
        pal::SecurityManager &pal_sm
    );

    /**
     * @see Gap::~Gap
     */
    virtual ~GenericGap();

    /** @copydoc Gap::IsFeatureSupported
     */
    virtual bool isFeatureSupported(
        controller_supported_features_t feature
    );

    /** @copydoc Gap::getMaxAdvertisingSetNumber
     */
    virtual uint8_t getMaxAdvertisingSetNumber();

    /** @copydoc Gap::getMaxAdvertisingDataLength
     */
    virtual uint16_t getMaxAdvertisingDataLength();

    /** @copydoc Gap::createAdvertisingSet
     */
    virtual ble_error_t createAdvertisingSet(
        advertising_handle_t *handle,
        const AdvertisingParameters &parameters
    );

    /** @copydoc Gap::destroyAdvertisingSet
     */
    virtual ble_error_t destroyAdvertisingSet(advertising_handle_t handle);

    /** @copydoc Gap::setAdvertisingParams
     */
    virtual ble_error_t setAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &params
    );

    /** @copydoc Gap::setAdvertisingPayload
     */
    virtual ble_error_t setAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload
    );

    /** @copydoc Gap::setAdvertisingScanResponse
     */
    virtual ble_error_t setAdvertisingScanResponse(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> response
    );

    /** @copydoc Gap::startAdvertising
     */
    virtual ble_error_t startAdvertising(
        advertising_handle_t handle,
        adv_duration_t maxDuration,
        uint8_t maxEvents
    );

    /** @copydoc Gap::stopAdvertising
     */
    virtual ble_error_t stopAdvertising(advertising_handle_t handle);

    /** @copydoc Gap::isAdvertisingActive
     */
    virtual bool isAdvertisingActive(advertising_handle_t handle);

    /** @copydoc Gap::setPeriodicAdvertisingParameters
     */
    virtual ble_error_t setPeriodicAdvertisingParameters(
        advertising_handle_t handle,
        periodic_interval_t periodicAdvertisingIntervalMin,
        periodic_interval_t periodicAdvertisingIntervalMax,
        bool advertiseTxPower
    );

    /** @copydoc Gap::setPeriodicAdvertisingPayload
     */
    virtual ble_error_t setPeriodicAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload
    );

    /** @copydoc Gap::startPeriodicAdvertising
     */
    virtual ble_error_t startPeriodicAdvertising(advertising_handle_t handle);

    /** @copydoc Gap::stopPeriodicAdvertising
     */
    virtual ble_error_t stopPeriodicAdvertising(advertising_handle_t handle);

    /** @copydoc Gap::isPeriodicAdvertisingActive
     */
    virtual bool isPeriodicAdvertisingActive(advertising_handle_t handle);

    /** @copydoc Gap::setScanParameters
     */
    virtual ble_error_t setScanParameters(const ScanParameters &params);

    /** @copydoc Gap::startScan
     */
    virtual ble_error_t startScan(
        scan_duration_t duration,
        duplicates_filter_t filtering,
        scan_period_t period
    );

    /** @copydoc Gap::createSync
     */
    virtual ble_error_t createSync(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid,
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** @copydoc Gap::createSync
     */
    virtual ble_error_t createSync(
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** @copydoc Gap::cancelCreateSync
     */
    virtual ble_error_t cancelCreateSync();

    /** @copydoc Gap::terminateSync
     */
    virtual ble_error_t terminateSync(periodic_sync_handle_t handle);

    /** @copydoc Gap::addDeviceToPeriodicAdvertiserList
     */
    virtual ble_error_t addDeviceToPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid
    );

    /** @copydoc Gap::removeDeviceFromPeriodicAdvertiserList
     */
    virtual ble_error_t removeDeviceFromPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid
    );

    /** @copydoc Gap::clearPeriodicAdvertiserList
     */
    virtual ble_error_t clearPeriodicAdvertiserList();

    /** @copydoc Gap::getMaxPeriodicAdvertiserListSize
     */
    virtual uint8_t getMaxPeriodicAdvertiserListSize();

    /**
     * @see Gap::setAddress
     */
    virtual ble_error_t setAddress(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::getAddress
     */
    virtual ble_error_t getAddress(
        BLEProtocol::AddressType_t *type,
        BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::getMinAdvertisingInterval
     */
    virtual uint16_t getMinAdvertisingInterval() const;

    /**
     * @see Gap::getMinNonConnectableAdvertisingInterval
     */
    virtual uint16_t getMinNonConnectableAdvertisingInterval() const;

    /**
     * @see Gap::getMaxAdvertisingInterval
     */
    virtual uint16_t getMaxAdvertisingInterval() const;

    /**
     * @see Gap::stopAdvertising
     */
    virtual ble_error_t stopAdvertising();

    /**
     * @see Gap::stopScan
     */
    virtual ble_error_t stopScan();

    /**
     * @see Gap::connect
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        PeerAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * @see Gap::connect
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * @see Gap::connect
     */
    virtual ble_error_t connect(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        const ConnectionParameters &connectionParams
    );

    /**
     * @see Gap::cancelConnect
     */
    virtual ble_error_t cancelConnect();

    virtual ble_error_t manageConnectionParametersUpdateRequest(
        bool userManageConnectionUpdateRequest
    );

    virtual ble_error_t updateConnectionParameters(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervisionTimeout,
        conn_event_length_t minConnectionEventLength,
        conn_event_length_t maxConnectionEventLength
    );

    virtual ble_error_t acceptConnectionParametersUpdate(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervisionTimeout,
        conn_event_length_t minConnectionEventLength,
        conn_event_length_t maxConnectionEventLength
    );

    virtual ble_error_t rejectConnectionParametersUpdate(
        connection_handle_t connectionHandle
    );

    /**
     * @see Gap::readPhy
     */
    virtual ble_error_t readPhy(Handle_t connection);

    /**
    * @see Gap::setPreferredPhys
    */
    virtual ble_error_t setPreferredPhys(
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys
    );

    /**
    * @see Gap::setPhy
    */
    virtual ble_error_t setPhy(
        Handle_t connection,
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys,
        CodedSymbolPerBit_t codedSymbol
    );

    virtual ble_error_t disconnect(
        connection_handle_t connectionHandle,
        local_disconnection_reason_t reason
    );

    /**
     * @see Gap::disconnect
     */
    virtual ble_error_t disconnect(
        Handle_t connectionHandle,
        DisconnectionReason_t reason
    );

    /**
     * @see Gap::updateConnectionParams
     */
    virtual ble_error_t updateConnectionParams(
        Handle_t handle,
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::getPreferredConnectionParams
     */
    virtual ble_error_t getPreferredConnectionParams(
        ConnectionParams_t *params
    );

    /**
     * @see Gap::setPreferredConnectionParams
     */
    virtual ble_error_t setPreferredConnectionParams(
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::setDeviceName
     */
    virtual ble_error_t setDeviceName(const uint8_t *deviceName);

    /**
     * @see Gap::getDeviceName
     */
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);

    /**
     * @see Gap::setAppearance
     */
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);

    /**
     * @see Gap::getAppearance
     */
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);

    /**
     * @see Gap::setTxPower
     */
    virtual ble_error_t setTxPower(int8_t txPower);

    /**
     * @see Gap::getPermittedTxPowerValues
     */
    virtual void getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP);

    /**
     * @see Gap::getMaxWhitelistSize
     */
    virtual uint8_t getMaxWhitelistSize(void) const;

    /**
     * @see Gap::getWhitelist
     */
    virtual ble_error_t getWhitelist(Whitelist_t &whitelist) const;

    /**
     * @see Gap::setWhitelist
     */
    virtual ble_error_t setWhitelist(const Whitelist_t &whitelist);

    /**
     * @see Gap::setAdvertisingPolicyMode
     */
    virtual ble_error_t setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode);

    /**
     * @see Gap::setScanningPolicyMode
     */
    virtual ble_error_t setScanningPolicyMode(ScanningPolicyMode_t mode);

    /**
     * @see Gap::setInitiatorPolicyMode
     */
    virtual ble_error_t setInitiatorPolicyMode(InitiatorPolicyMode_t mode);

    /**
     * @see Gap::getAdvertisingPolicyMode
     */
    virtual AdvertisingPolicyMode_t getAdvertisingPolicyMode(void) const;

    /**
     * @see Gap::getScanningPolicyMode
     */
    virtual ScanningPolicyMode_t getScanningPolicyMode(void) const;

    /**
     * @see Gap::getInitiatorPolicyMode
     */
    virtual InitiatorPolicyMode_t getInitiatorPolicyMode(void) const;

    /**
     * @see Gap::startRadioScan
     */
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);

    /**
     * @see Gap::initRadioNotification
     */
    virtual ble_error_t initRadioNotification(void);

    /**
     * @see Gap::enablePrivacy
     */
    virtual ble_error_t enablePrivacy(bool enable);

    /**
     * @see Gap::setPeripheralPrivacyConfiguration
     */
    virtual ble_error_t setPeripheralPrivacyConfiguration(
        const PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getPeripheralPrivacyConfiguration
     */
    virtual ble_error_t getPeripheralPrivacyConfiguration(
        PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setCentralPrivacyConfiguration
     */
    virtual ble_error_t setCentralPrivacyConfiguration(
        const CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getCentralPrivacyConfiguration
     */
    virtual ble_error_t getCentralPrivacyConfiguration(
        CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setAdvertisingData
     */
    virtual ble_error_t setAdvertisingData(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    );

    /**
     * @see Gap::startAdvertising
     */
    virtual ble_error_t startAdvertising(const GapAdvertisingParams &params);

    /**
     * @see Gap::reset
     */
    virtual ble_error_t reset(void);

    /**
     * @copydoc ::Gap::processConnectionEvent
     */
    virtual void processConnectionEvent(
        Handle_t handle,
        Role_t role,
        peer_address_type_t peerAddrType,
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t ownAddrType,
        const BLEProtocol::AddressBytes_t ownAddr,
        const ConnectionParams_t *connectionParams,
        const uint8_t *peerResolvableAddr,
        const uint8_t *localResolvableAddr
    );

    /**
     * @copydoc ::Gap::processDisconnectionEvent
     */
    virtual void processDisconnectionEvent(
        Handle_t handle,
        DisconnectionReason_t reason
    );

private:
    ble_error_t setAdvertisingData(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload,
        bool minimiseFragmentation,
        bool scan_response
    );

    /** @note Implements ConnectionEventMonitor.
     *  @copydoc ConnectionEventMonitor::set_connection_event_handler
     */
    void set_connection_event_handler(pal::ConnectionEventMonitor::EventHandler *_connection_event_handler);

    void on_scan_timeout();

    void process_scan_timeout();

    void on_advertising_timeout();

    void process_advertising_timeout();

    void on_gap_event_received(const pal::GapEvent &e);

    void on_advertising_report(const pal::GapAdvertisingReportEvent &e);

    void on_connection_complete(const pal::GapConnectionCompleteEvent &e);

    void on_disconnection_complete(const pal::GapDisconnectionCompleteEvent &e);

    void on_connection_parameter_request(
        const pal::GapRemoteConnectionParameterRequestEvent &e
    );

    void on_connection_update(const pal::GapConnectionUpdateEvent &e);

    void on_unexpected_error(const pal::GapUnexpectedErrorEvent &e);

    enum AddressUseType_t {
        CENTRAL_CONNECTION,
        CENTRAL_SCAN,
        PERIPHERAL_CONNECTABLE,
        PERIPHERAL_NON_CONNECTABLE
    };

    pal::own_address_type_t get_own_address_type(AddressUseType_t address_use_type);

    bool initialize_whitelist() const;

    ble_error_t update_address_resolution_setting();

    void set_random_address_rotation(bool enable);

    void update_random_address();

    bool getUnresolvableRandomAddress(ble::address_t &address);

    void on_address_rotation_timeout();

    virtual void useVersionOneAPI() const;

    virtual void useVersionTwoAPI() const;

    /* implements pal::Gap::EventHandler */
private:
    virtual void on_read_phy(
        pal::hci_error_code_t hci_status,
        Handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    virtual void on_phy_update_complete(
        pal::hci_error_code_t hci_status,
        Handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    virtual void on_enhanced_connection_complete(
        pal::hci_error_code_t status,
        connection_handle_t connection_handle,
        pal::connection_role_t own_role,
        pal::connection_peer_address_type_t peer_address_type,
        const ble::address_t &peer_address,
        const ble::address_t &local_resolvable_private_address,
        const ble::address_t &peer_resolvable_private_address,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        pal::clock_accuracy_t master_clock_accuracy
    );

    virtual void on_extended_advertising_report(
        advertising_event_t event_type,
        const pal::connection_peer_address_type_t *address_type,
        const ble::address_t &address,
        phy_t primary_phy,
        const phy_t *secondary_phy,
        advertising_sid_t advertising_sid,
        pal::advertising_power_t tx_power,
        pal::rssi_t rssi,
        uint16_t periodic_advertising_interval,
        pal::direct_address_type_t direct_address_type,
        const ble::address_t &direct_address,
        uint8_t data_length,
        const uint8_t *data
    );

    virtual void on_periodic_advertising_sync_established(
        pal::hci_error_code_t error,
        pal::sync_handle_t sync_handle,
        advertising_sid_t advertising_sid,
        pal::connection_peer_address_type_t advertiser_address_type,
        const ble::address_t &advertiser_address,
        phy_t advertiser_phy,
        uint16_t periodic_advertising_interval,
        pal::clock_accuracy_t clock_accuracy
    );

    virtual void on_periodic_advertising_report(
        pal::sync_handle_t sync_handle,
        pal::advertising_power_t tx_power,
        pal::rssi_t rssi,
        pal::advertising_data_status_t data_status,
        uint8_t data_length,
        const uint8_t *data
    );

    virtual void on_periodic_advertising_sync_loss(pal::sync_handle_t sync_handle);

    virtual void on_advertising_set_terminated(
        pal::hci_error_code_t status,
        advertising_handle_t advertising_handle,
        connection_handle_t connection_handle,
        uint8_t number_of_completed_extended_advertising_events
    );

    virtual void on_scan_request_received(
        advertising_handle_t advertising_handle,
        pal::connection_peer_address_type_t scanner_address_type,
        const ble::address_t &address
    );

    virtual void on_connection_update_complete(
        pal::hci_error_code_t status,
        connection_handle_t connection_handle,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

    virtual void on_remote_connection_parameter(
        connection_handle_t connection_handle,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

private:
    pal::EventQueue &_event_queue;
    pal::Gap &_pal_gap;
    pal::GenericAccessService &_gap_service;
    pal::SecurityManager &_pal_sm;
    BLEProtocol::AddressType_t _address_type;
    ble::address_t _address;
    pal::initiator_policy_t _initiator_policy_mode;
    pal::scanning_filter_policy_t _scanning_filter_policy;
    pal::advertising_filter_policy_t _advertising_filter_policy;
    mutable Whitelist_t _whitelist;

    bool _privacy_enabled;
    PeripheralPrivacyConfiguration_t _peripheral_privacy_configuration;
    CentralPrivacyConfiguration_t _central_privacy_configuration;
    ble::address_t _random_static_identity_address;
    bool _random_address_rotating;

    bool _scan_enabled;
    mbed::Timeout _advertising_timeout;
    mbed::Timeout _scan_timeout;
    mbed::Ticker _address_rotation_ticker;
    pal::ConnectionEventMonitor::EventHandler *_connection_event_handler;

    template<size_t bit_size>
    struct BitArray {
        BitArray() : data()
        {
        }

        bool get(size_t index) const
        {
            position p(index);
            return (data[p.byte_index] >> p.bit_index) & 0x01;
        }

        void set(size_t index)
        {
            position p(index);
            data[p.byte_index] |= (0x01 << p.bit_index);
        }

        void clear(size_t index)
        {
            position p(index);
            data[p.byte_index] &= ~(0x01 << p.bit_index);
        }

        void clear()
        {
            for (size_t i = 0; i < (bit_size / 8 + 1); ++i) {
                data[i] = 0;
            }
        }

    private:
        struct position {
            position(size_t bit_number) :
                byte_index(bit_number / 8),
                bit_index(bit_number % 8)
            {
            }

            size_t byte_index;
            uint8_t bit_index;
        };

        uint8_t data[bit_size / 8 + 1];
    };

    BitArray<MAX_ADVERTISING_SETS> _existing_sets;
    BitArray<MAX_ADVERTISING_SETS> _active_sets;
    BitArray<MAX_ADVERTISING_SETS> _active_periodic_sets;

    // deprecation flags
    mutable bool _deprecated_scan_api_used : 1;
    mutable bool _non_deprecated_scan_api_used : 1;
    bool _user_manage_connection_parameter_requests : 1;

private:
    ble_error_t setExtendedAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &parameters
    );

    bool is_extended_advertising_available();
};

}
}

#endif /* MBED_BLE_GENERIC_GAP */
