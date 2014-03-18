/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
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

/**
 * @brief GAP API SVC numbers.
 */
enum BLE_GAP_SVCS
{
  SD_BLE_GAP_ADDRESS_SET  = BLE_GAP_SVC_BASE,  /**< Set own Bluetooth Address. */
  SD_BLE_GAP_ADDRESS_GET,                      /**< Get own Bluetooth Address. */
  SD_BLE_GAP_ADV_DATA_SET,                     /**< Set Advertisement Data. */
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
  SD_BLE_GAP_SEC_INFO_REPLY,                   /**< Reply with Security Information. */
  SD_BLE_GAP_CONN_SEC_GET,                     /**< Obtain connection security level. */
  SD_BLE_GAP_RSSI_START,                       /**< Start reporting of changes in RSSI. */ 
  SD_BLE_GAP_RSSI_STOP,                        /**< Stop reporting of changes in RSSI. */ 
};


/** @addtogroup BLE_GAP_DEFINES Defines
 * @{ */

/** @defgroup BLE_ERRORS_GAP SVC return values specific to GAP
 * @{ */
#define BLE_ERROR_GAP_UUID_LIST_MISMATCH            (NRF_GAP_ERR_BASE + 0x000)  /**< UUID list does not contain an integral number of UUIDs. */
#define BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST   (NRF_GAP_ERR_BASE + 0x001)  /**< Use of Whitelist not permitted with discoverable advertising. */
#define BLE_ERROR_GAP_INVALID_BLE_ADDR              (NRF_GAP_ERR_BASE + 0x002)  /**< The upper two bits of the address do not correspond to the specified address type. */
/** @} */


/** @defgroup BLE_GAP_ROLES GAP Roles
 * @note Not explicitly used in peripheral API, but will be relevant for central API.
 * @{ */
#define BLE_GAP_ROLE_INVALID     0x0            /**< Invalid Role. */
#define BLE_GAP_ROLE_PERIPH      0x1            /**< Peripheral Role. */
#define BLE_GAP_ROLE_CENTRAL     0x2            /**< Central Role. */
/** @} */


/** @defgroup BLE_GAP_TIMEOUT_SOURCES GAP Timeout sources
 * @{ */
#define BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT              0x00 /**< Advertisement timeout. */
#define BLE_GAP_TIMEOUT_SRC_SECURITY_REQUEST           0x01 /**< Security request timeout. */
/** @} */


/** @defgroup BLE_GAP_ADDR_TYPES GAP Address types
 * @{ */
#define BLE_GAP_ADDR_TYPE_PUBLIC                        0x00 /**< Public address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_STATIC                 0x01 /**< Random Static address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE     0x02 /**< Private Resolvable address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE 0x03 /**< Private Non-Resolvable address. */
/** @} */


/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            6


/** @defgroup BLE_GAP_AD_TYPE_DEFINITIONS GAP Advertising and Scan Response Data format
 *  @note Found at https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm
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
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 /**< Service Data. */
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 /**< Public Target Address. */
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 /**< Random Target Address. */
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 /**< Appearance. */
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF /**< Manufacturer Specific Data. */
/** @} */


/** @defgroup BLE_GAP_ADV_FLAGS GAP Advertisement Flags
 * @{ */
#define BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE         (0x01)   /**< LE Limited Discoverable Mode. */
#define BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE         (0x02)   /**< LE General Discoverable Mode. */
#define BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED         (0x04)   /**< BR/EDR not supported. */
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_CONTROLLER         (0x08)   /**< Simultaneous LE and BR/EDR, Controller. */
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_HOST               (0x10)   /**< Simultaneous LE and BR/EDR, Host. */
#define BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE Limited Discoverable Mode, BR/EDR not supported. */
#define BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE General Discoverable Mode, BR/EDR not supported. */
/** @} */


/** @defgroup BLE_GAP_ADV_INTERVALS GAP Advertising interval max and min
 * @{ */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */
 /** @}  */


/** @brief Maximum size of advertising data in octets. */
#define  BLE_GAP_ADV_MAX_SIZE       31


/** @defgroup BLE_GAP_ADV_TYPES GAP Advertising types
 * @{ */
#define BLE_GAP_ADV_TYPE_ADV_IND          0x00   /**< Connectable undirected. */
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND   0x01   /**< Connectable directed. */
#define BLE_GAP_ADV_TYPE_ADV_SCAN_IND     0x02   /**< Scannable undirected. */
#define BLE_GAP_ADV_TYPE_ADV_NONCONN_IND  0x03   /**< Non connectable undirected. */
/** @} */


/** @defgroup BLE_GAP_ADV_FILTER_POLICIES GAP Advertising filter policies
 * @{ */
