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
 * \brief Link layer controller master PHY update state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_master_phy.h"
#include "lctr_int_slave_phy.h"
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
typedef void (*lctrActFn_t)(lctrConnCtx_t *pCtx);

/*! \brief      PHY update states. */
enum
{
  LCTR_PU_STATE_IDLE,                         /*!< Idle state. */
  LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /*!< ARQ flush before sending LL_PHY_REQ. */
  LCTR_PU_STATE_PHY_RSP,                      /*!< Wait for LL_PHY_RSP state. */
  LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /*!< ARQ flush before sending LL_PHY_UPDATE_IND. */
  LCTR_PU_STATE_PHY_UPD_INSTANT,              /*!< Wait for PHY update instant state. */
  LCTR_PU_STATE_TOTAL                         /*!< Total PHY update states. */
};

/*! \brief      PHY update events. */
enum
{
  LCTR_PU_EVENT_HOST_PHY_UPD,                 /*!< Received host PHY update command. */
  LCTR_PU_EVENT_PEER_PHY_RSP,                 /*!< Received peer LL_PHY_RSP. */
  LCTR_PU_EVENT_PEER_PHY_REQ,                 /*!< Received peer LL_PHY_REQ. */
  LCTR_PU_EVENT_PEER_REJECT,                  /*!< Received peer LL_REJECT_IND or LL_UNKNOWN_RSP. */
  LCTR_PU_EVENT_INT_PROC_COMP,                /*!< Procedure completion event. */
  LCTR_PU_EVENT_INT_START_PHY_UPD,            /*!< Start pending host PHY update procedure. */
  LCTR_PU_EVENT_INT_START_PHY_UPD_PEER,       /*!< Start pending peer PHY update procedure. */
  LCTR_PU_EVENT_ARQ_FLUSHED,                  /*!< Internal ARQ queue flushed. */
  LCTR_PU_EVENT_TOTAL,                        /*!< Total PHY update states. */
  LCTR_PU_EVENT_INVALID = 0xFF                /*!< Invalid event. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

extern void lctrActPeerRejectPhyReq(lctrConnCtx_t *pCtx);
extern void lctrActNotifyHostPhyUpdateSuccess(lctrConnCtx_t *pCtx);
extern uint8_t lctrChoosePreferredPhy(uint8_t val);
extern uint8_t lctrSuppPhys(void);
extern void lctrActStartPhyUpdate(lctrConnCtx_t *pCtx);
extern void lctrActFlushArq(lctrConnCtx_t *pCtx);

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Determine whether preference is symmetric.
 *
 *  \param      peerTxPhys  Peer's preferred transmitter PHYs.
 *  \param      peerRxPhys  Peer'sreferred receiver PHYs.
 *
 *  \return     TRUE if preference is symmetric.
 */
/*************************************************************************************************/
static bool_t lctrPreferenceIsSymmetric(uint8_t peerTxPhys, uint8_t peerRxPhys)
{
  uint8_t phys = lctrChoosePreferredPhy(peerTxPhys | peerRxPhys);

  /* Only one bit is set in each preference and same bit is set in each preference. */
  return ((phys == peerTxPhys) && (phys == peerRxPhys) && (phys != LL_PHYS_NONE));
}

/*************************************************************************************************/
/*!
 *  \brief      Send PHY update.
 *
 *  \param      pCtx        Connection context.
 *  \param      txPhys      Preferred transmitter PHYs.
 *  \param      rxPhys      Preferred receiver PHYs.
 *  \param      peerTxPhys  Peer's preferred transmitter PHYs.
 *  \param      peerRxPhys  Peer'sreferred receiver PHYs.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrSendPhyUpdate(lctrConnCtx_t *pCtx, uint8_t txPhys, uint8_t rxPhys, uint8_t peerTxPhys, uint8_t peerRxPhys)
{
  /* Combine master's request with slave's response. */
  txPhys &= peerRxPhys;
  rxPhys &= peerTxPhys;

