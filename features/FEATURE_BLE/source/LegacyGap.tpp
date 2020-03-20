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

#include "ble/Gap.h"

namespace ble {
namespace interface {

#if BLE_FEATURE_WHITELIST
template<class Impl>
uint8_t LegacyGap<Impl>::getMaxWhitelistSize(void) const {
    return impl()->getMaxWhitelistSize_();
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getWhitelist(Whitelist_t &whitelist) const {
    return impl()->getWhitelist_(whitelist);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setWhitelist(const Whitelist_t &whitelist) {
    return impl()->setWhitelist_(whitelist);
}
#endif // BLE_FEATURE_WHITELIST

template<class Impl>
void LegacyGap<Impl>::processConnectionEvent(
    Handle_t handle,
    Role_t role,
    PeerAddressType_t peerAddrType,
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t ownAddrType,
    const BLEProtocol::AddressBytes_t ownAddr,
    const ConnectionParams_t *connectionParams,
    const uint8_t *peerResolvableAddr,
    const uint8_t *localResolvableAddr
) {
    /* Update Gap state */
    state.advertising = 0;
    state.connected   = 1;
    ++connectionCount;

    ConnectionCallbackParams_t callbackParams(
        handle,
        role,
        peerAddrType,
        peerAddr,
        ownAddrType,
        ownAddr,
        connectionParams,
        peerResolvableAddr,
        localResolvableAddr
    );

    connectionCallChain.call(&callbackParams);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getAddress(
    BLEProtocol::AddressType_t *typeP,
    BLEProtocol::AddressBytes_t address
) {
    return impl()->getAddress_(typeP, address);
}

template<class Impl>
LegacyGap<Impl>::AdvertisementCallbackParams_t::AdvertisementCallbackParams_t() :
    peerAddr(),
    rssi(),
    isScanResponse(),
    type(),
    advertisingDataLen(0),
    advertisingData(NULL)
{
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getRandomAddressType(
    const BLEProtocol::AddressBytes_t address,
    RandomAddressType_t* type
) {
    // see section Device address in Bluetooth Link Layer specification
    // (Vol 6 - Part B)
    switch (address[5] >> 6) {
        case 0x03:
            *type = RandomAddressType_t::STATIC;
            return BLE_ERROR_NONE;
        case 0x00:
            *type = RandomAddressType_t::NON_RESOLVABLE_PRIVATE;
            return BLE_ERROR_NONE;
        case 0x01:
            *type = RandomAddressType_t::RESOLVABLE_PRIVATE;
            return BLE_ERROR_NONE;
        default:
            return BLE_ERROR_INVALID_PARAM;
    }
}

template<class Impl>
LegacyGap<Impl>::ConnectionCallbackParams_t::ConnectionCallbackParams_t(
    Handle_t handleIn,
    Role_t roleIn,
    PeerAddressType_t peerAddrTypeIn,
    const uint8_t *peerAddrIn,
    BLEProtocol::AddressType_t ownAddrTypeIn,
    const uint8_t *ownAddrIn,
    const ConnectionParams_t *connectionParamsIn,
    const uint8_t *peerResolvableAddrIn,
    const uint8_t *localResolvableAddrIn
) : handle(handleIn),
    role(roleIn),
    peerAddr(),
    ownAddrType(ownAddrTypeIn),
    connectionParams(connectionParamsIn),
    peerResolvableAddr(),
    localResolvableAddr(),
    peerAddressType(peerAddrTypeIn)
{
    constructor_helper(
        peerAddrIn,
        ownAddrIn,
        peerResolvableAddrIn,
        localResolvableAddrIn
    );
}

template <class Impl>
void LegacyGap<Impl>::ConnectionCallbackParams_t::constructor_helper(
    const uint8_t *peerAddrIn,
    const uint8_t *ownAddrIn,
    const uint8_t *peerResolvableAddrIn,
    const uint8_t *localResolvableAddrIn
) {
    memcpy(peerAddr, peerAddrIn, ADDR_LEN);

    if (peerResolvableAddrIn) {
        memcpy(peerResolvableAddr, peerResolvableAddrIn, ADDR_LEN);
    }

    if (localResolvableAddrIn) {
        memcpy(localResolvableAddr, localResolvableAddrIn, ADDR_LEN);
    }
}

template<class Impl>
void LegacyGap<Impl>::processAdvertisementReport(
    const BLEProtocol::AddressBytes_t peerAddr,
    int8_t rssi,
    bool isScanResponse,
    GapAdvertisingParams::AdvertisingType_t type,
    uint8_t advertisingDataLen,
    const uint8_t *advertisingData,
    PeerAddressType_t addressType
) {
    AdvertisementCallbackParams_t params;

    memcpy(params.peerAddr, peerAddr, ADDR_LEN);
    params.rssi = rssi;
    params.isScanResponse = isScanResponse;
    params.type = type;
    params.advertisingDataLen = advertisingDataLen;
    params.advertisingData = advertisingData;

    onAdvertisementReport.call(&params);
}

#if BLE_ROLE_BROADCASTER
template<class Impl>
uint16_t LegacyGap<Impl>::getMinAdvertisingInterval(void) const
{
    return impl()->getMinAdvertisingInterval_();
}

template<class Impl>
uint16_t LegacyGap<Impl>::getMinNonConnectableAdvertisingInterval(void) const
{
    return impl()->getMinNonConnectableAdvertisingInterval_();
}

template<class Impl>
uint16_t LegacyGap<Impl>::getMaxAdvertisingInterval(void) const
{
    return impl()->getMaxAdvertisingInterval_();
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_FEATURE_CONNECTABLE

template<class Impl>
ble_error_t LegacyGap<Impl>::getPreferredConnectionParams(ConnectionParams_t *params)
{
    return impl()->getPreferredConnectionParams_(params);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setPreferredConnectionParams(
    const ConnectionParams_t *params
)
{
    return impl()->setPreferredConnectionParams_(params);
}

#endif // BLE_FEATURE_CONNECTABLE

#if BLE_FEATURE_GATT_SERVER

template<class Impl>
ble_error_t LegacyGap<Impl>::setDeviceName(const uint8_t *deviceName)
{
    return impl()->setDeviceName_(deviceName);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    return impl()->getDeviceName_(deviceName, lengthP);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setAppearance(GapAdvertisingData::Appearance appearance)
{
    return impl()->setAppearance_(appearance);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    return impl()->getAppearance_(appearanceP);
}

#endif // BLE_FEATURE_GATT_SERVER

template<class Impl>
ble_error_t LegacyGap<Impl>::reset(void)
{
    return impl()->reset_();
}

template<class Impl>
LegacyGap<Impl>::LegacyGap() :
    _advParams(),
    _advPayload(),
    _scanningParams(),
    _scanResponse(),
    connectionCount(0),
    state(),
    scanningActive(false),
    timeoutCallbackChain(),
    radioNotificationCallback(),
    onAdvertisementReport(),
    connectionCallChain(),
    disconnectionCallChain()
{
#if BLE_ROLE_BROADCASTER
    _advPayload.clear();
    _scanResponse.clear();
#endif // BLE_ROLE_BROADCASTER
}

template<class Impl>
void LegacyGap<Impl>::processDisconnectionEvent(Handle_t handle, DisconnectionReason_t reason)
{
    /* Update Gap state */
    --connectionCount;
    if (!connectionCount) {
        state.connected = 0;
    }

    DisconnectionCallbackParams_t callbackParams(handle, reason);
    disconnectionCallChain.call(&callbackParams);
}

template<class Impl>
void LegacyGap<Impl>::processTimeoutEvent(TimeoutSource_t source)
{
    if (source == TIMEOUT_SRC_ADVERTISING) {
        /* Update gap state if the source is an advertising timeout */
        state.advertising = 0;
    }
    if (timeoutCallbackChain) {
        timeoutCallbackChain(source);
    }
}


template<class Impl>
uint16_t LegacyGap<Impl>::getMinAdvertisingInterval_(void) const
{
    return 0;
}

template<class Impl>
uint16_t LegacyGap<Impl>::getMinNonConnectableAdvertisingInterval_(void) const
{
    return 0;
}

template<class Impl>
uint16_t LegacyGap<Impl>::getMaxAdvertisingInterval_(void) const
{
    return 0xFFFF;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::reset_(void)
{
    /* Notify that the instance is about to shut down */
    shutdownCallChain.call(this);
    shutdownCallChain.clear();

    /* Clear Gap state */
    state.advertising = 0;
    state.connected = 0;
#if BLE_FEATURE_CONNECTABLE
    connectionCount = 0;
#endif

    /* Clear scanning state */
#if BLE_ROLE_OBSERVER
    scanningActive = false;
#endif

#if BLE_ROLE_BROADCASTER
    /* Clear advertising and scanning data */
    _advPayload.clear();
    _scanResponse.clear();
#endif // BLE_ROLE_BROADCASTER

    /* Clear callbacks */
    timeoutCallbackChain.clear();
#if BLE_FEATURE_CONNECTABLE
    connectionCallChain.clear();
    disconnectionCallChain.clear();
#endif // BLE_FEATURE_CONNECTABLE
    radioNotificationCallback = NULL;
#if BLE_ROLE_OBSERVER
    onAdvertisementReport = NULL;
#endif
    this->_eventHandler = NULL;

    return BLE_ERROR_NONE;
}

// -----------------------------------------------------------------------------
/* ------------------------- Default implementations ------------------------ */
// -----------------------------------------------------------------------------

template<class Impl>
uint8_t LegacyGap<Impl>::getMaxWhitelistSize_(void) const {
    return 0;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getWhitelist_(Whitelist_t &whitelist) const {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setWhitelist_(const Whitelist_t &whitelist) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getAddress_(
    BLEProtocol::AddressType_t *typeP,
    BLEProtocol::AddressBytes_t address
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getPreferredConnectionParams_(ConnectionParams_t *params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setPreferredConnectionParams_(
    const ConnectionParams_t *params
)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setDeviceName_(const uint8_t *deviceName)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getDeviceName_(uint8_t *deviceName, unsigned *lengthP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setAppearance_(GapAdvertisingData::Appearance appearance)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getAppearance_(GapAdvertisingData::Appearance *appearanceP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // namespace interface
} // namespace ble
