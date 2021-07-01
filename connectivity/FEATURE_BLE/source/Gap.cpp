/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#include "ble/Gap.h"
#include "source/generic/GapImpl.h"

namespace ble {

void Gap::setEventHandler(EventHandler *handler)
{
    impl->setEventHandler(handler);
}


bool Gap::isFeatureSupported(controller_supported_features_t feature)
{
    return impl->isFeatureSupported(feature);
}

/*                                     advertising                                           */
#if BLE_ROLE_BROADCASTER

uint8_t Gap::getMaxAdvertisingSetNumber()
{
    return impl->getMaxAdvertisingSetNumber();
}


uint16_t Gap::getMaxAdvertisingDataLength()
{
    return impl->getMaxAdvertisingDataLength();
}


uint16_t Gap::getMaxConnectableAdvertisingDataLength()
{
    return impl->getMaxConnectableAdvertisingDataLength();
}


uint16_t Gap::getMaxActiveSetAdvertisingDataLength()
{
    return impl->getMaxActiveSetAdvertisingDataLength();
}

#if BLE_FEATURE_EXTENDED_ADVERTISING

ble_error_t Gap::createAdvertisingSet(
    advertising_handle_t *handle,
    const AdvertisingParameters &parameters
)
{
    return impl->createAdvertisingSet(handle, parameters);
}


ble_error_t Gap::destroyAdvertisingSet(advertising_handle_t handle)
{
    return impl->destroyAdvertisingSet(handle);
}

#endif // BLE_FEATURE_EXTENDED_ADVERTISING


ble_error_t Gap::setAdvertisingParameters(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    return impl->setAdvertisingParameters(handle, params);
}


ble_error_t Gap::setAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return impl->setAdvertisingPayload(handle, payload);
}


ble_error_t Gap::setAdvertisingScanResponse(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> response
)
{
    return impl->setAdvertisingScanResponse(handle, response);
}

ble_error_t Gap::startAdvertising(
    advertising_handle_t handle,
    adv_duration_t maxDuration,
    uint8_t maxEvents
)
{
    return impl->startAdvertising(handle, maxDuration, maxEvents);
}

ble_error_t Gap::stopAdvertising(advertising_handle_t handle)
{
    return impl->stopAdvertising(handle);
}


bool Gap::isAdvertisingActive(advertising_handle_t handle)
{
    return impl->isAdvertisingActive(handle);
}

#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_BROADCASTER
#if BLE_FEATURE_PERIODIC_ADVERTISING

ble_error_t Gap::setPeriodicAdvertisingParameters(
    advertising_handle_t handle,
    periodic_interval_t periodicAdvertisingIntervalMin,
    periodic_interval_t periodicAdvertisingIntervalMax,
    bool advertiseTxPower
)
{
    return impl->setPeriodicAdvertisingParameters(
        handle,
        periodicAdvertisingIntervalMin,
        periodicAdvertisingIntervalMax,
        advertiseTxPower
    );
}


ble_error_t Gap::setPeriodicAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    return impl->setPeriodicAdvertisingPayload(handle, payload);
}


ble_error_t Gap::startPeriodicAdvertising(advertising_handle_t handle)
{
    return impl->startPeriodicAdvertising(handle);
}


ble_error_t Gap::stopPeriodicAdvertising(advertising_handle_t handle)
{
    return impl->stopPeriodicAdvertising(handle);
}


bool Gap::isPeriodicAdvertisingActive(advertising_handle_t handle)
{
    return impl->isPeriodicAdvertisingActive(handle);
}

#endif // BLE_ROLE_BROADCASTER
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

/*                                     scanning                                              */
#if BLE_ROLE_OBSERVER

ble_error_t Gap::setScanParameters(const ScanParameters &params)
{
    return impl->setScanParameters(params);
}


ble_error_t Gap::startScan(
    scan_duration_t duration,
    duplicates_filter_t filtering,
    scan_period_t period
)
{
    return impl->startScan(duration, filtering, period);
}

ble_error_t Gap::stopScan()
{
    return impl->stopScan();
}

#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_OBSERVER
#if BLE_FEATURE_PERIODIC_ADVERTISING

ble_error_t Gap::createSync(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    uint8_t sid,
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return impl->createSync(peerAddressType, peerAddress, sid, maxPacketSkip, timeout);
}


ble_error_t Gap::createSync(
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    return impl->createSync(maxPacketSkip, timeout);
}


ble_error_t Gap::cancelCreateSync()
{
    return impl->cancelCreateSync();
}


ble_error_t Gap::terminateSync(periodic_sync_handle_t handle)
{
    return impl->terminateSync(handle);
}


ble_error_t Gap::addDeviceToPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return impl->addDeviceToPeriodicAdvertiserList(peerAddressType, peerAddress, sid);
}


ble_error_t Gap::removeDeviceFromPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    advertising_sid_t sid
)
{
    return impl->removeDeviceFromPeriodicAdvertiserList(peerAddressType, peerAddress, sid);
}


