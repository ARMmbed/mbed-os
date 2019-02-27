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
 *  \brief L2CAP connection oriented channel module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_timer.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "wsf_os.h"
#include "util/bstream.h"
#include "l2c_api.h"
#include "l2c_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Channel states */
enum
{
  L2C_CHAN_STATE_UNUSED,
  L2C_CHAN_STATE_CONNECTING,
  L2C_CHAN_STATE_CONNECTED,
  L2C_CHAN_STATE_DISCONNECTING
};

/* Event handler messages */
enum
{
  /* messages from API */
  L2C_MSG_API_DATA_REQ,
  L2C_MSG_API_CONNECT_REQ,
  L2C_MSG_API_DISCONNECT_REQ,

  /* messages from timers */
  L2C_MSG_COC_REQ_TIMEOUT
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data type for L2C_MSG_API_DATA_REQ */
typedef struct
{
  wsfMsgHdr_t       hdr;
  uint8_t           *pPacket;
  uint16_t          localCid;
  uint16_t          sduLen;
} l2cApiDataReq_t;


/* Data type for L2C_MSG_API_CONNECT_REQ */
typedef struct
{
  wsfMsgHdr_t       hdr;
  uint16_t          localCid;
  uint16_t          remotePsm;
} l2cApiConnectReq_t;

/* Data type for event handler messages */
typedef union
{
  wsfMsgHdr_t         hdr;
  l2cApiDataReq_t     dataReq;
  l2cApiConnectReq_t  connectReq;
} l2cCocMsg_t;

/* Registration control block */
typedef struct
{
  l2cCocReg_t       reg;                  /* Registration data structure */
  l2cCocCback_t     cback;                /* Client callback */
  l2cCocRegId_t     regId;                /* Registration instance ID */
  bool_t            inUse;                /* TRUE if in use */
} l2cRegCb_t;

/* connection control block */
typedef struct
{
  uint16_t          handle;
  dmConnId_t        connId;
  bool_t            flowDisabled;
} l2cConnCb_t;

/* channel control block */
typedef struct
{
  l2cRegCb_t        *pRegCb;              /* Pointer to associated registration control block */
  l2cConnCb_t       *pConnCb;             /* Pointer to associated connection control block */
  wsfTimer_t        reqTimer;             /* Signaling request timeout timer */
  uint8_t           *pTxPkt;              /* Pointer to tx packet in progress */
  uint8_t           *pRxPkt;              /* Pointer to rx packet in progress */
  uint16_t          txTotalLen;           /* Total length of tx data */
  uint16_t          txCurrLen;            /* Current length of tx data */
  uint16_t          rxTotalLen;           /* Total length of rx data */
  uint16_t          rxCurrLen;            /* Current length of rx data */
  uint16_t          psm;
  uint16_t          peerMps;
  uint16_t          peerMtu;
  uint16_t          peerCredits;
  uint16_t          peerCid;
  uint16_t          localCredits;
  uint16_t          localCid;
  uint8_t           state;
  uint8_t           role;
  uint8_t           identifier;
} l2cChanCb_t;

/* main control block */
typedef struct
{
  l2cRegCb_t          regCb[L2C_COC_REG_MAX];   /* Registration control blocks */
  l2cConnCb_t         connCb[DM_CONN_MAX];      /* Connection control blocks */
  l2cChanCb_t         chanCb[L2C_COC_CHAN_MAX]; /* Channel control blocks */
  l2cCocAuthorCback_t authorCback;              /* Authorization callback */
  wsfHandlerId_t      handlerId;                /* WSF handler ID */
  uint16_t            errTest;                  /* Status code for error testing */
} l2cCocCb_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

l2cCocCb_t l2cCocCb;

/*************************************************************************************************/
/*!
 *  \brief  Allocate a registration control block.
 *
 *  \param  cback   Client callback function.
 *  \param  pReg    Registration data.
 *
 *  \return Pointer to control block or NULL if failure.
 */
/*************************************************************************************************/
l2cRegCb_t *l2cRegCbAlloc(l2cCocCback_t cback, l2cCocReg_t *pReg)
{
  l2cRegCb_t    *pCb = l2cCocCb.regCb;
  uint8_t       i;

  for (i = 0; i < L2C_COC_REG_MAX; i++, pCb++)
  {
    if (pCb->inUse == FALSE)
    {
      memcpy(&pCb->reg, pReg, sizeof(l2cCocReg_t));
      pCb->cback = cback;
      pCb->regId = i + 1;
      pCb->inUse = TRUE;

      /* if MPS is larger than maximum RX PDU length */
      if (pReg->mps > (HciGetMaxRxAclLen() - L2C_HDR_LEN))
      {
        l2cCocEvt_t evt;

        evt.hdr.param = 0;
        evt.hdr.event = DM_ERROR_IND;
        evt.hdr.status = DM_ERR_L2C_RX_PDU_LEN_EXCEEDED;

        (*pCb->cback)(&evt);
      }

      L2C_TRACE_INFO1("l2cRegCbAlloc %d", i+1);

      return pCb;
    }
  }

  DM_TRACE_ERR0("l2cRegCbAlloc failed");

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Find a registration control block accepting the given PSM.
 *
 *  \param  psm   The PSM.
 *
 *  \return Pointer to control block or NULL if not found.
 */
/*************************************************************************************************/
l2cRegCb_t *l2cRegCbAccept(dmConnId_t connId, uint16_t psm)
{
  l2cRegCb_t    *pRegCb = l2cCocCb.regCb;
  uint8_t       i;

  /* find acceptor control block with matching psm */
  for (i = 0; i < L2C_COC_REG_MAX; i++, pRegCb++)
  {
    if (pRegCb->inUse && (pRegCb->reg.role & L2C_COC_ROLE_ACCEPTOR) && (pRegCb->reg.psm == psm))
    {
      /* psm found */
      return pRegCb;
    }
  }

  /* psm not found */
  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Check if peer CID is already allocated to the same psm.
 *
 *  \param  connId  Connection ID.
 *  \param  peerCid Peer CID.
 *  \param  pRegCb  Registration control block.
 *
 *  \return TRUE if peer CID already allocated, otherwise FALSE.
 */
/*************************************************************************************************/
bool_t l2cCheckPeerCid(dmConnId_t connId, uint16_t peerCid, l2cRegCb_t *pRegCb)
{
  l2cChanCb_t   *pChanCb;
  uint8_t       i;

  /* make sure CID is not already allocated to same psm */
  for (i = 0, pChanCb = l2cCocCb.chanCb; i < L2C_COC_CHAN_MAX; i++, pChanCb++)
  {
    if ((pChanCb->state != L2C_CHAN_STATE_UNUSED) && (pChanCb->pRegCb == pRegCb)              &&
        (pChanCb->pConnCb->connId == connId)      && (pChanCb->role == L2C_COC_ROLE_ACCEPTOR) &&
        (pChanCb->peerCid == peerCid))
    {
      /* peer CID found */
      L2C_TRACE_WARN3("Peer CID in use psm:0x%04x connId:%d peerCid", pRegCb->reg.psm, connId, peerCid);
      return TRUE;
    }
  }

  /* peer CID not found */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Look up a connection control block by connection ID.
 *
 *  \param  connId  DM Connection ID.
 *
 *  \return Pointer to connection control block.
 */
/*************************************************************************************************/
l2cConnCb_t *l2cConnCbById(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return &l2cCocCb.connCb[connId-1];
}

/*************************************************************************************************/
/*!
 *  \brief  Get a registration control block by ID.
 *
 *  \param  regId    Registration ID.
 *
 *  \return Pointer to control block.
 */
/*************************************************************************************************/
l2cRegCb_t *l2cRegCbById(l2cCocRegId_t regId)
{
  WSF_ASSERT((regId > 0) && (regId <= L2C_COC_REG_MAX));

  regId--;
  return &l2cCocCb.regCb[regId];
}

/*************************************************************************************************/
/*!
 *  \brief  Deallocate a registration control block.
 *
 *  \param  regId    Registration ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cRegCbDealloc(l2cCocRegId_t regId)
{
  WSF_ASSERT((regId > 0) && (regId <= L2C_COC_REG_MAX));

  regId--;
  l2cCocCb.regCb[regId].inUse = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a registration control block.
 *
 *  \param  state   Initial channel state, either L2C_CHAN_STATE_CONNECTING or
 *                  L2C_CHAN_STATE_CONNECTED.
 *
 *  \return Pointer to control block or NULL if failure.
 */
/*************************************************************************************************/
l2cChanCb_t *l2cChanCbAlloc(uint8_t state)
{
  l2cChanCb_t   *pCb = l2cCocCb.chanCb;
  uint8_t       i;

  for (i = 0; i < L2C_COC_CHAN_MAX; i++, pCb++)
  {
    if (pCb->state == L2C_CHAN_STATE_UNUSED)
    {
      pCb->state = state;
      pCb->reqTimer.msg.param = pCb->localCid = i + L2C_CID_DYN_MIN;
      pCb->reqTimer.msg.event = L2C_MSG_COC_REQ_TIMEOUT;
      L2C_TRACE_INFO1("l2cChanCbAlloc cid=0x%04x", pCb->localCid);

      return pCb;
    }
  }

  L2C_TRACE_ERR0("l2cChanCbAlloc failed");

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Get a channel control block by ID.
 *
 *  \param  cid     Local channel ID.
 *
 *  \return Pointer to control block.
 */
/*************************************************************************************************/
l2cChanCb_t *l2cChanCbByCid(uint16_t cid)
{
  WSF_ASSERT(cid >= L2C_CID_DYN_MIN && cid < (L2C_CID_DYN_MIN + L2C_COC_CHAN_MAX));

  return &l2cCocCb.chanCb[cid - L2C_CID_DYN_MIN];
}

/*************************************************************************************************/
/*!
 *  \brief  Get a channel control block with matching CID, checking CID range.
 *
 *  \param  cid     Local channel ID.
 *
 *  \return Pointer to control block or NULL if not found.
 */
/*************************************************************************************************/
l2cChanCb_t *l2cChanCbByCidCheck(uint16_t cid)
{
  if (cid >= L2C_CID_DYN_MIN && cid < (L2C_CID_DYN_MIN + L2C_COC_CHAN_MAX))
  {
    return &l2cCocCb.chanCb[cid - L2C_CID_DYN_MIN];
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Get a channel control block with matching CID and state.
 *
 *  \param  cid     Local channel ID.
 *  \param  state   Channel state;
 *
 *  \return Pointer to control block or NULL if not found.
 */
/*************************************************************************************************/
l2cChanCb_t *l2cChanCbByCidState(uint16_t cid, uint8_t state)
{
  l2cChanCb_t   *pCb;

  if ((pCb = l2cChanCbByCidCheck(cid)) != NULL)
  {
    if (pCb->state == state)
    {
      return pCb;
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Find a matching channel control block in connecting state.
 *
 *  \param  connId      Connection ID.
 *  \param  identifier  Identifier value in sent connect request.
 *
 *  \return Pointer to control block or NULL if not found.
 */
/*************************************************************************************************/
l2cChanCb_t *l2cChanCbConnecting(dmConnId_t connId, uint8_t identifier)
{
  l2cChanCb_t   *pCb = l2cCocCb.chanCb;
  uint8_t       i;

  for (i = 0; i < L2C_COC_CHAN_MAX; i++, pCb++)
  {
    if (pCb->state == L2C_CHAN_STATE_CONNECTING &&
        pCb->identifier == identifier &&
        pCb->pConnCb->connId == connId)
    {
      return pCb;
    }
  }

  L2C_TRACE_INFO2("l2cChanCbConnecting not found connId=%d identifier=%d", connId, identifier);

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Deallocate a channel control block.
 *
 *  \param  pChanCb     Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cChanCbDealloc(l2cChanCb_t *pCb)
{
  L2C_TRACE_INFO1("l2cChanCbDealloc cid=0x%04x", pCb->localCid);

  pCb->state = L2C_CHAN_STATE_UNUSED;
  WsfTimerStop(&pCb->reqTimer);
  if (pCb->pRxPkt != NULL)
  {
    WsfMsgFree(pCb->pRxPkt);
    pCb->pRxPkt = NULL;
  }
  if (pCb->pTxPkt != NULL)
  {
    WsfMsgFree(pCb->pTxPkt);
    pCb->pTxPkt = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Check security requirements.
 *
 *  \param  connId  Connection ID.
 *  \param  pRegCb  Registration control block.
 *
 *  \return L2C_CONN_SUCCESS if security ok otherwise failure.
 */
/*************************************************************************************************/
uint16_t l2cCheckSecurity(dmConnId_t connId, l2cRegCb_t *pRegCb)
{
  uint16_t result;

  if (DmConnSecLevel(connId) >= pRegCb->reg.secLevel)
  {
    if (pRegCb->reg.authoriz)
    {
      if (l2cCocCb.authorCback != NULL)
      {
        result = (*l2cCocCb.authorCback)(connId, pRegCb->regId, pRegCb->reg.psm);
      }
      else
      {
        result = L2C_CONN_FAIL_AUTHORIZ;
      }
    }
    else
    {
      result = L2C_CONN_SUCCESS;
    }
  }
  else
  {
    result = L2C_CONN_FAIL_AUTH;
  }
  return result;
}

/*************************************************************************************************/
/*!
 *  \brief  Execute client callback with connect indication.
 *
 *  \param  pChanCb     Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cConnectInd(l2cChanCb_t *pChanCb)
{
  l2cCocConnectInd_t  connectInd;

  connectInd.hdr.event = L2C_COC_CONNECT_IND;
  connectInd.hdr.param = pChanCb->pConnCb->connId;
  connectInd.cid = pChanCb->localCid;
  connectInd.peerMtu = pChanCb->peerMtu;
  connectInd.psm = pChanCb->psm;
  (*pChanCb->pRegCb->cback)((l2cCocEvt_t *) &connectInd);
}

/*************************************************************************************************/
/*!
 *  \brief  Execute client callback with disconnect indication and deallocate the channel.
 *
 *  \param  pChanCb     Channel control block.
 *  \param  result      Connection result code.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDisconnectIndAndDealloc(l2cChanCb_t *pChanCb, uint16_t result)
{
  l2cCocDisconnectInd_t  disconnectInd;
  l2cCocCback_t          cback = pChanCb->pRegCb->cback;

  disconnectInd.hdr.event = L2C_COC_DISCONNECT_IND;
  disconnectInd.hdr.param = pChanCb->pConnCb->connId;
  disconnectInd.cid = pChanCb->localCid;
  disconnectInd.result = result;

  l2cChanCbDealloc(pChanCb);

  (*cback)((l2cCocEvt_t *) &disconnectInd);
}

/*************************************************************************************************/
/*!
 *  \brief  Execute client callback with data confirm.
 *
 *  \param  pChanCb     Channel control block.
 *  \param  status      Data confirm status.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDataCnf(l2cChanCb_t *pChanCb, uint8_t status)
{
  l2cCocDataCnf_t dataCnf;

  dataCnf.hdr.event = L2C_COC_DATA_CNF;
  dataCnf.hdr.status = status;
  dataCnf.hdr.param = pChanCb->pConnCb->connId;
  dataCnf.cid = pChanCb->localCid;
  (*pChanCb->pRegCb->cback)((l2cCocEvt_t *) &dataCnf);
}

/*************************************************************************************************/
/*!
 *  \brief  Deallocate all channel control block on the given connection.
 *
 *  \param  connId  DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cChanCbDeallocAll(dmConnId_t connId)
{
  l2cChanCb_t   *pCb = l2cCocCb.chanCb;
  uint8_t       i;

  for (i = 0; i < L2C_COC_CHAN_MAX; i++, pCb++)
  {
    if (pCb->state != L2C_CHAN_STATE_UNUSED && pCb->pConnCb->connId == connId)
    {
      l2cDisconnectIndAndDealloc(pCb, L2C_CONN_NONE);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send a flow control credit.
 *
 *  \param  handle      The connection handle.
 *  \param  cid         Local CID.
 *  \param  credits     Credits.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cSendFlowControlCredit(uint16_t handle, uint16_t cid, uint16_t credits)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_FLOW_CTRL_CREDIT_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_FLOW_CTRL_CREDIT);        /* command code */
    UINT8_TO_BSTREAM(p, l2cCb.identifier);                /* identifier */
    l2cCb.identifier = L2C_NEXT_ID(l2cCb.identifier);
    UINT16_TO_BSTREAM(p, L2C_SIG_FLOW_CTRL_CREDIT_LEN);   /* parameter length */
    UINT16_TO_BSTREAM(p, cid);                            /* cid */
    UINT16_TO_BSTREAM(p, credits);                        /* credit */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_FLOW_CTRL_CREDIT_LEN), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send an LE connection request.
 *
 *  \param  handle      The connection handle.
 *  \param  psm         Remote PSM.
 *  \param  pChanCb     Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cSendConnectReq(uint16_t handle, uint16_t psm, l2cChanCb_t *pChanCb)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_LE_CONN_REQ_LEN)) != NULL)
  {
    uint16_t mps = WSF_MIN(pChanCb->pRegCb->reg.mps, (HciGetMaxRxAclLen() - L2C_HDR_LEN));

    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_LE_CONNECT_REQ);          /* command code */
    UINT8_TO_BSTREAM(p, l2cCb.identifier);                /* identifier */
    pChanCb->identifier = l2cCb.identifier;
    l2cCb.identifier = L2C_NEXT_ID(l2cCb.identifier);
    UINT16_TO_BSTREAM(p, L2C_SIG_LE_CONN_REQ_LEN);        /* parameter length */
    UINT16_TO_BSTREAM(p, psm);                            /* psm */
    UINT16_TO_BSTREAM(p, pChanCb->localCid);              /* source cid */
    UINT16_TO_BSTREAM(p, pChanCb->pRegCb->reg.mtu);       /* mtu */
    UINT16_TO_BSTREAM(p, mps);                            /* mps */
    UINT16_TO_BSTREAM(p, pChanCb->pRegCb->reg.credits);   /* initial credits */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_LE_CONN_REQ_LEN), pPacket);

    /* start timer */
    WsfTimerStartSec(&pChanCb->reqTimer, pL2cCfg->reqTimeout);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send an LE connection response.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  result      Connection status result.
 *  \param  pChanCb     Channel control block (note: may be NULL).
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cSendConnectRsp(uint16_t handle, uint8_t identifier, uint16_t result, l2cChanCb_t *pChanCb)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_LE_CONN_RSP_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_LE_CONNECT_RSP);          /* command code */
    UINT8_TO_BSTREAM(p, identifier);                      /* identifier */
    UINT16_TO_BSTREAM(p, L2C_SIG_LE_CONN_RSP_LEN);        /* parameter length */
    if (pChanCb != NULL)
    {
      uint16_t mps = WSF_MIN(pChanCb->pRegCb->reg.mps, (HciGetMaxRxAclLen() - L2C_HDR_LEN));

      UINT16_TO_BSTREAM(p, pChanCb->localCid);            /* destination cid */
      UINT16_TO_BSTREAM(p, pChanCb->pRegCb->reg.mtu);     /* mtu */
      UINT16_TO_BSTREAM(p, mps);                          /* mps */
      UINT16_TO_BSTREAM(p, pChanCb->pRegCb->reg.credits); /* initial credits */
    }
    else
    {
      UINT16_TO_BSTREAM(p, 0);
      UINT16_TO_BSTREAM(p, 0);
      UINT16_TO_BSTREAM(p, 0);
      UINT16_TO_BSTREAM(p, 0);
    }
    UINT16_TO_BSTREAM(p, result);                         /* result */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_LE_CONN_RSP_LEN), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send a Disconnect request.
 *
 *  \param  handle      The connection handle.
 *  \param  destCid     Destination (remote) CID.
 *  \param  sourceCid   Source (local) CID.
 *  \param  pChanCb     Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cSendDisconnectReq(uint16_t handle, uint16_t destCid, uint16_t sourceCid, l2cChanCb_t *pChanCb)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_DISCONN_REQ_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_DISCONNECT_REQ);          /* command code */
    UINT8_TO_BSTREAM(p, l2cCb.identifier);                /* identifier */
    l2cCb.identifier = L2C_NEXT_ID(l2cCb.identifier);
    UINT16_TO_BSTREAM(p, L2C_SIG_DISCONN_REQ_LEN);        /* parameter length */
    UINT16_TO_BSTREAM(p, destCid);                        /* destination cid */
    UINT16_TO_BSTREAM(p, sourceCid);                      /* source cid */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_DISCONN_REQ_LEN), pPacket);

    /* start timer */
    WsfTimerStartSec(&pChanCb->reqTimer, pL2cCfg->reqTimeout);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send a Disconnect response.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  destCid     Destination (local) CID.
 *  \param  sourceCid   Source (remote) CID.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cSendDisconnectRsp(uint16_t handle, uint16_t identifier, uint16_t destCid,
                                 uint16_t sourceCid)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_DISCONN_RSP_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_DISCONNECT_RSP);          /* command code */
    UINT8_TO_BSTREAM(p, identifier);                      /* identifier */
    UINT16_TO_BSTREAM(p, L2C_SIG_DISCONN_RSP_LEN);        /* parameter length */
    UINT16_TO_BSTREAM(p, destCid);                        /* destination cid */
    UINT16_TO_BSTREAM(p, sourceCid);                      /* source cid */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_DISCONN_RSP_LEN), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Disconnect a channel.
 *
 *  \param  pChanCb   Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDisconnectChannel(l2cChanCb_t *pChanCb)
{
  pChanCb->state = L2C_CHAN_STATE_DISCONNECTING;

  /* send disconnect req */
  l2cSendDisconnectReq(pChanCb->pConnCb->handle, pChanCb->peerCid, pChanCb->localCid, pChanCb);
}


