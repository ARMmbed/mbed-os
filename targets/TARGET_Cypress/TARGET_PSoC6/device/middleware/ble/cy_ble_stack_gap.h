/*******************************************************************************
* \file cy_ble_stack_gap.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs of the Generic Access Profile.
*  It also specifies the defines, constants, and data structures required for the APIs.
*
* 
* Related Document:
*  BLE Standard Spec - CoreV4.2, CSS, CSAs, ESR05, ESR06
* 
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifndef CY_BLE_STACK_GAP_H_
#define CY_BLE_STACK_GAP_H_

/***************************************
* Common stack includes
***************************************/
#include "BLE_config.h"
#include "cy_ble_stack_host_main.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** GAP Constants
***************************************/
/** Bluetooth Device Address size  */
#define CY_BLE_GAP_BD_ADDR_SIZE                              0x06u

/** BD Address type flag*/

/** Public Device Address */
#define CY_BLE_GAP_ADDR_TYPE_PUBLIC                          0x00u
/** Random Device Address */
#define CY_BLE_GAP_ADDR_TYPE_RANDOM                          0x01u
/** Controller generates the Resolvable Private Address based on the local
  * IRK from the resolving list. If the resolving list contains no matching entry,
  * uses the public address. */
#define CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA                      0x02u
/** Controller generates the Resolvable Private Address based on the local
  * IRK from the resolving list. If the resolving list contains no matching entry,
  * uses random address. */
#define CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA                      0x03u

/** Max data length size  */
#define CY_BLE_GAP_MAX_ADV_DATA_LEN              0x1Fu

/** Max data length size  */
#define CY_BLE_GAP_MAX_SCAN_RSP_DATA_LEN         0x1Fu

/** Security modes  */
#define CY_BLE_GAP_SEC_MODE_1                    0x10u /**< security mode 1 */
#define CY_BLE_GAP_SEC_MODE_2                    0x20u /**< security mode 2 */
#define CY_BLE_GAP_SEC_MODE_MASK                 0xF0u /**< security mode bit mask */


/** Pairing properties MASK for cy_stc_ble_gap_auth_info_t */

/**
 * MASK to set MITM in pairing properties for Secure connections.
 */
#define CY_BLE_GAP_SMP_SC_PAIR_PROP_MITM_MASK    0x01u

/**
 * MASK to set keypress in pairing properties for Secure connections.
 */
#define CY_BLE_GAP_SMP_SC_PAIR_PROP_KP_MASK      0x02u

/** Maximum number of Bonded Devices */
#define CY_BLE_GAP_MAX_BONDED_DEVICE             0x10u

/** Out Of Band (OOB) flag*/
#define CY_BLE_GAP_OOB_ENABLE                    0x01u /**< oob enable */
#define CY_BLE_GAP_OOB_DISABLE                   0x00u /**< oob disable */

/** SMP Key size */
#define CY_BLE_GAP_SMP_LTK_SIZE                  0x10u /**< LTK size */
#define CY_BLE_GAP_SMP_IRK_SIZE                  0x10u /**< IRK size */
#define CY_BLE_GAP_SMP_CSRK_SIZE                 0x10u /**< CSRK size */
#define CY_BLE_GAP_SMP_IDADDR_DATA_SIZE          0x07u /**< ID Address data size */
#define CY_BLE_GAP_SMP_MID_INFO_SIZE             0x0Au /**< Mid info size */

/** Key Distribution Flags */
#define CY_BLE_GAP_SMP_INIT_ENC_KEY_DIST         0x01u /**< Initiator enc key */
#define CY_BLE_GAP_SMP_INIT_IRK_KEY_DIST         0x02u /**< Initiator IRK */
#define CY_BLE_GAP_SMP_INIT_CSRK_KEY_DIST        0x04u /**< Initiator CSRK */
#define CY_BLE_GAP_SMP_RESP_ENC_KEY_DIST         0x10u /**< Peer enc key */
#define CY_BLE_GAP_SMP_RESP_IRK_KEY_DIST         0x20u /**< Peer IRK */
#define CY_BLE_GAP_SMP_RESP_CSRK_KEY_DIST        0x40u /**< Peer CSRK */

/** SMP P256 Public-Private Key Size */
#define CY_BLE_GAP_SMP_P256_PUBLIC_KEY_SIZE      0x40u /**< P256 public key size */
#define CY_BLE_GAP_SMP_P256_PRIVATE_KEY_SIZE     0x20u /**< P256 private key size */

/** Passkey Response */
#define CY_BLE_GAP_REJECT_PASSKEY_REQ            0x00u /**< Reject passkey request */
#define CY_BLE_GAP_ACCEPT_PASSKEY_REQ            0x01u /**< Accept passkey request */

/** Fixed Passkey  */
#define CY_BLE_GAP_PASSKEY_FIXED                 0x01u /**< Fixed passkey */
#define CY_BLE_GAP_PASSKEY_NOT_FIXED             0x00u /**< Non-fixed passkey */


/***************************************
** Bonding definitions
***************************************/
/** No Bonding support */
#define CY_BLE_GAP_BONDING_NONE                          0x00u
/** Bonding support */
#define CY_BLE_GAP_BONDING                               0x01u

/** Encryption key size   */
#define CY_BLE_GAP_ENCRYP_KEY_MIN                        0x07u /**< Min Enc key size */
#define CY_BLE_GAP_ENCRYP_KEY_MAX                        0x10u /**< Max Enc key size */

/** User Passkey size */
#define CY_BLE_GAP_USER_PASSKEY_SIZE                     0x06u /**< user passkey size */

/** Address Masks */
/** random private resolvable addr mask */
#define CY_BLE_GAP_RANDOM_PRIV_RESOLVABLE_ADDR_MASK      0x40u
/** random private non-resolvable addr mask */
#define CY_BLE_GAP_RANDOM_PRIV_NON_RESOLVABLE_ADDR_MASK  0x00u
/** public addr mask */
#define CY_BLE_GAP_PUBLIC_ADDR_MASK                      0x80u
/** random static addr mask */
#define CY_BLE_GAP_RANDOM_STATIC_ADDR_MASK               0xC0u

/** Address Masks */
#define CY_BLE_GAP_RANDOM_NUMBER_SIZE                    0x10u 

/** Device Connected as Role. */
#define CY_BLE_GAP_LL_ROLE_SLAVE                         0x01u /**< Slave role */
#define CY_BLE_GAP_LL_ROLE_MASTER                        0x00u /**< Master role */

/** Link Level Encryption status */
#define CY_BLE_GAP_ENCRYPT_ON                            0x01u /**< Encryption on */
#define CY_BLE_GAP_ENCRYPT_OFF                           0x00u /**< Encryption off */

/** Security Requirements for strict pairing */
/** No security requirement */
#define CY_BLE_GAP_NO_SECURITY_REQUIREMENTS              (0x00u)
/** Unauthenticated legacy pairing */
#define CY_BLE_GAP_SEC_UNAUTH_PAIRING                    (0x01u)
/** Authenticated legacy pairing */
#define CY_BLE_GAP_SEC_AUTH_PAIRING                      (0x02u)
/** Unauthenticated SC pairing */
#define CY_BLE_GAP_SEC_SC_PAIRING_WITH_NO_MITM           (0x04u)
/** Authenticated SC pairing */
#define CY_BLE_GAP_SEC_SC_PAIRING_WITH_MITM              (0x08u)
/** Legacy with OOB pairing */
#define CY_BLE_GAP_SEC_OOB_IN_LEGACY_PAIRING             (0x10u)
/** SC with oob pairing */
#define CY_BLE_GAP_SEC_OOB_IN_SC_PAIRING                 (0x20u)
/** Security requirement bit mask */
#define CY_BLE_GAP_SEC_REQ_BIT_MASK                      (0x3Fu)


/***************************************
** Enumerated Types
***************************************/
/**
 \addtogroup group_ble_common_api_gap_definitions
 @{
*/

