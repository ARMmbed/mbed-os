/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

/** @file scl_types.h
 *  Defines common data types used in SCL
 *
 */

#include <stdint.h>
#include "cy_result.h"
#include "scl_common.h"
#ifndef INCLUDED_SCL_TYPES_H_
#define INCLUDED_SCL_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************
*                      Macros
******************************************************/
#define SSID_NAME_SIZE           (32)  /**< SSID Length */

#define WEP_ENABLED            0x0001  /**< Flag to enable WEP Security        */
#define TKIP_ENABLED           0x0002  /**< Flag to enable TKIP Encryption     */
#define AES_ENABLED            0x0004  /**< Flag to enable AES Encryption      */
#define SHARED_ENABLED     0x00008000  /**< Flag to enable Shared key Security */
#define WPA_SECURITY       0x00200000  /**< Flag to enable WPA Security        */
#define WPA2_SECURITY      0x00400000  /**< Flag to enable WPA2 Security       */
#define WPA3_SECURITY      0x01000000  /**< Flag to enable WPA3 PSK Security   */

#define ENTERPRISE_ENABLED 0x02000000  /**< Flag to enable Enterprise Security */
#define WPS_ENABLED        0x10000000  /**< Flag to enable WPS Security        */
#define IBSS_ENABLED       0x20000000  /**< Flag to enable IBSS mode           */
#define FBT_ENABLED        0x40000000  /**< Flag to enable FBT                 */

#define NO_POWERSAVE_MODE           (0) /**< No Power save mode */
#define PM1_POWERSAVE_MODE          (1) /**< Power save mode on specified interface without regard for throughput reduction */
#define PM2_POWERSAVE_MODE          (2) /**< Power save mode on specified interface with High throughput */

/**
 * Suppresses unused parameter warning
 */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/**
 * Suppresses unused variable warning
 */
#define UNUSED_VARIABLE(x) ( (void)(x) )

/**
 * Suppresses unused variable warning that occurs due to an assert being disabled in release mode
 */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*               Structures and Enumerations
******************************************************/

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum {
    SCL_SECURITY_OPEN             = 0,                                                                 /**< Open security                                         */
    SCL_SECURITY_WEP_PSK          = WEP_ENABLED,                                                       /**< WEP PSK Security with open authentication             */
    SCL_SECURITY_WEP_SHARED       = (WEP_ENABLED | SHARED_ENABLED),                                    /**< WEP PSK Security with shared authentication           */
    SCL_SECURITY_WPA_TKIP_PSK     = (WPA_SECURITY | TKIP_ENABLED),                                     /**< WPA PSK Security with TKIP                            */
    SCL_SECURITY_WPA_AES_PSK      = (WPA_SECURITY | AES_ENABLED),                                      /**< WPA PSK Security with AES                             */
    SCL_SECURITY_WPA_MIXED_PSK    = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                       /**< WPA PSK Security with AES & TKIP                      */
    SCL_SECURITY_WPA2_AES_PSK     = (WPA2_SECURITY | AES_ENABLED),                                     /**< WPA2 PSK Security with AES                            */
    SCL_SECURITY_WPA2_TKIP_PSK    = (WPA2_SECURITY | TKIP_ENABLED),                                    /**< WPA2 PSK Security with TKIP                           */
    SCL_SECURITY_WPA2_MIXED_PSK   = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                      /**< WPA2 PSK Security with AES & TKIP                     */
    SCL_SECURITY_WPA2_FBT_PSK     = (WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),                       /**< WPA2 FBT PSK Security with AES & TKIP */
    SCL_SECURITY_WPA3_SAE         = (WPA3_SECURITY | AES_ENABLED),                                     /**< WPA3 Security with AES */
    SCL_SECURITY_WPA3_WPA2_PSK    = (WPA3_SECURITY | WPA2_SECURITY | AES_ENABLED),                     /**< WPA3 WPA2 PSK Security with AES */
    SCL_SECURITY_WPA2_WPA_PSK     = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED),                      /**< WPA2 WPA PSK Security with AES */
    SCL_SECURITY_WPA2_WPA_TKIP_PSK = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),      /**< WPA2 WPA PSK Security with AES & TKIP*/

    SCL_SECURITY_WPA_TKIP_ENT     = (ENTERPRISE_ENABLED | WPA_SECURITY | TKIP_ENABLED),                /**< WPA Enterprise Security with TKIP                     */
    SCL_SECURITY_WPA_AES_ENT      = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED),                 /**< WPA Enterprise Security with AES                      */
    SCL_SECURITY_WPA_MIXED_ENT    = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),  /**< WPA Enterprise Security with AES & TKIP               */
    SCL_SECURITY_WPA2_TKIP_ENT    = (ENTERPRISE_ENABLED | WPA2_SECURITY | TKIP_ENABLED),               /**< WPA2 Enterprise Security with TKIP                    */
    SCL_SECURITY_WPA2_AES_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED),                /**< WPA2 Enterprise Security with AES                     */
    SCL_SECURITY_WPA2_MIXED_ENT   = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED), /**< WPA2 Enterprise Security with AES & TKIP              */
    SCL_SECURITY_WPA2_FBT_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),  /**< WPA2 Enterprise Security with AES & FBT               */

    SCL_SECURITY_IBSS_OPEN        = (IBSS_ENABLED),                                                    /**< Open security on IBSS ad-hoc network                  */
    SCL_SECURITY_WPS_OPEN         = (WPS_ENABLED),                                                     /**< WPS with open security                                */
    SCL_SECURITY_WPS_SECURE       = (WPS_ENABLED | AES_ENABLED),                                       /**< WPS with AES security                                 */

    SCL_SECURITY_UNKNOWN          = -1,                                                                /**< Returned by scan function if security is unknown. Do not pass this to the join function! */

    SCL_SECURITY_FORCE_32_BIT     = 0x7fffffff                                                         /**< Exists only to force scl_security_t type to 32 bits */
} scl_security_t;



