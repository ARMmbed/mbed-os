/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI command module implementation file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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
#include "util/bstream.h"
#include <string.h>

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
bool_t lhciSlvBisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_CREATE_BIG:
    {
      LlCreateBig_t param;

      BSTREAM_TO_UINT8 (param.bigHandle, pBuf);
      BSTREAM_TO_UINT8 (param.advHandle, pBuf);
      BSTREAM_TO_UINT8 (param.numBis, pBuf);
      BSTREAM_TO_UINT24(param.sduInterUsec, pBuf);
      BSTREAM_TO_UINT16(param.maxSdu, pBuf);
      BSTREAM_TO_UINT16(param.mtlMs, pBuf);
      BSTREAM_TO_UINT8 (param.rtn, pBuf);
      BSTREAM_TO_UINT8 (param.phys, pBuf);
      BSTREAM_TO_UINT8 (param.packing, pBuf);
      BSTREAM_TO_UINT8 (param.framing, pBuf);
      BSTREAM_TO_UINT8 (param.encrypt, pBuf);
      memcpy(param.bcstCode, pBuf, sizeof(param.bcstCode));

      status = LlCreateBig(&param);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_CREATE_BIG_TEST:
    {
      LlCreateBigTest_t param;

      BSTREAM_TO_UINT8 (param.bigHandle, pBuf);
      BSTREAM_TO_UINT8 (param.advHandle, pBuf);
      BSTREAM_TO_UINT8 (param.numBis, pBuf);
      BSTREAM_TO_UINT24(param.sduInterUsec, pBuf);
      BSTREAM_TO_UINT16(param.isoInter, pBuf);
      BSTREAM_TO_UINT8 (param.nse, pBuf);
      BSTREAM_TO_UINT16(param.maxSdu, pBuf);
      BSTREAM_TO_UINT16(param.maxPdu, pBuf);
      BSTREAM_TO_UINT8 (param.phys, pBuf);
      BSTREAM_TO_UINT8 (param.packing, pBuf);
      BSTREAM_TO_UINT8 (param.framing, pBuf);
      BSTREAM_TO_UINT8 (param.bn, pBuf);
      BSTREAM_TO_UINT8 (param.irc, pBuf);
      BSTREAM_TO_UINT8 (param.pto, pBuf);
      BSTREAM_TO_UINT8 (param.encrypt, pBuf);
      memcpy(param.bcstCode, pBuf, sizeof(param.bcstCode));

      status = LlCreateBigTest(&param);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_TERMINATE_BIG:
    {
      uint8_t bigHandle;
      uint8_t reason;

      BSTREAM_TO_UINT8(bigHandle, pBuf);
      BSTREAM_TO_UINT8(reason, pBuf);

      status = LlTerminateBig(bigHandle, reason);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    default:
      return FALSE;     /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }

  return TRUE;
}