/** Security Levels  */
typedef enum
{
    /** Level 1
        * Mode 1 - No security (No authentication and no encryption)
     */
    CY_BLE_GAP_SEC_LEVEL_1 = 0x00u,

    /** Level 2
        * Mode 1 - Unauthenticated pairing with encryption (No MITM)
        * Mode 2 - Unauthenticated pairing with data signing (No MITM)
     */
    CY_BLE_GAP_SEC_LEVEL_2,
    
    /** Level 3
        * Mode 1 - Authenticated pairing with encryption (With MITM)
        * Mode 2 - Authenticated pairing with data signing (With MITM)
     */
    CY_BLE_GAP_SEC_LEVEL_3,
    
    /** Level 4
        * Secured Connection
     */
    CY_BLE_GAP_SEC_LEVEL_4,

    /** LE Security Level Mask */
    CY_BLE_GAP_SEC_LEVEL_MASK = 0x0Fu

}cy_en_ble_gap_sec_level_t;

/** IO capability  */
typedef enum
{
    /** Platform supports only a mechanism to display or convey only a 6-digit number to the user.*/
    CY_BLE_GAP_IOCAP_DISPLAY_ONLY = 0x00u, 
    
    /** The device has a mechanism whereby the user can indicate 'yes' or 'no'.*/    
    CY_BLE_GAP_IOCAP_DISPLAY_YESNO, 
    
    /** Platform supports a numeric keyboard that can input the numbers '0' through '9' 
        and a confirmation key(s) for  'yes' and 'no'. */
    CY_BLE_GAP_IOCAP_KEYBOARD_ONLY,

    /** Platform does not have the ability to display or communicate a 6 digit decimal number.*/
    CY_BLE_GAP_IOCAP_NOINPUT_NOOUTPUT,

    /** Platform supports a mechanism through which 6 digit numeric value can be displayed 
        and numeric keyboard that can input the numbers '0' through '9'. */
    CY_BLE_GAP_IOCAP_KEYBOARD_DISPLAY
    
} cy_en_ble_gap_iocap_t;

/** Authentication Failed Error Codes */
typedef enum
{
    /** No Error */
    CY_BLE_GAP_AUTH_ERROR_NONE  = 0x00u,

    /** User input of passkey failed. For example, the user cancelled the operation. */
    CY_BLE_GAP_AUTH_ERROR_PASSKEY_ENTRY_FAILED,

    /** Out Of Band data is not available. Applicable if NFC is supported. */
    CY_BLE_GAP_AUTH_ERROR_OOB_DATA_NOT_AVAILABLE,

    /** Pairing procedure cannot be performed as authentication
       requirements cannot be met due to IO capabilities of one or both devices. */
    CY_BLE_GAP_AUTH_ERROR_AUTHENTICATION_REQ_NOT_MET,

    /** Confirm value does not match the calculated compare value.  */
    CY_BLE_GAP_AUTH_ERROR_CONFIRM_VALUE_NOT_MATCH,

    /** Pairing is not supported by the device. */
    CY_BLE_GAP_AUTH_ERROR_PAIRING_NOT_SUPPORTED,

    /** Insufficient key size for the security requirements of this device, 
        or if LTK is lost. */
    CY_BLE_GAP_AUTH_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE,

    /** Command received is not supported. */
    CY_BLE_GAP_AUTH_ERROR_COMMAND_NOT_SUPPORTED,

    /** Pairing failed due to an unspecified reason. */
    CY_BLE_GAP_AUTH_ERROR_UNSPECIFIED_REASON,

    /** Pairing or authentication procedure is disallowed because too little time
        has elapsed since the last pairing request or security request. */
    CY_BLE_GAP_AUTH_ERROR_REPEATED_ATTEMPTS,

    /** Invalid Parameters in Request - Invalid Command length and Parameter value outside range. */
    CY_BLE_GAP_AUTH_ERROR_INVALID_PARAMETERS ,

    /** Indicates to the remote device that the DHKey Check value received doesn't
       match the one calculated by the local device. */
    CY_BLE_GAP_AUTH_ERROR_DHKEY_CHECK_FAILED,
    
    /** Indicates that the confirm values in the numeric comparison protocol
       do not match. */
    CY_BLE_GAP_AUTH_ERROR_NUMERIC_COMPARISON_FAILED,
    
    /** Indicates that the pairing over the LE transport failed due to a Pairing
       Request sent over the BR/EDR transport is in process. */
    CY_BLE_GAP_AUTH_ERROR_BR_EDR_PAIRING_IN_PROGRESS,
    
    /** Indicates that the BR/EDR Link Key generated on the BR/EDR transport cannot
       be used to derive and distribute keys for LE transport. */
    CY_BLE_GAP_AUTH_ERROR_CROSS_TRANSPORT_KEY_GEN_DER_NOT_ALLOWED,

    /** Authentication process timeout - if pairing timeout happens for the first time, 
        the application can choose to re-initiate the pairing procedure. If timeout occurs again, 
        the application may choose to disconnect the peer device. */
    CY_BLE_GAP_AUTH_ERROR_AUTHENTICATION_TIMEOUT = 0x15u,

    /** Link disconnected. */
    CY_BLE_GAP_AUTH_ERROR_LINK_DISCONNECTED = 0x18u

}cy_en_ble_gap_auth_failed_reason_t;

/** GAP address type */
typedef enum
{
    /** Random private non-resolvable address */
    CY_BLE_GAP_RANDOM_PRIV_NON_RESOLVABLE_ADDR = 0x00u,

    /** Random private resolvable address */
    CY_BLE_GAP_RANDOM_PRIV_RESOLVABLE_ADDR = 0x01u,

    /** Public address */
    CY_BLE_GAP_PUBLIC_ADDR = 0x02u,

    /** Random static address */
    CY_BLE_GAP_RANDOM_STATIC_ADDR = 0x03u, 

} cy_en_ble_gap_addr_type_t;

/** Passkey entry notification types.
   These are used for the Cy_BLE_GAP_SecureConnectionKeyNotify API
   as well as with the CY_BLE_EVT_GAP_KEYPRESS_NOTIFICATION event parameter. */
typedef enum
{
    /** Passkey entry started */
    CY_BLE_GAP_PASSKEY_ENTRY_STARTED    = 0x00u,

    /** One digit entered */
    CY_BLE_GAP_PASSKEY_DIGIT_ENTERED = 0x01u,
    
    /** One digit erased */    
    CY_BLE_GAP_PASSKEY_DIGIT_ERASED = 0x02u,
    
    /** All digits cleared */
    CY_BLE_GAP_PASSKEY_CLEARED = 0x03u,

    /** Passkey entry completed */
    CY_BLE_GAP_PASSKEY_ENTRY_COMPLETED  = 0x04u

} cy_en_ble_gap_keypress_notify_type_t;

/** GAP Direct advertiser address type */
typedef enum
{
    /** Public device address type */
    CY_BLE_GAP_PUBLIC_ADDR_TYPE,

    /** Random private resolvable address type*/
    CY_BLE_GAP_RANDOM_RESOLVABLE_ADDR_TYPE,
    
    /** Public Identity address type*/
    CY_BLE_GAP_PUBLIC_IDENTITY_ADDR_TYPE,
    
    /** Random static Identity Address */
    CY_BLE_GAP_RANDOM_IDENTITY_ADDR_TYPE
    
} cy_en_ble_gap_adv_addr_type_t;

/***************************************
** Exported structures and unions
***************************************/

/** BD Address of device */
typedef cy_stc_ble_bd_addr_t     cy_stc_ble_gap_bd_addr_t;

