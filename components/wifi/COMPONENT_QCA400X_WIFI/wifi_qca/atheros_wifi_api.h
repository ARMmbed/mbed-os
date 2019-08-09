//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef __ATHEROS_WIFI_API_H__
#define __ATHEROS_WIFI_API_H__

/* PORT_NOTE: atheros_wifi_api.h is intended to expose user level API's and
 *  data structures.  These structures provide access to the driver that
 *  would not otherwise be provided by the system's existing driver interface.
 *  It is imagined that the existing driver interface would have some form
 *  of IO_CTRL API with pre-defined commands. These commands are intended to
 *  extend those provided by the default.  It is further imagined that in
 *  such a system the developer would add an IO_CTRL for Vendor specific
 *  commands which would act as a wrapper for the ath_ioctl_cmd commands
 *  defined below. */

#include <a_types.h>

enum ath_ioctl_cmd
{
    ATH_SET_TXPWR,
    ATH_SET_PMPARAMS,
    ATH_SET_LISTEN_INT,
    ATH_SET_CIPHER,
    ATH_SET_SEC_MODE,
    ATH_SET_PHY_MODE,
    ATH_GET_PHY_MODE,
    ATH_GET_RX_RSSI,                /* output uint32_t */
    ATH_SET_CONNECT_STATE_CALLBACK, /* input ATH_CONNECT_CB */
    ATH_DEVICE_SUSPEND_ENABLE,      /* input NONE */
    ATH_DEVICE_SUSPEND_START,       /* */
    ATH_SET_PMK,
    ATH_GET_PMK,
    ATH_GET_VERSION,
    ATH_START_WPS,
    ATH_AWAIT_WPS_COMPLETION,
    ATH_SCAN_CTRL,
    ATH_CHIP_STATE,
    ATH_MAC_TX_RAW,
    ATH_SET_CHANNEL,
    ATH_SET_AGGREGATION,
    ATH_ASSERT_DUMP,
    ATH_SET_SSID,
    ATH_SET_CONNECT,
    ATH_SET_SEC_TYPE,
    ATH_SET_COMMIT,
    ATH_SET_MODE,
    ATH_SET_FREQ,
    ATH_SET_RTS,
    ATH_SET_PASSPHRASE,
    ATH_SET_SCAN,
    ATH_GET_SCAN_RESULTS,
    ATH_SET_ENCODE,
    ATH_SET_POWER,
    ATH_GET_POWER,
    ATH_GET_ESSID,
    ATH_GET_SEC_TYPE,
    ATH_PROGRAM_FLASH,
    ATH_EXECUTE_FLASH,
    ATH_GET_MACADDR,
    ATH_IS_DRIVER_INITIALIZED,
    ATH_GET_TX_STATUS,
    ATH_SET_PROMISCUOUS_MODE,
    ATH_GET_REG_DOMAIN,
    ATH_START_SCAN_EXT,
    ATH_GET_SCAN_EXT,
    ATH_GET_LAST_ERROR,
    ATH_GET_CHANNEL,
    ATH_CONFIG_AP,
    ATH_P2P_CONNECT,
    ATH_P2P_CONNECT_CLIENT,
    ATH_P2P_FIND,
    ATH_P2P_LISTEN,
    ATH_P2P_CANCEL,
    ATH_P2P_STOP,
    ATH_P2P_JOIN,
    ATH_P2P_NODE_LIST,
    ATH_P2P_SET_CONFIG,
    ATH_P2P_WPS_CONFIG,
    ATH_P2P_AUTH,
    ATH_P2P_DISC_REQ,
    ATH_P2P_SET,
    ATH_P2P_INVITE_AUTH,
    ATH_P2P_PERSISTENT_LIST,
    ATH_P2P_INVITE,
    ATH_P2P_INV_CONNECT,
    ATH_P2P_JOIN_PROFILE,
    ATH_P2P_APMODE,
    ATH_P2P_APMODE_PP,
    ATH_P2P_SWITCH,
    ATH_P2P_SET_NOA,
    ATH_P2P_SET_OPPPS,
    ATH_P2P_SDPD,
    ATH_SET_P2P_CALLBACK,
    ATH_P2P_EVENT_CALLBACK,
    ATH_ONOFF_LPL,
    ATH_ONOFF_GTX,
    ATH_PROGRAM_MAC_ADDR,
    ATH_GET_RATE,
    ATH_SET_RATE,
    ATH_HTC_REVERSE_CREDITS_INIT,
    ATH_GPIO_CMD,
    ATH_SET_TX_PWR_SCALE,

