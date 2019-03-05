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
 *  \brief Device manager connection management module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "util/bstream.h"
#include "dm_api.h"
#include "dm_dev.h"
#include "dm_main.h"
#include "dm_conn.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Translate HCI event to state machine message */
#define DM_CONN_HCI_EVT_2_MSG(evt)  (DM_CONN_MSG_HCI_DISCONNECT_CMPL - 3 + (evt))

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Conn spec defaults */
static const hciConnSpec_t dmConnSpecDefaults =
{
  DM_GAP_INITIAL_CONN_INT_MIN,
  DM_GAP_INITIAL_CONN_INT_MAX,
  DM_GAP_CONN_EST_LATENCY,
  DM_DEFAULT_EST_SUP_TIMEOUT,
  DM_GAP_CONN_EST_MIN_CE_LEN,
  DM_GAP_CONN_EST_MAX_CE_LEN
};

/* Action set for this module */
static const dmConnAct_t dmConnActSetMain[] =
{
  dmConnSmActNone,
  dmConnSmActClose,
  dmConnSmActConnOpened,
  dmConnSmActConnFailed,
  dmConnSmActConnClosed,
  dmConnSmActHciUpdated
};

/* Component function interface */
static const dmFcnIf_t dmConnFcnIf =
{
  dmConnReset,
  dmConnHciHandler,
  dmConnMsgHandler
};

