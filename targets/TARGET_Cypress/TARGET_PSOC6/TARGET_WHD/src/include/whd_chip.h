/*
 * Copyright 2019 Cypress Semiconductor Corporation
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

#ifndef INCLUDED_WHD_CHIP_H
#define INCLUDED_WHD_CHIP_H

#include "cyabs_rtos.h" /* For cy_semaphore_t */

#include "whd_endian.h"
#include "whd.h"
#include "whd_wifi_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/
#define WHD_WLAN_WAKE_TIMEOUT           (10000)
#define WHD_SHARED_MEMORY_POLLING_DELAY (10)
#define SICF_CPUHALT                    (0x0020)

/******************************************************
*                 Enumerations
******************************************************/
/**
 * Enumerated list of aggregate codes and edit WHD_COUNTRY_AGGREGATE_CUSTOMER for supporting new aggregate
 * as per customer like XZ/278
 */
typedef enum
{
    WHD_COUNTRY_AGGREGATE_XA_0 = MK_CNTRY('X', 'A', 0),
    WHD_COUNTRY_AGGREGATE_XT_0 = MK_CNTRY('X', 'T', 0),
    WHD_COUNTRY_AGGREGATE_XV_0 = MK_CNTRY('X', 'V', 0),
    WHD_COUNTRY_AGGREGATE_CUSTOMER = MK_CNTRY('X', 'Z', 278),
} whd_aggregate_code_t;

typedef enum
{
    /* Note : If changing this, core_base_address must be changed also */
    WLAN_ARM_CORE = 0, SOCRAM_CORE = 1, SDIOD_CORE = 2
} device_core_t;

typedef enum
{
    WLAN_DOWN, WLAN_UP, WLAN_OFF
} wlan_state_t;

typedef enum
{
    WLAN_CORE_FLAG_NONE, WLAN_CORE_FLAG_CPU_HALT,
} wlan_core_flag_t;

/**
 * Enumeration of AKM (authentication and key management) suites. Table 8-140 802.11mc D3.0.
 */
typedef enum
{
    WHD_AKM_RESERVED = 0,
    WHD_AKM_8021X = 1, /**< WPA2 enterprise                 */
    WHD_AKM_PSK = 2, /**< WPA2 PSK                        */
    WHD_AKM_FT_8021X = 3, /**< 802.11r Fast Roaming enterprise */
    WHD_AKM_FT_PSK = 4, /**< 802.11r Fast Roaming PSK        */
    WHD_AKM_8021X_SHA256 = 5,
    WHD_AKM_PSK_SHA256 = 6,
    WHD_AKM_TDLS = 7, /**< Tunneled Direct Link Setup      */
    WHD_AKM_SAE_SHA256 = 8,
    WHD_AKM_FT_SAE_SHA256 = 9,
    WHD_AKM_AP_PEER_KEY_SHA256 = 10,
    WHD_AKM_SUITEB_8021X_HMAC_SHA256 = 11,
    WHD_AKM_SUITEB_8021X_HMAC_SHA384 = 12,
    WHD_AKM_SUITEB_FT_8021X_HMAC_SHA384 = 13,
} whd_akm_suite_t;

/**
 * Enumeration of cipher suites. Table 8-138 802.11mc D3.0.
 */
