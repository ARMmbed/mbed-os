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
 *  \brief Device manager connection management for extended slave.
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
  dmExtConnSmActAccept,
  dmExtConnSmActCancelAccept,
  dmConnSmActUpdateSlave,
  dmExtConnSmActConnAccepted,
  dmExtConnSmActAcceptFailed,
  dmConnSmActL2cUpdateCnf
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
void dmExtConnSmActAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmExtAdvStartDirected(pCcb->connId, pMsg->apiOpen.advHandle, pMsg->apiOpen.advType,
                        pMsg->apiOpen.duration, pMsg->apiOpen.maxEaEvents, pMsg->apiOpen.addrType,
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
void dmExtConnSmActCancelAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmExtAdvStopDirected(pCcb->connId);

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
void dmExtConnSmActConnAccepted(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmExtAdvConnected(pCcb->connId);

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
void dmExtConnSmActAcceptFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmExtAdvConnectFailed(pCcb->connId);

  dmConnSmActConnFailed(pCcb, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as extended slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnSlaveInit(void)
{
  dmConnActSet[DM_CONN_ACT_SET_SLAVE] = (dmConnAct_t *) dmConnActSetSlave;
}
