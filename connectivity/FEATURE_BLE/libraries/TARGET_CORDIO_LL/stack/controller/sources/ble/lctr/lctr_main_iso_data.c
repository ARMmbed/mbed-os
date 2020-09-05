/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller data path implementation file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#include "lctr_int_iso.h"
#include "lctr_pdu_iso.h"
#include "lctr_int_cis.h"
#include "ll_math.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "hci_defs.h"
#include "pal_bb.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Free list of Tx buffer descriptors. */
static wsfQueue_t lctrIsoTxBufDescQ;

/*! \brief      Completed transmit buffer. */
static uint8_t *pLctrTxCompBuf;

/*! \brief      Completed transmit buffer handle ID. */
static wsfHandlerId_t lctrTxCompBufHandlerId;

/*************************************************************************************************/
/*!
 *  \brief  Assemble CIS Data PDU.
 *
 *  \param  pIsoHdr     ISO header.
 *  \param  pBuf        Buffer to pack the Data PDU header.
 *  \param  llid        CIS LLID
 */
/*************************************************************************************************/
static void lctrAssembleCisDataPdu(lctrIsoHdr_t *pIsoHdr, uint8_t *pBuf, uint8_t llid)
{
  /* All additional fields must be zero'ed since flow control bits will be or'ed in at transmit. */
  lctrCisDataPduHdr_t dataHdr = { 0 };

  dataHdr.llid = llid;
  dataHdr.len = pIsoHdr->sduLen;

  lctrCisPackDataPduHdr(pBuf, &dataHdr);
}


/*************************************************************************************************/
/*!
 *  \brief      Initialize the transmit memory resources.
 *
 *  \param      pFreeMem    Pointer to free memory.
 *  \param      freeMemSize Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 */
/*************************************************************************************************/
uint16_t lctrIsoTxInitMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  pLctrTxCompBuf = NULL;

  WSF_ASSERT(pLctrRtCfg->numIsoTxBuf);
  WSF_ASSERT(pLctrRtCfg->maxIsoSduLen);

  uint8_t *pAvailMem = pFreeMem;

  const uint16_t descSize = sizeof(lctrIsoTxBufDesc_t) + (LL_MAX_FRAG * sizeof(((lctrIsoTxBufDesc_t *)0)->frag[0]));

  LL_TRACE_INFO2("    RAM: %u x %u bytes -- Tx buffer descriptors", pLctrRtCfg->numIsoTxBuf, sizeof(void *) + descSize);

  lctrIsoTxBufDescQ.pHead = NULL;
  lctrIsoTxBufDescQ.pTail = NULL;

  unsigned int i;

  for (i = 0; i < pLctrRtCfg->numIsoTxBuf; i++)
  {
    lctrIsoTxBufDesc_t *pDesc;

    if (((uint32_t)pAvailMem) & 3)
    {
      /* Align to next word. */
      pAvailMem = (uint8_t *)(((uint32_t)pAvailMem & ~3) + sizeof(uint32_t));
    }

    /* Allocate memory. */
    pDesc = (lctrIsoTxBufDesc_t *)pAvailMem;
    pAvailMem += (2 * sizeof(uint32_t)) + descSize;   /* wsfMsg_t header is at most 2 words */

    if (((uint32_t)(pAvailMem - pFreeMem)) > freeMemSize)
    {
      return 0;
    }

    /* Add to free list. */
    WsfQueueEnq(&lctrIsoTxBufDescQ, pDesc);
  }

  return (pAvailMem - pFreeMem);
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a Tx buffer descriptor.
 *
 *  \return A Tx buffer descriptor, NULL if allocation fails.
 */
/*************************************************************************************************/
lctrIsoTxBufDesc_t *lctrAllocIsoTxBufDesc(void)
{
  uint8_t *pElem;

  if ((pElem = WsfQueueDeq(&lctrIsoTxBufDescQ)) == NULL)
  {
    return NULL;
  }

  pElem += (2 * sizeof(uint32_t));   /* hide header */

  return (lctrIsoTxBufDesc_t *)pElem;
}

/*************************************************************************************************/
/*!
 *  \brief  Free a Tx buffer descriptor.
 *
 *  \param  pDesc       Pointer to a Tx buffer descriptor.
 */
/*************************************************************************************************/
void lctrFreeIsoTxBufDesc(lctrIsoTxBufDesc_t *pDesc)
{
  uint8_t *pElem = (uint8_t *)pDesc;
  pElem -= (2 * sizeof(uint32_t));   /* recover header */

  WsfQueueEnq(&lctrIsoTxBufDescQ, pElem);
}

/*************************************************************************************************/
/*!
 *  \brief  Queue data PDU onto ARQ queue.
 *
 *  \param  pCisCtx     CIS context.
 *  \param  pIsoHdr     ISO header.
 *  \param  pIsoBuf     ISO buffer.
 */
