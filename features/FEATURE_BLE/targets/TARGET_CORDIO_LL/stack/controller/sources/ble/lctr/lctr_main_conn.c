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
 * \brief Link layer controller data path implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_conn.h"
#include "lctr_pdu_conn.h"
#include "lmgr_api_conn.h"
#include "ll_math.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Cipher block initialization handler. */
lctrCipherBlkHdlr_t lctrInitCipherBlkHdlr = NULL;

/*! \brief      Packet encrypt handler. */
lctrPktEncHdlr_t lctrPktEncryptHdlr = NULL;

/*! \brief      Packet decrypt handler. */
lctrPktDecHdlr_t lctrPktDecryptHdlr = NULL;

/*! \brief      Set encrypt packet count handler. */
lctrPktCntHdlr_t lctrSetEncryptPktCountHdlr = NULL;

/*! \brief      Set decrypt packet count handler. */
lctrPktCntHdlr_t lctrSetDecryptPktCountHdlr = NULL;

/*! \brief      Pointer to connection context table. */
lctrConnCtx_t *pLctrConnTbl;

/*! \brief      Common data storage used for decoding a received PDU. */
lctrDataPdu_t lctrDataPdu;

/*! \brief      Common storage for task message data (parameter passing to action routines). */
lctrConnMsg_t *pLctrConnMsg;

/*! \brief      Vendor specific PDU handlers. */
const LctrVsHandlers_t *pLctrVsHdlrs = NULL;

/*! \brief      Control PDU handler. */
lctrCtrlPduHdlr_t lctrCtrlPduHdlr = NULL;

/*! \brief      Build remap table handlers. */
LctrChSelHdlr_t lctrChSelHdlr[LCTR_CH_SEL_MAX] = { 0 };

/*! \brief      Check if CIS is enabled function. */
lctrCheckCisEstCisFn_t  lctrCheckCisEstCisFn = NULL;

/*! \brief      Pointer to lctrSendPerSyncFromScan function. */
lctrLlcpEh_t lctrSendPerSyncFromScanFn = NULL;

/*! \brief      Pointer to lctrSendPerSyncFromBcst function. */
lctrLlcpEh_t lctrSendPerSyncFromBcstFn = NULL;

/*! \brief      Pointer to lctrSendPeriodicSyncInd function. */
lctrLlcpEh_t lctrSendPeriodicSyncIndFn = NULL;

/*! \brief      Pointer to lctrStorePeriodicSyncTrsf function. */
lctrLlcpEh_t lctrStorePeriodicSyncTrsfFn = NULL;

/*! \brief      Pointer to lctrReceivePeriodicSyncInd function. */
lctrLlcpEh_t lctrReceivePeriodicSyncIndFn = NULL;

/*************************************************************************************************/
/*!
 *  \brief      Initialize the connection memory resources.
 *
 *  \param      pFreeMem    Pointer to free memory.
 *  \param      freeMemSize Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 */
/*************************************************************************************************/
uint16_t LctrInitConnMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  uint8_t *pAvailMem = pFreeMem;
  uint16_t memUsed;

  /*** Connection Context ***/

  if (((uint32_t)pAvailMem) & 3)
  {
    /* Align to next word. */
    pAvailMem = (uint8_t *)(((uint32_t)pAvailMem & ~3) + sizeof(uint32_t));
  }

  /* Allocate memory. */
  LL_TRACE_INFO2("    RAM: %u x %u bytes -- connection context", pLctrRtCfg->maxConn, sizeof(lctrConnCtx_t));
  pLctrConnTbl = (lctrConnCtx_t *)pAvailMem;
  pAvailMem += sizeof(lctrConnCtx_t) * pLctrRtCfg->maxConn;

  if (((uint32_t)(pAvailMem - pFreeMem)) > freeMemSize)
  {
    LL_TRACE_ERR2("LctrInitConnMem: failed to allocate connection context, need=%u available=%u", (pAvailMem - pFreeMem), freeMemSize);
    WSF_ASSERT(FALSE);
    return 0;
  }

  /*** Tx Memory ***/

  freeMemSize -= pAvailMem - pFreeMem;
  memUsed = lctrTxInitMem(pAvailMem, freeMemSize);
  if (memUsed == 0)
  {
    LL_TRACE_ERR2("LctrInitConnMem: failed to allocate descriptors, need=%u available=%u", (pAvailMem - pFreeMem), freeMemSize);
    WSF_ASSERT(FALSE);
    return 0;
  }

  pAvailMem += memUsed;

  lmgrPersistCb.connCtxSize = sizeof(lctrConnCtx_t);

  return (pAvailMem - pFreeMem);
}

