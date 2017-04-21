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

#ifndef _ARM_GAP_H_
#define _ARM_GAP_H_

#include "mbed.h"
#include "blecommon.h"
#include "GapAdvertisingParams.h"
#include "GapAdvertisingData.h"
#include "Gap.h"
#include "GapScanningParams.h"
#include "dm_api.h"
#include "att_api.h"

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class ArmGap : public Gap
{
public:
    static ArmGap &getInstance();

    /* Functions that must be implemented from Gap */
    virtual ble_error_t setAddress(AddressType_t  type,  const Address_t address);
    virtual ble_error_t getAddress(AddressType_t *typeP, Address_t address);
    virtual ble_error_t setAdvertisingData(const GapAdvertisingData &, const GapAdvertisingData &);

    #define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
    #define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
    #define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Armum Advertising interval in 625 us units, i.e. 10.24 s. */

    virtual uint16_t getMinAdvertisingInterval(void) const { return BLE_GAP_ADV_INTERVAL_MIN; }
    virtual uint16_t getMinNonConnectableAdvertisingInterval(void) const { return BLE_GAP_ADV_NONCON_INTERVAL_MIN; }
    virtual uint16_t getMaxAdvertisingInterval(void) const { return BLE_GAP_ADV_INTERVAL_MAX; }

    virtual ble_error_t startAdvertising(const GapAdvertisingParams &);
    virtual ble_error_t stopAdvertising(void);
    virtual ble_error_t disconnect(Handle_t connectionHandle, DisconnectionReason_t reason);
    virtual ble_error_t disconnect(DisconnectionReason_t reason);

    virtual ble_error_t setDeviceName(const uint8_t *deviceName);
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);

    virtual ble_error_t setTxPower(int8_t txPower);
    virtual void        getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP);

    void     setConnectionHandle(uint16_t m_connectionHandle);
    uint16_t getConnectionHandle(void);

    virtual ble_error_t getPreferredConnectionParams(ConnectionParams_t *params);
    virtual ble_error_t setPreferredConnectionParams(const ConnectionParams_t *params);
    virtual ble_error_t updateConnectionParams(Handle_t handle, const ConnectionParams_t *params);

    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);
    virtual ble_error_t stopScan(void);

    void     advertisingStopped(void);

private:
    uint16_t m_connectionHandle;
    addr_type_t m_type;
    ArmGap() {
        m_connectionHandle = DM_CONN_ID_NONE;
        m_type = BLEProtocol::AddressType::RANDOM_STATIC;
    }

    ArmGap(ArmGap const &);
    void operator=(ArmGap const &);
};

#endif /* _ARM_GAP_H_ */
