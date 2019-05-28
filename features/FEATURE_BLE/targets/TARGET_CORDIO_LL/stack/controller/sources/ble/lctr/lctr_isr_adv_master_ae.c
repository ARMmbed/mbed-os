/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer controller master advertising event ISR callbacks.
 */
/*************************************************************************************************/

#include "lctr_int_adv_master_ae.h"
#include "lctr_int_conn_master.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "bb_ble_api_reslist.h"
#include "bb_ble_api_whitelist.h"
#include "bb_ble_api_periodiclist.h"
#include "lmgr_api_adv_master_ae.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Data Status field number of bit shift. */
#define LCTR_DATA_STATUS_SHIFT      5

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Transitive context (valid only for a single Advertising Event). */
struct
{
  uint8_t           advPduType;             /*!< Primary channel advertising PDU type. */
  bool_t            filtResult;             /*!< PDU filter result. */
  bool_t            skipFiltAnonymous;      /*!< If TRUE, skip PDU filtering due to anonymous advertising. */
  const uint8_t     *pAdvData;              /*!< Point to the advertising data payload. */
  uint8_t           advDataLen;             /*!< Advertising data length. */
  bool_t            isHdrParsed;            /*!< TRUE if header is received and parsed. */
  uint8_t           extAdvHdrFlags;         /*!< Extended advertising header flag. */
  uint8_t           extAdvHdrLen;           /*!< Extended advertising header length. */
  lctrAdvbPduHdr_t  advHdr;                 /*!< Advertising header. */

  /* Used for non-connectable non-scannable with auxiliary packet. */
  uint64_t          advA;                   /*!< Advertising address. */
  uint64_t          tgtA;                   /*!< Target address. */
  bool_t            advAReceived;           /*!< TRUE if advertising address is received. */
  bool_t            advARand;               /*!< TRUE if advertising address is random. */
  bool_t            tgtAReceived;           /*!< TRUE if target address is received. */
  bool_t            tgtARand;               /*!< TRUE if target address is random. */

  /* Chain state. */
  uint8_t           chainAdvMode;           /*!< AdvMode field for chain reports. */
  uint16_t          chainExtraEventType;    /*!< Extra event type flags for chain reports. */
} lctrMstExtScanIsr;

/*! \brief      Transitive context (valid only for a single Periodic Advertising Event). */
struct
{
  bool_t    filtResult;             /*!< PDU filter result, filter out if TRUE, FAlSE otherwise. */
  bool_t    syncWithSlave;          /*!< Flag indicating synchronize packet received from slave. */
  uint32_t  firstRxStartTs;         /*!< Timestamp of the first received frame regardless of CRC error. */
} lctrMstPerScanIsr;

/**************************************************************************************************
  Functions: Utility functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Get the local ID address.
 *
 *  \param      pExtScanCtx         Scan context.
 *  \param      targetAddr          Received target address.
 *  \param      pLocalIdAddr        Storage for local ID address.
 *  \param      pLocalIdAddrType    Storage for local ID address type;
 *  \param      peerIdAddr          Peer ID address.
 *  \param      peerIdAddrType      Peer ID address type.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void lctrGetLocalIdAddr(lctrExtScanCtx_t *pExtScanCtx, uint64_t targetAddr,
                                      uint64_t *pLocalIdAddr, uint8_t *pLocalIdAddrType,
                                      uint64_t peerIdAddr, uint8_t peerIdAddrType)
{
  if (BDA64_ADDR_IS_RPA(targetAddr))
  {
    /* Identity address is either public or random in the resolving list. */
    peerIdAddrType &= ~(1 << 1);

    if (BbBleResListIsLocalResolved(targetAddr, peerIdAddrType, peerIdAddr))
    {
      if (pExtScanCtx->scanParam.ownAddrType == LL_ADDR_RANDOM_IDENTITY)
      {
        *pLocalIdAddrType = LL_ADDR_RANDOM_IDENTITY;
        *pLocalIdAddr = lmgrCb.bdAddrRnd;
      }
      else
      {
        *pLocalIdAddrType = LL_ADDR_PUBLIC_IDENTITY;
        *pLocalIdAddr = lmgrPersistCb.bdAddr;
      }
    }
    else
    {
      *pLocalIdAddrType = LL_ADDR_RANDOM_UNRESOLVABLE;
      *pLocalIdAddr = targetAddr;
    }
  }
  else
  {
    if (pExtScanCtx->scanParam.ownAddrType == LL_ADDR_RANDOM)
    {
      *pLocalIdAddrType = LL_ADDR_RANDOM;
      *pLocalIdAddr = lmgrCb.bdAddrRnd;
    }
    else
    {
      *pLocalIdAddrType = LL_ADDR_PUBLIC;
      *pLocalIdAddr = lmgrPersistCb.bdAddr;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Master Acad handler.
 *
 *  \param      pPerScanCtx   Periodic scan context.
 *
 *  \return     None
 */
/*************************************************************************************************/
void lctrMstAcadHandler(lctrPerScanCtx_t * const pPerScanCtx)
{
  if (pPerScanCtx->extAdvHdr.acadLen == 0)
  {
    return;
  }

  /* Enable any new Acad if necessary */
  uint8_t len = pPerScanCtx->extAdvHdr.acadLen;
  uint8_t *pBuf = (uint8_t *) pPerScanCtx->extAdvHdr.pAcad;
  while(len > 0)
  {
    uint8_t acadLen = 0;
    BSTREAM_TO_UINT8(acadLen, pBuf);

    uint8_t opcode = 0;
    BSTREAM_TO_UINT8(opcode, pBuf);

    switch (opcode) {
      case LL_ACAD_OPCODE_CHANNEL_MAP_UPDATE:
      {
        lctrAcadParam_t *pAcadParam = &pPerScanCtx->acadParams[LCTR_ACAD_ID_CHAN_MAP_UPDATE];

        if (pAcadParam->hdr.state == LCTR_ACAD_STATE_DISABLED)
        {
          pAcadParam->hdr.state = LCTR_ACAD_STATE_ENABLED;

          BSTREAM_TO_UINT40(pAcadParam->chanMapUpdate.chanMask, pBuf);
          BSTREAM_TO_UINT16(pAcadParam->chanMapUpdate.instant, pBuf);
        }
        break;
      }

      default:
        break;
    }
    len -= (acadLen + 1); /* Minus the Acad plus the acadLen field. */
  }
}

/**************************************************************************************************
  Functions: Extended advertising reports
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Pend extended advertising report notification.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *  \param      pRpt            Report data.
 *  \param      pState          Report state.
 *
 *  \return     TRUE if report pended, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrExtAdvRptPend(lctrExtScanCtx_t *pExtScanCtx, LlExtAdvReportInd_t *pRpt, lctrRptState_t *pState)
{
  /* Only called with the first report in the sequence. */
  WSF_ASSERT(*pState == LCTR_RPT_STATE_IDLE);

  if (pExtScanCtx->state != LCTR_EXT_SCAN_STATE_DISCOVER)
  {
    return FALSE;
  }

  /* Check for duplicate report. */
  uint64_t hash;
  lctrAdvRptGenerateExtHash(&hash, pRpt->addrType, BstreamToBda64(pRpt->addr),
                            pRpt->eventType, pRpt->advSID, pExtScanCtx->extAdvHdr.did);
  if (lctrAdvRptCheckDuplicate(&lctrMstExtScan.advFilt, hash))
  {
    /* Duplicate found, just exit. */
    return FALSE;
  }

  *pState = LCTR_RPT_STATE_IN_PROGRESS;

  /* Event handling offloaded to task context, cf. lctrMstExtDiscoverEndOp(), lctrMstAuxDiscoverEndOp(). */
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Pend extended advertising report notification.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *  \param      pRpt            Report data.
 *  \param      pState          Report state.
 *
 *  \return     TRUE if report pended, FALSE otherwise.
 */
/*************************************************************************************************/
static void lctrExtAdvRptSubmit(lctrExtScanCtx_t *pExtScanCtx, LlExtAdvReportInd_t *pRpt, lctrRptState_t *pState)
{
  if (*pState == LCTR_RPT_STATE_IN_PROGRESS)
  {
    if (((pRpt->eventType >> 5) & LL_RPT_DATA_INC_TRUNC) == 0)
    {
      /* Only save the hash to the table when complete data is received. */
      uint64_t hash;
      lctrAdvRptGenerateExtHash(&hash, pRpt->addrType, BstreamToBda64(pRpt->addr),
                                pRpt->eventType, pRpt->advSID, pExtScanCtx->extAdvHdr.did);
      lctrAdvRptAddEntry(&lctrMstExtScan.advFilt, hash);
    }

    *pState = LCTR_RPT_STATE_COMP;
    WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_ADVB));
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Create extended advertising report.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *  \param      extraEventType  Extra event type flags.
 *  \param      pAdvHdr         Advertising header.
 *  \param      peerIdAddr      Peer identify address.
 *  \param      peerIdAddrType  Peer identify address type.
 *  \param      rssi            RSSI value.
 *  \param      pAdvBuf         Advertising buffer.
 *  \param      pRpt            Extended advertising report.
 *
 *  \return     TRUE if report delivery successful, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrExtAdvRptPack(lctrExtScanCtx_t *pExtScanCtx, uint16_t extraEventType,
                                lctrAdvbPduHdr_t *pAdvHdr, uint64_t peerIdAddr, uint8_t peerIdAddrType,
                                int8_t rssi, const uint8_t *pAdvBuf, LlExtAdvReportInd_t *pRpt)
{
  /* Ensure single report generation. */
  if (pExtScanCtx->data.scan.advRptState != LCTR_RPT_STATE_IDLE)
  {
    LL_TRACE_WARN0("Report generation flow controlled due to outstanding report notification");
    return FALSE;
  }

  memset(pRpt, 0, sizeof(*pRpt));

  pRpt->rssi           = rssi;
  pRpt->eventType      = extraEventType;
  pRpt->advSID         = LL_SID_NO_ADI;
  pRpt->txPwr          = LL_RSSI_NOT_AVAIL;

  switch (LCTR_GET_EXT_SCAN_HANDLE(pExtScanCtx))
  {
    case LCTR_SCAN_PHY_1M:
      pRpt->priPhy = LL_PHY_LE_1M;
      break;
    case LCTR_SCAN_PHY_CODED:
      pRpt->priPhy = LL_PHY_LE_CODED;
      break;
    default:
      break;
  }

  switch (pAdvHdr->pduType)
  {
    /* Extended advertising PDUs. */
    case LL_PDU_ADV_EXT_IND:
    /* case LL_PDU_AUX_ADV_IND: */      /* Shared auxiliary advertising PDU type values. */
    /* case LL_PDU_AUX_SCAN_RSP: */
    /* case LL_PDU_AUX_SYNC_IND: */
    /* case LL_PDU_AUX_CHAIN_IND: */
    case LL_PDU_AUX_CONNECT_RSP:
    {
      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT)
      {
        pRpt->addrType = peerIdAddrType;
        Bda64ToBstream(pRpt->addr, peerIdAddr);
      }
      else
      {
        pRpt->addrType = LL_ADDR_ANONYMOUS;
      }
      pRpt->pData = pExtScanCtx->pExtAdvData;
      pRpt->len = 0;
      pRpt->eventType |= ((pExtScanCtx->extAdvHdr.advMode & LCTR_ADV_MODE_CONN_BIT) ? LL_RPT_EVT_CONN_ADV_BIT : 0) |
                         ((pExtScanCtx->extAdvHdr.advMode & LCTR_ADV_MODE_SCAN_BIT) ? LL_RPT_EVT_SCAN_ADV_BIT : 0) |
                         ((pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT) ? LL_RPT_EVT_DIRECT_ADV_BIT : 0);
      pRpt->secPhy = lctrConvertAuxPtrPhyToAdvRptPhy(pExtScanCtx->priChAuxPtr.auxPhy);
      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_ADI_BIT)
      {
        pRpt->advSID = pExtScanCtx->extAdvHdr.sid;
      }
      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_TX_PWR_BIT)
      {
        pRpt->txPwr = pExtScanCtx->extAdvHdr.txPwr;
      }
      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_SYNC_INFO_BIT)
      {
        pRpt->perAdvInter = pExtScanCtx->secSyncInfo.syncInter;
      }

      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
      {
        uint64_t loocalIdAddr;
        uint8_t localIdAddrType;

        lctrGetLocalIdAddr(pExtScanCtx, pExtScanCtx->extAdvHdr.tgtAddr, &loocalIdAddr, &localIdAddrType, peerIdAddr, peerIdAddrType);

        Bda64ToBstream(pRpt->directAddr, loocalIdAddr);
        pRpt->directAddrType = localIdAddrType;
      }
      break;
    }

    /* Legacy advertising PDU. */
    default:
    {
      if ((pAdvHdr->len < LL_ADV_PREFIX_LEN) ||      /* invalid packet length. */
          (pAdvHdr->len > (LL_ADVB_MAX_LEN - LL_ADV_HDR_LEN)))
      {
        /* Do not send report for invalid packets. */
        return FALSE;
      }

      /* Default advertising data values. */
      const uint8_t *pData = pAdvBuf + LL_ADV_HDR_LEN + LL_ADV_PREFIX_LEN;
      uint8_t dataLen = pAdvHdr->len - LL_ADV_PREFIX_LEN;

      pRpt->eventType |= LL_RPT_EVT_LEGACY_ADV_BIT;
      switch (pAdvHdr->pduType)
      {
        case LL_PDU_ADV_IND:
          pRpt->eventType |= LL_RPT_EVT_CONN_ADV_BIT | LL_RPT_EVT_SCAN_ADV_BIT |                         0;
          break;
        case LL_PDU_ADV_DIRECT_IND:
        {
          pRpt->eventType |= LL_RPT_EVT_CONN_ADV_BIT | /* --------------- */ 0 | LL_RPT_EVT_DIRECT_ADV_BIT;
          dataLen = 0;        /* no payload */
          const uint8_t *pTargetA = pAdvBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
          uint64_t loocalIdAddr;
          uint8_t localIdAddrType;

          lctrGetLocalIdAddr(pExtScanCtx, BstreamToBda64(pTargetA), &loocalIdAddr, &localIdAddrType, peerIdAddr, peerIdAddrType);
          Bda64ToBstream(pRpt->directAddr, loocalIdAddr);
          pRpt->directAddrType = localIdAddrType;
          break;
        }
        case LL_PDU_ADV_SCAN_IND:
          pRpt->eventType |= /* --------------- */ 0 | LL_RPT_EVT_SCAN_ADV_BIT | /* ----------------- */ 0;
          break;
        case LL_PDU_ADV_NONCONN_IND:
          pRpt->eventType |= /* --------------- */ 0 | /* --------------- */ 0 | /* ----------------- */ 0;
          break;
        case LL_PDU_SCAN_RSP:
          pRpt->eventType |= /* --------------- */ 0 | LL_RPT_EVT_SCAN_ADV_BIT | /* ----------------- */ 0;
          pRpt->eventType |= (lctrMstExtScanIsr.advPduType == LL_PDU_ADV_IND) ? LL_RPT_EVT_CONN_ADV_BIT : 0;
          pRpt->eventType |= LL_RPT_EVT_SCAN_RSP_BIT;
          break;
        default:
          /* Invalid packet type. */
          LL_TRACE_WARN1("Received advertising PDU with invalid PDU type=%u", pAdvHdr->pduType);
          return FALSE;
      }

      pRpt->addrType = peerIdAddrType;
      Bda64ToBstream(pRpt->addr, peerIdAddr);

      pRpt->pData = pData;
      pRpt->len = dataLen;
      break;
    }
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Reassemble extended advertising data.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *  \param      pChainBuf       Chain buffer.
 *  \param      chainLen        Chain length.
 *  \param      rssi            RSSI of received CHAIN_IND.
 *
 *  \return     TRUE if reassembly successful, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrExtAdvDataReassemble(lctrExtScanCtx_t *pExtScanCtx, const uint8_t *pChainBuf, uint8_t chainLen, int8_t rssi)
{
  /* Ensure report event is already packed. */
  WSF_ASSERT(pExtScanCtx->data.scan.advRptState == LCTR_RPT_STATE_IN_PROGRESS);

  LlExtAdvReportInd_t * const pRpt = &pExtScanCtx->data.scan.advRpt;

  uint16_t availLen = pLctrRtCfg->maxExtScanDataLen - pRpt->len;
  uint16_t fragLen = WSF_MIN(availLen, chainLen);

  /* Reassemble AdvData with chain fragment. */
  memcpy(pExtScanCtx->pExtAdvData + pRpt->len, pChainBuf, fragLen);
  pRpt->len += fragLen;

  if (fragLen < chainLen)
  {
    return FALSE;
  }

  /* Update RSSI with last received CHAIN_IND. */
  pRpt->rssi = rssi;

  return TRUE;
}