/*************************************************************************************************/
/*!
 *  \brief  Assign vendor specific PDU handlers.
 *
 *  \param  pHdlrs      Static definition for VS handlers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrVsConnInit(const LctrVsHandlers_t *pHdlrs)
{
  pLctrVsHdlrs = pHdlrs;
}

/*************************************************************************************************/
/*!
 *  \brief      Validate connection specification parameter range.
 *
 *  \param      pConnSpec       New connection specification.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LctrValidateConnSpec(const LlConnSpec_t *pConnSpec)
{
  if ((pConnSpec->connIntervalMin < LL_MIN_CONN_INTERVAL) ||
      (pConnSpec->connIntervalMax > LL_MAX_CONN_INTERVAL) ||
      (pConnSpec->connIntervalMin > pConnSpec->connIntervalMax) ||
      /* (pConnSpec->connLatency < connLatencyMin) || */
      (pConnSpec->connLatency > LL_MAX_CONN_LATENCY) ||
      (pConnSpec->supTimeout < LL_MIN_SUP_TIMEOUT) ||
      (pConnSpec->supTimeout > LL_MAX_SUP_TIMEOUT) ||
      (pConnSpec->supTimeout <= ((1 + pConnSpec->connLatency) * pConnSpec->connIntervalMax >> 2)))
  {
    LL_TRACE_WARN0("LctrValidateConnSpec: invalid parameters");
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Validate modify sca parameters.
 *
 *  \param      action       Action to take.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LctrValidateModifyScaParam(uint8_t action)
{
  int8_t curSca = (int8_t) lctrComputeSca();

  if (((curSca == LL_SCA_MIN_INDEX) && (action == LL_MODIFY_SCA_LESS_ACCURATE)) ||
      ((curSca == LL_SCA_MAX_INDEX) && (action == LL_MODIFY_SCA_MORE_ACCURATE)))
  {
    return LL_ERROR_CODE_LIMIT_REACHED;
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  Check whether a procedure is already active or pended.
 *
 *  \param  handle      Connection handle.
 *  \param  event       Event.
 *
 *  \return TRUE if the procedure is already active or pended, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LctrIsProcActPended(uint16_t handle, uint8_t event)
{
  uint8_t proc;
  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(handle);

  if ((proc = lctrGetProcId(event)) == LCTR_PROC_INVALID)
  {
    return FALSE;
  }

  if ((pCtx->llcpActiveProc == proc) ||
      (pCtx->llcpPendMask & (1 << proc)))
  {
    LL_TRACE_WARN1("The new procedure=%u is already active or pended.", pCtx->llcpActiveProc);
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get encryption mode used in a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pMode           New encryption mode.
 *
 *  \return     None.
 *
 *  Get the encryption mode used by a connection.
 */
/*************************************************************************************************/
void LctrGetEncMode(uint16_t handle, LlEncMode_t *pMode)
{
  WSF_ASSERT(pMode);

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(handle);

  pMode->enaAuth = pCtx->bleData.chan.enc.enaAuth;
}

/*************************************************************************************************/
/*!
 *  \brief      Set encryption mode used in a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pMode           Current encryption mode.
 *
 *  \return     TRUE if successful, FALSE otherwise.
 *
 *  Set the encryption mode used by a connection. Must be called before encryption is started or
 *  when encryption is paused.
 */
/*************************************************************************************************/
bool_t LctrSetEncMode(uint16_t handle, const LlEncMode_t *pMode)
{
  WSF_ASSERT(pMode);

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(handle);

  if (pCtx->pendEncMode)
  {
    /* Mode already pending. */
    return FALSE;
  }

  pCtx->newEncMode = *pMode;
  pCtx->pendEncMode = TRUE;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Set connection operational mode flags.
 *
 *  \param      handle  Connection handle.
 *  \param      flags   Flags.
 *  \param      enable  TRUE to set flags or FALSE to clear flags.
 *
 *  \return     None.
 *
 *  Set mode flags governing LL operations of a given connection.
 */
/*************************************************************************************************/
void LctrSetConnOpFlags(uint16_t handle, uint32_t flags, bool_t enable)
{
  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(handle);

  if (enable)
  {
    pCtx->opModeFlags |= flags;
  }
  else
  {
    pCtx->opModeFlags &= ~flags;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a connection context.
 *
 *  \return Connection context.
 */
/*************************************************************************************************/
lctrConnCtx_t *lctrAllocConnCtx(void)
{
  unsigned int connIdx;

  for (connIdx = 0; connIdx < pLctrRtCfg->maxConn; connIdx++)
  {
    if (!pLctrConnTbl[connIdx].enabled)
    {
      lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connIdx);

      memset(&pLctrConnTbl[connIdx], 0, sizeof(lctrConnCtx_t));
      pCtx->enabled = TRUE;

      pCtx->opModeFlags = lmgrCb.opModeFlags;

      /* Assume all features are supported; if not supported remote sends unknown response. */
      pCtx->usedFeatSet = lmgrCb.features;

      /* Setup supervision timer. */
      pCtx->tmrSupTimeout.handlerId = lmgrPersistCb.handlerId;
      lctrMsgHdr_t *pMsg = (lctrMsgHdr_t *)&pCtx->tmrSupTimeout.msg;
      pMsg->handle = connIdx;
      pMsg->dispId = LCTR_DISP_CONN;
      pMsg->event = LCTR_CONN_TERM_SUP_TIMEOUT;

      /* Setup LLCP timer. */
      pCtx->tmrProcRsp.handlerId = lmgrPersistCb.handlerId;
      pMsg = (lctrMsgHdr_t *)&pCtx->tmrProcRsp.msg;
      pMsg->handle = connIdx;
      pMsg->dispId = LCTR_DISP_CONN;
      pMsg->event = LCTR_CONN_TMR_LLCP_RSP_EXP;

      /* LE Ping period timer. */
      pCtx->tmrPingTimeout.handlerId = lmgrPersistCb.handlerId;
      pMsg = (lctrMsgHdr_t *)&pCtx->tmrPingTimeout.msg;
      pMsg->handle = connIdx;
      pMsg->dispId = LCTR_DISP_CONN;
      pMsg->event = LCTR_CONN_TMR_PING_PERIOD_EXP;

      /* Setup authentication payload timer. */
      pCtx->tmrAuthTimeout.handlerId = lmgrPersistCb.handlerId;
      pMsg = (lctrMsgHdr_t *)&pCtx->tmrAuthTimeout.msg;
      pMsg->handle = connIdx;
      pMsg->dispId = LCTR_DISP_CONN;
      pMsg->event = LCTR_CONN_TMR_AUTH_PAYLOAD_EXP;

      /* Default packet lengths. */
      pCtx->localDataPdu.maxTxLen = lmgrConnCb.maxTxLen;
      pCtx->localDataPdu.maxRxLen = WSF_MIN(LCTR_MAX_DATA_LEN_MAX, pLctrRtCfg->maxAclLen);
      pCtx->localDataPdu.maxTxTime = lmgrConnCb.maxTxTime;
      /* Limit with absolute time. lctrSendDataLengthPdu() limits time by PHY capability. */
      pCtx->localDataPdu.maxRxTime = LL_DATA_LEN_TO_TIME_CODED_S8(pCtx->localDataPdu.maxRxLen);
      pCtx->effDataPdu.maxTxLen = LL_MAX_DATA_LEN_MIN;
      pCtx->effDataPdu.maxRxLen = LL_MAX_DATA_LEN_MIN;
      pCtx->effDataPdu.maxTxTime = LL_MAX_DATA_TIME_MIN;
      pCtx->effDataPdu.maxRxTime = LL_MAX_DATA_TIME_MIN;

      /* Connection event duration (update once PHY is known). */
      pCtx->bleData.chan.txPhy = pCtx->bleData.chan.rxPhy = BB_PHY_BLE_1M;
      pCtx->effConnDurUsec = lctrCalcConnDurationUsec(pCtx, &pCtx->effDataPdu);
      pCtx->localConnDurUsec = lctrCalcConnDurationUsec(pCtx, &pCtx->localDataPdu);

      /* Default PHY. */
      pCtx->allPhys = lmgrConnCb.allPhys;
      pCtx->txPhys = lmgrConnCb.txPhys;
      pCtx->rxPhys = lmgrConnCb.rxPhys;

      /* SCA initialization. */
      pCtx->scaMod = lmgrCb.scaMod;

      /* Default settings for PAST(periodic advertising sync transfer). */
      pCtx->syncMode = lmgrConnCb.syncMode;
      pCtx->syncSkip = lmgrConnCb.syncSkip;
      pCtx->syncTimeout = lmgrConnCb.syncTimeout;

      /* Set default minimum number of used channels. */
      for (unsigned i = 0; i < LL_MAX_PHYS; i++)
      {
        pCtx->peerMinUsedChan[i] = LL_MIN_NUM_CHAN_DATA;
      }

      /* VS initialization. */
      if (pLctrVsHdlrs && pLctrVsHdlrs->connSetup)
      {
        pLctrVsHdlrs->connSetup(connIdx);
      }

      LmgrIncResetRefCount();
      lmgrCb.numConnEnabled++;

      /* Enable BB. */
      BbStart(BB_PROT_BLE);

      return LCTR_GET_CONN_CTX(connIdx);
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief      Free a connection context.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrFreeConnCtx(lctrConnCtx_t *pCtx)
{
  uint8_t *pBuf;
  uint8_t numRxBufs;
  uint8_t numTxBufs;
  wsfHandlerId_t handlerId;

  /* Flush remaining receive packets. */
  numRxBufs = lctrRxConnClear(pCtx);
  lctrDataRxIncAvailBuf(numRxBufs);

  /* Flush remaining transmit packets. */
  numTxBufs = lctrTxQueueClear(pCtx);

  /* Flush remaining transmit packets. */
  while ((pBuf = WsfMsgDeq(&pCtx->txLeuQ, &handlerId)) != NULL)
  {
    if (handlerId != LCTR_CTRL_DATA_HANDLE)
    {
      lctrDataTxIncAvailBuf();
      numTxBufs++;
    }

    WsfMsgFree(pBuf);
  }

  /* Cleanup timers. */
  WsfTimerStop(&pCtx->tmrSupTimeout);
  WsfTimerStop(&pCtx->tmrProcRsp);
  WsfTimerStop(&pCtx->tmrPingTimeout);
  WsfTimerStop(&pCtx->tmrAuthTimeout);

  uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  /* VS cleanup. */
  if (pLctrVsHdlrs && pLctrVsHdlrs->connCleanup)
  {
    pLctrVsHdlrs->connCleanup(handle);
  }

  pCtx->enabled = FALSE;

  LmgrDecResetRefCount();
  lmgrCb.numConnEnabled--;

  /* Shutdown BB. */
  BbStop(BB_PROT_BLE);

  LL_TRACE_INFO1("    >>> Connection terminated, handle=%u <<<", handle);
}

/*************************************************************************************************/
/*!
 *  \brief  Compute next data channel index.
 *
 *  \param  pCtx    Connection context.
 *  \param  numSkip Number of channels to skip.
 *
 *  \return Next data channel index.
 */
/*************************************************************************************************/
uint8_t lctrSelectNextDataChannel(lctrConnCtx_t *pCtx, uint16_t numSkip)
{
  /* unmappedChannel = (lastUnmappedChannel + hopIncrement) mod 37 */
  uint16_t unmapChan = pCtx->lastChanIdx + ((uint16_t)pCtx->hopInc * (numSkip + 1));
  if (unmapChan > LL_CHAN_DATA_MAX_IDX)
  {
    unmapChan = LL_MATH_MOD_37(unmapChan);
  }

  pCtx->lastChanIdx = unmapChan;

  if (!((UINT64_C(1) << unmapChan) & pCtx->chanMask))
  {
    /* remappingIndex = unmappedChannel mod numUsedChannels */

    uint8_t remapIdx;

    /* Avoid division. */
    if (unmapChan < pCtx->numUsedChan)
    {
      remapIdx = unmapChan;
    }
    else if (pCtx->numUsedChan > 18)    /* max is 37; no double rollovers */
    {
      remapIdx = unmapChan - pCtx->numUsedChan;
    }
    else
    {
      /* Unlikely case where available channels <= 18, loop is required. */
      remapIdx = unmapChan;
      while (remapIdx >= pCtx->numUsedChan)
      {
        remapIdx -= pCtx->numUsedChan;
      }
    }

    return pCtx->chanRemapTbl[remapIdx];
  }

  return unmapChan;
}

/*************************************************************************************************/
/*!
 *  \brief  Build channel remapping table.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrBuildRemapTable(lctrConnCtx_t *pCtx)
{
  unsigned int chanIdx;
  unsigned int numUsedChan = 0;

  for (chanIdx = 0; chanIdx < sizeof(pCtx->chanRemapTbl); chanIdx++)
  {
    if (pCtx->chanMask & (UINT64_C(1) << chanIdx))
    {
      pCtx->chanRemapTbl[numUsedChan++] = chanIdx;
    }
  }

  WSF_ASSERT(numUsedChan);          /* must have at least one channel */

  pCtx->numUsedChan = numUsedChan;
}

/*************************************************************************************************/
/*!
 *  \brief      Tx data completed task event handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrConnTxCompletedHandler(void)
{
  WSF_CS_INIT(cs);
  uint8_t numBufs[LL_MAX_CONN] = { 0 };
  unsigned int i = pLctrRtCfg->maxConn - 1;

  /* Cache buffer count within single CS. */
  WSF_CS_ENTER(cs);
  do
  {
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(i);

    if (pCtx->enabled)
    {
      numBufs[i] = pCtx->numTxComp;
      pCtx->numTxComp = 0;
    }
  } while (i--);
  WSF_CS_EXIT(cs);

  /* Call completion callbacks. */
  i = pLctrRtCfg->maxConn - 1;
  do
  {
    if (numBufs[i])
    {
      if (lmgrPersistCb.sendCompCback)
      {
        lmgrPersistCb.sendCompCback(i, numBufs[i]);
      }
    }

    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(i);

    if (pCtx->pauseTxData &&
        WsfQueueEmpty(&pCtx->txArqQ))
    {
      lctrSendConnMsg(pCtx, LCTR_CONN_ARQ_Q_FLUSHED);
    }
  } while (i--);
}

/*************************************************************************************************/
/*!
 *  \brief      Rx data pending task event handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrConnRxPendingHandler(void)
{
  uint16_t connHandle = 0;
  uint8_t *pRxBuf;

  /* Route and demux received Data PDUs. */

  while ((pRxBuf = lctrRxDeq(&connHandle)) != NULL)
  {
    WSF_ASSERT(pRxBuf);

    WSF_ASSERT(connHandle < pLctrRtCfg->maxConn);
    lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connHandle);

    if (!pCtx->enabled)
    {
      LL_TRACE_ERR1("!!! Data received on terminated connHandle=%u", connHandle);
      lctrRxPduFree(pRxBuf);
      lctrDataRxIncAvailBuf(1);
      continue;
    }

    /* Disassemble PDU. */
    lctrDataPduHdr_t rxHdr;
    lctrUnpackDataPduHdr(&rxHdr, pRxBuf);

    /* Decrypt PDU. */
    if (lctrPktDecryptHdlr)
    {
      if (lctrPktDecryptHdlr(&pCtx->bleData.chan.enc, pRxBuf))
      {
        if (pCtx->bleData.chan.enc.enaDecrypt)
        {
          /* Restart authentication timers. */
          WsfTimerStartMs(&pCtx->tmrAuthTimeout, pCtx->authTimeoutMs);
          WsfTimerStartMs(&pCtx->tmrPingTimeout, pCtx->pingPeriodMs);
        }
      }
      else
      {
        LL_TRACE_ERR1("!!! MIC verification failed on connHandle=%u", connHandle);
        lctrRxPduFree(pRxBuf);
        lctrDataRxIncAvailBuf(1);
        lctrSendConnMsg(pCtx, LCTR_CONN_TERM_MIC_FAILED);
        continue;
      }
    }

    /* Demux PDU. */
    switch (rxHdr.llid)
    {
      case LL_LLID_CTRL_PDU:
      {
#if (LL_ENABLE_TESTER)
        if (llTesterCb.llcpForwardEnabled)
        {
          if (!llTesterCb.llcpLlcpIntercept)
          {
            if (lctrCtrlPduHdlr != NULL)
            {
              lctrCtrlPduHdlr(pCtx, pRxBuf);
            }
          }
          pRxBuf[0] = LL_LLID_VS_PDU;     /* buffer is treated like a VS buffer */
          lctrRxConnEnq(pCtx, pRxBuf);
          /* lctrRxPduFree(pRxBuf); */        /* Freed in rxDataQ handler. */
          /* lctrDataRxIncAvailBuf(); */      /* Incremented in rxDataQ handler. */
        }
        else
#endif
        {
          if (lctrCtrlPduHdlr != NULL)
          {
            lctrCtrlPduHdlr(pCtx, pRxBuf);
          }
          lctrRxPduFree(pRxBuf);
          lctrDataRxIncAvailBuf(1);
        }
        break;
      }

      case LL_LLID_START_PDU:
      case LL_LLID_CONT_PDU:
        if (pCtx->pauseRxData)
        {
          LL_TRACE_ERR1("!!! Data PDU received during data pause, connHandle=%u", connHandle);
          lctrRxPduFree(pRxBuf);
          lctrDataRxIncAvailBuf(1);
          lctrSendConnMsg(pCtx, LCTR_CONN_TERM_MIC_FAILED);
        }
        else
        {
          lctrRxConnEnq(pCtx, pRxBuf);
          /* lctrRxPduFree(pRxBuf); */        /* Freed in rxDataQ handler. */
          /* lctrDataRxIncAvailBuf(); */      /* Incremented in rxDataQ handler. */
        }
        break;

      case LL_LLID_VS_PDU:
      default:
        if (pLctrVsHdlrs && pLctrVsHdlrs->dataRecv)
        {
          pLctrVsHdlrs->dataRecv(connHandle, pRxBuf);
        }
        else
        {
          lctrRxPduFree(pRxBuf);
          lctrDataRxIncAvailBuf(1);
          LL_TRACE_ERR1("!!! Invalid LLID; dropping Rx data PDU, connHandle=%u", connHandle);
        }
        break;
    }
  }

  /* Notify host of pending Rx data. */

  WSF_CS_INIT(cs);
  uint8_t numBufs[LL_MAX_CONN];
  unsigned int i = pLctrRtCfg->maxConn - 1;

  /* Cache buffer count within single CS. */
  WSF_CS_ENTER(cs);
  do
  {
    numBufs[i] = pLctrConnTbl[i].numRxPend;
    pLctrConnTbl[i].numRxPend = 0;
  } while (i--);
  WSF_CS_EXIT(cs);

  /* Call completion callbacks. */
  i = pLctrRtCfg->maxConn - 1;
  do
  {
    if (numBufs[i] && lmgrPersistCb.recvPendCback)
    {
      lmgrPersistCb.recvPendCback(i, numBufs[i]);
    }
  } while (i--);
}

/*************************************************************************************************/
/*!
 *  \brief  Transmit ACL data path.
 *
 *  \param  pAclBuf     ACL buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrTxAcl(uint8_t *pAclBuf)
{
  lctrAclHdr_t aclHdr;

  /*** Disassemble ACL packet. ***/

  lctrUnpackAclHdr(&aclHdr, pAclBuf);

  /*** Assemble data PDU. ***/

  if (aclHdr.connHandle >= pLctrRtCfg->maxConn)
  {
    LL_TRACE_ERR1("Invalid ACL header: connHandle=%u; dropping packet", aclHdr.connHandle);
    WsfMsgFree(pAclBuf);
    lmgrPersistCb.sendCompCback(aclHdr.connHandle, 1);
    return;
  }

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(aclHdr.connHandle);

  if (!pCtx->enabled)
  {
    LL_TRACE_ERR1("Invalid ACL header: connection disconnected for connHandle=%u; dropping packet", aclHdr.connHandle);
    /* LL shall not inform host of the number of completed event */
    WsfMsgFree(pAclBuf);
    return;
  }

  if (pCtx->state == LCTR_CONN_STATE_TERMINATING)
  {
    LL_TRACE_ERR1("Invalid ACL header: connection terminated for connHandle=%u; dropping packet", aclHdr.connHandle);
    WsfMsgFree(pAclBuf);
    lmgrPersistCb.sendCompCback(aclHdr.connHandle, 1);
    return;
  }

  if (!pLctrVsHdlrs && (aclHdr.pktBound == LCTR_PB_VS_DATA))
  {
    LL_TRACE_ERR1("Invalid ACL header: pktBound=%u not valid; dropping packet", aclHdr.pktBound);
    WsfMsgFree(pAclBuf);
    lmgrPersistCb.sendCompCback(aclHdr.connHandle, 1);
    return;
  }

  if (((aclHdr.pktBound != LCTR_PB_VS_DATA) && (aclHdr.len == 0)) ||
      (aclHdr.len > pLctrRtCfg->maxAclLen))
  {
    LL_TRACE_ERR2("Invalid ACL header: invalid packet length, actLen=%u, maxLen=%u", aclHdr.len, pCtx->effDataPdu.maxTxLen);
    if ((aclHdr.pktBound == LCTR_PB_START_NON_AUTO_FLUSH) && (aclHdr.len == 0))
    {
      pCtx->forceStartPdu = TRUE; /* If this was supposed to be the start fragment, make the next packet a start fragment. */
      LL_TRACE_INFO0("Next ACL header will be forced to a start fragment.");
    }
    WsfMsgFree(pAclBuf);
    lmgrPersistCb.sendCompCback(aclHdr.connHandle, 1);
    return;
  }

  lctrDataTxDecAvailBuf();

  if (!pCtx->pauseTxData)
  {
    WSF_ASSERT(WsfQueueEmpty(&pCtx->txLeuQ));

    /*** Queue for transmit. ***/

    uint16_t fragLen = lctrTxFragLen(pCtx);
    lctrTxDataPduQueue(pCtx, fragLen, &aclHdr, pAclBuf);
    WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_TX_PENDING));
  }
  else
  {
    /*** Pend in LE-U pause queue. ***/

    WsfMsgEnq(&pCtx->txLeuQ, aclHdr.connHandle, pAclBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Receive ACL data path.
 *
 *  \return Received data PDU buffer or NULL if queue empty.
 */
/*************************************************************************************************/
uint8_t *LctrRxAcl(void)
{
  unsigned int i = pLctrRtCfg->maxConn - 1;
  uint8_t *pAclBuf = NULL;

  do
  {
    if ((pLctrConnTbl[i].enabled) &&
        ((pAclBuf = lctrRxConnDeqAcl(&pLctrConnTbl[i])) != NULL))
    {
      break;
    }

  } while (i--);

  return pAclBuf;
}

/*************************************************************************************************/
/*!
 *  \brief      Indicate that received ACL data buffer has been deallocated
 *
 *  \param      numBufs     Number of completed packets.
 *
 *  \return     None.
 *
 *  Indicate that received ACL data buffer has been deallocated.
 */
/*************************************************************************************************/
void LctrRxAclComplete(uint8_t numBufs)
{
  lctrDataRxIncAvailBuf(numBufs);
}

/*************************************************************************************************/
/*!
 *  \brief  Get enable state of a handle.
 *
 *  \param  handle      Connection handle.
 *
 *  \return TRUE if enabled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LctrIsConnHandleEnabled(uint16_t handle)
{
  return pLctrConnTbl[handle].enabled;
}

/*************************************************************************************************/
/*!
 *  \brief  Get enable state of a handle.
 *
 *  \param  handle      Connection handle.
 *
 *  \return TRUE if enabled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LctrIsCisConnHandleEnabled(uint16_t handle)
{
  if (lctrCheckCisEstCisFn)
  {
    return lctrCheckCisEstCisFn(handle);
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Get role of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Role.
 */
/*************************************************************************************************/
uint8_t LctrGetRole(uint16_t handle)
{
  return pLctrConnTbl[handle].role;
}

/*************************************************************************************************/
/*!
 *  \brief  Get RSSI value of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return RSSI value.
 */
/*************************************************************************************************/
int8_t LctrGetRssi(uint16_t handle)
{
  return pLctrConnTbl[handle].rssi;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the transmit power level of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Transmit power level.
 */
/*************************************************************************************************/
int8_t LctrGetTxPowerLevel(uint16_t handle)
{
  return pLctrConnTbl[handle].bleData.chan.txPower;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the transmit power level of a connection.
 *
 *  \param  handle      Connection handle.
 *  \param  level       Transmit power level.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrSetTxPowerLevel(uint16_t handle, int8_t level)
{
  pLctrConnTbl[handle].bleData.chan.txPower = level;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the channel map mask of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Channel map mask.
 */
/*************************************************************************************************/
uint64_t LctrGetChannelMap(uint16_t handle)
{
  return pLctrConnTbl[handle].chanMask;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the used feature set of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Used feature set bitmask.
 */
/*************************************************************************************************/
uint64_t LctrGetUsedFeatures(uint16_t handle)
{
//  LL_TRACE_INFO1("LctrGetUsedFeatures, lmgrCb.features=%x", lmgrCb.features);
//  LL_TRACE_INFO1("LctrGetUsedFeatures, pLctrConnTbl[handle].usedFeatSet=%x", pLctrConnTbl[handle].usedFeatSet);

  return pLctrConnTbl[handle].usedFeatSet;
}

/*************************************************************************************************/
/*!
 *  \brief      Get transmitter PHY used by a connection.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Transmitter PHY.
 *
 *  Get transmitter PHY used by a connection.
 */
/*************************************************************************************************/
uint8_t LctrGetTxPhy(uint16_t handle)
{
  return pLctrConnTbl[handle].bleData.chan.txPhy;
}

/*************************************************************************************************/
/*!
 *  \brief      Get receiver PHY used by a connection.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Receiver PHY.
 *
 *  Get receiver PHY used by a connection.
 */
/*************************************************************************************************/
uint8_t LctrGetRxPhy(uint16_t handle)
{
  return pLctrConnTbl[handle].bleData.chan.rxPhy;
}

/*************************************************************************************************/
/*!
 *  \brief      Get peer minimum number of used channels.
 *
 *  \param      handle          Connection handle.
 *  \param      pPeerMinCh      Peer minimum used channels.
 *
 *  \return     Peer minimum number of used channels.
 *
 *  Get peer minimum number of used channels.
 */
/*************************************************************************************************/
void LctrGetPeerMinUsedChan(uint16_t handle, uint8_t *pPeerMinCh)
{
  for (unsigned i = 0; i < LL_MAX_PHYS; i++)
  {
    pPeerMinCh[i] = pLctrConnTbl[handle].peerMinUsedChan[i];
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Is current a connection context waiting for a host reply?
 *
 *  \param  handle      Connection handle.
 *  \param  reply       Reply bitmask.
 *
 *  \return TRUE if current connection context is waiting for host reply, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LctrIsWaitingForReply(uint16_t handle, uint8_t reply)
{
  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(handle);

  return (pCtx->replyWaitingMsk & reply) ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Send internal connection subsystem message.
 *
 *  \param  pCtx        Connection context.
 *  \param  event       Connection event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrSendConnMsg(lctrConnCtx_t *pCtx, uint8_t event)
{
  lctrMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(lctrMsgHdr_t))) != NULL)
  {
    pMsg->handle = LCTR_GET_CONN_HANDLE(pCtx);
    pMsg->dispId = LCTR_DISP_CONN;
    pMsg->event = event;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Compute the LE Ping period value.
 *
 *  \param  pCtx            Connection context.
 *  \param  authTimeoutMs   Authentication timeout value in milliseconds.
 *
 *  \return LE Ping period in milliseconds.
 */
/*************************************************************************************************/
uint32_t lctrCalcPingPeriodMs(lctrConnCtx_t *pCtx, uint32_t authTimeoutMs)
{
  /* LE Ping attempts before the expiration of authentication payload timeout. */
  uint32_t pingAttemptsMs = LCTR_CONN_IND_MS((LCTR_LE_PING_ATTEMPTS + pCtx->maxLatency) * pCtx->connInterval);
  uint32_t pingPeriodMs   = authTimeoutMs; /* fallback */

  if ((authTimeoutMs > pingAttemptsMs) &&  /* Timeout must be longer than time to attempt ping */
      (authTimeoutMs > WSF_MS_PER_TICK))   /*             and longer than one tick.            */
  {
    pingPeriodMs = authTimeoutMs - pingAttemptsMs;

    if (pingPeriodMs < WSF_MS_PER_TICK)   /* minimum */
    {
      /* At least 1 timer tick. */
      pingPeriodMs = WSF_MS_PER_TICK;
    }
  }

  return pingPeriodMs;
}

/*************************************************************************************************/
/*!
 *  \brief  Connection conflict resolution handler.
 *
 *  \param  pNewOp      New BOD.
 *  \param  pExistOp    Existing BOD.
 *
 *  \return Prioritized BOD.
 *
 *  Prioritize BLE connection at most risk for termination.
 */
/*************************************************************************************************/
BbOpDesc_t *lctrConnResolveConflict(BbOpDesc_t *pNewOp, BbOpDesc_t *pExistOp)
{
  lctrConnCtx_t *pNewCtx = pNewOp->pCtx;
  lctrConnCtx_t *pExistCtx = pExistOp->pCtx;

  WSF_ASSERT(pNewOp->protId == BB_PROT_BLE);
  WSF_ASSERT((pNewOp->prot.pBle->chan.opType == BB_BLE_OP_SLV_CONN_EVENT) ||
             (pNewOp->prot.pBle->chan.opType == BB_BLE_OP_MST_CONN_EVENT));

  /* BLE connections. */
  if ((pExistOp->protId != BB_PROT_BLE) ||
      !((pExistOp->prot.pBle->chan.opType == BB_BLE_OP_SLV_CONN_EVENT) ||
        (pExistOp->prot.pBle->chan.opType == BB_BLE_OP_MST_CONN_EVENT)))
  {
    LL_TRACE_WARN1("!!! Scheduling conflict, BLE connections: incoming handle=%u prioritized over non-BLE operation", LCTR_GET_CONN_HANDLE(pNewCtx));
    return pNewOp;
  }

  /* Established connections. */
  if (pNewCtx->state == LCTR_CONN_STATE_INITIALIZED)
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, established connection: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
    return pExistOp;
  }

  /* Supervision timeout is imminent (2 CE). */
  if ((pExistCtx->svtState > pNewCtx->svtState) ||
      ((pExistCtx->tmrSupTimeout.ticks * WSF_MS_PER_TICK * 1000) < (uint32_t)(LCTR_CONN_IND_US(pExistCtx->connInterval) << 1)))
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, imminent SVT: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
    return pExistOp;
  }

  if ((pNewCtx->svtState != LCTR_SVT_STATE_IDLE) ||
      ((pNewCtx->tmrSupTimeout.ticks * WSF_MS_PER_TICK * 1000) < (uint32_t)(LCTR_CONN_IND_US(pNewCtx->connInterval) << 1)))
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, imminent SVT: incoming handle=%u prioritized over existing handle=%u", LCTR_GET_CONN_HANDLE(pNewCtx), LCTR_GET_CONN_HANDLE(pExistCtx));
    return pNewOp;
  }

  /* Active LLCP is pending. */
  if (pExistCtx->llcpActiveProc != LCTR_PROC_INVALID)
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, LLCP pending: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
    return pExistOp;
  }
  if (pNewCtx->llcpActiveProc != LCTR_PROC_INVALID)
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, LLCP pending: incoming handle=%u prioritized over existing handle=%u", LCTR_GET_CONN_HANDLE(pNewCtx), LCTR_GET_CONN_HANDLE(pExistCtx));
    return pNewOp;
  }

  /* Data is pending. */
  if (!WsfQueueEmpty(&pExistCtx->txArqQ))
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, data pending: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
    return pExistOp;
  }
  if (!WsfQueueEmpty(&pNewCtx->txArqQ))
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, data pending: incoming handle=%u prioritized over existing handle=%u", LCTR_GET_CONN_HANDLE(pNewCtx), LCTR_GET_CONN_HANDLE(pExistCtx));
    return pNewOp;
  }

  /* Less frequent connInterval (4x). */
  if ((pExistCtx->connInterval >> 2) > pNewCtx->connInterval)
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, CI frequency: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
    return pExistOp;
  }
  if ((pNewCtx->connInterval >> 2) > pExistCtx->connInterval)
  {
    LL_TRACE_WARN2("!!! Scheduling conflict, CI frequency: incoming handle=%u prioritized over existing handle=%u", LCTR_GET_CONN_HANDLE(pNewCtx), LCTR_GET_CONN_HANDLE(pExistCtx));
    return pNewOp;
  }

  /* Default. */
  LL_TRACE_WARN2("!!! Scheduling conflict, default: existing handle=%u prioritized over incoming handle=%u", LCTR_GET_CONN_HANDLE(pExistCtx), LCTR_GET_CONN_HANDLE(pNewCtx));
  return pExistOp;
}

/*************************************************************************************************/
/*!
 *  \brief  Compute the master sleep clock accuracy index.
 *
 *  \return SCA index.
 */
/*************************************************************************************************/
uint8_t lctrComputeSca(void)
{
  const uint16_t clkPpm = BbGetClockAccuracy();
  int8_t sca;

       if (clkPpm <=  20) sca = 7;
  else if (clkPpm <=  30) sca = 6;
  else if (clkPpm <=  50) sca = 5;
  else if (clkPpm <=  75) sca = 4;
  else if (clkPpm <= 100) sca = 3;
  else if (clkPpm <= 150) sca = 2;
  else if (clkPpm <= 250) sca = 1;
  else                    sca = 0;

  return (uint8_t) (sca + lmgrCb.scaMod);
}

/*************************************************************************************************/
/*!
 *  \brief      Get reference time(due time) of the connection handle.
 *
 *  \param      connHandle    Connection handle.
 *  \param      pDurUsec      Pointer to duration of the connection BOD.
 *
 *  \return     Due time in BB ticks of the connection handle.
 */
/*************************************************************************************************/
uint32_t lctrGetConnRefTime(uint8_t connHandle, uint32_t *pDurUsec)
{
  uint32_t refTime = 0;
  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connHandle);

  if (pCtx->enabled && (pCtx->bleData.chan.opType == BB_BLE_OP_MST_CONN_EVENT))
  {
    refTime = pCtx->connBod.due;
    if (pDurUsec)
    {
      *pDurUsec = pCtx->connBod.minDurUsec;
    }
  }

  return refTime;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the time of anchor point of the specified connection event counter.
 *
 *  \param      pCtx        Connection context.
 *  \param      ceCounter     Connection event counter.
 *
 *  \return     The time of anchor point of the given connection event counter.
 */
/*************************************************************************************************/
uint32_t lctrConnGetAnchorPoint(lctrConnCtx_t *pCtx, uint16_t ceCounter)
{
  uint16_t numCe;

  if (pCtx->role == LL_ROLE_MASTER)
  {
    if ((uint16_t)(ceCounter - pCtx->eventCounter) < (uint16_t)LCTR_MAX_INSTANT)  /* ceCounter is in the future. */
    {
      numCe = ceCounter - pCtx->eventCounter;
      return (pCtx->connBod.due + BB_US_TO_BB_TICKS(LCTR_CONN_IND_US(pCtx->connInterval * numCe)));
    }
    else
    {
      numCe = pCtx->eventCounter - ceCounter;
      return (pCtx->connBod.due - BB_US_TO_BB_TICKS(LCTR_CONN_IND_US(pCtx->connInterval * numCe)));
    }
  }
  else
  {
    if ((uint16_t)(ceCounter - (pCtx->data.slv.lastActiveEvent - 1)) < (uint16_t)LCTR_MAX_INSTANT)  /* ceCounter is in the future. */
    {
      numCe = ceCounter - (pCtx->data.slv.lastActiveEvent - 1);
      return (pCtx->data.slv.anchorPoint + BB_US_TO_BB_TICKS(LCTR_CONN_IND_US(pCtx->connInterval * numCe)));
    }
    else
    {
      numCe = (pCtx->data.slv.lastActiveEvent - 1) - ceCounter;
      return (pCtx->data.slv.anchorPoint - BB_US_TO_BB_TICKS(LCTR_CONN_IND_US(pCtx->connInterval * numCe)));
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Check if there is a CIS established for the ACL indicated by the handle.
 *
 *  \param  handle      Connection handle.
 *
 *  \return TRUE if there is one established, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LctrIsCisEnabled(uint16_t handle)
{
  if (pLctrConnTbl[handle].checkCisEstAcl)
  {
    return pLctrConnTbl[handle].checkCisEstAcl(handle);
  }

  return FALSE;
}
