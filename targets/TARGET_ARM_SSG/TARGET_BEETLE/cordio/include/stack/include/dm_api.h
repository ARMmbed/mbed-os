/*************************************************************************************************/
/*!
 *  \file   dm_api.h
 *        
 *  \brief  Device Manager subsystem API.
 *
 *          $Date: 2016-03-29 11:20:44 -0700 (Tue, 29 Mar 2016) $
 *          $Revision: 6512 $
 *  
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef DM_API_H
#define DM_API_H

#include "hci_api.h"
#include "cfg_stack.h"
#include "smp_defs.h"
#include "wsf_sec.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Device role */
#define DM_ROLE_MASTER              HCI_ROLE_MASTER  /*! Role is master */
#define DM_ROLE_SLAVE               HCI_ROLE_SLAVE   /*! Role is slave */

/*! The GAP discovery mode */
#define DM_DISC_MODE_NONE           0     /*! GAP non-discoverable */
#define DM_DISC_MODE_LIMITED        1     /*! GAP limited discoverable mode */
#define DM_DISC_MODE_GENERAL        2     /*! GAP general discoverable mode */

/*! The type of connectable or discoverable of advertising */
#define DM_ADV_CONN_UNDIRECT        0     /*! Connectable undirected advertising */
#define DM_ADV_CONN_DIRECT          1     /*! Connectable directed advertising */
#define DM_ADV_DISC_UNDIRECT        2     /*! Discoverable undirected advertising */
#define DM_ADV_NONCONN_UNDIRECT     3     /*! Non-connectable undirected advertising */
#define DM_ADV_CONN_DIRECT_LO_DUTY  4     /*! Connectable directed low duty cycle advertising */
#define DM_ADV_SCAN_RESPONSE        4     /*! Scan response */
#define DM_ADV_NONE                 255   /*! For internal use only */

/*! Whether data is located in the advertising data or the scan response data */
#define DM_DATA_LOC_ADV             0     /*! Locate data in the advertising data */
#define DM_DATA_LOC_SCAN            1     /*! Locate data in the scan response data */

/*! The scan type */
#define DM_SCAN_TYPE_PASSIVE        0     /*! Passive scan */
#define DM_SCAN_TYPE_ACTIVE         1     /*! Active scan */

/*! Advertising channel map */
#define DM_ADV_CHAN_37              HCI_ADV_CHAN_37  /*! Advertising channel 37 */
#define DM_ADV_CHAN_38              HCI_ADV_CHAN_38  /*! Advertising channel 38 */
#define DM_ADV_CHAN_39              HCI_ADV_CHAN_39  /*! Advertising channel 39 */
#define DM_ADV_CHAN_ALL             (HCI_ADV_CHAN_37 | HCI_ADV_CHAN_38 | HCI_ADV_CHAN_39)

/*! The client ID parameter to function DmConnRegister() */
#define DM_CLIENT_ID_ATT            0     /*! Identifier for attribute protocol, for internal use only */
#define DM_CLIENT_ID_SMP            1     /*! Identifier for security manager protocol, for internal use only */
#define DM_CLIENT_ID_DM             2     /*! Identifier for device manager, for internal use only */
#define DM_CLIENT_ID_APP            3     /*! Identifier for the application */
#define DM_CLIENT_ID_L2C            4     /*! Identifier for L2CAP */
#define DM_CLIENT_ID_MAX            5     /*! For internal use only */

/*! Unknown connection ID or other error */
#define DM_CONN_ID_NONE             0

/*! The address type */
#define DM_ADDR_PUBLIC              0     /*! Public address */
#define DM_ADDR_RANDOM              1     /*! Random address */
#define DM_ADDR_PUBLIC_IDENTITY     2     /*! Public identity address */
#define DM_ADDR_RANDOM_IDENTITY     3     /*! Random (static) identity address */

