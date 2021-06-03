/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

/** @file whd_types.h
 *  Defines common data types used with WHD
 *
 */

#include <stdint.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#ifndef INCLUDED_WHD_TYPES_H_
#define INCLUDED_WHD_TYPES_H_

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
#define SECURITY_MASK      (WEP_ENABLED | TKIP_ENABLED | AES_ENABLED) /**< Flag to Security mask */

#define ENTERPRISE_ENABLED 0x02000000  /**< Flag to enable Enterprise Security */
#define WPS_ENABLED        0x10000000  /**< Flag to enable WPS Security        */
#define IBSS_ENABLED       0x20000000  /**< Flag to enable IBSS mode           */
#define FBT_ENABLED        0x40000000  /**< Flag to enable FBT                 */

#define PM1_POWERSAVE_MODE          (1) /**< Powersave mode on specified interface without regard for throughput reduction */
#define PM2_POWERSAVE_MODE          (2) /**< Powersave mode on specified interface with High throughput */
#define NO_POWERSAVE_MODE           (0) /**< No Powersave mode */

/**
 * Suppress unused parameter warning
 */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/**
 * Suppress unused variable warning
 */
#define UNUSED_VARIABLE(x) ( (void)(x) )

/**
 * Suppress unused variable warning occurring due to an assert which is disabled in release mode
 */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/******************************************************
*@cond               Type Definitions
******************************************************/
typedef void *whd_buffer_t;
typedef struct wl_bss_info_struct whd_bss_info_t;
typedef struct edcf_acparam whd_edcf_ac_param_t;
typedef struct wl_af_params whd_af_params_t;
typedef struct whd_arp_stats_s whd_arp_stats_t;
typedef struct wl_pkt_filter_stats whd_pkt_filter_stats_t;
typedef struct whd_tko_retry whd_tko_retry_t;
typedef struct whd_tko_connect whd_tko_connect_t;
typedef struct whd_tko_status whd_tko_status_t;
/** @endcond */
/******************************************************
*                    Constants
******************************************************/

#define WIFI_IE_OUI_LENGTH    (3)    /**< OUI length for Information Element */

/* Below constants are used to allocate the buffer pool by the application */

#define BDC_HEADER_WITH_PAD 6  /**< BDC Header with padding 4 + 2 */

/** From bdc header, Ethernet data starts after an offset of (bdc_header->data_offset<<2).
 * It is variable, but usually 4.
 */
#define BDC_HEADER_OFFSET_TO_DATA 4

#define SDPCM_HEADER (8 + 4)  /**< SDPCM SW header + Frame tag */

#define MAX_BUS_HEADER_SIZE 4 /**< Max bus header size for all bus types (sdio/spi) */

#define BUFFER_OVERHEAD 4 /**< Buffer overhead, sizeof(void *) */

/**
 * The maximum space in bytes required for headers in front of the Ethernet header.
 * 6 + (8 + 4) + 4 + 4 + 4 = 30 bytes
 */
#define WHD_LINK_HEADER (BDC_HEADER_WITH_PAD + BDC_HEADER_OFFSET_TO_DATA + \
                         SDPCM_HEADER + MAX_BUS_HEADER_SIZE + BUFFER_OVERHEAD)

/**
 * The size of an Ethernet header
 */
#define WHD_ETHERNET_SIZE         (14)

/**
 * The size in bytes of the Link layer header i.e. the whd specific headers and the Ethernet header
 */
#define WHD_PHYSICAL_HEADER       (WHD_LINK_HEADER + WHD_ETHERNET_SIZE)

/**
 * The maximum size in bytes of the data part of an Ethernet frame
 */
#ifndef WHD_PAYLOAD_MTU
#define WHD_PAYLOAD_MTU           (1500)
#endif

/**
 * The maximum size in bytes of a packet used within whd.
 * Pool should be atleast of this size.
 * Usually buffer pools needs implementation specific headers like pbuf header etc, that should be
 * taken into account along with this during buffer pool creation. Also buffer pools needs alignment with
 * cache size of the platform for better performance
 */
#define WHD_LINK_MTU            (WHD_PAYLOAD_MTU + WHD_PHYSICAL_HEADER)

/** @cond */
#ifdef __x86_64__
typedef uint64_t whd_thread_arg_t;
#else
typedef uint32_t whd_thread_arg_t;
#endif
/** @endcond */
/******************************************************
*               Structures and  Enumerations
******************************************************/
/**
 * Enumeration of Dot11 Reason Codes
 */
typedef enum
{
    WHD_DOT11_RC_RESERVED = 0,   /**< Reserved     */
    WHD_DOT11_RC_UNSPECIFIED = 1 /**< Unspecified  */
} whd_dot11_reason_code_t;

/**
 * Boolean values
 */
typedef enum
{
    WHD_FALSE = 0, /**< Boolean True */
    WHD_TRUE = 1   /**< Boolean False */
} whd_bool_t;


/**
 * Transfer direction for the WHD platform bus interface
 */
typedef enum
{
    /* If updating this enum, the bus_direction_mapping variable will also need to be updated */
    BUS_READ, /**< Specifies bus type and read direction */
    BUS_WRITE /**< Specifies bus type and write direction */
} whd_bus_transfer_direction_t;

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum
{
    WHD_SECURITY_OPEN             = 0,                                                                 /**< Open security                                         */
    WHD_SECURITY_WEP_PSK          = WEP_ENABLED,                                                       /**< WEP PSK Security with open authentication             */
    WHD_SECURITY_WEP_SHARED       = (WEP_ENABLED | SHARED_ENABLED),                                    /**< WEP PSK Security with shared authentication           */
    WHD_SECURITY_WPA_TKIP_PSK     = (WPA_SECURITY | TKIP_ENABLED),                                     /**< WPA PSK Security with TKIP                            */
    WHD_SECURITY_WPA_AES_PSK      = (WPA_SECURITY | AES_ENABLED),                                      /**< WPA PSK Security with AES                             */
    WHD_SECURITY_WPA_MIXED_PSK    = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                       /**< WPA PSK Security with AES & TKIP                      */
    WHD_SECURITY_WPA2_AES_PSK     = (WPA2_SECURITY | AES_ENABLED),                                     /**< WPA2 PSK Security with AES                            */
    WHD_SECURITY_WPA2_TKIP_PSK    = (WPA2_SECURITY | TKIP_ENABLED),                                    /**< WPA2 PSK Security with TKIP                           */
    WHD_SECURITY_WPA2_MIXED_PSK   = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                      /**< WPA2 PSK Security with AES & TKIP                     */
    WHD_SECURITY_WPA2_FBT_PSK     = (WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),                       /**< WPA2 FBT PSK Security with AES & TKIP */
    WHD_SECURITY_WPA3_SAE         = (WPA3_SECURITY | AES_ENABLED),                                     /**< WPA3 Security with AES */
    WHD_SECURITY_WPA2_WPA_AES_PSK  = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED),                     /**< WPA2 WPA PSK Security with AES                        */
    WHD_SECURITY_WPA2_WPA_MIXED_PSK = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),      /**< WPA2 WPA PSK Security with AES & TKIP                 */

    WHD_SECURITY_WPA3_WPA2_PSK    = (WPA3_SECURITY | WPA2_SECURITY | AES_ENABLED),                     /**< WPA3 WPA2 PSK Security with AES */

    WHD_SECURITY_WPA_TKIP_ENT     = (ENTERPRISE_ENABLED | WPA_SECURITY | TKIP_ENABLED),                /**< WPA Enterprise Security with TKIP                     */
    WHD_SECURITY_WPA_AES_ENT      = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED),                 /**< WPA Enterprise Security with AES                      */
    WHD_SECURITY_WPA_MIXED_ENT    = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),  /**< WPA Enterprise Security with AES & TKIP               */
    WHD_SECURITY_WPA2_TKIP_ENT    = (ENTERPRISE_ENABLED | WPA2_SECURITY | TKIP_ENABLED),               /**< WPA2 Enterprise Security with TKIP                    */
    WHD_SECURITY_WPA2_AES_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED),                /**< WPA2 Enterprise Security with AES                     */
    WHD_SECURITY_WPA2_MIXED_ENT   = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED), /**< WPA2 Enterprise Security with AES & TKIP              */
    WHD_SECURITY_WPA2_FBT_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),  /**< WPA2 Enterprise Security with AES & FBT               */

    WHD_SECURITY_IBSS_OPEN        = (IBSS_ENABLED),                                                    /**< Open security on IBSS ad-hoc network                  */
    WHD_SECURITY_WPS_SECURE       = AES_ENABLED,                                                       /**< WPS with AES security                                 */

    WHD_SECURITY_UNKNOWN          = -1,                                                                /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */

    WHD_SECURITY_FORCE_32_BIT     = 0x7fffffff                                                         /**< Exists only to force whd_security_t type to 32 bits */
} whd_security_t;

/**
 * Enumeration of methods of scanning
 */
typedef enum
{
    WHD_SCAN_TYPE_ACTIVE              = 0x00, /**< Actively scan a network by sending 802.11 probe(s)                              */
    WHD_SCAN_TYPE_PASSIVE             = 0x01, /**< Passively scan a network by listening for beacons from APs                      */
    WHD_SCAN_TYPE_PNO                 = 0x02, /**< Use preferred network offload to detect an AP                                   */
    WHD_SCAN_TYPE_PROHIBITED_CHANNELS = 0x04, /**< Permit (passively) scanning a channel that isn't valid for the current country  */
    WHD_SCAN_TYPE_NO_BSSID_FILTER     = 0x08  /**< Return a scan record for each beacon or probe response RX'ed                    */
} whd_scan_type_t;

