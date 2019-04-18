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
#include "lctr_api.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

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
static void lhciMstConnSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
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

    case HCI_OPCODE_LE_SET_HOST_CHAN_CLASS:
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
 *      - connection control
 *      - default
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciMstConnVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- connection control --- */

    case HCI_OPCODE_LE_CREATE_CONN:
    {
      LlInitParam_t initParam;
      LlConnSpec_t connSpec;

      BSTREAM_TO_UINT16(initParam.scanInterval, pBuf);
      BSTREAM_TO_UINT16(initParam.scanWindow, pBuf);
      BSTREAM_TO_UINT8 (initParam.filterPolicy, pBuf);
      BSTREAM_TO_UINT8 (initParam.peerAddrType, pBuf);
      initParam.pPeerAddr = pBuf;
      pBuf += sizeof(bdAddr_t);
      BSTREAM_TO_UINT8 (initParam.ownAddrType, pBuf);
      /* pBuf += */ lhciUnpackConnSpec(&connSpec, pBuf);

      status = LlCreateConn(&initParam, &connSpec);

      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_CREATE_CONN_CANCEL:
      LlCreateConnCancel();
      /* Command completed by SM. */
      break;
    case HCI_OPCODE_LE_SET_HOST_CHAN_CLASS:
    {
      status = LlSetChannelClass(pBuf);
      paramLen = LHCI_LEN_LE_SET_HOST_CHAN_CLASS;
      break;
    }

    /* --- default --- */

    default:
      /* Fall back to slave connection handler. */
      return lhciConnDecodeCmdPkt(pHdr, pBuf);    /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }
  else if (paramLen > 0)
  {
    lhciMstConnSendCmdCmplEvt(pHdr, status, paramLen);
  }

  return TRUE;
}
