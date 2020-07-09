/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller slave BIG action routines.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#include "lctr_int_bis_slave.h"
#include "lctr_int_iso.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Start BIS slave advertising.
 *
 *  \param      pBigCtx     BIG context.
 */
/*************************************************************************************************/
void lctrSlvBigActStart(lctrBigCtx_t *pBigCtx)
{
  uint8_t status;

  if ((status = lctrSlvBigBuildOp(pBigCtx)) != LL_SUCCESS)
  {
    lctrSlvBigSendMsg(pBigCtx, LCTR_SLV_BIG_MSG_TERMINATED);
    lctrNotifyHostCreateBigComplete(pBigCtx, status);
    return;
  }

  BbStart(BB_PROT_BLE);

  lctrAdvSet_t * const pAdvSet = pBigCtx->roleData.slv.pAdvSet;

  if (pAdvSet)
  {
    pAdvSet->perParam.perAdvEnabled = TRUE;

    /* Add SyncInfo to the Extended Advertising. */
    if (pAdvSet->state == LCTR_EXT_ADV_STATE_ENABLED)
    {
      if ((pAdvSet->auxBodUsed == FALSE))
      {
        pAdvSet->perParam.perAuxStart = TRUE;
      }

      /* The Advertising DID is required to change when a SyncInfo field is added to or removed. */
      pAdvSet->advData.alt.ext.did = lctrCalcDID(pAdvSet->advData.pBuf, pAdvSet->advData.len);
      pAdvSet->didPerUpdate = TRUE;
    }
  }

  lctrSlvBigSendAcadMsg(pBigCtx, LCTR_ACAD_MSG_BIG_CREATED);

  LmgrIncResetRefCount();
}

/*************************************************************************************************/
/*!
 *  \brief      Broadcast channel map updates.
 *
 *  \param      pBigCtx     BIG context.
 */
/*************************************************************************************************/
void lctrSlvBigActSendChMapUpd(lctrBigCtx_t *pBigCtx)
{
  if (pBigCtx->bcp.actMsk)
  {
    LL_TRACE_INFO1("BIG Control Procedure in progress; pend BIG Channel Map Update, bigHandle=%u", pBigCtx->handle);
    pBigCtx->bcp.pendMsk |= 1 << LL_BIG_OPCODE_CHAN_MAP_IND;
    return;
  }

  uint8_t *pPdu;

  if ((pPdu = lctrBigTxCtrlAlloc(LL_BIG_CHAN_MAP_IND_PDU_LEN)) != NULL)
  {
    uint16_t inst = pBigCtx->eventCounter + LL_BIG_MIN_INSTANT;

    uint8_t *pBuf = pPdu;

    lctrBisDataPduHdr_t hdr =
    {
      .llid = LL_LLID_BIG_CTRL_PDU,
      .cssn = 0,      /* Completed in ISR. */
      .cstf = 0,      /* Always 0. */
      .len  = LL_BIG_OPCODE_LEN + LL_BIG_CHAN_MAP_IND_PDU_LEN
    };

    pBuf += lctrBisPackDataPduHdr(pBuf, &hdr);
    lctrBisPackBigChannelMapInd(pBuf, lmgrCb.chanClass, inst);

    lctrBigTxCtrlQueue(pBigCtx, pPdu, LL_MIN_INSTANT);

    pBigCtx->bcp.actMsk |= 1 << LL_BIG_OPCODE_CHAN_MAP_IND;
    pBigCtx->bcp.chanMapUpd.chanMap = lmgrCb.chanClass;
    pBigCtx->bcp.chanMapUpd.inst = inst;

    LL_TRACE_INFO2("BIG Channel Map Procedure, bigHandle=%u, instant=%u", pBigCtx->handle, inst);

    if (pBigCtx->roleData.slv.pAdvSet)
    {
      /* Temporarily disable BIG Info transmissions. */
      LctrAcadBigInfo_t *pBigInfo = &pBigCtx->roleData.slv.pAdvSet->acadParams[LCTR_ACAD_ID_BIG_INFO].bigInfo;
      pBigInfo->hdr.state = LCTR_ACAD_STATE_DISABLED;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Broadcast terminate.
 *
 *  \param      pBigCtx     BIG context.
 */
/*************************************************************************************************/
void lctrSlvBigActSendTerm(lctrBigCtx_t *pBigCtx)
{
  if (pBigCtx->bcp.actMsk)
  {
    LL_TRACE_INFO1("BIG Control Procedure in progress; pend BIG Terminate, bigHandle=%u", pBigCtx->handle);
    pBigCtx->bcp.pendMsk |= 1 << LL_BIG_OPCODE_BIG_TERM_IND;
    return;
  }

  uint8_t *pBuf;

  if ((pBuf = lctrBigTxCtrlAlloc(LL_BIG_TERMINATE_IND_PDU_LEN)) != NULL)
  {
    uint16_t inst = pBigCtx->eventCounter + LL_BIG_MIN_INSTANT;

    uint8_t *pPdu = pBuf;

    lctrBisDataPduHdr_t hdr =
    {
      .llid = LL_LLID_BIG_CTRL_PDU,
      .cssn = 0,      /* Completed in ISR. */
      .cstf = 0,      /* Always 0. */
      .len  = LL_BIG_OPCODE_LEN + LL_BIG_TERMINATE_IND_PDU_LEN
    };

    pBuf += lctrBisPackDataPduHdr(pBuf, &hdr);
    lctrBisPackBigTerminateInd(pBuf, pBigCtx->bcp.term.reason, inst);

    lctrBigTxCtrlQueue(pBigCtx, pPdu, LL_BIG_MIN_INSTANT);

    pBigCtx->bcp.actMsk |= 1 << LL_BIG_OPCODE_BIG_TERM_IND;
    pBigCtx->bcp.term.inst = inst;

    LL_TRACE_INFO2("BIG Terminate Procedure, bigHandle=%u, instant=%u", pBigCtx->handle, inst);
  }

  lctrSlvBigSendAcadMsg(pBigCtx, LCTR_ACAD_MSG_BIG_TERMINATED);
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active BIS operation.
 *
 *  \param      pBigCtx     BIG context.
 */
/*************************************************************************************************/
void lctrSlvBigActShutdown(lctrBigCtx_t *pBigCtx)
{
  /* By removing BOD from scheduler, BOD end callback will be called. */
  /* Shutdown completes with events generated in BOD end callback.    */
  SchRemove(&pBigCtx->bod);

  lctrSlvBigSendAcadMsg(pBigCtx, LCTR_ACAD_MSG_BIG_TERMINATED);
}

/*************************************************************************************************/
/*!
 *  \brief      Cleanup BIG contexts.
 *
 *  \param      pBigCtx     BIG context.
 */
/*************************************************************************************************/
void lctrSlvBigActCleanup(lctrBigCtx_t *pBigCtx)
{
  lctrNotifyHostTerminateBigComplete(pBigCtx);

  SchRmRemove(LCTR_BIG_TO_RM_HANDLE(pBigCtx));

  lctrFreeBigCtx(pBigCtx);

  BbStop(BB_PROT_BLE);

  LmgrDecResetRefCount();
}