/**
 * Enumeration of network types
 */
typedef enum
{
    WHD_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    WHD_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    WHD_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */
    WHD_BSS_TYPE_MESH           = 3, /**< Denotes 802.11 mesh network                     */

    WHD_BSS_TYPE_UNKNOWN = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} whd_bss_type_t;

/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
    WHD_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
    WHD_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} whd_802_11_band_t;

/**
 * Enumeration of custom IE(Information Element) management actions
 */
typedef enum
{
    WHD_ADD_CUSTOM_IE,   /**< Add a custom IE(Information Element)    */
    WHD_REMOVE_CUSTOM_IE /**< Remove a custom IE(Information Element) */
} whd_custom_ie_action_t;


/**
 * Enumeration of listen interval time unit types
 */
typedef enum
{
    WHD_LISTEN_INTERVAL_TIME_UNIT_BEACON, /**< Time units specified in beacon periods */
    WHD_LISTEN_INTERVAL_TIME_UNIT_DTIM    /**< Time units specified in DTIM periods   */
} whd_listen_interval_time_unit_t;

/**
 * Structure for storing scan status
 */
typedef enum
{
    WHD_SCAN_INCOMPLETE,                /**< Denotes that scan is not finished */
    WHD_SCAN_COMPLETED_SUCCESSFULLY,    /**< Successful completion of scan */
    WHD_SCAN_ABORTED,                   /**< Scan is aborted */
} whd_scan_status_t;

/**
 * Structure for storing radio band list information
 */
typedef struct
{
    int32_t number_of_bands; /**< Number of bands supported, currently 1 or 2      */
    int32_t current_band;    /**< Current band type: WLC_BAND_2G or WLC_BAND_5G   */
    int32_t other_band;      /**< If value of number_of_bands parameter is 2, then this member specifies the 2nd band */
} whd_band_list_t;

/**
 * Enumeration of scan result flags
 */
typedef enum
{
    WHD_SCAN_RESULT_FLAG_RSSI_OFF_CHANNEL = 0x01, /**< RSSI came from an off channel DSSS (1 or 1 Mb) Rx */
    WHD_SCAN_RESULT_FLAG_BEACON = 0x02            /**< Beacon (vs probe response)                        */
} whd_scan_result_flag_t;

/**
 * Protected Management Frame Capability
 */
enum
{
    WL_MFP_NONE = 0,  /**< Disables the client support for MFP. */
    WL_MFP_CAPABLE,   /**< Allows both MFP-capable and clients that do not support MFP to join the network. */
    WL_MFP_REQUIRED   /**< Clients are allowed to associate only if MFP is negotiated. */
};


/**
 * Enumeration of ioctl get
 */
typedef enum
{
    WHD_IOCTL_GET_RATE = 12,
    WHD_IOCTL_GET_COUNTRY = 83,
    WHD_IOCTL_GET_CLK = 160,
    WHD_IOCTL_GET_WSEC = 133,
    WHD_IOCTL_GET_AUTH = 21,
    WHD_IOCTL_GET_WPA_AUTH = 164,
    WHD_IOCTL_GET_PM = 85,
    WHD_IOCTL_GET_BSSID = 23,
    WHD_IOCTL_GET_ASSOCLIST = 159,
    WHD_IOCTL_GET_BSS_INFO = 136,
    WHD_IOCTL_GET_CHANNEL = 29

} whd_usr_ioctl_get_list_t;

/**
 * Enumeration of ioctl set
 */
typedef enum
{
    WHD_IOCTL_SET_CHANNEL = 30,
    WHD_IOCTL_SET_WSEC_PMK = 268,
    WHD_IOCTL_SET_KEY = 45,
    WHD_IOCTL_SET_WPA_AUTH = 165,
    WHD_IOCTL_SCB_DEAUTHENTICATE_FOR_REASON = 201,
    WHD_IOCTL_SET_PM = 86,
    WHD_IOCTL_SET_SSID = 26,
    WHD_IOCTL_SET_BCNPRD = 76,
    WHD_IOCTL_SET_DTIMPRD = 78,
    WHD_IOCTL_SET_WSEC = 134,
    WHD_IOCTL_SET_INFRA = 20,
    WHD_IOCTL_SET_AUTH = 22

} whd_usr_ioctl_set_list_t;

/**
 * IOVAR set list
 */
typedef enum
{
    WHD_IOVAR_SET_MFP = 0,
    WHD_IOVAR_SET_MPC,
    WHD_IOVAR_SET_AMPDU_BA_WINDOW_SIZE,
    WHD_IOVAR_SET_AMPDU_MPDU,
    WHD_IOVAR_SET_LISTEN_INTERVAL_BEACON,
    WHD_IOVAR_SET_LISTEN_INTERVAL_DTIM,
    WHD_IOVAR_SET_LISTEN_INTERVAL_ASSOC,
} whd_usr_iovar_set_list_t;

/**
 * IOVAR get list
 */
typedef enum
{
    WHD_IOVAR_GET_MFP = 0,
    WHD_IOVAR_GET_MPC,
    WHD_IOVAR_GET_AMPDU_BA_WINDOW_SIZE,
    WHD_IOVAR_GET_AMPDU_MPDU,
    WHD_IOVAR_GET_LISTEN_INTERVAL,          /**< Get Listen Interval value */
    WHD_IOVAR_GET_MAC_ADDRESS,              /**< Get mac address */
} whd_usr_iovar_get_list_t;

/******************************************************
*                 Type Definitions
******************************************************/
/** @cond */
typedef struct whd_event_msg whd_event_header_t;
/** @endcond */

/**
 * Structure for storing a MAC address (Wi-Fi Media Access Control address).
 */
typedef struct
{
    uint8_t octet[6]; /**< Unique 6-byte MAC address */
} whd_mac_t;

/**
 * Structure for storing a Service Set Identifier (i.e. Name of Access Point)
 */
typedef struct
{
    uint8_t length;                /**< SSID length */
    uint8_t value[SSID_NAME_SIZE]; /**< SSID name (AP name)  */
} whd_ssid_t;

/**<
   The received packet formats are different when EXT_STA is enabled. In case
   of EXT_STA the received packets are in 802.11 format, where as in other
   case the received packets have Ethernet II format

   1. 802.11 frames
   ----------------------------------------------------------------------------
 | FC (2) | DID (2) |A1 (6) |A2 (6)|A3 (6) |SID (2) |SNAP (6) |type (2) |data (46 - 1500) |
   ----------------------------------------------------------------------------

   2. Ethernet II frames
   -------------------------------------------------
 | DA (6) | SA (6) | type (2) | data (46 - 1500) |
   -------------------------------------------------
 */


/**
 * Macro for creating country codes according to endianness
 * @cond !ADDTHIS
 */
#ifdef IL_BIGENDIAN
#define MK_CNTRY(a, b, \
                 rev)  ( ( (unsigned char)(b) ) + ( ( (unsigned char)(a) ) << 8 ) + \
                         ( ( (unsigned short)(rev) ) << 16 ) )
#else /* ifdef IL_BIGENDIAN */
#define MK_CNTRY(a, b, \
                 rev)  ( ( (unsigned char)(a) ) + ( ( (unsigned char)(b) ) << 8 ) + \
                         ( ( (unsigned short)(rev) ) << 16 ) )
#endif /* ifdef IL_BIGENDIAN */
/** @endcond */

/**
 * Enumerated list of country codes
 */
