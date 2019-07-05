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
#include "ll_api.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a read local P-256 public key complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Read local P-256 public key indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadLocalP256PubKeyCmplEvt(uint8_t *pBuf, const LlReadLocalP256PubKeyInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_READ_PUB_KEY_CMPL;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, LL_SUCCESS);
  memcpy(pBuf, pEvt->pubKey_x, sizeof(pEvt->pubKey_x));
  pBuf += sizeof(pEvt->pubKey_x);
  memcpy(pBuf, pEvt->pubKey_y, sizeof(pEvt->pubKey_y));

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a generate Diffie-Hellman key complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Generate Diffie-Hellman key indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackGenerateDhKeyCmplEvt(uint8_t *pBuf, const LlGenerateDhKeyInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_GEN_DHKEY_CMPL;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_GENERATE_DHKEY_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  memcpy(pBuf, pEvt->dhKey, sizeof(pEvt->dhKey));

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL secure connections data event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciScEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_READ_LOCAL_P256_PUB_KEY_CMPL_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_READ_LOCAL_P256_PUB_KEY_CMPL) << LHCI_BYTE_TO_BITS(0))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_READ_PUB_KEY_CMPL)) != NULL)
        {
          lhciPackReadLocalP256PubKeyCmplEvt(pEvtBuf, &pEvt->readLocalP256PubKeyInd);
        }
      }
      break;
    case LL_GENERATE_DHKEY_CMPL_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_GENERATE_DHKEY_CMPL) << LHCI_BYTE_TO_BITS(1))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_GEN_DHKEY_CMPL)) != NULL)
        {
          lhciPackGenerateDhKeyCmplEvt(pEvtBuf, &pEvt->generateDhKeyInd);
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
