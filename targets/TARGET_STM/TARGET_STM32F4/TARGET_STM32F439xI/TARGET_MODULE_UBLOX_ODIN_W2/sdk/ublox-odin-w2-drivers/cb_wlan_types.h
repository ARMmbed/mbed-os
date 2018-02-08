/*---------------------------------------------------------------------------
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
 *
 * Component   : WLAN driver
 * File        : cb_wlan_types.h
 *
 * Description : Common wireless LAN defines and types.
 *-------------------------------------------------------------------------*/

/**
 * @file cb_wlan_types.h The main WLAN 802.11 interface
 *
 * @ingroup WLANDriver
 */

#ifndef _CB_WLAN_TYPES_H_
#define _CB_WLAN_TYPES_H_

#include "cb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/

/**
 * Max length for an SSID
 *
 * @ingroup wlantypes
 */
#define cbWLAN_SSID_MAX_LENGTH (32)

/**
 * EAPOL ethernet type
 *
 * @ingroup wlantypes
 */
#define cbWLAN_ETHTYPE_EAPOL     (0x888E)

/**
 * Maximum size of a predefined WEP key
 *
 * @ingroup wlantypes
 */
#define cbWLAN_KEY_SIZE_WEP_MAX (cbWLAN_KEY_SIZE_WEP128)

#define cbWLAN_OUI_SIZE                    3

#define cbRATE_MASK_B   (cbRATE_MASK_01 | cbRATE_MASK_02 | cbRATE_MASK_5_5 | cbRATE_MASK_11)
#define cbRATE_MASK_G   (cbRATE_MASK_06 | cbRATE_MASK_09 | cbRATE_MASK_12 | cbRATE_MASK_18 | cbRATE_MASK_24 | cbRATE_MASK_36 | cbRATE_MASK_48 | cbRATE_MASK_54)
#define cbRATE_MASK_N   (cbRATE_MASK_MCS0 | cbRATE_MASK_MCS1 | cbRATE_MASK_MCS2 | cbRATE_MASK_MCS3 | cbRATE_MASK_MCS4 | cbRATE_MASK_MCS5 | cbRATE_MASK_MCS6 | cbRATE_MASK_MCS7)
#define cbRATE_MASK_ALL (cbRATE_MASK_B | cbRATE_MASK_G | cbRATE_MASK_N)

#define cbWLAN_MAX_CHANNEL_LIST_LENGTH      38

#define cbWLAN_TX_POWER_AUTO    0xFF

/*===========================================================================
 * TYPES
 *=========================================================================*/

/** 
 * The encryption mode.
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_EncryptionMode_e {
    cbWLAN_ENC_NONE,
    cbWLAN_ENC_WEP64,
    cbWLAN_ENC_WEP128,
    cbWLAN_ENC_TKIP,
    cbWLAN_ENC_AES,
} cbWLAN_EncryptionMode;


/**  
 * Enterprise authentication mode.
 *
 * @ingroup wlan
 */
typedef enum cbWLAN_EnterpriseMode {
    cbWLAN_ENTERPRISE_MODE_LEAP,
    cbWLAN_ENTERPRISE_MODE_PEAP,
    cbWLAN_ENTERPRISE_MODE_EAPTLS,
} cbWLAN_EnterpriseMode;

/**
 * Key sizes for the supported encryptions.
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_EncryptionKeySize_e {
    cbWLAN_KEY_SIZE_WEP64      = 5,
    cbWLAN_KEY_SIZE_WEP128     = 13,
    cbWLAN_KEY_SIZE_WEP2       = 16,
    cbWLAN_KEY_SIZE_TKIP       = 16,    
    cbWLAN_KEY_SIZE_AES        = 16,
    cbWLAN_KEY_SIZE_TKIP_MIC   = 8
} cbWLAN_EncryptionKeySize;

enum cbWLAN_Channel_e {
    cbWLAN_CHANNEL_ALL = 0,
    cbWLAN_CHANNEL_01 = 1,
    cbWLAN_CHANNEL_02,
    cbWLAN_CHANNEL_03,
    cbWLAN_CHANNEL_04,
    cbWLAN_CHANNEL_05,
    cbWLAN_CHANNEL_06,
    cbWLAN_CHANNEL_07,
    cbWLAN_CHANNEL_08,
    cbWLAN_CHANNEL_09,
    cbWLAN_CHANNEL_10,
    cbWLAN_CHANNEL_11,
    cbWLAN_CHANNEL_12,
    cbWLAN_CHANNEL_13,
    cbWLAN_CHANNEL_14,

    cbWLAN_CHANNEL_36 = 36,
    cbWLAN_CHANNEL_40 = 40,
    cbWLAN_CHANNEL_44 = 44,
    cbWLAN_CHANNEL_48 = 48,
    cbWLAN_CHANNEL_52 = 52,
    cbWLAN_CHANNEL_56 = 56,
    cbWLAN_CHANNEL_60 = 60,
    cbWLAN_CHANNEL_64 = 64,
    cbWLAN_CHANNEL_100 = 100,
    cbWLAN_CHANNEL_104 = 104,
    cbWLAN_CHANNEL_108 = 108,
    cbWLAN_CHANNEL_112 = 112,
    cbWLAN_CHANNEL_116 = 116,
    cbWLAN_CHANNEL_120 = 120,
    cbWLAN_CHANNEL_124 = 124,
    cbWLAN_CHANNEL_128 = 128,
    cbWLAN_CHANNEL_132 = 132,
    cbWLAN_CHANNEL_136 = 136,
    cbWLAN_CHANNEL_140 = 140,
    cbWLAN_CHANNEL_149 = 149,
    cbWLAN_CHANNEL_153 = 153,
    cbWLAN_CHANNEL_157 = 157,
    cbWLAN_CHANNEL_161 = 161,
    cbWLAN_CHANNEL_165 = 165
};

/**
 * WLAN Channels
 * Valid values are found in @ref cbWLAN_Channel_e
 * @ingroup wlantypes
 */
