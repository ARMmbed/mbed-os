/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
  @addtogroup BLE_GAP Generic Access Profile (GAP)
  @{
  @brief Definitions and prototypes for the GAP interface.
 */

#ifndef BLE_GAP_H__
#define BLE_GAP_H__

#include "ble_types.h"
#include "ble_ranges.h"
#include "nrf_svc.h"

/**@addtogroup BLE_GAP_ENUMERATIONS Enumerations
 * @{ */

/**@brief GAP API SVC numbers.
 */
enum BLE_GAP_SVCS
{
  SD_BLE_GAP_ADDRESS_SET  = BLE_GAP_SVC_BASE,  /**< Set own Bluetooth Address. */
  SD_BLE_GAP_ADDRESS_GET,                      /**< Get own Bluetooth Address. */
  SD_BLE_GAP_ADV_DATA_SET,                     /**< Set Advertising Data. */
  SD_BLE_GAP_ADV_START,                        /**< Start Advertising. */
  SD_BLE_GAP_ADV_STOP,                         /**< Stop Advertising. */
  SD_BLE_GAP_CONN_PARAM_UPDATE,                /**< Connection Parameter Update. */
  SD_BLE_GAP_DISCONNECT,                       /**< Disconnect. */
  SD_BLE_GAP_TX_POWER_SET,                     /**< Set TX Power. */
  SD_BLE_GAP_APPEARANCE_SET,                   /**< Set Appearance. */
  SD_BLE_GAP_APPEARANCE_GET,                   /**< Get Appearance. */
  SD_BLE_GAP_PPCP_SET,                         /**< Set PPCP. */
  SD_BLE_GAP_PPCP_GET,                         /**< Get PPCP. */
  SD_BLE_GAP_DEVICE_NAME_SET,                  /**< Set Device Name. */
  SD_BLE_GAP_DEVICE_NAME_GET,                  /**< Get Device Name. */
  SD_BLE_GAP_AUTHENTICATE,                     /**< Initiate Pairing/Bonding. */
  SD_BLE_GAP_SEC_PARAMS_REPLY,                 /**< Reply with Security Parameters. */
  SD_BLE_GAP_AUTH_KEY_REPLY,                   /**< Reply with an authentication key. */
  SD_BLE_GAP_ENCRYPT,                          /**< Initiate encryption procedure. */
  SD_BLE_GAP_SEC_INFO_REPLY,                   /**< Reply with Security Information. */
  SD_BLE_GAP_CONN_SEC_GET,                     /**< Obtain connection security level. */
  SD_BLE_GAP_RSSI_START,                       /**< Start reporting of changes in RSSI. */ 
  SD_BLE_GAP_RSSI_STOP,                        /**< Stop reporting of changes in RSSI. */ 
  SD_BLE_GAP_SCAN_START,                       /**< Start Scanning. */
  SD_BLE_GAP_SCAN_STOP,                        /**< Stop Scanning. */
  SD_BLE_GAP_CONNECT,                          /**< Connect. */
  SD_BLE_GAP_CONNECT_CANCEL,                   /**< Cancel ongoing connection procedure. */
  SD_BLE_GAP_RSSI_GET,                         /**< Get the last RSSI sample. */
};

/**@brief GAP Event IDs.
 * IDs that uniquely identify an event coming from the stack to the application.
 */
enum BLE_GAP_EVTS
{
  BLE_GAP_EVT_CONNECTED  = BLE_GAP_EVT_BASE,    /**< Connection established. @ref ble_gap_evt_connected_t */
  BLE_GAP_EVT_DISCONNECTED,                     /**< Disconnected from peer. @ref ble_gap_evt_disconnected_t */
  BLE_GAP_EVT_CONN_PARAM_UPDATE,                /**< Connection Parameters updated. ble_gap_evt_conn_param_update_t */
  BLE_GAP_EVT_SEC_PARAMS_REQUEST,               /**< Request to provide security parameters. @ref ble_gap_evt_sec_params_request_t */
  BLE_GAP_EVT_SEC_INFO_REQUEST,                 /**< Request to provide security information. @ref ble_gap_evt_sec_info_request_t */
  BLE_GAP_EVT_PASSKEY_DISPLAY,                  /**< Request to display a passkey to the user. @ref ble_gap_evt_passkey_display_t */
  BLE_GAP_EVT_AUTH_KEY_REQUEST,                 /**< Request to provide an authentication key. @ref ble_gap_evt_auth_key_request_t */
  BLE_GAP_EVT_AUTH_STATUS,                      /**< Authentication procedure completed with status. @ref ble_gap_evt_auth_status_t */
  BLE_GAP_EVT_CONN_SEC_UPDATE,                  /**< Connection security updated. @ref ble_gap_evt_conn_sec_update_t */
  BLE_GAP_EVT_TIMEOUT,                          /**< Timeout expired. @ref ble_gap_evt_timeout_t */
  BLE_GAP_EVT_RSSI_CHANGED,                     /**< RSSI report. @ref ble_gap_evt_rssi_changed_t */
  BLE_GAP_EVT_ADV_REPORT,                       /**< Advertising report. @ref ble_gap_evt_adv_report_t */
  BLE_GAP_EVT_SEC_REQUEST,                      /**< Security Request. @ref ble_gap_evt_sec_request_t */
  BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST,        /**< Connection Parameter Update Request. @ref ble_gap_evt_conn_param_update_request_t */
  BLE_GAP_EVT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_evt_scan_req_report_t */
};

/**@brief GAP Option IDs.
 * IDs that uniquely identify a GAP option.
 */
enum BLE_GAP_OPTS
{
  BLE_GAP_OPT_CH_MAP  = BLE_GAP_OPT_BASE,       /**< Channel Map. @ref ble_gap_opt_ch_map_t  */
  BLE_GAP_OPT_LOCAL_CONN_LATENCY,               /**< Local connection latency. @ref ble_gap_opt_local_conn_latency_t */
  BLE_GAP_OPT_PASSKEY,                          /**< Set passkey. @ref ble_gap_opt_passkey_t */
  BLE_GAP_OPT_PRIVACY,                          /**< Custom privacy. @ref ble_gap_opt_privacy_t */
  BLE_GAP_OPT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_opt_scan_req_report_t */
  BLE_GAP_OPT_COMPAT_MODE                       /**< Compatibility mode. @ref ble_gap_opt_compat_mode_t */
};
/** @} */

/**@addtogroup BLE_GAP_DEFINES Defines
 * @{ */

/**@defgroup BLE_ERRORS_GAP SVC return values specific to GAP
 * @{ */
#define BLE_ERROR_GAP_UUID_LIST_MISMATCH            (NRF_GAP_ERR_BASE + 0x000)  /**< UUID list does not contain an integral number of UUIDs. */
#define BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST   (NRF_GAP_ERR_BASE + 0x001)  /**< Use of Whitelist not permitted with discoverable advertising. */
#define BLE_ERROR_GAP_INVALID_BLE_ADDR              (NRF_GAP_ERR_BASE + 0x002)  /**< The upper two bits of the address do not correspond to the specified address type. */
#define BLE_ERROR_GAP_WHITELIST_IN_USE              (NRF_GAP_ERR_BASE + 0x003)  /**< Attempt to overwrite the whitelist while already in use by another operation. */ 
/**@} */


/**@defgroup BLE_GAP_ROLES GAP Roles
 * @note Not explicitly used in peripheral API, but will be relevant for central API.
 * @{ */
#define BLE_GAP_ROLE_INVALID     0x0            /**< Invalid Role. */
#define BLE_GAP_ROLE_PERIPH      0x1            /**< Peripheral Role. */
#define BLE_GAP_ROLE_CENTRAL     0x2            /**< Central Role. */
/**@} */


/**@defgroup BLE_GAP_TIMEOUT_SOURCES GAP Timeout sources
 * @{ */
#define BLE_GAP_TIMEOUT_SRC_ADVERTISING                0x00 /**< Advertising timeout. */
#define BLE_GAP_TIMEOUT_SRC_SECURITY_REQUEST           0x01 /**< Security request timeout. */
#define BLE_GAP_TIMEOUT_SRC_SCAN                       0x02 /**< Scanning timeout. */
#define BLE_GAP_TIMEOUT_SRC_CONN                       0x03 /**< Connection timeout. */
/**@} */


/**@defgroup BLE_GAP_ADDR_TYPES GAP Address types
 * @{ */
#define BLE_GAP_ADDR_TYPE_PUBLIC                        0x00 /**< Public address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_STATIC                 0x01 /**< Random Static address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE     0x02 /**< Private Resolvable address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE 0x03 /**< Private Non-Resolvable address. */
/**@} */

/**@defgroup BLE_GAP_ADDR_CYCLE_MODES GAP Address cycle modes
 * @{ */
#define BLE_GAP_ADDR_CYCLE_MODE_NONE      0x00 /**< Set addresses directly, no automatic address cycling. */
#define BLE_GAP_ADDR_CYCLE_MODE_AUTO      0x01 /**< Automatically generate and update private addresses. */
/** @} */

/**@brief The default interval in seconds at which a private address is refreshed when address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO.  */
#define BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S (60 * 15)

/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            6


/**@defgroup BLE_GAP_AD_TYPE_DEFINITIONS GAP Advertising and Scan Response Data format
 * @note Found at https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm
 * @{ */
#define BLE_GAP_AD_TYPE_FLAGS                               0x01 /**< Flags for discoverability. */
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02 /**< Partial list of 16 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03 /**< Complete list of 16 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE   0x04 /**< Partial list of 32 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE         0x05 /**< Complete list of 32 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06 /**< Partial list of 128 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE        0x07 /**< Complete list of 128 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME                    0x08 /**< Short local device name. */
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME                 0x09 /**< Complete local device name. */
#define BLE_GAP_AD_TYPE_TX_POWER_LEVEL                      0x0A /**< Transmit power level. */
#define BLE_GAP_AD_TYPE_CLASS_OF_DEVICE                     0x0D /**< Class of device. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C               0x0E /**< Simple Pairing Hash C. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R         0x0F /**< Simple Pairing Randomizer R. */
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE           0x10 /**< Security Manager TK Value. */
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS          0x11 /**< Security Manager Out Of Band Flags. */
#define BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     0x12 /**< Slave Connection Interval Range. */
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       0x14 /**< List of 16-bit Service Solicitation UUIDs. */
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      0x15 /**< List of 128-bit Service Solicitation UUIDs. */
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 /**< Service Data - 16-bit UUID. */
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 /**< Public Target Address. */
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 /**< Random Target Address. */
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 /**< Appearance. */
#define BLE_GAP_AD_TYPE_ADVERTISING_INTERVAL                0x1A /**< Advertising Interval. */ 
#define BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS         0x1B /**< LE Bluetooth Device Address. */
#define BLE_GAP_AD_TYPE_LE_ROLE                             0x1C /**< LE Role. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C256            0x1D /**< Simple Pairing Hash C-256. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256      0x1E /**< Simple Pairing Randomizer R-256. */
#define BLE_GAP_AD_TYPE_SERVICE_DATA_32BIT_UUID             0x20 /**< Service Data - 32-bit UUID. */
#define BLE_GAP_AD_TYPE_SERVICE_DATA_128BIT_UUID            0x21 /**< Service Data - 128-bit UUID. */
#define BLE_GAP_AD_TYPE_3D_INFORMATION_DATA                 0x3D /**< 3D Information Data. */
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF /**< Manufacturer Specific Data. */
/**@} */


/**@defgroup BLE_GAP_ADV_FLAGS GAP Advertisement Flags
 * @{ */
#define BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE         (0x01)   /**< LE Limited Discoverable Mode. */
#define BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE         (0x02)   /**< LE General Discoverable Mode. */
#define BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED         (0x04)   /**< BR/EDR not supported. */
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_CONTROLLER         (0x08)   /**< Simultaneous LE and BR/EDR, Controller. */
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_HOST               (0x10)   /**< Simultaneous LE and BR/EDR, Host. */
#define BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE Limited Discoverable Mode, BR/EDR not supported. */
#define BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE General Discoverable Mode, BR/EDR not supported. */
/**@} */


/**@defgroup BLE_GAP_ADV_INTERVALS GAP Advertising interval max and min
 * @{ */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */
 /**@}  */


/**@defgroup BLE_GAP_SCAN_INTERVALS GAP Scan interval max and min
 * @{ */
#define BLE_GAP_SCAN_INTERVAL_MIN       0x0004 /**< Minimum Scan interval in 625 us units, i.e. 2.5 ms. */
#define BLE_GAP_SCAN_INTERVAL_MAX       0x4000 /**< Maximum Scan interval in 625 us units, i.e. 10.24 s. */
 /** @}  */


/**@defgroup BLE_GAP_SCAN_WINDOW GAP Scan window max and min
 * @{ */
#define BLE_GAP_SCAN_WINDOW_MIN         0x0004 /**< Minimum Scan window in 625 us units, i.e. 2.5 ms. */
#define BLE_GAP_SCAN_WINDOW_MAX         0x4000 /**< Maximum Scan window in 625 us units, i.e. 10.24 s. */
 /** @}  */


/**@defgroup BLE_GAP_SCAN_TIMEOUT GAP Scan timeout max and min
 * @{ */
#define BLE_GAP_SCAN_TIMEOUT_MIN        0x0001 /**< Minimum Scan timeout in seconds. */
#define BLE_GAP_SCAN_TIMEOUT_MAX        0xFFFF /**< Maximum Scan timeout in seconds. */
 /** @}  */


/**@brief Maximum size of advertising data in octets. */
#define  BLE_GAP_ADV_MAX_SIZE           31


/**@defgroup BLE_GAP_ADV_TYPES GAP Advertising types
 * @{ */
#define BLE_GAP_ADV_TYPE_ADV_IND          0x00   /**< Connectable undirected. */
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND   0x01   /**< Connectable directed. */
#define BLE_GAP_ADV_TYPE_ADV_SCAN_IND     0x02   /**< Scannable undirected. */
#define BLE_GAP_ADV_TYPE_ADV_NONCONN_IND  0x03   /**< Non connectable undirected. */
/**@} */


/**@defgroup BLE_GAP_ADV_FILTER_POLICIES GAP Advertising filter policies
 * @{ */
#define BLE_GAP_ADV_FP_ANY                0x00   /**< Allow scan requests and connect requests from any device. */
#define BLE_GAP_ADV_FP_FILTER_SCANREQ     0x01   /**< Filter scan requests with whitelist. */
#define BLE_GAP_ADV_FP_FILTER_CONNREQ     0x02   /**< Filter connect requests with whitelist. */
#define BLE_GAP_ADV_FP_FILTER_BOTH        0x03   /**< Filter both scan and connect requests with whitelist. */
/**@} */


/**@defgroup BLE_GAP_ADV_TIMEOUT_VALUES GAP Advertising timeout values
 * @{ */
#define BLE_GAP_ADV_TIMEOUT_LIMITED_MAX      180 /**< Maximum advertising time in limited discoverable mode (TGAP(lim_adv_timeout) = 180s). */
#define BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED  0 /**< Unlimited advertising in general discoverable mode. */
/**@} */


/**@defgroup BLE_GAP_DISC_MODES GAP Discovery modes
 * @{ */
#define BLE_GAP_DISC_MODE_NOT_DISCOVERABLE  0x00   /**< Not discoverable discovery Mode. */
#define BLE_GAP_DISC_MODE_LIMITED           0x01   /**< Limited Discovery Mode. */
#define BLE_GAP_DISC_MODE_GENERAL           0x02   /**< General Discovery Mode. */
/**@} */

/**@defgroup BLE_GAP_IO_CAPS GAP IO Capabilities
 * @{ */
#define BLE_GAP_IO_CAPS_DISPLAY_ONLY      0x00   /**< Display Only. */
#define BLE_GAP_IO_CAPS_DISPLAY_YESNO     0x01   /**< Display and Yes/No entry. */
#define BLE_GAP_IO_CAPS_KEYBOARD_ONLY     0x02   /**< Keyboard Only. */
#define BLE_GAP_IO_CAPS_NONE              0x03   /**< No I/O capabilities. */
#define BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY  0x04   /**< Keyboard and Display. */
/**@} */


/**@defgroup BLE_GAP_AUTH_KEY_TYPES GAP Authentication Key Types
 * @{ */
#define BLE_GAP_AUTH_KEY_TYPE_NONE        0x00   /**< No key (may be used to reject). */
#define BLE_GAP_AUTH_KEY_TYPE_PASSKEY     0x01   /**< 6-digit Passkey. */
#define BLE_GAP_AUTH_KEY_TYPE_OOB         0x02   /**< Out Of Band data. */
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS GAP Security status
 * @{ */
#define BLE_GAP_SEC_STATUS_SUCCESS                0x00  /**< Procedure completed with success. */
#define BLE_GAP_SEC_STATUS_TIMEOUT                0x01  /**< Procedure timed out. */
#define BLE_GAP_SEC_STATUS_PDU_INVALID            0x02  /**< Invalid PDU received. */
#define BLE_GAP_SEC_STATUS_RFU_RANGE1_BEGIN       0x03  /**< Reserved for Future Use range #1 begin. */
#define BLE_GAP_SEC_STATUS_RFU_RANGE1_END         0x80  /**< Reserved for Future Use range #1 end. */
#define BLE_GAP_SEC_STATUS_PASSKEY_ENTRY_FAILED   0x81  /**< Passkey entry failed (user cancelled or other). */
#define BLE_GAP_SEC_STATUS_OOB_NOT_AVAILABLE      0x82  /**< Out of Band Key not available. */
#define BLE_GAP_SEC_STATUS_AUTH_REQ               0x83  /**< Authentication requirements not met. */
#define BLE_GAP_SEC_STATUS_CONFIRM_VALUE          0x84  /**< Confirm value failed. */
#define BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP       0x85  /**< Pairing not supported.  */
#define BLE_GAP_SEC_STATUS_ENC_KEY_SIZE           0x86  /**< Encryption key size. */
#define BLE_GAP_SEC_STATUS_SMP_CMD_UNSUPPORTED    0x87  /**< Unsupported SMP command. */
#define BLE_GAP_SEC_STATUS_UNSPECIFIED            0x88  /**< Unspecified reason. */
#define BLE_GAP_SEC_STATUS_REPEATED_ATTEMPTS      0x89  /**< Too little time elapsed since last attempt. */
#define BLE_GAP_SEC_STATUS_INVALID_PARAMS         0x8A  /**< Invalid parameters. */
#define BLE_GAP_SEC_STATUS_RFU_RANGE2_BEGIN       0x8B  /**< Reserved for Future Use range #2 begin. */
#define BLE_GAP_SEC_STATUS_RFU_RANGE2_END         0xFF  /**< Reserved for Future Use range #2 end. */
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS_SOURCES GAP Security status sources
 * @{ */
#define BLE_GAP_SEC_STATUS_SOURCE_LOCAL           0x00  /**< Local failure. */
#define BLE_GAP_SEC_STATUS_SOURCE_REMOTE          0x01  /**< Remote failure. */
/**@} */

/**@defgroup BLE_GAP_CP_LIMITS GAP Connection Parameters Limits
 * @{ */
#define BLE_GAP_CP_MIN_CONN_INTVL_NONE           0xFFFF  /**< No new minimum connction interval specified in connect parameters. */
#define BLE_GAP_CP_MIN_CONN_INTVL_MIN            0x0006  /**< Lowest mimimum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */
#define BLE_GAP_CP_MIN_CONN_INTVL_MAX            0x0C80  /**< Highest minimum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */
#define BLE_GAP_CP_MAX_CONN_INTVL_NONE           0xFFFF  /**< No new maximum connction interval specified in connect parameters. */
#define BLE_GAP_CP_MAX_CONN_INTVL_MIN            0x0006  /**< Lowest maximum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */
#define BLE_GAP_CP_MAX_CONN_INTVL_MAX            0x0C80  /**< Highest maximum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */
#define BLE_GAP_CP_SLAVE_LATENCY_MAX             0x01F3  /**< Highest slave latency permitted, in connection events. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_NONE         0xFFFF  /**< No new supervision timeout specified in connect parameters. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN          0x000A  /**< Lowest supervision timeout permitted, in units of 10 ms, i.e. 100 ms. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX          0x0C80  /**< Highest supervision timeout permitted, in units of 10 ms, i.e. 32 s. */
/**@} */


/**@brief GAP device name maximum length. */
#define BLE_GAP_DEVNAME_MAX_LEN           31

/**@brief Disable RSSI events for connections */
#define BLE_GAP_RSSI_THRESHOLD_INVALID    0xFF

/**@defgroup BLE_GAP_CONN_SEC_MODE_SET_MACROS GAP attribute security requirement setters
 *
 * See @ref ble_gap_conn_sec_mode_t.
 * @{ */
/**@brief Set sec_mode pointed to by ptr to have no access rights.*/
#define BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(ptr)         do {(ptr)->sm = 0; (ptr)->lv = 0;} while(0)
/**@brief Set sec_mode pointed to by ptr to require no protection, open link.*/
#define BLE_GAP_CONN_SEC_MODE_SET_OPEN(ptr)              do {(ptr)->sm = 1; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption, but no MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(ptr)       do {(ptr)->sm = 1; (ptr)->lv = 2;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption and MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(ptr)     do {(ptr)->sm = 1; (ptr)->lv = 3;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption, no MITM protection needed.*/
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(ptr)    do {(ptr)->sm = 2; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption with MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(ptr)  do {(ptr)->sm = 2; (ptr)->lv = 2;} while(0)
/**@} */


/**@brief GAP Security Random Number Length. */
#define BLE_GAP_SEC_RAND_LEN 8

/**@brief GAP Security Key Length. */
#define BLE_GAP_SEC_KEY_LEN 16

/**@brief GAP Passkey Length. */
#define BLE_GAP_PASSKEY_LEN 6

/**@brief Maximum amount of addresses in a whitelist. */
#define BLE_GAP_WHITELIST_ADDR_MAX_COUNT (8)

/**@brief Maximum amount of IRKs in a whitelist.
 * @note  The number of IRKs is limited to 8, even if the hardware supports more.
 */
#define BLE_GAP_WHITELIST_IRK_MAX_COUNT (8)

/**@defgroup GAP_SEC_MODES GAP Security Modes
 * @{ */
#define BLE_GAP_SEC_MODE 0x00 /**< No key (may be used to reject). */
/**@} */

/**@} */

/**@addtogroup BLE_GAP_STRUCTURES Structures
 * @{ */

/**@brief Bluetooth Low Energy address. */
typedef struct
{
  uint8_t addr_type;                    /**< See @ref BLE_GAP_ADDR_TYPES. */
  uint8_t addr[BLE_GAP_ADDR_LEN];       /**< 48-bit address, LSB format. */
} ble_gap_addr_t;


/**@brief GAP connection parameters.
 *
 * @note  When ble_conn_params_t is received in an event, both min_conn_interval and
 *        max_conn_interval will be equal to the connection interval set by the central.
 *
 * @note If both conn_sup_timeout and max_conn_interval are specified, then the following constraint applies:
 *       conn_sup_timeout * 4 > (1 + slave_latency) * max_conn_interval
 *       that corresponds to the following Bluetooth Spec requirement:
 *       The Supervision_Timeout in milliseconds shall be larger than
 *       (1 + Conn_Latency) * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds.
 */
typedef struct
{
  uint16_t min_conn_interval;         /**< Minimum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t max_conn_interval;         /**< Maximum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t slave_latency;             /**< Slave Latency in number of connection events, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units, see @ref BLE_GAP_CP_LIMITS.*/
} ble_gap_conn_params_t;


/**@brief GAP connection security modes.
 *
 * Security Mode 0 Level 0: No access permissions at all (this level is not defined by the Bluetooth Core specification).\n
 * Security Mode 1 Level 1: No security is needed (aka open link).\n
 * Security Mode 1 Level 2: Encrypted link required, MITM protection not necessary.\n
 * Security Mode 1 Level 3: MITM protected encrypted link required.\n
 * Security Mode 2 Level 1: Signing or encryption required, MITM protection not necessary.\n
 * Security Mode 2 Level 2: MITM protected signing required, unless link is MITM protected encrypted.\n
 */
typedef struct
{
  uint8_t sm : 4;                     /**< Security Mode (1 or 2), 0 for no permissions at all. */
  uint8_t lv : 4;                     /**< Level (1, 2 or 3), 0 for no permissions at all. */

} ble_gap_conn_sec_mode_t;


/**@brief GAP connection security status.*/
typedef struct
{
  ble_gap_conn_sec_mode_t sec_mode;           /**< Currently active security mode for this connection.*/
  uint8_t                 encr_key_size;      /**< Length of currently active encryption key, 7 to 16 octets (only applicable for bonding procedures). */
} ble_gap_conn_sec_t;


/**@brief Identity Resolving Key. */
typedef struct
{
  uint8_t irk[BLE_GAP_SEC_KEY_LEN];   /**< Array containing IRK. */
} ble_gap_irk_t;


/**@brief Whitelist structure. */
typedef struct
{
  ble_gap_addr_t    **pp_addrs;        /**< Pointer to an array of device address pointers, pointing to addresses to be used in whitelist. NULL if none are given. */
  uint8_t             addr_count;      /**< Count of device addresses in array, up to @ref BLE_GAP_WHITELIST_ADDR_MAX_COUNT. */
  ble_gap_irk_t     **pp_irks;         /**< Pointer to an array of Identity Resolving Key (IRK) pointers, each pointing to an IRK in the whitelist. NULL if none are given. */
  uint8_t             irk_count;       /**< Count of IRKs in array, up to @ref BLE_GAP_WHITELIST_IRK_MAX_COUNT. */
} ble_gap_whitelist_t;

/**@brief Channel mask for RF channels used in advertising and scanning. */ 
typedef struct
{
  uint8_t ch_37_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 37 */
  uint8_t ch_38_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 38 */
  uint8_t ch_39_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 39 */
} ble_gap_adv_ch_mask_t;

/**@brief GAP advertising parameters.*/
typedef struct
{
  uint8_t               type;                 /**< See @ref BLE_GAP_ADV_TYPES. */
  ble_gap_addr_t       *p_peer_addr;          /**< For @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND mode only, known peer address. */
  uint8_t               fp;                   /**< Filter Policy, see @ref BLE_GAP_ADV_FILTER_POLICIES. */
  ble_gap_whitelist_t  *p_whitelist;          /**< Pointer to whitelist, NULL if no whitelist or the current active whitelist is to be used. */
  uint16_t              interval;             /**< Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s), see @ref BLE_GAP_ADV_INTERVALS.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for high duty cycle directed advertising.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, set @ref BLE_GAP_ADV_INTERVAL_MIN <= interval <= @ref BLE_GAP_ADV_INTERVAL_MAX for low duty cycle advertising.*/
  uint16_t              timeout;              /**< Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. See also @ref BLE_GAP_ADV_TIMEOUT_VALUES. If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for High duty cycle directed advertising. */
  ble_gap_adv_ch_mask_t channel_mask;         /**< Advertising channel mask. @see ble_gap_channel_mask_t for documentation. */
} ble_gap_adv_params_t;


/**@brief GAP scanning parameters. */
typedef struct
{
  uint8_t                 active    : 1;        /**< If 1, perform active scanning (scan requests). */
  uint8_t                 selective : 1;        /**< If 1, ignore unknown devices (non whitelisted). */
  ble_gap_whitelist_t *   p_whitelist;          /**< Pointer to whitelist, NULL if no whitelist or the current active whitelist is to be used. */
  uint16_t                interval;             /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
  uint16_t                window;               /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
  uint16_t                timeout;              /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */
} ble_gap_scan_params_t;


/** @brief Keys that can be exchanged during a bonding procedure. */
typedef struct
{
  uint8_t enc     : 1;                        /**< Long Term Key and Master Identification. */
  uint8_t id      : 1;                        /**< Identity Resolving Key and Identity Address Information. */
  uint8_t sign    : 1;                        /**< Connection Signature Resolving Key. */
} ble_gap_sec_kdist_t;


/**@brief GAP security parameters. */
typedef struct
{
  uint8_t               bond    : 1;               /**< Perform bonding. */
  uint8_t               mitm    : 1;               /**< Man In The Middle protection required. */
  uint8_t               io_caps : 3;               /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */
  uint8_t               oob     : 1;               /**< Out Of Band data available. */
  uint8_t               min_key_size;              /**< Minimum encryption key size in octets between 7 and 16. If 0 then not applicable in this instance. */
  uint8_t               max_key_size;              /**< Maximum encryption key size in octets between min_key_size and 16. */
  ble_gap_sec_kdist_t   kdist_periph;              /**< Key distribution bitmap: keys that the peripheral device will distribute. */
  ble_gap_sec_kdist_t   kdist_central;             /**< Key distribution bitmap: keys that the central device will distribute. */
} ble_gap_sec_params_t;


/**@brief GAP Encryption Information. */
typedef struct
{
  uint8_t   ltk[BLE_GAP_SEC_KEY_LEN];   /**< Long Term Key. */
  uint8_t   auth : 1;                   /**< Authenticated Key. */
  uint8_t   ltk_len : 7;                /**< LTK length in octets. */
} ble_gap_enc_info_t;


/**@brief GAP Master Identification. */
typedef struct
{
  uint16_t  ediv;                       /**< Encrypted Diversifier. */
  uint8_t   rand[BLE_GAP_SEC_RAND_LEN]; /**< Random Number. */
} ble_gap_master_id_t;


/**@brief GAP Signing Information. */
typedef struct
{
  uint8_t   csrk[BLE_GAP_SEC_KEY_LEN];        /**< Connection Signature Resolving Key. */
} ble_gap_sign_info_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONNECTED. */
typedef struct
{
  ble_gap_addr_t        peer_addr;              /**< Bluetooth address of the peer device. */
  ble_gap_addr_t        own_addr;               /**< Bluetooth address of the local device used during connection setup. */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
  uint8_t               role;                   /**< BLE role for this connection, see @ref BLE_GAP_ROLES */
#endif
  uint8_t               irk_match :1;           /**< If 1, peer device's address resolved using an IRK. */
  uint8_t               irk_match_idx  :7;      /**< Index in IRK list where the address was matched. */
  ble_gap_conn_params_t conn_params;            /**< GAP Connection Parameters. */
} ble_gap_evt_connected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_DISCONNECTED. */
typedef struct
{
  uint8_t reason;                               /**< HCI error code, see @ref BLE_HCI_STATUS_CODES. */
} ble_gap_evt_disconnected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE. */
typedef struct
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */
} ble_gap_evt_conn_param_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST. */
typedef struct
{
  ble_gap_sec_params_t peer_params;             /**< Initiator Security Parameters. */
} ble_gap_evt_sec_params_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_INFO_REQUEST. */
typedef struct
{
  ble_gap_addr_t      peer_addr;                     /**< Bluetooth address of the peer device. */
  ble_gap_master_id_t master_id;                     /**< Master Identification for LTK lookup. */
  uint8_t             enc_info  : 1;                 /**< If 1, Encryption Information required. */
  uint8_t             id_info   : 1;                 /**< If 1, Identity Information required. */
  uint8_t             sign_info : 1;                 /**< If 1, Signing Information required. */
} ble_gap_evt_sec_info_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_PASSKEY_DISPLAY. */
typedef struct
{
  uint8_t passkey[BLE_GAP_PASSKEY_LEN];         /**< 6-digit passkey in ASCII ('0'-'9' digits only). */
} ble_gap_evt_passkey_display_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_KEY_REQUEST. */
typedef struct
{
  uint8_t key_type;                             /**< See @ref BLE_GAP_AUTH_KEY_TYPES. */
} ble_gap_evt_auth_key_request_t;


/**@brief Security levels supported.
 * @note See Bluetooth Specification Version 4.1 Volume 3, Part C, Chapter 10.
*/
typedef struct
{
  uint8_t lv1 : 1;                              /**< If 1: Level 1 is supported. */
  uint8_t lv2 : 1;                              /**< If 1: Level 2 is supported. */
  uint8_t lv3 : 1;                              /**< If 1: Level 3 is supported. */
} ble_gap_sec_levels_t;


/**@brief Encryption Key. */
typedef struct
{
  ble_gap_enc_info_t    enc_info;             /**< Encryption Information. */
  ble_gap_master_id_t   master_id;            /**< Master Identification. */
} ble_gap_enc_key_t;


/**@brief Identity Key. */
typedef struct
{
  ble_gap_irk_t         id_info;              /**< Identity Information. */
  ble_gap_addr_t        id_addr_info;         /**< Identity Address Information. */
} ble_gap_id_key_t;


/**@brief Security Keys. */
typedef struct
{
  ble_gap_enc_key_t     *p_enc_key;           /**< Encryption Key, or NULL. */
  ble_gap_id_key_t      *p_id_key;            /**< Identity Key, or NULL. */
  ble_gap_sign_info_t   *p_sign_key;          /**< Signing Key, or NULL. */
} ble_gap_sec_keys_t;


/**@brief Security key set (both Peripheral and Central keys).
 *         Note that when distributing Bluetooth addresses pertaining to the local device those
 *         will have to be filled in by the user. */
typedef struct
{
  ble_gap_sec_keys_t keys_periph;     /**< Keys distributed by the device in the Peripheral role. */
  ble_gap_sec_keys_t keys_central;    /**< Keys distributed by the device in the Central role. */
} ble_gap_sec_keyset_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_STATUS. */
typedef struct
{
  uint8_t               auth_status;            /**< Authentication status, see @ref BLE_GAP_SEC_STATUS. */
  uint8_t               error_src : 2;          /**< On error, source that caused the failure, see @ref BLE_GAP_SEC_STATUS_SOURCES. */
  uint8_t               bonded : 1;             /**< Procedure resulted in a bond. */
  ble_gap_sec_levels_t  sm1_levels;             /**< Levels supported in Security Mode 1. */
  ble_gap_sec_levels_t  sm2_levels;             /**< Levels supported in Security Mode 2. */
  ble_gap_sec_kdist_t   kdist_periph;           /**< Bitmap stating which keys were exchanged (distributed) by the peripheral. */
  ble_gap_sec_kdist_t   kdist_central;          /**< Bitmap stating which keys were exchanged (distributed) by the central. */
} ble_gap_evt_auth_status_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_SEC_UPDATE. */
typedef struct
{
  ble_gap_conn_sec_t conn_sec;                  /**< Connection security level. */
} ble_gap_evt_conn_sec_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_TIMEOUT. */
typedef struct
{
  uint8_t src;                                  /**< Source of timeout event, see @ref BLE_GAP_TIMEOUT_SOURCES. */
} ble_gap_evt_timeout_t;


/**@brief Event structure for @ref BLE_GAP_EVT_RSSI_CHANGED. */
typedef struct
{
  int8_t  rssi;                               /**< Received Signal Strength Indication in dBm. */
} ble_gap_evt_rssi_changed_t;


/**@brief Event structure for @ref BLE_GAP_EVT_ADV_REPORT. */
typedef struct
{
  ble_gap_addr_t peer_addr;                     /**< Bluetooth address of the peer device. */
  int8_t         rssi;                          /**< Received Signal Strength Indication in dBm. */
  uint8_t        scan_rsp : 1;                  /**< If 1, the report corresponds to a scan response and the type field may be ignored. */
  uint8_t        type     : 2;                  /**< See @ref BLE_GAP_ADV_TYPES. Only valid if the scan_rsp field is 0. */
  uint8_t        dlen     : 5;                  /**< Advertising or scan response data length. */
  uint8_t        data[BLE_GAP_ADV_MAX_SIZE];    /**< Advertising or scan response data. */
} ble_gap_evt_adv_report_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_REQUEST. */
typedef struct
{
  uint8_t    bond    : 1;                       /**< Perform bonding. */
  uint8_t    mitm    : 1;                       /**< Man In The Middle protection required. */
} ble_gap_evt_sec_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST. */
typedef struct
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */
} ble_gap_evt_conn_param_update_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SCAN_REQ_REPORT. */
typedef struct
{
  int8_t                  rssi;              /**< Received Signal Strength Indication in dBm. */
  ble_gap_addr_t          peer_addr;         /**< Bluetooth address of the peer device. */
} ble_gap_evt_scan_req_report_t;



/**@brief GAP event structure. */
typedef struct
{
  uint16_t conn_handle;                                     /**< Connection Handle on which event occured. */
  union                                                     /**< union alternative identified by evt_id in enclosing struct. */
  {
    ble_gap_evt_connected_t                   connected;                    /**< Connected Event Parameters. */
    ble_gap_evt_disconnected_t                disconnected;                 /**< Disconnected Event Parameters. */
    ble_gap_evt_conn_param_update_t           conn_param_update;            /**< Connection Parameter Update Parameters. */
    ble_gap_evt_sec_params_request_t          sec_params_request;           /**< Security Parameters Request Event Parameters. */
    ble_gap_evt_sec_info_request_t            sec_info_request;             /**< Security Information Request Event Parameters. */
    ble_gap_evt_passkey_display_t             passkey_display;              /**< Passkey Display Event Parameters. */
    ble_gap_evt_auth_key_request_t            auth_key_request;             /**< Authentication Key Request Event Parameters. */
    ble_gap_evt_auth_status_t                 auth_status;                  /**< Authentication Status Event Parameters. */
    ble_gap_evt_conn_sec_update_t             conn_sec_update;              /**< Connection Security Update Event Parameters. */
    ble_gap_evt_timeout_t                     timeout;                      /**< Timeout Event Parameters. */
    ble_gap_evt_rssi_changed_t                rssi_changed;                 /**< RSSI Event parameters. */
    ble_gap_evt_adv_report_t                  adv_report;                   /**< Advertising Report Event Parameters. */
    ble_gap_evt_sec_request_t                 sec_request;                  /**< Security Request Event Parameters. */
    ble_gap_evt_conn_param_update_request_t   conn_param_update_request;    /**< Connection Parameter Update Parameters. */
    ble_gap_evt_scan_req_report_t             scan_req_report;              /**< Scan Request Report parameters. */
  } params;                                                                 /**< Event Parameters. */

} ble_gap_evt_t;


/**@brief Channel Map option.
 *        Used with @ref sd_ble_opt_get to get the current channel map
 *        or @ref sd_ble_opt_set to set a new channel map. When setting the
 *        channel map, it applies to all current and future connections. When getting the
 *        current channel map, it applies to a single connection and the connection handle
 *        must be supplied.
 *
 * @note  Setting the channel map may take some time, depending on connection parameters.
 *        The time taken may be different for each connection and the get operation will
 *        return the previous channel map until the new one has taken effect.
 *
 * @note  After setting the channel map, by spec it can not be set again until at least 1 s has passed.
 *        See Bluetooth Specification Version 4.1 Volume 2, Part E, Section 7.3.46.
 *
 * @retval ::NRF_SUCCESS Get or set successful.
 * @retval ::NRF_ERROR_BUSY Channel map was set again before enough time had passed.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied for get.
 * @retval ::NRF_ERROR_NOT_SUPPORTED Returned by sd_ble_opt_set in peripheral-only SoftDevices.
 *
 */
typedef struct
{
  uint16_t conn_handle;                   /**< Connection Handle (only applicable for get) */
  uint8_t ch_map[5];                      /**< Channel Map (37-bit). */
} ble_gap_opt_ch_map_t;


/**@brief Local connection latency option.
 *
 *        Local connection latency is a feature which enables the slave to improve
 *        current consumption by ignoring the slave latency set by the peer. The
 *        local connection latency can only be set to a multiple of the slave latency,
 *        and cannot be longer than half of the supervision timeout.
 *
 *        Used with @ref sd_ble_opt_set to set the local connection latency. The
 *        @ref sd_ble_opt_get is not supported for this option, but the actual
 *        local connection latency (unless set to NULL) is set as a return parameter
 *        when setting the option.
 *
 * @note  The latency set will be truncated down to the closest slave latency event
 *        multiple, or the nearest multiple before half of the supervision timeout.
 *
 * @note  The local connection latency is disabled by default, and needs to be enabled for new
 *        connections and whenever the connection is updated.
 *
 * @retval ::NRF_SUCCESS Set successfully.
 * @retval ::NRF_ERROR_NOT_SUPPORTED Get is not supported.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle parameter.
 */
typedef struct
{
  uint16_t   conn_handle;                       /**< Connection Handle */
  uint16_t   requested_latency;                 /**< Requested local connection latency. */
  uint16_t * p_actual_latency;                  /**< Pointer to storage for the actual local connection latency (can be set to NULL to skip return value). */
} ble_gap_opt_local_conn_latency_t;


/**@brief Passkey Option.
 *
 *        Structure containing the passkey to be used during pairing. This can be used with @ref
 *        sd_ble_opt_set to make the SoftDevice use a pre-programmed passkey for authentication
 *        instead of generating a random one.
 *
 * @note  @ref sd_ble_opt_get is not supported for this option.
 *
 */
typedef struct
{
  uint8_t * p_passkey;                          /**< Pointer to 6-digit ASCII string (digit 0..9 only, no NULL termination) passkey to be used during pairing. If this is NULL, the SoftDevice will generate a random passkey if required.*/
} ble_gap_opt_passkey_t;


/**@brief Custom Privacy Option.
 *
 *        This structure is used with both @ref sd_ble_opt_set (as input) and with
 *        @ref sd_ble_opt_get (as output).
 *
 *        Structure containing:
 *        - A pointer to an IRK to set (if input), or a place to store a read IRK (if output).
 *        - A private address refresh cycle.
 *
 * @note  The specified address cycle interval is used when the address cycle mode is
 *        @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO. If 0 is given, the address will not be automatically 
 *        refreshed at all. The default interval is @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S.
 *
 * @note  If the current address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address will immediately be
 *        refreshed when a custom privacy option is set. A new address can be generated manually by calling
 *        @ref sd_ble_gap_address_set with the same type again. 
 *
 * @note  If the IRK is updated, the new IRK becomes the one to be distributed in all
 *        bonding procedures performed after @ref sd_ble_opt_set returns.
 *
 * @retval ::NRF_SUCCESS Set or read successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR The pointer to IRK storage is invalid.
 */
typedef struct
{
  ble_gap_irk_t * p_irk;        /**< When input: Pointer to custom IRK, or NULL to use/reset to the device's default IRK. When output: Pointer to where the current IRK is to be stored, or NULL to not read out the IRK. */
  uint16_t        interval_s;   /**< When input: Custom private address cycle interval in seconds. When output: The current private address cycle interval. */
} ble_gap_opt_privacy_t;


/**@brief Scan request report option.
 *
 *        This can be used with @ref sd_ble_opt_set to make the SoftDevice send
 *        @ref BLE_GAP_EVT_SCAN_REQ_REPORT events.
 *
 *  @note   Due to the limited space reserved for scan request report events,
 *          not all received scan requests will be reported.
 *
 *  @note   If whitelisting is used, only whitelisted requests are reported.
 *
 *  @retval ::NRF_SUCCESS Set successfully.
 *  @retval ::NRF_ERROR_INVALID_STATE When advertising is ongoing while the option is set.
 */
typedef struct
{
   uint8_t enable : 1;                           /**< Enable scan request reports. */
} ble_gap_opt_scan_req_report_t;

/**@brief Compatibility mode option.
 *
 *        This can be used with @ref sd_ble_opt_set to enable and disable
 *        compatibility modes. Compatibility modes are disabled by default.
 *
 *  @note  Compatibility mode 1 enables interoperability with devices that do not support 
 *         a value of 0 for the WinOffset parameter in the Link Layer CONNECT_REQ packet.
 *
 *  @retval ::NRF_SUCCESS Set successfully.
 *  @retval ::NRF_ERROR_INVALID_STATE When connection creation is ongoing while mode 1 is set.
 */
typedef struct
{
   uint8_t mode_1_enable : 1;                           /**< Enable compatibility mode 1.*/
} ble_gap_opt_compat_mode_t;


/**@brief Option structure for GAP options. */
typedef union
{
  ble_gap_opt_ch_map_t                  ch_map;                    /**< Parameters for the Channel Map option. */
  ble_gap_opt_local_conn_latency_t      local_conn_latency;        /**< Parameters for the Local connection latency option */
  ble_gap_opt_passkey_t                 passkey;                   /**< Parameters for the Passkey option.*/
  ble_gap_opt_privacy_t                 privacy;                   /**< Parameters for the Custom privacy option. */
  ble_gap_opt_scan_req_report_t         scan_req_report;           /**< Parameters for the scan request report option.*/
  ble_gap_opt_compat_mode_t             compat_mode;               /**< Parameters for the compatibility mode option.*/
} ble_gap_opt_t;
/**@} */


/**@addtogroup BLE_GAP_FUNCTIONS Functions
 * @{ */

/**@brief Set local Bluetooth address.
 *
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address type is required to
 * be @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE. The given address is ignored and the
 * SoftDevice will generate a new private address automatically every 
 * @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S seconds. If this API
 * call is used again with the same parameters, the SoftDevice will immediately
 * generate a new private address to replace the current address.
 *
 * @note If the application wishes to use a @ref BLE_GAP_ADDR_TYPE_PUBLIC or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_STATIC address, the cycle mode must be
 * @ref BLE_GAP_ADDR_CYCLE_MODE_NONE.
 *
 * @note If this API function is called while advertising or scanning, the softdevice will immediately update the
 * advertising or scanning address without the need to stop the procedure in the following cases:
 *   - If the previously set address is of type @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address
 *   is also of type @ref BLE_GAP_ADDR_TYPE_PUBLIC
 *   - If the previously set address is not @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address is
 *   also not @ref BLE_GAP_ADDR_TYPE_PUBLIC.
 * If the address is changed from a @ref BLE_GAP_ADDR_TYPE_PUBLIC address to another type or from
 * another type to a @ref BLE_GAP_ADDR_TYPE_PUBLIC address, the change will take effect the next
 * time an advertising or scanning procedure is started.
 *
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_NONE and the application is
 *       using privacy, the application must take care to generate and set new private addresses
 *       periodically to comply with the Privacy specification in Bluetooth Core Spec.
 *
 * @param[in] addr_cycle_mode Address cycle mode, see @ref BLE_GAP_ADDR_CYCLE_MODES.
 * @param[in] p_addr          Pointer to address structure.
 *
 * @retval ::NRF_SUCCESS Address successfully set.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameters.
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid address.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 */
SVCALL(SD_BLE_GAP_ADDRESS_SET, uint32_t, sd_ble_gap_address_set(uint8_t addr_cycle_mode, const ble_gap_addr_t *p_addr));


/**@brief Get local Bluetooth address.
 *
 * @param[out] p_addr Pointer to address structure to be filled in.
 *
 * @retval ::NRF_SUCCESS Address successfully retrieved.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_ADDRESS_GET, uint32_t, sd_ble_gap_address_get(ble_gap_addr_t *p_addr));


/**@brief Set, clear or update advertising and scan response data.
 *
 * @note The format of the advertising data will be checked by this call to ensure interoperability.
 *       Limitations imposed by this API call to the data provided include having a flags data type in the scan response data and
 *       duplicating the local name in the advertising data and scan response data. 
 *
 * @note To clear the advertising data and set it to a 0-length packet, simply provide a valid pointer (p_data/p_sr_data) with its corresponding 
 *        length (dlen/srdlen) set to 0.
 *
 * @note The call will fail if p_data and p_sr_data are both NULL since this would have no effect.
 *
 * @param[in] p_data    Raw data to be placed in advertising packet. If NULL, no changes are made to the current advertising packet data.
 * @param[in] dlen      Data length for p_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_data is NULL, can be 0 if p_data is not NULL.
 * @param[in] p_sr_data Raw data to be placed in scan response packet. If NULL, no changes are made to the current scan response packet data.
 * @param[in] srdlen    Data length for p_sr_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_sr_data is NULL, can be 0 if p_data is not NULL.
 *
 * @retval ::NRF_SUCCESS Advertising data successfully updated or cleared.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_FLAGS Invalid combination of advertising flags supplied.
 * @retval ::NRF_ERROR_INVALID_DATA Invalid data type(s) supplied, check the advertising data format specification.
 * @retval ::NRF_ERROR_INVALID_LENGTH Invalid data length(s) supplied.
 * @retval ::NRF_ERROR_NOT_SUPPORTED Unsupported data type.
 * @retval ::BLE_ERROR_GAP_UUID_LIST_MISMATCH Invalid UUID list supplied.
 */
SVCALL(SD_BLE_GAP_ADV_DATA_SET, uint32_t, sd_ble_gap_adv_data_set(uint8_t const *p_data, uint8_t dlen, uint8_t const *p_sr_data, uint8_t srdlen));


/**@brief Start advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
 *
 * @note An application can start an advertising procedure for broadcasting purposes while a connection
 *       is active. After a @ref BLE_GAP_EVT_CONNECTED event is received, this function may therefore
 *       be called to start a broadcast advertising procedure. The advertising procedure
 *       cannot however be connectable (it must be of type @ref BLE_GAP_ADV_TYPE_ADV_SCAN_IND or
 *       @ref BLE_GAP_ADV_TYPE_ADV_NONCONN_IND). @note Only one advertiser may be active at any time.
 *
 * @note To use the currently active whitelist set p_adv_params->p_whitelist to NULL.
 *
 * @param[in] p_adv_params Pointer to advertising parameters structure.
 *
 * @retval ::NRF_SUCCESS The BLE stack has started advertising.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check the accepted ranges and limits.
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Bluetooth address supplied.
 * @retval ::BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST Discoverable mode and whitelist incompatible.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::BLE_ERROR_GAP_WHITELIST_IN_USE Unable to replace the whitelist while another operation is using it.
 */
SVCALL(SD_BLE_GAP_ADV_START, uint32_t, sd_ble_gap_adv_start(ble_gap_adv_params_t const *p_adv_params));


/**@brief Stop advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
 *
 * @retval ::NRF_SUCCESS The BLE stack has stopped advertising.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in advertising state).
 */
SVCALL(SD_BLE_GAP_ADV_STOP, uint32_t, sd_ble_gap_adv_stop(void));


/**@brief Update connection parameters.
 *
 * @details In the central role this will initiate a Link Layer connection parameter update procedure,
 *          otherwise in the peripheral role, this will send the corresponding L2CAP request and wait for
 *          the central to perform the procedure. In both cases, and regardless of success or failure, the application
 *          will be informed of the result with a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE event.
 *
 * @details This function can be used as a central both to reply to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST or to start the procedure unrequested.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_conn_params  Pointer to desired connection parameters. If NULL is provided on a peripheral role,
 *                           the parameters in the PPCP characteristic of the GAP service will be used instead.
 *                           If NULL is provided on a central role and in response to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST, the peripheral request will be rejected
 *
 * @retval ::NRF_SUCCESS The Connection Update procedure has been started successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, process pending events and wait for pending procedures to complete and retry.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_NO_MEM Not enough memory to complete operation.
 */
SVCALL(SD_BLE_GAP_CONN_PARAM_UPDATE, uint32_t, sd_ble_gap_conn_param_update(uint16_t conn_handle, ble_gap_conn_params_t const *p_conn_params));


/**@brief Disconnect (GAP Link Termination).
 *
 * @details This call initiates the disconnection procedure, and its completion will be communicated to the application
 *          with a @ref BLE_GAP_EVT_DISCONNECTED event.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] hci_status_code HCI status code, see @ref BLE_HCI_STATUS_CODES (accepted values are @ref BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION and @ref BLE_HCI_CONN_INTERVAL_UNACCEPTABLE).
 *
 * @retval ::NRF_SUCCESS The disconnection procedure has been started successfully.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (disconnection is already in progress).
 */
SVCALL(SD_BLE_GAP_DISCONNECT, uint32_t, sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code));


/**@brief Set the radio's transmit power.
 *
 * @param[in] tx_power Radio transmit power in dBm (accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm).
 *
 * @note -40 dBm will not actually give -40 dBm, but will instead be remapped to -30 dBm.
 *
 * @retval ::NRF_SUCCESS Successfully changed the transmit power.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 */
SVCALL(SD_BLE_GAP_TX_POWER_SET, uint32_t, sd_ble_gap_tx_power_set(int8_t tx_power));


/**@brief Set GAP Appearance value.
 *
 * @param[in] appearance Appearance (16-bit), see @ref BLE_APPEARANCES.
 *
 * @retval ::NRF_SUCCESS  Appearance value set successfully.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 */
SVCALL(SD_BLE_GAP_APPEARANCE_SET, uint32_t, sd_ble_gap_appearance_set(uint16_t appearance));


/**@brief Get GAP Appearance value.
 *
 * @param[out] p_appearance Pointer to appearance (16-bit) to be filled in, see @ref BLE_APPEARANCES.
 *
 * @retval ::NRF_SUCCESS Appearance value retrieved successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_APPEARANCE_GET, uint32_t, sd_ble_gap_appearance_get(uint16_t *p_appearance));


/**@brief Set GAP Peripheral Preferred Connection Parameters.
 *
 * @param[in] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure with the desired parameters.
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters set successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 */
SVCALL(SD_BLE_GAP_PPCP_SET, uint32_t, sd_ble_gap_ppcp_set(ble_gap_conn_params_t const *p_conn_params));


/**@brief Get GAP Peripheral Preferred Connection Parameters.
 *
 * @param[out] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure where the parameters will be stored.
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters retrieved successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_PPCP_GET, uint32_t, sd_ble_gap_ppcp_get(ble_gap_conn_params_t *p_conn_params));


/**@brief Set GAP device name.
 *
 * @param[in] p_write_perm Write permissions for the Device Name characteristic, see @ref ble_gap_conn_sec_mode_t.
 * @param[in] p_dev_name Pointer to a UTF-8 encoded, <b>non NULL-terminated</b> string.
 * @param[in] len Length of the UTF-8, <b>non NULL-terminated</b> string pointed to by p_dev_name in octets (must be smaller or equal than @ref BLE_GAP_DEVNAME_MAX_LEN).
 *
 * @retval ::NRF_SUCCESS GAP device name and permissions set successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_SET, uint32_t, sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const *p_write_perm, uint8_t const *p_dev_name, uint16_t len));


/**@brief Get GAP device name.
 *
 * @param[out]    p_dev_name Pointer to an empty buffer where the UTF-8 <b>non NULL-terminated</b> string will be placed. Set to NULL to obtain the complete device name length.
 * @param[in,out] p_len      Length of the buffer pointed by p_dev_name, complete device name length on output.
 *
 * @note          If the device name is longer than the size of the supplied buffer,
 *                p_len will return the complete device name length,
 *                and not the number of bytes actually returned in p_dev_name.
 *                The application may use this information to allocate a suitable buffer size.
 *
 * @retval ::NRF_SUCCESS GAP device name retrieved successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_GET, uint32_t, sd_ble_gap_device_name_get(uint8_t *p_dev_name, uint16_t *p_len));


/**@brief Initiate the GAP Authentication procedure.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_sec_params Pointer to the @ref ble_gap_sec_params_t structure with the security parameters to be used during the pairing or bonding procedure.
 *                         In the peripheral role, only the timeout, bond and mitm fields of this structure are used.
 *                         In the central role, this pointer may be NULL to reject a Security Request.
 *
 * @details In the central role, this function will send an SMP Pairing Request (or an SMP Pairing Failed if rejected), 
 *          otherwise in the peripheral role, an SMP Security Request will be sent.
 *
 * @note    Calling this function may result in the following events depending on the outcome and parameters: @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST,
 *          @ref BLE_GAP_EVT_SEC_INFO_REQUEST, @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, @ref BLE_GAP_EVT_CONN_SEC_UPDATE, @ref BLE_GAP_EVT_AUTH_STATUS.
 *
 *
 * @retval ::NRF_SUCCESS Successfully initiated authentication procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_TIMEOUT A SMP timout has occured, and further SMP operations on this link is prohibited.
 */
SVCALL(SD_BLE_GAP_AUTHENTICATE, uint32_t, sd_ble_gap_authenticate(uint16_t conn_handle, ble_gap_sec_params_t const *p_sec_params));


/**@brief Reply with GAP security parameters.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] sec_status Security status, see @ref BLE_GAP_SEC_STATUS.
 * @param[in] p_sec_params Pointer to a @ref ble_gap_sec_params_t security parameters structure. In the central role this must be set to NULL, as the parameters have
 *                         already been provided during a previous call to @ref sd_ble_gap_authenticate.
 * @param[in,out] p_sec_keyset Pointer to a @ref ble_gap_sec_keyset_t security keyset structure. Any keys distributed as a result of the ongoing security procedure 
 *                         will be stored into the memory referenced by the pointers inside this structure. The keys will be stored and available to the application 
 *                         upon reception of a @ref BLE_GAP_EVT_AUTH_STATUS event.
 *                         The pointer to the ID key data distributed by the <b>local device</b> constitutes however an exception. It can either point to ID key data 
 *                         filled in by the user before calling this function, in which case a user-supplied Bluetooth address and IRK will be distributed, 
 *                         or the pointer to the ID key data structure can be NULL, in which case the device's configured (or default, if none is configured) 
 *                         Bluetooth address and IRK will be distributed. 
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST, calling it at other times will result in an @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 *
 * @retval ::NRF_SUCCESS Successfully accepted security parameter from the application.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_SEC_PARAMS_REPLY, uint32_t, sd_ble_gap_sec_params_reply(uint16_t conn_handle, uint8_t sec_status, ble_gap_sec_params_t const *p_sec_params, ble_gap_sec_keyset_t const *p_sec_keyset));


/**@brief Reply with an authentication key.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] key_type See @ref BLE_GAP_AUTH_KEY_TYPES.
 * @param[in] p_key If key type is @ref BLE_GAP_AUTH_KEY_TYPE_NONE, then NULL.
 *                If key type is @ref BLE_GAP_AUTH_KEY_TYPE_PASSKEY, then a 6-byte ASCII string (digit 0..9 only, no NULL termination).
 *                If key type is @ref BLE_GAP_AUTH_KEY_TYPE_OOB, then a 16-byte OOB key value in Little Endian format.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, calling it at other times will result in an @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 *
 * @retval ::NRF_SUCCESS Authentication key successfully set.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_AUTH_KEY_REPLY, uint32_t, sd_ble_gap_auth_key_reply(uint16_t conn_handle, uint8_t key_type, uint8_t const *p_key));


/**@brief Initiate GAP Encryption procedure.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_master_id Pointer to a @ref ble_gap_master_id_t master identification structure.
 * @param[in] p_enc_info  Pointer to a @ref ble_gap_enc_info_t encryption information structure.
 *
 * @details In the central role, this function will initiate the encryption procedure using the encryption information provided.
 *
 * @note    Calling this function may result in the following event depending on the outcome and parameters: @ref BLE_GAP_EVT_CONN_SEC_UPDATE.
 *
 * @retval ::NRF_SUCCESS Successfully initiated authentication procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::BLE_ERROR_INVALID_ROLE Operation is not supported in the Peripheral role.
 * @retval ::NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, wait for pending procedures to complete and retry.
 */
SVCALL(SD_BLE_GAP_ENCRYPT, uint32_t, sd_ble_gap_encrypt(uint16_t conn_handle, ble_gap_master_id_t const *p_master_id, ble_gap_enc_info_t const *p_enc_info));


/**@brief Reply with GAP security information.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_enc_info Pointer to a @ref ble_gap_enc_info_t encryption information structure. May be NULL to signal none is available.
 * @param[in] p_id_info Pointer to a @ref ble_gap_irk_t identity information structure. May be NULL to signal none is available.
 * @param[in] p_sign_info Pointer to a @ref ble_gap_sign_info_t signing information structure. May be NULL to signal none is available.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_INFO_REQUEST, calling it at other times will result in @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 * @note    Data signing is not yet supported, and p_sign_info must therefore be NULL.
 *
 * @retval ::NRF_SUCCESS Successfully accepted security information.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_SEC_INFO_REPLY, uint32_t, sd_ble_gap_sec_info_reply(uint16_t conn_handle, ble_gap_enc_info_t const *p_enc_info, ble_gap_irk_t const *p_id_info, ble_gap_sign_info_t const *p_sign_info));


/**@brief Get the current connection security.
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_conn_sec  Pointer to a @ref ble_gap_conn_sec_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS Current connection security successfully retrieved.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_CONN_SEC_GET, uint32_t, sd_ble_gap_conn_sec_get(uint16_t conn_handle, ble_gap_conn_sec_t *p_conn_sec));


/**@brief Start reporting the received signal strength to the application. 
 *
 * A new event is reported whenever the RSSI value changes, until @ref sd_ble_gap_rssi_stop is called.
 *
 * @param[in] conn_handle        Connection handle.
 * @param[in] threshold_dbm      Minimum change in dBm before triggering the @ref BLE_GAP_EVT_RSSI_CHANGED event. Events are disabled if threshold_dbm equals @ref BLE_GAP_RSSI_THRESHOLD_INVALID.
 * @param[in] skip_count         Number of RSSI samples with a change of threshold_dbm or more before sending a new @ref BLE_GAP_EVT_RSSI_CHANGED event.
 *
 * @retval ::NRF_SUCCESS                   Successfully activated RSSI reporting.
 * @retval ::NRF_ERROR_INVALID_STATE       Disconnection in progress. Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_START, uint32_t, sd_ble_gap_rssi_start(uint16_t conn_handle, uint8_t threshold_dbm, uint8_t skip_count));


/**@brief Stop reporting the received signal strength.
 *
 * @note An RSSI change detected before the call but not yet received by the application 
 * may be reported after @ref sd_ble_gap_rssi_stop has been called.
 *
 * @param[in] conn_handle Connection handle.
 *
 * @retval ::NRF_SUCCESS                   Successfully deactivated RSSI reporting.
 * @retval ::NRF_ERROR_INVALID_STATE       Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_STOP, uint32_t, sd_ble_gap_rssi_stop(uint16_t conn_handle));


/**@brief Get the received signal strength for the last connection event.
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_rssi      Pointer to the location where the RSSI measurement shall be stored.
 *
 * @ref sd_ble_gap_rssi_start must be called to start reporting RSSI before using this function. @ref NRF_ERROR_NOT_FOUND
 * will be returned until RSSI was sampled for the first time after calling @ref sd_ble_gap_rssi_start.
 *
 * @retval ::NRF_SUCCESS                   Successfully read the RSSI.
 * @retval ::NRF_ERROR_NOT_FOUND           No sample is available.
 * @retval ::NRF_ERROR_INVALID_ADDR        Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_INVALID_STATE       RSSI reporting is not ongoing, or disconnection in progress.
 */
SVCALL(SD_BLE_GAP_RSSI_GET, uint32_t, sd_ble_gap_rssi_get(uint16_t conn_handle, int8_t *p_rssi));


/**@brief Start scanning (GAP Discovery procedure, Observer Procedure).
 *
 * @note To use the currently active whitelist set p_scan_params->p_whitelist to NULL.
 *
 * @param[in] p_scan_params Pointer to scan parameters structure.
 *
 * @retval ::NRF_SUCCESS Successfully initiated scanning procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::BLE_ERROR_GAP_WHITELIST_IN_USE Unable to replace the whitelist while another operation is using it.
 */
SVCALL(SD_BLE_GAP_SCAN_START, uint32_t, sd_ble_gap_scan_start(ble_gap_scan_params_t const *p_scan_params));


/**@brief Stop scanning (GAP Discovery procedure, Observer Procedure).
 *
 * @retval ::NRF_SUCCESS Successfully stopped scanning procedure.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in scanning state).
 */
SVCALL(SD_BLE_GAP_SCAN_STOP, uint32_t, sd_ble_gap_scan_stop(void));


/**@brief Create a connection (GAP Link Establishment).
 *
 * @note To use the currently active whitelist set p_scan_params->p_whitelist to NULL.    
 *
 * @param[in] p_peer_addr   Pointer to peer address. If the selective bit is set in @ref ble_gap_scan_params_t, then this must be NULL.
 * @param[in] p_scan_params Pointer to scan parameters structure.
 * @param[in] p_conn_params Pointer to desired connection parameters.
 *
 * @retval ::NRF_SUCCESS Successfully initiated connection procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid parameter(s) pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Peer address.
 * @retval ::NRF_ERROR_NO_MEM limit of available connections reached.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry. If another connection is being established wait for the corresponding
 *                          @ref BLE_GAP_EVT_CONNECTED event before calling again.
 * @retval ::BLE_ERROR_GAP_WHITELIST_IN_USE Unable to replace the whitelist while another operation is using it.
 */
SVCALL(SD_BLE_GAP_CONNECT, uint32_t, sd_ble_gap_connect(ble_gap_addr_t const *p_peer_addr, ble_gap_scan_params_t const *p_scan_params, ble_gap_conn_params_t const *p_conn_params));


/**@brief Cancel a connection establishment.
 *
 * @retval ::NRF_SUCCESS Successfully cancelled an ongoing connection procedure.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 */
SVCALL(SD_BLE_GAP_CONNECT_CANCEL, uint32_t, sd_ble_gap_connect_cancel(void));

/** @} */

#endif // BLE_GAP_H__

/**
  @}
*/