    ATH_PFM_CMD,
    ATH_DIAG_CMD,
    ATH_PMU_SET_PARAMS,
    ATH_SET_DEVICE_ID,
    ATH_GET_VERSION_STR,
    ATH_GET_CONC_DEV_CHANNEL,
    ATH_DSET_READ_CMD,
    ATH_DSET_WRITE_CMD,

    ATH_DSET_OP_CMD,
    ATH_GET_TEMPERATURE,
    ATH_ROAM_CTRL,
    ATH_GET_CHANNELHINT,
    ATH_SET_OP_MODE,
    ATH_GET_OP_MODE,
    ATH_SET_WEPKEY,
    ATH_GET_WEPKEY,
    ATH_SET_WEPINDEX,
    ATH_GET_WEPINDEX,
    ATH_SET_SCAN_PARAM,
    ATH_SET_PARAM,
    ATH_SET_PROBEREQ_CB,
    ATH_SET_PROBEREQ_EV_ENABLE,
    ATH_PROGRAM_COUNTRY_CODE,
    ATH_GET_COUNTRY_CODE,
    ATH_SET_APPIE,
    ATH_WLAN_WPS_INIT_KEY,
    ATH_HEARTBEAT_CHALLEANGE,
    /************************************/
    /* add new commands above this line */
    /************************************/
    ATH_CMD_LAST /* NOTE: ensure that this is the last entry in the enum */
};

// IEEE 802.11 channels in MHz										Geographies where channel is allowed
#define ATH_IOCTL_FREQ_1 (2412)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_2 (2417)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_3 (2422)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_4 (2427)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_5 (2432)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_6 (2437)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_7 (2442)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_8 (2447)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_9 (2452)   // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_10 (2457)  // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_11 (2462)  // USA, Canada, Europe, Japan
#define ATH_IOCTL_FREQ_12 (2467)  // Europe, Japan
#define ATH_IOCTL_FREQ_13 (2472)  // Europe, Japan
#define ATH_IOCTL_FREQ_14 (2484)  // Japan
#define ATH_IOCTL_FREQ_36 (5180)  // 11 A channels
#define ATH_IOCTL_FREQ_40 (5200)  // 11 A channels
#define ATH_IOCTL_FREQ_44 (5220)  // 11 A channels
#define ATH_IOCTL_FREQ_48 (5240)  // 11 A channels
#define ATH_IOCTL_FREQ_52 (5260)  // 11 A channels
#define ATH_IOCTL_FREQ_56 (5280)  // 11 A channels
#define ATH_IOCTL_FREQ_60 (5300)  // 11 A channels
#define ATH_IOCTL_FREQ_64 (5320)  // 11 A channels
#define ATH_IOCTL_FREQ_100 (5500) // 11 A channels
#define ATH_IOCTL_FREQ_104 (5520) // 11 A channels
#define ATH_IOCTL_FREQ_108 (5540) // 11 A channels
#define ATH_IOCTL_FREQ_112 (5560) // 11 A channels
#define ATH_IOCTL_FREQ_116 (5580) // 11 A channels
#define ATH_IOCTL_FREQ_132 (5660) // 11 A channels
#define ATH_IOCTL_FREQ_136 (5680) // 11 A channels
#define ATH_IOCTL_FREQ_140 (5700) // 11 A channels
#define ATH_IOCTL_FREQ_149 (5745) // 11 A channels
#define ATH_IOCTL_FREQ_153 (5765) // 11 A channels
#define ATH_IOCTL_FREQ_157 (5785) // 11 A channels
#define ATH_IOCTL_FREQ_161 (5805) // 11 A channels
#define ATH_IOCTL_FREQ_165 (5825) // 11 A channels

