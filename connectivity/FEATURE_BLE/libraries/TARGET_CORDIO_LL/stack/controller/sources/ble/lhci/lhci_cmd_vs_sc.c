/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Arm Ltd. vendor specific HCI command module implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Decode Arm Ltd. master secure connections vendor specific HCI command packet.
 *
 *  \param  pHdr    Unpacked command header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing for vendor specific commands.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciScVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t evtParamLen = 1;      /* default is status field only */

  /* Decode and consume command packet. */
  switch (pHdr->opCode)
  {
    /* --- extended device commands --- */

    case LHCI_OPCODE_VS_SET_P256_PRIV_KEY:
    {
      status = LlSetP256PrivateKey(pBuf);
      break;
    }

    case LHCI_OPCODE_VS_VALIDATE_PUB_KEY_MODE:
    {
      status = LlSetValidatePublicKeyMode(pBuf[0]);
      break;
    }

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  uint8_t *pEvtBuf;

  /* Encode and send command complete event packet. */
  if ((pEvtBuf = lhciAllocCmdCmplEvt(evtParamLen, pHdr->opCode)) != NULL)
  {
    pBuf  = pEvtBuf;
    /* pBuf += */ lhciPackCmdCompleteEvtStatus(pBuf, status);

    switch (pHdr->opCode)
    {
      /* --- extended device commands --- */

      case LHCI_OPCODE_VS_SET_P256_PRIV_KEY:
        /* no action */
        break;

      /* --- default --- */

      default:
        break;
    }

    lhciSendCmdCmplEvt(pEvtBuf);
  }

  return TRUE;
}