typedef cb_uint8 cbWLAN_Channel;

/**
 * WLAN Channel list
 * @ingroup wlantypes
 */
typedef struct {
    cb_uint32 length;
    cbWLAN_Channel channels[cbWLAN_MAX_CHANNEL_LIST_LENGTH];
} cbWLAN_ChannelList;

/**
 * Standard 802.11 rates
 *
 * @ingroup wlantypes
 */
enum cbWLAN_Rate_e {    
    cbWLAN_RATE_01 = 1, // 1
    cbWLAN_RATE_02,     // 2
    cbWLAN_RATE_5_5,    // 3
    cbWLAN_RATE_06,     // 4
    cbWLAN_RATE_09,     // 5
    cbWLAN_RATE_11,     // 6
    cbWLAN_RATE_12,     // 7
    cbWLAN_RATE_18,     // 8 
    cbWLAN_RATE_24,     // 9 
    cbWLAN_RATE_36,     // 10 
    cbWLAN_RATE_48,     // 11
    cbWLAN_RATE_54,     // 12
    cbWLAN_RATE_MCS0,   // 13
    cbWLAN_RATE_MCS1,   // 14
    cbWLAN_RATE_MCS2,   // 15
    cbWLAN_RATE_MCS3,   // 16
    cbWLAN_RATE_MCS4,   // 17
    cbWLAN_RATE_MCS5,   // 18
    cbWLAN_RATE_MCS6,   // 19
    cbWLAN_RATE_MCS7,   // 20
    cbWLAN_RATE_MCS8,   // 21
    cbWLAN_RATE_MCS9,   // 22
    cbWLAN_RATE_MCS10,  // 23
    cbWLAN_RATE_MCS11,  // 24
    cbWLAN_RATE_MCS12,  // 25
    cbWLAN_RATE_MCS13,  // 26
    cbWLAN_RATE_MCS14,  // 27
    cbWLAN_RATE_MCS15,  // 28
};

/**
 * Type for containing values found in @ref cbWLAN_Rate_e
 * @ingroup wlantypes
 */
typedef cb_uint8 cbWLAN_Rate;


/**
 * Mask bits for standard 802.11 rates
 *
 * @ingroup wlantypes
 */
enum cbWLAN_RateMask_e {    
    cbRATE_MASK_01 = 0x00000001,
    cbRATE_MASK_02 = 0x00000002,
    cbRATE_MASK_5_5 = 0x00000004,
    cbRATE_MASK_11 = 0x00000008,
    cbRATE_MASK_06 = 0x00000010,
    cbRATE_MASK_09 = 0x00000020,
    cbRATE_MASK_12 = 0x00000040,
    cbRATE_MASK_18 = 0x00000080,
    cbRATE_MASK_24 = 0x00000100,
    cbRATE_MASK_36 = 0x00000200,
    cbRATE_MASK_48 = 0x00000400,
    cbRATE_MASK_54 = 0x00000800,
    // NOTE: Don't move MCS rates bit offset, see note on define below
    cbRATE_MASK_MCS0 = 0x00001000,
    cbRATE_MASK_MCS1 = 0x00002000,
    cbRATE_MASK_MCS2 = 0x00004000,
    cbRATE_MASK_MCS3 = 0x00008000,
    cbRATE_MASK_MCS4 = 0x00010000,
    cbRATE_MASK_MCS5 = 0x00020000,
    cbRATE_MASK_MCS6 = 0x00040000,
    cbRATE_MASK_MCS7 = 0x00080000,
    cbRATE_MASK_MCS8 = 0x00100000,
    cbRATE_MASK_MCS9 = 0x00200000,
    cbRATE_MASK_MCS10 = 0x00400000,
    cbRATE_MASK_MCS11 = 0x00800000,
    cbRATE_MASK_MCS12 = 0x01000000,
    cbRATE_MASK_MCS13 = 0x02000000,
    cbRATE_MASK_MCS14 = 0x04000000,
    cbRATE_MASK_MCS15 = 0x08000000,
};