/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
    SCL_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
    SCL_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} scl_802_11_band_t;

/** Structure for storing 802.11 powersave listen interval values \n
 *  See @ref scl_wifi_get_listen_interval for more information
 */
typedef struct
{
    uint8_t beacon; /**< Listen interval in beacon periods */
    uint8_t dtim;   /**< Listen interval in DTIM periods   */
    uint16_t assoc; /**< Listen interval as sent to APs    */
} scl_listen_interval_t;

/**
 * Enumeration of methods of scanning
 */
typedef enum
{
    SCL_SCAN_TYPE_ACTIVE              = 0x00, /**< Actively scan a network by sending 802.11 probe(s)                              */
    SCL_SCAN_TYPE_PASSIVE             = 0x01, /**< Passively scan a network by listening for beacons from APs                      */
    SCL_SCAN_TYPE_PNO                 = 0x02, /**< Use preferred network offload to detect an AP                                   */
    SCL_SCAN_TYPE_PROHIBITED_CHANNELS = 0x04, /**< Permit (passively) scanning a channel that isn't valid for the current country  */
    SCL_SCAN_TYPE_NO_BSSID_FILTER     = 0x08  /**< Return a scan record for each beacon or probe response RX'ed                    */
} scl_scan_type_t;

/**
 * Enumeration of network types
 */
typedef enum
{
    SCL_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    SCL_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    SCL_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */
    SCL_BSS_TYPE_MESH           = 3, /**< Denotes 802.11 mesh network                     */

    SCL_BSS_TYPE_UNKNOWN = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} scl_bss_type_t;

/**
 * Structure for storing a Service Set Identifier (i.e. Name of Access Point)
 */
typedef struct
{
    uint8_t length;                /**< SSID length */
    uint8_t value[SSID_NAME_SIZE]; /**< SSID name (AP name)  */
} scl_ssid_t;

/**
 * Structure for storing scan status
 */
typedef enum
{
    SCL_SCAN_INCOMPLETE = 0,                /**< Denotes that scan is not finished */
    SCL_SCAN_COMPLETED_SUCCESSFULLY,    /**< Successful completion of scan */
    SCL_SCAN_ABORTED,                   /**< Scan is aborted */
} scl_scan_status_t;

/**
 * Structure for storing extended scan parameters
 */
