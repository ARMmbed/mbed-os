/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI command module implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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
#include "ll_api.h"
#include "ll_defs.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  handle      Handle.
 */
/*************************************************************************************************/
static void lhciCisSlvSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocCmdCmplEvt(paramLen, pCmdHdr->opCode)) == NULL)
  {
    return;
  }
  pBuf = pEvtBuf;

  switch (pCmdHdr->opCode)
  {
    /* --- command completion with status only parameter --- */
    case HCI_OPCODE_LE_ACCEPT_CIS_REQ:
    {
      break;
    }
    case HCI_OPCODE_LE_REJECT_CIS_REQ:
    {
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;
    }
    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvCisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint16_t handle = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_ACCEPT_CIS_REQ:
    {
      uint16_t cisHandle;

      BSTREAM_TO_UINT16(cisHandle, pBuf);
      status = LlAcceptCisReq(cisHandle);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    case HCI_OPCODE_LE_REJECT_CIS_REQ:
    {
      uint16_t cisHandle;
      uint8_t reason;

      BSTREAM_TO_UINT16(cisHandle, pBuf);
      BSTREAM_TO_UINT8(reason, pBuf);
      status = LlRejectCisReq(cisHandle, reason);
      paramLen = LHCI_LEN_LE_REJECT_CIS_REQ;
      break;
    }
    default:
      return FALSE;     /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }
  else if (paramLen > 0)
  {
    lhciCisSlvSendCmdCmplEvt(pHdr, status, paramLen, handle);
  }

  return TRUE;
}
