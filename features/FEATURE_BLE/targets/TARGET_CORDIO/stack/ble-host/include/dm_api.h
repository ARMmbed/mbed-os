/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Device Manager subsystem API.
 */
/*************************************************************************************************/
#ifndef DM_API_H
#define DM_API_H

#include "hci_api.h"
#include "cfg_stack.h"
#include "smp_defs.h"
#include "sec_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_DM_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name GAP Device Role
 * Connectable GAP Roles.
 */
/**@{*/
#define DM_ROLE_MASTER              HCI_ROLE_MASTER  /*!< \brief Role is master */
#define DM_ROLE_SLAVE               HCI_ROLE_SLAVE   /*!< \brief Role is slave */
/**@}*/

/** \name GAP Discovery Mode
 * When setup as a discoverable device, these are the possible modes of discovery.
 */
/**@{*/
#define DM_DISC_MODE_NONE           0     /*!< \brief GAP non-discoverable */
#define DM_DISC_MODE_LIMITED        1     /*!< \brief GAP limited discoverable mode */
#define DM_DISC_MODE_GENERAL        2     /*!< \brief GAP general discoverable mode */
/**@}*/

/** \name GAP Advertising Type
 * Type of connectable or disconverable advertising to perform.
 */
/**@{*/
#define DM_ADV_CONN_UNDIRECT        0     /*!< \brief Connectable and scannable undirected advertising */
#define DM_ADV_CONN_DIRECT          1     /*!< \brief Connectable directed advertising */
#define DM_ADV_SCAN_UNDIRECT        2     /*!< \brief Scannable undirected advertising */
#define DM_ADV_NONCONN_UNDIRECT     3     /*!< \brief Non-connectable and non-scannable undirected advertising */
#define DM_ADV_CONN_DIRECT_LO_DUTY  4     /*!< \brief Connectable directed low duty cycle advertising */
/**@}*/

/** \name GAP AE Advertising Types
 * Advertising extension types - AE only.
 */
/**@{*/
#define DM_EXT_ADV_CONN_UNDIRECT    5     /*!< \brief Connectable undirected advertising */
#define DM_EXT_ADV_NONCONN_DIRECT   6     /*!< \brief Non-connectable and non-scannable directed advertising */
#define DM_EXT_ADV_SCAN_DIRECT      7     /*!< \brief Scannable directed advertising */
#define DM_ADV_NONE                 255   /*!< \brief For internal use only */
/**@}*/

/** \name GAP Advertising Report Type
 * Type of an advertising report observed while scanning.
 */
/**@{*/
#define DM_RPT_CONN_UNDIRECT        0    /*!< \brief Connectable and scannable undirected advertising */
#define DM_RPT_CONN_DIRECT          1    /*!< \brief Connectable directed advertising */
#define DM_RPT_SCAN_UNDIRECT        2    /*!< \brief Scannable undirected advertising */
#define DM_RPT_NONCONN_UNDIRECT     3    /*!< \brief Non-connectable undirected advertising */
#define DM_RPT_SCAN_RESPONSE        4    /*!< \brief Scan response */
/**@}*/

/** \name GAP Advertising Data Location
 * Whether data is located in the advertising data or in the scan response data
 */
/**@{*/
#define DM_DATA_LOC_ADV             0     /*!< \brief Locate data in the advertising data */
#define DM_DATA_LOC_SCAN            1     /*!< \brief Locate data in the scan response data */
/**@}*/

/** \name GAP Scan Type
 * When setup as a connectable or observer device, this is the type of scanning to perform.
 */
/**@{*/
#define DM_SCAN_TYPE_PASSIVE        0     /*!< \brief Passive scan */
#define DM_SCAN_TYPE_ACTIVE         1     /*!< \brief Active scan */
/**@}*/

/** \name GAP Advertising Channel Map
 * Advertising channel map codes
 */
/**@{*/
#define DM_ADV_CHAN_37              HCI_ADV_CHAN_37  /*!< \brief Advertising channel 37 */
#define DM_ADV_CHAN_38              HCI_ADV_CHAN_38  /*!< \brief Advertising channel 38 */
#define DM_ADV_CHAN_39              HCI_ADV_CHAN_39  /*!< \brief Advertising channel 39 */
/*! \brief All advertising channels */
#define DM_ADV_CHAN_ALL             (HCI_ADV_CHAN_37 | HCI_ADV_CHAN_38 | HCI_ADV_CHAN_39)
/**@}*/

/** \name DM Client IDs
 * The client ID parameter to function DmConnRegister()
 */
/**@{*/
#define DM_CLIENT_ID_ATT            0     /*!< \brief Identifier for attribute protocol, for internal use only */
#define DM_CLIENT_ID_SMP            1     /*!< \brief Identifier for security manager protocol, for internal use only */
#define DM_CLIENT_ID_DM             2     /*!< \brief Identifier for device manager, for internal use only */
#define DM_CLIENT_ID_APP            3     /*!< \brief Identifier for the application */
#define DM_CLIENT_ID_L2C            4     /*!< \brief Identifier for L2CAP */
#define DM_CLIENT_ID_MAX            5     /*!< \brief For internal use only */
/**@}*/

/** \name DM Unknown IDs
 * Values for unknown or unspecificed device identifiers.
 */
/**@{*/
/*! \brief Unknown connection ID or other error */
#define DM_CONN_ID_NONE             0

/*! \brief Unknown sync ID or other error */
#define DM_SYNC_ID_NONE             0
/**@}*/

/** \name GAP Address Type
 * The address type to use over the air or that is associated with a received address.
 */
/**@{*/
#define DM_ADDR_PUBLIC              0x00  /*!< \brief Public device address */
#define DM_ADDR_RANDOM              0x01  /*!< \brief Random device address */
#define DM_ADDR_PUBLIC_IDENTITY     0x02  /*!< \brief Public identity address (corresponds to resolved private address) */
#define DM_ADDR_RANDOM_IDENTITY     0x03  /*!< \brief Random (static) identity address (corresponds to resolved private address) */
#define DM_ADDR_RANDOM_UNRESOLVED   0xFE  /*!< \brief Random device address (Controller unable to resolve) */
#define DM_ADDR_NONE                0xFF  /*!< \brief No address provided (anonymous) */
/**@}*/

/** \name GAP Advertising Data Types
 * Advertising data types flags.
 */
/**@{*/
#define DM_ADV_TYPE_FLAGS           0x01  /*!< \brief Flag bits */
#define DM_ADV_TYPE_16_UUID_PART    0x02  /*!< \brief Partial list of 16 bit UUIDs */
#define DM_ADV_TYPE_16_UUID         0x03  /*!< \brief Complete list of 16 bit UUIDs */
#define DM_ADV_TYPE_32_UUID_PART    0x04  /*!< \brief Partial list of 32 bit UUIDs */
#define DM_ADV_TYPE_32_UUID         0x05  /*!< \brief Complete list of 32 bit UUIDs */
#define DM_ADV_TYPE_128_UUID_PART   0x06  /*!< \brief Partial list of 128 bit UUIDs */
#define DM_ADV_TYPE_128_UUID        0x07  /*!< \brief Complete list of 128 bit UUIDs */
#define DM_ADV_TYPE_SHORT_NAME      0x08  /*!< \brief Shortened local name */
#define DM_ADV_TYPE_LOCAL_NAME      0x09  /*!< \brief Complete local name */
#define DM_ADV_TYPE_TX_POWER        0x0A  /*!< \brief TX power level */
#define DM_ADV_TYPE_SM_TK_VALUE     0x10  /*!< \brief Security manager TK value */
#define DM_ADV_TYPE_SM_OOB_FLAGS    0x11  /*!< \brief Security manager OOB flags */
#define DM_ADV_TYPE_CONN_INTERVAL   0x12  /*!< \brief Slave preferred connection interval */
#define DM_ADV_TYPE_SIGNED_DATA     0x13  /*!< \brief Signed data */
#define DM_ADV_TYPE_16_SOLICIT      0x14  /*!< \brief Service soliticiation list of 16 bit UUIDs */
#define DM_ADV_TYPE_128_SOLICIT     0x15  /*!< \brief Service soliticiation list of 128 bit UUIDs */
#define DM_ADV_TYPE_SERVICE_DATA    0x16  /*!< \brief Service data - 16-bit UUID */
#define DM_ADV_TYPE_PUBLIC_TARGET   0x17  /*!< \brief Public target address */
#define DM_ADV_TYPE_RANDOM_TARGET   0x18  /*!< \brief Random target address */
#define DM_ADV_TYPE_APPEARANCE      0x19  /*!< \brief Device appearance */
#define DM_ADV_TYPE_ADV_INTERVAL    0x1A  /*!< \brief Advertising interval */
#define DM_ADV_TYPE_BD_ADDR         0x1B  /*!< \brief LE Bluetooth device address */
#define DM_ADV_TYPE_ROLE            0x1C  /*!< \brief LE role */
#define DM_ADV_TYPE_32_SOLICIT      0x1F  /*!< \brief Service soliticiation list of 32 bit UUIDs */
#define DM_ADV_TYPE_SVC_DATA_32     0x20  /*!< \brief Service data - 32-bit UUID */
#define DM_ADV_TYPE_SVC_DATA_128    0x21  /*!< \brief Service data - 128-bit UUID */
#define DM_ADV_TYPE_LESC_CONFIRM    0x22  /*!< \brief LE Secure Connections confirm value */
#define DM_ADV_TYPE_LESC_RANDOM     0x23  /*!< \brief LE Secure Connections random value */
#define DM_ADV_TYPE_URI             0x24  /*!< \brief URI */
#define DM_ADV_TYPE_MANUFACTURER    0xFF  /*!< \brief Manufacturer specific data */
/**@}*/

/** \name GAP Advertising Data Flag Advertising Type
 * Bit mask for Advertising Type flag in advertising data.
 */
/**@{*/
#define DM_FLAG_LE_LIMITED_DISC     0x01  /*!< \brief Limited discoverable flag */
#define DM_FLAG_LE_GENERAL_DISC     0x02  /*!< \brief General discoverable flag */
#define DM_FLAG_LE_BREDR_NOT_SUP    0x04  /*!< \brief BR/EDR not supported flag */
/**@}*/

/** \name GAP Advertising Data Element Indexes
 * Advertising data element indexes.
 */
/**@{*/
#define DM_AD_LEN_IDX               0     /*!< \brief Advertising data element len */
#define DM_AD_TYPE_IDX              1     /*!< \brief Advertising data element type */
#define DM_AD_DATA_IDX              2     /*!< \brief Advertising data element data */
/**@}*/

/** \name GAP Advertising URI
 * Advertising URI Scheme
 */
/**@{*/
#define DM_URI_SCHEME_HTTP          0x16  /*!< \brief URI HTTP Scheme */
#define DM_URI_SCHEME_HTTPS         0x17  /*!< \brief URI HTTPS Scheme */
/**@}*/

