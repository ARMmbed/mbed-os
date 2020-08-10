/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
 *
 *  Copyright (c) 2016-2019 Arm Ltd. All Rights Reserved.
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
#include "chci_api.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Pack a CIS established event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        CIS established event data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackCisEstEvt(uint8_t *pBuf, const LlCisEstInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_CIS_EST;

  UINT8_TO_BSTREAM  (pBuf, HCI_LE_CIS_EST_EVT);
  UINT8_TO_BSTREAM  (pBuf, pEvt->status);
  UINT16_TO_BSTREAM (pBuf, pEvt->cisHandle);
  UINT24_TO_BSTREAM (pBuf, pEvt->cigSyncDelayUsec);
  UINT24_TO_BSTREAM (pBuf, pEvt->cisSyncDelayUsec);
  UINT24_TO_BSTREAM (pBuf, pEvt->transLatUsecMToS);
  UINT24_TO_BSTREAM (pBuf, pEvt->transLatUsecSToM);
  UINT8_TO_BSTREAM  (pBuf, pEvt->phyMToS);
  UINT8_TO_BSTREAM  (pBuf, pEvt->phySToM);
  UINT8_TO_BSTREAM  (pBuf, pEvt->nse);
  UINT8_TO_BSTREAM  (pBuf, pEvt->bnMToS);
  UINT8_TO_BSTREAM  (pBuf, pEvt->bnSToM);
  UINT8_TO_BSTREAM  (pBuf, pEvt->ftMToS);
  UINT8_TO_BSTREAM  (pBuf, pEvt->ftSToM);
  UINT16_TO_BSTREAM (pBuf, pEvt->maxPduMToS);
  UINT16_TO_BSTREAM (pBuf, pEvt->maxPduSToM);
  UINT16_TO_BSTREAM (pBuf, pEvt->isoInterval);

  return len;
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  LL CIS master event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstCisEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CIS_EST_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_CIS_EST_EVT) << LHCI_BYTE_TO_BITS(3))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {

        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_CIS_EST)) != NULL)
        {
          lhciPackCisEstEvt(pEvtBuf, &pEvt->cisEstInd);
        }
      }
      break;
    }

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