#define ATH_PMK_LEN (32)
#define ATH_PASSPHRASE_LEN (64)
#define ATH_MAX_SSID_LENGTH (32)
#define ATH_MAX_SCAN_CHANNELS (16)
#define ATH_MAX_SCAN_BUFFERS (12)

#if DRIVER_CONFIG_ENABLE_STORE_RECALL
#define STORE_RECALL_BUF_SIZE (1400) // If store-recall is enabled, allocate shared buffer
// big enough to hold 1400 bytes of target data
#else
#define STORE_RECALL_BUF_SIZE (0)
#endif

#define ATH_WPS_PIN_LEN (9)
#define ATH_WPS_MODE_PIN (1)
#define ATH_WPS_MODE_PUSHBUTTON (2)
#define WEP_SHORT_KEY (5)
#define WEP_LONG_KEY (13)
#define ATH_ACTIVE_CHAN_DWELL_TIME (60) // Max Dwell time per channel

/*Structure definition for passing Atheros specific data from App*/
typedef struct ath_ioctl_params
{
    uint16_t cmd_id;
    void *data;
    uint16_t length;
} ATH_IOCTL_PARAM_STRUCT, *ATH_IOCTL_PARAM_STRUCT_PTR;

typedef struct ath_scan_list
{
    uint32_t num_scan_entries;
    void *scan_list;
} ATH_SCAN_LIST, *ATH_SCAN_LIST_PTR;

typedef struct ath_ap_params
{
    uint16_t cmd_subset;
    void *data;
} ATH_AP_PARAM_STRUCT, *ATH_AP_PARAM_STRUCT_PTR;

// AP mode sub commands
#define AP_SUB_CMD_BCON_INT 0x0001
#define AP_SUB_CMD_HIDDEN_FLAG 0x0002
#define AP_SUB_CMD_INACT_TIME 0x0003
#define AP_SUB_CMD_SET_COUNTRY 0x0004
#define AP_SUB_CMD_WPS_FLAG 0x0005
#define AP_SUB_CMD_DTIM_INT 0x0006
#define AP_SUB_CMD_PSBUF 0x0007
/* A-MPDU aggregation is enabled on a per TID basis where each TID (0-7)
 * represents a different traffic priority.  The mapping to WMM access categories
 * is as follows; WMM Best effort = TID 0-1
 *				  WMM Background  = TID 2-3
 *				  WMM Video		  = TID 4-5
 *				  WMM Voice 	  = TID 6-7
 * Once enabled A-MPDU aggregation may be negotiated with an Access Point/Peer
 * device and then both devices may optionally use A-MPDU aggregation for
 * transmission.  Due to other bottle necks in the data path a system may not
 * get improved performance by enabling A-MPDU aggregation.
 */
typedef struct
{
    uint16_t txTIDMask; /* bit mask to enable tx A-MPDU aggregation */
    uint16_t rxTIDMask; /* bit mask to enable rx A-MPDU aggregation */
} ATH_SET_AGGREGATION_PARAM;

#define ATH_CIPHER_TYPE_TKIP 0x04
#define ATH_CIPHER_TYPE_CCMP 0x08
#define ATH_CIPHER_TYPE_WEP 0x02

typedef struct cipher
{
    uint32_t ucipher;
    uint32_t mcipher;
} cipher_t;

typedef void (*ATH_CONNECT_CB)(int value, uint8_t devId, uint8_t *bssid, boolean bssConn);
typedef void (*WMI_HTTP_POST_EVENT_CB)(void *cxt, void *buf);
typedef void (*ATH_OTA_CB)(uint32_t ota_cmd, uint32_t resp_code, uint32_t result);
typedef void (*ATH_PROBEREQ_CB)(void *buf, int len, int freq);