/**
 * Access categories
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_AccessCategory_e {
    cbWLAN_AC_BK = 1, /**< Background  */
    cbWLAN_AC_SP = 2, /**< Background (Spare) */

    cbWLAN_AC_BE = 0, /**< Best effort  */
    cbWLAN_AC_EE = 3, /**< Best effort (Excellent Effort) */

    cbWLAN_AC_CL = 4, /**< Video (Controlled Load) */
    cbWLAN_AC_VI = 5, /**< Video  */

    cbWLAN_AC_VO = 6, /**< Voice */
    cbWLAN_AC_NC = 7, /**< Voice (Network Control)*/
} cbWLAN_AccessCategory;



/**
* connectBlue Hardware Identification
*
* @ingroup types
*/
typedef enum cbWM_ModuleType_e {
    cbWM_MODULE_UNKNOWN,
    cbWM_MODULE_OWL22X,
    cbWM_MODULE_OWL253,
    cbWM_MODULE_OWS451,
    cbWM_MODULE_OWL351,
    cbWM_MODULE_ODIN_W16X = cbWM_MODULE_OWL351,
    cbWM_MODULE_ODIN_W26X,
} cbWM_ModuleType;

/**
 * Mac address type
 *
 * @ingroup wlantypes
 */
typedef cb_uint8 cbWLAN_MACAddress[6];

/**
 * Type for containing values found in @ref cbWLAN_RateMask_e
 * @ingroup wlantypes
 */
typedef cb_uint32 cbWLAN_RateMask;

/**
* Transmission power
*
* @ingroup wlantypes
*/
typedef cb_uint8 cbWLAN_TxPower;

/**
 * The different frequency bands to choose from.
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_Band_e {
    cbWLAN_BAND_UNDEFINED,
    cbWLAN_BAND_2_4GHz,
    cbWLAN_BAND_5GHz,
} cbWLAN_Band;

/**
 * The operational mode.
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_OperationalMode_e {
    cbWLAN_OPMODE_MANAGED,
    cbWLAN_OPMODE_ADHOC,    
} cbWLAN_OperationalMode;

/**
 * Encryption key type
 *
 * @ingroup wlantypes
 */
typedef enum cbWLAN_KeyType_e {
    cbWLAN_KEY_UNICAST,
    cbWLAN_KEY_BROADCAST,
} cbWLAN_KeyType;

typedef enum {
    cbWLAN_CONNECT_MODE_OPEN,
    cbWLAN_CONNECT_MODE_WEP_OPEN,
    cbWLAN_CONNECT_MODE_WPA_PSK,
    cbWLAN_CONNECT_MODE_ENTERPRISE,
} cbWLAN_ConnectMode;

typedef enum {
    cbWLAN_AP_MODE_OPEN,
    cbWLAN_AP_MODE_WEP_OPEN,
    cbWLAN_AP_MODE_WPA_PSK,
    cbWLAN_AP_MODE_ENTERPRISE,
} cbWLAN_ApMode;

/** 
 * Ethernet header
 *
 * @ingroup wlantypes
 */
cb_PACKED_STRUCT_BEGIN(cbWLAN_EthernetHeader) {
    cbWLAN_MACAddress   dest;
    cbWLAN_MACAddress   src;
    cb_uint16           type;
} cb_PACKED_STRUCT_END(cbWLAN_EthernetHeader);


cb_PACKED_STRUCT_BEGIN(cbWLAN_EthernetFrame) {
    cbWLAN_EthernetHeader header;
    cb_uint8            payload[cb_EMPTY_ARRAY];
} cb_PACKED_STRUCT_END(cbWLAN_EthernetFrame);

/** 
 * SNAP header
 *
 * @ingroup wlantypes
 */
cb_PACKED_STRUCT_BEGIN(cbWLAN_SNAPHeader) {
    cb_uint8    dsap;
    cb_uint8    ssap;
    cb_uint8    ctrl;
    cb_uint8    encapsulation[3];
    cb_uint16   ethType;
} cb_PACKED_STRUCT_END(cbWLAN_SNAPHeader);

cb_PACKED_STRUCT_BEGIN(cbWLAN_SNAPFrame) {
    cbWLAN_SNAPHeader header;
    cb_uint8    payload[cb_EMPTY_ARRAY];
} cb_PACKED_STRUCT_END(cbWLAN_SNAPFrame);


