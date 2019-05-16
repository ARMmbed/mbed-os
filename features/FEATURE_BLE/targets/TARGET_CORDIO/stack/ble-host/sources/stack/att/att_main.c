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
 *  \brief ATT main module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* indexes into base UUID for 16-to-128 bit UUID conversion */
#define ATT_BASE_UUID_POS_0      12
#define ATT_BASE_UUID_POS_1      13

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

static uint8_t attBaseUuid[] = {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
                                0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Default component function inteface */
const attFcnIf_t attFcnDefault =
{
  attEmptyDataCback,
  (l2cCtrlCback_t) attEmptyHandler,
  (attMsgHandler_t) attEmptyHandler,
  attEmptyConnCback
};

/* Control block */
attCb_t attCb;

/*************************************************************************************************/
/*!
 *  \brief  L2C data callback for ATT.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attL2cDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t pduType;

  /* parse PDU type */
  pduType = *(pPacket + L2C_PAYLOAD_START);

  /* if from server */
  if ((pduType & ATT_PDU_MASK_SERVER) != 0)
  {
    /* call client data callback */
    (*attCb.pClient->dataCback)(handle, len, pPacket);
  }
  /* else from client */
  else
  {
    /* call server data callback */
    (*attCb.pServer->dataCback)(handle, len, pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2C control callback for ATT.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attL2cCtrlCback(wsfMsgHdr_t *pMsg)
{
  attCcb_t      *pCcb;

  /* get connection control block */
  pCcb = attCcbByConnId((dmConnId_t) pMsg->param);

  /* verify connection is open */
  if (pCcb->connId != DM_CONN_ID_NONE)
  {
    if (pMsg->event == L2C_CTRL_FLOW_DISABLE_IND)
    {
      /* flow disabled */
      pCcb->control |= ATT_CCB_STATUS_FLOW_DISABLED;
    }
    else
    {
      /* flow enabled */
      pCcb->control &= ~ATT_CCB_STATUS_FLOW_DISABLED;

      /* call server control callback */
      (*attCb.pServer->ctrlCback)(pMsg);

      /* check flow again; could be changed recursively */
      if (!(pCcb->control & ATT_CCB_STATUS_FLOW_DISABLED))
      {
        /* call client control callback */
        (*attCb.pClient->ctrlCback)(pMsg);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM connection callback for ATT.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attDmConnCback(dmEvt_t *pDmEvt)
{
  attCcb_t  *pCcb;

  pCcb = attCcbByConnId((dmConnId_t) pDmEvt->hdr.param);

  /* if new connection created */
  if (pDmEvt->hdr.event == DM_CONN_OPEN_IND)
  {
    /* initialize control block before handling event */
    pCcb->handle = pDmEvt->connOpen.handle;
    pCcb->mtu = ATT_DEFAULT_MTU;
    pCcb->connId = (dmConnId_t) pDmEvt->hdr.param;
    pCcb->control = 0;
    pCcb->pPendDbHashRsp = NULL;
  }

  /* if connection has been opened */
  if (pCcb->connId != DM_CONN_ID_NONE)
  {
    /* pass event to server */
    (*attCb.pServer->connCback)(pCcb, pDmEvt);

    /* pass event to client */
    (*attCb.pClient->connCback)(pCcb, pDmEvt);

    /* if connection closed */
    if (pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
    {
      /* clear control block after handling event */
      pCcb->connId = DM_CONN_ID_NONE;

      if (pCcb->pPendDbHashRsp)
      {
        WsfBufFree(pCcb->pPendDbHashRsp);
      }
    }
  }

  /* execute ATT connection callback */
  if (attCb.connCback != NULL)
  {
    (*attCb.connCback)(pDmEvt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  ATT empty event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attEmptyHandler(wsfMsgHdr_t *pMsg)
{
  return;
}

/*************************************************************************************************/
/*!
 *  \brief  Empty connection callback for ATT.
 *
 *  \param  pCcb    ATT control block.
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attEmptyConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt)
{
  return;
}

/*************************************************************************************************/
/*!
 *  \brief  Empty data callback for ATT.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attEmptyDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  return;
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
attCcb_t *attCcbByHandle(uint16_t handle)
{
  dmConnId_t  connId;

  if ((connId = DmConnIdByHandle(handle)) != DM_CONN_ID_NONE)
  {
    return &attCb.ccb[connId - 1];
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection control block for the connection ID.
 *
 *  \param  connId    Connection ID.
 *
 *  \return Pointer to connection control block.
 */
/*************************************************************************************************/
attCcb_t *attCcbByConnId(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return &attCb.ccb[connId - 1];
}

/*************************************************************************************************/
/*!
 *  \brief  Compare a 16 bit UUID to a 128 bit UUID.
 *
 *  \param  pUuid16   Pointer to 16 bit UUID.
 *  \param  pUuid128  Pointer to 128 bit UUID.
 *
 *  \return TRUE of UUIDs match, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t attUuidCmp16to128(const uint8_t *pUuid16, const uint8_t *pUuid128)
{
  attBaseUuid[ATT_BASE_UUID_POS_0] = pUuid16[0];
  attBaseUuid[ATT_BASE_UUID_POS_1] = pUuid16[1];

  return (memcmp(attBaseUuid, pUuid128, ATT_128_UUID_LEN) == 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the attribute protocol MTU of a connection.
 *
 *  \param  pCcb     Connection control block.
 *  \param  peerMtu  Peer MTU.
 *  \param  localMtu Local MTU.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attSetMtu(attCcb_t *pCcb, uint16_t peerMtu, uint16_t localMtu)
{
  uint16_t  mtu;

  /* set negotiated mtu for the connection to the lesser of ours and theirs */
  mtu = WSF_MIN(peerMtu, localMtu);

  /* if current mtu is not the same as the negotiated value */
  if (pCcb->mtu != mtu)
  {
    /* set mtu to the new value */
    pCcb->mtu = mtu;

    /* notify app about the new value */
    attExecCallback(pCcb->connId, ATT_MTU_UPDATE_IND, 0, ATT_SUCCESS, mtu);
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
 *  \param  mtu     Negotiated MTU value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attExecCallback(dmConnId_t connId, uint8_t event, uint16_t handle, uint8_t status, uint16_t mtu)
{
  if (attCb.cback)
  {
    attEvt_t evt;

    evt.hdr.param = connId;
    evt.hdr.event = event;
    evt.hdr.status = status;
    evt.valueLen = 0;
    evt.handle = handle;
    evt.continuing = 0;
    evt.mtu = mtu;

    (*attCb.cback)(&evt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate an ATT data message buffer to be used for the ATT attribute protocol messages.
 *
 *  \param  len   Message length in bytes.
 *
 *  \return Pointer to data message buffer or NULL if allocation failed.
 */
/*************************************************************************************************/
void *attMsgAlloc(uint16_t len)
{
  return WsfMsgDataAlloc(len, HCI_TX_DATA_TAILROOM);
}

/*************************************************************************************************/
/*!
 *  \brief  ATT handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID for ATT.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttHandlerInit(wsfHandlerId_t handlerId)
{
  /* store handler ID */
  attCb.handlerId = handlerId;

  /* initialize control block */
  attCb.pClient = &attFcnDefault;
  attCb.pServer = &attFcnDefault;

  /* Register with L2C */
  L2cRegister(L2C_CID_ATT,  attL2cDataCback, attL2cCtrlCback);

  /* Register with DM */
  DmConnRegister(DM_CLIENT_ID_ATT, attDmConnCback);

}

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for ATT.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  /* Handle message */
  if (pMsg != NULL)
  {
    if (pMsg->event >= ATTS_MSG_START)
    {
      /* pass event to server */
      (*attCb.pServer->msgCback)(pMsg);
    }
    else
    {
      /* pass event to client */
      (*attCb.pClient->msgCback)(pMsg);
    }
  }
  /* Handle events */
  else if (event)
  {

  }
}

/*************************************************************************************************/
/*!
 *  \brief  Register a callback with ATT.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttRegister(attCback_t cback)
{
  attCb.cback = cback;

  /* if configured MTU size is larger than maximum RX PDU length */
  if (pAttCfg->mtu > (HciGetMaxRxAclLen() - L2C_HDR_LEN))
  {
    /* notify app about MTU misconfiguration */
    attExecCallback(0, DM_ERROR_IND, 0, DM_ERR_ATT_RX_PDU_LEN_EXCEEDED, 0);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Register a connection callback with ATT.  The callback is typically used to
 *          manage the attribute server database.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttConnRegister(dmCback_t cback)
{
  attCb.connCback = cback;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the attribute protocol MTU of a connection.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return MTU of the connection.
 */
/*************************************************************************************************/
uint16_t AttGetMtu(dmConnId_t connId)
{
  return (attCcbByConnId(connId)->mtu);
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate an ATT message buffer to be sent with the ATT attribute protocol
 *          zero-copy APIs.
 *
 *  \param  len         Message length in bytes.
 *  \param  opcode      Opcode for ATT message.
 *
 *  \return Pointer to message buffer or NULL if allocation failed.
 */
/*************************************************************************************************/
void *AttMsgAlloc(uint16_t len, uint8_t opcode)
{
  uint8_t  *pMsg;
  uint8_t  hdrLen;

  WSF_ASSERT((opcode == ATT_PDU_VALUE_IND) || (opcode == ATT_PDU_VALUE_NTF));

  switch (opcode)
  {
    case ATT_PDU_VALUE_IND:
    case ATT_PDU_VALUE_NTF:
      hdrLen = ATT_VALUE_IND_NTF_BUF_LEN;
      break;

    default:
      hdrLen = 0;
      break;
  }

  if (hdrLen > 0)
  {
    pMsg = attMsgAlloc(hdrLen + len);
    if (pMsg != NULL)
    {
      /* return pointer to attribute value buffer */
      return (pMsg + hdrLen);
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Free an ATT message buffer allocated with AttMsgAlloc().
 *
 *  \param  pMsg        Pointer to message buffer.
 *  \param  opcode      Opcode for ATT message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttMsgFree(void *pMsg, uint8_t opcode)
{
  uint8_t  hdrLen;

  WSF_ASSERT((opcode == ATT_PDU_VALUE_IND) || (opcode == ATT_PDU_VALUE_NTF));

  switch (opcode)
  {
    case ATT_PDU_VALUE_IND:
    case ATT_PDU_VALUE_NTF:
      hdrLen = ATT_VALUE_IND_NTF_BUF_LEN;
      break;

    default:
      hdrLen = 0;
      break;
  }

  WsfMsgFree(((uint8_t *)pMsg) - hdrLen);
}
