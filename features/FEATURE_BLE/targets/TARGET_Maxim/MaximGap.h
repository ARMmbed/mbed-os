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
class MaximGap : public ble::interface::LegacyGap<MaximGap>
{
public:
    static MaximGap &getInstance();

    /* Functions that must be implemented from Gap */
    ble_error_t setAddress_(AddressType_t  type,  const Address_t address);
    ble_error_t getAddress_(AddressType_t *typeP, Address_t address);
    ble_error_t setAdvertisingData_(const GapAdvertisingData &, const GapAdvertisingData &);

    #define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
    #define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
    #define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */

    uint16_t getMinAdvertisingInterval_(void) const { return BLE_GAP_ADV_INTERVAL_MIN; }
    uint16_t getMinNonConnectableAdvertisingInterval_(void) const { return BLE_GAP_ADV_NONCON_INTERVAL_MIN; }
    uint16_t getMaxAdvertisingInterval_(void) const { return BLE_GAP_ADV_INTERVAL_MAX; }

    ble_error_t startAdvertising_(const GapAdvertisingParams &);
    ble_error_t stopAdvertising_(void);
    ble_error_t disconnect_(Handle_t connectionHandle, DisconnectionReason_t reason);
    ble_error_t disconnect_(DisconnectionReason_t reason);

    ble_error_t setDeviceName_(const uint8_t *deviceName);
    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);
    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);
    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);

    ble_error_t setTxPower_(int8_t txPower);
    void        getPermittedTxPowerValues_(const int8_t **valueArrayPP, size_t *countP);

    void     setConnectionHandle(uint16_t m_connectionHandle);
    uint16_t getConnectionHandle(void);

    ble_error_t getPreferredConnectionParams_(ConnectionParams_t *params);
    ble_error_t setPreferredConnectionParams_(const ConnectionParams_t *params);
    ble_error_t updateConnectionParams_(Handle_t handle, const ConnectionParams_t *params);

    ble_error_t startRadioScan_(const GapScanningParams &scanningParams);
    ble_error_t stopScan_(void);

    void     advertisingStopped(void);

protected:
    // import from Gap
    friend ble::interface::Gap<MaximGap>;

    using ble::interface::Gap<MaximGap>::startAdvertising_;
    using ble::interface::Gap<MaximGap>::stopAdvertising_;
    using ble::interface::Gap<MaximGap>::connect_;
    using ble::interface::Gap<MaximGap>::disconnect_;

    // import from LegacyGap
    friend ble::interface::LegacyGap<MaximGap>;

    using ble::interface::LegacyGap<MaximGap>::startAdvertising_;
    using ble::interface::LegacyGap<MaximGap>::stopAdvertising_;
    using ble::interface::LegacyGap<MaximGap>::connect_;
    using ble::interface::LegacyGap<MaximGap>::disconnect_;

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
