/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS LLCP slave state machine implementation file.
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

#include "lctr_int_cis_slave.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn_slave.h"
#include "lmgr_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/*! \brief      Slave CIS establishment events. */
enum
{
  LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ,                /*!< Received peer LL_CIS_REQ. */
  LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ,            /*!< Received internal peer LL_CIS_REQ. */
  LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT,             /*!< Received host LE CIS Request Accept command. */
  LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT,             /*!< Received host LE CIS Request Reject command. */
  LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND,                /*!< Received peer LL_CIS_IND. */
  LCTR_CIS_SLV_EST_EVENT_PEER_REJECT,                 /*!< Received peer LL_REJECT_IND_EXT or LL_UNKNOWN_RSP. */
  LCTR_CIS_SLV_EST_EVENT_TOTAL,                       /*!< Total CIS events. */
  LCTR_CIS_SLV_EST_EVENT_INVALID = 0xFF               /*!< Invalid event. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
static void lctrSlvCisLlcpActRejectCollision(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      CIS establishment state machine action table. */
static const lctrActFn_t lctrSlvCisEstActionTbl[LCTR_CIS_SLV_EST_STATE_TOTAL][LCTR_CIS_SLV_EST_EVENT_TOTAL] =
{
  { /* LCTR_CIS_SLV_EST_STATE_IDLE */
    lctrSlvCisLlcpActPeerCisReq,                  /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
    lctrSlvCisLlcpActIntPeerCisReq,               /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
    NULL                                          /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  },
  { /* LCTR_CIS_SLV_EST_STATE_HOST_REPLY */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
    lctrSlvCisLlcpActRejectCollision,             /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
    lctrSlvCisLlcpActAcpCisReq,                   /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
    lctrSlvCisLlcpActRejCisReq,                   /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
    NULL                                          /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  },
  { /* LCTR_CIS_SLV_EST_STATE_CIS_IND */
    lctrSlvCisLlcpActRejectCollision,             /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
    NULL,                                         /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
    lctrSlvCisLlcpActPeerCisInd,                  /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
    lctrSlvCisLlcpActPeerCisRej                   /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  }
};

/*! \brief      CIS establishment state machine next state table. */
static const uint8_t lctrSlvCisEstNextStateTbl[LCTR_CIS_SLV_EST_STATE_TOTAL][LCTR_CIS_SLV_EST_EVENT_TOTAL] =
{
  { /* LCTR_CIS_SLV_EST_STATE_IDLE */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY,          /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY,          /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_IDLE,                /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
      LCTR_CIS_SLV_EST_STATE_IDLE,                /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
      LCTR_CIS_SLV_EST_STATE_IDLE,                /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
      LCTR_CIS_SLV_EST_STATE_IDLE                 /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  },
  { /* LCTR_CIS_SLV_EST_STATE_HOST_REPLY */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY,          /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY,          /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_CIS_IND,             /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
      LCTR_CIS_SLV_EST_STATE_IDLE,                /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY,          /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
      LCTR_CIS_SLV_EST_STATE_HOST_REPLY           /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  },
  { /* LCTR_CIS_SLV_EST_STATE_CIS_IND */
      LCTR_CIS_SLV_EST_STATE_CIS_IND,             /* LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_CIS_IND,             /* LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ */
      LCTR_CIS_SLV_EST_STATE_CIS_IND,             /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT */
      LCTR_CIS_SLV_EST_STATE_CIS_IND,             /* LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT */
      LCTR_CIS_SLV_EST_STATE_IDLE,                /* LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND */
      LCTR_CIS_SLV_EST_STATE_IDLE                 /* LCTR_CIS_SLV_EST_EVENT_PEER_REJECT */
  }
};

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief      Send peer LL_REJECT_IND_EXT due to pending operation.
 *
 *  \param      pCtx    Connection context.
 *  \param      pCisCtx CIS context.
 */
/*************************************************************************************************/
static void lctrSlvCisLlcpActRejectCollision(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx)
{
  LL_TRACE_WARN1("Peer requested PHY while procedure pending, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
  lctrSendRejectInd(pCtx, LL_ERROR_CODE_LMP_ERR_TRANSACTION_COLLISION, TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute action function.
 *
 *  \param      pCtx      Connection context.
 *  \param      pCisCtx   CIS context.
 *  \param      event     Event ID.
 */
/*************************************************************************************************/
static inline void lctrExecAction(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx, uint8_t event)
{
  if (lctrSlvCisEstActionTbl[pCisCtx->estState][event])
  {
    lctrSlvCisEstActionTbl[pCisCtx->estState][event](pCtx, pCisCtx);
  }

  pCisCtx->estState = lctrSlvCisEstNextStateTbl[pCisCtx->estState][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Get remapped event. This function also update the CIS handle for the LLCP.
 *
 *  \param      event   Subsystem event.
 *  \param      pCtx    Connection context.
 *
 *  \return     Procedure event ID.
 */
/*************************************************************************************************/
static uint8_t lctrRemapEvent(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    /*** Peer messages ***/

    case LCTR_CONN_MSG_RX_LLCP:
      switch (lctrDataPdu.opcode)
      {
        case LL_PDU_CIS_REQ:
        {
          lctrCigCtx_t *pCigCtx;
          if ((pCigCtx = lctrFindCigById(lctrDataPdu.pld.cisReq.cigId)) == NULL)
          {
            if ((pCigCtx = lctrAllocCigCtx(lctrDataPdu.pld.cisReq.cigId)) == NULL)
            {
              lctrSendRejectInd(pCtx, LL_ERROR_CODE_LIMIT_REACHED, TRUE);
              LL_TRACE_WARN0("Not able to allocate a CIG context");
              break;
            }
          }

          lctrCisCtx_t *pCisCtx ;
          if ((pCisCtx = lctrAllocCisCtx(pCigCtx)) == NULL)
          {
            lctrSendRejectInd(pCtx, LL_ERROR_CODE_LIMIT_REACHED, TRUE);
            LL_TRACE_WARN0("Not able to allocate a CIS context");
            break;
          }
          pCisCtx->aclHandle = LCTR_GET_CONN_HANDLE(pCtx);    /* Save the ACL handle. */
          pCisCtx->role = LL_ROLE_SLAVE;
          pCtx->llcpCisHandle = pCisCtx->cisHandle;       /* Update LLCP CIS handle for connection context from peer. */
          pCtx->checkCisTerm = lctrCheckForCisLinkTerm;
          lctrCheckCisEstCisFn = lctrCheckIsCisEstCis;
          return LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ;
        }

        case LL_PDU_CIS_IND:
          return LCTR_CIS_SLV_EST_EVENT_PERR_CIS_IND;

        case LL_PDU_REJECT_EXT_IND:
        {
          if (lctrDataPdu.pld.rejInd.opcode == LL_PDU_CIS_RSP)
          {
            return LCTR_CIS_SLV_EST_EVENT_PEER_REJECT;
          }
          break;
        }

        default:
          break;
      }
      break;

    /*** Host messages ***/
    case LCTR_CONN_MSG_API_CIS_REQ_REJECT:
      return LCTR_CIS_SLV_EST_EVENT_HOST_REQ_REJECT;

    case LCTR_CONN_MSG_API_CIS_REQ_ACCEPT:
      return LCTR_CIS_SLV_EST_EVENT_HOST_REQ_ACCEPT;

    /*** Internal messages ***/
    case LCTR_CONN_LLCP_START_PENDING:
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_CIS_EST_PEER))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_CIS_EST_PEER);
        return LCTR_CIS_SLV_EST_EVENT_INT_PEER_CIS_REQ;
      }
      break;
    default:
      break;
  }
  return LCTR_CIS_SLV_EST_EVENT_INVALID;
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
    case LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ:
      if ((pCtx->llcpActiveProc == LCTR_PROC_CMN_CH_MAP_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_PARAM))
      {
        lctrSendRejectInd(pCtx, LL_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION, TRUE);
      }
      else
      {
        lctrCisStorePeerCisReq(pCtx, pCisCtx);
        pCtx->llcpPendMask |= 1 << LCTR_PROC_CIS_EST_PEER;
        LL_TRACE_INFO2("Pending CIS_REQ=%u procedure: activeProc=%u", LCTR_PROC_CIS_EST_PEER, pCtx->llcpActiveProc);
      }
      break;

    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Check if active procedure can be overridden with phy update procedure.
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
    case LCTR_CIS_SLV_EST_EVENT_PEER_CIS_REQ:
      /* Only the procedure without instant fields can be overridden. */
      switch (pCtx->llcpActiveProc)
      {
        case LCTR_PROC_CMN_VER_EXCH:
        case LCTR_PROC_CMN_FEAT_EXCH:
        case LCTR_PROC_CMN_DATA_LEN_UPD:
        case LCTR_PROC_CMN_REQ_PEER_SCA:
          pCtx->llcpPendMask |= 1 << pCtx->llcpActiveProc;
          pCtx->llcpActiveProc = LCTR_PROC_CIS_EST;
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
 *  \brief      Execute slave CIS establishment LLCP state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     TRUE if the event is handled by this state machine, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrSlvLlcpExecuteCisEstSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  if ((event = lctrRemapEvent(pCtx, event)) == LCTR_CIS_SLV_EST_EVENT_INVALID)
  {
    return FALSE;
  }

  lctrCisCtx_t *pCisCtx = lctrFindCisByHandle(pCtx->llcpCisHandle);
  WSF_ASSERT(pCisCtx != NULL)

  switch (pCtx->llcpState)
  {
    case LCTR_LLCP_STATE_IDLE:
      LL_TRACE_INFO3("lctrSlvLlcpExecuteCisEstSm: handle=%u, llcpState=IDLE, estState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCisCtx->estState, event);

      lctrExecAction(pCtx, pCisCtx, event);

      if (pCisCtx->estState != LCTR_CIS_SLV_EST_STATE_IDLE)
      {
        pCtx->llcpState = LCTR_LLCP_STATE_BUSY;
        pCtx->llcpActiveProc = LCTR_PROC_CIS_EST;
      }
      break;

    case LCTR_LLCP_STATE_BUSY:
      LL_TRACE_INFO3("lctrSlvLlcpExecuteCisEstSm: handle=%u, llcpState=BUSY, estState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCisCtx->estState, event);

      lctrCheckProcOverride(pCtx, event);

      if (pCtx->llcpActiveProc == LCTR_PROC_CIS_EST)
      {
        lctrExecAction(pCtx, pCisCtx, event);

        if (pCisCtx->estState == LCTR_CIS_SLV_EST_STATE_IDLE)
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

