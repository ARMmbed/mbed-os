/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "ble/gap/Gap.h"

namespace ble {
namespace interface {

template<class Impl>
bool Gap<Impl>::isFeatureSupported(controller_supported_features_t feature)
{
#if !BLE_FEATURE_PHY_MANAGEMENT
    if (feature == ble::controller_supported_features_t::LE_CODED_PHY ||
        feature == ble::controller_supported_features_t::LE_2M_PHY
        ) {
        return false;
    }
#endif

#if !BLE_FEATURE_EXTENDED_ADVERTISING
    if (feature == ble::controller_supported_features_t::LE_EXTENDED_ADVERTISING) {
        return false;
    }
#endif

#if !BLE_FEATURE_PERIODIC_ADVERTISING
    if (feature == ble::controller_supported_features_t::LE_PERIODIC_ADVERTISING) {
        return false;
    }
#endif

#if !BLE_FEATURE_PRIVACY
    if (feature == ble::controller_supported_features_t::LL_PRIVACY) {
        return false;
    }
#endif

    return impl()->isFeatureSupported_(feature);
}

#if BLE_ROLE_BROADCASTER
template<class Impl>
uint8_t Gap<Impl>::getMaxAdvertisingSetNumber()
{
    return impl()->getMaxAdvertisingSetNumber_();
}

template<class Impl>
uint16_t Gap<Impl>::getMaxAdvertisingDataLength()
{
    return impl()->getMaxAdvertisingDataLength_();
}

template<class Impl>
uint16_t Gap<Impl>::getMaxConnectableAdvertisingDataLength()
{
    return impl()->getMaxConnectableAdvertisingDataLength_();
}

template<class Impl>
uint16_t Gap<Impl>::getMaxActiveSetAdvertisingDataLength()
{
    return impl()->getMaxActiveSetAdvertisingDataLength_();
}

#if BLE_FEATURE_EXTENDED_ADVERTISING
template<class Impl>
ble_error_t Gap<Impl>::createAdvertisingSet(
    advertising_handle_t *handle,
    const AdvertisingParameters &parameters
)
{
    return impl()->createAdvertisingSet_(handle, parameters);
}

template<class Impl>
ble_error_t Gap<Impl>::destroyAdvertisingSet(advertising_handle_t handle)
{
    return impl()->destroyAdvertisingSet_(handle);
}
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingParameters(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    return impl()->setAdvertisingParameters_(handle, params);
}

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return impl()->setAdvertisingPayload_(handle, payload);
}

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingScanResponse(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> response
)
{
    return impl()->setAdvertisingScanResponse_(handle, response);
}

template<class Impl>
ble_error_t Gap<Impl>::startAdvertising(
    advertising_handle_t handle,
    adv_duration_t maxDuration,
    uint8_t maxEvents
)
{
    return impl()->startAdvertising_(handle, maxDuration, maxEvents);
}

template<class Impl>
ble_error_t Gap<Impl>::stopAdvertising(advertising_handle_t handle)
{
    return impl()->stopAdvertising_(handle);
}

template<class Impl>
bool Gap<Impl>::isAdvertisingActive(advertising_handle_t handle)
{
    return impl()->isAdvertisingActive_(handle);
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_FEATURE_PERIODIC_ADVERTISING
template<class Impl>
ble_error_t Gap<Impl>::setPeriodicAdvertisingParameters(
    advertising_handle_t handle,
    periodic_interval_t periodicAdvertisingIntervalMin,
    periodic_interval_t periodicAdvertisingIntervalMax,
    bool advertiseTxPower
)
{
    return impl()->setPeriodicAdvertisingParameters_(
        handle,
        periodicAdvertisingIntervalMin,
        periodicAdvertisingIntervalMax,
        advertiseTxPower
    );
}

template<class Impl>
ble_error_t Gap<Impl>::setPeriodicAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return impl()->setPeriodicAdvertisingPayload_(handle, payload);
}

template<class Impl>
ble_error_t Gap<Impl>::startPeriodicAdvertising(advertising_handle_t handle)
{
    return impl()->startPeriodicAdvertising_(handle);
}

template<class Impl>
ble_error_t Gap<Impl>::stopPeriodicAdvertising(advertising_handle_t handle)
{
    return impl()->stopPeriodicAdvertising_(handle);
}

template<class Impl>
bool Gap<Impl>::isPeriodicAdvertisingActive(advertising_handle_t handle)
{
    return impl()->isPeriodicAdvertisingActive_(handle);
}
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_ROLE_OBSERVER
template<class Impl>
ble_error_t Gap<Impl>::setScanParameters(const ScanParameters &params)
{
    return impl()->setScanParameters_(params);
}

template<class Impl>
ble_error_t Gap<Impl>::startScan(
    scan_duration_t duration,
    duplicates_filter_t filtering,
    scan_period_t period
)
{
    return impl()->startScan_(duration, filtering, period);
}

template<class Impl>
ble_error_t Gap<Impl>::stopScan()
{
    return impl()->stopScan_();
}
#endif // BLE_ROLE_OBSERVER
#if BLE_FEATURE_PERIODIC_ADVERTISING
template<class Impl>
ble_error_t Gap<Impl>::createSync(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    uint8_t sid,
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return impl()->createSync_(
        peerAddressType,
        peerAddress,
        sid,
        maxPacketSkip,
        timeout
    );
}

template<class Impl>
ble_error_t Gap<Impl>::createSync(
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return impl()->createSync_(maxPacketSkip, timeout);
}

template<class Impl>
ble_error_t Gap<Impl>::cancelCreateSync()
{
    return impl()->cancelCreateSync_();
}

template<class Impl>
ble_error_t Gap<Impl>::terminateSync(periodic_sync_handle_t handle)
{
    return impl()->terminateSync_(handle);
}

template<class Impl>
ble_error_t Gap<Impl>::addDeviceToPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return impl()->addDeviceToPeriodicAdvertiserList_(
        peerAddressType,
        peerAddress,
        sid
    );
}