/** \name GAP Timeouts
 * Timeouts defined by the GAP specification; in units of milliseconds.
 */
/**@{*/
#define DM_GAP_LIM_ADV_TIMEOUT      180000  /*!< \brief Maximum advertising duration in limited discoverable mode */
#define DM_GAP_GEN_DISC_SCAN_MIN    10240   /*!< \brief Minimum scan duration for general discovery */
#define DM_GAP_LIM_DISC_SCAN_MIN    10240   /*!< \brief Minimum scan duration for limited discovery */
#define DM_GAP_CONN_PARAM_TIMEOUT   30000   /*!< \brief Connection parameter update timeout */
#define DM_GAP_SCAN_FAST_PERIOD     30720   /*!< \brief Minimum time to perform scanning when user initiated */
#define DM_GAP_ADV_FAST_PERIOD      30000   /*!< \brief Minimum time to perform advertising when user initiated */
/**@}*/

/** \name GAP 1M PHY Timing
 * Advertising, scanning, and connection parameters defined in the GAP specification for the LE 1M PHY.
 * In units of 625 microseconds.
 */
/**@{*/
#define DM_GAP_SCAN_FAST_INT_MIN          48      /*!< \brief Minimum scan interval when user initiated */
#define DM_GAP_SCAN_FAST_INT_MAX          96      /*!< \brief Maximum scan interval when user initiated */
#define DM_GAP_SCAN_FAST_WINDOW           48      /*!< \brief Scan window when user initiated */
#define DM_GAP_SCAN_SLOW_INT_1            2048    /*!< \brief Scan interval 1 when background scannning */
#define DM_GAP_SCAN_SLOW_WINDOW_1         18      /*!< \brief Scan window 1 when background scanning */
#define DM_GAP_SCAN_SLOW_INT_2            4096    /*!< \brief Scan interval 2 when background scannning */
#define DM_GAP_SCAN_SLOW_WINDOW_2         36      /*!< \brief Scan window 2 when background scanning */
#define DM_GAP_ADV_FAST_INT_MIN_1         48      /*!< \brief Minimum advertising interval 1 when user initiated */
#define DM_GAP_ADV_FAST_INT_MAX_1         96      /*!< \brief Maximum advertising interval 1 when user initiated */
#define DM_GAP_ADV_FAST_INT_MIN_2         160     /*!< \brief Minimum advertising interval 2 when user initiated */
#define DM_GAP_ADV_FAST_INT_MAX_2         240     /*!< \brief Maximum advertising interval 2 when user initiated */
#define DM_GAP_ADV_SLOW_INT_MIN           1600    /*!< \brief Minimum advertising interval when background advertising */
#define DM_GAP_ADV_SLOW_INT_MAX           1920    /*!< \brief Maximum advertising interval when background advertising */
/**@}*/

/** \name GAP Coded PHY Timing
 * Advertising, scanning, and connection parameters defined in the GAP specification for the LE Coded PHY.
 * In units of 625 microseconds.
 */
/**@{*/
#define DM_GAP_SCAN_CODED_FAST_INT_MIN    144     /*!< \brief Minimum scan interval when user initiated on LE Coded PHY */
#define DM_GAP_SCAN_CODED_FAST_INT_MAX    288     /*!< \brief Maximum scan interval when user initiated on LE Coded PHY */
#define DM_GAP_SCAN_CODED_FAST_WINDOW     144     /*!< \brief Scan window when user initiated on LE Coded PHY */
#define DM_GAP_SCAN_CODED_SLOW_INT_1      6144    /*!< \brief Scan interval 1 when background scannning on LE Coded PHY  */
#define DM_GAP_SCAN_CODED_SLOW_WINDOW_1   54      /*!< \brief Scan window 1 when background scanning on LE Coded PHY */
#define DM_GAP_SCAN_CODED_SLOW_INT_2      12288   /*!< \brief Scan interval 2 when background scannning on LE Coded PHY */
#define DM_GAP_SCAN_CODED_SLOW_WINDOW_2   108     /*!< \brief Scan window 2 when background scanning on LE Coded PHY */
#define DM_GAP_ADV_CODED_FAST_INT_MIN_1   144     /*!< \brief Minimum advertising interval 1 when user initiated on LE Coded PHY */
#define DM_GAP_ADV_CODED_FAST_INT_MAX_1   288     /*!< \brief Maximum advertising interval 1 when user initiated on LE Coded PHY */
#define DM_GAP_ADV_CODED_FAST_INT_MIN_2   480     /*!< \brief Minimum advertising interval 2 when user initiated on LE Coded PHY */
#define DM_GAP_ADV_CODED_FAST_INT_MAX_2   720     /*!< \brief Maximum advertising interval 2 when user initiated on LE Coded PHY */
#define DM_GAP_ADV_CODED_SLOW_INT_MIN     4800    /*!< \brief Minimum advertising interval when background advertising on LE Coded PHY */
#define DM_GAP_ADV_CODED_SLOW_INT_MAX     5760    /*!< \brief Maximum advertising interval when background advertising on LE Coded PHY */
/**@}*/

/** \name GAP Connection Slave Latency
 *
 */
/**@{*/
/*! \brief GAP connection establishment slaves latency */
#define DM_GAP_CONN_EST_LATENCY     0
/**@}*/

/** \name GAP Connection Interval
 * GAP connection interval in 1.25ms units.
 */
/**@{*/
#define DM_GAP_INITIAL_CONN_INT_MIN 24      /*!< \brief Minimum initial connection interval */
#define DM_GAP_INITIAL_CONN_INT_MAX 40      /*!< \brief Maximum initial connection interval */
/**@}*/

/** \name GAP Connection Event Lengths
 * GAP connection establishment minimum and maximum connection event lengths.
 */
/**@{*/
#define DM_GAP_CONN_EST_MIN_CE_LEN  0  /*!< \brief Connection establishment minimum event length */
#define DM_GAP_CONN_EST_MAX_CE_LEN  0  /*!< \brief Connection establishment maximum event length */
/**@}*/

/** \name GAP Peripheral Privacy Characteristic Values
 *
 */
/**@{*/
#define DM_GAP_PRIV_DISABLED        0  /*!< \brief Privacy Disabled */
#define DM_GAP_PRIV_ENABLED         1  /*!< \brief Privacy Enabled */
/**@}*/

/** \name GAP Connection Supervision Timeout
 * Connection supervision timeout, in 10ms units
 */
/**@{*/
/*! \brief Connection establishment supervision timeout default, in 10ms units */
#define DM_DEFAULT_EST_SUP_TIMEOUT  2000
/**@}*/

/** \name GAP Security Pairing Authentication Requirements
 * Pairing authentication/security properties bit mask.
 */
/**@{*/
#define DM_AUTH_BOND_FLAG           SMP_AUTH_BOND_FLAG  /*!< \brief Bonding requested */
#define DM_AUTH_MITM_FLAG           SMP_AUTH_MITM_FLAG  /*!< \brief MITM (authenticated pairing) requested */
#define DM_AUTH_SC_FLAG             SMP_AUTH_SC_FLAG    /*!< \brief LE Secure Connections requested */
#define DM_AUTH_KP_FLAG             SMP_AUTH_KP_FLAG    /*!< \brief Keypress notifications requested */
/**@}*/

/** \name GAP Key Distribution Flags
 * Key distribution bit mask
 */
/**@{*/
#define DM_KEY_DIST_LTK             SMP_KEY_DIST_ENC   /*!< \brief Distribute LTK used for encryption */
#define DM_KEY_DIST_IRK             SMP_KEY_DIST_ID    /*!< \brief Distribute IRK used for privacy */
#define DM_KEY_DIST_CSRK            SMP_KEY_DIST_SIGN  /*!< \brief Distribute CSRK used for signed data */
/**@}*/

/** \name DM Security Key Indication Types
 * Type of key used in \ref DM_SEC_KEY_IND.
 */
/**@{*/
#define DM_KEY_LOCAL_LTK            0x01  /*!< \brief LTK generated locally for this device */
#define DM_KEY_PEER_LTK             0x02  /*!< \brief LTK received from peer device */
#define DM_KEY_IRK                  0x04  /*!< \brief IRK and identity info of peer device */
#define DM_KEY_CSRK                 0x08  /*!< \brief CSRK of peer device */
/**@}*/

/*! \brief Base value for HCI error status values for \ref DM_SEC_PAIR_CMPL_IND */
#define DM_SEC_HCI_ERR_BASE         0x20

/** \name GAP Security Level
 * GAP Mode 1 Security Levels
 */
/**@{*/
#define DM_SEC_LEVEL_NONE           0     /*!< \brief Connection has no security */
#define DM_SEC_LEVEL_ENC            1     /*!< \brief Connection is encrypted with unauthenticated key */
#define DM_SEC_LEVEL_ENC_AUTH       2     /*!< \brief Connection is encrypted with authenticated key */
#define DM_SEC_LEVEL_ENC_LESC       3     /*!< \brief Connection is encrypted with LE Secure Connections */
/**@}*/

/** \name GAP Random Address Types
 * Random address type masks.
 */
/**@{*/
#define DM_RAND_ADDR_STATIC         0xC0  /*!< \brief Static address */
#define DM_RAND_ADDR_RESOLV         0x40  /*!< \brief Resolvable private address */
#define DM_RAND_ADDR_NONRESOLV      0x00  /*!< \brief Non-resolvable private address */
/**@}*/

/** \name GAP Random Address Macros
 * Macros for identifying address type.
 */
/**@{*/
/*! \brief Get the type of random address */
#define DM_RAND_ADDR_GET(addr)          ((addr)[5] & 0xC0)

/*! \brief Set the type of random address */
#define DM_RAND_ADDR_SET(addr, type)    {(addr)[5] = ((addr)[5] & 0x3F) | (type);}

/*! \brief Check for Static Address */
#define DM_RAND_ADDR_SA(addr, type)     (((type) == DM_ADDR_RANDOM) && \
                                         (DM_RAND_ADDR_GET((addr)) == DM_RAND_ADDR_STATIC))

/*! \brief Check for Resolvable Private Address */
#define DM_RAND_ADDR_RPA(addr, type)    (((type) == DM_ADDR_RANDOM) && \
                                         (DM_RAND_ADDR_GET((addr)) == DM_RAND_ADDR_RESOLV))
/**@}*/

/** \name GAP Privacy Mode
 * Privacy Mode of this device in regards to a peer device.
 */
/**@{*/
#define DM_PRIV_MODE_NETWORK        0x00  /*!< \brief Network privacy mode (default). */
#define DM_PRIV_MODE_DEVICE         0x01  /*!< \brief Device privacy mode. */
/**@}*/

/** \name DM Internal State
 * Connection busy or idle state
 */
