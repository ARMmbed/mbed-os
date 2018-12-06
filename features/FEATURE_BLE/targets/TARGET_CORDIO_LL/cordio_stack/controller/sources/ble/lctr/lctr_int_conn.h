/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connection interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_CONN_H
#define LCTR_INT_CONN_H

#include "lctr_int.h"
#include "lctr_api_conn.h"
#include "lctr_pdu_conn.h"
#include "lctr_pdu_adv.h"
#include "lmgr_api_conn.h"
#include "ll_defs.h"
#include "sch_api.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "wsf_cs.h"
#include "wsf_msg.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LCTR_MAX_CONS_CRC           2   /*!< Maximum number of consecutive CRC failures. */

#ifndef LCTR_DATA_PDU_START_OFFSET
#define LCTR_DATA_PDU_START_OFFSET  2   /*!< Data PDU start offset in a buffer (match ACL header size). */
#endif

#define LCTR_DATA_PDU_FC_OFFSET     0   /*!< Flow control fields data PDU offset. */
#define LCTR_DATA_PDU_LEN_OFFSET    1   /*!< Length field data PDU offset. */

#define LCTR_MAX_INSTANT            32767   /*!< Maximum instant value for connection update. */

#define LCTR_CTRL_DATA_HANDLE       0xFF    /*!< Connection handle used for LL control PDUs. */

#define LCTR_LE_PING_ATTEMPTS       4   /*!< LE Ping attempts. */

#define LCTR_CH_SEL_MAX             2   /*!< Total number of channel selection algorithms. */

/*! \brief      Resolve the connection handle from the context pointer. */
#define LCTR_GET_CONN_HANDLE(pCtx)  (pCtx - pLctrConnTbl)

/*! \brief      Resolve connection context from the handle. */
#define LCTR_GET_CONN_CTX(h)        &(pLctrConnTbl[h])

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Connected states. */
enum
{
  LCTR_CONN_STATE_INITIALIZED,          /*!< Connection initialized. */
  LCTR_CONN_STATE_ESTABLISHED_STARTUP,  /*!< Connection established before ready to start LLCP. */
  LCTR_CONN_STATE_ESTABLISHED_READY,    /*!< Connection established and ready to start LLCP. */
  LCTR_CONN_STATE_TERMINATING           /*!< Connection terminating. */
};

/*! \brief      LLCP states. */
enum
{
  LCTR_LLCP_STATE_IDLE,                 /*!< LLCP idle state. */
  LCTR_LLCP_STATE_BUSY,                 /*!< LLCP busy state. */
  LCTR_LLCP_STATE_TOTAL                 /*!< Total number of LLCP states. */
};

/*! \brief      Common LLCP states. */
enum
{
  LCTR_CMN_STATE_IDLE,                 /*!< LLCP idle state. */
  LCTR_CMN_STATE_BUSY,                 /*!< LLCP busy state. */
  LCTR_CMN_STATE_TOTAL                 /*!< Total number of LLCP states. */
};

/*! \brief      Common LLCP procedure IDs. */
enum
{
  /* Common SM LLCP procedures. */
  LCTR_PROC_CMN_TERM,                   /*!< Termination procedure. */
  LCTR_PROC_CMN_CH_MAP_UPD,             /*!< Channel map update procedure. */
  LCTR_PROC_CMN_FEAT_EXCH,              /*!< Feature exchange procedure. */
  LCTR_PROC_CMN_VER_EXCH,               /*!< Version exchange procedure. */
  LCTR_PROC_CMN_DATA_LEN_UPD,           /*!< Data length update procedure. */
  LCTR_PROC_CMN_SET_MIN_USED_CHAN,      /*!< Set minimum number of used channels procedure. */
  LCTR_PROC_CMN_TOTAL,                  /*!< Total number of common procedures. */

  /* Custom SM LLCP procedures. */
  LCTR_PROC_CONN_UPD,                   /*!< Connection update procedure. */
  LCTR_PROC_CONN_PARAM,                 /*!< Connection parameter procedure. */
  LCTR_PROC_ENCRYPT,                    /*!< Encryption Start or Restart procedure. */
  LCTR_PROC_LE_PING,                    /*!< LE Ping procedure. */
  LCTR_PROC_PHY_UPD,                    /*!< PHY update procedure. */
  LCTR_PROC_PHY_UPD_PEER,               /*!< Peer-initiated PHY update procedure. */