/**************************************************************************************************
  Functions: Periodic advertising reports
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Pend periodic advertising report notification.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     TRUE if report pended, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrPerAdvRptPend(lctrPerScanCtx_t *pPerScanCtx)
{
  /* Only called with the first report in the sequence. */
  WSF_ASSERT(pPerScanCtx->advRptState == LCTR_RPT_STATE_IDLE);

  if (pPerScanCtx->state != LCTR_PER_SCAN_STATE_SYNC_ESTD)
  {
    return FALSE;
  }

  pPerScanCtx->advRptState = LCTR_RPT_STATE_IN_PROGRESS;

  /* Event handling offloaded to task context, cf. lctrMstExtDiscoverEndOp(), lctrMstAuxDiscoverEndOp(). */
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Pend periodic advertising report notification.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     TRUE if report pended, FALSE otherwise.
 */
/*************************************************************************************************/
static void lctrPerAdvRptSubmit(lctrPerScanCtx_t *pPerScanCtx)
{
  if (pPerScanCtx->advRptState == LCTR_RPT_STATE_IN_PROGRESS)
  {
    pPerScanCtx->advRptState = LCTR_RPT_STATE_COMP;
    WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_ADVB));
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Reassemble periodic advertising data.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *  \param      pChainBuf       Chain buffer.
 *  \param      chainLen        Chain length.
 *  \param      rssi            RSSI of received CHAIN_IND.
 *
 *  \return     TRUE if reassembly successful, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrPerAdvDataReassemble(lctrPerScanCtx_t *pPerScanCtx, const uint8_t *pChainBuf, uint8_t chainLen, int8_t rssi)
{
  /* Ensure report event is already packed. */
  WSF_ASSERT(pPerScanCtx->advRptState == LCTR_RPT_STATE_IN_PROGRESS);

  LlPerAdvReportInd_t * const pRpt = &pPerScanCtx->advRpt;
  uint16_t availLen = pLctrRtCfg->maxExtScanDataLen - pRpt->len;
  uint16_t fragLen = WSF_MIN(availLen, chainLen);

  /* Reassemble AdvData with chain fragment. */
  memcpy(pPerScanCtx->pPerAdvData + pRpt->len, pChainBuf, fragLen);
  pRpt->len += fragLen;

  if (fragLen < chainLen)
  {
    return FALSE;
  }

  /* Update RSSI with last received CHAIN_IND. */
  pRpt->rssi = rssi;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Pack periodic advertising report.
 *
 *  \param      pOp             Originating operation.
 *  \param      pAdvBuf         Received adv buffer, NULL if receive failed.
 *  \param      extAdvHdrFlags  Extended adv header flags.
 *  \param      pRpt            Periodic advertising report.
 *
 *  \return     TRUE if pack report successful, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrPerAdvRptPack(BbOpDesc_t *pOp, const uint8_t *pAdvBuf,
                                       uint8_t extAdvHdrFlags,
                                       LlPerAdvReportInd_t *pRpt)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstPerScanEvent_t * const pPerScan= &pBle->op.mstPerScan;
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;

  memset(pRpt, 0, sizeof(*pRpt));

  pRpt->syncHandle = LCTR_GET_PER_SCAN_HANDLE(pPerScanCtx);
  if (extAdvHdrFlags & LL_EXT_HDR_TX_PWR_BIT)
  {
    pRpt->txPwr = pPerScanCtx->extAdvHdr.txPwr;
  }
  pRpt->rssi = pPerScan->perAdvRssi;
  pRpt->unused = 0xFF;    /* Defined by spec. */
  pRpt->pData = pPerScanCtx->pPerAdvData;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Pack periodic advertising truncate report.
 *
 *  \param      pOp         Originating operation.
 *  \param      pAdvBuf     Received adv buffer, NULL if receive failed.
 *  \param      pRpt        Periodic advertising report.
 *
 *  \return     TRUE if pack report successful, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrPerAdvRptPackTruncate(BbOpDesc_t *pOp, const uint8_t *pAdvBuf, LlPerAdvReportInd_t *pRpt)
{
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;

  memset(pRpt, 0, sizeof(LlPerAdvReportInd_t));

  pRpt->syncHandle = LCTR_GET_PER_SCAN_HANDLE(pPerScanCtx);
  pRpt->txPwr = LL_RSSI_NOT_AVAIL;
  pRpt->rssi = LL_RSSI_NOT_AVAIL;
  pRpt->unused = 0xFF;    /* Defined by spec. */
  pRpt->dataStatus = LL_RPT_DATA_INC_TRUNC;

  return TRUE;
}

/**************************************************************************************************
  Functions: Scan backoff
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Scan backoff maintenance when response reception is successful.
 *
 *  \param  pExtScanCtx     Extended scan context.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrScanBackoffRspSuccess(lctrExtScanCtx_t *pExtScanCtx)
{
  if (++pExtScanCtx->data.scan.consRspSuccess >= 2)
  {
    pExtScanCtx->data.scan.upperLimit = pExtScanCtx->data.scan.upperLimit >> 1;           /* divide by 2 */

    if (pExtScanCtx->data.scan.upperLimit == 0)
    {
      pExtScanCtx->data.scan.upperLimit = 1;
    }

    pExtScanCtx->data.scan.consRspSuccess = 0;
  }

  pExtScanCtx->data.scan.consRspFailure = 0;

  /* backoffCount = [1..upperLimit] */
  pExtScanCtx->data.scan.backoffCount = (LlMathRandNum() & (pExtScanCtx->data.scan.upperLimit - 1)) + 1;
}

