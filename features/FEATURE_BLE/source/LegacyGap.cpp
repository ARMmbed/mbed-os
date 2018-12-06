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
    Gap::PeerAddressType_t input_type,
    const BLEProtocol::AddressBytes_t address,
    BLEProtocol::AddressType_t& output_type
) {
    typedef Gap::RandomAddressType_t RandomAddressType_t;
    typedef Gap::PeerAddressType_t PeerAddressType_t;
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

Gap::PeerAddressType_t convert_legacy_address_type(
    BLEProtocol::AddressType_t legacy_address
) {
    if (legacy_address == BLEProtocol::AddressType::PUBLIC) {
        return Gap::PeerAddressType_t::PUBLIC;
    } else {
        return Gap::PeerAddressType_t::RANDOM;
    }
}

} // end of anonymous namespace

uint8_t Gap::getMaxWhitelistSize(void) const
{
    return 0;
}

ble_error_t Gap::getWhitelist(Whitelist_t &whitelist) const
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setWhitelist(const Whitelist_t &whitelist)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void Gap::processConnectionEvent(
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

ble_error_t Gap::setAddress(
    BLEProtocol::AddressType_t type,
    const BLEProtocol::AddressBytes_t address
) {
    /* avoid compiler warnings about unused variables */
    (void)type;
    (void)address;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::getAddress(
    BLEProtocol::AddressType_t *typeP,
    BLEProtocol::AddressBytes_t address
) {
    /* Avoid compiler warnings about unused variables. */
    (void)typeP;
    (void)address;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::stopAdvertising(void)
{
    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    PeerAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    /* Avoid compiler warnings about unused variables. */
    (void)peerAddr;
    (void)peerAddrType;
    (void)connectionParams;
    (void)scanParams;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
) {
    /* Avoid compiler warnings about unused variables. */
    (void)peerAddr;
    (void)peerAddrType;
    (void)connectionParams;
    (void)scanParams;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::disconnect(
    Handle_t connectionHandle, DisconnectionReason_t reason
) {
    /* avoid compiler warnings about unused variables */
    (void)connectionHandle;
    (void)reason;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::disconnect(DisconnectionReason_t reason) {
    /* Avoid compiler warnings about unused variables. */
    (void)reason;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::updateConnectionParams(
    Handle_t handle,
    const ConnectionParams_t *params
) {
    /* avoid compiler warnings about unused variables */
    (void)handle;
    (void)params;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setTxPower(int8_t txPower)
{
    /* Avoid compiler warnings about unused variables. */
    (void)txPower;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void Gap::getPermittedTxPowerValues(
    const int8_t **valueArrayPP, size_t *countP
)  {
    /* Avoid compiler warnings about unused variables. */
    (void)valueArrayPP;

    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    *countP = 0;
}

ble_error_t Gap::setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode)
{
    (void) mode;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setScanningPolicyMode(ScanningPolicyMode_t mode)
{
    (void) mode;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setInitiatorPolicyMode(InitiatorPolicyMode_t mode)
{
    (void) mode;
    return BLE_ERROR_NOT_IMPLEMENTED;
}

Gap::AdvertisingPolicyMode_t Gap::getAdvertisingPolicyMode(void) const
{
    return ADV_POLICY_IGNORE_WHITELIST;
}

Gap::ScanningPolicyMode_t Gap::getScanningPolicyMode(void) const
{
    return SCAN_POLICY_IGNORE_WHITELIST;
}

Gap::InitiatorPolicyMode_t Gap::getInitiatorPolicyMode(void) const
{
    return INIT_POLICY_IGNORE_WHITELIST;
}

ble_error_t Gap::startRadioScan(const GapScanningParams &scanningParams)
{
    (void)scanningParams;
    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::initRadioNotification(void)
{
    /* Requesting action from porter(s): override this API if this capability
       is supported. */
    return BLE_ERROR_NOT_IMPLEMENTED;
}


#if defined(__GNUC__) && !defined(__CC_ARM)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__CC_ARM)
#pragma push
#pragma diag_suppress 1361
#endif

Gap::GapState_t Gap::getState(void) const
{
    return state;
}

void Gap::setAdvertisingType(GapAdvertisingParams::AdvertisingType_t advType)
{
    _advParams.setAdvertisingType(advType);
}

void Gap::setAdvertisingInterval(uint16_t interval)
{
    if (interval == 0) {
        stopAdvertising();
    } else if (interval < getMinAdvertisingInterval()) {
        interval = getMinAdvertisingInterval();
    }
    _advParams.setInterval(interval);
}

void Gap::setAdvertisingTimeout(uint16_t timeout)
{
    _advParams.setTimeout(timeout);
}

ble_error_t Gap::startAdvertising(void)
{
    ble_error_t rc;
    if ((rc = startAdvertising(_advParams)) == BLE_ERROR_NONE) {
        state.advertising = 1;
    }
    return rc;
}

void Gap::clearAdvertisingPayload(void)
{
    _advPayload.clear();
    setAdvertisingData(_advPayload, _scanResponse);
}

ble_error_t Gap::accumulateAdvertisingPayload(uint8_t flags)
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

ble_error_t Gap::accumulateAdvertisingPayload(GapAdvertisingData::Appearance app)
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

ble_error_t Gap::accumulateAdvertisingPayloadTxPower(int8_t power)
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

ble_error_t Gap::accumulateAdvertisingPayload(
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

ble_error_t Gap::updateAdvertisingPayload(
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

ble_error_t Gap::setAdvertisingPayload(const GapAdvertisingData &payload)
{
    ble_error_t rc = setAdvertisingData(payload, _scanResponse);
    if (rc == BLE_ERROR_NONE) {
        _advPayload = payload;
    }

    return rc;
}

const GapAdvertisingData &Gap::getAdvertisingPayload(void) const
{
    return _advPayload;
}

ble_error_t Gap::accumulateScanResponse(
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

void Gap::clearScanResponse(void)
{
    _scanResponse.clear();
    setAdvertisingData(_advPayload, _scanResponse);
}

ble_error_t Gap::setScanParams(
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

ble_error_t Gap::setScanParams(const GapScanningParams& scanningParams) {
    return setScanParams(
        scanningParams.getInterval(),
        scanningParams.getWindow(),
        scanningParams.getTimeout(),
        scanningParams.getActiveScanning()
    );
}

ble_error_t Gap::setScanInterval(uint16_t interval)
{
    return _scanningParams.setInterval(interval);
}

ble_error_t Gap::setScanWindow(uint16_t window)
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

ble_error_t Gap::setScanTimeout(uint16_t timeout)
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

ble_error_t Gap::setActiveScanning(bool activeScanning)
{
    _scanningParams.setActiveScanning(activeScanning);

    /* If scanning is already active, propagate the new settings to the stack. */
    if (scanningActive) {
        return startRadioScan(_scanningParams);
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::startScan(
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

GapAdvertisingParams &Gap::getAdvertisingParams(void)
{
    return _advParams;
}

const GapAdvertisingParams &Gap::getAdvertisingParams(void) const
{
    return _advParams;
}

void Gap::setAdvertisingParams(const GapAdvertisingParams &newParams)
{
    _advParams = newParams;
}

void Gap::onTimeout(TimeoutEventCallback_t callback)
{
    timeoutCallbackChain.add(callback);
}

Gap::TimeoutEventCallbackChain_t& Gap::onTimeout()
{
    return timeoutCallbackChain;
}

void Gap::onConnection(ConnectionEventCallback_t callback)
{
    connectionCallChain.add(callback);
}

Gap::ConnectionEventCallbackChain_t& Gap::onConnection()
{
    return connectionCallChain;
}

void Gap::onDisconnection(DisconnectionEventCallback_t callback)
{
    disconnectionCallChain.add(callback);
}

Gap::DisconnectionEventCallbackChain_t& Gap::onDisconnection()
{
    return disconnectionCallChain;
}

void Gap::onRadioNotification(void (*callback)(bool param))
{
    radioNotificationCallback.attach(callback);
}

void Gap::onShutdown(const GapShutdownCallback_t& callback)
{
    shutdownCallChain.add(callback);
}

Gap::GapShutdownCallbackChain_t& Gap::onShutdown()
{
    return shutdownCallChain;
}


Gap::AdvertisementCallbackParams_t::AdvertisementCallbackParams_t() :
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

ble_error_t Gap::getRandomAddressType(
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

Gap::ConnectionCallbackParams_t::ConnectionCallbackParams_t(
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

Gap::ConnectionCallbackParams_t::ConnectionCallbackParams_t(
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

void Gap::ConnectionCallbackParams_t::constructor_helper(
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

ble_error_t Gap::connect(
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

void Gap::processConnectionEvent(
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

void Gap::processAdvertisementReport(
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

void Gap::processAdvertisementReport(
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

#if defined(__GNUC__) && !defined(__CC_ARM)
#pragma GCC diagnostic pop
#elif defined(__CC_ARM)
#pragma pop
#endif