/**@{*/
#define DM_CONN_IDLE                0     /*!< \brief Connection is idle. */
#define DM_CONN_BUSY                1     /*!< \brief Connection is busy. */
/**@}*/

/** \name DM Internal State Flags
 * Connection busy/idle state bitmask.
 */
/**@{*/
#define DM_IDLE_SMP_PAIR            0x0001  /*!< \brief SMP pairing in progress */
#define DM_IDLE_DM_ENC              0x0002  /*!< \brief DM Encryption setup in progress */
#define DM_IDLE_ATTS_DISC           0x0004  /*!< \brief ATTS service discovery in progress */
#define DM_IDLE_APP_DISC            0x0008  /*!< \brief App framework service discovery in progress */
#define DM_IDLE_USER_1              0x0010  /*!< \brief For use by user application */
#define DM_IDLE_USER_2              0x0020  /*!< \brief For use by user application */
#define DM_IDLE_USER_3              0x0040  /*!< \brief For use by user application */
#define DM_IDLE_USER_4              0x0080  /*!< \brief For use by user application */
/**@}*/

/** \name GAP Filter Policy Modes
 * Filter policy modes.
 */
/**@{*/
#define DM_FILT_POLICY_MODE_ADV     0     /*!< \brief Advertising filter policy mode */
#define DM_FILT_POLICY_MODE_SCAN    1     /*!< \brief Scanning filter policy mode */
#define DM_FILT_POLICY_MODE_INIT    2     /*!< \brief Initiator filter policy mode */
#define DM_FILT_POLICY_MODE_SYNC    3     /*!< \brief Synchronization filter policy mode */
/**@}*/

/** \name DM Proprietary Error Codes
 * Internal error codes not sent in any PDU.
 */
/**@{*/
#define DM_ERR_SMP_RX_PDU_LEN_EXCEEDED    0x01  /*!< \brief LESC key length exceeded maximum RX PDU length */
#define DM_ERR_ATT_RX_PDU_LEN_EXCEEDED    0x02  /*!< \brief Configured ATT MTU exceeded maximum RX PDU length */
#define DM_ERR_L2C_RX_PDU_LEN_EXCEEDED    0x03  /*!< \brief Registered COC MPS exceeded maximum RX PDU length */
/**@}*/

/** \name DM Legacy Advertising Handle
 * Default handle for legacy advertising when using legacy HCI interface.  In this case only one advertising
 * set is allowed so all activity uses the same handle.
 */
/**@{*/
/*! \brief Default Advertising handle for legacy advertising */
#define DM_ADV_HANDLE_DEFAULT       0
/**@}*/

/** \name DM Callback Events
 * Events handled by the DM state machine.
 */
/**@{*/
#define DM_CBACK_START              0x20  /*!< \brief DM callback event starting value */

/*! \brief DM callback events */
enum
{
  DM_RESET_CMPL_IND = DM_CBACK_START,     /*!< \brief Reset complete */
  DM_ADV_START_IND,                       /*!< \brief Advertising started */
  DM_ADV_STOP_IND,                        /*!< \brief Advertising stopped */
  DM_ADV_NEW_ADDR_IND,                    /*!< \brief New resolvable address has been generated */
  DM_SCAN_START_IND,                      /*!< \brief Scanning started */
  DM_SCAN_STOP_IND,                       /*!< \brief Scanning stopped */
  DM_SCAN_REPORT_IND,                     /*!< \brief Scan data received from peer device */
  DM_CONN_OPEN_IND,                       /*!< \brief Connection opened */
  DM_CONN_CLOSE_IND,                      /*!< \brief Connection closed */
  DM_CONN_UPDATE_IND,                     /*!< \brief Connection update complete */
  DM_SEC_PAIR_CMPL_IND,                   /*!< \brief Pairing completed successfully */
  DM_SEC_PAIR_FAIL_IND,                   /*!< \brief Pairing failed or other security failure */
  DM_SEC_ENCRYPT_IND,                     /*!< \brief Connection encrypted */
  DM_SEC_ENCRYPT_FAIL_IND,                /*!< \brief Encryption failed */
  DM_SEC_AUTH_REQ_IND,                    /*!< \brief PIN or OOB data requested for pairing */
  DM_SEC_KEY_IND,                         /*!< \brief Security key indication */
  DM_SEC_LTK_REQ_IND,                     /*!< \brief LTK requested for encyption */
  DM_SEC_PAIR_IND,                        /*!< \brief Incoming pairing request from master */
  DM_SEC_SLAVE_REQ_IND,                   /*!< \brief Incoming security request from slave */
  DM_SEC_CALC_OOB_IND,                    /*!< \brief Result of OOB Confirm Calculation Generation */
  DM_SEC_ECC_KEY_IND,                     /*!< \brief Result of ECC Key Generation */
  DM_SEC_COMPARE_IND,                     /*!< \brief Result of Just Works/Numeric Comparison Compare Value Calculation */
  DM_SEC_KEYPRESS_IND,                    /*!< \brief Keypress indication from peer in passkey security */
  DM_PRIV_RESOLVED_ADDR_IND,              /*!< \brief Private address resolved */
  DM_PRIV_GENERATE_ADDR_IND,              /*!< \brief Private resolvable address generated */
  DM_CONN_READ_RSSI_IND,                  /*!< \brief Connection RSSI read */
  DM_PRIV_ADD_DEV_TO_RES_LIST_IND,        /*!< \brief Device added to resolving list */
  DM_PRIV_REM_DEV_FROM_RES_LIST_IND,      /*!< \brief Device removed from resolving list */
  DM_PRIV_CLEAR_RES_LIST_IND,             /*!< \brief Resolving list cleared */
  DM_PRIV_READ_PEER_RES_ADDR_IND,         /*!< \brief Peer resolving address read */
  DM_PRIV_READ_LOCAL_RES_ADDR_IND,        /*!< \brief Local resolving address read */
  DM_PRIV_SET_ADDR_RES_ENABLE_IND,        /*!< \brief Address resolving enable set */
  DM_REM_CONN_PARAM_REQ_IND,              /*!< \brief Remote connection parameter requested */
  DM_CONN_DATA_LEN_CHANGE_IND,            /*!< \brief Data length changed */
  DM_CONN_WRITE_AUTH_TO_IND,              /*!< \brief Write authenticated payload complete */
  DM_CONN_AUTH_TO_EXPIRED_IND,            /*!< \brief Authenticated payload timeout expired */
  DM_PHY_READ_IND,                        /*!< \brief Read PHY */
  DM_PHY_SET_DEF_IND,                     /*!< \brief Set default PHY */
  DM_PHY_UPDATE_IND,                      /*!< \brief PHY update */
  DM_ADV_SET_START_IND,                   /*!< \brief Advertising set(s) started */
  DM_ADV_SET_STOP_IND,                    /*!< \brief Advertising set(s) stopped */
  DM_SCAN_REQ_RCVD_IND,                   /*!< \brief Scan request received */
  DM_EXT_SCAN_START_IND,                  /*!< \brief Extended scanning started */
  DM_EXT_SCAN_STOP_IND,                   /*!< \brief Extended scanning stopped */
  DM_EXT_SCAN_REPORT_IND,                 /*!< \brief Extended scan data received from peer device */
  DM_PER_ADV_SET_START_IND,               /*!< \brief Periodic advertising set started */
  DM_PER_ADV_SET_STOP_IND,                /*!< \brief Periodic advertising set stopped */
  DM_PER_ADV_SYNC_EST_IND,                /*!< \brief Periodic advertising sync established */
  DM_PER_ADV_SYNC_EST_FAIL_IND,           /*!< \brief Periodic advertising sync establishment failed */
  DM_PER_ADV_SYNC_LOST_IND,               /*!< \brief Periodic advertising sync lost */
  DM_PER_ADV_SYNC_TRSF_EST_IND,           /*!< \brief Periodic advertising sync transfer established */
  DM_PER_ADV_SYNC_TRSF_EST_FAIL_IND,      /*!< \brief Periodic advertising sync transfer establishment failed */
  DM_PER_ADV_SYNC_TRSF_IND,               /*!< \brief Periodic advertising sync info transferred */
  DM_PER_ADV_SET_INFO_TRSF_IND,           /*!< \brief Periodic advertising set sync info transferred */
  DM_PER_ADV_REPORT_IND,                  /*!< \brief Periodic advertising data received from peer device */
  DM_REMOTE_FEATURES_IND,                 /*!< \brief Remote features from peer device */
  DM_READ_REMOTE_VER_INFO_IND,            /*!< \brief Remote LL version information read */
  DM_CONN_IQ_REPORT_IND,                  /*!< \brief IQ samples from CTE of received packet from peer device */
  DM_CTE_REQ_FAIL_IND,                    /*!< \brief CTE request failed */
  DM_CONN_CTE_RX_SAMPLE_START_IND,        /*!< \brief Sampling received CTE started */
  DM_CONN_CTE_RX_SAMPLE_STOP_IND,         /*!< \brief Sampling received CTE stopped */
  DM_CONN_CTE_TX_CFG_IND,                 /*!< \brief Connection CTE transmit parameters configured */
  DM_CONN_CTE_REQ_START_IND,              /*!< \brief Initiating connection CTE request started */
  DM_CONN_CTE_REQ_STOP_IND,               /*!< \brief Initiating connection CTE request stopped */
  DM_CONN_CTE_RSP_START_IND,              /*!< \brief Responding to connection CTE request started */
  DM_CONN_CTE_RSP_STOP_IND,               /*!< \brief Responding to connection CTE request stopped */
  DM_READ_ANTENNA_INFO_IND,               /*!< \brief Antenna information read */
  DM_L2C_CMD_REJ_IND,                     /*!< \brief L2CAP Command Reject */
  DM_ERROR_IND,                           /*!< \brief General error */
  DM_HW_ERROR_IND,                        /*!< \brief Hardware error */
  DM_VENDOR_SPEC_IND                      /*!< \brief Vendor specific event */
};

#define DM_CBACK_END                DM_VENDOR_SPEC_IND  /*!< \brief DM callback event ending value */
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Connection identifier. */
typedef uint8_t dmConnId_t;

/*! \brief Synchronization identifier. */
typedef uint8_t dmSyncId_t;

/*! \brief Configuration structure. */
typedef struct
{
  uint8_t dummy;  /*!< \brief Placeholder variable. */
} dmCfg_t;

/*! \brief LTK data type. */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];     /*!< \brief LTK */
  uint8_t                   rand[SMP_RAND8_LEN];  /*!< \brief Rand */
  uint16_t                  ediv;                 /*!< \brief EDIV */
} dmSecLtk_t;

/*! \brief IRK data type. */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];   /*!< \brief IRK */
  bdAddr_t                  bdAddr;             /*!< \brief BD Address */
  uint8_t                   addrType;           /*!< \brief Address Type */
} dmSecIrk_t;

/*! \brief CSRK data type. */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];   /*!< \brief CSRK */
} dmSecCsrk_t;