template<class Impl>
ble_error_t Gap<Impl>::removeDeviceFromPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return impl()->removeDeviceFromPeriodicAdvertiserList_(
        peerAddressType,
        peerAddress,
        sid
    );
}

template<class Impl>
ble_error_t Gap<Impl>::clearPeriodicAdvertiserList()
{
    return impl()->clearPeriodicAdvertiserList_();
}

template<class Impl>
uint8_t Gap<Impl>::getMaxPeriodicAdvertiserListSize()
{
    return impl()->getMaxPeriodicAdvertiserListSize_();
}
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_ROLE_CENTRAL
template<class Impl>
ble_error_t Gap<Impl>::connect(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    const ConnectionParameters &connectionParams
)
{
    return impl()->connect_(
        peerAddressType,
        peerAddress,
        connectionParams
    );
}

template<class Impl>
ble_error_t Gap<Impl>::cancelConnect()
{
    return impl()->cancelConnect_();
}
#endif

#if BLE_FEATURE_CONNECTABLE
template<class Impl>
ble_error_t Gap<Impl>::updateConnectionParameters(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervision_timeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    return impl()->updateConnectionParameters_(
        connectionHandle,
        minConnectionInterval,
        maxConnectionInterval,
        slaveLatency,
        supervision_timeout,
        minConnectionEventLength,
        maxConnectionEventLength
    );
}

template<class Impl>
ble_error_t Gap<Impl>::manageConnectionParametersUpdateRequest(
    bool userManageConnectionUpdateRequest
)
{
    return impl()->manageConnectionParametersUpdateRequest_(userManageConnectionUpdateRequest);
}

template<class Impl>
ble_error_t Gap<Impl>::acceptConnectionParametersUpdate(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervision_timeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    return impl()->acceptConnectionParametersUpdate_(
        connectionHandle,
        minConnectionInterval,
        maxConnectionInterval,
        slaveLatency,
        supervision_timeout,
        minConnectionEventLength,
        maxConnectionEventLength
    );
}

template<class Impl>
ble_error_t Gap<Impl>::rejectConnectionParametersUpdate(
    connection_handle_t connectionHandle
)
{
    return impl()->rejectConnectionParametersUpdate_(connectionHandle);
}

template<class Impl>
ble_error_t Gap<Impl>::disconnect(
    connection_handle_t connectionHandle,
    local_disconnection_reason_t reason
)
{
    return impl()->disconnect_(connectionHandle, reason);
}
#endif // BLE_FEATURE_CONNECTABLE

#if BLE_FEATURE_PHY_MANAGEMENT
template<class Impl>
ble_error_t Gap<Impl>::readPhy(connection_handle_t connection)
{
    return impl()->readPhy_(connection);
}

template<class Impl>
ble_error_t Gap<Impl>::setPreferredPhys(
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys
)
{
    return impl()->setPreferredPhys_(txPhys, rxPhys);
}

template<class Impl>
ble_error_t Gap<Impl>::setPhy(
    connection_handle_t connection,
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys,
    coded_symbol_per_bit_t codedSymbol
)
{
    return impl()->setPhy_(
        connection,
        txPhys,
        rxPhys,
        codedSymbol
    );
}
#endif // BLE_FEATURE_PHY_MANAGEMENT

template<class Impl>
void Gap<Impl>::useVersionOneAPI() const
{
    return impl()->useVersionOneAPI_();
}

template<class Impl>
void Gap<Impl>::useVersionTwoAPI() const
{
    return impl()->useVersionTwoAPI_();
}

template<class Impl>
Gap<Impl>::Gap() : _eventHandler(NULL)
{
}


/* -------------------- Future deprecation ------------------------- */

template<class Impl>
const peripheral_privacy_configuration_t Gap<Impl>::default_peripheral_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ peripheral_privacy_configuration_t::PERFORM_PAIRING_PROCEDURE
};

template<class Impl>
const central_privay_configuration_t Gap<Impl>::default_central_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ central_privay_configuration_t::RESOLVE_AND_FORWARD
};

