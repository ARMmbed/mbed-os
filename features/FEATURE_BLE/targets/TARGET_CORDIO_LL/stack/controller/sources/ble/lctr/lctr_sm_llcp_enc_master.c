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
 * \brief Link layer controller master encryption connection state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_enc_master.h"
#include "lctr_int_enc_slave.h"
#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn_slave.h"
#include "lmgr_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(lctrConnCtx_t *pCtx);

/*! \brief      Encryption states. */
enum
{
  LCTR_ENC_STATE_UNENCRYPTED,           /*!< Connection established, unencrypted. */
  LCTR_ENC_STATE_FLUSH_START,           /*!< Wait for Tx queue flush on encryption start. */
  LCTR_ENC_STATE_PEER_LL_ENC_RSP,       /*!< Wait for LL_ENC_RSP from peer. */
  LCTR_ENC_STATE_PEER_START_ENC_REQ,    /*!< Wait for LL_START_ENC_REQ from peer. */
  LCTR_ENC_STATE_PEER_START_ENC_RSP,    /*!< Wait for LL_START_ENC_RSP from peer. */
  LCTR_ENC_STATE_ENCRYPTED,             /*!< Connection encrypted. */
  LCTR_ENC_STATE_FLUSH_RESTART,         /*!< Wait for Tx queue flush on encryption restart. */
  LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,    /*!< Wait for LL_PAUSE_ENC_RSP from peer. */
  LCTR_ENC_STATE_TOTAL                  /*!< Total encryption states. */
};