typedef enum
{
    WHD_COUNTRY_AFGHANISTAN                           = MK_CNTRY('A', 'F', 0),   /**< AF Afghanistan */
    WHD_COUNTRY_ALBANIA                               = MK_CNTRY('A', 'L', 0),   /**< AL Albania */
    WHD_COUNTRY_ALGERIA                               = MK_CNTRY('D', 'Z', 0),   /**< DZ Algeria */
    WHD_COUNTRY_AMERICAN_SAMOA                        = MK_CNTRY('A', 'S', 0),   /**< AS American_Samoa */
    WHD_COUNTRY_ANGOLA                                = MK_CNTRY('A', 'O', 0),   /**< AO Angola */
    WHD_COUNTRY_ANGUILLA                              = MK_CNTRY('A', 'I', 0),   /**< AI Anguilla */
    WHD_COUNTRY_ANTIGUA_AND_BARBUDA                   = MK_CNTRY('A', 'G', 0),   /**< AG Antigua_and_Barbuda */
    WHD_COUNTRY_ARGENTINA                             = MK_CNTRY('A', 'R', 0),   /**< AR Argentina */
    WHD_COUNTRY_ARMENIA                               = MK_CNTRY('A', 'M', 0),   /**< AM Armenia */
    WHD_COUNTRY_ARUBA                                 = MK_CNTRY('A', 'W', 0),   /**< AW Aruba */
    WHD_COUNTRY_AUSTRALIA                             = MK_CNTRY('A', 'U', 0),   /**< AU Australia */
    WHD_COUNTRY_AUSTRIA                               = MK_CNTRY('A', 'T', 0),   /**< AT Austria */
    WHD_COUNTRY_AZERBAIJAN                            = MK_CNTRY('A', 'Z', 0),   /**< AZ Azerbaijan */
    WHD_COUNTRY_BAHAMAS                               = MK_CNTRY('B', 'S', 0),   /**< BS Bahamas */
    WHD_COUNTRY_BAHRAIN                               = MK_CNTRY('B', 'H', 0),   /**< BH Bahrain */
    WHD_COUNTRY_BAKER_ISLAND                          = MK_CNTRY('0', 'B', 0),   /**< 0B Baker_Island */
    WHD_COUNTRY_BANGLADESH                            = MK_CNTRY('B', 'D', 0),   /**< BD Bangladesh */
    WHD_COUNTRY_BARBADOS                              = MK_CNTRY('B', 'B', 0),   /**< BB Barbados */
    WHD_COUNTRY_BELARUS                               = MK_CNTRY('B', 'Y', 0),   /**< BY Belarus */
    WHD_COUNTRY_BELGIUM                               = MK_CNTRY('B', 'E', 0),   /**< BE Belgium */
    WHD_COUNTRY_BELIZE                                = MK_CNTRY('B', 'Z', 0),   /**< BZ Belize */
    WHD_COUNTRY_BENIN                                 = MK_CNTRY('B', 'J', 0),   /**< BJ Benin */
    WHD_COUNTRY_BERMUDA                               = MK_CNTRY('B', 'M', 0),   /**< BM Bermuda */
    WHD_COUNTRY_BHUTAN                                = MK_CNTRY('B', 'T', 0),   /**< BT Bhutan */
    WHD_COUNTRY_BOLIVIA                               = MK_CNTRY('B', 'O', 0),   /**< BO Bolivia */
    WHD_COUNTRY_BOSNIA_AND_HERZEGOVINA                = MK_CNTRY('B', 'A', 0),   /**< BA Bosnia_and_Herzegovina */
    WHD_COUNTRY_BOTSWANA                              = MK_CNTRY('B', 'W', 0),   /**< BW Botswana */
    WHD_COUNTRY_BRAZIL                                = MK_CNTRY('B', 'R', 0),   /**< BR Brazil */
    WHD_COUNTRY_BRITISH_INDIAN_OCEAN_TERRITORY        = MK_CNTRY('I', 'O', 0),   /**< IO British_Indian_Ocean_Territory */
    WHD_COUNTRY_BRUNEI_DARUSSALAM                     = MK_CNTRY('B', 'N', 0),   /**< BN Brunei_Darussalam */
    WHD_COUNTRY_BULGARIA                              = MK_CNTRY('B', 'G', 0),   /**< BG Bulgaria */
    WHD_COUNTRY_BURKINA_FASO                          = MK_CNTRY('B', 'F', 0),   /**< BF Burkina_Faso */
    WHD_COUNTRY_BURUNDI                               = MK_CNTRY('B', 'I', 0),   /**< BI Burundi */
    WHD_COUNTRY_CAMBODIA                              = MK_CNTRY('K', 'H', 0),   /**< KH Cambodia */
    WHD_COUNTRY_CAMEROON                              = MK_CNTRY('C', 'M', 0),   /**< CM Cameroon */
    WHD_COUNTRY_CANADA                                = MK_CNTRY('C', 'A', 0),   /**< CA Canada */
    WHD_COUNTRY_CANADA_REV950                         = MK_CNTRY('C', 'A', 950), /**< CA Canada Revision 950 */
    WHD_COUNTRY_CAPE_VERDE                            = MK_CNTRY('C', 'V', 0),   /**< CV Cape_Verde */
    WHD_COUNTRY_CAYMAN_ISLANDS                        = MK_CNTRY('K', 'Y', 0),   /**< KY Cayman_Islands */
    WHD_COUNTRY_CENTRAL_AFRICAN_REPUBLIC              = MK_CNTRY('C', 'F', 0),   /**< CF Central_African_Republic */
    WHD_COUNTRY_CHAD                                  = MK_CNTRY('T', 'D', 0),   /**< TD Chad */
    WHD_COUNTRY_CHILE                                 = MK_CNTRY('C', 'L', 0),   /**< CL Chile */
    WHD_COUNTRY_CHINA                                 = MK_CNTRY('C', 'N', 0),   /**< CN China */
    WHD_COUNTRY_CHRISTMAS_ISLAND                      = MK_CNTRY('C', 'X', 0),   /**< CX Christmas_Island */
    WHD_COUNTRY_COLOMBIA                              = MK_CNTRY('C', 'O', 0),   /**< CO Colombia */
    WHD_COUNTRY_COMOROS                               = MK_CNTRY('K', 'M', 0),   /**< KM Comoros */
    WHD_COUNTRY_CONGO                                 = MK_CNTRY('C', 'G', 0),   /**< CG Congo */
    WHD_COUNTRY_CONGO_THE_DEMOCRATIC_REPUBLIC_OF_THE  = MK_CNTRY('C', 'D', 0),   /**< CD Congo,_The_Democratic_Republic_Of_The */
    WHD_COUNTRY_COSTA_RICA                            = MK_CNTRY('C', 'R', 0),   /**< CR Costa_Rica */
    WHD_COUNTRY_COTE_DIVOIRE                          = MK_CNTRY('C', 'I', 0),   /**< CI Cote_D'ivoire */
    WHD_COUNTRY_CROATIA                               = MK_CNTRY('H', 'R', 0),   /**< HR Croatia */
    WHD_COUNTRY_CUBA                                  = MK_CNTRY('C', 'U', 0),   /**< CU Cuba */
    WHD_COUNTRY_CYPRUS                                = MK_CNTRY('C', 'Y', 0),   /**< CY Cyprus */
    WHD_COUNTRY_CZECH_REPUBLIC                        = MK_CNTRY('C', 'Z', 0),   /**< CZ Czech_Republic */
    WHD_COUNTRY_DENMARK                               = MK_CNTRY('D', 'K', 0),   /**< DK Denmark */
    WHD_COUNTRY_DJIBOUTI                              = MK_CNTRY('D', 'J', 0),   /**< DJ Djibouti */
    WHD_COUNTRY_DOMINICA                              = MK_CNTRY('D', 'M', 0),   /**< DM Dominica */
    WHD_COUNTRY_DOMINICAN_REPUBLIC                    = MK_CNTRY('D', 'O', 0),   /**< DO Dominican_Republic */
    WHD_COUNTRY_DOWN_UNDER                            = MK_CNTRY('A', 'U', 0),   /**< AU G'Day mate! */
    WHD_COUNTRY_ECUADOR                               = MK_CNTRY('E', 'C', 0),   /**< EC Ecuador */
    WHD_COUNTRY_EGYPT                                 = MK_CNTRY('E', 'G', 0),   /**< EG Egypt */
    WHD_COUNTRY_EL_SALVADOR                           = MK_CNTRY('S', 'V', 0),   /**< SV El_Salvador */
    WHD_COUNTRY_EQUATORIAL_GUINEA                     = MK_CNTRY('G', 'Q', 0),   /**< GQ Equatorial_Guinea */
    WHD_COUNTRY_ERITREA                               = MK_CNTRY('E', 'R', 0),   /**< ER Eritrea */
    WHD_COUNTRY_ESTONIA                               = MK_CNTRY('E', 'E', 0),   /**< EE Estonia */
    WHD_COUNTRY_ETHIOPIA                              = MK_CNTRY('E', 'T', 0),   /**< ET Ethiopia */
    WHD_COUNTRY_FALKLAND_ISLANDS_MALVINAS             = MK_CNTRY('F', 'K', 0),   /**< FK Falkland_Islands_(Malvinas) */
    WHD_COUNTRY_FAROE_ISLANDS                         = MK_CNTRY('F', 'O', 0),   /**< FO Faroe_Islands */
    WHD_COUNTRY_FIJI                                  = MK_CNTRY('F', 'J', 0),   /**< FJ Fiji */
    WHD_COUNTRY_FINLAND                               = MK_CNTRY('F', 'I', 0),   /**< FI Finland */
    WHD_COUNTRY_FRANCE                                = MK_CNTRY('F', 'R', 0),   /**< FR France */
    WHD_COUNTRY_FRENCH_GUINA                          = MK_CNTRY('G', 'F', 0),   /**< GF French_Guina */
    WHD_COUNTRY_FRENCH_POLYNESIA                      = MK_CNTRY('P', 'F', 0),   /**< PF French_Polynesia */
    WHD_COUNTRY_FRENCH_SOUTHERN_TERRITORIES           = MK_CNTRY('T', 'F', 0),   /**< TF French_Southern_Territories */
    WHD_COUNTRY_GABON                                 = MK_CNTRY('G', 'A', 0),   /**< GA Gabon */
    WHD_COUNTRY_GAMBIA                                = MK_CNTRY('G', 'M', 0),   /**< GM Gambia */
    WHD_COUNTRY_GEORGIA                               = MK_CNTRY('G', 'E', 0),   /**< GE Georgia */
    WHD_COUNTRY_GERMANY                               = MK_CNTRY('D', 'E', 0),   /**< DE Germany */
    WHD_COUNTRY_EUROPEAN_WIDE_REV895                  = MK_CNTRY('E', '0', 895), /**< E0 European_Wide Revision 895 */
    WHD_COUNTRY_GHANA                                 = MK_CNTRY('G', 'H', 0),   /**< GH Ghana */
    WHD_COUNTRY_GIBRALTAR                             = MK_CNTRY('G', 'I', 0),   /**< GI Gibraltar */
    WHD_COUNTRY_GREECE                                = MK_CNTRY('G', 'R', 0),   /**< GR Greece */
    WHD_COUNTRY_GRENADA                               = MK_CNTRY('G', 'D', 0),   /**< GD Grenada */
    WHD_COUNTRY_GUADELOUPE                            = MK_CNTRY('G', 'P', 0),   /**< GP Guadeloupe */
    WHD_COUNTRY_GUAM                                  = MK_CNTRY('G', 'U', 0),   /**< GU Guam */
    WHD_COUNTRY_GUATEMALA                             = MK_CNTRY('G', 'T', 0),   /**< GT Guatemala */
    WHD_COUNTRY_GUERNSEY                              = MK_CNTRY('G', 'G', 0),   /**< GG Guernsey */
    WHD_COUNTRY_GUINEA                                = MK_CNTRY('G', 'N', 0),   /**< GN Guinea */
    WHD_COUNTRY_GUINEA_BISSAU                         = MK_CNTRY('G', 'W', 0),   /**< GW Guinea-bissau */
    WHD_COUNTRY_GUYANA                                = MK_CNTRY('G', 'Y', 0),   /**< GY Guyana */
    WHD_COUNTRY_HAITI                                 = MK_CNTRY('H', 'T', 0),   /**< HT Haiti */
    WHD_COUNTRY_HOLY_SEE_VATICAN_CITY_STATE           = MK_CNTRY('V', 'A', 0),   /**< VA Holy_See_(Vatican_City_State) */
    WHD_COUNTRY_HONDURAS                              = MK_CNTRY('H', 'N', 0),   /**< HN Honduras */
    WHD_COUNTRY_HONG_KONG                             = MK_CNTRY('H', 'K', 0),   /**< HK Hong_Kong */
    WHD_COUNTRY_HUNGARY                               = MK_CNTRY('H', 'U', 0),   /**< HU Hungary */
    WHD_COUNTRY_ICELAND                               = MK_CNTRY('I', 'S', 0),   /**< IS Iceland */
    WHD_COUNTRY_INDIA                                 = MK_CNTRY('I', 'N', 0),   /**< IN India */
    WHD_COUNTRY_INDONESIA                             = MK_CNTRY('I', 'D', 0),   /**< ID Indonesia */
    WHD_COUNTRY_IRAN_ISLAMIC_REPUBLIC_OF              = MK_CNTRY('I', 'R', 0),   /**< IR Iran,_Islamic_Republic_Of */
    WHD_COUNTRY_IRAQ                                  = MK_CNTRY('I', 'Q', 0),   /**< IQ Iraq */
    WHD_COUNTRY_IRELAND                               = MK_CNTRY('I', 'E', 0),   /**< IE Ireland */
    WHD_COUNTRY_ISRAEL                                = MK_CNTRY('I', 'L', 0),   /**< IL Israel */
    WHD_COUNTRY_ITALY                                 = MK_CNTRY('I', 'T', 0),   /**< IT Italy */
    WHD_COUNTRY_JAMAICA                               = MK_CNTRY('J', 'M', 0),   /**< JM Jamaica */
    WHD_COUNTRY_JAPAN                                 = MK_CNTRY('J', 'P', 0),   /**< JP Japan */
    WHD_COUNTRY_JERSEY                                = MK_CNTRY('J', 'E', 0),   /**< JE Jersey */
    WHD_COUNTRY_JORDAN                                = MK_CNTRY('J', 'O', 0),   /**< JO Jordan */
    WHD_COUNTRY_KAZAKHSTAN                            = MK_CNTRY('K', 'Z', 0),   /**< KZ Kazakhstan */
    WHD_COUNTRY_KENYA                                 = MK_CNTRY('K', 'E', 0),   /**< KE Kenya */
    WHD_COUNTRY_KIRIBATI                              = MK_CNTRY('K', 'I', 0),   /**< KI Kiribati */
    WHD_COUNTRY_KOREA_REPUBLIC_OF                     = MK_CNTRY('K', 'R', 1),   /**< KR Korea,_Republic_Of */
    WHD_COUNTRY_KOSOVO                                = MK_CNTRY('0', 'A', 0),   /**< 0A Kosovo */
    WHD_COUNTRY_KUWAIT                                = MK_CNTRY('K', 'W', 0),   /**< KW Kuwait */
    WHD_COUNTRY_KYRGYZSTAN                            = MK_CNTRY('K', 'G', 0),   /**< KG Kyrgyzstan */
    WHD_COUNTRY_LAO_PEOPLES_DEMOCRATIC_REPUBIC        = MK_CNTRY('L', 'A', 0),   /**< LA Lao_People's_Democratic_Repubic */
    WHD_COUNTRY_LATVIA                                = MK_CNTRY('L', 'V', 0),   /**< LV Latvia */
    WHD_COUNTRY_LEBANON                               = MK_CNTRY('L', 'B', 0),   /**< LB Lebanon */
    WHD_COUNTRY_LESOTHO                               = MK_CNTRY('L', 'S', 0),   /**< LS Lesotho */
    WHD_COUNTRY_LIBERIA                               = MK_CNTRY('L', 'R', 0),   /**< LR Liberia */
    WHD_COUNTRY_LIBYAN_ARAB_JAMAHIRIYA                = MK_CNTRY('L', 'Y', 0),   /**< LY Libyan_Arab_Jamahiriya */
    WHD_COUNTRY_LIECHTENSTEIN                         = MK_CNTRY('L', 'I', 0),   /**< LI Liechtenstein */
    WHD_COUNTRY_LITHUANIA                             = MK_CNTRY('L', 'T', 0),   /**< LT Lithuania */
    WHD_COUNTRY_LUXEMBOURG                            = MK_CNTRY('L', 'U', 0),   /**< LU Luxembourg */
    WHD_COUNTRY_MACAO                                 = MK_CNTRY('M', 'O', 0),   /**< MO Macao */
    WHD_COUNTRY_MACEDONIA_FORMER_YUGOSLAV_REPUBLIC_OF = MK_CNTRY('M', 'K', 0),   /**< MK Macedonia,_Former_Yugoslav_Republic_Of */
    WHD_COUNTRY_MADAGASCAR                            = MK_CNTRY('M', 'G', 0),   /**< MG Madagascar */
    WHD_COUNTRY_MALAWI                                = MK_CNTRY('M', 'W', 0),   /**< MW Malawi */
    WHD_COUNTRY_MALAYSIA                              = MK_CNTRY('M', 'Y', 0),   /**< MY Malaysia */
    WHD_COUNTRY_MALDIVES                              = MK_CNTRY('M', 'V', 0),   /**< MV Maldives */
    WHD_COUNTRY_MALI                                  = MK_CNTRY('M', 'L', 0),   /**< ML Mali */
    WHD_COUNTRY_MALTA                                 = MK_CNTRY('M', 'T', 0),   /**< MT Malta */
    WHD_COUNTRY_MAN_ISLE_OF                           = MK_CNTRY('I', 'M', 0),   /**< IM Man,_Isle_Of */
    WHD_COUNTRY_MARTINIQUE                            = MK_CNTRY('M', 'Q', 0),   /**< MQ Martinique */
    WHD_COUNTRY_MAURITANIA                            = MK_CNTRY('M', 'R', 0),   /**< MR Mauritania */
    WHD_COUNTRY_MAURITIUS                             = MK_CNTRY('M', 'U', 0),   /**< MU Mauritius */
    WHD_COUNTRY_MAYOTTE                               = MK_CNTRY('Y', 'T', 0),   /**< YT Mayotte */
    WHD_COUNTRY_MEXICO                                = MK_CNTRY('M', 'X', 0),   /**< MX Mexico */
    WHD_COUNTRY_MICRONESIA_FEDERATED_STATES_OF        = MK_CNTRY('F', 'M', 0),   /**< FM Micronesia,_Federated_States_Of */
    WHD_COUNTRY_MOLDOVA_REPUBLIC_OF                   = MK_CNTRY('M', 'D', 0),   /**< MD Moldova,_Republic_Of */
    WHD_COUNTRY_MONACO                                = MK_CNTRY('M', 'C', 0),   /**< MC Monaco */
    WHD_COUNTRY_MONGOLIA                              = MK_CNTRY('M', 'N', 0),   /**< MN Mongolia */
    WHD_COUNTRY_MONTENEGRO                            = MK_CNTRY('M', 'E', 0),   /**< ME Montenegro */
    WHD_COUNTRY_MONTSERRAT                            = MK_CNTRY('M', 'S', 0),   /**< MS Montserrat */
    WHD_COUNTRY_MOROCCO                               = MK_CNTRY('M', 'A', 0),   /**< MA Morocco */
    WHD_COUNTRY_MOZAMBIQUE                            = MK_CNTRY('M', 'Z', 0),   /**< MZ Mozambique */
    WHD_COUNTRY_MYANMAR                               = MK_CNTRY('M', 'M', 0),   /**< MM Myanmar */
    WHD_COUNTRY_NAMIBIA                               = MK_CNTRY('N', 'A', 0),   /**< NA Namibia */
    WHD_COUNTRY_NAURU                                 = MK_CNTRY('N', 'R', 0),   /**< NR Nauru */
    WHD_COUNTRY_NEPAL                                 = MK_CNTRY('N', 'P', 0),   /**< NP Nepal */
    WHD_COUNTRY_NETHERLANDS                           = MK_CNTRY('N', 'L', 0),   /**< NL Netherlands */
    WHD_COUNTRY_NETHERLANDS_ANTILLES                  = MK_CNTRY('A', 'N', 0),   /**< AN Netherlands_Antilles */
    WHD_COUNTRY_NEW_CALEDONIA                         = MK_CNTRY('N', 'C', 0),   /**< NC New_Caledonia */
    WHD_COUNTRY_NEW_ZEALAND                           = MK_CNTRY('N', 'Z', 0),   /**< NZ New_Zealand */
    WHD_COUNTRY_NICARAGUA                             = MK_CNTRY('N', 'I', 0),   /**< NI Nicaragua */
    WHD_COUNTRY_NIGER                                 = MK_CNTRY('N', 'E', 0),   /**< NE Niger */
    WHD_COUNTRY_NIGERIA                               = MK_CNTRY('N', 'G', 0),   /**< NG Nigeria */
    WHD_COUNTRY_NORFOLK_ISLAND                        = MK_CNTRY('N', 'F', 0),   /**< NF Norfolk_Island */
    WHD_COUNTRY_NORTHERN_MARIANA_ISLANDS              = MK_CNTRY('M', 'P', 0),   /**< MP Northern_Mariana_Islands */
    WHD_COUNTRY_NORWAY                                = MK_CNTRY('N', 'O', 0),   /**< NO Norway */
    WHD_COUNTRY_OMAN                                  = MK_CNTRY('O', 'M', 0),   /**< OM Oman */
    WHD_COUNTRY_PAKISTAN                              = MK_CNTRY('P', 'K', 0),   /**< PK Pakistan */
    WHD_COUNTRY_PALAU                                 = MK_CNTRY('P', 'W', 0),   /**< PW Palau */
    WHD_COUNTRY_PANAMA                                = MK_CNTRY('P', 'A', 0),   /**< PA Panama */
    WHD_COUNTRY_PAPUA_NEW_GUINEA                      = MK_CNTRY('P', 'G', 0),   /**< PG Papua_New_Guinea */
    WHD_COUNTRY_PARAGUAY                              = MK_CNTRY('P', 'Y', 0),   /**< PY Paraguay */
    WHD_COUNTRY_PERU                                  = MK_CNTRY('P', 'E', 0),   /**< PE Peru */
    WHD_COUNTRY_PHILIPPINES                           = MK_CNTRY('P', 'H', 0),   /**< PH Philippines */
    WHD_COUNTRY_POLAND                                = MK_CNTRY('P', 'L', 0),   /**< PL Poland */
    WHD_COUNTRY_PORTUGAL                              = MK_CNTRY('P', 'T', 0),   /**< PT Portugal */
    WHD_COUNTRY_PUETO_RICO                            = MK_CNTRY('P', 'R', 0),   /**< PR Pueto_Rico */
    WHD_COUNTRY_QATAR                                 = MK_CNTRY('Q', 'A', 0),   /**< QA Qatar */
    WHD_COUNTRY_REUNION                               = MK_CNTRY('R', 'E', 0),   /**< RE Reunion */
    WHD_COUNTRY_ROMANIA                               = MK_CNTRY('R', 'O', 0),   /**< RO Romania */
    WHD_COUNTRY_RUSSIAN_FEDERATION                    = MK_CNTRY('R', 'U', 0),   /**< RU Russian_Federation */
    WHD_COUNTRY_RWANDA                                = MK_CNTRY('R', 'W', 0),   /**< RW Rwanda */
    WHD_COUNTRY_SAINT_KITTS_AND_NEVIS                 = MK_CNTRY('K', 'N', 0),   /**< KN Saint_Kitts_and_Nevis */
    WHD_COUNTRY_SAINT_LUCIA                           = MK_CNTRY('L', 'C', 0),   /**< LC Saint_Lucia */
    WHD_COUNTRY_SAINT_PIERRE_AND_MIQUELON             = MK_CNTRY('P', 'M', 0),   /**< PM Saint_Pierre_and_Miquelon */
    WHD_COUNTRY_SAINT_VINCENT_AND_THE_GRENADINES      = MK_CNTRY('V', 'C', 0),   /**< VC Saint_Vincent_and_The_Grenadines */
    WHD_COUNTRY_SAMOA                                 = MK_CNTRY('W', 'S', 0),   /**< WS Samoa */
    WHD_COUNTRY_SANIT_MARTIN_SINT_MARTEEN             = MK_CNTRY('M', 'F', 0),   /**< MF Sanit_Martin_/_Sint_Marteen */
    WHD_COUNTRY_SAO_TOME_AND_PRINCIPE                 = MK_CNTRY('S', 'T', 0),   /**< ST Sao_Tome_and_Principe */
    WHD_COUNTRY_SAUDI_ARABIA                          = MK_CNTRY('S', 'A', 0),   /**< SA Saudi_Arabia */
    WHD_COUNTRY_SENEGAL                               = MK_CNTRY('S', 'N', 0),   /**< SN Senegal */
    WHD_COUNTRY_SERBIA                                = MK_CNTRY('R', 'S', 0),   /**< RS Serbia */
    WHD_COUNTRY_SEYCHELLES                            = MK_CNTRY('S', 'C', 0),   /**< SC Seychelles */
    WHD_COUNTRY_SIERRA_LEONE                          = MK_CNTRY('S', 'L', 0),   /**< SL Sierra_Leone */
    WHD_COUNTRY_SINGAPORE                             = MK_CNTRY('S', 'G', 0),   /**< SG Singapore */
    WHD_COUNTRY_SLOVAKIA                              = MK_CNTRY('S', 'K', 0),   /**< SK Slovakia */
    WHD_COUNTRY_SLOVENIA                              = MK_CNTRY('S', 'I', 0),   /**< SI Slovenia */
    WHD_COUNTRY_SOLOMON_ISLANDS                       = MK_CNTRY('S', 'B', 0),   /**< SB Solomon_Islands */
    WHD_COUNTRY_SOMALIA                               = MK_CNTRY('S', 'O', 0),   /**< SO Somalia */
    WHD_COUNTRY_SOUTH_AFRICA                          = MK_CNTRY('Z', 'A', 0),   /**< ZA South_Africa */
    WHD_COUNTRY_SPAIN                                 = MK_CNTRY('E', 'S', 0),   /**< ES Spain */
    WHD_COUNTRY_SRI_LANKA                             = MK_CNTRY('L', 'K', 0),   /**< LK Sri_Lanka */
    WHD_COUNTRY_SURINAME                              = MK_CNTRY('S', 'R', 0),   /**< SR Suriname */
    WHD_COUNTRY_SWAZILAND                             = MK_CNTRY('S', 'Z', 0),   /**< SZ Swaziland */
    WHD_COUNTRY_SWEDEN                                = MK_CNTRY('S', 'E', 0),   /**< SE Sweden */
    WHD_COUNTRY_SWITZERLAND                           = MK_CNTRY('C', 'H', 0),   /**< CH Switzerland */
    WHD_COUNTRY_SYRIAN_ARAB_REPUBLIC                  = MK_CNTRY('S', 'Y', 0),   /**< SY Syrian_Arab_Republic */
    WHD_COUNTRY_TAIWAN_PROVINCE_OF_CHINA              = MK_CNTRY('T', 'W', 0),   /**< TW Taiwan,_Province_Of_China */
    WHD_COUNTRY_TAJIKISTAN                            = MK_CNTRY('T', 'J', 0),   /**< TJ Tajikistan */
    WHD_COUNTRY_TANZANIA_UNITED_REPUBLIC_OF           = MK_CNTRY('T', 'Z', 0),   /**< TZ Tanzania,_United_Republic_Of */
    WHD_COUNTRY_THAILAND                              = MK_CNTRY('T', 'H', 0),   /**< TH Thailand */
    WHD_COUNTRY_TOGO                                  = MK_CNTRY('T', 'G', 0),   /**< TG Togo */
    WHD_COUNTRY_TONGA                                 = MK_CNTRY('T', 'O', 0),   /**< TO Tonga */
    WHD_COUNTRY_TRINIDAD_AND_TOBAGO                   = MK_CNTRY('T', 'T', 0),   /**< TT Trinidad_and_Tobago */
    WHD_COUNTRY_TUNISIA                               = MK_CNTRY('T', 'N', 0),   /**< TN Tunisia */
    WHD_COUNTRY_TURKEY                                = MK_CNTRY('T', 'R', 0),   /**< TR Turkey */
    WHD_COUNTRY_TURKMENISTAN                          = MK_CNTRY('T', 'M', 0),   /**< TM Turkmenistan */
    WHD_COUNTRY_TURKS_AND_CAICOS_ISLANDS              = MK_CNTRY('T', 'C', 0),   /**< TC Turks_and_Caicos_Islands */
    WHD_COUNTRY_TUVALU                                = MK_CNTRY('T', 'V', 0),   /**< TV Tuvalu */
    WHD_COUNTRY_UGANDA                                = MK_CNTRY('U', 'G', 0),   /**< UG Uganda */
    WHD_COUNTRY_UKRAINE                               = MK_CNTRY('U', 'A', 0),   /**< UA Ukraine */
    WHD_COUNTRY_UNITED_ARAB_EMIRATES                  = MK_CNTRY('A', 'E', 0),   /**< AE United_Arab_Emirates */
    WHD_COUNTRY_UNITED_KINGDOM                        = MK_CNTRY('G', 'B', 0),   /**< GB United_Kingdom */
    WHD_COUNTRY_UNITED_STATES                         = MK_CNTRY('U', 'S', 0),   /**< US United_States */
    WHD_COUNTRY_UNITED_STATES_REV4                    = MK_CNTRY('U', 'S', 4),   /**< US United_States Revision 4 */
    WHD_COUNTRY_UNITED_STATES_REV931                  = MK_CNTRY('Q', '1', 931), /**< Q1 United_States Revision 931 */
    WHD_COUNTRY_UNITED_STATES_NO_DFS                  = MK_CNTRY('Q', '2', 0),   /**< Q2 United_States_(No_DFS) */
    WHD_COUNTRY_UNITED_STATES_MINOR_OUTLYING_ISLANDS  = MK_CNTRY('U', 'M', 0),   /**< UM United_States_Minor_Outlying_Islands */
    WHD_COUNTRY_URUGUAY                               = MK_CNTRY('U', 'Y', 0),   /**< UY Uruguay */
    WHD_COUNTRY_UZBEKISTAN                            = MK_CNTRY('U', 'Z', 0),   /**< UZ Uzbekistan */
    WHD_COUNTRY_VANUATU                               = MK_CNTRY('V', 'U', 0),   /**< VU Vanuatu */
    WHD_COUNTRY_VENEZUELA                             = MK_CNTRY('V', 'E', 0),   /**< VE Venezuela */
    WHD_COUNTRY_VIET_NAM                              = MK_CNTRY('V', 'N', 0),   /**< VN Viet_Nam */
    WHD_COUNTRY_VIRGIN_ISLANDS_BRITISH                = MK_CNTRY('V', 'G', 0),   /**< VG Virgin_Islands,_British */
    WHD_COUNTRY_VIRGIN_ISLANDS_US                     = MK_CNTRY('V', 'I', 0),   /**< VI Virgin_Islands,_U.S. */
    WHD_COUNTRY_WALLIS_AND_FUTUNA                     = MK_CNTRY('W', 'F', 0),   /**< WF Wallis_and_Futuna */
    WHD_COUNTRY_WEST_BANK                             = MK_CNTRY('0', 'C', 0),   /**< 0C West_Bank */
    WHD_COUNTRY_WESTERN_SAHARA                        = MK_CNTRY('E', 'H', 0),   /**< EH Western_Sahara */
    WHD_COUNTRY_WORLD_WIDE_XV_REV983                  = MK_CNTRY('X', 'V', 983), /**< Worldwide Locale Revision 983 */
    WHD_COUNTRY_WORLD_WIDE_XX                         = MK_CNTRY('X', 'X', 0),   /**< Worldwide Locale (passive Ch12-14) */
    WHD_COUNTRY_WORLD_WIDE_XX_REV17                   = MK_CNTRY('X', 'X', 17),  /**< Worldwide Locale (passive Ch12-14) Revision 17 */
    WHD_COUNTRY_YEMEN                                 = MK_CNTRY('Y', 'E', 0),   /**< YE Yemen */
    WHD_COUNTRY_ZAMBIA                                = MK_CNTRY('Z', 'M', 0),   /**< ZM Zambia */
    WHD_COUNTRY_ZIMBABWE                              = MK_CNTRY('Z', 'W', 0),   /**< ZW Zimbabwe */
} whd_country_code_t;