#define BLE_GAP_ADV_FP_ANY                0x00   /**< Allow scan requests and connect requests from any device. */
#define BLE_GAP_ADV_FP_FILTER_SCANREQ     0x01   /**< Filter scan requests with whitelist. */
#define BLE_GAP_ADV_FP_FILTER_CONNREQ     0x02   /**< Filter connect requests with whitelist. */
#define BLE_GAP_ADV_FP_FILTER_BOTH        0x03   /**< Filter both scan and connect requests with whitelist. */
/** @} */


/** @defgroup BLE_GAP_ADV_TIMEOUT_VALUES GAP Advertising timeout values
 * @{ */
#define BLE_GAP_ADV_TIMEOUT_LIMITED_MAX      180 /**< Maximum advertising time in limited discoverable mode (TGAP(lim_adv_timeout) = 180s in spec (Addendum 2)). */
#define BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED  0 /**< Unlimited advertising in general discoverable mode. */
/** @} */


/** @defgroup BLE_GAP_DISC_MODES GAP Discovery modes
 * @{ */
#define BLE_GAP_DISC_MODE_NOT_DISCOVERABLE  0x00   /**< Not discoverable discovery Mode. */
#define BLE_GAP_DISC_MODE_LIMITED           0x01   /**< Limited Discovery Mode. */
#define BLE_GAP_DISC_MODE_GENERAL           0x02   /**< General Discovery Mode. */
/** @} */

/** @defgroup BLE_GAP_IO_CAPS GAP IO Capabilities
 * @{ */
#define BLE_GAP_IO_CAPS_DISPLAY_ONLY      0x00   /**< Display Only. */
#define BLE_GAP_IO_CAPS_DISPLAY_YESNO     0x01   /**< Display and Yes/No entry. */
#define BLE_GAP_IO_CAPS_KEYBOARD_ONLY     0x02   /**< Keyboard Only. */
#define BLE_GAP_IO_CAPS_NONE              0x03   /**< No I/O capabilities. */
#define BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY  0x04   /**< Keyboard and Display. */
/** @} */


/** @defgroup BLE_GAP_AUTH_KEY_TYPES GAP Authentication Key Types
 * @{ */
#define BLE_GAP_AUTH_KEY_TYPE_NONE        0x00   /**< No key (may be used to reject). */
#define BLE_GAP_AUTH_KEY_TYPE_PASSKEY     0x01   /**< 6-digit Passkey. */
#define BLE_GAP_AUTH_KEY_TYPE_OOB         0x02   /**< Out Of Band data. */
/** @} */

/** @defgroup BLE_GAP_SEC_STATUS GAP Security status
 * @{ */
#define BLE_GAP_SEC_STATUS_SUCCESS                0x00  /**< Successful parameters. */
#define BLE_GAP_SEC_STATUS_TIMEOUT                0x01  /**< Procedure timed out. */
#define BLE_GAP_SEC_STATUS_PDU_INVALID            0x02  /**< Invalid PDU received. */
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
/** @} */

/** @defgroup BLE_GAP_SEC_STATUS_SOURCES GAP Security status sources
 * @{ */
#define BLE_GAP_SEC_STATUS_SOURCE_LOCAL           0x00  /**< Local failure. */
#define BLE_GAP_SEC_STATUS_SOURCE_REMOTE          0x01  /**< Remote failure. */
/** @} */

/** @defgroup BLE_GAP_CP_LIMITS GAP Connection Parameters Limits
 * @{ */
#define BLE_GAP_CP_MIN_CONN_INTVL_NONE           0xFFFF  /**< No new minimum connction interval specified in connect parameters. */
#define BLE_GAP_CP_MIN_CONN_INTVL_MIN            0x0006  /**< Lowest mimimum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */
#define BLE_GAP_CP_MIN_CONN_INTVL_MAX            0x0C80  /**< Highest minimum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */
#define BLE_GAP_CP_MAX_CONN_INTVL_NONE           0xFFFF  /**< No new maximum connction interval specified in connect parameters. */
#define BLE_GAP_CP_MAX_CONN_INTVL_MIN            0x0006  /**< Lowest maximum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */
#define BLE_GAP_CP_MAX_CONN_INTVL_MAX            0x0C80  /**< Highest maximum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */
#define BLE_GAP_CP_SLAVE_LATENCY_MAX             0x03E8  /**< Highest slave latency permitted, in connection events. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_NONE         0xFFFF  /**< No new supervision timeout specified in connect parameters. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN          0x000A  /**< Lowest supervision timeout permitted, in units of 10 ms, i.e. 100 ms. */
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX          0x0C80  /**< Highest supervision timeout permitted, in units of 10 ms, i.e. 32 s. */
/** @} */


