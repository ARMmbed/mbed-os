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

bool Gap::isFeatureSupported(controller_supported_features_t feature)
{
    return false;
}

uint8_t Gap::getMaxAdvertisingSetNumber()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    // There is at least one advertising set available: the legacy advertising set
    return 1;
}

uint16_t Gap::getMaxAdvertisingDataLength()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return LEGACY_ADVERTISING_MAX_SIZE;
}

ble_error_t Gap::createAdvertisingSet(
    advertising_handle_t *handle,
    const AdvertisingParameters &parameters
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::destroyAdvertisingSet(advertising_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setAdvertisingParameters(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setAdvertisingScanResponse(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> response
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::startAdvertising(
    advertising_handle_t handle,
    adv_duration_t maxDuration,
    uint8_t maxEvents
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::stopAdvertising(advertising_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

bool Gap::isAdvertisingActive(advertising_handle_t handle)
{
    return false;
}

ble_error_t Gap::setPeriodicAdvertisingParameters(
    advertising_handle_t handle,
    periodic_interval_t periodicAdvertisingIntervalMin,
    periodic_interval_t periodicAdvertisingIntervalMax,
    bool advertiseTxPower
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setPeriodicAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::startPeriodicAdvertising(advertising_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::stopPeriodicAdvertising(advertising_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

bool Gap::isPeriodicAdvertisingActive(advertising_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return false;
}

ble_error_t Gap::setScanParameters(const ScanParameters &params)
{
    useVersionTwoAPI();
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
};

ble_error_t Gap::startScan(
    scan_duration_t duration,
    duplicates_filter_t filtering,
    scan_period_t period
)
{
    useVersionTwoAPI();
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
};

ble_error_t Gap::stopScan()
{
    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::createSync(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    uint8_t sid,
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::createSync(
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::cancelCreateSync()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::terminateSync(periodic_sync_handle_t handle)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::addDeviceToPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::removeDeviceFromPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::clearPeriodicAdvertiserList()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

uint8_t Gap::getMaxPeriodicAdvertiserListSize()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return 0;
}

ble_error_t Gap::connect(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    const ConnectionParameters &connectionParams
)
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::cancelConnect()
{
    /* Requesting action from porter(s): override this API if this capability is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::updateConnectionParameters(
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

ble_error_t Gap::manageConnectionParametersUpdateRequest(
    bool userManageConnectionUpdateRequest
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::acceptConnectionParametersUpdate(
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

ble_error_t Gap::rejectConnectionParametersUpdate(
    connection_handle_t connectionHandle
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::disconnect(
    connection_handle_t connectionHandle,
    local_disconnection_reason_t reason
)
{
    // Forward to the old implementation for now.
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::readPhy(connection_handle_t connection)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setPreferredPhys(
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setPhy(
    connection_handle_t connection,
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys,
    coded_symbol_per_bit_t codedSymbol
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

/* -------------------- Future deprecation ------------------------- */

const peripheral_privacy_configuration_t Gap::default_peripheral_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ peripheral_privacy_configuration_t::PERFORM_PAIRING_PROCEDURE
};

const central_privay_configuration_t Gap::default_central_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ central_privay_configuration_t::RESOLVE_AND_FORWARD
};

ble_error_t Gap::enablePrivacy(bool enable)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setPeripheralPrivacyConfiguration(
    const peripheral_privacy_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::getPeripheralPrivacyConfiguration(
    peripheral_privacy_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setCentralPrivacyConfiguration(
    const central_privay_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::getCentralPrivacyConfiguration(
    central_privay_configuration_t *configuration
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}


} // namespace ble

