/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager connected isochronous stream implementation file.
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

#include "bb_ble_api.h"
#include "lctr_api.h"
#include <string.h>
#include "lmgr_api_iso.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Isochronous control block. */
lmgrIsoCtrlBlk_t lmgrIsoCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager connection resources.
 */
/*************************************************************************************************/
void LmgrIsoInit(void)
{
  memset(&lmgrIsoCb, 0, sizeof(lmgrIsoCb));

  lmgrIsoCb.availTxBuf = pLctrRtCfg->numIsoTxBuf;
  lmgrIsoCb.availRxBuf = pLctrRtCfg->numIsoRxBuf;

  lmgrIsoCb.allPhys = LL_ALL_PHY_TX_PREFERENCE_BIT | LL_ALL_PHY_RX_PREFERENCE_BIT;
  lmgrIsoCb.rxPhys = LL_PHYS_NONE;
  lmgrIsoCb.txPhys = LL_PHYS_NONE;
}
