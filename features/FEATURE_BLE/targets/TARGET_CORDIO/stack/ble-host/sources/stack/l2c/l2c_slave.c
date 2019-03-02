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
 *  \brief L2CAP module for slave operations.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_timer.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "util/bstream.h"
#include "l2c_api.h"
#include "l2c_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Signaling request timeout in seconds */
#define L2C_SIG_REQ_TIMEOUT     30

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Slave control block */
typedef struct
{
  wsfTimer_t        reqTimer;              /* Signaling request timeout timer */
  wsfHandlerId_t    handlerId;             /* ID for this event handler */
  uint8_t           lastCode[DM_CONN_MAX]; /* last code sent on each handle */
  uint8_t           signId[DM_CONN_MAX];   /* expected signaling identifier */
} l2cSlaveCb_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

static l2cSlaveCb_t l2cSlaveCb;

/*************************************************************************************************/
/*!
 *  \brief  Handle slave signaling request timeout.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cSlaveReqTimeout(wsfMsgHdr_t *pMsg)
{
  L2C_TRACE_WARN0("conn update req timeout");

  /* Notify DM that connection update has failed (handle is stored in param) */
  DmL2cConnUpdateCnf(pMsg->param, L2C_CONN_PARAM_REJECTED);
}

/*************************************************************************************************/
/*!
 *  \brief  Slave processing of received L2CAP signaling packets.
 *
 *  \param  handle    The connection handle.
 *  \param  l2cLen    The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cSlaveRxSignalingPkt(uint16_t handle, uint16_t l2cLen, uint8_t *pPacket)
{
  uint8_t   code;
  uint8_t   id;
  uint16_t  len;
  uint16_t  result;

  /* parse code, len, and identifier */
  pPacket += L2C_PAYLOAD_START;
  BSTREAM_TO_UINT8(code, pPacket);
  BSTREAM_TO_UINT8(id, pPacket);
  BSTREAM_TO_UINT16(len, pPacket);

  /* verify signal identifier is valid */
  if (id == L2C_SIGNAL_ID_INVALID)
  {
    /* not expected, ignore */
    return;
  }

  /* verify signal identifier is expected
   * verify signaling length vs. l2c length
   * verify this is a conn param update rsp or command reject
   * verify parameter length
   */
  if ((id == l2cSlaveCb.signId[handle])   &&
      (l2cLen == (len + L2C_SIG_HDR_LEN)) &&
      (((code == L2C_SIG_CONN_UPDATE_RSP) && (len == L2C_SIG_CONN_UPDATE_RSP_LEN)) ||
       (code == L2C_SIG_CMD_REJ)))
  {
    /* get last sent code */
    uint8_t lastCode = l2cSlaveCb.lastCode[handle];

    /* clear pending signal id */
    l2cSlaveCb.signId[handle] = L2C_SIGNAL_ID_INVALID;

    /* parse result parameter */
    BSTREAM_TO_UINT16(result, pPacket);

    /* stop req timer */
    WsfTimerStop(&l2cSlaveCb.reqTimer);

    if (lastCode == L2C_SIG_CONN_UPDATE_REQ)
    {
      if (code == L2C_SIG_CMD_REJ)
      {
        /* got command reject */
        result = L2C_CONN_PARAM_REJECTED;
      }

      /* send to DM */
      DmL2cConnUpdateCnf(handle, result);
    }
    else
    {
      /* send to DM */
      DmL2cCmdRejInd(handle, result);
    }
  }
  else
  {
    L2C_TRACE_WARN3("invalid msg code:%d len:%d l2cLen:%d", code, len, l2cLen);

    /* reject all unknown, invalid or unidentified commands except command reject. */
    if (code != L2C_SIG_CMD_REJ)
    {
      l2cSendCmdReject(handle, id, L2C_REJ_NOT_UNDERSTOOD);
    }

    return;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C for operation as a Bluetooth LE slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveInit(void)
{
  l2cCb.slaveRxSignalingPkt = l2cSlaveRxSignalingPkt;

  for (uint8_t i = 0; i < DM_CONN_MAX; i++)
  {
    l2cSlaveCb.signId[i] = L2C_SIGNAL_ID_INVALID;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a signaling packet.
 *
 *  \param  handle      The connection handle.
 *  \param  code        Type of command.
 *  \param  len         Length of \ref pParam.
 *  \param  pParam      parameters of command to send.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmSigReq(uint16_t handle, uint8_t code, uint16_t len, uint8_t *pParam)
{
  uint8_t *pPacket;
  uint8_t *p;

  WSF_ASSERT(handle < DM_CONN_MAX);

  /* record code */
  l2cSlaveCb.lastCode[handle] = code;

  /* Start signaling request timer and store handle */
  WsfTimerStartSec(&l2cSlaveCb.reqTimer, L2C_SIG_REQ_TIMEOUT);
  l2cSlaveCb.reqTimer.msg.param = handle;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + len)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, code);                         /* command code */
    l2cSlaveCb.signId[handle] = l2cCb.identifier;
    UINT8_TO_BSTREAM(p, l2cCb.identifier);             /* identifier */
    l2cCb.identifier = L2C_NEXT_ID(l2cCb.identifier);
    UINT16_TO_BSTREAM(p, len);                         /* parameter length */
    memcpy(p, pParam, len);                            /* parameters */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + len), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to send an L2CAP connection update request.
 *
 *  \param  handle      The connection handle.
 *  \param  pConnSpec   Pointer to the connection specification structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateReq(uint16_t handle, hciConnSpec_t *pConnSpec)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* record code */
  l2cSlaveCb.lastCode[handle] = L2C_SIG_CONN_UPDATE_REQ;

  /* Start signaling request timer and store handle */
  WsfTimerStartSec(&l2cSlaveCb.reqTimer, L2C_SIG_REQ_TIMEOUT);
  l2cSlaveCb.reqTimer.msg.param = handle;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_CONN_UPDATE_REQ_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_CONN_UPDATE_REQ);       /* command code */
    UINT8_TO_BSTREAM(p, l2cCb.identifier);              /* identifier */
    l2cSlaveCb.signId[handle] = l2cCb.identifier;
    l2cCb.identifier = L2C_NEXT_ID(l2cCb.identifier);
    UINT16_TO_BSTREAM(p, L2C_SIG_CONN_UPDATE_REQ_LEN);  /* parameter length */
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMin);   /* interval min */
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMax);   /* interval max */
    UINT16_TO_BSTREAM(p, pConnSpec->connLatency);       /* slave latency */
    UINT16_TO_BSTREAM(p, pConnSpec->supTimeout);        /* timeout multiplier */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_CONN_UPDATE_REQ_LEN), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Event handler initialization function for L2C when operating as a slave.
 *
 *  \param  handlerId  ID for this event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveHandlerInit(wsfHandlerId_t handlerId)
{
  l2cSlaveCb.reqTimer.msg.event = L2C_MSG_REQ_TIMEOUT;
  l2cSlaveCb.reqTimer.handlerId = handlerId;
  l2cSlaveCb.handlerId = handlerId;
}

/*************************************************************************************************/
/*!
 *  \brief  The WSF event handler for L2C when operating as a slave.
 *
 *  \param  event   Event mask.
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  /* Handle message */
  if (pMsg != NULL)
  {
    WSF_ASSERT(pMsg->event > 0 && pMsg->event <= L2C_MSG_TYPE_MAX);

    /* handle slave signaling request timeout */
    if (pMsg->event == L2C_MSG_REQ_TIMEOUT)
    {
      l2cSlaveReqTimeout(pMsg);
    }
  }
  /* Handle events */
  else if (event)
  {

  }
}

