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
 * \brief Link layer controller connection state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_conn.h"
#include "lctr_int_conn_slave.h"
#include "lctr_int_adv_slave.h"
#include "lmgr_api_conn.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Execute slave connection state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvConnExecuteSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  /* State-specific events. */
  switch (pCtx->state)
  {
    case LCTR_CONN_STATE_ESTABLISHED_READY:
      switch (event)
      {
        case LCTR_CONN_DATA_PENDING:
        case LCTR_CONN_TERM_SUP_TIMEOUT:
        case LCTR_CONN_TERM_MIC_FAILED:
          /* Data path related events. */
          break;
        default:
          /* Invoke LLCP child state machine */
          lctrSlvLlcpExecuteSm(pCtx, event);
          break;
      }
      break;

    case LCTR_CONN_STATE_INITIALIZED:
      switch (event)
      {
        case LCTR_CONN_MSG_RX_CONNECT_IND:
          LL_TRACE_INFO1("lctrSlvConnExecuteSm: handle=%u, state=INITIALIZED, event=RX_CONNECT_IND", LCTR_GET_CONN_HANDLE(pCtx));

          lctrSlvConnBuildOp(pCtx);
          lctrStoreConnFailEstablishTerminateReason(pCtx);    /* default termination reason */

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_SLV_DELAY_LLCP_STARTUP))
          {
            /* The following LLCP startup procedures continue when receiving the first non-control PDU from the master. */
            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_VER_LLCP_STARTUP))
            {
              pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_VER_EXCH;
            }

            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_FEAT_LLCP_STARTUP))
            {
              pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_FEAT_EXCH;
            }

            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_LEN_LLCP_STARTUP))
            {
              /* Only send initial length feature if the default is an extended length. */
              if ((pCtx->localDataPdu.maxTxLen  > LL_MAX_DATA_LEN_MIN)  ||
                  (pCtx->localDataPdu.maxTxTime > LL_MAX_DATA_TIME_MIN))
              {
                pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_DATA_LEN_UPD;
              }
            }
            pCtx->state = LCTR_CONN_STATE_ESTABLISHED_STARTUP;
          }
          else
          {
            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_VER_LLCP_STARTUP))
            {
              lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_VERSION_EXCH);
            }

            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_FEAT_LLCP_STARTUP))
            {
              lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_FEATURE_EXCH);
            }

            if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_LEN_LLCP_STARTUP))
            {
              /* Only send initial length feature if the default is an extended length. */
              if ((pCtx->localDataPdu.maxTxLen  > LL_MAX_DATA_LEN_MIN)  ||
                  (pCtx->localDataPdu.maxTxTime > LL_MAX_DATA_TIME_MIN))
              {
                lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_LENGTH_EXCH);
              }
            }
            pCtx->state = LCTR_CONN_STATE_ESTABLISHED_READY;
          }

          lctrNotifyHostConnectInd(LCTR_GET_CONN_HANDLE(pCtx), LL_ROLE_SLAVE,
              &pLctrConnMsg->connEstablish.connInd, pLctrConnMsg->connEstablish.peerIdAddrType,
              pLctrConnMsg->connEstablish.peerIdAddr, pLctrConnMsg->connEstablish.peerRpa,
              pLctrConnMsg->connEstablish.localRpa, LL_SUCCESS, pCtx->usedChSel);
          break;

        default:
          break;
      }
      break;

    case LCTR_CONN_STATE_ESTABLISHED_STARTUP:
      LL_TRACE_INFO1("lctrSlvConnExecuteSm: state=ESTABLISHED_STARTUP, event=%u", event);
      switch(event)
      {
        case LCTR_CONN_SLV_INIT_STARTUP_LLCP:
          pCtx->state = LCTR_CONN_STATE_ESTABLISHED_READY;
          lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_START_PENDING);
          break;

        /* Data path related events. */
        case LCTR_CONN_DATA_PENDING:
        case LCTR_CONN_TERM_SUP_TIMEOUT:
        case LCTR_CONN_TERM_MIC_FAILED:

          break;

        /* The following events are master only.
         * case LCTR_CONN_MSG_API_CHAN_MAP_UPDATE:
         * case LCTR_CONN_MSG_API_START_ENC:
         */

        /* The following events will be pended. */
        case LCTR_CONN_MSG_API_CONN_UPDATE:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CONN_UPD;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CONN_UPD;
          break;
        case LCTR_CONN_MSG_API_REMOTE_FEATURE:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_FEAT_EXCH;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_FEAT_EXCH;
          break;
        case LCTR_CONN_MSG_API_REMOTE_VERSION:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_VER_EXCH;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_VER_EXCH;
          break;
        case LCTR_CONN_MSG_API_DATA_LEN_CHANGE:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_DATA_LEN_UPD;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_DATA_LEN_UPD;
          break;
        case LCTR_CONN_MSG_API_REQ_PEER_SCA:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_REQ_PEER_SCA;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_REQ_PEER_SCA;
          break;
        case LCTR_CONN_MSG_API_PHY_UPDATE:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_PHY_UPD;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_PHY_UPD;
          break;
        case LCTR_CONN_MSG_API_SET_MIN_USED_CHAN:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_SET_MIN_USED_CHAN;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_SET_MIN_USED_CHAN;
          break;
        case LCTR_CONN_MSG_API_PER_ADV_SYNC_TRSF:
          pCtx->llcpPendMask |= 1 << LCTR_PROC_CMN_PER_ADV_SYNC_TRSF;
          pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CMN_PER_ADV_SYNC_TRSF;
          break;

        default:
          /* Invoke LLCP child state machine */
          lctrSlvLlcpExecuteSm(pCtx, event);
          break;
      }
      break;

    case LCTR_CONN_STATE_TERMINATING:
      switch (event)
      {
        case LCTR_CONN_TMR_LLCP_RSP_EXP:
          pCtx->llcpNotifyMask &= ~(1 << LCTR_PROC_CMN_TERM);
          lctrStoreLlcpTimeoutTerminateReason(pCtx);
          pCtx->llcpState = LCTR_LLCP_STATE_IDLE;  /* Stop waiting to send TERMINATE_IND. */
          break;
        default:
          break;
      }
      break;

    default:
      break;
  }

  /* State-less events. */
  lctrConnStatelessEventHandler(pCtx, event);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute stateless connection event handler.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrConnStatelessEventHandler(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    case LCTR_CONN_TERMINATED:
      LL_TRACE_INFO2("lctrConnStatelessEventHandler: handle=%u, state=%u, event=TERMINATED", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      lctrNotifyHostDisconnectInd(pCtx);
      lctrFreeConnCtx(pCtx);
      break;
    case LCTR_CONN_TERM_SUP_TIMEOUT:
      LL_TRACE_WARN2("lctrConnStatelessEventHandler: handle=%u, state=%u, event=SUP_TIMEOUT", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      /* lctrStoreConnFailEstablishTerminateReason or lctrStoreConnTimeoutTerminateReason */  /* already set as termination reason */
      lctrFlagLinkTerm(pCtx);
      break;
    case LCTR_CONN_TERM_MIC_FAILED:
      LL_TRACE_WARN2("lctrConnStatelessEventHandler: handle=%u, state=%u, event=MIC_FAILED", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      lctrStoreMicFailedTerminateReason(pCtx);
      lctrFlagLinkTerm(pCtx);
      break;
    case LCTR_CONN_MSG_RESET:
      LL_TRACE_INFO2("lctrConnStatelessEventHandler: handle=%u, state=%u, event=RESET", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      lctrStoreResetTerminateReason(pCtx);
      lctrFlagLinkTerm(pCtx);
      break;
    default:
      break;
  }
}
