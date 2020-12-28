/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller connection state machine action routines.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#include "ll_defs.h"
#include "lctr_int_conn.h"
#include "lctr_int_adv_slave.h"
#include "lctr_int_adv_master_ae.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "lmgr_api_conn.h"
#include "pal_radio.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Valid feature bits applicable between controllers */
#define LCTR_FEAT_PEER_MASK     (LL_FEAT_ENCRYPTION | \
                                 LL_FEAT_CONN_PARAM_REQ_PROC | \
                                 LL_FEAT_EXT_REJECT_IND | \
                                 LL_FEAT_SLV_INIT_FEAT_EXCH | \
                                 LL_FEAT_DATA_LEN_EXT | \
                                 LL_FEAT_LE_2M_PHY | \
                                 LL_FEAT_STABLE_MOD_IDX_TRANSMITTER | \
                                 LL_FEAT_STABLE_MOD_IDX_RECEIVER | \
                                 LL_FEAT_LE_CODED_PHY | \
                                 LL_FEAT_CH_SEL_2 | \
                                 LL_FEAT_LE_POWER_CLASS_1 | \
                                 LL_FEAT_MIN_NUM_USED_CHAN | \
                                 LL_FEAT_CONN_CTE_REQ | \
                                 LL_FEAT_CONN_CTE_RSP | \
                                 LL_FEAT_RECV_CTE | \
                                 LL_FEAT_PAST_SENDER | \
                                 LL_FEAT_PAST_RECIPIENT | \
                                 LL_FEAT_SCA_UPDATE | \
                                 LL_FEAT_CIS_MASTER_ROLE | \
                                 LL_FEAT_CIS_SLAVE_ROLE | \
                                 LL_FEAT_ISO_BROADCASTER | \
                                 LL_FEAT_ISO_SYNC | \
                                 LL_FEAT_ISO_HOST_SUPPORT | \
                                 LL_FEAT_POWER_CONTROL_REQUEST | \
                                 LL_FEAT_POWER_CHANGE_IND | \
                                 LL_FEAT_PATH_LOSS_MONITOR)

/*! \brief      Used feature bitmask. */
#define LCTR_USED_FEAT_SET_MASK     0x000000FFFF

/*! \brief      Features bits mask over the air */
#define LCTR_OTA_FEAT_MASK      (~LL_FEAT_REMOTE_PUB_KEY_VALIDATION & LCTR_FEAT_PEER_MASK)

/*************************************************************************************************/
/*!
 *  \brief      Validate connection parameter range.
 *
 *  \param      pConnParam    Connection Parameter.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
static bool_t lctrValidateConnParam(const lctrConnParam_t *pConnParam)
{
  if (pConnParam->connIntervalMin != pConnParam->connIntervalMax)
  {
    if ((pConnParam->prefPeriod == 0) ||
        (pConnParam->prefPeriod > pConnParam->connIntervalMax))
    {
      return FALSE;
    }
  }

  return TRUE;
}


/*************************************************************************************************/
/*!
 *  \brief  Compute the sleep clock accuracy index in connection context.
 *
 *  \param  pCtx    Connection context.
 *
 *  \return SCA index.
 */
/*************************************************************************************************/
static uint8_t lctrComputeConnSca(lctrConnCtx_t *pCtx)
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

  return (uint8_t) (sca + pCtx->scaMod);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of connect indication.
 *
 *  \param      handle          Connection handle.
 *  \param      role            Role.
 *  \param      pConnInd        Connection indication.
 *  \param      peerIdAddrType  Peer ID address type.
 *  \param      peerIdAddr      Peer ID address.
 *  \param      peerRpa         Peer RPA.
 *  \param      localRpa        Local RPA.
 *  \param      status          Status.
 *  \param      usedChSel       Used channel selection algorithm.
 */