  LCTR_PROC_INVALID = 0xFF              /*!< Invalid ID. */

  /* Note: additional procedures without instants can be overridden. */
};

/*! \brief      Connection context. */
typedef struct
{
  bool_t            enabled;            /*!< Context enabled. */
  uint8_t           state;              /*!< Current state. */
  uint8_t           role;               /*!< Connection role. */
  uint32_t          opModeFlags;        /*!< Operational mode flags. */

  /* Host/API */
  uint8_t           termReason;         /*!< Termination reason code. */
  uint8_t           replyWaitingMsk;    /*!< Host reply waiting mask. */

  /* BB data */
  BbOpDesc_t        connBod;            /*!< Connection BOD. */
  BbBleData_t       bleData;            /*!< BLE BB operation data. */

  union
  {
    struct
    {
      uint16_t      totalAcc;           /*!< Combined sleep clock inaccuracy. */
      uint16_t      lastActiveEvent;    /*!< Last active event counter. */
      uint32_t      txWinSizeUsec;      /*!< Tx window size. */
      uint32_t      anchorPoint;        /*!< Anchor point. */
      uint32_t      unsyncedTime;       /*!< Unsynced time in BB tick before connection update. */
      bool_t        initAckRcvd;        /*!< Ack received from master. */
      bool_t        abortSlvLatency;    /*!< If TRUE abort slave latency. */
    } slv;                              /*!< Slave connection specific data. */

    struct
    {
      bool_t        sendConnUpdInd;     /*!< Send LL_CONNECTION_UPDATE_IND flag. */
    } mst;                              /*!< Master connection specific data. */
  } data;                               /*!< Role specific data. */

  /* Connection event parameters */
  uint16_t          maxLatency;         /*!< Maximum latency. */
  uint16_t          eventCounter;       /*!< Connection event counter. */
  uint16_t          connInterval;       /*!< Connection interval. */

  /* RF parameters */
  int8_t            rssi;               /*!< RSSI. */

  /* Channel parameters */
  uint8_t           lastChanIdx;        /*!< Current channel index. */
  uint8_t           hopInc;             /*!< Hop increment. */
  uint8_t           numUsedChan;        /*!< Number of used channels. */
  uint64_t          chanMask;           /*!< Channel mask. */
  uint8_t           chanRemapTbl[LL_CHAN_DATA_MAX_IDX + 1];
                                        /*!< Channel remapping table. */
  uint8_t           usedChSel;          /*!< Used channel selection. */
  uint16_t          chIdentifier;       /*!< Channel identifier. */

  /* Flow control */
  lctrDataPduHdr_t  txHdr;              /*!< Transmit data PDU header. */
  lctrDataPduHdr_t  rxHdr;              /*!< Receive data PDU header. */
  wsfQueue_t        txLeuQ;             /*!< Transmit LE-U queue. */
  wsfQueue_t        txArqQ;             /*!< Transmit ARQ queue. */
  wsfQueue_t        rxDataQ;            /*!< Receive data pending queue. */
  uint8_t           numTxComp;          /*!< Number of completed Tx buffers. */
  uint8_t           numRxPend;          /*!< Number of Rx pending buffers. */
  bool_t            emptyPduPend;       /*!< Empty PDU ACK pending. */
  bool_t            emptyPduFirstAtt;   /*!< Empty PDU first attempt. */

  /* Supervision */
  uint16_t          supTimeoutMs;       /*!< Supervision timeout in milliseconds. */
  wsfTimer_t        tmrSupTimeout;      /*!< Supervision timer. */
  bool_t            connEst;            /*!< Connection established. */

  /* Encryption */
  bool_t            pauseRxData;        /*!< Pause Rx data PDUs. */
  bool_t            pauseTxData;        /*!< Pause Tx data PDUs. */
  uint8_t           ltk[LL_KEY_LEN];    /*!< Long term key. */
  uint8_t           skd[LL_SKD_LEN];    /*!< Session key diversifier. */
  uint8_t           iv[LL_IV_LEN];      /*!< Initialization vector. */
  uint8_t           rand[LL_RAND_LEN];  /*!< Random number. */
  uint16_t          ediv;               /*!< Encrypted diversifier. */

  uint64_t          txPktCounter;       /*!< Transmit packet counter. */
  uint64_t          rxPktCounter;       /*!< Transmit packet counter. */

  /* Authentication */
  uint32_t          pingPeriodMs;       /*!< Ping period in milliseconds. */
  wsfTimer_t        tmrPingTimeout;     /*!< Ping timer. */
  uint32_t          authTimeoutMs;      /*!< Authentication payload timeout in milliseconds. */
  wsfTimer_t        tmrAuthTimeout;     /*!< Authentication payload timer. */

  LlEncMode_t       newEncMode;         /*!< Pending encryption mode. */
  bool_t            pendEncMode;        /*!< New encryption mode pending. */

  bool_t            keyUpdFlag;         /*!< Flag for key update. */

  /* Peer device info */
  uint8_t           numSentVerInd;      /*!< Number of sent LL_VERSION_IND. */
  bool_t            remoteVerValid;     /*!< Peer version data valid. */
  lctrVerInd_t      remoteVer;          /*!< Peer version data. */
  bool_t            featExchFlag;       /*!< Flag for completed feature exchange. */
  uint64_t          usedFeatSet;        /*!< Used feature set. */

  /* Data length */
  lctrDataLen_t     localDataPdu;       /*!< Local Data PDU parameters. */
  lctrDataLen_t     effDataPdu;         /*!< Effective Data PDU parameters. */
  uint16_t          localConnDurUsec;   /*!< Local connection event duration. */
  uint16_t          effConnDurUsec;     /*!< Effective connection event duration. */

  /* PHY */
  uint8_t           allPhys;            /*!< Default all PHYs. */
  uint8_t           txPhys;             /*!< Default transmitter PHYs. */
  uint8_t           rxPhys;             /*!< Default receiver PHYs. */
  uint8_t           txPhysPending;      /*!< Pending transmitter PHYs. */

  /* Peer minimum number of used channels */
  uint8_t           peerMinUsedChan[LL_MAX_PHYS];
                                        /*!< Peer minimum number of used channels for PHYs. */

  /* LLCP */
  uint8_t           llcpState;          /*!< Current LLCP state. */
  uint8_t           encState;           /*!< Current encryption state. */
  uint8_t           pingState;          /*!< Current ping state. */
  uint8_t           connUpdState;       /*!< Connection update state. */
  uint8_t           phyUpdState;        /*!< PHY update state. */
  uint8_t           cmnState;           /*!< Common LLCP state. */
  bool_t            peerReplyWaiting;   /*!< Peer waiting for reply. */
  bool_t            llcpInstantComp;    /*!< Procedure instant completed. */
  bool_t            termAckReqd;        /*!< Ack required for Rx'ed LL_TERMINATE_IND. */
  bool_t            ackAfterCtrlPdu;    /*!< Ack Tx'ed after last Control PDU Rx'ed. */
  bool_t            llcpIsOverridden;   /*!< Is the current procedure overridden by other procedure. */
  bool_t            isSlvReadySent;     /*!< True if slave ready to initiate startup LLCP procedure is sent. */
  bool_t            isFirstNonCtrlPdu;  /*!< True if first non-control PDU from master and slave. */
  bool_t            isSlvPhyUpdInstant; /*!< True if slave is in PHY update instant state. */
  uint8_t           llcpActiveProc;     /*!< Current procedure. */
  uint16_t          llcpNotifyMask;     /*!< Host notification mask. */
  uint16_t          llcpPendMask;       /*!< Pending LLCP procedures. */
  uint16_t          llcpIncompMask;     /*!< Incomplete LLCP procedures. */
  LlConnSpec_t      connUpdSpec;        /*!< Host connection update specification. */
  lctrConnUpdInd_t  connUpd;            /*!< Connection update parameters. */
  lctrConnParam_t   connParam;          /*!< Stored peer connection parameter request or response. */
  lctrChanMapInd_t  chanMapUpd;         /*!< Channel map parameters. */
  lctrPhy_t         phyReq;             /*!< Stored peer PHY request. */
  lctrPhyUpdInd_t   phyUpd;             /*!< PHY update parameters. */
  wsfTimer_t        tmrProcRsp;         /*!< Procedure response timer. */
} lctrConnCtx_t;

