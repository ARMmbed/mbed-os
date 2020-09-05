/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager connection management for master.
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
#include "wsf_msg.h"
#include "wsf_os.h"
#include "dm_api.h"
#include "dm_dev.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "l2c_api.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Action set for this module */
const dmConnAct_t dmConnUpdActSetMaster[] =
{
  dmConnUpdActUpdateMaster,
  dmConnUpdActL2cUpdateInd
};

/*************************************************************************************************/
/*!
 *  \brief  Cancel an opening connection.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActCancelOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* cancel create connection */
  HciLeCreateConnCancelCmd();

  /* pass connection initiation stopped to dev priv */
  dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_CTRL, DM_DEV_PRIV_MSG_CONN_INIT_STOP, 0, 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Update a connection as a master.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnUpdActUpdateMaster(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* send HCI command */
  HciLeConnUpdateCmd(pCcb->handle, &pMsg->apiUpdate.connSpec);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an L2CAP connection update indication.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnUpdActL2cUpdateInd(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  /* always send back response */
  L2cDmConnUpdateRsp(pMsg->l2cUpdateInd.identifier, pCcb->handle, L2C_CONN_PARAM_ACCEPTED);

  /* send HCI command */
  HciLeConnUpdateCmd(pCcb->handle, pMsg->l2cUpdateInd.pConnSpec);
}

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  L2C calls this function when it receives a connection update
 *          request from a peer device.
 *
 *  \param  identifier  Identifier value.
 *  \param  handle      Connection handle.
 *  \param  pConnSpec   Connection spec parameters.
 *  \return None.
 */
/*************************************************************************************************/
void DmL2cConnUpdateInd(uint8_t identifier, uint16_t handle, hciConnSpec_t *pConnSpec)
{
  dmConnL2cUpdateInd_t  updateInd;
  dmConnCcb_t           *pCcb;

  if ((pCcb = dmConnCcbByHandle(handle)) != NULL)
  {
    updateInd.hdr.event = DM_CONN_MSG_L2C_UPDATE_IND;
    updateInd.pConnSpec = pConnSpec;
    updateInd.identifier = identifier;

    dmConnUpdExecute(pCcb, (dmConnMsg_t *) &updateInd);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Open a connection to a peer device with the given address.
 *
 *  \param  clientId  The client identifier.
 *  \param  initPhys  Initiator PHYs.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
dmConnId_t DmConnOpen(uint8_t clientId, uint8_t initPhys, uint8_t addrType, uint8_t *pAddr)
{
  return dmConnOpenAccept(clientId, initPhys, 0, 0, 0, 0, addrType, pAddr, DM_ROLE_MASTER);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the local address type used for connections created with DmConnOpen().
 *
 *  \param  addrType  Address type.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSetAddrType(uint8_t addrType)
{
  WsfTaskLock();
  dmCb.connAddrType = addrType;
  WsfTaskUnlock();
}
