/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Vendor specific HCI event module implementation file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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
#include "hci_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "bb_ble_api_op.h"
#include <string.h>
#include "bb_ble_sniffer_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Note: Arm Ltd. vendor specific subevent code is 0xFFF0-0xFFFF. */

#define LHCI_OPCODE_VS_SUBEVT_TRACE_MSG     0xFFF0         /*!< Trace message event. */
#define LHCI_OPCODE_VS_SUBEVT_SCAN_REPORT   0xFFF1         /*!< Scan report event. */
#define LHCI_OPCODE_VS_SUBEVT_PACKET_REPORT 0xFFF2         /*!< Packet report event from sniffer. */
#define LHCI_OPCODE_VS_SUBEVT_ISO_EVT_CMPL  0xFFF3         /*!< ISO Event complete event. */

#define LHCI_LEN_VS_SUBEVT_SCAN_REPORT     13             /*!< Scan report event length. */
#define LHCI_LEN_VS_SUBEVT_ISO_EVT_CMPL    8              /*!< ISO Event complete length. */

#if (BB_SNIFFER_ENABLED == TRUE)
/*************************************************************************************************/
/*!
 *  \brief  Send a packet report event.
 *
 *  \param  pBuf      Message.
 *
 *  \return TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t LhciVsEncodeSnifferPktEvtPkt(BbBleSnifferPkt_t *pPktData)
{
  uint8_t *pEvtBuf;
  uint8_t *pPkt = lhciAllocEvt(HCI_VENDOR_SPEC_EVT, LHCI_LEN_VS_EVT + BB_SNIFFER_MAX_PKT_SIZE);

  if (pPkt == NULL)
  {
    return FALSE;
  }

  pEvtBuf = pPkt;
  pEvtBuf += lhciPackVsEvt(pEvtBuf, LHCI_OPCODE_VS_SUBEVT_PACKET_REPORT);

  /* Pack metadata. */
  BbBleSnifferMeta_t meta = pPktData->pktType.meta;
  UINT8_TO_BSTREAM(pEvtBuf, meta.type);
  UINT8_TO_BSTREAM(pEvtBuf, meta.status);
  UINT8_TO_BSTREAM(pEvtBuf, meta.state);
  UINT32_TO_BSTREAM(pEvtBuf, meta.timeStamp);
  UINT8_TO_BSTREAM(pEvtBuf, meta.rssi);

  /* Pack channelization metadata. */
  PalBbBleChan_t chan = meta.chan;
  UINT8_TO_BSTREAM(pEvtBuf, chan.opType);
  UINT8_TO_BSTREAM(pEvtBuf, chan.chanIdx);
  UINT8_TO_BSTREAM(pEvtBuf, chan.txPower);
  UINT32_TO_BSTREAM(pEvtBuf, chan.accAddr);
  UINT24_TO_BSTREAM(pEvtBuf, chan.crcInit);
  UINT8_TO_BSTREAM(pEvtBuf, chan.txPhy);
  UINT8_TO_BSTREAM(pEvtBuf, chan.rxPhy);
  UINT8_TO_BSTREAM(pEvtBuf, chan.initTxPhyOptions);
  UINT8_TO_BSTREAM(pEvtBuf, chan.tifsTxPhyOptions);
  UINT8_TO_BSTREAM(pEvtBuf, chan.peerTxStableModIdx);
  UINT8_TO_BSTREAM(pEvtBuf, chan.peerRxStableModIdx);

  /* Pack data based on packet. */
  switch (chan.opType)
  {
    case BB_BLE_OP_MST_ADV_EVENT:
    case BB_BLE_OP_SLV_ADV_EVENT:
    case BB_BLE_OP_MST_AUX_ADV_EVENT:
    case BB_BLE_OP_SLV_AUX_ADV_EVENT:
    case BB_BLE_OP_MST_PER_SCAN_EVENT:
    case BB_BLE_OP_SLV_PER_ADV_EVENT:
      memcpy(pEvtBuf, pPktData->pktType.advPkt.hdr, LL_ADV_HDR_LEN);
      break;

    case BB_BLE_OP_MST_CONN_EVENT:
    case BB_BLE_OP_SLV_CONN_EVENT:
      memcpy(pEvtBuf, pPktData->pktType.dataPkt.hdr, LL_DATA_HDR_MAX_LEN);
      break;

    default:
      break;
  }

  lhciSendEvt(pPkt);
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a received packet.
 *
 *  \return TRUE if packets still remain. FALSE if not.
 */
