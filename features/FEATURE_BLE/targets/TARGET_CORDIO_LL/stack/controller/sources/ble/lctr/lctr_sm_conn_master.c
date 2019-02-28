/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master connection state machine implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn_slave.h"
#include "lctr_int_adv_master.h"
#include "lmgr_api_conn.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Execute master connection state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstConnExecuteSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (pCtx->state)
  {
    case LCTR_CONN_STATE_ESTABLISHED_READY:
      switch (event)
      {
        case LCTR_CONN_DATA_PENDING:
          lctrMstReloadDataPdu(pCtx);
          break;
        case LCTR_CONN_MST_ESTABLISH:
        case LCTR_CONN_TERM_SUP_TIMEOUT:
        case LCTR_CONN_TERM_MIC_FAILED:
          /* Data path related events. */
          break;
        default:
          /* Invoke LLCP child state machine */
          lctrMstLlcpExecuteSm(pCtx, event);
          break;
      }
      break;

    case LCTR_CONN_STATE_INITIALIZED:
      switch (event)
      {
        case LCTR_CONN_MST_ESTABLISH:
          LL_TRACE_INFO2("lctrMstConnExecuteSm: handle=%u, state=%u, event=MST_ESTABLISH", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);

          lctrMstSetEstablishConn(pCtx);
          lctrStoreConnFailEstablishTerminateReason(pCtx);        /* default termination reason */

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_VER_LLCP_STARTUP))
          {
            /* Queue version exchange. */
            lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_VERSION_EXCH);
          }

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_FEAT_LLCP_STARTUP))
          {
            /* Queue feature exchange. */
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

          lctrNotifyHostConnectInd(LCTR_GET_CONN_HANDLE(pCtx), LL_ROLE_MASTER,
              &pLctrConnMsg->connEstablish.connInd, pLctrConnMsg->connEstablish.peerIdAddrType,
              pLctrConnMsg->connEstablish.peerIdAddr, pLctrConnMsg->connEstablish.peerRpa,
              pLctrConnMsg->connEstablish.localRpa, LL_SUCCESS, pCtx->usedChSel);

          pCtx->state = LCTR_CONN_STATE_ESTABLISHED_READY;
          break;

        default:
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
  switch (event)
  {
    case LCTR_CONN_TERMINATED:
      LL_TRACE_INFO2("lctrMstConnExecuteSm: handle=%u, state=%u, event=TERMINATED", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      SchRmRemove(LCTR_GET_CONN_HANDLE(pCtx));
      lctrNotifyHostDisconnectInd(pCtx);
      lctrFreeConnCtx(pCtx);
      break;
    default:
      lctrConnStatelessEventHandler(pCtx, event);
      break;
  }
}