/*************************************************************************************************/
/*!
 *  \brief  Scan backoff maintenance when response reception failed.
 *
 *  \param  pExtScanCtx     Extended scan context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrScanBackoffRspFailed(lctrExtScanCtx_t *pExtScanCtx)
{
  if (++pExtScanCtx->data.scan.consRspFailure >= 2)
  {
    pExtScanCtx->data.scan.upperLimit = pExtScanCtx->data.scan.upperLimit << 1;           /* multiply by 2 */

    if (pExtScanCtx->data.scan.upperLimit > 256)
    {
      pExtScanCtx->data.scan.upperLimit = 256;
    }

    pExtScanCtx->data.scan.consRspFailure = 0;
  }

  pExtScanCtx->data.scan.consRspSuccess = 0;

  /* backoffCount = [1..upperLimit] */
  pExtScanCtx->data.scan.backoffCount = (LlMathRandNum() & (pExtScanCtx->data.scan.upperLimit - 1)) + 1;
}

/*************************************************************************************************/
/*!
 *  \brief  Check whether scan backoff should be applied.
 *
 *  \param  pExtScanCtx     Extended scan context.
 *
 *  \return TRUE if request should be sent, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrScanBackoffCheckReqAllowed(lctrExtScanCtx_t *pExtScanCtx)
{
  if (!lmgrGetOpFlag(LL_OP_MODE_FLAG_ENA_SCAN_BACKOFF))
  {
    /* Backoff is disabled, always send request. */
    return TRUE;
  }

  if (pExtScanCtx->data.scan.backoffCount)
  {
    pExtScanCtx->data.scan.backoffCount--;
  }

  if (pExtScanCtx->data.scan.backoffCount == 0)
  {
    /* No backoff; send request. */
    return TRUE;
  }

  /* Apply backoff. */
  return FALSE;
}

/**************************************************************************************************
  Functions: Discovery packet handlers
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Extended scan discover state advertising packet receive (primary channel)
 *              completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if scan request transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxExtAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  lctrAdvbPduHdr_t advHdr;
  lctrUnpackAdvbPduHdr(&advHdr, pAdvBuf);

  /* Store Advertising Event state. */
  lctrMstExtScanIsr.advPduType = advHdr.pduType;

  /*** Transmit response PDU processing. ***/

  bool_t txScanReq = FALSE;

  switch (advHdr.pduType)
  {
    case LL_PDU_ADV_EXT_IND:
      /* FIXME Check if secondary PHY is supported. */
      break;
    default:
      /* Legacy advertising. */
      txScanReq = lctrMstDiscoverRxLegacyAdvPktHandler(pOp, pAdvBuf);
      break;
  }

  return txScanReq;
}

