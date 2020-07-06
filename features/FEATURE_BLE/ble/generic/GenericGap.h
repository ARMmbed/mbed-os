/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef MBED_BLE_GENERIC_GAP
#define MBED_BLE_GENERIC_GAP

#include <algorithm>

#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/pal/EventQueue.h"
#include "ble/pal/ConnectionEventMonitor.h"

#include "drivers/LowPowerTimeout.h"
#include "drivers/LowPowerTicker.h"
#include "platform/mbed_error.h"

namespace ble {
namespace generic {
/**
 * Generic implementation of the Gap class.
 * It requires a pal::Gap and a pal::GenericAccessService injected at
 * construction site.
 *
 * @attention: Not part of the public interface of BLE API.
 */
template<
    template<class> class TPalGap,
    class PalSecurityManager,
    class ConnectionEventMonitorEventHandler
>
class GenericGap :
    public interface::Gap<
        GenericGap<TPalGap, PalSecurityManager, ConnectionEventMonitorEventHandler>
    >,
    public pal::ConnectionEventMonitor<
        ConnectionEventMonitorEventHandler
    >,
    public pal::GapEventHandler<
        GenericGap<TPalGap, PalSecurityManager, ConnectionEventMonitorEventHandler>
    >
{
    // Typedef of base and companion classes .
    typedef ::ble::interface::Gap<GenericGap> Gap;
    typedef pal::ConnectionEventMonitor<ConnectionEventMonitorEventHandler> ConnectionEventMonitor;
    typedef TPalGap<GenericGap> PalGap;
    typedef pal::GapEventHandler<GenericGap> PalGapEventHandler;

    // Friendship with base classes
    friend Gap;
    friend ConnectionEventMonitor;
    friend pal::GapEventHandler<GenericGap>;
    friend PalGap;

    // Imports from LegacyGap
    using Gap::_eventHandler;
    using Gap::default_peripheral_privacy_configuration;
    using Gap::default_central_privacy_configuration;

    typedef typename ble::whitelist_t whitelist_t;
    typedef typename Gap::PreferredConnectionParams_t PreferredConnectionParams_t;

    // Imports from Gap
#if BLE_ROLE_BROADCASTER
    using ble::interface::Gap<GenericGap>::getMaxAdvertisingSetNumber;
    using ble::interface::Gap<GenericGap>::getMaxAdvertisingDataLength;
#endif // BLE_ROLE_BROADCASTER
    using ble::interface::Gap<GenericGap>::isFeatureSupported;

    // Imports from PalGap EventHandler
    using PalGapEventHandler::on_scan_timeout;

    // Imports from ConnectionEventMonitor
    using ConnectionEventMonitor::_connection_event_handler;
    using ConnectionEventMonitor::set_connection_event_handler;

public:
    /* TODO: move to config */
    static const uint8_t MAX_ADVERTISING_SETS = 15;

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
        PalGap &pal_gap,
        pal::GenericAccessService &generic_access_service,
        PalSecurityManager &pal_sm
    );

    /**
     * @see Gap::~Gap
     */
    ~GenericGap();

    /** @copydoc Gap::IsFeatureSupported
     */
    bool isFeatureSupported_(
        controller_supported_features_t feature
    );

    /** @copydoc Gap::getMaxAdvertisingSetNumber
     */
    uint8_t getMaxAdvertisingSetNumber_();

    /** @copydoc Gap::getMaxAdvertisingDataLength
     */
    uint16_t getMaxAdvertisingDataLength_();

    /** @copydoc Gap::getMaxConnectableAdvertisingDataLength
     */
    uint16_t getMaxConnectableAdvertisingDataLength_();

    /** @copydoc Gap::getMaxActiveSetAdvertisingDataLength
     */
    uint16_t getMaxActiveSetAdvertisingDataLength_();

    /** @copydoc Gap::createAdvertisingSet
     */
    ble_error_t createAdvertisingSet_(
        advertising_handle_t *handle,
        const AdvertisingParameters &parameters
    );

    /** @copydoc Gap::destroyAdvertisingSet
     */
    ble_error_t destroyAdvertisingSet_(advertising_handle_t handle);

    /** @copydoc Gap::setAdvertisingParams
     */
    ble_error_t setAdvertisingParameters_(
        advertising_handle_t handle,
        const AdvertisingParameters &params
    );

    /** @copydoc Gap::setAdvertisingPayload
     */
    ble_error_t setAdvertisingPayload_(
        advertising_handle_t handle,
        Span<const uint8_t> payload
    );

    /** @copydoc Gap::setAdvertisingScanResponse
     */
    ble_error_t setAdvertisingScanResponse_(
        advertising_handle_t handle,
        Span<const uint8_t> response
    );

