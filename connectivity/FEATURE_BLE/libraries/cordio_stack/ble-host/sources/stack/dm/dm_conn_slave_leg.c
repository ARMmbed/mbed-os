/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager connection management for legacy slave.
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
  Local Variables
**************************************************************************************************/

/* Action set for this module */
static const dmConnAct_t dmConnActSetSlave[] =
{
  dmConnSmActAccept,
  dmConnSmActCancelAccept,
  dmConnSmActConnAccepted,
  dmConnSmActAcceptFailed
};

/*************************************************************************************************/
/*!
 *  \brief  Accept a connection.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmAdvStartDirected(pMsg->apiOpen.advType, pMsg->apiOpen.duration, pMsg->apiOpen.addrType,
                     pMsg->apiOpen.peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Cancel a connection accept.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActCancelAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmAdvStopDirected();

  dmConnSmActConnFailed(pCcb, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Connection accepted.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActConnAccepted(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmAdvConnected();

  dmConnSmActConnOpened(pCcb, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Connection accept failed.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmConnSmActAcceptFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmAdvConnectFailed();

  dmConnSmActConnFailed(pCcb, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as legacy slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnSlaveInit(void)
{
  WsfTaskLock();

  dmConnActSet[DM_CONN_ACT_SET_SLAVE] = (dmConnAct_t *) dmConnActSetSlave;
  dmConnUpdActSet[DM_CONN_ACT_SET_SLAVE] = (dmConnAct_t *) dmConnUpdActSetSlave;

  WsfTaskUnlock();
}
