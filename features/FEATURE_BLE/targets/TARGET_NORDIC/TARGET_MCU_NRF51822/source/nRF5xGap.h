/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef __NRF5x_GAP_H__
#define __NRF5x_GAP_H__

#ifdef YOTTA_CFG_MBED_OS
    #include "mbed-drivers/mbed.h"
#else
    #include "mbed.h"
#endif
#ifndef YOTTA_CFG_WHITELIST_MAX_SIZE
    #define YOTTA_CFG_WHITELIST_MAX_SIZE BLE_GAP_WHITELIST_ADDR_MAX_COUNT
#elif YOTTA_CFG_WHITELIST_MAX_SIZE > BLE_GAP_WHITELIST_ADDR_MAX_COUNT
    #undef YOTTA_CFG_WHITELIST_MAX_SIZE
    #define YOTTA_CFG_WHITELIST_MAX_SIZE BLE_GAP_WHITELIST_ADDR_MAX_COUNT
#endif
#ifndef YOTTA_CFG_IRK_TABLE_MAX_SIZE
    #define YOTTA_CFG_IRK_TABLE_MAX_SIZE BLE_GAP_WHITELIST_IRK_MAX_COUNT
#elif YOTTA_CFG_IRK_TABLE_MAX_SIZE > BLE_GAP_WHITELIST_IRK_MAX_COUNT
    #undef YOTTA_CFG_IRK_TABLE_MAX_SIZE
    #define YOTTA_CFG_IRK_TABLE_MAX_SIZE BLE_GAP_WHITELIST_IRK_MAX_COUNT
#endif
#include "ble/blecommon.h"
#include "nrf_ble.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapAdvertisingData.h"
#include "ble/Gap.h"
#include "ble/GapScanningParams.h"

#include "nrf_soc.h"

extern "C" {
#include "ble_radio_notification.h"
}

#include "btle_security.h"

void radioNotificationStaticCallback(bool param);

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class nRF5xGap : public Gap
{
public:
    /* Functions that must be implemented from Gap */
    virtual ble_error_t setAddress(AddressType_t  type,  const Address_t address);
    virtual ble_error_t getAddress(AddressType_t *typeP, Address_t address);
    virtual ble_error_t setAdvertisingData(const GapAdvertisingData &, const GapAdvertisingData &);

    virtual uint16_t    getMinAdvertisingInterval(void) const {return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_INTERVAL_MIN);}
    virtual uint16_t    getMinNonConnectableAdvertisingInterval(void) const {return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_NONCON_INTERVAL_MIN);}
    virtual uint16_t    getMaxAdvertisingInterval(void) const {return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_INTERVAL_MAX);}

    virtual ble_error_t startAdvertising(const GapAdvertisingParams &);
    virtual ble_error_t stopAdvertising(void);
    virtual ble_error_t connect(const Address_t, BLEProtocol::AddressType_t peerAddrType, const ConnectionParams_t *connectionParams, const GapScanningParams *scanParams);
    virtual ble_error_t disconnect(Handle_t connectionHandle, DisconnectionReason_t reason);
    virtual ble_error_t disconnect(DisconnectionReason_t reason);

    virtual ble_error_t setDeviceName(const uint8_t *deviceName);
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);

    virtual ble_error_t setTxPower(int8_t txPower);
    virtual void        getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP);

    void     setConnectionHandle(uint16_t con_handle);
    uint16_t getConnectionHandle(void);

    virtual ble_error_t getPreferredConnectionParams(ConnectionParams_t *params);
    virtual ble_error_t setPreferredConnectionParams(const ConnectionParams_t *params);
    virtual ble_error_t updateConnectionParams(Handle_t handle, const ConnectionParams_t *params);

    virtual ble_error_t reset(void);

    /*
     * The following functions are part of the whitelisting experimental API.
     * Therefore, this functionality can change in the near future.
     */
    virtual uint8_t getMaxWhitelistSize(void) const;
    virtual ble_error_t getWhitelist(Gap::Whitelist_t &whitelistOut) const;
    virtual ble_error_t setWhitelist(const Gap::Whitelist_t &whitelistIn);

    virtual ble_error_t setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode);
    virtual ble_error_t setScanningPolicyMode(ScanningPolicyMode_t mode);
    virtual ble_error_t setInitiatorPolicyMode(InitiatorPolicyMode_t mode);
    virtual Gap::AdvertisingPolicyMode_t getAdvertisingPolicyMode(void) const;
    virtual Gap::ScanningPolicyMode_t getScanningPolicyMode(void) const;
    virtual Gap::InitiatorPolicyMode_t getInitiatorPolicyMode(void) const;

    virtual ble_error_t initRadioNotification(void) {
        if (ble_radio_notification_init(NRF_APP_PRIORITY_HIGH, NRF_RADIO_NOTIFICATION_DISTANCE_800US, radioNotificationStaticCallback) == NRF_SUCCESS) {
            return BLE_ERROR_NONE;
        }

        return BLE_ERROR_UNSPECIFIED;
    }

/* Observer role is not supported by S110, return BLE_ERROR_NOT_IMPLEMENTED */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);
    virtual ble_error_t stopScan(void);
#endif

