/**
 *---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Component   : Wireless LAN driver 
 * File        : cb_wlan_driver_config.h
 *
 * Description : OS related functions
 *-------------------------------------------------------------------------*/
/**
 * @file cb_wlan_driver_config.h Driver configuration.
 * @ingroup port
 */
#ifndef _CB_WLAN_DRIVER_CONFIG_H_
#define _CB_WLAN_DRIVER_CONFIG_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cb_target.h"
#include "cb_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbWLAN_ENABLE_802_11b
#define cbWLAN_ENABLE_802_11g
#define cbWLAN_ENABLE_802_11n

/*===========================================================================
 * TYPES
 *=========================================================================*/

#define cbTARGET_GSETTING_4_ADDRESS_MODE_STATION_DYNAMIC cb_BIT_0
#define cbTARGET_GSETTING_4_ADDRESS_MODE_STATION_ALWAYS cb_BIT_1
/**
 * General settings and tuning parameters .
 *
 * @ingroup types
 */
typedef enum wm_gsetting_e {
    cbTARGET_GSETTING_START = 0,
    cbTARGET_GSETTING_PREAMBLE = cbTARGET_GSETTING_START, /**< 0 = Long preamble, 1 = Short preamble */
    cbTARGET_GSETTING_MIN_SCAN_TIME, /**< Minimum scan time in TU per channel */
    cbTARGET_GSETTING_MAX_SCAN_TIME, /**< Maximum scan time in TU per channel */
    cbTARGET_GSETTING_SCAN_TYPE, /**< Use @ref cbWM_ScanType. */
    cbTARGET_GSETTING_BEACON_INTERVAL, /**< Beacon interval for Ad-hoc/IBSS networks. */
    cbTARGET_GSETTING_JOIN_FAILURE_TIMEOUT, /**< Time in beacons before the join procedure times out. */
    cbTARGET_GSETTING_BT_COEX, /**< Bluetooth co-existence 0 = off, 1 = on */
    cbTARGET_GSETTING_DATA_RATE, /**< Set the data rate to use. Use @ref cbWLAN_Rate */
    cbTARGET_GSETTING_LINK_ADAPTION, /**< Dynamically adapt the data rate. 0 = off, 1 = on */
    cbTARGET_GSETTING_POWER_SAVE, /**< 0 = off, 1 = Fast PSP, 2 = Max PSP */
    cbTARGET_GSETTING_DTIM_ENABLE, /**< Enable DTIM when powersaving */
    cbTARGET_GSETTING_QOS_ENABLE, /**< Enable QoS */
    cbTARGET_GSETTING_RTS_THRESHOLD, /**< Set the RTS (Request to send) threshold */
    cbTARGET_GSETTING_FRAGMENTATION_THRESHOLD, /**< Set the fragmentation threshold */
    cbTARGET_GSETTING_TX_POWER, /**< Desired output power in dBm. */
    cbTARGET_GSETTING_MAX_PASSIVE_SCAN_TIME, /**< Maximum scan time for passive scan */
    cbTARGET_GSETTING_MODULE_TYPE, /**< Type of module. TODO: remove. Legacy parameter that shouldn't be possible to change. */
    cbTARGET_GSETTING_SCAN_LISTEN_INTERVAL, /**< Listen interval between channel scans */
    cbTARGET_GSETTING_SLEEP_TIMEOUT, /**< Only allow power save modes after this period of inactivity. Timeout in ms. Only used when power save is enabled */
    cbTARGET_GSETTING_DOT11_SHORT_RETRY_LIMIT, /**< 802.11 short retry limit for station (dot11ShortRetryLimit). Bit 31-24 reserved, bit 23-16 EAPOL & Broadcast, bit 15-8 MGMT, bit 7-0 data. */
    cbTARGET_GSETTING_DOT11_LONG_RETRY_LIMIT, /**< 802.11 long retry limit for station (dot11LongRetryLimit). Bit 31-24 reserved, bit 23-16 EAPOL & Broadcast, bit 15-8 MGMT, bit 7-0 data. */
    cbTARGET_GSETTING_AP_DOT11_SHORT_RETRY_LIMIT, /**< 802.11 short retry limit for AP (dot11ShortRetryLimit). Bit 31-24 reserved, bit 23-16 EAPOL & Broadcast, bit 15-8 MGMT, bit 7-0 data. */
    cbTARGET_GSETTING_AP_DOT11_LONG_RETRY_LIMIT, /**< 802.11 long retry limit for AP (dot11LongRetryLimit). Bit 31-24 reserved, bit 23-16 EAPOL & Broadcast, bit 15-8 MGMT, bit 7-0 data. */
    cbTARGET_GSETTING_CHANNEL_TYPE, /**< 0-NO_HT, 1-HT20, 2-HT40MINUS, 3-HT40PLUS */
    cbTARGET_GSETTING_PMF_STA, /**< Protected Management frame Option for STA*/
    cbTARGET_GSETTING_REMAIN_ON_CHANNEL, /**< Set 0 disable, 1 enable */
    cbTARGET_GSETTING_STA_TX_RATE_MASK, /**< TX rates for station. May be overridden if not supported by AP. Set to 0 for AP default. @sa cbWLAN_RateMask_e */
    cbTARGET_GSETTING_RSSI_GOOD, /**< When to abort scanning and initiate connection */
    cbTARGET_GSETTING_RSSI_BAD, /**< When to use BAD_RSSI_SCAN_YIELD_TIMEOUT for scanning between each channel for roaming */
    cbTARGET_GSETTING_GOOD_RSSI_SCAN_YIELD_TIMEOUT, /**< Gap between each channel when doing background scan with a good connection */
    cbTARGET_GSETTING_BAD_RSSI_SCAN_YIELD_TIMEOUT, /**< Gap between each channel when doing background scan with a bad connection */
    cbTARGET_GSETTING_ROAM_BLACKLIST_LAST_BSSID_TIMEOUT, /**< How long a new connection to the last connected AP should be banned */
    cbTARGET_GSETTING_FORCE_WORLD_MODE, /**< Set 0 disable, 1 enable */
    cbTARGET_GSETTING_TX_PACKET_ACK_TIMEOUT_WD, /**< Max time for an Tx packet to not being acked by the radio before we send a soft-error event */
    cbTARGET_GSETTING_CTS_PROTECTION, /** send CTS to self before transmission. 0 disable, 1 enable. */
    cbTARGET_GSETTING_HIDDEN_SSID, /** Hidden ssid, 0 disable else enable. */
    cbTARGET_GSETTING_AP_STA_INACTIVITY_TIMEOUT, /**< Aging period for Station in seconds */
    cbTARGET_GSETTING_ROAMING_AREA_HYSTERESIS, /** Threshold between good and bad connection. */
    cbTARGET_GSETTING_FT_MODE, /** 802.11r (FT) mode, 0 - Disabled, 1 - FT over air, 2 - FT over DS */
    cbTARGET_GSETTING_4_ADDRESS_MODE, /** Bit0 - enable dynamic mode for station. Bit1 - always enable for station. */
    cbTARGET_GSETTING_MAX,
} cbWM_GSETTING;

