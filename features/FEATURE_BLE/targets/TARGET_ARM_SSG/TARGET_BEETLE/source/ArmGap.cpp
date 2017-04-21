/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ArmGap.h"
#include "mbed.h"

ArmGap &ArmGap::getInstance() {
    static ArmGap m_instance;
    return m_instance;
}

ble_error_t ArmGap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
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
    DmAdvSetData(DM_DATA_LOC_ADV, advData.getPayloadLen(), (uint8_t*)advData.getPayload());
    DmAdvSetData(DM_DATA_LOC_SCAN, scanResponse.getPayloadLen(), (uint8_t*)scanResponse.getPayload());

    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::startAdvertising(const GapAdvertisingParams &params)
{
    /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) {
        /* ToDo: This requires a proper security implementation, etc. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getInterval() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getInterval() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    } else {
        if ((params.getInterval() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getInterval() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
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

    /* Peer Addr Type 0 = Public */
    uint8_t peerAddrType = 0;
    uint8_t peerAddr[6];
    memset(peerAddr, 0, 6);

    DmAdvSetInterval(params.getInterval(), params.getInterval());
    DmAdvStart(params.getAdvertisingType(), params.getTimeout(), peerAddrType, peerAddr);

    state.advertising = 1;

    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::stopAdvertising(void)
{
    DmAdvStop();

    state.advertising = 0;

    return BLE_ERROR_NONE;
}

void ArmGap::advertisingStopped(void)
{
    /* If advertising stopped due to a call to stopAdvertising(), state.advertising will
     * be '0.' Otherwise, advertising must have stopped due to a timeout
     */
    if (state.advertising) {
        processTimeoutEvent(Gap::TIMEOUT_SRC_ADVERTISING);
    }
}

ble_error_t ArmGap::disconnect(DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, m_connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *newParams)
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

ble_error_t ArmGap::startRadioScan(const GapScanningParams &scanningParams)
{
    DmScanSetInterval(scanningParams.getInterval(), scanningParams.getWindow());

    uint8_t scanType = scanningParams.getActiveScanning() ? DM_SCAN_TYPE_ACTIVE : DM_SCAN_TYPE_PASSIVE;
    uint32_t duration = (uint32_t)scanningParams.getTimeout() * 1000;
    if (duration > 0xFFFF) {
        // saturate to 16-bits
        duration = 0xFFFF;
    }

    DmScanStart(DM_DISC_MODE_NONE, scanType, TRUE, duration);

    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::stopScan(void)
{
    DmScanStop();
    return BLE_ERROR_NONE;
}

void ArmGap::setConnectionHandle(uint16_t connectionHandle)
{
    m_connectionHandle = connectionHandle;
}

uint16_t ArmGap::getConnectionHandle(void)
{
    return m_connectionHandle;
}

ble_error_t ArmGap::setAddress(AddressType_t type, const Address_t address)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::getAddress(AddressType_t *typeP, Address_t address)
{
    *typeP = m_type;
    BdaCpy(address, HciGetBdAddr());
    return BLE_ERROR_NONE;
}

ble_error_t ArmGap::setDeviceName(const uint8_t *deviceName)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t ArmGap::setTxPower(int8_t txPower)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void ArmGap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
    static const int8_t permittedTxValues[] = {
        -18, -15, -12, -9, -6, -3, 0, 3
    };

    *valueArrayPP = permittedTxValues;
    *countP = sizeof(permittedTxValues) / sizeof(int8_t);
}
