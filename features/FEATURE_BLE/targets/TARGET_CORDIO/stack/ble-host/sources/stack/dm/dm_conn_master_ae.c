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
 *  \brief Device manager connection management module for extended master.
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
  dmExtConnSmActOpen,
  dmConnSmActCancelOpen,
  dmConnSmActUpdateMaster,
  dmConnSmActL2cUpdateInd
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
static void dmExtConnOpen(uint8_t initPhys, uint8_t addrType, uint8_t *pAddr)
{
  uint8_t i;
  uint8_t idx;
  uint8_t phyIdx;
  hciExtInitParam_t initParam;
  hciConnSpec_t connSpec[DM_NUM_PHYS];
  hciExtInitScanParam_t scanParam[DM_NUM_PHYS];

  /* set initiating parameters */
  initParam.filterPolicy = dmCb.initFiltPolicy;
  initParam.ownAddrType = DmLlAddrType(dmCb.connAddrType);
  initParam.peerAddrType = addrType;
  initParam.pPeerAddr = pAddr;
  initParam.initPhys = initPhys;

  /* see advertising packets to be received on which PHY */
  for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
  {
    if (initPhys & (1 << i))
    {
      phyIdx = DmInitPhyToIdx(1 << i);

      /* set extended create conection parameters for this PHY */
      scanParam[idx].scanInterval = dmConnCb.scanInterval[phyIdx];
      scanParam[idx].scanWindow = dmConnCb.scanWindow[phyIdx];
      connSpec[idx] = dmConnCb.connSpec[phyIdx];
      idx++;
    }
  }

  /* Create connection */
  HciLeExtCreateConnCmd(&initParam, scanParam, connSpec);

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
void dmExtConnSmActOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg)
{
  dmExtConnOpen(pMsg->apiOpen.initPhys, pMsg->apiOpen.addrType, pMsg->apiOpen.peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM connection manager for operation as extended master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtConnMasterInit(void)
{
  dmConnActSet[DM_CONN_ACT_SET_MASTER] = (dmConnAct_t *) dmConnActSetMaster;
}