typedef enum targetConfigParams {
    cbTARGET_CFG_FIRST,
    cbTARGET_CFG_SET_POWER_SAVE_MODE = cbTARGET_CFG_FIRST,       //!< Set power mode  @ref cbWLAN_IoctlPowerSaveMode
    cbTARGET_CFG_GET_POWER_SAVE_MODE,                            //!< Get power mode  @ref cbWLAN_IoctlPowerSaveMode
    cbTARGET_CFG_SET_LISTEN_INTERVAL,                            //!< Set listen interval, integer value 0 - 16 
    cbTARGET_CFG_GET_LISTEN_INTERVAL,                            //!< Get listen interval, integer value 0 - 16 
    cbTARGET_CFG_SET_DTIM_ENABLE,                                //!< Set DTIM enable 0, disable 1 enable
    cbTARGET_CFG_GET_DTIM_ENABLE,                                //!< Get DTIM enable 0, disable 1 enable
    cbTARGET_CFG_SET_SLEEP_TIMEOUT,                              //!< Set enter power save entry delay (in ms). Power save mode will be entered only if there no activity during this delay
    cbTARGET_CFG_GET_SLEEP_TIMEOUT,                              //!< Get enter power save entry delay (in ms). Power save mode will be entered only if there no activity during this delay
    cbTARGET_CFG_SET_RSSI_GOOD,                                  //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_GET_RSSI_GOOD,                                  //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_SET_RSSI_BAD,                                   //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_GET_RSSI_BAD,                                   //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_SET_GOOD_RSSI_YIELD_TMO,                        //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_GET_GOOD_RSSI_YIELD_TMO,                        //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_SET_BAD_RSSI_YIELD_TMO,                         //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_GET_BAD_RSSI_YIELD_TMO,                         //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_SET_BLACKLIST_LAST_BSSID_TIMEOUT,               //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_GET_BLACKLIST_LAST_BSSID_TIMEOUT,               //!< SEE CORRESPONDING GSETTING
    cbTARGET_CFG_LAST,                                           
    cbTARGET_CFG_SET_GSETTING = 1000,                            //!< Pipe to @ref cbWM_gSet.
    cbTARGET_CFG_SET_TSETTING = 2000,                            //!< Pipe to @ref cbWM_tSet.
    cbTARGET_CFG_GET_GSETTING = 3000,                            //!< Pipe to @ref cbWM_gGet.
    cbTARGET_CFG_GET_TSETTING = 4000,                            //!< Pipe to @ref cbWM_tGet.
} cbTARGET_ConfigParams;

