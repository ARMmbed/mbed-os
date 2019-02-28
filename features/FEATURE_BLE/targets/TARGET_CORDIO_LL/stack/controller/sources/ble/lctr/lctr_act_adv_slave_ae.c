/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Link layer controller slave extended advertising action routines.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave_ae.h"
#include "lctr_int_adv_slave.h"
#include "lmgr_api_adv_slave_ae.h"
#include "bb_ble_api_reslist.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "util/bstream.h"

#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Common advertise resource cleanup.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrExtAdvCleanup(lctrAdvSet_t *pAdvSet)
{
  lctrMsgHdr_t *pMsg;

  BbBleSlvAuxAdvEvent_t * const pAuxAdv = &pAdvSet->auxBleData.op.slvAuxAdv;

  if ((pMsg = (lctrMsgHdr_t *)pAuxAdv->pRxAuxReqBuf) != NULL)
  {
    /* Recover header. */
    WsfMsgFree(pMsg - 1);
    pAuxAdv->pRxAuxReqBuf = NULL;
  }

  BbBleSlvAdvEvent_t * const pAdv = &pAdvSet->bleData.op.slvAdv;
  if ((pMsg = (lctrMsgHdr_t *)pAdv->pRxReqBuf) != NULL)
  {
    /* Recover header. */
    WsfMsgFree(pMsg - 1);
    pAdv->pRxReqBuf = NULL;
  }

  if (pAdvSet->param.advFiltPolicy & LL_ADV_FILTER_SCAN_WL_BIT)
  {
    LmgrDecWhitelistRefCount();
  }

  WSF_ASSERT(lmgrCb.numExtAdvEnabled > 0);
  lmgrCb.numExtAdvEnabled--;
}