typedef struct _wepkeys
{
    uint8_t defKeyIndex; /* tx key index */
    uint8_t keyLength;   /* must be one of WEP_SHORT_KEY || WEP_LONG_KEY */
    uint8_t numKeys;     /* how many of the 4 keys below are populated */
    char *keys[4];       /* keys */
} ATH_WEPKEYS, *ATH_WEPKEYS_PTR;

typedef struct
{
    uint8_t ssid[32];
    uint8_t macaddress[6];
    uint16_t channel;
    uint8_t ssid_len;
} WPS_SCAN_LST_ENTRY;

typedef struct
{
    uint32_t host_ver;
    uint32_t target_ver;
    uint32_t wlan_ver;
    uint32_t abi_ver;
} ATH_VERSION, *ATH_VERSION_PTR;

typedef struct
{
    uint8_t host_ver[20];
    uint8_t target_ver[20];
    uint8_t wlan_ver[20];
    uint8_t abi_ver[20];
} ATH_VERSION_STR, *ATH_VERSION_STR_PTR;

typedef struct _wps_start
{
    WPS_SCAN_LST_ENTRY ssid_info;
    uint8_t wps_mode; /* ATH_WPS_MODE_PIN | ATH_WPS_MODE_PUSHBUTTON */
    uint8_t timeout_seconds;
    uint8_t connect_flag;
    uint8_t pin[ATH_WPS_PIN_LEN];
    uint8_t pin_length;
} ATH_WPS_START, *ATH_WPS_START_PTR;

typedef struct
{
    uint16_t idle_period; /* msec */
    uint16_t pspoll_number;
    uint16_t dtim_policy;      /*IGNORE_DTIM = 0x01, NORMAL_DTIM = 0x02,STICK_DTIM  = 0x03, AUTO_DTIM = 0x04*/
    uint16_t tx_wakeup_policy; /*TX_WAKEUP_UPON_SLEEP = 1,TX_DONT_WAKEUP_UPON_SLEEP = 2*/
    uint16_t num_tx_to_wakeup;
    uint16_t
        ps_fail_event_policy; /*SEND_POWER_SAVE_FAIL_EVENT_ALWAYS = 1,IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN = 2*/
} ATH_WMI_POWER_PARAMS_CMD;

typedef enum
{
    ATH_WPS_ERROR_INVALID_START_INFO = 0x1,
    ATH_WPS_ERROR_MULTIPLE_PBC_SESSIONS,
    ATH_WPS_ERROR_WALKTIMER_TIMEOUT,
    ATH_WPS_ERROR_M2D_RCVD
} ATH_WPS_ERROR_CODE;

#define ATH_WPS_ERROR_SUCCESS 0x00
#define ATH_WPS_ERROR_INVALID_START_INFO 0x01
#define ATH_WPS_ERROR_MULTIPLE_PBC_SESSIONS 0x02
#define ATH_WPS_ERROR_WALKTIMER_TIMEOUT 0x03
#define ATH_WPS_ERROR_M2D_RCVD 0x04
#define ATH_WPS_ERROR_PWD_AUTH_FAIL 0x05
#define ATH_WPS_ERROR_CANCELLED 0x06
#define ATH_WPS_ERROR_INVALID_PIN 0x07

typedef struct
{
    uint16_t ap_channel;
    int8_t ssid[ATH_MAX_SSID_LENGTH + 1]; /* [OUT] network ssid */
    int16_t ssid_len;                     /* [OUT] number of valid chars in ssid[] */
    cipher_t cipher;                      /* [OUT] network cipher type values not defined */
    uint8_t key_index;                    /* [OUT] for WEP only. key index for tx */
    union
    {/* [OUT] security key or passphrase */
        uint8_t wepkey[ATH_PASSPHRASE_LEN + 1];
        uint8_t passphrase[ATH_PASSPHRASE_LEN + 1];
    } u;

    uint8_t sec_type;   /* [OUT] security type; one of QCA_MEDIACTL_SECURITY_TYPE... */
    uint8_t error;      /* [OUT] error code one of ATH_WPS_ERROR_... */
    uint8_t dont_block; /* [IN] 1 - returns immediately if operation is not complete.
         * 		0 - blocks until operation completes. */
} ATH_NETPARAMS;

