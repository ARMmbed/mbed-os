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
#include "chci_api.h"
#include "ll_defs.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief  Pack a command status event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  opCode      OpCode.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t LhciPackCmdStatusEvt(uint8_t *pBuf, uint8_t status, uint16_t opCode)
{
  const uint8_t len = HCI_LEN_CMD_STATUS;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT8_TO_BSTREAM (pBuf, 1);       /* Num_HCI_Command_Packets is always 1 */
  UINT16_TO_BSTREAM(pBuf, opCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a command complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  opCode      OpCode.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t LhciPackCmdCompleteEvt(uint8_t *pBuf, uint16_t opCode)
{
  const uint8_t len = HCI_LEN_CMD_CMPL;

  UINT8_TO_BSTREAM (pBuf, 1);       /* Num_HCI_Command_Packets is always 1 */
  UINT16_TO_BSTREAM(pBuf, opCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a command complete status parameter.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t LhciPackCmdCompleteEvtStatus(uint8_t *pBuf, uint8_t status)
{
  const uint8_t len = sizeof(uint8_t);

  UINT8_TO_BSTREAM (pBuf, status);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event sent, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciLlEvtHandler(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;
  bool_t evtSent = TRUE;
  unsigned int msg = 0;

  WSF_ASSERT(lhciEvtTbl[LHCI_MSG_ADV]);    /* proper init guarantees this handler exists. */

  LL_TRACE_INFO1("Encoding LL event=%u", pEvt->hdr.event);

  /* Standard event handlers. */
  do
  {
    if (lhciEvtTbl[msg] && lhciEvtTbl[msg](pEvt))
    {
      break;
    }
  } while (++msg < LHCI_MSG_TOTAL);

  /* Unhandled events. */
  if (msg == LHCI_MSG_TOTAL)
  {
    switch (pEvt->hdr.event)
    {
      case LL_RESET_CNF:
        lhciReset();

        if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_RESET_EVT)) != NULL)
        {
          uint8_t *pBuf = pEvtBuf;
          pBuf += LhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_RESET);
          lhciPackCmdCompleteEvtStatus(pBuf, pEvt->hdr.status);
        }
        break;

      case LL_ERROR_IND:
        if ((pEvtBuf = lhciAllocEvt(HCI_HW_ERROR_EVT, HCI_LEN_HW_ERR)) != NULL)
        {
          UINT8_TO_BSTREAM(pEvtBuf, pEvt->hwErrorInd.code);
        }
        break;

      default:
        break;
    }

    if (pEvtBuf)
    {
      lhciSendEvt(pEvtBuf);
    }
    else
    {
      LL_TRACE_WARN1("Unhandled, allocation failed or masked LL event=%u", pEvt->hdr.event);
      evtSent = FALSE;
    }
  }

  return evtSent;
}

/*************************************************************************************************/
/*!
 *  \brief  Send an HCI event and service the event queue.
 *
 *  \param  pBuf    Buffer containing HCI event to send or NULL to service the queue.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LhciSendEvent(uint8_t *pBuf)
{
  if (pBuf != NULL)
  {
    WsfMsgEnq(&lhciPersistCb.evtQ, 0, pBuf);
  }

  if (!lhciPersistCb.evtTrPending)
  {
    ChciTrNeedsService(CHCI_TR_PROT_BLE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return event pending status.
 *
 *  \return TRUE if HCI event is pending, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LhciIsEventPending(void)
{
  return lhciPersistCb.evtTrPending;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate an event.
 *
 *  \param  evtCode     Event code.
 *  \param  paramLen    Parameter length.
 *
 *  \return Buffer for event.
 */
/*************************************************************************************************/
uint8_t *lhciAllocEvt(uint8_t evtCode, uint8_t paramLen)
{
  uint8_t *pEvtBuf;

  if ((pEvtBuf = WsfMsgAlloc(HCI_EVT_HDR_LEN + paramLen)) != NULL)
  {
    pEvtBuf += lhciPackEvtHdr(pEvtBuf, evtCode, paramLen);
  }

  return pEvtBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate a command complete event.
 *
 *  \param  paramLen    Parameter length.
 *  \param  opCode      OpCode command field.
 *
 *  \return Buffer for command complete event.
 */
/*************************************************************************************************/
uint8_t *lhciAllocCmdCmplEvt(uint8_t paramLen, uint16_t opCode)
{
  uint8_t *pEvtBuf;

  if ((pEvtBuf = WsfMsgAlloc(HCI_EVT_HDR_LEN + HCI_LEN_CMD_CMPL + paramLen)) != NULL)
  {
    pEvtBuf += lhciPackEvtHdr(pEvtBuf, HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + paramLen);
    pEvtBuf += lhciPackCmdCompleteEvt(pEvtBuf, opCode);
  }

  return pEvtBuf;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack an event packet header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  evtCode     Event code.
 *  \param  paramLen    Parameter length.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t LhciPackEvtHdr(uint8_t *pBuf, uint8_t evtCode, uint8_t paramLen)
{
  const uint8_t len = HCI_EVT_HDR_LEN;

  UINT8_TO_BSTREAM(pBuf, evtCode);
  UINT8_TO_BSTREAM(pBuf, paramLen);

  return len;
}