/**@brief GAP device name maximum length. */
#define BLE_GAP_DEVNAME_MAX_LEN           31


/** @defgroup BLE_GAP_CONN_SEC_MODE_SET_MACROS GAP attribute security requirement setters
 *
 * See @ref ble_gap_conn_sec_mode_t.
 * @{ */
/** @brief Set sec_mode pointed to by ptr to have no access rights.*/
#define BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(ptr)         do {(ptr)->sm = 0; (ptr)->lv = 0;} while(0)
/** @brief Set sec_mode pointed to by ptr to require no protection, open link.*/
#define BLE_GAP_CONN_SEC_MODE_SET_OPEN(ptr)              do {(ptr)->sm = 1; (ptr)->lv = 1;} while(0)
/** @brief Set sec_mode pointed to by ptr to require encryption, but no MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(ptr)       do {(ptr)->sm = 1; (ptr)->lv = 2;} while(0)
/** @brief Set sec_mode pointed to by ptr to require encryption and MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(ptr)     do {(ptr)->sm = 1; (ptr)->lv = 3;} while(0)
/** @brief Set sec_mode pointed to by ptr to require signing or encryption, no MITM protection needed.*/
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(ptr)    do {(ptr)->sm = 2; (ptr)->lv = 1;} while(0)
/** @brief Set sec_mode pointed to by ptr to require signing or encryption with MITM protection.*/
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(ptr)  do {(ptr)->sm = 2; (ptr)->lv = 2;} while(0)
/** @} */


/**@brief GAP Security Key Length. */
#define BLE_GAP_SEC_KEY_LEN 16

/**@brief Maximum amount of addresses in a whitelist. */
#define BLE_GAP_WHITELIST_ADDR_MAX_COUNT (8)

/**@brief Maximum amount of IRKs in a whitelist.
 * @note  The number of IRKs is limited to 8, even if the hardware supports more.
 */
#define BLE_GAP_WHITELIST_IRK_MAX_COUNT (8)

/** @defgroup GAP_SEC_MODES GAP Security Modes
 * @{ */
#define BLE_GAP_SEC_MODE 0x00 /**< No key (may be used to reject). */

/** @} */


/** @} */

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
 */
typedef struct
{
  uint16_t min_conn_interval;         /**< Minimum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t max_conn_interval;         /**< Maximum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t slave_latency;             /**< Slave Latency in number of connection events, see @ref BLE_GAP_CP_LIMITS.*/
  uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units, see @ref BLE_GAP_CP_LIMITS.*/
} ble_gap_conn_params_t;


/**@brief GAP link requirements.
 *
 * See Bluetooth Core specification, Volume 3 Part C 10.2 for details.
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
  uint8_t                 encr_key_size;      /**< Length of currently active encryption key, 7 to 16 octets.*/
} ble_gap_conn_sec_t;



/**@brief Identity Resolving Key. */
typedef struct
{
  uint8_t irk[BLE_GAP_SEC_KEY_LEN];   /**< Array containing IRK. */
} ble_gap_irk_t;


/**@brief Whitelist structure. */
typedef struct
{
  ble_gap_addr_t   ** pp_addrs;        /**< Pointer to array of device address pointers, pointing to addresses to be used in whitelist. NULL if none are given. */
  uint8_t             addr_count;      /**< Count of device addresses in array, up to @ref BLE_GAP_WHITELIST_ADDR_MAX_COUNT. */
  ble_gap_irk_t    ** pp_irks;         /**< Pointer to array of Identity Resolving Key (IRK) pointers, each pointing to an IRK in the whitelist. NULL if none are given. */
  uint8_t             irk_count;       /**< Count of IRKs in array, up to @ref BLE_GAP_WHITELIST_IRK_MAX_COUNT. */
} ble_gap_whitelist_t;


/**@brief GAP advertising parameters.*/
typedef struct
{
  uint8_t               type;                 /**< See @ref BLE_GAP_ADV_TYPES. */
  ble_gap_addr_t*       p_peer_addr;          /**< For BLE_GAP_CONN_MODE_DIRECTED mode only, known peer address. */
  uint8_t               fp;                   /**< Filter Policy, see @ref BLE_GAP_ADV_FILTER_POLICIES. */
  ble_gap_whitelist_t * p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */
  uint16_t              interval;             /**< Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s), see @ref BLE_GAP_ADV_INTERVALS. This parameter must be set to 0 if type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND. */
  uint16_t              timeout;              /**< Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. See also @ref BLE_GAP_ADV_TIMEOUT_VALUES. This parameter must be set to 0 if type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND. */
} ble_gap_adv_params_t;


