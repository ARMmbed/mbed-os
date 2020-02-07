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

#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/pal/EventQueue.h"
#include "ble/pal/ConnectionEventMonitor.h"
#include "ble/pal/Deprecated.h"

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
    public interface::LegacyGap<
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
    typedef ::ble::interface::LegacyGap<GenericGap> LegacyGap;
    typedef ::ble::interface::Gap<GenericGap> Gap;
    typedef pal::ConnectionEventMonitor<ConnectionEventMonitorEventHandler> ConnectionEventMonitor;
    typedef TPalGap<GenericGap> PalGap;
    typedef pal::GapEventHandler<GenericGap> PalGapEventHandler;

    // Friendship with base classes
    friend LegacyGap;
    friend Gap;
    friend ConnectionEventMonitor;
    friend pal::GapEventHandler<GenericGap>;
    friend PalGap;

    // Imports from LegacyGap
    using LegacyGap::_eventHandler;
    using LegacyGap::default_peripheral_privacy_configuration;
    using LegacyGap::default_central_privacy_configuration;
    using LegacyGap::state;

    typedef typename LegacyGap::Address_t Address_t;
    typedef typename LegacyGap::PeerAddressType_t PeerAddressType_t;
    typedef typename LegacyGap::ConnectionParams_t ConnectionParams_t;
    typedef typename LegacyGap::Handle_t Handle_t;
    typedef typename LegacyGap::CodedSymbolPerBit_t CodedSymbolPerBit_t;
    typedef typename LegacyGap::Whitelist_t Whitelist_t;
    typedef typename LegacyGap::DisconnectionReason_t DisconnectionReason_t;
    typedef typename LegacyGap::AdvertisingPolicyMode_t AdvertisingPolicyMode_t;
    typedef typename LegacyGap::ScanningPolicyMode_t ScanningPolicyMode_t;
    typedef typename LegacyGap::InitiatorPolicyMode_t InitiatorPolicyMode_t;
    typedef typename LegacyGap::PeripheralPrivacyConfiguration_t PeripheralPrivacyConfiguration_t;
    typedef typename LegacyGap::CentralPrivacyConfiguration_t CentralPrivacyConfiguration_t;
    typedef typename LegacyGap::Role_t Role_t;

    // Imports from Gap
#if BLE_ROLE_BROADCASTER
    using ble::interface::Gap<GenericGap>::getMaxAdvertisingSetNumber;
    using ble::interface::Gap<GenericGap>::getMaxAdvertisingDataLength;