/*************************************************************************************************/
/*!
 *  \brief      Extended scan discover state advertising packet receive (primary channel)
 *              post process handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstDiscoverRxExtAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  lctrAdvbPduHdr_t advHdr;
  lctrUnpackAdvbPduHdr(&advHdr, pAdvBuf);

  /*** Received advertising PDU post-processing. ***/

  lctrMstExtScanIsr.filtResult = FALSE;

  switch (advHdr.pduType)
  {
    /* Extended advertising. */
    case LL_PDU_ADV_EXT_IND:
    {
      pExtScanCtx->extAdvHdr.extHdrFlags = 0;

      /*** Extended advertising event filtering. ***/
      const uint8_t *pExtAdvHdr = pAdvBuf + LL_ADV_HDR_LEN;
      uint8_t extAdvHdrFlags;
      lctrUnpackExtAdvHeader(&pExtScanCtx->extAdvHdr, &extAdvHdrFlags, pExtAdvHdr);

      if (pExtScanCtx->extAdvHdr.advMode == 0)         /* Non-connectable and non-scannable. */
      {
        if ((extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT) == 0)
        {
          if ((extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT) == 0)
          {
            LL_TRACE_WARN0("Ignoring LL_PDU_ADV_EXT_IND due to missing mandatory advA when there is no auxiliary packet.");
            lctrMstExtScanIsr.filtResult = TRUE;
            break;
          }

          /* AdvA and/or TargetA are received, go through the filter now. */
          bbBlePduExtFiltParams_t params;

          params.pduType = advHdr.pduType;
          params.extHdrFlags = pExtScanCtx->extAdvHdr.extHdrFlags;
          params.peerAddr = pExtScanCtx->extAdvHdr.advAddr;
          params.peerAddrRand = advHdr.txAddrRnd;
          params.localAddr = pExtScanCtx->extAdvHdr.tgtAddr;
          params.localAddrRand = advHdr.rxAddrRnd;

          if (BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, FALSE, &pScan->filtResults) == FALSE)
          {
            LL_TRACE_WARN0("Ignoring LL_PDU_ADV_EXT_IND due to PDU filtering.");
            lctrMstExtScanIsr.filtResult = TRUE;
            break;
          }
        }
        else
        {
          /* Delay Non-connectable and non-scannable with auxiliary packet PDU filtering in the AUX_ADV_IND.*/
          lctrMstExtScanIsr.advAReceived = FALSE;
          lctrMstExtScanIsr.tgtAReceived = FALSE;
          lctrMstExtScanIsr.advA = 0;
          lctrMstExtScanIsr.tgtA = 0;
          lctrMstExtScanIsr.advARand = 0;
          lctrMstExtScanIsr.tgtARand = 0;

          /* Retrieve advA and tgtA if present. */
          if (extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT)
          {
            lctrMstExtScanIsr.advAReceived = TRUE;
            lctrMstExtScanIsr.advA = pExtScanCtx->extAdvHdr.advAddr;
            lctrMstExtScanIsr.advARand = advHdr.txAddrRnd;
          }
          if (extAdvHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
          {
            lctrMstExtScanIsr.tgtAReceived = TRUE;
            lctrMstExtScanIsr.tgtA = pExtScanCtx->extAdvHdr.tgtAddr;
            lctrMstExtScanIsr.tgtARand = advHdr.rxAddrRnd;
          }
        }
      }
      /* else case, delay connectable and scannable PDU filtering in the AUX_ADV_IND. */

      /*** Periodic advertising event filtering. ***/
      lctrMstPerScanIsr.filtResult = TRUE;

      if ((lctrPerCreateSync.state == LCTR_CREATE_SYNC_STATE_DISCOVER) &&
          (extAdvHdrFlags & LL_EXT_HDR_ADI_BIT) &&
          (extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT))
      {
        bool_t advAMatch = FALSE;
        bbBlePduExtFiltParams_t params;
        uint64_t peerIdAddr = 0;
        uint8_t peerIdAddrType = 0;

        params.pduType = advHdr.pduType;
        params.extHdrFlags = pExtScanCtx->extAdvHdr.extHdrFlags;
        params.peerAddr = pExtScanCtx->extAdvHdr.advAddr;
        params.peerAddrRand = advHdr.txAddrRnd;
        params.localAddr = pExtScanCtx->extAdvHdr.tgtAddr;
        params.localAddrRand = advHdr.rxAddrRnd;

        /* This function is only used here for address conversion, not for filtering. */
        (void)BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, FALSE, &pScan->filtResults);

        BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &peerIdAddr, &peerIdAddrType);

        if (lctrPerCreateSync.filtParam.filterPolicy == LL_PER_SCAN_FILTER_PL_BIT)
        {
          if ((BbBlePeriodicListCheckAddr((peerIdAddrType & LL_ADDR_RANDOM_BIT), peerIdAddr,
                                          pExtScanCtx->extAdvHdr.sid)) == FALSE)
          {
            /* Not in the periodic list. */
            break;
          }
          else
          {
            lctrMstPerScanIsr.filtResult = FALSE;
            advAMatch = TRUE;
          }
        }
        else
        {
          if ((lctrPerCreateSync.filtParam.advSID != pExtScanCtx->extAdvHdr.sid) ||
              (lctrPerCreateSync.filtParam.advAddrType != (peerIdAddrType & LL_ADDR_RANDOM_BIT)) ||
              (lctrPerCreateSync.filtParam.advAddr != peerIdAddr))
          {
            /* Address type, address or SID does not match. */
            break;
          }
          else
          {
            lctrMstPerScanIsr.filtResult = FALSE;
            advAMatch = TRUE;
          }
        }

        if (advAMatch == TRUE)
        {
          /* AdvA is received in the adv_ext_ind and pass the filtering, save info in the context. */
          lctrPerCreateSync.pPerScanCtx->advAddr = peerIdAddr;
          lctrPerCreateSync.pPerScanCtx->advAddrType = peerIdAddrType;
          lctrPerCreateSync.pPerScanCtx->advSID = pExtScanCtx->extAdvHdr.sid;

          /* Address to be used for sync transfer. */
          lctrPerCreateSync.pPerScanCtx->trsfAdvAddr = pExtScanCtx->extAdvHdr.advAddr;
          lctrPerCreateSync.pPerScanCtx->trsfAddrType = advHdr.txAddrRnd;
        }
      }

      if (extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT)
      {
        pOp->minDurUsec = 0;  /* Update primary scan BOD min duration so that secondary scan can be scheduled. */

        lctrUnpackAuxPtr(&pExtScanCtx->priChAuxPtr, pExtScanCtx->extAdvHdr.pAuxPtr);
        uint32_t endTs = pScan->advStartTs +
                         BB_US_TO_BB_TICKS(SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pScan->advRxPhyOptions, LL_ADV_HDR_LEN + pScan->filtResults.pduLen));
        lctrMstAuxDiscoverOpCommit(pExtScanCtx, &pExtScanCtx->priChAuxPtr, pScan->advStartTs, endTs);

        if ((pExtScanCtx->auxOpPending == FALSE) &&
            (lctrPerCreateSync.state == LCTR_CREATE_SYNC_STATE_DISCOVER) &&
            (lctrMstPerScanIsr.filtResult == FALSE))
        {
          /* Reset the flag if cannot schedule the auxiliary operation. */
          LL_TRACE_WARN0("Reset filter flag due to auxiliary operation scheduling conflict.");
          lctrMstPerScanIsr.filtResult = TRUE;
        }

        if (pExtScanCtx->auxOpPending)
        {
          /* Extended advertising event continues with auxiliary BOD */
          BbSetBodTerminateFlag();
        }

        /* Auxiliary PDU expected; defer report. */
      }
      else
      {
        /* No auxiliary PDU; send report now. */
        uint64_t peerIdAddr = 0;
        uint8_t peerIdAddrType = 0;

        BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &peerIdAddr, &peerIdAddrType);

        LlExtAdvReportInd_t * const pRpt = &pExtScanCtx->data.scan.advRpt;
        if (lctrExtAdvRptPack(pExtScanCtx, 0, &advHdr,
                              peerIdAddr, peerIdAddrType, pScan->advRssi, pAdvBuf, pRpt))
        {
          pRpt->secPhy = LL_PHY_NONE;
          if (lctrExtAdvRptPend(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState))
          {
            lctrExtAdvRptSubmit(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState);
          }
        }
      }
      break;
    }

    /* Legacy advertising. */
    default:
    {
      uint64_t peerIdAddr = 0;
      uint8_t peerIdAddrType = 0;

      BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &peerIdAddr, &peerIdAddrType);

      LlExtAdvReportInd_t * const pRpt = &pExtScanCtx->data.scan.advRpt;
      if (lctrExtAdvRptPack(pExtScanCtx, 0, &advHdr,
                            peerIdAddr,peerIdAddrType, pScan->advRssi, pAdvBuf, pRpt))
      {
        pRpt->secPhy = LL_PHY_NONE;
        if (lctrExtAdvRptPend(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState))
        {
          lctrExtAdvRptSubmit(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState);
        }
      }
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Auxiliary scan discover state advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if scan request transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxAuxAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_AUX_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAuxAdvEvent_t * const pAuxScan = &pBle->op.mstAuxAdv;
  lctrExtScanCtx_t * pExtScanCtx = pOp->pCtx;
  const uint8_t *pExtAdvHdr = pAdvBuf + lctrUnpackAdvbPduHdr(&lctrMstExtScanIsr.advHdr, pAdvBuf);
  lctrMstExtScanIsr.extAdvHdrLen = lctrUnpackExtAdvHeader(&pExtScanCtx->extAdvHdr, &lctrMstExtScanIsr.extAdvHdrFlags, pExtAdvHdr);
  lctrMstExtScanIsr.isHdrParsed = TRUE;

  /*** Extended advertising event filtering. ***/

  switch (lctrMstExtScanIsr.advHdr.pduType)
  {
    case LL_PDU_AUX_ADV_IND:
    {
      if (pExtScanCtx->extAdvHdr.advMode == 0)          /* Non-connectable and non-scannable. */
      {
        /* Retrieve advA and tgtA if not present in the ADV_EXT_IND and present here. */
        if ((lctrMstExtScanIsr.advAReceived == FALSE) &&
            (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT))
        {
          lctrMstExtScanIsr.advAReceived = TRUE;
          lctrMstExtScanIsr.advA = pExtScanCtx->extAdvHdr.advAddr;
          lctrMstExtScanIsr.advARand = lctrMstExtScanIsr.advHdr.txAddrRnd;
        }

        if ((lctrMstExtScanIsr.tgtAReceived == FALSE) &&
            (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT))
        {
          lctrMstExtScanIsr.tgtAReceived = TRUE;
          lctrMstExtScanIsr.tgtA = pExtScanCtx->extAdvHdr.tgtAddr;
          lctrMstExtScanIsr.tgtARand = lctrMstExtScanIsr.advHdr.rxAddrRnd;
        }

        /* Update extended flags, address types and addresses. */
        if (lctrMstExtScanIsr.advAReceived == TRUE)
        {
          pExtScanCtx->extAdvHdr.extHdrFlags |= LL_EXT_HDR_ADV_ADDR_BIT;

        }
        pExtScanCtx->extAdvHdr.advAddr = lctrMstExtScanIsr.advA;
        lctrMstExtScanIsr.advHdr.txAddrRnd = lctrMstExtScanIsr.advARand;

        if (lctrMstExtScanIsr.tgtAReceived == TRUE)
        {
          pExtScanCtx->extAdvHdr.extHdrFlags |= LL_EXT_HDR_TGT_ADDR_BIT;
        }
        pExtScanCtx->extAdvHdr.tgtAddr = lctrMstExtScanIsr.tgtA;
        lctrMstExtScanIsr.advHdr.rxAddrRnd = lctrMstExtScanIsr.tgtARand;
      }
      else                                              /* Connectable or scannable. */
      {
        /* AdvA is mandatory. */
        if ((lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT) == 0)
        {
          LL_TRACE_WARN0("Ignoring LL_PDU_AUX_ADV_IND due to missing mandatory advA.");
          lctrMstExtScanIsr.filtResult = TRUE;
          break;
        }
      }

      /* Go through the extended PDU filter. */
      bbBlePduExtFiltParams_t params;
      params.pduType = lctrMstExtScanIsr.advHdr.pduType;
      params.extHdrFlags = pExtScanCtx->extAdvHdr.extHdrFlags;
      params.peerAddr = pExtScanCtx->extAdvHdr.advAddr;
      params.peerAddrRand = lctrMstExtScanIsr.advHdr.txAddrRnd;
      params.localAddr = pExtScanCtx->extAdvHdr.tgtAddr;
      params.localAddrRand = lctrMstExtScanIsr.advHdr.rxAddrRnd;

      if (BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, FALSE, &pAuxScan->filtResults) == FALSE)
      {
        LL_TRACE_INFO1("Ignoring LL_PDU_AUX_ADV_IND due to PDU filtering, SID=%u", pExtScanCtx->extAdvHdr.sid);
        lctrMstExtScanIsr.filtResult = TRUE;
        /* Continue processing for sync establishment filter even when scan filtering failed. */
      }

      /*** Periodic advertising event filtering. ***/
      bool_t advAMatch = FALSE;
      uint64_t peerIdAddr = 0;
      uint8_t peerIdAddrType = 0;

      BbBlePduFiltResultsGetPeerIdAddr(&pAuxScan->filtResults, &peerIdAddr, &peerIdAddrType);

      if ((lctrPerCreateSync.state == LCTR_CREATE_SYNC_STATE_DISCOVER) &&
          (lctrMstPerScanIsr.filtResult == FALSE))
      {
        if ((lctrPerCreateSync.filtParam.advSID != pExtScanCtx->extAdvHdr.sid) ||
            (lctrPerCreateSync.filtParam.advAddrType != (peerIdAddrType & LL_ADDR_RANDOM_BIT)) ||
            (lctrPerCreateSync.filtParam.advAddr != peerIdAddr))
        {
          /* Address type, address or SID does not match. */
          lctrMstPerScanIsr.filtResult = TRUE;
        }
      }

      if ((lctrPerCreateSync.state == LCTR_CREATE_SYNC_STATE_DISCOVER) &&
          (lctrMstPerScanIsr.filtResult == TRUE) &&
          (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_ADI_BIT) &&
          (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT))
      {
        if (lctrPerCreateSync.filtParam.filterPolicy == LL_PER_SCAN_FILTER_PL_BIT)
        {
          if ((BbBlePeriodicListCheckAddr((peerIdAddrType & LL_ADDR_RANDOM_BIT), peerIdAddr,
                                          pExtScanCtx->extAdvHdr.sid)) == FALSE)
          {
            /* Not in the periodic list. */
            break;
          }
          else
          {
            lctrMstPerScanIsr.filtResult = FALSE;
            advAMatch = TRUE;
          }
        }
        else
        {
          if ((lctrPerCreateSync.filtParam.advSID != pExtScanCtx->extAdvHdr.sid) ||
              (lctrPerCreateSync.filtParam.advAddrType != (peerIdAddrType & LL_ADDR_RANDOM_BIT)) ||
              (lctrPerCreateSync.filtParam.advAddr != peerIdAddr))
          {
            /* Address type, address or SID does not match. */
            break;
          }
          else
          {
            lctrMstPerScanIsr.filtResult = FALSE;
            advAMatch = TRUE;
          }
        }
      }

      if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_SYNC_INFO_BIT)
      {
        lctrUnpackSyncInfo(&pExtScanCtx->secSyncInfo, pExtScanCtx->extAdvHdr.pSyncInfo);
      }

      if ((lctrPerCreateSync.state == LCTR_CREATE_SYNC_STATE_DISCOVER) &&
          (lctrMstPerScanIsr.filtResult == FALSE) &&
          (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_SYNC_INFO_BIT) &&
          lctrPerCreateSync.createSyncPending == FALSE)
      {
        lctrPerScanCtx_t *pPerScanCtx = lctrPerCreateSync.pPerScanCtx;
        /*** Save peer periodic advertising parameters. ***/
        pPerScanCtx->eventCounter = pExtScanCtx->secSyncInfo.eventCounter;
        pPerScanCtx->initEventCounter = pExtScanCtx->secSyncInfo.eventCounter;
        pPerScanCtx->perInter = BB_US_TO_BB_TICKS(LCTR_PER_INTER_TO_US(pExtScanCtx->secSyncInfo.syncInter));
        pPerScanCtx->sca = pExtScanCtx->secSyncInfo.sca;
        pPerScanCtx->rxPhys = lctrConvertAuxPtrPhyToBbPhy(pExtScanCtx->priChAuxPtr.auxPhy);
        pPerScanCtx->skipInter = pPerScanCtx->perInter * pPerScanCtx->skip;

        if (advAMatch == TRUE)
        {
          /* AdvA is received in the aux_adv_ind and pass the filtering, save info in the context. */
          pPerScanCtx->advAddr = peerIdAddr;
          pPerScanCtx->advAddrType = peerIdAddrType;
          pPerScanCtx->advSID = pExtScanCtx->extAdvHdr.sid;

          /* Address to be used for sync transfer. */
          pPerScanCtx->trsfAdvAddr = pExtScanCtx->extAdvHdr.advAddr;
          pPerScanCtx->trsfAddrType = lctrMstExtScanIsr.advHdr.txAddrRnd;
        }

        uint32_t endTs = pAuxScan->auxStartTs +
                         BB_US_TO_BB_TICKS(SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, pAuxScan->auxRxPhyOptions, pAuxScan->txAuxReqLen));
        lctrMstPerScanOpCommit(pExtScanCtx, &pExtScanCtx->priChAuxPtr, &pExtScanCtx->secSyncInfo, pAuxScan->auxStartTs, endTs);
        lctrMstPerScanIsr.syncWithSlave = FALSE;
      }
      break;
    }

    default:
      LL_TRACE_WARN1("Received advertising PDU with invalid PDU type=%u", lctrMstExtScanIsr.advHdr.pduType);
      lctrMstExtScanIsr.filtResult = TRUE;
  }

  if (lctrMstExtScanIsr.filtResult == TRUE)
  {
    /* No more processing needed. */
    return FALSE;
  }

  /*** ACAD processing. ***/

  /* TODO route ACAD */
  (void)pExtScanCtx->extAdvHdr.pAcad;

  /*** Transmit request PDU processing. ***/

  bool_t txScanReq = FALSE;
  if (pExtScanCtx->extAdvHdr.advMode & LCTR_ADV_MODE_SCAN_BIT)
  {
    if (pAuxScan->pTxAuxReqBuf)     /* Active scanner. */
    {
      if (!pAuxScan->filtResults.peerMatch)
      {
        /* Require peer match. */
        lctrMstExtScanIsr.filtResult = TRUE;
        return FALSE;
      }

      if ((pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT) == 0)
      {
        LL_TRACE_WARN0("Ignoring extended advertising event with anonymous advertiser");
        lctrMstExtScanIsr.filtResult = TRUE;
        return FALSE;
      }

      /* Check backoff in effect. */
      if ((txScanReq = lctrScanBackoffCheckReqAllowed(pExtScanCtx)) == TRUE)
      {
        /* Update scan request header with advertiser's address. */
        uint8_t *pScanReqAdvA = pExtScanCtx->reqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
        Bda64ToBstream(pScanReqAdvA, pAuxScan->filtResults.peerAddr);
        pExtScanCtx->reqPduHdr.rxAddrRnd = pAuxScan->filtResults.peerAddrRand;

        /* Save adv address here and compare it with the one from AUX_SCAN_RSP later. */
        pExtScanCtx->data.scan.scanReqAdvAddr = pAuxScan->filtResults.peerAddr;

        /* Update scan request header with scanner's address. */
        if (pExtScanCtx->scanParam.ownAddrType & LL_ADDR_IDENTITY_BIT)
        {
          bool_t  localAddrRand;
          uint64_t localAddr;

          if (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
          {
            /* If peer is using directed advertising then use tgtA from the    */
            /* AUX_ADV_IND as our scanA in the auxiliary scan request.       */
            localAddrRand = lctrMstExtScanIsr.advHdr.rxAddrRnd;
            localAddr     = pExtScanCtx->extAdvHdr.tgtAddr;
            /* Update the local RPA if the received one is RPA. */
            if (BDA64_ADDR_IS_RPA(localAddr))
            {
              BbBleResListUpdateLocal(pAuxScan->filtResults.peerIdAddrRand, pAuxScan->filtResults.peerIdAddr, &localAddr);
            }
          }
          else
          {
            /* Otherwise, use local address, could be public, static random or RPA. */
            localAddrRand = BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
            localAddr     = pBle->pduFilt.localAddrMatch;
            if (BbBleResListReadLocal(pAuxScan->filtResults.peerIdAddrRand, pAuxScan->filtResults.peerIdAddr, &localAddr))
            {
              localAddrRand = TRUE;
            }
          }
          uint8_t *pAuxScanReqScanA = pExtScanCtx->reqBuf + LL_ADV_HDR_LEN;
          Bda64ToBstream(pAuxScanReqScanA, localAddr);
          pExtScanCtx->reqPduHdr.txAddrRnd = localAddrRand;
        }

        lctrPackAdvbPduHdr(pExtScanCtx->reqBuf, &pExtScanCtx->reqPduHdr);
      }
    }
  }

  /*** Advertising Data processing. ***/

  /* Report handled by lctrMstDiscoverRxAuxChainHandler() */
  lctrMstExtScanIsr.chainAdvMode = pExtScanCtx->extAdvHdr.advMode;
  lctrMstExtScanIsr.chainExtraEventType = 0;

  /*** Advertising report processing. ***/

  /* Store relevant AUX_ADV_IND report data; submitted in the lctrMstDiscoverRxAuxChainPostProcessHandler(). */
  pExtScanCtx->data.scan.auxAdvRpt.eventType = (pExtScanCtx->extAdvHdr.extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT) ?
                                               LL_RPT_EVT_DIRECT_ADV_BIT : 0;
  pExtScanCtx->data.scan.auxAdvRpt.rssi = pAuxScan->auxAdvRssi;

  return txScanReq;
}

