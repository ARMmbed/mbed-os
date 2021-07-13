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
/*
 * Custom OID/ioctl definitions for
 * 802.11abg Networking Device Driver
 */
#ifndef INCLUDED_WHD_WLIOCTL_H
#define INCLUDED_WHD_WLIOCTL_H

#ifdef __cplusplus
extern "C"
{
#endif
/* These are the flags in the BSS Capability Information field as defined in section 7.3.1.4 of IEEE Std 802.11-2007  */
#define DOT11_CAP_ESS                     (0x0001)     /** Extended service set capability */
#define DOT11_CAP_IBSS                    (0x0002)     /** Ad-hoc capability (Independent Basic Service Set) */
#define DOT11_CAP_PRIVACY                 (0x0010)     /** Privacy subfield - indicates data confidentiality is required for all data frames exchanged */

#define CH_MAX_2G_CHANNEL       (14)      /* Max channel in 2G band */
#define MAX_WFDS_SVC_NAME_LEN   (200)     /* maximum service_name length */

#define ACTION_FRAME_SIZE 1040
typedef uint16_t chanspec_t;
#define    ETHER_ADDR_LEN        6

typedef struct ether_addr
{
    uint8_t octet[ETHER_ADDR_LEN];
} wl_ether_addr_t;
struct wl_ether_header
{
    uint8_t ether_dhost[ETHER_ADDR_LEN];
    uint8_t ether_shost[ETHER_ADDR_LEN];
    uint16_t ether_type;
};
typedef struct wl_action_frame
{
    wl_ether_addr_t da;
    uint16_t len;
    uint32_t packetId;
    uint8_t data[ACTION_FRAME_SIZE];
} wl_action_frame_t;

typedef struct wl_af_params
{
    uint32_t channel;
    int32_t dwell_time;
    struct ether_addr BSSID;
    wl_action_frame_t action_frame;
} wl_af_params_t;

#define WL_WIFI_AF_PARAMS_SIZE sizeof(struct wl_af_params)

/* ether types */
#define ETHER_TYPE_LEN      2
#define ETHER_TYPE_MIN      0x0600      /* Anything less than MIN is a length */
#define ETHER_TYPE_IP       0x0800      /* IP */
#define ETHER_TYPE_ARP      0x0806      /* ARP */
#define ETHER_TYPE_8021Q    0x8100      /* 802.1Q */
#define ETHER_TYPE_802_1X   0x888e      /* 802.1x */
#define ETHER_TYPE_WAI      0x88b4      /* WAPI WAI */
#define ETHER_TYPE_802_1X_PREAUTH 0x88c7    /* 802.1x preauthentication */
#define WL_WIFI_ACTION_FRAME_SIZE sizeof(struct wl_action_frame)
#define BWL_DEFAULT_PACKING

#define RWL_ACTION_WIFI_CATEGORY    127
#define RWL_WIFI_OUI_BYTE1        0x90
#define RWL_WIFI_OUI_BYTE2        0x4C
#define RWL_WIFI_OUI_BYTE3        0x0F
#define RWL_WIFI_ACTION_FRAME_SIZE    sizeof(struct dot11_action_wifi_vendor_specific)
#define RWL_WIFI_DEFAULT                0x00
#define RWL_WIFI_FIND_MY_PEER        0x09
#define RWL_WIFI_FOUND_PEER        0x0A
#define RWL_ACTION_WIFI_FRAG_TYPE    0x55

typedef struct ssid_info
{
    uint8_t ssid_len;
    uint8_t ssid[32];
} ssid_info_t;
typedef struct cnt_rx
{
    uint32_t cnt_rxundec;
    uint32_t cnt_rxframe;
} cnt_rx_t;
#define RWL_REF_MAC_ADDRESS_OFFSET    17
#define RWL_DUT_MAC_ADDRESS_OFFSET    23
#define RWL_WIFI_CLIENT_CHANNEL_OFFSET    50
#define RWL_WIFI_SERVER_CHANNEL_OFFSET    51
#define WL_BSS_INFO_VERSION    109
#define MCSSET_LEN    16

typedef struct wlc_ssid
{
    uint32_t SSID_len;
    uint8_t SSID[32];
} wlc_ssid_t;
#define WL_BSSTYPE_INFRA 1
#define WL_BSSTYPE_INDEP 0
#define WL_BSSTYPE_ANY   2
#define WL_SCANFLAGS_PASSIVE 0x01
#define WL_SCANFLAGS_PROHIBITED    0x04
typedef struct wl_scan_params
{
    wlc_ssid_t ssid;
    wl_ether_addr_t bssid;
    int8_t bss_type;
    int8_t scan_type;
    int32_t nprobes;
    int32_t active_time;
    int32_t passive_time;
    int32_t home_time;
    int32_t channel_num;
    uint16_t channel_list[1];
} wl_scan_params_t;
#define WL_SCAN_PARAMS_FIXED_SIZE          (64)
#define WL_SCAN_PARAMS_COUNT_MASK  (0x0000ffff)
#define WL_SCAN_PARAMS_NSSID_SHIFT         (16)
#define WL_SCAN_ACTION_START                (1)
#define WL_SCAN_ACTION_CONTINUE             (2)
#define WL_SCAN_ACTION_ABORT                (3)
#define ISCAN_REQ_VERSION                   (1)
typedef struct wl_iscan_params
{
    uint32_t version;
    uint16_t action;
    uint16_t scan_duration;
    wl_scan_params_t params;
} wl_iscan_params_t;
#define WL_ISCAN_PARAMS_FIXED_SIZE (offsetof(wl_iscan_params_t, params) + sizeof(wlc_ssid_t) )
typedef struct wl_scan_results
{
    uint32_t buflen;
    uint32_t version;
    uint32_t count;
    wl_bss_info_t bss_info[1];
} wl_scan_results_t;
#define WL_SCAN_RESULTS_FIXED_SIZE  (12)
#define WL_SCAN_RESULTS_SUCCESS         (0)
#define WL_SCAN_RESULTS_PARTIAL         (1)
#define WL_SCAN_RESULTS_PENDING         (2)
#define WL_SCAN_RESULTS_ABORTED         (3)
#define WL_SCAN_RESULTS_NO_MEM         (4)
#define ESCAN_REQ_VERSION 1
typedef struct wl_escan_params
{
    uint32_t version;
    uint16_t action;
    uint16_t sync_id;
    wl_scan_params_t params;
} wl_escan_params_t;
#define WL_ESCAN_PARAMS_FIXED_SIZE (offsetof(wl_escan_params_t, params) + sizeof(wlc_ssid_t) )
typedef struct wl_escan_result
{
    uint32_t buflen;
    uint32_t version;
    uint16_t sync_id;
    uint16_t bss_count;
    wl_bss_info_t bss_info[1];
} wl_escan_result_t;
#define WL_ESCAN_RESULTS_FIXED_SIZE (sizeof(wl_escan_result_t) - sizeof(wl_bss_info_t) )
typedef struct wl_iscan_results
{
    uint32_t status;
    wl_scan_results_t results;
} wl_iscan_results_t;
#define WL_ISCAN_RESULTS_FIXED_SIZE \
    (WL_SCAN_RESULTS_FIXED_SIZE + offsetof(wl_iscan_results_t, results) )
#define WL_MAXRATES_IN_SET      16  /* max # of rates in a rateset */
typedef struct wl_rateset
{
    uint32_t count;     /* # rates in this set */
    uint8_t rates[WL_MAXRATES_IN_SET];     /* rates in 500kbps units w/hi bit set if basic */
} wl_rateset_t;

typedef struct wl_rateset_args
{
    uint32_t count;     /* # rates in this set */
    uint8_t rates[WL_MAXRATES_IN_SET];     /* rates in 500kbps units w/hi bit set if basic */
    uint8_t mcs[WL_MAXRATES_IN_SET];     /* supported mcs index bit map */
} wl_rateset_args_t;

#define WL_RSPEC_RATE_MASK      0x000000FF      /* rate or HT MCS value */
#define WL_RSPEC_VHT_MCS_MASK   0x0000000F      /* VHT MCS value */
#define WL_RSPEC_VHT_NSS_MASK   0x000000F0      /* VHT Nss value */
#define WL_RSPEC_VHT_NSS_SHIFT  4               /* VHT Nss value shift */
#define WL_RSPEC_TXEXP_MASK     0x00000300
#define WL_RSPEC_TXEXP_SHIFT    8
#define WL_RSPEC_BW_MASK        0x00070000      /* bandwidth mask */
#define WL_RSPEC_BW_SHIFT       16              /* bandwidth shift */
#define WL_RSPEC_STBC           0x00100000      /* STBC encoding, Nsts = 2 x Nss */
#define WL_RSPEC_TXBF           0x00200000      /* bit indicates TXBF mode */
#define WL_RSPEC_LDPC           0x00400000      /* bit indicates adv coding in use */
#define WL_RSPEC_SGI            0x00800000      /* Short GI mode */
#define WL_RSPEC_ENCODING_MASK  0x03000000      /* Encoding of Rate/MCS field */
#define WL_RSPEC_OVERRIDE_RATE  0x40000000      /* bit indicate to override mcs only */
#define WL_RSPEC_OVERRIDE_MODE  0x80000000      /* bit indicates override both rate & mode */
#define WL_RSPEC_BW_UNSPECIFIED 0
#define WL_RSPEC_BW_20MHZ       0x00010000
#define WL_RSPEC_BW_40MHZ       0x00020000
#define WL_RSPEC_BW_80MHZ       0x00030000
#define WL_RSPEC_BW_160MHZ      0x00040000
#define WL_RSPEC_BW_10MHZ       0x00050000
#define WL_RSPEC_BW_5MHZ        0x00060000
#define WL_RSPEC_BW_2P5MHZ      0x00070000
#define WL_RSPEC_ENCODE_RATE    0x00000000      /* Legacy rate is stored in RSPEC_RATE_MASK */
#define WL_RSPEC_ENCODE_HT      0x01000000      /* HT MCS is stored in RSPEC_RATE_MASK */
#define WL_RSPEC_ENCODE_VHT     0x02000000      /* VHT MCS and Nss is stored in RSPEC_RATE_MASK */

typedef struct wl_uint32_list
{
    uint32_t count;
    uint32_t element[1];
} wl_uint32_list_t;

typedef struct wl_join_scan_params
{
    uint8_t scan_type;     /* 0 use default, active or passive scan */
    int32_t nprobes;     /* -1 use default, number of probes per channel */
    int32_t active_time;     /* -1 use default, dwell time per channel for
                              * active scanning
                              */
    int32_t passive_time;     /* -1 use default, dwell time per channel
                               * for passive scanning
                               */
    int32_t home_time;     /* -1 use default, dwell time for the home channel
                            * between channel scans
                            */
} wl_join_scan_params_t;

#define NRATE_MCS_INUSE            (0x00000080)
#define NRATE_RATE_MASK         (0x0000007f)
#define NRATE_STF_MASK            (0x0000ff00)
#define NRATE_STF_SHIFT                     (8)
#define NRATE_OVERRIDE            (0x80000000)
#define NRATE_OVERRIDE_MCS_ONLY   (0x40000000)
#define NRATE_SGI_MASK          (0x00800000)
#define NRATE_SGI_SHIFT                 (23)
#define NRATE_LDPC_CODING       (0x00400000)
#define NRATE_LDPC_SHIFT                (22)
#define NRATE_BCMC_OVERRIDE     (0x00200000)
#define NRATE_BCMC_SHIFT                (21)
#define NRATE_STF_SISO                     (0)
#define NRATE_STF_CDD                     (1)
#define NRATE_STF_STBC                     (2)
#define NRATE_STF_SDM                     (3)
#define ANTENNA_NUM_1                     (1)
#define ANTENNA_NUM_2                     (2)
#define ANTENNA_NUM_3                     (3)
#define ANTENNA_NUM_4                     (4)
#define ANT_SELCFG_AUTO                  (0x80)
#define ANT_SELCFG_MASK                  (0x33)
#define ANT_SELCFG_MAX                     (4)
#define ANT_SELCFG_TX_UNICAST             (0)
#define ANT_SELCFG_RX_UNICAST             (1)
#define ANT_SELCFG_TX_DEF                 (2)
#define ANT_SELCFG_RX_DEF                 (3)
typedef struct
{
    uint8_t ant_config[ANT_SELCFG_MAX];
    uint8_t num_antcfg;
} wlc_antselcfg_t;
#define HIGHEST_SINGLE_STREAM_MCS    (7)
#define WLC_CNTRY_BUF_SZ    (4)
typedef struct wl_country
{
    char country_abbrev[WLC_CNTRY_BUF_SZ];
    int32_t rev;
    char ccode[WLC_CNTRY_BUF_SZ];
} wl_country_t;
typedef struct wl_channels_in_country
{
    uint32_t buflen;
    uint32_t band;
    int8_t country_abbrev[WLC_CNTRY_BUF_SZ];
    uint32_t count;
    uint32_t channel[1];
} wl_channels_in_country_t;
typedef struct wl_country_list
{
    uint32_t buflen;
    uint32_t band_set;
    uint32_t band;
    uint32_t count;
    int8_t country_abbrev[1];
} wl_country_list_t;
#define WL_NUM_RPI_BINS        8
#define WL_RM_TYPE_BASIC    1
#define WL_RM_TYPE_CCA        2
#define WL_RM_TYPE_RPI        3
#define WL_RM_FLAG_PARALLEL    (1 << 0)
#define WL_RM_FLAG_LATE        (1 << 1)
#define WL_RM_FLAG_INCAPABLE    (1 << 2)
#define WL_RM_FLAG_REFUSED    (1 << 3)
typedef struct wl_rm_req_elt
{
    int8_t type;
    int8_t flags;
    wl_chanspec_t chanspec;
    uint32_t token;
    uint32_t tsf_h;
    uint32_t tsf_l;
    uint32_t dur;
} wl_rm_req_elt_t;
typedef struct wl_rm_req
{
    uint32_t token;
    uint32_t count;
    void *cb;
    void *cb_arg;
    wl_rm_req_elt_t req[1];
} wl_rm_req_t;
#define WL_RM_REQ_FIXED_LEN    offsetof(wl_rm_req_t, req)
typedef struct wl_rm_rep_elt
{
    int8_t type;
    int8_t flags;
    wl_chanspec_t chanspec;
    uint32_t token;
    uint32_t tsf_h;
    uint32_t tsf_l;
    uint32_t dur;
    uint32_t len;
    uint8_t data[1];
} wl_rm_rep_elt_t;
#define WL_RM_REP_ELT_FIXED_LEN    24
#define WL_RPI_REP_BIN_NUM 8
typedef struct wl_rm_rpi_rep
{
    uint8_t rpi[WL_RPI_REP_BIN_NUM];
    int8_t rpi_max[WL_RPI_REP_BIN_NUM];
} wl_rm_rpi_rep_t;
typedef struct wl_rm_rep
{
    uint32_t token;
    uint32_t len;
    wl_rm_rep_elt_t rep[1];
} wl_rm_rep_t;
#define WL_RM_REP_FIXED_LEN    8
#define    CRYPTO_ALGO_OFF            0
#define    CRYPTO_ALGO_WEP1        1
#define    CRYPTO_ALGO_TKIP        2
#define    CRYPTO_ALGO_WEP128        3
#define CRYPTO_ALGO_AES_CCM        4
#define CRYPTO_ALGO_AES_OCB_MSDU    5
#define CRYPTO_ALGO_AES_OCB_MPDU    6
#define CRYPTO_ALGO_NALG        7
#define WSEC_GEN_MIC_ERROR    0x0001
#define WSEC_GEN_REPLAY        0x0002
#define WSEC_GEN_ICV_ERROR    0x0004
#define WL_SOFT_KEY    (1 << 0)
#define WL_PRIMARY_KEY    (1 << 1)
#define WL_KF_RES_4    (1 << 4)
#define WL_KF_RES_5    (1 << 5)
#define WL_IBSS_PEER_GROUP_KEY    (1 << 6)
#define DOT11_MAX_KEY_SIZE    32
typedef struct wl_wsec_key
{
    uint32_t index;
    uint32_t len;
    uint8_t data[DOT11_MAX_KEY_SIZE];
    uint32_t pad_1[18];
    uint32_t algo;
    uint32_t flags;
    uint32_t pad_2[2];
    int32_t pad_3;
    int32_t iv_initialized;
    int32_t pad_4;
    struct
    {
        uint32_t hi;
        uint16_t lo;
    } rxiv;
    uint32_t pad_5[2];
    wl_ether_addr_t ea;
} wl_wsec_key_t;
#define WSEC_MIN_PSK_LEN    8
#define WSEC_MAX_PSK_LEN    64
#define WSEC_PASSPHRASE        (1 << 0)
typedef struct
{
    uint16_t key_len;
    uint16_t flags;
    uint8_t key[WSEC_MAX_PSK_LEN];
} wsec_pmk_t;

#define WSEC_MAX_SAE_PASSWORD_LEN      128
typedef struct
{
    uint16_t password_len;     /* octets in key materials  */
    uint8_t password[WSEC_MAX_SAE_PASSWORD_LEN];     /* maximum key len for SAE passphrase */
} wsec_sae_password_t;

#define OPEN_AUTH                   0x0000
#define SHARED_AUTH                 0x0001
//#define WEP_ENABLED                 0x0001  // moved to whd_types.h
//#define TKIP_ENABLED                0x0002
//#define AES_ENABLED                 0x0004

typedef enum
{
    AUTH_ALGO_80211_OPEN = 1, AUTH_ALGO_80211_SHARED_KEY = 2, AUTH_ALGO_WPA = 3, AUTH_ALGO_WPA_PSK = 4,
    AUTH_ALGO_WPA_NONE = 5, AUTH_ALGO_RSNA = 6, AUTH_ALGO_RSNA_PSK = 7,
} AUTH_ALGORITHM;

#define WSEC_SWFLAG                 0x0008
#define CKIP_KP_ENABLED             0x0010
#define CKIP_MIC_ENABLED            0x0020
#define SES_OW_ENABLED              0x0040
#define FIPS_ENABLED                0x0080
#define SMS4_ENABLED                0x0100

#define MFP_NONE                    0x0000
#define MFP_CAPABLE                 0x0200
#define MFP_REQUIRED                0x0400
#define MFP_SHA256                  0x0800 /* a special configuration for STA for WIFI test tool */

#define WPA_AUTH_DISABLED           0x0000
#define WPA_AUTH_NONE               0x0001
#define WPA_AUTH_UNSPECIFIED        0x0002
#define WPA_AUTH_PSK                0x0004
#define WPA_AUTH_CCKM               0x0008
#define WPA2_AUTH_CCKM              0x0010
#define WPA2_AUTH_UNSPECIFIED       0x0040
#define WPA2_AUTH_PSK               0x0080
#define BRCM_AUTH_PSK               0x0100
#define BRCM_AUTH_DPT               0x0200
#define WPA_AUTH_WAPI               0x0400
#define WPA2_AUTH_MFP           0x1000  /* MFP (11w) in contrast to CCX */

#define WPA2_AUTH_1X_SHA256     0x1000  /* 1X with SHA256 key derivation */
#define WPA2_AUTH_TPK           0x2000  /* TDLS Peer Key */
#define WPA2_AUTH_FT            0x4000  /* Fast Transition. */
#define WPA2_AUTH_PSK_SHA256    0x8000  /* PSK with SHA256 key derivation */
#define WPA2_AUTH_FILS_SHA256   0x10000 /* FILS with SHA256 key derivation */
#define WPA2_AUTH_FILS_SHA384   0x20000 /* FILS with SHA384 key derivation */
#define WPA2_AUTH_IS_FILS(auth) ( (auth) & (WPA2_AUTH_FILS_SHA256 | WPA2_AUTH_FILS_SHA384) )
#define WPA3_AUTH_SAE_PSK       0x40000 /* SAE authentication with SHA-256 */
#define WPA3_AUTH_SAE_FBT       0x80000 /* FT authentication over SAE */
#define WPA3_AUTH_OWE           0x100000  /* OWE */
#define WPA_AUTH_PFN_ANY        0xffffffff  /* for PFN, match only ssid */

#define    MAXPMKID                 16
#define WPA2_PMKID_LEN              16
typedef struct _pmkid
{
    wl_ether_addr_t BSSID;
    uint8_t PMKID[WPA2_PMKID_LEN];
} pmkid_t;
typedef struct _pmkid_list
{
    uint32_t npmkid;
    pmkid_t pmkid[1];
} pmkid_list_t;
typedef struct _pmkid_cand
{
    wl_ether_addr_t BSSID;
    uint8_t preauth;
} pmkid_cand_t;
typedef struct _pmkid_cand_list
{
    uint32_t npmkid_cand;
    pmkid_cand_t pmkid_cand[1];
} pmkid_cand_list_t;
typedef struct wl_led_info
{
    uint32_t index;
    uint32_t behavior;
    uint8_t activehi;
} wl_led_info_t;
struct wl_dot11_assoc_req
{
    uint16_t capability;
    uint16_t listen;
};
struct wl_dot11_assoc_resp
{
    uint16_t capability;
    uint16_t status;
    uint16_t aid;
};
typedef struct wl_assoc_info
{
    uint32_t req_len;
    uint32_t resp_len;
    uint32_t flags;
    struct wl_dot11_assoc_req req;
    wl_ether_addr_t reassoc_bssid;
    struct wl_dot11_assoc_resp resp;
} wl_assoc_info_t;
#define WLC_ASSOC_REQ_IS_REASSOC 0x01
typedef struct
{
    uint32_t byteoff;
    uint32_t nbytes;
    uint16_t buf[1];
} srom_rw_t;
typedef struct
{
    uint32_t source;
    uint32_t byteoff;
    uint32_t nbytes;
} cis_rw_t;
#define WLC_CIS_DEFAULT    0
#define WLC_CIS_SROM    1
#define WLC_CIS_OTP    2
typedef struct
{
    uint32_t byteoff;
    uint32_t val;
    uint32_t size;
    uint32_t band;
} rw_reg_t;
#define WL_ATTEN_APP_INPUT_PCL_OFF    0
#define WL_ATTEN_PCL_ON            1
#define WL_ATTEN_PCL_OFF        2
typedef struct
{
    uint16_t auto_ctrl;
    uint16_t bb;
    uint16_t radio;
    uint16_t txctl1;
} atten_t;
struct wme_tx_params_s
{
    uint8_t short_retry;
    uint8_t short_fallback;
    uint8_t long_retry;
    uint8_t long_fallback;
    uint16_t max_rate;
};
typedef struct wme_tx_params_s wme_tx_params_t;
#define WL_WME_TX_PARAMS_IO_BYTES (sizeof(wme_tx_params_t) * AC_COUNT)
#define WL_PWRIDX_PCL_OFF    -2
#define WL_PWRIDX_PCL_ON    -1
#define WL_PWRIDX_LOWER_LIMIT    -2
#define WL_PWRIDX_UPPER_LIMIT    63
typedef struct
{
    uint32_t val;
    wl_ether_addr_t ea;
} scb_val_t;
#define BCM_MAC_STATUS_INDICATION    (0x40010200L)
typedef struct
{
    uint16_t ver;
    uint16_t len;
    uint16_t cap;
    uint32_t flags;
    uint32_t idle;
    wl_ether_addr_t ea;
    wl_rateset_t rateset;
    uint32_t in;
    uint32_t listen_interval_inms;
    uint32_t tx_pkts;
    uint32_t tx_failures;
    uint32_t rx_ucast_pkts;
    uint32_t rx_mcast_pkts;
    uint32_t tx_rate;
    uint32_t rx_rate;
} sta_info_t;
#define WL_OLD_STAINFO_SIZE    offsetof(sta_info_t, tx_pkts)
#define WL_STA_VER        2
#define WL_STA_BRCM        0x1
#define WL_STA_WME        0x2
#define WL_STA_ABCAP        0x4
#define WL_STA_AUTHE        0x8
#define WL_STA_ASSOC        0x10
#define WL_STA_AUTHO        0x20
#define WL_STA_WDS        0x40
#define WL_STA_WDS_LINKUP    0x80
#define WL_STA_PS        0x100
#define WL_STA_APSD_BE        0x200
#define WL_STA_APSD_BK        0x400
#define WL_STA_APSD_VI        0x800
#define WL_STA_APSD_VO        0x1000
#define WL_STA_N_CAP        0x2000
#define WL_STA_SCBSTATS        0x4000
#define WL_WDS_LINKUP        WL_STA_WDS_LINKUP
typedef struct channel_info
{
    int32_t hw_channel;
    int32_t target_channel;
    int32_t scan_channel;
} channel_info_t;
struct mac_list
{
    uint32_t count;
    wl_ether_addr_t ea[1];
};
typedef struct get_pktcnt
{
    uint32_t rx_good_pkt;
    uint32_t rx_bad_pkt;
    uint32_t tx_good_pkt;
    uint32_t tx_bad_pkt;
    uint32_t rx_ocast_good_pkt;
} get_pktcnt_t;
typedef struct wl_ioctl
{
    uint32_t cmd;
    void *buf;
    uint32_t len;
    uint8_t set;
    uint32_t used;
    uint32_t needed;
} wl_ioctl_t;
typedef struct wlc_rev_info
{
    uint32_t vendorid;
    uint32_t deviceid;
    uint32_t radiorev;
    uint32_t chiprev;
    uint32_t corerev;
    uint32_t boardid;
    uint32_t boardvendor;
    uint32_t boardrev;
    uint32_t driverrev;
    uint32_t ucoderev;
    uint32_t bus;
    uint32_t chipnum;
    uint32_t phytype;
    uint32_t phyrev;
    uint32_t anarev;
} wlc_rev_info_t;
#define WL_REV_INFO_LEGACY_LENGTH    48
#define WL_BRAND_MAX 10
typedef struct wl_instance_info
{
    uint32_t instance;
    int8_t brand[WL_BRAND_MAX];
} wl_instance_info_t;
typedef struct wl_txfifo_sz
{
    uint8_t fifo;
    uint8_t size;
} wl_txfifo_sz_t;
#define WLC_IOV_NAME_LEN 30
typedef struct wlc_iov_trx_s
{
    uint8_t module;
    uint8_t type;
    int8_t name[WLC_IOV_NAME_LEN];
} wlc_iov_trx_t;

/* Event mask ext support */
typedef enum event_msgs_ext_command
{
    EVENTMSGS_NONE = 0, EVENTMSGS_SET_BIT = 1, EVENTMSGS_RESET_BIT = 2, EVENTMSGS_SET_MASK = 3
} event_msgs_ext_command_t;

#define EVENTMSGS_VER 1

/* len- for SET it would be mask size from the application to the firmware */
/*      for GET it would be actual firmware mask size */
/* maxgetsize - is only used for GET. indicate max mask size that the */
/*              application can read from the firmware */
typedef struct eventmsgs_ext
{
    uint8_t ver;
    uint8_t command;
    uint8_t len;
    uint8_t maxgetsize;
    uint8_t mask[1];
} eventmsgs_ext_t;

#define IOVAR_STR_ACTFRAME               "actframe"
#define IOVAR_STR_BSS                    "bss"
#define IOVAR_STR_BSS_RATESET            "bss_rateset"
#define IOVAR_STR_CSA                    "csa"
#define IOVAR_STR_AMPDU_TID              "ampdu_tid"
#define IOVAR_STR_APSTA                  "apsta"
#define IOVAR_STR_ALLMULTI               "allmulti"
#define IOVAR_STR_COUNTRY                "country"
#define IOVAR_STR_EVENT_MSGS             "event_msgs"
#define IOVAR_STR_EVENT_MSGS_EXT         "event_msgs_ext"
#define IOVAR_STR_ESCAN                  "escan"
#define IOVAR_STR_SUP_WPA                "sup_wpa"
#define IOVAR_STR_CUR_ETHERADDR          "cur_etheraddr"
#define IOVAR_STR_QTXPOWER               "qtxpower"
#define IOVAR_STR_MCAST_LIST             "mcast_list"
#define IOVAR_STR_PM2_SLEEP_RET          "pm2_sleep_ret"
#define IOVAR_STR_PM_LIMIT               "pm_limit"
#define IOVAR_STR_LISTEN_INTERVAL_BEACON "bcn_li_bcn"
#define IOVAR_STR_LISTEN_INTERVAL_DTIM   "bcn_li_dtim"
#define IOVAR_STR_LISTEN_INTERVAL_ASSOC  "assoc_listen"
#define IOVAR_PSPOLL_PERIOD              "pspoll_prd"
#define IOVAR_STR_VENDOR_IE              "vndr_ie"
#define IOVAR_STR_TX_GLOM                "bus:txglom"
#define IOVAR_STR_ACTION_FRAME           "actframe"
#define IOVAR_STR_AC_PARAMS_STA          "wme_ac_sta"
#define IOVAR_STR_COUNTERS               "counters"
#define IOVAR_STR_PKT_FILTER_ADD         "pkt_filter_add"
#define IOVAR_STR_PKT_FILTER_DELETE      "pkt_filter_delete"
#define IOVAR_STR_PKT_FILTER_ENABLE      "pkt_filter_enable"
#define IOVAR_STR_PKT_FILTER_MODE        "pkt_filter_mode"
#define IOVAR_STR_PKT_FILTER_LIST        "pkt_filter_list"
#define IOVAR_STR_PKT_FILTER_STATS       "pkt_filter_stats"
#define IOVAR_STR_PKT_FILTER_CLEAR_STATS "pkt_filter_clear_stats"
#define IOVAR_STR_DUTY_CYCLE_CCK         "dutycycle_cck"
#define IOVAR_STR_DUTY_CYCLE_OFDM        "dutycycle_ofdm"
#define IOVAR_STR_MKEEP_ALIVE            "mkeep_alive"
#define IOVAR_STR_VERSION                "ver"
#define IOVAR_STR_SUP_WPA2_EAPVER        "sup_wpa2_eapver"
#define IOVAR_STR_ROAM_OFF               "roam_off"
#define IOVAR_STR_CLOSEDNET              "closednet"
#define IOVAR_STR_P2P_DISC               "p2p_disc"
#define IOVAR_STR_P2P_DEV                "p2p_dev"
#define IOVAR_STR_P2P_IF                 "p2p_if"
#define IOVAR_STR_P2P_IFADD              "p2p_ifadd"
#define IOVAR_STR_P2P_IFDEL              "p2p_ifdel"
#define IOVAR_STR_P2P_IFUPD              "p2p_ifupd"
#define IOVAR_STR_P2P_SCAN               "p2p_scan"
#define IOVAR_STR_P2P_STATE              "p2p_state"
#define IOVAR_STR_P2P_SSID               "p2p_ssid"
#define IOVAR_STR_P2P_IP_ADDR            "p2p_ip_addr"
#define IOVAR_STR_NRATE                  "nrate"
#define IOVAR_STR_BGRATE                 "bg_rate"
#define IOVAR_STR_ARATE                  "a_rate"
#define IOVAR_STR_NMODE                  "nmode"
#define IOVAR_STR_MAX_ASSOC              "maxassoc"
#define IOVAR_STR_2G_MULTICAST_RATE      "2g_mrate"
#define IOVAR_STR_2G_RATE                "2g_rate"
#define IOVAR_STR_MPC                    "mpc"
#define IOVAR_STR_IBSS_JOIN              "IBSS_join_only"
#define IOVAR_STR_AMPDU_BA_WINDOW_SIZE   "ampdu_ba_wsize"
#define IOVAR_STR_AMPDU_MPDU             "ampdu_mpdu"
#define IOVAR_STR_AMPDU_RX               "ampdu_rx"
#define IOVAR_STR_AMPDU_RX_FACTOR        "ampdu_rx_factor"
#define IOVAR_STR_AMPDU_HOST_REORDER     "ampdu_hostreorder"
#define IOVAR_STR_MIMO_BW_CAP            "mimo_bw_cap"
#define IOVAR_STR_RMC_ACKREQ             "rmc_ackreq"
#define IOVAR_STR_RMC_STATUS             "rmc_status"
#define IOVAR_STR_RMC_COUNTS             "rmc_stats"
#define IOVAR_STR_RMC_ROLE               "rmc_role"
#define IOVAR_STR_HT40_INTOLERANCE       "intol40"
#define IOVAR_STR_RAND                   "rand"
#define IOVAR_STR_SSID                   "ssid"
#define IOVAR_STR_WSEC                   "wsec"
#define IOVAR_STR_WPA_AUTH               "wpa_auth"
#define IOVAR_STR_INTERFACE_REMOVE       "interface_remove"
#define IOVAR_STR_SUP_WPA_TMO            "sup_wpa_tmo"
#define IOVAR_STR_JOIN                   "join"
#define IOVAR_STR_TLV                    "tlv"
#define IOVAR_STR_NPHY_ANTSEL            "nphy_antsel"
#define IOVAR_STR_AVB_TIMESTAMP_ADDR     "avb_timestamp_addr"
#define IOVAR_STR_BSS_MAX_ASSOC          "bss_maxassoc"
#define IOVAR_STR_RM_REQ                 "rm_req"
#define IOVAR_STR_RM_REP                 "rm_rep"
#define IOVAR_STR_PSPRETEND_RETRY_LIMIT  "pspretend_retry_limit"
#define IOVAR_STR_PSPRETEND_THRESHOLD    "pspretend_threshold"
#define IOVAR_STR_SWDIV_TIMEOUT          "swdiv_timeout"
#define IOVAR_STR_RESET_CNTS             "reset_cnts"
#define IOVAR_STR_PHYRATE_LOG            "phyrate_log"
#define IOVAR_STR_PHYRATE_LOG_SIZE       "phyrate_log_size"
#define IOVAR_STR_PHYRATE_LOG_DUMP       "phyrate_dump"
#define IOVAR_STR_SCAN_ASSOC_TIME        "scan_assoc_time"
#define IOVAR_STR_SCAN_UNASSOC_TIME      "scan_unassoc_time"
#define IOVAR_STR_SCAN_PASSIVE_TIME      "scan_passive_time"
#define IOVAR_STR_SCAN_HOME_TIME         "scan_home_time"
#define IOVAR_STR_SCAN_NPROBES           "scan_nprobes"
#define IOVAR_STR_AUTOCOUNTRY            "autocountry"
#define IOVAR_STR_CAP                    "cap"
#define IOVAR_STR_MPDU_PER_AMPDU         "ampdu_mpdu"
#define IOVAR_STR_VHT_FEATURES           "vht_features"
#define IOVAR_STR_CHANSPEC               "chanspec"

#define IOVAR_STR_WOWL                   "wowl"
#define IOVAR_STR_WOWL_OS                "wowl_os"
#define IOVAR_STR_WOWL_KEEP_ALIVE        "wowl_keepalive"
#define IOVAR_STR_WOWL_PATTERN           "wowl_pattern"
#define IOVAR_STR_WOWL_PATTERN_CLR       "clr"
#define IOVAR_STR_WOWL_PATTERN_ADD       "add"
#define IOVAR_STR_WOWL_ARP_HOST_IP       "wowl_arp_hostip"
#define IOVAR_STR_ULP_WAIT               "ulp_wait"
#define IOVAR_STR_ULP                    "ulp"

#define IOVAR_STR_PNO_ON                 "pfn"
#define IOVAR_STR_PNO_ADD                "pfn_add"
#define IOVAR_STR_PNO_SET                "pfn_set"
#define IOVAR_STR_PNO_CLEAR              "pfnclear"
#define IOVAR_STR_SCAN_CACHE_CLEAR       "scancache_clear"
#define MCS_SETLEN                       16

#define IOVAR_STR_RRM                    "rrm"
#define IOVAR_STR_RRM_NOISE_REQ          "rrm_noise_req"
#define IOVAR_STR_RRM_NBR_REQ            "rrm_nbr_req"
#define IOVAR_STR_RRM_LM_REQ             "rrm_lm_req"
#define IOVAR_STR_RRM_STAT_REQ           "rrm_stat_req"
#define IOVAR_STR_RRM_FRAME_REQ          "rrm_frame_req"
#define IOVAR_STR_RRM_CHLOAD_REQ         "rrm_chload_req"
#define IOVAR_STR_RRM_BCN_REQ            "rrm_bcn_req"
#define IOVAR_STR_RRM_NBR_LIST           "rrm_nbr_list"
#define IOVAR_STR_RRM_NBR_ADD            "rrm_nbr_add_nbr"
#define IOVAR_STR_RRM_NBR_DEL            "rrm_nbr_del_nbr"
#define IOVAR_STR_RRM_BCNREQ_THRTL_WIN   "rrm_bcn_req_thrtl_win"
#define IOVAR_STR_RRM_BCNREQ_MAXOFF_TIME "rrm_bcn_req_max_off_chan_time"
#define IOVAR_STR_RRM_BCNREQ_TRFMS_PRD   "rrm_bcn_req_traff_meas_per"

#define IOVAR_STR_WNM                    "wnm"
#define IOVAR_STR_BSSTRANS_QUERY         "wnm_bsstrans_query"
#define IOVAR_STR_BSSTRANS_RESP          "wnm_bsstrans_resp"

#define IOVAR_STR_MESH_ADD_ROUTE         "mesh_add_route"
#define IOVAR_STR_MESH_DEL_ROUTE         "mesh_del_route"
#define IOVAR_STR_MESH_FIND              "mesh_find"
#define IOVAR_STR_MESH_FILTER            "mesh_filter"
#define IOVAR_STR_MESH_PEER              "mesh_peer"
#define IOVAR_STR_MESH_PEER_STATUS       "mesh_peer_status"
#define IOVAR_STR_MESH_DELFILTER         "mesh_delfilter"
#define IOVAR_STR_MESH_MAX_PEERS         "mesh_max_peers"

#define IOVAR_STR_FBT_OVER_DS            "fbtoverds"
#define IOVAR_STR_FBT_CAPABILITIES       "fbt_cap"

#define IOVAR_STR_MFP                    "mfp"

#define IOVAR_STR_OTPRAW                 "otpraw"
#define IOVAR_NAN                        "nan"
#define IOVAR_STR_CLMLOAD                "clmload"
#define IOVAR_STR_CLMLOAD_STATUS         "clmload_status"
#define IOVAR_STR_CLMVER                 "clmver"
#define IOVAR_STR_MEMUSE                 "memuse"

#define IOVAR_STR_LDPC_CAP               "ldpc_cap"
#define IOVAR_STR_LDPC_TX                "ldpc_tx"
#define IOVAR_STR_SGI_RX                 "sgi_rx"
#define IOVAR_STR_SGI_TX                 "sgi_tx"

#define IOVAR_STR_APIVTW_OVERRIDE        "brcmapivtwo"

#define IOVAR_STR_BWTE_BWTE_GCI_MASK     "bwte_gci_mask"
#define IOVAR_STR_BWTE_GCI_SENDMSG       "bwte_gci_sendm"
#define IOVAR_STR_WD_DISABLE             "wd_disable"
#define IOVAR_STR_DLTRO                  "dltro"
#define IOVAR_STR_SAE_PASSWORD           "sae_password"

#define IOVAR_STR_BTC_LESCAN_PARAMS      "btc_lescan_params"

#define IOVAR_STR_ARP_VERSION            "arp_version"
#define IOVAR_STR_ARP_PEERAGE            "arp_peerage"
#define IOVAR_STR_ARPOE                  "arpoe"
#define IOVAR_STR_ARP_OL                 "arp_ol"
#define IOVAR_STR_ARP_TABLE_CLEAR        "arp_table_clear"
#define IOVAR_STR_ARP_HOSTIP             "arp_hostip"
#define IOVAR_STR_ARP_HOSTIP_CLEAR       "arp_hostip_clear"
#define IOVAR_STR_ARP_STATS              "arp_stats"
#define IOVAR_STR_ARP_STATS_CLEAR        "arp_stats_clear"
#define IOVAR_STR_TKO                    "tko"

/* This value derived from the above strings, which appear maxed out in the 20s */
#define IOVAR_NAME_STR_MAX_SIZE          32

#define WLC_IOCTL_MAGIC                    (0x14e46c77)
#define WLC_IOCTL_VERSION                  (1)
#define WLC_IOCTL_SMLEN                    (256)
#define WLC_IOCTL_MEDLEN                   (1536)
#define WLC_IOCTL_MAXLEN                   (8192)

#define WLC_GET_MAGIC                      ( (uint32_t)0 )
#define WLC_GET_VERSION                    ( (uint32_t)1 )
#define WLC_UP                             ( (uint32_t)2 )
#define WLC_DOWN                           ( (uint32_t)3 )
#define WLC_GET_LOOP                       ( (uint32_t)4 )
#define WLC_SET_LOOP                       ( (uint32_t)5 )
#define WLC_DUMP                           ( (uint32_t)6 )
#define WLC_GET_MSGLEVEL                   ( (uint32_t)7 )
#define WLC_SET_MSGLEVEL                   ( (uint32_t)8 )
#define WLC_GET_PROMISC                    ( (uint32_t)9 )
#define WLC_SET_PROMISC                    ( (uint32_t)10 )
#define WLC_GET_RATE                       ( (uint32_t)12 )
#define WLC_GET_INSTANCE                   ( (uint32_t)14 )
#define WLC_GET_INFRA                      ( (uint32_t)19 )
#define WLC_SET_INFRA                      ( (uint32_t)20 )
#define WLC_GET_AUTH                       ( (uint32_t)21 )
#define WLC_SET_AUTH                       ( (uint32_t)22 )
#define WLC_GET_BSSID                      ( (uint32_t)23 )
#define WLC_SET_BSSID                      ( (uint32_t)24 )
#define WLC_GET_SSID                       ( (uint32_t)25 )
#define WLC_SET_SSID                       ( (uint32_t)26 )
#define WLC_RESTART                        ( (uint32_t)27 )
#define WLC_GET_CHANNEL                    ( (uint32_t)29 )
#define WLC_SET_CHANNEL                    ( (uint32_t)30 )
#define WLC_GET_SRL                        ( (uint32_t)31 )
#define WLC_SET_SRL                        ( (uint32_t)32 )
#define WLC_GET_LRL                        ( (uint32_t)33 )
#define WLC_SET_LRL                        ( (uint32_t)34 )
#define WLC_GET_PLCPHDR                    ( (uint32_t)35 )
#define WLC_SET_PLCPHDR                    ( (uint32_t)36 )
#define WLC_GET_RADIO                      ( (uint32_t)37 )
#define WLC_SET_RADIO                      ( (uint32_t)38 )
#define WLC_GET_PHYTYPE                    ( (uint32_t)39 )
#define WLC_DUMP_RATE                      ( (uint32_t)40 )
#define WLC_SET_RATE_PARAMS                ( (uint32_t)41 )
#define WLC_GET_KEY                        ( (uint32_t)44 )
#define WLC_SET_KEY                        ( (uint32_t)45 )
#define WLC_GET_REGULATORY                 ( (uint32_t)46 )
#define WLC_SET_REGULATORY                 ( (uint32_t)47 )
#define WLC_GET_PASSIVE_SCAN               ( (uint32_t)48 )
#define WLC_SET_PASSIVE_SCAN               ( (uint32_t)49 )
#define WLC_SCAN                           ( (uint32_t)50 )
#define WLC_SCAN_RESULTS                   ( (uint32_t)51 )
#define WLC_DISASSOC                       ( (uint32_t)52 )
#define WLC_REASSOC                        ( (uint32_t)53 )
#define WLC_GET_ROAM_TRIGGER               ( (uint32_t)54 )
#define WLC_SET_ROAM_TRIGGER               ( (uint32_t)55 )
#define WLC_GET_ROAM_DELTA                 ( (uint32_t)56 )
#define WLC_SET_ROAM_DELTA                 ( (uint32_t)57 )
#define WLC_GET_ROAM_SCAN_PERIOD           ( (uint32_t)58 )
#define WLC_SET_ROAM_SCAN_PERIOD           ( (uint32_t)59 )
#define WLC_EVM                            ( (uint32_t)60 )
#define WLC_GET_TXANT                      ( (uint32_t)61 )
#define WLC_SET_TXANT                      ( (uint32_t)62 )
#define WLC_GET_ANTDIV                     ( (uint32_t)63 )
#define WLC_SET_ANTDIV                     ( (uint32_t)64 )
#define WLC_GET_CLOSED                     ( (uint32_t)67 )
#define WLC_SET_CLOSED                     ( (uint32_t)68 )
#define WLC_GET_MACLIST                    ( (uint32_t)69 )
#define WLC_SET_MACLIST                    ( (uint32_t)70 )
#define WLC_GET_RATESET                    ( (uint32_t)71 )
#define WLC_SET_RATESET                    ( (uint32_t)72 )
#define WLC_LONGTRAIN                      ( (uint32_t)74 )
#define WLC_GET_BCNPRD                     ( (uint32_t)75 )
#define WLC_SET_BCNPRD                     ( (uint32_t)76 )
#define WLC_GET_DTIMPRD                    ( (uint32_t)77 )
#define WLC_SET_DTIMPRD                    ( (uint32_t)78 )
#define WLC_GET_SROM                       ( (uint32_t)79 )
#define WLC_SET_SROM                       ( (uint32_t)80 )
#define WLC_GET_WEP_RESTRICT               ( (uint32_t)81 )
#define WLC_SET_WEP_RESTRICT               ( (uint32_t)82 )
#define WLC_GET_COUNTRY                    ( (uint32_t)83 )
#define WLC_SET_COUNTRY                    ( (uint32_t)84 )
#define WLC_GET_PM                         ( (uint32_t)85 )
#define WLC_SET_PM                         ( (uint32_t)86 )
#define WLC_GET_WAKE                       ( (uint32_t)87 )
#define WLC_SET_WAKE                       ( (uint32_t)88 )
#define WLC_GET_FORCELINK                  ( (uint32_t)90 )
#define WLC_SET_FORCELINK                  ( (uint32_t)91 )
#define WLC_FREQ_ACCURACY                  ( (uint32_t)92 )
#define WLC_CARRIER_SUPPRESS               ( (uint32_t)93 )
#define WLC_GET_PHYREG                     ( (uint32_t)94 )
#define WLC_SET_PHYREG                     ( (uint32_t)95 )
#define WLC_GET_RADIOREG                   ( (uint32_t)96 )
#define WLC_SET_RADIOREG                   ( (uint32_t)97 )
#define WLC_GET_REVINFO                    ( (uint32_t)98 )
#define WLC_GET_UCANTDIV                   ( (uint32_t)99 )
#define WLC_SET_UCANTDIV                   ( (uint32_t)100 )
#define WLC_R_REG                          ( (uint32_t)101 )
#define WLC_W_REG                          ( (uint32_t)102 )
#define WLC_GET_MACMODE                    ( (uint32_t)105 )
#define WLC_SET_MACMODE                    ( (uint32_t)106 )
#define WLC_GET_MONITOR                    ( (uint32_t)107 )
#define WLC_SET_MONITOR                    ( (uint32_t)108 )
#define WLC_GET_GMODE                      ( (uint32_t)109 )
#define WLC_SET_GMODE                      ( (uint32_t)110 )
#define WLC_GET_LEGACY_ERP                 ( (uint32_t)111 )
#define WLC_SET_LEGACY_ERP                 ( (uint32_t)112 )
#define WLC_GET_RX_ANT                     ( (uint32_t)113 )
#define WLC_GET_CURR_RATESET               ( (uint32_t)114 )
#define WLC_GET_SCANSUPPRESS               ( (uint32_t)115 )
#define WLC_SET_SCANSUPPRESS               ( (uint32_t)116 )
#define WLC_GET_AP                         ( (uint32_t)117 )
#define WLC_SET_AP                         ( (uint32_t)118 )
#define WLC_GET_EAP_RESTRICT               ( (uint32_t)119 )
#define WLC_SET_EAP_RESTRICT               ( (uint32_t)120 )
#define WLC_SCB_AUTHORIZE                  ( (uint32_t)121 )
#define WLC_SCB_DEAUTHORIZE                ( (uint32_t)122 )
#define WLC_GET_WDSLIST                    ( (uint32_t)123 )
#define WLC_SET_WDSLIST                    ( (uint32_t)124 )
#define WLC_GET_ATIM                       ( (uint32_t)125 )
#define WLC_SET_ATIM                       ( (uint32_t)126 )
#define WLC_GET_RSSI                       ( (uint32_t)127 )
#define WLC_GET_PHYANTDIV                  ( (uint32_t)128 )
#define WLC_SET_PHYANTDIV                  ( (uint32_t)129 )
#define WLC_AP_RX_ONLY                     ( (uint32_t)130 )
#define WLC_GET_TX_PATH_PWR                ( (uint32_t)131 )
#define WLC_SET_TX_PATH_PWR                ( (uint32_t)132 )
#define WLC_GET_WSEC                       ( (uint32_t)133 )
#define WLC_SET_WSEC                       ( (uint32_t)134 )
#define WLC_GET_PHY_NOISE                  ( (uint32_t)135 )
#define WLC_GET_BSS_INFO                   ( (uint32_t)136 )
#define WLC_GET_PKTCNTS                    ( (uint32_t)137 )
#define WLC_GET_LAZYWDS                    ( (uint32_t)138 )
#define WLC_SET_LAZYWDS                    ( (uint32_t)139 )
#define WLC_GET_BANDLIST                   ( (uint32_t)140 )
#define WLC_GET_BAND                       ( (uint32_t)141 )
#define WLC_SET_BAND                       ( (uint32_t)142 )
#define WLC_SCB_DEAUTHENTICATE             ( (uint32_t)143 )
#define WLC_GET_SHORTSLOT                  ( (uint32_t)144 )
#define WLC_GET_SHORTSLOT_OVERRIDE         ( (uint32_t)145 )
#define WLC_SET_SHORTSLOT_OVERRIDE         ( (uint32_t)146 )
#define WLC_GET_SHORTSLOT_RESTRICT         ( (uint32_t)147 )
#define WLC_SET_SHORTSLOT_RESTRICT         ( (uint32_t)148 )
#define WLC_GET_GMODE_PROTECTION           ( (uint32_t)149 )
#define WLC_GET_GMODE_PROTECTION_OVERRIDE  ( (uint32_t)150 )
#define WLC_SET_GMODE_PROTECTION_OVERRIDE  ( (uint32_t)151 )
#define WLC_UPGRADE                        ( (uint32_t)152 )
#define WLC_GET_IGNORE_BCNS                ( (uint32_t)155 )
#define WLC_SET_IGNORE_BCNS                ( (uint32_t)156 )
#define WLC_GET_SCB_TIMEOUT                ( (uint32_t)157 )
#define WLC_SET_SCB_TIMEOUT                ( (uint32_t)158 )
#define WLC_GET_ASSOCLIST                  ( (uint32_t)159 )
#define WLC_GET_CLK                        ( (uint32_t)160 )
#define WLC_SET_CLK                        ( (uint32_t)161 )
#define WLC_GET_UP                         ( (uint32_t)162 )
#define WLC_OUT                            ( (uint32_t)163 )
#define WLC_GET_WPA_AUTH                   ( (uint32_t)164 )
#define WLC_SET_WPA_AUTH                   ( (uint32_t)165 )
#define WLC_GET_UCFLAGS                    ( (uint32_t)166 )
#define WLC_SET_UCFLAGS                    ( (uint32_t)167 )
#define WLC_GET_PWRIDX                     ( (uint32_t)168 )
#define WLC_SET_PWRIDX                     ( (uint32_t)169 )
#define WLC_GET_TSSI                       ( (uint32_t)170 )
#define WLC_GET_SUP_RATESET_OVERRIDE       ( (uint32_t)171 )
#define WLC_SET_SUP_RATESET_OVERRIDE       ( (uint32_t)172 )
#define WLC_GET_PROTECTION_CONTROL         ( (uint32_t)178 )
#define WLC_SET_PROTECTION_CONTROL         ( (uint32_t)179 )
#define WLC_GET_PHYLIST                    ( (uint32_t)180 )
#define WLC_ENCRYPT_STRENGTH               ( (uint32_t)181 )
#define WLC_DECRYPT_STATUS                 ( (uint32_t)182 )
#define WLC_GET_KEY_SEQ                    ( (uint32_t)183 )
#define WLC_GET_SCAN_CHANNEL_TIME          ( (uint32_t)184 )
#define WLC_SET_SCAN_CHANNEL_TIME          ( (uint32_t)185 )
#define WLC_GET_SCAN_UNASSOC_TIME          ( (uint32_t)186 )
#define WLC_SET_SCAN_UNASSOC_TIME          ( (uint32_t)187 )
#define WLC_GET_SCAN_HOME_TIME             ( (uint32_t)188 )
#define WLC_SET_SCAN_HOME_TIME             ( (uint32_t)189 )
#define WLC_GET_SCAN_NPROBES               ( (uint32_t)190 )
#define WLC_SET_SCAN_NPROBES               ( (uint32_t)191 )
#define WLC_GET_PRB_RESP_TIMEOUT           ( (uint32_t)192 )
#define WLC_SET_PRB_RESP_TIMEOUT           ( (uint32_t)193 )
#define WLC_GET_ATTEN                      ( (uint32_t)194 )
#define WLC_SET_ATTEN                      ( (uint32_t)195 )
#define WLC_GET_SHMEM                      ( (uint32_t)196 )
#define WLC_SET_SHMEM                      ( (uint32_t)197 )
#define WLC_SET_WSEC_TEST                  ( (uint32_t)200 )
#define WLC_SCB_DEAUTHENTICATE_FOR_REASON  ( (uint32_t)201 )
#define WLC_TKIP_COUNTERMEASURES           ( (uint32_t)202 )
#define WLC_GET_PIOMODE                    ( (uint32_t)203 )
#define WLC_SET_PIOMODE                    ( (uint32_t)204 )
#define WLC_SET_ASSOC_PREFER               ( (uint32_t)205 )
#define WLC_GET_ASSOC_PREFER               ( (uint32_t)206 )
#define WLC_SET_ROAM_PREFER                ( (uint32_t)207 )
#define WLC_GET_ROAM_PREFER                ( (uint32_t)208 )
#define WLC_SET_LED                        ( (uint32_t)209 )
#define WLC_GET_LED                        ( (uint32_t)210 )
#define WLC_GET_INTERFERENCE_MODE          ( (uint32_t)211 )
#define WLC_SET_INTERFERENCE_MODE          ( (uint32_t)212 )
#define WLC_GET_CHANNEL_QA                 ( (uint32_t)213 )
#define WLC_START_CHANNEL_QA               ( (uint32_t)214 )
#define WLC_GET_CHANNEL_SEL                ( (uint32_t)215 )
#define WLC_START_CHANNEL_SEL              ( (uint32_t)216 )
#define WLC_GET_VALID_CHANNELS             ( (uint32_t)217 )
#define WLC_GET_FAKEFRAG                   ( (uint32_t)218 )
#define WLC_SET_FAKEFRAG                   ( (uint32_t)219 )
#define WLC_GET_PWROUT_PERCENTAGE          ( (uint32_t)220 )
#define WLC_SET_PWROUT_PERCENTAGE          ( (uint32_t)221 )
#define WLC_SET_BAD_FRAME_PREEMPT          ( (uint32_t)222 )
#define WLC_GET_BAD_FRAME_PREEMPT          ( (uint32_t)223 )
#define WLC_SET_LEAP_LIST                  ( (uint32_t)224 )
#define WLC_GET_LEAP_LIST                  ( (uint32_t)225 )
#define WLC_GET_CWMIN                      ( (uint32_t)226 )
#define WLC_SET_CWMIN                      ( (uint32_t)227 )
#define WLC_GET_CWMAX                      ( (uint32_t)228 )
#define WLC_SET_CWMAX                      ( (uint32_t)229 )
#define WLC_GET_WET                        ( (uint32_t)230 )
#define WLC_SET_WET                        ( (uint32_t)231 )
#define WLC_GET_PUB                        ( (uint32_t)232 )
#define WLC_GET_KEY_PRIMARY                ( (uint32_t)235 )
#define WLC_SET_KEY_PRIMARY                ( (uint32_t)236 )
#define WLC_GET_ACI_ARGS                   ( (uint32_t)238 )
#define WLC_SET_ACI_ARGS                   ( (uint32_t)239 )
#define WLC_UNSET_CALLBACK                 ( (uint32_t)240 )
#define WLC_SET_CALLBACK                   ( (uint32_t)241 )
#define WLC_GET_RADAR                      ( (uint32_t)242 )
#define WLC_SET_RADAR                      ( (uint32_t)243 )
#define WLC_SET_SPECT_MANAGMENT            ( (uint32_t)244 )
#define WLC_GET_SPECT_MANAGMENT            ( (uint32_t)245 )
#define WLC_WDS_GET_REMOTE_HWADDR          ( (uint32_t)246 )
#define WLC_WDS_GET_WPA_SUP                ( (uint32_t)247 )
#define WLC_SET_CS_SCAN_TIMER              ( (uint32_t)248 )
#define WLC_GET_CS_SCAN_TIMER              ( (uint32_t)249 )
#define WLC_MEASURE_REQUEST                ( (uint32_t)250 )
#define WLC_INIT                           ( (uint32_t)251 )
#define WLC_SEND_QUIET                     ( (uint32_t)252 )
#define WLC_KEEPALIVE                      ( (uint32_t)253 )
#define WLC_SEND_PWR_CONSTRAINT            ( (uint32_t)254 )
#define WLC_UPGRADE_STATUS                 ( (uint32_t)255 )
#define WLC_CURRENT_PWR                    ( (uint32_t)256 )
#define WLC_GET_SCAN_PASSIVE_TIME          ( (uint32_t)257 )
#define WLC_SET_SCAN_PASSIVE_TIME          ( (uint32_t)258 )
#define WLC_LEGACY_LINK_BEHAVIOR           ( (uint32_t)259 )
#define WLC_GET_CHANNELS_IN_COUNTRY        ( (uint32_t)260 )
#define WLC_GET_COUNTRY_LIST               ( (uint32_t)261 )
#define WLC_GET_VAR                        ( (uint32_t)262 )
#define WLC_SET_VAR                        ( (uint32_t)263 )
#define WLC_NVRAM_GET                      ( (uint32_t)264 )
#define WLC_NVRAM_SET                      ( (uint32_t)265 )
#define WLC_NVRAM_DUMP                     ( (uint32_t)266 )
#define WLC_REBOOT                         ( (uint32_t)267 )
#define WLC_SET_WSEC_PMK                   ( (uint32_t)268 )
#define WLC_GET_AUTH_MODE                  ( (uint32_t)269 )
#define WLC_SET_AUTH_MODE                  ( (uint32_t)270 )
#define WLC_GET_WAKEENTRY                  ( (uint32_t)271 )
#define WLC_SET_WAKEENTRY                  ( (uint32_t)272 )
#define WLC_NDCONFIG_ITEM                  ( (uint32_t)273 )
#define WLC_NVOTPW                         ( (uint32_t)274 )
#define WLC_OTPW                           ( (uint32_t)275 )
#define WLC_IOV_BLOCK_GET                  ( (uint32_t)276 )
#define WLC_IOV_MODULES_GET                ( (uint32_t)277 )
#define WLC_SOFT_RESET                     ( (uint32_t)278 )
#define WLC_GET_ALLOW_MODE                 ( (uint32_t)279 )
#define WLC_SET_ALLOW_MODE                 ( (uint32_t)280 )
#define WLC_GET_DESIRED_BSSID              ( (uint32_t)281 )
#define WLC_SET_DESIRED_BSSID              ( (uint32_t)282 )
#define WLC_DISASSOC_MYAP                  ( (uint32_t)283 )
#define WLC_GET_NBANDS                     ( (uint32_t)284 )
#define WLC_GET_BANDSTATES                 ( (uint32_t)285 )
#define WLC_GET_WLC_BSS_INFO               ( (uint32_t)286 )
#define WLC_GET_ASSOC_INFO                 ( (uint32_t)287 )
#define WLC_GET_OID_PHY                    ( (uint32_t)288 )
#define WLC_SET_OID_PHY                    ( (uint32_t)289 )
#define WLC_SET_ASSOC_TIME                 ( (uint32_t)290 )
#define WLC_GET_DESIRED_SSID               ( (uint32_t)291 )
#define WLC_GET_CHANSPEC                   ( (uint32_t)292 )
#define WLC_GET_ASSOC_STATE                ( (uint32_t)293 )
#define WLC_SET_PHY_STATE                  ( (uint32_t)294 )
#define WLC_GET_SCAN_PENDING               ( (uint32_t)295 )
#define WLC_GET_SCANREQ_PENDING            ( (uint32_t)296 )
#define WLC_GET_PREV_ROAM_REASON           ( (uint32_t)297 )
#define WLC_SET_PREV_ROAM_REASON           ( (uint32_t)298 )
#define WLC_GET_BANDSTATES_PI              ( (uint32_t)299 )
#define WLC_GET_PHY_STATE                  ( (uint32_t)300 )
#define WLC_GET_BSS_WPA_RSN                ( (uint32_t)301 )
#define WLC_GET_BSS_WPA2_RSN               ( (uint32_t)302 )
#define WLC_GET_BSS_BCN_TS                 ( (uint32_t)303 )
#define WLC_GET_INT_DISASSOC               ( (uint32_t)304 )
#define WLC_SET_NUM_PEERS                  ( (uint32_t)305 )
#define WLC_GET_NUM_BSS                    ( (uint32_t)306 )
#define WLC_GET_WSEC_PMK                   ( (uint32_t)318 )
#define WLC_GET_RANDOM_BYTES               ( (uint32_t)319 )
#define WLC_LAST                           ( (uint32_t)320 )

#define EPICTRL_COOKIE        0xABADCEDE
#define CMN_IOCTL_OFF 0x180
#define WL_OID_BASE        0xFFE41420
#define OID_WL_GETINSTANCE    (WL_OID_BASE + WLC_GET_INSTANCE)
#define OID_WL_GET_FORCELINK    (WL_OID_BASE + WLC_GET_FORCELINK)
#define OID_WL_SET_FORCELINK    (WL_OID_BASE + WLC_SET_FORCELINK)
#define    OID_WL_ENCRYPT_STRENGTH    (WL_OID_BASE + WLC_ENCRYPT_STRENGTH)
#define OID_WL_DECRYPT_STATUS    (WL_OID_BASE + WLC_DECRYPT_STATUS)
#define OID_LEGACY_LINK_BEHAVIOR (WL_OID_BASE + WLC_LEGACY_LINK_BEHAVIOR)
#define OID_WL_NDCONFIG_ITEM (WL_OID_BASE + WLC_NDCONFIG_ITEM)
#define OID_STA_CHANSPEC    (WL_OID_BASE + WLC_GET_CHANSPEC)
#define OID_STA_NBANDS        (WL_OID_BASE + WLC_GET_NBANDS)
#define OID_STA_GET_PHY        (WL_OID_BASE + WLC_GET_OID_PHY)
#define OID_STA_SET_PHY        (WL_OID_BASE + WLC_SET_OID_PHY)
#define OID_STA_ASSOC_TIME    (WL_OID_BASE + WLC_SET_ASSOC_TIME)
#define OID_STA_DESIRED_SSID (WL_OID_BASE + WLC_GET_DESIRED_SSID)
#define OID_STA_SET_PHY_STATE (WL_OID_BASE + WLC_SET_PHY_STATE)
#define OID_STA_SCAN_PENDING    (WL_OID_BASE + WLC_GET_SCAN_PENDING)
#define OID_STA_SCANREQ_PENDING (WL_OID_BASE + WLC_GET_SCANREQ_PENDING)
#define OID_STA_GET_ROAM_REASON (WL_OID_BASE + WLC_GET_PREV_ROAM_REASON)
#define OID_STA_SET_ROAM_REASON (WL_OID_BASE + WLC_SET_PREV_ROAM_REASON)
#define OID_STA_GET_PHY_STATE (WL_OID_BASE + WLC_GET_PHY_STATE)
#define OID_STA_INT_DISASSOC    (WL_OID_BASE + WLC_GET_INT_DISASSOC)
#define OID_STA_SET_NUM_PEERS    (WL_OID_BASE + WLC_SET_NUM_PEERS)
#define OID_STA_GET_NUM_BSS        (WL_OID_BASE + WLC_GET_NUM_BSS)
#define WL_DECRYPT_STATUS_SUCCESS    1
#define WL_DECRYPT_STATUS_FAILURE    2
#define WL_DECRYPT_STATUS_UNKNOWN    3
#define WLC_UPGRADE_SUCCESS            0
#define WLC_UPGRADE_PENDING            1
typedef struct
{
    int8_t *name;
    void *param;
} ndconfig_item_t;
typedef struct _wl_assoc_result
{
    unsigned long associated;
    unsigned long NDIS_auth;
    unsigned long NDIS_infra;
} wl_assoc_result_t;
#define WL_RADIO_SW_DISABLE        (1 << 0)
#define WL_RADIO_HW_DISABLE        (1 << 1)
#define WL_RADIO_MPC_DISABLE       (1 << 2)
#define WL_RADIO_COUNTRY_DISABLE   (1 << 3)
#define WL_TXPWR_OVERRIDE          (1U << 31)
#define WL_PHY_PAVARS_LEN          (6)
#define WL_DIAG_INTERRUPT          (1)
#define WL_DIAG_LOOPBACK           (2)
#define WL_DIAG_MEMORY             (3)
#define WL_DIAG_LED                (4)
#define WL_DIAG_REG                (5)
#define WL_DIAG_SROM               (6)
#define WL_DIAG_DMA                (7)
#define WL_DIAGERR_SUCCESS            (0)
#define WL_DIAGERR_FAIL_TO_RUN        (1)
#define WL_DIAGERR_NOT_SUPPORTED      (2)
#define WL_DIAGERR_INTERRUPT_FAIL     (3)
#define WL_DIAGERR_LOOPBACK_FAIL      (4)
#define WL_DIAGERR_SROM_FAIL          (5)
#define WL_DIAGERR_SROM_BADCRC        (6)
#define WL_DIAGERR_REG_FAIL           (7)
#define WL_DIAGERR_MEMORY_FAIL        (8)
#define WL_DIAGERR_NOMEM              (9)
#define WL_DIAGERR_DMA_FAIL           (10)
#define WL_DIAGERR_MEMORY_TIMEOUT     (11)
#define WL_DIAGERR_MEMORY_BADPATTERN  (12)
#define    WLC_BAND_AUTO                (0)
#define    WLC_BAND_5G                  (1)
#define    WLC_BAND_2G                  (2)
#define    WLC_BAND_ALL                 (3)
#define WL_CHAN_FREQ_RANGE_2G           (0)
#define WL_CHAN_FREQ_RANGE_5GL          (1)
#define WL_CHAN_FREQ_RANGE_5GM          (2)
#define WL_CHAN_FREQ_RANGE_5GH          (3)
#define WLC_PHY_TYPE_A               (0)
#define WLC_PHY_TYPE_B               (1)
#define WLC_PHY_TYPE_G               (2)
#define WLC_PHY_TYPE_N               (4)
#define WLC_PHY_TYPE_LP              (5)
#define WLC_PHY_TYPE_SSN             (6)
#define WLC_PHY_TYPE_NULL          (0xf)
#define WLC_MACMODE_DISABLED         (0)
#define WLC_MACMODE_DENY             (1)
#define WLC_MACMODE_ALLOW            (2)
#define GMODE_LEGACY_B               (0)
#define GMODE_AUTO                   (1)
#define GMODE_ONLY                   (2)
#define GMODE_B_DEFERRED             (3)
#define GMODE_PERFORMANCE            (4)
#define GMODE_LRS                    (5)
#define GMODE_MAX                    (6)
#define WLC_PLCP_AUTO               (-1)
#define WLC_PLCP_SHORT               (0)
#define WLC_PLCP_LONG                (1)
#define WLC_PROTECTION_AUTO         (-1)
#define WLC_PROTECTION_OFF           (0)
#define WLC_PROTECTION_ON            (1)
#define WLC_PROTECTION_MMHDR_ONLY    (2)
#define WLC_PROTECTION_CTS_ONLY      (3)
#define WLC_PROTECTION_CTL_OFF       (0)
#define WLC_PROTECTION_CTL_LOCAL     (1)
#define WLC_PROTECTION_CTL_OVERLAP   (2)
#define WLC_N_PROTECTION_OFF         (0)
#define WLC_N_PROTECTION_OPTIONAL    (1)
#define WLC_N_PROTECTION_20IN40      (2)
#define WLC_N_PROTECTION_MIXEDMODE   (3)
#define WLC_N_PREAMBLE_MIXEDMODE     (0)
#define WLC_N_PREAMBLE_GF            (1)
#define WLC_N_BW_20ALL               (0)
#define WLC_N_BW_40ALL               (1)
#define WLC_N_BW_20IN2G_40IN5G       (2)
#define WLC_N_TXRX_CHAIN0            (0)
#define WLC_N_TXRX_CHAIN1            (1)
#define WLC_N_SGI_20              (0x01)
#define WLC_N_SGI_40              (0x02)
#define PM_OFF                       (0)
#define PM_MAX                       (1)
#define PM_FAST                      (2)
#define PM_FORCE_OFF                 (3)
#define INTERFERE_NONE               (0)
#define NON_WLAN                     (1)
#define WLAN_MANUAL                  (2)
#define WLAN_AUTO                    (3)
#define AUTO_ACTIVE             (1 << 7)
typedef struct wl_aci_args
{
    int32_t enter_aci_thresh;
    int32_t exit_aci_thresh;
    int32_t usec_spin;
    int32_t glitch_delay;
    uint16_t nphy_adcpwr_enter_thresh;
    uint16_t nphy_adcpwr_exit_thresh;
    uint16_t nphy_repeat_ctr;
    uint16_t nphy_num_samples;
    uint16_t nphy_undetect_window_sz;
    uint16_t nphy_b_energy_lo_aci;
    uint16_t nphy_b_energy_md_aci;
    uint16_t nphy_b_energy_hi_aci;
} wl_aci_args_t;
#define WL_ACI_ARGS_LEGACY_LENGTH    16
typedef struct
{
    int32_t npulses;
    int32_t ncontig;
    int32_t min_pw;
    int32_t max_pw;
    uint16_t thresh0;
    uint16_t thresh1;
    uint16_t blank;
    uint16_t fmdemodcfg;
    int32_t npulses_lp;
    int32_t min_pw_lp;
    int32_t max_pw_lp;
    int32_t min_fm_lp;
    int32_t max_deltat_lp;
    int32_t min_deltat;
    int32_t max_deltat;
    uint16_t autocorr;
    uint16_t st_level_time;
    uint16_t t2_min;
    uint32_t version;
} wl_radar_args_t;
#define WL_RADAR_ARGS_VERSION 1
#define WL_RADAR_DETECTOR_OFF        0
#define WL_RADAR_DETECTOR_ON        1
#define WL_RADAR_SIMULATED        2
#define WL_RSSI_ANT_VERSION    1
#define WL_RSSI_ANT_MAX        4
typedef struct
{
    uint32_t version;
    uint32_t count;
    int8_t rssi_ant[WL_RSSI_ANT_MAX];
} wl_rssi_ant_t;
#define WL_DFS_CACSTATE_IDLE        0
#define    WL_DFS_CACSTATE_PREISM_CAC    1
#define WL_DFS_CACSTATE_ISM        2
#define WL_DFS_CACSTATE_CSA        3
#define WL_DFS_CACSTATE_POSTISM_CAC    4
#define WL_DFS_CACSTATE_PREISM_OOC    5
#define WL_DFS_CACSTATE_POSTISM_OOC    6
#define WL_DFS_CACSTATES        7
typedef struct
{
    uint32_t state;
    uint32_t duration;
    wl_chanspec_t chanspec_cleared;
    uint16_t pad;
} wl_dfs_status_t;
#define NUM_PWRCTRL_RATES 12
typedef struct
{
    uint8_t txpwr_band_max[NUM_PWRCTRL_RATES];
    uint8_t txpwr_limit[NUM_PWRCTRL_RATES];
    uint8_t txpwr_local_max;
    uint8_t txpwr_local_constraint;
    uint8_t txpwr_chan_reg_max;
    uint8_t txpwr_target[2][NUM_PWRCTRL_RATES];
    uint8_t txpwr_est_Pout[2];
    uint8_t txpwr_opo[NUM_PWRCTRL_RATES];
    uint8_t txpwr_bphy_cck_max[NUM_PWRCTRL_RATES];
    uint8_t txpwr_bphy_ofdm_max;
    uint8_t txpwr_aphy_max[NUM_PWRCTRL_RATES];
    int8_t txpwr_antgain[2];
    uint8_t txpwr_est_Pout_gofdm;
} tx_power_legacy_t;
#define WL_TX_POWER_RATES    45
#define WL_TX_POWER_CCK_FIRST    0
#define WL_TX_POWER_CCK_NUM    4
#define WL_TX_POWER_OFDM_FIRST    4
#define WL_TX_POWER_OFDM_NUM    8
#define WL_TX_POWER_MCS_SISO_NUM 8
#define WL_TX_POWER_MCS20_FIRST    12
#define WL_TX_POWER_MCS20_NUM    16
#define WL_TX_POWER_MCS40_FIRST    28
#define WL_TX_POWER_MCS40_NUM    17
#define WL_TX_POWER_MCS20SISO_NUM    8
#define WL_TX_POWER_MCS40_LAST    44
#define WL_TX_POWER_F_ENABLED    1
#define WL_TX_POWER_F_HW    2
#define WL_TX_POWER_F_MIMO    4
#define WL_TX_POWER_F_SISO    8
#define WL_TX_POWER_F_40M_CAP    16

#define MAX_QTX_POWER 32
typedef struct
{
    uint32_t flags;
    wl_chanspec_t chanspec;
    wl_chanspec_t local_chanspec;
    uint8_t local_max;
    uint8_t local_constraint;
    int8_t antgain[2];
    uint8_t rf_cores;
    uint8_t est_Pout[4];
    uint8_t est_Pout_cck;
    uint8_t user_limit[WL_TX_POWER_RATES];
    uint8_t reg_limit[WL_TX_POWER_RATES];
    uint8_t board_limit[WL_TX_POWER_RATES];
    uint8_t target[WL_TX_POWER_RATES];
} tx_power_t;
typedef struct tx_inst_power
{
    uint8_t txpwr_est_Pout[2];
    uint8_t txpwr_est_Pout_gofdm;
} tx_inst_power_t;
#define WLC_MEASURE_TPC            1
#define WLC_MEASURE_CHANNEL_BASIC    2
#define WLC_MEASURE_CHANNEL_CCA        3
#define WLC_MEASURE_CHANNEL_RPI        4
#define SPECT_MNGMT_OFF            0
#define SPECT_MNGMT_LOOSE_11H        1
#define SPECT_MNGMT_STRICT_11H        2
#define SPECT_MNGMT_STRICT_11D        3
#define SPECT_MNGMT_LOOSE_11H_D        4
#define WL_CHAN_VALID_HW    (1 << 0)
#define WL_CHAN_VALID_SW    (1 << 1)
#define WL_CHAN_BAND_5G        (1 << 2)
#define WL_CHAN_RADAR        (1 << 3)
#define WL_CHAN_INACTIVE    (1 << 4)
#define WL_CHAN_PASSIVE        (1 << 5)
#define WL_CHAN_RESTRICTED    (1 << 6)
#define    WL_BTC_DISABLE        0
#define WL_BTC_ENABLE        (1 << 0)
#define WL_BTC_PREMPT        (1 << 1)
#define WL_BTC_PARTIAL        (1 << 2)
#define WL_BTC_DEFAULT        (1 << 3)
#define WL_BTC_HYBRID        (WL_BTC_ENABLE | WL_BTC_PARTIAL)
#define WL_INF_BTC_DISABLE      0
#define WL_INF_BTC_ENABLE       1
#define WL_INF_BTC_AUTO         3
#define    WL_BTC_DEFWIRE        0
#define WL_BTC_2WIRE        2
#define WL_BTC_3WIRE        3
#define WL_BTC_4WIRE        4
#define WL_BTC_FLAG_PREMPT               (1 << 0)
#define WL_BTC_FLAG_BT_DEF               (1 << 1)
#define WL_BTC_FLAG_ACTIVE_PROT          (1 << 2)
#define WL_BTC_FLAG_SIM_RSP              (1 << 3)
#define WL_BTC_FLAG_PS_PROTECT           (1 << 4)
#define WL_BTC_FLAG_SIM_TX_LP             (1 << 5)
#define WL_BTC_FLAG_ECI                  (1 << 6)
#define WL_ERROR_VAL        0x00000001
#define WL_TRACE_VAL        0x00000002
#define WL_PRHDRS_VAL        0x00000004
#define WL_PRPKT_VAL        0x00000008
#define WL_INFORM_VAL        0x00000010
#define WL_TMP_VAL        0x00000020
#define WL_OID_VAL        0x00000040
#define WL_RATE_VAL        0x00000080
#define WL_ASSOC_VAL        0x00000100
#define WL_PRUSR_VAL        0x00000200
#define WL_PS_VAL        0x00000400
#define WL_TXPWR_VAL        0x00000800
#define WL_PORT_VAL        0x00001000
#define WL_DUAL_VAL        0x00002000
#define WL_WSEC_VAL        0x00004000
#define WL_WSEC_DUMP_VAL    0x00008000
#define WL_LOG_VAL        0x00010000
#define WL_NRSSI_VAL        0x00020000
#define WL_LOFT_VAL        0x00040000
#define WL_REGULATORY_VAL    0x00080000
#define WL_PHYCAL_VAL        0x00100000
#define WL_RADAR_VAL        0x00200000
#define WL_MPC_VAL        0x00400000
#define WL_APSTA_VAL        0x00800000
#define WL_DFS_VAL        0x01000000
#define WL_BA_VAL        0x02000000
#define WL_NITRO_VAL        0x04000000
#define WL_MBSS_VAL        0x04000000
#define WL_CAC_VAL        0x08000000
#define WL_AMSDU_VAL        0x10000000
#define WL_AMPDU_VAL        0x20000000
#define WL_FFPLD_VAL        0x40000000
#define WL_NIN_VAL        0x80000000
#define WL_DPT_VAL         0x00000001
#define WL_SCAN_VAL        0x00000002
#define WL_WOWL_VAL        0x00000004
#define WL_COEX_VAL        0x00000008
#define WL_RTDC_VAL        0x00000010
#define WL_BTA_VAL        0x00000040
#define    WL_LED_NUMGPIO        16
#define    WL_LED_OFF        0
#define    WL_LED_ON        1
#define    WL_LED_ACTIVITY        2
#define    WL_LED_RADIO        3
#define    WL_LED_ARADIO        4
#define    WL_LED_BRADIO        5
#define    WL_LED_BGMODE        6
#define    WL_LED_WI1        7
#define    WL_LED_WI2        8
#define    WL_LED_WI3        9
#define    WL_LED_ASSOC        10
#define    WL_LED_INACTIVE        11
#define    WL_LED_ASSOCACT        12
#define    WL_LED_NUMBEHAVIOR    13
#define    WL_LED_BEH_MASK        0x7f
#define    WL_LED_AL_MASK        0x80
#define WL_NUMCHANNELS        64
#define WL_NUMCHANSPECS        100
#define WL_WDS_WPA_ROLE_AUTH    0
#define WL_WDS_WPA_ROLE_SUP    1
#define WL_WDS_WPA_ROLE_AUTO    255
#define WL_EVENTING_MASK_LEN    ( (WLC_E_LAST + 7) / 8 )

#define VNDR_IE_CMD_LEN        4
#define VNDR_IE_BEACON_FLAG    0x1
#define VNDR_IE_PRBRSP_FLAG    0x2
#define VNDR_IE_ASSOCRSP_FLAG    0x4
#define VNDR_IE_AUTHRSP_FLAG    0x8
#define VNDR_IE_PRBREQ_FLAG    0x10
#define VNDR_IE_ASSOCREQ_FLAG    0x20
#define VNDR_IE_CUSTOM_FLAG        0x100
#define VNDR_IE_INFO_HDR_LEN    (sizeof(uint32_t) )
struct wl_vndr_ie
{
    uint8_t id;
    uint8_t len;
    uint8_t oui[3];
    uint8_t data[1];
};
typedef struct wl_vndr_ie wl_vndr_ie_t;
typedef struct
{
    uint32_t pktflag;
    wl_vndr_ie_t vndr_ie_data;
} vndr_ie_info_t;
typedef struct
{
    int32_t iecount;
    vndr_ie_info_t vndr_ie_list[1];
} vndr_ie_buf_t;
typedef struct
{
    int8_t cmd[VNDR_IE_CMD_LEN];
    vndr_ie_buf_t vndr_ie_buffer;
} vndr_ie_setbuf_t;
#define WL_JOIN_PREF_RSSI    1
#define WL_JOIN_PREF_WPA    2
#define WL_JOIN_PREF_BAND    3
#define WLJP_BAND_ASSOC_PREF    255
#define WL_WPA_ACP_MCS_ANY    "\x00\x00\x00\x00"
struct tsinfo_arg
{
    uint8_t octets[3];
};
#define NREINITREASONCOUNT      8
/* XXX NREINITREASONCOUNT is 8 in other branches.
 * Any change to this will break wl tool compatibility with other branches
 * #define NREINITREASONCOUNT   WL_REINIT_RC_LAST
 */

#define    NFIFO            6
#define    WL_CNT_T_VERSION    6
#define    WL_CNT_EXT_T_VERSION    1
#define    WL_PHYRATE_LOG_SIZE     1200
typedef struct
{
    uint16_t version;             /**< see definition of WL_CNT_T_VERSION */
    uint16_t datalen;             /**< length of data including all paddings. */
    uint8_t data[1];              /**< variable length payload:
                                   * 1 or more bcm_xtlv_t type of tuples.
                                   * each tuple is padded to multiple of 4 bytes.
                                   * 'datalen' field of this structure includes all paddings.
                                   */
} wl_cnt_info_t;
/** wlc layer counters */
typedef struct
{
    /* transmit stat counters */
    uint32_t txframe;       /**< tx data frames */
    uint32_t txbyte;        /**< tx data bytes */
    uint32_t txretrans;     /**< tx mac retransmits */
    uint32_t txerror;       /**< tx data errors (derived: sum of others) */
    uint32_t txctl;         /**< tx management frames */
    uint32_t txprshort;     /**< tx short preamble frames */
    uint32_t txserr;        /**< tx status errors */
    uint32_t txnobuf;       /**< tx out of buffers errors */
    uint32_t txnoassoc;     /**< tx discard because we're not associated */
    uint32_t txrunt;        /**< tx runt frames */
    uint32_t txchit;        /**< tx header cache hit (fastpath) */
    uint32_t txcmiss;       /**< tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t txuflo;        /**< tx fifo underflows */
    uint32_t txphyerr;      /**< tx phy errors (indicated in tx status) */
    uint32_t txphycrs;      /**< PR8861/8963 counter */

    /* receive stat counters */
    uint32_t rxframe;       /**< rx data frames */
    uint32_t rxbyte;        /**< rx data bytes */
    uint32_t rxerror;       /**< rx data errors (derived: sum of others) */
    uint32_t rxctl;         /**< rx management frames */
    uint32_t rxnobuf;       /**< rx out of buffers errors */
    uint32_t rxnondata;     /**< rx non data frames in the data channel errors */
    uint32_t rxbadds;       /**< rx bad DS errors */
    uint32_t rxbadcm;       /**< rx bad control or management frames */
    uint32_t rxfragerr;     /**< rx fragmentation errors */
    uint32_t rxrunt;        /**< rx runt frames */
    uint32_t rxgiant;       /**< rx giant frames */
    uint32_t rxnoscb;       /**< rx no scb error */
    uint32_t rxbadproto;    /**< rx invalid frames */
    uint32_t rxbadsrcmac;       /**< rx frames with Invalid Src Mac */
    uint32_t rxbadda;       /**< rx frames tossed for invalid da */
    uint32_t rxfilter;      /**< rx frames filtered out */

    /* receive chip error counters */
    uint32_t rxoflo;        /**< rx fifo overflow errors */
    uint32_t rxuflo[NFIFO];     /**< rx dma descriptor underflow errors */

    uint32_t d11cnt_txrts_off;      /**< d11cnt txrts value when reset d11cnt */
    uint32_t d11cnt_rxcrc_off;      /**< d11cnt rxcrc value when reset d11cnt */
    uint32_t d11cnt_txnocts_off;    /**< d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t dmade;         /**< tx/rx dma descriptor errors */
    uint32_t dmada;         /**< tx/rx dma data errors */
    uint32_t dmape;         /**< tx/rx dma descriptor protocol errors */
    uint32_t reset;         /**< reset count */
    uint32_t tbtt;          /**< cnts the TBTT int's */
    uint32_t txdmawar;      /**< # occurrences of PR15420 workaround */
    uint32_t pkt_callback_reg_fail;     /**< callbacks register failure */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t txfrag;        /**< dot11TransmittedFragmentCount */
    uint32_t txmulti;       /**< dot11MulticastTransmittedFrameCount */
    uint32_t txfail;        /**< dot11FailedCount */
    uint32_t txretry;       /**< dot11RetryCount */
    uint32_t txretrie;      /**< dot11MultipleRetryCount */
    uint32_t rxdup;         /**< dot11FrameduplicateCount */
    uint32_t txrts;         /**< dot11RTSSuccessCount */
    uint32_t txnocts;       /**< dot11RTSFailureCount */
    uint32_t txnoack;       /**< dot11ACKFailureCount */
    uint32_t rxfrag;        /**< dot11ReceivedFragmentCount */
    uint32_t rxmulti;       /**< dot11MulticastReceivedFrameCount */
    uint32_t rxcrc;         /**< dot11FCSErrorCount */
    uint32_t txfrmsnt;      /**< dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t rxundec;       /**< dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill;      /**< TKIPLocalMICFailures */
    uint32_t tkipcntrmsr;       /**< TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay;    /**< TKIPReplays */
    uint32_t ccmpfmterr;    /**< CCMPFormatErrors */
    uint32_t ccmpreplay;    /**< CCMPReplays */
    uint32_t ccmpundec;     /**< CCMPDecryptErrors */
    uint32_t fourwayfail;       /**< FourWayHandshakeFailures */
    uint32_t wepundec;      /**< dot11WEPUndecryptableCount */
    uint32_t wepicverr;     /**< dot11WEPICVErrorCount */
    uint32_t decsuccess;    /**< DecryptSuccessCount */
    uint32_t tkipicverr;    /**< TKIPICVErrorCount */
    uint32_t wepexcluded;       /**< dot11WEPExcludedCount */

    uint32_t txchanrej;     /**< Tx frames suppressed due to channel rejection */
    uint32_t psmwds;        /**< Count PSM watchdogs */
    uint32_t phywatchdog;       /**< Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t prq_entries_handled;       /**< PRQ entries read in */
    uint32_t prq_undirected_entries;    /**<    which were bcast bss & ssid */
    uint32_t prq_bad_entries;       /**<    which could not be translated to info */
    uint32_t atim_suppress_count;       /**< TX suppressions on ATIM fifo */
    uint32_t bcn_template_not_ready;    /**< Template marked in use on send bcn ... */
    uint32_t bcn_template_not_ready_done;    /**< ...but "DMA done" interrupt rcvd */
    uint32_t late_tbtt_dpc;     /**< TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t rx1mbps;   /**< packets rx at 1Mbps */
    uint32_t rx2mbps;   /**< packets rx at 2Mbps */
    uint32_t rx5mbps5;  /**< packets rx at 5.5Mbps */
    uint32_t rx6mbps;   /**< packets rx at 6Mbps */
    uint32_t rx9mbps;   /**< packets rx at 9Mbps */
    uint32_t rx11mbps;  /**< packets rx at 11Mbps */
    uint32_t rx12mbps;  /**< packets rx at 12Mbps */
    uint32_t rx18mbps;  /**< packets rx at 18Mbps */
    uint32_t rx24mbps;  /**< packets rx at 24Mbps */
    uint32_t rx36mbps;  /**< packets rx at 36Mbps */
    uint32_t rx48mbps;  /**< packets rx at 48Mbps */
    uint32_t rx54mbps;  /**< packets rx at 54Mbps */
    uint32_t rx108mbps;     /**< packets rx at 108mbps */
    uint32_t rx162mbps;     /**< packets rx at 162mbps */
    uint32_t rx216mbps;     /**< packets rx at 216 mbps */
    uint32_t rx270mbps;     /**< packets rx at 270 mbps */
    uint32_t rx324mbps;     /**< packets rx at 324 mbps */
    uint32_t rx378mbps;     /**< packets rx at 378 mbps */
    uint32_t rx432mbps;     /**< packets rx at 432 mbps */
    uint32_t rx486mbps;     /**< packets rx at 486 mbps */
    uint32_t rx540mbps;     /**< packets rx at 540 mbps */

    uint32_t rfdisable;     /**< count of radio disables */

    uint32_t txexptime;     /**< Tx frames suppressed due to timer expiration */

    uint32_t txmpdu_sgi;    /**< count for sgi transmit */
    uint32_t rxmpdu_sgi;    /**< count for sgi received */
    uint32_t txmpdu_stbc;       /**< count for stbc transmit */
    uint32_t rxmpdu_stbc;       /**< count for stbc received */

    uint32_t rxundec_mcst;      /**< dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill_mcst;     /**< TKIPLocalMICFailures */
    uint32_t tkipcntrmsr_mcst;      /**< TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay_mcst;       /**< TKIPReplays */
    uint32_t ccmpfmterr_mcst;       /**< CCMPFormatErrors */
    uint32_t ccmpreplay_mcst;       /**< CCMPReplays */
    uint32_t ccmpundec_mcst;    /**< CCMPDecryptErrors */
    uint32_t fourwayfail_mcst;      /**< FourWayHandshakeFailures */
    uint32_t wepundec_mcst;     /**< dot11WEPUndecryptableCount */
    uint32_t wepicverr_mcst;    /**< dot11WEPICVErrorCount */
    uint32_t decsuccess_mcst;       /**< DecryptSuccessCount */
    uint32_t tkipicverr_mcst;       /**< TKIPICVErrorCount */
    uint32_t wepexcluded_mcst;      /**< dot11WEPExcludedCount */

    uint32_t dma_hang;      /**< count for dma hang */
    uint32_t reinit;        /**< count for reinit */

    uint32_t pstatxucast;   /**< count of ucast frames xmitted on all psta assoc */
    uint32_t pstatxnoassoc;     /**< count of txnoassoc frames xmitted on all psta assoc */
    uint32_t pstarxucast;   /**< count of ucast frames received on all psta assoc */
    uint32_t pstarxbcmc;    /**< count of bcmc frames received on all psta */
    uint32_t pstatxbcmc;    /**< count of bcmc frames transmitted on all psta */

    uint32_t cso_passthrough;  /**< hw cso required but passthrough */
    uint32_t cso_normal;    /**< hw cso hdr for normal process */
    uint32_t chained;       /**< number of frames chained */
    uint32_t chainedsz1;    /**< number of chain size 1 frames */
    uint32_t unchained;     /**< number of frames not chained */
    uint32_t maxchainsz;    /**< max chain size so far */
    uint32_t currchainsz;       /**< current chain size */
    uint32_t pciereset;     /**< Secondary Bus Reset issued by driver */
    uint32_t cfgrestore;    /**< configspace restore by driver */
    uint32_t reinitreason[NREINITREASONCOUNT];    /**< reinitreason counters; 0: Unknown reason */
    uint32_t rxrtry;
    uint32_t rxmpdu_mu;       /**< Number of MU MPDUs received */

    /* detailed control/management frames */
    uint32_t txbar;         /**< Number of TX BAR */
    uint32_t rxbar;         /**< Number of RX BAR */
    uint32_t txpspoll;      /**< Number of TX PS-poll */
    uint32_t rxpspoll;      /**< Number of RX PS-poll */
    uint32_t txnull;        /**< Number of TX NULL_DATA */
    uint32_t rxnull;        /**< Number of RX NULL_DATA */
    uint32_t txqosnull;     /**< Number of TX NULL_QoSDATA */
    uint32_t rxqosnull;     /**< Number of RX NULL_QoSDATA */
    uint32_t txassocreq;    /**< Number of TX ASSOC request */
    uint32_t rxassocreq;    /**< Number of RX ASSOC request */
    uint32_t txreassocreq;      /**< Number of TX REASSOC request */
    uint32_t rxreassocreq;      /**< Number of RX REASSOC request */
    uint32_t txdisassoc;    /**< Number of TX DISASSOC */
    uint32_t rxdisassoc;    /**< Number of RX DISASSOC */
    uint32_t txassocrsp;    /**< Number of TX ASSOC response */
    uint32_t rxassocrsp;    /**< Number of RX ASSOC response */
    uint32_t txreassocrsp;      /**< Number of TX REASSOC response */
    uint32_t rxreassocrsp;      /**< Number of RX REASSOC response */
    uint32_t txauth;        /**< Number of TX AUTH */
    uint32_t rxauth;        /**< Number of RX AUTH */
    uint32_t txdeauth;      /**< Number of TX DEAUTH */
    uint32_t rxdeauth;      /**< Number of RX DEAUTH */
    uint32_t txprobereq;    /**< Number of TX probe request */
    uint32_t rxprobereq;    /**< Number of RX probe request */
    uint32_t txprobersp;    /**< Number of TX probe response */
    uint32_t rxprobersp;    /**< Number of RX probe response */
    uint32_t txaction;      /**< Number of TX action frame */
    uint32_t rxaction;      /**< Number of RX action frame */
    uint32_t ampdu_wds;     /**< Number of AMPDU watchdogs */
    uint32_t txlost;        /**< Number of lost packets reported in txs */
    uint32_t txdatamcast;       /**< Number of TX multicast data packets */
    uint32_t txdatabcast;       /**< Number of TX broadcast data packets */
    uint32_t psmxwds;       /**< Number of PSMx watchdogs */
    uint32_t rxback;
    uint32_t txback;
    uint32_t p2p_tbtt;  /**< Number of P2P TBTT Events */
    uint32_t p2p_tbtt_miss;     /**< Number of P2P TBTT Events Miss */
    uint32_t txqueue_start;
    uint32_t txqueue_end;
    uint32_t txbcast;         /* Broadcast TransmittedFrameCount */
    uint32_t txdropped;       /* tx dropped pkts */
    uint32_t rxbcast;         /* BroadcastReceivedFrameCount */
    uint32_t rxdropped;       /* rx dropped pkts (derived: sum of others) */
    /* XXX: Do not remove or rename inthe middle of this struct.
     * All counter variables have to be of uint32_t.
     * Please follow the instruction in
     * http://hwnbu-twiki.sj.broadcom.com/bin/view/Mwgroup/WlCounters#Counter_Edition
     */
} wl_cnt_ver_30_t;
typedef struct
{
    uint16_t version;     /* see definition of WL_CNT_T_VERSION */
    uint16_t length;     /* length of entire structure */

    /* transmit stat counters */
    uint32_t txframe;     /* tx data frames */
    uint32_t txbyte;     /* tx data bytes */
    uint32_t txretrans;     /* tx mac retransmits */
    uint32_t txerror;     /* tx data errors (derived: sum of others) */
    uint32_t txctl;     /* tx management frames */
    uint32_t txprshort;     /* tx short preamble frames */
    uint32_t txserr;     /* tx status errors */
    uint32_t txnobuf;     /* tx out of buffers errors */
    uint32_t txnoassoc;     /* tx discard because we're not associated */
    uint32_t txrunt;     /* tx runt frames */
    uint32_t txchit;     /* tx header cache hit (fastpath) */
    uint32_t txcmiss;     /* tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t txuflo;     /* tx fifo underflows */
    uint32_t txphyerr;     /* tx phy errors (indicated in tx status) */
    uint32_t txphycrs;     /* PR8861/8963 counter */

    /* receive stat counters */
    uint32_t rxframe;     /* rx data frames */
    uint32_t rxbyte;     /* rx data bytes */
    uint32_t rxerror;     /* rx data errors (derived: sum of others) */
    uint32_t rxctl;     /* rx management frames */
    uint32_t rxnobuf;     /* rx out of buffers errors */
    uint32_t rxnondata;     /* rx non data frames in the data channel errors */
    uint32_t rxbadds;     /* rx bad DS errors */
    uint32_t rxbadcm;     /* rx bad control or management frames */
    uint32_t rxfragerr;     /* rx fragmentation errors */
    uint32_t rxrunt;     /* rx runt frames */
    uint32_t rxgiant;     /* rx giant frames */
    uint32_t rxnoscb;     /* rx no scb error */
    uint32_t rxbadproto;     /* rx invalid frames */
    uint32_t rxbadsrcmac;     /* rx frames with Invalid Src Mac */
    uint32_t rxbadda;     /* rx frames tossed for invalid da */
    uint32_t rxfilter;     /* rx frames filtered out */

    /* receive chip error counters */
    uint32_t rxoflo;     /* rx fifo overflow errors */
    uint32_t rxuflo[NFIFO];     /* rx dma descriptor underflow errors */

    uint32_t d11cnt_txrts_off;     /* d11cnt txrts value when reset d11cnt */
    uint32_t d11cnt_rxcrc_off;     /* d11cnt rxcrc value when reset d11cnt */
    uint32_t d11cnt_txnocts_off;     /* d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t dmade;     /* tx/rx dma descriptor errors */
    uint32_t dmada;     /* tx/rx dma data errors */
    uint32_t dmape;     /* tx/rx dma descriptor protocol errors */
    uint32_t reset;     /* reset count */
    uint32_t tbtt;     /* cnts the TBTT int's */
    uint32_t txdmawar;     /* # occurrences of PR15420 workaround */
    uint32_t pkt_callback_reg_fail;     /* callbacks register failure */

    /* MAC counters: 32-bit version of d11.h's macstat_t */
    uint32_t txallfrm;     /* total number of frames sent, incl. Data, ACK, RTS, CTS,
                            * Control Management (includes retransmissions)
                            */
    uint32_t txrtsfrm;     /* number of RTS sent out by the MAC */
    uint32_t txctsfrm;     /* number of CTS sent out by the MAC */
    uint32_t txackfrm;     /* number of ACK frames sent out */
    uint32_t txdnlfrm;     /* Not used */
    uint32_t txbcnfrm;     /* beacons transmitted */
    uint32_t txfunfl[8];     /* per-fifo tx underflows */
    uint32_t txtplunfl;     /* Template underflows (mac was too slow to transmit ACK/CTS
                             * or BCN)
                             */
    uint32_t txphyerror;     /* Transmit phy error, type of error is reported in tx-status for
                              * driver enqueued frames
                              */
    uint32_t rxfrmtoolong;     /* Received frame longer than legal limit (2346 bytes) */
    uint32_t rxfrmtooshrt;     /* Received frame did not contain enough bytes for its frame type */
    uint32_t rxinvmachdr;     /* Either the protocol version != 0 or frame type not
                               * data/control/management
                               */
    uint32_t rxbadfcs;     /* number of frames for which the CRC check failed in the MAC */
    uint32_t rxbadplcp;     /* parity check of the PLCP header failed */
    uint32_t rxcrsglitch;     /* PHY was able to correlate the preamble but not the header */
    uint32_t rxstrt;     /* Number of received frames with a good PLCP
                          * (i.e. passing parity check)
                          */
    uint32_t rxdfrmucastmbss;     /* Number of received DATA frames with good FCS and matching RA */
    uint32_t rxmfrmucastmbss;     /* number of received mgmt frames with good FCS and matching RA */
    uint32_t rxcfrmucast;     /* number of received CNTRL frames with good FCS and matching RA */
    uint32_t rxrtsucast;     /* number of unicast RTS addressed to the MAC (good FCS) */
    uint32_t rxctsucast;     /* number of unicast CTS addressed to the MAC (good FCS) */
    uint32_t rxackucast;     /* number of ucast ACKS received (good FCS) */
    uint32_t rxdfrmocast;     /* number of received DATA frames (good FCS and not matching RA) */
    uint32_t rxmfrmocast;     /* number of received MGMT frames (good FCS and not matching RA) */
    uint32_t rxcfrmocast;     /* number of received CNTRL frame (good FCS and not matching RA) */
    uint32_t rxrtsocast;     /* number of received RTS not addressed to the MAC */
    uint32_t rxctsocast;     /* number of received CTS not addressed to the MAC */
    uint32_t rxdfrmmcast;     /* number of RX Data multicast frames received by the MAC */
    uint32_t rxmfrmmcast;     /* number of RX Management multicast frames received by the MAC */
    uint32_t rxcfrmmcast;     /* number of RX Control multicast frames received by the MAC
                               * (unlikely to see these)
                               */
    uint32_t rxbeaconmbss;     /* beacons received from member of BSS */
    uint32_t rxdfrmucastobss;     /* number of unicast frames addressed to the MAC from
                                   * other BSS (WDS FRAME)
                                   */
    uint32_t rxbeaconobss;     /* beacons received from other BSS */
    uint32_t rxrsptmout;     /* Number of response timeouts for transmitted frames
                              * expecting a response
                              */
    uint32_t bcntxcancl;     /* transmit beacons canceled due to receipt of beacon (IBSS) */
    uint32_t rxf0ovfl;     /* Number of receive fifo 0 overflows */
    uint32_t rxf1ovfl;     /* Number of receive fifo 1 overflows (obsolete) */
    uint32_t rxf2ovfl;     /* Number of receive fifo 2 overflows (obsolete) */
    uint32_t txsfovfl;     /* Number of transmit status fifo overflows (obsolete) */
    uint32_t pmqovfl;     /* Number of PMQ overflows */
    uint32_t rxcgprqfrm;     /* Number of received Probe requests that made it into
                              * the PRQ fifo
                              */
    uint32_t rxcgprsqovfl;     /* Rx Probe Request Que overflow in the AP */
    uint32_t txcgprsfail;     /* Tx Probe Response Fail. AP sent probe response but did
                               * not get ACK
                               */
    uint32_t txcgprssuc;     /* Tx Probe Response Success (ACK was received) */
    uint32_t prs_timeout;     /* Number of probe requests that were dropped from the PRQ
                               * fifo because a probe response could not be sent out within
                               * the time limit defined in M_PRS_MAXTIME
                               */
    uint32_t rxnack;     /* XXX Number of NACKS received (Afterburner) */
    uint32_t frmscons;     /* XXX Number of frames completed without transmission because of an
                            * Afterburner re-queue
                            */
    uint32_t txnack;     /* XXX Number of NACKs transmitted (Afterburner) */
    uint32_t txglitch_nack;     /* obsolete */
    uint32_t txburst;     /* obsolete */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t txfrag;     /* dot11TransmittedFragmentCount */
    uint32_t txmulti;     /* dot11MulticastTransmittedFrameCount */
    uint32_t txfail;     /* dot11FailedCount */
    uint32_t txretry;     /* dot11RetryCount */
    uint32_t txretrie;     /* dot11MultipleRetryCount */
    uint32_t rxdup;     /* dot11FrameduplicateCount */
    uint32_t txrts;     /* dot11RTSSuccessCount */
    uint32_t txnocts;     /* dot11RTSFailureCount */
    uint32_t txnoack;     /* dot11ACKFailureCount */
    uint32_t rxfrag;     /* dot11ReceivedFragmentCount */
    uint32_t rxmulti;     /* dot11MulticastReceivedFrameCount */
    uint32_t rxcrc;     /* dot11FCSErrorCount */
    uint32_t txfrmsnt;     /* dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t rxundec;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay;     /* TKIPReplays */
    uint32_t ccmpfmterr;     /* CCMPFormatErrors */
    uint32_t ccmpreplay;     /* CCMPReplays */
    uint32_t ccmpundec;     /* CCMPDecryptErrors */
    uint32_t fourwayfail;     /* FourWayHandshakeFailures */
    uint32_t wepundec;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess;     /* DecryptSuccessCount */
    uint32_t tkipicverr;     /* TKIPICVErrorCount */
    uint32_t wepexcluded;     /* dot11WEPExcludedCount */

    uint32_t rxundec_mcst;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill_mcst;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr_mcst;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay_mcst;     /* TKIPReplays */
    uint32_t ccmpfmterr_mcst;     /* CCMPFormatErrors */
    uint32_t ccmpreplay_mcst;     /* CCMPReplays */
    uint32_t ccmpundec_mcst;     /* CCMPDecryptErrors */
    uint32_t fourwayfail_mcst;     /* FourWayHandshakeFailures */
    uint32_t wepundec_mcst;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr_mcst;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess_mcst;     /* DecryptSuccessCount */
    uint32_t tkipicverr_mcst;     /* TKIPICVErrorCount */
    uint32_t wepexcluded_mcst;     /* dot11WEPExcludedCount */

    uint32_t txchanrej;     /* Tx frames suppressed due to channel rejection */
    uint32_t txexptime;     /* Tx frames suppressed due to timer expiration */
    uint32_t psmwds;     /* Count PSM watchdogs */
    uint32_t phywatchdog;     /* Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t prq_entries_handled;     /* PRQ entries read in */
    uint32_t prq_undirected_entries;     /*    which were bcast bss & ssid */
    uint32_t prq_bad_entries;     /*    which could not be translated to info */
    uint32_t atim_suppress_count;     /* TX suppressions on ATIM fifo */
    uint32_t bcn_template_not_ready;     /* Template marked in use on send bcn ... */
    uint32_t bcn_template_not_ready_done;     /* ...but "DMA done" interrupt rcvd */
    uint32_t late_tbtt_dpc;     /* TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t rx1mbps;     /* packets rx at 1Mbps */
    uint32_t rx2mbps;     /* packets rx at 2Mbps */
    uint32_t rx5mbps5;     /* packets rx at 5.5Mbps */
    uint32_t rx6mbps;     /* packets rx at 6Mbps */
    uint32_t rx9mbps;     /* packets rx at 9Mbps */
    uint32_t rx11mbps;     /* packets rx at 11Mbps */
    uint32_t rx12mbps;     /* packets rx at 12Mbps */
    uint32_t rx18mbps;     /* packets rx at 18Mbps */
    uint32_t rx24mbps;     /* packets rx at 24Mbps */
    uint32_t rx36mbps;     /* packets rx at 36Mbps */
    uint32_t rx48mbps;     /* packets rx at 48Mbps */
    uint32_t rx54mbps;     /* packets rx at 54Mbps */
    uint32_t rx108mbps;     /* packets rx at 108mbps */
    uint32_t rx162mbps;     /* packets rx at 162mbps */
    uint32_t rx216mbps;     /* packets rx at 216 mbps */
    uint32_t rx270mbps;     /* packets rx at 270 mbps */
    uint32_t rx324mbps;     /* packets rx at 324 mbps */
    uint32_t rx378mbps;     /* packets rx at 378 mbps */
    uint32_t rx432mbps;     /* packets rx at 432 mbps */
    uint32_t rx486mbps;     /* packets rx at 486 mbps */
    uint32_t rx540mbps;     /* packets rx at 540 mbps */

    /* pkteng rx frame stats */
    uint32_t pktengrxducast;     /* unicast frames rxed by the pkteng code */
    uint32_t pktengrxdmcast;     /* multicast frames rxed by the pkteng code */

    uint32_t rfdisable;     /* count of radio disables */
    uint32_t bphy_rxcrsglitch;     /* PHY count of bphy glitches */

    uint32_t txmpdu_sgi;     /* count for sgi transmit */
    uint32_t rxmpdu_sgi;     /* count for sgi received */
    uint32_t txmpdu_stbc;     /* count for stbc transmit */
    uint32_t rxmpdu_stbc;     /* count for stbc received */
} wl_cnt_ver_six_t;

typedef struct
{
    uint16_t version;     /* see definition of WL_CNT_T_VERSION */
    uint16_t length;     /* length of entire structure */

    /* transmit stat counters */
    uint32_t txframe;     /* tx data frames */
    uint32_t txbyte;     /* tx data bytes */
    uint32_t txretrans;     /* tx mac retransmits */
    uint32_t txerror;     /* tx data errors (derived: sum of others) */
    uint32_t txctl;     /* tx management frames */
    uint32_t txprshort;     /* tx short preamble frames */
    uint32_t txserr;     /* tx status errors */
    uint32_t txnobuf;     /* tx out of buffers errors */
    uint32_t txnoassoc;     /* tx discard because we're not associated */
    uint32_t txrunt;     /* tx runt frames */
    uint32_t txchit;     /* tx header cache hit (fastpath) */
    uint32_t txcmiss;     /* tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t txuflo;     /* tx fifo underflows */
    uint32_t txphyerr;     /* tx phy errors (indicated in tx status) */
    uint32_t txphycrs;     /* PR8861/8963 counter */

    /* receive stat counters */
    uint32_t rxframe;     /* rx data frames */
    uint32_t rxbyte;     /* rx data bytes */
    uint32_t rxerror;     /* rx data errors (derived: sum of others) */
    uint32_t rxctl;     /* rx management frames */
    uint32_t rxnobuf;     /* rx out of buffers errors */
    uint32_t rxnondata;     /* rx non data frames in the data channel errors */
    uint32_t rxbadds;     /* rx bad DS errors */
    uint32_t rxbadcm;     /* rx bad control or management frames */
    uint32_t rxfragerr;     /* rx fragmentation errors */
    uint32_t rxrunt;     /* rx runt frames */
    uint32_t rxgiant;     /* rx giant frames */
    uint32_t rxnoscb;     /* rx no scb error */
    uint32_t rxbadproto;     /* rx invalid frames */
    uint32_t rxbadsrcmac;     /* rx frames with Invalid Src Mac */
    uint32_t rxbadda;     /* rx frames tossed for invalid da */
    uint32_t rxfilter;     /* rx frames filtered out */

    /* receive chip error counters */
    uint32_t rxoflo;     /* rx fifo overflow errors */
    uint32_t rxuflo[NFIFO];     /* rx dma descriptor underflow errors */

    uint32_t d11cnt_txrts_off;     /* d11cnt txrts value when reset d11cnt */
    uint32_t d11cnt_rxcrc_off;     /* d11cnt rxcrc value when reset d11cnt */
    uint32_t d11cnt_txnocts_off;     /* d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t dmade;     /* tx/rx dma descriptor errors */
    uint32_t dmada;     /* tx/rx dma data errors */
    uint32_t dmape;     /* tx/rx dma descriptor protocol errors */
    uint32_t reset;     /* reset count */
    uint32_t tbtt;     /* cnts the TBTT int's */
    uint32_t txdmawar;     /* # occurrences of PR15420 workaround */
    uint32_t pkt_callback_reg_fail;     /* callbacks register failure */

    /* MAC counters: 32-bit version of d11.h's macstat_t */
    uint32_t txallfrm;     /* total number of frames sent, incl. Data, ACK, RTS, CTS,
                            * Control Management (includes retransmissions)
                            */
    uint32_t txrtsfrm;     /* number of RTS sent out by the MAC */
    uint32_t txctsfrm;     /* number of CTS sent out by the MAC */
    uint32_t txackfrm;     /* number of ACK frames sent out */
    uint32_t txdnlfrm;     /* Not used */
    uint32_t txbcnfrm;     /* beacons transmitted */
    uint32_t txfunfl[8];     /* per-fifo tx underflows */
    uint32_t txtplunfl;     /* Template underflows (mac was too slow to transmit ACK/CTS
                             * or BCN)
                             */
    uint32_t txphyerror;     /* Transmit phy error, type of error is reported in tx-status for
                              * driver enqueued frames
                              */
    uint32_t rxfrmtoolong;     /* Received frame longer than legal limit (2346 bytes) */
    uint32_t rxfrmtooshrt;     /* Received frame did not contain enough bytes for its frame type */
    uint32_t rxinvmachdr;     /* Either the protocol version != 0 or frame type not
                               * data/control/management
                               */
    uint32_t rxbadfcs;     /* number of frames for which the CRC check failed in the MAC */
    uint32_t rxbadplcp;     /* parity check of the PLCP header failed */
    uint32_t rxcrsglitch;     /* PHY was able to correlate the preamble but not the header */
    uint32_t rxstrt;     /* Number of received frames with a good PLCP
                          * (i.e. passing parity check)
                          */
    uint32_t rxdfrmucastmbss;     /* Number of received DATA frames with good FCS and matching RA */
    uint32_t rxmfrmucastmbss;     /* number of received mgmt frames with good FCS and matching RA */
    uint32_t rxcfrmucast;     /* number of received CNTRL frames with good FCS and matching RA */
    uint32_t rxrtsucast;     /* number of unicast RTS addressed to the MAC (good FCS) */
    uint32_t rxctsucast;     /* number of unicast CTS addressed to the MAC (good FCS) */
    uint32_t rxackucast;     /* number of ucast ACKS received (good FCS) */
    uint32_t rxdfrmocast;     /* number of received DATA frames (good FCS and not matching RA) */
    uint32_t rxmfrmocast;     /* number of received MGMT frames (good FCS and not matching RA) */
    uint32_t rxcfrmocast;     /* number of received CNTRL frame (good FCS and not matching RA) */
    uint32_t rxrtsocast;     /* number of received RTS not addressed to the MAC */
    uint32_t rxctsocast;     /* number of received CTS not addressed to the MAC */
    uint32_t rxdfrmmcast;     /* number of RX Data multicast frames received by the MAC */
    uint32_t rxmfrmmcast;     /* number of RX Management multicast frames received by the MAC */
    uint32_t rxcfrmmcast;     /* number of RX Control multicast frames received by the MAC
                               * (unlikely to see these)
                               */
    uint32_t rxbeaconmbss;     /* beacons received from member of BSS */
    uint32_t rxdfrmucastobss;     /* number of unicast frames addressed to the MAC from
                                   * other BSS (WDS FRAME)
                                   */
    uint32_t rxbeaconobss;     /* beacons received from other BSS */
    uint32_t rxrsptmout;     /* Number of response timeouts for transmitted frames
                              * expecting a response
                              */
    uint32_t bcntxcancl;     /* transmit beacons canceled due to receipt of beacon (IBSS) */
    uint32_t rxf0ovfl;     /* Number of receive fifo 0 overflows */
    uint32_t rxf1ovfl;     /* Number of receive fifo 1 overflows (obsolete) */
    uint32_t rxf2ovfl;     /* Number of receive fifo 2 overflows (obsolete) */
    uint32_t txsfovfl;     /* Number of transmit status fifo overflows (obsolete) */
    uint32_t pmqovfl;     /* Number of PMQ overflows */
    uint32_t rxcgprqfrm;     /* Number of received Probe requests that made it into
                              * the PRQ fifo
                              */
    uint32_t rxcgprsqovfl;     /* Rx Probe Request Que overflow in the AP */
    uint32_t txcgprsfail;     /* Tx Probe Response Fail. AP sent probe response but did
                               * not get ACK
                               */
    uint32_t txcgprssuc;     /* Tx Probe Response Success (ACK was received) */
    uint32_t prs_timeout;     /* Number of probe requests that were dropped from the PRQ
                               * fifo because a probe response could not be sent out within
                               * the time limit defined in M_PRS_MAXTIME
                               */
    uint32_t rxnack;     /* obsolete */
    uint32_t frmscons;     /* obsolete */
    uint32_t txnack;     /* obsolete */
    uint32_t txglitch_nack;     /* obsolete */
    uint32_t txburst;     /* obsolete */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t txfrag;     /* dot11TransmittedFragmentCount */
    uint32_t txmulti;     /* dot11MulticastTransmittedFrameCount */
    uint32_t txfail;     /* dot11FailedCount */
    uint32_t txretry;     /* dot11RetryCount */
    uint32_t txretrie;     /* dot11MultipleRetryCount */
    uint32_t rxdup;     /* dot11FrameduplicateCount */
    uint32_t txrts;     /* dot11RTSSuccessCount */
    uint32_t txnocts;     /* dot11RTSFailureCount */
    uint32_t txnoack;     /* dot11ACKFailureCount */
    uint32_t rxfrag;     /* dot11ReceivedFragmentCount */
    uint32_t rxmulti;     /* dot11MulticastReceivedFrameCount */
    uint32_t rxcrc;     /* dot11FCSErrorCount */
    uint32_t txfrmsnt;     /* dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t rxundec;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay;     /* TKIPReplays */
    uint32_t ccmpfmterr;     /* CCMPFormatErrors */
    uint32_t ccmpreplay;     /* CCMPReplays */
    uint32_t ccmpundec;     /* CCMPDecryptErrors */
    uint32_t fourwayfail;     /* FourWayHandshakeFailures */
    uint32_t wepundec;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess;     /* DecryptSuccessCount */
    uint32_t tkipicverr;     /* TKIPICVErrorCount */
    uint32_t wepexcluded;     /* dot11WEPExcludedCount */

    uint32_t txchanrej;     /* Tx frames suppressed due to channel rejection */
    uint32_t psmwds;     /* Count PSM watchdogs */
    uint32_t phywatchdog;     /* Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t prq_entries_handled;     /* PRQ entries read in */
    uint32_t prq_undirected_entries;     /*    which were bcast bss & ssid */
    uint32_t prq_bad_entries;     /*    which could not be translated to info */
    uint32_t atim_suppress_count;     /* TX suppressions on ATIM fifo */
    uint32_t bcn_template_not_ready;     /* Template marked in use on send bcn ... */
    uint32_t bcn_template_not_ready_done;     /* ...but "DMA done" interrupt rcvd */
    uint32_t late_tbtt_dpc;     /* TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t rx1mbps;     /* packets rx at 1Mbps */
    uint32_t rx2mbps;     /* packets rx at 2Mbps */
    uint32_t rx5mbps5;     /* packets rx at 5.5Mbps */
    uint32_t rx6mbps;     /* packets rx at 6Mbps */
    uint32_t rx9mbps;     /* packets rx at 9Mbps */
    uint32_t rx11mbps;     /* packets rx at 11Mbps */
    uint32_t rx12mbps;     /* packets rx at 12Mbps */
    uint32_t rx18mbps;     /* packets rx at 18Mbps */
    uint32_t rx24mbps;     /* packets rx at 24Mbps */
    uint32_t rx36mbps;     /* packets rx at 36Mbps */
    uint32_t rx48mbps;     /* packets rx at 48Mbps */
    uint32_t rx54mbps;     /* packets rx at 54Mbps */
    uint32_t rx108mbps;     /* packets rx at 108mbps */
    uint32_t rx162mbps;     /* packets rx at 162mbps */
    uint32_t rx216mbps;     /* packets rx at 216 mbps */
    uint32_t rx270mbps;     /* packets rx at 270 mbps */
    uint32_t rx324mbps;     /* packets rx at 324 mbps */
    uint32_t rx378mbps;     /* packets rx at 378 mbps */
    uint32_t rx432mbps;     /* packets rx at 432 mbps */
    uint32_t rx486mbps;     /* packets rx at 486 mbps */
    uint32_t rx540mbps;     /* packets rx at 540 mbps */

    /* pkteng rx frame stats */
    uint32_t pktengrxducast;     /* unicast frames rxed by the pkteng code */
    uint32_t pktengrxdmcast;     /* multicast frames rxed by the pkteng code */

    uint32_t rfdisable;     /* count of radio disables */
    uint32_t bphy_rxcrsglitch;     /* PHY count of bphy glitches */

    uint32_t txexptime;     /* Tx frames suppressed due to timer expiration */

    uint32_t txmpdu_sgi;     /* count for sgi transmit */
    uint32_t rxmpdu_sgi;     /* count for sgi received */
    uint32_t txmpdu_stbc;     /* count for stbc transmit */
    uint32_t rxmpdu_stbc;     /* count for stbc received */

    uint32_t rxundec_mcst;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill_mcst;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr_mcst;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay_mcst;     /* TKIPReplays */
    uint32_t ccmpfmterr_mcst;     /* CCMPFormatErrors */
    uint32_t ccmpreplay_mcst;     /* CCMPReplays */
    uint32_t ccmpundec_mcst;     /* CCMPDecryptErrors */
    uint32_t fourwayfail_mcst;     /* FourWayHandshakeFailures */
    uint32_t wepundec_mcst;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr_mcst;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess_mcst;     /* DecryptSuccessCount */
    uint32_t tkipicverr_mcst;     /* TKIPICVErrorCount */
    uint32_t wepexcluded_mcst;     /* dot11WEPExcludedCount */

    uint32_t dma_hang;     /* count for stbc received */
} wl_cnt_ver_seven_t;

typedef struct
{
    uint16_t version;     /* see definition of WL_CNT_T_VERSION */
    uint16_t length;     /* length of entire structure */

    /* transmit stat counters */
    uint32_t txframe;     /* tx data frames */
    uint32_t txbyte;     /* tx data bytes */
    uint32_t txretrans;     /* tx mac retransmits */
    uint32_t txerror;     /* tx data errors (derived: sum of others) */
    uint32_t txctl;     /* tx management frames */
    uint32_t txprshort;     /* tx short preamble frames */
    uint32_t txserr;     /* tx status errors */
    uint32_t txnobuf;     /* tx out of buffers errors */
    uint32_t txnoassoc;     /* tx discard because we're not associated */
    uint32_t txrunt;     /* tx runt frames */
    uint32_t txchit;     /* tx header cache hit (fastpath) */
    uint32_t txcmiss;     /* tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t txuflo;     /* tx fifo underflows */
    uint32_t txphyerr;     /* tx phy errors (indicated in tx status) */
    uint32_t txphycrs;     /* PR8861/8963 counter */

    /* receive stat counters */
    uint32_t rxframe;     /* rx data frames */
    uint32_t rxbyte;     /* rx data bytes */
    uint32_t rxerror;     /* rx data errors (derived: sum of others) */
    uint32_t rxctl;     /* rx management frames */
    uint32_t rxnobuf;     /* rx out of buffers errors */
    uint32_t rxnondata;     /* rx non data frames in the data channel errors */
    uint32_t rxbadds;     /* rx bad DS errors */
    uint32_t rxbadcm;     /* rx bad control or management frames */
    uint32_t rxfragerr;     /* rx fragmentation errors */
    uint32_t rxrunt;     /* rx runt frames */
    uint32_t rxgiant;     /* rx giant frames */
    uint32_t rxnoscb;     /* rx no scb error */
    uint32_t rxbadproto;     /* rx invalid frames */
    uint32_t rxbadsrcmac;     /* rx frames with Invalid Src Mac */
    uint32_t rxbadda;     /* rx frames tossed for invalid da */
    uint32_t rxfilter;     /* rx frames filtered out */

    /* receive chip error counters */
    uint32_t rxoflo;     /* rx fifo overflow errors */
    uint32_t rxuflo[NFIFO];     /* rx dma descriptor underflow errors */

    uint32_t d11cnt_txrts_off;     /* d11cnt txrts value when reset d11cnt */
    uint32_t d11cnt_rxcrc_off;     /* d11cnt rxcrc value when reset d11cnt */
    uint32_t d11cnt_txnocts_off;     /* d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t dmade;     /* tx/rx dma descriptor errors */
    uint32_t dmada;     /* tx/rx dma data errors */
    uint32_t dmape;     /* tx/rx dma descriptor protocol errors */
    uint32_t reset;     /* reset count */
    uint32_t tbtt;     /* cnts the TBTT int's */
    uint32_t txdmawar;     /* # occurrences of PR15420 workaround */
    uint32_t pkt_callback_reg_fail;     /* callbacks register failure */

    /* MAC counters: 32-bit version of d11.h's macstat_t */
    uint32_t txallfrm;     /* total number of frames sent, incl. Data, ACK, RTS, CTS,
                            * Control Management (includes retransmissions)
                            */
    uint32_t txrtsfrm;     /* number of RTS sent out by the MAC */
    uint32_t txctsfrm;     /* number of CTS sent out by the MAC */
    uint32_t txackfrm;     /* number of ACK frames sent out */
    uint32_t txdnlfrm;     /* Not used */
    uint32_t txbcnfrm;     /* beacons transmitted */
    uint32_t txfunfl[6];     /* per-fifo tx underflows */
    uint32_t rxtoolate;     /* receive too late */
    uint32_t txfbw;     /* transmit at fallback bw (dynamic bw) */
    uint32_t txtplunfl;     /* Template underflows (mac was too slow to transmit ACK/CTS
                             * or BCN)
                             */
    uint32_t txphyerror;     /* Transmit phy error, type of error is reported in tx-status for
                              * driver enqueued frames
                              */
    uint32_t rxfrmtoolong;     /* Received frame longer than legal limit (2346 bytes) */
    uint32_t rxfrmtooshrt;     /* Received frame did not contain enough bytes for its frame type */
    uint32_t rxinvmachdr;     /* Either the protocol version != 0 or frame type not
                               * data/control/management
                               */
    uint32_t rxbadfcs;     /* number of frames for which the CRC check failed in the MAC */
    uint32_t rxbadplcp;     /* parity check of the PLCP header failed */
    uint32_t rxcrsglitch;     /* PHY was able to correlate the preamble but not the header */
    uint32_t rxstrt;     /* Number of received frames with a good PLCP
                          * (i.e. passing parity check)
                          */
    uint32_t rxdfrmucastmbss;     /* Number of received DATA frames with good FCS and matching RA */
    uint32_t rxmfrmucastmbss;     /* number of received mgmt frames with good FCS and matching RA */
    uint32_t rxcfrmucast;     /* number of received CNTRL frames with good FCS and matching RA */
    uint32_t rxrtsucast;     /* number of unicast RTS addressed to the MAC (good FCS) */
    uint32_t rxctsucast;     /* number of unicast CTS addressed to the MAC (good FCS) */
    uint32_t rxackucast;     /* number of ucast ACKS received (good FCS) */
    uint32_t rxdfrmocast;     /* number of received DATA frames (good FCS and not matching RA) */
    uint32_t rxmfrmocast;     /* number of received MGMT frames (good FCS and not matching RA) */
    uint32_t rxcfrmocast;     /* number of received CNTRL frame (good FCS and not matching RA) */
    uint32_t rxrtsocast;     /* number of received RTS not addressed to the MAC */
    uint32_t rxctsocast;     /* number of received CTS not addressed to the MAC */
    uint32_t rxdfrmmcast;     /* number of RX Data multicast frames received by the MAC */
    uint32_t rxmfrmmcast;     /* number of RX Management multicast frames received by the MAC */
    uint32_t rxcfrmmcast;     /* number of RX Control multicast frames received by the MAC
                               * (unlikely to see these)
                               */
    uint32_t rxbeaconmbss;     /* beacons received from member of BSS */
    uint32_t rxdfrmucastobss;     /* number of unicast frames addressed to the MAC from
                                   * other BSS (WDS FRAME)
                                   */
    uint32_t rxbeaconobss;     /* beacons received from other BSS */
    uint32_t rxrsptmout;     /* Number of response timeouts for transmitted frames
                              * expecting a response
                              */
    uint32_t bcntxcancl;     /* transmit beacons canceled due to receipt of beacon (IBSS) */
    uint32_t rxf0ovfl;     /* Number of receive fifo 0 overflows */
    uint32_t rxf1ovfl;     /* Number of receive fifo 1 overflows (obsolete) */
    uint32_t rxf2ovfl;     /* Number of receive fifo 2 overflows (obsolete) */
    uint32_t txsfovfl;     /* Number of transmit status fifo overflows (obsolete) */
    uint32_t pmqovfl;     /* Number of PMQ overflows */
    uint32_t rxcgprqfrm;     /* Number of received Probe requests that made it into
                              * the PRQ fifo
                              */
    uint32_t rxcgprsqovfl;     /* Rx Probe Request Que overflow in the AP */
    uint32_t txcgprsfail;     /* Tx Probe Response Fail. AP sent probe response but did
                               * not get ACK
                               */
    uint32_t txcgprssuc;     /* Tx Probe Response Success (ACK was received) */
    uint32_t prs_timeout;     /* Number of probe requests that were dropped from the PRQ
                               * fifo because a probe response could not be sent out within
                               * the time limit defined in M_PRS_MAXTIME
                               */
    uint32_t rxnack;     /* obsolete */
    uint32_t frmscons;     /* obsolete */
    uint32_t txnack;     /* obsolete */
    uint32_t rxback;     /* blockack rxcnt */
    uint32_t txback;     /* blockack txcnt */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t txfrag;     /* dot11TransmittedFragmentCount */
    uint32_t txmulti;     /* dot11MulticastTransmittedFrameCount */
    uint32_t txfail;     /* dot11FailedCount */
    uint32_t txretry;     /* dot11RetryCount */
    uint32_t txretrie;     /* dot11MultipleRetryCount */
    uint32_t rxdup;     /* dot11FrameduplicateCount */
    uint32_t txrts;     /* dot11RTSSuccessCount */
    uint32_t txnocts;     /* dot11RTSFailureCount */
    uint32_t txnoack;     /* dot11ACKFailureCount */
    uint32_t rxfrag;     /* dot11ReceivedFragmentCount */
    uint32_t rxmulti;     /* dot11MulticastReceivedFrameCount */
    uint32_t rxcrc;     /* dot11FCSErrorCount */
    uint32_t txfrmsnt;     /* dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t rxundec;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay;     /* TKIPReplays */
    uint32_t ccmpfmterr;     /* CCMPFormatErrors */
    uint32_t ccmpreplay;     /* CCMPReplays */
    uint32_t ccmpundec;     /* CCMPDecryptErrors */
    uint32_t fourwayfail;     /* FourWayHandshakeFailures */
    uint32_t wepundec;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess;     /* DecryptSuccessCount */
    uint32_t tkipicverr;     /* TKIPICVErrorCount */
    uint32_t wepexcluded;     /* dot11WEPExcludedCount */

    uint32_t txchanrej;     /* Tx frames suppressed due to channel rejection */
    uint32_t psmwds;     /* Count PSM watchdogs */
    uint32_t phywatchdog;     /* Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t prq_entries_handled;     /* PRQ entries read in */
    uint32_t prq_undirected_entries;     /*    which were bcast bss & ssid */
    uint32_t prq_bad_entries;     /*    which could not be translated to info */
    uint32_t atim_suppress_count;     /* TX suppressions on ATIM fifo */
    uint32_t bcn_template_not_ready;     /* Template marked in use on send bcn ... */
    uint32_t bcn_template_not_ready_done;     /* ...but "DMA done" interrupt rcvd */
    uint32_t late_tbtt_dpc;     /* TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t rx1mbps;     /* packets rx at 1Mbps */
    uint32_t rx2mbps;     /* packets rx at 2Mbps */
    uint32_t rx5mbps5;     /* packets rx at 5.5Mbps */
    uint32_t rx6mbps;     /* packets rx at 6Mbps */
    uint32_t rx9mbps;     /* packets rx at 9Mbps */
    uint32_t rx11mbps;     /* packets rx at 11Mbps */
    uint32_t rx12mbps;     /* packets rx at 12Mbps */
    uint32_t rx18mbps;     /* packets rx at 18Mbps */
    uint32_t rx24mbps;     /* packets rx at 24Mbps */
    uint32_t rx36mbps;     /* packets rx at 36Mbps */
    uint32_t rx48mbps;     /* packets rx at 48Mbps */
    uint32_t rx54mbps;     /* packets rx at 54Mbps */
    uint32_t rx108mbps;     /* packets rx at 108mbps */
    uint32_t rx162mbps;     /* packets rx at 162mbps */
    uint32_t rx216mbps;     /* packets rx at 216 mbps */
    uint32_t rx270mbps;     /* packets rx at 270 mbps */
    uint32_t rx324mbps;     /* packets rx at 324 mbps */
    uint32_t rx378mbps;     /* packets rx at 378 mbps */
    uint32_t rx432mbps;     /* packets rx at 432 mbps */
    uint32_t rx486mbps;     /* packets rx at 486 mbps */
    uint32_t rx540mbps;     /* packets rx at 540 mbps */

    /* pkteng rx frame stats */
    uint32_t pktengrxducast;     /* unicast frames rxed by the pkteng code */
    uint32_t pktengrxdmcast;     /* multicast frames rxed by the pkteng code */

    uint32_t rfdisable;     /* count of radio disables */
    uint32_t bphy_rxcrsglitch;     /* PHY count of bphy glitches */
    uint32_t bphy_badplcp;

    uint32_t txexptime;     /* Tx frames suppressed due to timer expiration */

    uint32_t txmpdu_sgi;     /* count for sgi transmit */
    uint32_t rxmpdu_sgi;     /* count for sgi received */
    uint32_t txmpdu_stbc;     /* count for stbc transmit */
    uint32_t rxmpdu_stbc;     /* count for stbc received */

    uint32_t rxundec_mcst;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill_mcst;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr_mcst;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay_mcst;     /* TKIPReplays */
    uint32_t ccmpfmterr_mcst;     /* CCMPFormatErrors */
    uint32_t ccmpreplay_mcst;     /* CCMPReplays */
    uint32_t ccmpundec_mcst;     /* CCMPDecryptErrors */
    uint32_t fourwayfail_mcst;     /* FourWayHandshakeFailures */
    uint32_t wepundec_mcst;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr_mcst;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess_mcst;     /* DecryptSuccessCount */
    uint32_t tkipicverr_mcst;     /* TKIPICVErrorCount */
    uint32_t wepexcluded_mcst;     /* dot11WEPExcludedCount */

    uint32_t dma_hang;     /* count for dma hang */
    uint32_t reinit;     /* count for reinit */

    uint32_t pstatxucast;     /* count of ucast frames xmitted on all psta assoc */
    uint32_t pstatxnoassoc;     /* count of txnoassoc frames xmitted on all psta assoc */
    uint32_t pstarxucast;     /* count of ucast frames received on all psta assoc */
    uint32_t pstarxbcmc;     /* count of bcmc frames received on all psta */
    uint32_t pstatxbcmc;     /* count of bcmc frames transmitted on all psta */

    uint32_t cso_passthrough;     /* hw cso required but passthrough */
    uint32_t cso_normal;     /* hw cso hdr for normal process */
    uint32_t chained;     /* number of frames chained */
    uint32_t chainedsz1;     /* number of chain size 1 frames */
    uint32_t unchained;     /* number of frames not chained */
    uint32_t maxchainsz;     /* max chain size so far */
    uint32_t currchainsz;     /* current chain size */

    uint32_t rxdrop20s;     /* drop secondary cnt */

} wl_cnt_ver_eight_t;

/* per-rate receive stat counters subset of full counters */
typedef struct
{
    uint32_t rx1mbps;     /* packets rx at 1Mbps */
    uint32_t rx2mbps;     /* packets rx at 2Mbps */
    uint32_t rx5mbps5;     /* packets rx at 5.5Mbps */
    uint32_t rx6mbps;     /* packets rx at 6Mbps */
    uint32_t rx9mbps;     /* packets rx at 9Mbps */
    uint32_t rx11mbps;     /* packets rx at 11Mbps */
    uint32_t rx12mbps;     /* packets rx at 12Mbps */
    uint32_t rx18mbps;     /* packets rx at 18Mbps */
    uint32_t rx24mbps;     /* packets rx at 24Mbps */
    uint32_t rx36mbps;     /* packets rx at 36Mbps */
    uint32_t rx48mbps;     /* packets rx at 48Mbps */
    uint32_t rx54mbps;     /* packets rx at 54Mbps */
    uint32_t rx108mbps;     /* packets rx at 108mbps */
    uint32_t rx162mbps;     /* packets rx at 162mbps */
    uint32_t rx216mbps;     /* packets rx at 216 mbps */
    uint32_t rx270mbps;     /* packets rx at 270 mbps */
} whd_phyrate_counters_t;

typedef struct
{
    uint32_t count;
    uint8_t log[WL_PHYRATE_LOG_SIZE];
} whd_phyrate_log_t;

typedef struct
{
    uint16_t version;     /* see definition of WL_CNT_T_VERSION */
    uint16_t length;     /* length of entire structure */

    /* transmit stat counters */
    uint32_t txframe;     /* tx data frames */
    uint32_t txbyte;     /* tx data bytes */
    uint32_t txretrans;    /* tx mac retransmits */
    uint32_t txerror;     /* tx data errors (derived: sum of others) */
    uint32_t txctl;     /* tx management frames */
    uint32_t txprshort;    /* tx short preamble frames */
    uint32_t txserr;     /* tx status errors */
    uint32_t txnobuf;     /* tx out of buffers errors */
    uint32_t txnoassoc;    /* tx discard because we're not associated */
    uint32_t txrunt;     /* tx runt frames */
    uint32_t txchit;     /* tx header cache hit (fastpath) */
    uint32_t txcmiss;     /* tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t txuflo;     /* tx fifo underflows */
    uint32_t txphyerr;     /* tx phy errors (indicated in tx status) */
    uint32_t txphycrs;     /* PR8861/8963 counter */

    /* receive stat counters */
    uint32_t rxframe;     /* rx data frames */
    uint32_t rxbyte;     /* rx data bytes */
    uint32_t rxerror;     /* rx data errors (derived: sum of others) */
    uint32_t rxctl;     /* rx management frames */
    uint32_t rxnobuf;     /* rx out of buffers errors */
    uint32_t rxnondata;    /* rx non data frames in the data channel errors */
    uint32_t rxbadds;     /* rx bad DS errors */
    uint32_t rxbadcm;     /* rx bad control or management frames */
    uint32_t rxfragerr;    /* rx fragmentation errors */
    uint32_t rxrunt;     /* rx runt frames */
    uint32_t rxgiant;     /* rx giant frames */
    uint32_t rxnoscb;     /* rx no scb error */
    uint32_t rxbadproto;     /* rx invalid frames */
    uint32_t rxbadsrcmac;    /* rx frames with Invalid Src Mac */
    uint32_t rxbadda;     /* rx frames tossed for invalid da */
    uint32_t rxfilter;     /* rx frames filtered out */

    /* receive chip error counters */
    uint32_t rxoflo;     /* rx fifo overflow errors */
    uint32_t rxuflo[NFIFO];     /* rx dma descriptor underflow errors */

    uint32_t d11cnt_txrts_off;     /* d11cnt txrts value when reset d11cnt */
    uint32_t d11cnt_rxcrc_off;     /* d11cnt rxcrc value when reset d11cnt */
    uint32_t d11cnt_txnocts_off;     /* d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t dmade;     /* tx/rx dma descriptor errors */
    uint32_t dmada;     /* tx/rx dma data errors */
    uint32_t dmape;     /* tx/rx dma descriptor protocol errors */
    uint32_t reset;     /* reset count */
    uint32_t tbtt;     /* cnts the TBTT int's */
    uint32_t txdmawar;     /* # occurrences of PR15420 workaround */
    uint32_t pkt_callback_reg_fail;     /* callbacks register failure */

    /* MAC counters: 32-bit version of d11.h's macstat_t */
    uint32_t txallfrm;     /* total number of frames sent, incl. Data, ACK, RTS, CTS,
                            * Control Management (includes retransmissions)
                            */
    uint32_t txrtsfrm;     /* number of RTS sent out by the MAC */
    uint32_t txctsfrm;     /* number of CTS sent out by the MAC */
    uint32_t txackfrm;     /* number of ACK frames sent out */
    uint32_t txdnlfrm;     /* Not used */
    uint32_t txbcnfrm;     /* beacons transmitted */
    uint32_t txfunfl[6];     /* per-fifo tx underflows */
    uint32_t rxtoolate;     /* receive too late */
    uint32_t txfbw;     /* transmit at fallback bw (dynamic bw) */
    uint32_t txtplunfl;     /* Template underflows (mac was too slow to transmit ACK/CTS
                             * or BCN)
                             */
    uint32_t txphyerror;     /* Transmit phy error, type of error is reported in tx-status for
                              * driver enqueued frames
                              */
    uint32_t rxfrmtoolong;    /* Received frame longer than legal limit (2346 bytes) */
    uint32_t rxfrmtooshrt;    /* Received frame did not contain enough bytes for its frame type */
    uint32_t rxinvmachdr;     /* Either the protocol version != 0 or frame type not
                               * data/control/management
                               */
    uint32_t rxbadfcs;     /* number of frames for which the CRC check failed in the MAC */
    uint32_t rxbadplcp;     /* parity check of the PLCP header failed */
    uint32_t rxcrsglitch;    /* PHY was able to correlate the preamble but not the header */
    uint32_t rxstrt;     /* Number of received frames with a good PLCP
                          * (i.e. passing parity check)
                          */
    uint32_t rxdfrmucastmbss;     /* Number of received DATA frames with good FCS and matching RA */
    uint32_t rxmfrmucastmbss;     /* number of received mgmt frames with good FCS and matching RA */
    uint32_t rxcfrmucast;     /* number of received CNTRL frames with good FCS and matching RA */
    uint32_t rxrtsucast;     /* number of unicast RTS addressed to the MAC (good FCS) */
    uint32_t rxctsucast;     /* number of unicast CTS addressed to the MAC (good FCS) */
    uint32_t rxackucast;     /* number of ucast ACKS received (good FCS) */
    uint32_t rxdfrmocast;     /* number of received DATA frames (good FCS and not matching RA) */
    uint32_t rxmfrmocast;     /* number of received MGMT frames (good FCS and not matching RA) */
    uint32_t rxcfrmocast;     /* number of received CNTRL frame (good FCS and not matching RA) */
    uint32_t rxrtsocast;     /* number of received RTS not addressed to the MAC */
    uint32_t rxctsocast;     /* number of received CTS not addressed to the MAC */
    uint32_t rxdfrmmcast;     /* number of RX Data multicast frames received by the MAC */
    uint32_t rxmfrmmcast;     /* number of RX Management multicast frames received by the MAC */
    uint32_t rxcfrmmcast;     /* number of RX Control multicast frames received by the MAC
                               * (unlikely to see these)
                               */
    uint32_t rxbeaconmbss;     /* beacons received from member of BSS */
    uint32_t rxdfrmucastobss;     /* number of unicast frames addressed to the MAC from
                                   * other BSS (WDS FRAME)
                                   */
    uint32_t rxbeaconobss;     /* beacons received from other BSS */
    uint32_t rxrsptmout;     /* Number of response timeouts for transmitted frames
                              * expecting a response
                              */
    uint32_t bcntxcancl;     /* transmit beacons canceled due to receipt of beacon (IBSS) */
    uint32_t rxf0ovfl;     /* Number of receive fifo 0 overflows */
    uint32_t rxf1ovfl;     /* Number of receive fifo 1 overflows (obsolete) */
    uint32_t rxf2ovfl;     /* Number of receive fifo 2 overflows (obsolete) */
    uint32_t txsfovfl;     /* Number of transmit status fifo overflows (obsolete) */
    uint32_t pmqovfl;     /* Number of PMQ overflows */
    uint32_t rxcgprqfrm;     /* Number of received Probe requests that made it into
                              * the PRQ fifo
                              */
    uint32_t rxcgprsqovfl;     /* Rx Probe Request Que overflow in the AP */
    uint32_t txcgprsfail;     /* Tx Probe Response Fail. AP sent probe response but did
                               * not get ACK
                               */
    uint32_t txcgprssuc;     /* Tx Probe Response Success (ACK was received) */
    uint32_t prs_timeout;     /* Number of probe requests that were dropped from the PRQ
                               * fifo because a probe response could not be sent out within
                               * the time limit defined in M_PRS_MAXTIME
                               */
    uint32_t rxnack;     /* obsolete */
    uint32_t frmscons;     /* obsolete */
    uint32_t txnack;     /* obsolete */
    uint32_t rxback;     /* blockack rxcnt */
    uint32_t txback;     /* blockack txcnt */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t txfrag;     /* dot11TransmittedFragmentCount */
    uint32_t txmulti;     /* dot11MulticastTransmittedFrameCount */
    uint32_t txfail;     /* dot11FailedCount */
    uint32_t txretry;     /* dot11RetryCount */
    uint32_t txretrie;     /* dot11MultipleRetryCount */
    uint32_t rxdup;     /* dot11FrameduplicateCount */
    uint32_t txrts;     /* dot11RTSSuccessCount */
    uint32_t txnocts;     /* dot11RTSFailureCount */
    uint32_t txnoack;     /* dot11ACKFailureCount */
    uint32_t rxfrag;     /* dot11ReceivedFragmentCount */
    uint32_t rxmulti;     /* dot11MulticastReceivedFrameCount */
    uint32_t rxcrc;     /* dot11FCSErrorCount */
    uint32_t txfrmsnt;     /* dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t rxundec;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill;     /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay;     /* TKIPReplays */
    uint32_t ccmpfmterr;     /* CCMPFormatErrors */
    uint32_t ccmpreplay;     /* CCMPReplays */
    uint32_t ccmpundec;     /* CCMPDecryptErrors */
    uint32_t fourwayfail;     /* FourWayHandshakeFailures */
    uint32_t wepundec;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess;     /* DecryptSuccessCount */
    uint32_t tkipicverr;     /* TKIPICVErrorCount */
    uint32_t wepexcluded;     /* dot11WEPExcludedCount */

    uint32_t txchanrej;     /* Tx frames suppressed due to channel rejection */
    uint32_t psmwds;     /* Count PSM watchdogs */
    uint32_t phywatchdog;     /* Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t prq_entries_handled;     /* PRQ entries read in */
    uint32_t prq_undirected_entries;     /*    which were bcast bss & ssid */
    uint32_t prq_bad_entries;     /*    which could not be translated to info */
    uint32_t atim_suppress_count;     /* TX suppressions on ATIM fifo */
    uint32_t bcn_template_not_ready;     /* Template marked in use on send bcn ... */
    uint32_t bcn_template_not_ready_done;     /* ...but "DMA done" interrupt rcvd */
    uint32_t late_tbtt_dpc;     /* TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t rx1mbps;     /* packets rx at 1Mbps */
    uint32_t rx2mbps;     /* packets rx at 2Mbps */
    uint32_t rx5mbps5;     /* packets rx at 5.5Mbps */
    uint32_t rx6mbps;     /* packets rx at 6Mbps */
    uint32_t rx9mbps;     /* packets rx at 9Mbps */
    uint32_t rx11mbps;     /* packets rx at 11Mbps */
    uint32_t rx12mbps;     /* packets rx at 12Mbps */
    uint32_t rx18mbps;     /* packets rx at 18Mbps */
    uint32_t rx24mbps;     /* packets rx at 24Mbps */
    uint32_t rx36mbps;     /* packets rx at 36Mbps */
    uint32_t rx48mbps;     /* packets rx at 48Mbps */
    uint32_t rx54mbps;     /* packets rx at 54Mbps */
    uint32_t rx108mbps;    /* packets rx at 108mbps */
    uint32_t rx162mbps;    /* packets rx at 162mbps */
    uint32_t rx216mbps;    /* packets rx at 216 mbps */
    uint32_t rx270mbps;    /* packets rx at 270 mbps */
    uint32_t rx324mbps;    /* packets rx at 324 mbps */
    uint32_t rx378mbps;    /* packets rx at 378 mbps */
    uint32_t rx432mbps;    /* packets rx at 432 mbps */
    uint32_t rx486mbps;    /* packets rx at 486 mbps */
    uint32_t rx540mbps;    /* packets rx at 540 mbps */

    /* pkteng rx frame stats */
    uint32_t pktengrxducast;     /* unicast frames rxed by the pkteng code */
    uint32_t pktengrxdmcast;     /* multicast frames rxed by the pkteng code */

    uint32_t rfdisable;     /* count of radio disables */
    uint32_t bphy_rxcrsglitch;     /* PHY count of bphy glitches */
    uint32_t bphy_badplcp;

    uint32_t txexptime;     /* Tx frames suppressed due to timer expiration */

    uint32_t txmpdu_sgi;     /* count for sgi transmit */
    uint32_t rxmpdu_sgi;     /* count for sgi received */
    uint32_t txmpdu_stbc;     /* count for stbc transmit */
    uint32_t rxmpdu_stbc;     /* count for stbc received */

    uint32_t rxundec_mcst;     /* dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t tkipmicfaill_mcst;    /* TKIPLocalMICFailures */
    uint32_t tkipcntrmsr_mcst;     /* TKIPCounterMeasuresInvoked */
    uint32_t tkipreplay_mcst;     /* TKIPReplays */
    uint32_t ccmpfmterr_mcst;     /* CCMPFormatErrors */
    uint32_t ccmpreplay_mcst;     /* CCMPReplays */
    uint32_t ccmpundec_mcst;     /* CCMPDecryptErrors */
    uint32_t fourwayfail_mcst;     /* FourWayHandshakeFailures */
    uint32_t wepundec_mcst;     /* dot11WEPUndecryptableCount */
    uint32_t wepicverr_mcst;     /* dot11WEPICVErrorCount */
    uint32_t decsuccess_mcst;     /* DecryptSuccessCount */
    uint32_t tkipicverr_mcst;     /* TKIPICVErrorCount */
    uint32_t wepexcluded_mcst;     /* dot11WEPExcludedCount */

    uint32_t dma_hang;     /* count for dma hang */
    uint32_t reinit;     /* count for reinit */

    uint32_t pstatxucast;     /* count of ucast frames xmitted on all psta assoc */
    uint32_t pstatxnoassoc;     /* count of txnoassoc frames xmitted on all psta assoc */
    uint32_t pstarxucast;     /* count of ucast frames received on all psta assoc */
    uint32_t pstarxbcmc;     /* count of bcmc frames received on all psta */
    uint32_t pstatxbcmc;     /* count of bcmc frames transmitted on all psta */

    uint32_t cso_passthrough;     /* hw cso required but passthrough */
    uint32_t cso_normal;     /* hw cso hdr for normal process */
    uint32_t chained;     /* number of frames chained */
    uint32_t chainedsz1;     /* number of chain size 1 frames */
    uint32_t unchained;     /* number of frames not chained */
    uint32_t maxchainsz;     /* max chain size so far */
    uint32_t currchainsz;     /* current chain size */
    uint32_t rxdrop20s;     /* drop secondary cnt */
    uint32_t pciereset;     /* Secondary Bus Reset issued by driver */
    uint32_t cfgrestore;     /* configspace restore by driver */
    uint32_t reinitreason[8];     /* reinitreason counters; 0: Unknown reason */
    uint32_t rxrtry;     /* num of received packets with retry bit on */
} wl_cnt_ver_ten_t;

typedef struct
{
    uint16_t version;
    uint16_t length;
    uint32_t rxampdu_sgi;
    uint32_t rxampdu_stbc;
    uint32_t rxmpdu_sgi;
    uint32_t rxmpdu_stbc;
    uint32_t rxmcs0_40M;
    uint32_t rxmcs1_40M;
    uint32_t rxmcs2_40M;
    uint32_t rxmcs3_40M;
    uint32_t rxmcs4_40M;
    uint32_t rxmcs5_40M;
    uint32_t rxmcs6_40M;
    uint32_t rxmcs7_40M;
    uint32_t rxmcs32_40M;
    uint32_t txfrmsnt_20Mlo;
    uint32_t txfrmsnt_20Mup;
    uint32_t txfrmsnt_40M;
    uint32_t rx_20ul;
} wl_cnt_ext_t;
#define    WL_RXDIV_STATS_T_VERSION    1
typedef struct
{
    uint16_t version;
    uint16_t length;
    uint32_t rxant[4];
} wl_rxdiv_stats_t;
#define    WL_DELTA_STATS_T_VERSION    1
typedef struct
{
    uint16_t version;
    uint16_t length;
    uint32_t txframe;
    uint32_t txbyte;
    uint32_t txretrans;
    uint32_t txfail;
    uint32_t rxframe;
    uint32_t rxbyte;
    uint32_t rx1mbps;
    uint32_t rx2mbps;
    uint32_t rx5mbps5;
    uint32_t rx6mbps;
    uint32_t rx9mbps;
    uint32_t rx11mbps;
    uint32_t rx12mbps;
    uint32_t rx18mbps;
    uint32_t rx24mbps;
    uint32_t rx36mbps;
    uint32_t rx48mbps;
    uint32_t rx54mbps;
    uint32_t rx108mbps;
    uint32_t rx162mbps;
    uint32_t rx216mbps;
    uint32_t rx270mbps;
    uint32_t rx324mbps;
    uint32_t rx378mbps;
    uint32_t rx432mbps;
    uint32_t rx486mbps;
    uint32_t rx540mbps;
} wl_delta_stats_t;
#define WL_WME_CNT_VERSION    1
typedef struct
{
    uint32_t packets;
    uint32_t bytes;
} wl_traffic_stats_t;
#define AC_COUNT        4
typedef struct
{
    uint16_t version;
    uint16_t length;
    wl_traffic_stats_t tx[AC_COUNT];
    wl_traffic_stats_t tx_failed[AC_COUNT];
    wl_traffic_stats_t rx[AC_COUNT];
    wl_traffic_stats_t rx_failed[AC_COUNT];
    wl_traffic_stats_t forward[AC_COUNT];
    wl_traffic_stats_t tx_expired[AC_COUNT];
} wl_wme_cnt_t;

#define WL_MKEEP_ALIVE_VERSION      1
#define WL_MKEEP_ALIVE_FIXED_LEN    offsetof(wl_mkeep_alive_pkt_t, data)
#define WL_MKEEP_ALIVE_PRECISION    500

#define    WLC_BA_CNT_VERSION    1
typedef struct wlc_ba_cnt
{
    uint16_t version;
    uint16_t length;
    uint32_t txpdu;
    uint32_t txsdu;
    uint32_t txfc;
    uint32_t txfci;
    uint32_t txretrans;
    uint32_t txbatimer;
    uint32_t txdrop;
    uint32_t txaddbareq;
    uint32_t txaddbaresp;
    uint32_t txdelba;
    uint32_t txba;
    uint32_t txbar;
    uint32_t txpad[4];
    uint32_t rxpdu;
    uint32_t rxqed;
    uint32_t rxdup;
    uint32_t rxnobuf;
    uint32_t rxaddbareq;
    uint32_t rxaddbaresp;
    uint32_t rxdelba;
    uint32_t rxba;
    uint32_t rxbar;
    uint32_t rxinvba;
    uint32_t rxbaholes;
    uint32_t rxunexp;
    uint32_t rxpad[4];
} wlc_ba_cnt_t;
struct ampdu_tid_control
{
    uint8_t tid;
    uint8_t enable;
};
struct wl_msglevel2
{
    uint32_t low;
    uint32_t high;
};
struct ampdu_ea_tid
{
    wl_ether_addr_t ea;
    uint8_t tid;
};
struct ampdu_retry_tid
{
    uint8_t tid;
    uint8_t retry;
};
struct ampdu_ba_sizes
{
    uint8_t ba_tx_wsize;
    uint8_t ba_rx_wsize;
};
#define    DPT_DISCOVERY_MANUAL    0x01
#define    DPT_DISCOVERY_AUTO    0x02
#define    DPT_DISCOVERY_SCAN    0x04
#define DPT_PATHSEL_AUTO    0
#define DPT_PATHSEL_DIRECT    1
#define DPT_PATHSEL_APPATH    2
#define DPT_DENY_LIST_ADD     1
#define DPT_DENY_LIST_REMOVE     2
#define DPT_MANUAL_EP_CREATE    1
#define DPT_MANUAL_EP_MODIFY    2
#define DPT_MANUAL_EP_DELETE    3
typedef struct dpt_iovar
{
    wl_ether_addr_t ea;
    uint8_t mode;
    uint32_t pad;
} dpt_iovar_t;
#define    DPT_STATUS_ACTIVE    0x01
#define    DPT_STATUS_AES        0x02
#define    DPT_STATUS_FAILED    0x04
#define    DPT_FNAME_LEN        48
typedef struct dpt_status
{
    uint8_t status;
    uint8_t fnlen;
    uint8_t name[DPT_FNAME_LEN];
    uint32_t rssi;
    sta_info_t sta;
} dpt_status_t;
typedef struct dpt_list
{
    uint32_t num;
    dpt_status_t status[1];
} dpt_list_t;
typedef struct dpt_fname
{
    uint8_t len;
    uint8_t name[DPT_FNAME_LEN];
} dpt_fname_t;
#define    BDD_FNAME_LEN        32
typedef struct bdd_fname
{
    uint8_t len;
    uint8_t name[BDD_FNAME_LEN];
} bdd_fname_t;
struct ts_list
{
    int32_t count;
    struct tsinfo_arg tsinfo[1];
};
typedef struct tspec_arg
{
    uint16_t version;
    uint16_t length;
    uint32_t flag;
    struct tsinfo_arg tsinfo;
    uint16_t nom_msdu_size;
    uint16_t max_msdu_size;
    uint32_t min_srv_interval;
    uint32_t max_srv_interval;
    uint32_t inactivity_interval;
    uint32_t suspension_interval;
    uint32_t srv_start_time;
    uint32_t min_data_rate;
    uint32_t mean_data_rate;
    uint32_t peak_data_rate;
    uint32_t max_burst_size;
    uint32_t delay_bound;
    uint32_t min_phy_rate;
    uint16_t surplus_bw;
    uint16_t medium_time;
    uint8_t dialog_token;
} tspec_arg_t;
typedef struct tspec_per_sta_arg
{
    wl_ether_addr_t ea;
    struct tspec_arg ts;
} tspec_per_sta_arg_t;
typedef struct wme_max_bandwidth
{
    uint32_t ac[AC_COUNT];
} wme_max_bandwidth_t;
#define WL_WME_MBW_PARAMS_IO_BYTES (sizeof(wme_max_bandwidth_t) )
#define    TSPEC_ARG_VERSION        2
#define TSPEC_ARG_LENGTH        55
#define TSPEC_DEFAULT_DIALOG_TOKEN    42
#define TSPEC_DEFAULT_SBW_FACTOR    0x3000
#define TSPEC_PENDING        0
#define TSPEC_ACCEPTED        1
#define TSPEC_REJECTED        2
#define TSPEC_UNKNOWN        3
#define TSPEC_STATUS_MASK    7
#define WL_WLAN_ASSOC_REASON_NORMAL_NETWORK    0
#define WL_WLAN_ASSOC_REASON_ROAM_FROM_CELLULAR_NETWORK    1
#define WL_WLAN_ASSOC_REASON_ROAM_FROM_LAN    2
#define WL_WLAN_ASSOC_REASON_MAX        2
#define WL_SWFL_ABBFL        0x0001
#define WL_SWFL_ABENCORE    0x0002
#define WL_SWFL_NOHWRADIO    0x0004
#define WL_LIFETIME_MAX 0xFFFF
typedef struct wl_lifetime
{
    uint32_t ac;
    uint32_t lifetime;
} wl_lifetime_t;
typedef struct wl_chan_switch
{
    uint8_t mode;
    uint8_t count;
    wl_chanspec_t chspec;
    uint8_t reg;
} wl_chan_switch_t;
#define WLC_ROAM_TRIGGER_DEFAULT    0
#define WLC_ROAM_TRIGGER_BANDWIDTH    1
#define WLC_ROAM_TRIGGER_DISTANCE    2
#define WLC_ROAM_TRIGGER_MAX_VALUE    2
enum
{
    PFN_LIST_ORDER, PFN_RSSI
};
#define SORT_CRITERIA_BIT       0
#define AUTO_NET_SWITCH_BIT     1
#define ENABLE_BKGRD_SCAN_BIT   2
#define IMMEDIATE_SCAN_BIT      3
#define AUTO_CONNECT_BIT        4
#define IMMEDIATE_EVENT_BIT     8
#define SUPPRESS_SSID_BIT       9
#define ENABLE_NET_OFFLOAD_BIT  10
#define SORT_CRITERIA_MASK      0x01
#define AUTO_NET_SWITCH_MASK    0x02
#define ENABLE_BKGRD_SCAN_MASK  0x04
#define IMMEDIATE_SCAN_MASK     0x08
#define AUTO_CONNECT_MASK       0x10
#define PFN_VERSION             2

/* PFN network info structure */
typedef struct wl_pfn_subnet_info
{
    struct ether_addr BSSID;
    uint8_t channel;     /* channel number only */
    uint8_t SSID_len;
    uint8_t SSID[32];
} wl_pfn_subnet_info_t;

typedef struct wl_pfn_net_info
{
    wl_pfn_subnet_info_t pfnsubnet;
    int16_t RSSI;     /* receive signal strength (in dBm) */
    uint16_t timestamp;     /* age in seconds */
} wl_pfn_net_info_t;

/* used to report exactly one scan result */
/* plus reports detailed scan info in bss_info */
typedef struct wl_pfn_scanresult
{
    uint32_t version;
    uint32_t status;
    uint32_t count;
    wl_pfn_net_info_t netinfo;
    wl_bss_info_t bss_info;
} wl_pfn_scanresult_t;

/* PFN data structure */
typedef struct wl_pfn_param
{
    int32_t version;     /* PNO parameters version */
    int32_t scan_freq;     /* Scan frequency */
    int32_t lost_network_timeout;     /* Timeout in sec. to declare
                                       * discovered network as lost
                                       */
    int16_t flags;     /* Bit field to control features
                        * of PFN such as sort criteria auto
                        * enable switch and background scan
                        */
    int16_t rssi_margin;     /* Margin to avoid jitter for choosing a
                              * PFN based on RSSI sort criteria
                              */
    uint8_t bestn;     /* number of best networks in each scan */
    uint8_t mscan;     /* number of scans recorded */
    uint8_t repeat;     /* Minimum number of scan intervals
                         * before scan frequency changes in adaptive scan
                         */
    uint8_t exp;     /* Exponent of 2 for maximum scan interval */

    int32_t slow_freq;     /* slow scan period */
} wl_pfn_param_t;

typedef struct wl_pfn_bssid
{
    struct ether_addr macaddr;
    /* Bit4: suppress_lost, Bit3: suppress_found */
    uint16_t flags;
} wl_pfn_bssid_t;

typedef struct wl_pfn_cfg
{
    uint32_t reporttype;
    int32_t channel_num;
    uint16_t channel_list[WL_NUMCHANNELS];
    uint32_t flags;
} wl_pfn_cfg_t;

/* for use with wl_pfn.flags */
#define WL_PFN_HIDDEN_MASK       0x4
#define WL_PFN_SUPPRESSLOST_MASK 0x10

typedef struct wl_pfn
{
    wlc_ssid_t ssid;     /* ssid name and its length */
    int32_t flags;     /* bit2: hidden */
    int32_t infra;     /* BSS Vs IBSS */
    int32_t auth;     /* Open Vs Closed */
    int32_t wpa_auth;     /* WPA type */
    int32_t wsec;     /* wsec value */
} wl_pfn_t;

#define TOE_TX_CSUM_OL        0x00000001
#define TOE_RX_CSUM_OL        0x00000002
#define TOE_ERRTEST_TX_CSUM    0x00000001
#define TOE_ERRTEST_RX_CSUM    0x00000002
#define TOE_ERRTEST_RX_CSUM2    0x00000004
struct toe_ol_stats_t
{
    uint32_t tx_summed;
    uint32_t tx_iph_fill;
    uint32_t tx_tcp_fill;
    uint32_t tx_udp_fill;
    uint32_t tx_icmp_fill;
    uint32_t rx_iph_good;
    uint32_t rx_iph_bad;
    uint32_t rx_tcp_good;
    uint32_t rx_tcp_bad;
    uint32_t rx_udp_good;
    uint32_t rx_udp_bad;
    uint32_t rx_icmp_good;
    uint32_t rx_icmp_bad;
    uint32_t tx_tcp_errinj;
    uint32_t tx_udp_errinj;
    uint32_t tx_icmp_errinj;
    uint32_t rx_tcp_errinj;
    uint32_t rx_udp_errinj;
    uint32_t rx_icmp_errinj;
};
#define ARP_OL_AGENT        0x00000001
#define ARP_OL_SNOOP        0x00000002
#define ARP_OL_HOST_AUTO_REPLY    0x00000004
#define ARP_OL_PEER_AUTO_REPLY    0x00000008
#define ARP_ERRTEST_REPLY_PEER    0x1
#define ARP_ERRTEST_REPLY_HOST    0x2
#define ARP_MULTIHOMING_MAX    8
typedef struct arp_ol_stats
{
    uint32_t host_ip_entries;
    uint32_t host_ip_overflow;
    uint32_t arp_table_entries;
    uint32_t arp_table_overflow;
    uint32_t host_request;
    uint32_t host_reply;
    uint32_t host_service;
    uint32_t peer_request;
    uint32_t peer_request_drop;
    uint32_t peer_reply;
    uint32_t peer_reply_drop;
    uint32_t peer_service;
}arp_ol_stats_t;
typedef struct wl_keep_alive_pkt
{
    uint32_t period_msec;
    uint16_t len_bytes;
    uint8_t data[1];
} wl_keep_alive_pkt_t;
#define WL_KEEP_ALIVE_FIXED_LEN        offsetof(wl_keep_alive_pkt_t, data)
typedef enum wl_pkt_filter_type
{
    WL_PKT_FILTER_TYPE_PATTERN_MATCH
} wl_pkt_filter_type_t;
#define WL_PKT_FILTER_TYPE wl_pkt_filter_type_t
typedef struct wl_pkt_filter_pattern
{
    uint32_t offset;
    uint32_t size_bytes;
    uint8_t mask_and_pattern[1];
} wl_pkt_filter_pattern_t;
typedef struct wl_pkt_filter
{
    uint32_t id;
    uint32_t type;
    uint32_t negate_match;
    union
    {
        wl_pkt_filter_pattern_t pattern;
    } u;
} wl_pkt_filter_t;
#define WL_PKT_FILTER_FIXED_LEN              offsetof(wl_pkt_filter_t, u)
#define WL_PKT_FILTER_PATTERN_FIXED_LEN      offsetof(wl_pkt_filter_pattern_t, mask_and_pattern)
typedef struct wl_pkt_filter_enable
{
    uint32_t id;
    uint32_t enable;
} wl_pkt_filter_enable_t;
typedef struct wl_pkt_filter_list
{
    uint32_t num;
    wl_pkt_filter_t filter[1];
} wl_pkt_filter_list_t;
#define WL_PKT_FILTER_LIST_FIXED_LEN      offsetof(wl_pkt_filter_list_t, filter)
typedef struct wl_pkt_filter_stats
{
    uint32_t num_pkts_matched;
    uint32_t num_pkts_forwarded;
    uint32_t num_pkts_discarded;
} wl_pkt_filter_stats_t;
typedef struct wl_seq_cmd_ioctl
{
    uint32_t cmd;
    uint32_t len;
} wl_seq_cmd_ioctl_t;
#define WL_SEQ_CMD_ALIGN_BYTES    4
#define WL_SEQ_CMDS_GET_IOCTL_FILTER(cmd) \
    ( ( (cmd) == WLC_GET_MAGIC )        || \
      ( (cmd) == WLC_GET_VERSION )        || \
      ( (cmd) == WLC_GET_AP )            || \
      ( (cmd) == WLC_GET_INSTANCE ) )
#define WL_PKTENG_PER_TX_START            0x01
#define WL_PKTENG_PER_TX_STOP            0x02
#define WL_PKTENG_PER_RX_START            0x04
#define WL_PKTENG_PER_RX_WITH_ACK_START     0x05
#define WL_PKTENG_PER_TX_WITH_ACK_START     0x06
#define WL_PKTENG_PER_RX_STOP            0x08
#define WL_PKTENG_PER_MASK            0xff
#define WL_PKTENG_SYNCHRONOUS            0x100
typedef struct wl_pkteng
{
    uint32_t flags;
    uint32_t delay;
    uint32_t nframes;
    uint32_t length;
    uint8_t seqno;
    wl_ether_addr_t dest;
    wl_ether_addr_t src;
} wl_pkteng_t;
#define NUM_80211b_RATES    4
#define NUM_80211ag_RATES    8
#define NUM_80211n_RATES    32
#define NUM_80211_RATES        (NUM_80211b_RATES + NUM_80211ag_RATES + NUM_80211n_RATES)
typedef struct wl_pkteng_stats
{
    uint32_t lostfrmcnt;
    int32_t rssi;
    int32_t snr;
    uint16_t rxpktcnt[NUM_80211_RATES + 1];
} wl_pkteng_stats_t;
typedef struct wl_sslpnphy_papd_debug_data
{
    uint8_t psat_pwr;
    uint8_t psat_indx;
    uint8_t final_idx;
    uint8_t start_idx;
    int32_t min_phase;
    int32_t voltage;
    int8_t temperature;
} wl_sslpnphy_papd_debug_data_t;
typedef struct wl_sslpnphy_debug_data
{
    int16_t papdcompRe[64];
    int16_t papdcompIm[64];
} wl_sslpnphy_debug_data_t;
typedef struct wl_sslpnphy_spbdump_data
{
    uint16_t tbl_length;
    int16_t spbreal[256];
    int16_t spbimg[256];
} wl_sslpnphy_spbdump_data_t;
typedef struct wl_sslpnphy_percal_debug_data
{
    uint32_t cur_idx;
    uint32_t tx_drift;
    uint8_t prev_cal_idx;
    uint32_t percal_ctr;
    int32_t nxt_cal_idx;
    uint32_t force_1idxcal;
    uint32_t onedxacl_req;
    int32_t last_cal_volt;
    int8_t last_cal_temp;
    uint32_t vbat_ripple;
    uint32_t exit_route;
    int32_t volt_winner;
} wl_sslpnphy_percal_debug_data_t;
#define WL_WOWL_MAGIC       (1 << 0)
#define WL_WOWL_NET         (1 << 1)
#define WL_WOWL_DIS         (1 << 2)
#define WL_WOWL_RETR        (1 << 3)
#define WL_WOWL_BCN         (1 << 4)
#define WL_WOWL_TST         (1 << 5)
#define WL_WOWL_TRAFFIC     (1 << 12)
#define WL_WOWL_BCAST       (1 << 15)
#define WL_WOWL_GTK         (0x441f)
#define WL_WOWL_DEAUTH      (0x1F)
#define WL_WOWL_ALL         (0x541E)

#define MAGIC_PKT_MINLEN 102
typedef struct
{
    uint32_t masksize;
    uint32_t offset;
    uint32_t patternoffset;
    uint32_t patternsize;
} wl_wowl_pattern_t;
typedef struct
{
    uint32_t count;
    wl_wowl_pattern_t pattern[1];
} wl_wowl_pattern_list_t;
typedef struct
{
    uint8_t pci_wakeind;
    uint16_t ucode_wakeind;
} wl_wowl_wakeind_t;
typedef struct wl_txrate_class
{
    uint8_t init_rate;
    uint8_t min_rate;
    uint8_t max_rate;
} wl_txrate_class_t;
#define WL_DELAYMODE_DEFER    0
#define WL_DELAYMODE_FORCE    1
#define WL_DELAYMODE_AUTO    2
#define WLC_OBSS_SCAN_PASSIVE_DWELL_DEFAULT        100
#define WLC_OBSS_SCAN_PASSIVE_DWELL_MIN            5
#define WLC_OBSS_SCAN_PASSIVE_DWELL_MAX            1000
#define WLC_OBSS_SCAN_ACTIVE_DWELL_DEFAULT        20
#define WLC_OBSS_SCAN_ACTIVE_DWELL_MIN            10
#define WLC_OBSS_SCAN_ACTIVE_DWELL_MAX            1000
#define WLC_OBSS_SCAN_WIDTHSCAN_INTERVAL_DEFAULT    300
#define WLC_OBSS_SCAN_WIDTHSCAN_INTERVAL_MIN        10
#define WLC_OBSS_SCAN_WIDTHSCAN_INTERVAL_MAX        900
#define WLC_OBSS_SCAN_CHANWIDTH_TRANSITION_DLY_DEFAULT    5
#define WLC_OBSS_SCAN_CHANWIDTH_TRANSITION_DLY_MIN    5
#define WLC_OBSS_SCAN_CHANWIDTH_TRANSITION_DLY_MAX    100
#define WLC_OBSS_SCAN_PASSIVE_TOTAL_PER_CHANNEL_DEFAULT    200
#define WLC_OBSS_SCAN_PASSIVE_TOTAL_PER_CHANNEL_MIN    200
#define WLC_OBSS_SCAN_PASSIVE_TOTAL_PER_CHANNEL_MAX    10000
#define WLC_OBSS_SCAN_ACTIVE_TOTAL_PER_CHANNEL_DEFAULT    20
#define WLC_OBSS_SCAN_ACTIVE_TOTAL_PER_CHANNEL_MIN    20
#define WLC_OBSS_SCAN_ACTIVE_TOTAL_PER_CHANNEL_MAX    10000
#define WLC_OBSS_SCAN_ACTIVITY_THRESHOLD_DEFAULT    25
#define WLC_OBSS_SCAN_ACTIVITY_THRESHOLD_MIN        0
#define WLC_OBSS_SCAN_ACTIVITY_THRESHOLD_MAX        100
typedef struct wl_obss_scan_arg
{
    int16_t passive_dwell;
    int16_t active_dwell;
    int16_t bss_widthscan_interval;
    int16_t passive_total;
    int16_t active_total;
    int16_t chanwidth_transition_delay;
    int16_t activity_threshold;
} wl_obss_scan_arg_t;
#define WL_OBSS_SCAN_PARAM_LEN    sizeof(wl_obss_scan_arg_t)
#define WL_MIN_NUM_OBSS_SCAN_ARG 7
#define WL_COEX_INFO_MASK        0x07
#define WL_COEX_INFO_REQ        0x01
#define    WL_COEX_40MHZ_INTOLERANT    0x02
#define    WL_COEX_WIDTH20            0x04
typedef struct wl_action_obss_coex_req
{
    uint8_t info;
    uint8_t num;
    uint8_t ch_list[1];
} wl_action_obss_coex_req_t;
#define MAX_RSSI_LEVELS 8
typedef struct wl_rssi_event
{
    uint32_t rate_limit_msec;
    uint8_t num_rssi_levels;
    int8_t rssi_levels[MAX_RSSI_LEVELS];
} wl_rssi_event_t;
#define WLFEATURE_DISABLE_11N        0x00000001
#define WLFEATURE_DISABLE_11N_STBC_TX    0x00000002
#define WLFEATURE_DISABLE_11N_STBC_RX    0x00000004
#define WLFEATURE_DISABLE_11N_SGI_TX    0x00000008
#define WLFEATURE_DISABLE_11N_SGI_RX    0x00000010
#define WLFEATURE_DISABLE_11N_AMPDU_TX    0x00000020
#define WLFEATURE_DISABLE_11N_AMPDU_RX    0x00000040
#define WLFEATURE_DISABLE_11N_GF    0x00000080

#pragma pack(1)

typedef struct sta_prbreq_wps_ie_hdr
{
    wl_ether_addr_t staAddr;
    uint16_t ieLen;
} sta_prbreq_wps_ie_hdr_t;

typedef struct sta_prbreq_wps_ie_data
{
    sta_prbreq_wps_ie_hdr_t hdr;
    uint8_t ieData[1];
} sta_prbreq_wps_ie_data_t;

typedef struct sta_prbreq_wps_ie_list
{
    uint32_t totLen;
    uint8_t ieDataList[1];
} sta_prbreq_wps_ie_list_t;

/* EDCF related items from 802.11.h */

/* ACI from 802.11.h */
#define EDCF_AIFSN_MIN               1           /* AIFSN minimum value */
#define EDCF_AIFSN_MAX               15          /* AIFSN maximum value */
#define EDCF_AIFSN_MASK              0x0f        /* AIFSN mask */
#define EDCF_ACM_MASK                0x10        /* ACM mask */
#define EDCF_ACI_MASK                0x60        /* ACI mask */
#define EDCF_ACI_SHIFT               5           /* ACI shift */
#define EDCF_AIFSN_SHIFT             12          /* 4 MSB(0xFFF) in ifs_ctl for AC idx */

/* ECW from 802.11.h */
#define EDCF_ECW_MIN                 0           /* cwmin/cwmax exponent minimum value */
#define EDCF_ECW_MAX                 15          /* cwmin/cwmax exponent maximum value */
#define EDCF_ECW2CW(exp)             ( (1 << (exp) ) - 1 )
#define EDCF_ECWMIN_MASK             0x0f        /* cwmin exponent form mask */
#define EDCF_ECWMAX_MASK             0xf0        /* cwmax exponent form mask */
#define EDCF_ECWMAX_SHIFT            4           /* cwmax exponent form shift */

/* TXOP from 802.11.h */
#define EDCF_TXOP_MIN                0           /* TXOP minimum value */
#define EDCF_TXOP_MAX                65535       /* TXOP maximum value */
#define EDCF_TXOP2USEC(txop)         ( (txop) << 5 )

#define EDCF_ACPARAM
struct edcf_acparam
{
    uint8_t ACI;
    uint8_t ECW;
    uint16_t TXOP;     /* stored in network order (ls octet first) */
};
typedef struct edcf_acparam edcf_acparam_t;

/* Stop packing structures */
#pragma pack()

/* discovery state */
typedef struct wl_p2p_disc_st
{
    uint8_t state;     /* see p2p_discovery_state_t */
    uint16_t chanspec;     /* valid in listen state */
    uint16_t dwell_time_ms;     /* valid in listen state, in ms */
} wl_p2p_disc_st_t;

/* scan request */
typedef struct wl_p2p_scan
{
    uint8_t type;     /* 'S' for WLC_SCAN, 'E' for "escan" */
    uint8_t reserved[3];
    /* scan or escan parms... */
} wl_p2p_scan_t;

/* escan request */
typedef struct
{
    uint8_t type;     /* 'S' for WLC_SCAN, 'E' for "escan" */
    uint8_t reserved[3];

    /* escan params */
    wl_escan_params_t escan;
} wl_p2p_escan_t;

/* i/f request */
typedef struct wl_p2p_if
{
    struct ether_addr mac_address;
    uint8_t interface_type;
    uint16_t chan_spec;
} wl_p2p_if_t;

/* i/f query */
typedef struct wl_p2p_ifq
{
    uint32_t bsscfgidx;
    char ifname[16];
} wl_p2p_ifq_t;

/* OppPS & CTWindow */
typedef struct wl_p2p_ops
{
    uint8_t ops;     /* 0: disable 1: enable */
    uint8_t ctw;     /* >= 10 */
} wl_p2p_ops_t;

/* absence and presence request */
typedef struct wl_p2p_sched_desc
{
    uint32_t start;
    uint32_t interval;
    uint32_t duration;
    uint32_t count;     /* see count */
} wl_p2p_sched_desc_t;

typedef struct wl_p2p_sched
{
    uint8_t type;     /* see schedule type */
    uint8_t action;     /* see schedule action */
    uint8_t option;     /* see schedule option */
    wl_p2p_sched_desc_t desc[1];
} wl_p2p_sched_t;

/* schedule type */
#define WL_P2P_SCHED_TYPE_ABS           0       /* Scheduled Absence */
#define WL_P2P_SCHED_TYPE_REQ_ABS       1       /* Requested Absence */
#define WL_P2P_SCHED_TYPE_REQ_PSC       2       /* Requested Presence */

/* schedule action during absence periods (for WL_P2P_SCHED_ABS type) */
#define WL_P2P_SCHED_ACTION_NONE        0       /* no action */
#define WL_P2P_SCHED_ACTION_DOZE        1       /* doze */
#define WL_P2P_SCHED_ACTION_RESET       255     /* reset */

/* schedule option - WL_P2P_SCHED_TYPE_ABS */
#define WL_P2P_SCHED_OPTION_NORMAL      0       /* normal start/interval/duration/count in time */
#define WL_P2P_SCHED_OPTION_BCNPCT      1       /* percentage of beacon interval */

typedef struct wl_p2p_wfds_hash
{
    uint32_t advt_id;
    uint16_t nw_cfg_method;
    uint8_t wfds_hash[6];
    uint8_t name_len;
    uint8_t service_name[MAX_WFDS_SVC_NAME_LEN];
} wl_p2p_wfds_hash_t;

#define P2P_IP_ALLOC_LEN            12
/* Definitions for Reliable Multicast */
/* NOTE: RMC structures should not be packed */

#define WL_RMC_CNT_VERSION         (1)
#define WL_RMC_TR_VERSION          (1)
#define WL_RMC_MAX_CLIENT          (32)
#define WL_RMC_FLAG_INBLACKLIST    (1)
#define WL_RMC_FLAG_ACTIVEACKER    (2)
#define WL_RMC_FLAG_RELMCAST       (4)
#define WL_RMC_FLAG_MASTER_TX      (8)
#define WL_RMC_MAX_TABLE_ENTRY     (8)

#define WL_RMC_VER                 (1)
#define WL_RMC_INDEX_ACK_ALL       (255)
#define WL_RMC_NUM_OF_MC_STREAMS   (4)
#define WL_RMC_MAX_TRS_PER_GROUP   (1)
#define WL_RMC_MAX_TRS_IN_ACKALL   (1)
#define WL_RMC_ACK_MCAST0          (0x02)
#define WL_RMC_ACK_MCAST_ALL       (0x01)
#define WL_RMC_ACTF_TIME_MIN       (300)     /* time in ms */
#define WL_RMC_ACTF_TIME_MAX       (20000)   /* time in ms */
#define WL_RMC_MAX_NUM_TRS         (32)      /* maximun transmitters allowed */
#define WL_RMC_ARTMO_MIN           (350)     /* time in ms */
#define WL_RMC_ARTMO_MAX           (40000)   /* time in ms */

enum rmc_opcodes
{
    RELMCAST_ENTRY_OP_DISABLE = 0,     /* Disable multi-cast group */
    RELMCAST_ENTRY_OP_DELETE = 1,     /* Delete multi-cast group */
    RELMCAST_ENTRY_OP_ENABLE = 2,     /* Enable multi-cast group */
    RELMCAST_ENTRY_OP_ACK_ALL = 3     /* Enable ACK ALL bit in AMT */
};

/* RMC operational modes */
enum rmc_modes
{
    WL_RMC_MODE_RECEIVER = 0,     /* Receiver mode by default */
    WL_RMC_MODE_TRANSMITTER = 1,     /* Transmitter mode using wl ackreq */
    WL_RMC_MODE_INITIATOR = 2     /* Initiator mode using wl ackreq */
};

/* Each RMC mcast client info */
typedef struct wl_relmcast_client
{
    uint8_t flag;     /* status of client such as AR, R, or blacklisted */
    int16_t rssi;     /* rssi value of RMC client */
    struct ether_addr addr;     /* mac address of RMC client */
} wl_relmcast_client_t;

/* RMC Counters */
typedef struct wl_rmc_cnts
{
    uint16_t version;     /* see definition of WL_CNT_T_VERSION */
    uint16_t length;     /* length of entire structure */
    uint16_t dupcnt;     /* counter for duplicate rmc MPDU */
    uint16_t ackreq_err;     /* counter for wl ackreq error    */
    uint16_t af_tx_err;     /* error count for action frame transmit   */
    uint16_t null_tx_err;     /* error count for rmc null frame transmit */
    uint16_t af_unicast_tx_err;     /* error count for rmc unicast frame transmit */
    uint16_t mc_no_amt_slot;     /* No mcast AMT entry available */
    /* Unused. Keep for rom compatibility */
    uint16_t mc_no_glb_slot;     /* No mcast entry available in global table */
    uint16_t mc_not_mirrored;     /* mcast group is not mirrored */
    uint16_t mc_existing_tr;     /* mcast group is already taken by transmitter */
    uint16_t mc_exist_in_amt;     /* mcast group is already programmed in amt */
    /* Unused. Keep for rom compatibility */
    uint16_t mc_not_exist_in_gbl;     /* mcast group is not in global table */
    uint16_t mc_not_exist_in_amt;     /* mcast group is not in AMT table */
    uint16_t mc_utilized;     /* mcast addressed is already taken */
    uint16_t mc_taken_other_tr;     /* multi-cast addressed is already taken */
    uint32_t rmc_rx_frames_mac;     /* no of mc frames received from mac */
    uint32_t rmc_tx_frames_mac;     /* no of mc frames transmitted to mac */
    uint32_t mc_null_ar_cnt;     /* no. of times NULL AR is received */
    uint32_t mc_ar_role_selected;     /* no. of times took AR role */
    uint32_t mc_ar_role_deleted;     /* no. of times AR role cancelled */
    uint32_t mc_noacktimer_expired;     /* no. of times noack timer expired */
    uint16_t mc_no_wl_clk;     /* no wl clk detected when trying to access amt */
    uint16_t mc_tr_cnt_exceeded;     /* No of transmitters in the network exceeded */
} wl_rmc_cnts_t;

/* RMC Status */
typedef struct wl_relmcast_st
{
    uint8_t ver;     /* version of RMC */
    uint8_t num;     /* number of clients detected by transmitter */
    wl_relmcast_client_t clients[WL_RMC_MAX_CLIENT];
    uint16_t err;     /* error status (used in infra) */
    uint16_t actf_time;     /* action frame time period */
} wl_relmcast_status_t;

/* Entry for each STA/node */
typedef struct wl_rmc_entry
{
    /* operation on multi-cast entry such add,
     * delete, ack-all
     */
    int8_t flag;
    struct ether_addr addr;     /* multi-cast group mac address */
} wl_rmc_entry_t;

/* RMC table */
typedef struct wl_rmc_entry_table
{
    uint8_t index;     /* index to a particular mac entry in table */
    uint8_t opcode;     /* opcodes or operation on entry */
    wl_rmc_entry_t entry[WL_RMC_MAX_TABLE_ENTRY];
} wl_rmc_entry_table_t;

typedef struct wl_rmc_trans_elem
{
    struct ether_addr tr_mac;     /* transmitter mac */
    struct ether_addr ar_mac;     /* ar mac */
    uint16_t artmo;     /* AR timeout */
    uint8_t amt_idx;     /* amt table entry */
    uint16_t flag;     /* entry will be acked, not acked, programmed, full etc */
} wl_rmc_trans_elem_t;

/* RMC transmitters */
typedef struct wl_rmc_trans_in_network
{
    uint8_t ver;     /* version of RMC */
    uint8_t num_tr;     /* number of transmitters in the network */
    wl_rmc_trans_elem_t trs[WL_RMC_MAX_NUM_TRS];
} wl_rmc_trans_in_network_t;

/* To update vendor specific ie for RMC */
typedef struct wl_rmc_vsie
{
    uint8_t oui[3];
    uint16_t payload;     /* IE Data Payload */
} wl_rmc_vsie_t;

/* WLC_E_ULP event data */
#define WL_ULP_EVENT_VERSION        1
#define WL_ULP_DISABLE_CONSOLE      1   /* Disable console message on ULP entry */
#define WL_ULP_UCODE_DOWNLOAD       2   /* Download ULP ucode file */
#define WL_ULP_ENTRY                3   /* inform ulp entry to Host during warmboot */

typedef struct wl_ulp_event
{
    uint16_t version;
    uint16_t ulp_dongle_action;
} wl_ulp_event_t;

/* clm download */
#define DLOAD_HANDLER_VER   1   /* Downloader version */
#define DLOAD_FLAG_VER_MASK 0xf000  /* Downloader version mask */
#define DLOAD_FLAG_VER_SHIFT    12  /* Downloader version shift */

#define DL_CRC_NOT_INUSE    0x0001
#define DL_BEGIN        0x0002
#define DL_END          0x0004

enum
{
    DL_TYPE_UCODE = 1, DL_TYPE_CLM = 2
};

struct wl_dload_data
{
    uint16_t flag;
    uint16_t dload_type;
    uint32_t len;
    uint32_t crc;
    uint8_t data[1];
};
typedef struct wl_dload_data wl_dload_data_t;

struct wl_clm_dload_info
{
    uint32_t ds_id;
    uint32_t clm_total_len;
    uint32_t num_chunks;
    uint32_t chunk_len;
    uint32_t chunk_offset;
    uint8_t data_chunk[1];
};
typedef struct wl_clm_dload_info wl_clm_dload_info_t;

#pragma pack(1)
typedef struct mesh_peer_info
{
    uint16_t mesh_peer_prot_id;
    uint16_t local_link_id;
    uint16_t peer_link_id;
    uint16_t peer_aid;
    uint8_t state;
} mesh_peer_info_t;

typedef struct mesh_peer_info_ext
{
    mesh_peer_info_t peer_info;
    uint16_t local_aid;
    struct ether_addr ea;
    uint32_t entry_state;
    int32_t rssi;
} mesh_peer_info_ext_t;

typedef struct mesh_peer_info_dump
{
    uint32_t buflen;
    uint32_t version;
    uint32_t count;     /* number of results */
    mesh_peer_info_ext_t mpi_ext[1];
} mesh_peer_info_dump_t;

#define WL_CHANSPEC_CTL_SB_LLL      0x0000
#define WL_CHANSPEC_CTL_SB_LLU      0x0100
#define WL_CHANSPEC_CTL_SB_LUL      0x0200
#define WL_CHANSPEC_CTL_SB_LUU      0x0300
#define WL_CHANSPEC_CTL_SB_ULL      0x0400
#define WL_CHANSPEC_CTL_SB_ULU      0x0500
#define WL_CHANSPEC_CTL_SB_UUL      0x0600
#define WL_CHANSPEC_CTL_SB_UUU      0x0700
#define WL_CHANSPEC_CTL_SB_LL       WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_LU       WL_CHANSPEC_CTL_SB_LLU
#define WL_CHANSPEC_CTL_SB_UL       WL_CHANSPEC_CTL_SB_LUL
#define WL_CHANSPEC_CTL_SB_UU       WL_CHANSPEC_CTL_SB_LUU
#define WL_CHANSPEC_CTL_SB_L        WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_U        WL_CHANSPEC_CTL_SB_LLU
#define INVCHANSPEC            255
#define WL_CHANSPEC_CTL_SB_SHIFT         8
#define WL_CHANSPEC_BW_5            0x0000
#define WL_CHANSPEC_BW_80        0x2000
#define WL_CHANSPEC_BW_160       0x2800
#define WL_CHANSPEC_BW_8080      0x3000

struct ether_addr;
struct wl_join_scan_params;

typedef struct wl_assoc_params
{
    struct ether_addr bssid;
    uint16_t bssid_cnt;
    uint32_t chanspec_num;
    chanspec_t chanspec_list[1];
} wl_assoc_params_t;
#define WL_ASSOC_PARAMS_FIXED_SIZE     (sizeof(wl_assoc_params_t) - sizeof(wl_chanspec_t) )
typedef wl_assoc_params_t wl_reassoc_params_t;
#define WL_REASSOC_PARAMS_FIXED_SIZE    WL_ASSOC_PARAMS_FIXED_SIZE
typedef wl_assoc_params_t wl_join_assoc_params_t;
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE        WL_ASSOC_PARAMS_FIXED_SIZE
typedef struct wl_join_params
{
    wlc_ssid_t ssid;
    struct wl_assoc_params params;
} wl_join_params_t;
#define WL_JOIN_PARAMS_FIXED_SIZE     (sizeof(wl_join_params_t) - sizeof(wl_chanspec_t) )

/* extended join params */
typedef struct wl_extjoin_params
{
    wlc_ssid_t ssid;     /* {0, ""}: wildcard scan */
    struct wl_join_scan_params scan_params;
    wl_join_assoc_params_t assoc_params;     /* optional field, but it must include the fixed portion
                                              * of the wl_join_assoc_params_t struct when it does
                                              * present.
                                              */
} wl_extjoin_params_t;
#define WL_EXTJOIN_PARAMS_FIXED_SIZE    (sizeof(wl_extjoin_params_t) - sizeof(chanspec_t) )

#define WLC_SET_CUSTOM_COUNTRY             ( (uint32_t)320 )

/* WLC_GET_AUTH, WLC_SET_AUTH values */
#define WL_AUTH_OPEN_SYSTEM     0   /* d11 open authentication */
#define WL_AUTH_SHARED_KEY      1   /* d11 shared authentication */
#define WL_AUTH_OPEN_SHARED     2   /* try open, then shared if open failed w/rc 13 */
#define WL_AUTH_SAE             3   /* Simultaneous Authentication of Equals (SAE) */

/** ARP offload statistics */
struct whd_arp_stats_s
{
    uint32_t version;               /**< WLAN version											*/
    uint32_t peerage;               /**< Current peer age-out time								*/
    uint32_t arpoe;                 /**< Agent enabled state									*/
    uint32_t features_enabled;      /**< ARP Feature Flags @ref CY_ARP_OL_AGENT_ENABLE, etc.	*/
    arp_ol_stats_t stats;   /**< Current stats											*/
    uint32_t host_ip_list[ARP_MULTIHOMING_MAX];     /**< host_ip addresses in one call			*/
};

#pragma pack()

/* TCP Keepalive offload related defintions should not be added inside
 * pragma pack to avoid functionality breakage.
 */
/*
 * TCP keepalive offload definitions
 */
#define MAX_TKO_CONN                 4
#define IPV4_ADDR_LEN                4      /* IPV4 address length   */

/* Default TCP Keepalive retry parameters.  */
#define TCP_KEEPALIVE_OFFLOAD_INTERVAL_SEC       (20)
#define TCP_KEEPALIVE_OFFLOAD_RETRY_COUNT        (3)
#define TCP_KEEPALIVE_OFFLOAD_RETRY_INTERVAL_SEC (3)

/* common iovar struct */
typedef struct wl_tko
{
    uint16_t subcmd_id;       /* subcommand id */
    uint16_t len;             /* total length of data[] */
    uint8_t data[1];          /* subcommand data */
} wl_tko_t;

/* subcommand ids */
#define WL_TKO_SUBCMD_MAX_TCP           0       /* max TCP connections supported */
#define WL_TKO_SUBCMD_PARAM             1       /* configure offload common parameters  */
#define WL_TKO_SUBCMD_CONNECT           2       /* TCP connection info */
#define WL_TKO_SUBCMD_ENABLE            3       /* enable/disable */
#define WL_TKO_SUBCMD_STATUS            4       /* TCP connection status */

/* WL_TKO_SUBCMD_MAX_CONNECT subcommand data */
typedef struct wl_tko_max_tcp
{
    uint8_t max;      /* max TCP connections supported */
    uint8_t pad[3];   /* 4-byte struct alignment */
} wl_tko_max_tcp_t;

/* WL_TKO_SUBCMD_PARAM subcommand data */
typedef struct wl_tko_param
{
    uint16_t interval;        /* keepalive tx interval (secs) */
    uint16_t retry_interval;  /* keepalive retry interval (secs) */
    uint16_t retry_count;     /* retry_count */
    uint8_t pad[2];           /* 4-byte struct alignment */
} wl_tko_param_t;

/* WL_TKO_SUBCMD_CONNECT subcommand data
 * invoke with unique 'index' for each TCP connection
 */
typedef struct wl_tko_connect
{
    uint8_t index;            /* TCP connection index, 0 to max-1 */
    uint8_t ip_addr_type;     /* 0 - IPv4, 1 - IPv6 */
    uint16_t local_port;      /* local port */
    uint16_t remote_port;     /* remote port */
    uint32_t local_seq;       /* local sequence number */
    uint32_t remote_seq;      /* remote sequence number */
    uint16_t request_len;     /* TCP keepalive request packet length */
    uint16_t response_len;    /* TCP keepalive response packet length */
    uint8_t data[1];          /* variable length field containing local/remote IPv4/IPv6,
                               * TCP keepalive request packet, TCP keepalive response packet
                               *	  For IPv4, length is 4 * 2 + request_length + response_length
                               *		 offset 0 - local IPv4
                               *		 offset 4 - remote IPv4
                               *		 offset 8 - TCP keepalive request packet
                               *		 offset 8+request_length - TCP keepalive response packet
                               *	  For IPv6, length is 16 * 2 + request_length + response_length
                               *		 offset 0 - local IPv6
                               *		 offset 16 - remote IPv6
                               *		 offset 32 - TCP keepalive request packet
                               *		 offset 32+request_length - TCP keepalive response packet
                               */
} wl_tko_connect_t;

/* WL_TKO_SUBCMD_CONNECT subcommand data to GET configured info for specific index */
typedef struct wl_tko_get_connect
{
    uint8_t index;            /* TCP connection index, 0 to max-1 */
    uint8_t pad[3];           /* 4-byte struct alignment */
} wl_tko_get_connect_t;

typedef struct wl_tko_enable
{
    uint8_t enable;   /* 1 - enable, 0 - disable */
    uint8_t pad[3];   /* 4-byte struct alignment */
} wl_tko_enable_t;

/* WL_TKO_SUBCMD_STATUS subcommand data */
/* must be invoked before tko is disabled else status is unavailable */
typedef struct wl_tko_status
{
    uint8_t count;        /* number of status entries (i.e. equals
                           * max TCP connections supported)
                           */
    uint8_t status[1];    /* variable length field contain status for
                           * each TCP connection index
                           */
} wl_tko_status_t;
/** Get/Set TKO intervals & retrys */
struct whd_tko_retry
{
    uint16_t tko_interval;          /**< How often to send (in seconds) */
    uint16_t tko_retry_count;       /**< Max times to retry if original fails */
    uint16_t tko_retry_interval;    /**< Wait time between retries (in seconds) */
};

/** Status values used in conjunction with whd_tko_status_t */
typedef enum
{
    TKO_STATUS_NORMAL                       = 0,    /**< TCP connection normal, no error */
    TKO_STATUS_NO_RESPONSE                  = 1,    /**< no response to TCP keepalive */
    TKO_STATUS_NO_TCP_ACK_FLAG              = 2,    /**< TCP ACK flag not set */
    TKO_STATUS_UNEXPECT_TCP_FLAG            = 3,    /**< unexpect TCP flags set other than ACK */
    TKO_STATUS_SEQ_NUM_INVALID              = 4,    /**< ACK != sequence number */
    TKO_STATUS_REMOTE_SEQ_NUM_INVALID       = 5,    /**< SEQ > remote sequence number */
    TKO_STATUS_TCP_DATA                     = 6,    /**< TCP data available */
    TKO_STATUS_UNAVAILABLE                  = 255,  /**< not used/configured */
} tko_status_t;

/** Status of each TCP connection */
struct whd_tko_status
{
    uint8_t count;              /**< number of status entries */
    uint8_t status[MAX_TKO_CONN];   /**< each TCP status */
};

/** Struct to query FW for current TKO configuation */
struct whd_tko_connect
{
    uint8_t index;            /**< TCP connection index, 0 to max-1 */
    uint8_t ip_addr_type;     /**< 0 - IPv4, 1 - IPv6 */
    uint16_t local_port;      /**< local port */
    uint16_t remote_port;     /**< remote port */
    uint32_t local_seq;       /**< local sequence number */
    uint32_t remote_seq;      /**< remote sequence number */
    uint16_t request_len;     /**< TCP keepalive request packet length */
    uint16_t response_len;    /**< TCP keepalive response packet length */
    uint8_t data[1];          /**< variable length field containing local/remote IPv4/IPv6,
                               * TCP keepalive request packet, TCP keepalive response packet
                               *	  For IPv4, length is 4 * 2 + request_length + response_length
                               *		 offset 0 - local IPv4
                               *		 offset 4 - remote IPv4
                               *		 offset 8 - TCP keepalive request packet
                               *		 offset 8+request_length - TCP keepalive response packet
                               *	  For IPv6, length is 16 * 2 + request_length + response_length
                               *		 offset 0 - local IPv6
                               *		 offset 16 - remote IPv6
                               *		 offset 32 - TCP keepalive request packet
                               *		 offset 32+request_length - TCP keepalive response packet
                               */
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