/*! Advertising data types */
#define DM_ADV_TYPE_FLAGS           0x01  /*! Flag bits */
#define DM_ADV_TYPE_16_UUID_PART    0x02  /*! Partial list of 16 bit UUIDs */
#define DM_ADV_TYPE_16_UUID         0x03  /*! Complete list of 16 bit UUIDs */
#define DM_ADV_TYPE_32_UUID_PART    0x04  /*! Partial list of 32 bit UUIDs */
#define DM_ADV_TYPE_32_UUID         0x05  /*! Complete list of 32 bit UUIDs */
#define DM_ADV_TYPE_128_UUID_PART   0x06  /*! Partial list of 128 bit UUIDs */
#define DM_ADV_TYPE_128_UUID        0x07  /*! Complete list of 128 bit UUIDs */
#define DM_ADV_TYPE_SHORT_NAME      0x08  /*! Shortened local name */
#define DM_ADV_TYPE_LOCAL_NAME      0x09  /*! Complete local name */
#define DM_ADV_TYPE_TX_POWER        0x0A  /*! TX power level */
#define DM_ADV_TYPE_SM_TK_VALUE     0x10  /*! Security manager TK value */
#define DM_ADV_TYPE_SM_OOB_FLAGS    0x11  /*! Security manager OOB flags */
#define DM_ADV_TYPE_CONN_INTERVAL   0x12  /*! Slave preferred connection interval */
#define DM_ADV_TYPE_SIGNED_DATA     0x13  /*! Signed data */
#define DM_ADV_TYPE_16_SOLICIT      0x14  /*! Service soliticiation list of 16 bit UUIDs */
#define DM_ADV_TYPE_128_SOLICIT     0x15  /*! Service soliticiation list of 128 bit UUIDs */
#define DM_ADV_TYPE_SERVICE_DATA    0x16  /*! Service data - 16-bit UUID */
#define DM_ADV_TYPE_PUBLIC_TARGET   0x17  /*! Public target address */
#define DM_ADV_TYPE_RANDOM_TARGET   0x18  /*! Random target address */
#define DM_ADV_TYPE_APPEARANCE      0x19  /*! Device appearance */
#define DM_ADV_TYPE_ADV_INTERVAL    0x1A  /*! Advertising interval */
#define DM_ADV_TYPE_BD_ADDR         0x1B  /*! LE Bluetooth device address */
#define DM_ADV_TYPE_ROLE            0x1C  /*! LE role */
#define DM_ADV_TYPE_32_SOLICIT      0x1F  /*! Service soliticiation list of 32 bit UUIDs */
#define DM_ADV_TYPE_SVC_DATA_32     0x20  /*! Service data - 32-bit UUID */
#define DM_ADV_TYPE_SVC_DATA_128    0x21  /*! Service data - 128-bit UUID */
#define DM_ADV_TYPE_LESC_CONFIRM    0x22  /*! LE Secure Connections confirm value */
#define DM_ADV_TYPE_LESC_RANDOM     0x23  /*! LE Secure Connections random value */
#define DM_ADV_TYPE_URI             0x24  /*! URI */
#define DM_ADV_TYPE_MANUFACTURER    0xFF  /*! Manufacturer specific data */

/*! Bit mask for flags advertising data type */
#define DM_FLAG_LE_LIMITED_DISC     0x01  /*! Limited discoverable flag */
#define DM_FLAG_LE_GENERAL_DISC     0x02  /*! General discoverable flag */
#define DM_FLAG_LE_BREDR_NOT_SUP    0x04  /*! BR/EDR not supported flag */

/*! Advertising data element indexes */
#define DM_AD_LEN_IDX               0     /*! Advertising data element len */
#define DM_AD_TYPE_IDX              1     /*! Advertising data element type */
#define DM_AD_DATA_IDX              2     /*! Advertising data element data */

/*! Timeouts defined by the GAP specification; in units of milliseconds */
#define DM_GAP_LIM_ADV_TIMEOUT      180000  /*! Maximum advertising duration in limited discoverable mode */
#define DM_GAP_GEN_DISC_SCAN_MIN    10240   /*! Minimum scan duration for general discovery */
#define DM_GAP_LIM_DISC_SCAN_MIN    10240   /*! Minimum scan duration for limited discovery */
#define DM_GAP_CONN_PARAM_TIMEOUT   30000   /*! Connection parameter update timeout */
#define DM_GAP_SCAN_FAST_PERIOD     30720   /*! Minimum time to perform scanning when user initiated */
#define DM_GAP_ADV_FAST_PERIOD      30000   /*! Minimum time to perform advertising when user initiated */

/*!
 * Advertising, scanning, and connection parameters defined in the GAP specification.
 * In units of 625 microseconds.
 */
#define DM_GAP_SCAN_FAST_INT_MIN    48      /*! Minimum scan interval when user initiated */
#define DM_GAP_SCAN_FAST_INT_MAX    96      /*! Maximum scan interval when user initiated */
#define DM_GAP_SCAN_FAST_WINDOW     48      /*! Scan window when user initiated */
#define DM_GAP_SCAN_SLOW_INT_1      2048    /*! Scan interval 1 when background scannning */
#define DM_GAP_SCAN_SLOW_WINDOW_1   18      /*! Scan window 1 when background scanning */
#define DM_GAP_SCAN_SLOW_INT_2      4096    /*! Scan interval 2 when background scannning */
#define DM_GAP_SCAN_SLOW_WINDOW_2   18      /*! Scan window 2 when background scanning */
#define DM_GAP_ADV_FAST_INT_MIN     48      /*! Minimum advertising interval when user initiated */
#define DM_GAP_ADV_FAST_INT_MAX     96      /*! Maximum advertising interval when user initiated */
#define DM_GAP_ADV_SLOW_INT_MIN     1600    /*! Minimum advertising interval when background advertising */
#define DM_GAP_ADV_SLOW_INT_MAX     1920    /*! Maximum advertising interval when background advertising */

/*! GAP connection establishment latency */
#define DM_GAP_CONN_EST_LATENCY     0

/*! GAP connection intervals in 1.25ms units */
#define DM_GAP_INITIAL_CONN_INT_MIN 24      /*! Minimum initial connection interval */
#define DM_GAP_INITIAL_CONN_INT_MAX 40      /*! Maximum initial onnection interval */

/*! GAP connection establishment minimum and maximum connection event lengths */
#define DM_GAP_CONN_EST_MIN_CE_LEN  0
#define DM_GAP_CONN_EST_MAX_CE_LEN  0

/*! GAP peripheral privacy flag characteristic values */
#define DM_GAP_PRIV_DISABLED        0
#define DM_GAP_PRIV_ENABLED         1

/*! Connection establishment supervision timeout default, in 10ms units */
#define DM_DEFAULT_EST_SUP_TIMEOUT  2000

