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
 * \brief Link layer controller slave advertising ISR callbacks.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "bb_api.h"
#include "bb_ble_api.h"
#include "ll_math.h"
#include "bb_ble_api_reslist.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_cs.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Scan request packet handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      reqLen  Request packet length.
 *
 *  \return     TRUE if response transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrScanReqHandler(BbOpDesc_t *pOp, uint8_t reqLen)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;

  if (pAdv->pTxRspBuf &&
     (reqLen == LL_SCAN_REQ_PDU_LEN))
  {
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Connection indication packet handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      reqLen  Request packet length.
 *  \param      pReqBuf Received request buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrConnIndHandler(BbOpDesc_t *pOp, uint8_t reqLen, const uint8_t *pReqBuf)
{
  if (lctrMsgDispTbl[LCTR_DISP_CONN] &&
     (reqLen == LL_CONN_IND_PDU_LEN))
  {
    /* Stop advertising. */
    lctrSlvAdv.connIndRcvd = TRUE;
    BbSetBodTerminateFlag();

    /* CONN_IND packet delivered when advertising termination completes.
     * cf. lctrAdvActSelfTerm() */

    /*** Received advertising PDU post-processing. ***/

    if ((lctrSlvAdv.advBuf[0] & (1 << LCTR_ADV_HDR_CH_SEL_SHIFT)) &&   /* local advertiser supports CS#2 */
        (pReqBuf[0] & (1 << LCTR_ADV_HDR_CH_SEL_SHIFT)))               /* peer initiator supports CS#2 */
    {
      lctrSlvAdv.usedChSel = LL_CH_SEL_2;
    }
    else
    {
      lctrSlvAdv.usedChSel = LL_CH_SEL_1;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Slave advertising packet receive completion handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pReqBuf Received request buffer.
 *
 *  \return     TRUE if response transmission is required, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrSlvAdvHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;

  bool_t sendRsp = FALSE;

  /* Unwanted PDUs must have been filtered in the BB. */
  uint8_t pduLen = pAdv->filtResults.pduLen;
  uint8_t pduType = pAdv->filtResults.pduType;
  switch (pduType)
  {
    case LL_PDU_SCAN_REQ:
      /* Require that the peer address matched. */
      if (pAdv->filtResults.peerMatch)
      {
        sendRsp = lctrScanReqHandler(pOp, pduLen);
      }
      break;
    case LL_PDU_CONNECT_IND:
      /* BOD must be terminated before setting up the next operation. */
      lctrConnIndHandler(pOp, pduLen, pReqBuf);
      lctrSlvAdv.reqEndTs = pAdv->reqStartTs +
                            BB_US_TO_BB_TICKS(LCTR_CONN_IND_PKT_1M_US); /* N.B.: may round to an earlier time */

      /* Requirement for peer address match will be enforced in action handler. */
      break;
    default:
      break;
  }

  return sendRsp;
}

/*************************************************************************************************/
/*!
 *  \brief      Slave advertising packet receive post process handler.
 *
 *  \param      pOp     Originating operation.
 *  \param      pReqBuf Received request buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvAdvPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;

  bool_t sendRsp = FALSE;

  /* Unwanted PDUs must have been filtered in the BB. */
  uint8_t pduLen = pAdv->filtResults.pduLen;
  uint8_t pduType = pAdv->filtResults.pduType;
  switch (pduType)
  {
    case LL_PDU_SCAN_REQ:
      /* Require that the peer address matched. */
      if (pAdv->filtResults.peerMatch)
      {
        sendRsp = lctrScanReqHandler(pOp, pduLen);
        /* lctrSlvAdv.reqEndTs = 0; */      /* not needed */
        if (sendRsp && lmgrSlvAdvCb.scanReportEna)
        {
          lctrSlvScanReport_t *pScanReq;
          if ((pScanReq = WsfMsgAlloc(sizeof(lctrSlvScanReport_t))) != NULL)
          {
            pScanReq->scanIdAddr     = pAdv->filtResults.peerIdAddr;
            pScanReq->scanIdAddrType = pAdv->filtResults.peerIdAddrRand;
            pScanReq->scanAddr       = pAdv->filtResults.peerAddr;
            pScanReq->scanAddrType   = pAdv->filtResults.peerAddrRand;
            WsfMsgEnq(&lctrSlvAdv.rxScanReqQ, 0, pScanReq);
            WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_RX_SCAN_REQ));
          }
        }
      }
      break;
    case LL_PDU_CONNECT_IND:
      break;
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End an advertising operation.
 *
 *  \param  pOp     Completed operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrSlvAdvEndOp(BbOpDesc_t *pOp)
{
  BbBleData_t * const pBle = pOp->prot.pBle;
  BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;

  WSF_ASSERT(pOp->protId == BB_PROT_BLE);
  WSF_ASSERT(pBle->chan.opType == BB_BLE_OP_SLV_ADV_EVENT);

  if (lctrSlvAdv.shutdown || lctrSlvAdv.connIndRcvd)
  {
    lctrMsgHdr_t *pMsg;

    /* Send SM an advertising termination event. */
    if ((pMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
    {
      /* pMsg->handle = 0; */           /* not used */
      pMsg->dispId = LCTR_DISP_ADV;
      pMsg->event = LCTR_ADV_MSG_TERMINATE;

      WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
    }

    return;
  }

  /*** Update advertising data ***/

  /* Update local private address. */
  bool_t   update = FALSE;
  uint64_t newAddr = 0;
  if (lmgrCb.bdAddrRndModAdv &&
      (lmgrSlvAdvCb.ownAddrType == LL_ADDR_RANDOM)) /* Should be comparison; not valid for RPA. */
  {
    lmgrCb.bdAddrRndModAdv = FALSE;

    update  = TRUE;
    newAddr = lmgrCb.bdAddrRnd;
  }
  else if (lmgrSlvAdvCb.ownAddrType & LL_ADDR_IDENTITY_BIT)
  {
    uint64_t localRpa;
    if (BbBleResListReadLocal(lmgrSlvAdvCb.advParam.peerAddrType, lmgrSlvAdvCb.advParam.peerAddr, &localRpa))
    {
      update = TRUE;
      newAddr = localRpa;
    }
  }
  if (update)
  {
    uint8_t *pBuf = lctrSlvAdv.advBuf + LL_ADV_HDR_LEN;

    BDA64_TO_BSTREAM(pBuf, newAddr);                  /* update adv PDU */
    memcpy(lctrSlvAdv.scanRspBuf + LL_ADV_HDR_LEN,    /* update scan response PDU */
           lctrSlvAdv.advBuf + LL_ADV_HDR_LEN,
           BDA_ADDR_LEN);
    pBle->pduFilt.localAddrMatch = newAddr;
  }

  /* Update peer address. */
  if ((lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_DIRECT_LOW_DUTY) ||
      (lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_DIRECT_HIGH_DUTY))
  {
    if (BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, PEER_ADDR_MATCH_ENA) &&
        BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, PEER_ADDR_RES_ENA))
    {
      uint64_t peerRpa;

      if (BbBleResListReadPeer(lmgrSlvAdvCb.advParam.peerAddrType, lmgrSlvAdvCb.advParam.peerAddr, &peerRpa))
      {
        uint8_t *pBuf = lctrSlvAdv.advBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN;
        BDA64_TO_BSTREAM(pBuf, peerRpa);                /* update adv PDU */
      }
    }
  }

  /* Update advertising data. */
  if ((lmgrSlvAdvCb.advParam.advType != LL_ADV_CONN_DIRECT_LOW_DUTY) &&
      (lmgrSlvAdvCb.advParam.advType != LL_ADV_CONN_DIRECT_HIGH_DUTY) &&
      lmgrSlvAdvCb.advData.modified)
  {
    lmgrSlvAdvCb.advData.modified = FALSE;
    pAdv->txAdvLen = LL_ADVB_MIN_LEN + lmgrSlvAdvCb.advData.len;
    lctrSlvAdv.advBuf[LL_ADV_HDR_LEN_OFFS] = LL_ADV_PREFIX_LEN + lmgrSlvAdvCb.advData.len;
    memcpy(lctrSlvAdv.advBuf + LL_ADVB_MIN_LEN,
           lmgrSlvAdvCb.advData.buf,
           lmgrSlvAdvCb.advData.len);
#if (LL_ENABLE_TESTER)
    if (pAdv->pTxAdvBuf == llTesterCb.txAdvPdu)
    {
      pAdv->txAdvLen = llTesterCb.txAdvPduLen;        /* restore alternate length */
    }
#endif
  }

  /* Update scan response data. */
  if (lmgrSlvAdvCb.scanRspData.modified)
  {
    lmgrSlvAdvCb.scanRspData.modified = FALSE;
    pAdv->txRspLen = LL_ADVB_MIN_LEN + lmgrSlvAdvCb.scanRspData.len;
    lctrSlvAdv.scanRspBuf[LL_ADV_HDR_LEN_OFFS] = LL_SCAN_PREFIX_LEN + lmgrSlvAdvCb.scanRspData.len;
    memcpy(lctrSlvAdv.scanRspBuf + LL_ADVB_MIN_LEN,
           lmgrSlvAdvCb.scanRspData.buf,
           lmgrSlvAdvCb.scanRspData.len);

#if (LL_ENABLE_TESTER)
    if (pAdv->pTxRspBuf == llTesterCb.txScanRspPdu)
    {
      pAdv->txRspLen = llTesterCb.txScanRspPduLen;    /* restore alternate length */
    }
#endif
  }
  SchBleCalcAdvOpDuration(pOp, 0);

  /*** Reschedule operation ***/

  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_UNDIRECT:
    case LL_ADV_SCAN_UNDIRECT:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
    case LL_ADV_NONCONN_UNDIRECT:
    {
      bool_t result;

      do
      {
        if (lmgrGetOpFlag(LL_OP_MODE_FLAG_ENA_ADV_DLY))
        {
          /* maxDelay is 16, it times 625 still fits in uint32_t. */
          /* coverity[overflow_before_widen] */
          pOp->due += BB_BLE_TO_BB_TICKS(lctrCalcAdvDelay());
        }

        if (lmgrSlvAdvCb.advParam.advInterMin == lmgrSlvAdvCb.advParam.advInterMax)
        {
          pOp->due += lmgrSlvAdvCb.advParam.advInterMin;
          result = SchInsertAtDueTime(pOp, NULL);
        }
        else
        {
          result = SchInsertEarlyAsPossible(pOp,
                                            lmgrSlvAdvCb.advParam.advInterMin,
                                            lmgrSlvAdvCb.advParam.advInterMax);
          if (!result)
          {
            pOp->due += lmgrSlvAdvCb.advParam.advInterMax;
          }
        }

        if (!result)
        {
          LL_TRACE_WARN1("!!! Adv schedule conflict at due=%u", pOp->due);
          LL_TRACE_WARN1("!!!                          minDurUsec=%u", pOp->minDurUsec);
        }

      } while (!result);

      break;
    }
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    {
      uint32_t advEventStart = pOp->due;
      uint32_t advTermCntDown = lmgrSlvAdvCb.advTermCntDown;
      bool_t result = FALSE;

      while (!result && lmgrSlvAdvCb.advTermCntDown)
      {
        result = SchInsertLateAsPossible(pOp, lmgrSlvAdvCb.advParam.advInterMin, lmgrSlvAdvCb.advParam.advInterMax);
        if (!result)
        {
          pOp->due += lmgrSlvAdvCb.advParam.advInterMax;
        }

        uint32_t advEventDur = pOp->due - advEventStart;

        if ((advEventDur + lmgrSlvAdvCb.advParam.advInterMax) < advTermCntDown)
        {
          lmgrSlvAdvCb.advTermCntDown = advTermCntDown - advEventDur;
        }
        else
        {
          /* Terminate at end of next advertising event. */
          lmgrSlvAdvCb.advTermCntDown = 0;
        }
      }

      if (!result && !lmgrSlvAdvCb.advTermCntDown)
      {
        lctrMsgHdr_t *pMsg;

        /* Send SM an advertising termination event. */
        if ((pMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
        {
          /* pMsg->handle = 0; */           /* not used */
          pMsg->dispId = LCTR_DISP_ADV;
          pMsg->event = LCTR_ADV_MSG_TERMINATE;

          WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
        }
      }
      break;
    }
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Compute a random advDelay value between 0 and 10 ms.
 *
 *  \return Value for advDelay in 625 us ticks.
 */
/*************************************************************************************************/
uint32_t lctrCalcAdvDelay(void)
{
  uint32_t randNum = LlMathRandNum();

  /* Compute "randNum % (maxDelay + 1)" with no division, where maxDelay=16 (10ms). */
  return (randNum & 0xF) + ((randNum >> 4) & 1);
}

