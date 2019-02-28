/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
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

/*************************************************************************************************/
/*!
 *  \brief  LL slave advertising data event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvAdvEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

#if LHCI_ENABLE_VS
  if (lhciSlvVsStdEncodeEvtPkt(pEvt))
  {
    return TRUE;
  }
#endif

  switch (pEvt->hdr.event)
  {
    case LL_ADV_ENABLE_CNF:
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_SET_ADV_ENABLE_EVT)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_SET_ADV_ENABLE);
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