typedef enum
{
    WHD_CIPHER_GROUP = 0, /**< Use group cipher suite                                        */
    WHD_CIPHER_WEP_40 = 1, /**< WEP-40                                                        */
    WHD_CIPHER_TKIP = 2, /**< TKIP                                                          */
    WHD_CIPHER_RESERVED = 3, /**< Reserved                                                      */
    WHD_CIPHER_CCMP_128 = 4, /**< CCMP-128 - default pairwise and group cipher suite in an RSNA */
    WHD_CIPHER_WEP_104 = 5, /**< WEP-104 - also known as WEP-128                               */
    WHD_CIPHER_BIP_CMAC_128 = 6, /**< BIP-CMAC-128 - default management frame cipher suite          */
    WHD_CIPHER_GROUP_DISALLOWED = 7, /**< Group address traffic not allowed                             */
    WHD_CIPHER_GCMP_128 = 8, /**< GCMP-128 - default for 60 GHz STAs                            */
    WHD_CIPHER_GCMP_256 = 9, /**< GCMP-256 - introduced for Suite B                             */
    WHD_CIPHER_CCMP_256 = 10, /**< CCMP-256 - introduced for suite B                             */
    WHD_CIPHER_BIP_GMAC_128 = 11, /**< BIP-GMAC-128 - introduced for suite B                         */
    WHD_CIPHER_BIP_GMAC_256 = 12, /**< BIP-GMAC-256 - introduced for suite B                         */
    WHD_CIPHER_BIP_CMAC_256 = 13, /**< BIP-CMAC-256 - introduced for suite B                         */
} whd_80211_cipher_t;

/******************************************************
*               Structures
******************************************************/

typedef struct whd_chip_info
{
    uint16_t chip_id;
    whd_bool_t save_restore_enable;

} whd_chip_info_t;

typedef struct
{
    wlan_state_t state;
    whd_country_code_t country_code;
    whd_aggregate_code_t aggregate_code;
    uint32_t keep_wlan_awake;
} whd_wlan_status_t;

#pragma pack(1)

/* 802.11 Information Element structures */

/* Country Information */
#define COUNTRY_INFO_IE_MINIMUM_LENGTH (6)

typedef struct
{
    uint8_t first_chan_num;
    uint8_t num_chans;
    uint8_t max_tx_pwr_level;
} country_chan_info_t;

/* Structures for TLVs with 8-bit type and 8-bit length */
typedef struct
{
    uint8_t type;
    uint8_t length;
} whd_tlv8_header_t;

typedef struct
{
    uint8_t type;
    uint8_t length;
    uint8_t data[1];
} whd_tlv8_data_t;

typedef struct
{
    whd_tlv8_header_t tlv_header; /* id, length                                                          */
    char ccode[2]; /* dot11CountryString MIB octet 1~2, two letter country code           */
    uint8_t env; /* dot11CountryString MIB octet 3, indicate indoor/outdoor environment */
    country_chan_info_t country_chan_info[1]; /* At least one country channel info triples                           */
} country_info_ie_fixed_portion_t;

/* Robust Secure Network */
typedef struct
{
    whd_tlv8_header_t tlv_header; /* id, length */
    uint16_t version;
    uint32_t group_key_suite; /* See whd_80211_cipher_t for values */
    uint16_t pairwise_suite_count;
    uint32_t pairwise_suite_list[1];
} rsn_ie_fixed_portion_t;

#define RSN_IE_MINIMUM_LENGTH (8)

typedef struct
{
    whd_tlv8_header_t tlv_header; /* id, length */
    uint8_t oui[4];
} vendor_specific_ie_header_t;

#define VENDOR_SPECIFIC_IE_MINIMUM_LENGTH (4)

/* WPA IE */
typedef struct
{
    vendor_specific_ie_header_t vendor_specific_header;
    uint16_t version;
    uint32_t multicast_suite;
    uint16_t unicast_suite_count;
    uint8_t unicast_suite_list[1][4];
} wpa_ie_fixed_portion_t;

#define WPA_IE_MINIMUM_LENGTH (12)

typedef struct
{
    uint16_t akm_suite_count;
    uint32_t akm_suite_list[1];
} akm_suite_portion_t;

typedef struct
{
    whd_tlv8_header_t tlv_header; /* id, length */
    uint16_t ht_capabilities_info;
    uint8_t ampdu_parameters;
    uint8_t rx_mcs[10];
    uint16_t rxhighest_supported_data_rate;
    uint8_t tx_supported_mcs_set;
    uint8_t tx_mcs_info[3];
    uint16_t ht_extended_capabilities;
    uint32_t transmit_beam_forming_capabilities;
    uint8_t antenna_selection_capabilities;
} ht_capabilities_ie_t;

