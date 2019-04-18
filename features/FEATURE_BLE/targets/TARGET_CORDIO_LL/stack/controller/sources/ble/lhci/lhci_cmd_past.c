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
#include "wsf_math.h"
#include "util/bstream.h"
#include <string.h>

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
bool_t lhciPastDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint16_t connHandle = 0;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_SET_PER_ADV_RCV_ENABLE:
    {
      uint16_t syncHandle;
      uint8_t enable;

      BSTREAM_TO_UINT16(syncHandle, pBuf);
      BSTREAM_TO_UINT8 (enable,    pBuf);
      status   = LlSetPeriodicAdvRcvEnable(syncHandle, enable);
      paramLen = LHCI_LEN_LE_SET_PER_ADV_RCV_ENABLE;
      break;
    }
    case HCI_OPCODE_LE_PER_ADV_SYNC_TRANSFER:
    {
      uint16_t syncHandle;
      uint16_t serviceData;

      BSTREAM_TO_UINT16(connHandle, pBuf);
      BSTREAM_TO_UINT16(serviceData, pBuf);
      BSTREAM_TO_UINT16(syncHandle, pBuf);
      status = LlPeriodicAdvSyncTransfer(connHandle, serviceData, syncHandle);
      paramLen = LHCI_LEN_LE_PER_ADV_SYNC_TRANSFER;
      break;
    }
    case HCI_OPCODE_LE_PER_ADV_SET_INFO_TRANSFER:
    {
      uint16_t advHandle;
      uint16_t serviceData;

      BSTREAM_TO_UINT16(connHandle, pBuf);
      BSTREAM_TO_UINT16(serviceData, pBuf);
      BSTREAM_TO_UINT8(advHandle, pBuf);
      status = LlPeriodicAdvSetInfoTransfer(connHandle, serviceData, advHandle);
      paramLen = LHCI_LEN_LE_PER_ADV_SET_INFO_TRANSFER;
      break;
    }
    case HCI_OPCODE_LE_SET_PAST_PARAM:
    {
      uint8_t mode;
      uint16_t skip;
      uint16_t syncTimeout;
      uint8_t cteType;

      BSTREAM_TO_UINT16(connHandle, pBuf);
      BSTREAM_TO_UINT8(mode, pBuf);
      BSTREAM_TO_UINT16(skip, pBuf);
      BSTREAM_TO_UINT16(syncTimeout, pBuf);
      BSTREAM_TO_UINT8(cteType, pBuf);
      status = LlSetPeriodicAdvSyncTransParams(connHandle, mode, skip, syncTimeout, cteType);
      paramLen = LHCI_LEN_LE_SET_PAST_PARAM;
      break;
    }
    case HCI_OPCODE_LE_SET_DEFAULT_PAST_PARAM:
    {
      uint8_t mode;
      uint16_t skip;
      uint16_t syncTimeout;
      uint8_t cteType;

      BSTREAM_TO_UINT8(mode, pBuf);
      BSTREAM_TO_UINT16(skip, pBuf);
      BSTREAM_TO_UINT16(syncTimeout, pBuf);
      BSTREAM_TO_UINT8(cteType, pBuf);
      status = LlSetDefaultPeriodicAdvSyncTransParams(mode, skip, syncTimeout, cteType);
      paramLen = LHCI_LEN_LE_SET_DEFAULT_PAST_PARAM;
      break;
    }

    default:
      return FALSE;     /* exit dispatcher routine */
  }

  lhciConnSendCmdCmplEvt(pHdr, status, paramLen, NULL, connHandle);

  return TRUE;
}
