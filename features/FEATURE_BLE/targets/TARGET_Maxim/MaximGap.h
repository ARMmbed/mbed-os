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

#ifndef _MAXIM_GAP_H_
#define _MAXIM_GAP_H_

#include "mbed.h"
#include "ble/blecommon.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapAdvertisingData.h"
#include "ble/Gap.h"
#include "ble/GapScanningParams.h"
#include "dm_api.h"
#include "att_api.h"

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class MaximGap : public Gap
{
public:
    static MaximGap &getInstance();

    /* Functions that must be implemented from Gap */
    virtual ble_error_t setAddress(AddressType_t  type,  const Address_t address);
    virtual ble_error_t getAddress(AddressType_t *typeP, Address_t address);
    virtual ble_error_t setAdvertisingData(const GapAdvertisingData &, const GapAdvertisingData &);

    #define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
    #define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
    #define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */

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
    MaximGap() {
        m_connectionHandle = DM_CONN_ID_NONE;
        m_type = BLEProtocol::AddressType::RANDOM_STATIC;
    }

    MaximGap(MaximGap const &);
    void operator=(MaximGap const &);
    
    uint8_t advDataCache[HCI_LEN_LE_SET_ADV_DATA];
    uint8_t scanResponseCache[HCI_LEN_LE_SET_SCAN_RESP_DATA];
    int advDataLen;
    int scanResponseLen;
};

#endif /* _MAXIM_GAP_H_ */