/**
 * Structure for storing extended scan parameters
 */
typedef struct
{
    int32_t number_of_probes_per_channel;                     /**< Number of probes to send on each channel                                               */
    int32_t scan_active_dwell_time_per_channel_ms;            /**< Period of time to wait on each channel when active scanning                            */
    int32_t scan_passive_dwell_time_per_channel_ms;           /**< Period of time to wait on each channel when passive scanning                           */
    int32_t scan_home_channel_dwell_time_between_channels_ms; /**< Period of time to wait on the home channel when scanning. Only relevant if associated. */
} whd_scan_extended_params_t;

/**
 * Structure for storing scan results
 */
#pragma pack(1)
typedef struct whd_scan_result
{
    whd_ssid_t SSID;                            /**< Service Set Identification (i.e. Name of Access Point)                    */
    whd_mac_t BSSID;                            /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength;                    /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    uint32_t max_data_rate;                     /**< Maximum data rate in kilobits/s                                           */
    whd_bss_type_t bss_type;                    /**< Network type                                                              */
    whd_security_t security;                    /**< Security type                                                             */
    uint8_t channel;                            /**< Radio channel that the AP beacon was received on                          */
    whd_802_11_band_t band;                     /**< Radio band                                                                */
    uint8_t ccode[2];                           /**< Two letter ISO country code from AP                                       */
    uint8_t flags;                              /**< flags                                                                     */
    struct whd_scan_result *next;               /**< Pointer to the next scan result                                           */
    uint8_t *ie_ptr;                            /**< Pointer to received Beacon/Probe Response IE(Information Element)         */
    uint32_t ie_len;                            /**< Length of IE(Information Element)                                         */
} whd_scan_result_t;
#pragma pack()