/** Out of Band Parameters Information  */
typedef struct 
{
    /** bd handle of the remote device */
    uint8_t        bdHandle; 

    /** OOB data presence flag. Allowed value are:
         * CY_BLE_GAP_OOB_DISABLE
         * CY_BLE_GAP_OOB_ENABLE
     */
    uint8_t       oobFlag;  
    
    /** 16 Octet Temporary Key to be used for OOB authentication */
    uint8_t        * key;  

    /** Pointer to OOB data */    
    uint8_t        * oobData;

   /** Length of OOB data */
    uint8_t        oobDataLen;

} cy_stc_ble_gap_oob_info_t;

/** Security requirement of local device used in strict pairing */
typedef struct
{
    /** Security requirement */
    uint8_t       secReq;
    
    /** Encryption key size */
    uint8_t       encKeySize;
} cy_stc_ble_gap_sec_req_t;

/** Disconnect command information */
typedef struct
{
    /** Reason for disconnection */
    uint8_t       reason;

    /** bd handle of the remote device */
    uint8_t       bdHandle;

}cy_stc_ble_gap_disconnect_info_t;

/** Peer Bluetooth Device Address  information */
typedef struct
{
    /** Bluetooth device address */
    cy_stc_ble_gap_bd_addr_t     bdAddr;

    /** bd handle of the remote device */
    uint8_t                   bdHandle;

}cy_stc_ble_gap_peer_addr_info_t;

/** Security keys information */
typedef struct
{
    /** Long Term Key */
    uint8_t       ltkInfo[CY_BLE_GAP_SMP_LTK_SIZE];

    /** Encrypted Diversifier and Random Number */
    uint8_t       midInfo[CY_BLE_GAP_SMP_MID_INFO_SIZE];

    /** Identity Resolving Key */
    uint8_t       irkInfo[CY_BLE_GAP_SMP_IRK_SIZE];

    /**
     * Public device/Static Random address type
     * idAddrInfo[0] - Address Type
     * idAddrInfo[1] to idAddrInfo[6] - Address
     */
    uint8_t       idAddrInfo[CY_BLE_GAP_SMP_IDADDR_DATA_SIZE];

    /** Connection Signature Resolving Key */
    uint8_t       csrkInfo[CY_BLE_GAP_SMP_CSRK_SIZE];

    /** bd handle of the remote device */
    uint8_t       bdHandle; 

} cy_stc_ble_gap_sec_key_param_t;

/** Security information */
typedef struct
{
    /** Security keys information parameter */
    cy_stc_ble_gap_sec_key_param_t   SecKeyParam;

    /**This parameter indicates which keys get exchanged with a peer device.
        *   The following is the bit field mapping for the keys.
        *   Bit 0: Local Encryption information
        *   Bit 1: Local Identity information
        *   Bit 2: Local Signature Key
        *   Bit 3: Reserved
        *   Bit 4: Remote Encryption information
        *   Bit 5: Remote Identity information
        *   Bit 6: Remote Signature Key
        *   Bit 7: Reserved
        */
    uint8_t                       exchangeKeysFlag;

    /**This parameter indicates which keys are to be set/generated in the BLE Stack.
        *  The following is the bit field mapping for the keys.
        *  Bit 0: Local Encryption information
        *  Bit 1: Local Identity information
        *  Bit 2: Local Signature Key
        *  Bits 3-7: Reserved
        */
    uint8_t                       localKeysFlag;

} cy_stc_ble_gap_sec_key_info_t;

/** Bluetooth Device Address information */
typedef struct
{
    /** Identity Resolving Key */
    uint8_t                   irkInfo[CY_BLE_GAP_SMP_IRK_SIZE];

    /** Memory for gapBdAddr. Completion event will
     *  use this memory as output parameter.
     */
    cy_stc_ble_gap_bd_addr_t     gapBdAddr;

    /** Device address type */
    cy_en_ble_gap_addr_type_t   addrType;

}cy_stc_ble_gap_bd_addr_info_t;


/** Authentication Parameters Information  */
typedef struct 
{
    /** Security Mode setting will be value of enum cy_en_ble_gap_sec_level_t as follows:
        * (CY_BLE_GAP_SEC_MODE_1 | CY_BLE_GAP_SEC_LEVEL_2)
        * (CY_BLE_GAP_SEC_MODE_1 | CY_BLE_GAP_SEC_LEVEL_3)
        * (CY_BLE_GAP_SEC_MODE_1 | CY_BLE_GAP_SEC_LEVEL_4)
        * (CY_BLE_GAP_SEC_MODE_2 | CY_BLE_GAP_SEC_LEVEL_2)
        * (CY_BLE_GAP_SEC_MODE_2 | CY_BLE_GAP_SEC_LEVEL_3)
     */
    uint8_t                               security;

    /** Bonding type setting:
         * CY_BLE_GAP_BONDING_NONE
         * CY_BLE_GAP_BONDING
     */
    uint8_t                               bonding;

    /** Encryption Key Size (octets)
         * Minimum = 7 
         * maximum = 16
         For a slave initiated security request, this parameter must be ignored.
     */
    uint8_t                               ekeySize;

    /** Parameter to say whether authentication is accepted or rejected with a reason.
       accepted = CY_BLE_GAP_AUTH_ERROR_NONE or error code cy_en_ble_gap_auth_failed_reason_t. */
    cy_en_ble_gap_auth_failed_reason_t      authErr;

   /**
    * Bit 0: MITM (Applicable only if Secure connections)
    *        Use CY_BLE_GAP_SMP_SC_PAIR_PROP_MITM_MASK
    * Bit 1: Key press (sets Key press bit in authentication requirements flags of
    *        pairing request/response. Applicable only for secure connections)
    *        Use CY_BLE_GAP_SMP_SC_PAIR_PROP_KP_MASK
    * Bit [2-7]: RFU
    */
    uint8_t                               pairingProperties;

    /** Peer bdHandle */
    uint8_t                               bdHandle;

} cy_stc_ble_gap_auth_info_t;


/** Authentication Passkey Information  */
typedef struct 
{
    /** 6-digit decimal number (authentication passkey) */
    uint32_t                      passkey;

    /** Accept or reject passkey entry request. Allowed values are,
           * CY_BLE_GAP_REJECT_PASSKEY_REQ
           * CY_BLE_GAP_ACCEPT_PASSKEY_REQ
     */
    uint8_t                       accept;

    /** bdHandle */
    uint8_t                       bdHandle;

}cy_stc_ble_gap_auth_pk_info_t;


/** Fixed Passkey Parameters Information  */
typedef struct 
{
    /** 6-digit decimal number (<=999999). This is used only if isFixed is
            set to true, else ignored */
    uint32_t                      fixedPassKey;

    /** Peer bdHandle */
    uint8_t                       bdHandle;

    /** Passkey is fixed application,
           * CY_BLE_GAP_PASSKEY_FIXED: 'fixedPassKey' will be used by the BLE Stack that is provided by the application.
           * CY_BLE_GAP_PASSKEY_NOT_FIXED: 'fixedPassKey' will be ignored and the BLE Stack will generate as per
                security procedure.
     */
    uint8_t                       isFixed;

}cy_stc_ble_gap_auth_fix_pk_info_t;


/** GAP Connection Update parameters  */
typedef struct
{
    /** Minimum value for the connection event interval. This shall be less than
       or equal to conn_Interval_Max. Minimum connection interval will be
         connIntvMin * 1.25 ms
        * Time Range: 7.5 ms to 4 sec
     */
    uint16_t      connIntvMin;

    /** Maximum value for the connection event interval. This shall be greater
       than or equal to conn_Interval_Min. Maximum connection interval will be
         connIntvMax * 1.25 ms
     * Time Range: 7.5 ms to 4 sec
     */
    uint16_t      connIntvMax;

    /** Slave latency for the connection in number of connection events.
        * Range: 0x0000 to 0x01F3
     */
    uint16_t      connLatency;

    /** Supervision timeout for the LE Link. Supervision timeout will be
       supervisionTO * 10 ms
        * Time Range: 100 msec to 32 secs
     */
    uint16_t      supervisionTO;

    /** Peer bdHandle */
    uint8_t       bdHandle;

    /** connection length */
    uint16_t      ceLength;

}cy_stc_ble_gap_conn_update_param_info_t;


