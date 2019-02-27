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
 *  \brief ATT client main module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "attc_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* type for send request function */
typedef void (*attcSendReq_t)(attcCcb_t *pCcb);

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

static void attcDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket);
static void attcCtrlCback(wsfMsgHdr_t *pMsg);
static void attcConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt);
static void attcMsgCback(attcApiMsg_t *pMsg);

static void attcSendSimpleReq(attcCcb_t *pCcb);
static void attcSendContinuingReq(attcCcb_t *pCcb);
static void attcSendMtuReq(attcCcb_t *pCcb);
static void attcSendWriteCmd(attcCcb_t *pCcb);
static void attcSendPrepWriteReq(attcCcb_t *pCcb);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Interface to ATT */
static const attFcnIf_t attcFcnIf =
{
  attcDataCback,
  attcCtrlCback,
  (attMsgHandler_t) attcMsgCback,
  attcConnCback
};

/* Table of send request functions */
static const attcSendReq_t attcSendReqTbl[] =
{
  NULL,                         /* ATTC_MSG_API_NONE (unused) */
  attcSendMtuReq,               /* ATTC_MSG_API_MTU */
  attcSendContinuingReq,        /* ATTC_MSG_API_FIND_INFO */
  attcSendContinuingReq,        /* ATTC_MSG_API_FIND_BY_TYPE_VALUE */
  attcSendContinuingReq,        /* ATTC_MSG_API_READ_BY_TYPE */
  attcSendSimpleReq,            /* ATTC_MSG_API_READ */
  attcSendContinuingReq,        /* ATTC_MSG_API_READ_LONG */
  attcSendSimpleReq,            /* ATTC_MSG_API_READ_MULTIPLE */
  attcSendContinuingReq,        /* ATTC_MSG_API_READ_BY_GROUP_TYPE */
  attcSendSimpleReq,            /* ATTC_MSG_API_WRITE */
  attcSendWriteCmd,             /* ATTC_MSG_API_WRITE_CMD */
  attcSendPrepWriteReq,         /* ATTC_MSG_API_PREP_WRITE */
  attcSendSimpleReq             /* ATTC_MSG_API_EXEC_WRITE */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Control block */
attcCb_t attcCb;

/*************************************************************************************************/
/*!
 *  \brief  Check if application callback is pending for a given write command, or the maximum
 *          number of simultaneous write commands has been reached.
 *
 *  \param  pCcb  ATTC control block.
 *  \param  pMsg  ATTC message.
 *
 *  \return TRUE if app callback's pending or maximum number of simultaneous write commands reached.
 *          FALSE, otherwise.
 */
/*************************************************************************************************/
static bool_t attcPendWriteCmd(attcCcb_t *pCcb, attcApiMsg_t *pMsg)
{
  uint8_t     pendRsp;
  uint8_t     i;

  /* initialize number of response callbacks pending */
  pendRsp = 0;

  for (i = 0; i < ATT_NUM_SIMUL_WRITE_CMD; i++)
  {
    /* if callback pending for write command */
    if (pCcb->pendWriteCmdHandle[i] != ATT_HANDLE_NONE)
    {
      /* if callback pending for this handle */
      if (pCcb->pendWriteCmdHandle[i] == pMsg->handle)
      {
        /* callback pending for this write command */
        return TRUE;
      }

      pendRsp++;
    }
  }

  /* no callback is pending for this write command but see if the maximum number of simultaneous
     write commands has been reached */
  return (pendRsp < ATT_NUM_SIMUL_WRITE_CMD) ? FALSE : TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Set pending write command response callback for a given attribute handle.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSetPendWriteCmd(attcCcb_t *pCcb)
{
  uint8_t     i;

  for (i = 0; i < ATT_NUM_SIMUL_WRITE_CMD; i++)
  {
    /* if entry free */
    if (pCcb->pendWriteCmdHandle[i] == ATT_HANDLE_NONE)
    {
      /* set pending write command handle */
      pCcb->pendWriteCmdHandle[i] = pCcb->outReq.handle;
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Call pending write command response application callback.
 *
 *  \param  connId  DM connection ID.
 *  \param  pCcb    ATTC control block.
 *  \param  status  Callback event status.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcWriteCmdCallback(dmConnId_t connId, attcCcb_t *pCcb, uint8_t status)
{
  uint8_t     i;

  /* if any pending write command callback */
  for (i = 0; i < ATT_NUM_SIMUL_WRITE_CMD; i++)
  {
    if (pCcb->pendWriteCmdHandle[i] != ATT_HANDLE_NONE)
    {
      /* call callback with status */
      attcExecCallback(connId, ATTC_WRITE_CMD_RSP, pCcb->pendWriteCmdHandle[i], status);
      pCcb->pendWriteCmdHandle[i] = ATT_HANDLE_NONE;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send attribute client request that has already been built.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSendSimpleReq(attcCcb_t *pCcb)
{
  attcPktParam_t  *pPkt = pCcb->outReq.pPkt;

  /* clear stored packet pointer */
  pCcb->outReq.pPkt = NULL;

  /* start request timer (except for write command) */
  if (pCcb->outReq.hdr.event != ATTC_MSG_API_WRITE_CMD)
  {
    pCcb->outReqTimer.msg.event = ATTC_MSG_REQ_TIMEOUT;
    WsfTimerStartSec(&pCcb->outReqTimer, pAttCfg->transTimeout);
  }

  /* send packet to L2CAP */
  L2cDataReq(L2C_CID_ATT, pCcb->pMainCcb->handle, pPkt->len, (uint8_t *) pPkt);
}

/*************************************************************************************************/
/*!
 *  \brief  Send a request that may be continuing.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSendContinuingReq(attcCcb_t *pCcb)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* if continuing */
  if (pCcb->outReq.hdr.status == ATTC_CONTINUING)
  {
    /* allocate new buffer */
    if ((pPkt = attMsgAlloc(pCcb->outReq.pPkt->len + L2C_PAYLOAD_START)) != NULL)
    {
      /* copy stored packet to new */
      memcpy(pPkt, pCcb->outReq.pPkt, pCcb->outReq.pPkt->len + L2C_PAYLOAD_START);
    }
    /* else handle error case of allocation failure */
    else
    {
      /* free stored packet and call callback with failure status */
      attcReqClear(pCcb, &pCcb->outReq, ATT_ERR_MEMORY);
      return;
    }
  }
  /* else not continuing */
  else
  {
    /* send the stored packet */
    pPkt = pCcb->outReq.pPkt;

    /* clear stored packet pointer */
    pCcb->outReq.pPkt = NULL;
  }

  /* build remaining fields of packet from stored parameters */
  p = (uint8_t *) pPkt + L2C_PAYLOAD_START + ATT_HDR_LEN;
  if (pCcb->outReq.hdr.event == ATTC_MSG_API_READ_LONG)
  {
    /* build offset field */
    p += sizeof(uint16_t);
    UINT16_TO_BSTREAM(p, pCcb->outReqParams.o.offset);
  }
  else
  {
    /*
     * Find Info, Find By Type Value, Read By Type, and Read By Group Type
     * request packets all begin with a start handle and end handle.
     */
    UINT16_TO_BSTREAM(p, pCcb->outReqParams.h.startHandle);
    UINT16_TO_BSTREAM(p, pCcb->outReqParams.h.endHandle);
  }

  /* start request timer */
  pCcb->outReqTimer.msg.event = ATTC_MSG_REQ_TIMEOUT;
  WsfTimerStartSec(&pCcb->outReqTimer, pAttCfg->transTimeout);

  /* send packet to L2CAP */
  L2cDataReq(L2C_CID_ATT, pCcb->pMainCcb->handle, pPkt->len, (uint8_t *) pPkt);
}

/*************************************************************************************************/
/*!
 *  \brief  Send MTU request.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSendMtuReq(attcCcb_t *pCcb)
{
  /* if MTU already exchanged */
  if (pCcb->pMainCcb->control & ATT_CCB_STATUS_MTU_SENT)
  {
    /* discard request */
    attcFreePkt(&pCcb->outReq);

    /* clear out req */
    pCcb->outReq.hdr.event = ATTC_MSG_API_NONE;

    ATT_TRACE_WARN0("MTU req discarded");
  }
  else
  {
    /* set MTU sent */
    pCcb->pMainCcb->control |= ATT_CCB_STATUS_MTU_SENT;

    /* send packet */
    attcSendSimpleReq(pCcb);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send attribute client Write command.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSendWriteCmd(attcCcb_t *pCcb)
{
  /* send packet */
  attcSendSimpleReq(pCcb);

  /* if flow not disabled */
  if (!(pCcb->pMainCcb->control & ATT_CCB_STATUS_FLOW_DISABLED))
  {
    /* call callback */
    attcExecCallback(pCcb->pMainCcb->connId, ATTC_WRITE_CMD_RSP, pCcb->outReq.handle, ATT_SUCCESS);
  }
  else
  {
    /* set pending write command callback for this handle */
    attcSetPendWriteCmd(pCcb);
  }

  /* clear out req */
  pCcb->outReq.hdr.event = ATTC_MSG_API_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  Send attribute client Prepare Write request.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcSendPrepWriteReq(attcCcb_t *pCcb)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;
  uint16_t        dataLen;

  /* if continuing */
  if (pCcb->outReq.hdr.status == ATTC_CONTINUING)
  {
    /* determine size of buffer to allocate */
    if (pCcb->outReqParams.w.len < (pCcb->pMainCcb->mtu - ATT_PREP_WRITE_REQ_LEN))
    {
      dataLen = pCcb->outReqParams.w.len;
    }
    else
    {
      dataLen = pCcb->pMainCcb->mtu - ATT_PREP_WRITE_REQ_LEN;
    }

    /* allocate new buffer */
    if ((pPkt = attMsgAlloc(dataLen + ATT_PREP_WRITE_REQ_LEN + L2C_PAYLOAD_START)) != NULL)
    {
      /* copy fixed fields */
      memcpy(pPkt, pCcb->outReq.pPkt, ATT_PREP_WRITE_REQ_LEN + L2C_PAYLOAD_START);

      /* copy data */
      memcpy(((uint8_t *) pPkt + L2C_PAYLOAD_START + ATT_PREP_WRITE_REQ_LEN),
             pCcb->outReqParams.w.pValue, dataLen);

      /* update length and data pointer */
      pCcb->outReqParams.w.pValue += dataLen;
      pCcb->outReqParams.w.len -= dataLen;
    }
    /* else handle error case of allocation failure */
    else
    {
      /* free stored packet and call callback with failure status */
      attcReqClear(pCcb, &pCcb->outReq, ATT_ERR_MEMORY);
      return;
    }
  }
  /* else not continuing */
  else
  {
    dataLen = pCcb->outReqParams.w.len;

    /* send the stored packet */
    pPkt = pCcb->outReq.pPkt;

    /* clear stored packet pointer */
    pCcb->outReq.pPkt = NULL;
  }

  /* build remaining fields of packet from stored parameters */
  p = (uint8_t *) pPkt + L2C_PAYLOAD_START + ATT_HDR_LEN + sizeof(uint16_t);
  UINT16_TO_BSTREAM(p, pCcb->outReqParams.w.offset);

  /* update offset after building packet */
  pCcb->outReqParams.w.offset += dataLen;

  /* start request timer */
  pCcb->outReqTimer.msg.event = ATTC_MSG_REQ_TIMEOUT;
  WsfTimerStartSec(&pCcb->outReqTimer, pAttCfg->transTimeout);

  /* send packet to L2CAP */
  L2cDataReq(L2C_CID_ATT, pCcb->pMainCcb->handle, dataLen + ATT_PREP_WRITE_REQ_LEN, (uint8_t *) pPkt);
}

/*************************************************************************************************/
/*!
 *  \brief  Lookup and execute function to build and send request.
 *
 *  \param  pCcb  ATTC control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcSendReq(attcCcb_t *pCcb)
{
  (*attcSendReqTbl[pCcb->outReq.hdr.event])(pCcb);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up and send an attribute client request.
 *
 *  \param  pCcb  ATTC control block.
 *  \param  pMsg  ATTC message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcSetupReq(attcCcb_t *pCcb, attcApiMsg_t *pMsg)
{
  /* set out req to api message */
  pCcb->outReq = *pMsg;

  /* store parameters */
  pCcb->outReqParams = *(pMsg->pPkt);

  /* build and send request */
  attcSendReq(pCcb);
}

/*************************************************************************************************/
/*!
 *  \brief  Data callback for ATTC.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t       opcode;
  attcCcb_t     *pCcb;

  /* get connection control block for this handle, ignore packet if not found */
  if ((pCcb = attcCcbByHandle(handle)) == NULL)
  {
    return;
  }

  /* parse opcode */
  opcode = *(pPacket + L2C_PAYLOAD_START);

  /* if response */
  if (opcode <= ATT_PDU_EXEC_WRITE_RSP)
  {
    attcProcRsp(pCcb, len, pPacket);
  }
  /* else if indication or notification */
  else if ((opcode == ATT_PDU_VALUE_NTF) || (opcode == ATT_PDU_VALUE_IND))
  {
    attcProcInd(pCcb, len, pPacket);
  }
  /* else unknown opcode */
  else
  {
    ATT_TRACE_WARN1("attc unknown opcode 0x%02x", opcode);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2CAP control callback.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcCtrlCback(wsfMsgHdr_t *pMsg)
{
  attcCcb_t     *pCcb;

  /* note this function is currently only called when flow is enabled */

  /* get CCB */
  if ((pCcb = attcCcbByConnId((dmConnId_t) pMsg->param)) != NULL)
  {
    /* if confirmation pending try sending now */
    AttcIndConfirm((dmConnId_t) pMsg->param);

    /* call pending write command callback */
    attcWriteCmdCallback((dmConnId_t) pMsg->param, pCcb, ATT_SUCCESS);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Connection callback for ATTC.
 *
 *  \param  pCcb    ATT control block.
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt)
{
  attcCcb_t *pClient;
  uint16_t  localMtu;
  uint8_t   status;

  /* if connection opened */
  if (pDmEvt->hdr.event == DM_CONN_OPEN_IND)
  {
    /* if we initiated connection send MTU request */
    if (DmConnRole(pCcb->connId) == DM_ROLE_MASTER)
    {
      localMtu = WSF_MIN(pAttCfg->mtu, (HciGetMaxRxAclLen() - L2C_HDR_LEN));

      /* if desired MTU is not the default */
      if (localMtu != ATT_DEFAULT_MTU)
      {
        AttcMtuReq(pCcb->connId, localMtu);
      }
    }
  }
  /* if connection closed */
  else if (pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
  {
    /* set status */
    if (pDmEvt->connClose.hdr.status == HCI_SUCCESS)
    {
      status = pDmEvt->connClose.reason + ATT_HCI_ERR_BASE;
    }
    else
    {
      status = pDmEvt->connClose.hdr.status + ATT_HCI_ERR_BASE;
    }

    /* get client control block directly */
    pClient = &attcCb.ccb[pCcb->connId - 1];

    /* free any out req */
    if (pClient->outReq.hdr.event != ATTC_MSG_API_NONE)
    {
      WsfTimerStop(&pClient->outReqTimer);
      attcReqClear(pClient, &pClient->outReq, status);
    }

    /* free any req on deck */
    if (pClient->onDeck.hdr.event != ATTC_MSG_API_NONE)
    {
      attcReqClear(pClient, &pClient->onDeck, status);
    }

    /* initialize other control block variables */
    pClient->flowDisabled = FALSE;
    pClient->cnfPending = FALSE;

    /* pass to connection close callback for signed data */
    if (attcCb.pSign != NULL)
    {
      (*attcCb.pSign->closeCback)(pClient, status);
    }

    /* call pending write command callback */
    attcWriteCmdCallback(pCcb->connId, pClient, status);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Message handler callback for ATTC.
 *
 *  \param  pMsg  ATTC message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attcMsgCback(attcApiMsg_t *pMsg)
{
  attcCcb_t   *pCcb;

  /* get CCB and verify connection still in use */
  if ((pCcb = attcCcbByConnId((dmConnId_t) pMsg->hdr.param)) == NULL)
  {
    /* if message has a packet buffer */
    if (pMsg->hdr.event >= ATTC_MSG_API_MTU &&
        pMsg->hdr.event <= ATTC_MSG_API_SIGNED_WRITE_CMD)
    {
      /* free packet buffer */
      attcFreePkt(pMsg);
    }

    /* ignore if connection not in use */
    return;
  }

  /* if an API request to send packet (non-signed) */
  if (pMsg->hdr.event <= ATTC_MSG_API_EXEC_WRITE)
  {
    /* verify no API request already waiting on deck, in progress, or no pending write command
       already for this handle */
    if ((pCcb->onDeck.hdr.event != ATTC_MSG_API_NONE) ||
        (pCcb->outReq.hdr.event > ATTC_MSG_API_MTU)   ||
        ((pMsg->hdr.event == ATTC_MSG_API_WRITE_CMD)  &&
         attcPendWriteCmd(pCcb, pMsg)))
    {
      /* free request and call callback with failure status */
      attcReqClear(pCcb, pMsg, ATT_ERR_OVERFLOW);
      return;
    }

    /* if MTU request in progress or flow controlled */
    if (pCcb->outReq.hdr.event == ATTC_MSG_API_MTU || pCcb->flowDisabled)
    {
      /* put request "on deck" for processing later */
      pCcb->onDeck = *pMsg;
    }
    /* otherwise ready to send; set up request */
    else
    {
      attcSetupReq(pCcb, pMsg);
    }
  }
  /* else if signed data event */
  else if (pMsg->hdr.event >= ATTC_MSG_API_SIGNED_WRITE_CMD &&
           pMsg->hdr.event <= ATTC_MSG_CMAC_CMPL)
  {
    /* pass to message callback for signed data */
    if (attcCb.pSign != NULL)
    {
      (*attcCb.pSign->msgCback)(pCcb, pMsg);
    }
  }
  /* else if cancel request */
  else if (pMsg->hdr.event == ATTC_MSG_API_CANCEL)
  {
    /* free any out req (except mtu req) */
    if (pCcb->outReq.hdr.event != ATTC_MSG_API_NONE &&
        pCcb->outReq.hdr.event != ATTC_MSG_API_MTU)
    {
      WsfTimerStop(&pCcb->outReqTimer);
      attcReqClear(pCcb, &pCcb->outReq, ATT_ERR_CANCELLED);
    }
    /* else free any req on deck */
    else if (pCcb->onDeck.hdr.event != ATTC_MSG_API_NONE)
    {
      attcReqClear(pCcb, &pCcb->onDeck, ATT_ERR_CANCELLED);
    }
  }
  /* else if timeout */
  else if (pMsg->hdr.event == ATTC_MSG_REQ_TIMEOUT)
  {
    /* free any out req */
    if (pCcb->outReq.hdr.event != ATTC_MSG_API_NONE)
    {
      attcReqClear(pCcb, &pCcb->outReq, ATT_ERR_TIMEOUT);
      pCcb->pMainCcb->control |= ATT_CCB_STATUS_TX_TIMEOUT;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return the ATTC connection control block connection ID.
 *
 *  \param  connId    Connection ID.
 *
 *  \return Pointer to connection control block or NULL if not in use.
 */
/*************************************************************************************************/
attcCcb_t *attcCcbByConnId(dmConnId_t connId)
{
  if (DmConnInUse(connId))
  {
    return &attcCb.ccb[connId - 1];
  }
  else
  {
    ATT_TRACE_WARN1("attc ccb not in use: %d", connId);
    return NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection control block for the given handle.
 *
 *  \param  handle    The connection handle.
 *
 *  \return Pointer to connection control block or NULL if not found.
 */
/*************************************************************************************************/
attcCcb_t *attcCcbByHandle(uint16_t handle)
{
  dmConnId_t  connId;

  if ((connId = DmConnIdByHandle(handle)) != DM_CONN_ID_NONE)
  {
    return &attcCb.ccb[connId - 1];
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Free the packet buffer of an API message structure.
 *
 *  \param  pMsg    Pointer to API message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcFreePkt(attcApiMsg_t *pMsg)
{
  if (pMsg->pPkt != NULL)
  {
    WsfMsgFree(pMsg->pPkt);
    pMsg->pPkt = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Execute application callback function.
 *
 *  \param  connId  DM connection ID.
 *  \param  event   Callback event ID.
 *  \param  handle  Attribute handle.
 *  \param  status  Callback event status.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcExecCallback(dmConnId_t connId, uint8_t event, uint16_t handle, uint8_t status)
{
  if (event != ATT_METHOD_MTU)
  {
    attExecCallback(connId, event, handle, status, 0);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear an outstanding request and execute the callback.
 *
 *  \param  pCcb    Pointer to control block.
 *  \param  pMsg    API message.
 *  \param  status  Callback event status.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcReqClear(attcCcb_t *pCcb, attcApiMsg_t *pMsg, uint8_t status)
{
  attcFreePkt(pMsg);
  attcExecCallback(pCcb->pMainCcb->connId, pMsg->hdr.event, pMsg->handle, status);
  pMsg->hdr.event = ATTC_MSG_API_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  Set automatic Indication Confirmations sent from this ATT Client.
 *
 *  \param  enable  \ref TRUE to enable automatic confirmations (default), \ref FALSE to disable.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcSetAutoConfirm(bool_t enable)
{
  attcCb.autoCnf = enable;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize ATT client.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcInit(void)
{
  uint8_t     i;
  attcCcb_t   *pCcb;

  /* Initialize control block */
  attcCb.pSign = NULL;
  attcCb.autoCnf = TRUE;

  /* Initialize control block CCBs */
  for (i = 0, pCcb = attcCb.ccb; i < DM_CONN_MAX; i++, pCcb++)
  {
    /* set pointer to main CCB */
    pCcb->pMainCcb = &attCb.ccb[i];

    /* initialize timer */
    pCcb->outReqTimer.handlerId = attCb.handlerId;
    pCcb->outReqTimer.msg.param = i + 1;  /* param stores the conn id */
  }

  /* set up callback interface */
  attCb.pClient = &attcFcnIf;
}