#define HT_CAPABILITIES_INFO_LDPC_CODING_CAPABILITY        (1 <<  0)
#define HT_CAPABILITIES_INFO_SUPPORTED_CHANNEL_WIDTH_SET   (1 <<  1)
#define HT_CAPABILITIES_INFO_SM_POWER_SAVE_OFFSET          (1 <<  2)
#define HT_CAPABILITIES_INFO_SM_POWER_SAVE_MASK            (3 <<  2)
#define HT_CAPABILITIES_INFO_HT_GREENFIELD                 (1 <<  4)
#define HT_CAPABILITIES_INFO_SHORT_GI_FOR_20MHZ            (1 <<  5)
#define HT_CAPABILITIES_INFO_SHORT_GI_FOR_40MHZ            (1 <<  6)
#define HT_CAPABILITIES_INFO_TX_STBC                       (1 <<  7)
#define HT_CAPABILITIES_INFO_RX_STBC_OFFSET                (1 <<  8)
#define HT_CAPABILITIES_INFO_RX_STBC_MASK                  (3 <<  8)
#define HT_CAPABILITIES_INFO_HT_DELAYED_BLOCK_ACK          (1 << 10)
#define HT_CAPABILITIES_INFO_MAXIMUM_A_MSDU_LENGTH         (1 << 11)
#define HT_CAPABILITIES_INFO_DSSS_CCK_MODE_IN_40MHZ        (1 << 12)
/* bit 13 reserved */
#define HT_CAPABILITIES_INFO_40MHZ_INTOLERANT              (1 << 14)
#define HT_CAPABILITIES_INFO_L_SIG_TXOP_PROTECTION_SUPPORT (1 << 15)

typedef unsigned int uint;
typedef struct
{
    uint buf;
    uint buf_size;
    uint idx;
    uint out_idx; /* output index */
} hnd_log_t;

#define CBUF_LEN 128

typedef struct
{
    /* Virtual UART
     *   When there is no UART (e.g. Quickturn), the host should write a complete
     *   input line directly into cbuf and then write the length into vcons_in.
     *   This may also be used when there is a real UART (at risk of conflicting with
     *   the real UART).  vcons_out is currently unused.
     */
    volatile uint vcons_in;
    volatile uint vcons_out;

    /* Output (logging) buffer
     *   Console output is written to a ring buffer log_buf at index log_idx.
     *   The host may read the output when it sees log_idx advance.
     *   Output will be lost if the output wraps around faster than the host polls.
     */
    hnd_log_t log;

    /* Console input line buffer
     *   Characters are read one at a time into cbuf until <CR> is received, then
     *   the buffer is processed as a command line.  Also used for virtual UART.
     */
    uint cbuf_idx;
    char cbuf[CBUF_LEN];
} hnd_cons_t;

typedef struct wifi_console
{
    uint count; /* Poll interval msec counter */
    uint log_addr; /* Log struct address (fixed) */
    hnd_log_t log; /* Log struct (host copy) */
    uint bufsize; /* Size of log buffer */
    char *buf; /* Log buffer (host copy) */
    uint last; /* Last buffer read index */
} wifi_console_t;

typedef struct
{
    uint flags;
    uint trap_addr;
    uint assert_exp_addr;
    uint assert_file_addr;
    uint assert_line;
    uint console_addr;
    uint msgtrace_addr;
    uint fwid;
} wlan_shared_t;

/* Buffer size to be allocated to read wlan log */
#define WLAN_LOG_BUF_LEN (4 * 1024)

#define WHD_IOCTL_LOG_SIZE 64
#define WHD_IOVAR_STRING_SIZE 128
#define WHD_MAX_DATA_SIZE 64

#define WHD_IOCTL_LOG_ADD(x, y, z) whd_ioctl_log_add(x, y, z)
#define WHD_IOCTL_LOG_ADD_EVENT(w, x, y, z) whd_ioctl_log_add_event(w, x, y, z)
#define WHD_IOCTL_PRINT(x) whd_ioctl_print(x)