/*! \brief Union of key types. */
typedef union
{
  dmSecLtk_t                ltk;   /*!< \brief LTK */
  dmSecIrk_t                irk;   /*!< \brief IRK */
  dmSecCsrk_t               csrk;  /*!< \brief CSRK */
} dmSecKey_t;

/*! \brief Data type for \ref DM_SEC_PAIR_CMPL_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  uint8_t                   auth;         /*!< \brief Authentication and bonding flags */
} dmSecPairCmplIndEvt_t;

/*! \brief Data type for \ref DM_SEC_ENCRYPT_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  bool_t                    usingLtk;     /*!< \brief TRUE if connection encrypted with LTK */
} dmSecEncryptIndEvt_t;

/*! \brief Data type for \ref DM_SEC_AUTH_REQ_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  bool_t                    oob;          /*!< \brief Out-of-band data requested */
  bool_t                    display;      /*!< \brief TRUE if pin is to be displayed */
} dmSecAuthReqIndEvt_t;

/*! \brief Data type for \ref DM_SEC_PAIR_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  uint8_t                   auth;         /*!< \brief Authentication and bonding flags */
  bool_t                    oob;          /*!< \brief Out-of-band pairing data present or not present */
  uint8_t                   iKeyDist;     /*!< \brief Initiator key distribution flags */
  uint8_t                   rKeyDist;     /*!< \brief Responder key distribution flags */
} dmSecPairIndEvt_t;

/*! \brief Data type for \ref DM_SEC_SLAVE_REQ_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  uint8_t                   auth;         /*!< \brief Authentication and bonding flags */
} dmSecSlaveIndEvt_t;

/*! \brief Data type for \ref DM_SEC_KEY_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*!< \brief Header */
  dmSecKey_t                keyData;      /*!< \brief Key data */
  uint8_t                   type;         /*!< \brief Key type */
  uint8_t                   secLevel;     /*!< \brief Security level of pairing when key was exchanged */
  uint8_t                   encKeyLen;    /*!< \brief Length of encryption key used when data was transferred */
} dmSecKeyIndEvt_t;

/*! \brief Data type for \ref DM_SEC_COMPARE_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   confirm[SMP_CONFIRM_LEN];  /*!< \brief Confirm value */
} dmSecCnfIndEvt_t;

/*! \brief Data type for \ref DM_SEC_KEYPRESS_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   notificationType;          /*!< \brief Type of keypress notification */
} dmSecKeypressIndEvt_t;

/*! \brief Data type for \ref DM_PRIV_GENERATE_ADDR_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  bdAddr_t                  addr;                      /*!< \brief Resolvable private address */
} dmPrivGenAddrIndEvt_t;

/*! \brief Data type for \ref DM_SEC_CALC_OOB_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   confirm[SMP_CONFIRM_LEN];  /*!< \brief Local confirm value */
  uint8_t                   random[SMP_RAND_LEN];      /*!< \brief Local random value */
} dmSecOobCalcIndEvt_t;

/*! \brief Data type for \ref DM_ADV_NEW_ADDR_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  bdAddr_t                  addr;                      /*!< \brief New resolvable private address */
  bool_t                    firstTime;                 /*!< \brief TRUE when address is generated for the first time */
} dmAdvNewAddrIndEvt_t;

/*! \brief Data structure for \ref DM_ADV_SET_START_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   numSets;                   /*!< \brief Number of advertising sets */
  uint8_t                   advHandle[DM_NUM_ADV_SETS];/*!< \brief Advertising handle array */
} dmAdvSetStartEvt_t;

/*! \brief Data structure for \ref DM_PER_ADV_SET_START_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   advHandle;                 /*!< \brief Advertising handle */
} dmPerAdvSetStartEvt_t;

/*! \brief Data structure for \ref DM_PER_ADV_SET_STOP_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint8_t                   advHandle;                 /*!< \brief Advertising handle */
} dmPerAdvSetStopEvt_t;

/*! \brief Data structure for \ref DM_L2C_CMD_REJ_IND. */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*!< \brief Header */
  uint16_t                  reason;                    /*!< \brief Rejection reason */
  uint16_t                  handle;                    /*!< \brief Connection handle */
} dmL2cCmdRejEvt_t;

/*! \brief Union of DM callback event data types.
 *
 *  \note the following events use only the common \ref wsfMsgHdr_t header:
 *  \ref DM_RESET_CMPL_IND,
 *  \ref DM_ADV_START_IND,
 *  \ref DM_ADV_STOP_IND,
 *  \ref DM_SCAN_START_IND,
 *  \ref DM_SCAN_STOP_IND,
 *  \ref DM_SEC_PAIR_FAIL_IND,
 *  \ref DM_SEC_ENCRYPT_FAIL_IND,
 *  \ref DM_PRIV_RESOLVED_ADDR_IND,
 *  \ref DM_EXT_SCAN_START_IND,
 *  \ref DM_EXT_SCAN_STOP_IND,
 *  \ref DM_ERROR_IND
 */
typedef union
{
  wsfMsgHdr_t                         hdr;                  /*!< \brief Common header */
  /* common header used by                                                           DM_RESET_CMPL_IND */
  /* common header used by                                                           DM_ADV_START_IND */
  /* common header used by                                                           DM_ADV_STOP_IND */
  dmAdvNewAddrIndEvt_t                advNewAddr;           /*!< \brief handles \ref DM_ADV_NEW_ADDR_IND */
  /* common header used by                                                           DM_SCAN_START_IND */
  /* common header used by                                                           DM_SCAN_STOP_IND */
  hciLeAdvReportEvt_t                 scanReport;           /*!< \brief handles \ref DM_SCAN_REPORT_IND */
  hciLeConnCmplEvt_t                  connOpen;             /*!< \brief handles \ref DM_CONN_OPEN_IND */
  hciDisconnectCmplEvt_t              connClose;            /*!< \brief handles \ref DM_CONN_CLOSE_IND */
  hciLeConnUpdateCmplEvt_t            connUpdate;           /*!< \brief handles \ref DM_CONN_UPDATE_IND */
  dmSecPairCmplIndEvt_t               pairCmpl;             /*!< \brief handles \ref DM_SEC_PAIR_CMPL_IND */
  /* common header used by                                                           DM_SEC_PAIR_FAIL_IND */
  dmSecEncryptIndEvt_t                encryptInd;           /*!< \brief handles \ref DM_SEC_ENCRYPT_IND */
  /* common header used by                                                           DM_SEC_ENCRYPT_FAIL_IND */
  dmSecAuthReqIndEvt_t                authReq;              /*!< \brief handles \ref DM_SEC_AUTH_REQ_IND */
  dmSecKeyIndEvt_t                    keyInd;               /*!< \brief handles \ref DM_SEC_KEY_IND */
  hciLeLtkReqEvt_t                    ltkReqInd;            /*!< \brief handles \ref DM_SEC_LTK_REQ_IND */
  dmSecPairIndEvt_t                   pairInd;              /*!< \brief handles \ref DM_SEC_PAIR_IND */
  dmSecSlaveIndEvt_t                  slaveInd;             /*!< \brief handles \ref DM_SEC_SLAVE_REQ_IND */
  dmSecOobCalcIndEvt_t                oobCalcInd;           /*!< \brief handles \ref DM_SEC_CALC_OOB_IND */
  secEccMsg_t                         eccMsg;               /*!< \brief handles \ref DM_SEC_ECC_KEY_IND */
  dmSecCnfIndEvt_t                    cnfInd;               /*!< \brief handles \ref DM_SEC_COMPARE_IND */
  dmSecKeypressIndEvt_t               keypressInd;          /*!< \brief handles \ref DM_SEC_KEYPRESS_IND */
  /*  common header used by                                                          DM_PRIV_RESOLVED_ADDR_IND */
  dmPrivGenAddrIndEvt_t               genAddr;              /*!< \brief handles \ref DM_PRIV_GENERATE_ADDR_IND */
  hciReadRssiCmdCmplEvt_t             readRssi;             /*!< \brief handles \ref DM_CONN_READ_RSSI_IND */
  hciLeAddDevToResListCmdCmplEvt_t    addDevToResList;      /*!< \brief handles \ref DM_PRIV_ADD_DEV_TO_RES_LIST_IND */
  hciLeRemDevFromResListCmdCmplEvt_t  remDevFromResList;    /*!< \brief handles \ref DM_PRIV_REM_DEV_FROM_RES_LIST_IND */
  hciLeClearResListCmdCmplEvt_t       clearResList;         /*!< \brief handles \ref DM_PRIV_CLEAR_RES_LIST_IND */
  hciLeReadPeerResAddrCmdCmplEvt_t    readPeerResAddr;      /*!< \brief handles \ref DM_PRIV_READ_PEER_RES_ADDR_IND */
  hciLeReadLocalResAddrCmdCmplEvt_t   readLocalResAddr;     /*!< \brief handles \ref DM_PRIV_READ_LOCAL_RES_ADDR_IND */
  hciLeSetAddrResEnableCmdCmplEvt_t   setAddrResEnable;     /*!< \brief handles \ref DM_PRIV_SET_ADDR_RES_ENABLE_IND */
  hciLeRemConnParamReqEvt_t           remConnParamReq;      /*!< \brief handles \ref DM_REM_CONN_PARAM_REQ_IND */
  hciLeDataLenChangeEvt_t             dataLenChange;        /*!< \brief handles \ref DM_CONN_DATA_LEN_CHANGE_IND */
  hciWriteAuthPayloadToCmdCmplEvt_t   writeAuthTo;          /*!< \brief handles \ref DM_CONN_WRITE_AUTH_TO_IND */
  hciAuthPayloadToExpiredEvt_t        authToExpired;        /*!< \brief handles \ref DM_CONN_AUTH_TO_EXPIRED_IND */
  hciLeReadPhyCmdCmplEvt_t            readPhy;              /*!< \brief handles \ref DM_PHY_READ_IND */
  hciLeSetDefPhyCmdCmplEvt_t          setDefPhy;            /*!< \brief handles \ref DM_PHY_SET_DEF_IND */
  hciLePhyUpdateEvt_t                 phyUpdate;            /*!< \brief handles \ref DM_PHY_UPDATE_IND */
  dmAdvSetStartEvt_t                  advSetStart;          /*!< \brief handles \ref DM_ADV_SET_START_IND */
  hciLeAdvSetTermEvt_t                advSetStop;           /*!< \brief handles \ref DM_ADV_SET_STOP_IND */
  hciLeScanReqRcvdEvt_t               scanReqRcvd;          /*!< \brief handles \ref DM_SCAN_REQ_RCVD_IND */
  /* common header used by                                                           DM_EXT_SCAN_START_IND */
  /* common header used by                                                           DM_EXT_SCAN_STOP_IND */
  hciLeExtAdvReportEvt_t              extScanReport;         /*!< \brief handles \ref DM_EXT_SCAN_REPORT_IND */
  dmPerAdvSetStartEvt_t               perAdvSetStart;        /*!< \brief handles \ref DM_PER_ADV_SET_START_IND */
  dmPerAdvSetStopEvt_t                perAdvSetStop;         /*!< \brief handles \ref DM_PER_ADV_SET_STOP_IND */
  hciLePerAdvSyncEstEvt_t             perAdvSyncEst;         /*!< \brief handles \ref DM_PER_ADV_SYNC_EST_IND */
  hciLePerAdvSyncEstEvt_t             perAdvSyncEstFail;     /*!< \brief handles \ref DM_PER_ADV_SYNC_EST_FAIL_IND */
  hciLePerAdvSyncLostEvt_t            perAdvSyncLost;        /*!< \brief handles \ref DM_PER_ADV_SYNC_LOST_IND */
  HciLePerAdvSyncTrsfRcvdEvt_t        perAdvSyncTrsfEst;     /*!< \brief handles \ref DM_PER_ADV_SYNC_TRSF_EST_IND */
  HciLePerAdvSyncTrsfRcvdEvt_t        perAdvSyncTrsEstFail;  /*!< \brief handles \ref DM_PER_ADV_SYNC_TRSF_EST_FAIL_IND */
  hciLePerAdvSyncTrsfCmdCmplEvt_t     perAdvSyncTrsf;        /*!< \brief handles \ref DM_PER_ADV_SYNC_TRSF_IND */
  hciLePerAdvSetInfoTrsfCmdCmplEvt_t  perAdvSetInfoTrsf;     /*!< \brief handles \ref DM_PER_ADV_SET_INFO_TRSF_IND */
  hciLePerAdvReportEvt_t              perAdvReport;          /*!< \brief handles \ref DM_PER_ADV_REPORT_IND */
  hciLeReadRemoteFeatCmplEvt_t        readRemoteFeat;        /*!< \brief handles \ref DM_REMOTE_FEATURES_IND */
  hciReadRemoteVerInfoCmplEvt_t       readRemVerInfo;        /*!< \brief handles \ref DM_READ_REMOTE_VER_INFO_IND */
  hciLeConnIQReportEvt_t              connIQReport;          /*!< \brief handles \ref DM_CONN_IQ_REPORT_IND */
  hciLeCteReqFailedEvt_t              cteReqFail;            /*!< \brief handles \ref DM_CTE_REQ_FAIL_IND */
  hciLeSetConnCteRxParamsCmdCmplEvt_t connCteRxSampleStart;  /*!< \brief handles \ref DM_CONN_CTE_RX_SAMPLE_START_IND */
  hciLeSetConnCteRxParamsCmdCmplEvt_t connCteRxSampleStop;   /*!< \brief handles \ref DM_CONN_CTE_RX_SAMPLE_STOP_IND */
  hciLeSetConnCteTxParamsCmdCmplEvt_t connCteTxCfg;          /*!< \brief handles \ref DM_CONN_CTE_TX_CFG_IND */
  hciLeConnCteReqEnableCmdCmplEvt_t   connCteReqStart;       /*!< \brief handles \ref DM_CONN_CTE_REQ_START_IND */
  hciLeConnCteReqEnableCmdCmplEvt_t   connCteReqStop;        /*!< \brief handles \ref DM_CONN_CTE_REQ_STOP_IND */
  hciLeConnCteRspEnableCmdCmplEvt_t   connCteRspStart;       /*!< \brief handles \ref DM_CONN_CTE_RSP_START_IND */
  hciLeConnCteRspEnableCmdCmplEvt_t   connCteRspStop;        /*!< \brief handles \ref DM_CONN_CTE_RSP_STOP_IND */
  hciLeReadAntennaInfoCmdCmplEvt_t    readAntennaInfo;       /*!< \brief handles \ref DM_READ_ANTENNA_INFO_IND */
  dmL2cCmdRejEvt_t                    l2cCmdRej;             /*!< \brief handles \ref DM_L2C_CMD_REJ_IND */
  /* common header used by                                                            DM_ERROR_IND */
  hciHwErrorEvt_t                     hwError;               /*!< \brief handles \ref DM_HW_ERROR_IND */
  hciVendorSpecEvt_t                  vendorSpec;            /*!< \brief handles \ref DM_VENDOR_SPEC_IND */
} dmEvt_t;

