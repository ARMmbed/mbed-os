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

#ifndef _MAXIM_GATT_SERVER_H_
#define _MAXIM_GATT_SERVER_H_

#include <stddef.h>

#include "ble/blecommon.h"
#include "ble/GattServer.h"
#include "wsf_types.h"
#include "att_api.h"

class MaximGattServer : public GattServer
{
public:
    static MaximGattServer &getInstance();

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
    MaximGattServer() : GattServer(), cccSet(), cccValues(), cccHandles(), cccCnt(0) {
        /* empty */
    }

    MaximGattServer(const MaximGattServer &);
    const MaximGattServer& operator=(const MaximGattServer &);
};

#endif /* _MAXIM_GATT_SERVER_H_ */
