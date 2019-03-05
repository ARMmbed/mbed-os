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
class nRF5xGap : public ble::interface::LegacyGap<nRF5xGap>
{
public:
    /* Functions that must be implemented from Gap */
    ble_error_t setAddress_(AddressType_t  type,  const Address_t address);
    ble_error_t getAddress_(AddressType_t *typeP, Address_t address);
    ble_error_t setAdvertisingData_(const GapAdvertisingData &, const GapAdvertisingData &);

    uint16_t    getMinAdvertisingInterval_(void) const {
        return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_INTERVAL_MIN);
    }
    uint16_t    getMinNonConnectableAdvertisingInterval_(void) const {
        return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_NONCON_INTERVAL_MIN);
    }
    uint16_t    getMaxAdvertisingInterval_(void) const {
        return GapAdvertisingParams::ADVERTISEMENT_DURATION_UNITS_TO_MS(BLE_GAP_ADV_INTERVAL_MAX);
    }

    ble_error_t startAdvertising_(const GapAdvertisingParams &);
    ble_error_t stopAdvertising_(void);
    ble_error_t connect_(const Address_t, BLEProtocol::AddressType_t peerAddrType, const ConnectionParams_t *connectionParams, const GapScanningParams *scanParams);
    ble_error_t disconnect_(Handle_t connectionHandle, DisconnectionReason_t reason);
    ble_error_t disconnect_(DisconnectionReason_t reason);

    ble_error_t setDeviceName_(const uint8_t *deviceName);
    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);
    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);
    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);

    ble_error_t setTxPower_(int8_t txPower);
    void        getPermittedTxPowerValues_(const int8_t **valueArrayPP, size_t *countP);

    void     setConnectionHandle(uint16_t con_handle);
    uint16_t getConnectionHandle(void);

    ble_error_t getPreferredConnectionParams_(ConnectionParams_t *params);
    ble_error_t setPreferredConnectionParams_(const ConnectionParams_t *params);
    ble_error_t updateConnectionParams_(Handle_t handle, const ConnectionParams_t *params);

    ble_error_t reset_(void);

    /*
     * The following functions are part of the whitelisting experimental API.
     * Therefore, this functionality can change in the near future.
     */
    uint8_t getMaxWhitelistSize_(void) const;
    ble_error_t getWhitelist_(Whitelist_t &whitelistOut) const;
    ble_error_t setWhitelist_(const Whitelist_t &whitelistIn);

    ble_error_t setAdvertisingPolicyMode_(AdvertisingPolicyMode_t mode);
    ble_error_t setScanningPolicyMode_(ScanningPolicyMode_t mode);
    ble_error_t setInitiatorPolicyMode_(InitiatorPolicyMode_t mode);
    AdvertisingPolicyMode_t getAdvertisingPolicyMode_(void) const;
    ScanningPolicyMode_t getScanningPolicyMode_(void) const;
    InitiatorPolicyMode_t getInitiatorPolicyMode_(void) const;

    ble_error_t initRadioNotification_(void) {
        if (ble_radio_notification_init(NRF_APP_PRIORITY_HIGH, NRF_RADIO_NOTIFICATION_DISTANCE_800US, radioNotificationStaticCallback) == NRF_SUCCESS) {
            return BLE_ERROR_NONE;
        }

        return BLE_ERROR_UNSPECIFIED;
    }

/* Observer role is not supported by S110, return BLE_ERROR_NOT_IMPLEMENTED */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
    ble_error_t startRadioScan_(const GapScanningParams &scanningParams);
    ble_error_t stopScan_(void);
#endif

protected:
    // import from Gap
    friend ble::interface::Gap<nRF5xGap>;

    using ble::interface::Gap<nRF5xGap>::startAdvertising_;
    using ble::interface::Gap<nRF5xGap>::stopAdvertising_;
    using ble::interface::Gap<nRF5xGap>::connect_;
    using ble::interface::Gap<nRF5xGap>::disconnect_;

    // import from LegacyGap
    friend ble::interface::LegacyGap<nRF5xGap>;

    using ble::interface::LegacyGap<nRF5xGap>::startAdvertising_;
    using ble::interface::LegacyGap<nRF5xGap>::stopAdvertising_;
    using ble::interface::LegacyGap<nRF5xGap>::connect_;
    using ble::interface::LegacyGap<nRF5xGap>::disconnect_;

private:
    /*
     * Whitelisting API related structures and helper functions.
     */

    /* Policy modes set by the user. By default these are set to ignore the whitelist */
    AdvertisingPolicyMode_t advertisingPolicyMode;
    ScanningPolicyMode_t    scanningPolicyMode;

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
        advertisingPolicyMode(ADV_POLICY_IGNORE_WHITELIST),
        scanningPolicyMode(SCAN_POLICY_IGNORE_WHITELIST),
        whitelistAddressesSize(0) {
        m_connectionHandle = BLE_CONN_HANDLE_INVALID;
    }

    nRF5xGap(nRF5xGap const &);
    void operator=(nRF5xGap const &);
};

#endif // ifndef __NRF5x_GAP_H__
