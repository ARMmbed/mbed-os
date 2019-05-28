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
 * \brief HCI command module implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a read resolving list size event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Status code.
 *  \param  size        Resolving list size.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadResListSizeEvt(uint8_t *pBuf, uint8_t status, uint16_t size)
{
  const uint8_t len = LHCI_LEN_LE_READ_RES_LIST_SIZE_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, size);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPrivAdvSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
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

    case HCI_OPCODE_LE_ADD_DEV_RES_LIST:
    case HCI_OPCODE_LE_REMOVE_DEV_RES_LIST:
    case HCI_OPCODE_LE_CLEAR_RES_LIST:
    case HCI_OPCODE_LE_SET_ADDR_RES_ENABLE:
    case HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO:
    case HCI_OPCODE_LE_SET_PRIVACY_MODE:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

    case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
    {
      uint8_t size = 0;
      status = LlReadResolvingListSize(&size);
      lhciPackReadResListSizeEvt(pBuf, status, size);
      break;
    }

    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

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
bool_t lhciPrivAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_ADD_DEV_RES_LIST:
    {
      status = LlAddDeviceToResolvingList(pBuf[0], pBuf + 1, pBuf + 7, pBuf + 23);
      paramLen = LHCI_LEN_LE_ADD_DEV_RES_LIST_EVT;
      break;
    }
    case HCI_OPCODE_LE_REMOVE_DEV_RES_LIST:
    {
      status = LlRemoveDeviceFromResolvingList(pBuf[0], pBuf + 1);
      paramLen = LHCI_LEN_LE_REMOVE_DEV_RES_LIST_EVT;
      break;
    }
    case HCI_OPCODE_LE_CLEAR_RES_LIST:
    {
      status = LlClearResolvingList();
      paramLen = LHCI_LEN_LE_CLEAR_RES_LIST_EVT;
      break;
    }
    case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
    {
      paramLen = LHCI_LEN_LE_READ_RES_LIST_SIZE_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_ADDR_RES_ENABLE:
    {
      status = LlSetAddrResolutionEnable(pBuf[0]);
      paramLen = LHCI_LEN_LE_SET_ADDR_RES_ENABLE_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO:
    {
      uint16_t rpaTimeout;
      BSTREAM_TO_UINT16(rpaTimeout, pBuf);
      status = LlSetResolvablePrivateAddrTimeout(rpaTimeout);
      paramLen = LHCI_LEN_LE_SET_RES_PRIV_ADDR_TO_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_PRIVACY_MODE:
    {
      status = LlSetPrivacyMode(pBuf[0], pBuf + 1, pBuf[7]);
      paramLen = LHCI_LEN_LE_SET_PRIVACY_MODE;
      break;
    }

    default:
      return FALSE;     /* exit dispatcher routine */
  }

  lhciPrivAdvSendCmdCmplEvt(pHdr, status, paramLen);

  return TRUE;
}
