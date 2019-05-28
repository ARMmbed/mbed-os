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
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief  Used channel selection event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Data length change indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackUsedChSelEvt(uint8_t *pBuf, const LlChSelInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_CH_SEL_ALGO;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_CH_SEL_ALGO_EVT);
  UINT16_TO_BSTREAM(pBuf, pEvt->handle);
  UINT8_TO_BSTREAM (pBuf, pEvt->usedChSel);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL channel selection event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciConnCsEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CH_SEL_ALGO_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_CH_SEL_ALGO_EVT) << LHCI_BYTE_TO_BITS(2))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_CH_SEL_ALGO)) != NULL)
        {
          lhciPackUsedChSelEvt(pEvtBuf, &pEvt->usedChSelInd);
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
