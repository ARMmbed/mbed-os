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

#include "lctr_int_adv_master.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_init_master.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "bb_ble_api_reslist.h"
#include "util/bstream.h"
#include <string.h>
#if (LL_ENABLE_TESTER == TRUE)
extern bool_t bbTxAccAddrShiftMask;
#endif

/*************************************************************************************************/
/*!
 *  \brief  End a discovery scan operation in the master role.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrMstDiscoverEndOp(BbOpDesc_t *pOp)
{
  lctrMstScanCtx_t *pCtx = (lctrMstScanCtx_t *)pOp->pCtx;

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);
  WSF_ASSERT(pCtx);

  BbBleData_t * const pBle = &pCtx->bleData;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  if (pCtx->shutdown || pCtx->selfTerm)
  {
    lctrMsgHdr_t *pMsg;

    /* Send SM a scan termination event. */
    if ((pMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
    {
      if (pCtx == &lctrMstScan)
      {
        pMsg->dispId = LCTR_DISP_SCAN;
        pMsg->event = LCTR_SCAN_MSG_TERMINATE;
      }
      else
      {
        pMsg->dispId = LCTR_DISP_INIT;
        pMsg->event = LCTR_INIT_MSG_TERMINATE;
      }

      WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
    }
    return;
  }

  /*** Update scan data ***/

  /* Update scanner's address. */
  if (lmgrCb.bdAddrRndModScan &&
      (pCtx->scanParam.ownAddrType & LL_ADDR_RANDOM))
  {
    lmgrCb.bdAddrRndModScan = FALSE;

    if ((pCtx->scanParam.ownAddrType & LL_ADDR_IDENTITY_BIT) == 0)
    {
      /* Update ScanA field. */
      uint8_t *pBuf = pCtx->reqBuf + LL_ADV_HDR_LEN;
      BDA64_TO_BSTREAM(pBuf, lmgrCb.bdAddrRnd);         /* update ScanA field */
    }
    pBle->pduFilt.localAddrMatch = lmgrCb.bdAddrRnd;
  }

  /*** Reschedule operation ***/

  /* Reset due time to start of scan window. */
  pOp->due = pCtx->scanWinStart;

  if ((pCtx->scanParam.scanInterval != pCtx->scanParam.scanWindow) &&
      ((pScan->elapsedUsec + pOp->minDurUsec) < LCTR_BLE_TO_US(pCtx->scanParam.scanWindow)))
  {
    const uint32_t min = BB_US_TO_BB_TICKS(pScan->elapsedUsec);
    const uint32_t max = BB_BLE_TO_BB_TICKS(pCtx->scanParam.scanWindow);

    if (SchInsertEarlyAsPossible(pOp, min, max))
    {
      /* Continue interrupted operation. */
      pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pCtx->scanWinStart);
      WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
      return;
    }
  }

  /* Advance to next scanInterval. */

  pScan->elapsedUsec = 0;

  /* Compute next channel. */
  pBle->chan.chanIdx = lctrScanChanSelectNext(pBle->chan.chanIdx, pScan->scanChMap);

  if (pCtx->scanParam.scanInterval == pCtx->scanParam.scanWindow)
  {
    /* Continuous scan. */
    SchInsertNextAvailable(pOp);
    pCtx->scanWinStart = pOp->due;
  }
  else
  {
    /* Next scan interval. */
    const uint32_t min = BB_BLE_TO_BB_TICKS(pCtx->scanParam.scanInterval);
    const uint32_t max = min + BB_BLE_TO_BB_TICKS(pCtx->scanParam.scanWindow);

    while (TRUE)
    {
      /* Store start of next scan window. */
      pCtx->scanWinStart = pOp->due + min;

      if (SchInsertEarlyAsPossible(pOp, min, max))
      {
        pScan->elapsedUsec = BB_TICKS_TO_US(pOp->due - pCtx->scanWinStart);
        WSF_ASSERT(pScan->elapsedUsec < pOp->maxDurUsec);
        break;
      }
      else
      {
        /* Advance to next scan window. */
        pOp->due = pCtx->scanWinStart;

        LL_TRACE_WARN1("!!! Scan schedule conflict at due=%u", pOp->due + min);
        LL_TRACE_WARN1("!!!                           scanWindowUsec=%u", LCTR_BLE_TO_US(pCtx->scanParam.scanWindow));
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Scan discover state advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     TRUE if scan request transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstDiscoverAdvPktHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

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
          txScanReq = FALSE;
          return txScanReq;
        }

        /* Update scan request header with advertiser's address. */
        uint8_t *pScanReqAdvA = lctrMstScan.reqBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
        Bda64ToBstream(pScanReqAdvA, pScan->filtResults.peerAddr);
        lctrMstScan.reqPduHdr.rxAddrRnd = pScan->filtResults.peerAddrRand;

        /* Save AdvA in the SCAN_REQ here and compare it with the one from SCAN_RSP later. */
        lctrMstScan.data.disc.scanReqAdvAddr = pScan->filtResults.peerAddr;

        /* Update scan request header with scanner's address. */
        if (lctrMstScan.scanParam.ownAddrType & LL_ADDR_IDENTITY_BIT)
        {
          bool_t   localAddrRand = BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
          uint64_t localAddr     = pBle->pduFilt.localAddrMatch;
          if (BbBleResListReadLocal(pScan->filtResults.peerIdAddrRand, pScan->filtResults.peerIdAddr, &localAddr))
          {
            localAddrRand = TRUE;
          }
          uint8_t *pScanReqScanA = lctrMstScan.reqBuf + LL_ADV_HDR_LEN;
          Bda64ToBstream(pScanReqScanA, localAddr);
          lctrMstScan.reqPduHdr.txAddrRnd = localAddrRand;
        }

        lctrPackAdvbPduHdr(lctrMstScan.reqBuf, &lctrMstScan.reqPduHdr);

        /* Scan backoff. */
        if (lmgrGetOpFlag(LL_OP_MODE_FLAG_ENA_SCAN_BACKOFF)
#if (LL_ENABLE_TESTER == TRUE)
            && !bbTxAccAddrShiftMask
#endif
            )
        {
          if (lctrMstScan.backoffCount)
          {
            lctrMstScan.backoffCount--;
          }

          if (lctrMstScan.backoffCount == 0)
          {
            txScanReq = TRUE;
          }
        }
        else
        {
          txScanReq = TRUE;
        }
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
 *  \brief      Scan discover state advertising packet receive post process handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pAdvBuf Received advertising buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstDiscoverAdvPktPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  /*** Received advertising PDU post-processing. ***/

  uint8_t *pRxAdvBuf;

  /* The peer match will be enforced in the scan SM. */

  switch (pScan->filtResults.pduType)
  {
    /* Valid PDU types. */
    case LL_PDU_ADV_DIRECT_IND:
    case LL_PDU_ADV_IND:
    case LL_PDU_ADV_NONCONN_IND:
    case LL_PDU_ADV_SCAN_IND:
      if ((lmgrMstScanCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
          ((pRxAdvBuf = WsfMsgAlloc(LCTR_ADVB_BUF_SIZE)) != NULL))
      {
        /* Store data in extra buffer area. */
        pScan->pRxAdvBuf[LCTR_ADVB_BUF_OFFSET_RSSI] = pScan->advRssi;
        pScan->pRxAdvBuf[LCTR_ADVB_BUF_OFFSET_RX_RPA] =
            (pScan->filtResults.peerIdAddrRand != pScan->filtResults.peerAddrRand) ||
            (pScan->filtResults.peerIdAddr     != pScan->filtResults.peerAddr);
        UINT32_TO_BUF(pScan->pRxAdvBuf + LCTR_ADVB_BUF_OFFSET_CRC, pScan->advCrc);

        /* Queue the received AdvB buffer for post-processing. */

        if (pScan->filtResults.localMatch)
        {
          WsfMsgEnq(&lctrMstScan.rxAdvbQ, 0, pScan->pRxAdvBuf);
          WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_ADVB));
          lctrAdvReportsInc();
        }
        else
        {
          WsfMsgEnq(&lctrMstScan.rxDirectAdvbQ, 0, pScan->pRxAdvBuf);
          WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_DIRECT_ADVB));
          lctrAdvReportsInc();
        }

        /* Update scan operation with new allocated AdvB buffer. */
        pScan->pRxAdvBuf = pRxAdvBuf;
      }
      break;

    default:
      break;
  }
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
bool_t lctrMstScanReqTxCompHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleMstAdvEvent_t * const pAdv = &pOp->prot.pBle->op.mstAdv;

  return pAdv->pRxRspBuf ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Master scan response receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pRspBuf Received response buffer, NULL if receive failed.
 *
 *  \return     Always return FALSE (ignored by BB).
 */
