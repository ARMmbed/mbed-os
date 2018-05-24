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

}

const Gap::PeripheralPrivacyConfiguration_t Gap::default_peripheral_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ PeripheralPrivacyConfiguration_t::PERFORM_PAIRING_PROCEDURE
};

const Gap::CentralPrivacyConfiguration_t Gap::default_central_privacy_configuration = {
    /* use_non_resolvable_random_address */ false,
    /* resolution_strategy */ CentralPrivacyConfiguration_t::DO_NOT_RESOLVE
};

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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__CC_ARM)
#pragma push
#pragma diag_suppress 1361
#endif

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

#if defined(__GNUC__) && !defined(__CC_ARM)
#pragma GCC diagnostic pop
#elif defined(__CC_ARM)
#pragma pop
#endif