/*! Pairing authentication/security properties bit mask */
#define DM_AUTH_BOND_FLAG           SMP_AUTH_BOND_FLAG  /*! Bonding requested */
#define DM_AUTH_MITM_FLAG           SMP_AUTH_MITM_FLAG  /*! MITM (authenticated pairing) requested */
#define DM_AUTH_SC_FLAG             SMP_AUTH_SC_FLAG    /*! LE Secure Connections requested */
#define DM_AUTH_KP_FLAG             SMP_AUTH_KP_FLAG    /*! Keypress notifications requested */

/*! Key distribution bit mask */
#define DM_KEY_DIST_LTK             SMP_KEY_DIST_ENC   /*! Distribute LTK used for encryption */
#define DM_KEY_DIST_IRK             SMP_KEY_DIST_ID    /*! Distribute IRK used for privacy */
#define DM_KEY_DIST_CSRK            SMP_KEY_DIST_SIGN  /*! Distribute CSRK used for signed data */

/*! Key type used in DM_SEC_KEY_IND */
#define DM_KEY_LOCAL_LTK            0x01  /*! LTK generated locally for this device */
#define DM_KEY_PEER_LTK             0x02  /*! LTK received from peer device */
#define DM_KEY_IRK                  0x04  /*! IRK and identity info of peer device */
#define DM_KEY_CSRK                 0x08  /*! CSRK of peer device */

/*! Base value for HCI error status values for DM_SEC_PAIR_CMPL_IND */
#define DM_SEC_HCI_ERR_BASE         0x20

#define DM_SEC_LEVEL_NONE           0     /*! Connection has no security */
#define DM_SEC_LEVEL_ENC            1     /*! Connection is encrypted with unauthenticated key */
#define DM_SEC_LEVEL_ENC_AUTH       2     /*! Connection is encrypted with authenticated key */
#define DM_SEC_LEVEL_ENC_LESC       3     /*! Connection is encrypted with LE Secure Connections */
  
/*! Random address types */
#define DM_RAND_ADDR_STATIC         0xC0  /*! Static address */
#define DM_RAND_ADDR_RESOLV         0x40  /*! Resolvable private address */
#define DM_RAND_ADDR_NONRESOLV      0x00  /*! Non-resolvable private address */

/*! Get the type of random address */
#define DM_RAND_ADDR_GET(addr)          ((addr)[5] & 0xC0)

/*! Set the type of random address */
#define DM_RAND_ADDR_SET(addr, type)    {(addr)[5] = ((addr)[5] & 0x3F) | (type);}

/*! Check for Resolvable Random Address */
#define DM_RAND_ADDR_RPA(addr, type)    (((type) == DM_ADDR_RANDOM) && \
                                         (DM_RAND_ADDR_GET((addr)) == DM_RAND_ADDR_RESOLV))

/*! Connection busy/idle state */
#define DM_CONN_IDLE                0     /*! Connection is idle */
#define DM_CONN_BUSY                1     /*! Connection is busy */

/*! Connection busy/idle state bitmask */
#define DM_IDLE_SMP_PAIR            0x0001  /*! SMP pairing in progress */
#define DM_IDLE_DM_ENC              0x0002  /*! DM Encryption setup in progress */
#define DM_IDLE_ATTS_DISC           0x0004  /*! ATTS service discovery in progress */
#define DM_IDLE_APP_DISC            0x0008  /*! App framework service discovery in progress */
#define DM_IDLE_USER_1              0x0010  /*! For use by user application */
#define DM_IDLE_USER_2              0x0020  /*! For use by user application */
#define DM_IDLE_USER_3              0x0040  /*! For use by user application */
#define DM_IDLE_USER_4              0x0080  /*! For use by user application */

/*! Filter policy modes */
#define DM_FILT_POLICY_MODE_ADV     0     /*! Advertising filter policy mode */
#define DM_FILT_POLICY_MODE_SCAN    1     /*! Scanning filter policy mode */
#define DM_FILT_POLICY_MODE_INIT    2     /*! Initiator filter policy mode */

/*! Proprietary internal error codes */
#define DM_ERR_SMP_RX_PDU_LEN_EXCEEDED    0x01  /*! LESC key length exceeded maximum RX PDU length */
#define DM_ERR_ATT_RX_PDU_LEN_EXCEEDED    0x02  /*! Configured ATT MTU exceeded maximum RX PDU length */
#define DM_ERR_L2C_RX_PDU_LEN_EXCEEDED    0x03  /*! Registered COC MPS exceeded maximum RX PDU length */