/*************************************************************************************************/
bool_t lctrMstScanRspRxCompHandler(BbOpDesc_t *pOp, const uint8_t *pRspBuf)
{
  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pOp->prot.pBle->chan.opType == BB_BLE_OP_MST_ADV_EVENT);

  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

  if (pRspBuf)
  {
    /*** Received advertising PDU post-processing. ***/

    uint8_t *pRxRspBuf;

    switch (pScan->filtResults.pduType)
    {
      /* Valid PDU types. */
      case LL_PDU_SCAN_RSP:

        /* scanReqAdvAddr is assigned when LL_PDU_ADV_SCAN_IND is received. */
        if (lctrMstScan.data.disc.scanReqAdvAddr != pScan->filtResults.peerAddr)
        {
          LL_TRACE_WARN0("Ignore scan_rsp since advAddr doesn't match the one sent in the scan_req.");
          break;
        }

        if ((lmgrMstScanCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
            ((pRxRspBuf = WsfMsgAlloc(LCTR_ADVB_BUF_SIZE)) != NULL))
        {
          /* Store data in extra buffer area. */
          pScan->pRxRspBuf[LCTR_ADVB_BUF_OFFSET_RSSI] = pScan->advRssi;
          pScan->pRxRspBuf[LCTR_ADVB_BUF_OFFSET_RX_RPA] =
              (pScan->filtResults.peerIdAddr != pScan->filtResults.peerAddrRand) &&
              (pScan->filtResults.peerIdAddr != pScan->filtResults.peerAddr);
          UINT32_TO_BUF(pScan->pRxAdvBuf + LCTR_ADVB_BUF_OFFSET_CRC, pScan->advCrc);

          /* Queue the received AdvB buffer for post-processing. */
          WsfMsgEnq(&lctrMstScan.rxAdvbQ, 0, pScan->pRxRspBuf);
          WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_ADVB));
          lctrAdvReportsInc();

          /* Update scan operation with new allocated AdvB buffer. */
          pScan->pRxRspBuf = pRxRspBuf;
        }
        break;

      default:
        break;
    }
  }

  /*** Scan backoff maintenance ***/

  if (pRspBuf)
  {
    if (++lctrMstScan.consRspSuccess >= 2)
    {
      lctrMstScan.upperLimit = lctrMstScan.upperLimit >> 1;           /* divide by 2 */

      if (lctrMstScan.upperLimit == 0)
      {
        lctrMstScan.upperLimit = 1;
      }

      lctrMstScan.consRspSuccess = 0;
    }

    lctrMstScan.consRspFailure = 0;
  }
  else
  {
    if (++lctrMstScan.consRspFailure >= 2)
    {
      lctrMstScan.upperLimit = lctrMstScan.upperLimit << 1;           /* multiply by 2 */

      if (lctrMstScan.upperLimit > 256)
      {
        lctrMstScan.upperLimit = 256;
      }

      lctrMstScan.consRspFailure = 0;
    }

    lctrMstScan.consRspSuccess = 0;
  }

  /* backoffCount = [1..upperLimit] */
  lctrMstScan.backoffCount = (LlMathRandNum() & (lctrMstScan.upperLimit - 1)) + 1;

  return FALSE;
}
