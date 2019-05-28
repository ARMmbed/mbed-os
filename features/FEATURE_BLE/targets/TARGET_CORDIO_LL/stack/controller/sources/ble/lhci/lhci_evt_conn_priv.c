/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief LL HCI event module implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief  Pack an enhanced connection complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Connection indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackEnhancedConnCompleteEvt(uint8_t *pBuf, const LlConnInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_ENHANCED_CONN_CMPL;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_ENHANCED_CONN_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  UINT16_TO_BSTREAM(pBuf, pEvt->handle);
  UINT8_TO_BSTREAM (pBuf, pEvt->role);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->peerAddr);
  BDA_TO_BSTREAM   (pBuf, pEvt->localRpa);
  BDA_TO_BSTREAM   (pBuf, pEvt->peerRpa);
  UINT16_TO_BSTREAM(pBuf, pEvt->connInterval);
  UINT16_TO_BSTREAM(pBuf, pEvt->connLatency);
  UINT16_TO_BSTREAM(pBuf, pEvt->supTimeout);
  UINT8_TO_BSTREAM (pBuf, pEvt->clockAccuracy);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL privacy connection event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciPrivConnEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CONN_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_ENHANCED_CONN_CMPL_EVT) << LHCI_BYTE_TO_BITS(1))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_ENHANCED_CONN_CMPL)) != NULL)
        {
          uint8_t *pBuf = pEvtBuf;
          lhciPackEnhancedConnCompleteEvt(pBuf, &pEvt->connInd);
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
