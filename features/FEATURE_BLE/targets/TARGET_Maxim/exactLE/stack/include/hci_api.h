/*************************************************************************************************/
/*!
 *  \file   hci_api.h
 *
 *  \brief  HCI subsystem API.
 *
 *          $Date: 2011-10-14 21:35:03 -0700 (Fri, 14 Oct 2011) $
 *          $Revision: 191 $
 *
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_API_H
#define HCI_API_H

#include "wsf_types.h"
#include "hci_defs.h"
#include "wsf_os.h"
#include "bda.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Internal event values for the HCI event and sec callbacks */
#define HCI_RESET_SEQ_CMPL_CBACK_EVT                    0   /*! Reset sequence complete */
#define HCI_LE_CONN_CMPL_CBACK_EVT                      1   /*! LE connection complete */
#define HCI_DISCONNECT_CMPL_CBACK_EVT                   2   /*! LE disconnect complete */
#define HCI_LE_CONN_UPDATE_CMPL_CBACK_EVT               3   /*! LE connection update complete */
#define HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL_CBACK_EVT    4   /*! LE create connection cancel command complete */
#define HCI_LE_ADV_REPORT_CBACK_EVT                     5   /*! LE advertising report */
#define HCI_READ_RSSI_CMD_CMPL_CBACK_EVT                6   /*! Read RSSI command complete */
#define HCI_LE_READ_CHAN_MAP_CMD_CMPL_CBACK_EVT         7   /*! LE Read channel map command complete */
#define HCI_READ_TX_PWR_LVL_CMD_CMPL_CBACK_EVT          8   /*! Read transmit power level command complete */
#define HCI_READ_REMOTE_VER_INFO_CMPL_CBACK_EVT         9   /*! Read remote version information complete */
#define HCI_LE_READ_REMOTE_FEAT_CMPL_CBACK_EVT          10  /*! LE read remote features complete */
#define HCI_LE_LTK_REQ_REPL_CMD_CMPL_CBACK_EVT          11  /*! LE LTK request reply command complete */
#define HCI_LE_LTK_REQ_NEG_REPL_CMD_CMPL_CBACK_EVT      12  /*! LE LTK request negative reply command complete */
#define HCI_ENC_KEY_REFRESH_CMPL_CBACK_EVT              13  /*! Encryption key refresh complete */
#define HCI_ENC_CHANGE_CBACK_EVT                        14  /*! Encryption change */
#define HCI_LE_LTK_REQ_CBACK_EVT                        15  /*! LE LTK request */
#define HCI_VENDOR_SPEC_CMD_STATUS_CBACK_EVT            16  /*! Vendor specific command status */
#define HCI_VENDOR_SPEC_CMD_CMPL_CBACK_EVT              17  /*! Vendor specific command complete */
#define HCI_VENDOR_SPEC_CBACK_EVT                       18  /*! Vendor specific */
#define HCI_HW_ERROR_CBACK_EVT                          19  /*! Hardware error */
#define HCI_LE_ENCRYPT_CMD_CMPL_CBACK_EVT               20  /*! LE encrypt command complete */
#define HCI_LE_RAND_CMD_CMPL_CBACK_EVT                  21  /*! LE rand command complete */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Connection specification type */
typedef struct
{
  uint16_t            connIntervalMin;
  uint16_t            connIntervalMax;
  uint16_t            connLatency;
  uint16_t            supTimeout;
  uint16_t            minCeLen;
  uint16_t            maxCeLen;
} hciConnSpec_t;

