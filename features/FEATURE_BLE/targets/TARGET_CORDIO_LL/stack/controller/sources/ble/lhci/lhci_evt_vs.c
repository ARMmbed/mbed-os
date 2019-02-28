/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Vendor specific HCI event module implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Note: ARM Ltd. vendor specific subevent code is 0xFFF0-0xFFFF. */

#define LHCI_OPCODE_VS_SUBEVT_TRACE_MSG    0xFFF0         /*!< Trace message event. */
#define LHCI_OPCODE_VS_SUBEVT_SCAN_REPORT  0xFFF1         /*!< Scan report event. */

#define LHCI_LEN_VS_SUBEVT_SCAN_REPORT     13             /*!< Scan report event length. */

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
 *  \return None.
 */
/*************************************************************************************************/
void LhciVsEncodeTraceMsgEvtPkt(uint8_t *pBuf, uint8_t len)
{
  uint8_t *pPkt;
  uint8_t *pEvtBuf;

  if ((pPkt = lhciAllocEvt(HCI_VENDOR_SPEC_EVT, LHCI_LEN_VS_EVT + len)) != NULL)
  {
    pEvtBuf  = pPkt;
    pEvtBuf += lhciPackVsEvt(pEvtBuf, LHCI_OPCODE_VS_SUBEVT_TRACE_MSG);
    memcpy(pEvtBuf, pBuf, len);

    lhciSendEvt(pPkt);
  }
}

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
