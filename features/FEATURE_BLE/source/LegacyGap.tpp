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

namespace {
ble_error_t convert_address_type(
    ble::peer_address_type_t input_type,
    const BLEProtocol::AddressBytes_t address,
    BLEProtocol::AddressType_t& output_type
) {
    typedef ble::random_address_type_t RandomAddressType_t;
    typedef ble::peer_address_type_t PeerAddressType_t;
    typedef BLEProtocol::AddressType LegacyAddressType_t;

    // best effort; peerAddrTypeIn should not be used when privacy is on.
    switch(input_type.value()) {
        case PeerAddressType_t::PUBLIC:
        case PeerAddressType_t::PUBLIC_IDENTITY:
            output_type = LegacyAddressType_t::PUBLIC;
            break;
        case PeerAddressType_t::RANDOM: {
            RandomAddressType_t random_address_type(RandomAddressType_t::STATIC);
            ble_error_t err = Gap::getRandomAddressType(address, &random_address_type);
            if (err) {
                return err;
            }
            switch (random_address_type.value()) {
                case RandomAddressType_t::STATIC:
                    output_type = LegacyAddressType_t::RANDOM_STATIC;
                    break;
                case RandomAddressType_t::NON_RESOLVABLE_PRIVATE:
                    output_type = LegacyAddressType_t::RANDOM_PRIVATE_NON_RESOLVABLE;
                    break;
                case RandomAddressType_t::RESOLVABLE_PRIVATE:
                    output_type = LegacyAddressType_t::RANDOM_PRIVATE_RESOLVABLE;
                    break;
            }
            break;
        }
        case PeerAddressType_t::RANDOM_STATIC_IDENTITY:
            output_type = LegacyAddressType_t::RANDOM_STATIC;
            break;
    }

    return BLE_ERROR_NONE;
}

ble::peer_address_type_t convert_legacy_address_type(
    BLEProtocol::AddressType_t legacy_address
) {
    if (legacy_address == BLEProtocol::AddressType::PUBLIC) {
        return ble::peer_address_type_t::PUBLIC;
    } else {
        return ble::peer_address_type_t::RANDOM;
    }
}

} // end of anonymous namespace

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
ble_error_t LegacyGap<Impl>::setAddress(
    BLEProtocol::AddressType_t type,
    const BLEProtocol::AddressBytes_t address
) {
    return impl()->setAddress_(type, address);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::getAddress(
    BLEProtocol::AddressType_t *typeP,
    BLEProtocol::AddressBytes_t address
) {
    return impl()->getAddress_(typeP, address);
}

#if BLE_ROLE_BROADCASTER
template<class Impl>
ble_error_t LegacyGap<Impl>::stopAdvertising(void) {
    return impl()->stopAdvertising_();
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_CENTRAL
template<class Impl>
ble_error_t LegacyGap<Impl>::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    PeerAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    return impl()->connect_(
        peerAddr,
        peerAddrType,
        connectionParams,
        scanParams
    );
}

template<class Impl>
ble_error_t LegacyGap<Impl>::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    return impl()->connect_(
        peerAddr,
        peerAddrType,
        connectionParams,
        scanParams
    );
}
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_CONNECTABLE
template<class Impl>
ble_error_t LegacyGap<Impl>::disconnect(
    Handle_t connectionHandle, DisconnectionReason_t reason
) {
    return impl()->disconnect_(connectionHandle, reason);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::disconnect(DisconnectionReason_t reason) {
    return impl()->disconnect_(reason);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::updateConnectionParams(
    Handle_t handle,
    const ConnectionParams_t *params
) {
    return impl()->updateConnectionParams_(handle, params);
}
#endif // BLE_FEATURE_CONNECTABLE

template<class Impl>
ble_error_t LegacyGap<Impl>::setTxPower(int8_t txPower) {
    return impl()->setTxPower_(txPower);
}

template<class Impl>
void LegacyGap<Impl>::getPermittedTxPowerValues(
    const int8_t **valueArrayPP, size_t *countP
) {
    return impl()->getPermittedTxPowerValues_(valueArrayPP, countP);
}

#if BLE_FEATURE_WHITELIST
#if BLE_ROLE_BROADCASTER
template<class Impl>
ble_error_t LegacyGap<Impl>::setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode) {
    return impl()->setAdvertisingPolicyMode_(mode);
}

template<class Impl>
typename LegacyGap<Impl>::AdvertisingPolicyMode_t
LegacyGap<Impl>::getAdvertisingPolicyMode(void) const {
    return impl()->getAdvertisingPolicyMode_();
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
template<class Impl>
ble_error_t LegacyGap<Impl>::setScanningPolicyMode(ScanningPolicyMode_t mode) {
    return impl()->setScanningPolicyMode_(mode);
}

template<class Impl>
typename LegacyGap<Impl>::ScanningPolicyMode_t
LegacyGap<Impl>::getScanningPolicyMode(void) const {
    return impl()->getScanningPolicyMode_();
}
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL
template<class Impl>
ble_error_t LegacyGap<Impl>::setInitiatorPolicyMode(InitiatorPolicyMode_t mode) {
    return impl()->setInitiatorPolicyMode_(mode);
}

template<class Impl>
typename LegacyGap<Impl>::InitiatorPolicyMode_t
LegacyGap<Impl>::getInitiatorPolicyMode(void) const {
    return impl()->getInitiatorPolicyMode_();
}
#endif // BLE_ROLE_CENTRAL
#endif // BLE_FEATURE_WHITELIST

#if BLE_ROLE_OBSERVER
template<class Impl>
ble_error_t LegacyGap<Impl>::startRadioScan(const GapScanningParams &scanningParams) {
    return impl()->startRadioScan_(scanningParams);
}
#endif // BLE_ROLE_OBSERVER

template<class Impl>
ble_error_t LegacyGap<Impl>::initRadioNotification(void) {
    return impl()->initRadioNotification_();
}

/* -------- deprecated template implementation -------- */

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

template<class Impl>
typename LegacyGap<Impl>::GapState_t LegacyGap<Impl>::getState(void) const
{
    return state;
}

#if BLE_ROLE_BROADCASTER
template<class Impl>
void LegacyGap<Impl>::setAdvertisingType(GapAdvertisingParams::AdvertisingType_t advType)
{
    _advParams.setAdvertisingType(advType);
}

template<class Impl>
void LegacyGap<Impl>::setAdvertisingInterval(uint16_t interval)
{
    if (interval == 0) {
        stopAdvertising();
    } else if (interval < getMinAdvertisingInterval()) {
        interval = getMinAdvertisingInterval();
    }
    _advParams.setInterval(interval);
}

template<class Impl>
void LegacyGap<Impl>::setAdvertisingTimeout(uint16_t timeout)
{
    _advParams.setTimeout(timeout);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::startAdvertising(void)
{
    ble_error_t rc;
    if ((rc = startAdvertising(_advParams)) == BLE_ERROR_NONE) {
        state.advertising = 1;
    }
    return rc;
}

template<class Impl>
void LegacyGap<Impl>::clearAdvertisingPayload(void)
{
    _advPayload.clear();
    setAdvertisingData(_advPayload, _scanResponse);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::accumulateAdvertisingPayload(uint8_t flags)
{
    GapAdvertisingData advPayloadCopy = _advPayload;
    ble_error_t rc;
    if ((rc = advPayloadCopy.addFlags(flags)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(advPayloadCopy, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = advPayloadCopy;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::accumulateAdvertisingPayload(GapAdvertisingData::Appearance app)
{
    GapAdvertisingData advPayloadCopy = _advPayload;
    ble_error_t rc;
    if ((rc = advPayloadCopy.addAppearance(app)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(advPayloadCopy, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = advPayloadCopy;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::accumulateAdvertisingPayloadTxPower(int8_t power)
{
    GapAdvertisingData advPayloadCopy = _advPayload;
    ble_error_t rc;
    if ((rc = advPayloadCopy.addTxPower(power)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(advPayloadCopy, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = advPayloadCopy;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::accumulateAdvertisingPayload(
    GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
) {
    GapAdvertisingData advPayloadCopy = _advPayload;
    ble_error_t rc;
    if ((rc = advPayloadCopy.addData(type, data, len)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(advPayloadCopy, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = advPayloadCopy;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::updateAdvertisingPayload(
    GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
) {
    GapAdvertisingData advPayloadCopy = _advPayload;
    ble_error_t rc;
    if ((rc = advPayloadCopy.updateData(type, data, len)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(advPayloadCopy, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = advPayloadCopy;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setAdvertisingPayload(const GapAdvertisingData &payload)
{
    ble_error_t rc = setAdvertisingData(payload, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = payload;
    }

    return rc;
}

template<class Impl>
const GapAdvertisingData &LegacyGap<Impl>::getAdvertisingPayload(void) const
{
    return _advPayload;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::accumulateScanResponse(
    GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
) {
    GapAdvertisingData scanResponseCopy = _scanResponse;
    ble_error_t rc;
    if ((rc = scanResponseCopy.addData(type, data, len)) != BLE_ERROR_NONE) {
        return rc;
    }

    rc = setAdvertisingData(_advPayload, scanResponseCopy);
    if (rc == BLE_ERROR_NONE) {
        _scanResponse = scanResponseCopy;
    }

    return rc;
}

template<class Impl>
void LegacyGap<Impl>::clearScanResponse(void)
{
    _scanResponse.clear();
    setAdvertisingData(_advPayload, _scanResponse);
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
template<class Impl>
ble_error_t LegacyGap<Impl>::setScanParams(
    uint16_t interval,
    uint16_t window,
    uint16_t timeout,
    bool activeScanning
) {
    ble_error_t rc;
    if (((rc = _scanningParams.setInterval(interval)) == BLE_ERROR_NONE) &&
        ((rc = _scanningParams.setWindow(window))     == BLE_ERROR_NONE) &&
        ((rc = _scanningParams.setTimeout(timeout))   == BLE_ERROR_NONE)) {
        _scanningParams.setActiveScanning(activeScanning);
        return BLE_ERROR_NONE;
    }

    return rc;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setScanParams(const GapScanningParams& scanningParams) {
    return setScanParams(
        scanningParams.getInterval(),
        scanningParams.getWindow(),
        scanningParams.getTimeout(),
        scanningParams.getActiveScanning()
    );
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setScanInterval(uint16_t interval)
{
    return _scanningParams.setInterval(interval);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setScanWindow(uint16_t window)
{
    ble_error_t rc;
    if ((rc = _scanningParams.setWindow(window)) != BLE_ERROR_NONE) {
        return rc;
    }

    /* If scanning is already active, propagate the new setting to the stack. */
    if (scanningActive) {
        return startRadioScan(_scanningParams);
    }

    return BLE_ERROR_NONE;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setScanTimeout(uint16_t timeout)
{
    ble_error_t rc;
    if ((rc = _scanningParams.setTimeout(timeout)) != BLE_ERROR_NONE) {
        return rc;
    }

    /* If scanning is already active, propagate the new settings to the stack. */
    if (scanningActive) {
        return startRadioScan(_scanningParams);
    }

    return BLE_ERROR_NONE;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setActiveScanning(bool activeScanning)
{
    _scanningParams.setActiveScanning(activeScanning);

    /* If scanning is already active, propagate the new settings to the stack. */
    if (scanningActive) {
        return startRadioScan(_scanningParams);
    }

    return BLE_ERROR_NONE;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::startScan(
    void (*callback)(const AdvertisementCallbackParams_t *params)
) {
    ble_error_t err = BLE_ERROR_NONE;
    if (callback) {
        if ((err = startRadioScan(_scanningParams)) == BLE_ERROR_NONE) {
            scanningActive = true;
            onAdvertisementReport.attach(callback);
        }
    }

    return err;
}
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_BROADCASTER
template<class Impl>
GapAdvertisingParams &LegacyGap<Impl>::getAdvertisingParams(void)
{
    return _advParams;
}

template<class Impl>
const GapAdvertisingParams &LegacyGap<Impl>::getAdvertisingParams(void) const
{
    return _advParams;
}

template<class Impl>
void LegacyGap<Impl>::setAdvertisingParams(const GapAdvertisingParams &newParams)
{
    _advParams = newParams;
}
#endif // BLE_ROLE_BROADCASTER

template<class Impl>
void LegacyGap<Impl>::onTimeout(TimeoutEventCallback_t callback)
{
    timeoutCallbackChain.add(callback);
}

template<class Impl>
typename LegacyGap<Impl>::TimeoutEventCallbackChain_t& LegacyGap<Impl>::onTimeout()
{
    return timeoutCallbackChain;
}

#if BLE_FEATURE_CONNECTABLE
template<class Impl>
void LegacyGap<Impl>::onConnection(ConnectionEventCallback_t callback)
{
    connectionCallChain.add(callback);
}

template<class Impl>
typename LegacyGap<Impl>::ConnectionEventCallbackChain_t& LegacyGap<Impl>::onConnection()
{
    return connectionCallChain;
}

template<class Impl>
void LegacyGap<Impl>::onDisconnection(DisconnectionEventCallback_t callback)
{
    disconnectionCallChain.add(callback);
}

template<class Impl>
typename LegacyGap<Impl>::DisconnectionEventCallbackChain_t& LegacyGap<Impl>::onDisconnection()
{
    return disconnectionCallChain;
}
#endif // BLE_FEATURE_CONNECTABLE

template<class Impl>
void LegacyGap<Impl>::onRadioNotification(void (*callback)(bool param))
{
    radioNotificationCallback.attach(callback);
}

template<class Impl>
void LegacyGap<Impl>::onShutdown(const GapShutdownCallback_t& callback)
{
    shutdownCallChain.add(callback);
}

template<class Impl>
typename LegacyGap<Impl>::GapShutdownCallbackChain_t& LegacyGap<Impl>::onShutdown()
{
    return shutdownCallChain;
}

template<class Impl>
LegacyGap<Impl>::AdvertisementCallbackParams_t::AdvertisementCallbackParams_t() :
    peerAddr(),
    rssi(),
    isScanResponse(),
    type(),
    advertisingDataLen(0),
    advertisingData(NULL),
    addressType(),
    peerAddrType(PeerAddressType_t::PUBLIC)
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
    BLEProtocol::AddressType_t peerAddrTypeIn,
    const uint8_t *peerAddrIn,
    BLEProtocol::AddressType_t ownAddrTypeIn,
    const uint8_t *ownAddrIn,
    const ConnectionParams_t *connectionParamsIn,
    const uint8_t *peerResolvableAddrIn,
    const uint8_t *localResolvableAddrIn
) : handle(handleIn),
    role(roleIn),
    peerAddrType(peerAddrTypeIn),
    peerAddr(),
    ownAddrType(ownAddrTypeIn),
    ownAddr(),
    connectionParams(connectionParamsIn),
    peerResolvableAddr(),
    localResolvableAddr(),
    peerAddressType(convert_legacy_address_type(peerAddrTypeIn))
{
    constructor_helper(
        peerAddrIn,
        ownAddrIn,
        peerResolvableAddrIn,
        localResolvableAddrIn
    );
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
    peerAddrType(),
    peerAddr(),
    ownAddrType(ownAddrTypeIn),
    ownAddr(),
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

    convert_address_type(peerAddrTypeIn, peerAddrIn, peerAddrType);
}

template <class Impl>
void LegacyGap<Impl>::ConnectionCallbackParams_t::constructor_helper(
    const uint8_t *peerAddrIn,
    const uint8_t *ownAddrIn,
    const uint8_t *peerResolvableAddrIn,
    const uint8_t *localResolvableAddrIn
) {
    memcpy(peerAddr, peerAddrIn, ADDR_LEN);

    if (ownAddrIn) {
        memcpy(ownAddr, ownAddrIn, ADDR_LEN);
    } else {
        memset(ownAddr, 0, ADDR_LEN);
    }

    if (peerResolvableAddrIn) {
        memcpy(peerResolvableAddr, peerResolvableAddrIn, ADDR_LEN);
    } else {
        memset(ownAddr, 0, ADDR_LEN);
    }

    if (localResolvableAddrIn) {
        memcpy(localResolvableAddr, localResolvableAddrIn, ADDR_LEN);
    } else {
        memset(ownAddr, 0, ADDR_LEN);
    }
}

#if BLE_ROLE_CENTRAL
template<class Impl>
ble_error_t LegacyGap<Impl>::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    DeprecatedAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    return connect(
        peerAddr,
        (BLEProtocol::AddressType_t) peerAddrType,
        connectionParams,
        scanParams
    );
}
#endif // BLE_ROLE_CENTRAL

template<class Impl>
void LegacyGap<Impl>::processConnectionEvent(
    Handle_t handle,
    Role_t role,
    BLEProtocol::AddressType_t peerAddrType,
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
void LegacyGap<Impl>::processAdvertisementReport(
    const BLEProtocol::AddressBytes_t peerAddr,
    int8_t rssi,
    bool isScanResponse,
    GapAdvertisingParams::AdvertisingType_t type,
    uint8_t advertisingDataLen,
    const uint8_t *advertisingData,
    BLEProtocol::AddressType_t addressType
) {
    AdvertisementCallbackParams_t params;
    memcpy(params.peerAddr, peerAddr, ADDR_LEN);
    params.rssi = rssi;
    params.isScanResponse = isScanResponse;
    params.type = type;
    params.advertisingDataLen = advertisingDataLen;
    params.advertisingData = advertisingData;
    params.addressType = addressType;

    params.peerAddrType = convert_legacy_address_type(addressType);
    onAdvertisementReport.call(&params);
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
    params.peerAddrType = addressType;

    convert_address_type(
        addressType,
        peerAddr,
        params.addressType
    );

    onAdvertisementReport.call(&params);
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

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

#if BLE_ROLE_BROADCASTER
template<class Impl>
ble_error_t LegacyGap<Impl>::setAdvertisingData(
    const GapAdvertisingData &advData,
    const GapAdvertisingData &scanResponse
) {
    return impl()->setAdvertisingData_(advData, scanResponse);
}

template<class Impl>
ble_error_t LegacyGap<Impl>::startAdvertising(const GapAdvertisingParams &params) {
    return impl()->startAdvertising_(params);
}
#endif // BLE_ROLE_BROADCASTER

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
ble_error_t LegacyGap<Impl>::setAddress_(
    BLEProtocol::AddressType_t type,
    const BLEProtocol::AddressBytes_t address
) {
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
ble_error_t LegacyGap<Impl>::stopAdvertising_(void) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::connect_(
    const BLEProtocol::AddressBytes_t peerAddr,
    PeerAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::connect_(
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::disconnect_(
    Handle_t connectionHandle, DisconnectionReason_t reason
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::disconnect_(DisconnectionReason_t reason) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::updateConnectionParams_(
    Handle_t handle,
    const ConnectionParams_t *params
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}


template<class Impl>
ble_error_t LegacyGap<Impl>::setTxPower_(int8_t txPower) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
void LegacyGap<Impl>::getPermittedTxPowerValues_(
    const int8_t **valueArrayPP, size_t *countP
) {
    *countP = 0;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setAdvertisingPolicyMode_(AdvertisingPolicyMode_t mode) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setScanningPolicyMode_(ScanningPolicyMode_t mode) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::setInitiatorPolicyMode_(InitiatorPolicyMode_t mode) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
typename LegacyGap<Impl>::AdvertisingPolicyMode_t
LegacyGap<Impl>::getAdvertisingPolicyMode_(void) const {
    return LegacyGap::ADV_POLICY_IGNORE_WHITELIST;
}

template<class Impl>
typename LegacyGap<Impl>::ScanningPolicyMode_t
LegacyGap<Impl>::getScanningPolicyMode_(void) const {
    return LegacyGap::SCAN_POLICY_IGNORE_WHITELIST;
}

template<class Impl>
typename LegacyGap<Impl>::InitiatorPolicyMode_t
LegacyGap<Impl>::getInitiatorPolicyMode_(void) const {
    return LegacyGap::INIT_POLICY_IGNORE_WHITELIST;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::startRadioScan_(const GapScanningParams &scanningParams) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::initRadioNotification_(void) {
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

template<class Impl>
ble_error_t LegacyGap<Impl>::setAdvertisingData_(
    const GapAdvertisingData &advData,
    const GapAdvertisingData &scanResponse
) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class Impl>
ble_error_t LegacyGap<Impl>::startAdvertising_(const GapAdvertisingParams &params) {
    return BLE_ERROR_NOT_IMPLEMENTED;
}

} // namespace interface
} // namespace ble