/*************************************************************************************************/
bool_t LhciSnifferHandler(void)
{
  BbBleSnifferHciCtx_t * pHci = &bbSnifferCtx.outputCtx.hci;
  if ((bbSnifferCtx.enabled == FALSE) ||
      (pHci->bufIdx == 0))
  {
    return FALSE;
  }

  BbBleSnifferPkt_t * pPktData = &pHci->pktBuf[--pHci->bufIdx];
  LhciVsEncodeSnifferPktEvtPkt(pPktData);

  return pHci->bufIdx == 0;
}
#endif

/*************************************************************************************************/
/*!
 *  \brief  Pack a scan report packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Scan report data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciVsPackScanReportEvt(uint8_t *pBuf, const LlScanReportInd_t *pEvt)
{
  const uint8_t len = LHCI_LEN_VS_SUBEVT_SCAN_REPORT;

  UINT8_TO_BSTREAM(pBuf, pEvt->peerAddrType);
  BDA64_TO_BSTREAM(pBuf, pEvt->peerAddr);
  BDA64_TO_BSTREAM(pBuf, pEvt->peerRpa);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a vendor specific event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  vsEvtCode   Event code.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t LhciPackVsEvt(uint8_t *pBuf, uint16_t vsEvtCode)
{
  const uint8_t len = LHCI_LEN_VS_EVT;

  UINT16_TO_BSTREAM(pBuf, vsEvtCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Send a token pending event.
 *
 *  \param  pBuf      Message.
 *  \param  len       Message length.
 *
 *  \return TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LhciVsEncodeTraceMsgEvtPkt(const uint8_t *pBuf, uint32_t len)
{
  uint8_t *pEvtBuf;
  uint8_t *pPkt = lhciAllocEvt(HCI_VENDOR_SPEC_EVT, LHCI_LEN_VS_EVT + len);

  if (pPkt == NULL)
  {
    return FALSE;
  }

  pEvtBuf  = pPkt;
  pEvtBuf += lhciPackVsEvt(pEvtBuf, LHCI_OPCODE_VS_SUBEVT_TRACE_MSG);
  memcpy(pEvtBuf, pBuf, len);

  lhciSendEvt(pPkt);

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  LL slave VS event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvVsStdEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_SCAN_REPORT_IND:
      /* No need to check the event mask; LL should not generate this event without the event masked. */
      if ((pEvtBuf = lhciAllocEvt(HCI_VENDOR_SPEC_EVT, LHCI_LEN_VS_EVT + LHCI_LEN_VS_SUBEVT_SCAN_REPORT)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += LhciPackVsEvt(pBuf, LHCI_OPCODE_VS_SUBEVT_SCAN_REPORT);
        lhciVsPackScanReportEvt(pBuf, &pEvt->scanReportInd);
      }
      break;

    case LL_ISO_EVT_CMPL_IND:
      /* No need to check the event mask; LL should not generate this event without the event masked. */
      if ((pEvtBuf = lhciAllocEvt(HCI_VENDOR_SPEC_EVT, LHCI_LEN_VS_EVT + LHCI_LEN_VS_SUBEVT_ISO_EVT_CMPL)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += LhciPackVsEvt(pBuf, LHCI_OPCODE_VS_SUBEVT_ISO_EVT_CMPL);
        UINT8_TO_BSTREAM(pBuf, pEvt->isoEvtCmplInd.handle);
        pBuf += 3;  /* padding */
        UINT32_TO_BSTREAM(pBuf, pEvt->isoEvtCmplInd.evtCtr);
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