/*! DM callback events */
#define DM_CBACK_START              0x20  /*! DM callback event starting value */
enum
{
  DM_RESET_CMPL_IND = DM_CBACK_START,     /*! Reset complete */
  DM_ADV_START_IND,                       /*! Advertising started */
  DM_ADV_STOP_IND,                        /*! Advertising stopped */
  DM_ADV_NEW_ADDR_IND,                    /*! New resolvable address has been generated */
  DM_SCAN_START_IND,                      /*! Scanning started */
  DM_SCAN_STOP_IND,                       /*! Scanning stopped */
  DM_SCAN_REPORT_IND,                     /*! Scan data received from peer device */
  DM_CONN_OPEN_IND,                       /*! Connection opened */
  DM_CONN_CLOSE_IND,                      /*! Connection closed */
  DM_CONN_UPDATE_IND,                     /*! Connection update complete */
  DM_SEC_PAIR_CMPL_IND,                   /*! Pairing completed successfully */
  DM_SEC_PAIR_FAIL_IND,                   /*! Pairing failed or other security failure */
  DM_SEC_ENCRYPT_IND,                     /*! Connection encrypted */
  DM_SEC_ENCRYPT_FAIL_IND,                /*! Encryption failed */
  DM_SEC_AUTH_REQ_IND,                    /*! PIN or OOB data requested for pairing */
  DM_SEC_KEY_IND,                         /*! Security key indication */
  DM_SEC_LTK_REQ_IND,                     /*! LTK requested for encyption */
  DM_SEC_PAIR_IND,                        /*! Incoming pairing request from master */
  DM_SEC_SLAVE_REQ_IND,                   /*! Incoming security request from slave */
  DM_SEC_CALC_OOB_IND,                    /*! Result of OOB Confirm Calculation Generation */
  DM_SEC_ECC_KEY_IND,                     /*! Result of ECC Key Generation */
  DM_SEC_COMPARE_IND,                     /*! Result of Just Works/Numeric Comparison Compare Value Calculation */
  DM_SEC_KEYPRESS_IND,                    /*! Keypress indication from peer in passkey security */
  DM_PRIV_RESOLVED_ADDR_IND,              /*! Private address resolved */
  DM_CONN_READ_RSSI_IND,                  /*! Connection RSSI read */
  DM_PRIV_ADD_DEV_TO_RES_LIST_IND,        /*! Device added to resolving list */
  DM_PRIV_REM_DEV_FROM_RES_LIST_IND,      /*! Device removed from resolving list */
  DM_PRIV_CLEAR_RES_LIST_IND,             /*! Resolving list cleared */
  DM_PRIV_READ_PEER_RES_ADDR_IND,         /*! Peer resolving address read */
  DM_PRIV_READ_LOCAL_RES_ADDR_IND,        /*! Local resolving address read */
  DM_PRIV_SET_ADDR_RES_ENABLE_IND,        /*! Address resolving enable set */
  DM_REM_CONN_PARAM_REQ_IND,              /*! Remote connection parameter requested */
  DM_CONN_DATA_LEN_CHANGE_IND,            /*! Data length changed */
  DM_CONN_WRITE_AUTH_TO_IND,              /*! Write authenticated payload complete */
  DM_CONN_AUTH_TO_EXPIRED_IND,            /*! Authenticated payload timeout expired */
  DM_ERROR_IND,                           /*! General error */
  DM_VENDOR_SPEC_IND,                     /*! Vendor specific event */
};

#define DM_CBACK_END                DM_VENDOR_SPEC_IND  /*! DM callback event ending value */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Connection identifier */
typedef uint8_t dmConnId_t;

/*! Configuration structure */
typedef struct
{
  uint8_t dummy;
} dmCfg_t;

/*! LTK data type */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];
  uint8_t                   rand[SMP_RAND8_LEN];
  uint16_t                  ediv;
} dmSecLtk_t;

/*! IRK data type */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];
  bdAddr_t                  bdAddr;
  uint8_t                   addrType;
} dmSecIrk_t;

/*! CSRK data type */
typedef struct
{
  uint8_t                   key[SMP_KEY_LEN];
} dmSecCsrk_t;

/*! union of key types */
typedef union
{
  dmSecLtk_t                ltk;
  dmSecIrk_t                irk;
  dmSecCsrk_t               csrk;
} dmSecKey_t;

/*! Data type for DM_SEC_PAIR_CMPL_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  uint8_t                   auth;         /*! Authentication and bonding flags */
} dmSecPairCmplIndEvt_t;

/*! Data type for DM_SEC_ENCRYPT_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  bool_t                    usingLtk;     /*! TRUE if connection encrypted with LTK */
} dmSecEncryptIndEvt_t;

/*! Data type for DM_SEC_AUTH_REQ_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  bool_t                    oob;          /*! Out-of-band data requested */
  bool_t                    display;      /*! TRUE if pin is to be displayed */
} dmSecAuthReqIndEvt_t;

/*! Data type for DM_SEC_PAIR_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  uint8_t                   auth;         /*! Authentication and bonding flags */
  bool_t                    oob;          /*! Out-of-band pairing data present or not present */
  uint8_t                   iKeyDist;     /*! Initiator key distribution flags */
  uint8_t                   rKeyDist;     /*! Responder key distribution flags */
} dmSecPairIndEvt_t;

/*! Data type for DM_SEC_SLAVE_REQ_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  uint8_t                   auth;         /*! Authentication and bonding flags */
} dmSecSlaveIndEvt_t;

/*! Data type for DM_SEC_KEY_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  dmSecKey_t                keyData;      /*! Key data */
  uint8_t                   type;         /*! Key type */
  uint8_t                   secLevel;     /*! Security level of pairing when key was exchanged */
  uint8_t                   encKeyLen;    /*! Length of encryption key used when data was transferred */
} dmSecKeyIndEvt_t;

/*! Data type for DM_SEC_COMPARE_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*! Header */
  uint8_t                   confirm[SMP_CONFIRM_LEN];  /*! Confirm value */
} dmSecCnfIndEvt_t;

/*! Data type for DM_SEC_KEYPRESS_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;                       /*! Header */
  uint8_t                   notificationType;          /*! Type of keypress notification */
} dmSecKeypressIndEvt_t;

typedef struct
{
  wsfMsgHdr_t               hdr;                       /*! Header */
  uint8_t                   confirm[SMP_CONFIRM_LEN];  /*! Local confirm value */
  uint8_t                   random[SMP_RAND_LEN];      /*! Local random value */
} dmSecOobCalcIndEvt_t;

