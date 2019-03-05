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
 *  \brief L2CAP main module.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "l2c_api.h"
#include "l2c_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Control block */
l2cCb_t l2cCb;

/*************************************************************************************************/
/*!
 *  \brief  Default callback function for unregistered CID.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDefaultDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  L2C_TRACE_WARN0("rcvd data on uregistered cid");
}

/*************************************************************************************************/
/*!
 *  \brief  Default callback function for unregistered CID.
 *
 *  \param  handle    The connection handle.
 *  \param  cid       The L2CAP connection ID.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDefaultDataCidCback(uint16_t handle, uint16_t cid, uint16_t len, uint8_t *pPacket)
{
  L2C_TRACE_WARN1("unknown cid=0x%04x", cid);
}

/*************************************************************************************************/
/*!
 *  \brief  Default L2CAP control callback function.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cDefaultCtrlCback(wsfMsgHdr_t *pMsg)
{
  return;
}

/*************************************************************************************************/
/*!
 *  \brief  Process received L2CAP signaling packets.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cRxSignalingPkt(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t role;
  dmConnId_t connId;

  if ((connId = DmConnIdByHandle(handle)) == DM_CONN_ID_NONE)
  {
    return;
  }

  role = DmConnRole(connId);

  if ((role == DM_ROLE_MASTER) && (l2cCb.masterRxSignalingPkt != NULL))
  {
    (*l2cCb.masterRxSignalingPkt)(handle, len, pPacket);
  }
  else if ((role == DM_ROLE_SLAVE) && (l2cCb.slaveRxSignalingPkt != NULL))
  {
    (*l2cCb.slaveRxSignalingPkt)(handle, len, pPacket);
  }
  else
  {
    L2C_TRACE_ERR1("Invalid role configuration: role=%d", role);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI ACL data callback function.
 *
 *  \param  pPacket   A buffer containing an ACL packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cHciAclCback(uint8_t *pPacket)
{
  uint16_t  handle;
  uint16_t  hciLen;
  uint16_t  cid;
  uint16_t  l2cLen;
  uint8_t   *p = pPacket;

  /* parse HCI handle and length */
  BSTREAM_TO_UINT16(handle, p);
  handle &= HCI_HANDLE_MASK;
  BSTREAM_TO_UINT16(hciLen, p);

  /* parse L2CAP length */
  if (hciLen >= L2C_HDR_LEN)
  {
    BSTREAM_TO_UINT16(l2cLen, p);
  }
  else
  {
    l2cLen = 0;
  }

  /* verify L2CAP length vs HCI length */
  if (hciLen == (l2cLen + L2C_HDR_LEN))
  {
    /* parse CID */
    BSTREAM_TO_UINT16(cid, p);

    switch (cid)
    {
      case L2C_CID_LE_SIGNALING:
        (*l2cCb.l2cSignalingCback)(handle, l2cLen, pPacket);
        break;

      case L2C_CID_ATT:
        (*l2cCb.attDataCback)(handle, l2cLen, pPacket);
        break;

      case L2C_CID_SMP:
        (*l2cCb.smpDataCback)(handle, l2cLen, pPacket);
        break;

      default:
        (*l2cCb.l2cDataCidCback)(handle, cid, l2cLen, pPacket);
        break;
    }
  }
  /* else length mismatch */
  else
  {
    L2C_TRACE_WARN2("length mismatch: l2c=%u hci=%u", l2cLen, hciLen);
  }

  /* deallocate buffer */
  WsfMsgFree(pPacket);
}

