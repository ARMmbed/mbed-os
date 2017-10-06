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

#include <algorithm>

#include "CordioGap.h"
#include "mbed.h"
#include "dm_api.h"
#include "CordioGattServer.h"
#include "hci_core.h"

/**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020

/**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0

/**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000

namespace ble {
namespace vendor {
namespace cordio {

Gap &Gap::getInstance()
{
    static Gap m_instance;
    return m_instance;
}

void Gap::initialize()
{
    uint8_t whitelist_size = HciGetWhiteListSize();

    if (whitelist_size == 0) {
        return;
    }

    whitelist.addresses = new(std::nothrow) BLEProtocol::Address_t[whitelist_size];
    if (whitelist.addresses == NULL) {
        return;
    }

    whitelist.size = 0;
    whitelist.capacity = hciCoreCb.whiteListSize;
}

ble_error_t Gap::setAddress(AddressType_t type, const Address_t address)
{
    switch (type) {
        case BLEProtocol::AddressType::PUBLIC:
            // TODO: use vendor specific commands from the driver
            return BLE_ERROR_OPERATION_NOT_PERMITTED;

        // See bluetooth 5, Vol 6 part, part B, 1.3.2
        case BLEProtocol::AddressType::RANDOM_STATIC:
            if ((address[5] >> 6) != 3) {
                return BLE_ERROR_PARAM_OUT_OF_RANGE;
            }

            m_type = type;
            BdaCpy(m_addr, address);
            DmDevSetRandAddr(m_addr);
            break;

        // should not be here, generation is supposed to be handled by the controller.
        case BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE:
        case BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE:
            m_type = type;
            return BLE_ERROR_NONE;

        default:
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    DmAdvSetAddrType(m_type);
    DmConnSetAddrType(m_type);
    DmScanSetAddrType(m_type);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::getAddress(AddressType_t *typeP, Address_t address)
{
    *typeP = m_type;

    if(m_type == BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE ||
       m_type == BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE) {
        return BLE_ERROR_NONE;
    }

    BdaCpy(address, m_addr);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* set advertising and scan response data for discoverable mode */
    DmAdvSetData(DM_ADV_HANDLE_DEFAULT, HCI_ADV_DATA_OP_COMP_FRAG, DM_DATA_LOC_ADV, advData.getPayloadLen(), (uint8_t*)advData.getPayload());
    DmAdvSetData(DM_ADV_HANDLE_DEFAULT, HCI_ADV_DATA_OP_COMP_FRAG, DM_DATA_LOC_SCAN, scanResponse.getPayloadLen(), (uint8_t*)scanResponse.getPayload());

    return BLE_ERROR_NONE;
}