/*! Data type for DM_ADV_NEW_ADDR_IND */
typedef struct
{
  wsfMsgHdr_t               hdr;          /*! Header */
  bdAddr_t                  addr;         /*! New resolvable private address */
  bool_t                    firstTime;    /*! TRUE when address is generated for the first time */
} dmAdvNewAddrIndEvt_t;

/*! Union of DM callback event data types */
typedef union
{
  wsfMsgHdr_t                        hdr;
  hciLeAdvReportEvt_t                scanReport;
  hciLeConnCmplEvt_t                 connOpen;
  hciLeConnUpdateCmplEvt_t           connUpdate;
  hciDisconnectCmplEvt_t             connClose;
  dmSecPairCmplIndEvt_t              pairCmpl;
  dmSecEncryptIndEvt_t               encryptInd;
  dmSecAuthReqIndEvt_t               authReq;
  dmSecPairIndEvt_t                  pairInd;
  dmSecSlaveIndEvt_t                 slaveInd;
  dmSecKeyIndEvt_t                   keyInd;
  dmSecOobCalcIndEvt_t               oobCalcInd;
  dmSecCnfIndEvt_t                   cnfInd;
  hciLeLtkReqEvt_t                   ltkReqInd;
  hciVendorSpecEvt_t                 vendorSpec;
  dmAdvNewAddrIndEvt_t               advNewAddr;
  wsfSecEccMsg_t                     eccMsg;
  hciReadRssiCmdCmplEvt_t            readRssi;
  hciLeReadPeerResAddrCmdCmplEvt_t   readPeerResAddr;
  hciLeReadLocalResAddrCmdCmplEvt_t  readLocalResAddr;
  hciLeSetAddrResEnableCmdCmplEvt_t  setAddrResEnable;
  hciLeAddDevToResListCmdCmplEvt_t   addDevToResList;
  hciLeRemDevFromResListCmdCmplEvt_t remDevFromResList;
  hciLeClearResListCmdCmplEvt_t      clearResList;
  hciLeRemConnParamReqEvt_t          remConnParamReq;
  hciLeDataLenChangeEvt_t            dataLenChange;
  hciWriteAuthPayloadToCmdCmplEvt_t writeAuthTo;
  hciAuthPayloadToExpiredEvt_t      authToExpired;
} dmEvt_t;

/*! Data type for DmSecSetOob */
typedef struct
{
  uint8_t localRandom[SMP_RAND_LEN];      /*! Random value of the local device */
  uint8_t localConfirm[SMP_CONFIRM_LEN];  /*! Confirm value of the local device */
  uint8_t peerRandom[SMP_RAND_LEN];       /*! Random value of the peer device */
  uint8_t peerConfirm[SMP_CONFIRM_LEN];   /*! Confirm value of the peer device */
} dmSecLescOobCfg_t;

/*! Callback type */
typedef void (*dmCback_t)(dmEvt_t *pDmEvt);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     DmRegister
 *        
 *  \brief  Register a callback with DM for scan and advertising events.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmRegister(dmCback_t cback);

/*************************************************************************************************/
/*!
 *  \fn     DmFindAdType
 *        
 *  \brief  Find an advertising data element in the given advertising or scan response data.
 *
 *  \param  adType  Advertising data element type to find.
 *  \param  dataLen Data length.
 *  \param  pData   Pointer to advertising or scan response data.
 *
 *  \return Pointer to the advertising data element byte array or NULL if not found.
 */