#if BLE_FEATURE_PRIVACY
template<class Impl>
ble_error_t Gap<Impl>::enablePrivacy(bool enable)
{
    return impl()->enablePrivacy_(enable);
}

#if BLE_ROLE_BROADCASTER
template<class Impl>
ble_error_t Gap<Impl>::setPeripheralPrivacyConfiguration(
    const peripheral_privacy_configuration_t *configuration
)
{
    return impl()->setPeripheralPrivacyConfiguration_(configuration);
}

template<class Impl>
ble_error_t Gap<Impl>::getPeripheralPrivacyConfiguration(
    peripheral_privacy_configuration_t *configuration
)
{
    return impl()->getPeripheralPrivacyConfiguration_(configuration);
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
template<class Impl>
ble_error_t Gap<Impl>::setCentralPrivacyConfiguration(
    const central_privay_configuration_t *configuration
)
{
    return impl()->setCentralPrivacyConfiguration_(configuration);
}

template<class Impl>
ble_error_t Gap<Impl>::getCentralPrivacyConfiguration(
    central_privay_configuration_t *configuration
)
{
    return impl()->getCentralPrivacyConfiguration_(configuration);
}
#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PRIVACY

// -----------------------------------------------------------------------------
/* ------------------------- Default implementations ------------------------ */
// -----------------------------------------------------------------------------

template<class Impl>
bool Gap<Impl>::isFeatureSupported_(controller_supported_features_t feature)
{
    return false;
}

template<class Impl>
uint8_t Gap<Impl>::getMaxAdvertisingSetNumber_()
{
    return 1;
}

template<class Impl>
uint16_t Gap<Impl>::getMaxAdvertisingDataLength_()
{
    return LEGACY_ADVERTISING_MAX_SIZE;
}

template<class Impl>
uint16_t Gap<Impl>::getMaxConnectableAdvertisingDataLength_()
{
    return LEGACY_ADVERTISING_MAX_SIZE;
}

template<class Impl>
uint16_t Gap<Impl>::getMaxActiveSetAdvertisingDataLength_()
{
    return LEGACY_ADVERTISING_MAX_SIZE;
}

template<class Impl>
ble_error_t Gap<Impl>::createAdvertisingSet_(
    advertising_handle_t *handle,
    const AdvertisingParameters &parameters
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::destroyAdvertisingSet_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingParameters_(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingPayload_(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setAdvertisingScanResponse_(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> response
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::startAdvertising_(
    advertising_handle_t handle,
    adv_duration_t maxDuration,
    uint8_t maxEvents
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::stopAdvertising_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
bool Gap<Impl>::isAdvertisingActive_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setPeriodicAdvertisingParameters_(
    advertising_handle_t handle,
    periodic_interval_t periodicAdvertisingIntervalMin,
    periodic_interval_t periodicAdvertisingIntervalMax,
    bool advertiseTxPower
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setPeriodicAdvertisingPayload_(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::startPeriodicAdvertising_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::stopPeriodicAdvertising_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
bool Gap<Impl>::isPeriodicAdvertisingActive_(advertising_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setScanParameters_(const ScanParameters &params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::startScan_(
    scan_duration_t duration,
    duplicates_filter_t filtering,
    scan_period_t period
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::stopScan_()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::createSync_(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    uint8_t sid,
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::createSync_(
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::cancelCreateSync_()
{
    return impl()->cancelCreateSync_();
}

template<class Impl>
ble_error_t Gap<Impl>::terminateSync_(periodic_sync_handle_t handle)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::addDeviceToPeriodicAdvertiserList_(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::removeDeviceFromPeriodicAdvertiserList_(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}


template<class Impl>
ble_error_t Gap<Impl>::clearPeriodicAdvertiserList_()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
uint8_t Gap<Impl>::getMaxPeriodicAdvertiserListSize_()
{
    return 0;
}

template<class Impl>
ble_error_t Gap<Impl>::connect_(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    const ConnectionParameters &connectionParams
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::cancelConnect_()
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::updateConnectionParameters_(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervision_timeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::manageConnectionParametersUpdateRequest_(
    bool userManageConnectionUpdateRequest
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::acceptConnectionParametersUpdate_(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervision_timeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::rejectConnectionParametersUpdate_(
    connection_handle_t connectionHandle
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::disconnect_(
    connection_handle_t connectionHandle,
    local_disconnection_reason_t reason
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::readPhy_(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setPreferredPhys_(
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setPhy_(
    connection_handle_t connection,
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys,
    coded_symbol_per_bit_t codedSymbol
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
void Gap<Impl>::useVersionOneAPI_() const
{
}

template<class Impl>
void Gap<Impl>::useVersionTwoAPI_() const
{
}

template<class Impl>
ble_error_t Gap<Impl>::enablePrivacy_(bool enable)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setPeripheralPrivacyConfiguration_(
    const peripheral_privacy_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::getPeripheralPrivacyConfiguration_(
    peripheral_privacy_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::setCentralPrivacyConfiguration_(
    const central_privay_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t Gap<Impl>::getCentralPrivacyConfiguration_(
    central_privay_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // namespace interface
} // namespace ble