/*************************************************************************************************/
/*!
 *  \brief  HCI flow control callback function.
 *
 *  \param  handle        The connection handle.
 *  \param  flowDisabled  TRUE if data flow is disabled.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cHciFlowCback(uint16_t handle, bool_t flowDisabled)
{
  wsfMsgHdr_t hdr;

  L2C_TRACE_INFO2("flowDisabled=%u handle=%u", flowDisabled, handle);

  /* get conn ID for handle */
  if ((hdr.param = DmConnIdByHandle(handle)) != DM_CONN_ID_NONE)
  {
    /* execute higher layer flow control callbacks */
    hdr.event = flowDisabled;
    (*l2cCb.attCtrlCback)(&hdr);
    hdr.event = flowDisabled;
    (*l2cCb.smpCtrlCback)(&hdr);

    /* execute connection oriented channel flow control callback */
    hdr.event = flowDisabled;
    (*l2cCb.l2cCocCtrlCback)(&hdr);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send a command reject message with reason "not understood".
 *
 *  \param  handle      The connection handle.
 *  \param  identifier  Identifier value in received message being rejected.
 *  \param  reason      Why request was rejected.
 *
 *  \return None.
 */
/*************************************************************************************************/
void l2cSendCmdReject(uint16_t handle, uint8_t identifier, uint16_t reason)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_CMD_REJ_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_CMD_REJ);         /* command code */
    UINT8_TO_BSTREAM(p, identifier);              /* identifier */
    UINT16_TO_BSTREAM(p, L2C_SIG_CMD_REJ_LEN);    /* parameter length */
    UINT16_TO_BSTREAM(p, reason);                 /* reason */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_CMD_REJ_LEN), pPacket);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate an L2CAP data message buffer to be used for the L2CAP protocol messages.
 *
 *  \param  len   Message length in bytes.
 *
 *  \return Pointer to data message buffer or NULL if allocation failed.
 */
/*************************************************************************************************/
void *l2cMsgAlloc(uint16_t len)
{
  return WsfMsgDataAlloc(len, HCI_TX_DATA_TAILROOM);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cInit(void)
{
  /* Initialize control block */
  l2cCb.attDataCback = l2cDefaultDataCback;
  l2cCb.smpDataCback = l2cDefaultDataCback;
  l2cCb.l2cSignalingCback = l2cRxSignalingPkt;
  l2cCb.attCtrlCback = l2cDefaultCtrlCback;
  l2cCb.smpCtrlCback = l2cDefaultCtrlCback;
  l2cCb.l2cCocCtrlCback = l2cDefaultCtrlCback;
  l2cCb.l2cDataCidCback = l2cDefaultDataCidCback;
  l2cCb.identifier = 1;

  /* Register with HCI */
  HciAclRegister(l2cHciAclCback, l2cHciFlowCback);
}

/*************************************************************************************************/
/*!
 *  \brief  called by the L2C client, such as ATT or SMP, to register for the given CID.
 *
 *  \param  cid       channel identifier.
 *  \param  dataCback Callback function for L2CAP data received for this CID.
 *  \param  ctrlCback Callback function for control events for this CID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cRegister(uint16_t cid, l2cDataCback_t dataCback, l2cCtrlCback_t ctrlCback)
{
  WSF_ASSERT((cid == L2C_CID_ATT) || (cid == L2C_CID_SMP));

  /* store the callbacks */
  if (cid == L2C_CID_ATT)
  {
    /* registering for attribute protocol */
    l2cCb.attDataCback = dataCback;
    l2cCb.attCtrlCback = ctrlCback;
  }
  else
  {
    /* registering for security manager protocol */
    l2cCb.smpDataCback = dataCback;
    l2cCb.smpCtrlCback = ctrlCback;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send an L2CAP data packet on the given CID.
 *
 *  \param  cid       The channel identifier.
 *  \param  handle    The connection handle.  The client receives this handle from DM.
 *  \param  len       The length of the payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDataReq(uint16_t cid, uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t *p = pPacket;

  /* Set HCI header */
  UINT16_TO_BSTREAM(p, handle);
  UINT16_TO_BSTREAM(p, (len + L2C_HDR_LEN));

  /* Set L2CAP header */
  UINT16_TO_BSTREAM(p, len);
  UINT16_TO_BSTREAM(p, cid);

  /* Send to HCI */
  HciSendAclData(pPacket);
}