/**@brief GAP scanning parameters. */
typedef struct
{
  uint8_t    filter;                    /**< Filter based on discovery mode, see @ref BLE_GAP_DISC_MODES. */
  uint8_t    active    : 1;             /**< If 1, perform active scanning (scan requests). */
  uint8_t    selective : 1;             /**< If 1, ignore unknown devices (non whitelisted). */
  uint16_t   interval;                  /**< Scan interval between 0x0020 and 0x4000 in 0.625ms units (20ms to 10.24s). */
  uint16_t   window;                    /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
  uint16_t   timeout;                   /**< Scan timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. */
} ble_gap_scan_params_t;


/**@brief GAP security parameters. */
typedef struct
{
  uint16_t   timeout;                   /**< Timeout for SMP transactions or Security Request in seconds, see @ref sd_ble_gap_authenticate and @ref sd_ble_gap_sec_params_reply for more information. */
  uint8_t    bond    : 1;               /**< Perform bonding. */
  uint8_t    mitm    : 1;               /**< Man In The Middle protection required. */
  uint8_t    io_caps : 3;               /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */
  uint8_t    oob     : 1;               /**< Out Of Band data available. */
  uint8_t    min_key_size;              /**< Minimum encryption key size in octets between 7 and 16. */
  uint8_t    max_key_size;              /**< Maximum encryption key size in octets between min_key_size and 16. */
} ble_gap_sec_params_t;


/**@brief GAP Encryption Information. */
typedef struct
{
  uint16_t  div;                        /**< Encryption Diversifier. */
  uint8_t   ltk[BLE_GAP_SEC_KEY_LEN];   /**< Long Term Key. */
  uint8_t   auth : 1;                   /**< Authenticated Key. */
  uint8_t   ltk_len : 7;                /**< LTK length in octets. */
} ble_gap_enc_info_t;


/**@brief GAP Master Identification. */
typedef struct
{
  uint16_t  ediv;                       /**< Encrypted Diversifier. */
  uint8_t   rand[8];                    /**< Random Number. */
} ble_gap_master_id_t;


/**@brief GAP Identity Information. */
typedef struct
{
  ble_gap_addr_t  addr;                       /**< Bluetooth address to which this key applies. */
  uint8_t         irk[BLE_GAP_SEC_KEY_LEN];   /**< Identity Resolution Key. */
} ble_gap_id_info_t;


/**@brief GAP Signing Information. */
typedef struct
{
  uint8_t   csrk[BLE_GAP_SEC_KEY_LEN]; /* Connection Signature Resolving Key. */
} ble_gap_sign_info_t;



/**
 * @brief GAP Event IDs.
 * Those IDs uniquely identify an event coming from the stack to the application.
 */
enum BLE_GAP_EVTS
{
  BLE_GAP_EVT_CONNECTED  = BLE_GAP_EVT_BASE,    /**< Connection established. */
  BLE_GAP_EVT_DISCONNECTED,                     /**< Disconnected from peer. */
  BLE_GAP_EVT_CONN_PARAM_UPDATE,                /**< Connection Parameters updated. */
  BLE_GAP_EVT_SEC_PARAMS_REQUEST,               /**< Request to provide security parameters. */
  BLE_GAP_EVT_SEC_INFO_REQUEST,                 /**< Request to provide security information. */
  BLE_GAP_EVT_PASSKEY_DISPLAY,                  /**< Request to display a passkey to the user. */
  BLE_GAP_EVT_AUTH_KEY_REQUEST,                 /**< Request to provide an authentication key. */
  BLE_GAP_EVT_AUTH_STATUS,                      /**< Authentication procedure completed with status. */
  BLE_GAP_EVT_CONN_SEC_UPDATE,                  /**< Connection security updated. */
  BLE_GAP_EVT_TIMEOUT,                          /**< Timeout expired. */
  BLE_GAP_EVT_RSSI_CHANGED,                     /**< Signal strength measurement report. */
};


/** @brief Event data for connected event. */
typedef struct
{
  ble_gap_addr_t        peer_addr;              /**< Bluetooth address of the peer device. */
  uint8_t               irk_match :1;           /**< If 1, peer device's address resolved using an IRK. */
  uint8_t               irk_match_idx  :7;      /**< Index in IRK list where the address was matched. */
  ble_gap_conn_params_t conn_params;            /**< GAP Connection Parameters. */
} ble_gap_evt_connected_t;


/** @brief Event data for disconnected event. */
typedef struct
{
  uint8_t reason;                               /**< HCI error code. */
} ble_gap_evt_disconnected_t;


/** @brief Event data for connection parameter update event. */
typedef struct
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */
} ble_gap_evt_conn_param_update_t;


/** @brief Event data for security parameters request event. */
typedef struct
{
  ble_gap_sec_params_t peer_params;             /**< Initiator Security Parameters. */
} ble_gap_evt_sec_params_request_t;