/*! \brief Data type for DmSecSetOob(). */
typedef struct
{
  uint8_t localRandom[SMP_RAND_LEN];      /*!< \brief Random value of the local device */
  uint8_t localConfirm[SMP_CONFIRM_LEN];  /*!< \brief Confirm value of the local device */
  uint8_t peerRandom[SMP_RAND_LEN];       /*!< \brief Random value of the peer device */
  uint8_t peerConfirm[SMP_CONFIRM_LEN];   /*!< \brief Confirm value of the peer device */
} dmSecLescOobCfg_t;

/*! \brief Callback type. */
typedef void (*dmCback_t)(dmEvt_t *pDmEvt);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name DM App Callback Registration
 *
 */
/**@{*/
/*************************************************************************************************/
/*!
 *  \brief  Register a callback with DM for scan and advertising events.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmRegister(dmCback_t cback);

/**@}*/

/** \name DM Advertising Functions
 * Functions used to control Legacy and Extended Advertising.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Find an advertising data element in the given advertising or scan response data.
 *
 *  \param  adType  Advertising data element type to find.
 *  \param  dataLen Data length.
 *  \param  pData   Pointer to advertising or scan response data.
 *
 *  \return Pointer to the advertising data element byte array or NULL if not found.
 */
/*************************************************************************************************/
uint8_t *DmFindAdType(uint8_t adType, uint16_t dataLen, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM legacy advertising.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM extended advertising.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtAdvInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Whether DM advertising is in legacy mode.
 *
 *  \return TRUE if DM advertising is in legacy mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvModeLeg(void);

/*************************************************************************************************/
/*!
 *  \brief  Whether DM advertising is in extended mode.
 *
 *  \return TRUE if DM advertising is in extended mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvModeExt(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the advertising parameters using the given advertising type, and peer address.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  advType       Advertising type.
 *  \param  peerAddrType  Peer address type.
 *  \param  pPeerAddr     Peer address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvConfig(uint8_t advHandle, uint8_t advType, uint8_t peerAddrType, uint8_t *pPeerAddr);

/*************************************************************************************************/
/*!
 *  \brief  Set the advertising or scan response data to the given data.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  op            Data operation.
 *  \param  location      Data location.
 *  \param  len           Length of the data.  Maximum length is 236 bytes.
 *  \param  pData         Pointer to the data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetData(uint8_t advHandle, uint8_t op, uint8_t location, uint8_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Start advertising using the given advertising set and duration.
 *
 *  \param  numSets       Number of advertising sets to enable.
 *  \param  pAdvHandles   Advertising handles array.
 *  \param  pDuration     Advertising duration (in milliseconds) array.
 *  \param  pMaxEaEvents  Maximum number of extended advertising events array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvStart(uint8_t numSets, uint8_t *pAdvHandles, uint16_t *pDuration, uint8_t *pMaxEaEvents);

/*************************************************************************************************/
/*!
 *  \brief  Stop advertising for the given advertising set.  If the number of sets is set to 0
 *          then all advertising sets are disabled.
 *
 *  \param  numSets       Number of advertising sets to disable.
 *  \param  pAdvHandles   Advertising handles array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvStop(uint8_t numSets, uint8_t *pAdvHandles);

/*************************************************************************************************/
/*!
 *  \brief  Remove an advertising set.
 *
 *  \param  advHandle     Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvRemoveAdvSet(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Clear advertising sets.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvClearAdvSets(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the random device address for a given advertising set.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  pAddr         Random device address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetRandAddr(uint8_t advHandle, const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Set the minimum and maximum advertising intervals.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  intervalMin   Minimum advertising interval.
 *  \param  intervalMax   Maximum advertising interval.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetInterval(uint8_t advHandle, uint16_t intervalMin, uint16_t intervalMax);

/*************************************************************************************************/
/*!
 *  \brief  Include or exclude certain channels from the advertising channel map.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  channelMap    Advertising channel map.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetChannelMap(uint8_t advHandle, uint8_t channelMap);

/*************************************************************************************************/
/*!
 *  \brief  Set the local address type used while advertising.  This function can be used to
 *          configure advertising to use a random address.
 *
 *  \param  addrType      Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of an advertising data element in the given advertising or
 *          scan response data.  If the element already exists in the data then it is replaced
 *          with the new value.  If the element does not exist in the data it is appended
 *          to it, space permitting.
 *
 *  \param  adType        Advertising data element type.
 *  \param  len           Length of the value.  Maximum length is 29 bytes.
 *  \param  pValue        Pointer to the value.
 *  \param  pAdvDataLen   Advertising or scan response data length.  The new length is returned
 *                        in this parameter.
 *  \param  pAdvData      Pointer to advertising or scan response data.
 *  \param  advDataBufLen Length of the advertising or scan response data buffer maintained by
 *                        Application.
 *
 *  \return TRUE if the element was successfully added to the data, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvSetAdValue(uint8_t adType, uint8_t len, uint8_t *pValue, uint16_t *pAdvDataLen,
                       uint8_t *pAdvData, uint16_t advDataBufLen);

/*************************************************************************************************/
/*!
 *  \brief  Set the device name in the given advertising or scan response data.  If the
 *          name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  len           Length of the name.  Maximum length is 29 bytes.
 *  \param  pValue        Pointer to the name in UTF-8 format.
 *  \param  pAdvDataLen   Advertising or scan response data length.  The new length is returned
 *                        in this parameter.
 *  \param  pAdvData      Pointer to advertising or scan response data.
 *  \param  advDataBufLen Length of the advertising or scan response data buffer maintained by
 *                        Application.
 *
 *  \return TRUE if the element was successfully added to the data, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvSetName(uint8_t len, uint8_t *pValue, uint16_t *pAdvDataLen, uint8_t *pAdvData,
                    uint16_t advDataBufLen);

/*************************************************************************************************/
/*!
 *  \brief  Initialize device privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Start using a private resolvable address.
 *
 *  \param  changeInterval  Interval between automatic address changes, in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivStart(uint16_t changeInterval);

/*************************************************************************************************/
/*!
 *  \brief  Stop using a private resolvable address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Set whether or not to use legacy advertising PDUs with extended advertising.
 *
 *  \param  advHandle    Advertising handle.
 *  \param  useLegacyPdu Whether to use legacy advertising PDUs (default value is TRUE).
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvUseLegacyPdu(uint8_t advHandle, bool_t useLegacyPdu);

/*************************************************************************************************/
/*!
 *  \brief  Set whether or not to omit advertiser's address from all PDUs (anonymous advertising).
 *
 *  \param  advHandle    Advertising handle.
 *  \param  omitAdvAddr  Whether to omit advertiser's address from all PDUs (default value is FALSE).
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvOmitAdvAddr(uint8_t advHandle, bool_t omitAdvAddr);

/*************************************************************************************************/
/*!
 *  \brief  Set whether or not to include TxPower in extended header of advertising PDU.
 *
 *  \param  advHandle    Advertising handle.
 *  \param  incTxPwr     Whether to include TxPower in extended header of advertising PDU (default
 *                       value is FALSE).
 *  \param  advTxPwr     Advertising tx power (127 = no preference).
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvIncTxPwr(uint8_t advHandle, bool_t incTxPwr, int8_t advTxPwr);

/*************************************************************************************************/
/*!
 *  \brief  Set extended advertising PHY parameters.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  priAdvPhy     Primary advertising Phy.
 *  \param  secAdvMaxSkip Maximum advertising events Controller can skip before sending AUX_ADV_IND
 *                        on secondary advertising channel (0 = AUX_ADV_IND will be sent prior to
 *                        next advertising event).
 *  \param  secAdvPhy     Secondary advertising Phy.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetPhyParam(uint8_t advHandle, uint8_t priAdvPhy, uint8_t secAdvMaxSkip, uint8_t secAdvPhy);

/*************************************************************************************************/
/*!
 *  \brief  Set scan request notification enable.
 *
 *  \param  advHandle       Advertising handle.
 *  \param  scanReqNotifEna Scan request notification enable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvScanReqNotifEnable(uint8_t advHandle, bool_t scanReqNotifEna);

/*************************************************************************************************/
/*!
 *  \brief  Set fragment preference for advertising data.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  fragPref      Fragment preference.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetFragPref(uint8_t advHandle, uint8_t fragPref);

/*************************************************************************************************/
/*!
 *  \brief  Set the advertising parameters for periodic advertising.
 *
 *  \param  advHandle     Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvConfig(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Set the advertising data to the given data for periodic advertising.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  op            Data operation.
 *  \param  len           Length of the data.  Maximum length is 236 bytes.
 *  \param  pData         Pointer to the data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvSetData(uint8_t advHandle, uint8_t op, uint8_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Start periodic advertising for the advertising set specified by the advertising handle.
 *
 *  \param  advHandle     Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvStart(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Stop periodic advertising for the advertising set specified by the advertising handle.
 *
 *  \param  advHandle     Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvStop(uint8_t advHandle);


/*************************************************************************************************/
/*!
 *  \brief  Set the minimum and maximum advertising intervals for periodic advertising.
 *
 *  \param  advHandle     Advertising handle.
 *  \param  intervalMin   Minimum advertising interval.
 *  \param  intervalMax   Maximum advertising interval.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvSetInterval(uint8_t advHandle, uint16_t intervalMin, uint16_t intervalMax);

/*************************************************************************************************/
/*!
 *  \brief  Set whether or not to include TxPower in extended header of advertising PDU for
 *          periodic advertising.
 *
 *  \param  advHandle    Advertising handle.
 *  \param  incTxPwr     Whether to include TxPower in extended header of advertising PDU (default
 *                       value is FALSE).
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPerAdvIncTxPwr(uint8_t advHandle, bool_t incTxPwr);

/*************************************************************************************************/
/*!
 *  \brief  Get the maximum advertising data length supported by Controller for a given advertising
 *          type.
 *
 *  \param  advType      Advertising type.
 *  \param  useLegacyPdu Whether to use legacy advertising PDUs with extended advertising.
 *
 *  \return Maximum advertising data length.
 */
/*************************************************************************************************/
uint16_t DmExtMaxAdvDataLen(uint8_t advType, bool_t useLegacyPdu);

/**@}*/

/** \name DM Privacy Functions
 * Functions for controlling Privacy.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Resolve a private resolvable address.  When complete the client's callback function
 *          is called with a DM_PRIV_RESOLVED_ADDR_IND event.  The client must wait to receive
 *          this event before executing this function again.
 *
 *  \param  pAddr     Peer device address.
 *  \param  pIrk      The peer's identity resolving key.
 *  \param  param     Client-defined parameter returned with callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivResolveAddr(uint8_t *pAddr, uint8_t *pIrk, uint16_t param);

/*************************************************************************************************/
/*!
 *  \brief  Add device to resolving list.  When complete the client's callback function
 *          is called with a DM_PRIV_ADD_DEV_TO_RES_LIST_IND event.  The client must wait
 *          to receive this event before executing this function again.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  pPeerIrk      The peer's identity resolving key.
 *  \param  pLocalIrk     The local identity resolving key.
 *  \param  enableLlPriv  Set to TRUE to enable address resolution in LL.
 *  \param  param         client-defined parameter returned with callback event.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 *
 *  \Note   If the local or peer IRK associated with the peer Identity Address is all zeros then
 *          the Controller will use or accept the local or peer Identity Address respectively.
 *
 *  \Note   Parameter 'enableLlPriv' should be set to TRUE when the last device is being added
 *          to resolving list to enable address resolution in the Controller.
 */
/*************************************************************************************************/
void DmPrivAddDevToResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t *pPeerIrk,
                           uint8_t *pLocalIrk, bool_t enableLlPriv, uint16_t param);