    /** @copydoc Gap::startAdvertising
     */
    ble_error_t startAdvertising_(
        advertising_handle_t handle,
        adv_duration_t maxDuration,
        uint8_t maxEvents
    );

    /** @copydoc Gap::stopAdvertising
     */
    ble_error_t stopAdvertising_(advertising_handle_t handle);

    /** @copydoc Gap::isAdvertisingActive
     */
    bool isAdvertisingActive_(advertising_handle_t handle);

    /** @copydoc Gap::setPeriodicAdvertisingParameters
     */
    ble_error_t setPeriodicAdvertisingParameters_(
        advertising_handle_t handle,
        periodic_interval_t periodicAdvertisingIntervalMin,
        periodic_interval_t periodicAdvertisingIntervalMax,
        bool advertiseTxPower
    );

    /** @copydoc Gap::setPeriodicAdvertisingPayload
     */
    ble_error_t setPeriodicAdvertisingPayload_(
        advertising_handle_t handle,
        Span<const uint8_t> payload
    );

    /** @copydoc Gap::startPeriodicAdvertising
     */
    ble_error_t startPeriodicAdvertising_(advertising_handle_t handle);

    /** @copydoc Gap::stopPeriodicAdvertising
     */
    ble_error_t stopPeriodicAdvertising_(advertising_handle_t handle);

    /** @copydoc Gap::isPeriodicAdvertisingActive
     */
    bool isPeriodicAdvertisingActive_(advertising_handle_t handle);

    /** @copydoc Gap::setScanParameters
     */
    ble_error_t setScanParameters_(const ScanParameters &params);

    /** @copydoc Gap::startScan
     */
    ble_error_t startScan_(
        scan_duration_t duration,
        duplicates_filter_t filtering,
        scan_period_t period
    );

