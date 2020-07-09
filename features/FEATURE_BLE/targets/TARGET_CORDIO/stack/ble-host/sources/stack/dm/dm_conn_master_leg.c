/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager connection management module for legacy master.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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
#include "dm_api.h"
#include "dm_dev.h"
#include "dm_main.h"
#include "dm_conn.h"

/**************************************************************************************************
Local Variables
**************************************************************************************************/

/* Action set for this module */
static const dmConnAct_t dmConnActSetMaster[] =
{
  dmConnSmActOpen,
  dmConnSmActCancelOpen
};

/*************************************************************************************************/
/*!
 *  \brief  Open a connection to a peer device with the given address.
 *
 *  \param  initPhys  Initiating PHYs.
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return Connection identifier.
 */
/*************************************************************************************************/
static void dmConnOpen(uint8_t initPhys, uint8_t addrType, uint8_t *pAddr)
{
  uint8_t phyIdx = DmScanPhyToIdx(HCI_SCAN_PHY_LE_1M_BIT);

  /* Create connection */
  HciLeCreateConnCmd(dmConnCb.scanInterval[phyIdx], dmConnCb.scanWindow[phyIdx], dmCb.initFiltPolicy,
                     addrType, pAddr, DmLlAddrType(dmCb.connAddrType), &(dmConnCb.connSpec[phyIdx]));

  /* pass connection initiation started to dev priv */
  dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_CTRL, DM_DEV_PRIV_MSG_CONN_INIT_START, 0, 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Open a connection.
 *
 *  \param  pMsg    WSF message.
 *  \param  pCcb    Connection control block.
 *
 *  \return None.
*/
/*************************************************************************************************/
void dmConnSmActOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmConnOpen(pMsg->apiOpen.initPhys, pMsg->apiOpen.addrType, pMsg->apiOpen.peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as legacy master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmConnMasterInit(void)
{
  WsfTaskLock();

  dmConnActSet[DM_CONN_ACT_SET_MASTER] = (dmConnAct_t *) dmConnActSetMaster;
  dmConnUpdActSet[DM_CONN_ACT_SET_MASTER] = (dmConnAct_t *) dmConnUpdActSetMaster;

  WsfTaskUnlock();
}