/*************************************************************************************************/
/*!
 *  \brief  Remove device from resolving list.  When complete the client's callback function
 *          is called with a DM_PRIV_REM_DEV_FROM_RES_LIST_IND event.  The client must wait to
 *          receive this event before executing this function again.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  param         client-defined parameter returned with callback event.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivRemDevFromResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint16_t param);

/*************************************************************************************************/
/*!
 *  \brief  Clear resolving list.  When complete the client's callback function is called with a
 *          DM_PRIV_CLEAR_RES_LIST_IND event.  The client must wait to receive this event before
 *          executing this function again.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 *
 *  \Note   Address resolution in Controller will be disabled when resolving list's cleared
 *          successfully.
 */
/*************************************************************************************************/
void DmPrivClearResList(void);

/*************************************************************************************************/
/*!
 *  \brief  HCI read peer resolvable address command.  When complete the client's callback
 *          function is called with a DM_PRIV_READ_PEER_RES_ADDR_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivReadPeerResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief  Read local resolvable address command.  When complete the client's callback
 *          function is called with a DM_PRIV_READ_LOCAL_RES_ADDR_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivReadLocalResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief  Enable or disable address resolution in LL.  When complete the client's callback
 *          function is called with a DM_PRIV_SET_ADDR_RES_ENABLE_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  enable   Set to TRUE to enable address resolution or FALSE to disable it.
 *
 *  \return None.
 *
 *  \Note   This command can be used at any time except when:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivSetAddrResEnable(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief  Set resolvable private address timeout command.
 *
 *  \param  rpaTimeout    Timeout measured in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivSetResolvablePrivateAddrTimeout(uint16_t rpaTimeout);

/*************************************************************************************************/
/*!
 *  \brief  Set privacy mode for a given entry in the resolving list.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  mode          Privacy mode (by default, network privacy mode is used).
 *
 *  \return None.
 *
 *  \Note   This command can be used at any time except when:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivSetPrivacyMode(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t mode);

/*************************************************************************************************/
/*!
 *  \brief  Generate a Resolvable Private Address (RPA).
 *
 *  \param  pIrk      The identity resolving key.
 *  \param  param     Client-defined parameter returned with callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivGenerateAddr(uint8_t *pIrk, uint16_t param);

/*************************************************************************************************/
/*!
 *  \brief  Whether LL Privacy is enabled.
 *
 *  \return TRUE if LL Privacy is enabled. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmLlPrivEnabled(void);

/**@}*/

/** \name DM Scanner Functions
 * Functions for controlling Legacy and Extended Scanner behavior.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM legacy scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM extended scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtScanInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM Periodic Advertising Sync Transfer (PAST) module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM Connection Constant Tone Extension (CTE) module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Whether DM scanning is in legacy mode.
 *
 *  \return TRUE if DM scanning is in legacy mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmScanModeLeg(void);

/*************************************************************************************************/
/*!
 *  \brief  Whether DM scanning is in extended mode.
 *
 *  \return TRUE if DM scanning is in extended mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmScanModeExt(void);

/*************************************************************************************************/
/*!
 *  \brief  Start scanning on the given PHYs.
 *
 *  \param  scanPhys  Scanner PHYs.
 *  \param  mode      Discoverability mode.
 *  \param  pScanType Scan type array.
 *  \param  filterDup Filter duplicates.  Set to TRUE to filter duplicate responses received
 *                    from the same device.  Set to FALSE to receive all responses.
 *  \param  duration  The scan duration, in milliseconds.  If set to zero or both duration and
 *                    period set to non-zero, scanning will continue until DmScanStop() is called.
 *  \param  period    The scan period, in 1.28 sec units (only applicable to AE).  If set to zero,
 *                    periodic scanning is disabled.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void DmScanStart(uint8_t scanPhys, uint8_t mode, const uint8_t *pScanType, bool_t filterDup,
                 uint16_t duration, uint16_t period);

/*************************************************************************************************/
/*!
 *  \brief  Stop scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for the specified PHYs.
 *
 *  \param  scanPhys      Scanning PHYs.
 *  \param  pScanInterval Scan interval array.
 *  \param  pScanWindow   Scan window array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanSetInterval(uint8_t scanPhys, uint16_t *pScanInterval, uint16_t *pScanWindow);

/*************************************************************************************************/
/*!
 *  \brief  Set the local address type used while scanning.  This function can be used to
 *          configure scanning to use a random address.
 *
 *  \param  addrType  Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanSetAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \brief  Synchronize with periodic advertising from the given advertiser, and start receiving
 *          periodic advertising packets.
 *
 *          Note: The synchronization filter policy is used to determine whether the periodic
 *                advertiser list is used. If the periodic advertiser list is not used, the
 *                advertising SID, advertiser address type, and advertiser address parameters
 *                specify the periodic advertising device to listen to; otherwise these parameters
 *                are ignored.
 *
 *  \param  advSid        Advertising SID.
 *  \param  advAddrType   Advertiser address type.
 *  \param  pAdvAddr      Advertiser address.
 *  \param  skip          Number of periodic advertising packets that can be skipped after
 *                        successful receive.
 *  \param  syncTimeout   Synchronization timeout.
 *
 *  \return Sync indentifier.
 */
/*************************************************************************************************/
dmSyncId_t DmSyncStart(uint8_t advSid, uint8_t advAddrType, const uint8_t *pAdvAddr, uint16_t skip,
                       uint16_t syncTimeout);