private:
    /*
     * Whitelisting API related structures and helper functions.
     */

    /* Policy modes set by the user. By default these are set to ignore the whitelist */
    Gap::AdvertisingPolicyMode_t advertisingPolicyMode;
    Gap::ScanningPolicyMode_t    scanningPolicyMode;

    /* Internal representation of a whitelist */
    uint8_t         whitelistAddressesSize;
    ble_gap_addr_t  whitelistAddresses[YOTTA_CFG_WHITELIST_MAX_SIZE];

    /*
     * An internal function used to populate the ble_gap_whitelist_t that will be used by
     * the SoftDevice for filtering requests. This function is needed because for the BLE
     * API the whitelist is just a collection of keys, but for the stack it also includes
     * the IRK table.
     */
    ble_error_t generateStackWhitelist(ble_gap_whitelist_t &whitelist);

private:
    bool    radioNotificationCallbackParam; /* parameter to be passed into the Timeout-generated radio notification callback. */
    Timeout radioNotificationTimeout;

    /*
     * A helper function to post radio notification callbacks with low interrupt priority.
     */
    void postRadioNotificationCallback(void) {
#ifdef YOTTA_CFG_MBED_OS
        /*
         * In mbed OS, all user-facing BLE events (interrupts) are posted to the
         * MINAR scheduler to be executed as callbacks in thread mode. MINAR guards
         * its critical sections from interrupts by acquiring CriticalSectionLock,
         * which results in a call to sd_nvic_critical_region_enter(). Thus, it is
         * safe to invoke MINAR APIs from interrupt context as long as those
         * interrupts are blocked by sd_nvic_critical_region_enter().
         *
         * Radio notifications are a special case for the above. The Radio
         * Notification IRQ is handled at a very high priority--higher than the
         * level blocked by sd_nvic_critical_region_enter(). Thus Radio Notification
         * events can preempt MINAR's critical sections. Using MINAR APIs (such as
         * posting an event) directly in processRadioNotification() may result in a
         * race condition ending in a hard-fault.
         *
         * The solution is to *not* call MINAR APIs directly from the Radio
         * Notification handling; i.e. to do the bulk of RadioNotification
         * processing at a reduced priority which respects MINAR's critical
         * sections. Unfortunately, on a cortex-M0, there is no clean way to demote
         * priority for the currently executing interrupt--we wouldn't want to
         * demote the radio notification handling anyway because it is sensitive to
         * timing, and the system expects to finish this handling very quickly. The
         * workaround is to employ a Timeout to trigger
         * postRadioNotificationCallback() after a very short delay (~0 us) and post
         * the MINAR callback that context.
         *
         * !!!WARNING!!! Radio notifications are very time critical events. The
         * current solution is expected to work under the assumption that
         * postRadioNotificationCalback() will be executed BEFORE the next radio
         * notification event is generated.
         */
        minar::Scheduler::postCallback(
            mbed::util::FunctionPointer1<void, bool>(&radioNotificationCallback, &FunctionPointerWithContext<bool>::call).bind(radioNotificationCallbackParam)
        );
#else
        /*
         * In mbed classic, all user-facing BLE events execute callbacks in interrupt
         * mode. Radio Notifications are a special case because its IRQ is handled at
         * a very high priority. Thus Radio Notification events can preempt other
         * operations that require interaction with the SoftDevice such as advertising
         * payload updates and changing the Gap state. Therefore, executing a Radio
         * Notification callback directly from processRadioNotification() may result
         * in a race condition ending in a hard-fault.
         *
         * The solution is to *not* execute the Radio Notification callback directly
         * from the Radio Notification handling; i.e. to do the bulk of the
         * Radio Notification processing at a reduced priority. Unfortunately, on a
         * cortex-M0, there is no clean way to demote priority for the currently
         * executing interrupt--we wouldn't want to demote the radio notification
         * handling anyway because it is sensitive to timing, and the system expects
         * to finish this handling very quickly. The workaround is to employ a Timeout
         * to trigger postRadioNotificationCallback() after a very short delay (~0 us)
         * and execute the callback in that context.
         *
         * !!!WARNING!!! Radio notifications are very time critical events. The
         * current solution is expected to work under the assumption that
         * postRadioNotificationCalback() will be executed BEFORE the next radio
         * notification event is generated.
         */
        radioNotificationCallback.call(radioNotificationCallbackParam);
#endif /* #ifdef YOTTA_CFG_MBED_OS */
    }

    /**
     * A helper function to process radio-notification events; to be called internally.
     * @param param [description]
     */
    void processRadioNotificationEvent(bool param) {
        radioNotificationCallbackParam = param;
        radioNotificationTimeout.attach_us(mbed::callback(this, &nRF5xGap::postRadioNotificationCallback), 0);
    }
    friend void radioNotificationStaticCallback(bool param); /* allow invocations of processRadioNotificationEvent() */

private:
    uint16_t m_connectionHandle;

    /*
     * Allow instantiation from nRF5xn when required.
     */
    friend class nRF5xn;

    nRF5xGap() :
        advertisingPolicyMode(Gap::ADV_POLICY_IGNORE_WHITELIST),
        scanningPolicyMode(Gap::SCAN_POLICY_IGNORE_WHITELIST),
        whitelistAddressesSize(0) {
        m_connectionHandle = BLE_CONN_HANDLE_INVALID;
    }

    nRF5xGap(nRF5xGap const &);
    void operator=(nRF5xGap const &);
};

#endif // ifndef __NRF5x_GAP_H__
