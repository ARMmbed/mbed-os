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

#ifndef _ARM_GATT_SERVER_H_
#define _ARM_GATT_SERVER_H_

#include <stddef.h>

#include "blecommon.h"
#include "GattServer.h"
#include "generic/wsf_types.h"
#include "att_api.h"

class ArmGattServer : public GattServer
{
public:
    static ArmGattServer &getInstance();

    /* Functions that must be implemented from GattServer */
    virtual ble_error_t addService(GattService &);

    virtual ble_error_t read(GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP);
    virtual ble_error_t read(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP);
    virtual ble_error_t write(GattAttribute::Handle_t, const uint8_t[], uint16_t, bool localOnly = false);
    virtual ble_error_t write(Gap::Handle_t connectionHandle, GattAttribute::Handle_t, const uint8_t[], uint16_t, bool localOnly = false);

    virtual ble_error_t areUpdatesEnabled(const GattCharacteristic &characteristic, bool *enabledP);
    virtual ble_error_t areUpdatesEnabled(Gap::Handle_t connectionHandle, const GattCharacteristic &characteristic, bool *enabledP);

    virtual bool isOnDataReadAvailable() const { return true; }

private:
    static void cccCback(attsCccEvt_t *pEvt);
    static void attCback(attEvt_t *pEvt);
    static uint8_t attsReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, attsAttr_t *pAttr);
    static uint8_t attsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

    /*! client characteristic configuration descriptors settings */
    #define MAX_CCC_CNT 20
    attsCccSet_t cccSet[MAX_CCC_CNT];
    uint16_t cccValues[MAX_CCC_CNT];
    uint16_t cccHandles[MAX_CCC_CNT];
    uint8_t cccCnt;

private:
    ArmGattServer() : GattServer(), cccSet(), cccValues(), cccHandles(), cccCnt(0) {
        /* empty */
    }

    ArmGattServer(const ArmGattServer &);
    const ArmGattServer& operator=(const ArmGattServer &);
};

#endif /* _ARM_GATT_SERVER_H_ */