ble_error_t Gap::clearPeriodicAdvertiserList()
{
    return impl->clearPeriodicAdvertiserList();
}


uint8_t Gap::getMaxPeriodicAdvertiserListSize()
{
    return impl->getMaxPeriodicAdvertiserListSize();
}

#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_ROLE_CENTRAL

ble_error_t Gap::connect(
    peer_address_type_t peerAddressType,
    const address_t &peerAddress,
    const ConnectionParameters &connectionParams
)
{
    return impl->connect(peerAddressType, peerAddress, connectionParams);
}


ble_error_t Gap::cancelConnect()
{
    return impl->cancelConnect();
}


#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_CONNECTABLE

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
    return impl->updateConnectionParameters(
        connectionHandle,
        minConnectionInterval,
        maxConnectionInterval,
        slaveLatency,
        supervision_timeout,
        minConnectionEventLength,
        maxConnectionEventLength
    );
}


ble_error_t Gap::manageConnectionParametersUpdateRequest(
    bool userManageConnectionUpdateRequest
)
{
    return impl->manageConnectionParametersUpdateRequest(userManageConnectionUpdateRequest);
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
    return impl->acceptConnectionParametersUpdate(
        connectionHandle,
        minConnectionInterval,
        maxConnectionInterval,
        slaveLatency,
        supervision_timeout,
        minConnectionEventLength,
        maxConnectionEventLength
    );
}


ble_error_t Gap::rejectConnectionParametersUpdate(
    connection_handle_t connectionHandle
)
{
    return impl->rejectConnectionParametersUpdate(connectionHandle);
}


ble_error_t Gap::disconnect(
    connection_handle_t connectionHandle,
    local_disconnection_reason_t reason
)
{
    return impl->disconnect(connectionHandle, reason);
}

#endif // BLE_FEATURE_CONNECTABLE
#if BLE_FEATURE_PHY_MANAGEMENT

ble_error_t Gap::readPhy(connection_handle_t connection)
{
    return impl->readPhy(connection);
}


ble_error_t Gap::setPreferredPhys(
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys
)
{
    return impl->setPreferredPhys(txPhys, rxPhys);
}


ble_error_t Gap::setPhy(
    connection_handle_t connection,
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys,
    coded_symbol_per_bit_t codedSymbol
)
{
    return impl->setPhy(
        connection,
        txPhys,
        rxPhys,
        codedSymbol
    );
}

#endif // BLE_FEATURE_PHY_MANAGEMENT

#if BLE_FEATURE_PRIVACY

ble_error_t Gap::enablePrivacy(bool enable)
{
    return impl->enablePrivacy(enable);
}

#if BLE_ROLE_BROADCASTER

ble_error_t Gap::setPeripheralPrivacyConfiguration(
    const peripheral_privacy_configuration_t *configuration
)
{
    return impl->setPeripheralPrivacyConfiguration(configuration);
}


ble_error_t Gap::getPeripheralPrivacyConfiguration(
    peripheral_privacy_configuration_t *configuration
)
{
    return impl->getPeripheralPrivacyConfiguration(configuration);
}

#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER

ble_error_t Gap::setCentralPrivacyConfiguration(
    const central_privacy_configuration_t *configuration
)
{
    return impl->setCentralPrivacyConfiguration(configuration);
}


ble_error_t Gap::getCentralPrivacyConfiguration(
    central_privacy_configuration_t *configuration
)
{
    return impl->getCentralPrivacyConfiguration(configuration);
}

#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PRIVACY

#if BLE_FEATURE_WHITELIST

uint8_t Gap::getMaxWhitelistSize() const
{
    return impl->getMaxWhitelistSize();
}


ble_error_t Gap::getWhitelist(whitelist_t &whitelist) const
{
    return impl->getWhitelist(whitelist);
}


ble_error_t Gap::setWhitelist(const whitelist_t &whitelist)
{
    return impl->setWhitelist(whitelist);
}

#endif // BLE_FEATURE_WHITELIST


ble_error_t Gap::getAddress(
    own_address_type_t &typeP,
    address_t &address
)
{
    return impl->getAddress(typeP, address);
}


ble_error_t Gap::getRandomAddressType(
    ble::address_t address,
    ble::random_address_type_t *addressType
)
{
    return impl::Gap::getRandomAddressType(address, addressType);
}


ble_error_t Gap::reset()
{
    return impl->reset();
}


void Gap::onShutdown(const GapShutdownCallback_t &callback)
{
    return impl->onShutdown(callback);
}


Gap::GapShutdownCallbackChain_t &Gap::onShutdown()
{
    return impl->onShutdown();
}


ble_error_t Gap::setRandomStaticAddress(const ble::address_t &address)
{
    return impl->setRandomStaticAddress(address);
}

ble::address_t Gap::getRandomStaticAddress()
{
    return impl->getRandomStaticAddress();
}

} // namespace ble