/**
 * Defines an ssid.
 *
 * @ingroup wlantypes
 */
typedef struct cbWLAN_Ssid_s {    
    cb_uint8 ssid[cbWLAN_SSID_MAX_LENGTH];
    cb_uint32 ssidLength;
} cbWLAN_Ssid;

/**
 * Defines one wep key.
 *
 * @ingroup wlantypes
 */
typedef struct cbWLAN_WepKey_s {
    cb_uint8 key[cbWLAN_KEY_SIZE_WEP_MAX];
    cb_uint32 length;
} cbWLAN_WEPKey;

/**
* Describes host revisions.
* @see cbWM_Version
*
* @ingroup types
*/
typedef struct {
    struct {
        cb_uint32 major;
        cb_uint32 minor;
        cb_uint32 patch1;
    } software;
    struct {
        const char* id;
    } manufacturer;
} cbWM_DriverRevision;

/**
* Describes firmware revisions.
* @see cbWM_Version
*
* @ingroup types
*/
typedef struct {
    struct {
        cb_uint32 major;
        cb_uint32 minor;
        cb_uint32 patch1;
        cb_uint32 patch2;
    } firmware;
    struct {
        const char* id;
    } manufacturer;
} cbWM_FWRevision;

/**
* Describes firmware revisions. Is divided into three parts; one for the
* host driver side, one for target firmware, and one information string
* descibing the HW manufacturer.
*
* @ingroup types
*/
typedef struct version_st{
    cbWM_DriverRevision host;
    cbWM_FWRevision target;
} cbWM_Version;

/**
* Describes power levels for dynamic power level control.
*
* @ingroup types
*/
typedef struct cbWM_TxPowerSettings_s {
    cbWLAN_TxPower lowTxPowerLevel;
    cbWLAN_TxPower medTxPowerLevel;
    cbWLAN_TxPower maxTxPowerLevel;
} cbWM_TxPowerSettings;

/**
* Describes an access point.
*
* @ingroup types
*/
typedef struct cbWLAN_ApInformation {
    cbWLAN_Ssid                 ssid;                   /**< SSID */
    cbWLAN_MACAddress           bssid;                  /**< BSSID */
    cbWLAN_Channel              channel;                /**< Channel */
} cbWLAN_ApInformation;

/**
* Describes a station connected to an access point.
*
* @ingroup types
*/
typedef struct cbWLAN_ApStaInformation {
    cbWLAN_MACAddress MAC;
} cbWLAN_ApStaInformation;

/*---------------------------------------------------------------------------
 * VARIABLE DECLARATIONS
 *-------------------------------------------------------------------------*/
extern const cbWLAN_MACAddress nullMac;
extern const cbWLAN_MACAddress broadcastMac;

extern const cb_uint8 OUI_Microsoft[cbWLAN_OUI_SIZE];
extern const cb_uint8 OUI_Epigram[cbWLAN_OUI_SIZE];
extern const cb_uint8 OUI_ConnectBlue[cbWLAN_OUI_SIZE];
extern const cb_uint8 OUI_IEEE8021[cbWLAN_OUI_SIZE];

extern const cb_uint8 PATTERN_HTInformationDraft[1];
extern const cb_uint8 PATTERN_TKIP[2];
extern const cb_uint8 PATTERN_WME_IE[3];
extern const cb_uint8 PATTERN_WME_PE[3];

/*---------------------------------------------------------------------------
 * FUNCTIONS
 *-------------------------------------------------------------------------*/

/**
 * Misc
 */

/**
 * Returns the correct frequency @ref cbWLAN_Band band based on the input channel.
 *
 * For @ref cbWLAN_CHANNEL_ALL This function will return @ref cbWLAN_BAND_2_4GHz.
 *
 * @param channel The channel to be queried for band.
 * @return The @ref cbWLAN_Band band for the requested channel.
 */
cbWLAN_Band cbWLAN_getBandFromChannel(cbWLAN_Channel channel);

/**
* Returns the valid rates @ref cbWLAN_RateMask based for the channel.
*
* @param channel The channel to be queried for rates.
* @return The valid rates @ref cbWLAN_RateMask for the requested channel.
*/
cbWLAN_RateMask cbWLAN_getRatesForChannel(cbWLAN_Channel channel);

/**
 * Checks is the input rate is a 802.11n rate or not.
 *
 * @param rate The rate to check
 * @return @ref TRUE if the input rate is an n-rate. @ref FALSE otherwise.
 */
cb_boolean cbWLAN_isNRate(cbWLAN_Rate rate);

/**
 * Checks if a channel is valid 
 *
 * @return @ref TRUE if the channel is valid. @ref FALSE otherwise.
 */
cb_boolean cbWLAN_isValidChannel(cbWLAN_Channel channel);

#ifdef __cplusplus
}
#endif

#endif

