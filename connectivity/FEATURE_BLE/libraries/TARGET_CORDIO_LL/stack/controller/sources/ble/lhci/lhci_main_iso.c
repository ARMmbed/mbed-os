/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
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

#include "lhci_int.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "bb_api.h"
#include "pal_bb.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      LL HCI message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 */
/*************************************************************************************************/
void LhciIsoHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (event & LHCI_EVT_ISO_RCVD)
  {
    uint8_t *pIsoBuf;
    wsfHandlerId_t handlerId;

    while ((pIsoBuf = WsfMsgDeq(&lhciPersistCb.isoQ, &handlerId)) != NULL)
    {
      LlSendIsoData(pIsoBuf);
    }
  }

  LhciHandler(event, pMsg);
}
