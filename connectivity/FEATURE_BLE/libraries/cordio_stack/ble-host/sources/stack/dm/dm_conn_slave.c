/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager slave connection management for slave.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "dm_adv.h"
#include "l2c_api.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Action set for this module */
const dmConnAct_t dmConnUpdActSetSlave[] =
{
  dmConnUpdActUpdateSlave,
  dmConnUpdActL2cUpdateCnf
};

/*************************************************************************************************/
/*!
 *  \brief  Call application callback with the connection update complete event.
 *
 *  \param  pCcb    Connection control block.
 *  \param  status  Status.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmConnUpdateCback(dmConnCcb_t *pCcb, uint8_t status)
{
  hciLeConnUpdateCmplEvt_t  evt;

  /* call callback */
  evt.hdr.event = DM_CONN_UPDATE_IND;
  evt.hdr.param = pCcb->connId;
  evt.status = evt.hdr.status = status;
  evt.handle = pCcb->handle;
  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Update a connection as a slave.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnUpdActUpdateSlave(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  if ((pCcb->features & HCI_LE_SUP_FEAT_CONN_PARAM_REQ_PROC) &&
      (HciGetLeSupFeat() & HCI_LE_SUP_FEAT_CONN_PARAM_REQ_PROC))
  {
    HciLeConnUpdateCmd(pCcb->handle, &pMsg->apiUpdate.connSpec);
  }
  /* else if L2CAP connection update not already in progress */
  else if (!pCcb->updating)
  {
    pCcb->updating = TRUE;

    /* send request via L2CAP */
    L2cDmConnUpdateReq(pCcb->handle, &pMsg->apiUpdate.connSpec);
  }
  /* else L2CAP connection update pending */
  else
  {
    /* call callback */
    dmConnUpdateCback(pCcb, (uint8_t) HCI_ERR_CMD_DISALLOWED);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an L2CAP connection update confirm.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnUpdActL2cUpdateCnf(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* if connection update in progress */
  if (pCcb->updating)
  {
    pCcb->updating = FALSE;

    /* if reason indicates failure */
    if (pMsg->l2cUpdateCnf.result != L2C_CONN_PARAM_ACCEPTED)
    {
      /* call callback */
      dmConnUpdateCback(pCcb, (uint8_t) pMsg->l2cUpdateCnf.result);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function to send the result of an L2CAP
 *          connection update response to DM.
 *
 *  \param  handle  Connection handle.
 *  \param  result  Connection update result code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cConnUpdateCnf(uint16_t handle, uint16_t result)
{
  dmConnL2cUpdateCnf_t  updateCnf;
  dmConnCcb_t           *pCcb;

  if ((pCcb = dmConnCcbByHandle(handle)) != NULL)
  {
    updateCnf.hdr.event = DM_CONN_MSG_L2C_UPDATE_CNF;
    updateCnf.result = result;

    dmConnUpdExecute(pCcb, (dmConnMsg_t *) &updateCnf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function to send the result of an L2CAP
 *          Command Reject indication up to the application.
 *
 *  \param  handle  Connection handle.
 *  \param  result  Connection update result code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cCmdRejInd(uint16_t handle, uint16_t result)
{
  dmL2cCmdRejEvt_t  evt;

  /* call callback */
  evt.hdr.event = DM_L2C_CMD_REJ_IND;
  evt.hdr.status = HCI_SUCCESS;
  evt.reason = result;
  evt.handle = handle;
  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Accept a connection from the given peer device by initiating directed advertising.
 *
 *  \param  clientId     The client identifier.
 *  \param  advHandle    Advertising handle.
 *  \param  advType      Advertising type.
 *  \param  duration     Advertising duration (in ms).
 *  \param  maxEaEvents  Maximum number of extended advertising events.
 *  \param  addrType     Address type.
 *  \param  pAddr        Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnAccept(uint8_t clientId, uint8_t advHandle, uint8_t advType, uint16_t duration,
                        uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr)
{
  return dmConnOpenAccept(clientId, 0, advHandle, advType, duration, maxEaEvents, addrType, pAddr,
                          DM_ROLE_SLAVE);
}