/*************************************************************************************************/
void lctrCisTxDataPduQueue(lctrCisCtx_t *pCisCtx, lctrIsoHdr_t *pIsoHdr, uint8_t *pIsoBuf)
{
  uint16_t fragOffset = 0;
  uint16_t isoLen = pIsoHdr->sduLen;
  uint16_t fragLen = pCisCtx->localDataPdu.maxTxLen;

  lctrIsoTxBufDesc_t *pDesc;

  if ((pDesc = lctrAllocIsoTxBufDesc()) == NULL)
  {
    LL_TRACE_ERR1("Failed to allocate transmit buffer descriptor: cisHandle=%u", pIsoHdr->handle);
    WsfMsgFree(pIsoBuf);
    if (pCisCtx->txTestEnabled == FALSE)
    {
      uint16_t handle = pIsoHdr->handle;
      uint16_t numSdu = 1;
      lmgrPersistCb.sendIsoCompCback(1, &handle, &numSdu);
    }
    lctrIsoSduTxIncAvailBuf();

    lctrNotifyHostHwErrInd(LL_ERROR_CODE_MEM_CAP_EXCEEDED);
    return;
  }

  if (fragLen >= pIsoHdr->sduLen)
  {
    fragLen = pIsoHdr->sduLen;
  }

  pDesc->isoLen  = pIsoHdr->sduLen;
  pDesc->fragLen = fragLen;
  pDesc->fragCnt = 0;

  /* Get the start of the buffer. */
  pDesc->pIsoSdu = pIsoBuf;
  pIsoBuf += LCTR_GET_ISO_DATA_HDR_LEN(pIsoHdr);
  pDesc->pPduBuf = pIsoBuf;
  uint8_t fragCnt = 0;

  uint8_t llid;

  do
  {
    const uint16_t dataRem = isoLen - fragOffset;
    const uint16_t fragSize = WSF_MIN(dataRem, fragLen);

    if (pCisCtx->framing == LL_ISO_PDU_TYPE_UNFRAMED)
    {
      llid = (fragSize == dataRem) ? LL_LLID_ISO_UNF_END_PDU : LL_LLID_ISO_UNF_CONT_PDU;
    }
    else
    {
      llid = LL_LLID_ISO_FRA_PDU;
    }

    pIsoHdr->sduLen = fragSize;
    lctrAssembleCisDataPdu(pIsoHdr, pDesc->frag[fragCnt].hdr, llid);
    pDesc->frag[fragCnt].hdrLen = LL_DATA_HDR_LEN;

    if (lctrPktEncryptHdlr && lctrPktEncryptHdlr(&pCisCtx->bleData.chan.enc, pDesc->frag[fragCnt].hdr, pIsoBuf, pDesc->frag[fragCnt].trl))
    {
      pDesc->frag[fragCnt].trlLen = LL_DATA_MIC_LEN;

  #if (LL_ENABLE_TESTER)
      pDesc->frag[fragCnt].trl[0] ^= (llTesterCb.pktMic >>  0) & 0xFF;
      pDesc->frag[fragCnt].trl[1] ^= (llTesterCb.pktMic >>  8) & 0xFF;
      pDesc->frag[fragCnt].trl[2] ^= (llTesterCb.pktMic >> 16) & 0xFF;
      pDesc->frag[fragCnt].trl[3] ^= (llTesterCb.pktMic >> 24) & 0xFF;
  #endif
    }
    else
    {
      pDesc->frag[fragCnt].trlLen = 0;
    }

    fragOffset += fragSize;
    pIsoBuf += fragSize;
    fragCnt++;
  }
  while (fragOffset < isoLen);

  WsfMsgEnq(&pCisCtx->txArqQ, pIsoHdr->handle, (uint8_t *)pDesc);
}

/*************************************************************************************************/
/*!
 *  \brief  Get top element in Tx queue.
 *
 *  \param  pCisCtx   CIS context.
 *  \param  pDescs    Storage for BB descriptors.
 *
 *  \return Number of BB descriptors.
 */
/*************************************************************************************************/
uint8_t lctrCisTxQueuePeek(lctrCisCtx_t *pCisCtx, PalBbBleTxBufDesc_t *pDescs)
{
  wsfHandlerId_t handlerId;
  uint8_t *pTxBuf;
  uint8_t descCnt = 0;

  /* Do not remove from ARQ until acknowledged by peer. */
  pTxBuf = WsfMsgPeek(&pCisCtx->txArqQ, &handlerId);
  if (pTxBuf != NULL)
  {
    /*** Send Data PDU ***/

    lctrIsoTxBufDesc_t *pDesc = (lctrIsoTxBufDesc_t *)pTxBuf;
    uint8_t  fragCnt  = pDesc->fragCnt;
    uint16_t fragSize = pDesc->fragLen;
    uint16_t fragOff  = fragSize * fragCnt;

    if ((fragOff + fragSize) > pDesc->isoLen)
    {
      fragSize = pDesc->isoLen - fragOff;
    }
    pDescs[0].len  = pDesc->frag[fragCnt].hdrLen;
    pDescs[0].pBuf = pDesc->frag[fragCnt].hdr;
    pDescs[1].len  = fragSize;
    pDescs[1].pBuf = pDesc->pPduBuf + fragOff;
    descCnt = 2;
    if (pDesc->frag[fragCnt].trlLen)
    {
      pDescs[2].len  = pDesc->frag[fragCnt].trlLen;
      pDescs[2].pBuf = pDesc->frag[fragCnt].trl;
      descCnt = 3;
    }
  }

  return descCnt;
}