/*************************************************************************************************/
void lctrNotifyHostConnectInd(uint16_t handle, uint8_t role, lctrConnInd_t *pConnInd,
                              uint8_t peerIdAddrType, uint64_t peerIdAddr, uint64_t peerRpa,
                              uint64_t localRpa, uint8_t status, uint8_t usedChSel)
{
  LlConnInd_t connEvt =
  {
    .hdr =
    {
      .param        = handle,
      .event        = LL_CONN_IND,
      .status       = status
    },

    .status         = status,
    .handle         = handle,
    .role           = role,
    .addrType       = peerIdAddrType
  };

  if (pConnInd)
  {
    connEvt.connInterval   = pConnInd->interval;
    connEvt.connLatency    = pConnInd->latency;
    connEvt.supTimeout     = pConnInd->timeout;
    connEvt.clockAccuracy  = pConnInd->masterSca;
  }

  uint8_t *pBuf = (uint8_t *)&connEvt.peerAddr;
  BDA64_TO_BSTREAM(pBuf, peerIdAddr);

  pBuf = (uint8_t *)&connEvt.peerRpa;
  BDA64_TO_BSTREAM(pBuf, peerRpa);

  pBuf = (uint8_t *)&connEvt.localRpa;
  BDA64_TO_BSTREAM(pBuf, localRpa);

  LL_TRACE_INFO2("### LlEvent ###  LL_CONN_IND, handle=%u, status=%u", handle, status);

  LmgrSendEvent((LlEvt_t *)&connEvt);

  if ((status == LL_SUCCESS) &&
      (lmgrCb.features & LL_FEAT_CH_SEL_2))
  {
    LlChSelInd_t chSelEvt =
    {
      .hdr =
      {
        .param        = handle,
        .event        = LL_CH_SEL_ALGO_IND,
        .status       = status
      },
      .handle         = handle,
      .usedChSel      = usedChSel
    };

    LL_TRACE_INFO3("### LlEvent ###  LL_CH_SEL_ALGO_IND, handle=%u, status=%u, usedChSel=%u", handle, status, usedChSel);

    LmgrSendEvent((LlEvt_t *)&chSelEvt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection update connection specification.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnUpdateSpec(lctrConnCtx_t *pCtx)
{
  pCtx->connUpdSpec = pLctrConnMsg->connUpd.connSpec;
}

/*************************************************************************************************/
/*!
 *  \brief      Store connect update parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnUpdate(lctrConnCtx_t *pCtx)
{
  /* Check for valid parameters. */
  if ((uint16_t)(lctrDataPdu.pld.connUpdInd.instant - pCtx->eventCounter) >= LCTR_MAX_INSTANT)
  {
    lctrSendConnMsg(pCtx, LCTR_CONN_TERM_INST_PASSED);
  }

  pCtx->connUpd = lctrDataPdu.pld.connUpdInd;
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of connection update indication.
 *
 *  \param      pCtx    Connection context.
 *  \param      status  Status.
 */
/*************************************************************************************************/
void lctrNotifyHostConnUpdateInd(lctrConnCtx_t *pCtx, uint8_t status)
{
  const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  LlConnUpdateInd_t evt =
  {
    .hdr =
    {
      .param        = handle,
      .event        = LL_CONN_UPDATE_IND,
      .status       = status
    },

    .status         = status,
    .handle         = handle,
  };

  if (status == LL_SUCCESS)
  {
    evt.connInterval = pCtx->connUpd.interval;
    evt.connLatency  = pCtx->connUpd.latency;
    evt.supTimeout   = pCtx->connUpd.timeout;
  }

  LL_TRACE_INFO2("### LlEvent ###  LL_CONN_UPDATE_IND, handle=%u, status=%u", handle, status);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Store channel map parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreChanMapUpdate(lctrConnCtx_t *pCtx)
{
  pCtx->chanMapUpd.chanMask = pLctrConnMsg->chanMapUpd.chanMap;
}

/*************************************************************************************************/
/*!
 *  \brief      Send channel map update indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendChanMapUpdateInd(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_CHAN_MAP_IND_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

#if (LL_ENABLE_TESTER)
    if (llTesterCb.eventCounterOverride == TRUE)
    {
      pCtx->chanMapUpd.instant = pCtx->eventCounter +
                                 llTesterCb.eventCounterOffset + 1;     /* +1 for next CE */
    }
    else
#endif
    {
      pCtx->chanMapUpd.instant = pCtx->eventCounter +
                                 LL_MIN_INSTANT + 1 +     /* +1 for next CE */
                                 pCtx->maxLatency;        /* ensure slave will listen to this packet */
    }

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, LL_PDU_CHANNEL_MAP_IND);

    UINT40_TO_BSTREAM(pBuf, pCtx->chanMapUpd.chanMask);
    UINT16_TO_BSTREAM(pBuf, pCtx->chanMapUpd.instant);

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store channel map parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreChanMap(lctrConnCtx_t *pCtx)
{
  pCtx->chanMapUpd = lctrDataPdu.pld.chanMapInd;

  /* Check for valid parameters. */
  if (((uint16_t)(pCtx->chanMapUpd.instant - pCtx->eventCounter) >= LCTR_MAX_INSTANT) ||
      (pCtx->chanMapUpd.chanMask == 0))     /* must have at least one channel */
  {
    lctrSendConnMsg(pCtx, LCTR_CONN_TERM_INST_PASSED);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send feature request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendFeatureReq(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_FEATURE_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;
    uint8_t opcode = (pCtx->role == LL_ROLE_MASTER) ? LL_PDU_FEATURE_REQ : LL_PDU_SLV_FEATURE_REQ;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, opcode);
    UINT64_TO_BSTREAM(pBuf, (lmgrCb.features & LCTR_OTA_FEAT_MASK));

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send feature response PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendFeatureRsp(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_FEATURE_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, LL_PDU_FEATURE_RSP);

    uint64_t featSet = (pCtx->usedFeatSet   & LCTR_USED_FEAT_SET_MASK) |  /* FeatureSet[0] used by master and slave */
                       (lmgrCb.features     & ~LCTR_USED_FEAT_SET_MASK);  /* FeatureSet[1..7] used by sender */
    UINT64_TO_BSTREAM(pBuf, (featSet & LCTR_OTA_FEAT_MASK));              /* Only send valid features bits between controllers. */

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store remote feature data.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreUsedFeatures(lctrConnCtx_t *pCtx)
{
  pCtx->usedFeatSet = lmgrCb.features & lctrDataPdu.pld.featReqRsp.featSet & LCTR_FEAT_PEER_MASK;
  pCtx->featExchFlag = TRUE;

  /* Update stable modulation index. */
  pCtx->bleData.chan.peerTxStableModIdx =
    (lctrDataPdu.pld.featReqRsp.featSet & LL_FEAT_STABLE_MOD_IDX_TRANSMITTER) ? TRUE : FALSE;
  pCtx->bleData.chan.peerRxStableModIdx =
    (lctrDataPdu.pld.featReqRsp.featSet & LL_FEAT_STABLE_MOD_IDX_RECEIVER) ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Send version indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendVersionInd(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  /* Maximum of one LL_VERSION_IND must be sent to peer. */
  if (pCtx->numSentVerInd == 0)
  {
    pCtx->numSentVerInd++;

    if ((pPdu = lctrTxCtrlPduAlloc(LL_VERSION_IND_PDU_LEN)) != NULL)
    {
      uint8_t *pBuf = pPdu;

      /*** Assemble control PDU. ***/

      UINT8_TO_BSTREAM(pBuf, LL_PDU_VERSION_IND);
      UINT8_TO_BSTREAM(pBuf, pLctrRtCfg->btVer);
      UINT16_TO_BSTREAM(pBuf, pLctrRtCfg->compId);
      UINT16_TO_BSTREAM(pBuf, pLctrRtCfg->implRev);

      /*** Queue for transmit. ***/

      lctrTxCtrlPduQueue(pCtx, pPdu);
    }
  }
  else
  {
    lctrSendRejectInd(pCtx, LL_ERROR_CODE_LMP_PDU_NOT_ALLOWED, FALSE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store remote version data.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreRemoteVer(lctrConnCtx_t *pCtx)
{
  if (!pCtx->remoteVerValid)
  {
    pCtx->remoteVerValid = TRUE;
    pCtx->remoteVer = lctrDataPdu.pld.verInd;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of read remote version confirm.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrNotifyHostReadRemoteVerCnf(lctrConnCtx_t *pCtx)
{
  const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  LlReadRemoteVerInfoCnf_t evt =
  {
    .hdr =
    {
      .param        = handle,
      .event        = LL_READ_REMOTE_VER_INFO_CNF,
      .status       = LL_SUCCESS
    },

    .status         = LL_SUCCESS,
    .handle         = handle,
    .version        = pCtx->remoteVer.versNr,
    .mfrName        = pCtx->remoteVer.compId,
    .subversion     = pCtx->remoteVer.subVersNr
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_READ_REMOTE_VER_INFO_CNF, handle=%u, status=LL_SUCCESS", handle);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send terminate indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendTerminateInd(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;
  uint8_t *pBuf;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_TERMINATE_IND_PDU_LEN)) != NULL)
  {
    pBuf = pPdu;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, LL_PDU_TERMINATE_IND);
    UINT8_TO_BSTREAM(pBuf, pCtx->termReason);

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of disconnect indication.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrNotifyHostDisconnectInd(lctrConnCtx_t *pCtx)
{
  if (pCtx->termReason < LCTR_RESET_TERM_REASON)
  {
    const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

    LlDisconnectInd_t evt =
    {
      .hdr =
      {
        .param        = handle,
        .event        = LL_DISCONNECT_IND,
        .status       = LL_SUCCESS
      },

      .status         = LL_SUCCESS,
      .handle         = handle,
      .reason         = pCtx->termReason
    };

    if (pCtx->llcpNotifyMask & (1 << LCTR_PROC_CMN_TERM))
    {
      /* Host initiated termination. */
      evt.reason = LL_ERROR_CODE_CONN_TERM_BY_LOCAL_HOST;
    }

    LL_TRACE_INFO2("### LlEvent ###  LL_DISCONNECT_IND, handle=%u, status=LL_SUCCESS, reason=%u", handle, pCtx->termReason);

    LmgrSendEvent((LlEvt_t *)&evt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store LLCP termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = lctrDataPdu.pld.termInd.errorCode;

  /* Ensure Ack for the received LL_TERMINATE_IND is Tx'ed. */
  pCtx->termAckReqd = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Store host initiated disconnect termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreDisconnectReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = pLctrConnMsg->disc.reason;
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection failed to establish termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnFailEstablishTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = LL_ERROR_CODE_CONN_FAILED_TO_ESTABLISH;
}

/*************************************************************************************************/
/*!
 *  \brief      Store LLCP timeout termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreLlcpTimeoutTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = LL_ERROR_CODE_LMP_LL_RESP_TIMEOUT;
}

/*************************************************************************************************/
/*!
 *  \brief      Store reset termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreResetTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = LCTR_RESET_TERM_REASON;
}

/*************************************************************************************************/
/*!
 *  \brief      Store invalid request termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreInstantPassedTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = LL_ERROR_CODE_INSTANT_PASSED;
}

/*************************************************************************************************/
/*!
 *  \brief      Store invalid request termination reason.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreMicFailedTerminateReason(lctrConnCtx_t *pCtx)
{
  pCtx->termReason = LL_ERROR_CODE_CONN_TERM_MIC_FAILURE;
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection parameter request.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnParamReq(lctrConnCtx_t *pCtx)
{
  pCtx->connParam = lctrDataPdu.pld.connParamReqRsp;
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection parameter request.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnParamRsp(lctrConnCtx_t *pCtx)
{
  LlConnSpec_t peerRsp;
  peerRsp.connIntervalMin = lctrDataPdu.pld.connParamReqRsp.connIntervalMin;
  peerRsp.connIntervalMax = lctrDataPdu.pld.connParamReqRsp.connIntervalMax;
  peerRsp.connLatency     = lctrDataPdu.pld.connParamReqRsp.connLatency;
  peerRsp.supTimeout      = lctrDataPdu.pld.connParamReqRsp.supTimeout;
  peerRsp.minCeLen        = 0;
  peerRsp.maxCeLen        = 0;

  if (!lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_MST_IGNORE_CP_RSP) &&
      (LctrValidateConnSpec(&peerRsp) == LL_SUCCESS))
  {
    /* Store slave's connection parameter response. */
    pCtx->connParam = lctrDataPdu.pld.connParamReqRsp;

    if (!lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_MST_UNCOND_CP_RSP))
    {
      /* Ensure slave's response is within master's original connection update parameters. */
      if ((lctrDataPdu.pld.connParamReqRsp.connIntervalMin >= pCtx->connUpdSpec.connIntervalMin) &&
          (lctrDataPdu.pld.connParamReqRsp.connIntervalMin <= pCtx->connUpdSpec.connIntervalMax))
      {
        pCtx->connUpdSpec.connIntervalMin = WSF_MAX(pCtx->connUpdSpec.connIntervalMin,
                                                    lctrDataPdu.pld.connParamReqRsp.connIntervalMin);
      }

      if ((lctrDataPdu.pld.connParamReqRsp.connIntervalMax >= pCtx->connUpdSpec.connIntervalMin) &&
          (lctrDataPdu.pld.connParamReqRsp.connIntervalMax <= pCtx->connUpdSpec.connIntervalMax))
      {
        pCtx->connUpdSpec.connIntervalMax = WSF_MIN(pCtx->connUpdSpec.connIntervalMax,
                                                    lctrDataPdu.pld.connParamReqRsp.connIntervalMax);
      }

      pCtx->connUpdSpec.connLatency = WSF_MIN(pCtx->connUpdSpec.connLatency,
                                              lctrDataPdu.pld.connParamReqRsp.connLatency);
      pCtx->connUpdSpec.supTimeout  = WSF_MAX(pCtx->connUpdSpec.supTimeout,
                                              lctrDataPdu.pld.connParamReqRsp.supTimeout);

      /* pCtx->connUpdSpec.minCeLen = 0; */   /* ignored */
      /* pCtx->connUpdSpec.maxCeLen = 0; */   /* ignored */
    }
    else
    {
      pCtx->connUpdSpec = peerRsp;
    }
  }
  else
  {
    LL_TRACE_WARN1("Ignoring peer parameter received LL_CONNECTION_PARAM_RSP, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Store connection parameter connection specification.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreConnParamSpec(lctrConnCtx_t *pCtx)
{
  pCtx->connUpdSpec = pLctrConnMsg->connParamReply.connSpec;
}

/*************************************************************************************************/
/*!
 *  \brief      Send connection parameter request PDU to peer.
 *
 *  \param      pCtx        Connection context.
 *  \param      opcode      Pdu Opcode.
 *  \param      pConnSpec   Connection specification.
 *  \param      prefPeriod  Preferred periodicity.
 */
/*************************************************************************************************/
static void lctrSendConnParamPdu(lctrConnCtx_t *pCtx, uint8_t opcode, LlConnSpec_t *pConnSpec, uint8_t prefPeriod)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_CONN_PARAM_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;
    unsigned int i;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM (pBuf, opcode);

    UINT16_TO_BSTREAM(pBuf, pConnSpec->connIntervalMin);
    UINT16_TO_BSTREAM(pBuf, pConnSpec->connIntervalMax);
    UINT16_TO_BSTREAM(pBuf, pConnSpec->connLatency);
    UINT16_TO_BSTREAM(pBuf, pConnSpec->supTimeout);

    UINT8_TO_BSTREAM (pBuf, prefPeriod);
    UINT16_TO_BSTREAM(pBuf, pCtx->eventCounter);

    for (i = 0; i < LCTR_OFFSET_COUNT; i++)
    {
      UINT16_TO_BSTREAM(pBuf, 0xFFFF);
    }

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send connection parameter request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendConnParamReq(lctrConnCtx_t *pCtx)
{
#if (LL_ENABLE_TESTER)
  if (llTesterCb.connParamReqEnabled)
  {
    pCtx->connUpdSpec.connIntervalMin = llTesterCb.connParamReq.connIntervalMin;
    pCtx->connUpdSpec.connIntervalMax = llTesterCb.connParamReq.connIntervalMax;
    pCtx->connUpdSpec.connLatency     = llTesterCb.connParamReq.connLatency;
    pCtx->connUpdSpec.supTimeout      = llTesterCb.connParamReq.supTimeout;
    pCtx->connUpdSpec.minCeLen        = 0;
    pCtx->connUpdSpec.maxCeLen        = 0;

    llTesterCb.connParamReqEnabled    = FALSE;

    lctrSendConnParamPdu(pCtx, LL_PDU_CONN_PARAM_REQ, &pCtx->connUpdSpec, llTesterCb.connParamReq.prefPeriod);

    return;
  }
#endif

  uint8_t prefPeriod = (LCTR_US_TO_CONN_IND(SCH_RM_PREF_PER_USEC) <= pCtx->connUpdSpec.connIntervalMax) ?
                                                                           LCTR_US_TO_CONN_IND(SCH_RM_PREF_PER_USEC)
                                                                           : LCTR_US_TO_CONN_IND(SCH_RM_PREF_PER_USEC_LOWEST);
  lctrSendConnParamPdu(pCtx, LL_PDU_CONN_PARAM_REQ, &pCtx->connUpdSpec, prefPeriod);
}

/*************************************************************************************************/
/*!
 *  \brief      Send connection parameter response PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendConnParamRsp(lctrConnCtx_t *pCtx)
{
  if ((pLctrRtCfg->maxConn > 1) &&
      (lmgrCb.numConnEnabled > 1))
  {
    /* TODO resolve scheduling with multiple connections */
  }

  lctrSendConnParamPdu(pCtx, LL_PDU_CONN_PARAM_RSP, &pLctrConnMsg->connParamReply.connSpec, 1);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of remote connection parameter change indication.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrNotifyHostConnParamInd(lctrConnCtx_t *pCtx)
{
  const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  LlConnSpec_t connSpec;
  connSpec.connIntervalMin = pCtx->connParam.connIntervalMin;
  connSpec.connIntervalMax = pCtx->connParam.connIntervalMax;
  connSpec.connLatency     = pCtx->connParam.connLatency;
  connSpec.supTimeout      = pCtx->connParam.supTimeout;
  connSpec.minCeLen        = 0;
  connSpec.maxCeLen        = 0;

  if (!lctrValidateConnParam(&pCtx->connParam) ||
      (LctrValidateConnSpec(&connSpec) != LL_SUCCESS))
  {
    LlRemoteConnParamReqNegReply(handle, LL_ERROR_CODE_INVALID_LMP_PARAMS);
    LL_TRACE_ERR1("Invalid connection parameters; reply with reason=INVALID_LMP_PARAMS, handle=%u", handle);
    return;
  }

  if ((pCtx->connParam.connIntervalMin                 != pCtx->connInterval) ||    /* TODO compare to original conn min/max? */
      (pCtx->connParam.connIntervalMax                 != pCtx->connInterval) ||
      (pCtx->connParam.connLatency                     != pCtx->maxLatency)   ||
      (LCTR_CONN_IND_TO_MS(pCtx->connParam.supTimeout) != pCtx->supTimeoutMs))
  {
    LlRemConnParamInd_t evt =
    {
      .hdr =
      {
        .param         = handle,
        .event         = LL_REM_CONN_PARAM_IND,
        .status        = LL_SUCCESS
      },

      .handle          = handle,
      .connIntervalMin = pCtx->connParam.connIntervalMin,
      .connIntervalMax = pCtx->connParam.connIntervalMax,
      .connLatency     = pCtx->connParam.connLatency,
      .supTimeout      = pCtx->connParam.supTimeout
    };

    LL_TRACE_INFO1("### LlEvent ###  LL_REM_CONN_PARAM_IND, handle=%u, status=LL_SUCCESS", handle);

    bool_t evtSent = LmgrSendEvent((LlEvt_t *)&evt);

    if (!evtSent)
    {
      LlRemoteConnParamReqNegReply(handle, LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE);
      LL_TRACE_WARN1("Host event masked; reply with reason=UNSUPPORTED_REMOTE_FEATURE, handle=%u", handle);
    }
    return;
  }

  /* If there is no change to the connection parameters, respond positively. */
  LlRemoteConnParamReqReply(handle, &connSpec);
}

/*************************************************************************************************/
/*!
 *  \brief      Store local data length parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreLocalDataLength(lctrConnCtx_t *pCtx)
{
  /* Store local parameters */
  pCtx->localDataPdu.maxTxLen = pLctrConnMsg->dataLenChange.maxTxLen;
  pCtx->localDataPdu.maxTxTime = pLctrConnMsg->dataLenChange.maxTxTime;

#if (LL_ENABLE_TESTER)
  if (llTesterCb.dataLenReqEnabled)
  {
    pCtx->localDataPdu = llTesterCb.dataLenReq;
    llTesterCb.dataLenReqEnabled = FALSE;
  }
#endif

  /* Update connection event duration. */
  pCtx->localConnDurUsec = lctrCalcConnDurationUsec(pCtx, &pCtx->localDataPdu);
}

/*************************************************************************************************/
/*!
 *  \brief      Send data length request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 *  \param      opcode  PDU opcode.
 */
/*************************************************************************************************/
static void lctrSendDataLengthPdu(lctrConnCtx_t *pCtx, uint8_t opcode)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_DATA_LEN_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;


    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM (pBuf, opcode);

    uint16_t maxRxTime = pCtx->localDataPdu.maxRxTime;
    uint16_t maxTxTime = pCtx->localDataPdu.maxTxTime;

    /* If LL_FEAT_LE_CODED_PHY is not supported, maxRxTime and maxTxTime can not be more than 2128.*/
    if (!pCtx->featExchFlag ||
        !(pCtx->usedFeatSet & LL_FEAT_LE_CODED_PHY))
    {
      maxRxTime = WSF_MIN(pCtx->localDataPdu.maxRxTime, LL_MAX_DATA_TIME_ABS_MAX_1M);
      maxTxTime = WSF_MIN(pCtx->localDataPdu.maxTxTime, LL_MAX_DATA_TIME_ABS_MAX_1M);
    }
    else
    {
      if (pCtx->bleData.chan.rxPhy == BB_PHY_BLE_CODED)
      {
        maxRxTime = WSF_MAX(pCtx->localDataPdu.maxRxTime, LL_MAX_DATA_TIME_ABS_MIN_CODED);
      }
      if (pCtx->bleData.chan.txPhy == BB_PHY_BLE_CODED)
      {
        maxTxTime = WSF_MAX(pCtx->localDataPdu.maxTxTime, LL_MAX_DATA_TIME_ABS_MIN_CODED);
      }
    }

    UINT16_TO_BSTREAM(pBuf, pCtx->localDataPdu.maxRxLen);
    UINT16_TO_BSTREAM(pBuf, maxRxTime);
    UINT16_TO_BSTREAM(pBuf, pCtx->localDataPdu.maxTxLen);
    UINT16_TO_BSTREAM(pBuf, maxTxTime);

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send data length request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendDataLengthReq(lctrConnCtx_t *pCtx)
{
  lctrSendDataLengthPdu(pCtx, LL_PDU_LENGTH_REQ);
}

/*************************************************************************************************/
/*!
 *  \brief      Send data length response PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendDataLengthRsp(lctrConnCtx_t *pCtx)
{
  lctrSendDataLengthPdu(pCtx, LL_PDU_LENGTH_RSP);
}

/*************************************************************************************************/
/*!
 *  \brief      Store remote data length parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreRemoteDataLength(lctrConnCtx_t *pCtx)
{
  /* Check for valid parameters; allow parameters greater than known limits for compatibility. */
  if ((lctrDataPdu.pld.lenReq.maxTxLen < LL_MAX_DATA_LEN_MIN) ||
      (lctrDataPdu.pld.lenReq.maxRxLen < LL_MAX_DATA_LEN_MIN) ||
      (lctrDataPdu.pld.lenReq.maxTxTime < LL_MAX_DATA_TIME_MIN) ||
      (lctrDataPdu.pld.lenReq.maxRxTime < LL_MAX_DATA_TIME_MIN))
  {
    LL_TRACE_WARN0("Received invalid parameters in LENGTH_PDU");
    return;
  }

  lctrDataLen_t oldEffDataPdu = pCtx->effDataPdu;

  uint16_t maxRxTime = pCtx->localDataPdu.maxRxTime;
  uint16_t maxTxTime = pCtx->localDataPdu.maxTxTime;

  /* If LL_FEAT_LE_CODED_PHY is not supported, maxRxTime and maxTxTime can not be more than 2128. */
  if (!pCtx->featExchFlag ||
      !(pCtx->usedFeatSet & LL_FEAT_LE_CODED_PHY))
  {
    maxRxTime = WSF_MIN(pCtx->localDataPdu.maxRxTime, LL_MAX_DATA_TIME_ABS_MAX_1M);
    maxTxTime = WSF_MIN(pCtx->localDataPdu.maxTxTime, LL_MAX_DATA_TIME_ABS_MAX_1M);
  }

  /* Compute effective values */
  pCtx->effDataPdu.maxTxLen  = WSF_MIN(pCtx->localDataPdu.maxTxLen,  lctrDataPdu.pld.lenReq.maxRxLen);
  pCtx->effDataPdu.maxRxLen  = WSF_MIN(pCtx->localDataPdu.maxRxLen,  lctrDataPdu.pld.lenReq.maxTxLen);
  pCtx->effDataPdu.maxTxTime = WSF_MIN(maxTxTime, lctrDataPdu.pld.lenReq.maxRxTime);
  pCtx->effDataPdu.maxRxTime = WSF_MIN(maxRxTime, lctrDataPdu.pld.lenReq.maxTxTime);

  /* connEffectiveMaxRxTimeCoded - the greater of 2704 and connEffectiveMaxRxTimeUncoded. */
  if (pCtx->bleData.chan.rxPhy == BB_PHY_BLE_CODED)
  {
    pCtx->effDataPdu.maxRxTime = WSF_MAX(pCtx->effDataPdu.maxRxTime, LL_MAX_DATA_TIME_ABS_MIN_CODED);
  }

  if (pCtx->bleData.chan.txPhy == BB_PHY_BLE_CODED)
  {
    pCtx->effDataPdu.maxTxTime = WSF_MAX(pCtx->effDataPdu.maxTxTime, LL_MAX_DATA_TIME_ABS_MIN_CODED);
  }

  if ((oldEffDataPdu.maxTxLen  != pCtx->effDataPdu.maxTxLen) ||
      (oldEffDataPdu.maxRxLen  != pCtx->effDataPdu.maxRxLen) ||
      (oldEffDataPdu.maxTxTime != pCtx->effDataPdu.maxTxTime) ||
      (oldEffDataPdu.maxRxTime != pCtx->effDataPdu.maxRxTime))
  {
    lctrNotifyHostDataLengthInd(pCtx, LL_SUCCESS);
  }
  pCtx->effConnDurUsec = lctrCalcConnDurationUsec(pCtx, &pCtx->effDataPdu);

  LL_TRACE_INFO2("Effective data lengths maxTxLen=%u, maxRxLen=%u", pCtx->effDataPdu.maxTxLen, pCtx->effDataPdu.maxRxLen);
  LL_TRACE_INFO2("Effective data times maxTxTime=%u, maxRxTime=%u", pCtx->effDataPdu.maxTxTime, pCtx->effDataPdu.maxRxTime);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of data length change indication.
 *
 *  \param      pCtx    Connection context.
 *  \param      status  Status.
 */
/*************************************************************************************************/
void lctrNotifyHostDataLengthInd(lctrConnCtx_t *pCtx, uint8_t status)
{
  const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  LlDataLenChangeInd_t evt =
  {
    .hdr =
    {
      .param         = handle,
      .event         = LL_DATA_LEN_CHANGE_IND,
      .status        = status
    },

    .handle          = handle,
  };

  evt.maxTxLen     = pCtx->effDataPdu.maxTxLen;
  evt.maxTxTime    = pCtx->effDataPdu.maxTxTime;
  evt.maxRxLen     = pCtx->effDataPdu.maxRxLen;
  evt.maxRxTime    = pCtx->effDataPdu.maxRxTime;

  LL_TRACE_INFO2("### LlEvent ###  LL_DATA_LEN_CHANGE_IND, handle=%u, status=%u", handle, status);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send set minimum number of used channels indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
static void lctrSendSetMinUsedChanPdu(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_MIN_USED_CHAN_PDU_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM (pBuf, LL_PDU_MIN_USED_CHAN_IND);

    UINT8_TO_BSTREAM(pBuf, pLctrConnMsg->setMinUsedChan.phys);
    UINT8_TO_BSTREAM(pBuf, pLctrConnMsg->setMinUsedChan.minUsedChan);

    /*** Queue for transmit. ***/
    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send set minimum number of used channels indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendSetMinUsedChanInd(lctrConnCtx_t *pCtx)
{
  lctrSendSetMinUsedChanPdu(pCtx);

  /* The procedure completes after sending out the indication. */
  pCtx->llcpNotifyMask &= ~(1 << LCTR_PROC_CMN_SET_MIN_USED_CHAN);
  lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_PROC_CMPL);
}

/*************************************************************************************************/
/*!
 *  \brief      Store remote minimum number of used channels parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStoreSetMinUsedChan(lctrConnCtx_t *pCtx)
{
  for (unsigned i = 0; i < LL_MAX_PHYS; i++)
  {
    /* Update the PHYs which are set. */
    if ((lctrDataPdu.pld.minUsedChanInd.phys & (1 << i)) == (1 << i))
    {
      pCtx->peerMinUsedChan[i] = lctrDataPdu.pld.minUsedChanInd.minUsedChan;
    }
  }

  /* The procedure completes after receiving the indication. */
  lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_PROC_CMPL);
}

/*************************************************************************************************/
/*!
 *  \brief      Send peer SCA request PDU.
 *
 *  \param      pCtx    Connection context.
 *  \param      opcode  PDU opcode.
 */
/*************************************************************************************************/
static void lctrSendPeerScaReqPdu(lctrConnCtx_t *pCtx, uint8_t opcode)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_PEER_SCA_REQ_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/
    UINT8_TO_BSTREAM (pBuf, opcode);
    UINT8_TO_BSTREAM (pBuf, lctrComputeConnSca(pCtx));

    /*** Queue for transmit. ***/
    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Update action for sca processing.
 *
 *  \param      pCtx    Connection Context.
 */
/*************************************************************************************************/
void lctrStoreScaAction(lctrConnCtx_t *pCtx)
{
  pCtx->scaUpdAction = pLctrConnMsg->scaReq.action;
}

/*************************************************************************************************/
/*!
 *  \brief      Send peer SCA request.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendPeerScaReq(lctrConnCtx_t *pCtx)
{
  switch (pCtx->scaUpdAction)
  {
    /* Update by syncing instead of increment/decrement to prevent desync of the SCA value. */
    case LL_MODIFY_SCA_MORE_ACCURATE:
      if (pCtx->scaMod < lmgrCb.scaMod)
      {
        pCtx->scaMod = lmgrCb.scaMod;
      }
      break;
    case LL_MODIFY_SCA_LESS_ACCURATE:
      if (pCtx->scaMod > lmgrCb.scaMod)
      {
        pCtx->scaMod = lmgrCb.scaMod;
      }
      break;

    default: /* LL_MODIFY_SCA_NO_ACTION */
      /* This happens when we are sending a tester REQ. */
      break;
  }

  lctrSendPeerScaReqPdu(pCtx, LL_PDU_PEER_SCA_REQ);
}

/*************************************************************************************************/
/*!
 *  \brief      Send peer SCA response PDU.
 *
 *  \param      pCtx    Connection context.
 *  \param      opcode  PDU opcode.
 */
/*************************************************************************************************/
static void lctrSendPeerScaRspPdu(lctrConnCtx_t *pCtx, uint8_t opcode)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_PEER_SCA_RSP_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/
    UINT8_TO_BSTREAM (pBuf, opcode);
    UINT8_TO_BSTREAM (pBuf, lctrComputeConnSca(pCtx));

    /*** Queue for transmit. ***/
    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send peer SCA response.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendPeerScaRsp(lctrConnCtx_t *pCtx)
{
  lctrSendPeerScaRspPdu(pCtx, LL_PDU_PEER_SCA_RSP);
}

/*************************************************************************************************/
/*!
 *  \brief      Store peer SCA.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStorePeerSca(lctrConnCtx_t *pCtx)
{
  pCtx->peerSca = lctrDataPdu.pld.peerSca.sca;

  if (pCtx->role == LL_ROLE_SLAVE)
  {
    pCtx->data.slv.totalAcc = lctrCalcTotalAccuracy(pCtx->peerSca);

    LL_TRACE_INFO1("lctrStorePeerSca pCtx->data.slv.totalAcc=%d", pCtx->data.slv.totalAcc);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of peer SCA request confirmation.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrNotifyHostPeerScaCnf(lctrConnCtx_t *pCtx)
{
  const uint16_t handle = LCTR_GET_CONN_HANDLE(pCtx);

  LlPeerScaCnf_t evt =
  {
    .hdr =
    {
      .param        = handle,
      .event        = LL_REQ_PEER_SCA_IND,
      .status       = LL_SUCCESS
    },

    .status         = LL_SUCCESS,
    .connHandle     = handle,
    .peerSca        = pCtx->peerSca,
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_REQ_PEER_SCA_CNF, handle=%u, status=LL_SUCCESS", handle);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send unknown response PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrSendUnknownRsp(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_UNKNOWN_RSP_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, LL_PDU_UNKNOWN_RSP);
    UINT8_TO_BSTREAM(pBuf, lctrDataPdu.opcode);

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send reject indication PDU to peer in response to an unacceptable PDU.
 *
 *  \param      pCtx                Connection context.
 *  \param      reason              Reason code.
 *  \param      forceRejectExtInd   TRUE to force using LL_REJECT_EXT_IND.
 */
/*************************************************************************************************/
void lctrSendRejectInd(lctrConnCtx_t *pCtx, uint8_t reason, bool_t forceRejectExtInd)
{
  uint8_t *pPdu;
  bool_t   rejectExtInd;

  rejectExtInd = forceRejectExtInd ||
                ((pCtx->usedFeatSet & LL_FEAT_EXT_REJECT_IND) &&
                 pCtx->featExchFlag);                /* only use extended if peer supports it */

  if (rejectExtInd)
  {
    if ((pPdu = lctrTxCtrlPduAlloc(LL_REJECT_EXT_IND_PDU_LEN)) != NULL)
    {
      uint8_t *pBuf = pPdu;

      /*** Assemble control PDU. ***/

      UINT8_TO_BSTREAM(pBuf, LL_PDU_REJECT_EXT_IND);
      UINT8_TO_BSTREAM(pBuf, lctrDataPdu.opcode);
      UINT8_TO_BSTREAM(pBuf, reason);

      /*** Queue for transmit. ***/

      lctrTxCtrlPduQueue(pCtx, pPdu);
    }
  }
  else
  {
    if ((pPdu = lctrTxCtrlPduAlloc(LL_REJECT_IND_PDU_LEN)) != NULL)
    {
      uint8_t *pBuf = pPdu;

      /*** Assemble control PDU. ***/

      UINT8_TO_BSTREAM(pBuf, LL_PDU_REJECT_IND);
      UINT8_TO_BSTREAM(pBuf, reason);

      /*** Queue for transmit. ***/

      lctrTxCtrlPduQueue(pCtx, pPdu);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Start LLCP timer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStartLlcpTimer(lctrConnCtx_t *pCtx)
{
  if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_LLCP_TIMER))
  {
    if (pCtx->llcpNotifyMask & (1 << LCTR_PROC_CMN_TERM))
    {
      WsfTimerStartMs(&pCtx->tmrProcRsp, pCtx->supTimeoutMs);
    }
    else
    {
      WsfTimerStartSec(&pCtx->tmrProcRsp, LL_T_PRT_SEC);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Stop LLCP timer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStopLlcpTimer(lctrConnCtx_t *pCtx)
{
  if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_LLCP_TIMER))
  {
    WsfTimerStop(&pCtx->tmrProcRsp);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Start pending LLCP procedure.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrStartPendingLlcp(lctrConnCtx_t *pCtx)
{
  lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_START_PENDING);
}

/*************************************************************************************************/
/*!
 *  \brief      Pause Tx data PDUs.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrPauseTxData(lctrConnCtx_t *pCtx)
{
  pCtx->pauseTxData = TRUE;
  LL_TRACE_INFO1("    >>> Data Path Tx Paused, handle=%u <<<", LCTR_GET_CONN_HANDLE(pCtx));
}

/*************************************************************************************************/
/*!
 *  \brief      Unpause Tx data PDUs.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrUnpauseTxData(lctrConnCtx_t *pCtx)
{
  pCtx->pauseTxData = FALSE;
  LL_TRACE_INFO1("    >>> Data Path Tx Unpaused/Resumed, handle=%u <<<", LCTR_GET_CONN_HANDLE(pCtx));

  /*** Queue any pending LE-U data for transmit. ***/

  if (!WsfQueueEmpty(&pCtx->txLeuQ))
  {
    wsfHandlerId_t connHandle;
    uint8_t *pAclBuf;
    uint16_t fragLen = lctrTxFragLen(pCtx);

    while ((pAclBuf = WsfMsgDeq(&pCtx->txLeuQ, &connHandle)) != NULL)
    {
      lctrAclHdr_t aclHdr;

      /*** Disassemble ACL packet. ***/

      lctrUnpackAclHdr(&aclHdr, pAclBuf);
      lctrTxDataPduQueue(pCtx, fragLen, &aclHdr, pAclBuf);
    }

    WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_TX_PENDING));
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Check if Tx data pending.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrCheckPauseComplete(lctrConnCtx_t *pCtx)
{
  if (WsfQueueEmpty(&pCtx->txArqQ))
  {
    lctrSendConnMsg(pCtx, LCTR_CONN_ARQ_Q_FLUSHED);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Pause Rx data PDUs.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrPauseRxData(lctrConnCtx_t *pCtx)
{
  pCtx->pauseRxData = TRUE;
  LL_TRACE_INFO1("    >>> Data Path Rx Paused, handle=%u <<<", LCTR_GET_CONN_HANDLE(pCtx));
}

/*************************************************************************************************/
/*!
 *  \brief      Unpause Rx data PDUs.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrUnpauseRxData(lctrConnCtx_t *pCtx)
{
  pCtx->pauseRxData = FALSE;
  LL_TRACE_INFO1("    >>> Data Path Rx Unpaused/Resumed, handle=%u <<<", LCTR_GET_CONN_HANDLE(pCtx));
}

/*************************************************************************************************/
/*!
 *  \brief      Store periodic advertising sync transfer parameters.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrActStorePeriodicSyncTrsf(lctrConnCtx_t *pCtx)
{
  if (lctrStorePeriodicSyncTrsfFn)
  {
    lctrStorePeriodicSyncTrsfFn(pCtx);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic sync indication PDU to peer.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrActSendPeriodicSyncInd(lctrConnCtx_t *pCtx)
{
  if (lctrSendPeriodicSyncIndFn)
  {
    lctrSendPeriodicSyncIndFn(pCtx);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Handle received periodic sync indication PDU.
 *
 *  \param      pCtx    Connection context.
 */
/*************************************************************************************************/
void lctrActReceivePeriodicSyncInd(lctrConnCtx_t *pCtx)
{
  if (lctrReceivePeriodicSyncIndFn)
  {
    lctrReceivePeriodicSyncIndFn(pCtx);
  }
}