typedef struct
{
    uint32_t ioct_log;
    uint8_t is_this_event;
    uint8_t data[WHD_MAX_DATA_SIZE];
    uint32_t data_size;
    uint16_t flag;
    uint32_t reason;
}whd_ioctl_log_t;

void whd_ioctl_log_add(whd_driver_t whd_driver, uint32_t cmd, whd_buffer_t buffer);
void whd_ioctl_log_add_event(whd_driver_t whd_driver, uint32_t cmd, uint16_t flag, uint32_t data);

whd_result_t whd_ioctl_print(whd_driver_t whd_driver);

#pragma pack()

typedef struct whd_internal_info
{
    whd_wlan_status_t whd_wlan_status;
    wifi_console_t *c;
    wifi_console_t console;
    wlan_shared_t sh;
    uint32_t console_addr;
    whd_scan_result_callback_t scan_result_callback;
    whd_scan_result_t *whd_scan_result_ptr;
    /* The semaphore used to wait for completion of a join;
     * whd_wifi_join_halt uses this to release waiting threads (if any) */
    cy_semaphore_t *active_join_semaphore;
    whd_bool_t active_join_mutex_initted;
    cy_semaphore_t active_join_mutex;
    uint con_lastpos;
    whd_bool_t whd_wifi_p2p_go_is_up;
    uint32_t whd_join_status[3];

} whd_internal_info_t;

#pragma pack(1)

typedef struct
{
    char abbrev[3];
    uint8_t rev;
    uint8_t data[64];
} whd_country_info_t;

#pragma pack()

void whd_internal_info_init(whd_driver_t whd_driver);

/******************************************************
*               Function Declarations
******************************************************/

extern void whd_wifi_chip_info_init(whd_driver_t whd_driver);
extern whd_result_t whd_wlan_bus_complete_ds_wake(whd_driver_t whd_driver, whd_bool_t wake_from_firmware,
                                                  uint32_t wake_event_indication_addr, uint32_t wake_indication_addr,
                                                  uint32_t sdio_control_addr);
extern whd_result_t whd_wifi_set_custom_country_code(whd_interface_t ifp, const whd_country_info_t *country_code);

/* Device core control functions */
extern whd_result_t whd_disable_device_core(whd_driver_t whd_driver, device_core_t core_id, wlan_core_flag_t core_flag);
extern whd_result_t whd_reset_device_core(whd_driver_t whd_driver, device_core_t core_id, wlan_core_flag_t core_flag);
extern whd_result_t whd_reset_core(whd_driver_t whd_driver, device_core_t core_id, uint32_t bits, uint32_t resetbits);
extern whd_result_t whd_wlan_armcore_run(whd_driver_t whd_driver, device_core_t core_id, wlan_core_flag_t core_flag);
extern whd_result_t whd_device_core_is_up(whd_driver_t whd_driver, device_core_t core_id);

/* Chip specific functions */
extern whd_result_t whd_allow_wlan_bus_to_sleep(whd_driver_t whd_driver);
extern whd_result_t whd_ensure_wlan_bus_is_up(whd_driver_t whd_driver);

extern whd_result_t whd_chip_specific_init(whd_driver_t whd_driver);
extern whd_result_t whd_chip_specific_socsram_init(whd_driver_t whd_driver);
extern whd_result_t whd_wifi_read_wlan_log(whd_driver_t whd_driver, char *buffer, uint32_t buffer_size);
extern whd_result_t whd_wifi_print_whd_log(whd_driver_t whd_driver);
extern whd_result_t whd_wifi_read_wlan_log_unsafe(whd_driver_t whd_driver, uint32_t wlan_shared_address, char *buffer,
                                                  uint32_t buffer_size);

extern void whd_wifi_peek(whd_driver_t whd_driver, uint32_t address, uint8_t register_length, uint8_t *value);
extern void whd_wifi_poke(whd_driver_t whd_driver, uint32_t address, uint8_t register_length, uint32_t value);
extern uint32_t whd_wifi_get_btc_params(whd_driver_t whd_driver, uint32_t address, whd_interface_t interface);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_CHIP_H */