/*************************************************************************************************/
/*!
 *  \brief  Stop reception of the periodic advertising identified by the given sync identifier.
 *
 *  \param  syncId        Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSyncStop(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  DM enable or disable initial periodic advertisement reporting.
 *
 *  \param  enable        TRUE to enable initial reporting, FALSE to disable initial reporting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSyncInitialRptEnable(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief  Add device to periodic advertiser list.
 *
 *  \param  advAddrType   Advertiser address type.
 *  \param  pAdvAddr      Advertiser address.
 *  \param  advSid        Advertising SID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAddDeviceToPerAdvList(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid);

/*************************************************************************************************/
/*!
 *  \brief  DM remove device from periodic advertiser list.
 *
 *  \param  advAddrType   Advertiser address type.
 *  \param  pAdvAddr      Advertiser address.
 *  \param  advSid        Advertising SID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmRemoveDeviceFromPerAdvList(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid);

/*************************************************************************************************/
/*!
 *  \brief  DM clear periodic advertiser list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmClearPerAdvList(void);

/*************************************************************************************************/
/*!
 *  \brief  Enable or disable reports for the periodic advertising identified by the sync id.
 *
 *  \param  syncId           Sync identifier.
 *  \param  enable           TRUE to enable reporting, FALSE to disable reporting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastRptRcvEnable(dmSyncId_t syncId, bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief  Send synchronization information about the periodic advertising identified by the
 *          sync id to a connected device.
 *
 *  \param  connId           Connection identifier.
 *  \param  serviceData      Value provided by the Host.
 *  \param  syncId           Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastSyncTrsf(dmConnId_t connId, uint16_t serviceData, dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Send synchronization information about the periodic advertising in an advertising
 *          set to a connected device.
 *
 *  \param  connId           Connection identifier.
 *  \param  serviceData      Value provided by the Host.
 *  \param  advHandle        Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastSetInfoTrsf(dmConnId_t connId, uint16_t serviceData, uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Specify how the Controller should process periodic advertising synchronization
 *          information received from the device identified by the connnection handle.
 *
 *  \param  connId           Connection identifier.
 *  \param  mode             Action to be taken when periodic advertising info is received.
 *  \param  skip             Number of consecutive periodic advertising packets that the receiver
 *                           may skip after successfully receiving a periodic advertising packet.
 *  \param  syncTimeout      Maximum permitted time between successful receives. If this time is
 *                           exceeded, synchronization is lost.
 *  \param  cteType          Whether to only synchronize to periodic advertising with certain
 *                           types of Constant Tone Extension.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastConfig(dmConnId_t connId, uint8_t mode, uint16_t skip, uint16_t syncTimeout,
                  uint8_t cteType);

/*************************************************************************************************/
/*!
 *  \brief  Specify the initial value for the mode, skip, timeout, and Constant Tone Extension type
 *          to be used for all subsequent connections over the LE transport.
 *
 *  \param  mode             Action to be taken when periodic advertising info is received.
 *  \param  skip             Number of consecutive periodic advertising packets that the receiver
 *                           may skip after successfully receiving a periodic advertising packet.
 *  \param  syncTimeout      Maximum permitted time between successful receives. If this time is
 *                           exceeded, synchronization is lost.
 *  \param  cteType          Whether to only synchronize to periodic advertising with certain
 *                           types of Constant Tone Extension.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPastDefaultConfig(uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType);

/*************************************************************************************************/
/*!
 *  \brief  Enable sampling received CTE fields on the specified connection, and configure the
 *          antenna switching pattern, and switching and sampling slot durations to be used.
 *
 *  \param  connId           Connection identifier.
 *  \param  slotDurations    Switching and sampling slot durations to be used while receiving CTE.
 *  \param  switchPatternLen Number of Antenna IDs in switching pattern.
 *  \param  pAntennaIDs      List of Antenna IDs in switching pattern.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteRxSampleStart(dmConnId_t connId, uint8_t slotDurations, uint8_t switchPatternLen,
                            uint8_t *pAntennaIDs);

/*************************************************************************************************/
/*!
 *  \brief  Disable sampling received CTE fields on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteRxSampleStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Configure the antenna switching pattern, and permitted CTE types used for transmitting
 *          CTEs requested by the peer device on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *  \param  cteTypeBits      Permitted CTE type bits used for transmitting CTEs requested by peer.
 *  \param  switchPatternLen Number of Antenna IDs in switching pattern.
 *  \param  pAntennaIDs      List of Antenna IDs in switching pattern.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteTxConfig(dmConnId_t connId, uint8_t cteTypeBits, uint8_t switchPatternLen,
                       uint8_t *pAntennaIDs);

/*************************************************************************************************/
/*!
 *  \brief  Initiate the CTE Request procedure on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *  \param  cteReqInt        CTE request interval.
 *  \param  reqCteLen        Minimum length of CTE being requested in 8 us units.
 *  \param  reqCteType       Requested CTE type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteReqStart(dmConnId_t connId, uint16_t cteReqInt, uint8_t reqCteLen,
                       uint8_t reqCteType);

/*************************************************************************************************/
/*!
 *  \brief  Stop initiating the CTE Request procedure on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteReqStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Start responding to LL_CTE_REQ PDUs with LL_CTE_RSP PDUs on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteRspStart(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Stop responding to LL_CTE_REQ PDUs with LL_CTE_RSP PDUs on the specified connection.
 *
 *  \param  connId           Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnCteRspStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the switching rates, the sampling rates, the number of antennae, and the maximum
 *          length of a transmitted Constant Tone Extension supported by the Controller.
 *
 *  \return None.
 *
 *  \note   The antenna info will be returned with DM indication \ref DM_READ_ANTENNA_INFO_IND.
 */
/*************************************************************************************************/
void DmReadAntennaInfo(void);

/**@}*/

/** \name DM Connection Functions
 * Functions for forming connections and managing connection behavior and parameter updates.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as legacy master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as extended master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as legacy slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as extended slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Register with the DM connection manager.
 *
 *  \param  clientId  The client identifier.
 *  \param  cback     Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnRegister(uint8_t clientId, dmCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Open a connection to a peer device with the given address.
 *
 *  \param  clientId  The client identifier.
 *  \param  initPhys  Initiator PHYs.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnOpen(uint8_t clientId, uint8_t initPhys, uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Close the connection with the give connection identifier.
 *
 *  \param  clientId  The client identifier.
 *  \param  connId    Connection identifier.
 *  \param  reason    Reason connection is being closed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnClose(uint8_t clientId, dmConnId_t connId, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief  Accept a connection from the given peer device by initiating directed advertising.
 *
 *  \param  clientId     The client identifier.
 *  \param  advHandle    Advertising handle.
 *  \param  advType      Advertising type.
 *  \param  duration     Advertising duration (in ms).
 *  \param  maxEaEvents  Maximum number of extended advertising events.
 *  \param  addrType     Address type.
 *  \param  pAddr        Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnAccept(uint8_t clientId, uint8_t advHandle, uint8_t advType, uint16_t duration,
                        uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Update the connection parameters of an open connection
 *
 *  \param  connId      Connection identifier.
 *  \param  pConnSpec   Connection specification.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnUpdate(dmConnId_t connId, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for connections to be created with DmConnOpen().
 *
 *  \param  scanInterval  The scan interval.
 *  \param  scanWindow    The scan window.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetScanInterval(uint16_t scanInterval, uint16_t scanWindow);

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for extended connections to be created with
 *          DmConnOpen().
 *
 *  \param  initPhys      Initiator PHYs.
 *  \param  pScanInterval Scan interval array.
 *  \param  pScanWindow   Scan window array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSetScanInterval(uint8_t initPhys, uint16_t *pScanInterval, uint16_t *pScanWindow);

/*************************************************************************************************/
/*!
 *  \brief  Set the connection spec parameters for connections to be created with DmConnOpen().
 *
 *  \param  pConnSpec   Connection spec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetConnSpec(hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  Set the extended connection spec parameters for extended connections to be created
 *          with DmConnOpen().
 *
 *  \param  initPhys    The initiator PHYs.
 *  \param  pConnSpec   Connection spec parameters array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSetConnSpec(uint8_t initPhys, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  Set the local address type used for connections created with DmConnOpen().
 *
 *  \param  addrType  Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \brief  Configure a bit in the connection idle state mask as busy or idle.
 *
 *  \param  connId      Connection identifier.
 *  \param  idleMask    Bit in the idle state mask to configure.
 *  \param  idle        DM_CONN_BUSY or DM_CONN_IDLE.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetIdle(dmConnId_t connId, uint16_t idleMask, uint8_t idle);

/*************************************************************************************************/
/*!
 *  \brief  Check if a connection is idle.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return Zero if connection is idle, nonzero if busy.
 */