/*************************************************************************************************/
uint8_t *DmFindAdType(uint8_t adType, uint8_t dataLen, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvInit
 *        
 *  \brief  Initialize DM advertising.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvStart
 *        
 *  \brief  Start advertising using the given advertising type and duration.
 *
 *  \param  advType       Advertising type.
 *  \param  duration      The advertising duration, in milliseconds.
 *  \param  peerAddrType  Peer address type.
 *  \param  pPeerAddr     Peer address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvStart(uint8_t advType, uint16_t duration, uint8_t peerAddrType, uint8_t *pPeerAddr);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvStop
 *        
 *  \brief  Stop advertising.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvStop(void);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetInterval
 *        
 *  \brief  Set the minimum and maximum advertising intervals.
 *
 *  \param  intervalMin Minimum advertising interval.
 *  \param  intervalMax Maximum advertising interval.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetInterval(uint16_t intervalMin, uint16_t intervalMax);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetChannelMap
 *        
 *  \brief  Include or exclude certain channels from the advertising channel map.
 *
 *  \param  channelMap  Advertising channel map. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetChannelMap(uint8_t channelMap);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetData
 *        
 *  \brief  Set the advertising or scan response data to the given data.
 *
 *  \param  location  Data location.
 *  \param  len       Length of the data.  Maximum length is 31 bytes.
 *  \param  pData     Pointer to the data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetData(uint8_t location, uint8_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetAddrType
 *        
 *  \brief  Set the local address type used while advertising.  This function can be used to
 *          configure advertising to use a random address.
 *
 *  \param  addrType  Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvSetAddrType(uint8_t addrType);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetAdValue
 *        
 *  \brief  Set the value of an advertising data element in the given advertising or
 *          scan response data.  If the element already exists in the data then it is replaced
 *          with the new value.  If the element does not exist in the data it is appended
 *          to it, space permitting.
 *
 *  \param  adType       Advertising data element type. 
 *  \param  len          Length of the value.  Maximum length is 29 bytes.
 *  \param  pValue       Pointer to the value.
 *  \param  pAdvDataLen  Advertising or scan response data length.  The new length is returned
 *                       in this parameter.
 *  \param  pAdvData     Pointer to advertising or scan response data.
 *
 *  \return TRUE if the element was successfully added to the data, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvSetAdValue(uint8_t adType, uint8_t len, uint8_t *pValue, uint8_t *pAdvDataLen,
                       uint8_t *pAdvData);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvSetName
 *        
 *  \brief  Set the device name in the given advertising or scan response data.  If the
 *          name can only fit in the data if it is shortened, the name is shortened
 *          and the AD type is changed to DM_ADV_TYPE_SHORT_NAME.
 *
 *  \param  len          Length of the name.  Maximum length is 29 bytes.
 *  \param  pValue       Pointer to the name in UTF-8 format.
 *  \param  pAdvDataLen  Advertising or scan response data length.  The new length is returned
 *                       in this parameter.
 *  \param  pAdvData     Pointer to advertising or scan response data.
 *
 *  \return TRUE if the element was successfully added to the data, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmAdvSetName(uint8_t len, uint8_t *pValue, uint8_t *pAdvDataLen, uint8_t *pAdvData);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvPrivInit
 *        
 *  \brief  Initialize private advertising.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvPrivInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvPrivStart
 *        
 *  \brief  Start using a private resolvable address.
 *
 *  \param  changeInterval  Interval between automatic address changes, in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvPrivStart(uint16_t changeInterval);

/*************************************************************************************************/
/*!
 *  \fn     DmAdvPrivStop
 *        
 *  \brief  Stop using a private resolvable address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmAdvPrivStop(void);

/*************************************************************************************************/
/*!
 *  \fn     DmScanInit
 *        
 *  \brief  Initialize DM scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmScanStart
 *        
 *  \brief  Start scanning.
 *
 *  \param  mode      Discoverability mode.
 *  \param  scanType  Scan type.
 *  \param  filterDup Filter duplicates.  Set to TRUE to filter duplicate responses received
 *                    from the same device.  Set to FALSE to receive all responses.
 *  \param  duration  The scan duration, in milliseconds.  If set to zero, scanning will
 *                    continue until DmScanStop() is called.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanStart(uint8_t mode, uint8_t scanType, bool_t filterDup, uint16_t duration);

/*************************************************************************************************/
/*!
 *  \fn     DmScanStop
 *        
 *  \brief  Stop scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanStop(void);

/*************************************************************************************************/
/*!
 *  \fn     DmScanSetInterval
 *        
 *  \brief  Set the scan interval and window.
 *
 *  \param  scanInterval  The scan interval.
 *  \param  scanWindow    The scan window.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanSetInterval(uint16_t scanInterval, uint16_t scanWindow);

/*************************************************************************************************/
/*!
 *  \fn     DmScanSetAddrType
 *        
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
 *  \fn     DmConnInit
 *        
 *  \brief  Initialize DM connection manager.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmConnMasterInit
 *        
 *  \brief  Initialize DM connection manager for operation as master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnMasterInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmConnSlaveInit
 *        
 *  \brief  Initialize DM connection manager for operation as slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmConnRegister
 *        
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
 *  \fn     DmConnOpen
 *        
 *  \brief  Open a connection to a peer device with the given address.
 *
 *  \param  clientId  The client identifier.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnOpen(uint8_t clientId, uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \fn     DmConnClose
 *        
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
 *  \fn     DmConnAccept
 *        
 *  \brief  Accept a connection from the given peer device by initiating directed advertising.
 *
 *  \param  clientId  The client identifier.
 *  \param  advType   Advertising type.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnAccept(uint8_t clientId, uint8_t advType, uint8_t addrType, uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \fn     DmConnUpdate
 *        
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
 *  \fn     DmConnSetScanInterval
 *        
 *  \brief  Set the scan interval and window for created connections created with DmConnOpen().
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
 *  \fn     DmConnSetConnSpec
 *        
 *  \brief  Set the connection specification parameters for connections created with DmConnOpen().
 *
 *  \param  pConnSpec   Connection spec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetConnSpec(hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \fn     DmConnSetAddrType
 *        
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
 *  \fn     DmConnSetIdle
 *        
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
 *  \fn     DmConnCheckIdle
 *        
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
 *  \fn     DmConnReadRssi
 *        
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
*  \fn     DmRemoteConnParamReqReply
*
*  \brief  Reply to the HCI remote connection parameter request event.  This command is used to
*          indicate that the Host has accepted the remote device’s request to change connection
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
*  \fn     DmRemoteConnParamReqNegReply
*
*  \brief  Negative reply to the HCI remote connection parameter request event.  This command
*          is used to indicate that the Host has rejected the remote device’s request to change
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
*  \fn     DmConnSetDataLen
*
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
*  \fn     DmWriteAuthPayloadTimeout
*
*  \brief  Set authenticated payload timeout for a given connection.
*
*  \param  connId      Connection identifier.
*  \param  timeout     Timeout period in units of 10ms
*
*  \return None.
*/
/*************************************************************************************************/
void DmWriteAuthPayloadTimeout(dmConnId_t connId, uint16_t timeout);

/*************************************************************************************************/
/*!
 *  \fn     DmDevReset
 *        
 *  \brief  Reset the device.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevReset(void);

/*************************************************************************************************/
/*!
 *  \fn     DmDevRole
 *        
 *  \brief  Return the device role indicating master or slave.
 *
 *  \return Device role.
 */
/*************************************************************************************************/
uint8_t DmDevRole(void);

/*************************************************************************************************/
/*!
 *  \fn     DmDevSetRandAddr
 *        
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
 *  \fn     DmDevWhiteListAdd
 *        
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
 *  \fn     DmDevWhiteListRemove
 *        
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
 *  \fn     DmDevWhiteListClear
 *        
 *  \brief  Clear the white list.  Note that this function cannot be called while
 *          advertising, scanning, or connecting with white list filtering active.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListClear(void);

/*************************************************************************************************/
/*!
 *  \fn     DmDevSetFilterPolicy
 *        
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
 *  \fn     DmDevVsInit
 *        
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevVsInit(uint8_t param);

/*************************************************************************************************/
/*!
 *  \fn     DmSecInit
 *        
 *  \brief  Initialize DM security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecLescInit
 *        
 *  \brief  Initialize DM LE Secure Connections security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecLescInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecPairReq
 *        
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
 *  \fn     DmSecPairRsp
 *        
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
 *  \fn     DmSecCancelReq
 *        
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
 *  \fn     DmSecAuthRsp
 *        
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
 *  \fn     DmSecSlaveReq
 *        
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
 *  \fn     DmSecEncryptReq
 *        
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
 *  \fn     DmSecLtkRsp
 *        
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
 *  \fn     DmSecSetLocalCsrk
 *        
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
 *  \fn     DmSecSetLocalIrk
 *        
 *  \brief  This function sets the local IRK used by the device.  
 *
 *  \param  pCsrk     Pointer to IRK.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetLocalIrk(uint8_t *pIrk);

/*************************************************************************************************/
/*!
 *  \fn     DmPrivInit
 *        
 *  \brief  Initialize DM privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DmPrivResolveAddr
 *        
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
*  \fn     DmPrivAddDevToResList
*
*  \brief  Add device to resolving list.  When complete the client's callback function
*          is called with a DM_PRIV_ADD_DEV_TO_RES_LIST_IND event.  The client must wait
*          to receive this event before executing this function again.
*
*          Note: This command cannot be used when address translation is enabled in the LL and:
*                - Advertising is enabled
*                - Scanning is enabled
*                - Create connection command is outstanding
*
*          Note: If the local or peer IRK associated with the peer Identity Address is all
*                zeros then the LL will use or accept the local or peer Identity Address.
*
*          Note: 'enableLlPriv' should be set to TRUE when the last device is being added
*                 to resolving list to enable address resolution in LL.
*
*  \param  addrType      Peer identity address type.
*  \param  pIdentityAddr Peer identity address.
*  \param  pPeerIrk      The peer's identity resolving key.
*  \param  pLocalIrk     The local identity resolving key.
*  \param  enableLlPriv  Set to TRUE to enable address resolution in LL.
*  \param  param         client-defined parameter returned with callback event.
*
*  \return None.
*/
/*************************************************************************************************/
void DmPrivAddDevToResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t *pPeerIrk,
                           uint8_t *pLocalIrk, bool_t enableLlPriv, uint16_t param);

/*************************************************************************************************/
/*!
*  \fn     DmPrivRemDevFromResList
*
*  \brief  Remove device from resolving list.  When complete the client's callback function
*          is called with a DM_PRIV_REM_DEV_FROM_RES_LIST_IND event.  The client must wait to
*          receive this event before executing this function again.
*
*          Note: This command cannot be used when address translation is enabled in the LL and:
*                - Advertising is enabled
*                - Scanning is enabled
*                - Create connection command is outstanding
*
*  \param  addrType      Peer identity address type.
*  \param  pIdentityAddr Peer identity address.
*  \param  param         client-defined parameter returned with callback event.
*
*  \return None.
*/
/*************************************************************************************************/
void DmPrivRemDevFromResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint16_t param);

