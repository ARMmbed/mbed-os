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
 *  \brief  Unpack a set scan parameter command packet.
 *
 *  \param  pCmd        Unpacked command structure.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Payload length.
 */
/*************************************************************************************************/
static uint8_t lhciUnpackSetScanParamCmd(LlScanParam_t *pCmd, const uint8_t *pBuf)
{
  const uint8_t len = 7;

  BSTREAM_TO_UINT8 (pCmd->scanType, pBuf);
  BSTREAM_TO_UINT16(pCmd->scanInterval, pBuf);
  BSTREAM_TO_UINT16(pCmd->scanWindow, pBuf);
  BSTREAM_TO_UINT8 (pCmd->ownAddrType, pBuf);
  BSTREAM_TO_UINT8 (pCmd->scanFiltPolicy, pBuf);

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
static void lhciMstScanSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
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

    case HCI_OPCODE_LE_SET_SCAN_PARAM:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

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
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - scan control
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstScanDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciMstScanVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- scan control --- */

    case HCI_OPCODE_LE_SET_SCAN_PARAM:
    {
      LlScanParam_t cmd;
      lhciUnpackSetScanParamCmd(&cmd, pBuf);
      status = LlSetScanParam(&cmd);
      paramLen = LHCI_LEN_LE_SET_SCAN_PARAM_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_SCAN_ENABLE:
      LlScanEnable(pBuf[0], pBuf[1]);
      return TRUE;      /* LL event handler sends command completion */

    /* --- default --- */

    default:
      return FALSE;     /* exit dispatcher routine */
  }

  lhciMstScanSendCmdCmplEvt(pHdr, status, paramLen);

  return TRUE;
}
