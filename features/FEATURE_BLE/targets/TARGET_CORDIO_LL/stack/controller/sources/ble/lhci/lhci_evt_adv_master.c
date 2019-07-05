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
#include "lctr_api.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a advertising report event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Advertising report indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackAdvRptEvt(uint8_t *pBuf, const LlAdvReportInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_ADV_RPT_MIN + pEvt->len;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_ADV_REPORT_EVT);
  UINT8_TO_BSTREAM (pBuf, 1);
  UINT8_TO_BSTREAM (pBuf, pEvt->eventType);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->addr);
  UINT8_TO_BSTREAM (pBuf, pEvt->len);
  memcpy(pBuf, pEvt->pData, pEvt->len);
  pBuf += pEvt->len;
  UINT8_TO_BSTREAM (pBuf, pEvt->rssi);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a direct advertising report event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Advertising report indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackDirectAdvRptEvt(uint8_t *pBuf, const LlAdvReportInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_DIRECT_ADV_REPORT;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_DIRECT_ADV_REPORT_EVT);
  UINT8_TO_BSTREAM (pBuf, 1);
  UINT8_TO_BSTREAM (pBuf, pEvt->eventType);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->addr);
  UINT8_TO_BSTREAM (pBuf, pEvt->directAddrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->directAddr);
  UINT8_TO_BSTREAM (pBuf, pEvt->rssi);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL master advertising event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstScanEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_ADV_REPORT_IND:
      if (pEvt->advReportInd.directAddrType != 0xFF)
      {
        bool_t evtSent = FALSE;

        if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_DIRECT_ADV_REPORT_EVT) << LHCI_BYTE_TO_BITS(1))) &&
            (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
        {
          if ((lhciCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
              ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_DIRECT_ADV_REPORT)) != NULL))
          {
            uint8_t *pBuf = pEvtBuf;
            lhciPackDirectAdvRptEvt(pBuf, &pEvt->advReportInd);
            evtSent = TRUE;
            lhciCb.numAdvReport++;
          }
        }
        if (!evtSent)
        {
          /* Handle event here even if event cannot be sent. */
          return TRUE;
        }
      }
      else
      {
        if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_ADV_REPORT_EVT) << LHCI_BYTE_TO_BITS(0))) &&
           (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
        {
          if ((lhciCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
              ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_ADV_RPT_MIN + pEvt->advReportInd.len)) != NULL))
          {
            uint8_t *pBuf = pEvtBuf;
            lhciPackAdvRptEvt(pBuf, &pEvt->advReportInd);
            lhciCb.numAdvReport++;
          }
        }
      }
      break;

    case LL_SCAN_ENABLE_CNF:
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_SET_SCAN_ENABLE_EVT)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_SET_SCAN_ENABLE);
        lhciPackCmdCompleteEvtStatus(pBuf, pEvt->hdr.status);
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