/*************************************************************************************************/
uint16_t DmConnCheckIdle(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read RSSI of a given connection.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnReadRssi(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Reply to the HCI remote connection parameter request event.  This command is used to
 *          indicate that the Host has accepted the remote device's request to change connection
 *          parameters.
 *
 *  \param  connId      Connection identifier.
 *  \param  pConnSpec   Connection specification.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmRemoteConnParamReqReply(dmConnId_t connId, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  Negative reply to the HCI remote connection parameter request event.  This command
 *          is used to indicate that the Host has rejected the remote device's request to change
 *          connection parameters.
 *
 *  \param  connId      Connection identifier.
 *  \param  reason      Reason for rejection.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmRemoteConnParamReqNegReply(dmConnId_t connId, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief  Set data length for a given connection.
 *
 *  \param  connId      Connection identifier.
 *  \param  txOctets    Maximum number of payload octets for a Data PDU.
 *  \param  txTime      Maximum number of microseconds for a Data PDU.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetDataLen(dmConnId_t connId, uint16_t txOctets, uint16_t txTime);

/*************************************************************************************************/
/*!
 *  \brief  Return the connection role indicating master or slave.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return Device role.
 */
/*************************************************************************************************/
uint8_t DmConnRole(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Set authenticated payload timeout for a given connection.
 *
 *  \param  connId      Connection identifier.
 *  \param  timeout     Timeout period in units of 10ms.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmWriteAuthPayloadTimeout(dmConnId_t connId, uint16_t timeout);

/**@}*/

/** \name DM PHY Control Functions
 * Functions for setting PHY preferences.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Set the preferred values for the transmitter PHY and receiver PHY for all subsequent
 *          connections.
 *
 *  \param  allPhys     All PHYs preferences.
 *  \param  txPhys      Preferred transmitter PHYs.
 *  \param  rxPhys      Preferred receiver PHYs.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys);

/*************************************************************************************************/
/*!
 *  \brief  Read the current transmitter PHY and receiver PHY for a given connection.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmReadPhy(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Set the PHY preferences for a given connection.
 *
 *  \param  connId      Connection identifier.
 *  \param  allPhys     All PHYs preferences.
 *  \param  txPhys      Preferred transmitter PHYs.
 *  \param  rxPhys      Preferred receiver PHYs.
 *  \param  phyOptions  PHY options.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSetPhy(dmConnId_t connId, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions);

/*************************************************************************************************/
/*!
*  \brief  Initialize DM PHY.
*
*  \return None.
*/
/*************************************************************************************************/
void DmPhyInit(void);

/**@}*/


/** \name DM Device Functions
 *  Device control functions
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Reset the device.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevReset(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the random address to be used by the local device.
 *
 *  \param  pAddr     Random address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevSetRandAddr(uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Add a peer device to the white list.  Note that this function cannot be called
 *          while advertising, scanning, or connecting with white list filtering active.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListAdd(uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Remove a peer device from the white list.  Note that this function cannot be called
 *          while advertising, scanning, or connecting with white list filtering active.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListRemove(uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Clear the white list.  Note that this function cannot be called while
 *          advertising, scanning, or connecting with white list filtering active.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListClear(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the Advertising, Scanning or Initiator filter policy.
 *
 *  \param  mode     Policy mode.
 *  \param  policy   Filter policy.
 *
 *  \return TRUE if the filter policy was successfully set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmDevSetFilterPolicy(uint8_t mode, uint8_t policy);

/*************************************************************************************************/
/*!
 *  \brief  Set the Advertising filter policy for the given advertising, Scanning or Initiator
 *          filter policy.
 *
 *  \param  advHandle  Advertising handle (only applicable to advertising).
 *  \param  mode       Policy mode.
 *  \param  policy     Filter policy.
 *
 *  \return TRUE if the filter policy was successfully set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmDevSetExtFilterPolicy(uint8_t advHandle, uint8_t mode, uint8_t policy);

/*************************************************************************************************/
/*!
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevVsInit(uint8_t param);

/**@}*/

/** \name DM Security Functions
 * Functions for accessing and controlling security configuration of device.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM LE Secure Connections security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecLescInit(void);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a master device to initiate pairing.
 *
 *  \param  connId    DM connection ID.
 *  \param  oob       Out-of-band pairing data present or not present.
 *  \param  auth      Authentication and bonding flags.
 *  \param  iKeyDist  Initiator key distribution flags.
 *  \param  rKeyDist  Responder key distribution flags.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecPairReq(dmConnId_t connId, bool_t oob, uint8_t auth, uint8_t iKeyDist, uint8_t rKeyDist);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave device to proceed with pairing after a
 *          DM_SEC_PAIR_IND event is received.
 *
 *  \param  connId    DM connection ID.
 *  \param  oob       Out-of-band pairing data present or not present.
 *  \param  auth      Authentication and bonding flags.
 *  \param  iKeyDist  Initiator key distribution flags.
 *  \param  rKeyDist  Responder key distribution flags.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecPairRsp(dmConnId_t connId, bool_t oob, uint8_t auth, uint8_t iKeyDist, uint8_t rKeyDist);

/*************************************************************************************************/
/*!
 *  \brief  This function is called to cancel the pairing process.
 *
 *  \param  connId    DM connection ID.
 *  \param  reason    Failure reason.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecCancelReq(dmConnId_t connId, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief  This function is called in response to a DM_SEC_AUTH_REQ_IND event to provide
 *          PIN or OOB data during pairing.
 *
 *  \param  connId      DM connection ID.
 *  \param  authDataLen Length of PIN or OOB data.
 *  \param  pAuthData   pointer to PIN or OOB data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecAuthRsp(dmConnId_t connId, uint8_t authDataLen, uint8_t *pAuthData);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave device to request that the master initiates
 *          pairing or link encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  auth      Authentication flags.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSlaveReq(dmConnId_t connId, uint8_t auth);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a master device to initiate link encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  secLevel  Security level of pairing when LTK was exchanged.
 *  \param  pLtk      Pointer to LTK parameter structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecEncryptReq(dmConnId_t connId, uint8_t secLevel, dmSecLtk_t *pLtk);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave in response to a DM_SEC_LTK_REQ_IND event
 *          to provide the long term key used for encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  keyFound  TRUE if key found.
 *  \param  secLevel  Security level of pairing when key was exchanged.
 *  \param  pKey      Pointer to the key, if found.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecLtkRsp(dmConnId_t connId, bool_t keyFound, uint8_t secLevel, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  This function sets the local CSRK used by the device.
 *
 *  \param  pCsrk     Pointer to CSRK.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetLocalCsrk(uint8_t *pCsrk);

/*************************************************************************************************/
/*!
 *  \brief  This function sets the local IRK used by the device.
 *
 *  \param  pIrk     Pointer to IRK.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetLocalIrk(uint8_t *pIrk);

/*************************************************************************************************/
/*!
 *  \brief  This function generates an ECC key for use with LESC security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecGenerateEccKeyReq(void);

/*************************************************************************************************/
/*!
 *  \brief  This function sets the ECC key for use with LESC security.
 *
 *  \param  pKey      Pointer to key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetEccKey(secEccKey_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  This function gets the local ECC key for use with LESC security.
 *
 *  \return Pointer to local ECC key.
 */
/*************************************************************************************************/
secEccKey_t *DmSecGetEccKey(void);

/*************************************************************************************************/
/*!
 *  \brief  This function sets the ECC key for use with LESC security to standard debug keys values.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetDebugEccKey(void);

/*************************************************************************************************/
/*!
 *  \brief  This function configures the DM to use OOB pairing for the given connection.
 *          The pRand and pConfirm contain the Random and Confirm values exchanged via
 *          out of band methods.
 *
 *  \param  connId      ID of the connection.
 *  \param  pConfig     Pointer to OOB configuration.
 *
 *  \return Pointer to IRK.
 */
/*************************************************************************************************/
void DmSecSetOob(dmConnId_t connId, dmSecLescOobCfg_t *pConfig);

/*************************************************************************************************/
/*!
 *  \brief  This function calculates the local random and confirm values used in LESC OOB pairing.
 *          The operation's result is posted as a DM_SEC_CALC_OOB_IND event to the application's DM
 *          callback handler.  The local rand and confirm values are exchanged with the peer via
 *          out-of-band (OOB) methods and passed into the DmSecSetOob after DM_CONN_OPEN_IND.
 *
 *  \param  pRand       Random value used in calculation.
 *  \param  pPubKeyX    X component of the local public key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecCalcOobReq(uint8_t *pRand, uint8_t *pPubKeyX);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by the application in response to a DM_SEC_COMPARE_IND event.
 *          The valid parameter indicates if the compare value of the DM_SEC_COMPARE_IND was valid.
 *
 *  \param  connId      ID of the connection.
 *  \param  valid       TRUE if compare value was valid
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecCompareRsp(dmConnId_t connId, bool_t valid);

/*************************************************************************************************/
/*!
 *  \brief  This function returns the 6-digit compare value for the specified 128-bit confirm value.
 *
 *  \param  pConfirm    Pointer to 128-bit comfirm value.
 *
 *  \return Six-digit compare value.
 */
/*************************************************************************************************/
uint32_t DmSecGetCompareValue(uint8_t *pConfirm);

/**@}*/

/** \name DM Internal Functions
 * Functions called internally by the stack.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Map an address type to a type used by LL.
 *
 *  \param  addrType   Address type used by Host.
 *
 *  \return Address type used by LL.
 */
/*************************************************************************************************/
uint8_t DmLlAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \brief  Map an address type to a type used by Host.
 *
 *  \param  addrType   Address type used by LL.
 *
 *  \return Address type used by Host.
 */
/*************************************************************************************************/
uint8_t DmHostAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \brief  Return size of a DM callback event.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return Size of DM callback event.
 */
/*************************************************************************************************/
uint16_t DmSizeOfEvt(dmEvt_t *pDmEvt);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function to send the result of an L2CAP
 *          connection update response to DM.
 *
 *  \param  handle  Connection handle.
 *  \param  reason  Connection update response reason code.
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cConnUpdateCnf(uint16_t handle, uint16_t reason);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function to send the result of an L2CAP
 *          Command Reject up to the application.
 *
 *  \param  handle  Connection handle.
 *  \param  result  Connection update result code.
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cCmdRejInd(uint16_t handle, uint16_t result);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function when it receives a connection update
 *          request from a peer device.
 *
 *  \param  identifier  Identifier value.
 *  \param  handle      Connection handle.
 *  \param  pConnSpec   Connection spec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cConnUpdateInd(uint8_t identifier, uint16_t handle, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Find the connection ID with matching handle.
 *
 *  \param  handle  Handle to find.
 *
 *  \return Connection ID or DM_CONN_ID_NONE if error.
 */
/*************************************************************************************************/
dmConnId_t DmConnIdByHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return TRUE if the connection is in use.
 *
 *  \param  connId  Connection ID.
 *
 *  \return TRUE if the connection is in use, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmConnInUse(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the peer address type.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Peer address type.
 */
/*************************************************************************************************/
uint8_t DmConnPeerAddrType(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the peer device address.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Pointer to peer device address.
 */
/*************************************************************************************************/
uint8_t *DmConnPeerAddr(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the local address type.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Local address type.
 */
/*************************************************************************************************/
uint8_t DmConnLocalAddrType(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the local address.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Pointer to local address.
 */
/*************************************************************************************************/
uint8_t *DmConnLocalAddr(dmConnId_t connId);

/*************************************************************************************************/
/*!
*  \brief  For internal use only.  Return the peer resolvable private address (RPA).
*
*  \param  connId  Connection ID.
*
*  \return Pointer to peer RPA.
*/
/*************************************************************************************************/
uint8_t *DmConnPeerRpa(dmConnId_t connId);

/*************************************************************************************************/
/*!
*  \brief  For internal use only.  Return the local resolvable private address (RPA).
*
*  \param  connId  Connection ID.
*
*  \return Pointer to local RPA.
*/
/*************************************************************************************************/
uint8_t *DmConnLocalRpa(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the security level of the connection.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Security level of the connection.
 */
/*************************************************************************************************/
uint8_t DmConnSecLevel(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function is called by SMP to request encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  secLevel  Security level of pairing when key was exchanged.
 *  \param  pKey      Pointer to key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSmpEncryptReq(dmConnId_t connId, uint8_t secLevel, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Execute DM callback from SMP procedures.
 *
 *  \param  pDmEvt    Pointer to callback event data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSmpCbackExec(dmEvt_t *pDmEvt);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function gets the local CSRK used by the device.
 *
 *  \return Pointer to CSRK.
 */
/*************************************************************************************************/
uint8_t *DmSecGetLocalCsrk(void);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function gets the local IRK used by the device.
 *
 *  \return Pointer to IRK.
 */
/*************************************************************************************************/
uint8_t *DmSecGetLocalIrk(void);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Read the features of the remote device.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmReadRemoteFeatures(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the version info of the remote device.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmReadRemoteVerInfo(dmConnId_t connId);

/**@}*/

/*! \} */    /* STACK_DM_API */

#ifdef __cplusplus
};
#endif

#endif /* DM_API_H */