/*! \brief      Call signature of a cipher block handler. */
typedef void (*lctrCipherBlkHdlr_t)(BbBleEnc_t *pEnc, uint8_t id, uint8_t dir);

/*! \brief      Call signature of a packet encryption handler. */
typedef bool_t (*lctrPktEncHdlr_t)(BbBleEnc_t *pEnc, uint8_t *pHdr, uint8_t *pBuf, uint8_t *pMic);

/*! \brief      Call signature of a packet decryption handler. */
typedef bool_t (*lctrPktDecHdlr_t)(BbBleEnc_t *pEnc, uint8_t *pBuf);

/*! \brief      Call signature of a set packet count handler. */
typedef void (*lctrPktCntHdlr_t)(BbBleEnc_t *pEnc, uint64_t pktCnt);

/*! \brief      Call signature of a LLCP state machine handler. */
typedef bool_t (*LctrLlcpHdlr_t)(lctrConnCtx_t *pCtx, uint8_t event);

/*! \brief      Call signature of a control PDU handler. */
typedef void (*lctrCtrlPduHdlr_t)(lctrConnCtx_t *pCtx, uint8_t *pBuf);

/*! \brief      Call signature of a Channel state machine handler. */
typedef uint8_t (*LctrChSelHdlr_t)(lctrConnCtx_t *pCtx, uint16_t numSkip);

