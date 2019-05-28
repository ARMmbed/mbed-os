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
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - LE secure connections
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciScDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciScVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- LE Secure Connections --- */

    case HCI_OPCODE_LE_READ_LOCAL_P256_PUB_KEY:
    {
      status = LlGenerateP256KeyPair();
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    case HCI_OPCODE_LE_GENERATE_DHKEY:
    {
      status = LlGenerateDhKey(pBuf, pBuf + LL_ECC_KEY_LEN);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    case HCI_OPCODE_LE_GENERATE_DHKEY_V2:
    {
      uint8_t keyType;
      uint8_t * pKeyType = pBuf + (2 * LL_ECC_KEY_LEN);
      BSTREAM_TO_UINT8(keyType, pKeyType);

      status = LlGenerateDhKeyV2(pBuf, pBuf + LL_ECC_KEY_LEN, keyType);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    /* --- default --- */

    default:
      return FALSE;
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }

  return TRUE;
}
