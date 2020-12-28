/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) encryptable master initialization implementation file.
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

#include "lctr_api_adv_master.h"
#include "lctr_api_conn.h"
#include "lmgr_api_adv_master.h"
#include "lmgr_api_conn.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a encryptable connectable master.
 *
 *  This function initializes the LL subsystem for use as an advertising and encryptable
 *  connectable master.
 */
/*************************************************************************************************/
void LlEncConnMasterInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrConnInit();
  LctrMstConnEncInit();
}
