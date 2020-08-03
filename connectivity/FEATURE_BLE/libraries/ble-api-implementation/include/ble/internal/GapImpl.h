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

#ifndef IMPL_GAP_GAP_H
#define IMPL_GAP_GAP_H

#include "CallChainOfFunctionPointersWithContext.h"

#include <algorithm>

#include "drivers/LowPowerTimeout.h"
#include "drivers/LowPowerTicker.h"
#include "platform/mbed_error.h"

#include "ble/common/ble/BLERoles.h"
#include "ble/common/ble/BLETypes.h"
#include "ble/common/ble/gap/AdvertisingDataBuilder.h"
#include "ble/common/ble/gap/AdvertisingDataSimpleBuilder.h"
#include "ble/common/ble/gap/ConnectionParameters.h"
#include "ble/common/ble/gap/ScanParameters.h"
#include "ble/common/ble/gap/AdvertisingParameters.h"
#include "ble/common/ble/gap/Events.h"

#include "ble/internal/PalGap.h"
#include "ble/common/ble/GapEvents.h"
#include "ble/common/ble/GapTypes.h"
#include "ble/internal/PalEventQueue.h"
#include "ble/internal/PalConnectionMonitor.h"
#include "PalEventQueue.h"

#include "ble/Gap.h"

namespace ble {

class PalGenericAccessService;
class PalSecurityManager;
class PalGap;
class BLEInstanceBase;

class Gap :
    public ble::interface::Gap,
    public PalConnectionMonitor,
    public PalGapEventHandler
{
    friend PalConnectionMonitor;
    friend PalGapEventHandler;
    friend PalGap;
    friend BLEInstanceBase;
public:
    /**
     * Default peripheral privacy configuration.
     */
    static const peripheral_privacy_configuration_t
        default_peripheral_privacy_configuration;

    /**
     * Default peripheral privacy configuration.
     */
    static const central_privacy_configuration_t
        default_central_privacy_configuration;

public:
    void setEventHandler(EventHandler *handler);

    bool isFeatureSupported(controller_supported_features_t feature);

    /*                                     advertising                                           */
#if BLE_ROLE_BROADCASTER

    uint8_t getMaxAdvertisingSetNumber();

    uint16_t getMaxAdvertisingDataLength();

    uint16_t getMaxConnectableAdvertisingDataLength();

    uint16_t getMaxActiveSetAdvertisingDataLength();

#if BLE_FEATURE_EXTENDED_ADVERTISING

    ble_error_t createAdvertisingSet(
        advertising_handle_t *handle,
        const AdvertisingParameters &parameters
    );

    ble_error_t destroyAdvertisingSet(advertising_handle_t handle);
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

    ble_error_t setAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &params
    );

    ble_error_t setAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload
    );

    ble_error_t setAdvertisingScanResponse(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> response
    );

    ble_error_t startAdvertising(
        advertising_handle_t handle,
        adv_duration_t maxDuration = adv_duration_t::forever(),
        uint8_t maxEvents = 0
    );

    ble_error_t stopAdvertising(advertising_handle_t handle);

    bool isAdvertisingActive(advertising_handle_t handle);
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_BROADCASTER
#if BLE_FEATURE_PERIODIC_ADVERTISING

    ble_error_t setPeriodicAdvertisingParameters(
        advertising_handle_t handle,
        periodic_interval_t periodicAdvertisingIntervalMin,
        periodic_interval_t periodicAdvertisingIntervalMax,
        bool advertiseTxPower = true
    );

    ble_error_t setPeriodicAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload
    );

    ble_error_t startPeriodicAdvertising(advertising_handle_t handle);

    ble_error_t stopPeriodicAdvertising(advertising_handle_t handle);

    bool isPeriodicAdvertisingActive(advertising_handle_t handle);
#endif // BLE_ROLE_BROADCASTER
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

    /*                                     scanning                                              */
#if BLE_ROLE_OBSERVER

    ble_error_t setScanParameters(const ScanParameters &params);

    ble_error_t startScan(
        scan_duration_t duration = scan_duration_t::forever(),
        duplicates_filter_t filtering = duplicates_filter_t::DISABLE,
        scan_period_t period = scan_period_t(0)
    );

    ble_error_t stopScan();
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_OBSERVER
#if BLE_FEATURE_PERIODIC_ADVERTISING

    ble_error_t createSync(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        uint8_t sid,
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    ble_error_t createSync(
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    ble_error_t cancelCreateSync();

    ble_error_t terminateSync(periodic_sync_handle_t handle);

    ble_error_t addDeviceToPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        advertising_sid_t sid
    );

    ble_error_t removeDeviceFromPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        advertising_sid_t sid
    );

    ble_error_t clearPeriodicAdvertiserList();

    uint8_t getMaxPeriodicAdvertiserListSize();
