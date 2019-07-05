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
#include "lctr_api.h"

/*************************************************************************************************/
/*!
 *  \brief  Pack an advertising set terminated event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Advertising set terminated indication data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackAdvSetTermEvt(uint8_t *pBuf, const LlAdvSetTermInd_t *pEvt)
{
  UINT8_TO_BSTREAM (pBuf, HCI_LE_ADV_SET_TERM_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  UINT8_TO_BSTREAM (pBuf, pEvt->advHandle);
  UINT16_TO_BSTREAM(pBuf, pEvt->connHandle);
  UINT8_TO_BSTREAM (pBuf, pEvt->numCmplAdvEvt);
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a scan request received event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Scan request received indication data.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackScanReqRcvdEvt(uint8_t *pBuf, const LlScanReqRcvdInd_t *pEvt)
{
  UINT8_TO_BSTREAM(pBuf, HCI_LE_SCAN_REQ_RCVD_EVT);
  UINT8_TO_BSTREAM(pBuf, pEvt->handle);
  UINT8_TO_BSTREAM(pBuf, pEvt->scanAddrType);
  BDA_TO_BSTREAM  (pBuf, pEvt->scanAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  LL slave advertising extension data event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvExtAdvEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_SCAN_REQ_RCVD_IND:
    {
      if ((lhciCb.numScanReqRcvd < pLctrRtCfg->maxScanReqRcvdEvt) &&
          (lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_SCAN_REQ_RCVD_EVT) << LHCI_BYTE_TO_BITS(2))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_SCAN_REQ_RCVD)) != NULL)
        {
          uint8_t *pBuf = pEvtBuf;
          lhciPackScanReqRcvdEvt(pBuf, &pEvt->scanReqRcvdInd);
          lhciCb.numScanReqRcvd++;
        }
      }
      break;
    }

    case LL_EXT_ADV_ENABLE_CNF:
    {
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_SET_EXT_ADV_ENABLE)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_SET_EXT_ADV_ENABLE);
        lhciPackCmdCompleteEvtStatus(pBuf, pEvt->extAdvEnableCnf.status);
      }
      break;
    }

    case LL_ADV_SET_TERM_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_ADV_SET_TERM_EVT) << LHCI_BYTE_TO_BITS(2))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_ADV_SET_TERM)) != NULL)
        {
          lhciPackAdvSetTermEvt(pEvtBuf, &pEvt->advSetTermInd);
        }
      }
      break;
    }

    case LL_PER_ADV_ENABLE_CNF:
    {
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_SET_PER_ADV_ENABLE)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_SET_PER_ADV_ENABLE);
        lhciPackCmdCompleteEvtStatus(pBuf, pEvt->perAdvEnableCnf.status);
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