/*************************************************************************************************/
/*!
*  \fn     DmPrivClearResList
*
*  \brief  Clear resolving list.  When complete the client's callback function is called with a
*          DM_PRIV_CLEAR_RES_LIST_IND event.  The client must wait to receive this event before
*          executing this function again.
*
*          Note: This command cannot be used when address translation is enabled in the LL and:
*                - Advertising is enabled
*                - Scanning is enabled
*                - Create connection command is outstanding
*
*          Note: Address resolution in LL will be disabled when resolving list's cleared
*                successfully.
*
*  \return None.
*/
/*************************************************************************************************/
void DmPrivClearResList(void);

/*************************************************************************************************/
/*!
*  \fn     DmPrivReadPeerResolvableAddr
*
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
*  \fn     DmPrivReadLocalResolvableAddr
*
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
*  \fn     DmPrivSetAddrResEnable
*
*  \brief  Enable or disable address resolution in LL.  When complete the client's callback
*          function is called with a DM_PRIV_SET_ADDR_RES_ENABLE_IND event.  The client must
*          wait to receive this event before executing this function again.
*
*          Note: This command can be used at any time except when:
*                - Advertising is enabled
*                - Scanning is enabled
*                - Create connection command is outstanding
*
*  \param  enable   Set to TRUE to enable address resolution or FALSE to disable it.
*
*  \return None.
*/
/*************************************************************************************************/
void DmPrivSetAddrResEnable(bool_t enable);