    /** @copydoc Gap::createSync
     */
    ble_error_t createSync_(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid,
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** @copydoc Gap::createSync
     */
    ble_error_t createSync_(
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** @copydoc Gap::cancelCreateSync
     */
    ble_error_t cancelCreateSync_();

    /** @copydoc Gap::terminateSync
     */
    ble_error_t terminateSync_(periodic_sync_handle_t handle);

    /** @copydoc Gap::addDeviceToPeriodicAdvertiserList
     */
    ble_error_t addDeviceToPeriodicAdvertiserList_(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid
    );

    /** @copydoc Gap::removeDeviceFromPeriodicAdvertiserList
     */
    ble_error_t removeDeviceFromPeriodicAdvertiserList_(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        advertising_sid_t sid
    );

    /** @copydoc Gap::clearPeriodicAdvertiserList
     */
    ble_error_t clearPeriodicAdvertiserList_();

    /** @copydoc Gap::getMaxPeriodicAdvertiserListSize
     */
    uint8_t getMaxPeriodicAdvertiserListSize_();

    /**
     * @see Gap::setRandomStaticAddress
     */
    ble_error_t setRandomStaticAddress_(const ble::address_t& address);

    /**
     * @see Gap::getAddress
     */
    ble_error_t getAddress_(
        own_address_type_t &type,
        address_t &address
    );

    /**
     * @see Gap::stopScan
     */
    ble_error_t stopScan_();

    /**
     * @see Gap::connect
     */
    ble_error_t connect_(
        peer_address_type_t peerAddressType,
        const ble::address_t &peerAddress,
        const ConnectionParameters &connectionParams
    );

    /**
     * @see Gap::cancelConnect
     */
    ble_error_t cancelConnect_();

    ble_error_t manageConnectionParametersUpdateRequest_(
        bool userManageConnectionUpdateRequest
    );

    ble_error_t updateConnectionParameters_(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervisionTimeout,
        conn_event_length_t minConnectionEventLength,
        conn_event_length_t maxConnectionEventLength
    );

    ble_error_t acceptConnectionParametersUpdate_(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervisionTimeout,
        conn_event_length_t minConnectionEventLength,
        conn_event_length_t maxConnectionEventLength
    );

    ble_error_t rejectConnectionParametersUpdate_(
        connection_handle_t connectionHandle
    );

    /**
     * @see Gap::readPhy
     */
    ble_error_t readPhy_(connection_handle_t connection);

    /**
    * @see Gap::setPreferredPhys
    */
    ble_error_t setPreferredPhys_(
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys
    );

    /**
    * @see Gap::setPhy
    */
    ble_error_t setPhy_(
        connection_handle_t connection,
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys,
        ble::coded_symbol_per_bit_t codedSymbol
    );

    ble_error_t disconnect_(
        connection_handle_t connectionHandle,
        local_disconnection_reason_t reason
    );

    /**
     * @see Gap::getMaxWhitelistSize
     */
    uint8_t getMaxWhitelistSize_(void) const;

    /**
     * @see Gap::getWhitelist
     */
    ble_error_t getWhitelist_(whitelist_t &whitelist) const;

    /**
     * @see Gap::setWhitelist
     */
    ble_error_t setWhitelist_(const whitelist_t &whitelist);

    /**
     * @see Gap::enablePrivacy
     */
    ble_error_t enablePrivacy_(bool enable);


    ble_error_t setPeripheralPrivacyConfiguration_(
        const peripheral_privacy_configuration_t *configuration
    );
    ble_error_t getPeripheralPrivacyConfiguration_(
        peripheral_privacy_configuration_t *configuration
    );
    ble_error_t setCentralPrivacyConfiguration_(
        const central_privacy_configuration_t *configuration
    );
    ble_error_t getCentralPrivacyConfiguration_(
        central_privacy_configuration_t *configuration
    );

    ble_error_t getAddress(
        own_address_type_t &typeP,
        address_t &address
    );

    static ble_error_t getRandomAddressType(
        const ble::address_t address,
        ble::random_address_type_t *addressType
    );

    /**
     * @see Gap::reset
     */
    ble_error_t reset_(void);


private:
    ble_error_t setAdvertisingData(
        advertising_handle_t handle,
        Span<const uint8_t> payload,
        bool minimiseFragmentation,
        bool scan_response
    );

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

    /* implements pal::Gap::EventHandler */
private:
    void on_read_phy_(
        pal::hci_error_code_t hci_status,
        connection_handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    void on_data_length_change_(
        connection_handle_t connection_handle,
        uint16_t tx_size,
        uint16_t rx_size
    );

    void on_phy_update_complete_(
        pal::hci_error_code_t hci_status,
        connection_handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    void on_enhanced_connection_complete_(
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

    void on_extended_advertising_report_(
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

    void on_periodic_advertising_sync_established_(
        pal::hci_error_code_t error,
        pal::sync_handle_t sync_handle,
        advertising_sid_t advertising_sid,
        pal::connection_peer_address_type_t advertiser_address_type,
        const ble::address_t &advertiser_address,
        phy_t advertiser_phy,
        uint16_t periodic_advertising_interval,
        pal::clock_accuracy_t clock_accuracy
    );

    void on_periodic_advertising_report_(
        pal::sync_handle_t sync_handle,
        pal::advertising_power_t tx_power,
        pal::rssi_t rssi,
        pal::advertising_data_status_t data_status,
        uint8_t data_length,
        const uint8_t *data
    );

    void on_periodic_advertising_sync_loss_(pal::sync_handle_t sync_handle);

    void on_advertising_set_terminated_(
        pal::hci_error_code_t status,
        advertising_handle_t advertising_handle,
        connection_handle_t connection_handle,
        uint8_t number_of_completed_extended_advertising_events
    );

    void on_scan_request_received_(
        advertising_handle_t advertising_handle,
        pal::connection_peer_address_type_t scanner_address_type,
        const ble::address_t &address
    );

    void on_connection_update_complete_(
        pal::hci_error_code_t status,
        connection_handle_t connection_handle,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

    void on_remote_connection_parameter_(
        connection_handle_t connection_handle,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

    void on_scan_timeout_();
    void process_legacy_scan_timeout();

private:
    pal::EventQueue &_event_queue;
    PalGap &_pal_gap;
    pal::GenericAccessService &_gap_service;
    PalSecurityManager &_pal_sm;
    ble::own_address_type_t _address_type;
    ble::address_t _address;
    pal::initiator_policy_t _initiator_policy_mode;
    pal::scanning_filter_policy_t _scanning_filter_policy;
    pal::advertising_filter_policy_t _advertising_filter_policy;
    mutable whitelist_t _whitelist;

    bool _privacy_enabled;
    peripheral_privacy_configuration_t _peripheral_privacy_configuration;
    central_privacy_configuration_t _central_privacy_configuration;
    ble::address_t _random_static_identity_address;
    bool _random_address_rotating;

    bool _scan_enabled;
    mbed::LowPowerTimeout _advertising_timeout;
    mbed::LowPowerTimeout _scan_timeout;
    mbed::LowPowerTicker _address_rotation_ticker;

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
    BitArray<MAX_ADVERTISING_SETS> _connectable_payload_size_exceeded;
    BitArray<MAX_ADVERTISING_SETS> _set_is_connectable;

    bool _user_manage_connection_parameter_requests : 1;

private:
    ble_error_t setExtendedAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &parameters
    );

    bool is_extended_advertising_available();

    void prepare_legacy_advertising_set();
};

} // namespace generic
} // namespace ble

#endif /* MBED_BLE_GENERIC_GAP */