#define cbTARGET_GSETTING_REG(X)	((cb_uint32)((X) - cbTARGET_GSETTING_START))

/**
 * Target specific settings and tuning parameters .
 *
 * @ingroup types
 */
typedef enum wm_tsetting_e {
    cbTARGET_TSETTING_START = 1000,
    cbTARGET_TSETTING_PS_LISTEN_INTERVAL = cbTARGET_TSETTING_START, /**< Powersave: Listen interval in beacons. */
    cbTARGET_TSETTING_PS_FAST_PSP_TIMEOUT, /**< Powersave: In mode Fast PSP timeout in ms before entering PS. */
    cbTARGET_TSETTING_ANTENNA_RECEIVE_DIVERSITY, /**< Enable receive antenna diversity. 0 = off, 1 = on. */
    cbTARGET_TSETTING_QOS_WMM_NOACK, /**< Enable WMM QoS no-ack acknowledgment policy. 0 = normal ack, 1 = no-ack. */
    cbTARGET_TSETTING_PS_BEACON_EARLY_TERMINATION, /**< Powersave: In Max PSP, use beacon early termination. */
    cbTARGET_TSETTING_MAX,
} cbWM_TSETTING;

#define cbTARGET_TSETTING_REG(X)	((cb_uint32)((X) - cbTARGET_TSETTING_START))

/**
 * Defines the type of scanning procedure.
 * Passive scan will only listen for beacons.
 * Active scan will send out a probe request
 * and listen for both probe responses and beacons.
 *
 * @ingroup types
 */
typedef enum cbWM_ScanType_e {
    cbWM_SCAN_INVALID,
    cbWM_SCAN_ACTIVE,
    cbWM_SCAN_PASSIVE,
} cbWM_ScanType;

/**
 * Power save levels.
 * @note Levels between 2 and cbWM_POWERSAVE_MAX are target specific.
 *
 * @ingroup types
 */
typedef enum cbWM_PowerSaveType_e {
    cbWM_POWERSAVE_OFF = 0,
    cbWM_POWERSAVE_FAST_PSP = 1,
    cbWM_POWERSAVE_MAX_PSP = 2,
    cbWM_POWERSAVE_MAX = cb_UINT8_MAX,
} cbWM_PowerSaveType;

/**
 * Power save modes set using  @ref cbWLAN_ioctl
 *
 * @ingroup wlan
 */
typedef enum {
    cbTARGET_POWER_SAVE_MODE_OFF,
    cbTARGET_POWER_SAVE_MODE_SLEEP,
    cbTARGET_POWER_SAVE_MODE_DEEP_SLEEP
} cbTARGET_PowerSaveMode;

typedef enum {
    cbWLAN_ONE_ANTENNA = 1,
    cbWLAN_TWO_ANTENNAS
} cbWLAN_NUMBER_OF_ANTENNAS;

typedef enum {
    cbWLAN_PRIMARY_ANTENNA_ONE = 1,
    cbWLAN_PRIMARY_ANTENNA_TWO
} cbWLAN_PRIMARY_ANTENNA;
/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Get general tuning parameter.
 *
 * @param hTarget Handle to the current driver instance.
 * @param setting setting to read.
 * @return parameter value
 */
cb_uint32 cbTARGET_gGet(cbTARGET_Handle* hTarget, cbWM_GSETTING setting);

/**
 * Set general tuning parameter.
 *
 * @param hTarget Handle to the current driver instance.
 * @param setting setting to modify.
 * @param value value to set.
 */
void cbTARGET_gSet(cbTARGET_Handle* hTarget, cbWM_GSETTING setting, cb_uint32 value);

/**
 * Get target specific tuning parameter.
 *
 * @param hTarget Handle to the current driver instance.
 * @param setting setting to read.
 * @return parameter value
 */
cb_uint32 cbTARGET_tGet(cbTARGET_Handle* hTarget, cbWM_TSETTING setting);

