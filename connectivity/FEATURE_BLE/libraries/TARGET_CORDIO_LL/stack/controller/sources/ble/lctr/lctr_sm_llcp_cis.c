/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS LLCP state machine implementation file shared by master
 *  and slave.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#include "lctr_int_cis_master.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn.h"
#include "lmgr_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      CIS termination state machine action table. */
static const lctrActFn_t lctrCisTermActionTbl[LCTR_CIS_TERM_STATE_TOTAL][LCTR_CIS_TERM_EVENT_TOTAL] =
{
  { /* LCTR_CIS_TERM_STATE_IDLE */
      lctrCisLlcpActHostDisc,                   /* LCTR_CIS_TERM_EVENT_HOST_DISC */
      lctrCisLlcpActPeerDisc,                   /* LCTR_CIS_TERM_EVENT_PEER_DISC */
      lctrCisLlcpActIntHostDisc,                /* LCTR_CIS_TERM_EVENT_INT_START_DISC */
      lctrCisLlcpActIntPeerDisc,                /* LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC */
      NULL                                      /* LCTR_CIS_TERM_EVENT_CIS_TERM */
  },
  { /* LCTR_CIS_TERM_STATE_TERMINATING */
      NULL,                                     /* LCTR_CIS_TERM_EVENT_HOST_DISC */
      NULL,                                     /* LCTR_CIS_TERM_EVENT_PEER_DISC */
      NULL,                                     /* LCTR_CIS_TERM_EVENT_INT_START_DISC */
      NULL,                                     /* LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC */
      lctrCisLlcpActCisTerm                     /* LCTR_CIS_TERM_EVENT_CIS_TERM */
  }
};

/*! \brief      CIS termination state machine next state table. */
static const uint8_t lctrCisTermNextStateTbl[LCTR_CIS_TERM_STATE_TOTAL][LCTR_CIS_TERM_EVENT_TOTAL] =
{
  { /* LCTR_CIS_TERM_STATE_IDLE */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_HOST_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_PEER_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_INT_START_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC */
      LCTR_CIS_TERM_STATE_IDLE                  /* LCTR_CIS_TERM_EVENT_CIS_TERM */
  },
  { /* LCTR_CIS_TERM_STATE_TERMINATING */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_HOST_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_PEER_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_INT_START_DISC */
      LCTR_CIS_TERM_STATE_TERMINATING,          /* LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC */
      LCTR_CIS_TERM_STATE_IDLE                  /* LCTR_CIS_TERM_EVENT_CIS_TERM */
  }
};


/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      CIS executes termination action function.
 *
 *  \param      pCtx      Connection context.
 *  \param      pCisCtx   CIS context.
 *  \param      event     Event ID.
 */
/*************************************************************************************************/
static inline void lctrExecTermAction(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx, uint8_t event)
{
  if (lctrCisTermActionTbl[pCisCtx->termState][event])
  {
    lctrCisTermActionTbl[pCisCtx->termState][event](pCtx, pCisCtx);
  }

  pCisCtx->termState = lctrCisTermNextStateTbl[pCisCtx->termState][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Get remapped event. This function also updates the CIS handle for the LLCP.
 *
 *  \param      event   Subsystem event.
 *  \param      pCtx    Connection context.
 *
 *  \return     Procedure event ID.
 */
/*************************************************************************************************/
static uint8_t lctrRemapTermEvent(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    /*** Peer messages ***/

    case LCTR_CONN_MSG_RX_LLCP:
      switch (lctrDataPdu.opcode)
      {
        case LL_PDU_CIS_TERM_IND:
        {
          lctrCisCtx_t *pCisCtx = lctrFindCisById(lctrDataPdu.pld.cisTerm.cigId, lctrDataPdu.pld.cisTerm.cisId);
          pCtx->llcpCisHandle = pCisCtx->cisHandle;   /* Update LLCP CIS handle for connection context from peer. */
          return LCTR_CIS_TERM_EVENT_PEER_DISC;
        }
        break;

        default:
          break;
      }
      break;

    /*** Host messages ***/

    case LCTR_CONN_MSG_API_DISCONNECT:
    {
      lctrCisCtx_t *pCisCtx;

      if ((pCisCtx = lctrFindCisByHandle(pLctrConnMsg->cisDisc.cisHandle)) != NULL)
      {
        pCtx->llcpCisHandle = pLctrConnMsg->cisDisc.cisHandle;  /* Update LLCP CIS handle for connection context from host. */
        pLctrConnMsg->cisDisc.cisHandle = 0;
        return LCTR_CIS_TERM_EVENT_HOST_DISC;
      }
      break;
    }
    /*** Internal messages ***/

    case LCTR_CIS_TERM_EVENT_CIS_TERM:
      return LCTR_CIS_TERM_EVENT_CIS_TERM;

    case LCTR_CONN_LLCP_START_PENDING:
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_CIS_TERM))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_CIS_TERM);
        return LCTR_CIS_TERM_EVENT_INT_START_DISC;
      }
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_CIS_TERM_PEER))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_CIS_TERM_PEER);
        return LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC;
      }
      break;

    default:
      break;
  }
  return LCTR_CIS_TERM_EVENT_INVALID;
}