#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_ROLE_CENTRAL

    ble_error_t connect(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        const ConnectionParameters &connectionParams
    );

    ble_error_t cancelConnect(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress
    );

    ble_error_t cancelConnect();
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_CONNECTABLE

    ble_error_t updateConnectionParameters(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervision_timeout,
        conn_event_length_t minConnectionEventLength = conn_event_length_t(0),
        conn_event_length_t maxConnectionEventLength = conn_event_length_t(0)
    );

    ble_error_t manageConnectionParametersUpdateRequest(
        bool userManageConnectionUpdateRequest
    );

    ble_error_t acceptConnectionParametersUpdate(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervision_timeout,
        conn_event_length_t minConnectionEventLength = conn_event_length_t(0),
        conn_event_length_t maxConnectionEventLength = conn_event_length_t(0)
    );

    ble_error_t rejectConnectionParametersUpdate(
        connection_handle_t connectionHandle
    );

    ble_error_t disconnect(
        connection_handle_t connectionHandle,
        local_disconnection_reason_t reason
    );
#endif // BLE_FEATURE_CONNECTABLE
#if BLE_FEATURE_PHY_MANAGEMENT

    ble_error_t readPhy(connection_handle_t connection);

    ble_error_t setPreferredPhys(
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys
    );

    ble_error_t setPhy(
        connection_handle_t connection,
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys,
        coded_symbol_per_bit_t codedSymbol
    );
#endif // BLE_FEATURE_PHY_MANAGEMENT

#if BLE_FEATURE_PRIVACY

    ble_error_t enablePrivacy(bool enable);

#if BLE_ROLE_BROADCASTER

    ble_error_t setPeripheralPrivacyConfiguration(
        const peripheral_privacy_configuration_t *configuration
    );

    ble_error_t getPeripheralPrivacyConfiguration(
        peripheral_privacy_configuration_t *configuration
    );
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER

    ble_error_t setCentralPrivacyConfiguration(
        const central_privacy_configuration_t *configuration
    );

    ble_error_t getCentralPrivacyConfiguration(
        central_privacy_configuration_t *configuration
    );
#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PRIVACY

#if BLE_FEATURE_WHITELIST

    uint8_t getMaxWhitelistSize(void) const;

    ble_error_t getWhitelist(whitelist_t &whitelist) const;

    ble_error_t setWhitelist(const whitelist_t &whitelist);

#endif // BLE_FEATURE_WHITELIST

    ble_error_t getAddress(
        own_address_type_t &typeP,
        address_t &address
    );

    static ble_error_t getRandomAddressType(
        const ble::address_t address,
        ble::random_address_type_t *addressType
    );

    ble_error_t reset(void);

    void onShutdown(const GapShutdownCallback_t &callback);

    template<typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const Gap *));

    GapShutdownCallbackChain_t &onShutdown();

#if !defined(DOXYGEN_ONLY)
    /*
     * API reserved for the controller driver to set the random static address.
     * Setting a new random static address while the controller is operating is
     * forbidden by the Bluetooth specification.
     */
    ble_error_t setRandomStaticAddress(const ble::address_t& address);
#endif // !defined(DOXYGEN_ONLY)

    /* ===================================================================== */
    /*                    private implementation follows                     */

private:
    /* Disallow copy and assignment. */
    Gap(const Gap &);
    Gap& operator=(const Gap &);

    Gap(
        PalEventQueue &event_queue,
        PalGap &pal_gap,
        PalGenericAccessService &generic_access_service,
        PalSecurityManager &pal_sm
    );

    ~Gap();

    ble_error_t setAdvertisingData(
        advertising_handle_t handle,
        Span<const uint8_t> payload,
        bool minimiseFragmentation,
        bool scan_response
    );

    void on_advertising_timeout();

    void process_advertising_timeout();

    void on_gap_event_received(const GapEvent &e);

    void on_advertising_report(const GapAdvertisingReportEvent &e);

    void on_connection_complete(const GapConnectionCompleteEvent &e);

    void on_disconnection_complete(const GapDisconnectionCompleteEvent &e);

    void on_connection_parameter_request(
        const GapRemoteConnectionParameterRequestEvent &e
    );

    void on_connection_update(const GapConnectionUpdateEvent &e);

    void on_unexpected_error(const GapUnexpectedErrorEvent &e);

    enum AddressUseType_t {
        CENTRAL_CONNECTION,
        CENTRAL_SCAN,
        PERIPHERAL_CONNECTABLE,
        PERIPHERAL_NON_CONNECTABLE
    };

    own_address_type_t get_own_address_type(AddressUseType_t address_use_type);

    bool initialize_whitelist() const;

    ble_error_t update_address_resolution_setting();

    void set_random_address_rotation(bool enable);

    void update_random_address();

    bool getUnresolvableRandomAddress(ble::address_t &address);

    void on_address_rotation_timeout();

    ble_error_t setExtendedAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &parameters
    );

    bool is_extended_advertising_available();

    void prepare_legacy_advertising_set();

    /* implements PalGap::EventHandler */