/**
 * Set target specific tuning parameter.
 *
 * @param hTarget Handle to the current driver instance.
 * @param setting setting to modify.
 * @param value value to set.
 */
void cbTARGET_tSet(cbTARGET_Handle* hTarget, cbWM_TSETTING setting, cb_uint32 value);

struct cb_wlan_configuration*  cbTARGET_configuration_create();

cbRTSL_Status cbTARGET_configure(cbTARGET_Handle* hTarget, cbTARGET_ConfigParams parameter, void* value);

/*--------------------------------------------------------------------------
 * Constants
 *-------------------------------------------------------------------------*/

#define W_CONST_PREAMBLE                            (1)
#define W_CONST_LISTEN_INTERVAL                     (16)
#define W_CONST_MIN_SCAN_TIME                       (25)
#define W_CONST_MAX_SCAN_TIME                       (50)
#define W_CONST_SCAN_TYPE                           (cbWM_SCAN_ACTIVE)
#define W_CONST_BEACON_INTERVAL					    (100)
#define W_CONST_JOIN_FAILURE_TIMEOUT                (600)
#define W_CONST_WLAN_BT_COEX                        (0)
#define W_CONST_DATA_RATE                           (cbWLAN_RATE_24)
#define W_CONST_LINK_ADAPTION                       (1)
#define W_CONST_POWER_SAVE                          (2)
#define W_CONST_DTIM_ENABLE                         (1)
#define W_CONST_QOS_ENABLE                          (1)
#define W_CONST_CHANNEL_TYPE                        (1) //HT20MHz
#define W_CONST_RTS_THRESHOLD                       (2347)
#define W_CONST_FRAGMENTATION_THRESHOLD             (2346)
#define W_CONST_TX_POWER                            (15)
#define W_CONST_MAX_PASSIVE_SCAN_TIME               (150)
#define W_CONST_SCAN_LISTEN_INTERVAL                (150)
#define W_CONST_SLEEP_TIMEOUT                       (100) // Timeout in ms, 100ms timeout gives almost same throughput as without power save but still low power consumption
#define W_CONST_DEFAULT_MODULE_TYPE                 (cbWM_MODULE_UNKNOWN)
#define W_CONST_PMF_OPTIONAL                        (cbWLAN_PMF_OPTIONAL)
#define W_CONST_REMAIN_ON_CHANNEL                   (1) // Enabled
#define W_CONST_DEFAULT_STA_TX_RATES                (0) // Use AP default
#define W_CONST_GOOD_RSSI                           (55)
#define W_CONST_BAD_RSSI                            (70)
#define W_CONST_GOOD_RSSI_SCAN_YIELD_TIMEOUT        (1500)
#define W_CONST_BAD_RSSI_SCAN_YIELD_TIMEOUT         (W_CONST_SCAN_LISTEN_INTERVAL)
#define W_CONST_BLACKLIST_LAST_BSSID_TIMEOUT        (20) // Seconds
#define W_CONST_ROAMING_AREA_HYSTERESIS             (3)
#define W_CONST_TX_PACKET_ACK_TIMEOUT               (10000)
#define W_CONST_FT_MODE                             (2)

#define W_CONST_DEFAULT_FORCE_WORLD_MODE            (0)

#define W_CONST_DEFAULT_AP_STA_INACTIVITY_TIMEOUT   (120)


#define W_CONST_DEFAULT_DOT11_SHORT_RETRY_LIMIT (0x0a0a0a0aul)
#define W_CONST_DEFAULT_DOT11_LONG_RETRY_LIMIT  (0x0a0a0a0aul)
#define W_CONST_DEFAULT_AP_DOT11_SHORT_RETRY_LIMIT  (0x0a0a0a0aul)
#define W_CONST_DEFAULT_AP_DOT11_LONG_RETRY_LIMIT   (0x0a0a0a0aul)

//Target specific values
#define W_CONST_PS_LISTEN_INTERVAL                  (0)
#define W_CONST_PS_FAST_PSP_TIMEOUT                 (50) 
#define W_CONST_ANTENNA_RECEIVE_DIVERSITY           (0)
#define W_CONST_PS_BEACON_EARLY_TERMINATION         (0)

#define W_CONST_2_4GHZ_MGMT_RATE                    (cbWLAN_RATE_01)
#define W_CONST_5GHZ_MGMT_RATE                      (cbWLAN_RATE_06)

#define W_CONST_DEFAULT_RSSI                        (-100)

#ifdef __cplusplus
}
#endif

#endif /* _CB_SYSTEM_H_ */
