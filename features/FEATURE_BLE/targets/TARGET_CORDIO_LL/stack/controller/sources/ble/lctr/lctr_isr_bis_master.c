/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master BIG ISR callbacks.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#include "lctr_int_bis_master.h"
#include "lctr_int_iso.h"
#include "ll_defs.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      BIS master ISR control block. */
static struct
{
  lctrSeCtx_t se;           /*!< Subevent context. */
  uint8_t numRcvdPkt;       /*!< Number of successfully received packets. */
  bool_t firstRxComp;       /*!< First Rx in ISO Event completion flag. */
  bool_t cstf;              /*!< BIG Event's valid Control Subevent Transmission Flag. */
  uint8_t cssn;             /*!< BIG Event's valid Control Subevent Sequence Number. */
  uint32_t nextSeOffs;      /*!< Next subevent time. */
  PalBbBleChan_t *pNextChan;/*!< Next subevent channel. */
  uint8_t *pCtrlBuf;        /*!< Received BIS Control PDU buffer. */
  uint8_t *pDataBuf[LL_MAX_BIS][LL_MAX_BN];
                            /*!< Received data buffers. */
} lctrMstBisIsr;

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Update loop counters for sequential packing.
 *
 *  \param  pBigCtx     BIG context.
 *
 *  \return TRUE if more Rx pending, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrMstBisLoopCounterSequential(lctrBigCtx_t *pBigCtx)
{
  size_t numSePkts = (pBigCtx->bn * pBigCtx->irc) + lctrMstBisIsr.se.ptIdx;

  /* BIG Event limit (beyond BIS Control SubEvent). */

  if (lctrMstBisIsr.se.bisEvtIdx >= pBigCtx->numBis)
  {
    return FALSE;
  }

  if (!lctrSlvBisCalcNextIdxSequential(pBigCtx, &lctrMstBisIsr.se, numSePkts))
  {
    if (!lctrMstBisIsr.cstf)
    {
      return FALSE;
    }
  }

  /* Compute next channel. */
  lctrSeCtx_t nextSe = lctrMstBisIsr.se;
  if (lctrSlvBisCalcNextIdxSequential(pBigCtx, &nextSe, numSePkts))
  {
    lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[nextSe.bisEvtIdx]->chan;
  }
  else
  {
    lctrMstBisIsr.pNextChan = &pBigCtx->ctrChan;
  }

  /* Compute next packet time. */
  lctrMstBisIsr.nextSeOffs += pBigCtx->subInterUsec;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Update loop counters for interleaved packing.
 *
 *  \param  pBigCtx     BIG context.
 *
 *  \return TRUE if more Rx pending, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrMstBisLoopCounterInterleaved(lctrBigCtx_t *pBigCtx)
{
  size_t numSePkts = (pBigCtx->bn * pBigCtx->irc) + lctrMstBisIsr.se.ptIdx;

  /* BIG Event limit (beyond BIS Control SubEvent). */

  if (numSePkts > pBigCtx->nse)
  {
    return FALSE;
  }

  if (!lctrSlvBisCalcNextIdxInterleaved(pBigCtx, &lctrMstBisIsr.se, numSePkts))
  {
    if (!lctrMstBisIsr.cstf)
    {
      return FALSE;
    }
  }

  /* Compute next channel. */
  lctrSeCtx_t nextSe = lctrMstBisIsr.se;
  if (lctrSlvBisCalcNextIdxInterleaved(pBigCtx, &nextSe, numSePkts))
  {
    lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[nextSe.bisEvtIdx]->chan;
  }
  else
  {
    lctrMstBisIsr.pNextChan = &pBigCtx->ctrChan;
  }

  /* Compute next packet time. */
  lctrMstBisIsr.nextSeOffs += pBigCtx->bisSpaceUsec;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Setup a receive PDU.
 *
 *  \param  pBigCtx     BIG context.
 *  \param  reAcqSync   Re-acquisition of synchronization is needed.
 *  \param  pBuf        Unused buffer from last receive operation.
 *
 *  \return TRUE if successful, FALSE if out of memory.
 */
/*************************************************************************************************/
static bool_t lctrMstBisRxData(lctrBigCtx_t *pBigCtx, bool_t reAcqSync, uint8_t *pBuf)
{
  if ((pBuf == NULL) &&
      ((pBuf = lctrBisRxIsoDataPduAlloc()) == NULL))
  {
    LL_TRACE_WARN2("BIG event flow controlled, bigHandle=%u, ec[15:0]=%u", pBigCtx->handle, pBigCtx->eventCounter);

    /* Flow control BIG Event. */
    return FALSE;
  }

  /*** Commit transmission ***/

  lctrBisCtx_t * const pBisCtx = pBigCtx->pBisCtx[lctrMstBisIsr.se.bisEvtIdx];
  BbBleBisRxData(pBuf, LL_ISO_DATA_HDR_LEN + BB_DATA_PLD_MAX_LEN,
                 pBigCtx->roleData.mst.anchorPoint + lctrMstBisIsr.nextSeOffs,
                 lctrMstBisIsr.pNextChan,
                 reAcqSync);

  /*** Post-commit calculation ***/

  /* Now that channel data is set in BB, compute next channel information. */
  pBisCtx->chan.chanIdx = LmgrSelectNextSubEvtChannel(&pBisCtx->chSelInfo);

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  BIG Control PDU decrypt.
 *
 *  \param  pBigCtx     BIG context.
 *
 *  \return TRUE if successful, FALSE for MIC failure.
 */
/*************************************************************************************************/
static bool_t lctrMstBigControlDecrypt(lctrBigCtx_t *pBigCtx)
{
  if (pBigCtx->encrypt && lctrPktDecryptHdlr)
  {
    uint64_t pktCtr = pBigCtx->eventCounter * pBigCtx->bn;
    pBigCtx->ctrChan.enc.pRxPktCounter = &pktCtr;

    if (!lctrPktDecryptHdlr(&pBigCtx->ctrChan.enc, lctrMstBisIsr.pCtrlBuf))
    {
      LL_TRACE_ERR0("!!! MIC verification failed on BIS Control PDU");
      lctrBisRxIsoDataPduFree(lctrMstBisIsr.pCtrlBuf);
      lctrMstBisIsr.pCtrlBuf = NULL;
      lctrMstBigSendMsg(pBigCtx, LCTR_MST_BIG_INT_MIC_FAILED);
      return FALSE;
    }
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  BIG Control PDU post processing.
 *
 *  \param  pBigCtx     BIG context.
 */
/*************************************************************************************************/
static void lctrMstBigControlPostProcess(lctrBigCtx_t *pBigCtx)
{
  uint8_t opcode = lctrMstBisIsr.pCtrlBuf[LL_DATA_HDR_LEN];

  if (!pBigCtx->bcp.actMsk)
  {
    if (opcode < LL_BIG_OPCODE_MAX)
    {
      switch (opcode)
      {
      case LL_BIG_OPCODE_CHAN_MAP_IND:
        lctrBisUnpackBigChannelMapInd(&pBigCtx->bcp.chanMapUpd.chanMap,
                                      &pBigCtx->bcp.chanMapUpd.inst,
                                      lctrMstBisIsr.pCtrlBuf + LL_DATA_HDR_LEN);

        if (((uint16_t)(pBigCtx->bcp.chanMapUpd.inst - (uint16_t)pBigCtx->eventCounter)) < 0x8000)
        {
          pBigCtx->bcp.actMsk |= 1 << opcode;
          LL_TRACE_INFO2("BIG Channel Map Procedure, bigHandle=%u, instant=%u", pBigCtx->handle, pBigCtx->bcp.chanMapUpd.inst);
        }
        else
        {
          /* Ignore error attempt to remain connected as long as possible. */
          LL_TRACE_WARN1("Invalid BIG Channel Map parameters, bigHandle=%u", pBigCtx->handle);
        }

        break;

      case LL_BIG_OPCODE_BIG_TERM_IND:
        lctrBisUnpackBigTerminateInd(&pBigCtx->bcp.term.reason,
                                     &pBigCtx->bcp.term.inst,
                                     lctrMstBisIsr.pCtrlBuf + LL_DATA_HDR_LEN);

        if (((uint16_t)(pBigCtx->bcp.term.inst - (uint16_t)pBigCtx->eventCounter)) < 0x8000)
        {
          pBigCtx->bcp.actMsk |= 1 << opcode;
          LL_TRACE_INFO2("BIG Terminate Procedure, bigHandle=%u, instant=%u", pBigCtx->handle, pBigCtx->bcp.term.inst);
        }
        else
        {
          /* Terminate regardless of error. */
          pBigCtx->bcp.term.reason = LL_ERROR_CODE_INVALID_LMP_PARAMS;
          lctrMstBigSendMsg(pBigCtx, LCTR_MST_BIG_PDU_TERM);
          LL_TRACE_WARN1("Invalid BIG Terminate parameters, bigHandle=%u", pBigCtx->handle);
        }

        break;

      default:
        break;
      }
    }
  }
  else
  {
    LL_TRACE_WARN2("Unexpected overlapped BIS Control Procedure, bigHandle=%u, opcode=%u", pBigCtx->handle, opcode);
  }

  lctrBisRxIsoDataPduFree(lctrMstBisIsr.pCtrlBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  BIG Control Procedure handler.
 *
 *  \param  pBigCtx     BIG context.
 */
/*************************************************************************************************/
static void lctrMstBigControlProcedureHandler(lctrBigCtx_t *pBigCtx)
{
  if (pBigCtx->bcp.actMsk)
  {
    uint16_t inst = (pBigCtx->eventCounter + 1) & 0xFFFF;

    if ((pBigCtx->bcp.actMsk & (1 << LL_BIG_OPCODE_CHAN_MAP_IND)) &&
        (inst == pBigCtx->bcp.chanMapUpd.inst))
    {
      pBigCtx->bcp.actMsk &= ~(1 << LL_BIG_OPCODE_CHAN_MAP_IND);

      lctrRemapBigChannels(pBigCtx, pBigCtx->bcp.chanMapUpd.chanMap);

      LL_TRACE_INFO2("Updated channel map: bigHandle=%u inst=%u", pBigCtx->handle, inst);
    }

    if (pBigCtx->bcp.actMsk & (1 << LL_BIG_OPCODE_BIG_TERM_IND))
    /* Does not need to wait for instant. */
    {
      /* pBigCtx->bcp.actMsk &= ~(1 << LL_BIG_OPCODE_BIG_TERM_IND); */   /* Not needed. */
      lctrMstBigSendMsg(pBigCtx, LCTR_MST_BIG_PDU_TERM);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  BIS Rx Data PDU consumer.
 *
 *  \param  pBigCtx     BIG context.
 *  \param  rxEvtCtr    Received event counter.
 */
/*************************************************************************************************/
static void lctrMstBisRxDataPduHandler(lctrBigCtx_t *pBigCtx, uint32_t rxEvtCtr)
{
  uint8_t numHandles = 0;
  uint16_t handles[LL_MAX_BIS] = { 0 };
  uint16_t numSdu[LL_MAX_BIS] = { 0 };
  uint64_t pktCtrBase = rxEvtCtr * pBigCtx->bn;
  lctrIsoalRxCtx_t *pRxCtx;

  for (unsigned int i = 0; i < pBigCtx->numBis; i++)
  {
    lctrBisCtx_t * const pBisCtx = pBigCtx->pBisCtx[i];
    pRxCtx = &pBisCtx->roleData.mst.isoalRxCtx;

    uint8_t *pDataPdu;
    uint8_t pldCtrOffs;

    while ((pDataPdu = lctrBisDequeueRxDataPdu(pBisCtx, &pldCtrOffs)) != NULL)
    {
      uint64_t pktCtr = pktCtrBase + pldCtrOffs;
      pBisCtx->chan.enc.pRxPktCounter = &pktCtr;

      if (lctrPktDecryptHdlr)
      {
        if (!lctrPktDecryptHdlr(&pBisCtx->chan.enc, pDataPdu))
        {
          LL_TRACE_ERR1("!!! MIC verification failed on bisHandle=%u", pBisCtx->handle);
          lctrBisRxIsoDataPduFree(pDataPdu);
          lctrMstBigSendMsg(pBigCtx, LCTR_MST_BIG_INT_MIC_FAILED);
          return;
        }
      }

      if (pBisCtx->pBigCtx->framing == LL_ISO_PDU_TYPE_UNFRAMED)
      {
        /* Store payload offset for test transit. */
        pBisCtx->test.util.unframed.payloadOffset = pldCtrOffs;

        lctrBisDataPduHdr_t pduHdr;
        lctrBisUnpackDataPduHdr(&pduHdr, pDataPdu);

        /*** Assemble ISO SDU ***/

        uint8_t * const pIsoSdu = pDataPdu - LCTR_ISO_SDU_START_OFFSET;

        switch (pBisCtx->path)
        {
          case LL_ISO_DATA_PATH_HCI:
          {
            lctrIsoHdr_t isoHdr =
            {
              /* ISO header */
              .handle = pBisCtx->handle,
              .len    = pduHdr.len,
              /* .pb  = 0, */  /* assigned below */
              .tsFlag = FALSE,

              /* Data load */
              .ts     = 0,
              .pktSn  = pktCtr,
              .sduLen = pduHdr.len,
              .ps     = LCTR_PS_VALID
            };

            if (pBigCtx->lastPduMissed)
            {
              pRxCtx->data.unframed.ps = LCTR_PS_INVALID;
            }

            switch (pBisCtx->lastLlid)
            {
              case LL_LLID_ISO_UNF_END_PDU:
              {
                switch (pduHdr.llid)
                {
                case LL_LLID_ISO_UNF_END_PDU:
                default:
                  isoHdr.pb = LCTR_PB_COMP;
                  isoHdr.tsFlag = TRUE;
                  break;
                case LL_LLID_ISO_UNF_CONT_PDU:
                  isoHdr.pb = LCTR_PB_FIRST;
                  isoHdr.tsFlag = TRUE;
                  break;
                }
                break;
              }
              case LL_LLID_ISO_UNF_CONT_PDU:
              {
                switch (pduHdr.llid)
                {
                case LL_LLID_ISO_UNF_END_PDU:
                default:
                  isoHdr.pb = LCTR_PB_LAST;
                  break;
                case LL_LLID_ISO_UNF_CONT_PDU:
                  isoHdr.pb = LCTR_PB_CONT;
                  break;
                }
                break;
              }
              default:
                LL_TRACE_WARN2("Unexpected LLID recieved; dropping Rx ISO PDU, bigHandle=%u, pktCtr[15:0]=%u", pBigCtx->handle, pktCtr);
                WsfMsgFree(pIsoSdu);
                continue;
            }

            pBisCtx->lastLlid = pduHdr.llid;

            uint8_t hdrLen;
            hdrLen = lctrIsoPackHdr(pIsoSdu, &isoHdr);
            /* Adjust SDU payload to start immediately after the header. */
            memmove(pIsoSdu + hdrLen, pDataPdu + LL_ISO_DATA_HDR_LEN, isoHdr.sduLen);

            /* Move to temporary queue to collect SDU fragments until complete. */
            if (!lctrIsoUnframedRxSduPendQueue(pRxCtx, pIsoSdu, pBisCtx->handle, isoHdr.sduLen, pduHdr.llid))
            {
              break;
            }

            /* Complete SDU ready to go out. Empty the temp queue and send it out. */
            uint8_t handlerId;
            uint8_t *pSduFrag;
            while ((pSduFrag = WsfMsgDeq(&pRxCtx->data.unframed.pendSduQ, &handlerId)) != NULL)
            {
              lctrBisRxIsoSduEnq(pBisCtx, pSduFrag);
              numSdu[numHandles]++;
            }
          break;
          }

          case LL_ISO_DATA_PATH_VS:
          {
            WSF_ASSERT(lctrCodecHdlr.out);
            lctrCodecHdlr.out(pBisCtx->handle, pDataPdu + LL_ISO_DATA_HDR_LEN, pDataPdu[LCTR_ISO_DATA_PDU_LEN_OFFSET], pktCtr);
            WsfMsgFree(pIsoSdu);  /* TODO Resolve PDU free in Audio layer */
            break;
          }

          default:
          {
            LL_TRACE_WARN2("Data path disabled; dropping Rx ISO PDU, bigHandle=%u, pktCtr[15:0]=%u", pBigCtx->handle, pktCtr);
            WsfMsgFree(pIsoSdu);
            break;
          }
        }
      }
      else /* LL_ISO_PDU_TYPE_FRAMED */
      {
        switch (pBisCtx->path)
        {
          case LL_ISO_DATA_PATH_HCI:
          {
            numSdu[numHandles] += lctrAssembleRxFramedSdu(&pBisCtx->roleData.mst.isoalRxCtx, &pBisCtx->roleData.mst.rxIsoSduQ,
                                                          pBisCtx->handle, pDataPdu, pDataPdu[LCTR_ISO_DATA_PDU_LEN_OFFSET]);

            break;
          }
          case LL_ISO_DATA_PATH_VS:
          {
            WSF_ASSERT(lctrCodecHdlr.out);

            lctrAssembleRxFramedSdu(&pBisCtx->roleData.mst.isoalRxCtx, &pBisCtx->roleData.mst.rxIsoSduQ,
                                    pBisCtx->handle, pDataPdu, pDataPdu[LCTR_ISO_DATA_PDU_LEN_OFFSET]);

            uint8_t *pIsoBuf;
            while ((pIsoBuf = lctrBisRxIsoSduDeq(pBisCtx)) != NULL)
            {
              lctrIsoHdr_t isoHdr;
              lctrIsoUnpackHdr(&isoHdr, pIsoBuf);

              lctrCodecHdlr.out(pBisCtx->handle, isoHdr.pSdu, isoHdr.sduLen, pktCtr);
              WsfMsgFree(pIsoBuf);  /* TODO Resolve PDU free in Audio layer */
            }
            break;
          }
          default:
          {
            LL_TRACE_WARN2("Data path disabled; dropping Rx ISO PDU, bigHandle=%u, pktCtr[15:0]=%u", pBigCtx->handle, pktCtr);
            break;
          }
        }
        lctrBisRxIsoDataPduFree(pDataPdu);
      }
    }

    if (numSdu[numHandles])
    {
      if (!pBisCtx->test.enabled)
      {
        handles[numHandles] = pBisCtx->handle;
        numHandles++;
      }
      else
      {
        numSdu[numHandles] = 0;
      }
    }
  }

  if (numHandles)
  {
    /* Notify host received SDUs. */
    lmgrPersistCb.recvIsoPendCback(numHandles, handles, numSdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  BIS Rx Test payload consumer.
 *
 *  \param  pBigCtx     BIG context.
 *  \param  rxEvtCtr    Received event counter.
 */
/*************************************************************************************************/
static void lctrMstBisRxTestPduHandler(lctrBigCtx_t *pBigCtx, uint32_t rxEvtCtr)
{
  for (unsigned int i = 0; i < pBigCtx->numBis; i++)
  {
    lctrBisCtx_t * const pBisCtx = pBigCtx->pBisCtx[i];

    /* Consume data for RX test. */
    if (pBisCtx->test.enabled)
    {
      if (!pBisCtx->test.term)
      {
        uint8_t *pTestBuf;
        uint32_t expPayloadCtr;

        lctrIsoHdr_t isoHdr;
        while ((pTestBuf = lctrBisRxIsoSduDeq(pBisCtx)) != NULL)
        {
          lctrIsoUnpackHdr(&isoHdr, pTestBuf);

          if (pBigCtx->framing == LL_ISO_PDU_TYPE_FRAMED)
          {
            if (pBisCtx->test.pendInit)
            {
              uint8_t *pTestDataBuf = (uint8_t *) isoHdr.pSdu;
              BSTREAM_TO_UINT32(pBisCtx->test.util.framed.payloadCtr, pTestDataBuf);
              LL_TRACE_INFO1("BIS Framed payload counter initialized to %d", pBisCtx->test.util.framed.payloadCtr);
              pBisCtx->test.pendInit = FALSE;
            }

            LL_TRACE_INFO1("COUNTER = %d", pBisCtx->test.util.framed.payloadCtr);
            expPayloadCtr = pBisCtx->test.util.framed.payloadCtr++;
          }
          else /* LL_ISO_PDU_TYPE_UNFRAMED */
          {
            expPayloadCtr = (rxEvtCtr * pBigCtx->bn) +
                            pBisCtx->test.util.unframed.payloadOffset;
          }

          lctrValidateIsoTestData((uint8_t *)isoHdr.pSdu, isoHdr.sduLen, &pBisCtx->roleData.mst.stats,
                                  pBisCtx->test.pldType, pBigCtx->maxSdu, expPayloadCtr);
          WsfMsgFree(pTestBuf);
          lctrIsoDataRxIncAvailBuf(1);
        }
      }

      else
      {
        /* Terminate ISO Test mode operation. */
        pBisCtx->test.enabled = FALSE;
        pBisCtx->test.term = FALSE;
        pBisCtx->test.pendInit = FALSE;
        pBisCtx->test.util.framed.payloadCtr = 0;
      }
    }
  }
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Complete a received BIS data buffer.
 *
 *  \param  pOp         Operation context.
 *  \param  pBuf        Receive buffer.
 *  \param  status      Receive status.
 */
/*************************************************************************************************/
void lctrMstBisRxCompletion(BbOpDesc_t *pOp, uint8_t *pBuf, uint8_t status)
{
  lctrBigCtx_t * const pBigCtx = pOp->pCtx;
  BbBleData_t * const pBle = &pBigCtx->bleData;
  BbBleMstBisEvent_t * const pBis = &pBle->op.mstBis;

  bool_t reAcqTrain = FALSE;

  if (status == BB_STATUS_SUCCESS)
  {
    lctrMstBisIsr.numRcvdPkt++;
  }

  if (pBigCtx->state != LCTR_MST_BIG_STATE_SYNCED)
  {
    BbSetBodTerminateFlag();
    goto RxPostProcess;
  }

  switch (status)
  {
  case BB_STATUS_FAILED:
  case BB_STATUS_CANCELED:
    /* Continue to next BIG Event. */
    BbSetBodTerminateFlag();
    goto RxPostProcess;

  default:
    break;
  }

  /* Synchronize BIS Event timing. */

  if (!lctrMstBisIsr.firstRxComp)
  {
    lctrMstBisIsr.firstRxComp = TRUE;

    switch (status)
    {
    case BB_STATUS_SUCCESS:
    case BB_STATUS_CRC_FAILED:
      /* Store peer's timing info for synchronizing next BIS Events and ISO Events. */
      pBigCtx->roleData.mst.anchorPoint = pBigCtx->roleData.mst.rxSyncTime = pBis->startTsUsec;
      break;

    default:
      break;
    }
  }

  /* Handle subevent flags. */

  lctrBisDataPduHdr_t hdr;
  lctrBisUnpackDataPduHdr(&hdr, pBuf);

  switch (status)
  {
  case BB_STATUS_SUCCESS:
    lctrMstBisIsr.cstf = hdr.cstf;
    lctrMstBisIsr.cssn = hdr.cssn;
    break;

  default:
    break;
  }

  /* Pend BIS PDU. */

  if ((status == BB_STATUS_SUCCESS) &&
      !(hdr.llid == LL_LLID_BIG_CTRL_PDU) &&
      !((hdr.llid == LL_LLID_ISO_EMPTY_PDU) && (hdr.len == 0)) &&                               /* Ignore Empty PDU */
      (lctrMstBisIsr.pDataBuf[lctrMstBisIsr.se.bisEvtIdx][lctrMstBisIsr.se.burstIdx] == NULL))  /* Ignore duplicate */
  {
    /* Pend unique BIS PDU reception. */
    lctrMstBisIsr.pDataBuf[lctrMstBisIsr.se.bisEvtIdx][lctrMstBisIsr.se.burstIdx] = pBuf;
    pBuf = NULL;
  }

  /* Handle failures. */

  switch (status)
  {
  case BB_STATUS_RX_TIMEOUT:
    /* Re-acquire the receive train. */
    BbBleBisRxDataReAcq(pBigCtx->roleData.mst.anchorPoint + lctrMstBisIsr.nextSeOffs,
                        lctrMstBisIsr.pNextChan);
    reAcqTrain = TRUE;
    pBigCtx->lastPduMissed = TRUE;
    break;

  default:
    break;
  }

  /* Adjust loop counters. */

  switch (pBigCtx->packing)
  {
  case LL_PACKING_INTERLEAVED:
    if (!lctrMstBisLoopCounterInterleaved(pBigCtx))
    {
      /* Continue to next BIG Event. */
      BbSetBodTerminateFlag();
      goto RxPostProcess;
    }
    break;

  case LL_PACKING_SEQUENTIAL:
  default:
    if (!lctrMstBisLoopCounterSequential(pBigCtx))
    {
      /* Continue to next BIG Event. */
      BbSetBodTerminateFlag();
      goto RxPostProcess;
    }
    break;
  }

  /* Receive next SubEvent. */

  if (lctrMstBisRxData(pBigCtx, reAcqTrain, pBuf))
  {
    pBuf = NULL;
  }
  else
  {
    BbSetBodTerminateFlag();
  }

  /* Now that next Rx is setup, process the received packet. */
RxPostProcess:

  switch (status)
  {
  case BB_STATUS_SUCCESS:
    switch (hdr.llid)
    {
    case LL_LLID_BIG_CTRL_PDU:
      if (lctrMstBisIsr.pCtrlBuf != NULL)
      {
        LL_TRACE_WARN1("Unexpected multiple BIS Control PDU received in a single BIG Event, bigHandle=%u", pBigCtx->handle);
      }
      else if (hdr.cssn == pBigCtx->bcp.cssn)
      {
        LL_TRACE_WARN1("Unexpected duplicate BIS Control PDU recieved, bigHandle=%u", pBigCtx->handle);
      }
      else
      {
        /* Do not receive future duplicate PDUs. */
        pBigCtx->bcp.cssn = hdr.cssn;

        lctrMstBisIsr.pCtrlBuf = pBuf;
        pBuf = NULL;
        /* Buffer freed in lctrMstBigEndOp(). */
      }
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }

  if (pBuf)
  {
    /* Free unconsumed Data PDU. */
    lctrBisRxIsoDataPduFree(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Begin a BIG operation.
 *
 *  \param  pOp     Begin operation.
 */
/*************************************************************************************************/
void lctrMstBigBeginOp(BbOpDesc_t *pOp)
{
  lctrBigCtx_t * const pBigCtx = pOp->pCtx;

  if (pBigCtx->state != LCTR_MST_BIG_STATE_SYNCED)
  {
    BbSetBodTerminateFlag();
    return;
  }

  memset(&lctrMstBisIsr, 0, sizeof(lctrMstBisIsr));

  /* Compute next packet time. */
  switch (pBigCtx->packing)
  {
    case LL_PACKING_INTERLEAVED:
      if (pBigCtx->numBis > 1)
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[1]->chan;
      }
      else if (pBigCtx->nse > 1)
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[0]->chan;
      }
      else
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->ctrChan;
      }

      lctrMstBisIsr.nextSeOffs = pBigCtx->bisSpaceUsec;
      break;

    case LL_PACKING_SEQUENTIAL:
    default:
      if (pBigCtx->nse > 1)
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[0]->chan;
      }
      else if (pBigCtx->numBis > 1)
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->pBisCtx[1]->chan;
      }
      else
      {
        lctrMstBisIsr.pNextChan = &pBigCtx->ctrChan;
      }

      lctrMstBisIsr.nextSeOffs = pBigCtx->subInterUsec;
      break;
  }

  lctrMstBisIsr.cstf = TRUE;    /* receive Control PDU by default */
  lctrMstBisIsr.cssn = 0xFF;

  if (!lctrMstBisRxData(pBigCtx, FALSE, NULL))
  {
    BbSetBodTerminateFlag();
    return;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  End a BIS operation.
 *
 *  \param  pOp     Completed operation.
 */
/*************************************************************************************************/
void lctrMstBigEndOp(BbOpDesc_t *pOp)
{
  lctrBigCtx_t * const pBigCtx = pOp->pCtx;
  BbBleData_t * const pBle = &pBigCtx->bleData;
  BbBleMstBisEvent_t * const pBis = &pBle->op.mstBis;
  if (pBigCtx->state != LCTR_MST_BIG_STATE_SYNCED)
  {
    lctrMstBigSendMsg(pBigCtx, LCTR_MST_BIG_INT_TERMINATED_SYNC);
    WsfTimerStop(&pBigCtx->roleData.mst.bigSyncTmr);
    return;
  }

  /* Enqueue BIS Data PDUs. */

  uint32_t rxEventCounter = pBigCtx->eventCounter;

  for (unsigned int bisIdx = 0; bisIdx < pBigCtx->numBis; bisIdx++)
  {
    for (unsigned int bnIdx = 0; bnIdx < pBigCtx->bn; bnIdx++)
    {
      if (lctrMstBisIsr.pDataBuf[bisIdx][bnIdx])
      {
        lctrBisEnqueueRxDataPdu(pBigCtx->pBisCtx[bisIdx],
                                lctrMstBisIsr.pDataBuf[bisIdx][bnIdx],
                                bnIdx);  /* payload counter offset from rxEventCounter */
      }
    }
  }

  /* BIG Control procedure. */

  if (lctrMstBisIsr.pCtrlBuf)
  {
    if (lctrMstBigControlDecrypt(pBigCtx))
    {
      lctrMstBigControlPostProcess(pBigCtx);
    }
    else
    {
      goto RxPostProcess;
    }
  }

  /* Reset synchronization timeout timer. */

  if (lctrMstBisIsr.numRcvdPkt)
  {
    WsfTimerStartMs(&pBigCtx->roleData.mst.bigSyncTmr, pBigCtx->roleData.mst.bigSyncTimeoutMs);
  }

  /* Setup for next BIG Event. */

  uint32_t unsyncTimeUsec = pBigCtx->roleData.mst.anchorPoint - pBigCtx->roleData.mst.rxSyncTime;

  while (TRUE)
  {
    /* Advance to next interval. */
    pBigCtx->eventCounter += 1;
    pBigCtx->roleData.mst.anchorPoint += pBigCtx->isoInterUsec;
    unsyncTimeUsec += pBigCtx->isoInterUsec;

    uint32_t wwTotalUsec = lctrCalcWindowWideningUsec(unsyncTimeUsec, pBigCtx->roleData.mst.totalAcc)
                           + pBigCtx->roleData.mst.extraWwUsec;
    /* TODO Limit to half the ISO Interval size */

    pOp->dueUsec = pBigCtx->roleData.mst.anchorPoint - wwTotalUsec;
    pBis->rxSyncDelayUsec = wwTotalUsec << 1;   /* multiply 2 for before and after BIG Anchor Point */

    lctrSelectBigChannels(pBigCtx);
    lctrMstBigControlProcedureHandler(pBigCtx);

    if (SchInsertAtDueTime(pOp, NULL))
    {
      break;
    }

    LL_TRACE_WARN2("!!! BIG schedule conflict handle=%u, ec[15:0]=%u", pBigCtx->handle, pBigCtx->eventCounter);
  }

RxPostProcess:
  /* Consume ISO Data PDU. */

  lctrMstBisRxDataPduHandler(pBigCtx, rxEventCounter);
  lctrMstBisRxTestPduHandler(pBigCtx, rxEventCounter);

  if (lmgrCb.sendIsoCmplEvt)
  {
    lctrNotifyHostIsoEventComplete(pBigCtx->handle, (uint32_t) pBigCtx->eventCounter);
  }
}
