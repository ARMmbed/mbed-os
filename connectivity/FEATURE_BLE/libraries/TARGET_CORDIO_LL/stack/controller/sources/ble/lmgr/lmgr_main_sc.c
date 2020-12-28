/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager secure connections implementation file.
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

#include "lmgr_api_sc.h"
#include "bb_ble_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Secure connection parameters. */
lmgrScCtrlBlk_t lmgrScCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager connection resources.
 */
/*************************************************************************************************/
void LmgrScInit(void)
{
  lmgrScCb.eccOpActive = FALSE;
  lmgrScCb.privKeySet = FALSE;
  lmgrScCb.pubKeyValid = FALSE;
}
