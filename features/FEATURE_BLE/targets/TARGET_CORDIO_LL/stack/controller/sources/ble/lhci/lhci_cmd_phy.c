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
 *  \brief  Pack a read PHY event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Status code.
 *  \param  handle      Connection handle.
 *  \param  txPhy       Transmitter PHY.
 *  \param  rxPhy       Receiver PHY.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadPhyEvt(uint8_t *pBuf, uint8_t status, uint16_t handle, uint8_t txPhy, uint8_t rxPhy)
{
  const uint8_t len = LHCI_LEN_LE_READ_PHY_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT8_TO_BSTREAM (pBuf, txPhy);
  UINT8_TO_BSTREAM (pBuf, rxPhy);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  handle      Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPhySendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint16_t handle)
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

    case HCI_OPCODE_LE_SET_DEF_PHY:
    case HCI_OPCODE_LE_ENHANCED_RECEIVER_TEST:
    case HCI_OPCODE_LE_ENHANCED_TRANSMITTER_TEST:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

    case HCI_OPCODE_LE_READ_PHY:
    {
      uint8_t txPhy = 0;
      uint8_t rxPhy = 0;
      status = LlReadPhy(handle, &txPhy, &rxPhy);
      lhciPackReadPhyEvt(pBuf, status, handle, txPhy, rxPhy);
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
bool_t lhciPhyDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint16_t handle = 0;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_READ_PHY:
    {
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_READ_PHY_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_DEF_PHY:
    {
      status   = LlSetDefaultPhy(pBuf[0], pBuf[1], pBuf[2]);
      paramLen = LHCI_LEN_LE_SET_DEF_PHY_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_PHY:
    {
      uint8_t allPhys;
      uint8_t txPhys;
      uint8_t rxPhys;
      uint16_t phyOptions;
      BSTREAM_TO_UINT16(handle,     pBuf);
      BSTREAM_TO_UINT8 (allPhys,    pBuf);
      BSTREAM_TO_UINT8 (txPhys,     pBuf);
      BSTREAM_TO_UINT8 (rxPhys,     pBuf);
      BSTREAM_TO_UINT16(phyOptions, pBuf);
      status   = LlSetPhy(handle, allPhys, txPhys, rxPhys, phyOptions);
      paramLen = 0xFF;
      break;
    }
    case HCI_OPCODE_LE_ENHANCED_RECEIVER_TEST:
    {
      status   = LlEnhancedRxTest(pBuf[0], pBuf[1], pBuf[2], 0);
      paramLen = LHCI_LEN_LE_ENHANCED_RECEIVER_TEST_EVT;
      break;
    }
    case HCI_OPCODE_LE_ENHANCED_TRANSMITTER_TEST:
    {
      status   = LlEnhancedTxTest(pBuf[0], pBuf[1], pBuf[2], pBuf[3], 0);
      paramLen = LHCI_LEN_LE_ENHANCED_TRANSMITTER_TEST_EVT;
      break;
    }

    default:
      return FALSE;     /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }
  else
  {
    lhciPhySendCmdCmplEvt(pHdr, status, paramLen, handle);
  }

  return TRUE;
}
