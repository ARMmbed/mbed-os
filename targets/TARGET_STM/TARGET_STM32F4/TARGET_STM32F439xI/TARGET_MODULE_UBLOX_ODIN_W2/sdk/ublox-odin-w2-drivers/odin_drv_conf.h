/*---------------------------------------------------------------------------
 * Copyright (c) 2019, u-blox Malm�, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef ODIN_DRV_CONFIG_H
#define ODIN_DRV_CONFIG_H

/** Enum of WiFi Configuration types
 *
 *  The configuration type specifies a particular parameter of
 *  a WiFi interface. And will be used particular configuration to 
 *  use when initializing a WiFi network e.g. 802.11r, 802.11w
 *  SEE CORRESPONDING GSETTING
 *
 */
typedef enum target_config_params_e {
    ODIN_CFG_FIRST,
    ODIN_CFG_SET_POWER_SAVE_MODE = ODIN_CFG_FIRST,
    ODIN_CFG_GET_POWER_SAVE_MODE,
    ODIN_CFG_SET_MIN_SCAN_TIME,
    ODIN_CFG_GET_MIN_SCAN_TIME,
    ODIN_CFG_SET_MAX_SCAN_TIME,
    ODIN_CFG_GET_MAX_SCAN_TIME,
    ODIN_CFG_SET_SCAN_TYPE,
    ODIN_CFG_GET_SCAN_TYPE,
    ODIN_CFG_SET_LISTEN_INTERVAL,
    ODIN_CFG_GET_LISTEN_INTERVAL,
    ODIN_CFG_SET_DTIM_ENABLE,
    ODIN_CFG_GET_DTIM_ENABLE,
    ODIN_CFG_SET_QOS_ENABLE,
    ODIN_CFG_GET_QOS_ENABLE,
    ODIN_CFG_SET_RTS_THRESHOLD,
    ODIN_CFG_GET_RTS_THRESHOLD,
    ODIN_CFG_SET_TX_POWER,
    ODIN_CFG_GET_TX_POWER,
    ODIN_CFG_SET_MAX_PASSIVE_SCAN_TIME,
    ODIN_CFG_GET_MAX_PASSIVE_SCAN_TIME,
    ODIN_CFG_SET_SCAN_LISTEN_INTERVAL,
    ODIN_CFG_GET_SCAN_LISTEN_INTERVAL,
    ODIN_CFG_SET_DOT11_SHORT_RETRY_LIMIT,
    ODIN_CFG_GET_DOT11_SHORT_RETRY_LIMIT,
    ODIN_CFG_SET_DOT11_LONG_RETRY_LIMIT,
    ODIN_CFG_GET_DOT11_LONG_RETRY_LIMIT,
    ODIN_CFG_SET_AP_DOT11_SHORT_RETRY_LIMIT,
    ODIN_CFG_GET_AP_DOT11_SHORT_RETRY_LIMIT,
    ODIN_CFG_SET_AP_DOT11_LONG_RETRY_LIMIT,
    ODIN_CFG_GET_AP_DOT11_LONG_RETRY_LIMIT,
    ODIN_CFG_SET_REMAIN_ON_CHANNEL,
    ODIN_CFG_GET_REMAIN_ON_CHANNEL,
    ODIN_CFG_SET_STA_TX_RATE_MASK,
    ODIN_CFG_GET_STA_TX_RATE_MASK,
    ODIN_CFG_SET_RSSI_GOOD,
    ODIN_CFG_GET_RSSI_GOOD,
    ODIN_CFG_SET_RSSI_BAD,
    ODIN_CFG_GET_RSSI_BAD,
    ODIN_CFG_SET_SLEEP_TIMEOUT,
    ODIN_CFG_GET_SLEEP_TIMEOUT,
    ODIN_CFG_SET_GOOD_RSSI_YIELD_TMO,
    ODIN_CFG_GET_GOOD_RSSI_YIELD_TMO,
    ODIN_CFG_SET_BAD_RSSI_YIELD_TMO,
    ODIN_CFG_GET_BAD_RSSI_YIELD_TMO,
    ODIN_CFG_SET_FORCE_WORLD_MODE,
    ODIN_CFG_GET_FORCE_WORLD_MODE,
    ODIN_CFG_SET_TX_PACKET_ACK_TIMEOUT_WD,
    ODIN_CFG_GET_TX_PACKET_ACK_TIMEOUT_WD,
    ODIN_CFG_SET_CTS_PROTECTION,
    ODIN_CFG_GET_CTS_PROTECTION,
    ODIN_CFG_SET_HIDDEN_SSID,
    ODIN_CFG_GET_HIDDEN_SSID,
    ODIN_CFG_SET_AP_STA_INACTIVITY_TIMEOUT,
    ODIN_CFG_GET_AP_STA_INACTIVITY_TIMEOUT,
    ODIN_CFG_SET_ROAMING_AREA_HYSTERESIS,
    ODIN_CFG_GET_ROAMING_AREA_HYSTERESIS,
    ODIN_CFG_SET_RSSI_MIN,
    ODIN_CFG_GET_RSSI_MIN,
    ODIN_CFG_SET_ROAM_BAD_CHANNEL_RSSI_HYSTERESIS,
    ODIN_CFG_GET_ROAM_BAD_CHANNEL_RSSI_HYSTERESIS,
    ODIN_CFG_SET_ROAM_NO_ROAM_TMO,
    ODIN_CFG_GET_ROAM_NO_ROAM_TMO,
    ODIN_CFG_SET_ROAM_WAIT_TIMEOUT,
    ODIN_CFG_GET_ROAM_WAIT_TIMEOUT,
    ODIN_CFG_SET_SHORT_GI,
    ODIN_CFG_GET_SHORT_GI,
    ODIN_CFG_SET_WIRELESS_ISOLATION,
    ODIN_CFG_GET_WIRELESS_ISOLATION,
    ODIN_CFG_SET_PMF_STA,
    ODIN_CFG_GET_PMF_STA,
    ODIN_CFG_SET_FT_MODE,
    ODIN_CFG_GET_FT_MODE,
    ODIN_CFG_LAST,
} target_config_params_e;

/**
* Power save modes set using  @ref cbWLAN_ioctl
*
* @ingroup wlan
*/
typedef enum {
    ODIN_POWER_SAVE_MODE_OFF,
    ODIN_POWER_SAVE_MODE_SLEEP,
    ODIN_POWER_SAVE_MODE_DEEP_SLEEP
} target_power_save_mode_e;

#endif