/** Secure connection only mode parameters  */
typedef struct 
{
    /** state: 0 - Disable (Device not in secure connections only mode) \n
               1 - Enable (Device is in secure connections only mode) */
    uint8_t       state;

}cy_stc_ble_gap_sc_mode_info_t;

/** Secure connection key press notification parameters  */
typedef struct 
{
    /** Peer bdHandle */
    uint8_t                           bdHandle;

    /** notification type to be sent to remote identified by bdHandle */
    cy_en_ble_gap_keypress_notify_type_t  notificationType;

}cy_stc_ble_gap_sc_kp_notif_info_t;

/** Secure connection key press notification parameters  */
typedef struct 
{
    /** 16 Bytes Random number to be used for generating OOB data */
    uint8_t       *rand;

    /** Peer bdHandle */
    uint8_t       bdHandle;
}cy_stc_ble_gap_sc_oob_info_t;


/** Bluetooth Bonded Device Address list */
typedef struct
{
    /** Pointer to list of Bluetooth device addresses and corresponding bdHandle of bonded devices */
    cy_stc_ble_gap_peer_addr_info_t      * bdHandleAddrList;

    /** Number of bonded devices */
    uint8_t                           noOfDevices;

}cy_stc_ble_gap_bonded_device_list_info_t;

/** SMP P-256 public-private key pair */
typedef struct
{
    /** P-256 public key */
    uint8_t publicKey[CY_BLE_GAP_SMP_P256_PUBLIC_KEY_SIZE];
    
    /** P-256 private key */
    uint8_t privateKey[CY_BLE_GAP_SMP_P256_PRIVATE_KEY_SIZE];
    
    /** If it is set to 1, the public key is validated. If it is set to 0, 
    public key is not validated. This parameter is not applicable for
    CY_BLE_EVT_GAP_GEN_SET_LOCAL_P256_KEYS_COMPLETE event. */
    uint8_t isValidateKeys;
    
} cy_stc_ble_gap_smp_local_p256_keys_t;

/** GAP Connection Update parameters  */
typedef struct
{

    /** connection length */
    uint16_t      ceLength;

    /** Peer bdHandle */
    uint8_t       bdHandle;

}cy_stc_ble_gap_ce_length_param_info_t;

/** GAP Connection Update parameters  */
typedef struct
{
    /** Priority of the connection compared to other connection */
    /** 0x00 Highest and 0xFF lowest and default for all the connections */
    uint8_t       connPriority;

    /** Peer bdHandle */
    uint8_t       bdHandle;

}cy_stc_ble_gap_set_conn_priority_param_t;

/* --------------------------Structure corresponding to events-------------------- */

/** Current Connection Parameters used by controller */
typedef struct
{
    /** Status corresponding to this event will be the HCI error code 
    as defined in BLE spec 4.1 */
    uint8_t       status;

    /** Connection interval used on this connection. 
        * Range: 0x0006 to 0x0C80
        * Time Range: 7.5 ms to 4 sec 
     */
    uint16_t      connIntv;

    /** Slave latency for the connection in a number of connection events.
        * Range: 0x0000 to 0x01F3 
     */
    uint16_t      connLatency;

    /** Supervision timeout for the LE Link. Supervision timeout will be  
       supervisionTO * 10 ms
       * Time Range: 100 msec to 32 secs 
     */
    uint16_t      supervisionTO;

    /** bd handle of the remote device */
    uint8_t       bdHandle; 

}cy_stc_ble_gap_conn_param_updated_in_controller_t;

/** Gap Connected Parameters */
typedef struct
{
    /** status corresponding to this event will be HCI error code 
        as defined in BLE spec 4.1 */
    uint8_t       status;

    /** bd handle of the remote device */
    uint8_t       bdHandle; 

    /** Connected as - master = CY_BLE_GAP_LL_ROLE_MASTER,
        Slave = CY_BLE_GAP_LL_ROLE_SLAVE */
    uint8_t       role; 

    /** Address type of the Bluetooth device address
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC (Public device address)
        - CY_BLE_GAP_ADDR_TYPE_RANDOM (Random device address) */
    uint8_t       peerAddrType; 

    /** Peer Bluetooth device address of size CY_BLE_BD_ADDR_SIZE*/
    uint8_t       * peerAddr;

    /** Connection interval used on this connection. 
        * Range: 0x0006 to 0x0C80
        * Time Range: 7.5 ms to 4 sec 
     */
    uint16_t      connIntv;

    /** Slave latency for the connection in number of connection events.
        * Range: 0x0000 to 0x01F3
     */
    uint16_t      connLatency;

    /** Supervision timeout for the LE Link. Supervision timeout will be  
            supervisionTO * 10 ms
            Time Range: 100 msec to 32 secs 
     */
    uint16_t      supervisionTO;

    /** Master Clock Accuracy parameter is only valid for a slave. On a master,
        this parameter shall be set to 0x00.*/
    uint8_t       masterClockAccuracy; 

}cy_stc_ble_gap_connected_param_t;

/** OOB data */
typedef struct
{
    /** Peer bdHandle */
    uint8_t   bdHandle;

    /** Status corresponding to this event will be HCI error code 
            as defined in BLE spec 4.2 */
    uint8_t   status;

    /** Rand for OOB. This is also stored in stack */
    uint8_t   * key; 

    /** OOB Data using 'key' and local Public Key */
    uint8_t   * oobData; 

    /** Length of OOB data which is 16 Bytes for Secure connections */
    uint8_t   oobDataLen; 
    
}cy_stc_ble_gap_oob_data_param_t;

/** Current Connection Parameters used by controller */
typedef struct
{
    /** Connection interval used on this connection.
     * Range: 0x0006 to 0x0C80
     * Time Range: 7.5 ms to 4 sec
     */
    uint16_t                      connIntv;

    /** Slave latency for the connection in number of connection events.
     * Range: 0x0000 to 0x01F3
     */
    uint16_t                      connLatency;

    /** Supervision timeout for the LE Link. Supervision timeout will be
     * supervisionTO * 10 ms
     * Time Range: 100 msec to 32 secs
     */
    uint16_t                      supervisionTo;

    /** Peer Device Address  */
    uint8_t                       * peerBdAddr;

    /** Peer Device Address type  */
    cy_en_ble_gap_adv_addr_type_t   peerBdAddrType;

    /** Local Resolvable Private Address  
     * Resolvable Private Address being used by the local device
     * for this connection.
     * This is valid only when the Own_Address_Type in 
     * connection/advertisement parameters 
     * is set to 0x02 or 0x03. For other Own_Address_Type values,
     * this will be all zeros.
     */
    uint8_t                       * localResolvablePvtAddr;

    /** Peer Resolvable Private Address  
     * Resolvable Private Address being used by the peer device
     * for this connection.
     * This is valid only for the Peer_Address_Type
     * 0x02 or 0x03. For other Peer_Address_Type values,
     * this will be all zeros.
    */
    uint8_t                       * peerResolvablePvtAddr;

    /** Connected as - master = CY_BLE_GAP_LL_ROLE_MASTER,
        Slave = CY_BLE_GAP_LL_ROLE_SLAVE */
    uint8_t                       role;

    /** Master clock accuracy
     * 0x00 -> 500 ppm
     * 0x01 -> 250 ppm
     * 0x02 -> 150 ppm
     * 0x03 -> 100 ppm
     * 0x04 -> 75 ppm
     * 0x05 -> 50 ppm
     * 0x06 -> 30 ppm
     * 0x07 -> 20 ppm
     */
    uint8_t                       masterClockAccuracy;

    /** Status corresponding to this event will be HCI error code.
     * Values of 0 indicates connection successfully completed.
     * Refer BLE spec 4.2,Vol2, Part D for Error codes.
     */
    uint8_t                       status;
    
    /** bd handle of the remote device */
    uint8_t                       bdHandle; 

}cy_stc_ble_gap_enhance_conn_complete_param_t;