/*! LE connection complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             role;
  uint8_t             addrType;
  bdAddr_t            peerAddr;
  uint16_t            connInterval;
  uint16_t            connLatency;
  uint16_t            supTimeout;
  uint8_t             clockAccuracy;
} hciLeConnCmplEvt_t;

/*! Disconnect complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             reason;
} hciDisconnectCmplEvt_t;

/*! LE connection update complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint16_t            connInterval;
  uint16_t            connLatency;
  uint16_t            supTimeout;
} hciLeConnUpdateCmplEvt_t;

/*! LE create connection cancel command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
} hciLeCreateConnCancelCmdCmplEvt_t;

/*! LE advertising report event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             *pData;
  uint8_t             len;
  int8_t              rssi;
  uint8_t             eventType;
  uint8_t             addrType;
  bdAddr_t            addr;
} hciLeAdvReportEvt_t;

/*! Read RSSI command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint8_t             handle;
  int8_t              rssi;
} hciReadRssiCmdCmplEvt_t;

/*! LE Read channel map command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             chanMap[HCI_CHAN_MAP_LEN];
} hciReadChanMapCmdCmplEvt_t;

/*! Read transmit power level command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint8_t             handle;
  int8_t              pwrLvl;
} hciReadTxPwrLvlCmdCmplEvt_t;

/*! Read remote version information complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             version;
  uint16_t            mfrName;
  uint16_t            subversion; 
} hciReadRemoteVerInfoCmplEvt_t;

/*! LE read remote features complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             features[HCI_FEAT_LEN];
} hciLeReadRemoteFeatCmplEvt_t;

/*! LE LTK request reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeLtkReqReplCmdCmplEvt_t;

/*! LE LTK request negative reply command complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciLeLtkReqNegReplCmdCmplEvt_t;

/*! Encryption key refresh complete event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
} hciEncKeyRefreshCmpl_t;

/*! Encryption change event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint8_t             status;
  uint16_t            handle;
  uint8_t             enabled;
} hciEncChangeEvt_t;

/*! LE LTK request event */
typedef struct
{
  wsfMsgHdr_t         hdr;
  uint16_t            handle;
  uint8_t             randNum[HCI_RAND_LEN];
  uint16_t            encDiversifier;
} hciLeLtkReqEvt_t;

/*! Vendor specific command status event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint16_t           opcode;
} hciVendorSpecCmdStatusEvt_t;

/*! Vendor specific command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint16_t           opcode;
  uint8_t            param[1];
} hciVendorSpecCmdCmplEvt_t;

/*! Vendor specific event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            param[1];
} hciVendorSpecEvt_t;

/*! Hardware error event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            code;
} hciHwErrorEvt_t;

/*! LE encrypt command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            data[HCI_ENCRYPT_DATA_LEN];
} hciLeEncryptCmdCmplEvt_t;

/*! LE rand command complete event */
typedef struct
{
  wsfMsgHdr_t        hdr;
  uint8_t            status;
  uint8_t            randNum[HCI_RAND_LEN];
} hciLeRandCmdCmplEvt_t;


/*! Union of all event types */
typedef union
{
  wsfMsgHdr_t                       hdr;
  wsfMsgHdr_t                       resetSeqCmpl;
  hciLeConnCmplEvt_t                leConnCmpl;
  hciDisconnectCmplEvt_t            disconnectCmpl;
  hciLeConnUpdateCmplEvt_t          leConnUpdateCmpl;
  hciLeCreateConnCancelCmdCmplEvt_t leCreateConnCancelCmdCmpl;
  hciLeAdvReportEvt_t               leAdvReport;
  hciReadRssiCmdCmplEvt_t           readRssiCmdCmpl;
  hciReadChanMapCmdCmplEvt_t        readChanMapCmdCmpl;    
  hciReadTxPwrLvlCmdCmplEvt_t       readTxPwrLvlCmdCmpl;   
  hciReadRemoteVerInfoCmplEvt_t     readRemoteVerInfoCmpl;  
  hciLeReadRemoteFeatCmplEvt_t      leReadRemoteFeatCmpl;  
  hciLeLtkReqReplCmdCmplEvt_t       leLtkReqReplCmdCmpl;   
  hciLeLtkReqNegReplCmdCmplEvt_t    leLtkReqNegReplCmdCmpl;
  hciEncKeyRefreshCmpl_t            encKeyRefreshCmpl;       
  hciEncChangeEvt_t                 encChange;             
  hciLeLtkReqEvt_t                  leLtkReq;                 
  hciVendorSpecCmdStatusEvt_t       vendorSpecCmdStatus;   
  hciVendorSpecCmdCmplEvt_t         vendorSpecCmdCmpl;     
  hciVendorSpecEvt_t                vendorSpec;            
  hciHwErrorEvt_t                   hwError;
  hciLeEncryptCmdCmplEvt_t          leEncryptCmdCmpl;
  hciLeRandCmdCmplEvt_t             leRandCmdCmpl;
} hciEvt_t;

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

