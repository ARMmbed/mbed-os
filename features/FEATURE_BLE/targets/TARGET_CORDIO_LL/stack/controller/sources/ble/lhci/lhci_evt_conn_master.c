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
#include "wsf_msg.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief  LL master connection event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstConnEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_CREATE_CONN_CANCEL_CNF:
      if ((pEvtBuf = lhciAllocEvt(HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + LHCI_LEN_LE_CREATE_CONN_CANCEL_EVT)) != NULL)
      {
        uint8_t *pBuf = pEvtBuf;
        pBuf += lhciPackCmdCompleteEvt(pBuf, HCI_OPCODE_LE_CREATE_CONN_CANCEL);
        lhciPackCmdCompleteEvtStatus(pBuf, pEvt->createConnCancelCnf.status);
      }
      break;

    default:
      /* Fall back to slave connection handler. */
      return lhciConnEncodeEvtPkt(pEvt);     /* exit routine */
  }

  if (pEvtBuf)
  {
    lhciSendEvt(pEvtBuf);
    return TRUE;
  }

  return FALSE;
}
