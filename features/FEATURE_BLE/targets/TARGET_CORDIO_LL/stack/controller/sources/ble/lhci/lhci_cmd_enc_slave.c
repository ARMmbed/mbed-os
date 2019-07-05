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
 *  \brief  Pack a advertising report event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Status code.
 *  \param  pData       Encrypted data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackEncryptEvt(uint8_t *pBuf, uint8_t status, uint8_t *pData)
{
  const uint8_t len = LHCI_LEN_LE_ENCRYPT_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  memcpy(pBuf, pData, HCI_ENCRYPT_DATA_LEN);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a common status and handle event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackCommonStatusHandleEvt(uint8_t *pBuf, uint8_t status, uint16_t handle)
{
  const uint8_t len = 3;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read authenticated payload TO event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *  \param  timeout     Authenticated payload timeout.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadAuthPayloadTimeoutEvt(uint8_t *pBuf, uint8_t status, uint16_t handle, uint16_t timeout)
{
  const uint8_t len = LHCI_LEN_READ_AUTH_PAYLOAD_TO_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT16_TO_BSTREAM(pBuf, timeout);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  pParam      Parameter buffer.
 *  \param  handle      Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciSlvEncSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam, uint16_t handle)
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
    /* --- Encryption --- */

    case HCI_OPCODE_LE_ENCRYPT:
      lhciPackEncryptEvt(pBuf, status, pParam);
      break;
    case HCI_OPCODE_LE_LTK_REQ_REPL:
    case HCI_OPCODE_LE_LTK_REQ_NEG_REPL:
    case HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO:
      lhciPackCommonStatusHandleEvt(pBuf, status, handle);
      break;
    case HCI_OPCODE_READ_AUTH_PAYLOAD_TO:
    {
      uint16_t timeout = 0;
      status = LlReadAuthPayloadTimeout(handle, &timeout);
      lhciPackReadAuthPayloadTimeoutEvt(pBuf, status, handle, timeout);
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
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - encryption
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvEncDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciSlvEncVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t *pParam = NULL;
  uint16_t handle = 0;

  switch (pHdr->opCode)
  {
    /* --- Encryption --- */

    case HCI_OPCODE_LE_ENCRYPT:
    {
      uint8_t *pKey = pBuf;
      pParam = pBuf + HCI_KEY_LEN;
      LlEncrypt(pKey, pParam);
      paramLen = LHCI_LEN_LE_ENCRYPT_EVT;
      break;
    }
    case HCI_OPCODE_LE_LTK_REQ_REPL:
      BSTREAM_TO_UINT16(handle, pBuf);
      status = LlLtkReqReply(handle, pBuf);
      paramLen = LHCI_LEN_LE_LTK_REQ_REPL_EVT;
      break;
    case HCI_OPCODE_LE_LTK_REQ_NEG_REPL:
      BSTREAM_TO_UINT16(handle, pBuf);
      status = LlLtkReqNegReply(handle);
      paramLen = LHCI_LEN_LE_LTK_REQ_NEG_REPL_EVT;
      break;
    case HCI_OPCODE_READ_AUTH_PAYLOAD_TO:
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_READ_AUTH_PAYLOAD_TO_EVT;
      break;
    case HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO:
    {
      uint16_t timeout;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT16(timeout, pBuf);
      status = LlWriteAuthPayloadTimeout(handle, timeout);
      paramLen = LHCI_LEN_WRITE_AUTH_PAYLOAD_TO_EVT;
      break;
    }

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  lhciSlvEncSendCmdCmplEvt(pHdr, status, paramLen, pParam, handle);

  return TRUE;
}
