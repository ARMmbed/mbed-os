/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager slave role implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#include "lmgr_api_adv_slave.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Slave role device parameters. */
lmgrSlvAdvCtrlBlk_t lmgrSlvAdvCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager slave mode resources.
 */
/*************************************************************************************************/
void LmgrSlvInit(void)
{
  static const lmgrAdvParam_t defAdvParam =
  {
    .advInterMinUsec    = BB_BLE_TO_US(0x0800),
    .advInterMaxUsec    = BB_BLE_TO_US(0x0800),
    .advType        = LL_ADV_CONN_UNDIRECT,
    .ownAddrType    = LL_ADDR_PUBLIC,
    .peerAddrType   = LL_ADDR_PUBLIC,
    .advChanMap     = LL_ADV_CHAN_ALL,
    .advFiltPolicy  = LL_ADV_FILTER_NONE
  };

  memset(&lmgrSlvAdvCb, 0, sizeof(lmgrSlvAdvCb));

  /* Set default Tx PHY options */
  lmgrSlvAdvCb.defTxPhyOpts = BB_PHY_OPTIONS_BLE_S8;

  lmgrSlvAdvCb.advParam = defAdvParam;
}