#define ATH_DISABLE_BG_SCAN (0x00000001)
#define ATH_DISABLE_FG_SCAN (0x00000002)

typedef struct
{
    uint32_t flags;
} ATH_SCANPARAMS;

typedef struct ath_program_flash
{
    uint8_t *buffer;
    uint32_t load_addr;
    uint16_t length;
    uint32_t result;
} ATH_PROGRAM_FLASH_STRUCT;

/* ATH_MAC_TX_FLAG... are used as TX qualifiers for frames containing WMI_TX_RATE_SCHEDULE in the
 * meta data.  0 or more of these flags should be assigned to the flags member of the schedule. */
#define ATH_MAC_TX_FLAG_ACK 0x01           // frame needs ACK response from receiver
#define ATH_MAC_TX_FLAG_SET_RETRY_BIT 0x02 // device will set retry bit in MAC header for retried frames.
#define ATH_MAC_TX_FLAG_SET_DURATION 0x04  // device will fill duration field in MAC header
                                           /* NOTE: If ATH_MAC_TX_FLAG_USE_PREFIX == 0 device will NOT use prefix frame.
                                            *       If ATH_MAC_TX_FLAG_USE_PREFIX == 1 && ATH_MAC_TX_FLAG_PREFIX_RTS == 0 device will use CTS prefix.
                                            *       If ATH_MAC_TX_FLAG_USE_PREFIX == 1 && ATH_MAC_TX_FLAG_PREFIX_RTS == 1 device will use RTS prefix.
                                            */
#define ATH_MAC_TX_FLAG_USE_PREFIX 0x08    // device will send either RTS or CTS frame prior to subject frame.
#define ATH_MAC_TX_FLAG_PREFIX_RTS 0x10    // device will send RTS and wait for CTS prior to sending subject frame.
#define ATH_MAC_TX_LOAD_TSF 0x20 // device will fill the TSF field during transmit procedure. <Beacons/probe responses>

#define ATH_ACCESS_CAT_BE 0 /* best effort */
#define ATH_ACCESS_CAT_BK 1 /* background */
#define ATH_ACCESS_CAT_VI 2 /* video */
#define ATH_ACCESS_CAT_VO 3 /* voice */

/* set ar4XXX_boot_param according to the desired boot options */
/* AR4XXX_PARAM_MODE_NORMAL - instructs chip to boot normally; load wlan firmware to provide
 *							  WMI services used in normal WIFI operation.
 * AR4XXX_PARAM_MODE_BMI - instructs chip to boot to BMI and await further BMI communication
 *	from host processor.  This option can be used to re-program chip serial flash and
 *	perform other non-standard tasks.
 * AR4XXX_PARAM_QUAD_SPI_FLASH - instructs the chip to access flash chip using QUAD mode
 *  this mode is faster than standard mode but the flash chip must support it. This
 *	option can be OR'd (|) with other options.
 * AR4XXX_PARAM_RAWMODE_BOOT - instructs the chip to boot a firmware image that supports the
 *	RAW TX mode but lacks support for many other WMI commands.  This mode will allow for
 *	a faster boot in those instances where the full set of WMI commands is not necessary.
 *	This mode must be OR'd with AR4XXX_PARAM_NORMAL_BOOT.
 */

#define AR4XXX_PARAM_MODE_NORMAL (0x00000002)
#define AR4XXX_PARAM_MODE_BMI (0x00000003)
#define AR4XXX_PARAM_MODE_MASK (0x0000000f)
#define AR4XXX_PARAM_QUAD_SPI_FLASH (0x80000000)
#define AR4XXX_PARAM_RAWMODE_BOOT (0x00000010 | AR4XXX_PARAM_MODE_NORMAL)
#define AR4XXX_PARAM_MACPROG_MODE (0x00000020 | AR4XXX_PARAM_MODE_NORMAL)
#define AR4XXX_PARAM_MANUFAC_MODE (0x00000030 | AR4XXX_PARAM_MODE_NORMAL)
#define AR4XXX_PARAM_COUNTRY_CODE_MODE (0x00000010 | AR4XXX_PARAM_MODE_NORMAL)