/** @brief Event data for securito info request event. */
typedef struct
{
  ble_gap_addr_t peer_addr;                     /**< Bluetooth address of the peer device. */
  uint16_t       div;                           /**< Encryption diversifier for LTK lookup. */
  uint8_t        enc_info  : 1;                 /**< If 1, Encryption Information required. */
  uint8_t        id_info   : 1;                 /**< If 1, Identity Information required. */
  uint8_t        sign_info : 1;                 /**< If 1, Signing Information required. */
} ble_gap_evt_sec_info_request_t;


/** @brief Event data for passkey display event. */
typedef struct
{
  uint8_t passkey[6];                           /**< 6-digit passkey in ASCII ('0'-'9' digits only). */
} ble_gap_evt_passkey_display_t;


/** @brief Event data for authentication key request event. */
typedef struct
{
  uint8_t key_type;                             /**< See @ref BLE_GAP_AUTH_KEY_TYPES. */
} ble_gap_evt_auth_key_request_t;


/** @brief Security levels supported.
 *  @note See Bluetooth Specification Version 4.0 Volume 3, Chapter 10.
*/
typedef struct
{
  uint8_t lv1 : 1;                              /**< If 1: Level 1 is supported. */
  uint8_t lv2 : 1;                              /**< If 1: Level 2 is supported. */
  uint8_t lv3 : 1;                              /**< If 1: Level 3 is supported. */
} ble_gap_sec_levels_t;


/** @brief Keys that have been exchanged. */
typedef struct
{
  uint8_t ltk       : 1;                        /**< Long Term Key. */
  uint8_t ediv_rand : 1;                        /**< Encrypted Diversifier and Random value. */
  uint8_t irk       : 1;                        /**< Identity Resolving Key. */
  uint8_t address   : 1;                        /**< Public or static random address. */
  uint8_t csrk      : 1;                        /**< Connection Signature Resolving Key. */
} ble_gap_sec_keys_t;


/** @brief Event data for authentication status event. */
typedef struct
{
  uint8_t               auth_status;            /**< Authentication status, see @ref BLE_GAP_SEC_STATUS. */
  uint8_t               error_src;              /**< On error, source that caused the failure, see @ref BLE_GAP_SEC_STATUS_SOURCES. */
  ble_gap_sec_levels_t  sm1_levels;             /**< Levels supported in Security Mode 1. */
  ble_gap_sec_levels_t  sm2_levels;             /**< Levels supported in Security Mode 2. */
  ble_gap_sec_keys_t    periph_kex;             /**< Bitmap stating which keys were exchanged (distributed) by the peripheral. */
  ble_gap_sec_keys_t    central_kex;            /**< Bitmap stating which keys were exchanged (distributed) by the central. */
  struct periph_keys_t
  {
    ble_gap_enc_info_t    enc_info;             /**< Peripheral's Encryption information. */
  } periph_keys;                                /**< Actual keys distributed from the Peripheral to the Central. */ 
  struct central_keys_t
  {
    ble_gap_irk_t         irk;                  /**< Central's IRK. */
    ble_gap_addr_t        id_info;              /**< Central's Identity Info. */
  } central_keys;                               /**< Actual keys distributed from the Central to the Peripheral. */
} ble_gap_evt_auth_status_t;


/** @brief Event data for connection security update event. */
typedef struct
{
  ble_gap_conn_sec_t conn_sec;                  /**< Connection security level. */
} ble_gap_evt_conn_sec_update_t;


/** @brief Event data for timeout event. */
typedef struct
{
  uint8_t src;                                  /**< Source of timeout event, see @ref BLE_GAP_TIMEOUT_SOURCES. */
} ble_gap_evt_timeout_t;


/** @brief Event data for advertisement report event. */
typedef struct
{
  int8_t  rssi;                               /**< Received Signal Strength Indication in dBm. */
} ble_gap_evt_rssi_changed_t;



/**@brief GAP event callback event structure. */
typedef struct
{
  uint16_t conn_handle;                                     /**< Connection Handle on which event occured. */
  union                                                     /**< union alternative identified by evt_id in enclosing struct. */
  {
    ble_gap_evt_connected_t          connected;             /**< Connected Event Parameters. */
    ble_gap_evt_disconnected_t       disconnected;          /**< Disconnected Event Parameters. */
    ble_gap_evt_conn_param_update_t  conn_param_update;     /**< Connection Parameter Update Parameters. */
    ble_gap_evt_sec_params_request_t sec_params_request;    /**< Security Parameters Request Event Parameters. */
    ble_gap_evt_sec_info_request_t   sec_info_request;      /**< Security Information Request Event Parameters. */
    ble_gap_evt_passkey_display_t    passkey_display;       /**< Passkey Display Event Parameters. */
    ble_gap_evt_auth_key_request_t   auth_key_request;      /**< Authentication Key Request Event Parameters. */
    ble_gap_evt_auth_status_t        auth_status;           /**< Authentication Status Event Parameters. */
    ble_gap_evt_conn_sec_update_t    conn_sec_update;       /**< Connection Security Update Event Parameters. */
    ble_gap_evt_timeout_t            timeout;               /**< Timeout Event Parameters. */
    ble_gap_evt_rssi_changed_t       rssi_changed;          /**< RSSI Event parameters. */
  } params;

} ble_gap_evt_t;


