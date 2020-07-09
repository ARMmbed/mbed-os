/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager connected isochronous stream master role implementation file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
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

#include "lmgr_api_cis_master.h"
#include "lctr_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Master role device parameters. */
lmgrCisMstCtrlBlk_t lmgrCisMstCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager master mode resources.
 */
/*************************************************************************************************/
void LmgrMstCisInit(void)
{
  memset(&lmgrCisMstCb, 0, sizeof(lmgrCisMstCb));

  lmgrCisMstCb.maxNumCis = pLctrRtCfg->maxCis;

  /* lmgrCisMstCb.numCisPend = 0; */    /* Clear by default */
}

/*************************************************************************************************/
/*!
 *  \brief      Get the maximum number of CIS.
 *
 *  \return     Maximum number of CIS.
 */
/*************************************************************************************************/
uint8_t LmgrMstCisGetMaxNumCis(void)
{
  return lmgrCisMstCb.maxNumCis;
}