/*! \brief      LLCP state machine handlers. */
enum
{
  LCTR_LLCP_SM_ENCRYPT,                 /*!< Encryption LLCP state machine. */
  LCTR_LLCP_SM_PING,                    /*!< Ping state machine. */
  LCTR_LLCP_SM_CONN_UPD,                /*!< Connection update state machine. */
  LCTR_LLCP_SM_PHY_UPD,                 /*!< PHY update state machine. */
  LCTR_LLCP_SM_CMN,                     /*!< Common LLCP state machine. */
  LCTR_LLCP_SM_TOTAL                    /*!< Total number of LLCP state machine. */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lctrCipherBlkHdlr_t lctrInitCipherBlkHdlr;
extern lctrPktEncHdlr_t lctrPktEncryptHdlr;
extern lctrPktDecHdlr_t lctrPktDecryptHdlr;
extern lctrPktCntHdlr_t lctrSetEncryptPktCountHdlr;
extern lctrPktCntHdlr_t lctrSetDecryptPktCountHdlr;
extern lctrConnCtx_t *pLctrConnTbl;
extern lctrDataPdu_t lctrDataPdu;
extern lctrConnMsg_t *pLctrConnMsg;
extern const LctrVsHandlers_t *pLctrVsHdlrs;
extern lctrCtrlPduHdlr_t lctrCtrlPduHdlr;
extern LctrChSelHdlr_t lctrChSelHdlr[LCTR_CH_SEL_MAX];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void lctrConnDefaults(void);

/* Builder */
lctrConnCtx_t *lctrAllocConnCtx(void);
void lctrFreeConnCtx(lctrConnCtx_t *pCtx);
uint8_t lctrSelectNextDataChannel(lctrConnCtx_t *pCtx, uint16_t numSkip);
uint8_t lctrSelectNextChannel2(lctrConnCtx_t *pCtx, uint16_t numSkip);
void lctrBuildRemapTable(lctrConnCtx_t *pCtx);

/* Task event handler */
void lctrConnTxCompletedHandler(void);
void lctrConnRxPendingHandler(void);

/* Connection update actions */
void lctrStoreConnUpdateSpec(lctrConnCtx_t *pCtx);
void lctrSendConnUpdateInd(lctrConnCtx_t *pCtx);
void lctrStoreConnUpdate(lctrConnCtx_t *pCtx);
void lctrNotifyHostConnUpdateInd(lctrConnCtx_t *pCtx, uint8_t status);
void lctrActNotifyHostConnUpdSuccess(lctrConnCtx_t *pCtx);
void lctrActHostNegReply(lctrConnCtx_t *pCtx);
void lctrActPeerRejectConnParam(lctrConnCtx_t *pCtx);

/* Channel map actions */
void lctrStoreChanMapUpdate(lctrConnCtx_t *pCtx);
void lctrSendChanMapUpdateInd(lctrConnCtx_t *pCtx);
void lctrStoreChanMap(lctrConnCtx_t *pCtx);

/* Feature exchange actions */
void lctrSendFeatureReq(lctrConnCtx_t *pCtx);
void lctrSendFeatureRsp(lctrConnCtx_t *pCtx);
void lctrStoreUsedFeatures(lctrConnCtx_t *pCtx);

/* Version exchange actions */
void lctrSendVersionInd(lctrConnCtx_t *pCtx);
void lctrStoreRemoteVer(lctrConnCtx_t *pCtx);
void lctrNotifyHostReadRemoteVerCnf(lctrConnCtx_t *pCtx);

/* Terminate actions */
void lctrSendTerminateInd(lctrConnCtx_t *pCtx);
void lctrNotifyHostDisconnectInd(lctrConnCtx_t *pCtx);
void lctrStoreTerminateReason(lctrConnCtx_t *pCtx);
void lctrStoreDisconnectReason(lctrConnCtx_t *pCtx);
void lctrStoreConnFailEstablishTerminateReason(lctrConnCtx_t *pCtx);
void lctrStoreLlcpTimeoutTerminateReason(lctrConnCtx_t *pCtx);
void lctrStoreResetTerminateReason(lctrConnCtx_t *pCtx);
void lctrStoreInstantPassedTerminateReason(lctrConnCtx_t *pCtx);
void lctrStoreMicFailedTerminateReason(lctrConnCtx_t *pCtx);

/* Connection parameter actions */
void lctrStoreConnParamReq(lctrConnCtx_t *pCtx);
void lctrStoreConnParamRsp(lctrConnCtx_t *pCtx);
void lctrStoreConnParamSpec(lctrConnCtx_t *pCtx);
void lctrSendConnParamReq(lctrConnCtx_t *pCtx);
void lctrSendConnParamRsp(lctrConnCtx_t *pCtx);
void lctrNotifyHostConnParamInd(lctrConnCtx_t *pCtx);

/* Data length exchange actions */
void lctrStoreLocalDataLength(lctrConnCtx_t *pCtx);
void lctrSendDataLengthReq(lctrConnCtx_t *pCtx);
void lctrSendDataLengthRsp(lctrConnCtx_t *pCtx);
void lctrStoreRemoteDataLength(lctrConnCtx_t *pCtx);
void lctrNotifyHostDataLengthInd(lctrConnCtx_t *pCtx, uint8_t status);

/* Set minimum number of used channels actions */
void lctrSendSetMinUsedChanInd(lctrConnCtx_t *pCtx);
void lctrStoreSetMinUsedChan(lctrConnCtx_t *pCtx);

/* Unknown/Unsupported */
void lctrSendUnknownRsp(lctrConnCtx_t *pCtx);
void lctrSendRejectInd(lctrConnCtx_t *pCtx, uint8_t reason, bool_t forceRejectExtInd);

/* Tx data path */
uint16_t lctrTxFragLen(lctrConnCtx_t *pTx);
uint16_t lctrTxInitMem(uint8_t *pFreeMem, uint32_t freeMemSize);
uint8_t *lctrTxCtrlPduAlloc(uint8_t pduLen);
void lctrTxDataPduQueue(lctrConnCtx_t *pCtx, uint16_t fragLen, lctrAclHdr_t *pAclHdr, uint8_t *pAclBuf);
void lctrTxCtrlPduQueue(lctrConnCtx_t *pCtx, uint8_t *pBuf);
uint8_t lctrTxQueuePeek(lctrConnCtx_t *pCtx, BbBleDrvTxBufDesc_t *bbDescs, bool_t *pMd);
bool_t lctrTxQueuePop(lctrConnCtx_t *pCtx);
void lctrTxQueuePopCleanup(lctrConnCtx_t *pCtx);
uint8_t lctrTxQueueClear(lctrConnCtx_t *pCtx);

/* Rx data path */
uint8_t *lctrRxPduAlloc(uint16_t maxRxLen);
void lctrRxPduFree(uint8_t *pBuf);
void lctrRxEnq(uint8_t *pBuf, uint16_t eventCounter, uint16_t connHandle);
uint8_t *lctrRxDeq(uint16_t *pConnHandle);
void lctrRxConnEnq(lctrConnCtx_t *pCtx, uint8_t *pBuf);
uint8_t *lctrRxConnDeqAcl(lctrConnCtx_t *pCtx);
uint8_t lctrRxConnClear(lctrConnCtx_t *pCtx);

/* LLCP */
void lctrStartLlcpTimer(lctrConnCtx_t *pCtx);
void lctrStopLlcpTimer(lctrConnCtx_t *pCtx);
void lctrStartPendingLlcp(lctrConnCtx_t *pCtx);

/* Data path pause */
void lctrPauseTxData(lctrConnCtx_t *pCtx);
void lctrUnpauseTxData(lctrConnCtx_t *pCtx);
void lctrCheckPauseComplete(lctrConnCtx_t *pCtx);
void lctrPauseRxData(lctrConnCtx_t *pCtx);
void lctrUnpauseRxData(lctrConnCtx_t *pCtx);

/* Packet times */
void lctrSetPacketTimeRestriction(lctrConnCtx_t *pCtx, uint8_t txPhys);
void lctrRemovePacketTimeRestriction(lctrConnCtx_t *pCtx);
uint16_t lctrCalcConnDurationUsec(lctrConnCtx_t *pCtx, const lctrDataLen_t *pDataLen);

/* Scheduler */
BbOpDesc_t *lctrConnResolveConflict(BbOpDesc_t *pNewOp, BbOpDesc_t *pExistOp);

/* ISR */
uint8_t *lctrProcessRxAck(lctrConnCtx_t *pCtx);
void lctrTxPduAck(lctrConnCtx_t *pCtx);
bool_t lctrProcessTxAck(lctrConnCtx_t *pCtx);
void lctrProcessTxAckCleanup(lctrConnCtx_t *pCtx);
uint16_t lctrSetupForTx(lctrConnCtx_t *pCtx, uint8_t rxStatus, bool_t reqTx);
void lctrRxPostProcessing(lctrConnCtx_t *pCtx, uint8_t *pRxBuf, uint8_t *pNextRxBuf, bool_t loadRxBuf);

/* Helper */
void lctrSendConnMsg(lctrConnCtx_t *pCtx, uint8_t event);
bool_t lctrExceededMaxDur(lctrConnCtx_t *pCtx, uint32_t ceStart, uint32_t pendDurUsec);
uint32_t lctrCalcPingPeriodMs(lctrConnCtx_t *pCtx, uint32_t authTimeoutMs);
uint8_t lctrComputeSca(void);

/* Reservation */
void lctrGetConnOffsets(uint32_t rsvnOffs[], uint32_t refTime);

/*************************************************************************************************/
/*!
 *  \brief  Check for a queue depth of 1 element.
 *
 *  \param  pArqQ       Queue.
 *
 *  \return TRUE if ARQ only has 1 element, FALSE otherwise.
 *
 *  \note   Checks without resource protection. This routine is only intended to be used in task
 *          context.
 */
/*************************************************************************************************/
static inline bool_t lctrIsQueueDepthOne(wsfQueue_t *pArqQ)
{
  return pArqQ->pHead == pArqQ->pTail;
}

/*************************************************************************************************/
/*!
 *  \brief  Set flags for link termination.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrFlagLinkTerm(lctrConnCtx_t *pCtx)
{
  bool_t removeBod = (pCtx->state == LCTR_CONN_STATE_ESTABLISHED_READY);

  pCtx->state = LCTR_CONN_STATE_TERMINATING;    /* signals ISR to terminate link */
  pCtx->llcpState = LCTR_LLCP_STATE_IDLE;       /* signals ISR do not wait to send TERMINATE_IND */