private:
    void on_read_phy(
        hci_error_code_t hci_status,
        connection_handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    void on_data_length_change(
        connection_handle_t connection_handle,
        uint16_t tx_size,
        uint16_t rx_size
    );

    void on_phy_update_complete(
        hci_error_code_t hci_status,
        connection_handle_t connection_handle,
        phy_t tx_phy,
        phy_t rx_phy
    );

    void on_enhanced_connection_complete(
        hci_error_code_t status,
        connection_handle_t connection_handle,
        connection_role_t own_role,
        connection_peer_address_type_t peer_address_type,
        const ble::address_t &peer_address,
        const ble::address_t &local_resolvable_private_address,
        const ble::address_t &peer_resolvable_private_address,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        clock_accuracy_t master_clock_accuracy
    );

    void on_extended_advertising_report(
        advertising_event_t event_type,
        const connection_peer_address_type_t *address_type,
        const ble::address_t &address,
        phy_t primary_phy,
        const phy_t *secondary_phy,
        advertising_sid_t advertising_sid,
        advertising_power_t tx_power,
        rssi_t rssi,
        uint16_t periodic_advertising_interval,
        direct_address_type_t direct_address_type,
        const ble::address_t &direct_address,
        uint8_t data_length,
        const uint8_t *data
    );

    void on_periodic_advertising_sync_established(
        hci_error_code_t error,
        sync_handle_t sync_handle,
        advertising_sid_t advertising_sid,
        connection_peer_address_type_t advertiser_address_type,
        const ble::address_t &advertiser_address,
        phy_t advertiser_phy,
        uint16_t periodic_advertising_interval,
        clock_accuracy_t clock_accuracy
    );

    void on_periodic_advertising_report(
        sync_handle_t sync_handle,
        advertising_power_t tx_power,
        rssi_t rssi,
        advertising_data_status_t data_status,
        uint8_t data_length,
        const uint8_t *data
    );

    void on_periodic_advertising_sync_loss(sync_handle_t sync_handle);

    void on_advertising_set_terminated(
        hci_error_code_t status,
        advertising_handle_t advertising_handle,
        connection_handle_t connection_handle,
        uint8_t number_of_completed_extended_advertising_events
    );

    void on_scan_request_received(
        advertising_handle_t advertising_handle,
        connection_peer_address_type_t scanner_address_type,
        const ble::address_t &address
    );

    void on_connection_update_complete(
        hci_error_code_t status,
        connection_handle_t connection_handle,
        uint16_t connection_interval,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

    void on_remote_connection_parameter(
        connection_handle_t connection_handle,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout
    );

    void on_scan_timeout();
    void process_legacy_scan_timeout();

private:
    /**
     * Callchain containing all registered callback handlers for shutdown
     * events.
     */
    GapShutdownCallbackChain_t shutdownCallChain;

    /**
     * Event handler provided by the application.
     */
    ble::Gap::EventHandler *_event_handler;

    PalEventQueue &_event_queue;
    PalGap &_pal_gap;
    PalGenericAccessService &_gap_service;
    PalSecurityManager &_pal_sm;
    ble::own_address_type_t _address_type;
    ble::address_t _address;
    initiator_policy_t _initiator_policy_mode;
    scanning_filter_policy_t _scanning_filter_policy;
    advertising_filter_policy_t _advertising_filter_policy;
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

    BitArray<BLE_GAP_MAX_ADVERTISING_SETS> _existing_sets;
    BitArray<BLE_GAP_MAX_ADVERTISING_SETS> _active_sets;
    BitArray<BLE_GAP_MAX_ADVERTISING_SETS> _active_periodic_sets;
    BitArray<BLE_GAP_MAX_ADVERTISING_SETS> _connectable_payload_size_exceeded;
    BitArray<BLE_GAP_MAX_ADVERTISING_SETS> _set_is_connectable;

    bool _user_manage_connection_parameter_requests : 1;

    /* these need be removed when the deprecated cancelConnect() is removed */
    peer_address_type_t _last_used_peer_address_type = peer_address_type_t::ANONYMOUS;
    ble::address_t _last_used_peer_address;
};

} // namespace ble

#endif //IMPL_GAP_GAP_H