/*************************************************************************************************/
/*!
 *  \brief  L2CAP data reassembly function.
 *
 *  \param  pChanCb   Channel control block.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet payload.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2CocDataReassemble(l2cChanCb_t *pChanCb, uint16_t len, uint8_t *pPacket)
{
  l2cCocDataInd_t dataInd;
  uint16_t        sduLen;

  /* sanity check payload length */
  if (len == 0)
  {
    return;
  }

  /* set pointer to start of payload */
  pPacket += L2C_PAYLOAD_START;

  /* reassembly not already in progress? */
  if (pChanCb->pRxPkt == NULL)
  {
    /* get sdu len */
    BSTREAM_TO_UINT16(sduLen, pPacket);

    /* verify sdu len not greater than our mtu */
    if (sduLen > pChanCb->pRegCb->reg.mtu)
    {
      /* invalid sdu len; disconnect */
      l2cDisconnectChannel(pChanCb);

      L2C_TRACE_WARN2("sduLen:%d > local mtu:%d", sduLen, pChanCb->pRegCb->reg.mtu);
    }
    /* check sdu len vs. packet payload len */
    else if ((sduLen + L2C_LE_SDU_HDR_LEN) == len)
    {
      /* no reassembly required; send directly up to application */
      dataInd.hdr.event = L2C_COC_DATA_IND;
      dataInd.hdr.param = pChanCb->pConnCb->connId;
      dataInd.cid = pChanCb->localCid;
      dataInd.pData = pPacket;
      dataInd.dataLen = sduLen;
      (*pChanCb->pRegCb->cback)((l2cCocEvt_t *) &dataInd);
    }
    else if ((sduLen + L2C_LE_SDU_HDR_LEN) > len)
    {
      /* reassembly required; allocate reassembly buffer */
      if ((pChanCb->pRxPkt = WsfMsgDataAlloc(sduLen, 0)) != NULL)
      {
        memcpy(pChanCb->pRxPkt, pPacket, (len - L2C_LE_SDU_HDR_LEN));
        pChanCb->rxCurrLen = (len - L2C_LE_SDU_HDR_LEN);
        pChanCb->rxTotalLen = sduLen;

        L2C_TRACE_INFO2("reassembling sduLen:%d  currLen:%d", sduLen, pChanCb->rxCurrLen);
      }
    }
    else
    {
      /* invalid sdu len */
      l2cDisconnectChannel(pChanCb);

      L2C_TRACE_WARN2("rx invalid sduLen:%d payload len:%d", sduLen, len);
    }
  }
  /* else reassembly already in progress */
  else
  {
    /* verify length */
    if ((pChanCb->rxCurrLen + len) <= pChanCb->rxTotalLen)
    {
      memcpy((pChanCb->pRxPkt + pChanCb->rxCurrLen), pPacket, len);
      pChanCb->rxCurrLen += len;

      /* if reassembly complete */
      if (pChanCb->rxCurrLen == pChanCb->rxTotalLen)
      {
        /* call callback */
        dataInd.hdr.event = L2C_COC_DATA_IND;
        dataInd.hdr.param = pChanCb->pConnCb->connId;
        dataInd.cid = pChanCb->localCid;
        dataInd.pData = pChanCb->pRxPkt;
        dataInd.dataLen = pChanCb->rxTotalLen;
        (*pChanCb->pRegCb->cback)((l2cCocEvt_t *) &dataInd);

        /* free reassembly buffer */
        WsfMsgFree(pChanCb->pRxPkt);
        pChanCb->pRxPkt = NULL;
      }
    }
    /* else too much data sent for this sdu */
    else
    {
      l2cDisconnectChannel(pChanCb);

      L2C_TRACE_WARN2("too much data currLen:%d payload len:%d", pChanCb->rxCurrLen, len);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send data and perform segmentation.
 *
 *  \param  pChanCb   Channel control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocSendData(l2cChanCb_t *pChanCb)
{
  uint16_t  len;
  uint8_t   *pBuf;

  L2C_TRACE_INFO3("l2cCocSendData pTxPkt:%x peerCredits:%d flowDisabled:%d", (uint32_t)pChanCb->pTxPkt, pChanCb->peerCredits, pChanCb->pConnCb->flowDisabled);

  /* while we have data and peer credits and flow is not disabled */
  while (pChanCb->pTxPkt != NULL && pChanCb->peerCredits > 0 && !pChanCb->pConnCb->flowDisabled)
  {
    /* determine next packet length */
    len = (pChanCb->peerMps < (pChanCb->txTotalLen - pChanCb->txCurrLen)) ?
          pChanCb->peerMps : (pChanCb->txTotalLen - pChanCb->txCurrLen);

    /* allocate a buffer */
    if ((pBuf = l2cMsgAlloc(len + L2C_PAYLOAD_START)) != NULL)
    {
      /* copy next segment of data to buffer */
      memcpy((pBuf + L2C_PAYLOAD_START),
             (pChanCb->pTxPkt + L2C_PAYLOAD_START + pChanCb->txCurrLen), len);
      pChanCb->txCurrLen += len;

      /* decrement credits */
      pChanCb->peerCredits--;

      /* send packet */
      L2cDataReq(pChanCb->peerCid, pChanCb->pConnCb->handle, len, pBuf);

      /* if this was the last segment free stored tx buffer and call callback */
      if (pChanCb->txCurrLen == pChanCb->txTotalLen)
      {
        WsfMsgFree(pChanCb->pTxPkt);
        pChanCb->pTxPkt = NULL;
        l2cDataCnf(pChanCb, L2C_COC_DATA_SUCCESS);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a flow control credit message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cProcFlowControlCredit(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cChanCb_t         *pChanCb;
  uint16_t            cid;
  uint16_t            credits;

  /* parse parameters */
  BSTREAM_TO_UINT16(cid, pPacket);
  BSTREAM_TO_UINT16(credits, pPacket);

  if ((pChanCb = l2cChanCbByCidState(cid, L2C_CHAN_STATE_CONNECTED)) != NULL)
  {
    if (credits != 0)
    {
      /* check credits for overflow */
      if (((uint32_t)(credits) + (uint32_t) pChanCb->peerCredits) <= L2C_CREDITS_MAX)
      {
        pChanCb->peerCredits += credits;

        /* send any pending data */
        l2cCocSendData(pChanCb);
      }
      /* else disconnect if credit overflow */
      else
      {
        l2cDisconnectChannel(pChanCb);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a connect request message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cProcLeConnectReq(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cRegCb_t          *pRegCb;
  l2cChanCb_t         *pChanCb = NULL;
  uint16_t            cid;
  uint16_t            psm;
  uint16_t            mps;
  uint16_t            mtu;
  uint16_t            credits;
  uint16_t            result;
  dmConnId_t          connId;

  /* parse parameters */
  BSTREAM_TO_UINT16(psm, pPacket);
  BSTREAM_TO_UINT16(cid, pPacket);
  BSTREAM_TO_UINT16(mtu, pPacket);
  BSTREAM_TO_UINT16(mps, pPacket);
  BSTREAM_TO_UINT16(credits, pPacket);

  /* get conn ID for handle */
  if ((connId = DmConnIdByHandle(handle)) == DM_CONN_ID_NONE)
  {
    return;
  }

  /* verify parameters */
  //TBD
  //if fail send command reject, return

  /* check for error test mode */
  if (l2cCocCb.errTest != L2C_CONN_SUCCESS)
  {
    result = l2cCocCb.errTest;
  }
  /* registered to accept connections on this psm? */
  else if ((pRegCb = l2cRegCbAccept(connId, psm)) != NULL)
  {
    /* check if peer CID already allocated to this psm */
    if (!l2cCheckPeerCid(connId, cid, pRegCb))
    {
      /* check security level */
      if ((result = l2cCheckSecurity(connId, pRegCb)) == L2C_CONN_SUCCESS)
      {
        /* allocate channel */
        if ((pChanCb = l2cChanCbAlloc(L2C_CHAN_STATE_CONNECTED)) != NULL)
        {
          /* initialize control block */
          pChanCb->pRegCb = pRegCb;
          pChanCb->pConnCb = l2cConnCbById(connId);
          pChanCb->psm = psm;
          pChanCb->peerMps = mps;
          pChanCb->peerMtu = mtu;
          pChanCb->peerCredits = credits;
          pChanCb->peerCid = cid;
          pChanCb->localCredits = pChanCb->pRegCb->reg.credits;
          pChanCb->role = L2C_COC_ROLE_ACCEPTOR;
          result = L2C_CONN_SUCCESS;
        }
        else
        {
          /* channel allocation failed */
          result = L2C_CONN_FAIL_RES;
        }
      }
    }
    else
    {
      /* peer CID already allocated to this psm */
      result = L2C_CONN_FAIL_ALLOCATED_SCID;
    }
  }
  else
  {
    /* not registered on this psm */
    result = L2C_CONN_FAIL_PSM;
  }

  /* send connection response */
  l2cSendConnectRsp(handle, identifier, result, pChanCb);

  /* if successful call client callback */
  if (result == L2C_CONN_SUCCESS)
  {
    l2cConnectInd(pChanCb);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a connect response message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cProcLeConnectRsp(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cChanCb_t         *pChanCb;
  uint16_t            cid;
  uint16_t            mps;
  uint16_t            mtu;
  uint16_t            credits;
  uint16_t            result;
  dmConnId_t          connId;

  /* parse parameters */
  BSTREAM_TO_UINT16(cid, pPacket);
  BSTREAM_TO_UINT16(mtu, pPacket);
  BSTREAM_TO_UINT16(mps, pPacket);
  BSTREAM_TO_UINT16(credits, pPacket);
  BSTREAM_TO_UINT16(result, pPacket);

  /* get conn ID for handle */
  if ((connId = DmConnIdByHandle(handle)) == DM_CONN_ID_NONE)
  {
    return;
  }

  /* verify parameters */
  // TBD
  // If fail ignore

  /* any channels waiting for connection response on this handle? */
  if ((pChanCb = l2cChanCbConnecting(connId, identifier)) != NULL)
  {
    if (result == L2C_CONN_SUCCESS)
    {
      /* yes; we are connected */
      pChanCb->state = L2C_CHAN_STATE_CONNECTED;

      /* stop timer */
      WsfTimerStop(&pChanCb->reqTimer);

      /* store parameters */
      pChanCb->peerMps = (mps < pChanCb->pRegCb->reg.mps) ? mps : pChanCb->pRegCb->reg.mps;
      pChanCb->peerMtu = mtu;
      pChanCb->peerCredits = credits;
      pChanCb->localCredits = pChanCb->pRegCb->reg.credits;
      pChanCb->peerCid = cid;

      /* call client callback */
      l2cConnectInd(pChanCb);
    }
    else
    {
      /* connection failed, call client callback with disconnect */
      l2cDisconnectIndAndDealloc(pChanCb, result);
    }
  }
  else
  {
    L2C_TRACE_INFO2("LeConnectRsp for unknown peerCid=0x%04x connId=%d", cid, connId);
  }

  // tbd consider case of api disconnect req before receive connect rsp
}

/*************************************************************************************************/
/*!
 *  \brief  Process a disconnect request message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cProcDisconnectReq(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cChanCb_t         *pChanCb;
  uint16_t            destCid;
  uint16_t            sourceCid;

  /* parse parameters */
  BSTREAM_TO_UINT16(destCid, pPacket);
  BSTREAM_TO_UINT16(sourceCid, pPacket);

  if ((pChanCb = l2cChanCbByCidCheck(destCid)) != NULL)
  {
    if ((pChanCb->peerCid == sourceCid) &&
        ((pChanCb->state == L2C_CHAN_STATE_CONNECTED) ||
         (pChanCb->state == L2C_CHAN_STATE_DISCONNECTING)))
    {
      /* send disconnect response */
      l2cSendDisconnectRsp(handle, identifier, destCid, sourceCid);

      /* execute client callback */
      l2cDisconnectIndAndDealloc(pChanCb, L2C_CONN_NONE);
    }
  }
  else
  {
    /* no matching local cid found; send command reject */
    //tbd add CID parameters
    l2cSendCmdReject(handle, identifier, L2C_REJ_INVALID_CID);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a disconnect response message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cProcDisconnectRsp(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cChanCb_t         *pChanCb;
  uint16_t            sourceCid;

  /* parse parameters */
  pPacket += 2;
  BSTREAM_TO_UINT16(sourceCid, pPacket);

  if ((pChanCb = l2cChanCbByCidState(sourceCid, L2C_CHAN_STATE_DISCONNECTING)) != NULL)
  {
    /* stop timer */
    WsfTimerStop(&pChanCb->reqTimer);

    /* execute client callback */
    l2cDisconnectIndAndDealloc(pChanCb, L2C_CONN_NONE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a command reject message from peer.
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received request.
 *  \param  pPacket     Packet data.
 *
 *  \return TRUE if command reject processed, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t l2cProcCommandRej(uint16_t handle, uint8_t identifier, uint8_t *pPacket)
{
  l2cChanCb_t         *pChanCb;
  dmConnId_t          connId;

  /* get conn ID for handle */
  if ((connId = DmConnIdByHandle(handle)) != DM_CONN_ID_NONE)
  {
    /* any channels waiting for connection response on this handle? */
    if ((pChanCb = l2cChanCbConnecting(connId, identifier)) != NULL)
    {
      /* connection failed, call client callback with disconnect */
      l2cDisconnectIndAndDealloc(pChanCb, L2C_CONN_NONE);
      return TRUE;
    }
  }

  /* otherwise command reject not processed here */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Manage local credits.
 *
 *  \param  pChanCb   Channel control block.
 *
 *  \return TRUE if successful, FALSE if received packet cannot be processed.
 */
/*************************************************************************************************/
static bool_t l2CocManageLocalCredits(l2cChanCb_t *pChanCb)
{
  /* if we have credits */
  if (pChanCb->localCredits > 0)
  {
    /* decrement credit count */
    pChanCb->localCredits--;

    /* if below threshold send a credit update */
    if (pChanCb->localCredits == 0 && pChanCb->pRegCb->reg.credits > 0)
    {
      pChanCb->localCredits = pChanCb->pRegCb->reg.credits;

      l2cSendFlowControlCredit(pChanCb->pConnCb->handle, pChanCb->localCid, pChanCb->localCredits);
    }
    return TRUE;
  }
  /* else we received a packet at zero credits; disconnect */
  else
  {
    l2cDisconnectChannel(pChanCb);
    return FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2CAP data callback function.
 *
 *  \param  handle    The connection handle.
 *  \param  cid       The L2CAP connection ID.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet payload.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocDataCidCback(uint16_t handle, uint16_t cid, uint16_t len, uint8_t *pPacket)
{
  l2cChanCb_t   *pChanCb;

  /* look for matching CID in connected state */
  if ((pChanCb = l2cChanCbByCidState(cid, L2C_CHAN_STATE_CONNECTED)) != NULL)
  {
    /* manage credits */
    if (l2CocManageLocalCredits(pChanCb))
    {
      /* reassemble */
      l2CocDataReassemble(pChanCb, len, pPacket);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2CAP signaling packet callback.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cCocSignalingCback(uint16_t handle, uint16_t l2cLen, uint8_t *pPacket)
{
  uint8_t   code;
  uint8_t   id;
  uint16_t  len;
  uint8_t   *p = pPacket;

  /* parse code, len, and identifier */
  p += L2C_PAYLOAD_START;
  BSTREAM_TO_UINT8(code, p);
  BSTREAM_TO_UINT8(id, p);
  BSTREAM_TO_UINT16(len, p);

  /* verify signaling length vs. l2c length */
  if (l2cLen != (len + L2C_SIG_HDR_LEN))
  {
    L2C_TRACE_WARN3("invalid msg code:%d len:%d l2cLen:%d", code, len, l2cLen);

    /* reject command */
    l2cSendCmdReject(handle, id, L2C_REJ_NOT_UNDERSTOOD);
    return;
  }


  /* if flow control credit */
  if ((code == L2C_SIG_FLOW_CTRL_CREDIT) && (len == L2C_SIG_FLOW_CTRL_CREDIT_LEN))
  {
    l2cProcFlowControlCredit(handle, id, p);
  }
  /* if connection request */
  else if ((code == L2C_SIG_LE_CONNECT_REQ) && (len == L2C_SIG_LE_CONN_REQ_LEN))
  {
    l2cProcLeConnectReq(handle, id, p);
  }
  /* if connection response */
  else if ((code == L2C_SIG_LE_CONNECT_RSP) && (len == L2C_SIG_LE_CONN_RSP_LEN))
  {
    l2cProcLeConnectRsp(handle, id, p);
  }
  /* if disconnection request */
  else if ((code == L2C_SIG_DISCONNECT_REQ) && (len == L2C_SIG_DISCONN_REQ_LEN))
  {
    l2cProcDisconnectReq(handle, id, p);
  }
  /* if disconnection response */
  else if ((code == L2C_SIG_DISCONNECT_RSP) && (len == L2C_SIG_DISCONN_RSP_LEN))
  {
    l2cProcDisconnectRsp(handle, id, p);
  }
  /* if command reject */
  else if ((code == L2C_SIG_CMD_REJ) && (len >= L2C_SIG_CMD_REJ_LEN))
  {
    if (!l2cProcCommandRej(handle, id, p))
    {
      /* pass to main signaling packet processing function if reject not handled */
      l2cRxSignalingPkt(handle, l2cLen, pPacket);
    }
  }
  /* else pass to main signaling packet processing function */
  else
  {
    l2cRxSignalingPkt(handle, l2cLen, pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Default L2CAP control callback.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocCtrlCback(wsfMsgHdr_t *pMsg)
{
  l2cConnCb_t *pConnCb = l2cConnCbById((dmConnId_t) pMsg->param);

  /* store flow control state */
  pConnCb->flowDisabled = (pMsg->event == L2C_CTRL_FLOW_DISABLE_IND);

  /* if flow enabled */
  if (!pConnCb->flowDisabled)
  {
    /* check for pending data to be sent on this connection */
    //tbd
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM connection callback for L2CAP.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocDmConnCback(dmEvt_t *pDmEvt)
{
  l2cConnCb_t *pConnCb = l2cConnCbById((dmConnId_t) pDmEvt->hdr.param);

  /* if new connection created */
  if (pDmEvt->hdr.event == DM_CONN_OPEN_IND)
  {
    /* initialize connection control block */
    pConnCb->connId = (dmConnId_t) pDmEvt->hdr.param;
    pConnCb->handle = pDmEvt->connOpen.handle;
    pConnCb->flowDisabled = FALSE;
  }
  /* else if connection closed */
  else if (pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
  {
    /* disconnect all channels on this connection */
    l2cChanCbDeallocAll(pConnCb->connId);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a data request message from API.
 *
 *  \param  pMsg  Message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocApiDataReq(l2cCocMsg_t *pMsg)
{
  l2cChanCb_t     *pChanCb = l2cChanCbByCid(pMsg->dataReq.localCid);

  L2C_TRACE_INFO2("l2cCocApiDataReq sduLen:%d peerMps:%d", pMsg->dataReq.sduLen, pChanCb->peerMps);

  /* if channel connected */
  if (pChanCb->state == L2C_CHAN_STATE_CONNECTED)
  {
    //tbd verify application didn't exceed peer mtu

    /* if no packet already on deck */
    if (pChanCb->pTxPkt == NULL)
    {
      /* if length does not exceed MPS and we have peer credits */
      if (((pMsg->dataReq.sduLen + L2C_LE_SDU_HDR_LEN) <= pChanCb->peerMps) &&
          (pChanCb->peerCredits > 0) && !pChanCb->pConnCb->flowDisabled)
      {
        /* decrement credits */
        pChanCb->peerCredits--;

        /* send packet now */
        L2cDataReq(pChanCb->peerCid, pChanCb->pConnCb->handle,
                   pMsg->dataReq.sduLen + L2C_LE_SDU_HDR_LEN, pMsg->dataReq.pPacket);

        /* call callback */
        l2cDataCnf(pChanCb, L2C_COC_DATA_SUCCESS);
      }
      else
      {
        /* set up packet to be sent */
        pChanCb->pTxPkt = pMsg->dataReq.pPacket;
        pChanCb->txTotalLen = pMsg->dataReq.sduLen + L2C_LE_SDU_HDR_LEN;
        pChanCb->txCurrLen = 0;
        l2cCocSendData(pChanCb);
      }
    }
    else
    {
      /* else transaction overflow; discard packet */
      WsfMsgFree(pMsg->dataReq.pPacket);

      /* call callback with failure status */
      l2cDataCnf(pChanCb, L2C_COC_DATA_ERR_OVERFLOW);
    }
  }
  else
  {
    /* channel closed; discard packet */
    WsfMsgFree(pMsg->dataReq.pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a Connect request message from API.
 *
 *  \param  pMsg  Message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocApiConnectReq(l2cCocMsg_t *pMsg)
{
  l2cChanCb_t *pChanCb = l2cChanCbByCid(pMsg->connectReq.localCid);

  /* verify still in connecting state (link could have been disconnected) */
  if (pChanCb->state == L2C_CHAN_STATE_CONNECTING)
  {
    pChanCb->role = L2C_COC_ROLE_INITIATOR;
    pChanCb->psm = pMsg->connectReq.remotePsm;

    /* send connect req */
    l2cSendConnectReq(pChanCb->pConnCb->handle, pChanCb->psm, pChanCb);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a Connect request message from API.
 *
 *  \param  pMsg  Message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocApiDisconnectReq(l2cCocMsg_t *pMsg)
{
  l2cChanCb_t *pChanCb = l2cChanCbByCid(pMsg->hdr.param);

  /* if in connected state */
  if (pChanCb->state == L2C_CHAN_STATE_CONNECTED)
  {
    l2cDisconnectChannel(pChanCb);
  }
  /* else if in connecting state */
  else if (pChanCb->state == L2C_CHAN_STATE_CONNECTING)
  {
    // tbd consider api disconnect req in this state
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a request timeout.
 *
 *  \param  pMsg  Message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cCocReqTimeout(wsfMsgHdr_t *pMsg)
{
  l2cChanCb_t *pChanCb = l2cChanCbByCid(pMsg->param);

  /* if still connecting or disconnecting  */
  if (pChanCb->state == L2C_CHAN_STATE_CONNECTING ||
      pChanCb->state == L2C_CHAN_STATE_DISCONNECTING)
  {
    /* deallocate channel and call client callback */
    l2cDisconnectIndAndDealloc(pChanCb, L2C_CONN_FAIL_TIMEOUT);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C connection oriented channel subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocInit(void)
{
  l2cCb.l2cSignalingCback = l2cCocSignalingCback;
  l2cCb.l2cCocCtrlCback = l2cCocCtrlCback;
  l2cCb.l2cDataCidCback = l2cCocDataCidCback;

  /* Register with DM */
  DmConnRegister(DM_CLIENT_ID_L2C, l2cCocDmConnCback);
}

/*************************************************************************************************/
/*!
 *  \brief  Event handler initialization function for L2C with connection oriented channels.
 *
 *  \param  handlerId  ID for this event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocHandlerInit(wsfHandlerId_t handlerId)
{
  l2cCocCb.handlerId = handlerId;
}

/*************************************************************************************************/
/*!
 *  \brief  The WSF event handler for L2C with connection oriented channels.
 *
 *  \param  event   Event mask.
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  /* Handle message */
  if (pMsg != NULL)
  {
    switch(pMsg->event)
    {
      case L2C_MSG_API_DATA_REQ:
        l2cCocApiDataReq((l2cCocMsg_t *) pMsg);
        break;

      case L2C_MSG_API_CONNECT_REQ:
        l2cCocApiConnectReq((l2cCocMsg_t *) pMsg);
        break;

      case L2C_MSG_API_DISCONNECT_REQ:
        l2cCocApiDisconnectReq((l2cCocMsg_t *) pMsg);
        break;

      case L2C_MSG_COC_REQ_TIMEOUT:
        l2cCocReqTimeout(pMsg);
        break;

      default:
        break;
    }
  }
  /* Handle events */
  else if (event)
  {

  }
}

/*************************************************************************************************/
/*!
 *  \brief  Register to use a connection oriented channel, as either a channel acceptor,
 *          initiator, or both.  If registering as channel acceptor then the PSM is specified.
 *          After registering, a connection can be established by the client using this
 *          registration instance.
 *
 *  \param  cback   Client callback function.
 *  \param  pReg    Registration parameter structure.
 *
 *  \return Registration instance ID or L2C_COC_REG_ID_NONE if registration failed.
 */
/*************************************************************************************************/
l2cCocRegId_t L2cCocRegister(l2cCocCback_t cback, l2cCocReg_t *pReg)
{
  l2cRegCb_t    *pRegCb;

  WSF_ASSERT(cback != NULL);

  WsfTaskLock();
  pRegCb = l2cRegCbAlloc(cback, pReg);
  WsfTaskUnlock();

  if (pRegCb != NULL)
  {
    return pRegCb->regId;
  }

  return L2C_COC_REG_ID_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  Deregister and deallocate a connection oriented channel registration instance.
 *          This function should only be called if there are no active channels using this
 *          registration instance.
 *
 *  \param  regId   Registration instance ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDeregister(l2cCocRegId_t regId)
{
  //check for active channels?
  //tbd

  WsfTaskLock();
  l2cRegCbDealloc(regId);
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate a connection to the given peer PSM.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The associated registration instance.
 *  \param  psm       Peer PSM.
 *
 *  \return Local CID or L2C_COC_CID_NONE none if failure.
 */
/*************************************************************************************************/
uint16_t L2cCocConnectReq(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm)
{
  l2cChanCb_t         *pChanCb = NULL;
  l2cRegCb_t          *pRegCb;
  l2cApiConnectReq_t  *pMsg;

  /* verify registration instance and allocate channel */
  WsfTaskLock();
  pRegCb = l2cRegCbById(regId);
  if (pRegCb->inUse && (pRegCb->reg.role & L2C_COC_ROLE_INITIATOR) && connId != DM_CONN_ID_NONE)
  {
    if ((pChanCb = l2cChanCbAlloc(L2C_CHAN_STATE_CONNECTING)) != NULL)
    {
      pChanCb->pRegCb = pRegCb;
      pChanCb->pConnCb = l2cConnCbById(connId);
    }
  }
  WsfTaskUnlock();

  if (pChanCb != NULL)
  {
    if ((pMsg = WsfMsgAlloc(sizeof(l2cApiConnectReq_t))) != NULL)
    {
      pMsg->hdr.param = connId;
      pMsg->hdr.event = L2C_MSG_API_CONNECT_REQ;
      pMsg->localCid = pChanCb->localCid;
      pMsg->remotePsm = psm;
      WsfMsgSend(l2cCocCb.handlerId, pMsg);

      return pChanCb->localCid;
    }
    else
    {
      WsfTaskLock();
      l2cChanCbDealloc(pChanCb);
      WsfTaskUnlock();
    }
  }

  return L2C_COC_CID_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  Disconnect the channel for the given CID.
 *
 *  \param  cid       Channel ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDisconnectReq(uint16_t cid)
{
  wsfMsgHdr_t *pMsg;

  /* allocate message buffer */
  if ((pMsg = WsfMsgAlloc(sizeof(wsfMsgHdr_t))) != NULL)
  {
    /* send message */
    pMsg->event = L2C_MSG_API_DISCONNECT_REQ;
    pMsg->param = cid;
    WsfMsgSend(l2cCocCb.handlerId, pMsg);
  }
}


/*************************************************************************************************/
/*!
 *  \brief  Send an L2CAP data packet on the given connection oriented CID.
 *
 *  \param  cid       The local channel identifier.
 *  \param  len       The length of the payload data in pPacket.
 *  \param  pPacket   Packet payload data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDataReq(uint16_t cid, uint16_t len, uint8_t *pPayload)
{
  l2cApiDataReq_t *pMsg;
  uint8_t         *pPkt;
  uint8_t         *p;

  /* allocate packet buffer */
  if ((pPkt = l2cMsgAlloc(L2C_LE_SDU_PKT_BASE_LEN + len)) != NULL)
  {
    /* allocate message buffer */
    if ((pMsg = WsfMsgAlloc(sizeof(l2cApiDataReq_t))) != NULL)
    {
      /* copy length and packet data to packet buffer */
      p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
      UINT16_TO_BSTREAM(p, len);
      memcpy(p, pPayload, len);

      /* copy message parameters */
      pMsg->pPacket = pPkt;
      pMsg->localCid = cid;
      pMsg->sduLen = len;

      /* send message */
      pMsg->hdr.event = L2C_MSG_API_DATA_REQ;
      WsfMsgSend(l2cCocCb.handlerId, pMsg);
    }
    else
    {
      /* allocation failed; free packet buffer */
      WsfMsgFree(pPkt);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.
 *
 *  \param  result    Result code
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocErrorTest(uint16_t result)
{
  l2cCocCb.errTest = result;
}

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.
 *
 *  \param  cid       The local channel identifier.
 *  \param  credits   Credits to send.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocCreditSendTest(uint16_t cid, uint16_t credits)
{
  l2cChanCb_t *pChanCb;

  WsfTaskLock();
  pChanCb = l2cChanCbByCid(cid);
  pChanCb->localCredits += credits;
  l2cSendFlowControlCredit(pChanCb->pConnCb->handle, cid, credits);
  WsfTaskUnlock();
}
