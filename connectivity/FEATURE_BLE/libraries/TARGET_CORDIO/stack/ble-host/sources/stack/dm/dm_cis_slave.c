/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM Connected Isochronous Stream (CIS) management for slave.
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

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "dm_adv.h"
#include "dm_cis.h"

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

static void dmCisSmActRequest(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg);
static void dmCisSmActAccept(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg);
static void dmCisSmActReject(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Action set for this module */
static const dmCisAct_t dmCisActSetSlave[] =
{
  dmCisSmActRequest,
  dmCisSmActAccept,
  dmCisSmActReject
};

/*************************************************************************************************/
/*!
 *  \brief  Handle a CIS request event from HCI.
 *
 *  \param  pCcb      CIS connection control block.
 *  \param  pMsg      WSF message.
 *  \param  oldState  Old state.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmCisSmActRequest(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg)
{
  /* save CIS and ACL handles */
  pCcb->cisHandle = pMsg->hciLeCisReq.cisHandle;
  pCcb->aclHandle = pMsg->hciLeCisReq.aclHandle;

  pMsg->hdr.event = DM_CIS_REQ_IND;
  (*dmCb.cback)((dmEvt_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Accept a requested CIS connection.
 *
 *  \param  pCcb      CIS connection control block.
 *  \param  pMsg      WSF message.
 *  \param  oldState  Old state.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmCisSmActAccept(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg)
{
  /* if slave */
  if (pCcb->role == DM_ROLE_SLAVE)
  {
    /* accept CIS */
    HciLeAcceptCisReqCmd(pMsg->hdr.param);
  }
  else
  {
    /* deallocate ccb */
    dmCisCcbDealloc(pCcb);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Reject a requested CIS connection.
 *
 *  \param  pCcb      CIS connection control block.
 *  \param  pMsg      WSF message.
 *  \param  oldState  Old state.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmCisSmActReject(dmCisCcb_t *pCcb, dmCisMsg_t *pMsg)
{
  /* if slave */
  if (pCcb->role == DM_ROLE_SLAVE)
  {
    /* reject CIS */
    HciLeRejectCisReqCmd(pMsg->hdr.param, pMsg->apiReject.reason);
  }

  /* deallocate ccb */
  dmCisCcbDealloc(pCcb);
}

/*************************************************************************************************/
/*!
 *  \brief  Inform the Controller to accept the request for the Connected Isochronous Stream (CIS)
 *          that is identified by the connection handle.
 *
 *  \param  handle    Connection handle of the CIS.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmCisAccept(uint16_t handle)
{
  wsfMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(wsfMsgHdr_t))) != NULL)
  {
    pMsg->event = DM_CIS_MSG_API_ACCEPT;
    pMsg->param = handle;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Inform the Controller to reject the request for the Connected Isochronous Stream (CIS)
 *          that is identified by the connection handle.
 *
 *  \param  handle    Connection handle of the CIS to be rejected.
 *  \param  reason    Reason the CIS request was rejected.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmCisReject(uint16_t handle, uint8_t reason)
{
  dmCisApiReject_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmCisApiReject_t))) != NULL)
  {
    pMsg->hdr.event = DM_CIS_MSG_API_REJECT;
    pMsg->hdr.param = handle;
    pMsg->hdr.status = pMsg->reason = reason;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM Connected Isochronous Stream (CIS) manager for operation as slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmCisSlaveInit(void)
{
  WsfTaskLock();

  dmCisActSet[DM_CIS_ACT_SET_SLAVE] = (dmCisAct_t *) dmCisActSetSlave;

  HciSetLeSupFeat(HCI_LE_SUP_FEAT_CIS_SLAVE, TRUE);

  WsfTaskUnlock();
}