/*************************************************************************************************/
/*!
 *  \brief      Master auxiliary scan response receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pRspBuf Received response buffer, NULL if receive failed.
 *
 *  \return     Always return FALSE (ignored by BB).
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxAuxScanRspHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_AUX_ADV_EVENT);

  lctrExtScanCtx_t * pExtScanCtx = pOp->pCtx;
  BbBleMstAuxAdvEvent_t * const pAuxScan = &pOp->prot.pBle->op.mstAuxAdv;

  /*** Scan backoff maintenance ***/

  if (pRspBuf == NULL)
  {
    lctrScanBackoffRspFailed(pExtScanCtx);
    return FALSE;
  }
  lctrScanBackoffRspSuccess(pExtScanCtx);

  const uint8_t *pExtAdvHdr = pRspBuf + lctrUnpackAdvbPduHdr(&lctrMstExtScanIsr.advHdr, pRspBuf);
  lctrMstExtScanIsr.extAdvHdrLen = lctrUnpackExtAdvHeader(&pExtScanCtx->extAdvHdr, &lctrMstExtScanIsr.extAdvHdrFlags, pExtAdvHdr);
  lctrMstExtScanIsr.isHdrParsed = TRUE;

  /*** Extended advertising event filtering. ***/
  bbBlePduExtFiltParams_t params;

  params.pduType = lctrMstExtScanIsr.advHdr.pduType;
  params.extHdrFlags = pExtScanCtx->extAdvHdr.extHdrFlags;
  params.peerAddr = pExtScanCtx->extAdvHdr.advAddr;
  params.peerAddrRand = lctrMstExtScanIsr.advHdr.txAddrRnd;
  params.localAddr = pExtScanCtx->extAdvHdr.tgtAddr;
  params.localAddrRand = lctrMstExtScanIsr.advHdr.rxAddrRnd;

  if (BbBleExtPduFiltCheck(&params, &pOp->prot.pBle->pduFilt, FALSE, &pAuxScan->filtResults) == FALSE)
  {
    LL_TRACE_WARN0("LL_PDU_AUX_SCAN_RSP failed PDU filtering.");
    lctrMstExtScanIsr.filtResult = TRUE;
    return FALSE;
  }

  if ((pExtScanCtx->extAdvHdr.advMode & (LCTR_ADV_MODE_SCAN_BIT | LCTR_ADV_MODE_CONN_BIT)) != 0)
  {
    LL_TRACE_WARN1("Received auxiliary PDU with invalid advMode=0x%02x", pExtScanCtx->extAdvHdr.advMode);
    lctrMstExtScanIsr.filtResult = TRUE;
    return FALSE;
  }

  /* scanReqAdvAddr is assigned when LL_PDU_ADV_SCAN_IND is received. */
  if (pExtScanCtx->data.scan.scanReqAdvAddr != pAuxScan->filtResults.peerAddr)
  {
    LL_TRACE_WARN0("Ignore AUX_SCAN_RSP since advAddr doesn't match the one sent in the AUX_SCAN_REQ.");
    lctrMstExtScanIsr.filtResult = TRUE;
    return FALSE;
  }

  /*** ACAD processing. ***/

  /* TODO route ACAD */
  (void)pExtScanCtx->extAdvHdr.pAcad;

  /*** Scan Response Data processing. ***/

  /* Report handled by lctrMstDiscoverRxAuxChainHandler() */
  lctrMstExtScanIsr.chainExtraEventType = LL_RPT_EVT_SCAN_RSP_BIT;

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Master auxiliary chain indication receive completion handler.
 *
 *  \param      pOp         Originating operation.
 *  \param      pChainBuf   Received chain buffer, NULL if receive failed.
 *
 *  \return     Next auxiliary offset in microseconds.
 */
