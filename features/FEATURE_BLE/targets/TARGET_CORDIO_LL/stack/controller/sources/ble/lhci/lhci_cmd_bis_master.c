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
#include "ll_api.h"
#include "util/bstream.h"
#include "wsf_math.h"
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
bool_t lhciMstBisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_BIG_CREATE_SYNC:
    {
      LlBigCreateSync_t param;

      BSTREAM_TO_UINT8(param.bigHandle, pBuf);
      BSTREAM_TO_UINT16(param.syncHandle, pBuf);
      BSTREAM_TO_UINT8(param.encrypt, pBuf);
      memcpy(param.bcstCode, pBuf, sizeof(param.bcstCode));
      pBuf += 16;
      BSTREAM_TO_UINT8(param.mse, pBuf);
      BSTREAM_TO_UINT16(param.bigSyncTimeout, pBuf);
      BSTREAM_TO_UINT8(param.numBis, pBuf);
      memcpy(param.bis, pBuf, WSF_MIN(param.numBis, sizeof(param.bis)));

      status = LlBigCreateSync(&param);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_BIG_TERMINATE_SYNC:
    {
      uint8_t bigHandle;

      BSTREAM_TO_UINT8(bigHandle, pBuf);

      LlBigTerminateSync(bigHandle);
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
