/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "MaximGap.h"
#include "mbed.h"
#include "hci_vs.h"

MaximGap &MaximGap::getInstance() {
    static MaximGap m_instance;
    return m_instance;
}

ble_error_t MaximGap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* save advertising and scan response data */
    advDataLen = advData.getPayloadLen();
    scanResponseLen = scanResponse.getPayloadLen();
    memcpy((void*)advDataCache, (void*)advData.getPayload(), advDataLen);
    memcpy((void*)advDataCache, (void*)advData.getPayload(), scanResponseLen);

    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::startAdvertising(const GapAdvertisingParams &params)
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

    /* set advertising and scan response data for discoverable mode */
    DmAdvSetData(DM_DATA_LOC_ADV, advDataLen, advDataCache);
    DmAdvSetData(DM_DATA_LOC_SCAN, scanResponseLen, scanResponseCache);

    DmAdvSetInterval(params.getInterval(), params.getInterval());
    DmAdvStart(params.getAdvertisingType(), params.getTimeout());

    state.advertising = 1;

    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::stopAdvertising(void)
{
    DmAdvStop();

    state.advertising = 0;

    return BLE_ERROR_NONE;
}

void MaximGap::advertisingStopped(void)
{
    /* If advertising stopped due to a call to stopAdvertising(), state.advertising will
     * be '0.' Otherwise, advertising must have stopped due to a timeout
     */
    if (state.advertising) {
        processTimeoutEvent(Gap::TIMEOUT_SRC_ADVERTISING);
    }
}

ble_error_t MaximGap::disconnect(DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, m_connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    DmConnClose(DM_CLIENT_ID_APP, connectionHandle, reason);

    state.advertising = 0;
    state.connected = 0;

    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *newParams)
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

ble_error_t MaximGap::startRadioScan(const GapScanningParams &scanningParams)
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

ble_error_t MaximGap::stopScan(void)
{
    DmScanStop();
    return BLE_ERROR_NONE;
}

void MaximGap::setConnectionHandle(uint16_t connectionHandle)
{
    m_connectionHandle = connectionHandle;
}

uint16_t MaximGap::getConnectionHandle(void)
{
    return m_connectionHandle;
}

ble_error_t MaximGap::setAddress(AddressType_t type, const Address_t address)
{
    if ((type != BLEProtocol::AddressType::PUBLIC) && (type != BLEProtocol::AddressType::RANDOM_STATIC)) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    m_type = type;
    HciVsSetPublicAddr((uint8_t*)address);

    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::getAddress(AddressType_t *typeP, Address_t address)
{
    *typeP = m_type;
    BdaCpy(address, HciGetBdAddr());
    return BLE_ERROR_NONE;
}

ble_error_t MaximGap::setDeviceName(const uint8_t *deviceName)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t MaximGap::setTxPower(int8_t txPower)
{
    HciVsSetTxPower(txPower);
    return BLE_ERROR_NONE;
}

void MaximGap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
    static const int8_t permittedTxValues[] = {
        -18, -15, -12, -9, -6, -3, 0, 3
    };

    *valueArrayPP = permittedTxValues;
    *countP = sizeof(permittedTxValues) / sizeof(int8_t);
}