/*************************************************************************************************/
uint32_t lctrMstDiscoverRxAuxChainHandler(BbOpDesc_t *pOp, const uint8_t *pChainBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_AUX_ADV_EVENT);

  if (lctrMstExtScanIsr.filtResult)
  {
    /* No further processing for filtered PDUs. */
    return 0;
  }

  if (pChainBuf == NULL)
  {
    /* The report will be submitted in the lctrMstDiscoverRxAuxChainPostProcessHandler(). */
    return 0;
  }

  BbBleData_t * const pBle = pOp->prot.pBle;
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  WSF_ASSERT(pExtScanCtx->state != LCTR_EXT_SCAN_STATE_DISABLED);

  const uint8_t *pExtAdvHdr;
  if (lctrMstExtScanIsr.isHdrParsed == TRUE)
  {
    pExtAdvHdr = pChainBuf + LL_ADV_HDR_LEN;
    lctrMstExtScanIsr.pAdvData = pExtAdvHdr + lctrMstExtScanIsr.extAdvHdrLen;
    lctrMstExtScanIsr.advDataLen = lctrMstExtScanIsr.advHdr.len - LCTR_EXT_HDR_CMN_LEN - pExtScanCtx->extAdvHdr.extHdrLen;
    lctrMstExtScanIsr.isHdrParsed = FALSE;
  }
  else
  {
    pExtAdvHdr = pChainBuf + lctrUnpackAdvbPduHdr(&lctrMstExtScanIsr.advHdr, pChainBuf);
    lctrMstExtScanIsr.pAdvData = pExtAdvHdr + lctrUnpackExtAdvHeader(&pExtScanCtx->extAdvHdr, &lctrMstExtScanIsr.extAdvHdrFlags, pExtAdvHdr);
    lctrMstExtScanIsr.advDataLen = lctrMstExtScanIsr.advHdr.len - LCTR_EXT_HDR_CMN_LEN - pExtScanCtx->extAdvHdr.extHdrLen;
  }

  /*** Extended advertising event filtering. ***/

  switch (lctrMstExtScanIsr.advHdr.pduType)
  {
    case LL_PDU_AUX_CHAIN_IND:
    /* case LL_PDU_AUX_ADV_IND: */      /* Same enum value as LL_PDU_AUX_CHAIN_IND. */
    /* case LL_PDU_AUX_SCAN_RSP: */     /* Same enum value as LL_PDU_AUX_CHAIN_IND. */
      break;
    default:
      LL_TRACE_WARN1("Received advertising PDU with invalid PDU type=%u", lctrMstExtScanIsr.advHdr.pduType);
      return 0;
  }

  /*** ACAD processing. ***/

  /* TODO route ACAD */
  (void)pExtScanCtx->extAdvHdr.pAcad;

  /*** Advertising Data processing. ***/

  uint32_t auxOffsetUsec = 0;
  if (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT)
  {
    lctrAuxPtr_t auxPtr;
    lctrUnpackAuxPtr(&auxPtr, pExtScanCtx->extAdvHdr.pAuxPtr);

    pBle->chan.chanIdx = auxPtr.auxChIdx;

    lctrMstComputeAuxOffset(&auxPtr, &auxOffsetUsec, &pBle->op.mstAuxAdv.rxSyncDelayUsec);
  }

  return auxOffsetUsec;
}

/*************************************************************************************************/
/*!
 *  \brief      Master auxiliary chain indication receive completion post process handler.
 *
 *  \param      pOp         Originating operation.
 *  \param      pChainBuf   Received chain buffer, NULL if receive failed.
 *
 *  \return     True if rx operation shall continue, False if rx operation shall terminate.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxAuxChainPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pChainBuf)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAuxAdvEvent_t * const pAuxScan = &pBle->op.mstAuxAdv;
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;
  bool_t result = TRUE;

  /*** Report generation. ***/
  if (lctrMstExtScanIsr.filtResult)
  {
    /* No further processing for filtered PDUs. */
    return FALSE;
  }

  if (pChainBuf == NULL)
  {
    WSF_ASSERT(pExtScanCtx->data.scan.advRptState == LCTR_RPT_STATE_IN_PROGRESS);
    pExtScanCtx->data.scan.advRpt.eventType |= LL_RPT_DATA_INC_TRUNC << LCTR_DATA_STATUS_SHIFT;
    lctrExtAdvRptSubmit(pExtScanCtx, &pExtScanCtx->data.scan.advRpt, &pExtScanCtx->data.scan.advRptState);
    return FALSE;
  }

  if (pExtScanCtx->data.scan.advRptState == LCTR_RPT_STATE_COMP)
  {
    /* Do not finish receiving chain. */
    LL_TRACE_WARN1("Cannot generate report due to pending report; do not receive CHAIN_IND, SID=%u", pExtScanCtx->extAdvHdr.sid);
    return FALSE;
  }

  do
  {
    if (pExtScanCtx->data.scan.advRptState == LCTR_RPT_STATE_IDLE)
    {
      /* First PDU in Auxiliary Advertising Event. */

      /* Restore AdvMode from original advertising event. */
      pExtScanCtx->extAdvHdr.advMode = lctrMstExtScanIsr.chainAdvMode;

      uint64_t peerIdAddr = 0;
      uint8_t peerIdAddrType = 0;

      BbBlePduFiltResultsGetPeerIdAddr(&pAuxScan->filtResults, &peerIdAddr, &peerIdAddrType);

      if (pExtScanCtx->extAdvHdr.advMode & LCTR_ADV_MODE_SCAN_BIT)
      {
        /* Complete the AUX_ADV_IND report for Auxiliary Advertising Event. */

        LlExtAdvReportInd_t * const pAuxAdvRpt = &pExtScanCtx->data.scan.auxAdvRpt;
        if (!lctrExtAdvRptPack(pExtScanCtx, pExtScanCtx->data.scan.auxAdvRpt.eventType, &lctrMstExtScanIsr.advHdr,
                               peerIdAddr, peerIdAddrType , pAuxAdvRpt->rssi, NULL, pAuxAdvRpt))
        {
          LL_TRACE_WARN1("Invalid report parameters; do not receive CHAIN_IND, SID=%u", pExtScanCtx->extAdvHdr.sid);
          pExtScanCtx->data.scan.auxAdvRptState = LCTR_RPT_STATE_IDLE;
          result = FALSE;
          break;
        }

        /* Update report properties for AUX_ADV_IND report. */
        pAuxAdvRpt->eventType &= ~LL_RPT_EVT_SCAN_RSP_BIT;
        pAuxAdvRpt->len = 0;
        pAuxAdvRpt->pData = NULL;

        /* Auxiliary Advertising report is ready; check filter and submit now. */
        if (lctrExtAdvRptPend(pExtScanCtx, pAuxAdvRpt, &pExtScanCtx->data.scan.auxAdvRptState))
        {
          lctrExtAdvRptSubmit(pExtScanCtx, pAuxAdvRpt, &pExtScanCtx->data.scan.auxAdvRptState);
        }
      }

      LlExtAdvReportInd_t * const pAdvRpt = &pExtScanCtx->data.scan.advRpt;
      if (!lctrExtAdvRptPack(pExtScanCtx, lctrMstExtScanIsr.chainExtraEventType, &lctrMstExtScanIsr.advHdr,
                             peerIdAddr, peerIdAddrType, pAuxScan->auxAdvRssi, NULL, pAdvRpt))
      {
        /* Do not start receiving chain. */
        LL_TRACE_WARN1("Invalid report parameters; do not receive CHAIN_IND, SID=%u", pExtScanCtx->extAdvHdr.sid);
        result = FALSE;
        break;
      }

      if (!lctrExtAdvRptPend(pExtScanCtx, pAdvRpt, &pExtScanCtx->data.scan.advRptState))
      {
        /* Do not start receiving chain. */
        result = FALSE;
        break;
      }
    }

    if (!lctrExtAdvDataReassemble(pExtScanCtx, lctrMstExtScanIsr.pAdvData, lctrMstExtScanIsr.advDataLen, pAuxScan->auxAdvRssi))
    {
      /* Do not finish receiving chain. */
      LL_TRACE_WARN1("Insufficient scan buffer size; do not receive CHAIN_IND, SID=%u", pExtScanCtx->extAdvHdr.sid);
      pExtScanCtx->data.scan.advRpt.eventType |= LL_RPT_DATA_INC_TRUNC << LCTR_DATA_STATUS_SHIFT;
      result = FALSE;
      break;
    }

  } while (FALSE);

  if ((result == FALSE) ||                                                   /* Invalid packet, duplicate found or insufficient buffer. */
      ((lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT) == 0))    /* No more auxiliary packet. */
  {
    /* End of auxiliary sequence. */
    lctrExtAdvRptSubmit(pExtScanCtx, &pExtScanCtx->data.scan.advRpt, &pExtScanCtx->data.scan.advRptState);
  }

  return result;
}

/*************************************************************************************************/
/*!
 *  \brief      Legacy discover state advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if scan request transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxLegacyAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  /*** Transmit response PDU processing. ***/

  bool_t txScanReq = FALSE;

  if (pScan->pTxReqBuf)
  {
    switch (pScan->filtResults.pduType)
    {
      case LL_PDU_ADV_IND:
      case LL_PDU_ADV_SCAN_IND:
      {
        if (!pScan->filtResults.peerMatch)
        {
          /* Require peer match. */
          return txScanReq;
        }

        /* Update scan request header with advertiser's address. */
        uint8_t *pScanReqAdvA = pExtScanCtx->reqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
        Bda64ToBstream(pScanReqAdvA, pScan->filtResults.peerAddr);
        pExtScanCtx->reqPduHdr.rxAddrRnd = pScan->filtResults.peerAddrRand;

        /* Save AdvA in the SCAN_REQ here and compare it with the one from SCAN_RSP later. */
        pExtScanCtx->data.scan.scanReqAdvAddr = pScan->filtResults.peerAddr;

        /* Update scan request header with scanner's address. */
        if (pExtScanCtx->scanParam.ownAddrType & LL_ADDR_IDENTITY_BIT)
        {
          bool_t   localAddrRand = BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
          uint64_t localAddr     = pBle->pduFilt.localAddrMatch;
          if (BbBleResListReadLocal(pScan->filtResults.peerIdAddrRand, pScan->filtResults.peerIdAddr, &localAddr))
          {
            localAddrRand = TRUE;
          }
          uint8_t *pScanReqScanA = pExtScanCtx->reqBuf + LL_ADV_HDR_LEN;
          Bda64ToBstream(pScanReqScanA, localAddr);
          pExtScanCtx->reqPduHdr.txAddrRnd = localAddrRand;
        }

        lctrPackAdvbPduHdr(pExtScanCtx->reqBuf, &pExtScanCtx->reqPduHdr);

        /* Check whether backoff is in effect. */
        txScanReq = lctrScanBackoffCheckReqAllowed(pExtScanCtx);
        break;
      }

      default:
        break;
    }
  }

  return txScanReq;
}