/** Disconnection Parameters */
typedef struct 
{
    /** HCI error code as defined in Bluetooth 4.1 core specification, Volume 2,
       Part D, section 1.3  */
    uint8_t       status;

    /** Peer bdHandle */
    uint8_t       bdHandle;

    /** Reason for disconnection */
    uint8_t       reason;

}cy_stc_ble_gap_disconnect_param_t;

/** Encryption Change Parameters */
typedef struct 
{
    /** Peer bdHandle */
    uint8_t       bdHandle;

    /** Encryption change event for active connection. 'evParam' can be decoded as
         encryption = CY_BLE_GAP_ENCRYPT_OFF -> Encryption OFF
         encryption = CY_BLE_GAP_ENCRYPT_ON -> Encryption ON */
    uint8_t       encryption;

}cy_stc_ble_gap_encrypt_change_param_t;
/** @} */

/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_gap_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GAP_SetIoCap
***************************************************************************//**
* 
*  This function sets the input and output capability of the BLE Device that is
*  used during authentication procedure. This is a blocking function. No event
*  is generated on calling this function. The input capabilities are described
*  in the following table:
*  
*   Capability  | Description
*   ----------- | -----------
*   No input    | Device does not have the ability to indicate "yes" or "no".
*   Yes/No      | Device has at least two buttons that can be easily mapped to "yes" and "no" or the device has a mechanism whereby the user can indicate either "yes" or "no".
*   Keyboard    | Device has a numeric keyboard that can input the numbers "0" through "9" and a confirmation. Device also has at least two buttons that can be easily mapped to "yes" and "no" or the device has a mechanism whereby the user can indicate either "yes" or "no".
*
*  The output capabilities are described in the following table:
*
*   Capability       | Description
*   -----------      | -----------
*   No output        | Device does not have the ability to display or communicate a 6-digit decimal number.
*   Numeric output   | Device has the ability to display or communicate a 6-digit decimal number.
*  
*  The combined capability is defined in the following table:
*
*   Input Capability     | No Output          | Numeric Output
*   -----------          | -----------        | ------------
*   No input             | NoInputNoOutput    | DisplayOnly
*   Yes/No               | NoInputNoOutput    | DisplayYesNo
*   Keyboard             | KeyboardOnly       | KeyboardDisplay
*  
*  This is a blocking function. No event is generated on calling this function.
*     
* \param param: IO parameter is of type cy_en_ble_gap_iocap_t. 
*     
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying invalid input parameter
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetIoCap
(
    const cy_en_ble_gap_iocap_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_SetSecurityRequirements
***************************************************************************//**
*
*  This function is used to set the security requirements of local device and encryption
*  key size requirement of the local device. This is a blocking function. No event 
*  is generated on calling this function. It is expected to call this API on host stack 
*  on, though can be called at any point except when any of SMP procedure is in progress. 
*  Security requirements are defined in the following table:
*
*   Security Requirement                   | Description
*   ----------------------                 | -----------
*   CY_BLE_GAP_NO_SECURITY_REQUIREMENTS    | Default:Security requirement specifies there are no security requirements
*   CY_BLE_GAP_SEC_UNAUTH_PAIRING          | Bit 0: Legacy pairing with NO MITM protection
*   CY_BLE_GAP_SEC_AUTH_PAIRING            | Bit 1: Legacy pairing with MITM protection
*   CY_BLE_GAP_SEC_SC_PAIRING_WITH_NO_MITM | Bit 2: Secured Connection pairing with NO MITM protection
*   CY_BLE_GAP_SEC_SC_PAIRING_WITH_MITM    | Bit 3: Secured Connection pairing with MITM protection
*   CY_BLE_GAP_SEC_OOB_IN_LEGACY_PAIRING   | Bit 4: Legacy pairing with OOB method
*   CY_BLE_GAP_SEC_OOB_IN_SC_PAIRING       | Bit 5: Secured Connection pairing with OOB method
*
*  After this API is called, the BLE Stack will check whether the received 
*  security request or pairing request or pairing response satisfies local 
*  device security requirements that are set using this API. If local device
*  security requirements are not met, then pairing is rejected by the BLE stack.
*
*  For example: Cy_BLE_GAP_SetSecurityRequirements() is called with secReq as CY_BLE_GAP_SEC_SC_PAIRING_WITH_MITM.
*  Now if the BLE Stack receives any pairing request with SC bit and MITM bit are not set, 
*  then that pairing request will be rejected by the stack.
*
*  Note: If the Secured Connection (SC) Only mode is set, then these security requirements are not 
*  considered during pairing procedure. This is to maintain BWC for SC Only mode.
*
*  \param param: Parameter is pointer to type cy_stc_ble_gap_sec_req_t.
*
*   secReq: Security requirement is a bit-field parameter. The application can
*     set this value with the above defined values in the table. The application can set 
*     multiple security requirements by ORing them in this parameter.
*     For example: If secReq is (CY_BLE_GAP_SEC_UNAUTH_PAIRING | CY_BLE_GAP_SEC_SC_PAIRING_WITH_NO_MITM),
*     then the stack allows pairing only if the received pairing request is either Legacy 
*     pairing with NO MITM or Secured Connection pairing with NO MITM.
*
*   encKeySize: Encryption key size requirement of the local device. 
*     This parameter does not affect anything on the central side. At the peripheral side,
*     when encryption key size is set using this API, then after during pairing if 
*     the negotiated key size is less than the key size set by this API, then the BLE Stack will 
*     reject that pairing request.
*
*     
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying invalid input parameter
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetSecurityRequirements
(
    cy_stc_ble_gap_sec_req_t * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_SetOobData
***************************************************************************//**
* 
*  This function sets OOB presence flag and data. This function should be used 
*  by the application layer if it wants to enable OOB bonding procedure for any
*  specific device identified by 'bdHandle'. This function should be called
*  before initiating authentication or before responding to an authentication
*  request to set OOB flag and data. 
*
*  This is a blocking function. No event is generated on calling this function.
*     
* \param param: parameter is of type cy_stc_ble_gap_oob_info_t. 
*  param->oobData is ignored in case of the Legacy Pairing procedure.
*         
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter
*   CY_BLE_ERROR_NO_DEVICE_ENTITY    | 'bdHandle' does not represent known device entity
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetOobData 
(   
    cy_stc_ble_gap_oob_info_t * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GenerateBdAddress
***************************************************************************//**
* 
*  This function generates either public or random address based on 'type' specified by 
*  'cy_en_ble_gap_addr_type_t' filed of 'cy_stc_ble_gap_bd_addr_info_t'. 
*  It uses BLE Controller's random number generator to generate the random part of the 
*  Bluetooth device address.
*  
*  This is none blocking function. Generated address is informed through 'CY_BLE_EVT_GAP_DEVICE_ADDR_GEN_COMPLETE'
* 
*  \param param: Buffer is of type cy_stc_ble_gap_bd_addr_info_t.
*              param->irk: Buffer containing 128-bit 'IRK' data. This parameter is only
*                                 used when CY_BLE_GAP_RANDOM_PRIV_RESOLVABLE_ADDR is the value set to
*              param->addrType'. type of address to be generated.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
* \note: 
*  Note1: param should point to valid memory location until completion of API
        is indicated via the CY_BLE_EVT_GAP_DEVICE_ADDR_GEN_COMPLETE event.
*  Note2: To guarantee that sequential calls to Cy_BLE_GAP_GenerateBdAddress API generate different addresses,
*         a call to Cy_BLE_GenerateRandomNumber should be made between calls made
*        to the Cy_BLE_GAP_GenerateBdAddress API. These calls to the Cy_BLE_GenerateRandomNumber API
*        should use unique seeds for these calls.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GenerateBdAddress
(
    cy_stc_ble_gap_bd_addr_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_Disconnect
***************************************************************************//**
* 
*  This function disconnects the peer device. It is to be used by the device in
*  GAP Central mode and may be used by a GAP Peripheral device to send a
*  disconnect request. This is a non-blocking function. On disconnection, the
*  following events are generated, in order.
*  * CY_BLE_EVT_GATT_DISCONNECT_IND
*  * CY_BLE_EVT_GAP_DEVICE_DISCONNECTED
*     
* \param param: parameter is of type 'cy_stc_ble_gap_disconnect_info_t'
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' pointer is NULL
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Device identified using 'bdHandle' does not exist
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES  | Stack resources are unavailable
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_Disconnect
(
    cy_stc_ble_gap_disconnect_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetPeerBdAddr
***************************************************************************//**
* 
*  This function reads the peer Bluetooth device address that has already been
*  fetched by the BLE Stack. 'peerBdAddr' stores the peer's Bluetooth device
*  address identified with 'bdHandle'. 
*
*  This is a blocking function. No event is generated on calling this function.
*
* \param param: parameter is of type cy_stc_ble_gap_peer_addr_info_t, where 
*             param->bdHandle: Peer device handle, input parameter
*             param->bdAddr: Empty buffer where the peer Bluetooth device address will be stored.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter for 'peerBdAddr'.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY    | Specified device handle does not map to any device handle entry in the BLE stack.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetPeerBdAddr
(
    cy_stc_ble_gap_peer_addr_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetPeerBdHandle
***************************************************************************//**
* 
*  This function reads the device handle of the remote Bluetooth device using 
*  'peerBdAddr', which has already been fetched by the BLE Stack. 'bdHandle'
*  stores the peer device handle. 
*
*  This is a blocking function. No event is generated on calling this function.
*     
* \param param: parameter is of type cy_stc_ble_gap_peer_addr_info_t, where 
*             param->bdHandle: buffer where peer device handle will be stored, output parameter
*             param->bdAddr: Peer Bluetooth device address will be stored.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | ----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | On specifying NULL as input parameter for 'peerBdAddr' or 'bdHandle'.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Specified device address does not map to any entry in BLE stack.
*             
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetPeerBdHandle
(
    cy_stc_ble_gap_peer_addr_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetPeerDevSecurity
***************************************************************************//**
* 
*  This function enables the application to get the device security of the peer
*  device, which has already been fetched by the BLE Stack, identified using 
*  'bdHandle' when the peer device is in the trusted list. 
*
*  This is a blocking function. No event is generated on calling this function.
* 
* \param param: Pointer to a buffer of type 'cy_stc_ble_gap_auth_info_t' into which security 
*                        information will be written.
*            param->security: It ignores LE Security mode. Security should be interpreted as MITM and no MITM as
*          encryption is always supported if pairing is performed between two devices.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | On specifying NULL as input parameter for 'authInfo'.
*   CY_BLE_ERROR_INVALID_OPERATION       | An error occurred in the BLE stack.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Specified device handle does not map to any device handle entry in the BLE stack.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetPeerDevSecurity 
(   
    cy_stc_ble_gap_auth_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetPeerDevSecurityKeyInfo
***************************************************************************//**
*
*  This function enables the application to know the keys shared by a given peer
*  device upon completion of the security sequence (already fetched by the BLE
*  Stack). The keys are shared by the peer device on initiation of authentication, which
*  is performed using the Cy_BLE_GAP_AuthReq() or Cy_BLE_GAPP_AuthReqReply()
*  function.
*
*  This is a blocking function. No event is generated on calling this function.
*
*  \param param: buffer is of type cy_stc_ble_gap_sec_key_info_t, where peer device security
*                         information will be stored.
*              param->SecKeyParam.bdHandle: input parameters
*              param->localKeysFlag : ignored
*              param->exchangeKeysFlag : Keys exchanged by peer (output parameter)
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter for 'keyInfo'.
*   CY_BLE_ERROR_INVALID_OPERATION   | An error occurred in BLE stack.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY    | Device identified using 'bdHandle' does not exist.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetPeerDevSecurityKeyInfo
(
    cy_stc_ble_gap_sec_key_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetLocalDevSecurityKeyInfo
***************************************************************************//**
* 
*  This function gets the local device's keys and key flags. The IRK received from
*  this function should be used as the input IRK for the function 
*  'Cy_BLE_GAP_GenerateDeviceAddress' to generate Random Private Resolvable address.
*  This is a blocking function. No event is generated on calling this function.
* 
*  \param param: buffer is of type cy_stc_ble_gap_sec_key_info_t, where local device security
*                         information will be stored.
*             param->SecKeyParam.bdHandle: keys corresponding to peer device
*             param->localKeysFlag : ignored
*             param->exchangeKeysFlag : Key to be sent to peer (output parameter)
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | On specifying NULL as input parameters
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Device identified using 'bdHandle' does not exist.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetLocalDevSecurityKeyInfo
(
   cy_stc_ble_gap_sec_key_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_SetSecurityKeys
***************************************************************************//**
* 
*  This function sets the security keys that are to be exchanged with a peer
*  device during key exchange stage of the authentication procedure and sets it in the 
*  BLE Stack. The application is expected to set new keys for new connections. By default,
*  stack will use 'ZEROs' for the keys if not set by the application. The last set keys will be used 
*  by the BLE stack during the pairing procedure.
*
*  This is a blocking function. No event is generated on calling this function.
* This API should be called before the pairing process begins, preferably after 
* 'CY_BLE_EVT_GAP_DEVICE_CONNECTED or CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE'
* is received.
*
*  \param param: buffer is of type cy_stc_ble_gap_sec_key_info_t, which contains the 
*                         security keys to be set to BLE Stack.
*              param->SecKeyParam.bdHandle: input parameters
*              param->exchangeKeysFlag : input parameters 
*              param->localKeysFlag : input parameters
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | On specifying NULL as input parameter for 'keyInfo'
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Device identified using 'bdHandle' does not exist.
* 
* \note:
*       Note1: idAddrInfo is ignored as it is device level information.
*        Use Cy_BLE_GAP_SetIdAddress to set the ID address for the device.
*       Note2: 'exchangeKeysFlag' flags will automatically be ignored if this does not match with 
*        the exchange key flag received for a peer during pairing process.
******************************************************************************/    
cy_en_ble_api_result_t Cy_BLE_GAP_SetSecurityKeys
(
    cy_stc_ble_gap_sec_key_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GenerateKeys
***************************************************************************//**
* 
* This function generates the security keys as per application requirement.
*
*  Generated Keys are informed through 'CY_BLE_EVT_GAP_KEYS_GEN_COMPLETE'
* This API does not generate identity address (keyInfo->idAddrInfo)
*
*  \param param: buffer is of type cy_stc_ble_gap_sec_key_info_t, where generated keys are store.
*              param->SecKeyParam.bdHandle: parameter ignored
*              param->exchangeKeysFlag : parameters ignored
*              param->localKeysFlag : input parameter
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter for 'keyInfo'
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
*  \note
*  Note1: param should point to a valid memory location until completion of API
*       is indicated via CY_BLE_EVT_GAP_KEYS_GEN_COMPLETE event.
*  Note2: To guarantee that sequential calls to Cy_BLE_GAP_GenerateKeys API generate different keys,
*         a call to Cy_BLE_GenerateRandomNumber should be made between calls made
*        to Cy_BLE_GAP_GenerateKeys API. These calls to Cy_BLE_GenerateRandomNumber API
*        should use unique seeds for these calls.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GenerateKeys
(
    cy_stc_ble_gap_sec_key_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_AuthReq
***************************************************************************//**
* 
*  This function starts authentication/pairing procedure with the peer device. 
*  It is a non-blocking function.
*  
*  If the local device is a GAP Central, the pairing request is sent to the GAP
*  Peripheral device. On receiving CY_BLE_EVT_GAP_AUTH_REQ event, the GAP
*  Peripheral is expected to respond by invoking the Cy_BLE_GAPP_AuthReqReply()
*  function.
* 
*  If the local device is GAP Peripheral, a Security Request is sent to GAP
*  Central device. On receiving CY_BLE_EVT_GAP_AUTH_REQ event, the GAP Central
*  device is expected to respond by invoking 'Cy_BLE_GAP_AuthReq ()' function.
*
*  Events shown in the following table may be received by the application based 
*  on the authentication result.
*
*   Event Name                   | Description
*   ------------                 | -----------
*   CY_BLE_EVT_GAP_AUTH_COMPLETE | Pointer to structure of type 'cy_stc_ble_gap_auth_info_t' is returned as parameter to both the peer devices on successful authentication.
*   CY_BLE_EVT_GAP_AUTH_FAILED   | Received by both GAP Central and Peripheral devices (peers) on authentication failure. Data is of type 'cy_en_ble_gap_auth_failed_reason_t'.
*
*  Following events may occur if API result is CY_BLE_SUCCESS:
*
*   Event Name                             | Description
*   ------------                           | -----------
*   CY_BLE_EVT_GAP_SMP_NEGOTIATED_AUTH_INFO| SMP has completed pairing properties (feature exchange) negotiation.
*   CY_BLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT   | SMP keys exchange with peer device is completed.
*   CY_BLE_EVT_GAP_ENCRYPT_CHANGE          | When there is a change in encryption after pairing procedure.
*
*  Based on IO capabilities, security modes following events may occur
*  during pairing procedure.
*  Please refer to the Core spec 4.2 volume 3 part - H section 2.3.5.1
*  to learn more about Selecting Key Generation Method.
*
*   | Event Name                                |
*   | ------------------------                  |
*   | CY_BLE_EVT_GAP_PASSKEY_DISPLAY_REQUEST    |
*   | CY_BLE_EVT_GAP_KEYPRESS_NOTIFICATION      |
*   | CY_BLE_EVT_GAP_NUMERIC_COMPARISON_REQUEST |
*
*  \param param: Pointer to security information of the device of type 
*             cy_stc_ble_gap_auth_info_t. Param->security can take the value
*             from enum cy_en_ble_gap_sec_level_t.
*  NOTE: If the bonding flag in param is set to CY_BLE_GAP_BONDING_NONE, then 
*        SMP keys will not be distributed even if the application has generated 
*        and set the keys explicitly.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*
*  
*   Error codes                          | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' pointer is NULL or if any of the element of this structure are invalid
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Device identified using 'bdHandle' does not exist
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES  | On bonded device is full and application tries to initiate pairing with bonding enable
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_AuthReq
(
    cy_stc_ble_gap_auth_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_AuthPassKeyReply
***************************************************************************//**
* 
*  This function sends a passkey for authentication. It is a non-blocking function.
* 
*  It should be invoked in reply to the authentication request event
*  CY_BLE_EVT_GAP_PASSKEY_ENTRY_REQUEST received by the BLE Stack. This function
*  is used to accept the passkey request and send the passkey or reject the
*  passkey request.
*  
*  * If the authentication operation succeeds, the CY_BLE_EVT_GAP_AUTH_COMPLETE is
*     generated. If the authentication process times out, the CY_BLE_EVT_TIMEOUT 
*     event is generated.
*  * If the authentication fails, the CY_BLE_EVT_GAP_AUTH_FAILED event is generated.
* 
*  \param param: Pointer to security information of the device of type 
*             cy_stc_ble_gap_auth_pk_info_t. 
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Error codes                          | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | If param pointer is NULL
*   CY_BLE_ERROR_NO_DEVICE_ENTITY        | Device identified using 'bdHandle' does not exist.
*   CY_BLE_ERROR_INVALID_OPERATION       | If CY_BLE_EVT_GAP_PASSKEY_ENTRY_REQUEST event is not received before this API call
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_AuthPassKeyReply
(   
    cy_stc_ble_gap_auth_pk_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_FixAuthPassKey
***************************************************************************//**
* Sets or clears a fixed passkey to be used by the SMP procedure.
* This is a blocking function. No event is generated on calling this function.
* \note:
*    Note1:  The fixed passkey will work only if we are the device displaying the
*         passkey and peer has to enter the passkey.
*         This will not work for numeric comparison (secure connections) method.
*    Note2:  The fixed passkey is not persistent across power cycle.
*    Note3:  This API should not be called during an ongoing SMP procedure.
*         Call this API on Stack Init completion.
*
*
*  \param param: Parameter is of type 'cy_stc_ble_gap_auth_fix_pk_info_t' 
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
* 
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If any of input parameters are invalid.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_FixAuthPassKey
(
    cy_stc_ble_gap_auth_fix_pk_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_SetSecureConnectionsOnlyMode
***************************************************************************//**
*
* This API sets the state of the SC Only mode for device.
* If device is in SC Only mode, it will allow pairing to
* complete only with secure connections security. Another kind of pairing will
* lead to pairing failure with reason "Authentication requirement not met"
* You should call this API on host stack on, though it can be called 
* at any point.
* SC Only mode is not persistent across power cycles. It is
* persistent across stack shutdown-init cycles.
*
*  \param param: Parameter is of type 'cy_stc_ble_gap_sc_mode_info_t'
*                bdHandle should be ignored.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Error codes                       | Description
*  ------------                      | -----------
*  CY_BLE_SUCCESS                    | On successful operation.
*  CY_BLE_ERROR_INVALID_OPERATION    | Secure connections feature was not selected in feature config.
*  CY_BLE_ERROR_INVALID_PARAMETER    | If 'param' is NULL or 'state' value is invalid.
*
* Note: This is device level policy and doesn't depend on bdHandle.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetSecureConnectionsOnlyMode
(
    cy_stc_ble_gap_sc_mode_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GenerateSetLocalP256Keys
***************************************************************************//**
*
* This API is used to generate and set  P-256 Public-Private key pair to be using during LE Secure connection 
* pairing procedure. The application may choose to generate a P-256 public-private key pair before pairing 
* process starts. If this API is not called before pairing process starts, the BLE stack will use a default
* public-private key pair.
* Successful completion is informed by CY_BLE_EVT_GAP_GEN_SET_LOCAL_P256_KEYS_COMPLETE event.
* Event parameter contains the keys that are generated and set for LE Secure connection pairing procedure.
*
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_OPERATION        | Pairing is in progress or Secure Connections feature is not enabled.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GenerateSetLocalP256Keys
(   
    void
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_SetLocalP256Keys
***************************************************************************//**
*
* This API is used to set a P-256 Public-Private key pair to be used during the LE Secure connection 
* pairing procedure. The application may choose to set a P-256 public-private key pair before the pairing 
* process starts. If this API is not called before the pairing process starts, the BLE stack will use a default
* public-private key pair. This API is not expected to be called when a pairing procedure is in progress.
* The application can provide a P-256 Public-Private key pair to be set for the LE Secure connection pairing procedure.
*
* \param param: Pointer to structure cy_stc_ble_gap_smp_local_p256_keys_t, that has
*                       fields for local P-256 public-private key pair.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                     | Description
*  ------------                     | -----------
*  CY_BLE_SUCCESS                   | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER   | Parameter is NULL Or Public key is not valid
*  CY_BLE_ERROR_INVALID_OPERATION   | Pairing is in progress.
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetLocalP256Keys
(
    cy_stc_ble_gap_smp_local_p256_keys_t * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_AuthSendKeyPress
***************************************************************************//**
*
* This API is used to send the LE Secure connections key press notification to a peer device during secure connection pairing.
* This API should be called by the application to inform the stack about the passkey entry process started for each digit:
* Started (0), entered (1), erased (2), cleared (3). Once all the digits are entered, application needs to call
* 'Cy_BLE_GAP_AuthPassKeyReply()' to inform the stack for passkey enter completed.
* An error will be returned if key press entry bit was not set in 'pairingProperties' of cy_stc_ble_gap_auth_info_t
* during the authentication procedure.
*
*  \param param: Parameter is of type 'cy_stc_ble_gap_sc_kp_notif_info_t'
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                         | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or notificationType is invalid.  
*  CY_BLE_ERROR_INVALID_OPERATION       | If keypress was not negotiated or Secured Connection is not enabled or passkey entry procedure or pairing procedure is not in progress.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_AuthSendKeyPress
(
    cy_stc_ble_gap_sc_kp_notif_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_GenerateOobData
***************************************************************************//**
*
* This API is used to generate OOB data based on the input parameter (16 Byte random number)
* This API is called to generate OOB data to be used by peer device. Peer device (or local device with peer's OOB data)
* will use 'Cy_BLE_GAP_SetOobData()' to set the oob data to be used for secure connections pairing.
* API compilation is informed through the 'CY_BLE_EVT_GAP_OOB_GENERATED_NOTIFICATION' event.
* Note: This API must be used only during secured connection pairing.
*
*  \param param: parameter is of type 'cy_stc_ble_gap_sc_oob_info_t'.
*              If 'param->rand' is NULL, stack will generate 16 Bytes random number and then will generate OOB data.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | If 'param' is NULL.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | Device identified using 'bdHandle' does not exist.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Sufficient memory is not available to handle this request.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GenerateOobData 
(
    cy_stc_ble_gap_sc_oob_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_SetBdAddress
***************************************************************************//**
* 
* This function sets the Bluetooth device address into the BLE stack's memory.
* This address shall be used for all BLE procedures unless explicitly changed by the application.
* The application layer must call this function every time an address change is required. 
* Bluetooth 4.1 Core specification [3.12] specifies that the Bluetooth device can change its
* private address periodically, with the period being decided by the
* application; there are no limits specified on this period. The application
* layer should maintain its own timers in order to do this.
*
* User should call 'Cy_BLE_GAP_SetIdAddress' API to set identity address if 'Cy_BLE_GAP_SetBdAddress'
* API is used to set public or random static address.
* API compilation is informed through the 'CY_BLE_EVT_SET_DEVICE_ADDR_COMPLETE' event
*
*
*  \param param: Bluetooth Device address retrieved from the BLE stack gets stored
*           to a variable pointed to by this pointer. The variable is of type
*           cy_stc_ble_bd_addr_t.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or 
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetBdAddress
(
    cy_stc_ble_gap_bd_addr_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetBdAddress
***************************************************************************//**
* 
*  This API reads the BD device address from the BLE Controller's memory. This
*  address shall be used for BLE procedures unless explicitly indicated by the BLE
*  Host through HCI commands. 
*  The BD Address is informed through event 'CY_BLE_EVT_GET_DEVICE_ADDR_COMPLETE'
*         
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or 
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetBdAddress
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_SetIdAddress
***************************************************************************//**
*
* This function sets the Bluetooth identity address into BLE Stack. Calling this API will change only
* the identity address of the device. If the public address or static random address is changed by the user, this
* API must be called to set the appropriate address as the identity address.
*
* This is a blocking function. No event is generated on calling this function.
*
* \param param: Pointer to the cy_stc_ble_gap_bd_addr_t structure variable. It has two
*          fields where,
*         * param.addr: Bluetooth Device address buffer that is populated with
*            the device address data.
*         * param.type: Caller function should fill the "address type" to
*            set appropriate address.
*         .
*           Caller function should use param.type = 0x00 to set the "Public
*            Device Address" as identity address. <br>
*           Caller function use param.type = 0x01 to set the "Static Random 
*            Device Address" as identity address.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                     | Description
*  ------------                     | -----------
*  CY_BLE_SUCCESS                   | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetIdAddress
(
    const cy_stc_ble_gap_bd_addr_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_GetBondList
***************************************************************************//**
*
* This function returns the count and Bluetooth device address along with BD handles of the devices in
* the bonded device list in the order of Rank*. This is a blocking function. No event is generated on
* calling this function.
*
* Rank: The newest device bonded will be at 0 index.
*
*
* Refer Bluetooth 4.1 core specification, Volume 3, Part C, section 9.4.4 for
* details on bonded devices.
*   
* \param param: Buffer to which list of bonded devices will be stored
*                       of type cy_stc_ble_gap_bonded_device_list_info_t.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                     | Description
*  ------------                     | -----------
*  CY_BLE_SUCCESS                   | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter.
*
*  Note : Provide sufficient memory for 'param->bdHandleAddrList' based on 'bondListSize' 
*  parameter that is passed during Stack Initialization otherwise provided memory might 
*  be corrupted if memory is not sufficient to store all bonded device information.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_GetBondList
(
    cy_stc_ble_gap_bonded_device_list_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_RemoveDeviceFromBondList
***************************************************************************//**
* 
*  This function removes the device from the bond list.
* 
*  Successful operation is informed through 'CY_BLE_EVT_PENDING_FLASH_WRITE' event
* 
*  \param param: Pointer to peer device address, of type cy_stc_ble_gap_bd_addr_t. If the device
*           address is set to 0, then all devices shall be removed from trusted white list.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter for 'bdAddr'.
*   CY_BLE_ERROR_INVALID_OPERATION   | Operation is not permitted when device is in connected state.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_RemoveDeviceFromBondList
(
    cy_stc_ble_gap_bd_addr_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_RemoveOldestDeviceFromBondedList
***************************************************************************//**
* 
*  This function removes the oldest device from the bonded and white lists. 
*  This API should not be called while in the connected state.
*  If the device is connected to the oldest device and this API is called, it will remove the device that is oldest and not
*  connected. 
* 
*  Successful operation is informed through 'CY_BLE_EVT_PENDING_FLASH_WRITE' event
*
* \return
*   cy_en_ble_api_result_t : Return value indicates whether the function succeeded (0x0000) or failed.
*   Following are the possible error codes returned.
*
*   Errors codes                        | Description
*   ------------                        | -----------
*   CY_BLE_SUCCESS                      | On successful operation.
*   CY_BLE_ERROR_MAX                    | On failure operation.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_RemoveOldestDeviceFromBondedList
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_GAP_SetCeLengthParam
***************************************************************************//**
* 
*  This function sets the connection event length to be used for a specific connection.
*  API completion is informed through the 'CY_BLE_EVT_SET_CE_LENGTH_COMPLETE' event
*
* \return
*   cy_en_ble_api_result_t : Return value indicates whether the function succeeded (0x0000) or failed.
*   Following are the possible error codes returned.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY         | Incorrect bdHandle.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
* Note1: The primary usage of this API is to allow application to control bandwidth
*        by setting connection event lengths for specific connections.
* Note2: Application shall set ceLength to 0xFFFF to allow stack to internally
*        use maximum possible bandwidth for this connection.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetCeLengthParam
(
    cy_stc_ble_gap_ce_length_param_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GAP_SetConnectionPriority
***************************************************************************//**
* 
*  This function sets the link layer connection priority to be used for a 
*  connection during arbitration. Link layer arbiter resolves radio contention 
*  based on the priority level of the contending connections. If one or more contending
*  connections have the same priority level (default configuration), then a round-robin 
*  scheme is used to resolve contention. Priority value of 0x00 corresponds to 
*  highest priority and 0xFF is the lowest and default priority setting for a 
*  connection.
*  API completion is informed through the 'CY_BLE_EVT_SET_CONN_PRIORITY_COMPLETE' event
*
* \return
*   cy_en_ble_api_result_t : Return value indicates whether the function succeeded (0x0000) or failed.
*   Following are the possible error codes returned.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY         | Incorrect bdHandle.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_SetConnectionPriority
(
    cy_stc_ble_gap_set_conn_priority_param_t  * param
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* CY_BLE_STACK_GAP_H_ */

/*EOF*/