/*************************************************************************************************/
/*!
 *  \brief  Pop top element from Tx queue.
 *
 *  \param  pCisCtx   CIS context.
 *
 *  \return TRUE if element popped.
 */
/*************************************************************************************************/
bool_t lctrCisTxQueuePop(lctrCisCtx_t *pCisCtx)
{
  wsfHandlerId_t handlerId;
  uint8_t *pBuf;

  WSF_ASSERT(pLctrTxCompBuf == NULL);

  /* Remove last transmitted PDU. */
  if ((pBuf = WsfMsgPeek(&pCisCtx->txArqQ, &handlerId)) != NULL)
  {
    lctrIsoTxBufDesc_t *pDesc = (lctrIsoTxBufDesc_t *)pBuf;
    uint16_t fragSize = pDesc->fragLen;

    if ((fragSize * (pDesc->fragCnt + 1)) >= pDesc->isoLen)  /* last fragment */
    {
      /* Store buffer for post setup cleanup. */
      pLctrTxCompBuf = pBuf;
      lctrTxCompBufHandlerId = handlerId;

      WsfMsgDeq(&pCisCtx->txArqQ, &handlerId);
    }
    else
    {
      /* Move to next fragment. */
      pDesc->fragCnt++;
    }
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Pop top element from Tx queue.
 *
 *  \param  pCisCtx      Connection context.
 */
/*************************************************************************************************/
void lctrCisTxQueuePopCleanup(lctrCisCtx_t *pCisCtx)
{
  if (pLctrTxCompBuf)
  {
    lctrIsoTxBufDesc_t *pDesc = (lctrIsoTxBufDesc_t *)pLctrTxCompBuf;

    WsfMsgFree(pDesc->pIsoSdu);
    lctrFreeIsoTxBufDesc(pDesc);
    lctrIsoSduTxIncAvailBuf();
    pCisCtx->numTxComp++;

    pLctrTxCompBuf = NULL;

    WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_ISO_TX_COMPLETE));
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear Tx queue.
 *
 *  \param  pCisCtx    CIS context.
 *
 *  \return Number of freed buffers.
 */
/*************************************************************************************************/
uint8_t lctrCisTxQueueClear(lctrCisCtx_t *pCisCtx)
{
  uint8_t *pBuf;
  uint8_t numTxBufs = 0;
  wsfHandlerId_t handlerId;

  /* Clear the pending Tx buffer. */
  if (pLctrTxCompBuf)
  {
    lctrCisTxQueuePopCleanup(pCisCtx);
  }

  /* Clear the rest of the Tx buffer. */
  while ((pBuf = WsfMsgDeq(&pCisCtx->txArqQ, &handlerId)) != NULL)
  {
    lctrIsoTxBufDesc_t *pDesc = (lctrIsoTxBufDesc_t *)pBuf;

    WsfMsgFree(pDesc->pIsoSdu);

    lctrFreeIsoTxBufDesc(pDesc);

    lctrIsoSduTxIncAvailBuf();
    numTxBufs++;
  }

  return numTxBufs;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a receive data PDU buffer.
 *
 *  \param  maxRxLen    Maximum receive length.
 *
 *  \return Pointer to the start of the PDU data buffer.
 */
/*************************************************************************************************/
uint8_t *lctrCisRxPduAlloc(uint16_t maxRxLen)
{
  /* LCTR_DATA_PDU_MAX_LEN includes LL_DATA_MIC_LEN if required. */
  const uint16_t allocLen = WSF_MAX(BB_FIXED_DATA_PKT_LEN, LCTR_CIS_DATA_PDU_LEN(maxRxLen)) + LCTR_CIS_DATA_PDU_START_OFFSET;

  uint8_t *pBuf;

  /* Include ISO header. */
  if ((pBuf = WsfMsgAlloc(HCI_ISO_DL_MAX_LEN + allocLen)) != NULL)
  {
    /* Return start of data PDU. */
    pBuf += LCTR_CIS_DATA_PDU_START_OFFSET + HCI_ISO_DL_MAX_LEN;
  }

  return pBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Free a receive CIS data PDU buffer.
 *
 *  \param  pBuf        PDU data buffer to free.
 */
/*************************************************************************************************/
void lctrCisRxPduFree(uint8_t *pBuf)
{
  /* Recover headroom, assume buffer starts at the beginning of the data PDU. */
  pBuf -= (LCTR_CIS_DATA_PDU_START_OFFSET + HCI_ISO_DL_MAX_LEN);

  WsfMsgFree(pBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Enqueue a receive data PDU buffer.
 *
 *  \param  pBuf            PDU data buffer to queue.
 *  \param  eventCounter    Event counter.
 *  \param  cisHandle      Connection handle.
 */
/*************************************************************************************************/
void lctrCisRxEnq(uint8_t *pBuf, uint16_t eventCounter, uint16_t cisHandle)
{
  /* Stamp packet with event counter. */
  pBuf -= LCTR_CIS_DATA_PDU_START_OFFSET;
  UINT16_TO_BUF(pBuf, eventCounter);

  /* Queue LE Data PDU. */
  WsfMsgEnq(&lmgrIsoCb.rxDataQ, cisHandle, pBuf);
  WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_CIS_RX_PENDING));
}

/*************************************************************************************************/
/*!
 *  \brief  Dequeue a receive data PDU buffer.
 *
 *  \param  pCisHandle     Storage for connection handle.
 *
 *  \return Pointer to the start of the PDU data buffer.
 */
/*************************************************************************************************/
uint8_t *lctrCisRxDeq(uint16_t *pCisHandle)
{
  uint8_t *pBuf;
  wsfHandlerId_t handlerId;

  if ((pBuf = WsfMsgDeq(&lmgrIsoCb.rxDataQ, &handlerId)) != NULL)
  {
    /* Return start of data PDU. */
    pBuf += LCTR_CIS_DATA_PDU_START_OFFSET;

    *pCisHandle = (uint16_t)handlerId;
  }

  return pBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Enqueue a receive data PDU buffer for a connection as a ISO message.
 *
 *  \param  pOutDataPathCtx   Datapath output context.
 *  \param  handle            ISO handle.
 *  \param  pBuf              SDU buffer.
 *
 *  \return TRUE if buffer successfully handled and will be used.
 *          FALSE if buffer was not handled and needs to be disposed.
 *
 */
/*************************************************************************************************/
bool_t lctrIsoRxConnEnq(lctrOutDataPathCtx_t *pOutDataPathCtx, uint16_t handle, uint8_t *pBuf)
{
  switch (pOutDataPathCtx->id)
  {
    case LL_ISO_DATA_PATH_HCI:
      WsfMsgEnq(&pOutDataPathCtx->cfg.hci.rxDataQ, handle, pBuf);
      pOutDataPathCtx->cfg.hci.numRxPend++;
      /* TODO optimize counter, accounting upon allocation. */
      lctrIsoDataRxDecAvailBuf();
      return TRUE;

    case LL_ISO_DATA_PATH_VS:
    default:
      return FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Dequeue a receive data PDU buffer for a connection as a ISO message.
 *
 *  \param  pOutCtx         Output context.
 *
 *  \return Pointer to the start of the ISO message.
 *
 *  \note   Returned pointer must be freed with WsfMsgFree().
 */
/*************************************************************************************************/
uint8_t *lctrIsoRxConnDeq(lctrOutDataPathCtx_t *pOutCtx)
{
  wsfHandlerId_t handle;

  if (pOutCtx->id != LL_ISO_DATA_PATH_HCI)
  {
    return NULL;
  }

  return WsfMsgDeq(&pOutCtx->cfg.hci.rxDataQ, &handle);
}

/*************************************************************************************************/
/*!
 *  \brief  Setup a datapath context.
 *
 *  \param  pOutCtx         Output datapath context.
 */
/*************************************************************************************************/
void lctrIsoOutDataPathSetup(lctrOutDataPathCtx_t *pOutCtx)
{
  switch (pOutCtx->id)
  {
    case LL_ISO_DATA_PATH_HCI:
      pOutCtx->cfg.hci.numRxPend = 0;
      WSF_QUEUE_INIT(&pOutCtx->cfg.hci.rxDataQ);
      break;

    case LL_ISO_DATA_PATH_VS:
      /* No action. */
      break;

    default:
      /* No action. */
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear a datapath context.
 *
 *  \param  pOutCtx         Output datapath context.
 */
/*************************************************************************************************/
void lctrIsoOutDataPathClear(lctrOutDataPathCtx_t *pOutCtx)
{
  switch (pOutCtx->id)
  {
    case LL_ISO_DATA_PATH_HCI:
    {
      uint8_t *pBuf;
      wsfHandlerId_t handlerId;
      while ((pBuf = WsfMsgDeq(&pOutCtx->cfg.hci.rxDataQ, &handlerId)) != NULL)
      {
        WsfMsgFree(pBuf);
        lctrIsoDataRxIncAvailBuf(1);
      }
      break;
    }

    case LL_ISO_DATA_PATH_VS:
      /* No action. */
      break;

    default:
      /* No action. */
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear a ISOAL receive context.
 *
 *  \param  pRxCtx      ISOAL receive context
 *  \param  framing     Framing scheme.
 */
/*************************************************************************************************/
void lctrIsoalRxDataPathClear(lctrIsoalRxCtx_t *pRxCtx, uint8_t framing)
{
  uint8_t *pSduBuf;
  uint8_t handlerId;
  if (framing == LL_ISO_PDU_TYPE_UNFRAMED)
  {
    while ((pSduBuf = WsfMsgDeq(&pRxCtx->data.unframed.pendSduQ, &handlerId)) != NULL)
    {
      WsfMsgFree(pSduBuf);
    }
  }
  else /* LL_ISO_PDU_TYPE_FRAMED */
  {
    if (pRxCtx->pPendSduBuf)
    {
      WsfMsgFree(pRxCtx->pPendSduBuf);
      pRxCtx->pPendSduBuf = NULL;
    }
  }

  pRxCtx->pduFlushed = FALSE;
  pRxCtx->rxState = LL_ISO_SDU_STATE_NEW;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate transmit ISO test data.
 *
 *  \param      handle      Connection handle.
 *  \param      pldType     Payload length type.
 *  \param      maxSdu      Max SDU length.
 *  \param      pktCtr      Packet counter.
 *
 *  \return     Pointer to an HCI ISO SDU.
 *
 *  Initialization of ISO test data will allocate and fill a ISO SDU to accommodate any payload
 *  type. The LL will modify the length and payload (i.e. Packet Counter) as needed.
 */
/*************************************************************************************************/
uint8_t *lctrGenerateIsoTestData(uint16_t handle, LlIsoPldType_t pldType, uint16_t maxSdu, uint32_t pktCtr)
{
  uint8_t *pSdu;

  if ((pSdu = lctrTxIsoDataPduAlloc()) == NULL)
  {
    LL_TRACE_WARN0("!!! Failed to generate a ISO Test Data SDU");
    return NULL;
  }

  uint8_t *pBuf = pSdu;
  uint16_t len;

  switch (pldType)
  {
    case LL_ISO_PLD_TYPE_VAR_LEN:
      len = LlMathRandNum() % maxSdu;    /* TODO Use portable mod routine */
      /* Ensure minimum SDU length includes mandatory payload. */
      len = WSF_MAX(len, sizeof(uint32_t));
      break;

    case LL_ISO_PLD_TYPE_MAX_LEN:
      /* Ensure minimum SDU length includes mandatory payload. */
      len = WSF_MAX(maxSdu, sizeof(uint32_t));
      break;

    case LL_ISO_PLD_TYPE_ZERO_LEN:
      len = 0;
      break;

    default:
    {
      LL_TRACE_ERR1("Invalid value pldType=%u", pldType);
      len = 0;
      break;
    }
  }

  /* Pack ISO header. */
  lctrIsoHdr_t isoHdr;

  isoHdr.handle = handle;
  isoHdr.pb = LCTR_PB_COMP;
  isoHdr.tsFlag = 0;
  isoHdr.len = len;
  isoHdr.pktSn = 0;
  isoHdr.sduLen = len;
  isoHdr.ps = LCTR_PS_VALID;

  pBuf += lctrIsoPackHdr(pBuf, &isoHdr);

  if (len > 0)
  {
    /* Fill mandatory payload (do not advance pBuf). */
    UINT32_TO_BUF(pBuf, pktCtr);

    /* Fill VS payload (byte content is offset number). */
    for (unsigned int i = sizeof(uint32_t); i < len; i++)
    {
      pBuf[i] = i;
    }
  }

  return pSdu;
}

/*************************************************************************************************/
/*!
 *  \brief      Validate received ISO test data.
 *
 *  \param      pPld        Payload start buffer.
 *  \param      actLen      Length of payload
 *  \param      pRxStats    Rx statistics.
 *  \param      pldType     Payload length type.
 *  \param      expMaxSdu   Expected max SDU length.
 *  \param      expPldCtr   Expected payload counter.
 */
/*************************************************************************************************/
void lctrValidateIsoTestData(uint8_t *pPld, uint8_t actLen, LlIsoTestCtrs_t *pRxStats, uint8_t pldType, uint16_t expMaxSdu, uint32_t expPldCtr)
{
  uint32_t actPldCtr;

  /* Parse transmitted payload counter. */
  switch (pldType)
  {
  case LL_ISO_PLD_TYPE_VAR_LEN:
  case LL_ISO_PLD_TYPE_MAX_LEN:
    BSTREAM_TO_UINT32(actPldCtr, pPld);
    break;
  case LL_ISO_PLD_TYPE_ZERO_LEN:
  default:
    actPldCtr = 0;
    break;
  }

  /* Validate received SDU. */
  switch (pldType)
  {
  case LL_ISO_PLD_TYPE_ZERO_LEN:
    if (actLen != 0)
    {
      LL_TRACE_WARN1("lctrValidateIsoTestData: wrong length, actLen=%u expLen=0", actLen);
      pRxStats->numFailed++;
    }
    else
    {
      pRxStats->numSuccess++;
    }
    break;
  case LL_ISO_PLD_TYPE_VAR_LEN:
    if ((actLen < sizeof(uint32_t)) || (actLen > expMaxSdu))
    {
      LL_TRACE_WARN2("lctrValidateIsoTestData: wrong length, actLen=%u expLen=[0..%u]", actLen, expMaxSdu);
      pRxStats->numFailed++;
    }
    else if (actPldCtr != expPldCtr)
    {
      LL_TRACE_WARN2("lctrValidateIsoTestData: wrong payload counter, actPldCtr[15:0]=%u expPldCtr[15:0]=%u", actPldCtr, expPldCtr);
      pRxStats->numFailed++;
    }
    else
    {
      pRxStats->numSuccess++;
    }
    break;
  case LL_ISO_PLD_TYPE_MAX_LEN:
    if (actLen != expMaxSdu)
    {
      LL_TRACE_WARN2("lctrValidateIsoTestData: wrong length, actLen=%u expLen=%u", actLen, expMaxSdu);
      pRxStats->numFailed++;
    }
    else if (actPldCtr != expPldCtr)
    {
      LL_TRACE_WARN2("lctrValidateIsoTestData: wrong payload counter, actPldCtr[15:0]=%u expPldCtr[15:0]=%u", actPldCtr, expPldCtr);
      pRxStats->numFailed++;
    }
    else
    {
      pRxStats->numSuccess++;
    }
    break;
  default:
    LL_TRACE_WARN1("lctrValidateIsoTestData: invalid parameter, pldType=%u", pldType);
    break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a transmit ISO Data PDU buffer.
 *
 *  \return Pointer to the start of the ISO Data PDU buffer, NULL if allocation failed.
 */
/*************************************************************************************************/
uint8_t *lctrTxIsoDataPduAlloc(void)
{
  uint8_t *pPdu;

  const uint32_t allocLen = HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN + pLctrRtCfg->maxIsoSduLen + BB_DATA_PDU_TAILROOM;

  /* Use LL_ISO_PDU_MAX_LEN to ensure use of data buffers located in the large pool. */
  if ((pPdu = (uint8_t*)WsfMsgAlloc(allocLen)) == NULL)
  {
    LL_TRACE_WARN1("lctrTxIsoDataPduAlloc: Unable to allocate framed Tx buffer, allocSize=%u", allocLen);
  }

  return pPdu;
}

/*************************************************************************************************/
/*!
 *  \brief  Assemble framed PDU.
 *
 *  \param  pIsoalTxCtx Transmit ISOAL context.
 *  \param  pPduBuf     PDU buffer.
 *  \param  maxPduLen   Maximum PDU length.
 *
 *  \return Total size of assembled PDU.
 */
/*************************************************************************************************/
uint8_t lctrAssembleTxFramedPdu(lctrIsoalTxCtx_t *pIsoalTxCtx, uint8_t *pPduBuf, uint16_t maxPduLen)
{
  wsfQueue_t *pSduQ = &pIsoalTxCtx->pendingSduQ;

  uint8_t handlerId; /* Temp variable for the wsfMsg. */
  uint8_t totalTx = 0;
  uint16_t remLen = maxPduLen;
  lctrIsoSegHdr_t segHdr = { 0 };
  uint8_t segHdrLen = 0;
  uint8_t *pFreeData = NULL;

  /* Set offset of pPduBuf */
  pPduBuf += HCI_ISO_HDR_LEN + HCI_ISO_DL_MIN_LEN;

  /*** Loop through and pack SDUs. ***/

  while (remLen)
  {
    uint8_t *pSduBuf = WsfMsgPeek(pSduQ, &handlerId);

    /* The buffer is empty, process the completed PDU */
    if (pSduBuf == NULL)
    {
      break;
    }

    lctrIsoHdr_t isoHdr;

    /*** Disassemble ISO packet. ***/

    lctrIsoUnpackHdr(&isoHdr, pSduBuf);

    pSduBuf += LCTR_GET_ISO_DATA_HDR_LEN(&isoHdr);
    segHdrLen = LL_DATA_HDR_LEN;

    /* If this is a segmented SDU, continue from the last segment. */
    if (pIsoalTxCtx->sduOffset)
    {
      /* Resume buffer at sduOffset. */
      pSduBuf += pIsoalTxCtx->sduOffset;
      isoHdr.sduLen -= pIsoalTxCtx->sduOffset;

      /* Set continuation bit. */
      segHdr.sc = 1;
    }
    else
    {
      segHdr.sc = 0;
      segHdrLen += LL_ISO_SEG_TO_LEN;
    }

    /*** Compute segment parameters. ***/

    /* There is enough room for the entire SDU. */
    if (remLen >= (isoHdr.sduLen + segHdrLen))
    {
      segHdr.len = isoHdr.sduLen;
      segHdr.cmplt = 1;
      if (segHdr.sc == 0)
      {
        /* TODO: put in time offset. */
        segHdr.toffs = 0x1337;
      }

      /* Reset pduOffset and update remaining length. */
      pIsoalTxCtx->sduOffset = 0;
      remLen -= segHdr.len + segHdrLen;

      /* Pop off completed SDU. */
      pFreeData = WsfMsgDeq(pSduQ, &handlerId);
      pIsoalTxCtx->pendQueueSize--;
      pIsoalTxCtx->compSdu++;
    }
    else
    {
      /* If it cannot fit at least one effective (data) byte, finish packing here. */
      if (remLen < (segHdrLen + 1))
      {
        break;
      }

      segHdr.len = remLen - segHdrLen;
      segHdr.cmplt = 0;
      if (segHdr.sc == 0)
      {
        /* TODO: put in time offset. */
        segHdr.toffs = 0x1337;
      }

      /* Save offset. */
      pIsoalTxCtx->sduOffset += segHdr.len;
      remLen = 0; /* We filled as much as we can, no need to track it anymore. */
    }

    /*** Pack PDU ***/

    /* Pack buffer with seg header */
    pPduBuf += lctrIsoPackSegHdr(pPduBuf, &segHdr);

    /* Pack data into buffer. */
    uint8_t sduDataLen =  segHdr.len;
    memcpy(pPduBuf, pSduBuf, sduDataLen);
    pPduBuf += sduDataLen;

    /* Free buffer. */
    if (pFreeData)
    {
      WsfMsgFree(pFreeData);
      pFreeData = NULL;
    }

    totalTx += sduDataLen + segHdrLen;
  }

  return totalTx;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a receive ISO SDU buffer.
 *
 *  \return Pointer to the start of the ISO Data PDU buffer, NULL if allocation failed.
 */
/*************************************************************************************************/
static uint8_t *lctrRxSduAlloc(void)
{
  uint8_t *pSdu;

  const uint32_t allocLen = HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN + pLctrRtCfg->maxIsoSduLen + BB_DATA_PDU_TAILROOM;

  /* Use LL_ISO_PDU_MAX_LEN to ensure use of data buffers located in the large pool. */
  if ((pSdu = (uint8_t*)WsfMsgAlloc(allocLen)) != NULL)
  {
    lctrIsoDataRxDecAvailBuf();
  }

  return pSdu;
}

/*************************************************************************************************/
/*!
 *  \brief  Queue the received SDU into the pending queue.
 *
 *  \param  pRxCtx     ISOAL receive context.
 *  \param  pSdu       SDU buffer.
 *  \param  handle     CIS connection handle.
 *  \param  dataLen    Data length.
 *  \param  llid       Rx LLID.
 *
 *  \return TRUE if pending queue ready to be sent to host.
 */
/*************************************************************************************************/
bool_t lctrIsoUnframedRxSduPendQueue(lctrIsoalRxCtx_t *pRxCtx, uint8_t *pSdu, uint16_t handle,
                                     uint16_t dataLen, uint8_t llid)
{
  /* Save data for head packet. */
  /* pRxCtx->data.unframed.pendSduIsoHdr.ps = 0; */   /* Should be done in calling function. */
  pRxCtx->data.unframed.curLen += dataLen;

  WsfMsgEnq(&pRxCtx->data.unframed.pendSduQ, handle, pSdu);

  /* If we received the last PDU, pack the head packet with the packet status and sdu length. */
  if (llid == LL_LLID_ISO_UNF_END_PDU)
  {
    uint8_t *pHeadPkt;
    uint8_t handlerId;
    lctrIsoHdr_t isoHdr = {0};

    pHeadPkt = WsfMsgPeek(&pRxCtx->data.unframed.pendSduQ, &handlerId);
    WSF_ASSERT(pHeadPkt);

    lctrIsoUnpackHdr(&isoHdr, pHeadPkt);
    isoHdr.pktSn = pRxCtx->packetSequence++;
    isoHdr.sduLen = pRxCtx->data.unframed.curLen;
    isoHdr.ps = pRxCtx->data.unframed.ps;
    isoHdr.len -= + HCI_ISO_DL_MAX_LEN;

    /* Reset the packet boundary in case a missed packet leaves it undefined. */
    uint8_t rfu; /* Used to peek into msg queue. */
    if (WsfMsgNPeek(&pRxCtx->data.unframed.pendSduQ, 1, &rfu))
    {
      isoHdr.pb = LCTR_PB_FIRST;
    }
    else
    {
      isoHdr.pb = LCTR_PB_COMP;
    }

    lctrIsoPackHdr(pHeadPkt, &isoHdr);

    /* Clean up context */
    pRxCtx->data.unframed.curLen = 0;
    pRxCtx->data.unframed.ps = 0;
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Assemble received framed SDU(s) from PDU.
 *
 *  \param  pIsoalRxCtx Receive ISOAL control block.
 *  \param  pRxQueue    Receive queue.
 *  \param  handle      CIS/BIS Handle.
 *  \param  pIsoBuf     ISO data buffer.
 *  \param  len         Length of data buffer.
 *
 *  \return Total number of SDUs queued.
 */
/*************************************************************************************************/
uint8_t lctrAssembleRxFramedSdu(lctrIsoalRxCtx_t *pIsoalRxCtx, wsfQueue_t *pRxQueue,
                                uint16_t handle, uint8_t *pIsoBuf, uint8_t len)
{
  uint8_t totalSduQueued = 0;

  /* Last PDU was flushed. SDU data is lost and will be flushed. */
  if (pIsoalRxCtx->pduFlushed)
  {
    if (pIsoalRxCtx->pPendSduBuf)
    {
      /* Flush SDU. */
      WsfMsgFree(pIsoalRxCtx->pPendSduBuf);
      pIsoalRxCtx->pPendSduBuf = NULL;
    }
    pIsoalRxCtx->pduFlushed = FALSE;
    return 0;
  }

  uint8_t *pDataBuf = pIsoBuf + LL_DATA_HDR_LEN;
  uint8_t *pSduBuf;

  uint8_t remLen = len;

  lctrIsoSegHdr_t segHdr = { 0 };

  while (remLen > 0)
  {
    uint8_t hdrLen = lctrIsoUnpackSegHdr(&segHdr, pDataBuf);
    pDataBuf += hdrLen;

    if (segHdr.sc == 0)
    {
      segHdr.len -= LL_ISO_SEG_TO_LEN;
    }

    switch (pIsoalRxCtx->rxState)
    {
      case LL_ISO_SDU_STATE_NEW:
      {
        if ((pIsoalRxCtx->pPendSduBuf = lctrRxSduAlloc()) == NULL)
        {
          LL_TRACE_WARN1("Unable to allocate framed Rx buffer, handle=%u", handle);
          continue;
        }

        /* Move pointer to start of data buffer. */
        pIsoalRxCtx->rxSduOffset = LCTR_ISO_SDU_DATA_START_OFFSET; /* Start at the data buffer, save space for Dataload. */
        pSduBuf = pIsoalRxCtx->pPendSduBuf + pIsoalRxCtx->rxSduOffset;

        switch (segHdr.sc)
        {
          case HCI_ISOAL_SEG_HDR_SC_START:
            pIsoalRxCtx->data.framed.timeOffset = segHdr.toffs;
            break;

          case HCI_ISOAL_SEG_HDR_SC_CONT:
          default:
            LL_TRACE_WARN1("Missed start of segmentation; dropping packet, handle=%u", handle);
            WsfMsgFree(pIsoalRxCtx->pPendSduBuf);
            pIsoalRxCtx->pPendSduBuf = NULL;
            pDataBuf += segHdr.len;
            remLen -= hdrLen + segHdr.len;
            continue;
        }
        break;
      }

      case LL_ISO_SDU_STATE_CONT:
      {
        pSduBuf = pIsoalRxCtx->pPendSduBuf + pIsoalRxCtx->rxSduOffset;

        switch (segHdr.sc)
        {
          case HCI_ISOAL_SEG_HDR_SC_CONT:
            break;

          case HCI_ISOAL_SEG_HDR_SC_START:
          default:
            LL_TRACE_WARN1("Expected continuation bit but got start bit; dropping packet, handle=%u", handle);
            WsfMsgFree(pIsoalRxCtx->pPendSduBuf);
            pIsoalRxCtx->pPendSduBuf = NULL;
            pDataBuf += segHdr.len;
            remLen -= hdrLen + segHdr.len;
            pIsoalRxCtx->rxState = LL_ISO_SDU_STATE_NEW;
            continue;
        }
        break;
      }

      default:
        LL_TRACE_WARN2("Unknown ISOAL state, handle=%u state=%u", handle, pIsoalRxCtx->rxState);
        if (pIsoalRxCtx->pPendSduBuf)
        {
          WsfMsgFree(pIsoalRxCtx->pPendSduBuf);
          pIsoalRxCtx->pPendSduBuf = NULL;
        }
        pDataBuf += segHdr.len;
        remLen -= hdrLen + segHdr.len;
        continue;
    }

    memcpy(pSduBuf, pDataBuf, segHdr.len);
    pDataBuf += segHdr.len;
    pIsoalRxCtx->rxSduOffset += segHdr.len;

    /* SDU Complete, queue up SDU to host. */
    if (segHdr.cmplt)
    {
      lctrIsoHdr_t isoHdr = { 0 };
      isoHdr.handle = handle;
      isoHdr.len = pIsoalRxCtx->rxSduOffset - (HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN);
      isoHdr.pktSn = pIsoalRxCtx->packetSequence++;
      isoHdr.sduLen = isoHdr.len;
      isoHdr.tsFlag = 1;
      isoHdr.pb = LCTR_PB_COMP;
      /* isoHdr.ps = LCTR_PS_VALID; */ /* Already done. */
      isoHdr.ts = pIsoalRxCtx->data.framed.timeOffset; /* TODO: Fill out timestamp based on anchor point */

      uint8_t *pIsoHdrBuf = (pIsoalRxCtx->pPendSduBuf);
      lctrIsoPackHdr(pIsoHdrBuf, &isoHdr);

      /* Queue SDU. */
      WsfMsgEnq(pRxQueue, handle, pIsoalRxCtx->pPendSduBuf);
      /* lctrIsoDataRxDecAvailBuf(); */ /* Handled in lctrRxSduAlloc. */
      totalSduQueued++;

      pIsoalRxCtx->pPendSduBuf = NULL;
      remLen -= hdrLen + segHdr.len;
      pIsoalRxCtx->rxState = LL_ISO_SDU_STATE_NEW;
    }
    /* SDU Incomplete, save offset and return. */
    else
    {
      pIsoalRxCtx->rxState = LL_ISO_SDU_STATE_CONT;
      return totalSduQueued;
    }
  }

  return totalSduQueued;
}