/**
 * Structure to store scan result parameters for each AP
 */
typedef struct whd_simple_scan_result
{
    whd_ssid_t SSID;         /**< Service Set Identification (i.e. Name of Access Point)                    */
    whd_mac_t BSSID;         /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength; /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    whd_security_t security; /**< Security type                                                             */
    uint8_t channel;         /**< Radio channel that the AP beacon was received on                          */
} whd_sync_scan_result_t;

typedef uint16_t wl_chanspec_t;  /**< Channel specified in uint16_t */
#define MCSSET_LEN    16 /**< Maximum allowed mcs rate */

/** BSS(Basic Service Set) information structure
 *
 * Applications MUST CHECK ie_offset field and length field to access IEs(Information Elements) and
 * next bss_info structure in a vector (in whd_sync_scan_result_t)
 */
typedef struct wl_bss_info_struct
{
    uint32_t version;              /**< version field */
    uint32_t length;               /**< byte length of data in this record, starting at version and including IEs */
    whd_mac_t BSSID;               /**< Unique 6-byte MAC address */
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

/** Structure for storing 802.11 powersave listen interval values \n
 *  See @ref whd_wifi_get_listen_interval for more information
 */
typedef struct
{
    uint8_t beacon; /**< Listen interval in beacon periods */
    uint8_t dtim;   /**< Listen interval in DTIM periods   */
    uint16_t assoc; /**< Listen interval as sent to APs    */
} whd_listen_interval_t;

/**
 * WHD result is specified as uint32_t value
 */
typedef uint32_t whd_result_t;

/* whd_result_t error code format
 * |31-18 (14 bit) for module id|17-16 (2 bit) for result type|15-0 for whd error code|
 */
#define WHD_RESULT_TYPE 0   /**< WHD Result type */
#define WHD_RESULT_CREATE(x) CY_RSLT_CREATE(WHD_RESULT_TYPE, CY_RSLT_MODULE_DRIVERS_WHD_BASE, (x) ) /**< Create a result value from the specified type, module, and result code */

#define WHD_SUCCESS                         CY_RSLT_SUCCESS        /**< Success */
#define WHD_PENDING                         WHD_RESULT_CREATE(1)   /**< Pending */
#define WHD_TIMEOUT                         WHD_RESULT_CREATE(2)   /**< Timeout */
#define WHD_BADARG                          WHD_RESULT_CREATE(5)   /**< Bad Arguments */
#define WHD_UNFINISHED                      WHD_RESULT_CREATE(10)  /**< Operation not finished yet WHD_RESULT_CREATE(maybe aborted) */

#define WHD_PARTIAL_RESULTS              WHD_RESULT_CREATE(1003)   /**< Partial results */
#define WHD_INVALID_KEY                  WHD_RESULT_CREATE(1004)   /**< Invalid key */
#define WHD_DOES_NOT_EXIST               WHD_RESULT_CREATE(1005)   /**< Does not exist */
#define WHD_NOT_AUTHENTICATED            WHD_RESULT_CREATE(1006)   /**< Not authenticated */
#define WHD_NOT_KEYED                    WHD_RESULT_CREATE(1007)   /**< Not keyed */
#define WHD_IOCTL_FAIL                   WHD_RESULT_CREATE(1008)   /**< IOCTL fail */
#define WHD_BUFFER_UNAVAILABLE_TEMPORARY WHD_RESULT_CREATE(1009)   /**< Buffer unavailable temporarily */
#define WHD_BUFFER_UNAVAILABLE_PERMANENT WHD_RESULT_CREATE(1010)   /**< Buffer unavailable permanently */
#define WHD_CONNECTION_LOST              WHD_RESULT_CREATE(1012)   /**< Connection lost */
#define WHD_OUT_OF_EVENT_HANDLER_SPACE   WHD_RESULT_CREATE(1013)   /**< Cannot add extra event handler */
#define WHD_SEMAPHORE_ERROR              WHD_RESULT_CREATE(1014)   /**< Error manipulating a semaphore */
#define WHD_FLOW_CONTROLLED              WHD_RESULT_CREATE(1015)   /**< Packet retrieval cancelled due to flow control */
#define WHD_NO_CREDITS                   WHD_RESULT_CREATE(1016)   /**< Packet retrieval cancelled due to lack of bus credits */
#define WHD_NO_PACKET_TO_SEND            WHD_RESULT_CREATE(1017)   /**< Packet retrieval cancelled due to no pending packets */
#define WHD_CORE_CLOCK_NOT_ENABLED       WHD_RESULT_CREATE(1018)   /**< Core disabled due to no clock */
#define WHD_CORE_IN_RESET                WHD_RESULT_CREATE(1019)   /**< Core disabled - in reset */
#define WHD_UNSUPPORTED                  WHD_RESULT_CREATE(1020)   /**< Unsupported function */
#define WHD_BUS_WRITE_REGISTER_ERROR     WHD_RESULT_CREATE(1021)   /**< Error writing to WLAN register */
#define WHD_SDIO_BUS_UP_FAIL             WHD_RESULT_CREATE(1022)   /**< SDIO bus failed to come up */
#define WHD_JOIN_IN_PROGRESS             WHD_RESULT_CREATE(1023)   /**< Join not finished yet */
#define WHD_NETWORK_NOT_FOUND            WHD_RESULT_CREATE(1024)   /**< Specified network was not found */
#define WHD_INVALID_JOIN_STATUS          WHD_RESULT_CREATE(1025)   /**< Join status error */
#define WHD_UNKNOWN_INTERFACE            WHD_RESULT_CREATE(1026)   /**< Unknown interface specified */
#define WHD_SDIO_RX_FAIL                 WHD_RESULT_CREATE(1027)   /**< Error during SDIO receive */
#define WHD_HWTAG_MISMATCH               WHD_RESULT_CREATE(1028)   /**< Hardware tag header corrupt */
#define WHD_RX_BUFFER_ALLOC_FAIL         WHD_RESULT_CREATE(1029)   /**< Failed to allocate a buffer to receive into */
#define WHD_BUS_READ_REGISTER_ERROR      WHD_RESULT_CREATE(1030)   /**< Error reading a bus hardware register */
#define WHD_THREAD_CREATE_FAILED         WHD_RESULT_CREATE(1031)   /**< Failed to create a new thread */
#define WHD_QUEUE_ERROR                  WHD_RESULT_CREATE(1032)   /**< Error manipulating a queue */
#define WHD_BUFFER_POINTER_MOVE_ERROR    WHD_RESULT_CREATE(1033)   /**< Error moving the current pointer of a packet buffer  */
#define WHD_BUFFER_SIZE_SET_ERROR        WHD_RESULT_CREATE(1034)   /**< Error setting size of packet buffer */
#define WHD_THREAD_STACK_NULL            WHD_RESULT_CREATE(1035)   /**< Null stack pointer passed when non null was reqired */
#define WHD_THREAD_DELETE_FAIL           WHD_RESULT_CREATE(1036)   /**< Error deleting a thread */
#define WHD_SLEEP_ERROR                  WHD_RESULT_CREATE(1037)   /**< Error sleeping a thread */
#define WHD_BUFFER_ALLOC_FAIL            WHD_RESULT_CREATE(1038)   /**< Failed to allocate a packet buffer */
#define WHD_NO_PACKET_TO_RECEIVE         WHD_RESULT_CREATE(1039)   /**< No Packets waiting to be received */
#define WHD_INTERFACE_NOT_UP             WHD_RESULT_CREATE(1040)   /**< Requested interface is not active */
#define WHD_DELAY_TOO_LONG               WHD_RESULT_CREATE(1041)   /**< Requested delay is too long */
#define WHD_INVALID_DUTY_CYCLE           WHD_RESULT_CREATE(1042)   /**< Duty cycle is outside limit 0 to 100 */
#define WHD_PMK_WRONG_LENGTH             WHD_RESULT_CREATE(1043)   /**< Returned pmk was the wrong length */
#define WHD_UNKNOWN_SECURITY_TYPE        WHD_RESULT_CREATE(1044)   /**< AP security type was unknown */
#define WHD_WEP_NOT_ALLOWED              WHD_RESULT_CREATE(1045)   /**< AP not allowed to use WEP - it is not secure - use Open instead */
#define WHD_WPA_KEYLEN_BAD               WHD_RESULT_CREATE(1046)   /**< WPA / WPA2 key length must be between 8 & 64 bytes */
#define WHD_FILTER_NOT_FOUND             WHD_RESULT_CREATE(1047)   /**< Specified filter id not found */
#define WHD_SPI_ID_READ_FAIL             WHD_RESULT_CREATE(1048)   /**< Failed to read 0xfeedbead SPI id from chip */
#define WHD_SPI_SIZE_MISMATCH            WHD_RESULT_CREATE(1049)   /**< Mismatch in sizes between SPI header and SDPCM header */
#define WHD_ADDRESS_ALREADY_REGISTERED   WHD_RESULT_CREATE(1050)   /**< Attempt to register a multicast address twice */
#define WHD_SDIO_RETRIES_EXCEEDED        WHD_RESULT_CREATE(1051)   /**< SDIO transfer failed too many times. */
#define WHD_NULL_PTR_ARG                 WHD_RESULT_CREATE(1052)   /**< Null Pointer argument passed to function. */
#define WHD_THREAD_FINISH_FAIL           WHD_RESULT_CREATE(1053)   /**< Error deleting a thread */
#define WHD_WAIT_ABORTED                 WHD_RESULT_CREATE(1054)   /**< Semaphore/mutex wait has been aborted */
#define WHD_SET_BLOCK_ACK_WINDOW_FAIL    WHD_RESULT_CREATE(1055)   /**< Failed to set block ack window */
#define WHD_DELAY_TOO_SHORT              WHD_RESULT_CREATE(1056)   /**< Requested delay is too short */
#define WHD_INVALID_INTERFACE            WHD_RESULT_CREATE(1057)   /**< Invalid interface provided */
#define WHD_WEP_KEYLEN_BAD               WHD_RESULT_CREATE(1058)   /**< WEP / WEP_SHARED key length must be 5 or 13 bytes */
#define WHD_HANDLER_ALREADY_REGISTERED   WHD_RESULT_CREATE(1059)   /**< EAPOL handler already registered */
#define WHD_AP_ALREADY_UP                WHD_RESULT_CREATE(1060)   /**< Soft AP or P2P group owner already up */
#define WHD_EAPOL_KEY_PACKET_M1_TIMEOUT  WHD_RESULT_CREATE(1061)   /**< Timeout occurred while waiting for EAPOL packet M1 from AP */
#define WHD_EAPOL_KEY_PACKET_M3_TIMEOUT  WHD_RESULT_CREATE(1062)   /**< Timeout occurred while waiting for EAPOL packet M3 from APwhich may indicate incorrect WPA2/WPA passphrase */
#define WHD_EAPOL_KEY_PACKET_G1_TIMEOUT  WHD_RESULT_CREATE(1063)   /**< Timeout occurred while waiting for EAPOL packet G1 from AP */
#define WHD_EAPOL_KEY_FAILURE            WHD_RESULT_CREATE(1064)   /**< Unknown failure occurred during the EAPOL key handshake */
#define WHD_MALLOC_FAILURE               WHD_RESULT_CREATE(1065)   /**< Memory allocation failure */
#define WHD_ACCESS_POINT_NOT_FOUND       WHD_RESULT_CREATE(1066)   /**< Access point not found */
#define WHD_RTOS_ERROR                   WHD_RESULT_CREATE(1067)   /**< RTOS operation failed */
#define WHD_CLM_BLOB_DLOAD_ERROR         WHD_RESULT_CREATE(1068)   /**< CLM blob download failed */
#define WHD_HAL_ERROR                    WHD_RESULT_CREATE(1069)   /**< WHD HAL Error */
#define WHD_RTOS_STATIC_MEM_LIMIT        WHD_RESULT_CREATE(1070)   /**< Exceeding the RTOS static objects memory */

#define WLAN_ENUM_OFFSET 2000            /**< WLAN enum offset for WHD_WLAN error processing */

#define WHD_WLAN_ERROR                   WHD_RESULT_CREATE(2001) /**< Generic Error */
#define WHD_WLAN_BADARG                  WHD_RESULT_CREATE(2002) /**< Bad Argument */
#define WHD_WLAN_BADOPTION               WHD_RESULT_CREATE(2003) /**< Bad option */
#define WHD_WLAN_NOTUP                   WHD_RESULT_CREATE(2004) /**< Not up */
#define WHD_WLAN_NOTDOWN                 WHD_RESULT_CREATE(2005) /**< Not down */
#define WHD_WLAN_NOTAP                   WHD_RESULT_CREATE(2006) /**< Not AP */
#define WHD_WLAN_NOTSTA                  WHD_RESULT_CREATE(2007) /**< Not STA  */
#define WHD_WLAN_BADKEYIDX               WHD_RESULT_CREATE(2008) /**< BAD Key Index */
#define WHD_WLAN_RADIOOFF                WHD_RESULT_CREATE(2009) /**< Radio Off */
#define WHD_WLAN_NOTBANDLOCKED           WHD_RESULT_CREATE(2010) /**< Not  band locked */
#define WHD_WLAN_NOCLK                   WHD_RESULT_CREATE(2011) /**< No Clock */
#define WHD_WLAN_BADRATESET              WHD_RESULT_CREATE(2012) /**< BAD Rate valueset */
#define WHD_WLAN_BADBAND                 WHD_RESULT_CREATE(2013) /**< BAD Band */
#define WHD_WLAN_BUFTOOSHORT             WHD_RESULT_CREATE(2014) /**< Buffer too short */
#define WHD_WLAN_BUFTOOLONG              WHD_RESULT_CREATE(2015) /**< Buffer too long */
#define WHD_WLAN_BUSY                    WHD_RESULT_CREATE(2016) /**< Busy */
#define WHD_WLAN_NOTASSOCIATED           WHD_RESULT_CREATE(2017) /**< Not Associated */
#define WHD_WLAN_BADSSIDLEN              WHD_RESULT_CREATE(2018) /**< Bad SSID len */
#define WHD_WLAN_OUTOFRANGECHAN          WHD_RESULT_CREATE(2019) /**< Out of Range Channel */
#define WHD_WLAN_BADCHAN                 WHD_RESULT_CREATE(2020) /**< Bad Channel */
#define WHD_WLAN_BADADDR                 WHD_RESULT_CREATE(2021) /**< Bad Address */
#define WHD_WLAN_NORESOURCE              WHD_RESULT_CREATE(2022) /**< Not Enough Resources */
#define WHD_WLAN_UNSUPPORTED             WHD_RESULT_CREATE(2023) /**< Unsupported */
#define WHD_WLAN_BADLEN                  WHD_RESULT_CREATE(2024) /**< Bad length */
#define WHD_WLAN_NOTREADY                WHD_RESULT_CREATE(2025) /**< Not Ready */
#define WHD_WLAN_EPERM                   WHD_RESULT_CREATE(2026) /**< Not Permitted */
#define WHD_WLAN_NOMEM                   WHD_RESULT_CREATE(2027) /**< No Memory */
#define WHD_WLAN_ASSOCIATED              WHD_RESULT_CREATE(2028) /**< Associated */
#define WHD_WLAN_RANGE                   WHD_RESULT_CREATE(2029) /**< Not In Range */
#define WHD_WLAN_NOTFOUND                WHD_RESULT_CREATE(2030) /**< Not Found */
#define WHD_WLAN_WME_NOT_ENABLED         WHD_RESULT_CREATE(2031) /**< WME Not Enabled */
#define WHD_WLAN_TSPEC_NOTFOUND          WHD_RESULT_CREATE(2032) /**< TSPEC Not Found */
#define WHD_WLAN_ACM_NOTSUPPORTED        WHD_RESULT_CREATE(2033) /**< ACM Not Supported */
#define WHD_WLAN_NOT_WME_ASSOCIATION     WHD_RESULT_CREATE(2034) /**< Not WME Association */
#define WHD_WLAN_SDIO_ERROR              WHD_RESULT_CREATE(2035) /**< SDIO Bus Error */
#define WHD_WLAN_WLAN_DOWN               WHD_RESULT_CREATE(2036) /**< WLAN Not Accessible */
#define WHD_WLAN_BAD_VERSION             WHD_RESULT_CREATE(2037) /**< Incorrect version */
#define WHD_WLAN_TXFAIL                  WHD_RESULT_CREATE(2038) /**< TX failure */
#define WHD_WLAN_RXFAIL                  WHD_RESULT_CREATE(2039) /**< RX failure */
#define WHD_WLAN_NODEVICE                WHD_RESULT_CREATE(2040) /**< Device not present */
#define WHD_WLAN_UNFINISHED              WHD_RESULT_CREATE(2041) /**< To be finished */
#define WHD_WLAN_NONRESIDENT             WHD_RESULT_CREATE(2042) /**< access to nonresident overlay */
#define WHD_WLAN_DISABLED                WHD_RESULT_CREATE(2043) /**< Disabled in this build */
#define WHD_WLAN_NOFUNCTION              WHD_RESULT_CREATE(2044) /**< Function pointer not provided */
#define WHD_WLAN_INVALID                 WHD_RESULT_CREATE(2045) /**< Not valid */
#define WHD_WLAN_NOBAND                  WHD_RESULT_CREATE(2046) /**< No Band */

/**
 * type definition for whd_buffer_t
 */
typedef whd_buffer_t whd_buffer_queue_ptr_t;

#pragma pack(1)

/**
 * Structure for storing AP information
 */
typedef struct whd_ap_info
{
    whd_ssid_t SSID;          /**< Service Set Identification (i.e. Name of Access Point)                    */
    whd_mac_t BSSID;          /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength;  /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    uint32_t max_data_rate;   /**< Maximum data rate in kilobits/s                                           */
    whd_bss_type_t bss_type;  /**< Network type                                                              */
    whd_security_t security;  /**< Security type                                                             */
    uint8_t channel;          /**< Radio channel that the AP beacon was received on                          */
    whd_802_11_band_t band;   /**< Radio band                                                                */
    struct whd_ap_info *next; /**< Pointer to the next scan result                                           */
} whd_ap_info_t;

/**
 * Structure describing a list of associated softAP clients
 */
typedef struct
{
    uint32_t count;          /**< Number of MAC addresses in the list    */
    whd_mac_t mac_list[1];   /**< Variable length array of MAC addresses */
} whd_maclist_t;

#pragma pack()

/**
 * Time value in milliseconds
 */
typedef uint32_t whd_time_t;

/**
 * Structure for storing a WEP key
 */
typedef struct
{
    uint8_t index;    /**< WEP key index [0/1/2/3]                                         */
    uint8_t length;   /**< WEP key length. Either 5 bytes (40-bits) or 13-bytes (104-bits) */
    uint8_t data[32]; /**< WEP key as values NOT characters                                */
} whd_wep_key_t;

/**
 * Structure for Out-of-band interrupt config parameters which can be set by application during whd power up
 */
typedef struct whd_oob_config
{
    cyhal_gpio_t host_oob_pin;   /**< Host-side GPIO pin selection */
    uint8_t dev_gpio_sel;        /**< WiFi device-side GPIO pin selection (must be zero) */
    whd_bool_t is_falling_edge;  /**< Interrupt trigger (polarity) */
    uint8_t intr_priority;       /**< OOB interrupt priority */
} whd_oob_config_t;

/**
 * Structure for SDIO config parameters which can be set by application during whd power up
 */
typedef struct whd_sdio_config
{
    /* Bus config */
    whd_bool_t sdio_1bit_mode;        /**< Default is false, means SDIO operates under 4 bit mode */
    whd_bool_t high_speed_sdio_clock; /**< Default is false, means SDIO operates in normal clock rate */
    whd_oob_config_t oob_config;      /**< Out-of-band interrupt configuration (required when bus can sleep) */
} whd_sdio_config_t;

/**
 * Structure for SPI config parameters which can be set by application during whd power up
 */
typedef struct whd_spi_config
{
    /* Bus config */
    whd_bool_t is_spi_normal_mode; /**< Default is false */
    whd_oob_config_t oob_config;   /**< Out-of-band interrupt configuration */
} whd_spi_config_t;

/**
 * Enumeration of applicable packet mask bits for custom Information Elements (IEs)
 */
typedef enum
{
    VENDOR_IE_BEACON = 0x1,         /**< Denotes beacon packet                  */
    VENDOR_IE_PROBE_RESPONSE = 0x2, /**< Denotes probe response packet          */
    VENDOR_IE_ASSOC_RESPONSE = 0x4, /**< Denotes association response packet    */
    VENDOR_IE_AUTH_RESPONSE = 0x8,  /**< Denotes authentication response packet */
    VENDOR_IE_PROBE_REQUEST = 0x10, /**< Denotes probe request packet           */
    VENDOR_IE_ASSOC_REQUEST = 0x20, /**< Denotes association request packet     */
    VENDOR_IE_CUSTOM = 0x100        /**< Denotes a custom IE(Information Element) identifier */
} whd_ie_packet_flag_t;

/**
 * Structure for LE Scan parameters
 */
typedef struct whd_btc_lescan_params
{
    uint16_t priority;      /**< LE scan priority */
    uint16_t duty_cycle;    /**< LE scan duty cycle */
    uint16_t max_win;       /**< LE Max Scan window */
    uint16_t int_grant;     /**< LE Small Interval Grant */
    uint16_t scan_int;      /**< LE scan interval */
    uint16_t scan_win;      /**< LE scan Window */
} whd_btc_lescan_params_t;

/**
 * Structure for coex config parameters which can be set by application
 */
typedef struct whd_coex_config
{
    whd_btc_lescan_params_t le_scan_params;  /**< LE Scan Parameters */
} whd_coex_config_t;

#define PORT_FILTER_LEN 26  /**< Port filter len */
#define PACKET_FILTER_LIST_BUFFER_MAX_LEN 1000  /**< Packet filter buffer max len */
/**
 * Enumeration of packet filter rules
 */
typedef enum
{
    WHD_PACKET_FILTER_RULE_POSITIVE_MATCHING  = 0, /**< Specifies that a filter should match a given pattern	 */
    WHD_PACKET_FILTER_RULE_NEGATIVE_MATCHING  = 1  /**< Specifies that a filter should NOT match a given pattern */
} whd_packet_filter_rule_t;

/**
 * Structure describing a packet filter list item
 */
typedef struct
{
    uint32_t id;                                  /**< Unique identifier for a packet filter item							  */
    whd_packet_filter_rule_t rule;                /**< Filter matches are either POSITIVE or NEGATIVE matching */
    uint16_t offset;                              /**< Offset in bytes to start filtering (referenced to the start of the ethernet packet) */
    uint16_t mask_size;                           /**< Size of the mask in bytes */
    uint8_t *mask;                                /**< Pattern mask bytes to be ANDed with the pattern eg. "\xff00" (must be in network byte order) */
    uint8_t *pattern;                             /**< Pattern bytes used to filter eg. "\x0800"  (must be in network byte order) */
    whd_bool_t enabled_status;                     /**< When returned from wwd_wifi_get_packet_filters, indicates if the filter is enabled */
} whd_packet_filter_t;

#define TKO_DATA_OFFSET offsetof(wl_tko_t, data)  /**< TKO data offset */

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_TYPES_H_ */