/* combined params for common cases */
#define AR4XXX_PARAM_NORMAL_QUAD (AR4XXX_PARAM_MODE_NORMAL | AR4XXX_PARAM_QUAD_SPI_FLASH)
#define AR4XXX_PARAM_RAW_QUAD (AR4XXX_PARAM_RAWMODE_BOOT | AR4XXX_PARAM_QUAD_SPI_FLASH)

/*The following regulatory domain settings can be passed to target during boot*/

#define AR4XXX_PARAM_REG_DOMAIN_DEFAULT (0x00000000) /*Default regulatory domain*/
#define AR4XXX_PARAM_REG_DOMAIN_1 (0x00000100)       /* FCC3_FCCA reg domain*/
#define AR4XXX_PARAM_REG_DOMAIN_2 (0x00000200)       /* ETSI1_WORLD reg domain*/
#define AR4XXX_PARAM_REG_DOMAIN_3 (0x00000300)       /* MKK5_MKKC reg domain */

/* ATH_MAC_TX_RATE_SCHEDULE - Acts as a host-provided rate schedule to replace what would be normally determined
 * by firmware.  This allows the host to specify what rates and attempts should be used to transmit the
 * frame. */
typedef struct
{
#define ATH_MAC_TX_MAX_RATE_SERIES (4)
    uint8_t rateSeries[ATH_MAC_TX_MAX_RATE_SERIES]; // rate index for each series. first invalid rate terminates series.
    uint8_t trySeries[ATH_MAC_TX_MAX_RATE_SERIES]; // number of tries for each series.
    uint8_t flags;          // combination of ATH_MAC_TX_FLAG...
    uint8_t accessCategory; // should be ATH_ACCESS_CAT_BE for managment frames and multicast frames.
} ATH_MAC_TX_RATE_SCHEDULE;

typedef struct
{
    ATH_MAC_TX_RATE_SCHEDULE rateSched;
    uint8_t pktID; /* The packet ID to identify the tx request */
} ATH_MAC_TX_PARAMS;

typedef struct ath_mac_tx_raw
{
    uint8_t *buffer;          /* pointer to contiguous tx buffer */
    uint16_t length;          /* valid length in bytes of tx buffer */
    ATH_MAC_TX_PARAMS params; /* params governing transmit rules */
} ATH_MAC_TX_RAW_S;

typedef struct ath_tx_status
{
#define ATH_TX_STATUS_IDLE 0x01         /* the TX pipe is 100% idle */
#define ATH_TX_STATUS_HOST_PENDING 0x02 /* the TX pipe has 1 or more frames waiting in the host queue */
#define ATH_TX_STATUS_WIFI_PENDING 0x03 /* the TX pipe has 1 or more frames in process on the WIFI device */
    uint16_t status;                    /* one of ATH_TX_STATUS_ */
} ATH_TX_STATUS;

typedef void (*ATH_PROMISCUOUS_CB)(void *);

typedef struct ath_prom_mode
{
    uint8_t src_mac[6];             /* filter source mac address if desired. */
    uint8_t dst_mac[6];             /* filter destination mac address if desired. */
    uint8_t enable;                 /* 0 to disable promiscuous mode 1 to enable. */
#define ATH_PROM_FILTER_SOURCE 0x01 /* only allow frames whose source mac matches src_mac */
#define ATH_PROM_FILTER_DEST 0x02   /* only allow frames whose destination mac matches dst_mac */
    uint8_t filter_flags;           /* filtering rules */
    ATH_PROMISCUOUS_CB cb;          /* callback function driver will use to feed rx frames */
} ATH_PROMISCUOUS_MODE;