/*************************************************************************************************/
/*!
 *  \brief      Resolve procedure collision.
 *
 *  \param      event   Subsystem event.
 *  \param      pCisCtx CIS context.
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
static void lctrResolveCollision(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx, uint8_t event)
{
  switch (event)
  {
    case LCTR_CIS_TERM_EVENT_PEER_DISC:
      if ((pCtx->llcpActiveProc == LCTR_PROC_CMN_CH_MAP_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_PARAM))
      {
        lctrSendRejectInd(pCtx, LL_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION, TRUE);
      }
      else
      {
        lctrCisStoreTerminateReason(pCisCtx);
        pCtx->llcpPendMask |= 1 << LCTR_PROC_CIS_TERM_PEER;
        LL_TRACE_INFO2("Pending CIS_TERM_IND=%u procedure: activeProc=%u", LCTR_PROC_CIS_TERM_PEER, pCtx->llcpActiveProc);
      }
      break;

    case LCTR_CIS_TERM_EVENT_HOST_DISC:
      pCtx->llcpNotifyMask |= 1 << LCTR_PROC_CIS_TERM;
      pCtx->llcpPendMask |= 1 << LCTR_PROC_CIS_TERM;
      LL_TRACE_INFO2("Pending CIS_TERM_IND=%u procedure: activeProc=%u", LCTR_PROC_CIS_TERM, pCtx->llcpActiveProc);
      break;

    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Check if active procedure can be overridden with CIS termination procedure.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   Subsystem event.
 *
 *  This routine will pend the active procedure and allow the phy update procedure to
 *  override it.
 */
/*************************************************************************************************/
static void lctrCheckProcOverride(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    case LCTR_CIS_TERM_EVENT_PEER_DISC:
      /* Only the procedure without instant fields can be overridden. */
      switch (pCtx->llcpActiveProc)
      {
        case LCTR_PROC_CMN_VER_EXCH:
        case LCTR_PROC_CMN_FEAT_EXCH:
        case LCTR_PROC_CMN_DATA_LEN_UPD:
        case LCTR_PROC_CMN_REQ_PEER_SCA:
          pCtx->llcpPendMask |= 1 << pCtx->llcpActiveProc;
          pCtx->llcpActiveProc = LCTR_PROC_CIS_TERM;
          pCtx->llcpIsOverridden = TRUE;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Execute CIS termination state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     TRUE if the event is handled by this state machine, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrLlcpExecuteCisTermSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  if ((event = lctrRemapTermEvent(pCtx, event)) == LCTR_CIS_TERM_EVENT_INVALID)
  {
    return FALSE;
  }

  lctrCisCtx_t *pCisCtx = lctrFindCisByHandle(pCtx->llcpCisHandle);

  WSF_ASSERT(pCisCtx != NULL)

  switch (pCtx->llcpState)
  {
    case LCTR_LLCP_STATE_IDLE:
      LL_TRACE_INFO3("lctrLlcpExecuteCisTermSm: cis_handle=%u, llcpState=IDLE, termState=%u, event=%u", pCisCtx->cisHandle, pCisCtx->termState, event);

      lctrExecTermAction(pCtx, pCisCtx, event);

      if (pCisCtx->termState != LCTR_CIS_TERM_STATE_IDLE)
      {
        pCtx->llcpState = LCTR_LLCP_STATE_BUSY;
        pCtx->llcpActiveProc = LCTR_PROC_CIS_TERM;
      }
      break;

    case LCTR_LLCP_STATE_BUSY:
      LL_TRACE_INFO3("lctrLlcpExecuteCisTermSm: cis_handle=%u, llcpState=BUSY, termState=%u, event=%u", pCisCtx->cisHandle, pCisCtx->termState, event);

      lctrCheckProcOverride(pCtx, event);

      if (pCtx->llcpActiveProc == LCTR_PROC_CIS_TERM)
      {
        lctrExecTermAction(pCtx, pCisCtx, event);

        if (pCisCtx->termState == LCTR_CIS_TERM_STATE_IDLE)
        {
          lctrCisStopLlcpTimer(pCtx, pCisCtx);
          pCtx->llcpState = LCTR_LLCP_STATE_IDLE;
          pCtx->llcpActiveProc = LCTR_PROC_INVALID;

          lctrStartPendingLlcp(pCtx);
        }
      }
      else
      {
        lctrResolveCollision(pCtx, pCisCtx, event);
      }
      break;

    default:
      break;
  }

  return TRUE;
}

