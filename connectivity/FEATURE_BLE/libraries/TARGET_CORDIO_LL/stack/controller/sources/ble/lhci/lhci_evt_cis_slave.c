/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
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

#include "lhci_int.h"
#include "lctr_api_cis_slave.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "util/bstream.h"

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Pack a CIS request event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Generate CIS request data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackCisReqEvt(uint8_t *pBuf, const LlCisReqInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_CIS_REQ;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_CIS_REQ_EVT);
  UINT16_TO_BSTREAM (pBuf, pEvt->aclHandle);
  UINT16_TO_BSTREAM(pBuf, pEvt->cisHandle);
  UINT8_TO_BSTREAM (pBuf, pEvt->cigId);
  UINT8_TO_BSTREAM (pBuf, pEvt->cisId);

  return len;
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  LL CIS slave event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvCisEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CIS_REQ_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_CIS_REQ_EVT) << LHCI_BYTE_TO_BITS(3))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_CIS_REQ)) != NULL)
        {
          lhciPackCisReqEvt(pEvtBuf, &pEvt->cisReqInd);
        }
      }
      break;

    default:
      break;
  }

  if (pEvtBuf)
  {
    lhciSendEvt(pEvtBuf);
    return TRUE;
  }

  return FALSE;
}
