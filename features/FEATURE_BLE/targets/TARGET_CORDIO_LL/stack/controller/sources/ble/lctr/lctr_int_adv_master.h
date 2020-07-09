/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller scanning master interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_ADV_MASTER_H
#define LCTR_INT_ADV_MASTER_H

#include "lctr_int.h"
#include "lctr_api_adv_master.h"
#include "lctr_pdu_adv.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Minimum amount of time required for scanning, to cover ADV + SCAN REQ + SCAN RSP.  */
#define LCTR_MIN_SCAN_USEC     BB_MIN_SCAN_US

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan states. */
enum
{
  LCTR_SCAN_STATE_DISABLED,             /*!< Scan disabled state. */
  LCTR_SCAN_STATE_DISCOVER,             /*!< Scan enabled state. */
  LCTR_SCAN_STATE_SHUTDOWN,             /*!< Scan shutdown in progress. */
  LCTR_SCAN_STATE_RESET,                /*!< Scan reset in progress. */
  LCTR_SCAN_STATE_TOTAL                 /*!< Total number of scan states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising report filter. */
typedef struct
{
  uint64_t      filtTbl[LL_NUM_ADV_FILT];
                                        /*!< Advertising filter table. */
  bool_t        enable;                 /*!< Enable advertising filtering. */
  bool_t        addToFiltTbl;           /*!< TRUE if duplicate is not found and is to be added to the table. */
  uint8_t       headIdx;                /*!< Head index of filter table. */
  uint32_t      validMask;              /*!< Mask for valid entries. */
} lctrAdvRptFilt_t;

/*! \brief      Master scan state context. */
typedef struct
{
  /* Scan buffer (placed here to 32-bit align) */
  uint8_t           reqBuf[BB_REQ_PDU_MAX_LEN];
                                        /*!< Scan host data buffer. */
  uint8_t           state;              /*!< Scan state. */
  wsfQueue_t        rxAdvbQ;            /*!< Receive ADVB queue. */
  wsfQueue_t        rxDirectAdvbQ;      /*!< Receive direct ADVB queue. */

  uint32_t          scanWinStartUsec;   /*!< Scan window origin in microseconds. */

  lmgrScanParam_t   scanParam;          /*!< Scan parameters. */

  union
  {
    /*! Discovery data. */
    struct
    {
      lctrAdvRptFilt_t  advFilt;        /*!< Advertising filter data. */
      uint64_t          scanReqAdvAddr; /*!< Advertiser address in the scan request. */
    } disc;

    /*! Initiate data. */
    struct
    {
      lctrConnInd_t connInd;            /*!< Connection indication. */
      uint64_t      localRpa;           /*!< Local RPA. */
      uint16_t      connHandle;         /*!< Connection handle. */
      uint16_t      connInterval;       /*!< Connection interval. */
      bool_t        connBodLoaded;      /*!< Connection BOD loaded flag. */
      uint8_t       usedChSel;          /*!< Used channel selection. */
    } init;                             /*!< Initiation specific data. */
  } data;                               /*!< Scan specific data. */

  /* BB/ISR context */
  bool_t            selfTerm;           /*!< Self-termination flag. */
  bool_t            shutdown;           /*!< Client initiated shutdown flag. */
  int8_t            advRssi;            /*!< Last received advertising or scan response RSSI. */
  BbOpDesc_t        scanBod;            /*!< Scan BOD. */
  BbBleData_t       bleData;            /*!< BLE BB operation data. */
  lctrAdvbPduHdr_t  reqPduHdr;          /*!< Request PDU header. */
  uint16_t          upperLimit;         /*!< Scan backoff upper limit. */
  uint8_t           backoffCount;       /*!< Scan backoff count. */
  uint8_t           consRspSuccess;     /*!< Number of consecutive scan response received. */
  uint8_t           consRspFailure;     /*!< Number of consecutive scan response failures. */
} lctrMstScanCtx_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrMstScanCtx_t lctrMstScan;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Builder */
void lctrMstDiscoverBuildOp(void);
void lctrMstScanCleanupOp(lctrMstScanCtx_t *pCtx);

/* Event handlers. */
void lctrMstRxAdvBPduHandler(void);
void lctrMstRxDirectAdvBPduHandler(void);

/* ISR */
void lctrMstDiscoverEndOp(BbOpDesc_t *pOp);
bool_t lctrMstDiscoverAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
void lctrMstDiscoverAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool_t lctrMstScanReqTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
bool_t lctrMstScanRspRxCompHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf);

/* Action routines */
void lctrScanActDiscover(void);
void lctrScanActShutdown(void);
void lctrScanActScanCnf(void);
void lctrScanActDisallowScan(void);
void lctrScanActSelfTerm(void);
void lctrScanActScanTerm(void);
void lctrScanActResetTerm(void);
void lctrScanActUpdateScanParam(void);
void lctrScanActUpdateScanFilt(void);
void lctrInitActInitiate(void);
void lctrInitActConnect(void);
void lctrInitActShutdown(void);

/* Helper routines */
void lctrScanCleanup(lctrMstScanCtx_t *pCtx);
void lctrScanNotifyHostInitiateError(uint8_t reason, uint8_t peerAddrType, uint64_t peerAddr);

/* Channel */
uint8_t lctrScanChanSelectInit(uint8_t chanMap);
uint8_t lctrScanChanSelectNext(uint8_t chanIdx, uint8_t chanMap);

/* Advertising report filtering */
void lctrAdvRptEnable(lctrAdvRptFilt_t *pAdvFilt, bool_t filtEna);
void lctrAdvRptGenerateLegacyHash(uint64_t *pHash, uint8_t addrType, uint64_t addr, uint8_t eventType);
void lctrAdvRptGenerateExtHash(uint64_t *pHash, uint8_t addrType, uint64_t addr, uint8_t eventType,
                               uint8_t sid, uint16_t did);
bool_t lctrAdvRptCheckDuplicate(lctrAdvRptFilt_t *pAdvFilt, uint64_t hash);
void lctrAdvRptAddEntry(lctrAdvRptFilt_t *pAdvFilt, uint64_t hash);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_MASTER_H */