#endif // BLE_ROLE_BROADCASTER
    using ble::interface::Gap<GenericGap>::isFeatureSupported;
    using ble::interface::Gap<GenericGap>::useVersionOneAPI;
    using ble::interface::Gap<GenericGap>::useVersionTwoAPI;

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
     * @see Gap::setAddress
     */
    ble_error_t setAddress_(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::setRandomStaticAddress
     */
    ble_error_t setRandomStaticAddress_(const ble::address_t& address);

    /**
     * @see Gap::getAddress
     */
    ble_error_t getAddress_(
        BLEProtocol::AddressType_t *type,
        BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::getMinAdvertisingInterval
     */
    uint16_t getMinAdvertisingInterval_() const;

    /**
     * @see Gap::getMinNonConnectableAdvertisingInterval
     */
    uint16_t getMinNonConnectableAdvertisingInterval_() const;

    /**
     * @see Gap::getMaxAdvertisingInterval
     */
    uint16_t getMaxAdvertisingInterval_() const;

    /**
     * @see Gap::stopAdvertising
     */
    ble_error_t stopAdvertising_();

    /**
     * @see Gap::stopScan
     */
    ble_error_t stopScan_();

    /**
     * @see Gap::connect
     */
    ble_error_t connect_(
        const BLEProtocol::AddressBytes_t peerAddr,
        PeerAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * @see Gap::connect
     */
    ble_error_t connect_(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

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
    ble_error_t readPhy_(Handle_t connection);

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
        Handle_t connection,
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys,
        CodedSymbolPerBit_t codedSymbol
    );

    ble_error_t disconnect_(
        connection_handle_t connectionHandle,
        local_disconnection_reason_t reason
    );

    /**
     * @see Gap::disconnect
     */
    ble_error_t disconnect_(
        Handle_t connectionHandle,
        DisconnectionReason_t reason
    );

    /**
     * @see Gap::disconnect
     */
    ble_error_t disconnect_(DisconnectionReason_t reason);

    /**
     * @see Gap::updateConnectionParams
     */
    ble_error_t updateConnectionParams_(
        Handle_t handle,
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::getPreferredConnectionParams
     */
    ble_error_t getPreferredConnectionParams_(
        ConnectionParams_t *params
    );

    /**
     * @see Gap::setPreferredConnectionParams
     */
    ble_error_t setPreferredConnectionParams_(
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::setDeviceName
     */
    ble_error_t setDeviceName_(const uint8_t *deviceName);

    /**
     * @see Gap::getDeviceName
     */
    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);

    /**
     * @see Gap::setAppearance
     */
    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);

    /**
     * @see Gap::getAppearance
     */
    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);

    /**
     * @see Gap::setTxPower
     */
    ble_error_t setTxPower_(int8_t txPower);

    /**
     * @see Gap::getPermittedTxPowerValues
     */
    void getPermittedTxPowerValues_(const int8_t **valueArrayPP, size_t *countP);

    /**
     * @see Gap::getMaxWhitelistSize
     */
    uint8_t getMaxWhitelistSize_(void) const;

    /**
     * @see Gap::getWhitelist
     */
    ble_error_t getWhitelist_(Whitelist_t &whitelist) const;

    /**
     * @see Gap::setWhitelist
     */
    ble_error_t setWhitelist_(const Whitelist_t &whitelist);

    /**
     * @see Gap::setAdvertisingPolicyMode
     */
    ble_error_t setAdvertisingPolicyMode_(AdvertisingPolicyMode_t mode);

    /**
     * @see Gap::setScanningPolicyMode
     */
    ble_error_t setScanningPolicyMode_(ScanningPolicyMode_t mode);

    /**
     * @see Gap::setInitiatorPolicyMode
     */
    ble_error_t setInitiatorPolicyMode_(InitiatorPolicyMode_t mode);

    /**
     * @see Gap::getAdvertisingPolicyMode
     */
    AdvertisingPolicyMode_t getAdvertisingPolicyMode_(void) const;

    /**
     * @see Gap::getScanningPolicyMode
     */
    ScanningPolicyMode_t getScanningPolicyMode_(void) const;

    /**
     * @see Gap::getInitiatorPolicyMode
     */
    InitiatorPolicyMode_t getInitiatorPolicyMode_(void) const;

    /**
     * @see Gap::startRadioScan
     */
    ble_error_t startRadioScan_(const GapScanningParams &scanningParams);

    /**
     * @see Gap::initRadioNotification
     */
    ble_error_t initRadioNotification_(void);

    /**
     * @see Gap::enablePrivacy
     */
    ble_error_t enablePrivacy_(bool enable);

    /**
     * @see Gap::setPeripheralPrivacyConfiguration
     */
    ble_error_t setPeripheralPrivacyConfiguration_(
        const PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getPeripheralPrivacyConfiguration
     */
    ble_error_t getPeripheralPrivacyConfiguration_(
        PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setCentralPrivacyConfiguration
     */
    ble_error_t setCentralPrivacyConfiguration_(
        const CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getCentralPrivacyConfiguration
     */
    ble_error_t getCentralPrivacyConfiguration_(
        CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setAdvertisingData
     */
    ble_error_t setAdvertisingData_(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    );

    /**
     * @see Gap::startAdvertising
     */
    ble_error_t startAdvertising_(const GapAdvertisingParams &params);

    /**
     * @see Gap::reset
     */
    ble_error_t reset_(void);

    /**
     * @copydoc ::Gap::processConnectionEvent
     */
    void processConnectionEvent(
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
    void processDisconnectionEvent(
        Handle_t handle,
        DisconnectionReason_t reason
    );

private:
    ble_error_t setAdvertisingData(
        advertising_handle_t handle,
        Span<const uint8_t> payload,
        bool minimiseFragmentation,
        bool scan_response
    );

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

    void useVersionOneAPI_() const;

    void useVersionTwoAPI_() const;

    /* implements pal::Gap::EventHandler */
private:
    void on_read_phy_(
        pal::hci_error_code_t hci_status,
        Handle_t connection_handle,
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
        Handle_t connection_handle,
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

} // namespace generic
} // namespace ble

#endif /* MBED_BLE_GENERIC_GAP */
