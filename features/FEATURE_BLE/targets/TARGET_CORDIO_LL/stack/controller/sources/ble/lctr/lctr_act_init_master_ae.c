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
 *  \brief Link layer controller master connection state machine action routines.
 */
/*************************************************************************************************/
#include "lctr_int_init_master_ae.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active initiation operation.
 *
 *  \param      pExtInitCtx   Extended scan context of the initiator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtInitActShutdown(lctrExtScanCtx_t *pExtInitCtx)
{
  pExtInitCtx->shutdown = TRUE;

  if (!pExtInitCtx->auxOpPending)
  {
    SchRemove(&pExtInitCtx->scanBod);
  }
  else
  {
    SchRemove(&pExtInitCtx->auxScanBod);
  }
  /* Shutdown completes with events generated in BOD end callback. */
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host initiate disable.
 *
 * \param       pExtInitCtx     Extended scan context of the initiator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtInitActScanTerm(lctrExtScanCtx_t *pExtInitCtx)
{
  lctrMstExtInitCleanupOp(pExtInitCtx);

  if (pExtInitCtx->state != LCTR_EXT_INIT_STATE_RESET)
  {
    LlCreateConnCancelCnf_t evt =
    {
        .hdr =
        {
            .event  = LL_CREATE_CONN_CANCEL_CNF,
            .status = LL_SUCCESS
        },

        .status = LL_SUCCESS,
    };

    LL_TRACE_INFO0("### LlEvent ###  LL_EXT_CREATE_CONN_CANCEL_CNF, status=LL_SUCCESS");

    LmgrSendEvent((LlEvt_t *)&evt);
    /* Send connection complete event after response to create connection cancel command. */
    lctrScanNotifyHostInitiateError(LL_ERROR_CODE_UNKNOWN_CONN_ID,
        lctrMstExtInit.peerAddrType,
        lctrMstExtInit.peerAddr);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send disallow initiate host notification.
 *
 *  \param      pExtInitCtx     Extended scan context of the initiator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtInitActDisallowInitiate(lctrExtScanCtx_t *pExtInitCtx)
{
  lctrInitiateMsg_t *pInitMsg = (lctrInitiateMsg_t *)pLctrMsg;

  lctrScanNotifyHostInitiateError(LL_ERROR_CODE_CMD_DISALLOWED, pInitMsg->peerAddrType, pInitMsg->peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief      Send disallow create connection cancel host notification.
 *
 *  \param      pExtInitCtx     Extended scan context of the initiator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtInitActDisallowCancel(lctrExtScanCtx_t *pExtInitCtx)
{
  LlCreateConnCancelCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_CREATE_CONN_CANCEL_CNF,
      .status = LL_ERROR_CODE_CMD_DISALLOWED
    },

    .status = LL_ERROR_CODE_CMD_DISALLOWED,
  };

  LL_TRACE_INFO0("### LlEvent ###  LL_EXT_CREATE_CONN_CANCEL_CNF, status=LL_ERROR_CODE_CMD_DISALLOWED");

  LmgrSendEvent((LlEvt_t *)&evt);
}
