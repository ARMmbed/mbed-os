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
 *  \brief  Pack a read resolvable address event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Status code.
 *  \param  pAddr       Resolvable address.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadResAddrEvt(uint8_t *pBuf, uint8_t status, const uint8_t *pAddr)
{
  const uint8_t len = LHCI_LEN_LE_READ_PEER_RES_ADDR_EVT;

  UINT8_TO_BSTREAM(pBuf, status);
  BDA_TO_BSTREAM  (pBuf, pAddr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  pParam      Pointer to parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPrivConnSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam)
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
    case HCI_OPCODE_LE_READ_PEER_RES_ADDR:
    case HCI_OPCODE_LE_READ_LOCAL_RES_ADDR:
      lhciPackReadResAddrEvt(pBuf, status, pParam);
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
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciPrivConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t *pParam = NULL;
  bdAddr_t rpa = {0};

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_READ_PEER_RES_ADDR:
    {
      status = LlReadPeerResolvableAddr(pBuf[0], pBuf + 1, rpa);
      paramLen = LHCI_LEN_LE_READ_PEER_RES_ADDR_EVT;
      pParam = (uint8_t *)rpa;
      break;
    }
    case HCI_OPCODE_LE_READ_LOCAL_RES_ADDR:
    {
      status = LlReadLocalResolvableAddr(pBuf[0], pBuf + 1, rpa);
      paramLen = LHCI_LEN_LE_READ_LOCAL_RES_ADDR_EVT;
      pParam = (uint8_t *)rpa;
      break;
    }

    default:
      return lhciPrivAdvDecodeCmdPkt(pHdr, pBuf);
  }

  lhciPrivConnSendCmdCmplEvt(pHdr, status, paramLen, pParam);

  return TRUE;
}
