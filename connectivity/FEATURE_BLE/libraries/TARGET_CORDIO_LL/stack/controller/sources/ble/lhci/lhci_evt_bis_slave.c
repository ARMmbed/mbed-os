/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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
#include "ll_api.h"
#include "util/bstream.h"

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Pack a BIS create BIG complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Generate BIS create BIG complete data.
 */
/*************************************************************************************************/
static void lhciPackCreateBigCnfEvt(uint8_t *pBuf, const LlCreateBigCnf_t *pEvt)
{
  UINT8_TO_BSTREAM (pBuf, HCI_LE_CREATE_BIG_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  UINT8_TO_BSTREAM (pBuf, pEvt->bigHandle);
  UINT24_TO_BSTREAM(pBuf, pEvt->syncDelayUsec);
  UINT24_TO_BSTREAM(pBuf, pEvt->transLatUsec);
  UINT8_TO_BSTREAM (pBuf, pEvt->phy);
  UINT8_TO_BSTREAM (pBuf, pEvt->nse);
  UINT8_TO_BSTREAM (pBuf, pEvt->bn);
  UINT8_TO_BSTREAM (pBuf, pEvt->pto);
  UINT8_TO_BSTREAM (pBuf, pEvt->irc);
  UINT16_TO_BSTREAM(pBuf, pEvt->maxPdu);
  UINT16_TO_BSTREAM(pBuf, pEvt->isoInterval);
  UINT8_TO_BSTREAM (pBuf, pEvt->numBis);
  for (unsigned int i = 0; i < pEvt->numBis; i++)
  {
    UINT16_TO_BSTREAM(pBuf, pEvt->bisHandle[i]);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a BIS terminate BIG complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Generate BIS create BIG complete data.
 */
/*************************************************************************************************/
static void lhciPackTerminateBigCnfEvt(uint8_t *pBuf, const LlTerminateBigInd_t *pEvt)
{
  UINT8_TO_BSTREAM(pBuf, HCI_LE_TERMINATE_BIG_CMPL_EVT);
  UINT8_TO_BSTREAM(pBuf, pEvt->bigHandle);
  UINT8_TO_BSTREAM(pBuf, pEvt->reason);
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  LL BIS slave event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvBigEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CREATE_BIG_CNF:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_CREATE_BIG_CMPL_EVT) << LHCI_BYTE_TO_BITS(3))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_CREATE_BIG_CMPL(pEvt->createBigCnf.numBis))) != NULL)
        {
          lhciPackCreateBigCnfEvt(pEvtBuf, &pEvt->createBigCnf);
        }
      }
      break;

    case LL_TERM_BIG_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_TERMINATE_BIG_CMPL_EVT) << LHCI_BYTE_TO_BITS(3))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_TERMINATE_BIG_CMPL)) != NULL)
        {
          lhciPackTerminateBigCnfEvt(pEvtBuf, &pEvt->termBigInd);
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