/*************************************************************************************************/
/*!
 *  \brief      Scan request Tx completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pReqBuf Transmitted request buffer.
 *
 *  \return     TRUE to signal receive scan response.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverTxLegacyScanReqHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf)
{
  /* Take same action as legacy scanning. */
  return lctrMstScanReqTxCompHandler(pOp, pReqBuf);
}

/*************************************************************************************************/
/*!
 *  \brief      Master legacy scan response receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pRspBuf Received response buffer, NULL if receive failed.
 *
 *  \return     Always return FALSE (ignored by BB).
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverRxLegacyScanRspHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  if (pRspBuf)
  {
    lctrAdvbPduHdr_t advHdr;
    lctrUnpackAdvbPduHdr(&advHdr, pRspBuf);

    /*** Received advertising PDU post-processing. ***/

    switch (pScan->filtResults.pduType)
    {
      /* Valid PDU types. */
      case LL_PDU_SCAN_RSP:
      {
        /* scanReqAdvAddr is assigned when LL_PDU_ADV_SCAN_IND is received. */
        if (pExtScanCtx->data.scan.scanReqAdvAddr != pScan->filtResults.peerAddr)
        {
          LL_TRACE_WARN0("Ignore scan_rsp since advAddr doesn't match the one sent in the scan_req.");
          break;
        }

        uint64_t peerIdAddr = 0;
        uint8_t peerIdAddrType = 0;

        BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &peerIdAddr, &peerIdAddrType);

        LlExtAdvReportInd_t * const pRpt = &pExtScanCtx->data.scan.advRpt;
        if (lctrExtAdvRptPack(pExtScanCtx, 0, &advHdr,
                              peerIdAddr, peerIdAddrType, pScan->advRssi, pRspBuf, pRpt))
        {
          pRpt->secPhy = LL_PHY_NONE;
          if (lctrExtAdvRptPend(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState))
          {
            lctrExtAdvRptSubmit(pExtScanCtx, pRpt, &pExtScanCtx->data.scan.advRptState);
          }
        }
        break;
      }
      default:
        break;
    }

    /*** Scan backoff maintenance ***/

    lctrScanBackoffRspSuccess(pExtScanCtx);
  }
  else
  {
    lctrScanBackoffRspFailed(pExtScanCtx);
  }

  return FALSE;
}

/**************************************************************************************************
  Functions: Discovery BOD handlers
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Reschedule primary scan operation.
 *
 *  \param  pExtScanCtx     Extended scan context.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lctrMstExtDiscoverReschedule(lctrExtScanCtx_t *pExtScanCtx)
{
  BbOpDesc_t *pOp = &pExtScanCtx->scanBod;
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  /*** Reschedule primary operation ***/

  /* Recover primary scan BOD min duration so that its run will be guaranteed in BB. */
  pOp->minDurUsec = LCTR_MIN_SCAN_USEC;

  /* Reset due time to start of scan window. */
  pOp->due = pExtScanCtx->scanWinStart;

  if ((pExtScanCtx->scanParam.scanInterval != pExtScanCtx->scanParam.scanWindow) &&
      ((pScan->elapsedUsec + pOp->minDurUsec) < LCTR_BLE_TO_US(pExtScanCtx->scanParam.scanWindow)))
  {
    const uint32_t min = BB_US_TO_BB_TICKS(pScan->elapsedUsec);
    const uint32_t max = BB_BLE_TO_BB_TICKS(pExtScanCtx->scanParam.scanWindow);

    if (SchInsertEarlyAsPossible(pOp, min, max))
    {
      /* Continue interrupted operation. */
      pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pExtScanCtx->scanWinStart);
      WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
      return;
    }
  }

  /* Advance to next scanInterval. */

  pScan->elapsedUsec = 0;

  /* Compute next channel. */
  pBle->chan.chanIdx = lctrScanChanSelectNext(pBle->chan.chanIdx, pScan->scanChMap);

  if (pExtScanCtx->scanParam.scanInterval == pExtScanCtx->scanParam.scanWindow)
  {
    /* Continuous scan. */
    SchInsertNextAvailable(pOp);
    pExtScanCtx->scanWinStart = pOp->due;
  }
  else
  {
    /* Next scan interval. */
    const uint32_t min = BB_BLE_TO_BB_TICKS(pExtScanCtx->scanParam.scanInterval);
    const uint32_t max = min + BB_BLE_TO_BB_TICKS(pExtScanCtx->scanParam.scanWindow);

    while (TRUE)
    {
      /* Store start of next scan window. */
      pExtScanCtx->scanWinStart = pOp->due + min;

      if (SchInsertEarlyAsPossible(pOp, min, max))
      {
        pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pExtScanCtx->scanWinStart);
        WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
        break;
      }
      else
      {
        /* Advance to next scan window. */
        pOp->due = pExtScanCtx->scanWinStart;

        LL_TRACE_WARN1("!!! Scan schedule conflict at due=%u", pOp->due + min);
        LL_TRACE_WARN1("!!!                           scanWindowUsec=%u", LCTR_BLE_TO_US(pExtScanCtx->scanParam.scanWindow));
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End a discovery scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstExtDiscoverEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  if (pExtScanCtx->shutdown || pExtScanCtx->selfTerm)
  {
    pExtScanCtx->bodTermCnt++;
    if (( pExtScanCtx->auxOpPending && (pExtScanCtx->bodTermCnt >= 2)) ||     /* Wait for both ExtScan and AuxScan operations. */
        (!pExtScanCtx->auxOpPending && (pExtScanCtx->bodTermCnt >= 1)))       /* Wait only for ExtScan operation. */
    {
      lctrSendExtScanMsg(pExtScanCtx, LCTR_EXT_SCAN_MSG_TERMINATE);
    }
    return;
  }

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  /*** Reschedule operation ***/

  if (!pExtScanCtx->auxOpPending)
  {
    lctrMstExtDiscoverReschedule(pExtScanCtx);
  }
  /* else postpone until lctrMstAuxDiscoverEndOp(). */
}

/*************************************************************************************************/
/*!
 *  \brief  End an auxiliary discovery scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstAuxDiscoverEndOp(BbOpDesc_t *pOp)
{
  lctrExtScanCtx_t * const pExtScanCtx = pOp->pCtx;

  if (pExtScanCtx->shutdown || pExtScanCtx->selfTerm)
  {
    lctrSendExtScanMsg(pExtScanCtx, LCTR_EXT_SCAN_MSG_TERMINATE);
    return;
  }

  pExtScanCtx->auxOpPending = FALSE;

  /* Reschedule primary channel scan operation. */
  lctrMstExtDiscoverReschedule(pExtScanCtx);
}