/* Component 2 function interface */
static const dmFcnIf_t dmConn2FcnIf =
{
  dmEmptyReset,
  dmConn2HciHandler,
  dmConn2MsgHandler
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Control block */
dmConnCb_t dmConnCb;

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
static void dmConn2ActRssiRead(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActRemoteConnParamReq(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActDataLenChange(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActWriteAuthToCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActAuthToExpired(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActReadRemoteFeaturesCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmConn2ActReadRemoteVerInfoCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent);

/*************************************************************************************************/
/*!
 *  \brief  Return the CCB with particular state conditions.
 *
 *  \return Pointer to CCB or NULL if failure.
 */
/*************************************************************************************************/
static dmConnCcb_t *dmConnCmplStates(void)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i;

  /* if there's a ccb in accepting state */
  for (i = DM_CONN_MAX; i > 0; i--, pCcb++)
  {
    /* look for connection in accepting state or disconnecting state, cancelled connection */
    if (pCcb->inUse &&
        ((pCcb->state == DM_CONN_SM_ST_ACCEPTING) ||
         ((pCcb->state == DM_CONN_SM_ST_DISCONNECTING) && (pCcb->handle == DM_CONN_HCI_HANDLE_NONE))))
    {
      DM_TRACE_INFO1("dmConnCmplStates %d", pCcb->connId);
      return pCcb;
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a DM connection control block.
 *
 *  \param  pAddr   BD address for connection.
 *
 *  \return Pointer to CCB or NULL if failure.
 */
/*************************************************************************************************/
dmConnCcb_t *dmConnCcbAlloc(uint8_t *pAddr)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i;

  for (i = 0; i < DM_CONN_MAX; i++, pCcb++)
  {
    if (pCcb->inUse == FALSE)
    {
      memset(pCcb, 0, sizeof(dmConnCcb_t));

      BdaCpy(pCcb->peerAddr, pAddr);
      pCcb->handle = DM_CONN_HCI_HANDLE_NONE;
      pCcb->connId = i + 1;
      pCcb->updating = FALSE;
      pCcb->inUse = TRUE;
      pCcb->featuresPresent = FALSE;

      DM_TRACE_ALLOC1("dmConnCcbAlloc %d", pCcb->connId);

      return pCcb;
    }
  }

  DM_TRACE_ERR0("dmConnCcbAlloc failed");

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Deallocate a DM connection control block.
 *
 *  \param  pCcb  Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnCcbDealloc(dmConnCcb_t *pCcb)
{
  DM_TRACE_FREE1("dmConnCcbDealloc %d", pCcb->connId);

  pCcb->inUse = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Find a connection control block with matching handle.
 *
 *  \param  handle  Handle to find.
 *
 *  \return Pointer to CCB or NULL if failure.
 */
/*************************************************************************************************/
dmConnCcb_t *dmConnCcbByHandle(uint16_t handle)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i;

  for (i = DM_CONN_MAX; i > 0; i--, pCcb++)
  {
    if (pCcb->inUse && (pCcb->handle == handle))
    {
      return pCcb;
    }
  }

  DM_TRACE_WARN1("dmConnCcbByHandle not found 0x%04x", handle);

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Find a connection control block with BD address.
 *
 *  \param  pAddr  BD address to find.
 *
 *  \return Pointer to CCB or NULL if failure.
 */
/*************************************************************************************************/
dmConnCcb_t *dmConnCcbByBdAddr(uint8_t *pAddr)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i;

  for (i = DM_CONN_MAX; i > 0; i--, pCcb++)
  {
    if (pCcb->inUse && BdaCmp(pCcb->peerAddr, pAddr))
    {
      return pCcb;
    }
  }

  DM_TRACE_INFO0("dmConnIdByBdAddr not found");

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection control block for the given connection ID.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Pointer to CCB or NULL if failure.
 */
/*************************************************************************************************/
dmConnCcb_t *dmConnCcbById(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  connId--;
  if (dmConnCb.ccb[connId].inUse)
  {
    return &dmConnCb.ccb[connId];
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the number of active connections.
 *
 *  \return Number of connections.
 */
/*************************************************************************************************/
uint8_t dmConnNum(void)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i, j;

  for (i = DM_CONN_MAX, j = 0; i > 0; i--, pCcb++)
  {
    if (pCcb->inUse)
    {
      j++;
    }
  }

  return j;
}

/*************************************************************************************************/
/*!
 *  \brief  Execute all registered DM connection callbacks.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnExecCback(dmConnMsg_t *pMsg)
{
  uint8_t i;

  for (i = 0; i < DM_CLIENT_ID_MAX; i++)
  {
    if (dmConnCb.connCback[i] != NULL)
    {
      (*dmConnCb.connCback[i])((dmEvt_t *) pMsg);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function is called to open or accept a connection.
 *
 *  \param  clientId     The client identifier.
 *  \param  initPhys     Initiator PHYs.
 *  \param  advHandle    Advertising handle.
 *  \param  advType      Advertising type.
 *  \param  duration     Advertising duration (in ms).
 *  \param  maxEaEvents  Maximum number of extended advertising events.
 *  \param  addrType     Address type.
 *  \param  pAddr        Peer device address.
 *  \param  role         Device role.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t dmConnOpenAccept(uint8_t clientId, uint8_t initPhys, uint8_t advHandle, uint8_t advType,
                            uint16_t duration, uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr,
                            uint8_t role)
{
  dmConnCcb_t           *pCcb = NULL;
  dmConnApiOpen_t       *pMsg;

  /* make sure ccb not already allocated */
  WsfTaskLock();
  if ((pCcb = dmConnCcbByBdAddr(pAddr)) == NULL)
  {
    /* allocate ccb */
    pCcb = dmConnCcbAlloc(pAddr);
  }
  WsfTaskUnlock();

  if (pCcb != NULL)
  {
    if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiOpen_t))) != NULL)
    {
      pMsg->hdr.param = pCcb->connId;
      pMsg->hdr.event = (role == DM_ROLE_MASTER) ?
                        DM_CONN_MSG_API_OPEN : DM_CONN_MSG_API_ACCEPT;
      pMsg->initPhys = initPhys;
      pMsg->advHandle = advHandle;
      pMsg->advType = advType;
      pMsg->duration = duration;
      pMsg->maxEaEvents = maxEaEvents;
      BdaCpy(pMsg->peerAddr, pAddr);
      pMsg->addrType = addrType;
      pMsg->clientId = clientId;
      WsfMsgSend(dmCb.handlerId, pMsg);

      /* set role */
      WsfTaskLock();
      pCcb->role = role;
      WsfTaskUnlock();

      /* return connection id */
      return pCcb->connId;
    }
    else
    {
      WsfTaskLock();
      dmConnCcbDealloc(pCcb);
      WsfTaskUnlock();
    }
  }

  /* open failed */
  return DM_CONN_ID_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  Empty action.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActNone(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  return;
}

/*************************************************************************************************/
/*!
 *  \brief  Close a connection.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActClose(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  HciDisconnectCmd(pCcb->handle, pMsg->apiClose.reason);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a connection complete event from HCI.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActConnOpened(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* store peer address, handle, and role */
  BdaCpy(pCcb->peerAddr, pMsg->hciLeConnCmpl.peerAddr);
  pCcb->handle = pMsg->hciLeConnCmpl.handle;
  pCcb->peerAddrType = DmHostAddrType(pMsg->hciLeConnCmpl.addrType);
  pCcb->role = pMsg->hciLeConnCmpl.role;

  /* set local address type of connection */
  if (pCcb->role == DM_ROLE_MASTER)
  {
    pCcb->localAddrType = dmCb.connAddrType;
  }
  else
  {
    pCcb->localAddrType = dmCb.advAddrType;
  }

  /* set local address of connection */
  if (pCcb->localAddrType == DM_ADDR_PUBLIC)
  {
    BdaCpy(pCcb->localAddr, HciGetBdAddr());
  }
  else
  {
    BdaCpy(pCcb->localAddr, dmCb.localAddr);
  }

  /* store enhanced fields */
  BdaCpy(pCcb->localRpa, pMsg->hciLeConnCmpl.localRpa);
  BdaCpy(pCcb->peerRpa, pMsg->hciLeConnCmpl.peerRpa);

  /* initialize idle state */
  pCcb->idleMask = 0;

  /* if central */
  if (pCcb->role == DM_ROLE_MASTER)
  {
    /* pass connection initiation completed to dev priv */
    dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_CTRL, DM_DEV_PRIV_MSG_CONN_INIT_STOP, 0 , 0);

    /* if first connection opened */
    if (dmConnNum() == 1)
    {
      /* pass conn open event to dev priv */
      dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_RPA_START, DM_CONN_OPEN_IND, 0, 0);
    }
  }

  /* pass conn open event to Connection CTE */
  dmDevPassEvtToConnCte(DM_CONN_OPEN_IND, pCcb->connId);

  pMsg->hdr.event = DM_CONN_OPEN_IND;
  dmConnExecCback(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a connection complete failure event from HCI.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActConnFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* deallocate ccb */
  dmConnCcbDealloc(pCcb);

  /* if central */
  if (pCcb->role == DM_ROLE_MASTER)
  {
    /* pass connection initiation completed to dev priv */
    dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_CTRL, DM_DEV_PRIV_MSG_CONN_INIT_STOP, 0, 0);

    /* if last connection closed */
    if (dmConnNum() == 0)
    {
      /* pass conn close event to dev priv */
      dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_RPA_STOP, DM_CONN_CLOSE_IND, 0 , 0);
    }
  }

  pMsg->hdr.event = DM_CONN_CLOSE_IND;
  pMsg->hciLeConnCmpl.handle = pMsg->hciLeConnCmpl.role = 0;
  dmConnExecCback(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a disconnect complete event from HCI.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActConnClosed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* pass conn close event to Connection CTE */
  dmDevPassEvtToConnCte(DM_CONN_CLOSE_IND, pCcb->connId);

  /* deallocate ccb */
  dmConnCcbDealloc(pCcb);

  /* if central and last connection closed */
  if ((pCcb->role == DM_ROLE_MASTER) && (dmConnNum() == 0))
  {
    /* pass conn close event to dev priv */
    dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_RPA_STOP, DM_CONN_CLOSE_IND, 0, 0);
  }

  pMsg->hdr.event = DM_CONN_CLOSE_IND;
  dmConnExecCback(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a connection update event from HCI.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActHciUpdated(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* call callback */
  pMsg->hdr.event = DM_CONN_UPDATE_IND;
  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Reset the scan module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnReset(void)
{
  dmConnCcb_t             *pCcb = dmConnCb.ccb;
  hciDisconnectCmplEvt_t  disconnectCmpl;
  uint8_t                 i;

  /* generate HCI disconnect complete event */
  disconnectCmpl.hdr.event = HCI_DISCONNECT_CMPL_CBACK_EVT;
  disconnectCmpl.hdr.status = disconnectCmpl.status = HCI_SUCCESS;
  disconnectCmpl.reason = HCI_ERR_LOCAL_TERMINATED;

  for (i = DM_CONN_MAX; i > 0; i--, pCcb++)
  {
    if (pCcb->inUse)
    {
      /* set connection id */
      disconnectCmpl.hdr.param = disconnectCmpl.handle = pCcb->handle;

      /* handle the event */
      dmConnHciHandler((hciEvt_t *) &disconnectCmpl);
    }
  }

  /* initialize control block */
  for (i = 0; i < DM_NUM_PHYS; i++)
  {
    dmConnCb.scanInterval[i] = DM_GAP_SCAN_FAST_INT_MIN;
    dmConnCb.scanWindow[i] = DM_GAP_SCAN_FAST_WINDOW;
    dmConnCb.connSpec[i] = dmConnSpecDefaults;
  }

  dmCb.initFiltPolicy = HCI_FILT_NONE;
  dmCb.connAddrType = DM_ADDR_PUBLIC;
}

/*************************************************************************************************/
/*!
 *  \brief  DM Conn WSF message handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnMsgHandler(wsfMsgHdr_t *pMsg)
{
  dmConnCcb_t *pCcb;

  /* look up ccb from conn id */
  if ((pCcb = dmConnCcbById((dmConnId_t) pMsg->param)) != NULL)
  {
    /* execute state machine */
    dmConnSmExecute(pCcb, (dmConnMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM Conn HCI callback event handler.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnHciHandler(hciEvt_t *pEvent)
{
  dmConnCcb_t *pCcb;

  /* handle special cases for connection complete event */
  if ((pEvent->hdr.event == HCI_LE_CONN_CMPL_CBACK_EVT) ||
      (pEvent->hdr.event == HCI_LE_ENHANCED_CONN_CMPL_CBACK_EVT))
  {
    /* first check if ccb exists for this bd addr */
    if ((pCcb = dmConnCcbByBdAddr(pEvent->leConnCmpl.peerAddr)) == NULL)
    {
      /* check for special case states */
      if ((pCcb = dmConnCmplStates()) == NULL)
      {
        /* else default case for slave, allocate new ccb */
        if ((pEvent->hdr.status == HCI_SUCCESS) && (pEvent->leConnCmpl.role == HCI_ROLE_SLAVE))
        {
          pCcb = dmConnCcbAlloc(pEvent->leConnCmpl.peerAddr);
        }
      }
    }

    /* translate HCI event to state machine event */
    if (pEvent->hdr.status == HCI_SUCCESS)
    {
      pEvent->hdr.event =  DM_CONN_MSG_HCI_LE_CONN_CMPL;
    }
    else
    {
      pEvent->hdr.event = DM_CONN_MSG_HCI_LE_CONN_CMPL_FAIL;
    }
  }
  else
  {
    pCcb = dmConnCcbByHandle(pEvent->hdr.param);

    /* translate HCI event to state machine message */
    pEvent->hdr.event = DM_CONN_HCI_EVT_2_MSG(pEvent->hdr.event);
  }

  /* if ccb found */
  if (pCcb != NULL)
  {
    /* set conn id */
    pEvent->hdr.param = pCcb->connId;

    /* execute state machine */
    dmConnSmExecute(pCcb, (dmConnMsg_t *) pEvent);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM Conn 2 WSF message handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConn2MsgHandler(wsfMsgHdr_t *pMsg)
{
  dmConnCcb_t *pCcb;

  /* look up ccb from conn id */
  if ((pCcb = dmConnCcbById((dmConnId_t) pMsg->param)) != NULL)
  {
    dmConn2Msg_t *pConn2Msg = (dmConn2Msg_t *) pMsg;

    /* handle incoming message */
    switch (pMsg->event)
    {
      case DM_CONN_MSG_API_READ_RSSI:
        HciReadRssiCmd(pCcb->handle);
        break;

      case DM_CONN_MSG_API_REM_CONN_PARAM_REQ_REPLY:
        {
          hciConnSpec_t *pConnSpec = &pConn2Msg->apiRemConnParamReqReply.connSpec;

          HciLeRemoteConnParamReqReply(pCcb->handle, pConnSpec->connIntervalMin,
                                       pConnSpec->connIntervalMax, pConnSpec->connLatency,
                                       pConnSpec->supTimeout, pConnSpec->minCeLen,
                                       pConnSpec->maxCeLen);
        }
        break;

      case DM_CONN_MSG_API_REM_CONN_PARAM_REQ_NEG_REPLY:
        HciLeRemoteConnParamReqNegReply(pCcb->handle, pConn2Msg->apiRemConnParamReqNegReply.reason);
        break;

      case DM_CONN_MSG_API_SET_DATA_LEN:
        {
          dmConnApiSetDataLen_t *pDataLen = &pConn2Msg->apiSetDataLen;

          HciLeSetDataLen(pCcb->handle, pDataLen->txOctets, pDataLen->txTime);
        }
        break;

      case DM_CONN_MSG_API_WRITE_AUTH_TO:
        HciWriteAuthPayloadTimeout(pCcb->handle, pConn2Msg->apiWriteAuthPayloadTo.timeout);
        break;

      default:
        /* should never get here */
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM Conn 2 HCI callback event handler.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConn2HciHandler(hciEvt_t *pEvent)
{
  dmConnCcb_t *pCcb;

  /* look up ccb from conn handle */
  if ((pCcb = dmConnCcbByHandle(pEvent->hdr.param)) != NULL)
  {
    /* handle incoming event */
    switch (pEvent->hdr.event)
    {
      case HCI_READ_RSSI_CMD_CMPL_CBACK_EVT:
        dmConn2ActRssiRead(pCcb, pEvent);
        break;

      case HCI_LE_REM_CONN_PARAM_REQ_CBACK_EVT:
        dmConn2ActRemoteConnParamReq(pCcb, pEvent);
        break;

      case HCI_LE_DATA_LEN_CHANGE_CBACK_EVT:
        dmConn2ActDataLenChange(pCcb, pEvent);
        break;

      case HCI_WRITE_AUTH_PAYLOAD_TO_CMD_CMPL_CBACK_EVT:
        dmConn2ActWriteAuthToCmpl(pCcb, pEvent);
        break;

      case HCI_AUTH_PAYLOAD_TO_EXPIRED_CBACK_EVT:
        dmConn2ActAuthToExpired(pCcb, pEvent);
        break;

      case HCI_LE_READ_REMOTE_FEAT_CMPL_CBACK_EVT:
        dmConn2ActReadRemoteFeaturesCmpl(pCcb, pEvent);
        break;

      case HCI_READ_REMOTE_VER_INFO_CMPL_CBACK_EVT:
        dmConn2ActReadRemoteVerInfoCmpl(pCcb, pEvent);
        break;

      default:
        /* should never get here */
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a read RSSI event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActRssiRead(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciReadRssiCmdCmplEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_CONN_READ_RSSI_IND;
  evt.hdr.param = pCcb->connId;
  evt.status = evt.hdr.status = (uint8_t) pEvent->readRssiCmdCmpl.status;
  evt.handle = pCcb->handle;
  evt.rssi = pEvent->readRssiCmdCmpl.rssi;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a remote connection parameter request event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActRemoteConnParamReq(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciLeRemConnParamReqEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_REM_CONN_PARAM_REQ_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;
  evt.handle = pCcb->handle;
  evt.intervalMin = pEvent->leRemConnParamReq.intervalMin;
  evt.intervalMax = pEvent->leRemConnParamReq.intervalMax;
  evt.latency = pEvent->leRemConnParamReq.latency;
  evt.timeout = pEvent->leRemConnParamReq.timeout;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a data length change event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActDataLenChange(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciLeDataLenChangeEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_CONN_DATA_LEN_CHANGE_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;
  evt.handle = pCcb->handle;
  evt.maxTxOctets = pEvent->leDataLenChange.maxTxOctets;
  evt.maxTxTime = pEvent->leDataLenChange.maxTxTime;
  evt.maxRxOctets = pEvent->leDataLenChange.maxRxOctets;
  evt.maxRxTime = pEvent->leDataLenChange.maxRxTime;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a Write Authenticated Payload Timeout Command Complete event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActWriteAuthToCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciWriteAuthPayloadToCmdCmplEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_CONN_WRITE_AUTH_TO_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;
  evt.handle = pEvent->writeAuthPayloadToCmdCmpl.handle;
  evt.status = pEvent->writeAuthPayloadToCmdCmpl.status;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an Authenticated Payload Timeout Expired event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActAuthToExpired(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciAuthPayloadToExpiredEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_CONN_AUTH_TO_EXPIRED_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;
  evt.handle = pEvent->authPayloadToExpired.handle;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an Authenticated Payload Timeout Expired event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActReadRemoteFeaturesCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciLeReadRemoteFeatCmplEvt_t evt;

  /* Save the features */
  BYTES_TO_UINT32(pCcb->features, pEvent->leReadRemoteFeatCmpl.features);
  pCcb->featuresPresent = TRUE;

  /* call callback */
  evt.hdr.event = DM_REMOTE_FEATURES_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;

  evt.status = pEvent->leReadRemoteFeatCmpl.status;
  evt.handle = pEvent->leReadRemoteFeatCmpl.handle;
  memcpy(evt.features, pEvent->leReadRemoteFeatCmpl.features, sizeof(evt.features));

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an Authenticated Payload Timeout Expired event from HCI.
 *
 *  \param  pCcb    Connection control block.
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConn2ActReadRemoteVerInfoCmpl(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciReadRemoteVerInfoCmplEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_READ_REMOTE_VER_INFO_IND;
  evt.hdr.param = pCcb->connId;
  evt.hdr.status = HCI_SUCCESS;

  evt.status = pEvent->readRemoteVerInfoCmpl.status;
  evt.handle = pEvent->readRemoteVerInfoCmpl.handle;
  evt.version = pEvent->readRemoteVerInfoCmpl.version;
  evt.mfrName = pEvent->readRemoteVerInfoCmpl.mfrName;
  evt.subversion = pEvent->readRemoteVerInfoCmpl.subversion;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnInit(void)
{
  dmFcnIfTbl[DM_ID_CONN] = (dmFcnIf_t *) &dmConnFcnIf;
  dmFcnIfTbl[DM_ID_CONN_2] = (dmFcnIf_t *) &dmConn2FcnIf;
  dmConnActSet[DM_CONN_ACT_SET_MAIN] = (dmConnAct_t *) dmConnActSetMain;
}

/*************************************************************************************************/
/*!
 *  \brief  register with the DM connection manager.
 *
 *  \param  clientId  The client identifier.
 *  \param  cback     Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnRegister(uint8_t clientId, dmCback_t cback)
{
  WSF_ASSERT(clientId < DM_CLIENT_ID_MAX);

  /* store callback */
  WsfTaskLock();
  dmConnCb.connCback[clientId] = cback;
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Close the connection with the give connection identifier.
 *
 *  \param  clientId  The client identifier.
 *  \param  connId    Connection identifier.
 *  \param  reason    Reason connection is being closed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnClose(uint8_t clientId, dmConnId_t connId, uint8_t reason)
{
  dmConnApiClose_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiClose_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_CLOSE;
    pMsg->hdr.param = connId;
    pMsg->hdr.status = pMsg->reason = reason;
    pMsg->clientId = clientId;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Read the features of the remote device.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmReadRemoteFeatures(dmConnId_t connId)
{
  dmConnCcb_t *pCcb;

  /* look up ccb from conn handle */
  if ((pCcb = dmConnCcbById(connId)) != NULL)
  {
    if (pCcb->featuresPresent)
    {
      hciLeReadRemoteFeatCmplEvt_t evt;
      uint8_t *p = evt.features;

      /* call callback */
      evt.hdr.event = DM_REMOTE_FEATURES_IND;
      evt.hdr.param = pCcb->connId;
      evt.hdr.status = HCI_SUCCESS;

      evt.status = HCI_SUCCESS;
      evt.handle = pCcb->handle;
      UINT32_TO_BSTREAM(p, pCcb->features);

      (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
    }
    else
    {
      /* Request the remote features from the peer */
      HciLeReadRemoteFeatCmd(pCcb->handle);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Read the version info of the remote device.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmReadRemoteVerInfo(dmConnId_t connId)
{
  dmConnCcb_t *pCcb;

  /* look up ccb from conn handle */
  if ((pCcb = dmConnCcbById(connId)) != NULL)
  {
    /* Request the version info from the peer */
    HciReadRemoteVerInfoCmd(pCcb->handle);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Update the connection parameters of an open connection
 *
 *  \param  connId      Connection identifier.
 *  \param  pConnSpec   Connection specification.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnUpdate(dmConnId_t connId, hciConnSpec_t *pConnSpec)
{
  dmConnApiUpdate_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiUpdate_t))) != NULL)
  {
    pMsg->hdr.event = (DmConnRole(connId) == DM_ROLE_MASTER) ?
                      DM_CONN_MSG_API_UPDATE_MASTER : DM_CONN_MSG_API_UPDATE_SLAVE;
    pMsg->hdr.param = connId;
    memcpy(&pMsg->connSpec, pConnSpec, sizeof(hciConnSpec_t));

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the connection spec parameters for created connections created with DmConnOpen().
 *
 *  \param  initPhy     The initiator PHY.
 *  \param  pConnSpec   Connection spec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConnSetConnSpec(uint8_t initPhy, hciConnSpec_t *pConnSpec)
{
  WSF_ASSERT((initPhy == HCI_INIT_PHY_LE_1M_BIT) ||
             (initPhy == HCI_INIT_PHY_LE_2M_BIT) ||
             (initPhy == HCI_INIT_PHY_LE_CODED_BIT));

  WsfTaskLock();
  dmConnCb.connSpec[DmInitPhyToIdx(initPhy)] = *pConnSpec;
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for created connections created with DmConnOpen().
 *
 *  \param  initPhy       The initiator PHY.
 *  \param  scanInterval  The scan interval.
 *  \param  scanWindow    The scan window.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConnSetScanInterval(uint8_t initPhy, uint16_t scanInterval, uint16_t scanWindow)
{
  uint8_t phyIdx;

  WSF_ASSERT((initPhy == HCI_INIT_PHY_LE_1M_BIT) || (initPhy == HCI_INIT_PHY_LE_CODED_BIT));

  WsfTaskLock();
  phyIdx = DmInitPhyToIdx(initPhy);
  dmConnCb.scanInterval[phyIdx] = scanInterval;
  dmConnCb.scanWindow[phyIdx] = scanWindow;
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for connections to be created with DmConnOpen().
 *
 *  \param  scanInterval  The scan interval.
 *  \param  scanWindow    The scan window.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetScanInterval(uint16_t scanInterval, uint16_t scanWindow)
{
  dmConnSetScanInterval(HCI_INIT_PHY_LE_1M_BIT, scanInterval, scanWindow);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the scan interval and window for extended connections to be created with
 *          DmConnOpen().
 *
 *  \param  initPhy       Initiator PHYs.
 *  \param  pScanInterval Scan interval array.
 *  \param  pScanWindow   Scan window array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSetScanInterval(uint8_t initPhys, uint16_t *pScanInterval, uint16_t *pScanWindow)
{
  uint8_t i;   /* initPhy bit position */
  uint8_t idx; /* param array index */

  for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
  {
    if (initPhys & (1 << i))
    {
      dmConnSetScanInterval((1 << i), pScanInterval[idx], pScanWindow[idx]);
      idx++;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the connection spec parameters for connections to be created with DmConnOpen().
 *
 *  \param  pConnSpec   Connection spec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetConnSpec(hciConnSpec_t *pConnSpec)
{
  dmConnSetConnSpec(HCI_INIT_PHY_LE_1M_BIT, pConnSpec);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the extended connection spec parameters for extended connections to be created
 *          with DmConnOpen().
 *
 *  \param  initPhys    The initiator PHYs.
 *  \param  pConnSpec   Connection spec parameters array.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSetConnSpec(uint8_t initPhys, hciConnSpec_t *pConnSpec)
{
  uint8_t i;   /* initPhy bit position */
  uint8_t idx; /* param array index */

  for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
  {
    if (initPhys & (1 << i))
    {
      dmConnSetConnSpec((1 << i), &(pConnSpec[idx]));
      idx++;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Read RSSI of a given connection.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnReadRssi(dmConnId_t connId)
{
  dmConnApiReadRssi_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiReadRssi_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_READ_RSSI;
    pMsg->hdr.param = connId;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
*  \brief  Reply to the HCI remote connection parameter request event.  This command is used to
*          indicate that the Host has accepted the remote device's request to change connection
*          parameters.
*
*  \param  connId      Connection identifier.
*  \param  pConnSpec   Connection specification.
*
*  \return None.
*/
/*************************************************************************************************/
void DmRemoteConnParamReqReply(dmConnId_t connId, hciConnSpec_t *pConnSpec)
{
  dmConnApiRemConnParamReqReply_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiRemConnParamReqReply_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_REM_CONN_PARAM_REQ_REPLY;
    pMsg->hdr.param = connId;
    memcpy(&pMsg->connSpec, pConnSpec, sizeof(hciConnSpec_t));

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
*  \brief  Negative reply to the HCI remote connection parameter request event.  This command
*          is used to indicate that the Host has rejected the remote device's request to change
*          connection parameters.
*
*  \param  connId      Connection identifier.
*  \param  reason      Reason for rejection.
*
*  \return None.
*/
/*************************************************************************************************/
void DmRemoteConnParamReqNegReply(dmConnId_t connId, uint8_t reason)
{
  dmConnApiRemConnParamReqNegReply_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiRemConnParamReqNegReply_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_REM_CONN_PARAM_REQ_NEG_REPLY;
    pMsg->hdr.param = connId;
    pMsg->reason = reason;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
*  \brief  Set data length for a given connection.
*
*  \param  connId      Connection identifier.
*  \param  txOctets    Maximum number of payload octets for a Data PDU.
*  \param  txTime      Maximum number of microseconds for a Data PDU.
*
*  \return None.
*/
/*************************************************************************************************/
void DmConnSetDataLen(dmConnId_t connId, uint16_t txOctets, uint16_t txTime)
{
  dmConnApiSetDataLen_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiSetDataLen_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_SET_DATA_LEN;
    pMsg->hdr.param = connId;
    pMsg->txOctets = txOctets;
    pMsg->txTime = txTime;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
*  \brief  Set authenticated payload timeout for a given connection.
*
*  \param  connId      Connection identifier.
*  \param  timeout     Timeout period in units of 10ms
*
*  \return None.
*/
/*************************************************************************************************/
void DmWriteAuthPayloadTimeout(dmConnId_t connId, uint16_t timeout)
{
  dmConnApiWriteAuthPayloadTo_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmConnApiWriteAuthPayloadTo_t))) != NULL)
  {
    pMsg->hdr.event = DM_CONN_MSG_API_WRITE_AUTH_TO;
    pMsg->hdr.param = connId;
    pMsg->timeout = timeout;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief   For internal use only.  Find the connection ID with matching handle.
 *
 *  \param  handle  Handle to find.
 *
 *  \return Connection ID or DM_CONN_ID_NONE if error.
 */
/*************************************************************************************************/
dmConnId_t DmConnIdByHandle(uint16_t handle)
{
  dmConnCcb_t   *pCcb = dmConnCb.ccb;
  uint8_t       i;

  for (i = DM_CONN_MAX; i > 0; i--, pCcb++)
  {
    if (pCcb->inUse && (pCcb->handle == handle))
    {
      return pCcb->connId;
    }
  }

  DM_TRACE_WARN1("DmConnIdByHandle not found 0x%04x", handle);

  return DM_CONN_ID_NONE;
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return TRUE if the connection is in use.
 *
 *  \param  connId  Connection ID.
 *
 *  \return TRUE if the connection is in use, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmConnInUse(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].inUse;
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the peer address type.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Peer address type.
 */
/*************************************************************************************************/
uint8_t DmConnPeerAddrType(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].peerAddrType;
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the peer device address.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Pointer to peer device address.
 */
/*************************************************************************************************/
uint8_t *DmConnPeerAddr(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].peerAddr;
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the local address type.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Local address type.
 */
/*************************************************************************************************/
uint8_t DmConnLocalAddrType(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].localAddrType;
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Return the local address.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Pointer to local address.
 */
/*************************************************************************************************/
uint8_t *DmConnLocalAddr(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].localAddr;
}

/*************************************************************************************************/
/*!
*  \brief  For internal use only.  Return the peer resolvable private address (RPA).
*
*  \param  connId  Connection ID.
*
*  \return Pointer to peer RPA.
*/
/*************************************************************************************************/
uint8_t *DmConnPeerRpa(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId - 1].peerRpa;
}

/*************************************************************************************************/
/*!
*  \brief  For internal use only.  Return the local resolvable private address (RPA).
*
*  \param  connId  Connection ID.
*
*  \return Pointer to local RPA.
*/
/*************************************************************************************************/
uint8_t *DmConnLocalRpa(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId - 1].localRpa;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the security level of the connection.
 *
 *  \param  connId  Connection ID.
 *
 *  \return Security level of the connection.
 */
/*************************************************************************************************/
uint8_t DmConnSecLevel(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].secLevel;
}

/*************************************************************************************************/
/*!
 *  \brief  Configure a bit in the connection idle state mask as busy or idle.
 *
 *  \param  connId      Connection identifier.
 *  \param  idleMask    Bit in the idle state mask to configure.
 *  \param  idle        DM_CONN_BUSY or DM_CONN_IDLE.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetIdle(dmConnId_t connId, uint16_t idleMask, uint8_t idle)
{
  WsfTaskLock();

  if (DmConnInUse(connId))
  {
    if (idle == DM_CONN_IDLE)
    {
      /* clear bit if idle */
      dmConnCb.ccb[connId-1].idleMask &= ~idleMask;
    }
    else
    {
      /* set bit if busy */
      dmConnCb.ccb[connId-1].idleMask |= idleMask;
    }
  }

  WsfTaskUnlock();

  DM_TRACE_INFO2("connId=%d idleMask=0x%04x", connId, dmConnCb.ccb[connId-1].idleMask);
}

/*************************************************************************************************/
/*!
 *  \brief  Check if a connection is idle.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return Zero if connection is idle, nonzero if busy.
 */
/*************************************************************************************************/
uint16_t DmConnCheckIdle(dmConnId_t connId)
{
  uint16_t idleMask;

  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  WsfTaskLock();
  idleMask = dmConnCb.ccb[connId-1].idleMask;
  WsfTaskUnlock();

  return idleMask;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection role indicating master or slave.
 *
 *  \param  connId      Connection identifier.
 *
 *  \return Connection role.
 */
/*************************************************************************************************/
uint8_t DmConnRole(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return dmConnCb.ccb[connId-1].role;
}