/*************************************************************************************************/
/*!
 *  \brief      Start extended advertising.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActStart(lctrAdvSet_t *pAdvSet)
{
  BbStart(BB_PROT_BLE);

  /* Reset state. */
  pAdvSet->maxEvents = pLctrSlvExtAdvMsg->enable.maxEvents;
  pAdvSet->numEvents = 0;
  pAdvSet->termReason = LL_SUCCESS;
  pAdvSet->pExtAdvAuxPtr = NULL;
  pAdvSet->connIndRcvd = FALSE;
  pAdvSet->shutdown = FALSE;
  pAdvSet->bodTermCnt = 0;

  uint8_t status;
  if ((status = lctrSlvExtAdvBuildOp(pAdvSet, pLctrSlvExtAdvMsg->enable.durMs)) != LL_SUCCESS)
  {
    // TODO suppress terminate event on failed start
    LmgrSendExtAdvEnableCnf(pAdvSet->handle, status);
    lctrSendAdvSetMsg(pAdvSet, LCTR_EXT_ADV_MSG_TERMINATE);
    return;
  }

  lmgrCb.numExtAdvEnabled++;

  if (pAdvSet->param.advFiltPolicy)
  {
    LmgrIncWhitelistRefCount();
  }

  if (pLctrSlvExtAdvMsg->enable.durMs)
  {
    WsfTimerStartMs(&pAdvSet->tmrAdvDur, pLctrSlvExtAdvMsg->enable.durMs);
  }

  LmgrSendExtAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Restart extended advertising.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActRestart(lctrAdvSet_t *pAdvSet)
{
  WsfTimerStop(&pAdvSet->tmrAdvDur);

  if (pLctrSlvExtAdvMsg->enable.durMs)
  {
    WsfTimerStartMs(&pAdvSet->tmrAdvDur, pLctrSlvExtAdvMsg->enable.durMs);
  }

  pAdvSet->maxEvents = pLctrSlvExtAdvMsg->enable.maxEvents;
  pAdvSet->numEvents = 0;

  LmgrSendExtAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active advertising operation.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActShutdown(lctrAdvSet_t *pAdvSet)
{
  WsfTimerStop(&pAdvSet->tmrAdvDur);

  pAdvSet->termReason = LL_SUCCESS;
  pAdvSet->shutdown = TRUE;

  /* TODO: Attempt to remove BOD immediately to reduce shutdown time. */

  /* Shutdown completes with events generated in BOD end callback. */
}

/*************************************************************************************************/
/*!
 *  \brief      Send advertising operation confirm.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActAdvCnf(lctrAdvSet_t *pAdvSet)
{
  LmgrSendExtAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Send advertising operation command disallowed.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActDisallowAdvCnf(lctrAdvSet_t *pAdvSet)
{
  LmgrSendExtAdvEnableCnf(pAdvSet->handle, LL_ERROR_CODE_CMD_DISALLOWED);
}

/*************************************************************************************************/
/*!
 *  \brief      Operation self terminated (e.g. on connection request).
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 *
 *  Slave self-termination is a result of the expiration of the duration timer, numEvents reached,
 *  or reception of an AUX_CONN_REQ.
 */
/*************************************************************************************************/
void lctrExtAdvActSelfTerm(lctrAdvSet_t *pAdvSet)
{
  if (!pAdvSet->connIndRcvd)
  {
    WsfTimerStop(&pAdvSet->tmrAdvDur);

    lctrExtAdvCleanup(pAdvSet);
    BbStop(BB_PROT_BLE);

    LmgrSendAdvSetTermInd(pAdvSet->handle, pAdvSet->termReason, 0, pAdvSet->numEvents);

    const uint8_t LEGACY_HIGH_DUTY = (LL_ADV_EVT_PROP_LEGACY_ADV_BIT | LL_ADV_EVT_PROP_HIGH_DUTY_ADV_BIT |
                                      LL_ADV_EVT_PROP_DIRECT_ADV_BIT | LL_ADV_EVT_PROP_CONN_ADV_BIT);
    if ((pAdvSet->param.advEventProp & LEGACY_HIGH_DUTY) == LEGACY_HIGH_DUTY )
    {
      /* Legacy connection complete event doesn't include peerRpa and localRpa. */
      lctrNotifyHostConnectInd(pAdvSet->handle, LL_ROLE_SLAVE, NULL,
                               pAdvSet->param.peerAddrType, pAdvSet->param.peerAddr, 0, 0,
                               LL_ERROR_CODE_ADV_TIMEOUT, 0);
    }
  }
  else /* CONNECT_IND or CONNECT_REQ received */
  {
    BbBleData_t * const pBle = &pAdvSet->bleData;
    BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;
    BbBleSlvAuxAdvEvent_t * const pAuxAdv = &pAdvSet->auxBleData.op.slvAuxAdv;

    uint8_t *pBuf;
    bool_t startConn = TRUE;

    if ((pBuf = pAdv->pRxReqBuf) != NULL)
    {
      /* Legacy advertising PDU. */

      /* If peer address was not resolved, attempt to resolve it now. */
      if (!BbBlePduFiltCheck(pBuf, &pBle->pduFilt, TRUE, &pAdv->filtResults))
      {
        startConn = FALSE;
      }
    }
    else if ((pBuf = pAuxAdv->pRxAuxReqBuf) != NULL)
    {
      /* Extended advertising PDU. */
      lctrAdvbPduHdr_t hdr;
      uint8_t *pTempBuf = pBuf;
      lctrConnInd_t connInd;

      pTempBuf += lctrUnpackAdvbPduHdr(&hdr, pTempBuf);
      lctrUnpackConnIndPdu(&connInd, pTempBuf);

      /* If peer address was not resolved, attempt to resolve it now. */
      bbBlePduExtFiltParams_t params;

      memset(&params, 0, sizeof(bbBlePduExtFiltParams_t));
      params.pduType = hdr.pduType;
      params.extHdrFlags |= (LL_EXT_HDR_ADV_ADDR_BIT | LL_EXT_HDR_TGT_ADDR_BIT);    /* ScanA and AdvA are mandatory. */
      params.peerAddr = connInd.initAddr;
      params.peerAddrRand = hdr.txAddrRnd;
      params.localAddr = connInd.advAddr;
      params.localAddrRand = hdr.rxAddrRnd;

      if (!BbBleExtPduFiltCheck(&params, &pBle->pduFilt, TRUE, &pAuxAdv->filtResults))
      {
        startConn = FALSE;
      }
    }

    if (startConn && pBuf)
    {
      lctrConnEstablish_t *pMsg;

      if ((pMsg = (lctrConnEstablish_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
      {
        lctrAdvbPduHdr_t hdr;

        pMsg->hdr.handle = pAdvSet->handle;
        pMsg->hdr.dispId = LCTR_DISP_CONN_IND;
        /* pMsg->hdr.event = 0; */

        pBuf += lctrUnpackAdvbPduHdr(&hdr, pBuf);
        lctrUnpackConnIndPdu(&pMsg->connInd, pBuf);

        if (pAdv->pRxReqBuf)
        {
          /* Legacy advertising. */
          BbBlePduFiltResultsGetPeerIdAddr(&pAdv->filtResults, &pMsg->peerIdAddr, &pMsg->peerIdAddrType);
          BbBlePduFiltResultsGetPeerRpa(&pAdv->filtResults, &pMsg->peerRpa);
          pMsg->phy = LL_PHY_LE_1M;
        }
        else
        {
          BbBlePduFiltResultsGetPeerIdAddr(&pAuxAdv->filtResults, &pMsg->peerIdAddr, &pMsg->peerIdAddrType);
          BbBlePduFiltResultsGetPeerRpa(&pAuxAdv->filtResults, &pMsg->peerRpa);
          pMsg->phy = pAdvSet->auxBleData.chan.rxPhy;   /* Same PHY as received CONN_IND. */
        }

        pMsg->connIndEndTs   = pAdvSet->connIndEndTs;
        pMsg->localRpa       = lmgrSlvAdvCb.localRpa;

        pMsg->usedChSel = pAdvSet->usedChSel;

        pMsg->sendAdvSetTerm = TRUE;
        pMsg->numExtAdvEvents = pAdvSet->numEvents;
        pMsg->isAuxConnReq = pAdvSet->isAuxConnReq;

        WsfMsgSend(lmgrPersistCb.handlerId, pMsg);

        lctrExtAdvCleanup(pAdvSet);
        /* BbStop(BB_PROT_BLE); */    /* Remains enabled; connection starts immediately afterwards. */
      }
      else
      {
        /* Restart advertising. */
        lctrSendAdvSetMsg(pAdvSet, LCTR_EXT_ADV_MSG_INT_START);

        /* Do not cleanup. Restarting will reuse resources. */
        /* lctrExtAdvCleanup(pAdvSet); */
        /* BbStop(BB_PROT_BLE); */
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated advertising after host advertising disable.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActAdvTerm(lctrAdvSet_t *pAdvSet)
{
  lctrExtAdvCleanup(pAdvSet);
  BbStop(BB_PROT_BLE);

  LmgrSendExtAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Cleanup Advertising Set after host reset.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActReset(lctrAdvSet_t *pAdvSet)
{
  /* Although the AdvSet is freed here, some benign modifications to the context may occurs
   * to complete the SM call path. */
  lctrFreeAdvSet(pAdvSet);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated advertising and cleanup Advertising Set after host reset.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActResetTerm(lctrAdvSet_t *pAdvSet)
{
  lctrExtAdvCleanup(pAdvSet);
  BbStop(BB_PROT_BLE);

  /* Although the AdvSet is freed here, some benign modifications to the context may occurs
   * to complete the SM call path. */
  lctrFreeAdvSet(pAdvSet);
}

/*************************************************************************************************/
/*!
 *  \brief      Advertising set duration timer expired.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtAdvActDurationExpired(lctrAdvSet_t *pAdvSet)
{
  pAdvSet->termReason = LL_ERROR_CODE_ADV_TIMEOUT;

  /* Signal shutdown, event completion occurs in lctrExtAdvActSelfTerm(). */
  pAdvSet->shutdown = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Build channel remapping table.
 *
 *  \param  pChanParam  Channel parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrPeriodicBuildRemapTable(lctrChanParam_t *pChanParam)
{
  unsigned int chanIdx;
  unsigned int numUsedChan = 0;

  for (chanIdx = 0; chanIdx < sizeof(pChanParam->chanRemapTbl); chanIdx++)
  {
    if (pChanParam->chanMask & (UINT64_C(1) << chanIdx))
    {
      pChanParam->chanRemapTbl[numUsedChan++] = chanIdx;
    }
  }

  WSF_ASSERT(numUsedChan);          /* must have at least one channel */

  pChanParam->numUsedChan = numUsedChan;
}

/*************************************************************************************************/
/*!
 *  \brief      Start periodic advertising.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActStart(lctrAdvSet_t *pAdvSet)
{
  uint8_t status;

  BbStart(BB_PROT_BLE);

  pAdvSet->perParam.shutdown = FALSE;
  pAdvSet->perParam.perAccessAddr = lctrComputeAccessAddr();
  pAdvSet->perParam.perEventCounter = 0;
  pAdvSet->perParam.perChanParam.chanMask = LL_CHAN_DATA_ALL;
  lctrPeriodicBuildRemapTable(&pAdvSet->perParam.perChanParam);
  pAdvSet->perParam.perChanParam.usedChSel = LL_CH_SEL_2;
  pAdvSet->perParam.perChanParam.chIdentifier = (pAdvSet->perParam.perAccessAddr >> 16) ^
                                                (pAdvSet->perParam.perAccessAddr >> 0);
  pAdvSet->perParam.perChIdx = lctrPeriodicSelectNextChannel(&pAdvSet->perParam.perChanParam,
                                                              pAdvSet->perParam.perEventCounter);

  if ((status = lctrSlvPeriodicAdvBuildOp(pAdvSet)) != LL_SUCCESS)
  {
    lctrSendPeriodicAdvSetMsg(pAdvSet, LCTR_PER_ADV_MSG_TERMINATE);
    LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, status);
    return;
  }

  pAdvSet->perParam.perAdvEnabled = TRUE;

  /* Need to set the flag to add aux BOD when extened adv is already started. */
  if (pAdvSet->state == LCTR_EXT_ADV_STATE_ENABLED && pAdvSet->auxBodUsed == FALSE)
  {
    pAdvSet->perParam.perAuxStart = TRUE;
  }

  LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Restart periodic advertising.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActUpdate(lctrAdvSet_t *pAdvSet)
{
  LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
  // TODO cause random address to change
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising operation confirm.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActAdvCnf(lctrAdvSet_t *pAdvSet)
{
  LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising operation command disallowed.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActDisallowAdvCnf(lctrAdvSet_t *pAdvSet)
{
  LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, LL_ERROR_CODE_CMD_DISALLOWED);
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active periodic advertising operation.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActShutdown(lctrAdvSet_t *pAdvSet)
{
  pAdvSet->perParam.shutdown = TRUE;
  pAdvSet->perParam.perAdvEnabled = FALSE;
  pAdvSet->perParam.perAuxStart = FALSE;
  /* Shutdown completes with events generated in BOD end callback. */
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated advertising after host periodic advertising disable.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActAdvTerm(lctrAdvSet_t *pAdvSet)
{
  BbStop(BB_PROT_BLE);

  SchRmRemove(LL_MAX_CONN + LCTR_GET_EXT_ADV_INDEX(pAdvSet));

  LmgrSendPeriodicAdvEnableCnf(pAdvSet->handle, LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated periodc advertising after host reset.
 *
 *  \param      pAdvSet     Advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPeriodicAdvActResetTerm(lctrAdvSet_t *pAdvSet)
{
  BbStop(BB_PROT_BLE);

  SchRmRemove(LL_MAX_CONN + LCTR_GET_EXT_ADV_INDEX(pAdvSet));
}