/*************************************************************************************************/
/*!
*  \fn     DmPrivSetResolvablePrivateAddrTimeout
*
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
*  \fn     DmLlPrivEnabled
*
*  \brief  Whether LL Privacy is enabled.
*
*  \return TRUE if LL Privacy is enabled. FALSE, otherwise.
*/
/*************************************************************************************************/
bool_t DmLlPrivEnabled(void);

/*************************************************************************************************/
/*!
*  \fn     DmLlAddrType
*
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
*  \fn     DmHostAddrType
*
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
 *  \fn     DmL2cConnUpdateCnf
 *        
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
 *  \fn     DmL2cConnUpdateInd
 *        
 *  \brief  For internal use only.  L2C calls this function when it receives a connection update
 *          request from a peer device.
 *
 *  \param  identifier  Identifier value.
 *  \param  handle      Connection handle.
 *  \param  pConnSpec   Connection spec parameters.
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cConnUpdateInd(uint8_t identifier, uint16_t handle, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \fn     DmConnIdByHandle
 *        
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
 *  \fn     DmConnInUse
 *        
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
 *  \fn     DmConnPeerAddrType
 *        
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
 *  \fn     DmConnPeerAddr
 *        
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
 *  \fn     DmConnLocalAddrType
 *        
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
 *  \fn     DmConnLocalAddr
 *        
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
*  \fn     DmConnPeerRpa
*
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
*  \fn     DmConnLocalRpa
*
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
 *  \fn     DmConnSecLevel
 *        
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
 *  \fn     DmSmpEncryptReq
 *        
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
 *  \fn     DmSmpCbackExec
 *        
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
 *  \fn     DmSecGetLocalCsrk
 *        
 *  \brief  For internal use only.  This function gets the local CSRK used by the device.  
 *
 *  \return Pointer to CSRK.
 */
/*************************************************************************************************/
uint8_t *DmSecGetLocalCsrk(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecGetLocalIrk
 *        
 *  \brief  For internal use only.  This function gets the local IRK used by the device.  
 *
 *  \return Pointer to IRK.
 */
/*************************************************************************************************/
uint8_t *DmSecGetLocalIrk(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecGenerateEccKeyReq
 *        
 *  \brief  This function generates an ECC key for use with LESC security.  
 *
 *  \return none.
 */
/*************************************************************************************************/
void DmSecGenerateEccKeyReq(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecSetEccKey
 *        
 *  \brief  This function sets the ECC key for use with LESC security.  
 *
 *  \param  pKey      Pointer to key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetEccKey(wsfSecEccKey_t *pKey);

/*************************************************************************************************/
/*!
 *  \fn     DmSecGetEccKey
 *        
 *  \brief  This function gets the local ECC key for use with LESC security.
 *
 *  \return Pointer to local ECC key.
 */
/*************************************************************************************************/
wsfSecEccKey_t *DmSecGetEccKey(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecSetDebugEccKey
 *        
 *  \brief  This function sets the ECC key for use with LESC security to standard debug keys values.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetDebugEccKey(void);

/*************************************************************************************************/
/*!
 *  \fn     DmSecSetOob
 *        
 *  \brief  This function configures the DM to use OOB pairing for the given connection.
 *          The pRand and pConfirm contain the Random and Confirm values exchanged via
 *          out of band methods.
 *
 *  \param  connId      ID of the connection
 *  \param  pConfig     Pointer to OOB configuration.
 *
 *  \return Pointer to IRK.
 */
/*************************************************************************************************/
void DmSecSetOob(dmConnId_t connId, dmSecLescOobCfg_t *pConfig);

/*************************************************************************************************/
/*!
 *  \fn     DmSecCalcOobReq
 *        
 *  \brief  This function calculates the local random and confirm values used in LESC OOB pairing. 
 *          The operation's result is posted as a DM_SEC_CALC_OOB_IND event to the application's DM
 *          callback handler.  The local rand and confirm values are exchanged with the peer via 
 *          out-of-band (OOB) methods and passed into the DmSecSetOob after DM_CONN_OPEN_IND.
 *
 *  \param  pRand       Random value used in calculation.
 *  \param  pPubKeyX    X component of the local public key.
 *
 *  \return none.
 */
/*************************************************************************************************/
void DmSecCalcOobReq(uint8_t *pRand, uint8_t *pPubKeyX);

/*************************************************************************************************/
/*!
 *  \fn     DmSecCompareRsp
 *        
 *  \brief  This function is called by the application in response to a DM_SEC_COMPARE_IND event.
 *          The valid parameter indicates if the compare value of the DM_SEC_COMPARE_IND was valid. 
 *
 *  \param  connId      ID of the connection
 *  \param  valid       TRUE if compare value was valid
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecCompareRsp(dmConnId_t connId, bool_t valid);

/*************************************************************************************************/
/*!
 *  \fn     DmSecGetCompareValue
 *        
 *  \brief  This function returns the 6-digit compare value for the specified 128-bit confirm value.
 *
 *  \param  pConfirm    Pointer to 128-bit comfirm value.
 *
 *  \return Six-digit compare value.
 */
/*************************************************************************************************/
uint32_t DmSecGetCompareValue(uint8_t *pConfirm);

#ifdef __cplusplus
};
#endif

#endif /* DM_API_H */