ble_error_t Gap::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t peerAddrType,
    const ConnectionParams_t* connectionParams,
    const GapScanningParams* scanParams
) {
    // prepare the scan interval
    if (scanParams != NULL) {
        DmConnSetScanInterval(scanParams->getInterval(), scanParams->getWindow());
    }

    if (connectionParams != NULL) {
        hciConnSpec_t conn_spec = {
            /* connIntervalMin */ connectionParams->minConnectionInterval,
            /* connIntervalMax */ connectionParams->maxConnectionInterval,
            /* connLatency */ connectionParams->slaveLatency,
            /* supTimeout */ connectionParams->connectionSupervisionTimeout,
            /* minCeLen */ DM_GAP_CONN_EST_MIN_CE_LEN,
            /* maxCeLen */ DM_GAP_CONN_EST_MAX_CE_LEN
        };
        DmConnSetConnSpec(&conn_spec);
    }

    DmScanStop();
    dmConnId_t connection_id = DmConnOpen(
        DM_CLIENT_ID_APP,
        HCI_INIT_PHY_LE_1M_BIT,
        peerAddrType,
        (uint8_t*) peerAddr
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

uint16_t Gap::getMinAdvertisingInterval() const
{
    return BLE_GAP_ADV_INTERVAL_MIN;
}

uint16_t Gap::getMinNonConnectableAdvertisingInterval() const
{
    return BLE_GAP_ADV_NONCON_INTERVAL_MIN;
}

uint16_t Gap::getMaxAdvertisingInterval() const
{
    return BLE_GAP_ADV_INTERVAL_MAX;
}

ble_error_t Gap::startAdvertising(const GapAdvertisingParams &params)
{
    /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) {
        /* ToDo: This requires a proper security implementation, etc. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    } else {
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    /* Check timeout is zero for Connectable Directed */
    if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) && (params.getTimeout() != 0)) {
        /* Timeout must be 0 with this type, although we'll never get here */
        /* since this isn't implemented yet anyway */
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check timeout for other advertising types */
    if ((params.getAdvertisingType() != GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() > GapAdvertisingParams::GAP_ADV_PARAMS_TIMEOUT_MAX)) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    uint16_t adv_interval_min = params.getIntervalInADVUnits();
    if (adv_interval_min == GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX) {
        --adv_interval_min;
    }
    uint16_t adv_interval_max = adv_interval_min + 1;

    DmAdvSetInterval(DM_ADV_HANDLE_DEFAULT, adv_interval_min, adv_interval_max);

    /* Peer Addr Type 0 = Public */
    uint8_t peerAddrType = 0;
    uint8_t peerAddr[6] = { 0 };
    DmAdvConfig(DM_ADV_HANDLE_DEFAULT, params.getAdvertisingType(), peerAddrType, peerAddr);

    uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
    uint16_t adv_durations[] = { (uint16_t) (params.getTimeout() * 1000) };
    uint8_t max_ea_events[] = { 0 };
    DmAdvStart(1, adv_handles, adv_durations, max_ea_events);

    state.advertising = 1;

    return BLE_ERROR_NONE;
}

ble_error_t Gap::stopAdvertising(void)
{
    uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
    DmAdvStop(1, adv_handles);

    state.advertising = 0;

    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, m_connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t Gap::setDeviceName(const uint8_t *deviceName)
{
    return GattServer::getInstance().setDeviceName(deviceName);
}

ble_error_t Gap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    const uint8_t* name = NULL;
    uint16_t length = 0;

    GattServer::getInstance().getDeviceName(name, length);

    if (deviceName != NULL) {
        memcpy(deviceName, name, std::min((uint16_t) *lengthP, length));
    }

    *lengthP = length;
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    GattServer::getInstance().setAppearance(appearance);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    *appearanceP = GattServer::getInstance().getAppearance();
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setTxPower(int8_t txPower)
{
#if 0
    HciVsSetTxPower(txPower);
    return BLE_ERROR_NONE;
#else
    return BLE_ERROR_NOT_IMPLEMENTED;
#endif
}

void Gap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
    *valueArrayPP = NULL;
    *countP = 0;
}

void Gap::setConnectionHandle(uint16_t connectionHandle)
{
    m_connectionHandle = connectionHandle;
}

uint16_t Gap::getConnectionHandle(void)
{
    return m_connectionHandle;
}

ble_error_t Gap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    *params = GattServer::getInstance().getPreferredConnectionParams();
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    // ensure that parameters are correct
    // see BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part C]
    // section 12.3 PERIPHERAL PREFERRED CONNECTION PARAMETERS CHARACTERISTIC
    if (((0x0006 > params->minConnectionInterval) || (params->minConnectionInterval > 0x0C80)) &&
        params->minConnectionInterval != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (((params->minConnectionInterval > params->maxConnectionInterval) || (params->maxConnectionInterval > 0x0C80)) &&
        params->maxConnectionInterval != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (params->slaveLatency > 0x01F3) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (((0x000A > params->connectionSupervisionTimeout) || (params->connectionSupervisionTimeout > 0x0C80)) &&
        params->connectionSupervisionTimeout != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    GattServer::getInstance().setPreferredConnectionParams(*params);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *newParams)
{
    if (DmConnCheckIdle(handle) != 0) {
        return BLE_STACK_BUSY;
    }

    hciConnSpec_t connSpec;
    connSpec.connIntervalMin = newParams->minConnectionInterval;
    connSpec.connIntervalMax = newParams->maxConnectionInterval;
    connSpec.connLatency = newParams->slaveLatency;
    connSpec.supTimeout = newParams->connectionSupervisionTimeout;
    DmConnUpdate(handle, &connSpec);

    return BLE_ERROR_NONE;
}

ble_error_t Gap::startRadioScan(const GapScanningParams &scanningParams)
{
    // not needed to start scanning if the whitelist is empty and the scanning
    // policy filter all the advertising packets
    if ((whitelist.size == 0) && (scanning_policy_mode == Gap::SCAN_POLICY_FILTER_ALL_ADV)) {
        return BLE_ERROR_INVALID_STATE;
    }

    uint16_t scan_intervals[] = { scanningParams.getInterval() };
    uint16_t scan_windows[] = { scanningParams.getWindow() };

    DmScanSetInterval(HCI_SCAN_PHY_LE_1M_BIT, scan_intervals, scan_windows);

    uint8_t scanType = scanningParams.getActiveScanning() ? DM_SCAN_TYPE_ACTIVE : DM_SCAN_TYPE_PASSIVE;
    uint32_t duration = (uint32_t)scanningParams.getTimeout() * 1000;
    if (duration > 0xFFFF) {
        // saturate to 16-bits
        duration = 0xFFFF;
    }

    DmScanStart(HCI_SCAN_PHY_LE_1M_BIT, DM_DISC_MODE_NONE, &scanType, TRUE, duration, 0);

    return BLE_ERROR_NONE;
}

ble_error_t Gap::stopScan(void)
{
    DmScanStop();
    return BLE_ERROR_NONE;
}

void Gap::advertisingStopped(void)
{
    /* If advertising stopped due to a call to stopAdvertising(), state.advertising will
     * be '0.' Otherwise, advertising must have stopped due to a timeout
     */
    if (state.advertising) {
        processTimeoutEvent(Gap::TIMEOUT_SRC_ADVERTISING);
    }
}

uint8_t Gap::getMaxWhitelistSize(void) const
{
    return whitelist.capacity;
}

ble_error_t Gap::getWhitelist(Whitelist_t &other) const
{
    // i is a shorthand for other.size
    uint8_t& i = other.size;

    for (i = 0; (i < whitelist.capacity) && (i < other.capacity); ++i) {
        other.addresses[i] = whitelist.addresses[i];
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::setWhitelist(const Whitelist_t& other)
{
    if (other.capacity > whitelist.capacity) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // note : can be improved by sending the diff instead of the full list

    DmDevWhiteListClear();

    // alias i to whitelist.size
    uint8_t& i = whitelist.size;

    for (i = 0; (i < other.capacity) && (i < whitelist.capacity); ++i) {
        whitelist.addresses[i] = other.addresses[i];
        DmDevWhiteListAdd(
            (whitelist.addresses[i].type > 1) ? 0xFF : whitelist.addresses[i].type,
            whitelist.addresses[i].address
        );
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode)
{
    bool_t result = DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_ADV,
        mode
    );

    if (result == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    advertising_policy_mode = mode;
    return BLE_ERROR_NONE;
}

Gap::AdvertisingPolicyMode_t Gap::getAdvertisingPolicyMode(void) const
{
    return advertising_policy_mode;
}

ble_error_t Gap::setScanningPolicyMode(ScanningPolicyMode_t mode)
{
    bool_t result = DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        mode
    );

    if (result == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    scanning_policy_mode = mode;
    return BLE_ERROR_NONE;

}

Gap::ScanningPolicyMode_t Gap::getScanningPolicyMode(void) const
{
    return scanning_policy_mode;
}

ble_error_t Gap::setInitiatorPolicyMode(InitiatorPolicyMode_t mode)
{
    bool_t result = DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_INIT,
        mode
    );

    if (result == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    initiator_policy_mode = mode;
    return BLE_ERROR_NONE;
}

Gap::InitiatorPolicyMode_t Gap::getInitiatorPolicyMode(void) const
{
    return initiator_policy_mode;
}

ble_error_t Gap::reset(void)
{
    this->::Gap::reset();
    delete[] whitelist.addresses;
    whitelist.addresses = NULL;
    whitelist.size = 0;
    whitelist.capacity = 0;
    return BLE_ERROR_NONE;
}

Gap::Gap() :
    ::Gap(),
    m_connectionHandle(DM_CONN_ID_NONE),
    m_type(BLEProtocol::AddressType::RANDOM_STATIC),
    m_addr(),
    advertising_policy_mode(ADV_POLICY_IGNORE_WHITELIST),
    scanning_policy_mode(SCAN_POLICY_IGNORE_WHITELIST),
    initiator_policy_mode(INIT_POLICY_IGNORE_WHITELIST),
    whitelist()
{
}

} // namespace cordio
} // namespace vendor
} // namespace ble