/**@brief Set local Bluetooth address.
 *
 * @param[in] p_addr Pointer to address structure.
 *
 * @return @ref NRF_SUCCESS Address successfully set.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid address.
 * @return @ref NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 */
SVCALL(SD_BLE_GAP_ADDRESS_SET, uint32_t, sd_ble_gap_address_set(ble_gap_addr_t const * const p_addr));


/**@brief Get local Bluetooth address.
 *
 * @param[out] p_addr Pointer to address structure.
 *
 * @return @ref NRF_SUCCESS Address successfully retrieved.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_ADDRESS_GET, uint32_t, sd_ble_gap_address_get(ble_gap_addr_t * const p_addr));


/**@brief Set, clear or update advertisement and scan response data.
 *
 * @note The format of the advertisement data will be checked by this call to ensure interoperability.
 *       Limitations imposed by this API call to the data provided include having a flags data type in the scan response data and
 *       duplicating the local name in the advertisement data and scan response data. 
 *
 * @note: To clear the advertisement data and set it to a 0-length packet, simply provide a valid pointer (p_data/p_sr_data) with its corresponding 
 *        length (dlen/srdlen) set to 0.
 *
 * @note: The call will fail if p_data and p_sr_data are both NULL since this would have no effect.
 *
 * @param[in] p_data    Raw data to be placed in advertisement packet. If NULL, no changes are made to the current advertisement packet data.
 * @param[in] dlen      Data length for p_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_data is NULL, can be 0 if p_data is not NULL.
 * @param[in] p_sr_data Raw data to be placed in scan response packet. If NULL, no changes are made to the current scan response packet data.
 * @param[in] srdlen    Data length for p_sr_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_sr_data is NULL, can be 0 if p_data is not NULL.
 *
 * @return @ref NRF_SUCCESS Advertisement data successfully updated or cleared.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_FLAGS Invalid combination of advertising flags supplied.
 * @return @ref NRF_ERROR_INVALID_DATA Invalid data type(s) supplied, check the advertising data format specification.
 * @return @ref NRF_ERROR_INVALID_LENGTH Invalid data length(s) supplied.
 * @return @ref BLE_ERROR_GAP_UUID_LIST_MISMATCH Invalid UUID list supplied.
 * @return @ref NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 */
SVCALL(SD_BLE_GAP_ADV_DATA_SET, uint32_t, sd_ble_gap_adv_data_set(uint8_t const * const p_data, uint8_t dlen, uint8_t const * const p_sr_data, uint8_t srdlen));


/**@brief Start advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
 *
 * @param[in] p_adv_params Pointer to advertising parameters structure.
 *
 * @return @ref NRF_SUCCESS The BLE stack has started advertising.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check the accepted ranges and limits.
 * @return @ref BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Bluetooth address supplied.
 * @return @ref BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST Discoverable mode and whitelist incompatible.
 */
SVCALL(SD_BLE_GAP_ADV_START, uint32_t, sd_ble_gap_adv_start(ble_gap_adv_params_t const * const p_adv_params));


/**@brief Stop advertising (GAP Discoverable, Connectable modes, Broadcast Procedure).
 *
 * @return @ref NRF_SUCCESS The BLE stack has stopped advertising.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in advertising state).
 */
SVCALL(SD_BLE_GAP_ADV_STOP, uint32_t, sd_ble_gap_adv_stop(void));


/**@brief Update connection parameters.
 *
 * @details In the central role this will initiate a Link Layer connection parameter update procedure,
 *          otherwise in the peripheral role, this will send the corresponding L2CAP request and wait for
 *          the central to perform the procedure. In both cases, and regardless of success or failure, the application
 *          will be informed of the result with a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE event.
 *
 * @note If both a connection supervision timeout and a maximum connection interval are specified, then the following constraint
 *       applies: (conn_sup_timeout * 8) >= (max_conn_interval * (slave_latency + 1))
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_conn_params  Pointer to desired connection parameters. If NULL is provided on a peripheral role,
 *                           the parameters in the PPCP characteristic of the GAP service will be used instead.
 *
 * @return @ref NRF_SUCCESS The Connection Update procedure has been started successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints.
 * @return @ref NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, process pending events and retry.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @return @ref NRF_ERROR_NO_MEM Not enough memory to complete operation.
 */