/*! \brief      Encryption events. */
enum
{
  LCTR_ENC_EVENT_HOST_START_ENC,        /*!< Received host start encryption command. */
  LCTR_ENC_EVENT_PEER_ENC_RSP,          /*!< Received peer LL_ENC_RSP. */
  LCTR_ENC_EVENT_PEER_START_ENC_REQ,    /*!< Received peer LL_START_ENC_REQ. */
  LCTR_ENC_EVENT_PEER_START_ENC_RSP,    /*!< Received peer LL_START_ENC_RSP. */
  LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP,    /*!< Received peer LL_PAUSE_ENC_RSP. */
  LCTR_ENC_EVENT_ARQ_FLUSHED,           /*!< Internal ARQ queue flushed. */
  LCTR_ENC_EVENT_PEER_REJECT,           /*!< Received peer LL_REJECT_IND. */
  LCTR_ENC_EVENT_TOTAL,                 /*!< Total encryption states. */
  LCTR_ENC_EVENT_INVALID = 0xFF         /*!< Invalid event. */
};

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Notify host of encryption change with disallowed status.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActEncChangeDisallow(lctrConnCtx_t *pCtx)
{
  LL_TRACE_WARN1("Host requested encryption while encryption procedure pending, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
  lctrNotifyEncChangeInd(pCtx, LL_ERROR_CODE_CMD_DISALLOWED);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received host start encryption command.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActStartEnc(lctrConnCtx_t *pCtx)
{
  lctrPauseTxData(pCtx);
  lctrCheckPauseComplete(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action Tx data queue flushed on encryption start.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActTxFlushedOnStart(lctrConnCtx_t *pCtx)
{
  lctrDisableTxDataEnc(pCtx);
  lctrDisableRxDataEnc(pCtx);
  lctrSendEncReq(pCtx);
  lctrStartLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer LL_ENC_RSP.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerEncRsp(lctrConnCtx_t *pCtx)
{
  lctrStopLlcpTimer(pCtx);
  lctrStoreSlvVectors(pCtx);
  lctrCalcSessionKey(pCtx);
  lctrPauseRxData(pCtx);

  /* Expect a LL_START_ENC_REQ or time out. */
  lctrStartLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer LL_START_ENC_REQ.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerStartEncReq(lctrConnCtx_t *pCtx)
{
  lctrEnableTxDataEnc(pCtx);
  lctrEnableRxDataEnc(pCtx);
  lctrSendStartEncRsp(pCtx);
  lctrStartLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer LL_START_ENC_RSP.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerStartEncRsp(lctrConnCtx_t *pCtx)
{
  lctrStopLlcpTimer(pCtx);
  lctrUnpauseTxData(pCtx);
  lctrUnpauseRxData(pCtx);

  (pCtx->keyUpdFlag) ? lctrNotifyEncKeyRefreshInd(pCtx) : lctrNotifyEncChangeInd(pCtx, LL_SUCCESS);
  pCtx->keyUpdFlag = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received host refresh key.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActRefreshEncKey(lctrConnCtx_t *pCtx)
{
  lctrPauseTxData(pCtx);
  lctrCheckPauseComplete(pCtx);

  pCtx->keyUpdFlag = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Action Tx data queue flushed on encryption restart.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActTxFlushedOnRestart(lctrConnCtx_t *pCtx)
{
  lctrSendPauseEncReq(pCtx);
  lctrStartLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer LL_PAUSE_ENC_RSP.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerPauseEncRsp(lctrConnCtx_t *pCtx)
{
  lctrDisableTxDataEnc(pCtx);
  lctrDisableRxDataEnc(pCtx);
  lctrPauseRxData(pCtx);
  lctrSendPauseEncRsp(pCtx);
  lctrSendEncReq(pCtx);
  lctrStartLlcpTimer(pCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Action indirection for received peer LL_REJECT_IND.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrActPeerEncReject(lctrConnCtx_t *pCtx)
{
  lctrStopLlcpTimer(pCtx);
  lctrUnpauseTxData(pCtx);
  lctrUnpauseRxData(pCtx);

  uint8_t reason = (lctrDataPdu.opcode == LL_PDU_UNKNOWN_RSP) ?
                   LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE :
                   lctrDataPdu.pld.rejInd.reason;

  LL_TRACE_WARN2("Peer rejected start/restart encryption, handle=%u, reason=%u", LCTR_GET_CONN_HANDLE(pCtx), reason);

  lctrNotifyEncChangeInd(pCtx, reason);

  pCtx->keyUpdFlag = FALSE;
}

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      State machine action table. */
static const lctrActFn_t lctrMstEncActionTbl[LCTR_ENC_STATE_TOTAL][LCTR_ENC_EVENT_TOTAL] =
{
  { /* LCTR_ENC_STATE_UNENCRYPTED */
    lctrActStartEnc,                    /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    NULL                                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_FLUSH_START */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    lctrActTxFlushedOnStart,            /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    lctrActPeerEncReject                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_LL_ENC_RSP */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrActPeerEncRsp,                  /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    lctrActPeerEncReject                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_START_ENC_REQ */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrActPeerStartEncReq,             /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    lctrActPeerEncReject                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_START_ENC_RSP */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrActPeerStartEncRsp,             /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    lctrActPeerEncReject                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_ENCRYPTED */
    lctrActRefreshEncKey,               /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    NULL                                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_FLUSH_RESTART */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    lctrActTxFlushedOnRestart,          /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    NULL                                /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP */
    lctrActEncChangeDisallow,           /* LCTR_ENC_EVENT_HOST_START_ENC */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    lctrInvalidEncPduSeq,               /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    lctrActPeerPauseEncRsp,             /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    NULL,                               /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    NULL                                /* LCTR_ENC_EVENT_PEER_REJECT */
  }
};

/*! \brief      State machine next state table. */
static const uint8_t lctrMstEncNextStateTbl[LCTR_ENC_STATE_TOTAL][LCTR_ENC_EVENT_TOTAL] =
{
  { /* LCTR_ENC_STATE_UNENCRYPTED */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_UNENCRYPTED,         /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_UNENCRYPTED,         /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_UNENCRYPTED,         /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_UNENCRYPTED,         /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_UNENCRYPTED,         /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_FLUSH_START */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_FLUSH_START,         /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_LL_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_REQ,  /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_START_ENC_RSP,  /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_ENCRYPTED */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_ENCRYPTED,           /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_ENCRYPTED            /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_FLUSH_RESTART */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_FLUSH_RESTART,       /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_UNENCRYPTED          /* LCTR_ENC_EVENT_PEER_REJECT */
  },
  { /* LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_HOST_START_ENC */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_ENC_RSP */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_START_ENC_REQ */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_PEER_START_ENC_RSP */
    LCTR_ENC_STATE_PEER_LL_ENC_RSP,     /* LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP,  /* LCTR_ENC_EVENT_ARQ_FLUSHED */
    LCTR_ENC_STATE_PEER_PAUSE_ENC_RSP   /* LCTR_ENC_EVENT_PEER_REJECT */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Get remapped encryption LLCP procedure event.
 *
 *  \param      event   Subsystem event.
 *  \param      pCtx    Connection context.
 *
 *  \return     Encryption LLCP procedure event ID.
 *
 *  This routine remaps events for the encryption LLCP procedure state machine for optimized
 *  compressed state tables.
 */
/*************************************************************************************************/
static uint8_t lctrMstRemapEncryptEvent(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (event)
  {
    /*** Peer messages ***/

    case LCTR_CONN_MSG_RX_LLCP:
      switch (lctrDataPdu.opcode)
      {
        case LL_PDU_ENC_RSP:
          return LCTR_ENC_EVENT_PEER_ENC_RSP;
        case LL_PDU_START_ENC_REQ:
          return LCTR_ENC_EVENT_PEER_START_ENC_REQ;
        case LL_PDU_START_ENC_RSP:
          return LCTR_ENC_EVENT_PEER_START_ENC_RSP;
        case LL_PDU_PAUSE_ENC_RSP:
          return LCTR_ENC_EVENT_PEER_PAUSE_ENC_RSP;

        case LL_PDU_UNKNOWN_RSP:
          if (lctrDataPdu.pld.unknownRsp.unknownType == LL_PDU_ENC_REQ)
          {
            return LCTR_ENC_EVENT_PEER_REJECT;
          }
          /* Not for this SM. */
          break;
        case LL_PDU_REJECT_IND:
          if (pCtx->llcpActiveProc == LCTR_PROC_ENCRYPT)
          {
            return LCTR_ENC_EVENT_PEER_REJECT;
          }
          /* Probably not for this SM. */
          break;
        case LL_PDU_REJECT_EXT_IND:
          if (lctrDataPdu.pld.rejInd.opcode == LL_PDU_ENC_REQ)
          {
            return LCTR_ENC_EVENT_PEER_REJECT;
          }
          /* Not for this SM. */
          break;

        default:
          break;
      }
      break;

    /*** Host messages ***/

    case LCTR_CONN_MSG_API_START_ENC:
      return LCTR_ENC_EVENT_HOST_START_ENC;

    /*** Internal messages ***/

    case LCTR_CONN_ARQ_Q_FLUSHED:
      if (pCtx->llcpActiveProc == LCTR_PROC_ENCRYPT)
      {
        return LCTR_ENC_EVENT_ARQ_FLUSHED;
      }
      break;

    case LCTR_CONN_LLCP_START_PENDING:
      if (pCtx->llcpPendMask & (1 << LCTR_PROC_ENCRYPT))
      {
        pCtx->llcpPendMask &= ~(1 << LCTR_PROC_ENCRYPT);
        return LCTR_ENC_EVENT_HOST_START_ENC;
      }
      break;

    default:
      break;
  }

  return LCTR_ENC_EVENT_INVALID;
}

/*************************************************************************************************/
/*!
 *  \brief      Execute encryption master LLCP state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     TRUE if peer response pending, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrMstEncryptSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  bool_t result;

  if (lctrMstEncActionTbl[pCtx->encState][event])
  {
    lctrMstEncActionTbl[pCtx->encState][event](pCtx);
  }

  pCtx->encState = lctrMstEncNextStateTbl[pCtx->encState][event];

  switch (pCtx->encState)
  {
    /* IDLE states. */
    case LCTR_ENC_STATE_UNENCRYPTED:
    case LCTR_ENC_STATE_ENCRYPTED:
      result = FALSE;
      break;

    /* BUSY states. */
    default:
      result = TRUE;
      break;
  }

  return result;
}

/*************************************************************************************************/
/*!
 *  \brief      Execute encryption master LLCP state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     TRUE if handled by this SM, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lctrMstExecuteEncryptSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  if (pCtx->pauseRxData == TRUE)        /* During encryption start or encryption pause. */
  {
    switch(event)
    {
      case LCTR_CONN_MSG_RX_LLCP:
        switch(lctrDataPdu.opcode)
        {
          /* Allowed PDUs. */
          case LL_PDU_TERMINATE_IND:
          case LL_PDU_REJECT_IND:
          case LL_PDU_REJECT_EXT_IND:
          case LL_PDU_ENC_RSP:
          case LL_PDU_START_ENC_REQ:
          case LL_PDU_START_ENC_RSP:
          case LL_PDU_PAUSE_ENC_RSP:
          case LL_PDU_UNKNOWN_RSP:
            break;

          /* Illegal PDUs. */
          default:
            LL_TRACE_WARN2("Unexpected PDU received during encryption procedure, handle=%u, opcode=%u", LCTR_GET_CONN_HANDLE(pCtx), lctrDataPdu.opcode);
            lctrInvalidEncPduSeq(pCtx);
            return FALSE;
        }
        break;

      case LCTR_CONN_MSG_RX_LLCP_UNKNOWN:
      case LCTR_CONN_MSG_RX_LLCP_INVALID_PARAM:
        LL_TRACE_WARN1("Unknown/Invalid PDU received during encryption procedure event=%u", event);
        lctrInvalidEncPduSeq(pCtx); /* Transit to standby state and notify the host */
        return TRUE;                /* Skip the lctrLlcpStatelessEventHandler */

      default:
        break;
    }
  }

  /* Save host parameters. */
  if (((pCtx->llcpState == LCTR_LLCP_STATE_IDLE) ||   /* only when encryption start not active */
                                                      /* otherwise this will be rejected */
      ((pCtx->llcpState == LCTR_LLCP_STATE_BUSY) && (pCtx->llcpActiveProc != LCTR_PROC_ENCRYPT))) &&
       (event == LCTR_CONN_MSG_API_START_ENC))
  {
    lctrGenerateMstVectors(pCtx);
  }

  if (((lmgrCb.features & LL_FEAT_ENCRYPTION) == 0) ||
      ((event = lctrMstRemapEncryptEvent(pCtx, event)) == LCTR_ENC_EVENT_INVALID))
  {
    return FALSE;
  }

  switch (pCtx->llcpState)
  {
    case LCTR_LLCP_STATE_IDLE:
      LL_TRACE_INFO3("lctrMstExecuteEncryptSm: handle=%u, llcpState=IDLE, encState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->encState, event);

      lctrMstEncryptSm(pCtx, event);

      if ((pCtx->encState != LCTR_ENC_STATE_UNENCRYPTED) &&
          (pCtx->encState != LCTR_ENC_STATE_ENCRYPTED))
      {
        pCtx->llcpActiveProc = LCTR_PROC_ENCRYPT;
        pCtx->llcpState = LCTR_LLCP_STATE_BUSY;
      }
      break;

    case LCTR_LLCP_STATE_BUSY:
      LL_TRACE_INFO3("lctrMstExecuteEncryptSm: handle=%u, llcpState=BUSY, encState=%u, event=%u", LCTR_GET_CONN_HANDLE(pCtx), pCtx->encState, event);

      if (lctrCheckActiveOrPend(pCtx, LCTR_PROC_ENCRYPT))
      {
        lctrMstEncryptSm(pCtx, event);

        if ((pCtx->encState == LCTR_ENC_STATE_ENCRYPTED) ||
            (pCtx->encState == LCTR_ENC_STATE_UNENCRYPTED))
        {
          pCtx->llcpActiveProc = LCTR_PROC_INVALID;
          pCtx->llcpState = LCTR_LLCP_STATE_IDLE;

          lctrStartPendingLlcp(pCtx);
        }
      }
      else
      {
        LL_TRACE_INFO2("Pending ENCRYPT=%u procedure: activeProc=%u", LCTR_PROC_ENCRYPT, pCtx->llcpActiveProc);
      }
      break;

    default:
      break;
  }

  return TRUE;
}