  if (removeBod)
  {
    SchRemove(&pCtx->connBod);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Service the Control PDU ACK state after a successful reception.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrCheckControlPduAck(lctrConnCtx_t *pCtx)
{
  if (pCtx->rxHdr.llid == LL_LLID_CTRL_PDU)
  {
    /* Control PDU received; reset Tx ACK pending state. */
    pCtx->ackAfterCtrlPdu = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Service the Control PDU ACK state after a successful transmission.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrSetControlPduAck(lctrConnCtx_t *pCtx)
{
  /* Unconditionally set state upon Tx completion. */
  pCtx->ackAfterCtrlPdu = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Set flags for link termination.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return TRUE if connection is terminated, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrCheckForLinkTerm(lctrConnCtx_t *pCtx)
{
  if (pCtx->state == LCTR_CONN_STATE_TERMINATING)
  {
    /* Peer device is LL_TERMINATE_IND initiator. */
    if (pCtx->termAckReqd)                  /* Tx Ack required after Rx of LL_TERMINATE_IND */
    {
      if (pCtx->ackAfterCtrlPdu)            /*     guarantee Ack Tx'ed */
      {
        return TRUE;
      }
    }
    /* Local device is LL_TERMINATE_IND initiator. */
    else if ((pCtx->llcpState == LCTR_LLCP_STATE_IDLE) || /* LL_TERMINATE_IND not pending */
             (pCtx->txArqQ.pHead == NULL))                /* guarantee LL_TERMINATE_IND is Ack'ed */
                                                          /*     i.e. "WsfQueueEmpty(&pCtx->txArqQ)" (optimized for ISR) */
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Increment the Tx/encrypt packet counter.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrIncPacketCounterTx(lctrConnCtx_t *pCtx)
{
  if (lctrSetEncryptPktCountHdlr)
  {
    BbBleEnc_t * const pEnc = &pCtx->bleData.chan.enc;

    if ((pEnc->enaEncrypt) &&
        (pEnc->nonceMode == BB_NONCE_MODE_PKT_CNTR))
    {
      pCtx->txPktCounter++;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Increment the Rx/decrypt packet counter.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrIncPacketCounterRx(lctrConnCtx_t *pCtx)
{
  if (lctrSetEncryptPktCountHdlr)
  {
    BbBleEnc_t * const pEnc = &pCtx->bleData.chan.enc;

    if ((pEnc->enaEncrypt) &&
        (pEnc->nonceMode == BB_NONCE_MODE_PKT_CNTR))
    {
      pCtx->rxPktCounter++;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the transmit packet counter value in the BB.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrSetBbPacketCounterTx(lctrConnCtx_t *pCtx)
{
  if (lctrSetEncryptPktCountHdlr)
  {
    BbBleEnc_t * const pEnc = &pCtx->bleData.chan.enc;

    if (!pEnc->enaEncrypt)
    {
      return;
    }

    switch (pEnc->nonceMode)
    {
      case BB_NONCE_MODE_PKT_CNTR:
        lctrSetEncryptPktCountHdlr(pEnc, pCtx->txPktCounter);
        break;
      case BB_NONCE_MODE_EVT_CNTR:
        lctrSetEncryptPktCountHdlr(pEnc, pCtx->eventCounter);
        break;
      default:
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the receive packet counter value in the BB.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrSetBbPacketCounterRx(lctrConnCtx_t *pCtx)
{
  if (lctrSetDecryptPktCountHdlr)
  {
    BbBleEnc_t * const pEnc = &pCtx->bleData.chan.enc;

    if (!pEnc->enaDecrypt)
    {
      return;
    }

    switch (pEnc->nonceMode)
    {
      case BB_NONCE_MODE_PKT_CNTR:
        lctrSetDecryptPktCountHdlr(pEnc, pCtx->rxPktCounter);
        break;
      case BB_NONCE_MODE_EVT_CNTR:
        lctrSetDecryptPktCountHdlr(pEnc, pCtx->eventCounter);
        break;
      default:
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Check if procedure is active, if not pend the procedure.
 *
 *  \param  pCtx        Connection context.
 *  \param  proc        Procedure ID to check/pend.
 *
 *  \return TRUE if active procedure, FALSE if pended.
 */
/*************************************************************************************************/
static inline bool_t lctrCheckActiveOrPend(lctrConnCtx_t *pCtx, uint8_t proc)
{
  if (pCtx->llcpActiveProc == proc)
  {
    return TRUE;
  }
  else
  {
    pCtx->llcpPendMask |= 1 << proc;
    return FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection timeout termination reason.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void lctrStoreConnTimeoutTerminateReason(lctrConnCtx_t *pCtx)
{
  if (pCtx->termReason == LL_ERROR_CODE_CONN_FAILED_TO_ESTABLISH)
  {
    pCtx->termReason = LL_ERROR_CODE_CONN_TIMEOUT;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Increment available Tx data buffers.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrDataTxIncAvailBuf(void)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrConnCb.availTxBuf++;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief  Decrement available Tx data buffers.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrDataTxDecAvailBuf(void)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrConnCb.availTxBuf--;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief  Increment available Rx data buffers.
*
 *  \param  numBufs     Number of completed packets.
  *
 *  \return None.
 */
/*************************************************************************************************/
static inline void lctrDataRxIncAvailBuf(uint8_t numBufs)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrConnCb.availRxBuf += numBufs;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief      Get connection operational mode flags.
 *
 *  \param      pCtx    Connection context to get the flag.
 *  \param      flag    Flags.
 *
 *  \return     Status error code.
 *
 *  Set mode flags governing LL operations of a given connection.
 */
/*************************************************************************************************/
static inline bool_t lctrGetConnOpFlag(lctrConnCtx_t *pCtx, uint32_t flag)
{
  return (pCtx->opModeFlags & flag) ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Convert PHYS bit to PHY.
 *
 *  \param  physBit     PHYS bit.
 *
 *  \return PHY.
 */
/*************************************************************************************************/
static inline uint8_t lctrPhysBitToPhy(uint8_t physBit)
{
  switch (physBit)
  {
    default:
    case LL_PHYS_LE_1M_BIT:
      return BB_PHY_BLE_1M;
    case LL_PHYS_LE_2M_BIT:
      return BB_PHY_BLE_2M;
    case LL_PHYS_LE_CODED_BIT:
      return BB_PHY_BLE_CODED;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Convert PHY to PHYS bit.
 *
 *  \param  phy         PHY.
 *
 *  \return PHYS bit.
 */
/*************************************************************************************************/
static inline uint8_t lctrPhyToPhysBit(uint8_t phy)
{
  switch (phy)
  {
    default:
    case BB_PHY_BLE_1M:
      return LL_PHYS_LE_1M_BIT;
    case BB_PHY_BLE_2M:
      return LL_PHYS_LE_2M_BIT;
    case BB_PHY_BLE_CODED:
      return LL_PHYS_LE_CODED_BIT;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get LLCP procedure ID.
 *
 *  \param      event   Event.
 *
 *  \return     LLCP procedure event ID.
 */
/*************************************************************************************************/
static inline uint8_t lctrGetProcId(uint8_t event)
{
  switch (event)
  {
    case LCTR_CONN_MSG_API_CHAN_MAP_UPDATE:
      return LCTR_PROC_CMN_CH_MAP_UPD;

    case LCTR_CONN_MSG_API_REMOTE_FEATURE:
      return LCTR_PROC_CMN_FEAT_EXCH;

    case LCTR_CONN_MSG_API_SET_MIN_USED_CHAN:
      return LCTR_PROC_CMN_SET_MIN_USED_CHAN;

    case LCTR_CONN_MSG_API_REMOTE_VERSION:
      return LCTR_PROC_CMN_VER_EXCH;

    case LCTR_CONN_MSG_API_DISCONNECT:
      return LCTR_PROC_CMN_TERM;

    case LCTR_CONN_MSG_API_DATA_LEN_CHANGE:
      return LCTR_PROC_CMN_DATA_LEN_UPD;

    case LCTR_CONN_MSG_API_CONN_UPDATE:
      return LCTR_PROC_CONN_UPD;

    case LCTR_CONN_MSG_API_PHY_UPDATE:
      return LCTR_PROC_PHY_UPD;

    case LCTR_CONN_MSG_API_START_ENC:
      return LCTR_PROC_ENCRYPT;

    default:
      return LCTR_PROC_INVALID;
  }
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CONN_H */