/**************************************************************************************************
  Functions: Periodic packet handlers
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  End an periodic scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstPerScanEndOp(BbOpDesc_t *pOp)
{
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;
  BbBleData_t * const pBle = &pPerScanCtx->bleData;

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_PER_SCAN_EVENT);

  if (pPerScanCtx->cancelCreateSync)
  {
    lctrSendCreateSyncMsg(pPerScanCtx, LCTR_CREATE_SYNC_MSG_TERMINATE);
    return;
  }

  if (pPerScanCtx->shutdown)
  {
    pPerScanCtx->shutdown = FALSE;
    /* Last BOD to terminate; send terminate event. */
    lctrSendPerScanMsg(pPerScanCtx, LCTR_PER_SCAN_MSG_TERMINATE_DONE);
    WsfTimerStop(&pPerScanCtx->tmrSupTimeout);
    return;
  }

  /* Failed to receive AUX_SYNC_IND within the first 6 periodic intervals. */
  if (pPerScanCtx->firstPerAdvRcv == FALSE)
  {
    /* Make sure create sync failed event is sent out after 6th RX is over. */
    uint16_t fastTermCnt = pPerScanCtx->bodAborted ? (LCTR_FAST_TERM_CNT + 1) : LCTR_FAST_TERM_CNT;

    if ((pPerScanCtx->eventCounter - pPerScanCtx->initEventCounter) == (fastTermCnt - 1))
    {
      LL_TRACE_ERR0("!!! lctrMstPerScanEndOp: Failed to receive AUX_SYNC_IND within first 6 intervals.");
      /* Notify create sync state machine with sync failed. */
      lctrSendCreateSyncMsg(pPerScanCtx, LCTR_CREATE_SYNC_MSG_FAILED);
    }
  }

  pPerScanCtx->bodAborted = FALSE;

  /*** Reschedule operation ***/
  uint16_t skip;
  if (lctrMstPerScanIsr.syncWithSlave)
  {
    /* Re-sync with advertiser */
    pPerScanCtx->lastAnchorPoint = lctrMstPerScanIsr.firstRxStartTs;
    lctrMstPerScanIsr.syncWithSlave = FALSE;
    pPerScanCtx->lastActiveEvent = pPerScanCtx->eventCounter;
    /* Reset supervision timer. */
    WsfTimerStartMs(&pPerScanCtx->tmrSupTimeout, pPerScanCtx->syncTimeOutMs);

    if (pPerScanCtx->skipInter &&
        pPerScanCtx->skipInter < BB_US_TO_BB_TICKS(pPerScanCtx->syncTimeOutMs * 1000))
    {
      /* Skip is set and shorter than the sync timeout. */
      skip = pPerScanCtx->skip;
    }
    else
    {
      skip = 1;
    }
  }
  else
  {
    /* Not able to re-sync with advertiser, stop skip. */
    skip = 1;
  }

  uint16_t numUnsyncIntervals     = pPerScanCtx->eventCounter - pPerScanCtx->lastActiveEvent;

  while (TRUE)
  {
    /* Handle Acad if any pending actions are waiting. */
    lctrAcadMsg_t acadMsg;
    acadMsg.hdr.eventCtr = pPerScanCtx->eventCounter;
    acadMsg.hdr.skip = skip;
    acadMsg.hdr.handle = LCTR_GET_PER_SCAN_HANDLE(pPerScanCtx);

    for (uint8_t acadId = 0; acadId < LCTR_ACAD_NUM_ID; acadId++)
    {
      if (pPerScanCtx->acadParams[acadId].hdr.state != LCTR_ACAD_STATE_DISABLED)
      {
        acadMsg.hdr.acadId = acadId;
        lctrPerScanActProcessAcad(&acadMsg);
      }
    }

    pPerScanCtx->eventCounter += skip;
    numUnsyncIntervals        += skip;

    uint32_t unsyncTimeUsec = BB_TICKS_TO_US(pPerScanCtx->perInter * numUnsyncIntervals);
    uint32_t caPpm          = lctrCalcTotalAccuracy(pPerScanCtx->sca);
    uint32_t wwTotalUsec    = lctrCalcWindowWideningUsec(unsyncTimeUsec, caPpm);
    uint32_t wwTotal        = BB_US_TO_BB_TICKS(wwTotalUsec);
    uint32_t connInterUsec  = BB_TICKS_TO_US(numUnsyncIntervals * pPerScanCtx->perInter);
    uint32_t connInter      = BB_US_TO_BB_TICKS(connInterUsec);
    int16_t  dueOffsetUsec  = (connInterUsec - wwTotalUsec) - BB_TICKS_TO_US(connInter - wwTotal);

    /* Advance to next interval. */
    pOp->due = pPerScanCtx->lastAnchorPoint + connInter - wwTotal;
    pOp->dueOffsetUsec = WSF_MAX(dueOffsetUsec, 0);
    pOp->minDurUsec = pPerScanCtx->minDurUsec + wwTotalUsec;
    pBle->op.mstPerScan.rxSyncDelayUsec = pPerScanCtx->rxSyncDelayUsec + (wwTotalUsec << 1);

    pBle->chan.chanIdx = lctrPeriodicSelectNextChannel(&pPerScanCtx->chanParam, pPerScanCtx->eventCounter);

    if (SchInsertAtDueTime(pOp, lctrPerScanResolveConflict))
    {
      break;
    }
    LL_TRACE_WARN1("!!! Periodic scanning schedule conflict eventCounter=%u", pPerScanCtx->eventCounter);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Abort an periodic scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstPerScanAbortOp(BbOpDesc_t *pOp)
{
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;

  pPerScanCtx->bodAborted = TRUE;
  lctrMstPerScanEndOp(pOp);
}

/*************************************************************************************************/
/*!
 *  \brief      Master periodic scan Rx periodic adv handler.
 *
 *  \param      pOp         Originating operation.
 *  \param      pAdvBuf     Received Periodic ADV packet buffer, NULL if receive failed.
 *  \param      status      Reception status.
 *
 *  \return     Next auxiliary offset in microseconds.
 */
/*************************************************************************************************/
uint32_t lctrMstPerScanRxPerAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf, uint8_t status)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_PER_SCAN_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;

  /* BB_STATUS_RX_TIMEOUT or BB_STATUS_CRC_FAILED with pAdvBuf NULL. */
  if (pAdvBuf == NULL)
  {
    /* The report will be submitted in the lctrMstPerScanRxPerAdvPktPostHandler */
    return 0;
  }

  uint8_t       advHdrLen = pAdvBuf[1];
  const uint8_t *pExtAdvHdr = pAdvBuf + LL_ADV_HDR_LEN;
  BbBleMstPerScanEvent_t * const pMstPerScan = &pBle->op.mstPerScan;
  lctrMstExtScanIsr.pAdvData = pExtAdvHdr + lctrUnpackExtAdvHeader(&pPerScanCtx->extAdvHdr, &lctrMstExtScanIsr.extAdvHdrFlags, pExtAdvHdr);
  lctrMstExtScanIsr.advDataLen = advHdrLen - 1 - pPerScanCtx->extAdvHdr.extHdrLen; /* 1 byte for the extended header length + advMode. */

  if (pPerScanCtx->firstPerAdvRcv == FALSE)
  {
    pPerScanCtx->firstPerAdvRcv = TRUE;
    lctrMstPerScanIsr.filtResult = TRUE;
    /* Notify create sync state machine with sync done. */
    lctrSendCreateSyncMsg(pPerScanCtx, LCTR_CREATE_SYNC_MSG_DONE);
    /* Notify periodic scan state machine with sync established. */
    lctrSendPerScanMsg(pPerScanCtx, LCTR_PER_SCAN_MSG_SYNC_ESTD);

    if (lctrPerTransferSync.state == LCTR_TRANSFER_SYNC_STATE_DISCOVER)
    {
      pPerScanCtx->skipInter = pPerScanCtx->perInter * pPerScanCtx->skip;
    }
  }

  /* Store anchor point. */
  if ((!lctrMstPerScanIsr.syncWithSlave) &&
      (pMstPerScan->perIsFirstTs == TRUE) &&
      (status == BB_STATUS_SUCCESS))
  {
    lctrMstPerScanIsr.firstRxStartTs = pMstPerScan->perStartTs;
    lctrMstPerScanIsr.syncWithSlave = TRUE;
  }

  /*** ACAD processing. ***/
  lctrMstAcadHandler(pPerScanCtx);

  /*** Periodic Advertising Data processing. ***/
  uint32_t auxOffsetUsec = 0;
  if (lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT)
  {
    lctrAuxPtr_t auxPtr;
    lctrUnpackAuxPtr(&auxPtr, pPerScanCtx->extAdvHdr.pAuxPtr);

    pBle->chan.chanIdx = auxPtr.auxChIdx;

    lctrMstComputeAuxOffset(&auxPtr, &auxOffsetUsec, &pBle->op.mstPerScan.rxSyncDelayUsec);
  }

  return auxOffsetUsec;
}

/*************************************************************************************************/
/*!
 *  \brief      Master periodic scan Rx periodic adv post handler.
 *
 *  \param      pOp         Originating operation.
 *  \param      pAdvBuf     Advertising buffer.
 *
 *  \return     None
 */
/*************************************************************************************************/
bool_t lctrMstPerScanRxPerAdvPktPostHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  lctrPerScanCtx_t * const pPerScanCtx = pOp->pCtx;
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstPerScanEvent_t * const pMstPerScan = &pBle->op.mstPerScan;
  bool_t result = TRUE;

  if (pPerScanCtx->state == LCTR_PER_SCAN_STATE_SYNC_ESTD)
  {
    WsfTimerStartMs(&pPerScanCtx->tmrSupTimeout, pPerScanCtx->syncTimeOutMs);
  }

  /* BB_STATUS_RX_TIMEOUT or BB_STATUS_CRC_FAILED with pAdvBuf NULL. */
  if (pAdvBuf == NULL)
  {
    /* Send truncated event. */
    if (pPerScanCtx->advRptState == LCTR_RPT_STATE_IN_PROGRESS && pPerScanCtx->state == LCTR_PER_SCAN_STATE_SYNC_ESTD)
    {
      if (lctrPerAdvRptPackTruncate(pOp, pAdvBuf, &pPerScanCtx->advRpt))
      {
        lctrPerAdvRptSubmit(pPerScanCtx);
      }
    }
    return FALSE;
  }

  /*** Report generation. ***/

  if (pPerScanCtx->advRptState == LCTR_RPT_STATE_COMP)
  {
    /* Do not finish receiving chain. */
    LL_TRACE_WARN1("Cannot generate periodic report due to pending report; do not receive CHAIN_IND, SID=%u", pPerScanCtx->extAdvHdr.sid);
    return FALSE;
  }

  do
  {
    if (pPerScanCtx->advRptState == LCTR_RPT_STATE_IDLE)
    {
      /* First PDU in Auxiliary Advertising Event. */

      if (!lctrPerAdvRptPack(pOp, pAdvBuf, lctrMstExtScanIsr.extAdvHdrFlags, &pPerScanCtx->advRpt))
      {
        /* Stop receiving chain. */
        LL_TRACE_WARN1("Invalid report parameters; do not receive CHAIN_IND, SID=%u", pPerScanCtx->extAdvHdr.sid);
        result = FALSE;
        break;
      }

      if (!lctrPerAdvRptPend(pPerScanCtx))
      {
        /* Stop receiving chain. */
        LL_TRACE_WARN1("Filter report; do not receive CHAIN_IND, SID=%u", pPerScanCtx->extAdvHdr.sid);
        result = FALSE;
        break;
      }
    }

    if (!lctrPerAdvDataReassemble(pPerScanCtx, lctrMstExtScanIsr.pAdvData, lctrMstExtScanIsr.advDataLen, pMstPerScan->perAdvRssi))
    {
      /* Stop receiving chain. */
      LL_TRACE_WARN1("Insufficient periodic scan buffer size; do not receive CHAIN_IND, SID=%u", pPerScanCtx->extAdvHdr.sid);
      pPerScanCtx->advRpt.dataStatus = LL_RPT_DATA_INC_TRUNC;
      result = FALSE;
      break;
    }
  } while (FALSE);

  if ((result == FALSE) ||                                              /* Invalid packet, duplicate found or insufficient buffer. */
      ((lctrMstExtScanIsr.extAdvHdrFlags & LL_EXT_HDR_AUX_PTR_BIT) == 0))    /* No more auxiliary packet. */
  {
    /* End of auxiliary sequence. */
    lctrPerAdvRptSubmit(pPerScanCtx);
  }
  return result;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize periodic scan ISR context.
 *
 *  \return     None
 */
/*************************************************************************************************/
void lctrMstPerScanIsrInit(void)
{
  lctrMstPerScanIsr.syncWithSlave = FALSE;
}