typedef void (*hciEvtCback_t)(hciEvt_t *pEvent);
typedef void (*hciSecCback_t)(hciEvt_t *pEvent);
typedef void (*hciAclCback_t)(uint8_t *pData);
typedef void (*hciFlowCback_t)(uint16_t handle, bool_t flowDisabled);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! Initialization, registration, and reset */
void HciEvtRegister(hciEvtCback_t evtCback);
void HciSecRegister(hciSecCback_t secCback);
void HciAclRegister(hciAclCback_t aclCback, hciFlowCback_t flowCback);
void HciResetSequence(void);
void HciVsInit(uint8_t param);

/*! Optimization interface */
uint8_t *HciGetBdAddr(void);
uint8_t HciGetWhiteListSize(void);
int8_t HciGetAdvTxPwr(void);
uint16_t HciGetBufSize(void);
uint8_t HciGetNumBufs(void);
uint8_t *HciGetSupStates(void);
uint8_t HciGetLeSupFeat(void);

/*! ACL data interface */
void HciSendAclData(uint8_t *pAclData);

/*! Command interface */
void HciDisconnectCmd(uint16_t handle, uint8_t reason);
void HciLeAddDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr);
void HciLeClearWhiteListCmd(void);
void HciLeConnUpdateCmd(uint16_t handle, hciConnSpec_t *pConnSpec);
void HciLeCreateConnCmd(uint16_t scanInterval, uint16_t scanWindow, uint8_t filterPolicy,
                        uint8_t peerAddrType, uint8_t *pPeerAddr, uint8_t ownAddrType,
                        hciConnSpec_t *pConnSpec);
void HciLeCreateConnCancelCmd(void);
void HciLeEncryptCmd(uint8_t *pKey, uint8_t *pData);
void HciLeLtkReqNegReplCmd(uint16_t handle);
void HciLeLtkReqReplCmd(uint16_t handle, uint8_t *pKey);
void HciLeRandCmd(void);
void HciLeReadAdvTXPowerCmd(void);
void HciLeReadBufSizeCmd(void);
void HciLeReadChanMapCmd(uint16_t handle);
void HciLeReadLocalSupFeatCmd(void);
void HciLeReadRemoteFeatCmd(uint16_t handle);
void HciLeReadSupStatesCmd(void);
void HciLeReadWhiteListSizeCmd(void);
void HciLeRemoveDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr);
void HciLeSetAdvEnableCmd(uint8_t enable);
void HciLeSetAdvDataCmd(uint8_t len, uint8_t *pData);
void HciLeSetAdvParamCmd(uint16_t advIntervalMin, uint16_t advIntervalMax, uint8_t advType,
                         uint8_t ownAddrType, uint8_t directAddrType, uint8_t *pDirectAddr,
                         uint8_t advChanMap, uint8_t advFiltPolicy);
void HciLeSetEventMaskCmd(uint8_t *pLeEventMask);
void HciLeSetHostChanClassCmd(uint8_t *pChanMap);
void HciLeSetRandAddrCmd(uint8_t *pAddr);
void HciLeSetScanEnableCmd(uint8_t enable, uint8_t filterDup);
void HciLeSetScanParamCmd(uint8_t scanType, uint16_t scanInterval, uint16_t scanWindow,
                          uint8_t ownAddrType, uint8_t scanFiltPolicy);
void HciLeSetScanRespDataCmd(uint8_t len, uint8_t *pData);
void HciLeStartEncryptionCmd(uint16_t handle, uint8_t *pRand, uint16_t diversifier, uint8_t *pKey);
void HciReadBdAddrCmd(void);
void HciReadBufSizeCmd(void);
void HciReadLocalSupFeatCmd(void);
void HciReadLocalVerInfoCmd(void);
void HciReadRemoteVerInfoCmd(uint16_t handle);
void HciReadRssiCmd(uint16_t handle);
void HciReadTxPwrLvlCmd(uint16_t handle, uint8_t type);
void HciResetCmd(void);
void HciSetEventMaskCmd(uint8_t *pEventMask);
void HciVendorSpecificCmd(uint16_t opcode, uint8_t len, uint8_t *pData);

#ifdef __cplusplus
};
#endif

#endif /* HCI_API_H */