typedef struct ath_reg_domain
{
    uint32_t domain;
} ATH_REG_DOMAIN;

#define SECURITY_AUTH_PSK 0x01
#define SECURITY_AUTH_1X 0x02

typedef struct ath_scan_ext
{
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t rssi;
    uint8_t security_enabled;
    uint16_t beacon_period;
    uint8_t preamble;
    uint8_t bss_type;
    uint8_t bssid[6];
    uint8_t ssid[32];
    uint8_t rsn_cipher;
    uint8_t rsn_auth;
    uint8_t wpa_cipher;
    uint8_t wpa_auth;
} ATH_SCAN_EXT;

typedef struct ath_get_scan
{
    uint16_t num_entries;
    ATH_SCAN_EXT *scan_list;
} ATH_GET_SCAN;

typedef struct ath_program_country_code_addr
{
    uint8_t countryCode[3];
    uint8_t result;
} ATH_PROGRAM_COUNTRY_CODE_PARAM;
/* these result codes are provided in the result field of the ATH_PROGRAM_COUNTRY_CODE_PARAM structure */
/*ATH_PROGRAM_COUNTRY_CODE_RESULT_SUCCESS -
successfully programmed country code address into wifi device
*/
#define ATH_PROGRAM_COUNTRY_CODE_RESULT_SUCCESS (1)
/*ATH_PROGRAM_ COUNTRY_CODE _RESULT_DEV_DENIED -
Device denied the operation for several possible reasons.
The most common reason for this result is that the country code equals
the current country code found already in the device.
Also an invalid country code value can cause this result
*/
#define ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_DENIED (2)
/*ATH_PROGRAM_ COUNTRY_CODE _RESULT_DEV_FAILED -
An error occurred on the device as it tried to program the country code.
*/
#define ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_FAILED (3)
/*ATH_PROGRAM_ COUNTRY_CODE _RESULT_DRIVER_FAILED -
Driver tried but failed to program country code.
Possibly the driver did not have the proper code compiled to perform this operation.
*/
#define ATH_PROGRAM_COUNTRY_CODE_RESULT_DRIVER_FAILED (4)
typedef struct ath_program_mac_addr
{
    uint8_t addr[6];
    uint8_t result;
} ATH_PROGRAM_MAC_ADDR_PARAM;

/* these result codes are provided in the result field of the ATH_PROGRAM_MAC_ADDR_PARAM structure */
/*ATH_PROGRAM_MAC_RESULT_SUCCESS - successfully (re-)programmed mac address into wifi device*/
#define ATH_PROGRAM_MAC_RESULT_SUCCESS (1)
/*ATH_PROGRAM_MAC_RESULT_DEV_DENIED - Device denied the operation for several possible reasons
 * the most common reason for this result is that the mac address equals the current mac
 * address found already in the device.  Also an invalid mac address value can cause this result
 */
#define ATH_PROGRAM_MAC_RESULT_DEV_DENIED (2)
/*ATH_PROGRAM_MAC_RESULT_DEV_FAILED - Device tried but failed to program mac address.
 * An error occurred on the device as it tried to program the mac address.
 */
#define ATH_PROGRAM_MAC_RESULT_DEV_FAILED (3)
/*ATH_PROGRAM_MAC_RESULT_DRIVER_FAILED - Driver tried but failed to program mac address
 * Possibly the driver did not have the proper code compiled to perform this operation.
 */
#define ATH_PROGRAM_MAC_RESULT_DRIVER_FAILED (4)

#define ATH_MAX_IE_LEN  64

typedef struct ath_appie
{
    uint8_t mgmtFrmType; 
    uint8_t ieLen;
    uint8_t *ieInfo;
} ATH_APPIE_PARAM;

/* ATH_API_... return values should be of type ATH_API_RETURN_CODE and
 * should be modified to suit the host sytem. */
#define ATH_API_OK A_OK
#define ATH_API_ERROR A_ERROR
#endif /* __ATHEROS_WIFI_H__ */