  /* If outcome must be symmetric, make Tx and Rx identical. */
  if (lctrPreferenceIsSymmetric(peerTxPhys, peerRxPhys) ||
      (BB_SYM_PHY_REQ || lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_REQ_SYM_PHY)))
  {
    txPhys &= rxPhys;
    rxPhys  = txPhys;
  }

  /* If more than one bit set, choose preferred PHY. */
  txPhys = lctrChoosePreferredPhy(txPhys);
  rxPhys = lctrChoosePreferredPhy(rxPhys);

  /* If no change, indicate no change. */
  if (txPhys == lctrPhyToPhysBit(pCtx->bleData.chan.txPhy))
  {
    txPhys = LL_PHYS_NONE;
  }
  if (rxPhys == lctrPhyToPhysBit(pCtx->bleData.chan.rxPhy))
  {
    rxPhys = LL_PHYS_NONE;
  }

  /* Send update request. */
  lctrSendPhyUpdateIndPdu(pCtx, txPhys, rxPhys);

  /* If neither PHY will change, this ends the procedure. */
  if ((txPhys == LL_PHYS_NONE) && (rxPhys == LL_PHYS_NONE))
  {
    lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_PROC_CMPL);
  }

  /* Set restriction for packet times and allow data again. */
  if (txPhys == LL_PHYS_NONE)
  {
    lctrRemovePacketTimeRestriction(pCtx);
  }
  else
  {
    lctrSetPacketTimeRestriction(pCtx, txPhys);
  }
  lctrUnpauseTxData(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for start PHY update.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActStartPhyUpdatePeer(lctrConnCtx_t *pCtx)
{
  uint8_t txPhys = pCtx->txPhys;
  uint8_t rxPhys = pCtx->rxPhys;

  /* If no preference, use supported PHYs. */
  if ((pCtx->allPhys & LL_ALL_PHY_TX_PREFERENCE_BIT) != 0)
  {
    txPhys = lctrSuppPhys();
  }
  if ((pCtx->allPhys & LL_ALL_PHY_RX_PREFERENCE_BIT) != 0)
  {
    rxPhys = lctrSuppPhys();
  }

  /* Send PHY update. */
  lctrSendPhyUpdate(pCtx, txPhys, rxPhys, pCtx->phyReq.txPhys, pCtx->phyReq.rxPhys);
  lctrStopLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received host PHY update command.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActHostPhyUpdate(lctrConnCtx_t *pCtx)
{
  pCtx->llcpNotifyMask |= 1 << LCTR_PROC_PHY_UPD;
  lctrStoreHostPhyUpdate(pCtx);
  lctrActFlushArq(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer PHY request.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerPhyReq(lctrConnCtx_t *pCtx)
{
  lctrStorePeerPhyReq(pCtx);
  lctrActFlushArq(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer PHY response.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerPhyRsp(lctrConnCtx_t *pCtx)
{
  lctrSendPhyUpdate(pCtx, pCtx->phyReq.txPhys, pCtx->phyReq.rxPhys, lctrDataPdu.pld.phyRsp.txPhys, lctrDataPdu.pld.phyRsp.rxPhys);
  lctrStopLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Send peer LL_REJECT_IND due to pending operation.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActRejectCollision(lctrConnCtx_t *pCtx)
{
  LL_TRACE_WARN1("Peer requested PHY while procedure pending, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
  lctrSendRejectInd(pCtx, LL_ERROR_CODE_LMP_ERR_TRANSACTION_COLLISION, TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of PHY update with disallowed status.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPhyUpdateDisallow(lctrConnCtx_t *pCtx)
{
  LL_TRACE_WARN1("Host requested PHY update while procedure pending, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
  lctrNotifyHostPhyUpdateInd(pCtx, LL_ERROR_CODE_CMD_DISALLOWED);
}

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      State machine action table. */
static const lctrActFn_t lctrMstPhyUpdateActionTbl[LCTR_PU_STATE_TOTAL][LCTR_PU_EVENT_TOTAL] =
{
  { /* LCTR_PU_STATE_IDLE */
    lctrActHostPhyUpdate,                       /* LCTR_PU_EVENT_HOST_PHY_UPD */
    NULL,                                       /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* out of sequence; ignore */
    lctrActPeerPhyReq,                          /* LCTR_PU_EVENT_PEER_PHY_REQ */
    NULL,                                       /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur */
    NULL,                                       /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    lctrActFlushArq,                            /* LCTR_PU_EVENT_INT_START_PHY_UPD */
    lctrActFlushArq,                            /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */
    NULL                                        /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  },
  { /* LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ */
    lctrActPhyUpdateDisallow,                   /* LCTR_PU_EVENT_HOST_PHY_UPD */
    NULL,                                       /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* should never occur */
    lctrActRejectCollision,                     /* LCTR_PU_EVENT_PEER_PHY_REQ */
    NULL,                                       /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur (not for this SM) */
    NULL,                                       /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    lctrActStartPhyUpdate                       /* LCTR_PU_EVENT_ARQ_FLUSHED */
  },
  { /* LCTR_PU_STATE_PHY_RSP */
    lctrActPhyUpdateDisallow,                   /* LCTR_PU_EVENT_HOST_PHY_UPD */
    lctrActPeerPhyRsp,                          /* LCTR_PU_EVENT_PEER_PHY_RSP */
    lctrActRejectCollision,                     /* LCTR_PU_EVENT_PEER_PHY_REQ */
    lctrActPeerRejectPhyReq,                    /* LCTR_PU_EVENT_PEER_REJECT */
    lctrActNotifyHostPhyUpdateSuccess,          /* LCTR_PU_EVENT_INT_PROC_COMP */             /* no change */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    NULL                                        /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  },
  { /* LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND */
    lctrActPhyUpdateDisallow,                   /* LCTR_PU_EVENT_HOST_PHY_UPD */
    NULL,                                       /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* should never occur */
    lctrActRejectCollision,                     /* LCTR_PU_EVENT_PEER_PHY_REQ */
    NULL,                                       /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur (not for this SM) */
    NULL,                                       /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    lctrActStartPhyUpdatePeer                   /* LCTR_PU_EVENT_ARQ_FLUSHED */
  },
  { /* LCTR_PU_STATE_PHY_UPD_INSTANT */
    lctrActPhyUpdateDisallow,                   /* LCTR_PU_EVENT_HOST_PHY_UPD */
    NULL,                                       /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* out of sequence; ignore */
    lctrActRejectCollision,                     /* LCTR_PU_EVENT_PEER_PHY_REQ */
    NULL,                                       /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur; ignore */
    lctrActNotifyHostPhyUpdateSuccess,          /* LCTR_PU_EVENT_INT_PROC_COMP */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    NULL,                                       /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    NULL                                        /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  }
};

/*! \brief      State machine next state table. */
static const uint8_t lctrMstPhyUpdateNextStateTbl[LCTR_PU_STATE_TOTAL][LCTR_PU_EVENT_TOTAL] =
{
  { /* LCTR_PU_STATE_IDLE */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_HOST_PHY_UPD */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* out of sequence; ignore */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_PEER_PHY_REQ */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_INT_START_PHY_UPD */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */
    LCTR_PU_STATE_IDLE                          /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  },
  { /* LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_HOST_PHY_UPD */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* should never occur */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_PEER_PHY_REQ */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur (not for this SM) */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_REQ,         /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    LCTR_PU_STATE_PHY_RSP                       /* LCTR_PU_EVENT_ARQ_FLUSHED */
  },
  { /* LCTR_PU_STATE_PHY_RSP */
    LCTR_PU_STATE_PHY_RSP,                      /* LCTR_PU_EVENT_HOST_PHY_UPD */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_PEER_PHY_RSP */
    LCTR_PU_STATE_PHY_RSP,                      /* LCTR_PU_EVENT_PEER_PHY_REQ */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_PEER_REJECT */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_INT_PROC_COMP */             /* no change */
    LCTR_PU_STATE_PHY_RSP,                      /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    LCTR_PU_STATE_PHY_RSP,                      /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    LCTR_PU_STATE_PHY_RSP                       /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  },
  { /* LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_HOST_PHY_UPD */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* should never occur */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_PEER_PHY_REQ */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur (not for this SM) */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_INT_PROC_COMP */             /* should never occur (not for this SM) */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    LCTR_PU_STATE_FLUSH_BEFORE_PHY_UPDATE_IND,  /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    LCTR_PU_STATE_PHY_UPD_INSTANT               /* LCTR_PU_EVENT_ARQ_FLUSHED */
  },
  { /* LCTR_PU_STATE_PHY_UPD_INSTANT */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_HOST_PHY_UPD */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_PEER_PHY_RSP */              /* out of sequence; ignore */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_PEER_PHY_REQ */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_PEER_REJECT */               /* should never occur; ignore */
    LCTR_PU_STATE_IDLE,                         /* LCTR_PU_EVENT_INT_PROC_COMP */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_INT_START_PHY_UPD */         /* should never occur */
    LCTR_PU_STATE_PHY_UPD_INSTANT,              /* LCTR_PU_EVENT_INT_START_PHY_UPD_PEER */    /* should never occur */
    LCTR_PU_STATE_PHY_UPD_INSTANT               /* LCTR_PU_EVENT_ARQ_FLUSHED */               /* should never occur (not for this SM) */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute action function.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   Event ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void lctrExecAction(lctrConnCtx_t *pCtx, uint8_t event)
{
  if (lctrMstPhyUpdateActionTbl[pCtx->phyUpdState][event])
  {
    lctrMstPhyUpdateActionTbl[pCtx->phyUpdState][event](pCtx);
  }

  pCtx->phyUpdState = lctrMstPhyUpdateNextStateTbl[pCtx->phyUpdState][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Get remapped event.
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
        case LL_PDU_PHY_REQ:
          return LCTR_PU_EVENT_PEER_PHY_REQ;
        case LL_PDU_PHY_RSP:
          return LCTR_PU_EVENT_PEER_PHY_RSP;

        case LL_PDU_UNKNOWN_RSP:
          if (lctrDataPdu.pld.unknownRsp.unknownType == LL_PDU_PHY_REQ)
          {
            return LCTR_PU_EVENT_PEER_REJECT;
          }
          /* Not for this SM. */
          break;
        case LL_PDU_REJECT_IND:
          if (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD)
          {
            return LCTR_PU_EVENT_PEER_REJECT;
          }
          /* Probably not for this SM. */
          break;
        case LL_PDU_REJECT_EXT_IND:
          if (lctrDataPdu.pld.rejInd.opcode == LL_PDU_PHY_REQ)
          {
            return LCTR_PU_EVENT_PEER_REJECT;
          }
          /* Not for this SM. */
          break;

        default:
          break;
      }
      break;

    /*** Host messages ***/

    case LCTR_CONN_MSG_API_PHY_UPDATE:
      return LCTR_PU_EVENT_HOST_PHY_UPD;

    /*** Internal messages ***/

    case LCTR_CONN_ARQ_Q_FLUSHED:
      if (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD)
      {
        return LCTR_PU_EVENT_ARQ_FLUSHED;
      }
      break;
    case LCTR_CONN_LLCP_PROC_CMPL:
      if (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD)
      {
        return LCTR_PU_EVENT_INT_PROC_COMP;
      }
      break;
    case LCTR_CONN_LLCP_START_PENDING:
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_PHY_UPD))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_PHY_UPD);
        return LCTR_PU_EVENT_INT_START_PHY_UPD;
      }
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_PHY_UPD_PEER))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_PHY_UPD_PEER);
        return LCTR_PU_EVENT_INT_START_PHY_UPD_PEER;
      }
      break;
    default:
      break;
  }
  return LCTR_PU_EVENT_INVALID;
}

/*************************************************************************************************/
/*!
 *  \brief      Resolve procedure collision.
 *
 *  \param      event   Subsystem event.
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrResolveCollision(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    case LCTR_PU_EVENT_PEER_PHY_REQ:
      if ((pCtx->llcpActiveProc == LCTR_PROC_CMN_CH_MAP_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_UPD) ||
          (pCtx->llcpActiveProc == LCTR_PROC_CONN_PARAM))
      {
        lctrSendRejectInd(pCtx, LL_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION, TRUE);
      }
      else
      {
        lctrStorePeerPhyReq(pCtx);
        pCtx->llcpPendMask |= 1 << LCTR_PROC_PHY_UPD_PEER;
        LL_TRACE_INFO2("Pending PHY_UPD=%u procedure: activeProc=%u", LCTR_PROC_PHY_UPD_PEER, pCtx->llcpActiveProc);
      }
      break;

    case LCTR_PU_EVENT_HOST_PHY_UPD:
      lctrStoreHostPhyUpdate(pCtx);
      pCtx->llcpNotifyMask |= 1 << LCTR_PROC_PHY_UPD;
      pCtx->llcpPendMask |= 1 << LCTR_PROC_PHY_UPD;
      LL_TRACE_INFO2("Pending PHY_UPD=%u procedure: activeProc=%u", LCTR_PROC_CONN_UPD, pCtx->llcpActiveProc);
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
 *  \return     None.
 *
 *  This routine will pend the active procedure and allow the phy update procedure to
 *  override it.
 */
/*************************************************************************************************/
static void lctrMstCheckProcOverride(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    case LCTR_PU_EVENT_PEER_PHY_REQ:
      /* Only the procedure without instant fields can be overridden. */
      switch (pCtx->llcpActiveProc)
      {
        case LCTR_PROC_CMN_VER_EXCH:
        case LCTR_PROC_CMN_FEAT_EXCH:
        case LCTR_PROC_CMN_DATA_LEN_UPD:
        case LCTR_PROC_CMN_REQ_PEER_SCA:
          pCtx->llcpPendMask |= 1 << pCtx->llcpActiveProc;
          pCtx->llcpActiveProc = LCTR_PROC_PHY_UPD;
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

/*************************************************************************************************/
/*!
 *  \brief      Execute master connection update LLCP state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     TRUE if peer response pending, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstLlcpExecutePhyUpdateSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  if ((event = lctrRemapEvent(pCtx, event)) == LCTR_PU_EVENT_INVALID)
  {
    return FALSE;
  }

  switch (pCtx->llcpState)
  {
    case LCTR_LLCP_STATE_IDLE:
      LL_TRACE_INFO3("lctrMstLlcpExecutePhyUpdateSm: handle=%u, llcpState=IDLE, phyUpdState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->phyUpdState, event);

      lctrExecAction(pCtx, event);

      if (pCtx->phyUpdState != LCTR_PU_STATE_IDLE)
      {
        pCtx->llcpState = LCTR_LLCP_STATE_BUSY;
        pCtx->llcpActiveProc = LCTR_PROC_PHY_UPD;
        pCtx->llcpInstantComp = FALSE;
      }
      break;

    case LCTR_LLCP_STATE_BUSY:
      LL_TRACE_INFO3("lctrMstLlcpExecutePhyUpdateSm: handle=%u, llcpState=BUSY, phyUpdState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->phyUpdState, event);

      lctrMstCheckProcOverride(pCtx, event);

      if (pCtx->llcpActiveProc == LCTR_PROC_PHY_UPD)
      {
        lctrExecAction(pCtx, event);

        if (pCtx->phyUpdState == LCTR_PU_STATE_IDLE)
        {
          lctrStopLlcpTimer(pCtx);
          pCtx->llcpState = LCTR_LLCP_STATE_IDLE;
          pCtx->llcpActiveProc = LCTR_PROC_INVALID;

          lctrStartPendingLlcp(pCtx);
        }
      }
      else
      {
        lctrResolveCollision(pCtx, event);
      }

      break;

    default:
      break;
  }

  return TRUE;
}
