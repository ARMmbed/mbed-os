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
#include "wsf_msg.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief  Maximum size of Data_Length field in extended advertising report. */
#define LHCI_EXT_ADV_RPT_DATA_LEN_MAX       229

/*! \brief  Maximum size of Data_Length field in periodic advertising report. */
#define LHCI_PER_ADV_RPT_DATA_LEN_MAX       247

/*************************************************************************************************/
/*!
 *  \brief  Allocate an extended advertising report event buffer.
 *
 *  \param  evtCode     Event code.
 *  \param  paramLen    Parameter length.
 *
 *  \return Buffer for event.
 */
/*************************************************************************************************/
static uint8_t *lhciAllocExtAdvRptEvt(uint8_t evtCode, uint8_t paramLen)
{
  uint8_t *pEvtBuf;

  /* Use LHCI_EXT_ADV_RPT_DATA_LEN_MAX to ensure allocation from the extended advertising buffer pool. */
  if ((pEvtBuf = WsfMsgAlloc(HCI_EVT_HDR_LEN + LHCI_EXT_ADV_RPT_DATA_LEN_MAX)) != NULL)
  {
    pEvtBuf += lhciPackEvtHdr(pEvtBuf, evtCode, paramLen);
  }

  WSF_ASSERT(pEvtBuf);  /* lhciCb.numAdvReport guarantee allocation always succeeds. */

  return pEvtBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a periodic advertising report event buffer.
 *
 *  \param  evtCode     Event code.
 *  \param  paramLen    Parameter length.
 *
 *  \return Buffer for event.
 */
/*************************************************************************************************/
static uint8_t *lhciAllocPerAdvRptEvt(uint8_t evtCode, uint8_t paramLen)
{
  uint8_t *pEvtBuf;

  /* Use LHCI_PER_ADV_RPT_DATA_LEN_MAX to ensure allocation from the extended advertising buffer pool. */
  if ((pEvtBuf = WsfMsgAlloc(HCI_EVT_HDR_LEN + LHCI_PER_ADV_RPT_DATA_LEN_MAX)) != NULL)
  {
    pEvtBuf += lhciPackEvtHdr(pEvtBuf, evtCode, paramLen);
  }

  WSF_ASSERT(pEvtBuf);  /* lhciCb.numAdvReport guarantee allocation always succeeds. */

  return pEvtBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a extended advertising report event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Extended advertising report indication data.
 *  \param  pFragStart  Start of fragmentation buffer.
 *  \param  fragLen     Length of fragment.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackExtAdvRptEvt(uint8_t *pBuf, const LlExtAdvReportInd_t *pEvt,
                                 const uint8_t *pFragStart, uint8_t fragLen)
{
  uint16_t eventType;
  if (((pFragStart + fragLen) - pEvt->pData) == pEvt->len)
  {
    eventType = pEvt->eventType;
  }
  else
  {
    /* Incomplete, more data to come. */
    eventType = (pEvt->eventType & 0x001F) | (0x0001 << 5);
  }

  UINT8_TO_BSTREAM (pBuf, HCI_LE_EXT_ADV_REPORT_EVT);
  UINT8_TO_BSTREAM (pBuf, 1);  /* Num_Reports */
  UINT16_TO_BSTREAM(pBuf, eventType);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->addr);
  UINT8_TO_BSTREAM (pBuf, pEvt->priPhy);
  UINT8_TO_BSTREAM (pBuf, pEvt->secPhy);
  UINT8_TO_BSTREAM (pBuf, pEvt->advSID);
  UINT8_TO_BSTREAM (pBuf, pEvt->txPwr);
  UINT8_TO_BSTREAM (pBuf, pEvt->rssi);
  UINT16_TO_BSTREAM(pBuf, pEvt->perAdvInter);
  UINT8_TO_BSTREAM (pBuf, pEvt->directAddrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->directAddr);
  UINT8_TO_BSTREAM (pBuf, fragLen);
  memcpy(pBuf, pFragStart, fragLen);
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a periodic advertising report event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Periodic advertising report indication data.
 *  \param  pFragStart  Start of fragmentation buffer.
 *  \param  fragLen     Length of fragment.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackPerAdvRptEvt(uint8_t *pBuf, const LlPerAdvReportInd_t *pEvt,
                                 const uint8_t *pFragStart, uint8_t fragLen)
{
  uint8_t dataStatus;

  if (((pFragStart + fragLen) - pEvt->pData) == pEvt->len)
  {
    if (pEvt->dataStatus == LL_RPT_DATA_INC_TRUNC)
    {
      dataStatus = LL_RPT_DATA_INC_TRUNC;
    }
    else
    {
      dataStatus = LL_RPT_DATA_CMPL;
    }
  }
  else
  {
    /* Incomplete, more data to come. */
    dataStatus = LL_RPT_DATA_INC_MORE;
  }

  UINT8_TO_BSTREAM (pBuf, HCI_LE_PER_ADV_REPORT_EVT);
  UINT16_TO_BSTREAM(pBuf, pEvt->syncHandle);
  UINT8_TO_BSTREAM (pBuf, pEvt->txPwr);
  UINT8_TO_BSTREAM (pBuf, pEvt->rssi);
  UINT8_TO_BSTREAM (pBuf, pEvt->unused);
  UINT8_TO_BSTREAM (pBuf, dataStatus);
  UINT8_TO_BSTREAM (pBuf, fragLen);
  if (fragLen)
  {
    memcpy(pBuf, pFragStart, fragLen);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a periodic advertising sync established event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Periodic advertising sync established event confirmation.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackPerAdvSyncEstdEvt(uint8_t *pBuf, const LlPerAdvSyncEstdCnf_t *pEvt)
{
  UINT8_TO_BSTREAM (pBuf, HCI_LE_PER_ADV_SYNC_EST_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  UINT16_TO_BSTREAM(pBuf, pEvt->syncHandle);
  UINT8_TO_BSTREAM (pBuf, pEvt->advSID);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->addr);
  UINT8_TO_BSTREAM (pBuf, pEvt->advPhy);
  UINT16_TO_BSTREAM(pBuf, pEvt->perAdvInterval);
  UINT8_TO_BSTREAM (pBuf, pEvt->advClkAccuracy);
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a periodic advertising sync lost event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Periodic advertising sync lost event confirmation.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackPerAdvSyncLostEvt(uint8_t *pBuf, const LlPerAdvSyncLostInd_t *pEvt)
{
  UINT8_TO_BSTREAM (pBuf, HCI_LE_PER_ADV_SYNC_LOST_EVT);
  UINT16_TO_BSTREAM(pBuf, pEvt->syncHandle);
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a periodic advertising sync transfer received event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Periodic advertising sync transfer received event indication.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPackPerSyncTrsfRcvdEvt(uint8_t *pBuf, const LlPerSyncTrsfRcvdInd_t *pEvt)
{
  UINT8_TO_BSTREAM (pBuf, HCI_LE_PER_SYNC_TRSF_RCVD_EVT);
  UINT8_TO_BSTREAM (pBuf, pEvt->status);
  UINT16_TO_BSTREAM(pBuf, pEvt->connHandle);
  UINT16_TO_BSTREAM(pBuf, pEvt->serviceData);
  UINT16_TO_BSTREAM(pBuf, pEvt->syncHandle);
  UINT8_TO_BSTREAM (pBuf, pEvt->advSID);
  UINT8_TO_BSTREAM (pBuf, pEvt->addrType);
  BDA_TO_BSTREAM   (pBuf, pEvt->addr);
  UINT8_TO_BSTREAM (pBuf, pEvt->advPhy);
  UINT16_TO_BSTREAM(pBuf, pEvt->perAdvInterval);
  UINT8_TO_BSTREAM (pBuf, pEvt->advClkAccuracy);
}

/*************************************************************************************************/
/*!
 *  \brief  LL master advertising extension event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstExtScanEncodeEvtPkt(LlEvt_t *pEvt)
{
  bool_t result = FALSE;
  uint8_t *pEvtBuf;

  switch (pEvt->hdr.event)
  {
    case LL_EXT_ADV_REPORT_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_EXT_ADV_REPORT_EVT) << LHCI_BYTE_TO_BITS(1))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        const uint8_t *pFragStart = pEvt->extAdvReportInd.pData;
        unsigned int remDataLen = pEvt->extAdvReportInd.len;
        uint8_t num_buffers = remDataLen/LHCI_EXT_ADV_RPT_DATA_LEN_MAX + (remDataLen % LHCI_EXT_ADV_RPT_DATA_LEN_MAX != 0);

        if ((lhciCb.numAdvReport + num_buffers) > pLctrRtCfg->maxAdvReports)
        {
          LL_TRACE_WARN0("LL_EXT_ADV_REPORT_IND, discarded, not enough advRport buffers available.");
          break;
        }

        do
        {
          unsigned int fragLen = WSF_MIN(remDataLen, LHCI_EXT_ADV_RPT_DATA_LEN_MAX);
          if ((lhciCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
              ((pEvtBuf = lhciAllocExtAdvRptEvt(HCI_LE_META_EVT, HCI_LEN_LE_EXT_ADV_REPORT_MIN + fragLen)) != NULL))
          {
            remDataLen -= fragLen;
            lhciCb.numAdvReport++;

            if ((lhciCb.numAdvReport == pLctrRtCfg->maxAdvReports) && (remDataLen > 0))
            {
              /* Tell the host the data is truncated because remaining fragments can't be sent. */
              pEvt->extAdvReportInd.eventType |= LL_RPT_DATA_INC_TRUNC << 5;
              result = FALSE;

              /* Drop remaining fragments. */
              remDataLen = 0;
              LL_TRACE_WARN0("LL_EXT_ADV_REPORT_IND, truncated, not enough advRport buffers.");
            }
            else
            {
              result = TRUE;
            }

            lhciPackExtAdvRptEvt(pEvtBuf, &pEvt->extAdvReportInd, pFragStart, fragLen);
            pFragStart += fragLen;
            lhciSendEvt(pEvtBuf);
          }
          else
          {
            /* Drop remaining fragments. */
            remDataLen = 0;
            result = FALSE;
          }
        } while (remDataLen > 0);
      }
      break;
    }

    case LL_EXT_SCAN_ENABLE_CNF:
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_SET_EXT_SCAN_ENABLE)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE);
        lhciPackCmdCompleteEvtStatus(pBuf, pEvt->hdr.status);
        lhciSendEvt(pEvtBuf);
        result = TRUE;
      }
      break;

    case LL_SCAN_TIMEOUT_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_SCAN_TIMEOUT_EVT) << LHCI_BYTE_TO_BITS(2))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_SCAN_TIMEOUT)) != NULL)
        {
          pEvtBuf[0] = HCI_LE_SCAN_TIMEOUT_EVT;
          lhciSendEvt(pEvtBuf);
          result = TRUE;
        }
      }
      break;
    }

    case LL_PER_ADV_SYNC_ESTD_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_PER_ADV_SYNC_EST_EVT) << LHCI_BYTE_TO_BITS(1))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_PER_ADV_SYNC_EST)) != NULL)
        {
          lhciPackPerAdvSyncEstdEvt(pEvtBuf, &pEvt->perAdvSyncEstdCnf);
          lhciSendEvt(pEvtBuf);
          result = TRUE;
        }
      }
      break;
    }
    case LL_PER_ADV_REPORT_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_PER_ADV_REPORT_EVT) << LHCI_BYTE_TO_BITS(1))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        const uint8_t *pFragStart = pEvt->perAdvReportInd.pData;
        unsigned int remDataLen = pEvt->perAdvReportInd.len;
        uint8_t num_buffers = remDataLen/LHCI_PER_ADV_RPT_DATA_LEN_MAX + (remDataLen % LHCI_PER_ADV_RPT_DATA_LEN_MAX != 0);

        if ((lhciCb.numAdvReport + num_buffers) > pLctrRtCfg->maxAdvReports)
        {
          LL_TRACE_WARN0("LL_PER_ADV_REPORT_IND, discarded, not enough advRport buffers available.");
          break;
        }

        do
        {
          unsigned int fragLen = WSF_MIN(remDataLen, (LHCI_PER_ADV_RPT_DATA_LEN_MAX));
          if ((lhciCb.numAdvReport < pLctrRtCfg->maxAdvReports) &&
              ((pEvtBuf = lhciAllocPerAdvRptEvt(HCI_LE_META_EVT, HCI_LEN_LE_PER_ADV_REPORT + fragLen)) != NULL))
          {
            remDataLen -= fragLen;
            lhciCb.numAdvReport++;
            if ((lhciCb.numAdvReport == pLctrRtCfg->maxAdvReports) && (remDataLen > 0))
            {
              /* Tell the host the data is truncated because remaining fragments can't be sent. */
              pEvt->perAdvReportInd.dataStatus = LL_RPT_DATA_INC_TRUNC;
              result = FALSE;

              /* Drop remaining fragments. */
              remDataLen = 0;
              LL_TRACE_WARN0("LL_PER_ADV_REPORT_IND, truncated, not enough advRport buffers.");
            }
            else
            {
              result = TRUE;
            }

            lhciPackPerAdvRptEvt(pEvtBuf, &pEvt->perAdvReportInd, pFragStart, fragLen);
            pFragStart += fragLen;
            lhciSendEvt(pEvtBuf);
          }
          else
          {
            /* Drop remaining fragments. */
            remDataLen = 0;
            result = FALSE;
          }
        } while (remDataLen > 0);
      }
      break;
    }
    case LL_PER_ADV_SYNC_LOST_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_PER_ADV_SYNC_LOST_EVT) << LHCI_BYTE_TO_BITS(1))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_PER_ADV_SYNC_LOST)) != NULL)
        {
          lhciPackPerAdvSyncLostEvt(pEvtBuf, &pEvt->perAdvSyncLostInd);
          lhciSendEvt(pEvtBuf);
          result = TRUE;
        }
      }
      break;
    }
    case LL_PER_SYNC_TRSF_RCVD_IND:
    {
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_PER_SYNC_TRSF_RCVT_EVT) << LHCI_BYTE_TO_BITS(2))) &&
          (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_PER_SYNC_TRSF_RCVT)) != NULL)
        {
          lhciPackPerSyncTrsfRcvdEvt(pEvtBuf, &pEvt->perASyncTrsfRcvdInd);
          lhciSendEvt(pEvtBuf);
          result = TRUE;
        }
      }
      break;
    }

    default:
      break;
  }

  return result;
}