typedef struct
{
    int32_t number_of_probes_per_channel;                     /**< Number of probes to send on each channel                                               */
    int32_t scan_active_dwell_time_per_channel_ms;            /**< Period of time to wait on each channel when active scanning                            */
    int32_t scan_passive_dwell_time_per_channel_ms;           /**< Period of time to wait on each channel when passive scanning                           */
    int32_t scan_home_channel_dwell_time_between_channels_ms; /**< Period of time to wait on the home channel when scanning. Only relevant if associated. */
} scl_scan_extended_params_t;

/**
 * Structure for storing scan results
 */
#pragma pack(1)
typedef struct scl_scan_result
{
    scl_ssid_t SSID;                            /**< Service Set Identification (i.e. Name of Access Point)                    */
    scl_mac_t BSSID;                            /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength;                    /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    uint32_t max_data_rate;                     /**< Maximum data rate in kilobits/s                                           */
    scl_bss_type_t bss_type;                    /**< Network type                                                              */
    scl_security_t security;                    /**< Security type                                                             */
    uint8_t channel;                            /**< Radio channel that the AP beacon was received on                          */
    scl_802_11_band_t band;                     /**< Radio band                                                                */
    uint8_t ccode[2];                           /**< Two letter ISO country code from AP                                       */
    uint8_t flags;                              /**< flags                                                                     */
    struct scl_scan_result *next;               /**< Pointer to the next scan result                                           */
    uint8_t *ie_ptr;                            /**< Pointer to received Beacon/Probe Response IE(Information Element)         */
    uint32_t ie_len;                            /**< Length of IE(Information Element)                                         */
} scl_scan_result_t;
#pragma pack()

/**
 * Structure to store scan result parameters for each AP
 */
typedef struct scl_simple_scan_result
{
    scl_ssid_t SSID;         /**< Service Set Identification (i.e. Name of Access Point)                    */
    scl_mac_t BSSID;         /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength; /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    scl_security_t security; /**< Security type                                                             */
    uint8_t channel;         /**< Radio channel that the AP beacon was received on                          */
} scl_sync_scan_result_t;

typedef uint16_t wl_chanspec_t;  /**< Channel specified in uint16_t */
#define MCSSET_LEN    16 /**< Maximum allowed mcs rate */

/** BSS(Basic Service Set) information structure
 *
 * Applications MUST CHECK ie_offset field and length field to access IEs(Information Elements) and
 * next bss_info structure in a vector (in scl_sync_scan_result_t)
 */
typedef struct wl_bss_info_struct
{
    uint32_t version;              /**< version field */
    uint32_t length;               /**< byte length of data in this record, starting at version and including IEs */
    scl_mac_t BSSID;               /**< Unique 6-byte MAC address */
    uint16_t beacon_period;        /**< Interval between two consecutive beacon frames. Units are Kusec */
    uint16_t capability;           /**< Capability information */
    uint8_t SSID_len;              /**< SSID length */
    uint8_t SSID[32];              /**< Array to store SSID */
    struct
    {
        uint32_t count;            /**< Count of rates in this set */
        uint8_t rates[16];         /**< rates in 500kbps units, higher bit set if basic */
    } rateset;                     /**< supported rates */
    wl_chanspec_t chanspec;        /**< Channel specification for basic service set */
    uint16_t atim_window;          /**< Announcement traffic indication message window size. Units are Kusec */
    uint8_t dtim_period;           /**< Delivery traffic indication message period */
    int16_t RSSI;                  /**< receive signal strength (in dBm) */
    int8_t phy_noise;              /**< noise (in dBm) */

    uint8_t n_cap;                 /**< BSS is 802.11N Capable */
    uint32_t nbss_cap;             /**< 802.11N BSS Capabilities (based on HT_CAP_*) */
    uint8_t ctl_ch;                /**< 802.11N BSS control channel number */
    uint32_t reserved32[1];        /**< Reserved for expansion of BSS properties */
    uint8_t flags;                 /**< flags */
    uint8_t reserved[3];           /**< Reserved for expansion of BSS properties */
    uint8_t basic_mcs[MCSSET_LEN]; /**< 802.11N BSS required MCS set */

    uint16_t ie_offset;            /**< offset at which IEs start, from beginning */
    uint32_t ie_length;            /**< byte length of Information Elements */
    int16_t SNR;                   /**< Average SNR(signal to noise ratio) during frame reception */
    /* Add new fields here */
    /* variable length Information Elements */
} wl_bss_info_t;


#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_TYPES_H_ */