SVCALL(SD_BLE_GAP_CONN_PARAM_UPDATE, uint32_t, sd_ble_gap_conn_param_update(uint16_t conn_handle, ble_gap_conn_params_t const * const p_conn_params));


/**@brief Disconnect (GAP Link Termination).
 *
 * @details This call initiates the disconnection procedure, and its completion will be communicated to the application
 *          with a BLE_GAP_EVT_DISCONNECTED event.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] hci_status_code HCI status code, see @ref BLE_HCI_STATUS_CODES (accepted values are BTLE_REMOTE_USER_TERMINATED_CONNECTION and BTLE_CONN_INTERVAL_UNACCEPTABLE).
 *
 * @return @ref NRF_SUCCESS The disconnection procedure has been started successfully.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation (disconnection is already in progress or not connected at all).
 */
SVCALL(SD_BLE_GAP_DISCONNECT, uint32_t, sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code));


/**@brief Set the radio's transmit power.
 *
 * @param[in] tx_power Radio transmit power in dBm (accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm).
 *
 * @note -40 dBm will not actually give -40 dBm, but will instead be remapped to -30 dBm.
 *
 * @return @ref NRF_SUCCESS Successfully changed the transmit power.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 */
SVCALL(SD_BLE_GAP_TX_POWER_SET, uint32_t, sd_ble_gap_tx_power_set(int8_t tx_power));


/**@brief Set GAP Appearance value.
 *
 * @param[in] appearance Appearance (16-bit), see @ref BLE_APPEARANCES.
 *
 * @return @ref NRF_SUCCESS  Appearance value set successfully.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 */
SVCALL(SD_BLE_GAP_APPEARANCE_SET, uint32_t, sd_ble_gap_appearance_set(uint16_t appearance));


/**@brief Get GAP Appearance value.
 *
 * @param[out] p_appearance Appearance (16-bit), see @ref BLE_APPEARANCES.
 *
 * @return @ref NRF_SUCCESS Appearance value retrieved successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_APPEARANCE_GET, uint32_t, sd_ble_gap_appearance_get(uint16_t * const p_appearance));


/**@brief Set GAP Peripheral Preferred Connection Parameters.
 *
 * @param[in] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure with the desired parameters.
 *
 * @return @ref NRF_SUCCESS Peripheral Preferred Connection Parameters set successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 */
SVCALL(SD_BLE_GAP_PPCP_SET, uint32_t, sd_ble_gap_ppcp_set(ble_gap_conn_params_t const * const p_conn_params));


/**@brief Get GAP Peripheral Preferred Connection Parameters.
 *
 * @param[out] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure where the parameters will be stored.
 *
 * @return @ref NRF_SUCCESS Peripheral Preferred Connection Parameters retrieved successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_GAP_PPCP_GET, uint32_t, sd_ble_gap_ppcp_get(ble_gap_conn_params_t * const p_conn_params));


/**@brief Set GAP device name.
 *
 * @param[in] p_write_perm Write permissions for the Device Name characteristic see @ref ble_gap_conn_sec_mode_t.
 * @param[in] p_dev_name Pointer to a UTF-8 encoded, <b>non NULL-terminated</b> string.
 * @param[in] len   Length of the UTF-8, <b>non NULL-terminated</b> string pointed to by p_dev_name in octets (must be smaller or equal than @ref BLE_GAP_DEVNAME_MAX_LEN).
 *
 * @return @ref NRF_SUCCESS GAP device name and permissions set successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_SET, uint32_t, sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const * const p_write_perm, uint8_t const * const p_dev_name, uint16_t len));


/**@brief Get GAP device name.
 *
 * @param[in]     p_dev_name Pointer to an empty buffer where the UTF-8 <b>non NULL-terminated</b> string will be placed. Set to NULL to obtain the complete device name length.
 * @param[in,out] p_len      Length of the buffer pointed by p_dev_name, complete device name length on output.
 *
 * @note          If the device name is longer than the size of the supplied buffer,
 *                p_len will return the complete device name length,
 *                and not the number of bytes actually returned in p_dev_name.
 *                The application may use this information to allocate a suitable buffer size.
 *
 * @return @ref NRF_SUCCESS GAP device name retrieved successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_GET, uint32_t, sd_ble_gap_device_name_get(uint8_t * const p_dev_name, uint16_t * const p_len));


/**@brief Initiate GAP Authentication procedure.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_sec_params Pointer to the @ref ble_gap_sec_params_t structure with the security parameters to be used during the pairing procedure.
 *
 * @details In the central role, this function will send an SMP Pairing Request, otherwise in the peripheral role, an SMP Security Request will be sent.
 *          In the peripheral role, only the timeout, bond and mitm fields of @ref ble_gap_sec_params_t are used.
 *
 * @note    The GAP Authentication procedure may be triggered by the central without calling this function when accessing a secure service.
 * @note    Calling this function may result in the following events depending on the outcome and parameters: @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST,
 *          @ref BLE_GAP_EVT_SEC_INFO_REQUEST, @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, @ref BLE_GAP_EVT_AUTH_STATUS.
 * @note    The timeout parameter in @ref ble_gap_sec_params_t is interpreted here as the Security Request timeout
 *
 *
 * @return @ref NRF_SUCCESS Successfully initiated authentication procedure.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_AUTHENTICATE, uint32_t, sd_ble_gap_authenticate(uint16_t conn_handle, ble_gap_sec_params_t const * const p_sec_params));


/**@brief Reply with GAP security parameters.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] sec_status Security status, see @ref BLE_GAP_SEC_STATUS.
 * @param[in] p_sec_params Pointer to a @ref ble_gap_sec_params_t security parameters structure.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST, calling it at other times will result in an NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 * @note    The timeout parameter in @ref ble_gap_sec_params_t is interpreted here as the SMP procedure timeout, and must be 30 seconds. The function will fail
 *          if the application supplies a different value.
 *
 * @return @ref NRF_SUCCESS Successfully accepted security parameter from the application.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_SEC_PARAMS_REPLY, uint32_t, sd_ble_gap_sec_params_reply(uint16_t conn_handle, uint8_t sec_status, ble_gap_sec_params_t const * const p_sec_params));


/**@brief Reply with an authentication key.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] key_type See @ref BLE_GAP_AUTH_KEY_TYPES.
 * @param[in] key If key type is BLE_GAP_AUTH_KEY_TYPE_NONE, then NULL.
 *                If key type is BLE_GAP_AUTH_KEY_TYPE_PASSKEY, then a 6-byte ASCII string (digit 0..9 only, no NULL termination).
 *                If key type is BLE_GAP_AUTH_KEY_TYPE_OOB, then a 16-byte OOB key value in Little Endian format.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, calling it at other times will result in an NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 *
 * @return @ref NRF_SUCCESS Authentication key successfully set.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_AUTH_KEY_REPLY, uint32_t, sd_ble_gap_auth_key_reply(uint16_t conn_handle, uint8_t key_type, uint8_t const * const key));


/**@brief Reply with GAP security information.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_enc_info Pointer to a @ref ble_gap_enc_info_t encryption information structure. May be NULL to signal none is available.
 * @param[in] p_sign_info Pointer to a @ref ble_gap_sign_info_t signing information structure. May be NULL to signal none is available.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_INFO_REQUEST, calling it at other times will result in NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 * @note    Data signing is not implemented yet. p_sign_info must therefore be NULL.
 *
 * @return @ref NRF_SUCCESS Successfully accepted security information.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @return @ref NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 */
SVCALL(SD_BLE_GAP_SEC_INFO_REPLY, uint32_t, sd_ble_gap_sec_info_reply(uint16_t conn_handle, ble_gap_enc_info_t const * const p_enc_info, ble_gap_sign_info_t const * const p_sign_info));


/**@brief Get the current connection security.
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_conn_sec  Pointer to a @ref ble_gap_conn_sec_t structure to be filled in.
 *
 * @return @ref NRF_SUCCESS Current connection security successfully retrieved.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_CONN_SEC_GET, uint32_t, sd_ble_gap_conn_sec_get(uint16_t conn_handle, ble_gap_conn_sec_t * const p_conn_sec));


/**@brief Start reporting the received signal strength to the application. 
 *
 * A new event is reported whenever the RSSI value changes, until @ref sd_ble_gap_rssi_stop is called.
 *
 * @param[in] conn_handle Connection handle.
 *
 * @return @ref NRF_SUCCESS Successfully activated RSSI reporting.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_START, uint32_t, sd_ble_gap_rssi_start(uint16_t conn_handle));


/**@brief Stop reporting the received singnal strength. 
 *
 * An RSSI change detected before the call but not yet received by the application 
 * may be reported after @ref sd_ble_gap_rssi_stop has been called.
 *
 * @param[in] conn_handle Connection handle.
 *
 * @return @ref NRF_SUCCESS Successfully deactivated RSSI reporting.
 * @return @ref NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_STOP, uint32_t, sd_ble_gap_rssi_stop(uint16_t conn_handle));

#endif // BLE_GAP_H__

/**
  @}
*/
